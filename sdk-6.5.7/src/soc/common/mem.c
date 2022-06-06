/*
 * $Id: mem.c,v 1.464 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * SOC Memory (Table) Utilities
 */

#include <shared/bsl.h>

#include <sal/core/libc.h>
#include <soc/mem.h>
#include <soc/cmic.h>
#include <soc/error.h>
#include <soc/register.h>
#include <soc/drv.h>
#include <soc/scache.h>
#include <soc/enet.h>
#include <soc/hash.h>
#include <soc/l2x.h>
#include <soc/l3x.h>
#include <soc/lpm.h>
#include <soc/vlan.h>
#include <shared/fabric.h>
#ifdef BCM_CMICM_SUPPORT
#include <soc/cmicm.h>
#endif
#ifdef BCM_SBUSDMA_SUPPORT
#include <soc/sbusdma.h>
#endif
#ifdef BCM_TRIUMPH3_SUPPORT
#include <soc/ism.h>
#include <soc/ism_hash.h>
#include <soc/triumph3.h>
#endif /* BCM_TRIUMPH3_SUPPORT */
#if defined(BCM_HERCULES_SUPPORT)
#include <soc/hercules.h>
#endif
#if defined(BCM_TRIUMPH_SUPPORT)
#include <soc/triumph.h>
#include <soc/er_tcam.h>
#endif /* BCM_TRIUMPH_SUPPORT */
#if defined(BCM_TRIUMPH2_SUPPORT)
#include <soc/triumph2.h>
#include <sal/core/dpc.h>
#endif /* BCM_TRIUMPH_SUPPORT */
#if defined(BCM_TRIDENT_SUPPORT)
#include <soc/trident.h>
#endif /* BCM_TRIDENT_SUPPORT */
#if defined(BCM_TRIDENT2_SUPPORT)
#include <soc/trident2.h>
#endif /* BCM_TRIDENT2_SUPPORT */
#if defined(BCM_KATANA_SUPPORT) && defined(BCM_CMICM_SUPPORT)
#include <soc/katana.h>
#endif /* BCM_KATANA_SUPPORT */
#if defined(BCM_HURRICANE2_SUPPORT)
#include <soc/hurricane2.h>
#endif /* BCM_HURRICANE2_SUPPORT */
#if defined(BCM_ENDURO_SUPPORT)
#include <soc/enduro.h>
#endif /* BCM_ENDURO_SUPPORT */
#if defined(BCM_APACHE_SUPPORT)
#include <soc/apache.h>
#endif /* BCM_APACHE_SUPPORT */
#if defined(BCM_TOMAHAWK_SUPPORT)
#include <soc/tomahawk.h>
#endif /* BCM_TOMAHAWK_SUPPORT */
#if defined(BCM_PETRA_SUPPORT)
#include <soc/dpp/drv.h>
#endif /* BCM_PETRA_SUPPORT */
#if defined(BCM_DFE_SUPPORT)
#include <soc/dfe/cmn/dfe_drv.h>
#endif /* BCM_DFE_SUPPORT */
#ifdef BCM_PETRA_SUPPORT
#include <soc/dcmn/dcmn_mem.h>
#include <soc/dpp/drv.h>
#endif
#if defined(BCM_GREYHOUND_SUPPORT)
#include <soc/greyhound.h>
#endif /* BCM_GREYHOUND_SUPPORT */
#if defined(BCM_KATANA2_SUPPORT)
#include <soc/katana2.h>
#endif /* BCM_KATANA2_SUPPORT */
#if defined(BCM_SABER2_SUPPORT)
#include <soc/saber2.h>
#endif /* BCM_SABER2_SUPPORT */
#if defined(BCM_HURRICANE3_SUPPORT)
#include <soc/hurricane3.h>
#endif /* BCM_HURRICANE3_SUPPORT */
#include <soc/drv.h>

#ifdef CRASH_RECOVERY_SUPPORT
#include <soc/hwstate/hw_log.h>
#include <soc/dcmn/dcmn_crash_recovery.h>
#endif /* CRASH_RECOVERY_SUPPORT */

#if defined(ALPM_ENABLE)
#include <soc/alpm.h>
#include <soc/esw/alpm_int.h>
#endif

#define LPM_INDEX_4K_ENTRIES 4096
#define LPM_INDEX_2K_ENTRIES 2048
#define LPM_INDEX_1K_ENTRIES 1024

#ifdef BCM_WARM_BOOT_SUPPORT
#define SOC_DEFIP_WARMBOOT_VERSION_1 SOC_SCACHE_VERSION(1,1)
#define SOC_DEFIP_WARMBOOT_DEFAULT_VERSION SOC_DEFIP_WARMBOOT_VERSION_1
#define SOC_OVERLAY_TCAM_WARMBOOT_VERSION_1 SOC_SCACHE_VERSION(1,1)
#define SOC_OVERLAY_TCAM_WARMBOOT_DEFAULT_VERSION \
        SOC_OVERLAY_TCAM_WARMBOOT_VERSION_1
#endif

#ifdef USE_VALGRIND_CLIENT_REQUESTS
#include "memcheck/memcheck.h"
#endif /* USE_VALGRIND_CLIENT_REQUESTS */

#define KATANA_L2X_VIEW_ARRAY_MAX_INDEX 4
#define KATANA2_L2X_VIEW_ARRAY_MAX_INDEX 6
#define TRIDENT2_L2X_VIEW_ARRAY_MAX_INDEX 10
#define TRIDENT2PLUS_L2X_VIEW_ARRAY_MAX_INDEX 10
#define TOMAHAWK_L2X_VIEW_ARRAY_MAX_INDEX 9

#ifdef BCM_PETRA_SUPPORT
/* read or write wide memory, supports ARAD only */
int _soc_arad_mem_array_wide_access(int unit, uint32 flags, soc_mem_t mem, unsigned array_index, int copyno, int index, void *entry_data,
                   unsigned operation)  ;/* operation should be 1 for read and 0 for write */
#endif
/*
 * This function pointer must be initialized by the application.
 * For the standard SDK this is done in diag_shell(), and
 * the function will point to sal_config_set() by default.
 */
int (*soc_mem_config_set)(char *name, char *value);

#if defined(BCM_PETRA_SUPPORT)
uint32 _soc_mem_entry_multicast_null[SOC_MAX_MEM_WORDS] = {0x7fffffff, 0};
uint32 _soc_mem_entry_jer_mcdb_null[SOC_MAX_MEM_WORDS] = {0xffffffff, 3};
uint32  _soc_mem_entry_llr_llvp_null[SOC_MAX_MEM_WORDS] = {0x90};
#endif /* BCM_PETRA_SUPPORT */

#if defined(BCM_ESW_SUPPORT) || defined(BCM_SIRIUS_SUPPORT) ||\
    defined(BCM_SAND_SUPPORT) || defined(BCM_CALADAN3_SUPPORT) || defined(PORTMOD_SUPPORT)

/* extra delay for quickturn */
uint32 soc_mem_fifo_delay_value = 15 * MILLISECOND_USEC;

#define _SOC_ENTRY_VALID_CHECK(unit, mem, valid_field)                      \
        if (SOC_MEM_FIELD_VALID(unit, mem, valid_field)) {                  \
            if (!soc_mem_field32_get(unit, mem, entry_data, valid_field)) { \
                return;                                                     \
            }                                                               \
        }

#define _SOC_ENTRY_PARITY_CLEAR(unit, mem, ecc_field)                       \
        if (SOC_MEM_FIELD_VALID(unit, mem, ecc_field)) {                    \
            soc_mem_field32_set(unit, mem, entry_data, ecc_field, 0);       \
            soc_mem_field32_set(unit, mem, entry_data_cache, ecc_field, 0); \
        }

/*
 * Empty (null) table entries
 */
uint32 _soc_mem_entry_null_zeroes[SOC_MAX_MEM_WORDS];
/* First word f's, rest 0's */
uint32 _soc_mem_entry_null_word0[SOC_MAX_MEM_WORDS] = { ~0, 0 };

#define SOC_MEM_COMPARE_RETURN(a, b) {       \
        if ((a) < (b)) { return -1; }        \
        if ((a) > (b)) { return  1; }        \
}
#define CMIC_B0_DMA_READ_COMMAND   (0x7)
#define CMIC_B0_DMA_WRITE_COMMAND  (0x9)

#define _SOC_MEM_CHK_L2_MEM(mem) \
        (mem == L2Xm || mem == L2_ENTRY_1m || mem == L2_ENTRY_2m)
#define _SOC_MEM_CHK_L3_MEM(mem) \
        (mem == L3_ENTRY_ONLYm || mem == L3_ENTRY_IPV4_UNICASTm || \
         mem == L3_ENTRY_IPV4_MULTICASTm || mem == L3_ENTRY_IPV6_UNICASTm || \
         mem == L3_ENTRY_IPV6_MULTICASTm || mem == L3_ENTRY_1m || \
         mem == L3_ENTRY_2m || mem == L3_ENTRY_4m)

#if defined(BCM_TOMAHAWK_SUPPORT)
#define _SOC_MEM_CHK_FPEM_MEM(mem) \
        (mem == EXACT_MATCH_2m || \
         mem == EXACT_MATCH_2_PIPE0m || mem == EXACT_MATCH_2_PIPE1m || \
         mem == EXACT_MATCH_2_PIPE2m || mem == EXACT_MATCH_2_PIPE3m || \
         mem == EXACT_MATCH_4m || \
         mem == EXACT_MATCH_4_PIPE0m || mem == EXACT_MATCH_4_PIPE1m || \
         mem == EXACT_MATCH_4_PIPE2m || mem == EXACT_MATCH_4_PIPE3m)
#define _SOC_MEM_CHK_IFP_TCAM(mem) \
        (mem == IFP_TCAMm || \
         mem == IFP_TCAM_PIPE0m || mem == IFP_TCAM_PIPE1m || \
         mem == IFP_TCAM_PIPE2m || mem == IFP_TCAM_PIPE3m)
#define _SOC_MEM_CHK_IFP_TCAM_WIDE(mem) \
        (mem == IFP_TCAM_WIDEm || \
         mem == IFP_TCAM_WIDE_PIPE0m || mem == IFP_TCAM_WIDE_PIPE1m || \
         mem == IFP_TCAM_WIDE_PIPE2m || mem == IFP_TCAM_WIDE_PIPE3m)
#endif /* BCM_TOMAHAWK_SUPPORT */

#define _SOC_MEM_CHK_EGR_IP_TUNNEL(mem) \
        (mem == EGR_IP_TUNNELm || mem == EGR_IP_TUNNEL_MPLSm || \
         mem == EGR_IP_TUNNEL_IPV6m)
#define _SOC_MEM_CHK_MPLS_MEM(mem) \
        (mem == MPLS_ENTRYm || mem == MPLS_ENTRY_1m || mem == MPLS_ENTRY_EXTDm)         
/* Macro to swap name to the mapped-to (actually-used) mem state name */
#define _SOC_MEM_REUSE_MEM_STATE(unit, mem) {\
    switch(mem) {\
    case VLAN_XLATE_1m: mem = VLAN_XLATEm; break;\
    case EP_VLAN_XLATE_1m: mem = EGR_VLAN_XLATEm; break;\
    case MPLS_ENTRY_1m: mem = MPLS_ENTRYm; break;\
    case VLAN_MACm: \
        if (SOC_IS_TRX(unit) && !soc_feature(unit, soc_feature_ism_memory)) {\
            mem = VLAN_XLATEm;\
        }\
        break;\
    case EGR_VLANm: \
        if (SOC_IS_KATANA2(unit)) {\
            mem = VLAN_TABm;\
        }\
        break;\
    default: break;\
    }\
}
#define _SOC_MEM_SKIP_READING_MEM_TO_CACHE(unit, mem, skip) {\
    soc_mem_t skip_mem = mem;\
    switch(mem) {\
        case RH_HGT_FLOWSETm:\
        case DLB_HGT_FLOWSETm:\
            if ((SOC_IS_TRIDENT2X(unit) || SOC_IS_TITAN2PLUS(unit)) &&\
                SOC_REG_IS_VALID(unit, ENHANCED_HASHING_CONTROLr)) {\
                uint32 is_rh_hgt = 0;\
                uint32 rval;\
                READ_ENHANCED_HASHING_CONTROLr(unit, &rval);\
                is_rh_hgt = soc_reg_field_get(unit, ENHANCED_HASHING_CONTROLr, rval,\
                                RH_HGT_ENABLEf);\
                skip_mem = (is_rh_hgt == 1)? DLB_HGT_FLOWSETm : RH_HGT_FLOWSETm;\
                skip = (skip_mem == mem)?1:0;\
            }\
            break;\
        default:\
            skip = (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CACHABLE)? 0 : 1;\
            break;\
    }\
}

#ifdef BROADCOM_DEBUG
static uint32 _soc_dma_debug_enable[SOC_MAX_NUM_DEVICES] = {0};
static uint32 _soc_dma_debug_op[SOC_MAX_NUM_DEVICES] = {0};
#endif /* BROADCOM_DEBUG */

/* Structure for hash bank info stored during each hash move */
typedef struct _soc_hash_bank_info_s {
    int         num_banks;
    int         bank_nums[7];
    int         banks;
    uint32      numb;
} _soc_hash_bank_info_t;


/* Check a mem has mapped-to (actually-used) mem state name or not */
int soc_mem_is_mapped_mem(int unit, soc_mem_t mem) 
{
    soc_mem_t mapped_mem = mem;
    
    _SOC_MEM_REUSE_MEM_STATE(unit, mapped_mem);

    if(mapped_mem == mem) {
        return FALSE;
    } else {
        return TRUE;    
    }

}
/* Two memory share same physical memory space, but they can't exist at the same time */
int soc_mem_is_shared_mem(int unit, soc_mem_t mem) 
{
    int skip = 0;

    switch(mem) {
        case DLB_HGT_FLOWSETm:
        case DLB_HGT_FLOWSET_Xm:
        case DLB_HGT_FLOWSET_Ym:
            mem = DLB_HGT_FLOWSETm;
            break;
        default:
            break;
    }
    _SOC_MEM_SKIP_READING_MEM_TO_CACHE(unit, mem, skip);
    if (skip == 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}


#ifdef BCM_PETRA_SUPPORT
int soc_arad_mem_broadcast_block_get(int unit, soc_mem_t mem){
    switch (SOC_BLOCK_TYPE(unit, SOC_MEM_BLOCK_ANY(unit, mem))) {
    case SOC_BLK_EGQ:
        return SOC_INFO(unit).brdc_egq_block;
    case SOC_BLK_IHB:
        return SOC_INFO(unit).brdc_ihb_block;
    case SOC_BLK_IHP:
        return SOC_INFO(unit).brdc_ihp_block;
    case SOC_BLK_IPS:
        return SOC_INFO(unit).brdc_ips_block;
    case SOC_BLK_IQM:
        return SOC_INFO(unit).brdc_iqm_block;
    case SOC_BLK_CGM:
        return SOC_INFO(unit).brdc_cgm_block;
    case SOC_BLK_SCH:
        return SOC_INFO(unit).brdc_sch_block;
    case SOC_BLK_EPNI:
        return SOC_INFO(unit).brdc_epni_block;
    default:
        return 0;
    }
}
#endif /* BCM_PETRA_SUPPORT */

/* Routine to check if this is the mem whose state is being (mapped-to) actually-used by others */
int _SOC_MEM_IS_REUSED_MEM(int unit, soc_mem_t mem) {\
    switch(mem) {\
    case EGR_VLAN_XLATEm: return TRUE;\
    case MPLS_ENTRYm: return TRUE;\
    case VLAN_XLATEm:\
        if (SOC_IS_TRX(unit)) {\
            return TRUE;\
        }\
        break;\
    case VLAN_TABm:\
        if (SOC_IS_KATANA2(unit)) {\
            return TRUE;\
        }\
        break;\
    case RH_HGT_FLOWSETm:\
        if ((SOC_IS_TRIDENT2X(unit) || SOC_IS_TITAN2PLUS(unit))) {\
           return TRUE;\
        }\
        break;\
    default: break;\
    }\
    return FALSE;\
}

/* Handle reverse mem state mapping updates - one to one or one to many */

/* DLB_HGT_FLOWSETm and RH_HGT_FLOWSETm share the same physical memory table,
if we write physical memory table according to format of DLB_HGT_FLOWSETm, and read the physical
memory according to format of RH_HGT_FLOWSETm, there will be a lot of SER error!*/
#define _SOC_MEM_SYNC_MAPPED_MEM_STATES(unit, mem) {\
    switch(mem) {\
    case EGR_VLAN_XLATEm: \
        SOC_MEM_STATE(unit, EP_VLAN_XLATE_1m).cache[blk] = cache;\
        SOC_MEM_STATE(unit, EP_VLAN_XLATE_1m).vmap[blk] = vmap;\
        break;\
    case MPLS_ENTRYm: \
        SOC_MEM_STATE(unit, MPLS_ENTRY_1m).cache[blk] = cache;\
        SOC_MEM_STATE(unit, MPLS_ENTRY_1m).vmap[blk] = vmap;\
        break;\
    case VLAN_XLATEm: \
        if (SOC_IS_TRX(unit)) {\
            if (soc_feature(unit, soc_feature_ism_memory)) {\
                SOC_MEM_STATE(unit, VLAN_XLATE_1m).cache[blk] = cache;\
                SOC_MEM_STATE(unit, VLAN_XLATE_1m).vmap[blk] = vmap;\
            } else {\
                SOC_MEM_STATE(unit, VLAN_MACm).cache[blk] = cache;\
                SOC_MEM_STATE(unit, VLAN_MACm).vmap[blk] = vmap;\
            }\
            break;\
        }\
    case VLAN_TABm: \
        if (SOC_IS_KATANA2(unit)) { \
            int blk_new; \
            SOC_MEM_BLOCK_ITER(unit, EGR_VLANm, blk_new) {\
                SOC_MEM_STATE(unit, EGR_VLANm).cache[blk_new] = cache;\
                SOC_MEM_STATE(unit, EGR_VLANm).vmap[blk_new] = vmap;\
                break;\
            }\
        }\
        break;\
    default: break;\
    }\
}
#ifdef BCM_TRIUMPH3_SUPPORT
#define _SOC_MEM_REPLACE_MEM(unit, mem) {\
    if (soc_feature(unit, soc_feature_ism_memory)) {\
        switch(mem) {\
        case VLAN_XLATE_1m: mem = VLAN_XLATEm; break;\
        case EP_VLAN_XLATE_1m: mem = EGR_VLAN_XLATEm; break;\
        case MPLS_ENTRY_1m: mem = MPLS_ENTRYm; break;\
        default: break;\
        }\
    }\
}
#else
#define _SOC_MEM_REPLACE_MEM(unit, mem)
#endif /* BCM_TRIUMPH3_SUPPORT */

/*
#ifdef BCM_PETRA_SUPPORT
 In case the memory is an alias or a format table, re-direct to the original memory
#define SOC_MEM_ALIAS_TO_ORIG(unit,mem)         ( mem = SOC_MEM_IS_VALID(unit, SOC_MEM_ALIAS_MAP(unit,mem)) ? \
                                                  SOC_MEM_ALIAS_MAP(unit,mem) : mem )
#else
#define SOC_MEM_ALIAS_TO_ORIG(unit,mem)
#endif
*/
#ifdef BCM_ESW_SUPPORT
#define HASH_MEM_OP_RETRY_COUNT 5
#endif

int
_soc_mem_cmp_word0(int unit, void *ent_a, void *ent_b)
{
    COMPILER_REFERENCE(unit);

    /*
     * Good for l3_def_ip_entry_t and l3_l3_entry_t.
     * The first uint32 (IP address) is the sort key.
     */

    SOC_MEM_COMPARE_RETURN(*(uint32 *)ent_a, *(uint32 *)ent_b);

    return 0;
}

int
_soc_mem_cmp_undef(int unit, void *ent_a, void *ent_b)
{
    COMPILER_REFERENCE(ent_a);
    COMPILER_REFERENCE(ent_b);

    LOG_CLI((BSL_META_U(unit,
                        "soc_mem_cmp: cannot compare entries of this type\n")));

    assert(0);

    return 0;
}

#ifdef BCM_XGS_SWITCH_SUPPORT

int
_soc_mem_cmp_l2x(int unit, void *ent_a, void *ent_b)
{
    sal_mac_addr_t mac_a, mac_b;
    vlan_id_t vlan_a, vlan_b;

    vlan_a = soc_L2Xm_field32_get(unit, ent_a, VLAN_IDf);
    vlan_b = soc_L2Xm_field32_get(unit, ent_b, VLAN_IDf);
    SOC_MEM_COMPARE_RETURN(vlan_a, vlan_b);

    soc_L2Xm_mac_addr_get(unit, ent_a, MAC_ADDRf, mac_a);
    soc_L2Xm_mac_addr_get(unit, ent_b, MAC_ADDRf, mac_b);

    return ENET_CMP_MACADDR(mac_a, mac_b);
}

int
_soc_mem_cmp_l2x_sync(int unit, void *ent_a, void *ent_b, int size)
{
    l2x_entry_t     *l2x1 = (l2x_entry_t *)ent_a;
    l2x_entry_t     *l2x2 = (l2x_entry_t *)ent_b;

    /* force bits that may be updated by HW to be the same */
    if (SOC_IS_XGS3_SWITCH(unit) || SOC_IS_XGS3_FABRIC(unit)) {
        soc_L2Xm_field32_set(unit, l2x1, HITSAf, 0);
        soc_L2Xm_field32_set(unit, l2x2, HITSAf, 0);
        soc_L2Xm_field32_set(unit, l2x1, HITDAf, 0);
        soc_L2Xm_field32_set(unit, l2x2, HITDAf, 0);
        if (SOC_MEM_FIELD_VALID(unit, L2Xm, LOCAL_SAf)) {
            soc_L2Xm_field32_set(unit, l2x1, LOCAL_SAf, 0);
            soc_L2Xm_field32_set(unit, l2x2, LOCAL_SAf, 0);
        }
        if (SOC_MEM_FIELD_VALID(unit, L2Xm, EVEN_PARITYf)) {
            soc_L2Xm_field32_set(unit, l2x1, EVEN_PARITYf, 0);
            soc_L2Xm_field32_set(unit, l2x2, EVEN_PARITYf, 0);
        }
#if defined(BCM_FELIX1_SUPPORT) || defined(BCM_HELIX1_SUPPORT)
        if (SOC_MEM_FIELD_VALID(unit, L2Xm, ODD_PARITYf)) {
            soc_L2Xm_field32_set(unit, l2x1, ODD_PARITYf, 0);
            soc_L2Xm_field32_set(unit, l2x2, ODD_PARITYf, 0);
        }
#endif
    }

    return sal_memcmp(ent_a, ent_b, size);
}

#ifdef BCM_TRIUMPH3_SUPPORT
int
_soc_mem_cmp_tr3_l2x_sync(int unit, void *ent_a, void *ent_b, uint8 hit_bits)
{
    uint32 val;
    soc_mem_t mem_type = L2_ENTRY_1m;
    l2_entry_1_entry_t *l2_1_1, *l2_1_2;
    l2_entry_2_entry_t *l2_2_1, *l2_2_2;

    val = soc_mem_field32_get(unit, L2_ENTRY_1m, (l2_entry_1_entry_t *)ent_a,
                              KEY_TYPEf);
    if ((val == SOC_MEM_KEY_L2_ENTRY_2_L2_BRIDGE) ||
        (val == SOC_MEM_KEY_L2_ENTRY_2_L2_VFI) ||
        (val == SOC_MEM_KEY_L2_ENTRY_2_L2_TRILL_NONUC_ACCESS)) {
        mem_type = L2_ENTRY_2m;
    }

    if (mem_type == L2_ENTRY_1m) {
        l2_1_1 = (l2_entry_1_entry_t *)ent_a;
        l2_1_2 = (l2_entry_1_entry_t *)ent_b;
        if (!(hit_bits & L2X_SHADOW_HIT_BITS)) {
            soc_L2_ENTRY_1m_field32_set(unit, l2_1_1, HITSAf, 0);
            soc_L2_ENTRY_1m_field32_set(unit, l2_1_2, HITSAf, 0);
            soc_L2_ENTRY_1m_field32_set(unit, l2_1_1, HITDAf, 0);
            soc_L2_ENTRY_1m_field32_set(unit, l2_1_2, HITDAf, 0);
        } else {
            if (!(hit_bits & L2X_SHADOW_HIT_SRC)) {
                soc_L2_ENTRY_1m_field32_set(unit, l2_1_1, HITSAf, 0);
                soc_L2_ENTRY_1m_field32_set(unit, l2_1_2, HITSAf, 0);
            }
            if (!(hit_bits & L2X_SHADOW_HIT_DST)) {
                soc_L2_ENTRY_1m_field32_set(unit, l2_1_1, HITDAf, 0);
                soc_L2_ENTRY_1m_field32_set(unit, l2_1_2, HITDAf, 0);
            }
        }
        soc_L2_ENTRY_1m_field32_set(unit, l2_1_1, LOCAL_SAf, 0);
        soc_L2_ENTRY_1m_field32_set(unit, l2_1_2, LOCAL_SAf, 0);
        soc_L2_ENTRY_1m_field32_set(unit, l2_1_1, EVEN_PARITYf, 0);
        soc_L2_ENTRY_1m_field32_set(unit, l2_1_2, EVEN_PARITYf, 0);
        return sal_memcmp(ent_a, ent_b, sizeof(l2_entry_1_entry_t));
    } else {
        l2_2_1 = (l2_entry_2_entry_t *)ent_a;
        l2_2_2 = (l2_entry_2_entry_t *)ent_b;
        if (!(hit_bits & L2X_SHADOW_HIT_BITS)) {
            soc_L2_ENTRY_2m_field32_set(unit, l2_2_1, HITSAf, 0);
            soc_L2_ENTRY_2m_field32_set(unit, l2_2_2, HITSAf, 0);
            soc_L2_ENTRY_2m_field32_set(unit, l2_2_1, HITDAf, 0);
            soc_L2_ENTRY_2m_field32_set(unit, l2_2_2, HITDAf, 0);
        } else {
            if (!(hit_bits & L2X_SHADOW_HIT_SRC)) {
                soc_L2_ENTRY_2m_field32_set(unit, l2_2_1, HITSAf, 0);
                soc_L2_ENTRY_2m_field32_set(unit, l2_2_2, HITSAf, 0);
            }
            if (!(hit_bits & L2X_SHADOW_HIT_DST)) {
                soc_L2_ENTRY_2m_field32_set(unit, l2_2_1, HITDAf, 0);
                soc_L2_ENTRY_2m_field32_set(unit, l2_2_2, HITDAf, 0);
            }
        }
        soc_L2_ENTRY_2m_field32_set(unit, l2_2_1, LOCAL_SAf, 0);
        soc_L2_ENTRY_2m_field32_set(unit, l2_2_2, LOCAL_SAf, 0);
        soc_L2_ENTRY_2m_field32_set(unit, l2_2_1, EVEN_PARITY_0f, 0);
        soc_L2_ENTRY_2m_field32_set(unit, l2_2_2, EVEN_PARITY_0f, 0);
        soc_L2_ENTRY_2m_field32_set(unit, l2_2_1, EVEN_PARITY_1f, 0);
        soc_L2_ENTRY_2m_field32_set(unit, l2_2_2, EVEN_PARITY_1f, 0);
        return sal_memcmp(ent_a, ent_b, sizeof(l2_entry_2_entry_t));
    }
}

int
_soc_mem_cmp_tr3_ext_l2x_1_sync(int unit, void *ent_a, void *ent_b,
                                uint8 hit_bits)
{
    ext_l2_entry_1_entry_t *ext_l2_1_1, *ext_l2_1_2;
    ext_l2_1_1 = (ext_l2_entry_1_entry_t *)ent_a;
    ext_l2_1_2 = (ext_l2_entry_1_entry_t *)ent_b;
    if (!(hit_bits & L2X_SHADOW_HIT_BITS)) {
        soc_EXT_L2_ENTRY_1m_field32_set(unit, ext_l2_1_1, HITSAf, 0);
        soc_EXT_L2_ENTRY_1m_field32_set(unit, ext_l2_1_2, HITSAf, 0);
        soc_EXT_L2_ENTRY_1m_field32_set(unit, ext_l2_1_1, HITDAf, 0);
        soc_EXT_L2_ENTRY_1m_field32_set(unit, ext_l2_1_2, HITDAf, 0);
    } else {
        if (!(hit_bits & L2X_SHADOW_HIT_SRC)) {
            soc_EXT_L2_ENTRY_1m_field32_set(unit, ext_l2_1_1, HITSAf, 0);
            soc_EXT_L2_ENTRY_1m_field32_set(unit, ext_l2_1_2, HITSAf, 0);
        }
        if (!(hit_bits & L2X_SHADOW_HIT_DST)) {
            soc_EXT_L2_ENTRY_1m_field32_set(unit, ext_l2_1_1, HITDAf, 0);
            soc_EXT_L2_ENTRY_1m_field32_set(unit, ext_l2_1_2, HITDAf, 0);
        }
    }
    soc_EXT_L2_ENTRY_1m_field32_set(unit, ext_l2_1_1, LOCAL_SAf, 0);
    soc_EXT_L2_ENTRY_1m_field32_set(unit, ext_l2_1_2, LOCAL_SAf, 0);
    soc_EXT_L2_ENTRY_1m_field32_set(unit, ext_l2_1_1, EVEN_PARITYf, 0);
    soc_EXT_L2_ENTRY_1m_field32_set(unit, ext_l2_1_2, EVEN_PARITYf, 0);
    return sal_memcmp(ent_a, ent_b, sizeof(ext_l2_entry_1_entry_t));
}

int
_soc_mem_cmp_tr3_ext_l2x_2_sync(int unit, void *ent_a, void *ent_b,
                                uint8 hit_bits)
{
    ext_l2_entry_2_entry_t *ext_l2_2_1, *ext_l2_2_2;
    ext_l2_2_1 = (ext_l2_entry_2_entry_t *)ent_a;
    ext_l2_2_2 = (ext_l2_entry_2_entry_t *)ent_b;
    if (!(hit_bits & L2X_SHADOW_HIT_BITS)) {
        soc_EXT_L2_ENTRY_2m_field32_set(unit, ext_l2_2_1, HITSAf, 0);
        soc_EXT_L2_ENTRY_2m_field32_set(unit, ext_l2_2_2, HITSAf, 0);
        soc_EXT_L2_ENTRY_2m_field32_set(unit, ext_l2_2_1, HITDAf, 0);
        soc_EXT_L2_ENTRY_2m_field32_set(unit, ext_l2_2_2, HITDAf, 0);
    } else {
        if (!(hit_bits & L2X_SHADOW_HIT_SRC)) {
            soc_EXT_L2_ENTRY_2m_field32_set(unit, ext_l2_2_1, HITSAf, 0);
            soc_EXT_L2_ENTRY_2m_field32_set(unit, ext_l2_2_2, HITSAf, 0);
        }
        if (!(hit_bits & L2X_SHADOW_HIT_DST)) {
            soc_EXT_L2_ENTRY_2m_field32_set(unit, ext_l2_2_1, HITDAf, 0);
            soc_EXT_L2_ENTRY_2m_field32_set(unit, ext_l2_2_2, HITDAf, 0);
        }
    }
    soc_EXT_L2_ENTRY_2m_field32_set(unit, ext_l2_2_1, LOCAL_SAf, 0);
    soc_EXT_L2_ENTRY_2m_field32_set(unit, ext_l2_2_2, LOCAL_SAf, 0);
    soc_EXT_L2_ENTRY_2m_field32_set(unit, ext_l2_2_1, EVEN_PARITYf, 0);
    soc_EXT_L2_ENTRY_2m_field32_set(unit, ext_l2_2_2, EVEN_PARITYf, 0);
    return sal_memcmp(ent_a, ent_b, sizeof(ext_l2_entry_2_entry_t));
}

int
_soc_mem_cmp_tr3_l2x(int unit, void *ent_a, void *ent_b)
{
    uint32 val_a, val_b;
    sal_mac_addr_t mac_a, mac_b;

    val_a = soc_L2_ENTRY_1m_field32_get(unit, ent_a, KEY_TYPEf);
    val_b = soc_L2_ENTRY_1m_field32_get(unit, ent_b, KEY_TYPEf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    switch (val_a) {
    case SOC_MEM_KEY_L2_ENTRY_1_L2_BRIDGE: /* BRIDGE */
        val_a = soc_L2_ENTRY_1m_field32_get(unit, ent_a, L2__VLAN_IDf);
        val_b = soc_L2_ENTRY_1m_field32_get(unit, ent_b, L2__VLAN_IDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        soc_L2_ENTRY_1m_mac_addr_get(unit, ent_a, L2__MAC_ADDRf, mac_a);
        soc_L2_ENTRY_1m_mac_addr_get(unit, ent_b, L2__MAC_ADDRf, mac_b);
        return ENET_CMP_MACADDR(mac_a, mac_b);

    case SOC_MEM_KEY_L2_ENTRY_2_L2_BRIDGE: /* BRIDGE */
        val_a = soc_L2_ENTRY_2m_field32_get(unit, ent_a, L2__VLAN_IDf);
        val_b = soc_L2_ENTRY_2m_field32_get(unit, ent_b, L2__VLAN_IDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        soc_L2_ENTRY_2m_mac_addr_get(unit, ent_a, L2__MAC_ADDRf, mac_a);
        soc_L2_ENTRY_2m_mac_addr_get(unit, ent_b, L2__MAC_ADDRf, mac_b);
        return ENET_CMP_MACADDR(mac_a, mac_b);

    case SOC_MEM_KEY_L2_ENTRY_1_L2_VFI: /* VFI */
        val_a = soc_L2_ENTRY_1m_field32_get(unit, ent_a, L2__VFIf);
        val_b = soc_L2_ENTRY_1m_field32_get(unit, ent_b, L2__VFIf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        soc_L2_ENTRY_1m_mac_addr_get(unit, ent_a, L2__MAC_ADDRf, mac_a);
        soc_L2_ENTRY_1m_mac_addr_get(unit, ent_b, L2__MAC_ADDRf, mac_b);
        return ENET_CMP_MACADDR(mac_a, mac_b);

    case SOC_MEM_KEY_L2_ENTRY_2_L2_VFI: /* VFI */
        val_a = soc_L2_ENTRY_2m_field32_get(unit, ent_a, L2__VFIf);
        val_b = soc_L2_ENTRY_2m_field32_get(unit, ent_b, L2__VFIf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        soc_L2_ENTRY_2m_mac_addr_get(unit, ent_a, L2__MAC_ADDRf, mac_a);
        soc_L2_ENTRY_2m_mac_addr_get(unit, ent_b, L2__MAC_ADDRf, mac_b);
        return ENET_CMP_MACADDR(mac_a, mac_b);

    case SOC_MEM_KEY_L2_ENTRY_1_VLAN_SINGLE_CROSS_CONNECT: /* SINGLE_CROSS_CONNECT */
        val_a = soc_L2_ENTRY_1m_field32_get(unit, ent_a, VLAN__OVIDf);
        val_b = soc_L2_ENTRY_1m_field32_get(unit, ent_b, VLAN__OVIDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);
        return 0;

    case SOC_MEM_KEY_L2_ENTRY_1_VLAN_DOUBLE_CROSS_CONNECT: /* DOUBLE_CROSS_CONNECT */
        val_a = soc_L2_ENTRY_1m_field32_get(unit, ent_a, VLAN__OVIDf);
        val_b = soc_L2_ENTRY_1m_field32_get(unit, ent_b, VLAN__OVIDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        val_a = soc_L2_ENTRY_1m_field32_get(unit, ent_a, VLAN__IVIDf);
        val_b = soc_L2_ENTRY_1m_field32_get(unit, ent_b, VLAN__IVIDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);
        return 0;

    case SOC_MEM_KEY_L2_ENTRY_1_VIF_VIF: /* VIF */
        val_a = soc_L2_ENTRY_1m_field32_get(unit, ent_a, VIF__NAMESPACEf);
        val_b = soc_L2_ENTRY_1m_field32_get(unit, ent_b, VIF__NAMESPACEf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        val_a = soc_L2_ENTRY_1m_field32_get(unit, ent_a, VIF__DST_VIFf);
        val_b = soc_L2_ENTRY_1m_field32_get(unit, ent_b, VIF__DST_VIFf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        val_a = soc_L2_ENTRY_1m_field32_get(unit, ent_a, VIF__Pf);
        val_b = soc_L2_ENTRY_1m_field32_get(unit, ent_b, VIF__Pf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);
        return 0;

    case SOC_MEM_KEY_L2_ENTRY_2_L2_TRILL_NONUC_ACCESS: /* TRILL_NONUC_ACCESS */
        val_a = soc_L2_ENTRY_2m_field32_get(unit, ent_a,
                                            L2__VLAN_IDf);
        val_b = soc_L2_ENTRY_2m_field32_get(unit, ent_b,
                                            L2__VLAN_IDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        soc_L2_ENTRY_2m_mac_addr_get(unit, ent_a, L2__MAC_ADDRf,
                                     mac_a);
        soc_L2_ENTRY_2m_mac_addr_get(unit, ent_b, L2__MAC_ADDRf,
                                     mac_b);
        return ENET_CMP_MACADDR(mac_a, mac_b);

    case SOC_MEM_KEY_L2_ENTRY_1_TRILL_NONUC_NETWORK_LONG_TRILL_NONUC_NETWORK_LONG: /* TRILL_NONUC_NETWORK_LONG */
        val_a = soc_L2_ENTRY_1m_field32_get(unit, ent_a,
                                            TRILL_NONUC_NETWORK_LONG__VLAN_IDf);
        val_b = soc_L2_ENTRY_1m_field32_get(unit, ent_b,
                                            TRILL_NONUC_NETWORK_LONG__VLAN_IDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        val_a = soc_L2_ENTRY_1m_field32_get(unit, ent_a,
                                            TRILL_NONUC_NETWORK_LONG__TREE_IDf);
        val_b = soc_L2_ENTRY_1m_field32_get(unit, ent_b,
                                            TRILL_NONUC_NETWORK_LONG__TREE_IDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        soc_L2_ENTRY_1m_mac_addr_get
            (unit, ent_a, TRILL_NONUC_NETWORK_LONG__MAC_ADDRESSf, mac_a);
        soc_L2_ENTRY_1m_mac_addr_get
            (unit, ent_b, TRILL_NONUC_NETWORK_LONG__MAC_ADDRESSf, mac_b);
        return ENET_CMP_MACADDR(mac_a, mac_b);

    case SOC_MEM_KEY_L2_ENTRY_1_TRILL_NONUC_NETWORK_SHORT_TRILL_NONUC_NETWORK_SHORT: /* TRILL_NONUC_NETWORK_SHORT */
        val_a = soc_L2_ENTRY_1m_field32_get(unit, ent_a,
                                            TRILL_NONUC_NETWORK_SHORT__VLAN_IDf);
        val_b = soc_L2_ENTRY_1m_field32_get(unit, ent_b,
                                            TRILL_NONUC_NETWORK_SHORT__VLAN_IDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        val_a = soc_L2_ENTRY_1m_field32_get(unit, ent_a,
                                            TRILL_NONUC_NETWORK_SHORT__TREE_IDf);
        val_b = soc_L2_ENTRY_1m_field32_get(unit, ent_b,
                                            TRILL_NONUC_NETWORK_SHORT__TREE_IDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);
        return 0;

    case SOC_MEM_KEY_L2_ENTRY_1_BFD_BFD: /* BFD */
        val_a = soc_L2_ENTRY_1m_field32_get(unit, ent_a, BFD__YOUR_DISCRIMINATORf);
        val_b = soc_L2_ENTRY_1m_field32_get(unit, ent_b, BFD__YOUR_DISCRIMINATORf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);
        return 0;
    default:
        return 1;
    }
}

int
_soc_mem_cmp_tr3_ext_l2x(int unit, void *ent_a, void *ent_b)
{
    uint32 val_a, val_b;
    sal_mac_addr_t mac_a, mac_b;

    val_a = soc_EXT_L2_ENTRY_1m_field32_get(unit, ent_a, KEY_TYPEf);
    val_b = soc_EXT_L2_ENTRY_1m_field32_get(unit, ent_b, KEY_TYPEf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    if (val_a) { /* VFI */
        val_a = soc_EXT_L2_ENTRY_1m_field32_get(unit, ent_a, VFIf);
        val_b = soc_EXT_L2_ENTRY_1m_field32_get(unit, ent_b, VFIf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        soc_EXT_L2_ENTRY_1m_mac_addr_get(unit, ent_a, MAC_ADDRf, mac_a);
        soc_EXT_L2_ENTRY_1m_mac_addr_get(unit, ent_b, MAC_ADDRf, mac_b);
        return ENET_CMP_MACADDR(mac_a, mac_b);
    } else { /* BRIDGE */
        val_a = soc_EXT_L2_ENTRY_1m_field32_get(unit, ent_a, VLAN_IDf);
        val_b = soc_EXT_L2_ENTRY_1m_field32_get(unit, ent_b, VLAN_IDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        soc_EXT_L2_ENTRY_1m_mac_addr_get(unit, ent_a, MAC_ADDRf, mac_a);
        soc_EXT_L2_ENTRY_1m_mac_addr_get(unit, ent_b, MAC_ADDRf, mac_b);
        return ENET_CMP_MACADDR(mac_a, mac_b);
    }
}
#endif /* BCM_TRIUMPH3_SUPPORT */

#ifdef BCM_FIREBOLT_SUPPORT
int
_soc_mem_cmp_l2x2(int unit, void *ent_a, void *ent_b)
{
    sal_mac_addr_t mac_a, mac_b;
    uint32 val_a, val_b;
    uint32 buf_a[SOC_MAX_MEM_FIELD_WORDS] = {0};
    uint32 buf_b[SOC_MAX_MEM_FIELD_WORDS] = {0};

    if ((SOC_IS_KATANA2(unit)) || (SOC_IS_SABER2(unit)) ||
        (SOC_IS_KATANA(unit)) || (SOC_IS_HURRICANE3(unit)) ||
        (SOC_IS_METROLITE(unit)) || (SOC_IS_GREYHOUND(unit))) {
        if (SOC_MEM_FIELD_VALID(unit, L2Xm, KEY_TYPEf)) {
            val_a = soc_L2Xm_field32_get(unit, ent_a, KEY_TYPEf);
            val_b = soc_L2Xm_field32_get(unit, ent_b, KEY_TYPEf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            switch (val_a) {
            case 0: /* BRIDGE */
                val_a = soc_L2Xm_field32_get(unit, ent_a, VLAN_IDf);
                val_b = soc_L2Xm_field32_get(unit, ent_b, VLAN_IDf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                soc_L2Xm_mac_addr_get(unit, ent_a, MAC_ADDRf, mac_a);
                soc_L2Xm_mac_addr_get(unit, ent_b, MAC_ADDRf, mac_b);
                return ENET_CMP_MACADDR(mac_a, mac_b);

            case 1: /* SINGLE_CROSS_CONNECT */
                if (SOC_MEM_FIELD_VALID(unit, L2Xm, VLAN__OVIDf)) {
                    val_a = soc_L2Xm_field32_get(unit, ent_a, VLAN__OVIDf);
                    val_b = soc_L2Xm_field32_get(unit, ent_b, VLAN__OVIDf);
                } else {
                    val_a = soc_L2Xm_field32_get(unit, ent_a, OVIDf);
                    val_b = soc_L2Xm_field32_get(unit, ent_b, OVIDf);
                }
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                return 0;
            case 2: /* DOUBLE_CROSS_CONNECT */
                if (SOC_MEM_FIELD_VALID(unit, L2Xm, VLAN__OVIDf)) {
                    val_a = soc_L2Xm_field32_get(unit, ent_a, VLAN__OVIDf);
                    val_b = soc_L2Xm_field32_get(unit, ent_b, VLAN__OVIDf);
                } else {
                    val_a = soc_L2Xm_field32_get(unit, ent_a, OVIDf);
                    val_b = soc_L2Xm_field32_get(unit, ent_b, OVIDf);
                }
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                if (SOC_MEM_FIELD_VALID(unit, L2Xm, VLAN__IVIDf)) {
                    val_a = soc_L2Xm_field32_get(unit, ent_a, VLAN__IVIDf);
                    val_b = soc_L2Xm_field32_get(unit, ent_b, VLAN__IVIDf);
                } else {
                    val_a = soc_L2Xm_field32_get(unit, ent_a, IVIDf);
                    val_b = soc_L2Xm_field32_get(unit, ent_b, IVIDf);
                }
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                return 0;

            case 3: /* VFI */
                val_a = soc_L2Xm_field32_get(unit, ent_a, VFIf);
                val_b = soc_L2Xm_field32_get(unit, ent_b, VFIf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                soc_L2Xm_mac_addr_get(unit, ent_a, MAC_ADDRf, mac_a);
                soc_L2Xm_mac_addr_get(unit, ent_b, MAC_ADDRf, mac_b);
                return ENET_CMP_MACADDR(mac_a, mac_b);

            case 4: /*  BFD */
                soc_L2Xm_field_get(unit, ent_a, BFD__KEYf, buf_a);
                soc_L2Xm_field_get(unit, ent_b, BFD__KEYf, buf_b);
                SOC_MEM_COMPARE_RETURN(buf_a[1], buf_b[1]);
                SOC_MEM_COMPARE_RETURN(buf_a[0], buf_b[0]);

            return 0;

            case 5: /* VIF */
                val_a = soc_L2Xm_field32_get(unit, ent_a, VIF__NAMESPACEf);
                val_b = soc_L2Xm_field32_get(unit, ent_b, VIF__NAMESPACEf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                val_a = soc_L2Xm_field32_get(unit, ent_a, VIF__DST_VIFf);
                val_b = soc_L2Xm_field32_get(unit, ent_b, VIF__DST_VIFf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                val_a = soc_L2Xm_field32_get(unit, ent_a, VIF__Pf);
                val_b = soc_L2Xm_field32_get(unit, ent_b, VIF__Pf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                return 0;

            case 6: /*  PE_VID */
                soc_L2Xm_field_get(unit, ent_a, PE_VID__KEYf, buf_a);
                soc_L2Xm_field_get(unit, ent_b, PE_VID__KEYf, buf_b);
                SOC_MEM_COMPARE_RETURN(buf_a[1], buf_b[1]);
                SOC_MEM_COMPARE_RETURN(buf_a[0], buf_b[0]);

                return 0;

            default:
                return 1;
            }
        }
    } else {

        if (SOC_MEM_FIELD_VALID(unit, L2Xm, KEY_TYPEf)) {
            val_a = soc_L2Xm_field32_get(unit, ent_a, KEY_TYPEf);
            val_b = soc_L2Xm_field32_get(unit, ent_b, KEY_TYPEf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            switch (val_a) {
            case 0: /* BRIDGE */
                val_a = soc_L2Xm_field32_get(unit, ent_a, VLAN_IDf);
                val_b = soc_L2Xm_field32_get(unit, ent_b, VLAN_IDf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                soc_L2Xm_mac_addr_get(unit, ent_a, MAC_ADDRf, mac_a);
                soc_L2Xm_mac_addr_get(unit, ent_b, MAC_ADDRf, mac_b);
                return ENET_CMP_MACADDR(mac_a, mac_b);

            case 1: /* SINGLE_CROSS_CONNECT */
                if (SOC_MEM_FIELD_VALID(unit, L2Xm, VLAN__OVIDf)) {
                    val_a = soc_L2Xm_field32_get(unit, ent_a, VLAN__OVIDf);
                    val_b = soc_L2Xm_field32_get(unit, ent_b, VLAN__OVIDf);
                } else {
                    val_a = soc_L2Xm_field32_get(unit, ent_a, OVIDf);
                    val_b = soc_L2Xm_field32_get(unit, ent_b, OVIDf);
                }
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                return 0;
            case 2: /* DOUBLE_CROSS_CONNECT */
                if (SOC_MEM_FIELD_VALID(unit, L2Xm, VLAN__OVIDf)) {
                    val_a = soc_L2Xm_field32_get(unit, ent_a, VLAN__OVIDf);
                    val_b = soc_L2Xm_field32_get(unit, ent_b, VLAN__OVIDf);
                } else {
                    val_a = soc_L2Xm_field32_get(unit, ent_a, OVIDf);
                    val_b = soc_L2Xm_field32_get(unit, ent_b, OVIDf);
                }
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                if (SOC_MEM_FIELD_VALID(unit, L2Xm, VLAN__IVIDf)) {
                    val_a = soc_L2Xm_field32_get(unit, ent_a, VLAN__IVIDf);
                    val_b = soc_L2Xm_field32_get(unit, ent_b, VLAN__IVIDf);
                } else {
                    val_a = soc_L2Xm_field32_get(unit, ent_a, IVIDf);
                    val_b = soc_L2Xm_field32_get(unit, ent_b, IVIDf);
                }
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                return 0;

            case 3: /* VFI */
                val_a = soc_L2Xm_field32_get(unit, ent_a, VFIf);
                val_b = soc_L2Xm_field32_get(unit, ent_b, VFIf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                soc_L2Xm_mac_addr_get(unit, ent_a, MAC_ADDRf, mac_a);
                soc_L2Xm_mac_addr_get(unit, ent_b, MAC_ADDRf, mac_b);
                return ENET_CMP_MACADDR(mac_a, mac_b);

            case 4: /* VIF */
                val_a = soc_L2Xm_field32_get(unit, ent_a, VIF__NAMESPACEf);
                val_b = soc_L2Xm_field32_get(unit, ent_b, VIF__NAMESPACEf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                val_a = soc_L2Xm_field32_get(unit, ent_a, VIF__DST_VIFf);
                val_b = soc_L2Xm_field32_get(unit, ent_b, VIF__DST_VIFf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                val_a = soc_L2Xm_field32_get(unit, ent_a, VIF__Pf);
                val_b = soc_L2Xm_field32_get(unit, ent_b, VIF__Pf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                return 0;

            case 5: /* TRILL_NONUC_ACCESS */
                if (SOC_IS_TRIDENT(unit)) {
                    val_a = soc_L2Xm_field32_get(unit, ent_a,
                                             TRILL_NONUC_ACCESS__VLAN_IDf);
                    val_b = soc_L2Xm_field32_get(unit, ent_b,
                                             TRILL_NONUC_ACCESS__VLAN_IDf);
                } else {
                    val_a = soc_L2Xm_field32_get(unit, ent_a, L2__VLAN_IDf);
                    val_b = soc_L2Xm_field32_get(unit, ent_b, L2__VLAN_IDf);
                }
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                if (SOC_IS_TRIDENT(unit)) {
                    soc_L2Xm_mac_addr_get(unit, ent_a,
                                         TRILL_NONUC_ACCESS__MAC_ADDRf, mac_a);
                    soc_L2Xm_mac_addr_get(unit, ent_b,
                                         TRILL_NONUC_ACCESS__MAC_ADDRf, mac_b);
                } else {
                    soc_L2Xm_mac_addr_get(unit, ent_a, L2__MAC_ADDRf, mac_a);
                    soc_L2Xm_mac_addr_get(unit, ent_b, L2__MAC_ADDRf, mac_b);
                }
                return ENET_CMP_MACADDR(mac_a, mac_b);

            case 6: /* TRILL_NONUC_NETWORK_LONG */
                val_a = soc_L2Xm_field32_get(unit, ent_a,
                                             TRILL_NONUC_NETWORK_LONG__VLAN_IDf);
                val_b = soc_L2Xm_field32_get(unit, ent_b,
                                             TRILL_NONUC_NETWORK_LONG__VLAN_IDf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                val_a = soc_L2Xm_field32_get(unit, ent_a,
                                             TRILL_NONUC_NETWORK_LONG__TREE_IDf);
                val_b = soc_L2Xm_field32_get(unit, ent_b,
                                             TRILL_NONUC_NETWORK_LONG__TREE_IDf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                soc_L2Xm_mac_addr_get
                    (unit, ent_a, TRILL_NONUC_NETWORK_LONG__MAC_ADDRESSf, mac_a);
                soc_L2Xm_mac_addr_get
                    (unit, ent_b, TRILL_NONUC_NETWORK_LONG__MAC_ADDRESSf, mac_b);
                return ENET_CMP_MACADDR(mac_a, mac_b);

            case 7: /* TRILL_NONUC_NETWORK_SHORT */
                val_a = soc_L2Xm_field32_get(unit, ent_a,
                                             TRILL_NONUC_NETWORK_SHORT__VLAN_IDf);
                val_b = soc_L2Xm_field32_get(unit, ent_b,
                                             TRILL_NONUC_NETWORK_SHORT__VLAN_IDf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                val_a = soc_L2Xm_field32_get(unit, ent_a,
                                             TRILL_NONUC_NETWORK_SHORT__TREE_IDf);
                val_b = soc_L2Xm_field32_get(unit, ent_b,
                                             TRILL_NONUC_NETWORK_SHORT__TREE_IDf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                return 0;

            case 8: /*  BFD */
                soc_L2Xm_field_get(unit, ent_a, BFD__KEYf, buf_a);
                soc_L2Xm_field_get(unit, ent_b, BFD__KEYf, buf_b);
                SOC_MEM_COMPARE_RETURN(buf_a[1], buf_b[1]);
                SOC_MEM_COMPARE_RETURN(buf_a[0], buf_b[0]);

                return 0;

            case 9: /*  PE_VID */
                soc_L2Xm_field_get(unit, ent_a, PE_VID__KEYf, buf_a);
                soc_L2Xm_field_get(unit, ent_b, PE_VID__KEYf, buf_b);
                SOC_MEM_COMPARE_RETURN(buf_a[1], buf_b[1]);
                SOC_MEM_COMPARE_RETURN(buf_a[0], buf_b[0]);

                return 0;

            case 10: /* FCOE_ZONE */
                soc_L2Xm_field_get(unit, ent_a, FCOE_ZONE__KEYf, buf_a);
                soc_L2Xm_field_get(unit, ent_b, FCOE_ZONE__KEYf, buf_b);
                SOC_MEM_COMPARE_RETURN(buf_a[2], buf_b[2]);
                SOC_MEM_COMPARE_RETURN(buf_a[1], buf_b[1]);
                SOC_MEM_COMPARE_RETURN(buf_a[0], buf_b[0]);

                return 0;
            default:
                return 1;
            }
        }
    }

    val_a = soc_L2Xm_field32_get(unit, ent_a, VLAN_IDf);
    val_b = soc_L2Xm_field32_get(unit, ent_b, VLAN_IDf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    soc_L2Xm_mac_addr_get(unit, ent_a, MAC_ADDRf, mac_a);
    soc_L2Xm_mac_addr_get(unit, ent_b, MAC_ADDRf, mac_b);
    return ENET_CMP_MACADDR(mac_a, mac_b);
}

int
_soc_mem_cmp_l3x2(int unit, void *ent_a, void *ent_b)
{
    uint32 val_a, val_b;

    if (!SOC_MEM_FIELD_VALID(unit, L3_ENTRY_ONLYm, KEY_TYPEf)) {
        return 0;
    }

    val_a = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_a, KEY_TYPEf);
    val_b = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_b, KEY_TYPEf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

#ifdef BCM_TRIDENT2_SUPPORT
    if (SOC_IS_TD2_TT2(unit)) {
        uint32 buf_a[SOC_MAX_MEM_FIELD_WORDS];
        uint32 buf_b[SOC_MAX_MEM_FIELD_WORDS];
        switch (val_a) {
        case TD2_L3_HASH_KEY_TYPE_V4UC:
            return _soc_mem_cmp_l3x2_ip4ucast(unit, ent_a, ent_b);
        case TD2_L3_HASH_KEY_TYPE_V4UC_EXT:
            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_a,
                                        KEY_TYPE_1f);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_b,
                                        KEY_TYPE_1f);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_a,
                                        IPV4UC_EXT__IP_ADDRf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_b,
                                        IPV4UC_EXT__IP_ADDRf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_a,
                                        IPV4UC_EXT__VRF_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_b,
                                        IPV4UC_EXT__VRF_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;
        case TD2_L3_HASH_KEY_TYPE_V4MC:
        case TD2_L3_HASH_KEY_TYPE_V4L2MC:
        case TD2_L3_HASH_KEY_TYPE_V4L2VPMC:
            return _soc_mem_cmp_l3x2_ip4mcast(unit, ent_a, ent_b);
        case TD2_L3_HASH_KEY_TYPE_V6UC:
            return _soc_mem_cmp_l3x2_ip6ucast(unit, ent_a, ent_b);
        case TD2_L3_HASH_KEY_TYPE_V6UC_EXT:
            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV6_MULTICASTm, ent_a,
                                        KEY_TYPE_1f);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV6_MULTICASTm, ent_b,
                                        KEY_TYPE_1f);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV6_MULTICASTm, ent_a,
                                        KEY_TYPE_2f);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV6_MULTICASTm, ent_b,
                                        KEY_TYPE_2f);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV6_MULTICASTm, ent_a,
                                        KEY_TYPE_3f);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV6_MULTICASTm, ent_b,
                                        KEY_TYPE_3f);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            soc_mem_field_get(unit, L3_ENTRY_IPV6_MULTICASTm, ent_a,
                              IPV6UC_EXT__IP_ADDR_LWR_64f, buf_a);
            soc_mem_field_get(unit, L3_ENTRY_IPV6_MULTICASTm, ent_b,
                              IPV6UC_EXT__IP_ADDR_LWR_64f, buf_b);
            SOC_MEM_COMPARE_RETURN(buf_a[0], buf_b[0]);
            SOC_MEM_COMPARE_RETURN(buf_a[1], buf_b[1]);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV6_MULTICASTm, ent_a,
                                        IPV6UC_EXT__VRF_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV6_MULTICASTm, ent_b,
                                        IPV6UC_EXT__VRF_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            soc_mem_field_get(unit, L3_ENTRY_IPV6_MULTICASTm, ent_a,
                              IPV6UC_EXT__IP_ADDR_UPR_64f, buf_a);
            soc_mem_field_get(unit, L3_ENTRY_IPV6_MULTICASTm, ent_b,
                              IPV6UC_EXT__IP_ADDR_UPR_64f, buf_b);
            SOC_MEM_COMPARE_RETURN(buf_a[0], buf_b[0]);
            SOC_MEM_COMPARE_RETURN(buf_a[1], buf_b[1]);

            return 0;
        case TD2_L3_HASH_KEY_TYPE_V6MC:
        case TD2_L3_HASH_KEY_TYPE_V6L2MC:
        case TD2_L3_HASH_KEY_TYPE_V6L2VPMC:
            return _soc_mem_cmp_l3x2_ip6mcast(unit, ent_a, ent_b);

        case TD2_L3_HASH_KEY_TYPE_TRILL:
            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_a,
                                        TRILL__INGRESS_RBRIDGE_NICKNAMEf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_b,
                                        TRILL__INGRESS_RBRIDGE_NICKNAMEf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_a,
                                        TRILL__TREE_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_b,
                                        TRILL__TREE_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;
        case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN:
            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_a,
                                        FCOE__MASKED_D_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_b,
                                        FCOE__MASKED_D_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_a,
                                        FCOE__VRF_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_b,
                                        FCOE__VRF_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;
        case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN_EXT:
            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_a,
                                        KEY_TYPE_1f);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_b,
                                        KEY_TYPE_1f);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_a,
                                        FCOE_EXT__MASKED_D_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_b,
                                        FCOE_EXT__MASKED_D_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_a,
                                        FCOE_EXT__VRF_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_b,
                                        FCOE_EXT__VRF_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;
        case TD2_L3_HASH_KEY_TYPE_FCOE_HOST:
            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_a,
                                        FCOE__D_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_b,
                                        FCOE__D_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_a,
                                        FCOE__VRF_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_b,
                                        FCOE__VRF_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;
        case TD2_L3_HASH_KEY_TYPE_FCOE_HOST_EXT:
            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_a,
                                        KEY_TYPE_1f);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_b,
                                        KEY_TYPE_1f);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm,  ent_a,
                                        FCOE_EXT__D_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm,  ent_b,
                                        FCOE_EXT__D_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm,  ent_a,
                                        FCOE_EXT__VRF_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm,  ent_b,
                                        FCOE_EXT__VRF_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;
        case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP:
            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_a,
                                        FCOE__S_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_b,
                                        FCOE__S_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_a,
                                        FCOE__VRF_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_UNICASTm, ent_b,
                                        FCOE__VRF_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;
        case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP_EXT:
            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_a,
                                        KEY_TYPE_1f);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_b,
                                        KEY_TYPE_1f);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm,  ent_a,
                                        FCOE_EXT__S_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm,  ent_b,
                                        FCOE_EXT__S_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm,  ent_a,
                                        FCOE_EXT__VRF_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm,  ent_b,
                                        FCOE_EXT__VRF_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;
        case TD2_L3_HASH_KEY_TYPE_DST_NAT:
            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_a,
                                        KEY_TYPE_1f);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_b,
                                        KEY_TYPE_1f);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_a,
                                        NAT__VRF_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_b,
                                        NAT__VRF_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_a,
                                        NAT__IP_ADDRf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_b,
                                        NAT__IP_ADDRf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;
        case TD2_L3_HASH_KEY_TYPE_DST_NAPT:
            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_a,
                                        KEY_TYPE_1f);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_b,
                                        KEY_TYPE_1f);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_a,
                                        NAT__VRF_IDf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_b,
                                        NAT__VRF_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_a,
                                        NAT__IP_ADDRf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_b,
                                        NAT__IP_ADDRf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_a,
                                        NAT__L4_DEST_PORTf);
            val_b = soc_mem_field32_get(unit, L3_ENTRY_IPV4_MULTICASTm, ent_b,
                                        NAT__L4_DEST_PORTf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;
        default:
            return 1;
        }
    } else
#endif /* BCM_TRIDENT2_SUPPORT */
    {
    switch (val_a) {
    case 0: /* IPV4_UNICAST */
        return _soc_mem_cmp_l3x2_ip4ucast(unit, ent_a, ent_b);
    case 1: /* IPV4_MULTICAST */
        return _soc_mem_cmp_l3x2_ip4mcast(unit, ent_a, ent_b);
    case 2: /* IPV6_UNICAST */
        return _soc_mem_cmp_l3x2_ip6ucast(unit, ent_a, ent_b);
    case 3: /* IPV6_MULTICAST */
        return _soc_mem_cmp_l3x2_ip6mcast(unit, ent_a, ent_b);
    case 4: /* CCM_LMEP */
        val_a = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_a, LMEP__SGLPf);
        val_b = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_b, LMEP__SGLPf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        val_a = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_a, LMEP__VIDf);
        val_b = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_b, LMEP__VIDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        return 0;
    case 5: /* CCM_RMEP */
        val_a = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_a, RMEP__SGLPf);
        val_b = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_b, RMEP__SGLPf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        val_a = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_a, RMEP__VIDf);
        val_b = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_b, RMEP__VIDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        val_a = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_a, RMEP__MDLf);
        val_b = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_b, RMEP__MDLf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        val_a = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_a, RMEP__MEPIDf);
        val_b = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_b, RMEP__MEPIDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        return 0;
    case 6: /* TRILL */
        val_a = soc_L3_ENTRY_ONLYm_field32_get
            (unit, ent_a, TRILL__INGRESS_RBRIDGE_NICKNAMEf);
        val_b = soc_L3_ENTRY_ONLYm_field32_get
            (unit, ent_b, TRILL__INGRESS_RBRIDGE_NICKNAMEf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        val_a = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_a, TRILL__TREE_IDf);
        val_b = soc_L3_ENTRY_ONLYm_field32_get(unit, ent_b, TRILL__TREE_IDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        return 0;
    default:
        return 1;
    }
    }
}

int
_soc_mem_cmp_l3x2_ip4ucast(int unit, void *ent_a, void *ent_b)
{
    uint32      type_a, type_b;
    ip_addr_t    ip_a, ip_b;

    if (SOC_MEM_FIELD_VALID(unit, L3_ENTRY_IPV4_UNICASTm, VRF_IDf)) {
        type_a = soc_L3_ENTRY_IPV4_UNICASTm_field32_get(unit, ent_a, VRF_IDf);
        type_b = soc_L3_ENTRY_IPV4_UNICASTm_field32_get(unit, ent_b, VRF_IDf);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }

    if (SOC_MEM_FIELD_VALID(unit, L3_ENTRY_IPV4_UNICASTm, KEY_TYPEf)) {
        type_a = soc_L3_ENTRY_IPV4_UNICASTm_field32_get(unit, ent_a, KEY_TYPEf);
        type_b = soc_L3_ENTRY_IPV4_UNICASTm_field32_get(unit, ent_b, KEY_TYPEf);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    } else {
        type_a = soc_L3_ENTRY_IPV4_UNICASTm_field32_get(unit, ent_a, V6f);
        type_b = soc_L3_ENTRY_IPV4_UNICASTm_field32_get(unit, ent_b, V6f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);

        type_a = soc_L3_ENTRY_IPV4_UNICASTm_field32_get(unit, ent_a, IPMCf);
        type_b = soc_L3_ENTRY_IPV4_UNICASTm_field32_get(unit, ent_b, IPMCf);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }

    ip_a = soc_L3_ENTRY_IPV4_UNICASTm_field32_get(unit, ent_a, IP_ADDRf);
    ip_b = soc_L3_ENTRY_IPV4_UNICASTm_field32_get(unit, ent_b, IP_ADDRf);
    SOC_MEM_COMPARE_RETURN(ip_a, ip_b);

    return(0);
}

int
_soc_mem_cmp_l3x2_ip4mcast(int unit, void *ent_a, void *ent_b)
{
    uint32      type_a, type_b;
    ip_addr_t    a, b;
    vlan_id_t    vlan_a, vlan_b;

    if (SOC_MEM_FIELD_VALID(unit, L3_ENTRY_IPV4_MULTICASTm, VRF_IDf)) {
        type_a = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_a, VRF_IDf);
        type_b = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_b, VRF_IDf);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }
#if defined(BCM_TRX_SUPPORT)
    if (SOC_MEM_FIELD_VALID(unit, L3_ENTRY_IPV4_MULTICASTm, KEY_TYPE_0f)) {
        type_a = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_a, KEY_TYPE_0f);
        type_b = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_b, KEY_TYPE_0f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);

        type_a = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_a, KEY_TYPE_1f);
        type_b = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_b, KEY_TYPE_1f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    } else
#endif /* BCM_TRX_SUPPORT */
    {
        type_a = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_a, V6f);
        type_b = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_b, V6f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);

        type_a = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_a, IPMCf);
        type_b = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_b, IPMCf);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }

    a = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_a, SOURCE_IP_ADDRf);
    b = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_b, SOURCE_IP_ADDRf);
    SOC_MEM_COMPARE_RETURN(a, b);

    a = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_a, GROUP_IP_ADDRf);
    b = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_b, GROUP_IP_ADDRf);
    SOC_MEM_COMPARE_RETURN(a, b);

    vlan_a = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_a, VLAN_IDf);
    vlan_b = soc_L3_ENTRY_IPV4_MULTICASTm_field32_get(unit, ent_b, VLAN_IDf);
    SOC_MEM_COMPARE_RETURN(vlan_a, vlan_b);

    return(0);
}

int
_soc_mem_cmp_l3x2_ip6ucast(int unit, void *ent_a, void *ent_b)
{
    uint32      type_a, type_b;
    uint32      a[SOC_MAX_MEM_FIELD_WORDS];
    uint32      b[SOC_MAX_MEM_FIELD_WORDS];
    int         i;
    int         ent_words;

#ifdef BCM_TRIUMPH_SUPPORT
    if (SOC_MEM_FIELD_VALID(unit, L3_ENTRY_IPV6_UNICASTm, VRF_IDf)) {
        type_a =
            soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_a, VRF_IDf);
        type_b =
            soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_b, VRF_IDf);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    } else
#endif
    if (SOC_MEM_FIELD_VALID(unit, L3_ENTRY_IPV6_UNICASTm, VRF_ID_0f)) {
        type_a =
            soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_a, VRF_ID_0f);
        type_b =
            soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_b, VRF_ID_0f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }
#if defined(BCM_TRX_SUPPORT)
    if (SOC_MEM_FIELD_VALID(unit, L3_ENTRY_IPV6_UNICASTm, KEY_TYPE_0f)) {
        type_a = soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_a, KEY_TYPE_0f);
        type_b = soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_b, KEY_TYPE_0f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    } else
#endif /* BCM_TRX_SUPPORT */
    {
        type_a = soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_a, V6_0f);
        type_b = soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_b, V6_0f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);

        type_a = soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_a, IPMC_0f);
        type_b = soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_b, IPMC_0f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }

    if (SOC_MEM_FIELD_VALID(unit, L3_ENTRY_IPV6_UNICASTm, VRF_ID_1f)) {
        type_a =
            soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_a, VRF_ID_1f);
        type_b =
            soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_b, VRF_ID_1f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }

#if defined(BCM_TRX_SUPPORT)
    if (SOC_MEM_FIELD_VALID(unit, L3_ENTRY_IPV6_UNICASTm, KEY_TYPE_1f)) {
        type_a = soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_a, KEY_TYPE_1f);
        type_b = soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_b, KEY_TYPE_1f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    } else
#endif /* BCM_TRX_SUPPORT */
    {
        type_a = soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_a, V6_1f);
        type_b = soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_b, V6_1f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);

        type_a = soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_a, IPMC_1f);
        type_b = soc_L3_ENTRY_IPV6_UNICASTm_field32_get(unit, ent_b, IPMC_1f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }

    soc_mem_field_get(unit, L3_ENTRY_IPV6_UNICASTm, ent_a, IP_ADDR_UPR_64f, a);
    soc_mem_field_get(unit, L3_ENTRY_IPV6_UNICASTm, ent_b, IP_ADDR_UPR_64f, b);
    ent_words = soc_mem_field_length(unit, L3_ENTRY_IPV6_UNICASTm,
                                     IP_ADDR_UPR_64f) / 32;
    for (i = ent_words - 1; i >= 0; i--) {
        SOC_MEM_COMPARE_RETURN(a[i], b[i]);
    }
    soc_mem_field_get(unit, L3_ENTRY_IPV6_UNICASTm, ent_a, IP_ADDR_LWR_64f, a);
    soc_mem_field_get(unit, L3_ENTRY_IPV6_UNICASTm, ent_b, IP_ADDR_LWR_64f, b);
    ent_words = soc_mem_field_length(unit, L3_ENTRY_IPV6_UNICASTm,
                                     IP_ADDR_LWR_64f) / 32;
    for (i = ent_words - 1; i >= 0; i--) {
        SOC_MEM_COMPARE_RETURN(a[i], b[i]);
    }

    return(0);
}

int
_soc_mem_cmp_l3x2_ip6mcast(int u, void *e_a, void *e_b)
{
    uint32      type_a, type_b;
    vlan_id_t    vlan_a, vlan_b;
    uint32      a[SOC_MAX_MEM_FIELD_WORDS];
    uint32      b[SOC_MAX_MEM_FIELD_WORDS];
    int         i;
    int         ent_words;

#ifdef BCM_TRIUMPH_SUPPORT
    if (SOC_MEM_FIELD_VALID(u, L3_ENTRY_IPV6_MULTICASTm, VRF_IDf)) {
        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, VRF_IDf);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, VRF_IDf);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    } else
#endif
    if (SOC_MEM_FIELD_VALID(u, L3_ENTRY_IPV6_MULTICASTm, VRF_ID_0f)) {
        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, VRF_ID_0f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, VRF_ID_0f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }
#if defined(BCM_TRX_SUPPORT)
    if (SOC_MEM_FIELD_VALID(u, L3_ENTRY_IPV6_MULTICASTm, KEY_TYPE_0f)) {
        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, KEY_TYPE_0f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, KEY_TYPE_0f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    } else
#endif /* BCM_TRX_SUPPORT */
    {
        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, V6_0f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, V6_0f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);

        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, IPMC_0f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, IPMC_0f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }

    if (SOC_MEM_FIELD_VALID(u, L3_ENTRY_IPV6_MULTICASTm, VRF_ID_1f)) {
        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, VRF_ID_1f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, VRF_ID_1f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }

#if defined(BCM_TRX_SUPPORT)
    if (SOC_MEM_FIELD_VALID(u, L3_ENTRY_IPV6_MULTICASTm, KEY_TYPE_1f)) {
        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, KEY_TYPE_1f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, KEY_TYPE_1f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    } else
#endif /* BCM_TRX_SUPPORT */
    {
        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, V6_1f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, V6_1f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);

        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, IPMC_1f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, IPMC_1f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }

    if (SOC_MEM_FIELD_VALID(u, L3_ENTRY_IPV6_MULTICASTm, VRF_ID_2f)) {
        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, VRF_ID_2f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, VRF_ID_2f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }

#if defined(BCM_TRX_SUPPORT)
    if (SOC_MEM_FIELD_VALID(u, L3_ENTRY_IPV6_MULTICASTm, KEY_TYPE_2f)) {
        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, KEY_TYPE_2f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, KEY_TYPE_2f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    } else
#endif /* BCM_TRX_SUPPORT */
    {
        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, V6_2f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, V6_2f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);

        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, IPMC_2f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, IPMC_2f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }

    if (SOC_MEM_FIELD_VALID(u, L3_ENTRY_IPV6_MULTICASTm, VRF_ID_3f)) {
        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, VRF_ID_3f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, VRF_ID_3f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }

#if defined(BCM_TRX_SUPPORT)
    if (SOC_MEM_FIELD_VALID(u, L3_ENTRY_IPV6_MULTICASTm, KEY_TYPE_3f)) {
        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, KEY_TYPE_3f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, KEY_TYPE_3f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    } else
#endif /* BCM_TRX_SUPPORT */
    {
        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, V6_3f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, V6_3f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);

        type_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, IPMC_3f);
        type_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, IPMC_3f);
        SOC_MEM_COMPARE_RETURN(type_a, type_b);
    }

    soc_mem_field_get(u, L3_ENTRY_IPV6_MULTICASTm,
                      e_a, SOURCE_IP_ADDR_UPR_64f, a);
    soc_mem_field_get(u, L3_ENTRY_IPV6_MULTICASTm,
                      e_b, SOURCE_IP_ADDR_UPR_64f, b);
    ent_words = soc_mem_field_length(u, L3_ENTRY_IPV6_MULTICASTm,
                                     SOURCE_IP_ADDR_UPR_64f) / 32;
    for (i = ent_words - 1; i >= 0; i--) {
        SOC_MEM_COMPARE_RETURN(a[i], b[i]);
    }

    soc_mem_field_get(u, L3_ENTRY_IPV6_MULTICASTm,
                      e_a, SOURCE_IP_ADDR_LWR_64f, a);
    soc_mem_field_get(u, L3_ENTRY_IPV6_MULTICASTm,
                      e_b, SOURCE_IP_ADDR_LWR_64f, b);
    ent_words = soc_mem_field_length(u, L3_ENTRY_IPV6_MULTICASTm,
                                     SOURCE_IP_ADDR_LWR_64f) / 32;
    for (i = ent_words - 1; i >= 0; i--) {
        SOC_MEM_COMPARE_RETURN(a[i], b[i]);
    }

    soc_mem_field_get(u, L3_ENTRY_IPV6_MULTICASTm,
                      e_a, GROUP_IP_ADDR_UPR_56f, a);
    soc_mem_field_get(u, L3_ENTRY_IPV6_MULTICASTm,
                      e_b, GROUP_IP_ADDR_UPR_56f, b);
    ent_words = soc_mem_field_length(u, L3_ENTRY_IPV6_MULTICASTm,
                                     GROUP_IP_ADDR_UPR_56f) / 32;
    for (i = ent_words - 1; i >= 0; i--) {
        SOC_MEM_COMPARE_RETURN(a[i], b[i]);
    }

    soc_mem_field_get(u, L3_ENTRY_IPV6_MULTICASTm,
                      e_a, GROUP_IP_ADDR_LWR_64f, a);
    soc_mem_field_get(u, L3_ENTRY_IPV6_MULTICASTm,
                      e_b, GROUP_IP_ADDR_LWR_64f, b);
    ent_words = soc_mem_field_length(u, L3_ENTRY_IPV6_MULTICASTm,
                                     GROUP_IP_ADDR_LWR_64f) / 32;
    for (i = ent_words - 1; i >= 0; i--) {
        SOC_MEM_COMPARE_RETURN(a[i], b[i]);
    }

#ifdef BCM_TRIUMPH_SUPPORT
    if (SOC_MEM_FIELD_VALID(u, L3_ENTRY_IPV6_MULTICASTm, VLAN_IDf)) {
        vlan_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, VLAN_IDf);
        vlan_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, VLAN_IDf);
        SOC_MEM_COMPARE_RETURN(vlan_a, vlan_b);
    } else
#endif
    {
        vlan_a = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_a, VLAN_ID_0f);
        vlan_b = soc_L3_ENTRY_IPV6_MULTICASTm_field32_get(u, e_b, VLAN_ID_0f);
        SOC_MEM_COMPARE_RETURN(vlan_a, vlan_b);
    }

    return(0);
}

/*
 * Compares both mask and address. Not a masked compare.
 */
int
_soc_mem_cmp_lpm(int u, void *e_a, void *e_b)
{
    uint32      a;
    uint32      b;

    a = soc_mem_field32_get(u, L3_DEFIPm, e_a, VALID1f);
    b = soc_mem_field32_get(u, L3_DEFIPm, e_b, VALID1f);

    if (a && b) {
        a = soc_mem_field32_get(u, L3_DEFIPm, e_a, MASK1f);
        b = soc_mem_field32_get(u, L3_DEFIPm, e_b, MASK1f);
        SOC_MEM_COMPARE_RETURN(a, b);

        a = soc_mem_field32_get(u, L3_DEFIPm, e_a, IP_ADDR1f);
        b = soc_mem_field32_get(u, L3_DEFIPm, e_b, IP_ADDR1f);
        SOC_MEM_COMPARE_RETURN(a, b);

        if (SOC_MEM_FIELD_VALID(u, L3_DEFIPm, VRF_ID_1f)) {
            a = soc_L3_DEFIPm_field32_get(u, e_a, VRF_ID_1f);
            b = soc_L3_DEFIPm_field32_get(u, e_b, VRF_ID_1f);
            SOC_MEM_COMPARE_RETURN(a, b);
        }

        a = soc_mem_field32_get(u, L3_DEFIPm, e_a, MODE1f);
        b = soc_mem_field32_get(u, L3_DEFIPm, e_b, MODE1f);
        SOC_MEM_COMPARE_RETURN(a, b);

        if (a == 0) {
            return(0); /* IPV4 entry */
        }
    }

    a = soc_mem_field32_get(u, L3_DEFIPm, e_a, VALID0f);
    b = soc_mem_field32_get(u, L3_DEFIPm, e_b, VALID0f);

    if (a && b) {
        a = soc_mem_field32_get(u, L3_DEFIPm, e_a, MASK0f);
        b = soc_mem_field32_get(u, L3_DEFIPm, e_b, MASK0f);
        SOC_MEM_COMPARE_RETURN(a, b);

        a = soc_mem_field32_get(u, L3_DEFIPm, e_a, IP_ADDR0f);
        b = soc_mem_field32_get(u, L3_DEFIPm, e_b, IP_ADDR0f);
        SOC_MEM_COMPARE_RETURN(a, b);

        if (SOC_MEM_FIELD_VALID(u, L3_DEFIPm, VRF_ID_0f)) {
            a = soc_L3_DEFIPm_field32_get(u, e_a, VRF_ID_0f);
            b = soc_L3_DEFIPm_field32_get(u, e_b, VRF_ID_0f);
            SOC_MEM_COMPARE_RETURN(a, b);
        }

        a = soc_mem_field32_get(u, L3_DEFIPm, e_a, MODE0f);
        b = soc_mem_field32_get(u, L3_DEFIPm, e_b, MODE0f);
        SOC_MEM_COMPARE_RETURN(a, b);
        if (a == 0) {
            return(0); /* IPV4 entry */
        }
    }

    a = soc_mem_field32_get(u, L3_DEFIPm, e_a, VALID1f);
    b = soc_mem_field32_get(u, L3_DEFIPm, e_b, VALID1f);
    SOC_MEM_COMPARE_RETURN(a, b);
    a = soc_mem_field32_get(u, L3_DEFIPm, e_a, VALID0f);
    b = soc_mem_field32_get(u, L3_DEFIPm, e_b, VALID0f);
    SOC_MEM_COMPARE_RETURN(a, b);

    return (0);
}

#endif /* BCM_FIREBOLT_SUPPORT */

#ifdef BCM_XGS3_SWITCH_SUPPORT
int
_soc_mem_cmp_vlan_mac(int unit, void *ent_a, void *ent_b)
{
    sal_mac_addr_t mac_a, mac_b;

    soc_VLAN_MACm_mac_addr_get(unit, ent_a, MAC_ADDRf, mac_a);
    soc_VLAN_MACm_mac_addr_get(unit, ent_b, MAC_ADDRf, mac_b);

    return ENET_CMP_MACADDR(mac_a, mac_b);
}
#endif /* BCM_XGS3_SWITCH_SUPPORT */

#ifdef BCM_TRX_SUPPORT
int
_soc_mem_cmp_vlan_mac_tr(int unit, void *ent_a, void *ent_b)
{
    sal_mac_addr_t mac_a, mac_b;
    uint32      type_a, type_b;
#ifdef BCM_TRIDENT2_SUPPORT
    uint32 key_a[SOC_MAX_MEM_FIELD_WORDS], key_b[SOC_MAX_MEM_FIELD_WORDS];
    sal_memset(key_a, 0, sizeof(key_a));
    sal_memset(key_b, 0, sizeof(key_b));
#endif
    type_a = soc_VLAN_MACm_field32_get(unit, ent_a, KEY_TYPEf);
    type_b = soc_VLAN_MACm_field32_get(unit, ent_b, KEY_TYPEf);
    SOC_MEM_COMPARE_RETURN(type_a, type_b);

    switch (type_a) {
    case 3: /* VLAN_MAC */
#ifdef BCM_TRIDENT2_SUPPORT
        if (SOC_IS_TD2_TT2(unit)) {
            soc_mem_field_get(unit, VLAN_MACm, ent_a, MAC__KEYf, key_a);
            soc_mem_field_get(unit, VLAN_MACm, ent_b, MAC__KEYf, key_b);
            return sal_memcmp(key_a, key_b, (sizeof(key_a)));
        } else
#endif
        {
            soc_VLAN_MACm_mac_addr_get(unit, ent_a, MAC_ADDRf, mac_a);
            soc_VLAN_MACm_mac_addr_get(unit, ent_b, MAC_ADDRf, mac_b);
            return ENET_CMP_MACADDR(mac_a, mac_b);
        }
#ifdef BCM_TRIUMPH2_SUPPORT
    case 7: /* HPAE (MAC_IP_BIND) */
#ifdef BCM_TRIDENT2_SUPPORT
        if (SOC_IS_TD2_TT2(unit)) {
            soc_mem_field_get(unit, VLAN_MACm, ent_a, MAC_IP_BIND__KEYf, key_a);
            soc_mem_field_get(unit, VLAN_MACm, ent_b, MAC_IP_BIND__KEYf, key_b);
            return sal_memcmp(key_a, key_b, (sizeof(key_a)));
        } else
#endif
        {
            type_a = soc_VLAN_MACm_field32_get(unit, ent_a, MAC_IP_BIND__SIPf);
            type_b = soc_VLAN_MACm_field32_get(unit, ent_b, MAC_IP_BIND__SIPf);
            SOC_MEM_COMPARE_RETURN(type_a, type_b);

            return 0;
        }
#endif /* BCM_TRIUMPH2_SUPPORT */
#ifdef BCM_TRIDENT2_SUPPORT
    case 14:
        if (SOC_IS_TD2_TT2(unit)) {
            sal_memset(key_a, 0, sizeof(key_a));
            sal_memset(key_b, 0, sizeof(key_b));
            soc_mem_field_get(unit, VLAN_MACm, ent_a, MAC_PORT__KEYf, key_a);
            soc_mem_field_get(unit, VLAN_MACm, ent_b, MAC_PORT__KEYf, key_b);
            return sal_memcmp(key_a, key_b, (sizeof(key_a)));
        }
#endif
    default:
        return 1;
    }
}

int
_soc_mem_cmp_vlan_xlate_tr(int unit, void *ent_a, void *ent_b)
{
    uint32 val_a, val_b;
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
    uint32 key_a[SOC_MAX_MEM_FIELD_WORDS], key_b[SOC_MAX_MEM_FIELD_WORDS];
    sal_memset(key_a, 0, sizeof(key_a));
    sal_memset(key_b, 0, sizeof(key_b));
#endif
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (SOC_IS_TRIUMPH3(unit)) {
        soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_a, VALID_0f, &val_a);
        soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_b, VALID_0f, &val_b);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_a, KEY_TYPE_0f, &val_a);
        soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_b, KEY_TYPE_0f, &val_b);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        switch (val_a) {
            case TR3_VLXLT_X_HASH_KEY_TYPE_IVID_OVID:
            case TR3_VLXLT_X_HASH_KEY_TYPE_OVID:
            case TR3_VLXLT_X_HASH_KEY_TYPE_IVID:
            case TR3_VLXLT_X_HASH_KEY_TYPE_OTAG:
            case TR3_VLXLT_X_HASH_KEY_TYPE_ITAG:
            case TR3_VLXLT_X_HASH_KEY_TYPE_PRI_CFI:
            case TR3_VLXLT_X_HASH_KEY_TYPE_IVID_OVID_SVP:
            case TR3_VLXLT_X_HASH_KEY_TYPE_OVID_SVP:
                soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_a, VALID_1f, &val_a);
                soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_b, VALID_1f, &val_b);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_a, KEY_TYPE_1f, &val_a);
                soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_b, KEY_TYPE_1f, &val_b);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_a, XLATE__KEY_0f, key_a);
                soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_b, XLATE__KEY_0f, key_b);
                return sal_memcmp(key_a, key_b, (sizeof(key_a)));
            case TR3_VLXLT_HASH_KEY_TYPE_IVID_OVID:
            case TR3_VLXLT_HASH_KEY_TYPE_OVID:
            case TR3_VLXLT_HASH_KEY_TYPE_IVID:
            case TR3_VLXLT_HASH_KEY_TYPE_OTAG:
            case TR3_VLXLT_HASH_KEY_TYPE_ITAG:
            case TR3_VLXLT_HASH_KEY_TYPE_PRI_CFI:
            case TR3_VLXLT_HASH_KEY_TYPE_IVID_OVID_SVP:
            case TR3_VLXLT_HASH_KEY_TYPE_OVID_SVP:
                soc_mem_field_get(unit, VLAN_XLATE_1m, ent_a, XLATE__KEYf, key_a);
                soc_mem_field_get(unit, VLAN_XLATE_1m, ent_b, XLATE__KEYf, key_b);
                return sal_memcmp(key_a, key_b, (sizeof(key_a)));
            case TR3_VLXLT_HASH_KEY_TYPE_VLAN_MAC:
                soc_mem_field_get(unit, VLAN_XLATE_1m, ent_a, VLAN_MAC__KEYf, key_a);
                soc_mem_field_get(unit, VLAN_XLATE_1m, ent_b, VLAN_MAC__KEYf, key_b);
                return sal_memcmp(key_a, key_b, (sizeof(key_a)));
            case TR3_VLXLT_HASH_KEY_TYPE_VIF:
            case TR3_VLXLT_HASH_KEY_TYPE_VIF_VLAN:
            case TR3_VLXLT_HASH_KEY_TYPE_VIF_CVLAN:
            case TR3_VLXLT_HASH_KEY_TYPE_VIF_OTAG:
            case TR3_VLXLT_HASH_KEY_TYPE_VIF_ITAG:
                soc_mem_field_get(unit, VLAN_XLATE_1m, ent_a, VIF__KEYf, key_a);
                soc_mem_field_get(unit, VLAN_XLATE_1m, ent_b, VIF__KEYf, key_b);
                return sal_memcmp(key_a, key_b, (sizeof(key_a)));
            case TR3_VLXLT_HASH_KEY_TYPE_L2GRE_DIP:
                soc_mem_field_get(unit, VLAN_XLATE_1m, ent_a, L2GRE_DIP__KEYf, key_a);
                soc_mem_field_get(unit, VLAN_XLATE_1m, ent_b, L2GRE_DIP__KEYf, key_b);
                return sal_memcmp(key_a, key_b, (sizeof(key_a)));
            case TR3_VLXLT_HASH_KEY_TYPE_HPAE:
                soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_a, VALID_1f, &val_a);
                soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_b, VALID_1f, &val_b);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_a, KEY_TYPE_1f, &val_a);
                soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_b, KEY_TYPE_1f, &val_b);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_a, MAC_IP_BIND__KEY_0f, key_a);
                soc_mem_field_get(unit, VLAN_XLATE_EXTDm, ent_b, MAC_IP_BIND__KEY_0f, key_b);
                return sal_memcmp(key_a, key_b, (sizeof(key_a)));
            default:
                return 1;
        }
    }
#endif
    val_a = soc_VLAN_XLATEm_field32_get(unit, ent_a, KEY_TYPEf);
    val_b = soc_VLAN_XLATEm_field32_get(unit, ent_b, KEY_TYPEf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);
#if defined(BCM_TRIDENT2_SUPPORT)
    if (SOC_IS_TD2_TT2(unit)) {
        switch (val_a) {
        case TD2_VLXLT_HASH_KEY_TYPE_IVID_OVID:
        case TD2_VLXLT_HASH_KEY_TYPE_OTAG:
        case TD2_VLXLT_HASH_KEY_TYPE_ITAG:
        case TD2_VLXLT_HASH_KEY_TYPE_OVID:
        case TD2_VLXLT_HASH_KEY_TYPE_IVID:
        case TD2_VLXLT_HASH_KEY_TYPE_PRI_CFI:
        case TD2_VLXLT_HASH_KEY_TYPE_IVID_OVID_VSAN:
        case TD2_VLXLT_HASH_KEY_TYPE_IVID_VSAN:
        case TD2_VLXLT_HASH_KEY_TYPE_OVID_VSAN:
            soc_mem_field_get(unit, VLAN_XLATEm, ent_a, XLATE__KEYf, key_a);
            soc_mem_field_get(unit, VLAN_XLATEm, ent_b, XLATE__KEYf, key_b);
            return sal_memcmp(key_a, key_b, (sizeof(key_a)));

        case TD2_VLXLT_HASH_KEY_TYPE_VIF:
        case TD2_VLXLT_HASH_KEY_TYPE_VIF_VLAN:
        case TD2_VLXLT_HASH_KEY_TYPE_VIF_CVLAN:
        case TD2_VLXLT_HASH_KEY_TYPE_VIF_OTAG:
        case TD2_VLXLT_HASH_KEY_TYPE_VIF_ITAG:
            soc_mem_field_get(unit, VLAN_XLATEm, ent_a, VIF__KEYf, key_a);
            soc_mem_field_get(unit, VLAN_XLATEm, ent_b, VIF__KEYf, key_b);
            return sal_memcmp(key_a, key_b, (sizeof(key_a)));

        case TD2_VLXLT_HASH_KEY_TYPE_L2GRE_DIP:
            soc_mem_field_get(unit, VLAN_XLATEm, ent_a, L2GRE_DIP__KEYf, key_a);
            soc_mem_field_get(unit, VLAN_XLATEm, ent_b, L2GRE_DIP__KEYf, key_b);
            return sal_memcmp(key_a, key_b, (sizeof(key_a)));

        case TD2_VLXLT_HASH_KEY_TYPE_VXLAN_DIP:
            soc_mem_field_get(unit, VLAN_XLATEm, ent_a, VXLAN_DIP__KEYf, key_a);
            soc_mem_field_get(unit, VLAN_XLATEm, ent_b, VXLAN_DIP__KEYf, key_b);
            return sal_memcmp(key_a, key_b, (sizeof(key_a)));

        /* VLAN_MAC table */
        case TD2_VLXLT_HASH_KEY_TYPE_VLAN_MAC:
        case TD2_VLXLT_HASH_KEY_TYPE_HPAE:
        case TD2_VLXLT_HASH_KEY_TYPE_VLAN_MAC_PORT:
            return _soc_mem_cmp_vlan_mac_tr(unit, ent_a, ent_b);

        default:
            return 1;
        }
    } else
#endif
    {
        switch (val_a) {
        case 3: /* VLAN_MAC */
        case 7: /* HPAE (MAC_IP_BIND) */
            return _soc_mem_cmp_vlan_mac_tr(unit, ent_a, ent_b);

        case 0: /* IVID_OVID */
        case 1: /* OTAG */
        case 2: /* ITAG */
        case 4: /* OVID */
        case 5: /* IVID */
        case 6: /* PRI_CFI */
            val_a = soc_VLAN_XLATEm_field32_get(unit, ent_a, GLPf);
            val_b = soc_VLAN_XLATEm_field32_get(unit, ent_b, GLPf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_VLAN_XLATEm_field32_get(unit, ent_a, INCOMING_VIDSf);
            val_b = soc_VLAN_XLATEm_field32_get(unit, ent_b, INCOMING_VIDSf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;

        case 9: /* VIF_VLAN */
            val_a = soc_VLAN_XLATEm_field32_get(unit, ent_a, VIF__VLANf);
            val_b = soc_VLAN_XLATEm_field32_get(unit, ent_b, VIF__VLANf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);
            /* fall through to case for VIF */

        case 8: /* VIF */
            val_a = soc_VLAN_XLATEm_field32_get(unit, ent_a, VIF__GLPf);
            val_b = soc_VLAN_XLATEm_field32_get(unit, ent_b, VIF__GLPf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_VLAN_XLATEm_field32_get(unit, ent_a, VIF__SRC_VIFf);
            val_b = soc_VLAN_XLATEm_field32_get(unit, ent_b, VIF__SRC_VIFf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;

        default:
            return 1;
        }
    }
}

int
_soc_mem_cmp_egr_vlan_xlate_tr(int unit, void *ent_a, void *ent_b)
{
    sal_mac_addr_t mac_a, mac_b;
    uint32 val_a, val_b;
#if defined(BCM_TRIDENT2_SUPPORT)
    uint32 key_a[SOC_MAX_MEM_FIELD_WORDS], key_b[SOC_MAX_MEM_FIELD_WORDS];
    sal_memset(key_a, 0, sizeof(key_a));
    sal_memset(key_b, 0, sizeof(key_b));
#endif
    if (SOC_MEM_FIELD_VALID(unit, EGR_VLAN_XLATEm, ENTRY_TYPEf)) {
        val_a = soc_EGR_VLAN_XLATEm_field32_get(unit, ent_a, ENTRY_TYPEf);
        val_b = soc_EGR_VLAN_XLATEm_field32_get(unit, ent_b, ENTRY_TYPEf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        switch (val_a) {
        case 0: /* VLAN_XLATE */
        case 1: /* VLAN_XLATE_DVP */
        case 2: /* VLAN_XLATE_WLAN */
#if defined(BCM_TRIDENT2_SUPPORT)
            if (SOC_IS_TD2_TT2(unit)) {
                soc_mem_field_get(unit, EGR_VLAN_XLATEm, ent_a, XLATE__KEYf, key_a);
                soc_mem_field_get(unit, EGR_VLAN_XLATEm, ent_b, XLATE__KEYf, key_b);
                return sal_memcmp(key_a, key_b, (sizeof(key_a)));
            } else
#endif
            if (SOC_IS_TD_TT(unit)) {
                val_a = soc_EGR_VLAN_XLATEm_field32_get
                    (unit, ent_a, DST_MODIDf);
                val_b = soc_EGR_VLAN_XLATEm_field32_get
                    (unit, ent_b, DST_MODIDf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                val_a = soc_EGR_VLAN_XLATEm_field32_get
                    (unit, ent_a, DST_PORTf);
                val_b = soc_EGR_VLAN_XLATEm_field32_get
                    (unit, ent_b, DST_PORTf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);
            } else if (SOC_MEM_FIELD_VALID(unit, EGR_VLAN_XLATEm, DVPf)) {
                val_a = soc_EGR_VLAN_XLATEm_field32_get(unit, ent_a, DVPf);
                val_b = soc_EGR_VLAN_XLATEm_field32_get(unit, ent_b, DVPf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);
            } else {
                val_a = soc_EGR_VLAN_XLATEm_field32_get
                    (unit, ent_a, PORT_GROUP_IDf);
                val_b = soc_EGR_VLAN_XLATEm_field32_get
                    (unit, ent_b, PORT_GROUP_IDf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);
            }

            val_a = soc_EGR_VLAN_XLATEm_field32_get(unit, ent_a, OVIDf);
            val_b = soc_EGR_VLAN_XLATEm_field32_get(unit, ent_b, OVIDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_EGR_VLAN_XLATEm_field32_get(unit, ent_a, IVIDf);
            val_b = soc_EGR_VLAN_XLATEm_field32_get(unit, ent_b, IVIDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;

        case 3: /* ISID_XLATE */
        case 4: /* ISID_DVP_XLATE */
            val_a = soc_EGR_VLAN_XLATEm_field32_get
                (unit, ent_a, MIM_ISID__VFIf);
            val_b = soc_EGR_VLAN_XLATEm_field32_get
                (unit, ent_b, MIM_ISID__VFIf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_EGR_VLAN_XLATEm_field32_get
                (unit, ent_a, MIM_ISID__DVPf);
            val_b = soc_EGR_VLAN_XLATEm_field32_get
                (unit, ent_b, MIM_ISID__DVPf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;

        case 5: /* WLAN_SVP_TUNNEL */
        case 6: /* WLAN_SVP_BSSID */
#if defined(BCM_TRIDENT2_SUPPORT)
            if (SOC_IS_TD2_TT2(unit)) {
                soc_mem_field_get(unit, EGR_VLAN_XLATEm, ent_a, L2GRE_VFI__KEYf, key_a);
                soc_mem_field_get(unit, EGR_VLAN_XLATEm, ent_b, L2GRE_VFI__KEYf, key_b);
                return sal_memcmp(key_a, key_b, (sizeof(key_a)));
            } else
#endif
            {
                val_a = soc_EGR_VLAN_XLATEm_field32_get
                    (unit, ent_a, WLAN_SVP__RIDf);
                val_b = soc_EGR_VLAN_XLATEm_field32_get
                    (unit, ent_b, WLAN_SVP__RIDf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                soc_mem_mac_addr_get
                    (unit, EGR_VLAN_XLATEm, ent_a, WLAN_SVP__BSSIDf, mac_a);
                soc_mem_mac_addr_get
                    (unit, EGR_VLAN_XLATEm, ent_b, WLAN_SVP__BSSIDf, mac_b);
                return ENET_CMP_MACADDR(mac_a, mac_b);
            }
        case 7: /* WLAN_SVP_BSSID_RID */
#if defined(BCM_TRIDENT2_SUPPORT)
            if (SOC_IS_TD2_TT2(unit)) {
                soc_mem_field_get(unit, EGR_VLAN_XLATEm, ent_a, XLATE__KEYf, key_a);
                soc_mem_field_get(unit, EGR_VLAN_XLATEm, ent_b, XLATE__KEYf, key_b);
                return sal_memcmp(key_a, key_b, (sizeof(key_a)));
            } else
#endif
            {
                val_a = soc_EGR_VLAN_XLATEm_field32_get
                    (unit, ent_a, WLAN_SVP__RIDf);
                val_b = soc_EGR_VLAN_XLATEm_field32_get
                    (unit, ent_b, WLAN_SVP__RIDf);
                SOC_MEM_COMPARE_RETURN(val_a, val_b);

                soc_mem_mac_addr_get
                    (unit, EGR_VLAN_XLATEm, ent_a, WLAN_SVP__BSSIDf, mac_a);
                soc_mem_mac_addr_get
                    (unit, EGR_VLAN_XLATEm, ent_b, WLAN_SVP__BSSIDf, mac_b);
                return ENET_CMP_MACADDR(mac_a, mac_b);
            }
#ifdef BCM_TRIDENT2_SUPPORT
      case 8:
      case 9:
          if (SOC_IS_TD2_TT2(unit)) {
              soc_mem_field_get(unit, EGR_VLAN_XLATEm, ent_a, VXLAN_VFI__KEYf, key_a);
              soc_mem_field_get(unit, EGR_VLAN_XLATEm, ent_b, VXLAN_VFI__KEYf, key_b);
              return sal_memcmp(key_a, key_b, (sizeof(key_a)));
          }
#endif
        default:
            return 1;
        }
    }

    val_a = soc_EGR_VLAN_XLATEm_field32_get(unit, ent_a, PORT_GROUP_IDf);
    val_b = soc_EGR_VLAN_XLATEm_field32_get(unit, ent_b, PORT_GROUP_IDf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    val_a = soc_EGR_VLAN_XLATEm_field32_get(unit, ent_a, IVIDf);
    val_b = soc_EGR_VLAN_XLATEm_field32_get(unit, ent_b, IVIDf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    val_a = soc_EGR_VLAN_XLATEm_field32_get(unit, ent_a, OVIDf);
    val_b = soc_EGR_VLAN_XLATEm_field32_get(unit, ent_b, OVIDf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    return 0;
}

#ifdef BCM_TRIUMPH_SUPPORT
int
_soc_mem_cmp_mpls_entry_tr(int unit, void *ent_a, void *ent_b)
{
    sal_mac_addr_t mac_a, mac_b;
    uint32 val_a, val_b;
#if defined(BCM_TRIDENT2_SUPPORT)
    uint32 key_a[SOC_MAX_MEM_FIELD_WORDS], key_b[SOC_MAX_MEM_FIELD_WORDS];
    sal_memset(key_a, 0, sizeof(key_a));
    sal_memset(key_b, 0, sizeof(key_b));
#endif
    if (SOC_MEM_FIELD_VALID(unit, MPLS_ENTRYm, KEY_TYPEf)) {
        val_a = soc_MPLS_ENTRYm_field32_get(unit, ent_a, KEY_TYPEf);
        val_b = soc_MPLS_ENTRYm_field32_get(unit, ent_b, KEY_TYPEf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        switch (val_a) {
        case 0: /* MPLS */
            val_a = soc_MPLS_ENTRYm_field32_get(unit, ent_a, PORT_NUMf);
            val_b = soc_MPLS_ENTRYm_field32_get(unit, ent_b, PORT_NUMf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_MPLS_ENTRYm_field32_get(unit, ent_a, MODULE_IDf);
            val_b = soc_MPLS_ENTRYm_field32_get(unit, ent_b, MODULE_IDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_MPLS_ENTRYm_field32_get(unit, ent_a, Tf);
            val_b = soc_MPLS_ENTRYm_field32_get(unit, ent_b, Tf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_MPLS_ENTRYm_field32_get(unit, ent_a, MPLS_LABELf);
            val_b = soc_MPLS_ENTRYm_field32_get(unit, ent_b, MPLS_LABELf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;
        case 1: /* MIM_NVP */
            val_a = soc_MPLS_ENTRYm_field32_get(unit, ent_a, MIM_NVP__BVIDf);
            val_b = soc_MPLS_ENTRYm_field32_get(unit, ent_b, MIM_NVP__BVIDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            soc_mem_mac_addr_get
                (unit, MPLS_ENTRYm, ent_a, MIM_NVP__BMACSAf, mac_a);
            soc_mem_mac_addr_get
                (unit, MPLS_ENTRYm, ent_b, MIM_NVP__BMACSAf, mac_b);
            return ENET_CMP_MACADDR(mac_a, mac_b);

        case 2: /* MIM_ISID */
            val_a = soc_MPLS_ENTRYm_field32_get(unit, ent_a, MIM_ISID__ISIDf);
            val_b = soc_MPLS_ENTRYm_field32_get(unit, ent_b, MIM_ISID__ISIDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;

        case 3: /* MIM_ISID_SVP */
            val_a = soc_MPLS_ENTRYm_field32_get(unit, ent_a, MIM_ISID__ISIDf);
            val_b = soc_MPLS_ENTRYm_field32_get(unit, ent_b, MIM_ISID__ISIDf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            val_a = soc_MPLS_ENTRYm_field32_get(unit, ent_a, MIM_ISID__SVPf);
            val_b = soc_MPLS_ENTRYm_field32_get(unit, ent_b, MIM_ISID__SVPf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;

        case 4: /* WLAN_MAC */
#if defined(BCM_TRIDENT2_SUPPORT)
            if (SOC_IS_TD2_TT2(unit)) {
                soc_mem_field_get(unit, MPLS_ENTRYm, ent_a, L2GRE_VPNID__KEYf, key_a);
                soc_mem_field_get(unit, MPLS_ENTRYm, ent_b, L2GRE_VPNID__KEYf, key_b);
                return sal_memcmp(key_a, key_b, (sizeof(key_a)));
            } else
#endif
            {
                soc_mem_mac_addr_get
                    (unit, MPLS_ENTRYm, ent_a, WLAN_MAC__MAC_ADDRf, mac_a);
                soc_mem_mac_addr_get
                    (unit, MPLS_ENTRYm, ent_b, WLAN_MAC__MAC_ADDRf, mac_b);
                return ENET_CMP_MACADDR(mac_a, mac_b);
            }

        case 5: /* TRILL */
            val_a = soc_MPLS_ENTRYm_field32_get
                (unit, ent_a, TRILL__RBRIDGE_NICKNAMEf);
            val_b = soc_MPLS_ENTRYm_field32_get
                (unit, ent_b, TRILL__RBRIDGE_NICKNAMEf);
            SOC_MEM_COMPARE_RETURN(val_a, val_b);

            return 0;
#if defined(BCM_TRIDENT2_SUPPORT)
        case 6:
            if (SOC_IS_TD2_TT2(unit)) {
                soc_mem_field_get(unit, MPLS_ENTRYm, ent_a, L2GRE_SIP__KEYf, key_a);
                soc_mem_field_get(unit, MPLS_ENTRYm, ent_b, L2GRE_SIP__KEYf, key_b);
                return sal_memcmp(key_a, key_b, (sizeof(key_a)));
            }
            return 1;
       case 7:
            if (SOC_IS_TD2_TT2(unit)) {
                soc_mem_field_get(unit, MPLS_ENTRYm, ent_a, L2GRE_VPNID__KEYf, key_a);
                soc_mem_field_get(unit, MPLS_ENTRYm, ent_b, L2GRE_VPNID__KEYf, key_b);
                return sal_memcmp(key_a, key_b, (sizeof(key_a)));
            }
            return 1;
       case 8:
            if (SOC_IS_TD2_TT2(unit)) {
                soc_mem_field_get(unit, MPLS_ENTRYm, ent_a, VXLAN_SIP__KEYf, key_a);
                soc_mem_field_get(unit, MPLS_ENTRYm, ent_b, VXLAN_SIP__KEYf, key_b);
                return sal_memcmp(key_a, key_b, (sizeof(key_a)));
            }
            return 1;
       case 9:
       case 10:
            if (SOC_IS_TD2_TT2(unit)) {
                soc_mem_field_get(unit, MPLS_ENTRYm, ent_a, VXLAN_VN_ID__KEYf, key_a);
                soc_mem_field_get(unit, MPLS_ENTRYm, ent_b, VXLAN_VN_ID__KEYf, key_b);
                return sal_memcmp(key_a, key_b, (sizeof(key_a)));
            }
            return 1;
#endif
        default:
            return 1;
        }
    }

    val_a = soc_MPLS_ENTRYm_field32_get(unit, ent_a, PORT_NUMf);
    val_b = soc_MPLS_ENTRYm_field32_get(unit, ent_b, PORT_NUMf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    val_a = soc_MPLS_ENTRYm_field32_get(unit, ent_a, MODULE_IDf);
    val_b = soc_MPLS_ENTRYm_field32_get(unit, ent_b, MODULE_IDf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    val_a = soc_MPLS_ENTRYm_field32_get(unit, ent_a, Tf);
    val_b = soc_MPLS_ENTRYm_field32_get(unit, ent_b, Tf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    val_a = soc_MPLS_ENTRYm_field32_get(unit, ent_a, MPLS_LABELf);
    val_b = soc_MPLS_ENTRYm_field32_get(unit, ent_b, MPLS_LABELf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    return 0;
}
#endif /* BCM_TRIUMPH_SUPPORT */

STATIC void
_soc_mem_write_range_cache_update(int unit,
                        soc_mem_t mem, unsigned array_index,
                        int copyno, int no_cache, 
                        int index_min, int index_max, void *buffer)
{
    uint32          *cache;
    uint32          entry_dw;
    int mem_array_vmap_offset, mem_array_cache_offset;
    int i;
    uint8 *vmap;


    entry_dw = soc_mem_entry_words(unit, mem);
    mem_array_vmap_offset = array_index * soc_mem_index_count(unit, mem);
    mem_array_cache_offset = mem_array_vmap_offset * entry_dw;

    cache = SOC_MEM_STATE(unit, mem).cache[copyno];
    if (cache != NULL && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
        sal_memcpy(cache + mem_array_cache_offset + index_min * entry_dw,
                buffer, (index_max - index_min + 1) * entry_dw * 4);

        vmap = SOC_MEM_STATE(unit, mem).vmap[copyno];
        for (i = index_min; i <= index_max; i++) {
            CACHE_VMAP_SET(vmap, mem_array_vmap_offset + i);
        }
   }

}


/*
 * Function: _soc_mem_aggr_cache_update
 *
 * Purpose:  If this mem view has several physical copies, 
 *               it needs to be updated accordingly.
 *
 * Returns:  None
 */
STATIC void
_soc_mem_aggr_cache_update(int unit, soc_mem_t mem, 
                                int blk, int no_cache, 
                                int index_min, int index_max,
                                int array_index, void *entry_data)
{
    
    /*
     * There are 3 physical copies of MODPORT_MAP.
     * Writing this table writes all three physical copies.
     * The copies are MODPORT_MAP_SW, MODPORT_MAP_IM, MODPORT_MAP_EM.
     */
    if (soc_mem_is_aggr(unit, mem) && mem == MODPORT_MAPm) {
        if (soc_mem_is_valid(unit, MODPORT_MAP_SWm)) {
            _soc_mem_write_range_cache_update(unit, MODPORT_MAP_SWm, 
                        array_index, blk, no_cache,
                        index_min, index_max, entry_data);
        }
        if (soc_mem_is_valid(unit, MODPORT_MAP_IMm)) {
            _soc_mem_write_range_cache_update(unit, MODPORT_MAP_IMm, 
                        array_index, blk, no_cache,
                        index_min, index_max, entry_data);
        }
        if (soc_mem_is_valid(unit, MODPORT_MAP_EMm)) {
            _soc_mem_write_range_cache_update(unit, MODPORT_MAP_EMm, 
                        array_index, blk, no_cache,
                        index_min, index_max, entry_data);
        }
    }

}


#endif /* BCM_TRX_SUPPORT */

#ifdef BCM_TRIDENT2_SUPPORT
int
_soc_mem_cmp_ing_vp_vlan_membership(int unit, void *ent_a, void *ent_b)
{
    uint32 val_a, val_b;

    val_a = soc_mem_field32_get(unit, ING_VP_VLAN_MEMBERSHIPm, ent_a, VLANf);
    val_b = soc_mem_field32_get(unit, ING_VP_VLAN_MEMBERSHIPm, ent_b, VLANf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    val_a = soc_mem_field32_get(unit, ING_VP_VLAN_MEMBERSHIPm, ent_a, VPf);
    val_b = soc_mem_field32_get(unit, ING_VP_VLAN_MEMBERSHIPm, ent_b, VPf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    return 0;
}

int
_soc_mem_cmp_egr_vp_vlan_membership(int unit, void *ent_a, void *ent_b)
{
    uint32 val_a, val_b;

    val_a = soc_mem_field32_get(unit, EGR_VP_VLAN_MEMBERSHIPm, ent_a, VLANf);
    val_b = soc_mem_field32_get(unit, EGR_VP_VLAN_MEMBERSHIPm, ent_b, VLANf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    val_a = soc_mem_field32_get(unit, EGR_VP_VLAN_MEMBERSHIPm, ent_a, VPf);
    val_b = soc_mem_field32_get(unit, EGR_VP_VLAN_MEMBERSHIPm, ent_b, VPf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    return 0;
}

int
_soc_mem_cmp_ing_dnat_address_type(int unit, void *ent_a, void *ent_b)
{
    uint32 val_a, val_b;

    val_a = soc_mem_field32_get(unit, ING_DNAT_ADDRESS_TYPEm, ent_a,
                                DEST_IPV4_ADDRf);
    val_b = soc_mem_field32_get(unit, ING_DNAT_ADDRESS_TYPEm, ent_b,
                                DEST_IPV4_ADDRf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    val_a = soc_mem_field32_get(unit, ING_DNAT_ADDRESS_TYPEm, ent_a, VRFf);
    val_b = soc_mem_field32_get(unit, ING_DNAT_ADDRESS_TYPEm, ent_b, VRFf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    return 0;
}

int
_soc_mem_cmp_l2_endpoint_id(int unit, void *ent_a, void *ent_b)
{
    sal_mac_addr_t mac_a, mac_b;
    uint32 val_a, val_b;

    val_a = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_a, KEY_TYPEf);
    val_b = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_b, KEY_TYPEf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    switch (val_a) {
    case TD2_L2_HASH_KEY_TYPE_BRIDGE:
        val_a = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_a,
                                    L2__VLAN_IDf);
        val_b = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_b,
                                    L2__VLAN_IDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        soc_mem_mac_addr_get(unit, L2_ENDPOINT_IDm, ent_a, L2__MAC_ADDRf,
                             mac_a);
        soc_mem_mac_addr_get(unit, L2_ENDPOINT_IDm, ent_b, L2__MAC_ADDRf,
                             mac_b);
        return ENET_CMP_MACADDR(mac_a, mac_b);

    case TD2_L2_HASH_KEY_TYPE_VFI:
        val_a = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_a, L2__VFIf);
        val_b = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_b, L2__VFIf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        soc_mem_mac_addr_get(unit, L2_ENDPOINT_IDm, ent_a, L2__MAC_ADDRf,
                             mac_a);
        soc_mem_mac_addr_get(unit, L2_ENDPOINT_IDm, ent_b, L2__MAC_ADDRf,
                             mac_b);
        return ENET_CMP_MACADDR(mac_a, mac_b);

    case TD2_L2_HASH_KEY_TYPE_VIF:
        val_a = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_a,
                                    VIF__NAMESPACEf);
        val_b = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_b,
                                    VIF__NAMESPACEf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        val_a = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_a,
                                    VIF__DST_VIFf);
        val_b = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_b,
                                    VIF__DST_VIFf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        val_a = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_a, VIF__Pf);
        val_b = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_b, VIF__Pf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        return 0;

    case TD2_L2_HASH_KEY_TYPE_PE_VID:
        val_a = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_a,
                                    PE_VID__NAMESPACEf);
        val_b = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_b,
                                    PE_VID__NAMESPACEf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        val_a = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_a,
                                    PE_VID__ETAG_VIDf);
        val_b = soc_mem_field32_get(unit, L2_ENDPOINT_IDm, ent_b,
                                    PE_VID__ETAG_VIDf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        return 0;

    default:
        return 1;
    }

    return 0;
}

int
_soc_mem_cmp_endpoint_queue_map(int unit, void *ent_a, void *ent_b)
{
    uint32 val_a, val_b;

    val_a = soc_mem_field32_get(unit, ENDPOINT_QUEUE_MAPm, ent_a, KEY_TYPEf);
    val_b = soc_mem_field32_get(unit, ENDPOINT_QUEUE_MAPm, ent_b, KEY_TYPEf);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    switch (val_a) {
    case 0: /* endpoint queueing */
        val_a = soc_mem_field32_get(unit, ENDPOINT_QUEUE_MAPm, ent_a,
                                    EH_QUEUE_TAGf);
        val_b = soc_mem_field32_get(unit, ENDPOINT_QUEUE_MAPm, ent_b,
                                    EH_QUEUE_TAGf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        val_a = soc_mem_field32_get(unit, ENDPOINT_QUEUE_MAPm, ent_a,
                                    DEST_PORTf);
        val_b = soc_mem_field32_get(unit, ENDPOINT_QUEUE_MAPm, ent_b,
                                    DEST_PORTf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        return 0;

    default:
        return 1;
    }

    return 0;
}
#endif /* BCM_TRIDENT2_SUPPORT */

#ifdef BCM_TOMAHAWK_SUPPORT
int
_soc_mem_cmp_exact_match(int unit, void *ent_a, void *ent_b)
{
    uint32 val_a, val_b;
    uint32 buf_a[SOC_MAX_MEM_FIELD_WORDS];
    uint32 buf_b[SOC_MAX_MEM_FIELD_WORDS];

    val_a = soc_mem_field32_get(unit, EXACT_MATCH_2m, ent_a, KEY_TYPE_0f);
    val_b = soc_mem_field32_get(unit, EXACT_MATCH_2m, ent_b, KEY_TYPE_0f);
    SOC_MEM_COMPARE_RETURN(val_a, val_b);

    switch (val_a) {
    case TH_FPEM_HASH_KEY_TYPE_128B:
        soc_mem_field_get(unit, EXACT_MATCH_2m, ent_a, MODE128__KEY_0_ONLYf,
                          buf_a);
        soc_mem_field_get(unit, EXACT_MATCH_2m, ent_b, MODE128__KEY_0_ONLYf,
                          buf_b);
        SOC_MEM_COMPARE_RETURN(buf_a[3], buf_b[3]);
        SOC_MEM_COMPARE_RETURN(buf_a[2], buf_b[2]);
        SOC_MEM_COMPARE_RETURN(buf_a[1], buf_b[1]);
        SOC_MEM_COMPARE_RETURN(buf_a[0], buf_b[0]);

        val_a = soc_mem_field32_get(unit, EXACT_MATCH_2m, ent_a,
                                    MODE128__KEY_1_ONLYf);
        val_b = soc_mem_field32_get(unit, EXACT_MATCH_2m, ent_b,
                                    MODE128__KEY_1_ONLYf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        return 0;

    case TH_FPEM_HASH_KEY_TYPE_160B:
        soc_mem_field_get(unit, EXACT_MATCH_2m, ent_a, MODE160__KEY_0_ONLYf,
                          buf_a);
        soc_mem_field_get(unit, EXACT_MATCH_2m, ent_b, MODE160__KEY_0_ONLYf,
                          buf_b);
        SOC_MEM_COMPARE_RETURN(buf_a[3], buf_b[3]);
        SOC_MEM_COMPARE_RETURN(buf_a[2], buf_b[2]);
        SOC_MEM_COMPARE_RETURN(buf_a[1], buf_b[1]);
        SOC_MEM_COMPARE_RETURN(buf_a[0], buf_b[0]);

        soc_mem_field_get(unit, EXACT_MATCH_2m, ent_a, MODE160__KEY_1_ONLYf,
                          buf_a);
        soc_mem_field_get(unit, EXACT_MATCH_2m, ent_b, MODE160__KEY_1_ONLYf,
                          buf_b);
        SOC_MEM_COMPARE_RETURN(buf_a[1], buf_b[1]);
        SOC_MEM_COMPARE_RETURN(buf_a[0], buf_b[0]);

        return 0;

    case TH_FPEM_HASH_KEY_TYPE_320B:
        soc_mem_field_get(unit, EXACT_MATCH_4m, ent_a, MODE320__KEY_0_ONLYf,
                          buf_a);
        soc_mem_field_get(unit, EXACT_MATCH_4m, ent_b, MODE320__KEY_0_ONLYf,
                          buf_b);
        SOC_MEM_COMPARE_RETURN(buf_a[3], buf_b[3]);
        SOC_MEM_COMPARE_RETURN(buf_a[2], buf_b[2]);
        SOC_MEM_COMPARE_RETURN(buf_a[1], buf_b[1]);
        SOC_MEM_COMPARE_RETURN(buf_a[0], buf_b[0]);

        soc_mem_field_get(unit, EXACT_MATCH_4m, ent_a, MODE320__KEY_1_ONLYf,
                          buf_a);
        soc_mem_field_get(unit, EXACT_MATCH_4m, ent_b, MODE320__KEY_1_ONLYf,
                          buf_b);
        SOC_MEM_COMPARE_RETURN(buf_a[3], buf_b[3]);
        SOC_MEM_COMPARE_RETURN(buf_a[2], buf_b[2]);
        SOC_MEM_COMPARE_RETURN(buf_a[1], buf_b[1]);
        SOC_MEM_COMPARE_RETURN(buf_a[0], buf_b[0]);

        soc_mem_field_get(unit, EXACT_MATCH_4m, ent_a, MODE320__KEY_2_ONLYf,
                          buf_a);
        soc_mem_field_get(unit, EXACT_MATCH_4m, ent_b, MODE320__KEY_2_ONLYf,
                          buf_b);
        SOC_MEM_COMPARE_RETURN(buf_a[3], buf_b[3]);
        SOC_MEM_COMPARE_RETURN(buf_a[2], buf_b[2]);
        SOC_MEM_COMPARE_RETURN(buf_a[1], buf_b[1]);
        SOC_MEM_COMPARE_RETURN(buf_a[0], buf_b[0]);

        val_a = soc_mem_field32_get(unit, EXACT_MATCH_4m, ent_a,
                                    MODE320__KEY_3_ONLYf);
        val_b = soc_mem_field32_get(unit, EXACT_MATCH_4m, ent_b,
                                    MODE320__KEY_3_ONLYf);
        SOC_MEM_COMPARE_RETURN(val_a, val_b);

        return 0;

    default:
        return 1;
    }

    return 0;
}
#endif /* BCM_TOMAHAWK_SUPPORT */

/*
 *  This function caches the info needed by the L3 key comparison, below.
 */
int
_soc_mem_cmp_l3x_sync(int unit)
{
    return SOC_E_NONE;
}

int
_soc_mem_cmp_l3x_set(int unit, uint32 ipmc_config)
{
    return SOC_E_NONE;
}

int
_soc_mem_cmp_l3x(int unit, void *ent_a, void *ent_b)
{
    return 0;
}

/*
 * Function:
 *    soc_mem_index_limit
 * Purpose:
 *    Returns the maximum possible index for any dynamically extendable memory
 */
int
soc_mem_index_limit(int unit, soc_mem_t mem)
{
    _SOC_MEM_REPLACE_MEM(unit, mem);
    
    return soc_mem_index_count(unit, mem);
}

#endif /* BCM_XGS_SWITCH_SUPPORT */

/*
 * Function:
 *    soc_mem_index_last
 * Purpose:
 *    Compute the highest index currently occupied in a sorted table
 */

int
soc_mem_index_last(int unit, soc_mem_t mem, int copyno)
{
    assert(SOC_MEM_IS_VALID(unit, mem));
    assert(soc_attached(unit));
    assert(soc_mem_is_sorted(unit,mem));

    if (copyno == MEM_BLOCK_ANY) {
        /* coverity[overrun-local : FALSE] */
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }

    assert(SOC_MEM_BLOCK_VALID(unit, mem, copyno));
    /* coverity[overrun-local : FALSE] */
    return (SOC_MEM_INFO(unit, mem).index_min +
                SOP_MEM_STATE(unit, mem).count[copyno] - 1);
}


/*
 * Function:
 *    soc_mem_entry_dump
 * Purpose:
 *    Debug routine to dump a formatted table entry.
 *
 *    Note:  Prefix != NULL : Dump chg command
 *           Prefix == NULL : Dump     command
 *             (Actually should pass dump_chg flag but keeping for simplicity)
 */
STATIC void
soc_mem_entry_dump_common(int unit, soc_mem_t mem, void *buf, char *prefix, int vertical, char *field_names)
{
    soc_mem_info_t *memp;
    int f;
    char dummy[]="";
    char *mem_prefix=dummy;
    int  first_print_flag=0;

#if !defined(SOC_NO_NAMES)
    soc_field_info_t *fieldp;
    char temp_field_name[80];
    uint32 key_type = 0, default_type = 0;
    uint32 fval[SOC_MAX_MEM_FIELD_WORDS];
    uint32 nfval[SOC_MAX_MEM_FIELD_WORDS]={0};
    char tmp[(SOC_MAX_MEM_FIELD_WORDS * 8) + 3];
#endif
             /* Max nybbles + "0x" + null terminator */

    memp = &SOC_MEM_INFO(unit, mem);
    if (prefix != NULL) {
        mem_prefix = prefix;
    }

    if (!SOC_MEM_IS_VALID(unit, mem)) {
        LOG_CLI((BSL_META_U(unit,
                            "<%s:%d>Memory not valid for unit\n"),mem_prefix,mem));
        return;
    }
#if !defined(SOC_NO_NAMES)
    if ((memp->flags & SOC_MEM_FLAG_MULTIVIEW)
#if defined(BCM_SAND_SUPPORT)
             && (!SOC_IS_SAND(unit))
#endif
         ) {
#ifdef KEY_TYPEf
        if (SOC_MEM_FIELD_VALID(unit, mem, KEY_TYPEf)) {
            soc_mem_field_get(unit, mem, buf, KEY_TYPEf, &key_type);
        } else if (SOC_MEM_FIELD_VALID(unit, mem, KEY_TYPE_0f)) {
            soc_mem_field_get(unit, mem, buf, KEY_TYPE_0f, &key_type);
        } else if (SOC_MEM_FIELD_VALID(unit, mem, VP_TYPEf)) {
            soc_mem_field_get(unit, mem, buf, VP_TYPEf, &key_type);
        } else if (SOC_MEM_FIELD_VALID(unit, mem, ENTRY_TYPEf)) {
            soc_mem_field_get(unit, mem, buf, ENTRY_TYPEf, &key_type);
        } else if (SOC_MEM_FIELD_VALID(unit, mem, MODEf)) {
                soc_mem_field_get(unit, mem, buf, MODEf, &key_type);
        }
#endif
        default_type = 0;
#if defined(BCM_TRIUMPH2_SUPPORT)
        if (SOC_IS_TRIUMPH2(unit) || SOC_IS_APOLLO(unit) ||
            SOC_IS_VALKYRIE2(unit) || SOC_IS_ENDURO(unit) ||
            SOC_IS_HURRICANE(unit) || SOC_IS_TD_TT(unit) ||
            SOC_IS_KATANA(unit)) {
            if((mem == VLAN_MACm) ||
                (mem == L3_ENTRY_IPV4_MULTICASTm) ||
                (mem == L3_ENTRY_IPV6_UNICASTm) ||
                (mem == L3_ENTRY_IPV6_MULTICASTm) ||
               (mem == EXACT_MATCH_4m) ||
               (mem == EXACT_MATCH_4_ENTRY_ONLYm)) {
                default_type = key_type;
            }
        }
#endif /* BCM_TRIUMPH2_SUPPORT */
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIDENT2_SUPPORT)
        if ((mem == L2_BULK_MATCH_MASKm) ||
            (mem == L2_BULK_REPLACE_MASKm)) {
            if (SOC_IS_KATANA(unit) &&
               (key_type > KATANA_L2X_VIEW_ARRAY_MAX_INDEX)) {
                key_type = KATANA_L2X_VIEW_ARRAY_MAX_INDEX;
            } else if (SOC_IS_KATANA2(unit) &&
               (key_type > KATANA2_L2X_VIEW_ARRAY_MAX_INDEX)) {
                key_type = KATANA2_L2X_VIEW_ARRAY_MAX_INDEX;
            } else if (SOC_IS_TRIDENT2(unit) &&
               (key_type > TRIDENT2_L2X_VIEW_ARRAY_MAX_INDEX)) {
                key_type = TRIDENT2_L2X_VIEW_ARRAY_MAX_INDEX;
            } else if ((SOC_IS_TRIDENT2PLUS(unit) || SOC_IS_TITAN2PLUS(unit)) &&
               (key_type > TRIDENT2PLUS_L2X_VIEW_ARRAY_MAX_INDEX)) {
                key_type = TRIDENT2PLUS_L2X_VIEW_ARRAY_MAX_INDEX;
            }
        }
#endif

#ifdef BCM_TOMAHAWK_SUPPORT
        if (mem == L2_BULKm) {
            if (SOC_IS_TOMAHAWKX(unit) &&
               (key_type > TOMAHAWK_L2X_VIEW_ARRAY_MAX_INDEX)) {
                key_type = TOMAHAWK_L2X_VIEW_ARRAY_MAX_INDEX;
            }
        }
#endif

#ifdef BCM_TRIDENT2_SUPPORT
        if (mem == L2_LEARN_INSERT_FAILUREm) {
            if (SOC_IS_TRIDENT2(unit) &&
               (key_type > TRIDENT2_L2X_VIEW_ARRAY_MAX_INDEX)) {
                key_type = TRIDENT2_L2X_VIEW_ARRAY_MAX_INDEX;
            } if ((SOC_IS_TRIDENT2PLUS(unit) || SOC_IS_TITAN2PLUS(unit)) &&
               (key_type > TRIDENT2PLUS_L2X_VIEW_ARRAY_MAX_INDEX)) {
                key_type = TRIDENT2PLUS_L2X_VIEW_ARRAY_MAX_INDEX;
            } else if (SOC_IS_TOMAHAWKX(unit) &&
               (key_type > TOMAHAWK_L2X_VIEW_ARRAY_MAX_INDEX)) {
                key_type = TOMAHAWK_L2X_VIEW_ARRAY_MAX_INDEX;
            }
        }
#endif

#if defined(BCM_KATANA2_SUPPORT) || defined(BCM_APACHE_SUPPORT)
        
        if ((mem == L3_TUNNELm) || (mem == L3_TUNNEL_ONLYm)) {
            if (SOC_IS_KATANA2(unit) || 
                SOC_IS_APACHE(unit)) {
                key_type++;
            }
        }
#endif

#if defined(BCM_TRIUMPH3_SUPPORT)
        if (SOC_IS_TRIUMPH3(unit)) {
            if (mem == L3_TUNNELm) {
                if (key_type == 1) {
                    /* 
                     * In TR3 reg file, field names are defined for
                     * "IP" view which is common for IPv6 & IPv4.
                     * So, IPv6 (key type 1) entries use same field
                     * names as those of IPv4 (keytype 0) entries
                     */
                    key_type = 0;
                }
            }
        }
#endif /* BCM_TRIUMPH3_SUPPORT */
#if defined(BCM_TOMAHAWK2_SUPPORT)
        if (SOC_IS_TOMAHAWK2(unit)) {
            if ((mem == L3_TUNNELm) || (mem == L3_TUNNEL_ONLYm)) {
                if ((key_type == 0) || (key_type == 1)) {
                    /* 
                     * In TH2 reg file, field names are defined for
                     * "IP" view which is common for IPv6 & IPv4.
                     * So, IPv6 (key type 1) entries use same field
                     * names as those of IPv4 (keytype 0) entries
                     */
                    key_type = 0;
                } else {
                    key_type++;
                }
            }
        }
#endif

        if (sal_strlen(memp->views[key_type]) == 0) {
            LOG_CLI((BSL_META_U(unit,
                                "<Wrong Key_type %d>\n"),key_type));
            return;
        }
    }
#endif
    for (f = memp->nFields - 1; f >= 0; f--) {
#if !defined(SOC_NO_NAMES)
        fieldp = &memp->fields[f];
        if ((memp->flags & SOC_MEM_FLAG_MULTIVIEW)
#if defined(BCM_SAND_SUPPORT)
             && (!SOC_IS_SAND(unit))
#endif
             ) {
            if (!(strstr(soc_fieldnames[fieldp->field], memp->views[key_type]) ||
                (strcmp(memp->views[key_type], memp->views[default_type]) == 0
                 && !strstr(soc_fieldnames[fieldp->field], ":"))             ||
                (fieldp->flags & SOCF_GLOBAL))) {
                continue;
            }
        }
        if (field_names != NULL) {
            sal_strncpy(temp_field_name, ",", 2);
            sal_strncat(temp_field_name, soc_fieldnames[fieldp->field], 
                        sizeof(temp_field_name) - 4);
            sal_strncat(temp_field_name, ",", 1);
            if (sal_strstr(field_names, temp_field_name) == NULL) {
                continue;
            }
        }

        sal_memset(fval, 0, sizeof (fval));
        soc_mem_field_get(unit, mem, buf, fieldp->field, fval);
        if (mem_prefix == prefix) {
            if (sal_memcmp(fval, nfval, SOC_MAX_MEM_FIELD_WORDS *
                           sizeof (uint32)) == 0) {
                continue;
            }
        }
        if (first_print_flag == 0) {
            LOG_CLI((BSL_META_U(unit,
                                "%s%s"), mem_prefix, vertical ? "" : "<"));
            first_print_flag=1;
        }
        if (vertical) {
            _shr_format_long_integer(tmp, fval, BITS2BYTES(fieldp->len));
            LOG_CLI((BSL_META_U(unit,
                                "\n\t%30s: %s"), soc_fieldnames[fieldp->field], tmp));
        } else {
            _shr_format_long_integer(tmp, fval, BITS2BYTES(fieldp->len));
            LOG_CLI((BSL_META_U(unit,
                                "%s=%s%s"), soc_fieldnames[fieldp->field],
                                tmp, f > 0 ? "," : ""));
        }
#endif
    }
    if (first_print_flag == 1) {
        LOG_CLI((BSL_META_U(unit,
                            "%s\n"), vertical ? "" : ">"));
    }
}

void
soc_mem_entry_dump_fields(int unit, soc_mem_t mem, void *buf, char *field_names)
{
   soc_mem_entry_dump_common(unit, mem, buf, NULL, 0, field_names);
}
void
soc_mem_entry_dump(int unit, soc_mem_t mem, void *buf)
{
   soc_mem_entry_dump_common(unit, mem, buf, NULL, 0, NULL);
}
void
soc_mem_entry_dump_vertical(int unit, soc_mem_t mem, void *buf)
{
   soc_mem_entry_dump_common(unit, mem, buf, NULL, 1, NULL);
}
void
soc_mem_entry_dump_if_changed(int unit, soc_mem_t mem, void *buf, char *prefix)
{
    soc_mem_entry_dump_common(unit, mem, buf, prefix, 0, NULL);
}
void
soc_mem_entry_dump_if_changed_fields(int unit, soc_mem_t mem, void *buf,
                                     char *prefix, char *field_names)
{
    soc_mem_entry_dump_common(unit, mem, buf, prefix, 0, field_names);
}

/************************************************************************/
/* Routines for configuring the table memory caches            */
/************************************************************************/

/*
 * Function:
 *    soc_mem_cache_get
 * Purpose:
 *    Return whether or not caching is enabled for a specified memory
 *    or memories
 */

int
soc_mem_cache_get(int unit, soc_mem_t mem, int copyno)
{
    int        rv;

    assert(SOC_UNIT_VALID(unit));
    _SOC_MEM_REUSE_MEM_STATE(unit, mem);
    SOC_MEM_ALIAS_TO_ORIG(unit,mem);
    assert(SOC_MEM_IS_VALID(unit, mem));

    if (!soc_mem_is_cachable(unit, mem) || SOC_MEM_TEST_SKIP_CACHE(unit)) {
        return FALSE;
    }

    MEM_LOCK(unit, mem);

    if (copyno == SOC_BLOCK_ALL) {
        rv = TRUE;
        SOC_MEM_BLOCK_ITER(unit, mem, copyno) {
            rv = (rv && (SOC_MEM_STATE(unit, mem).cache[copyno] != NULL));
        }
    } else {
        rv = (SOC_MEM_STATE(unit, mem).cache[copyno] != NULL);
    }

    MEM_UNLOCK(unit, mem);

    return rv;
}

#if defined(BCM_TOMAHAWK_SUPPORT)
void
_soc_mem_fpem_entry_cache_entry_validate(int unit, soc_mem_t mem, uint32 *entry,
                                         uint8 *vmap, int *index)
{
    switch (mem) {
    case EXACT_MATCH_2m:
    case EXACT_MATCH_2_PIPE0m:
    case EXACT_MATCH_2_PIPE1m:
    case EXACT_MATCH_2_PIPE2m:
    case EXACT_MATCH_2_PIPE3m:
        if (!soc_mem_field32_get(unit, EXACT_MATCH_2m, entry,
                    VALID_0f)) {
            CACHE_VMAP_CLR(vmap, *index);
            break;
        }
        switch(soc_mem_field32_get(unit, EXACT_MATCH_2m, entry, KEY_TYPE_0f)) {
        case TH_FPEM_HASH_KEY_TYPE_320B:
            CACHE_VMAP_CLR(vmap, *index); *index += 1;
            CACHE_VMAP_CLR(vmap, *index);
            break;
        case TH_FPEM_HASH_KEY_TYPE_128B:
        case TH_FPEM_HASH_KEY_TYPE_160B:
            /* let vmap bit remain set */
            break;
        default: /* illegal key_type */
            CACHE_VMAP_CLR(vmap, *index);
            break;
        }
        break;
    
    case EXACT_MATCH_4m:
    case EXACT_MATCH_4_PIPE0m:
    case EXACT_MATCH_4_PIPE1m:
    case EXACT_MATCH_4_PIPE2m:
    case EXACT_MATCH_4_PIPE3m:
        if (!soc_mem_field32_get(unit, EXACT_MATCH_4m, entry,
                    VALID_0f)) {
            CACHE_VMAP_CLR(vmap, *index);
            break;
        }
        if (TH_FPEM_HASH_KEY_TYPE_320B !=
                soc_mem_field32_get(unit, EXACT_MATCH_4m, entry,
                                    KEY_TYPE_0f)) {
            CACHE_VMAP_CLR(vmap, *index);
        }
        break;

    default:
        break;
    }
}
#endif /* BCM_TOMAHAWK_SUPPORT */

void
_soc_mem_l3_entry_cache_entry_validate(int unit, soc_mem_t mem, uint32 *entry,
                                       uint8 *vmap, int *index)
{
    if (SOC_MEM_FIELD_VALID(unit, L3_ENTRY_ONLYm, KEY_TYPEf)) {
#ifdef BCM_TRIDENT2_SUPPORT
        if (SOC_IS_TD2_TT2(unit)) {
#ifdef BCM_TOMAHAWK_SUPPORT
            if (SOC_IS_TOMAHAWKX(unit)) {
                switch (mem) {
                case L3_ENTRY_ONLYm:
                case L3_ENTRY_IPV4_UNICASTm:
                    if (!soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, VALIDf)) {
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    switch (soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, KEY_TYPEf)) {
                    case TH_L3_HASH_KEY_TYPE_V4MC:
                    case TH_L3_HASH_KEY_TYPE_V4UC_EXT:
                    case TH_L3_HASH_KEY_TYPE_DST_NAT:
                    case TH_L3_HASH_KEY_TYPE_DST_NAPT:
                    case TH_L3_HASH_KEY_TYPE_V6UC:
                        CACHE_VMAP_CLR(vmap, *index); *index += 1;
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    case TH_L3_HASH_KEY_TYPE_V6MC:
                    case TH_L3_HASH_KEY_TYPE_V6UC_EXT:
                        CACHE_VMAP_CLR(vmap, *index); *index += 1;
                        CACHE_VMAP_CLR(vmap, *index); *index += 1;
                        CACHE_VMAP_CLR(vmap, *index); *index += 1;
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    case TH_L3_HASH_KEY_TYPE_V4UC:
                    case TH_L3_HASH_KEY_TYPE_TRILL:
                        /* let vmap bit remain set */
                        break;
                    default: /* illegal key_type */
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    break;
                case L3_ENTRY_IPV4_MULTICASTm:
                    if (!soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, VALIDf)) {
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    switch (soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, KEY_TYPEf)) {
                    case TH_L3_HASH_KEY_TYPE_V4UC:
                    case TH_L3_HASH_KEY_TYPE_TRILL:
                    case TH_L3_HASH_KEY_TYPE_V6UC:
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    case TH_L3_HASH_KEY_TYPE_V6MC:
                    case TH_L3_HASH_KEY_TYPE_V6UC_EXT:
                        CACHE_VMAP_CLR(vmap, *index); *index += 1;
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    case TH_L3_HASH_KEY_TYPE_V4MC:
                    case TH_L3_HASH_KEY_TYPE_V4UC_EXT:
                    case TH_L3_HASH_KEY_TYPE_DST_NAT:
                    case TH_L3_HASH_KEY_TYPE_DST_NAPT:
                        /* let vmap bit remain set */
                        break;
                    default: /* illegal key_type */
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    break;
                case L3_ENTRY_IPV6_UNICASTm:
                    if (!soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, VALIDf)) {
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    switch (soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, KEY_TYPEf)) {
                    case TH_L3_HASH_KEY_TYPE_V4UC:
                    case TH_L3_HASH_KEY_TYPE_TRILL:
                    case TH_L3_HASH_KEY_TYPE_V4MC:
                    case TH_L3_HASH_KEY_TYPE_V4UC_EXT:
                    case TH_L3_HASH_KEY_TYPE_DST_NAT:
                    case TH_L3_HASH_KEY_TYPE_DST_NAPT:
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    case TH_L3_HASH_KEY_TYPE_V6MC:
                    case TH_L3_HASH_KEY_TYPE_V6UC_EXT:
                        CACHE_VMAP_CLR(vmap, *index); *index += 1;
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    case TH_L3_HASH_KEY_TYPE_V6UC:
                        /* let vmap bit remain set */
                        break;
                    default: /* illegal key_type */
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    break;
                case L3_ENTRY_IPV6_MULTICASTm:
                    if (!soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, VALIDf)) {
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    switch (soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, KEY_TYPEf)) {
                    case TH_L3_HASH_KEY_TYPE_V6MC:
                    case TH_L3_HASH_KEY_TYPE_V6UC_EXT:
                        break;
                    default:
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    break;
                default:
                    break;
                }
            } else
#endif /* BCM_TOMAHAWK_SUPPORT */
            {
                switch (mem) {
                case L3_ENTRY_ONLYm:
                case L3_ENTRY_IPV4_UNICASTm:
                    if (!soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, VALIDf)) {
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    switch (soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, KEY_TYPEf)) {
                    case TD2_L3_HASH_KEY_TYPE_V4UC_EXT:
                    case TD2_L3_HASH_KEY_TYPE_V4MC:
                    case TD2_L3_HASH_KEY_TYPE_V4L2MC:
                    case TD2_L3_HASH_KEY_TYPE_V4L2VPMC:
                    case TD2_L3_HASH_KEY_TYPE_V6UC:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN_EXT:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_HOST_EXT:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP_EXT:
                    case TD2_L3_HASH_KEY_TYPE_DST_NAT:
                    case TD2_L3_HASH_KEY_TYPE_DST_NAPT:
                        CACHE_VMAP_CLR(vmap, *index); *index += 1;
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    case TD2_L3_HASH_KEY_TYPE_V6UC_EXT:
                    case TD2_L3_HASH_KEY_TYPE_V6MC:
                    case TD2_L3_HASH_KEY_TYPE_V6L2MC:
                    case TD2_L3_HASH_KEY_TYPE_V6L2VPMC:
                        CACHE_VMAP_CLR(vmap, *index); *index += 1;
                        CACHE_VMAP_CLR(vmap, *index); *index += 1;
                        CACHE_VMAP_CLR(vmap, *index); *index += 1;
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    case TD2_L3_HASH_KEY_TYPE_V4UC:
                    case TD2_L3_HASH_KEY_TYPE_TRILL:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_HOST:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP:
                        /* let vmap bit remain set */
                        break;
                    default: /* illegal key_type */
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    break;
                case L3_ENTRY_IPV4_MULTICASTm:
                    if (!soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, VALIDf)) {
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    switch (soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, KEY_TYPEf)) {
                    case TD2_L3_HASH_KEY_TYPE_V4UC:
                    case TD2_L3_HASH_KEY_TYPE_TRILL:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_HOST:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP:
                    case TD2_L3_HASH_KEY_TYPE_V6UC:
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    case TD2_L3_HASH_KEY_TYPE_V6UC_EXT:
                    case TD2_L3_HASH_KEY_TYPE_V6MC:
                    case TD2_L3_HASH_KEY_TYPE_V6L2MC:
                    case TD2_L3_HASH_KEY_TYPE_V6L2VPMC:
                        CACHE_VMAP_CLR(vmap, *index); *index += 1;
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    case TD2_L3_HASH_KEY_TYPE_V4UC_EXT:
                    case TD2_L3_HASH_KEY_TYPE_V4MC:
                    case TD2_L3_HASH_KEY_TYPE_V4L2MC:
                    case TD2_L3_HASH_KEY_TYPE_V4L2VPMC:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN_EXT:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_HOST_EXT:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP_EXT:
                    case TD2_L3_HASH_KEY_TYPE_DST_NAT:
                    case TD2_L3_HASH_KEY_TYPE_DST_NAPT:
                        /* let vmap bit remain set */
                        break;
                    default: /* illegal key_type */
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    break;
                case L3_ENTRY_IPV6_UNICASTm:
                    if (!soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, VALIDf)) {
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    switch (soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, KEY_TYPEf)) {
                    case TD2_L3_HASH_KEY_TYPE_V4UC:
                    case TD2_L3_HASH_KEY_TYPE_TRILL:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_HOST:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP:
                    case TD2_L3_HASH_KEY_TYPE_V4UC_EXT:
                    case TD2_L3_HASH_KEY_TYPE_V4MC:
                    case TD2_L3_HASH_KEY_TYPE_V4L2MC:
                    case TD2_L3_HASH_KEY_TYPE_V4L2VPMC:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN_EXT:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_HOST_EXT:
                    case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP_EXT:
                    case TD2_L3_HASH_KEY_TYPE_DST_NAT:
                    case TD2_L3_HASH_KEY_TYPE_DST_NAPT:
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    case TD2_L3_HASH_KEY_TYPE_V6UC_EXT:
                    case TD2_L3_HASH_KEY_TYPE_V6MC:
                    case TD2_L3_HASH_KEY_TYPE_V6L2MC:
                    case TD2_L3_HASH_KEY_TYPE_V6L2VPMC:
                        CACHE_VMAP_CLR(vmap, *index); *index += 1;
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    case TD2_L3_HASH_KEY_TYPE_V6UC:
                        /* let vmap bit remain set */
                        break;
                    default: /* illegal key_type */
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    break;
                case L3_ENTRY_IPV6_MULTICASTm:
                    if (!soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, VALIDf)) {
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    switch (soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, KEY_TYPEf)) {
                    case TD2_L3_HASH_KEY_TYPE_V6UC_EXT:
                    case TD2_L3_HASH_KEY_TYPE_V6MC:
                    case TD2_L3_HASH_KEY_TYPE_V6L2MC:
                    case TD2_L3_HASH_KEY_TYPE_V6L2VPMC:
                        /* let vmap bit remain set */
                        break;
                    default:
                        CACHE_VMAP_CLR(vmap, *index);
                        break;
                    }
                    break;
                default:
                    break;
                }
            }
        } else
#endif /* BCM_TRIDENT2_SUPPORT */
        {
            switch (mem) {
            case L3_ENTRY_ONLYm:
            case L3_ENTRY_IPV4_UNICASTm:
                if (!soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, VALIDf)) {
                    CACHE_VMAP_CLR(vmap, *index);
                    break;
                }
                switch (soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, KEY_TYPEf)) {
                case TR_L3_HASH_KEY_TYPE_V4MC:
                case TR_L3_HASH_KEY_TYPE_V6UC:
                    CACHE_VMAP_CLR(vmap, *index); *index += 1;
                    CACHE_VMAP_CLR(vmap, *index);
                    break;
                case TR_L3_HASH_KEY_TYPE_V6MC:
                    CACHE_VMAP_CLR(vmap, *index); *index += 1;
                    CACHE_VMAP_CLR(vmap, *index); *index += 1;
                    CACHE_VMAP_CLR(vmap, *index); *index += 1;
                    CACHE_VMAP_CLR(vmap, *index);
                    break;
                default:
                    break;
                }
                break;
            case L3_ENTRY_IPV4_MULTICASTm:
            case L3_ENTRY_IPV6_UNICASTm:
                if (!soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, VALIDf)) {
                    CACHE_VMAP_CLR(vmap, *index);
                    break;
                }
                switch (soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, KEY_TYPEf)) {
                case TR_L3_HASH_KEY_TYPE_V4UC:
                case TR_L3_HASH_KEY_TYPE_LMEP:
                case TR_L3_HASH_KEY_TYPE_RMEP:
                case TR_L3_HASH_KEY_TYPE_TRILL:
                    CACHE_VMAP_CLR(vmap, *index);
                    break;
                case TR_L3_HASH_KEY_TYPE_V4MC:
                    if (mem == L3_ENTRY_IPV6_UNICASTm) {
                        CACHE_VMAP_CLR(vmap, *index);
                    }
                    break;
                case TR_L3_HASH_KEY_TYPE_V6UC:
                    if (mem == L3_ENTRY_IPV4_MULTICASTm) {
                        CACHE_VMAP_CLR(vmap, *index);
                    }
                    break;
                case TR_L3_HASH_KEY_TYPE_V6MC:
                    CACHE_VMAP_CLR(vmap, *index); *index += 1;
                    CACHE_VMAP_CLR(vmap, *index);
                    break;
                default:
                    break;
                }
                break;
            case L3_ENTRY_IPV6_MULTICASTm:
                if (!soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, VALIDf)) {
                    CACHE_VMAP_CLR(vmap, *index);
                    break;
                }
                switch (soc_mem_field32_get(unit, L3_ENTRY_ONLYm, entry, KEY_TYPEf)) {
                case TR_L3_HASH_KEY_TYPE_V6MC:
                    break;
                default:
                    CACHE_VMAP_CLR(vmap, *index);
                    break;
                }
                break;
            default:
                break;
            }
        }
    }
#ifdef BCM_TRIUMPH3_SUPPORT
    else if (SOC_MEM_IS_VALID(unit, L3_ENTRY_1m)) {
        switch (mem) {
        case L3_ENTRY_1m:
            if (!soc_mem_field32_get(unit, mem, entry, VALIDf)) {
                CACHE_VMAP_CLR(vmap, *index);
                break;
            }
            switch (soc_mem_field32_get(unit, mem, entry, KEY_TYPEf)) {
            case SOC_MEM_VIEW_L3_ENTRY_2_IPV4UC:
            case SOC_MEM_VIEW_L3_ENTRY_2_IPV4MC:
            case SOC_MEM_VIEW_L3_ENTRY_2_IPV6UC:
                CACHE_VMAP_CLR(vmap, *index); *index += 1;
                CACHE_VMAP_CLR(vmap, *index);
                break;
            case SOC_MEM_VIEW_L3_ENTRY_4_IPV6UC:
            case SOC_MEM_VIEW_L3_ENTRY_4_IPV6MC:
                CACHE_VMAP_CLR(vmap, *index); *index += 1;
                CACHE_VMAP_CLR(vmap, *index); *index += 1;
                CACHE_VMAP_CLR(vmap, *index); *index += 1;
                CACHE_VMAP_CLR(vmap, *index);
                break;
            default:
                break;
            }
            break;
        case L3_ENTRY_2m:
            if (!soc_mem_field32_get(unit, mem, entry, VALID_0f)) {
                CACHE_VMAP_CLR(vmap, *index);
                break;
            }
            switch (soc_mem_field32_get(unit, mem, entry, KEY_TYPE_0f)) {
            case SOC_MEM_VIEW_L3_ENTRY_2_IPV4UC:
            case SOC_MEM_VIEW_L3_ENTRY_2_IPV4MC:
            case SOC_MEM_VIEW_L3_ENTRY_2_IPV6UC:
                CACHE_VMAP_CLR(vmap, *index);
                break;
            case SOC_MEM_VIEW_L3_ENTRY_4_IPV6UC:
            case SOC_MEM_VIEW_L3_ENTRY_4_IPV6MC:
                CACHE_VMAP_CLR(vmap, *index); *index += 1;
                CACHE_VMAP_CLR(vmap, *index);
                break;
            default:
                break;
            }
            break;
        case L3_ENTRY_4m:
            if (!soc_mem_field32_get(unit, mem, entry, VALID_0f)) {
                CACHE_VMAP_CLR(vmap, *index);
                break;
            }
            switch (soc_mem_field32_get(unit, mem, entry, KEY_TYPE_0f)) {
            case SOC_MEM_VIEW_L3_ENTRY_4_IPV6UC:
            case SOC_MEM_VIEW_L3_ENTRY_4_IPV6MC:
                break;
            default:
                CACHE_VMAP_CLR(vmap, *index);
                break;
            }
            break;
        default:
            break;
        }
    }
#endif /* BCM_TRIUMPH3_SUPPORT */
}

#ifdef BCM_TRIDENT2_SUPPORT
#ifdef BCM_WARM_BOOT_SUPPORT
int
soc_mem_block_count(int unit, soc_mem_t mem, int *count)
{
    int blk_count = 0, blk;

    if (NULL == count) {
        return SOC_E_PARAM;
    }

    SOC_MEM_BLOCK_ITER(unit, mem, blk) {
        blk_count++;
    }

    *count = blk_count;
    return SOC_E_NONE;
}

int
soc_control_overlay_tcam_scache_init(int unit, int blk_num)
{
    soc_scache_handle_t scache_handle;
    uint8 *overlay_tcam_scache_ptr;
    int rv;
    int stable_size, index_count = 0;
    int alloc_sz;
    int create = SOC_WARM_BOOT(unit) ? FALSE : TRUE;

    if (SOC_MEM_IS_VALID(unit, FP_GLOBAL_MASK_TCAMm)) {
        index_count = soc_mem_index_count(unit, FP_GLOBAL_MASK_TCAMm);
        alloc_sz = SHR_BITALLOCSIZE(index_count) * blk_num;
    } else {
        return SOC_E_UNAVAIL;
    }

    SOC_IF_ERROR_RETURN(soc_stable_size_get(unit, &stable_size));
    if (!stable_size) {
        /* If stable not allocated, just return */
        return SOC_E_NONE;
    }
    SOC_SCACHE_ALIGN_SIZE(alloc_sz);

    alloc_sz += SOC_WB_SCACHE_CONTROL_SIZE;

    if (!SOC_WARM_BOOT_SCACHE_IS_LIMITED(unit)) {
        SOC_SCACHE_HANDLE_SET(scache_handle, unit,
                              SOC_SCACHE_SOC_OVERLAY_TCAM_HANDLE, 0);
        /* Get the pointer for the Level 2 cache */
        rv = soc_extended_scache_ptr_get(unit, scache_handle, create,
                                         create ? alloc_sz : 0,
                                         &overlay_tcam_scache_ptr);
        if (SOC_FAILURE(rv) && (rv != SOC_E_NOT_FOUND)) {
            return rv;
        } else if (SOC_E_NOT_FOUND == rv && !create) {
            /* Not yet allocated in upgrade */
            rv = soc_extended_scache_ptr_get(unit, scache_handle, TRUE,
                                             alloc_sz,
                                             &overlay_tcam_scache_ptr);
            if (SOC_FAILURE(rv) && (rv != SOC_E_NOT_FOUND)) {
                return rv;
            }
            if (SOC_SUCCESS(rv)) {
                SOC_FP_TCAM_SCACHE_HANDLE(unit) = overlay_tcam_scache_ptr;
            }
        } else if (SOC_FAILURE(rv)) {
            return rv;
        } else {
            SOC_FP_TCAM_SCACHE_HANDLE(unit) = overlay_tcam_scache_ptr;
        }
    }

    return SOC_E_NONE;
}

int
soc_control_overlay_tcam_scache_sync(int unit)
{
    uint8 *overlay_tcam_scache_ptr = SOC_FP_TCAM_SCACHE_HANDLE(unit);
    SHR_BITDCL *overlay_tcam_bmp;
    int stable_size = 0, index_count, blk, scache_size;
    uint16 version = SOC_OVERLAY_TCAM_WARMBOOT_DEFAULT_VERSION;
    soc_mem_t mem;
    soc_memstate_t *memState;

    if (SOC_MEM_IS_VALID(unit, FP_GLOBAL_MASK_TCAMm)) {
        mem = FP_GLOBAL_MASK_TCAMm;
        index_count = soc_mem_index_count(unit, mem);
        scache_size = SHR_BITALLOCSIZE(index_count);
        memState = &SOC_MEM_STATE(unit, mem);
    } else {
        return SOC_E_UNAVAIL;
    }

    SOC_IF_ERROR_RETURN(soc_stable_size_get(unit, &stable_size));
    if (!stable_size) {
        /* If stable not allocated, just return */
        return SOC_E_NONE;
    }
    if (NULL == overlay_tcam_scache_ptr) {
        return SOC_E_UNAVAIL;
    }

    /* First save version */
    sal_memcpy(overlay_tcam_scache_ptr, &version, sizeof(version));
    overlay_tcam_scache_ptr += sizeof(version);

    /* Save overlay tcam bitmap */
    SOC_MEM_BLOCK_ITER(unit, mem, blk) {
        overlay_tcam_bmp = memState->OverlayTcamMap[blk];

        if (NULL == overlay_tcam_bmp) {
            return SOC_E_INIT;
        }
        sal_memcpy(overlay_tcam_scache_ptr,
                   overlay_tcam_bmp,
                   scache_size);
        overlay_tcam_scache_ptr += scache_size;
    }
    return SOC_E_NONE;
}

int
soc_control_overlay_tcam_scache_load(int unit, void *bmp, int blk,
                                     uint8 *overlay_tcam_scache_ptr)
{
    SHR_BITDCL *overlay_tcam_bmp = (SHR_BITDCL *)bmp;
    int index_count, scache_size, blk_count;
    int stable_size = 0;

    if (SOC_MEM_IS_VALID(unit, FP_GLOBAL_MASK_TCAMm)) {
        index_count = soc_mem_index_count(unit, FP_GLOBAL_MASK_TCAMm);
        scache_size = SHR_BITALLOCSIZE(index_count);
    } else {
        return SOC_E_UNAVAIL;
    }

    SOC_IF_ERROR_RETURN(soc_stable_size_get(unit, &stable_size));
    if (!stable_size) {
        /* If stable not allocated, just return */
        return SOC_E_RESOURCE;
    }

    if (NULL == overlay_tcam_scache_ptr) {
        return SOC_E_UNAVAIL;
    }

    if (NULL == overlay_tcam_bmp) {
        return SOC_E_INIT;
    }

    soc_mem_block_count(unit, FP_GLOBAL_MASK_TCAMm, &blk_count);
    if (blk >= blk_count || blk < 0) {
        return SOC_E_RESOURCE;
    }

    if (SOC_WARM_BOOT(unit)) {
        sal_memcpy(overlay_tcam_bmp,
                   overlay_tcam_scache_ptr + scache_size*blk,
                   scache_size);
    }
    return SOC_E_NONE;
}

/*
 * Function:
 *    soc_mem_cache_overlay_tcam_check
 * Purpose:
 *    Check if the overlay tcam cache is valid. Clear the invalid entry's
 *    cache and vmap.
 */
int
soc_mem_cache_overlay_tcam_check(int unit, soc_mem_t mem,
                         SHR_BITDCL *overlay_tcam_bmp, uint8 *vmap)
{
    int idx, index_min, index_max;

    if (NULL == vmap) {
        return SOC_E_PARAM;
    }

    index_min = soc_mem_index_min(unit, mem);
    index_max = soc_mem_index_max(unit, mem);

    if (mem != FP_GLOBAL_MASK_TCAMm &&
        mem != FP_GM_FIELDSm) {
        return SOC_E_NONE;
    }

    if (NULL == overlay_tcam_bmp) {
        return SOC_E_INIT;
    }

    for (idx = index_min;
         idx <= index_max;
         idx++) {
            if (FP_GLOBAL_MASK_TCAMm == mem) {
                if (SHR_BITGET(overlay_tcam_bmp, idx)) {
                    CACHE_VMAP_CLR(vmap, idx);
                }
            } else if (FP_GM_FIELDSm == mem) {
                if (!SHR_BITGET(overlay_tcam_bmp, idx)) {
                    CACHE_VMAP_CLR(vmap, idx);
                }
            }
    }

    return SOC_E_NONE;
}

/*
 * Function:
 *    soc_mem_overlay_tcam_update
 * Purpose:
 *    Update the overlay tcam bitmap during writing or clearing
 *
 */
void
soc_mem_overlay_tcam_update(int unit, soc_mem_t mem, int blk,
                         int index_begin, int index_end)
{
    int num_entries;
    SHR_BITDCL *soc_overlay_tcam_bmp;
    soc_memstate_t *memState;

    if (mem != FP_GLOBAL_MASK_TCAMm &&
        mem != FP_GM_FIELDSm) {
        return;
    }

    if (index_end - index_begin < 0 ||
        soc_mem_index_min(unit, mem) > index_begin ||
        soc_mem_index_max(unit, mem) < index_end) {
        return;
    }

    memState = &SOC_MEM_STATE(unit, mem);

    soc_overlay_tcam_bmp = memState->OverlayTcamMap[blk];
    if (NULL == soc_overlay_tcam_bmp) {
        return;
    }

    num_entries = index_end - index_begin + 1;

    if (mem == FP_GLOBAL_MASK_TCAMm) {
        SHR_BITCLR_RANGE(soc_overlay_tcam_bmp, index_begin, num_entries);
    } else if (mem == FP_GM_FIELDSm) {
        SHR_BITSET_RANGE(soc_overlay_tcam_bmp, index_begin, num_entries);
    }
}

/*
 * Function:
 *    soc_mem_overlay_tcam_cache_set
 * Purpose:
 *    Enable or disable caching for overlay tcam memories
 * Notes:
 *    Set cache for FP_GLOBAL_MASK_TCAMm and FP_GM_FIELDSm
 *    based on OverlayTcamMap.
 */
int
soc_mem_overlay_tcam_cache_set(int unit, 
                                soc_mem_t mem, 
                                int copyno, 
                                int enable)
{
    SHR_BITDCL  *OverlayTcamMap;
    int alloc_sz = 0, rv = SOC_E_NONE;
    int blk_count = 0, index_count = 0, valid_blk = 0, blk;
    soc_memstate_t *mem_state, *mem_state_temp;
    soc_mem_t fp_mem[2] = {FP_GLOBAL_MASK_TCAMm, FP_GM_FIELDSm};
    uint8 *overlay_tcam_scache_ptr = NULL;
    uint16 version = 0;

    if (mem != fp_mem[0] && mem != fp_mem[1]) {
        return SOC_E_PARAM;
    }

    index_count = soc_mem_index_count(unit, mem);
    alloc_sz = SHR_BITALLOCSIZE(index_count);
    soc_mem_block_count(unit, mem, &blk_count);
    mem_state = &SOC_MEM_STATE(unit, mem);

    if (mem == fp_mem[0]) {
        mem_state_temp = &SOC_MEM_STATE(unit, fp_mem[1]);
    } else {
        mem_state_temp = &SOC_MEM_STATE(unit, fp_mem[0]);
    }

    /* Init FP overlay tcam bitmap scache*/
    if (enable) {
        rv = soc_control_overlay_tcam_scache_init(unit, blk_count);
        if (SOC_FAILURE(rv)) {
            if (rv == SOC_E_UNAVAIL) {
                rv = SOC_E_NONE;
            }
            goto _tcam_set_done;
        }
    }

    /*load version*/
    if (SOC_WARM_BOOT(unit)) {
        overlay_tcam_scache_ptr = SOC_FP_TCAM_SCACHE_HANDLE(unit);
        if (overlay_tcam_scache_ptr != NULL) {
            sal_memcpy(&version, overlay_tcam_scache_ptr, sizeof(version));
            overlay_tcam_scache_ptr += sizeof(version);
        }
    }

    SOC_MEM_BLOCK_ITER(unit, mem, blk) {
        if (copyno != COPYNO_ALL && copyno != blk) {
            continue;
        }

        /*
                *FP_GLOBAL_MASK_TCAMm and FP_GM_FIELDSm share one overlay tcam 
                *bitmap. So only destory overlay tcam bitmap one time. 
                */
        if (!enable) {
            if (mem_state->OverlayTcamMap[blk] != NULL) {
                sal_free(mem_state->OverlayTcamMap[blk]);
                mem_state->OverlayTcamMap[blk] = NULL;
                mem_state_temp->OverlayTcamMap[blk] = NULL;
            }
            continue;
        }

        /*
                *Alloc overlay tcam bitmap when OverlayTcamMap for both mems equal NULL
                */
        if (mem_state->OverlayTcamMap[blk] == NULL) {
            if (mem_state_temp->OverlayTcamMap[blk] == NULL) {
                if ((OverlayTcamMap =
                    sal_alloc(alloc_sz,
                              "mem XY TCAM BITMAP")) == NULL) {
                    rv = SOC_E_MEMORY;
                    goto _tcam_set_done;
                }
                sal_memset(OverlayTcamMap, 0, alloc_sz);
                mem_state->OverlayTcamMap[blk] = OverlayTcamMap;

                if (SOC_WARM_BOOT(unit)) {
                    /* Recover overlay tcam bitmap from scache*/
                    if (version >= SOC_OVERLAY_TCAM_WARMBOOT_VERSION_1) {
                        rv = soc_control_overlay_tcam_scache_load
                               (unit, OverlayTcamMap, valid_blk, overlay_tcam_scache_ptr);
                    } else {
                        rv = SOC_E_UNAVAIL;
                    }
                    if (SOC_FAILURE(rv)) {
                        if (rv == SOC_E_UNAVAIL) {
                            rv = SOC_E_NONE;
                        }
                        goto _tcam_set_done;
                    }

                    /* Clear the invalid vmap based on overlay tcam bitmap*/
                    rv = soc_mem_cache_overlay_tcam_check
                        (unit, mem, OverlayTcamMap, mem_state->vmap[blk]);
                    if (SOC_FAILURE(rv)) {
                        goto _tcam_set_done;
                    }

                    valid_blk++;
                }
            } else {
                OverlayTcamMap = mem_state_temp->OverlayTcamMap[blk];
                mem_state->OverlayTcamMap[blk] = OverlayTcamMap;

                if (SOC_WARM_BOOT(unit)) {
                    rv = soc_mem_cache_overlay_tcam_check
                        (unit, mem, OverlayTcamMap, mem_state->vmap[blk]);
                    if (SOC_FAILURE(rv)) {
                        goto _tcam_set_done;
                    }
                }
            }
        }
    }
_tcam_set_done:
    return rv;
}
#endif
#endif

/*
 * Function:
 *    soc_mem_cache_set
 * Purpose:
 *    Enable or disable caching for a specified memory or memories
 * Notes:
 *    Only tables whose contents are not modified by hardware in
 *    any way are eligible for caching.
 */

int
soc_mem_cache_set(int unit, soc_mem_t mem, int copyno, int enable)
{
    int blk, entry_dw, index_cnt;
    int cache_size, vmap_size, numels = 1;
    uint8 *vmap;
#if ((defined BCM_XGS_SWITCH_SUPPORT ) || (defined BCM_SAND_SUPPORT))
    uint8 load_cache = 1, l2 = 0;
#endif

    uint32 *cache;
    soc_memstate_t *memState;
    soc_stat_t *stat = SOC_STAT(unit);
    soc_mem_t org_mem = mem;

#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_HELIX4_SUPPORT) || \
    defined(BCM_KATANA2_SUPPORT) || defined(BCM_HURRICANE2_SUPPORT) || \
    defined(BCM_GREYHOUND_SUPPORT)
    int corrupt_size = 0;
    uint8 *corrupt = NULL;
#endif
#ifdef BCM_KATANA2_SUPPORT
    int port_index[15]=
    {1,5,9,13,17,21,25,26,27,28,30,33,37,39,41};
    int port = 0;
    int idx, j;
#endif /*BCM_KATANA2_SUPPORT */
#if defined(BCM_TRIDENT2_SUPPORT)
    int skip = 0;
#endif

    assert(SOC_UNIT_VALID(unit));
    _SOC_MEM_REUSE_MEM_STATE(unit, mem);
    SOC_MEM_ALIAS_TO_ORIG(unit,mem);
    assert(SOC_MEM_IS_VALID(unit, mem));

    if (!(soc_mem_is_cachable(unit, mem))) {
        return (enable ? SOC_E_UNAVAIL : SOC_E_NONE);
    }
    memState = &SOC_MEM_STATE(unit, mem);
    if (memState->lock == NULL) {
        return SOC_E_NONE;
    }

    if (SOC_MEM_INFO(unit,mem).flags & SOC_MEM_FLAG_IS_ARRAY) {
        numels = SOC_MEM_ARRAY_INFO(unit,mem).numels;
    }

    entry_dw = soc_mem_entry_words(unit, mem);
    index_cnt = soc_mem_index_count(unit, mem);
    cache_size = numels * index_cnt * entry_dw * 4;
    vmap_size = (numels * index_cnt + 7) / 8;

#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_HELIX4_SUPPORT) || \
    defined(BCM_KATANA2_SUPPORT) || defined(BCM_HURRICANE2_SUPPORT) || \
    defined(BCM_GREYHOUND_SUPPORT)
    /* TCAM tables have corrupt bitmap */
    if((SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) &&
       (!(SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_EXT_CAM))) {
        corrupt_size = (index_cnt + 7) / 8;
    }
#endif

    LOG_INFO(BSL_LS_SOC_SOCMEM,
             (BSL_META_U(unit,
                         "soc_mem_cache_set: unit %d memory %s.%s %sable\n"),
              unit, SOC_MEM_UFNAME(unit, mem), SOC_BLOCK_NAME(unit, copyno),
              enable ? "en" : "dis"));

    if (copyno != COPYNO_ALL) {
        assert(SOC_MEM_BLOCK_VALID(unit, mem, copyno));
    }

    MEM_LOCK(unit, mem);

#ifdef BCM_KATANA2_SUPPORT
    if (SOC_IS_KATANA2(unit) && (mem == VLAN_TABm) && (!enable) ) {
        for (j = 0; j < SOC_MAX_NUM_BLKS; ++j) {
            soc_control[unit]->memState[EGR_VLANm].cache[j] = NULL;
            soc_control[unit]->memState[EGR_VLANm].vmap[j] = NULL;
            soc_control[unit]->memState[EGR_VLANm].corrupt[j] = NULL;
        }
    }
#endif
    SOC_MEM_BLOCK_ITER(unit, mem, blk) {
        if (copyno != COPYNO_ALL && copyno != blk) {
            continue;
        }
#if defined(BCM_TRIUMPH3_SUPPORT)
        if (SOC_IS_TRIUMPH3(unit) &&
            ((mem == PORT_EHG_RX_TUNNEL_MASKm) ||
             (mem == PORT_EHG_RX_TUNNEL_DATAm) ||
             (mem == PORT_EHG_TX_TUNNEL_DATAm))) {
            if (!_soc_tr3_port_mem_blk_enabled(unit, blk)) {
                continue;
            }
        }
#endif /* BCM_TRIUMPH3_SUPPORT */
        /* Turn off caching if currently enabled. */
        if (memState->cache[blk] != NULL) {
            sal_free(memState->cache[blk]);
            memState->cache[blk] = NULL;
        }
        if (memState->vmap[blk] != NULL) {
            sal_free(memState->vmap[blk]);
            memState->vmap[blk] = NULL;
        }
        /* If we don't so, there are dangling pointer */
        if (org_mem != mem) {
            /* Share the cache and vmap */
            SOC_MEM_STATE(unit, org_mem).cache[blk] = NULL;
            SOC_MEM_STATE(unit, org_mem).vmap[blk] = NULL;
        }
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_HELIX4_SUPPORT) || \
    defined(BCM_KATANA2_SUPPORT) || defined(BCM_HURRICANE2_SUPPORT) || \
    defined(BCM_GREYHOUND_SUPPORT)
        if (memState->corrupt[blk] != NULL) {
            sal_free(memState->corrupt[blk]);
            memState->corrupt[blk] = NULL;
        }
#endif

        if (!enable) {
            continue;
        }
        /* Allocate new cache */
        if ((cache = sal_alloc(cache_size, "table-cache")) == NULL) {
            MEM_UNLOCK(unit, mem);
            return SOC_E_MEMORY;
        }
        stat->mem_cache_count++;
        stat->mem_cache_size += cache_size;

        if ((vmap = sal_alloc(vmap_size, "table-vmap")) == NULL) {
            sal_free(cache);
            MEM_UNLOCK(unit, mem);
            return SOC_E_MEMORY;
        }
        stat->mem_cache_vmap_size += vmap_size;
        LOG_VERBOSE(BSL_LS_SOC_SER,
            (BSL_META_U(unit,
                        "soc_mem_cache_set: WB=%0d, memory %s.%s cache_count=%0d, "
                        "cache_size=%0d, vmap_size=%0d \n"),
             SOC_WARM_BOOT(unit), SOC_MEM_UFNAME(unit, mem), SOC_BLOCK_NAME(unit, copyno),
             stat->mem_cache_count, stat->mem_cache_size,
             stat->mem_cache_vmap_size));

        sal_memset(vmap, 0, vmap_size); /* zero out all valid bits */
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_HELIX4_SUPPORT) || \
    defined(BCM_KATANA2_SUPPORT) || defined(BCM_HURRICANE2_SUPPORT) || \
    defined(BCM_GREYHOUND_SUPPORT)
        if((SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) &&
           (!(SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_EXT_CAM))) {
            /* Allocate corruption bitmap */
            if ((corrupt = sal_alloc(corrupt_size, "corrupt indication map")) == NULL) {
                sal_free(cache);
                sal_free(vmap);
                MEM_UNLOCK(unit, mem);
                return SOC_E_MEMORY;
            }
            stat->tcam_corrupt_map_size += corrupt_size;
            sal_memset(corrupt, 0, corrupt_size);
        }
#endif


#if ((defined BCM_XGS_SWITCH_SUPPORT ) || (defined BCM_SAND_SUPPORT))
        if (_SOC_MEM_CHK_L2_MEM(mem)) {
            l2 = 1;
        }
        /* NOTE: This is a temporary way of enabling WB mem cache reload
         *       case by case on different devices as support is being implemented.
         *       Eventually this check should go away or be replaced by a feature.
         */
        if (!(SOC_IS_TD_TT(unit) || SOC_IS_ENDURO(unit) || SOC_IS_KATANAX(unit) || SOC_IS_DPP(unit) ||
             (SOC_IS_TRIUMPH3(unit)) || SOC_IS_GREYHOUND(unit) || 
             SOC_IS_HURRICANE3(unit))) {
            
            load_cache = 0;
        }
        /* Pre-load cache from hardware table (if quickly-doable) OR
           we are doing Warm-Boot */
#ifdef BCM_PETRA_SUPPORT
        if ((soc_feature(unit, soc_feature_table_dma) &&
             (soc_mem_dmaable(unit, mem, blk) || SOC_IS_ARAD(unit)) &&
             (SOC_CONTROL(unit)->soc_flags & SOC_F_INITED) && !l2) ||
            (SOC_WARM_BOOT(unit) && load_cache &&
             (SOC_CONTROL(unit)->mem_scache_ptr == NULL))) {
#else
        if ((soc_feature(unit, soc_feature_table_dma) &&
             soc_mem_dmaable(unit, mem, blk) &&
             (SOC_CONTROL(unit)->soc_flags & SOC_F_INITED) && !l2) ||
            (SOC_WARM_BOOT(unit) && load_cache &&
             (SOC_CONTROL(unit)->mem_scache_ptr == NULL))) {
#endif
            uint32 *table;
            int index, index_max = index_cnt - 1, elem;
            int index_min = soc_mem_index_min(unit, mem);
            uint32 entry[SOC_MAX_MEM_WORDS];

#if defined(BCM_TOMAHAWK_SUPPORT)
            if (SOC_IS_TOMAHAWKX(unit)) {
                if (soc_mem_is_cam(unit, mem) &&
                    SOC_TH_MEM_CHK_TCAM_GLOBAL_UNIQUE_MODE(mem)) {
                    int mem_mode = _SOC_SER_MEM_MODE_GLOBAL;
                    int rv = soc_th_check_hw_global_mode(unit, mem,
                                                         &mem_mode);
                    if (SOC_FAILURE(rv)) {
                        /* can skip read for this mem */
                        sal_memset(cache, 0, cache_size);
                        /* valid bits will remain 0 for skipped mems */
                        LOG_ERROR(BSL_LS_SOC_SER,
                            (BSL_META_U(unit,
                                        "soc_mem_cache_set: Will skip "
                                        "WB_cache_fill for mem %s "
                                        "cache=%p size=%d vmap=%p\n"),
                             SOC_MEM_NAME(unit, mem),
                             (void *)cache, cache_size, (void *)vmap));
                        goto iter_done;
                    } else if (_SOC_SER_MEM_MODE_PIPE_UNIQUE == mem_mode) {
                        /* must skip read for this mem */
                        sal_memset(cache, 0, cache_size);
                        /* valid bits will remain 0 for skipped mems */
                        LOG_VERBOSE(BSL_LS_SOC_SER,
                            (BSL_META_U(unit,
                                        "soc_mem_cache_set: Will skip "
                                        "WB_cache_fill for mem %s "
                                        "cache=%p size=%d vmap=%p\n"),
                             SOC_MEM_NAME(unit, mem),
                             (void *)cache, cache_size, (void *)vmap));
                        goto iter_done;
                    }
                    /* proceed to read the mem - normal flow */
                }
            }
#endif
            if ((table = soc_cm_salloc(unit, cache_size/numels, "dma")) != NULL) {
                if (SOC_IS_TRIUMPH3(unit) &&
                    ((mem == PORT_EHG_RX_TUNNEL_MASKm) ||
                     (mem == PORT_EHG_RX_TUNNEL_DATAm) ||
                     (mem == PORT_EHG_TX_TUNNEL_DATAm))) {
                    
                    if (blk == 12 || blk == 13) {
                        index_cnt = 16;
                        index_max = index_cnt - 1;
                    }
                }

#if defined(BCM_TOMAHAWK_SUPPORT)
                if (SOC_IS_TOMAHAWKX(unit)) {
                    if (_SOC_MEM_CHK_IFP_TCAM(mem) ||
                         _SOC_MEM_CHK_IFP_TCAM_WIDE(mem)) {

                        /* read ifp_tcam using per_slice_dma (which may skip
                         * reading of slices based on width, so first cleanup
                         * 'table' */
                        LOG_VERBOSE(BSL_LS_SOC_SER,
                            (BSL_META_U(unit,
                                        "soc_mem_cache_set: "
                                        "WB_cache_fill for mem %s "
                                        "using soc_th_ifp_tcam_dma_read "
                                        "cache=%p size=%d vmap=%p\n"),
                             SOC_MEM_NAME(unit, mem),
                             (void *)cache, cache_size, (void *)vmap));
                        sal_memset(table, 0, cache_size/numels);
                        if (soc_th_ifp_tcam_dma_read(unit, mem, blk, table, 0) >= 0) {
                            /* When done reading all slices */
                            sal_memcpy(cache, table, cache_size);

                            /* Set all vmap bits valid - even for skipped slices */
                            sal_memset(&vmap[index_min / 8], 0xff,
                                       (index_cnt + 7 - index_min) / 8);

                            /* Clear invalid bits at the left and right ends */
                            vmap[index_min / 8] &= 0xff00 >> (8 - index_min % 8);
                            vmap[vmap_size - 1] &= 0x00ff >> ((8 - index_cnt % 8) % 8);

                        }
                        /* done with read of table, with/without error */
                        soc_cm_sfree(unit, table);
                        goto iter_done;
                    }
                }
#endif /* BCM_TOMAHAWK_SUPPORT */
#if defined(BCM_TRIDENT2_SUPPORT)
                _SOC_MEM_SKIP_READING_MEM_TO_CACHE(unit, mem, skip);
                if (skip == 1) {
                    sal_memset(cache, 0, cache_size);
                    memState->vmap[blk] = vmap;
                    memState->cache[blk] = cache;
                    soc_cm_sfree(unit, table);
                    continue;
                }
#endif
                LOG_VERBOSE(BSL_LS_SOC_SER, (BSL_META_U(unit,
                    "cache_rebuild: memory %s \n"),
                    SOC_MEM_NAME(unit, mem)));

                if (numels > 1) {
                    for (elem = 0 ; elem < numels ; elem++) {
                         if (soc_mem_array_read_range(unit, mem, elem, blk, index_min, index_max,
                                               &table[index_min * entry_dw]) >= 0) {
                            uint32 first_read_elem = elem * index_cnt;
                            uint32 not_read_elem = first_read_elem + index_cnt;

                            sal_memcpy(cache + (index_cnt * entry_dw * elem), table, cache_size/numels);

                            /* Set all vmap bits as valid */
                            if (first_read_elem % 8) {
                                vmap[first_read_elem/8] |= ((uint8)(0xff >> (first_read_elem % 8)));
                            }
                            if (not_read_elem % 8) {
                                vmap[not_read_elem/8] |= ((uint8)(0xff << (8 - (not_read_elem % 8))));
                            }
                            first_read_elem = (first_read_elem + 7)/8;
                            not_read_elem /= 8;

                            if (not_read_elem > first_read_elem) {
                                sal_memset(vmap + first_read_elem, 0xff, not_read_elem - first_read_elem);
                            }
                        }
                    }
                } else if (soc_mem_read_range(unit, mem, blk, index_min, index_max,
                                       &table[index_min * entry_dw]) >= 0) {

                    sal_memcpy(cache, table, cache_size);

                    /* Set all vmap bits as valid */
                    sal_memset(&vmap[index_min / 8], 0xff,
                               (index_cnt + 7 - index_min) / 8);

#ifdef BCM_KATANA2_SUPPORT
                    /* In case of SP memories, out of SP0, SP1, SP2 & SP3
                     * only SP0 is valid i.e. only indices 0,4,8.This check
                     * is avoid processing invalid indices. Issue is handled
                     * for THDI memories alone. Has to be handled
                     * for other SP memories in future */
                    /* For PG memories, please note that for ports supporting 
                     * only one PG, there is only one location on the memory,
                     * eg. Port 6, Pg[0..7] will all point to a same location in the memory */
                    if (SOC_IS_KATANA2(unit)) {
                        if (mem == THDIEMA_THDI_PORT_SP_CONFIGm ||
                                mem == THDIEXT_THDI_PORT_SP_CONFIGm ||
                                mem == THDIQEN_THDI_PORT_SP_CONFIGm ||
                                mem == THDIRQE_THDI_PORT_SP_CONFIGm ||
                                mem == THDI_PORT_SP_CONFIGm) {
                            sal_memset(&vmap[index_min / 8], 0x11,
                                    (index_cnt + 7 - index_min) / 8);
                        } else if ((mem == THDIEMA_THDI_PORT_PG_CONFIGm ||
                                    mem == THDIEXT_THDI_PORT_PG_CONFIGm ||
                                    mem == THDIQEN_THDI_PORT_PG_CONFIGm ||
                                    mem == THDIRQE_THDI_PORT_PG_CONFIGm ||
                                    mem == THDI_PORT_PG_CONFIGm )) {
                            sal_memset(&vmap[index_min / 8], 0xff,
                                    (index_cnt + 7 - index_min) / 8);
                                    
                            if (!SOC_IS_SABER2(unit)) {
                                index = index_min / 8;
                                idx = 0;
                                for (port = 0  ; port <= 41; port++) {
                                     if (port_index[idx] == port) {
                                         sal_memset(&vmap[index], 0x01,
                                                1);
                                         idx++;
                                     }
                                     index++;
                                }
                            }     
                        }

                    }
#endif

                    /* Clear invalid bits at the left and right ends */
                    vmap[index_min / 8] &= 0xff00 >> (8 - index_min % 8);
                    vmap[vmap_size - 1] &= 0x00ff >> ((8 - index_cnt % 8) % 8);
                }
                if (SOC_IS_TD_TT(unit) && (mem == FP_GLOBAL_MASK_TCAMm) 
                                  && (SOC_SWITCH_BYPASS_MODE(unit) != 
                                         SOC_SWITCH_BYPASS_MODE_L3_AND_FP)) {
                    /* The read of FP_GLOBAL_MASK_TCAM only returns the
                     * X-pipe portions of the pbmp.  We need the Y-pipe
                     * portions also to match what was available
                     * during Cold Boot.  So we explicitly read the
                     * Y-pipe table here and OR the bitmaps together
                     * to form the complete entry that existed in
                     * the Cold Boot cache.
                     */
                    if (soc_mem_read_range(unit, FP_GLOBAL_MASK_TCAM_Ym,
                                           blk, index_min, index_max,
                                       &table[index_min * entry_dw]) >= 0) {
                        soc_pbmp_t pbmp_x, pbmp_y;
                        uint32 *entry_x, *entry_y;
                        for (index = index_min;
                             index <= index_max;
                             index++) {
                            /* FP_GLOBAL_MASK_TCAM[_X|_Y] have the same
                             * field structure, so we can use the field
                             * accessors for just the base memory. */
                            entry_x = cache + index * entry_dw;
                            entry_y = &table[index_min * entry_dw] +
                                index * entry_dw;

                            soc_mem_pbmp_field_get(unit, mem, entry_x,
                                                   IPBM_MASKf, &pbmp_x);
                            SOC_PBMP_AND(pbmp_x, PBMP_XPIPE(unit));
                            soc_mem_pbmp_field_get(unit, mem, entry_y,
                                                   IPBM_MASKf, &pbmp_y);
                            SOC_PBMP_AND(pbmp_y, PBMP_YPIPE(unit));
                            SOC_PBMP_OR(pbmp_x, pbmp_y);
                            soc_mem_pbmp_field_set(unit, mem, entry_x,
                                                   IPBM_MASKf, &pbmp_x);

                            soc_mem_pbmp_field_get(unit, mem, entry_x,
                                                   IPBMf, &pbmp_x);
                            SOC_PBMP_AND(pbmp_x, PBMP_XPIPE(unit));
                            soc_mem_pbmp_field_get(unit, mem, entry_y,
                                                   IPBMf, &pbmp_y);
                            SOC_PBMP_AND(pbmp_y, PBMP_YPIPE(unit));
                            SOC_PBMP_OR(pbmp_x, pbmp_y);
                            soc_mem_pbmp_field_set(unit, mem, entry_x,
                                                   IPBMf, &pbmp_x);
                        }
                    }
                }
                soc_cm_sfree(unit, table);
                if (_SOC_MEM_CHK_L2_MEM(mem)) {
                    for (index = index_min; index <= index_max; index++) {
                        sal_memcpy(entry, cache + index*entry_dw, entry_dw * 4);
                        if ((mem == L2_ENTRY_2m &&
                            (soc_mem_field32_get(unit, mem, entry, STATIC_BIT_0f) &&
                             soc_mem_field32_get(unit, mem, entry, STATIC_BIT_1f))) ||
                            ((mem == L2Xm || mem == L2_ENTRY_1m) &&
                             soc_mem_field32_get(unit, mem, entry, STATIC_BITf))) {
                            /* Continue */
                        } else {
                            CACHE_VMAP_CLR(vmap, index);
                        }
                    }
                } else if (_SOC_MEM_CHK_L3_MEM(mem)) {
                    uint32 *centry;

                    for (index = index_min; index <= index_max; index++) {
                        centry = cache + index*entry_dw;
                        sal_memcpy(entry, centry, entry_dw * 4);
                        _soc_mem_l3_entry_cache_entry_validate(unit, mem, entry, vmap, &index);
                        if (SOC_MEM_FIELD_VALID(unit, mem, HITf)) {
                            soc_mem_field32_set(unit, mem, centry, HITf, 0);
                        } else if (SOC_MEM_FIELD_VALID(unit, mem, HIT_1f)) {
                            soc_mem_field32_set(unit, mem, centry, HIT_0f, 0);
                            soc_mem_field32_set(unit, mem, centry, HIT_1f, 0);
                        }
                        if (SOC_MEM_FIELD_VALID(unit, mem, HIT_3f)) {
                            soc_mem_field32_set(unit, mem, centry, HIT_2f, 0);
                            soc_mem_field32_set(unit, mem, centry, HIT_3f, 0);
                        }
                    }
                }
#if defined(BCM_TOMAHAWK_SUPPORT)
                else if (_SOC_MEM_CHK_FPEM_MEM(mem)) {
                    uint32 *centry;

                    for (index = index_min; index <= index_max; index++) {
                        centry = cache + index*entry_dw;
                        sal_memcpy(entry, centry, entry_dw * 4);
                        _soc_mem_fpem_entry_cache_entry_validate(unit, mem, entry, vmap, &index);
                        soc_mem_field32_set(unit, mem, centry, HIT_0f, 0);
                        if (SOC_MEM_FIELD_VALID(unit, mem, HIT_1f)) {
                            soc_mem_field32_set(unit, mem, centry, HIT_1f, 0);
                        }
                    }
                }
#endif /* BCM_TOMAHAWK_SUPPORT */
                else if (_SOC_MEM_CHK_EGR_IP_TUNNEL(mem)) {
                    uint32 *centry;

                    for (index = index_min; index <= index_max; index++) {
                        centry = cache + index*entry_dw;
                        sal_memcpy(entry, centry, entry_dw * 4);
                        if (soc_mem_field32_get(unit, mem, entry, ENTRY_TYPEf) == 0) {
                            CACHE_VMAP_CLR(vmap, index);
                        }
                        if (mem == EGR_IP_TUNNELm) {
                            if (soc_mem_field32_get(unit, mem, entry, ENTRY_TYPEf) != 1) {
                                CACHE_VMAP_CLR(vmap, index);
                            }
                        } else if (mem == EGR_IP_TUNNEL_MPLSm) {
                            if (soc_mem_field32_get(unit, mem, entry, ENTRY_TYPEf) != 3) {
                                CACHE_VMAP_CLR(vmap, index);
                            }
                        } else if (mem == EGR_IP_TUNNEL_IPV6m) {
                            if (soc_mem_field32_get(unit, mem, entry, ENTRY_TYPEf) != 2) {
                                CACHE_VMAP_CLR(vmap, index);
                            }
                        }
                    }
                }
#ifdef BCM_TRIUMPH3_SUPPORT               
                else if (SOC_IS_TRIUMPH3(unit) && _SOC_MEM_CHK_MPLS_MEM(mem)) {
                    uint32 *centry;
                    
                    for (index = index_min; index <= index_max; index++) {
                        centry = cache + index*entry_dw;
                        sal_memcpy(entry, centry, entry_dw * 4);
                        if (mem == MPLS_ENTRYm || mem == MPLS_ENTRY_1m) {
                            if (soc_mem_field32_get(unit, mem, entry, VALIDf) == 0) {
                                CACHE_VMAP_CLR(vmap, index);
                            } else {
                                switch (soc_mem_field32_get(unit, mem, entry, KEY_TYPEf)) {
                                    case 0x06: /* 0x06 = L2GRE_SIP - key = {SIP}. */
                                    case 0x07: /* 0x07 = L2GRE_VPNID - key = {GRE.VPNID}. */
                                    case 0x08: /* 0x08 = L2GRE_VPNID_SIP - key = {GRE.VPNID, SIP}.*/
                                    case 0x10: /* 0x10 = MPLS_FIRST_PASS - key = {MPLS_LABEL, GLP}. */
                                    case 0x12: /* 0x12 = MPLS_SECOND_PASS - key = {MPLS_LABEL, GLP}. */
                                        break;
                                    case 0x11: /* 0x11 = MPLS_FIRST_PASS - key = {MPLS_LABEL, GLP}. */
                                    case 0x13: /* 0x13 = MPLS_SECOND_PASS - key = {MPLS_LABEL, GLP}. */
                                    case 0x17: /* 0x17 = MIM_NVP - key = {MAC-in-MAC B-VID, B-SA}. */
                                    case 0x18: /* 0x18 = MIM_ISID - key = {MAC-in-MAC ISID}. */
                                    case 0x19: /* 0x19 = MIM_ISID_SVP - key = {MAC-in-MAC ISID, B-VID, B-SA}. */
                                    case 0x1a: /* 0x1A = TRILL - key = {RBRIDGE_NICKNAME}.  */
                                    default:
                                        CACHE_VMAP_CLR(vmap, index);
                                        break;
                                }
                            }   
                        }  else if (mem == MPLS_ENTRY_EXTDm) {
                            if ((soc_mem_field32_get(unit, mem, entry, VALID_0f) == 0) ||
                                (soc_mem_field32_get(unit, mem, entry, VALID_1f) == 0)) {
                                CACHE_VMAP_CLR(vmap, index);
                            } else if (soc_mem_field32_get(unit, mem, entry, KEY_TYPE_0f) !=
                                          soc_mem_field32_get(unit, mem, entry, KEY_TYPE_1f)) {
                                CACHE_VMAP_CLR(vmap, index);
                            } else {
                                switch (soc_mem_field32_get(unit, mem, entry, KEY_TYPE_0f)) {
                                    case 0x06: /* 0x06 = L2GRE_SIP - key = {SIP}. */
                                    case 0x07: /* 0x07 = L2GRE_VPNID - key = {GRE.VPNID}. */
                                    case 0x08: /* 0x08 = L2GRE_VPNID_SIP - key = {GRE.VPNID, SIP}.*/
                                    case 0x10: /* 0x10 = MPLS_FIRST_PASS - key = {MPLS_LABEL, GLP}. */
                                    case 0x12: /* 0x12 = MPLS_SECOND_PASS - key = {MPLS_LABEL, GLP}. */
                                        CACHE_VMAP_CLR(vmap, index);
                                        break;
                                    case 0x11: /* 0x11 = MPLS_FIRST_PASS - key = {MPLS_LABEL, GLP}. */
                                    case 0x13: /* 0x13 = MPLS_SECOND_PASS - key = {MPLS_LABEL, GLP}. */
                                    case 0x17: /* 0x17 = MIM_NVP - key = {MAC-in-MAC B-VID, B-SA}. */
                                    case 0x18: /* 0x18 = MIM_ISID - key = {MAC-in-MAC ISID}. */
                                    case 0x19: /* 0x19 = MIM_ISID_SVP - key = {MAC-in-MAC ISID, B-VID, B-SA}. */
                                    case 0x1a: /* 0x1A = TRILL - key = {RBRIDGE_NICKNAME}.  */
                                        break;
                                    default:
                                        CACHE_VMAP_CLR(vmap, index);
                                        break;
                                }
                            }
                        }
                    }
                }
#endif  /* BCM_TRIUMPH3_SUPPORT */
                 else if (SOC_MEM_SER_CORRECTION_TYPE(unit, mem) &
                           SOC_MEM_FLAG_SER_WRITE_CACHE_RESTORE) {
                    uint32 *centry;

                    for (index = index_min; index <= index_max; index++) {
                        centry = cache + index*entry_dw;
                        if (SOC_MEM_FIELD_VALID(unit, mem, HITf)) {
                            soc_mem_field32_set(unit, mem, centry, HITf, 0);
                        } else if (SOC_MEM_FIELD_VALID(unit, mem, HIT0f)) {
                            soc_mem_field32_set(unit, mem, centry, HIT0f, 0);
                        } else if (SOC_MEM_FIELD_VALID(unit, mem, HIT1f)) {
                            soc_mem_field32_set(unit, mem, centry, HIT1f, 0);
                        } else if (SOC_MEM_FIELD_VALID(unit, mem, HIT_0f)) {
                            soc_mem_field32_set(unit, mem, centry, HIT_0f, 0);
                        } else if (SOC_MEM_FIELD_VALID(unit, mem, HIT_1f)) {
                            soc_mem_field32_set(unit, mem, centry, HIT_1f, 0);
                        } else if (SOC_MEM_FIELD_VALID(unit, mem, HIT_2f)) {
                            soc_mem_field32_set(unit, mem, centry, HIT_2f, 0);
                        } else if (SOC_MEM_FIELD_VALID(unit, mem, HIT_3f)) {
                            soc_mem_field32_set(unit, mem, centry, HIT_3f, 0);
                        } else if (SOC_MEM_FIELD_VALID(unit, mem, HIT_BITSf)) {
                            soc_mem_field32_set(unit, mem, centry, HIT_BITSf, 0);
                        }
                    }
                }
            }
        }
#endif /* BCM_XGS_SWITCH_SUPPORT */

#ifdef BCM_TOMAHAWK_SUPPORT
iter_done:
#endif
        LOG_INFO(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "soc_mem_cache_set: cache=%p size=%d vmap=%p\n"),
                  (void *)cache, cache_size, (void *)vmap));

        memState->vmap[blk] = vmap;
        memState->cache[blk] = cache;
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_HELIX4_SUPPORT) || \
    defined(BCM_KATANA2_SUPPORT) || defined(BCM_HURRICANE2_SUPPORT) || \
    defined(BCM_GREYHOUND_SUPPORT)
        if((SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) &&
           (!(SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_EXT_CAM))) {
            memState->corrupt[blk] = corrupt;
        }
#endif
        if (org_mem != mem) {
            /* Share the cache and vmap */
            SOC_MEM_STATE(unit, org_mem).cache[blk] = cache;
            SOC_MEM_STATE(unit, org_mem).vmap[blk] = vmap;
        } else if (_SOC_MEM_IS_REUSED_MEM(unit, mem)) {
            /* Sync the cache and vmap */
            _SOC_MEM_SYNC_MAPPED_MEM_STATES(unit, mem);
        }
    }

#ifdef BCM_TRIDENT2_SUPPORT
#ifdef BCM_WARM_BOOT_SUPPORT
    if ((SOC_IS_TRIDENT2X(unit) || SOC_IS_TITAN2X(unit))&&
        (mem == FP_GLOBAL_MASK_TCAMm || mem == FP_GM_FIELDSm)) {
        int rv;
        if ((rv = soc_mem_overlay_tcam_cache_set
            (unit, mem, copyno, enable)) != SOC_E_NONE) {
             MEM_UNLOCK(unit, mem);
             return rv;
        }
    }
#endif
#endif

    MEM_UNLOCK(unit, mem);

    /*
     * COVERITY
     *
     * This is a false positive alarm.
     * corrupt pointer is assigned to memState->corrupt[blk].
     */
    /* coverity[leaked_storage] */
    return SOC_E_NONE;
}

/*
 * Function:
 *    soc_mem_cache_invalidate
 * Purpose:
 *    Invalidate a cache entry. Forces read from the device instead
 *      of from cached copy.
 */

int
soc_mem_cache_invalidate(int unit, soc_mem_t mem, int copyno, int index)
{
    int blk;
    uint8 *vmap;

    assert(SOC_UNIT_VALID(unit));
    _SOC_MEM_REUSE_MEM_STATE(unit, mem);
    SOC_MEM_ALIAS_TO_ORIG(unit,mem);
    assert(SOC_MEM_IS_VALID(unit, mem));

    if (!soc_mem_is_cachable(unit, mem)) {
        return SOC_E_UNAVAIL;
    }

    if (index < soc_mem_index_min(unit, mem) ||
    index > soc_mem_index_max(unit, mem)) {
        LOG_WARN(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "soc_mem_cache_invalidate: invalid index %d "
                             "for memory %s\n"),
                  index, SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    MEM_LOCK(unit, mem);

    SOC_MEM_BLOCK_ITER(unit, mem, blk) {
        if (copyno != SOC_BLOCK_ALL && copyno != blk) {
            continue;
        }

        if (SOC_MEM_STATE(unit, mem).cache[blk] == NULL) {
            continue;
        }

        /* Invalidate the cached entry */

        vmap = SOC_MEM_STATE(unit, mem).vmap[blk];
        CACHE_VMAP_CLR(vmap, index);
    }

    MEM_UNLOCK(unit, mem);

    return SOC_E_NONE;
}

/*
 * Function:
 *      _soc_mem_bits_move_one_byte()
 * Purpose:
 *      same as _soc_mem_bits_move, but only for a single byte.
 * Parameters:
 *      dst_ptr          - (IN)  bit stream address.
 *      dst_first        - (IN)  starting bit after the address pointed to by dst_ptr.
 *      src              - (IN)  src byte.
 *      src_first        - (IN)  starting bit in src byte.
 *      nbits            - (IN)  the number of conntiguous bits to be move.
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 * The following constraints are kept:
 *      a. dst_first < 8
 *      b. src_first < 8
 *      c. dst_first + nbits <= 8
 *      d. src_first + nbits <= 8
 */

static void
_soc_mem_bits_move_one_byte(uint8 *dst_ptr, int dst_first, uint8 src, int src_first, int nbits)
{
    uint8 data;
    uint8 mask;

    /* no need to check that dst_first == 0 and src_first == 0,
       It must be becuse of the constrains */
    if ((nbits) == 8) {
        *(dst_ptr) = src;
        return;
    }
    /* get the data */
    data = src >> (src_first);
    /* align the data to the place it may be inserted */
    data <<= (dst_first);

    mask = ~0;
    mask >>= 8 - nbits;
    mask <<= dst_first;
    data &= mask; /* clear possile bits in src above src_first + range */
    *(dst_ptr) &= ~mask;
    *(dst_ptr) |= data;
}

/*
 * Function:
 *      soc_mem_bits_move()
 * Purpose:
 *      same as _soc_mem_bits_move, but only for a single byte.
 * Parameters:
 *      dst_ptr          - (IN)  dst bit stream address.
 *      dst_first        - (IN)  starting bit after the address pointed to by dst_ptr.
 *      src_ptr          - (IN)  src bit stream address.
 *      src_first        - (IN)  starting bit after the address pointed to by src_ptr.
 *      nbits            - (IN)  the number of conntiguous bits to be move.
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */

static void
_soc_mem_bits_move(uint8 *dst_ptr, int dst_first, uint8 *src_ptr, int src_first, int nbits)
{
    if (nbits <= 0) {
        return;
    }

    if ((((dst_first) & 0x7) == 0) &&
        (((src_first) & 0x7) == 0) &&
        (((nbits) & 0x7) == 0)) {
            sal_memcpy(&((dst_ptr)[(dst_first) >> 3]),
                &((src_ptr)[(src_first) >> 3]),
                ((nbits) >> 3));
    } else {
        uint8 *cur_dst;
        uint8 *cur_src;
        int woff_src, woff_dst, wremain;

        cur_dst = (dst_ptr) + ((dst_first) >> 3);
        cur_src = (src_ptr) + ((src_first) >> 3);

        woff_src = src_first & 0x7;
        woff_dst = dst_first & 0x7;

        if (woff_dst >= woff_src) {
            wremain = 8 - woff_dst;
        } else {
            wremain = 8 - woff_src;
        }
        if (nbits <= wremain) {
            _soc_mem_bits_move_one_byte(cur_dst, woff_dst, *cur_src, woff_src, nbits);
            return;
        }
        _soc_mem_bits_move_one_byte(cur_dst, woff_dst, *cur_src, woff_src, wremain);
        nbits -= wremain;

        while (nbits >= 8) {
            if (woff_dst >= woff_src) {
                ++cur_dst;
                wremain = woff_dst - woff_src;
                if(wremain > 0) {
                    _soc_mem_bits_move_one_byte(cur_dst, 0, *cur_src, 8 - wremain, wremain);
                }
            } else {
                ++cur_src;
                wremain = woff_src - woff_dst;
                _soc_mem_bits_move_one_byte(cur_dst, 8 - wremain, *cur_src, 0, wremain);
            }
            nbits -= wremain;
            wremain = 8 - wremain;
            if (woff_dst >= woff_src) {
                ++cur_src;
                _soc_mem_bits_move_one_byte(cur_dst, 8 - wremain, *cur_src, 0, wremain);
            } else {
                ++cur_dst;
                _soc_mem_bits_move_one_byte(cur_dst,
                                                0,
                                                *cur_src,
                                                8 - wremain,
                                                wremain);
            }
            nbits -= wremain;
        }

        if (woff_dst >= woff_src) {
            ++cur_dst;
            wremain = woff_dst - woff_src;
            if (nbits <= wremain) {
                if(nbits > 0) {
                    _soc_mem_bits_move_one_byte(cur_dst, 0, *cur_src, 8 - wremain,  nbits);
                }
                return;
            }
            if(wremain > 0) {
                _soc_mem_bits_move_one_byte(cur_dst, 0, *cur_src, 8 - wremain, wremain);
            }
        } else {
            ++cur_src;
            wremain = woff_src - woff_dst;
            if (nbits <= wremain) {
                if(nbits > 0) {
                    _soc_mem_bits_move_one_byte(cur_dst, 8 - wremain, *cur_src, 0, nbits);
                }
                return;
            }
            _soc_mem_bits_move_one_byte(cur_dst,
                                            8 - wremain,
                                            *cur_src,
                                            0,
                                            wremain);
        }
        nbits -= wremain;

        if(nbits > 0) {
            wremain = 8 - wremain;
            if (woff_dst >= woff_src) {
                ++cur_src;
                _soc_mem_bits_move_one_byte(cur_dst, 8 - wremain, *cur_src, 0, nbits);
            } else {
                ++cur_dst;
                _soc_mem_bits_move_one_byte(cur_dst, 0, *cur_src, 8 - wremain, nbits);
            }
        }
    }
}


/*
 *    Move Cached old entries (block) to new location and invalidate the old entries.
 *    Imitate HW Support to move block of entries.
 */
int soc_mem_cache_block_move(int        unit,               /* unit of the memory */
                             uint32     flags,              /* flags */
                             soc_mem_t  mem,                /* Memory to be written to */
                             unsigned   src_arr_index,      /* min array index to be written to, not used in memories which are not arrays */
                             unsigned   dest_arr_index,     /* max array index to be written to, not used in memories which are not arrays */
                             int        copyno,             /* Memory block to write to */
                             int        src_index_start,    /* start memory index to read from and to invalidate */
                             int        dest_index_start,    /* start memory index to write */
                             int        entries_count)      /* number of entries to move (Range) */
{
    int     blk, index;
    int     entry_dw        = soc_mem_entry_words(unit, mem);
    int     block_size_byte = WORDS2BYTES(entries_count * entry_dw);
    int     src_index_end   = src_index_start + entries_count - 1;
    int     dest_index_end  = dest_index_start + entries_count - 1;
    int     src_mem_array_vmap_offset, src_mem_array_cache_offset;
    int     dest_mem_array_vmap_offset, dest_mem_array_cache_offset;
    int     invalidate_index_start, invalidate_index_end;
    uint32  *cache;
    uint8   *vmap;
    uint8   *tmp_cache_block;
    uint8   *tmp_vmap;


    LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
            (BSL_META_U(unit,
                    "%s: unit %d, memory %s, block %s, copyno %d, src_index_start %d, \ndest_index_start %d, entries_count %d, entry_dw=%d, \nind_min=%d, ind_max=%d\n  src_arr_index=%d, dest_arr_index=%d\n"),
                    FUNCTION_NAME(), unit, SOC_MEM_UFNAME(unit, mem), SOC_BLOCK_NAME(unit, copyno),
                    copyno, src_index_start, dest_index_start, entries_count, entry_dw, soc_mem_index_min(unit, mem), soc_mem_index_max(unit, mem), src_arr_index, dest_arr_index));

    if (!soc_mem_is_valid(unit, mem))
    {
        return SOC_E_MEMORY;
    }

    if (!soc_mem_is_cachable(unit, mem))
    {
        /* If cache is not available just return */
        return SOC_E_NONE;
    }

    if ((entries_count < 0) || (entries_count > soc_mem_index_count(unit, mem)))
    {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                (BSL_META_U(unit,
                        "%s: invalid entries_count=%d for memory %s\n"), FUNCTION_NAME(), entries_count, SOC_MEM_NAME( unit, mem)));
        return SOC_E_PARAM;
    }

    if (((src_index_start < soc_mem_index_min(unit, mem))
            || (src_index_end > soc_mem_index_max(unit, mem)))
            || ((dest_index_start < soc_mem_index_min(unit, mem))
            || (dest_index_end > soc_mem_index_max(unit, mem))))
    {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                (BSL_META_U(unit, "%s: invalid index for memory %s\n"), FUNCTION_NAME(), SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    if ((tmp_cache_block = sal_alloc(block_size_byte, "tmp_cache_block")) == NULL)
    {
        return SOC_E_MEMORY;
    }

    src_mem_array_vmap_offset = src_arr_index * soc_mem_index_count(unit, mem);
    src_mem_array_cache_offset = src_mem_array_vmap_offset * entry_dw;
    dest_mem_array_vmap_offset = dest_arr_index * soc_mem_index_count(unit, mem);
    dest_mem_array_cache_offset = dest_mem_array_vmap_offset * entry_dw;

    if ((tmp_vmap = sal_alloc(((src_mem_array_vmap_offset + src_index_start + entries_count + 7) / 8), "tmp_vmap")) == NULL)
    {
        sal_free(tmp_cache_block);
        return SOC_E_MEMORY;
    }

    /* Calculate the index range for invalidation */
    if ((src_index_start > dest_index_start) && (src_index_start <= dest_index_end) && (src_arr_index == dest_arr_index))
    {
        /* overlapping move up */
        invalidate_index_start  = dest_index_end + 1;
        invalidate_index_end    = src_index_end;
    }
    else if((dest_index_start > src_index_start) && (dest_index_start <= src_index_end) && (src_arr_index == dest_arr_index))
    {
        /* overlapping move down */
        invalidate_index_start  = src_index_start;
        invalidate_index_end    = dest_index_start - 1;
    }
    else
    {
        /* not-overlapping move */
        invalidate_index_start  = src_index_start;
        invalidate_index_end    = src_index_end;
    }

    /* Copy cache entries + invalidate old entries */
    MEM_LOCK(unit, mem);

    SOC_MEM_BLOCK_ITER(unit, mem, blk)
    {
        if (copyno != SOC_BLOCK_ALL && copyno != blk)
        {
            continue;
        }

        cache = SOC_MEM_STATE(unit, mem).cache[blk];
        vmap = SOC_MEM_STATE(unit, mem).vmap[blk];

        if (cache == NULL)
        {
            continue;
        }

        /* Copy cache entries from old location to new location. We are doing it in 2 copies since the blocks can overlap. */
        sal_memcpy(tmp_cache_block, cache + src_mem_array_cache_offset+ (src_index_start * entry_dw), block_size_byte);
        sal_memcpy(cache + dest_mem_array_cache_offset + (dest_index_start * entry_dw), tmp_cache_block, block_size_byte);

        /* Copy vmap from old location to new location. We are doing it in 2 copies since the blocks can overlap. */
        sal_memcpy(tmp_vmap, vmap, ((src_mem_array_vmap_offset + src_index_start + entries_count + 7) / 8));
        _soc_mem_bits_move(vmap, dest_mem_array_vmap_offset + dest_index_start, tmp_vmap, src_mem_array_vmap_offset + src_index_start, entries_count);

        /* Invalidate the cached entry */
        for (index = invalidate_index_start; index <= invalidate_index_end; index++)
        {
            CACHE_VMAP_CLR(vmap, src_mem_array_vmap_offset + index);
        }
    }

    MEM_UNLOCK(unit, mem);

    sal_free(tmp_cache_block);
    sal_free(tmp_vmap);

    return SOC_E_NONE;

}


#ifdef BCM_WARM_BOOT_SUPPORT

/*
 *     Mem cache structure:
 * +++++++++++++++++++++++++++
 * |  Mem 0: Name string     | -> 128 characters
 * +++++++++++++++++++++++++++
 * |                         |
 * |        Mem table        |
 * |                         |
 * |                         |
 * +++++++++++++++++++++++++++
 * |      Valid bitmap       |
 * +++++++++++++++++++++++++++
 *            ....
 *            ....
 *            ....
 *            ....
 * +++++++++++++++++++++++++++
 * |  Mem n: Name string     |
 * +++++++++++++++++++++++++++
 * |                         |
 * |        Mem table        |
 * |                         |
 * |                         |
 * +++++++++++++++++++++++++++
 * |      Valid bitmap       |
 * +++++++++++++++++++++++++++
 * |                         |
 * |       Reg cache         |
 * |                         |
 * +++++++++++++++++++++++++++
 */

int
soc_mem_scache_size_get(int unit)
{
    int size = 0;
#if defined(BCM_XGS_SUPPORT) || defined(BCM_SIRIUS_SUPPORT)
    int count;
    soc_ser_reg_cache_info(unit, &count, &size);
#endif
    return size + SOC_STAT(unit)->mem_cache_size +
           SOC_STAT(unit)->mem_cache_count*128;
}

int
soc_mem_cache_scache_init(int unit)
{
    uint32 alloc_get;
    soc_scache_handle_t scache_handle;
    uint8 *mem_scache_ptr;
    int rv, stable_size, alloc_sz = soc_mem_scache_size_get(unit);

    /* This code is not executed for now, but will enable it in future
       if required */
    if (0 /*NULL == SOC_CONTROL(unit)->mem_scache_ptr*/) {
        SOC_IF_ERROR_RETURN(soc_stable_size_get(unit, &stable_size));
        if ((stable_size > SOC_DEFAULT_LVL2_STABLE_SIZE) &&
            !SOC_WARM_BOOT_SCACHE_IS_LIMITED(unit)) {
            SOC_SCACHE_HANDLE_SET(scache_handle, unit,
                                  SOC_SCACHE_MEMCACHE_HANDLE, 0);
            /* Get the pointer for the Level 2 cache */
            rv = soc_scache_ptr_get(unit, scache_handle,
                                    &mem_scache_ptr, &alloc_get);
            if (!SOC_WARM_BOOT(unit) && (SOC_E_NOT_FOUND == rv)) {
                /* Not yet allocated in Cold Boot */
                SOC_IF_ERROR_RETURN
                    (soc_scache_alloc(unit, scache_handle,
                                      alloc_sz + SOC_WB_SCACHE_CONTROL_SIZE));
                rv = soc_scache_ptr_get(unit, scache_handle,
                                        &mem_scache_ptr, &alloc_get);
                if (SOC_FAILURE(rv)) {
                    return rv;
                } else if (alloc_get != alloc_sz +
                                        SOC_WB_SCACHE_CONTROL_SIZE) {
                    /* Expected size doesn't match retrieved size */
                    return SOC_E_INTERNAL;
                } else if (NULL == mem_scache_ptr) {
                    return SOC_E_MEMORY;
                }
                SOC_CONTROL(unit)->mem_scache_ptr = mem_scache_ptr;
            }
        }
    }
    return SOC_E_NONE;
}

int
soc_mem_cache_scache_sync(int unit)
{
    uint32 *cache;
    int blk, entry_dw, index_cnt;
    int cache_size, vmap_size, offset= 0;
    soc_mem_t mem, tmp_mem;
    uint8 *vmap, *mem_scache_ptr = SOC_CONTROL(unit)->mem_scache_ptr;

    if (NULL == mem_scache_ptr) {
        return SOC_E_UNAVAIL;
    }
    for (mem = 0; mem < NUM_SOC_MEM; mem++) {
        if (!SOC_MEM_IS_VALID(unit, mem)) {
            continue;
        }
        if (soc_mem_index_count(unit, mem) == 0) {
            continue;
        }
        tmp_mem = mem;
        _SOC_MEM_REUSE_MEM_STATE(unit, tmp_mem);
        if (tmp_mem != mem) {
            continue;
        }
        entry_dw = soc_mem_entry_words(unit, mem);
        index_cnt = soc_mem_index_count(unit, mem);
        cache_size = index_cnt * entry_dw * 4;
        vmap_size = (index_cnt + 7) / 8;

        MEM_LOCK(unit, mem);
        SOC_MEM_BLOCK_ITER(unit, mem, blk) {
            if (SOC_MEM_STATE(unit, mem).cache[blk] == NULL) {
                continue;
            }
            cache = SOC_MEM_STATE(unit, mem).cache[blk];
            vmap = SOC_MEM_STATE(unit, mem).vmap[blk];

            sal_memcpy(mem_scache_ptr+offset, SOC_MEM_UFNAME(unit, mem),
                       strlen(SOC_MEM_UFNAME(unit, mem)));
            LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
                        (BSL_META_U(unit,
                                    "Store at %d %s\n"), offset, SOC_MEM_UFNAME(unit, mem)));
            offset += 128;
            sal_memcpy(mem_scache_ptr+offset, cache, cache_size);
            offset += cache_size;
            sal_memcpy(mem_scache_ptr+offset, vmap, vmap_size);
            offset += vmap_size;
        }
        MEM_UNLOCK(unit, mem);
    }
    return SOC_E_NONE;
}

int
soc_mem_cache_scache_get(int unit)
{
    uint32 alloc_get;
    soc_scache_handle_t scache_handle;
    uint8 *mem_scache_ptr;
    int stable_size;

    /* This code is not executed for now, but will enable it in future
       if required */
    if (0 /*NULL == SOC_CONTROL(unit)->mem_scache_ptr*/) {
        SOC_IF_ERROR_RETURN(soc_stable_size_get(unit, &stable_size));
        if ((stable_size > SOC_DEFAULT_LVL2_STABLE_SIZE) &&
            !SOC_WARM_BOOT_SCACHE_IS_LIMITED(unit)) {
            SOC_SCACHE_HANDLE_SET(scache_handle, unit,
                                  SOC_SCACHE_MEMCACHE_HANDLE, 0);
            /* Get the pointer for the Level 2 cache */
            SOC_IF_ERROR_RETURN
                (soc_scache_ptr_get(unit, scache_handle,
                                    &mem_scache_ptr, &alloc_get));
            if (alloc_get == 0) {
                return SOC_E_INTERNAL;
            } else if (NULL == mem_scache_ptr) {
                return SOC_E_MEMORY;
            }
            SOC_CONTROL(unit)->mem_scache_ptr = mem_scache_ptr;
        }
    }
    return SOC_E_NONE;
}

int
soc_mem_cache_scache_load(int unit, soc_mem_t mem, int *offset)
{
    int blk, entry_dw, index_cnt;
    int cache_size, vmap_size;
    uint32 *cache;
    uint8 *vmap, *mem_scache_ptr = SOC_CONTROL(unit)->mem_scache_ptr;

    if (SOC_WARM_BOOT(unit) && SOC_CONTROL(unit)->mem_scache_ptr) {
        entry_dw = soc_mem_entry_words(unit, mem);
        index_cnt = soc_mem_index_count(unit, mem);
        cache_size = index_cnt * entry_dw * 4;
        vmap_size = (index_cnt + 7) / 8;

        SOC_MEM_BLOCK_ITER(unit, mem, blk) {
            if (SOC_MEM_STATE(unit, mem).cache[blk] == NULL) {
                continue;
            }
            cache = SOC_MEM_STATE(unit, mem).cache[blk];
            vmap = SOC_MEM_STATE(unit, mem).vmap[blk];
            
            LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
                        (BSL_META_U(unit,
                                    "Load from %d %s to %s\n"), *offset,
                         mem_scache_ptr + *offset, SOC_MEM_UFNAME(unit, mem)));
            *offset += 128;
            sal_memcpy(cache, mem_scache_ptr + *offset, cache_size);
            *offset += cache_size;
            sal_memcpy(vmap, mem_scache_ptr + *offset, vmap_size);
            *offset += vmap_size;
        }
    }
    return SOC_E_NONE;
}

int
soc_control_defip_scache_init(int unit)
{
    soc_scache_handle_t scache_handle;
    uint8 *l3_defip_scache_ptr;
    int rv;
    int stable_size;
    uint16 version = SOC_DEFIP_WARMBOOT_DEFAULT_VERSION;
    int alloc_sz = sizeof(SOC_L3_DEFIP_MAX_128B_ENTRIES(unit)) +
                   sizeof(version);
    int create = SOC_WARM_BOOT(unit) ? FALSE : TRUE;

    SOC_IF_ERROR_RETURN(soc_stable_size_get(unit, &stable_size));
    if (!stable_size) {
        /* If stable not allocated, just return */
        return SOC_E_NONE;
    }
    SOC_SCACHE_ALIGN_SIZE(alloc_sz);

    alloc_sz += SOC_WB_SCACHE_CONTROL_SIZE;

    if (!SOC_WARM_BOOT_SCACHE_IS_LIMITED(unit)) {
        SOC_SCACHE_HANDLE_SET(scache_handle, unit,
                              SOC_SCACHE_SOC_DEFIP_HANDLE, 0);
        /* Get the pointer for the Level 2 cache */
        rv = soc_extended_scache_ptr_get(unit, scache_handle, create,
                                         create ? alloc_sz : 0,
                                         &l3_defip_scache_ptr);
        if (SOC_FAILURE(rv) && (rv != SOC_E_NOT_FOUND)) {
            return rv;
        } else if (SOC_E_NOT_FOUND == rv && !create) {
            /* Not yet allocated in upgrade */
            rv = soc_extended_scache_ptr_get(unit, scache_handle, TRUE,
                                             alloc_sz,
                                             &l3_defip_scache_ptr);
            if (SOC_FAILURE(rv) && (rv != SOC_E_NOT_FOUND)) {
                return rv;
            }
            if (SOC_SUCCESS(rv)) {
                SOC_L3_DEFIP_SCACHE_HANDLE(unit) = l3_defip_scache_ptr;
            }
        } else if (SOC_FAILURE(rv)) {
            return rv;
        } else {
            SOC_L3_DEFIP_SCACHE_HANDLE(unit) = l3_defip_scache_ptr;
        }
    }

    return SOC_E_NONE;
}


int
soc_control_defip_scache_sync(int unit)
{
    uint8 *l3_defip_scache_ptr = SOC_L3_DEFIP_SCACHE_HANDLE(unit);
    int scache_size = sizeof(SOC_L3_DEFIP_MAX_128B_ENTRIES(unit));
    int stable_size = 0;
    uint16 version = SOC_DEFIP_WARMBOOT_DEFAULT_VERSION;

    SOC_IF_ERROR_RETURN(soc_stable_size_get(unit, &stable_size));
    if (!stable_size) {
        /* If stable not allocated, just return */
        return SOC_E_NONE;
    }
    if (NULL == l3_defip_scache_ptr) {
        return SOC_E_UNAVAIL;
    }

    /* First save version */
    sal_memcpy(l3_defip_scache_ptr, &version, sizeof(version));
    sal_memcpy(l3_defip_scache_ptr + sizeof(version),
               &SOC_L3_DEFIP_MAX_128B_ENTRIES(unit),
               scache_size);
    return SOC_E_NONE;
}

int
soc_control_defip_scache_load(int unit, int *num_ipv6_128b_entries)
{
    uint8 *l3_defip_scache_ptr = NULL;
    int scache_size = sizeof(SOC_L3_DEFIP_MAX_128B_ENTRIES(unit));
    int stable_size = 0;
    uint16 version = 0;

    SOC_IF_ERROR_RETURN(soc_stable_size_get(unit, &stable_size));
    if (!stable_size) {
        /* If stable not allocated, just return */
        return SOC_E_RESOURCE;
    }

    l3_defip_scache_ptr = SOC_L3_DEFIP_SCACHE_HANDLE(unit);
    if (l3_defip_scache_ptr == NULL) {
        return SOC_E_RESOURCE;
    }
    if (SOC_WARM_BOOT(unit)) {
        sal_memcpy(&version, l3_defip_scache_ptr, sizeof(version));
        if (version >= SOC_DEFIP_WARMBOOT_VERSION_1) {
            sal_memcpy(num_ipv6_128b_entries,
                       l3_defip_scache_ptr + sizeof(version),
                       scache_size);
        } else {
            return SOC_E_UNAVAIL;
        }
    }
    return SOC_E_NONE;
}
#endif /* BCM_WARM_BOOT_SUPPORT */

/************************************************************************/
/* Routines for CFAPINIT                                 */
/************************************************************************/

#ifdef BCM_ESW_SUPPORT
/*
 * Function:
 *    cfapinit_write_cb (internal)
 * Purpose:
 *    Memory test callback routine for cfapinit
 */

static int
cfapinit_write_cb(struct soc_mem_test_s *parm,
                  unsigned array_index,
                  int copyno,
                  int index,
                  uint32 *entry_data)
{
    return soc_mem_array_write(parm->unit, parm->mem, array_index, copyno,
                               index, entry_data);
}

/*
 * Function:
 *    cfapinit_read_cb (internal)
 * Purpose:
 *    Memory test callback routine for cfapinit
 */

static int
cfapinit_read_cb(struct soc_mem_test_s *parm,
                 unsigned array_index,
                 int copyno,
                 int index,
                 uint32 *entry_data)
{
    return soc_mem_array_read(parm->unit, parm->mem, array_index, copyno, index,
                              entry_data);
}

/*
 * Function:
 *    cfapinit_miscompare_cb (internal)
 * Purpose:
 *    Memory test callback routine for cfapinit
 */

static int
cfapinit_miscompare_cb(struct soc_mem_test_s *parm,
                       unsigned array_index,
                       int copyno,
                       int index,
                       uint32 *read_data,
                       uint32 *wrote_data,
                       uint32 *mask_data)
{
    uint8    *bad_list = parm->userdata;

    COMPILER_REFERENCE(array_index);
    COMPILER_REFERENCE(copyno);
    COMPILER_REFERENCE(read_data);
    COMPILER_REFERENCE(wrote_data);
    COMPILER_REFERENCE(mask_data);

    bad_list[index - parm->index_start] = 1;

    return 1;
}

#endif /* BCM_ESW_SUPPORT */
/*
 * Function:
 *    soc_mem_cfap_init
 * Purpose:
 *    Routine to diagnose and initialize CFAP pool.
 * Notes:
 *    Ordinarily this routine is not used.  When the chip is reset, it
 *    automatically writes the CFAP table with the correct pointer
 *    data.
 *
 *    This routine does the same thing, except it also runs a memory
 *    test and excludes bad entries in CBPHEADER/CBPDATA[0-3] from the
 *    CFAP pool.
 *
 *    On some devices, errors in the first 63 entries can't be eliminated.
 *    The routine fails if such errors are found.
 */

#ifdef BCM_ESW_SUPPORT
int
soc_mem_cfap_init(int unit)
{
    soc_mem_test_t    mt;
    uint8        *bad_list;
    int            entry_count, bad_count, bad_count_63;
    int            i, ptr, rv;
    uint32        cfappoolsize;
    soc_mem_t           cbpcellheader_m;
    soc_mem_t           cbppktheader_start_m;
    soc_mem_t           cbppktheader_end_m;
    soc_mem_t           cbpdata_start_m;
    soc_mem_t           cbpdata_end_m;
    soc_mem_t           cfap_m;

    if (!SOC_IS_XGS_SWITCH(unit)) {
        return SOC_E_NONE;
    }

    if (!soc_feature(unit, soc_feature_cfap_pool)) {
        return SOC_E_UNAVAIL;
    }

    cbpcellheader_m      = MMU_CBPCELLHEADERm;
    cbppktheader_start_m = MMU_CBPPKTHEADER0m;
    cbppktheader_end_m   = MMU_CBPPKTHEADER1m;
    cbpdata_start_m      = MMU_CBPDATA0m;
    cbpdata_end_m        = MMU_CBPDATA15m;
    cfap_m               = MMU_CFAPm;
    SOC_IF_ERROR_RETURN(READ_CFAPCONFIGr(unit, &cfappoolsize));
    cfappoolsize = soc_reg_field_get(unit, CFAPCONFIGr,
                                     cfappoolsize, CFAPPOOLSIZEf);

    mt.unit          = unit;
    mt.patterns      = soc_property_get(unit, spn_CFAP_TESTS,
                                        MT_PAT_CHECKER | MT_PAT_ICHECKER);
    mt.copyno        = COPYNO_ALL;
    mt.index_start   = soc_mem_index_min(unit, cbpcellheader_m);
    mt.index_end     = soc_mem_index_max(unit, cbpcellheader_m);
    mt.index_step    = 1;
    mt.read_count    = 1;
    mt.status_cb     = 0;            /* No status */
    mt.write_cb      = cfapinit_write_cb;
    mt.read_cb       = cfapinit_read_cb;
    mt.miscompare_cb = cfapinit_miscompare_cb;

    /*
     * Create array of status per entry, initially with all entries
     * marked good.  The test callback will mark bad entries.
     */

    entry_count = mt.index_end - mt.index_start + 1;

    if ((bad_list = sal_alloc(entry_count, "bad_list")) == NULL) {
        return SOC_E_MEMORY;
    }

    sal_memset(bad_list, 0, entry_count);

    /*
     * Test the five "parallel" memories: CBPHEADER and CBPDATA[0-3].
     */

    mt.userdata = bad_list;
    mt.mem = cbpcellheader_m;
    LOG_VERBOSE(BSL_LS_SOC_COMMON,
                (BSL_META_U(unit,
                            "soc_mem_cfap_init: unit %d: testing CBPHEADER\n"),
                 unit));

    if ((rv = soc_mem_parity_control(unit, mt.mem,
                                     mt.copyno, FALSE)) < 0) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "Unable to disable parity warnings on %s\n"),
                   SOC_MEM_UFNAME(unit, mt.mem)));
        goto done;
    }

    (void) soc_mem_test(&mt);

    if ((rv = soc_mem_parity_control(unit, mt.mem,
                                     mt.copyno, TRUE)) < 0) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "Unable to disable parity warnings on %s\n"),
                   SOC_MEM_UFNAME(unit, mt.mem)));
        goto done;
    }

    for (mt.mem = cbppktheader_start_m;
         mt.mem <= cbppktheader_end_m;
         mt.mem++) {
        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                                "soc_mem_cfap_init: unit %d: testing CBPPKTHEADER%d\n"),
                     unit, mt.mem - cbppktheader_start_m));

        if ((rv = soc_mem_parity_control(unit, mt.mem,
                                         mt.copyno, FALSE)) < 0) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "Unable to disable parity warnings on %s\n"),
                       SOC_MEM_UFNAME(unit, mt.mem)));
            goto done;
        }
        (void) soc_mem_test(&mt);

        if ((rv = soc_mem_parity_control(unit, mt.mem,
                                         mt.copyno, TRUE)) < 0) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "Unable to disable parity warnings on %s\n"),
                       SOC_MEM_UFNAME(unit, mt.mem)));
            goto done;
        }
    }

    for (mt.mem = cbpdata_start_m;
         mt.mem <= cbpdata_end_m;
         mt.mem++) {
        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                                "soc_mem_cfap_init: unit %d: testing CBPDATA%d\n"),
                     unit, mt.mem - cbpdata_start_m));

        if ((rv = soc_mem_parity_control(unit, mt.mem,
                                         mt.copyno, FALSE)) < 0) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "Unable to disable parity warnings on %s\n"),
                       SOC_MEM_UFNAME(unit, mt.mem)));
            goto done;
        }
        (void) soc_mem_test(&mt);

        if ((rv = soc_mem_parity_control(unit, mt.mem,
                                         mt.copyno, TRUE)) < 0) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "Unable to disable parity warnings on %s\n"),
                       SOC_MEM_UFNAME(unit, mt.mem)));
            goto done;
        }
    }

    /*
     * Test only
     for (i = 1; i < 512; i += 2) {
         if (i < entry_count) {
             bad_list[i] = 1;
         }
     }
     */

    bad_count = 0;
    bad_count_63 = 0;

    for (i = mt.index_start; i <= mt.index_end; i++) {
        if (bad_list[i - mt.index_start]) {
            bad_count++;
            if (i < mt.index_start + 63) {
                bad_count_63++;
            }
        }
    }

    if (bad_count_63 > 0) {
        /*
         * On some devices the first 63 entries MUST be good since they are
         * allocated by the hardware on startup.
         */

        LOG_CLI((BSL_META_U(unit,
                            "soc_mem_cfap_init: unit %d: "
                            "Chip unusable, %d error(s) in low entries\n"),
                 unit, bad_count_63));
        sal_free(bad_list);
        return SOC_E_FAIL;
    }

    if (bad_count >= entry_count / 2) {
        LOG_CLI((BSL_META_U(unit,
                            "soc_mem_cfap_init: unit %d: "
                            "Chip unusable, too many bad entries (%d)\n"),
                 unit, bad_count));
        sal_free(bad_list);
        return SOC_E_FAIL;
    }

    /*
     * For each bad entry, swap it with a good entry taken from the end
     * of the list.  This puts all bad entries at the end of the table
     * where they will not be used.
     */

    ptr = mt.index_end + 1;

    for (i = mt.index_start; i < ptr; i++) {
        uint32        e_good, e_bad;

        if (bad_list[i - mt.index_start]) {
            LOG_VERBOSE(BSL_LS_SOC_COMMON,
                        (BSL_META_U(unit,
                                    "soc_mem_cfap_init: unit %d: "
                                    "mapping out CBP index %d\n"),
                         unit, i));

            do {
                --ptr;
            } while (bad_list[ptr - mt.index_start]);

            if ((rv = soc_mem_read(unit, cfap_m,
                                   MEM_BLOCK_ANY, i, &e_bad)) < 0) {
                sal_free(bad_list);
                return rv;
            }

            if ((rv = soc_mem_read(unit, cfap_m,
                                   MEM_BLOCK_ANY, ptr, &e_good)) < 0) {
                sal_free(bad_list);
                return rv;
            }

            if ((rv = soc_mem_write(unit, cfap_m,
                                    MEM_BLOCK_ANY, ptr, &e_bad)) < 0) {
                sal_free(bad_list);
                return rv;
            }

            if ((rv = soc_mem_write(unit, cfap_m,
                                    MEM_BLOCK_ANY, i, &e_good)) < 0) {
                sal_free(bad_list);
                return rv;
            }
        }
    }

    /*
     * Write CFAPPOOLSIZE with the number of good entries (minus 1).
     */

    if (bad_count > 0) {
        uint32  cfap_config = 0;
        uint32  val;
        uint32  cfap_thresh;

        LOG_CLI((BSL_META_U(unit,
                            "soc_mem_cfap_init: unit %d: "
                            "detected and removed %d bad entries\n"),
                 unit, bad_count));

        cfappoolsize -= bad_count;

        soc_reg_field_set(unit, CFAPCONFIGr, &cfap_config,
                          CFAPPOOLSIZEf, cfappoolsize);

        if ((rv = WRITE_CFAPCONFIGr(unit, cfappoolsize)) < 0) {
            sal_free(bad_list);
            return rv;
        }

        if ((rv = READ_CFAPFULLTHRESHOLDr(unit, &val)) < 0) {
            sal_free(bad_list);
            return rv;
        }
        cfap_thresh = soc_reg_field_get(unit, CFAPFULLTHRESHOLDr,
                                        val, CFAPFULLRESETPOINTf);
        cfap_thresh -= bad_count;

        soc_reg_field_set(unit, CFAPFULLTHRESHOLDr, &val,
                          CFAPFULLRESETPOINTf, cfap_thresh);

        cfap_thresh = soc_reg_field_get(unit, CFAPFULLTHRESHOLDr,
                                        val, CFAPFULLSETPOINTf);
        cfap_thresh -= bad_count;
        soc_reg_field_set(unit, CFAPFULLTHRESHOLDr, &val,
                          CFAPFULLSETPOINTf, cfap_thresh);
        if ((rv = WRITE_CFAPFULLTHRESHOLDr(unit, val)) < 0) {
            sal_free(bad_list);
            return rv;
        }
    }

done:

    sal_free(bad_list);

    return SOC_E_NONE;
}

#endif /* BCM_ESW_SUPPORT */


/************************************************************************/
/* Routines for Filter Mask START/COUNT Maintenance                     */
/************************************************************************/

#if defined(BCM_XGS_SUPPORT) || defined(BCM_SIRIUS_SUPPORT) || \
    defined(BCM_CALADAN3_SUPPORT) || defined(BCM_SAND_SUPPORT) ||\
    defined(PORTMOD_SUPPORT)

/************************************************************************
 * Table DMA
 *
 *    In hardware, it shares the same buffers and registers used for ARL DMA
 *    In software, it shares with ARL DMA the same semaphore and interrupt
 *        handler
 * Assumption
 *    ARL DMA and Table DMA should not co-exist. The buffer should be
 *        pre-allocated at least (index_max - index_min + 1) * sizeof (entry)
 *
 ************************************************************************/

#define SOC_MEM_DMA_MAX_DATA_BEATS    4

/*
 * Function:
 *    soc_mem_dmaable
 * Purpose:
 *    Determine whether a table is DMA_able
 * Returns:
 *    0 if not, 1 otherwise
 */
int
soc_mem_dmaable(int unit, soc_mem_t mem, int copyno)
{
    if (SOC_CONTROL(unit)->tdma_enb == 0) {    /* not enabled */
        return FALSE;
    }


#if defined(BCM_RCPU_SUPPORT)
    if(SOC_IS_RCPU_ONLY(unit)) {
        return FALSE;
    }
#endif

#if defined(BCM_DFE_SUPPORT)
    if(SOC_IS_DFE(unit)) {
#ifndef PLISIM
        return TRUE;
#else
        return FALSE;
#endif /* PLISIM */
    }
#endif /* BCM_DFE_SUPPORT */

#if defined(BCM_PETRA_SUPPORT)
    if(SOC_IS_ARAD(unit)) {
#ifndef PLISIM
        if (SOC_IS_ARADPLUS_AND_BELOW(unit) && (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_SCH)) {
            return FALSE;
        }


        return TRUE;
#else
        return FALSE;
#endif /* PLISIM */
    }
#endif /* BCM_PETRA_SUPPORT */

    assert(SOC_MEM_IS_VALID(unit, mem)); /* some devices test id DMA is enabled per memory */

#ifdef BCM_TRIDENT2PLUS_SUPPORT
    if (soc_feature(unit, soc_feature_field_stage_quarter_slice)
        && soc_feature(unit, soc_feature_field_quarter_slice_single_tcam)
        && soc_feature(unit, soc_feature_field_ingress_two_slice_types)) {
        /* Do not use dma for TCAMs with holes */
        if ((FP_TCAMm == mem) || (FP_GM_FIELDSm == mem)
            || (FP_GLOBAL_MASK_TCAMm == mem) || (FP_GLOBAL_MASK_TCAM_Xm == mem)
            || (FP_GLOBAL_MASK_TCAM_Ym == mem) || (FP_COUNTER_TABLEm == mem)) {
            return FALSE;
        }
    }
#endif /* !BCM_TRIDENT2PLUS_SUPPORT */

#ifdef BCM_APACHE_SUPPORT
    if (soc_feature(unit, soc_feature_field_stage_half_slice)
        && soc_feature(unit, soc_feature_field_half_slice_single_tcam)
        && soc_feature(unit, soc_feature_field_ingress_two_slice_types)) {
        /* Do not use dma for TCAMs with holes */
        if ((FP_TCAMm == mem) || (FP_GM_FIELDSm == mem)
            || (FP_GLOBAL_MASK_TCAMm == mem) || (FP_GLOBAL_MASK_TCAM_Xm == mem)
            || (FP_GLOBAL_MASK_TCAM_Ym == mem) || (FP_COUNTER_TABLEm == mem)) {
            return FALSE;
        }
    }
#endif /* !BCM_APACHE_SUPPORT */
    /* Do not use DMA for TCAMs with holes */
    if (soc_feature(unit, soc_feature_field_stage_egress_512_half_slice)) {
        if ((mem == EFP_TCAMm) || (mem == EFP_POLICY_TABLEm)) {
            return FALSE;
        }
    }
    if (soc_feature(unit, soc_feature_field_stage_lookup_512_half_slice)) {
        if ((mem == VFP_TCAMm) || (mem == VFP_POLICY_TABLEm)) {
            return FALSE;
        }
    }

#ifdef ALPM_ENABLE
    if (soc_feature(unit, soc_feature_alpm) &&
        (mem == L3_DEFIP_ALPM_IPV4m || mem == L3_DEFIP_ALPM_IPV4_1m ||
         mem == L3_DEFIP_ALPM_IPV6_64m || mem == L3_DEFIP_ALPM_IPV6_64_1m ||
         mem == L3_DEFIP_ALPM_IPV6_128m)) {
        return FALSE;
    }
#endif

#if defined(BCM_FIREBOLT_SUPPORT)
#if defined(SOC_MEM_L3_DEFIP_WAR) || defined(BCM_TRIUMPH3_SUPPORT) || \
    defined(BCM_ENDURO_SUPPORT) || defined(BCM_KATANA2_SUPPORT) || \
    defined(BCM_KATANA_SUPPORT)
    if (soc_feature(unit, soc_feature_l3_defip_map) &&
        (mem == L3_DEFIPm ||
         mem == L3_DEFIP_ONLYm ||
         mem == L3_DEFIP_DATA_ONLYm ||
         mem == L3_DEFIP_HIT_ONLYm ||
         mem == L3_DEFIP_HIT_ONLY_Xm ||
         mem == L3_DEFIP_HIT_ONLY_Ym)) {
        return FALSE;   /* Could be non-contiguous */
    }
#endif /* SOC_MEM_L3_DEFIP_WAR || BCM_TRIUMPH3_SUPPORT ||
        * BCM_ENDURO_SUPPORT || BCM_KATANA2_SUPPORT*/
#endif /* BCM_FIREBOLT_SUPPORT */

#ifdef BCM_TRIUMPH2_SUPPORT
    if (soc_feature(unit, soc_feature_l3_defip_hole) &&
        (mem == L3_DEFIPm ||
         mem == L3_DEFIP_ONLYm ||
         mem == L3_DEFIP_DATA_ONLYm ||
         mem == L3_DEFIP_HIT_ONLYm)) {
        return FALSE;
    }
    if ((mem == LMEPm) || (mem == LMEP_1m)) {
        return FALSE;
    }
    if (mem == EFP_TCAMm) {
        /* Do not use dma for tcams with holes */
        if (soc_feature(unit, soc_feature_field_stage_half_slice)) {
            if (SOC_IS_TRIUMPH2(unit) || SOC_IS_APOLLO(unit) || SOC_IS_VALKYRIE2(unit))  {
                return FALSE;
            }
        }
     }

#endif /* BCM_TRIUMPH2_SUPPORT */

#if defined(BCM_TRIUMPH3_SUPPORT) || defined(BCM_KATANA2_SUPPORT) || \
    defined(BCM_KATANA_SUPPORT)
    if (soc_feature(unit, soc_feature_l3_defip_map) &&
        (mem == L3_DEFIP_PAIR_128m ||
         mem == L3_DEFIP_PAIR_128_ONLYm ||
         mem == L3_DEFIP_PAIR_128_DATA_ONLYm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLYm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Xm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Ym)) {
        return FALSE;
    }
#endif  /* BCM_TRIUMPH3_SUPPORT  || BCM_KATANA2_SUPPORT*/
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (mem == PORT_EHG_RX_TUNNEL_DATAm || mem == PORT_EHG_RX_TUNNEL_MASKm ||
        mem == PORT_EHG_TX_TUNNEL_DATAm) {
        return FALSE;
    }
#endif /* BCM_TRIUMPH3_SUPPORT*/

#if defined(BCM_TRIUMPH3_SUPPORT) || defined(BCM_HURRICANE2_SUPPORT) || defined(BCM_TRIUMPH2_SUPPORT)
    if (mem == FP_GLOBAL_MASK_TCAMm || mem == FP_TCAMm || mem == EFP_TCAMm) {
        /* Do not use dma for tcams with holes */
        if (soc_feature(unit, soc_feature_field_stage_half_slice) ||
                soc_feature(unit, soc_feature_field_slice_size128)) {
            return FALSE;
        }
    }
#endif /* BCM_TRIUMPH3_SUPPORT || BCM_HURRICANE2_SUPPORT*/

#ifdef BCM_TRX_SUPPORT
    if (SOC_IS_TRX(unit) && !SOC_IS_TD2_TT2(unit) && !SAL_BOOT_BCMSIM) {
        if (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_MMU) {
            switch (mem) {
            case CTR_FLEX_COUNT_0m: case CTR_FLEX_COUNT_1m: case CTR_FLEX_COUNT_2m:
            case CTR_FLEX_COUNT_3m: case CTR_FLEX_COUNT_4m: case CTR_FLEX_COUNT_5m:
            case CTR_FLEX_COUNT_6m: case CTR_FLEX_COUNT_7m: case CTR_FLEX_COUNT_8m:
            case CTR_FLEX_COUNT_9m: case CTR_FLEX_COUNT_10m: case CTR_FLEX_COUNT_11m:
                return TRUE;
            default:
                return FALSE;
            }
        }
    }
#endif /* BCM_TRX_SUPPORT */
    if (!soc_feature(unit, soc_feature_flexible_dma_steps) &&
        soc_mem_index_count(unit, mem) > 1) {
#ifdef BCM_TRIUMPH_SUPPORT
        /* For DNX chips - We can't reach here, and hence the coverity defect is not relevant for DNX chips */
        /* coverity[index:FALSE] */
        if (soc_feature(unit, soc_feature_esm_support) &&
            SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) {
            int index0, index1;
            soc_mem_t real_mem;

            /* On BCM56624_A0, don't do DMA for tables whose associated TCAM
             * entry is wider than a single raw TCAM entry */
            soc_tcam_mem_index_to_raw_index(unit, mem, 0, &real_mem, &index0);
            soc_tcam_mem_index_to_raw_index(unit, mem, 1, &real_mem, &index1);
            if (index1 - index0 != 1) {
                return FALSE;
            }
        }
#endif /* BCM_TRIUMPH_SUPPORT */
    }

#ifdef BCM_TRIDENT_SUPPORT
    if (SOC_IS_TRIDENT(unit) || SOC_IS_TITAN(unit)) {
        /*
         * EGR_VLAN requires to combine entries from both pipe
         * the single entry ISBS_PORT_TO_PIPE_MAPPING and
         * ESBS_PORT_TO_PIPE_MAPPING table need data_beat size adjustment
         */
        if (mem == EGR_VLANm ||
            mem == ISBS_PORT_TO_PIPE_MAPPINGm ||
            mem == ESBS_PORT_TO_PIPE_MAPPINGm) {
            return FALSE;
        }
    }
#endif /* BCM_TRIDENT_SUPPORT */

#ifdef BCM_TRIUMPH3_SUPPORT
    if (SOC_IS_TRIUMPH3(unit)) {
        if (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_IBOD) {
            return FALSE;
        }

    }
    if (soc_feature(unit, soc_feature_field_stage_quarter_slice)) {
        /* Do not use dma for tcams with holes */
        if (mem == FP_GLOBAL_MASK_TCAMm || mem == FP_TCAMm ||
            mem == VFP_TCAMm || mem == EFP_TCAMm || mem == FP_COUNTER_TABLEm) {
            return FALSE;
        }
    }
#endif /* BCM_TRIUMPH3_SUPPORT */

#ifdef BCM_HURRICANE2_SUPPORT
    if (SOC_IS_HURRICANE2(unit) || SOC_IS_GREYHOUND(unit) || 
        SOC_IS_HURRICANE3(unit)) {
        if (mem == EFP_TCAMm) {
            /* Probable Non Contiguous.. Need to Investigate */
            return FALSE;
        }
    }
#endif /* BCM_HURRICANE2_SUPPORT */

#ifdef BCM_SHADOW_SUPPORT
    if (SOC_IS_SHADOW(unit)) {
        /*
         * IL_STAT_MEM_x counters
         */
        if (mem == IL_STAT_MEM_3m ) {
            return FALSE;
        }
    }
#endif /* BCM_SHADOW_SUPPORT */


#ifdef  BCM_XGS3_SWITCH_SUPPORT
    if (SOC_IS_XGS3_SWITCH(unit)) {
        return TRUE;
    }
#endif /* BCM_XGS3_SWITCH_SUPPORT */

#ifdef BCM_SIRIUS_SUPPORT
    if (SOC_IS_SIRIUS(unit)) {
      return TRUE;
    }
#endif /* BCM_SIRIUS_SUPPORT */

#ifdef BCM_CALADAN3_SUPPORT
    if (SOC_IS_CALADAN3(unit)) {
      return TRUE;
    }
#endif /* BCM_CALADAN3_SUPPORT */

    return (soc_mem_entry_words(unit, mem) <= SOC_MEM_DMA_MAX_DATA_BEATS);
}

/*
 * Function:
 *    soc_mem_slamable
 * Purpose:
 *    Determine whether a table is SLAMable
 * Returns:
 *    0 if not, 1 otherwise
 */
int
soc_mem_slamable(int unit, soc_mem_t mem, int copyno)
{
    if (SOC_CONTROL(unit)->tslam_enb == 0) {    /* not enabled */
        return FALSE;
    }

#if defined(BCM_RCPU_SUPPORT)
    if(SOC_IS_RCPU_ONLY(unit)) {
        return FALSE;
    }
#endif

#if defined(BCM_DFE_SUPPORT)
    if(SOC_IS_DFE(unit)) {
#ifndef PLISIM
        return TRUE;
#else
        return FALSE;
#endif /* PLISIM */
    }
#endif /* BCM_DFE_SUPPORT */

#if defined(BCM_PETRA_SUPPORT)
    if (SOC_IS_DPP(unit)) {
#ifndef PLISIM
        return TRUE;
#else
        return FALSE;
#endif /* PLISIM */
    }
#endif /* BCM_PETRA_SUPPORT */

    assert(SOC_MEM_IS_VALID(unit, mem)); /* some devices test id DMA is enabled per memory */

#if defined(BCM_FIREBOLT_SUPPORT)
#if defined(SOC_MEM_L3_DEFIP_WAR) || defined(BCM_TRIUMPH3_SUPPORT) || \
    defined(BCM_ENDURO_SUPPORT) || defined(BCM_KATANA_SUPPORT)
    if (soc_feature(unit, soc_feature_l3_defip_map) &&
        (mem == L3_DEFIPm ||
         mem == L3_DEFIP_ONLYm ||
         mem == L3_DEFIP_DATA_ONLYm ||
         mem == L3_DEFIP_HIT_ONLYm ||
         mem == L3_DEFIP_HIT_ONLY_Xm ||
         mem == L3_DEFIP_HIT_ONLY_Ym)) {
        return FALSE;   /* Could be non-contiguous */
    }
#endif /* SOC_MEM_L3_DEFIP_WAR || BCM_TRIUMPH3_SUPPORT || BCM_ENDURO_SUPPORT */
#endif /* BCM_FIREBOLT_SUPPORT */

#ifdef BCM_TRIUMPH2_SUPPORT
    if (soc_feature(unit, soc_feature_l3_defip_hole) &&
        (mem == L3_DEFIPm ||
         mem == L3_DEFIP_ONLYm ||
         mem == L3_DEFIP_DATA_ONLYm ||
         mem == L3_DEFIP_HIT_ONLYm)) {
        return FALSE;
    }
    if ((mem == LMEPm) || (mem == LMEP_1m)) {
        return FALSE;
    }
#endif /* BCM_TRIUMPH2_SUPPORT */

#if defined(BCM_TRIUMPH3_SUPPORT) || defined(BCM_KATANA2_SUPPORT) || \
    defined(BCM_KATANA_SUPPORT)
    if (soc_feature(unit, soc_feature_l3_defip_map) &&
        (mem == L3_DEFIP_PAIR_128m ||
         mem == L3_DEFIP_PAIR_128_ONLYm ||
         mem == L3_DEFIP_PAIR_128_DATA_ONLYm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLYm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Xm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Ym)) {
        return FALSE;
    }
#endif /* BCM_TRIUMPH3_SUPPORT || BCM_KATANA2_SUPPORT*/

#ifdef BCM_TRX_SUPPORT
    if (SOC_IS_TRX(unit) && !SOC_IS_TD2_TT2(unit)) {
        if (!SAL_BOOT_BCMSIM && SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_MMU) {
            return FALSE;
        }
    }
#endif /* BCM_TRX_SUPPORT */

    if (!soc_feature(unit, soc_feature_flexible_dma_steps) &&
        soc_mem_index_count(unit, mem) > 1) {
#ifdef BCM_TRIUMPH_SUPPORT
        if (soc_feature(unit, soc_feature_esm_support) &&
            SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) {
            int index0, index1;
            soc_mem_t real_mem;

            /* On BCM56624_A0, don't do DMA for tables whose associated TCAM
             * entry is wider than a single raw TCAM entry */
            soc_tcam_mem_index_to_raw_index(unit, mem, 0, &real_mem, &index0);
            soc_tcam_mem_index_to_raw_index(unit, mem, 1, &real_mem, &index1);
            if (index1 - index0 != 1) {
                return FALSE;
            }
        }
#endif /* BCM_TRIUMPH_SUPPORT */
    }
#if defined(BCM_TRIUMPH3_SUPPORT) || defined(BCM_KATANA2_SUPPORT)
    if (SOC_CONTROL(unit)->l3_defip_index_remap &&
        (mem == L3_DEFIP_PAIR_128m || mem == L3_DEFIPm)) {
        return FALSE;
    }
#endif /* BCM_TRIUMPH3_SUPPORT || BCM_KATANA2_SUPPORT*/
    return TRUE;
}

/*
 * Function:
 *    _soc_xgs3_mem_dma
 * Purpose:
 *    DMA acceleration for soc_mem_read_range() on FB/ER
 * Parameters:
 *    buffer -- must be pointer to sufficiently large
 *            DMA-able block of memory
 */
STATIC int
_soc_xgs3_mem_dma(int unit, soc_mem_t mem, unsigned array_index,
                  int copyno, int index_min, int index_max,
                  uint32 ser_flags, void *buffer)
{
    soc_control_t *soc = SOC_CONTROL(unit);
    uint32        start_addr;
    uint32        count;
    uint32        data_beats;
    uint32        spacing;
    int           rv = SOC_E_NONE;
    uint32        cfg, rval;
    soc_mem_t     cmd_mem = INVALIDm;
    uint8         at;
#if (defined  BCM_CMICM_SUPPORT)
    int cmc = SOC_PCI_CMC(unit);
#endif

    /* coverity[var_tested_neg] */
    LOG_INFO(BSL_LS_SOC_DMA,
             (BSL_META_U(unit,
                         "_soc_xgs3_mem_dma: unit %d"
                         " mem %s.%s index %d-%d buffer %p\n"),
              unit, SOC_MEM_UFNAME(unit, mem), SOC_BLOCK_NAME(unit, copyno),
              index_min, index_max, buffer));

    data_beats = soc_mem_entry_words(unit, mem);

    count = index_max - index_min + 1;
    if (count < 1) {
        return SOC_E_NONE;
    }

    if (cmd_mem != INVALIDm) {
        MEM_LOCK(unit, cmd_mem);
    }

    TABLE_DMA_LOCK(unit);

#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_esm_correction) &&
          ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
         (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
        SOC_ESM_LOCK(unit);
    }
#endif

    /* coverity[negative_returns : FALSE] */
    start_addr = soc_mem_addr_get(unit, mem, array_index,
                                  copyno, index_min, &at);

#if defined(BCM_XGS3_SWITCH_SUPPORT)
    if (0 != (ser_flags & _SOC_SER_FLAG_MULTI_PIPE)) {
        uint32 acc_type = ser_flags & _SOC_SER_FLAG_ACC_TYPE_MASK;
        if (0 != acc_type) {
            /* Override ACC_TYPE in address */
            start_addr &= ~(_SOC_MEM_ADDR_ACC_TYPE_MASK <<
                            _SOC_MEM_ADDR_ACC_TYPE_SHIFT);
            start_addr |= (acc_type & _SOC_MEM_ADDR_ACC_TYPE_MASK) <<
                            _SOC_MEM_ADDR_ACC_TYPE_SHIFT;
        }
    }
#endif /* BCM_XGS3_SWITCH_SUPPORT */

#if (defined  BCM_CMICM_SUPPORT)
    if(soc_feature(unit, soc_feature_cmicm)) {
        soc_pci_write(unit, CMIC_CMCx_TABLE_DMA_PCIMEM_START_ADDR_OFFSET(cmc),
                      soc_cm_l2p(unit, buffer));
        soc_pci_write(unit, CMIC_CMCx_TABLE_DMA_SBUS_START_ADDR_OFFSET(cmc),
                      start_addr);
        rval = 0;
        soc_reg_field_set(unit, CMIC_CMC0_TABLE_DMA_ENTRY_COUNTr, &rval,
                          COUNTf, count);
        soc_pci_write(unit, CMIC_CMCx_TABLE_DMA_ENTRY_COUNT_OFFSET(cmc), rval);
    } else
#endif /* CMICM Support */
    {

        WRITE_CMIC_TABLE_DMA_PCIMEM_START_ADDRr(unit,
                                                soc_cm_l2p(unit, buffer));
        WRITE_CMIC_TABLE_DMA_SBUS_START_ADDRr(unit, start_addr);
        rval = 0;
        soc_reg_field_set(unit, CMIC_TABLE_DMA_ENTRY_COUNTr, &rval,
                          COUNTf, count);
        if (soc_feature(unit, soc_feature_flexible_dma_steps) &&
            soc_mem_index_count(unit, mem) > 1) {
#if defined(BCM_TRIUMPH_SUPPORT)
            if (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) {
                int index0, index1, increment;
                soc_mem_t real_mem;

                soc_tcam_mem_index_to_raw_index(unit, mem, 0,
                                                &real_mem, &index0);
                soc_tcam_mem_index_to_raw_index(unit, mem, 1,
                                                &real_mem, &index1);
                increment = _shr_popcount(index1 - index0 - 1);
                soc_reg_field_set(unit, CMIC_TABLE_DMA_ENTRY_COUNTr, &rval,
                                  SBUS_ADDR_INCREMENT_STEPf, increment);
            }
#endif /* BCM_TRIUMPH_SUPPORT */
#if defined(BCM_TRIUMPH3_SUPPORT)
            if (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU) {
                int index0, index1, increment;
                soc_mem_t real_mem;

                soc_tcam_mem_index_to_raw_index(unit, mem, 0,
                                                &real_mem, &index0);
                soc_tcam_mem_index_to_raw_index(unit, mem, 1,
                                                &real_mem, &index1);
                increment = _shr_popcount(index1 - index0 - 1);
                soc_reg_field_set(unit, CMIC_TABLE_DMA_ENTRY_COUNTr, &rval,
                                  SBUS_ADDR_INCREMENT_STEPf, increment);
            }
#endif /* BCM_TRIUMPH3_SUPPORT */
        }
        WRITE_CMIC_TABLE_DMA_ENTRY_COUNTr(unit, rval);
    }
    LOG_INFO(BSL_LS_SOC_DMA,
             (BSL_META_U(unit,
                         "_soc_xgs3_mem_dma: table dma of %d entries "
                         "of %d beats from 0x%x\n"),
              count, data_beats, start_addr));

    /* Set beats. Clear table DMA abort,done and error bit. Start DMA */
#if (defined  BCM_CMICM_SUPPORT)
    if(soc_feature(unit, soc_feature_cmicm)) {
        /* For CMICm retain Endianess etc... */
        cfg = soc_pci_read(unit, CMIC_CMCx_TABLE_DMA_CFG_OFFSET(cmc));
        soc_reg_field_set(unit, CMIC_CMC0_TABLE_DMA_CFGr, &cfg,
                            BEATSf, data_beats);
        soc_reg_field_set(unit, CMIC_CMC0_TABLE_DMA_CFGr, &cfg, ABORTf, 0);
        soc_reg_field_set(unit, CMIC_CMC0_TABLE_DMA_CFGr, &cfg, ENf, 0);
        soc_reg_field_set(unit, CMIC_CMC0_TABLE_DMA_CFGr, &cfg,
                          ENABLE_MULTIPLE_SBUS_CMDSf, 0);
        soc_pci_write(unit, CMIC_CMCx_TABLE_DMA_CFG_OFFSET(cmc), cfg);
        /* Clearing EN clears stats */

        
#ifdef BCM_EXTND_SBUS_SUPPORT
        if (soc_feature(unit, soc_feature_new_sbus_format)) {
            rval = 0;
            /* Use TR3 style sbus access */
            soc_reg_field_set(unit, CMIC_CMC0_TABLE_DMA_SBUS_CMD_CONFIGr,
                              &rval, EN_TR3_SBUS_STYLEf, 1);
            soc_reg_field_set(unit, CMIC_CMC0_TABLE_DMA_SBUS_CMD_CONFIGr,
                              &rval,
                              SBUS_BLOCKIDf, SOC_BLOCK2SCH(unit, copyno));
            soc_reg_field_set(unit, CMIC_CMC0_TABLE_DMA_SBUS_CMD_CONFIGr,
                              &rval,
                              SBUS_ACCTYPEf,
                              SOC_MEM_ACC_TYPE(unit, mem));
            WRITE_CMIC_CMC0_TABLE_DMA_SBUS_CMD_CONFIGr(unit, rval);
        }
#endif
    } else
#endif
    {
        cfg = 0;
        soc_reg_field_set(unit, CMIC_TABLE_DMA_CFGr, &cfg,
                            BEATSf, data_beats);
    }
    if (soc_feature(unit, soc_feature_multi_sbus_cmds)) {
        
        if (soc->sbusCmdSpacing < 0) {
            spacing = data_beats > 7 ? data_beats + 1 : 8;
        } else {
            spacing = soc->sbusCmdSpacing;
        }
        if ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
            (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_XQPORT) ||
            (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_GXPORT) ||
            (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_SPORT) ||
            (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_GPORT) ||
            (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_QGPORT) ||
            (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_LLS) ||
            (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_MMU)) {
            spacing = 0;
        }

#ifdef BCM_KATANA_SUPPORT
       if (SOC_IS_KATANA(unit)) {
           /* disable MOR for EFP_TCAM and EGR_VLAN_XLATE */
           if (mem == EFP_TCAMm || mem == EGR_VLAN_XLATEm) {
               spacing = 0;
           }
       }
#endif

        if (spacing) {
#if (defined  BCM_CMICM_SUPPORT)
            if(soc_feature(unit, soc_feature_cmicm)) {
                soc_reg_field_set(unit, CMIC_CMC0_TABLE_DMA_CFGr, &cfg,
                                  MULTIPLE_SBUS_CMD_SPACINGf, spacing);
                soc_reg_field_set(unit, CMIC_CMC0_TABLE_DMA_CFGr, &cfg,
                                  ENABLE_MULTIPLE_SBUS_CMDSf, 1);

            } else
#endif
            {
                soc_reg_field_set(unit, CMIC_TABLE_DMA_CFGr, &cfg,
                                  MULTIPLE_SBUS_CMD_SPACINGf, spacing);
                soc_reg_field_set(unit, CMIC_TABLE_DMA_CFGr, &cfg,
                                  ENABLE_MULTIPLE_SBUS_CMDSf, 1);
            }
        }
    }
#if (defined  BCM_CMICM_SUPPORT)
    if(soc_feature(unit, soc_feature_cmicm)) {
        soc_reg_field_set(unit, CMIC_CMC0_TABLE_DMA_CFGr, &cfg, ENf, 1);
        soc_pci_write(unit, CMIC_CMCx_TABLE_DMA_CFG_OFFSET(cmc), cfg);
    } else
#endif
    {
        soc_reg_field_set(unit, CMIC_TABLE_DMA_CFGr, &cfg, ENf, 1);
        WRITE_CMIC_TABLE_DMA_CFGr(unit, cfg);
    }
    rv = SOC_E_TIMEOUT;
    if (soc->tableDmaIntrEnb) {
#if (defined  BCM_CMICM_SUPPORT)
        if(soc_feature(unit, soc_feature_cmicm)) {
            soc_cmicm_intr0_enable(unit, IRQ_CMCx_TDMA_DONE);
            if (TABLE_DMA_INTR_WAIT(unit, soc->tableDmaTimeout) < 0) {
                rv = SOC_E_TIMEOUT;
            }
            soc_cmicm_intr0_disable(unit, IRQ_CMCx_TDMA_DONE);

            cfg = soc_pci_read(unit, CMIC_CMCx_TABLE_DMA_STAT_OFFSET(cmc));
            if (soc_reg_field_get(unit, CMIC_CMC0_TABLE_DMA_STATr,
                                                        cfg, DONEf)) {
                rv = SOC_E_NONE;
                if (soc_reg_field_get(unit, CMIC_CMC0_TABLE_DMA_STATr,
                                                        cfg, ERRORf)) {
                    rv = SOC_E_FAIL;
                }
            }
        } else
#endif
        {
            soc_intr_enable(unit, IRQ_TDMA_DONE);
            if (TABLE_DMA_INTR_WAIT(unit, soc->tableDmaTimeout) < 0) {
                rv = SOC_E_TIMEOUT;
            }
            soc_intr_disable(unit, IRQ_TDMA_DONE);

            READ_CMIC_TABLE_DMA_CFGr(unit, &cfg);
            if (soc_reg_field_get(unit, CMIC_TABLE_DMA_CFGr,
                    cfg, DONEf)) {
                rv = SOC_E_NONE;
                if (soc_reg_field_get(unit, CMIC_TABLE_DMA_CFGr,
                        cfg, ERRORf)) {
                    rv = SOC_E_FAIL;
                }
            }
        }
    } else {
        soc_timeout_t to;

        soc_timeout_init(&to, soc->tableDmaTimeout, 10000);

        do {
#if (defined  BCM_CMICM_SUPPORT)
            if(soc_feature(unit, soc_feature_cmicm)) {
                cfg = soc_pci_read(unit, CMIC_CMCx_TABLE_DMA_STAT_OFFSET(cmc));
                if (soc_reg_field_get(unit, CMIC_CMC0_TABLE_DMA_STATr,
                                                        cfg, DONEf)) {
                    rv = SOC_E_NONE;
                    if (soc_reg_field_get(unit, CMIC_CMC0_TABLE_DMA_STATr,
                                                        cfg, ERRORf)) {
                        rv = SOC_E_FAIL;
                    }
                    break;
                }
            } else
#endif
            {
                READ_CMIC_TABLE_DMA_CFGr(unit, &cfg);
                if (soc_reg_field_get(unit, CMIC_TABLE_DMA_CFGr,
                        cfg, DONEf)) {
                    rv = SOC_E_NONE;
                    if (soc_reg_field_get(unit, CMIC_TABLE_DMA_CFGr,
                            cfg, ERRORf)) {
                        rv = SOC_E_FAIL;
                    }
                    break;
                }
             }
        } while(!(soc_timeout_check(&to)));
    }

    if (rv < 0) {
        if (rv != SOC_E_TIMEOUT) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "%s.%s failed(NAK)\n"),
                       SOC_MEM_UFNAME(unit, mem),
                       SOC_BLOCK_NAME(unit, copyno)));
#ifdef BCM_TRIUMPH2_SUPPORT
            if (SOC_IS_TRIUMPH2(unit) || SOC_IS_APOLLO(unit) ||
                SOC_IS_VALKYRIE2(unit)) {
                /* SCHAN nack */
                sal_dpc(soc_triumph2_mem_nack, INT_TO_PTR(unit),
                        INT_TO_PTR(start_addr), 0, 0, 0);
            }
#endif /* BCM_TRIUMPH2_SUPPORT */
        } else {
            soc_timeout_t to;

            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "TableDmaTimeOut: %s.%s %s timeout\n"),
                       SOC_MEM_UFNAME(unit, mem),
                       SOC_BLOCK_NAME(unit, copyno),
                       soc->tableDmaIntrEnb ? "interrupt" : "polling"));

#if (defined  BCM_CMICM_SUPPORT)
            if(soc_feature(unit, soc_feature_cmicm)) {
                /* Abort Table DMA */
                cfg = soc_pci_read(unit, CMIC_CMCx_TABLE_DMA_CFG_OFFSET(cmc));
                soc_reg_field_set(unit, CMIC_CMC0_TABLE_DMA_CFGr, &cfg, ENf, 0);
                soc_reg_field_set(unit, CMIC_CMC0_TABLE_DMA_CFGr, &cfg, ABORTf, 1);
                soc_pci_write(unit, CMIC_CMCx_TABLE_DMA_CFG_OFFSET(cmc), cfg);

                /* Check the done bit to confirm */
                soc_timeout_init(&to, soc->tableDmaTimeout, 0);
                while (1) {
                    cfg = soc_pci_read(unit, CMIC_CMCx_TABLE_DMA_STAT_OFFSET(cmc));
                    if (soc_reg_field_get(unit, CMIC_CMC0_TABLE_DMA_STATr, cfg, DONEf)) {
                        break;
                    }
                    if (soc_timeout_check(&to)) {
                        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                                  (BSL_META_U(unit,
                                              "TableDmaTimeOut:_soc_xgs3_mem_dma, Abort Failed\n")));
                        break;
                    }
                }
            } else
#endif
            {
                /* Abort Table DMA */
                READ_CMIC_TABLE_DMA_CFGr(unit, &cfg);
                soc_reg_field_set(unit, CMIC_TABLE_DMA_CFGr, &cfg, ENf, 0);
                soc_reg_field_set(unit, CMIC_TABLE_DMA_CFGr, &cfg, ABORTf, 1);
                soc_reg_field_set(unit, CMIC_TABLE_DMA_CFGr, &cfg, DONEf, 0);
                soc_reg_field_set(unit, CMIC_TABLE_DMA_CFGr, &cfg, ERRORf, 0);
                WRITE_CMIC_TABLE_DMA_CFGr(unit, cfg);

                /* Check the done bit to confirm */
                soc_timeout_init(&to, soc->tableDmaTimeout, 0);
                while (1) {
                    READ_CMIC_TABLE_DMA_CFGr(unit, &cfg);
                    if (soc_reg_field_get(unit, CMIC_TABLE_DMA_CFGr, cfg, DONEf)) {
                        break;
                    }
                    if (soc_timeout_check(&to)) {
                        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                                  (BSL_META_U(unit,
                                              "TableDmaTimeOut:_soc_xgs3_mem_dma, Abort Failed\n")));
                        break;
                    }
                }
            }
        }
    }

    soc_cm_sinval(unit, (void *)buffer, WORDS2BYTES(data_beats) * count);

#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_esm_correction) &&
          ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
         (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
        SOC_ESM_UNLOCK(unit);
    }
#endif

    TABLE_DMA_UNLOCK(unit);

    if (cmd_mem != INVALIDm) {
        MEM_UNLOCK(unit, cmd_mem);
    }

    return rv;
}

/*
 * Function:
 *    _soc_xgs3_mem_slam
 * Purpose:
 *    DMA acceleration for soc_mem_write_range() on FB/ER
 * Parameters:
 *    buffer -- must be pointer to sufficiently large
 *            DMA-able block of memory
 *      index_begin <= index_end - Forward direction
 *      index_begin >  index_end - Reverse direction
 */

STATIC int
_soc_xgs3_mem_slam(int unit, uint32 flags, soc_mem_t mem, unsigned array_index, int copyno,
                   int index_begin, int index_end, void *buffer)
{
    soc_control_t *soc = SOC_CONTROL(unit);
    uint32        start_addr;
    uint32        count;
    uint32        data_beats;
    uint32        spacing;
    int           rv = SOC_E_NONE;
    uint32        cfg, rval;
    uint8         at;
#if (defined  BCM_CMICM_SUPPORT)
    int cmc = SOC_PCI_CMC(unit);
#endif

    LOG_INFO(BSL_LS_SOC_DMA,
             (BSL_META_U(unit,
                         "_soc_xgs3_mem_slam: unit %d"
                         " mem %s.%s index %d-%d buffer %p\n"),
              unit, SOC_MEM_UFNAME(unit, mem), SOC_BLOCK_NAME(unit, copyno),
              index_begin, index_end, buffer));

    data_beats = soc_mem_entry_words(unit, mem);

    if (index_begin > index_end) {
        /* coverity[negative_returns : FALSE] */
        start_addr = soc_mem_addr_get(unit, mem, array_index, copyno, index_end, &at);
        count = index_begin - index_end + 1;
    } else {
        /* coverity[negative_returns : FALSE] */
        start_addr = soc_mem_addr_get(unit, mem, array_index, copyno, index_begin, &at);
        count = index_end - index_begin + 1;
    }

    if (!flags || (flags & SOC_MEM_WRITE_SET_ONLY)) {

    #if (defined  BCM_CMICM_SUPPORT)
        if(soc_feature(unit, soc_feature_cmicm)) {
            /* For CMICm retain Endianess etc... */
            cfg = soc_pci_read(unit, CMIC_CMCx_SLAM_DMA_CFG_OFFSET(cmc));
            soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_CFGr, &cfg, ABORTf, 0);
            soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_CFGr, &cfg, ENf, 0);
            soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_CFGr, &cfg,
                              ENABLE_MULTIPLE_SBUS_CMDSf, 0);
            soc_pci_write(unit, CMIC_CMCx_SLAM_DMA_CFG_OFFSET(cmc), cfg);  /* Clearing EN clears the stats */

            /* Set beats. Clear tslam DMA abort,done and error bit. Start DMA */
            soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_CFGr, &cfg, BEATSf, data_beats);

#if defined(BCM958525)
            /* Modulo count for 128B payload size */
            soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_CFGr, &cfg,
                              REV_MODULO_COUNTf, (count % (32 / data_beats)));
#else
            soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_CFGr, &cfg,
                              REV_MODULO_COUNTf, (count % (64 / data_beats)));
#endif
            soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_CFGr, &cfg,
                                ORDERf, (index_begin > index_end) ? 1:0);

    #ifdef BCM_EXTND_SBUS_SUPPORT
            if (soc_feature(unit, soc_feature_new_sbus_format)) {
                rval = 0;
                /* Use TR3 style sbus access */
                soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_SBUS_CMD_CONFIGr, &rval,
                                  EN_TR3_SBUS_STYLEf, 1);
                soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_SBUS_CMD_CONFIGr, &rval,
                                  SBUS_BLOCKIDf, SOC_BLOCK2SCH(unit, copyno));
                soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_SBUS_CMD_CONFIGr, &rval,
                                  SBUS_ACCTYPEf, SOC_MEM_ACC_TYPE(unit, mem));
                WRITE_CMIC_CMC0_SLAM_DMA_SBUS_CMD_CONFIGr(unit, rval);
            }
    #endif
        } else
    #endif
        {
            /* Set beats. Clear tslam DMA abort,done and error bit. Start DMA */
            cfg = 0;
    #if defined(BCM958525)
            /* Modulo count for 128B payload size */
            soc_reg_field_set(unit, CMIC_SLAM_DMA_CFGr, &cfg, REV_MODULO_COUNTf,
                              (count % (32 / data_beats)));
    #else
            soc_reg_field_set(unit, CMIC_SLAM_DMA_CFGr, &cfg, REV_MODULO_COUNTf,
                              (count % (64 / data_beats)));
    #endif
            soc_reg_field_set(unit, CMIC_SLAM_DMA_CFGr, &cfg, BEATSf, data_beats);
            soc_reg_field_set(unit, CMIC_SLAM_DMA_CFGr, &cfg,
                                ORDERf, (index_begin > index_end) ? 1:0);
        }
        if (soc_feature(unit, soc_feature_multi_sbus_cmds)) {
            
            if (soc->sbusCmdSpacing < 0) {
                spacing = data_beats > 7 ? data_beats + 1 : 8;
            } else {
                spacing = soc->sbusCmdSpacing;
            }
            if ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
                (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_XQPORT) ||
                (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_GXPORT) ||
                (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_XLPORT) ||
                (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_SPORT) ||
                (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_GPORT) ||
                (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_QGPORT) ||
                (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_LLS) ||
                (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_MMU)) {
                spacing = 0;
            }

    #ifdef BCM_KATANA_SUPPORT
           if (SOC_IS_KATANA(unit)) {
               /* disable MOR for EFP_TCAM and EGR_VLAN_XLATE */
               if (mem == EFP_TCAMm || mem == EGR_VLAN_XLATEm) {
                   spacing = 0;
               }
           }
    #endif

            if (spacing) {
    #if (defined  BCM_CMICM_SUPPORT)
                if(soc_feature(unit, soc_feature_cmicm)) {
                    soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_CFGr, &cfg,
                                      MULTIPLE_SBUS_CMD_SPACINGf, spacing);
                    soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_CFGr, &cfg,
                                      ENABLE_MULTIPLE_SBUS_CMDSf, 1);
                } else
    #endif
                {
                    soc_reg_field_set(unit, CMIC_SLAM_DMA_CFGr, &cfg,
                                      MULTIPLE_SBUS_CMD_SPACINGf, spacing);
                    soc_reg_field_set(unit, CMIC_SLAM_DMA_CFGr, &cfg,
                                      ENABLE_MULTIPLE_SBUS_CMDSf, 1);
                }
            }
        }

        LOG_INFO(BSL_LS_SOC_DMA,
                 (BSL_META_U(unit,
                             "_soc_xgs3_mem_slam: tslam dma of %d entries "
                             "of %d beats from 0x%x to index %d-%d\n"),
                  count, data_beats, start_addr, index_begin, index_end));

        TSLAM_DMA_LOCK(unit);
#if defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit, soc_feature_esm_correction) &&
              ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
             (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
            SOC_ESM_LOCK(unit);
        }
#endif
        soc_cm_sflush(unit, buffer, WORDS2BYTES(data_beats) * count);
    #if (defined  BCM_CMICM_SUPPORT)
        if(soc_feature(unit, soc_feature_cmicm)) {
            soc_pci_write(unit, CMIC_CMCx_SLAM_DMA_PCIMEM_START_ADDR_OFFSET(cmc),
                          soc_cm_l2p(unit, buffer));
            soc_pci_write(unit, CMIC_CMCx_SLAM_DMA_SBUS_START_ADDR_OFFSET(cmc), start_addr);
            rval = 0;
            soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_ENTRY_COUNTr, &rval, COUNTf, count);
            soc_pci_write(unit, CMIC_CMCx_SLAM_DMA_ENTRY_COUNT_OFFSET(cmc), rval);
            soc_pci_write(unit, CMIC_CMCx_SLAM_DMA_CFG_OFFSET(cmc), cfg);
        } else
    #endif
        {
            WRITE_CMIC_SLAM_DMA_PCIMEM_START_ADDRr(unit, soc_cm_l2p(unit, buffer));
            WRITE_CMIC_SLAM_DMA_SBUS_START_ADDRr(unit, start_addr);
            rval = 0;
            soc_reg_field_set(unit, CMIC_SLAM_DMA_ENTRY_COUNTr, &rval, COUNTf, count);
            if (soc_feature(unit, soc_feature_flexible_dma_steps) &&
                soc_mem_index_count(unit, mem) > 1) {
    #if defined(BCM_TRIUMPH_SUPPORT)
                if (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) {
                    int index0, index1, increment;
                    soc_mem_t real_mem;

                    soc_tcam_mem_index_to_raw_index(unit, mem, 0, &real_mem, &index0);
                    soc_tcam_mem_index_to_raw_index(unit, mem, 1, &real_mem, &index1);
                    increment = _shr_popcount(index1 - index0 - 1);
                    soc_reg_field_set(unit, CMIC_SLAM_DMA_ENTRY_COUNTr, &rval,
                                      SBUS_ADDR_INCREMENT_STEPf, increment);
                }
    #endif /* BCM_TRIUMPH_SUPPORT */
    #if defined(BCM_TRIUMPH3_SUPPORT)
                if (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU) {
                    int index0, index1, increment;
                    soc_mem_t real_mem;

                    soc_tcam_mem_index_to_raw_index(unit, mem, 0, &real_mem, &index0);
                    soc_tcam_mem_index_to_raw_index(unit, mem, 1, &real_mem, &index1);
                    increment = _shr_popcount(index1 - index0 - 1);
                    soc_reg_field_set(unit, CMIC_SLAM_DMA_ENTRY_COUNTr, &rval,
                                      SBUS_ADDR_INCREMENT_STEPf, increment);
                }
    #endif /* BCM_TRIUMPH3_SUPPORT */
            }
            WRITE_CMIC_SLAM_DMA_ENTRY_COUNTr(unit, rval);
            WRITE_CMIC_SLAM_DMA_CFGr(unit, cfg);
        }
    }

    if (!flags || (flags & SOC_MEM_WRITE_COMMIT_ONLY)) {
        cfg = 0;
        /* write ENf bit to SLAM_DMA_CFG register */
        #if (defined  BCM_CMICM_SUPPORT)
        if(soc_feature(unit, soc_feature_cmicm)) {
            cfg = soc_pci_read(unit, CMIC_CMCx_SLAM_DMA_CFG_OFFSET(cmc));
            soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_CFGr, &cfg, ENf, 1);
            soc_pci_write(unit, CMIC_CMCx_SLAM_DMA_CFG_OFFSET(cmc), cfg);
        } else
    #endif
        {
            READ_CMIC_SLAM_DMA_CFGr(unit, &cfg);
            soc_reg_field_set(unit, CMIC_SLAM_DMA_CFGr, &cfg, ENf, 1);
            WRITE_CMIC_SLAM_DMA_CFGr(unit, cfg);
        }
    }

    if (!flags || (flags & SOC_MEM_WRITE_STATUS_ONLY)) {

        rv = SOC_E_TIMEOUT;
        if (soc->tslamDmaIntrEnb) {

    #if (defined  BCM_CMICM_SUPPORT)
            if(soc_feature(unit, soc_feature_cmicm)) {
                soc_cmicm_intr0_enable(unit, IRQ_CMCx_TSLAM_DONE);
                if (TSLAM_DMA_INTR_WAIT(unit, soc->tslamDmaTimeout) < 0) {
                    rv = SOC_E_TIMEOUT;
                }
                soc_cmicm_intr0_disable(unit, IRQ_CMCx_TSLAM_DONE);

                cfg = soc_pci_read(unit, CMIC_CMCx_SLAM_DMA_STAT_OFFSET(cmc));
                if (soc_reg_field_get(unit, CMIC_CMC0_SLAM_DMA_STATr, cfg, DONEf)) {
                    rv = SOC_E_NONE;
                }
                if (soc_reg_field_get(unit, CMIC_CMC0_SLAM_DMA_STATr, cfg, ERRORf)) {
                    rv = SOC_E_FAIL;
                }
            } else
    #endif
            {
                soc_intr_enable(unit, IRQ_TSLAM_DONE);
                if (TSLAM_DMA_INTR_WAIT(unit, soc->tslamDmaTimeout) < 0) {
                    rv = SOC_E_TIMEOUT;
                }
                soc_intr_disable(unit, IRQ_TSLAM_DONE);

                READ_CMIC_SLAM_DMA_CFGr(unit, &cfg);
                if (soc_reg_field_get(unit, CMIC_SLAM_DMA_CFGr, cfg, DONEf)) {
                    rv = SOC_E_NONE;
                }
                if (soc_reg_field_get(unit, CMIC_SLAM_DMA_CFGr, cfg, ERRORf)) {
                    rv = SOC_E_FAIL;
                }
            }
        } else {
            soc_timeout_t to;

            soc_timeout_init(&to, soc->tslamDmaTimeout, 10000);

            do {
    #if (defined  BCM_CMICM_SUPPORT)
                if(soc_feature(unit, soc_feature_cmicm)) {
                    cfg = soc_pci_read(unit, CMIC_CMCx_SLAM_DMA_STAT_OFFSET(cmc));
                    if (soc_reg_field_get(unit, CMIC_CMC0_SLAM_DMA_STATr, cfg, ERRORf)) {
                        rv = SOC_E_FAIL;
                        break;
                    }
                    if (soc_reg_field_get(unit, CMIC_CMC0_SLAM_DMA_STATr, cfg, DONEf)) {
                        rv = SOC_E_NONE;
                        break;
                    }
                } else
    #endif
                {
                    READ_CMIC_SLAM_DMA_CFGr(unit, &cfg);
                    if (soc_reg_field_get(unit, CMIC_SLAM_DMA_CFGr, cfg, ERRORf)) {
                        rv = SOC_E_FAIL;
                        break;
                    }
                    if (soc_reg_field_get(unit, CMIC_SLAM_DMA_CFGr, cfg, DONEf)) {
                        rv = SOC_E_NONE;
                        break;
                    }
                }
            } while (!(soc_timeout_check(&to)));
        }

        if (rv < 0) {
            if (rv != SOC_E_TIMEOUT) {
                LOG_ERROR(BSL_LS_SOC_SOCMEM,
                          (BSL_META_U(unit,
                                      "%s.%s failed(NAK)\n"),
                           SOC_MEM_UFNAME(unit, mem),
                           SOC_BLOCK_NAME(unit, copyno)));
    #ifdef BCM_TRIUMPH2_SUPPORT
                if (SOC_IS_TRIUMPH2(unit) || SOC_IS_APOLLO(unit) ||
                    SOC_IS_VALKYRIE2(unit)) {
                    /* SCHAN nack */
                    soc_triumph2_mem_nack(INT_TO_PTR(unit),
                            INT_TO_PTR(start_addr), 0, 0, 0);
                }
    #endif /* BCM_TRIUMPH2_SUPPORT */
            } else {
                soc_timeout_t to;

                LOG_ERROR(BSL_LS_SOC_SOCMEM,
                          (BSL_META_U(unit,
                                      "SlamDmaTimeOut:%s.%s %s timeout\n"),
                           SOC_MEM_UFNAME(unit, mem),
                           SOC_BLOCK_NAME(unit, copyno),
                           soc->tslamDmaIntrEnb ? "interrupt" : "polling"));

    #if (defined  BCM_CMICM_SUPPORT)
                if(soc_feature(unit, soc_feature_cmicm)) {
                    /* Abort Tslam DMA */
                    cfg = soc_pci_read(unit, CMIC_CMCx_SLAM_DMA_CFG_OFFSET(cmc));
                    soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_CFGr, &cfg, ENf, 0);
                    soc_reg_field_set(unit, CMIC_CMC0_SLAM_DMA_CFGr, &cfg, ABORTf, 1);
                    soc_pci_write(unit, CMIC_CMCx_SLAM_DMA_CFG_OFFSET(cmc), cfg);

                    /* Check the done bit to confirm */
                    soc_timeout_init(&to, soc->tslamDmaTimeout, 10000);
                    while (1) {
                        cfg = soc_pci_read(unit, CMIC_CMCx_SLAM_DMA_STAT_OFFSET(cmc));
                        if (soc_reg_field_get(unit, CMIC_CMC0_SLAM_DMA_STATr, cfg, DONEf)) {
                            break;
                        }
                        if (soc_timeout_check(&to)) {
                            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                                      (BSL_META_U(unit,
                                                  "SlamDmaTimeOut:_soc_xgs3_mem_slam, Abort Failed\n")));
                            break;
                        }
                    }
                } else
    #endif
                {
                    /* Abort Tslam DMA */
                    READ_CMIC_SLAM_DMA_CFGr(unit, &cfg);
                    soc_reg_field_set(unit, CMIC_SLAM_DMA_CFGr, &cfg, ENf, 0);
                    soc_reg_field_set(unit, CMIC_SLAM_DMA_CFGr, &cfg, ABORTf, 1);
                    soc_reg_field_set(unit, CMIC_SLAM_DMA_CFGr, &cfg, DONEf, 0);
                    soc_reg_field_set(unit, CMIC_SLAM_DMA_CFGr, &cfg, ERRORf, 0);
                    WRITE_CMIC_SLAM_DMA_CFGr(unit, cfg);

                    /* Check the done bit to confirm */
                    soc_timeout_init(&to, soc->tslamDmaTimeout, 10000);
                    while (1) {
                        READ_CMIC_SLAM_DMA_CFGr(unit, &cfg);
                        if (soc_reg_field_get(unit, CMIC_SLAM_DMA_CFGr, cfg, DONEf)) {
                            break;
                        }
                        if (soc_timeout_check(&to)) {
                            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                                      (BSL_META_U(unit,
                                                  "SlamDmaTimeOut:_soc_xgs3_mem_slam, Abort Failed\n")));
                            break;
                        }
                    }
                }
            }
        }
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_esm_correction) &&
          ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
         (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
        SOC_ESM_UNLOCK(unit);
    }
#endif
        TSLAM_DMA_UNLOCK(unit);
    }

    return rv;
}

#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) \
    || defined(BCM_HURRICANE2_SUPPORT) || defined(BCM_GREYHOUND_SUPPORT)
STATIC void
_soc_mem_tcam_shift(int unit, uint32 *f1, uint32 *ipbm)
{
    uint32 ipbm_105bit;
    uint32 f1_49bit;
    uint32 carrybit;
    uint32 value;
    uint32 next_carry;
    int word;

    f1_49bit    = (f1[ 1 ] >> 17) & 0x1;
    ipbm_105bit = (ipbm[ 3 ] >> 9 ) & 0x1;
    /*clear ipbm 105th bit*/
    ipbm[ 3 ] &= ((1 << 9) - 1);
    /*clear f1 49 bit*/
    f1[ 1 ] &= ((1 << 17) - 1);
    /*update f1 49th bit*/
    f1 [ 1 ] |= (ipbm_105bit << 17);

    carrybit = f1_49bit;
    for ( word = 0; word < 4; word++ ) {
        value = ipbm[ word ];
        next_carry = (value & (1<<31)) ? 1 : 0 ;
        ipbm[ word ] = (value << 1) | carrybit;
        carrybit = next_carry;
    }
}

STATIC void
_soc_mem_fp_global_mask_tcam_shift(int unit, soc_mem_t mem, uint32 *xy_entry, 
                                   int index_min, int index_max)
{
    int index, count;
    void *buf;
    uint32 f1[SOC_MAX_MEM_FIELD_WORDS], ipbm[SOC_MAX_MEM_FIELD_WORDS];
    
    if (soc_feature(unit, soc_feature_tcam_shift) &&
        ((mem == FP_GLOBAL_MASK_TCAMm) ||
	       (mem == FP_GLOBAL_MASK_TCAM_Xm) ||
	       (mem == FP_GLOBAL_MASK_TCAM_Ym))) {
	      count = 0;
        for (index = index_min; index <= index_max; index++, count++) {
            buf = soc_mem_table_idx_to_pointer(unit, mem, void *,
                                               xy_entry, count);
            sal_memset(f1, 0, sizeof(f1));
            sal_memset(ipbm, 0, sizeof(ipbm));

            /*update key part*/
            soc_mem_field_get(unit, mem, buf, F1f, f1);
            soc_mem_field_get(unit, mem, buf, IPBMf, ipbm);

            _soc_mem_tcam_shift(unit, f1, ipbm);

            soc_mem_field_set(unit, mem, buf, F1f, f1);
            soc_mem_field_set(unit, mem, buf, IPBMf, ipbm);

            /*update mask part*/
            soc_mem_field_get(unit, mem, buf, F1_MASKf, f1);
            soc_mem_field_get(unit, mem, buf, IPBM_MASKf, ipbm);

            _soc_mem_tcam_shift(unit, f1, ipbm);

            soc_mem_field_set(unit, mem, buf, F1_MASKf, f1);
            soc_mem_field_set(unit, mem, buf, IPBM_MASKf, ipbm);
        }
    }
}

void
_soc_mem_tcam_xy_to_dm(int unit, soc_mem_t mem, int count,
                       uint32 *xy_entry, uint32 *dm_entry)
{
    uint32 key[SOC_MAX_MEM_FIELD_WORDS], mask[SOC_MAX_MEM_FIELD_WORDS];
    uint32 converted_key, converted_mask;
    uint32 xor_value;
    soc_field_t key_field[4], mask_field[4];
    int bit_length[4], word_length[4], field_count;
    int index, i, word, data_words, data_bytes;
    soc_pbmp_t pbmp;
    int xy_lpt;

    xor_value = soc_feature(unit, soc_feature_xy_tcam_28nm) ? 0 : 0xffffffff;
    xy_lpt = soc_feature(unit, soc_feature_xy_tcam_lpt) &&
             (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_TCAM_ENCODING_LPT);

    if (mem == L3_DEFIPm || mem == L3_DEFIP_Xm || mem == L3_DEFIP_Ym ||
        mem == L3_DEFIP_ONLYm
        ) {
        if (SOC_MEM_FIELD_VALID(unit, mem, KEY0f)) {
            key_field[0] = KEY0f;
            key_field[1] = KEY1f;
            mask_field[0] = MASK0f;
            mask_field[1] = MASK1f;
            field_count = 2;
        } else {
            key_field[0] = KEYf;
            mask_field[0] = MASKf;
            field_count = 1;
        }
#if defined(BCM_TRIUMPH3_SUPPORT) || defined(BCM_KATANA_SUPPORT) || \
    defined(BCM_TRIDENT2_SUPPORT)
    } else if (mem == L3_DEFIP_PAIR_128m || mem == L3_DEFIP_PAIR_128_ONLYm) {
        key_field[0] = KEY0_UPRf;
        key_field[1] = KEY1_UPRf;
        key_field[2] = KEY0_LWRf;
        key_field[3] = KEY1_LWRf;
        mask_field[0] = MASK0_UPRf;
        mask_field[1] = MASK1_UPRf;
        mask_field[2] = MASK0_LWRf;
        mask_field[3] = MASK1_LWRf;
        field_count = 4;
#endif
    } else {
        if (SOC_MEM_FIELD_VALID(unit, mem, FULL_KEYf)) {
            key_field[0] = FULL_KEYf;
            mask_field[0] = FULL_MASKf;
        } else {
            key_field[0] = KEYf;
            mask_field[0] = MASKf;
        }
        field_count = 1;
#ifdef BCM_SABER2_SUPPORT
        if (SOC_IS_SABER2(unit) && (mem == FP_TCAMm) &&
             soc_feature(unit, soc_feature_xy_tcam_28nm) &&
             !SOC_MEM_FIELD_VALID(unit, mem, FULL_KEYf)) {

            /* Key field doesn't include F4 and double wide mode fields */
            if (SOC_MEM_FIELD_VALID(unit, mem, F4f)) {
                key_field[field_count] = F4f;
                mask_field[field_count] = F4_MASKf;
                field_count += 1;
            }

            if (SOC_MEM_FIELD_VALID(unit, mem, DOUBLE_WIDE_MODEf)) {
                key_field[field_count] = DOUBLE_WIDE_MODEf;
                mask_field[field_count] = DOUBLE_WIDE_MODE_MASKf;
                field_count += 1;
            }
        }
#endif
    }
    for (i = 0; i < field_count; i++) {
        bit_length[i] = soc_mem_field_length(unit, mem, key_field[i]);
        word_length[i] = (bit_length[i] + 31) / 32;
    }
    data_words = soc_mem_entry_words(unit, mem);
    data_bytes = data_words * sizeof(uint32);
    for (index = 0; index < count; index++) {
        if (dm_entry != xy_entry) {
            sal_memcpy(dm_entry, xy_entry, data_bytes);
        }
        for (i = 0; i < field_count; i++) {
            soc_mem_field_get(unit, mem, xy_entry, key_field[i], key);
            soc_mem_field_get(unit, mem, xy_entry, mask_field[i], mask);
            for (word = 0; word < word_length[i]; word++) {
                /*
                 * LPT -> XY
                 * xy_x[1:0] = {(lpt_y[0] & lpt_x[0]), (lpt_y[1] & lpt_y[0])}
                 * xy_y[1:0] = {(lpt_y[1] & lpt_x[1]), (lpt_x[1] & lpt_x[0])}
                 */
                if (xy_lpt) {
                    converted_key = (((key[word] & mask[word]) << 1) & 0xaaaaaaaa) |
                                    ((mask[word] & (mask[word] >> 1)) & 0x55555555);
                    converted_mask = ((key[word] & mask[word]) & 0xaaaaaaaa) |
                                     ((key[word] & (key[word] >> 1)) & 0x55555555);
                    key[word] = converted_key;
                    mask[word] = converted_mask;
                }
                /*
                 * 40nm:
                 *    Encode: K0 = MASK & KEY
                 *            K1 = ~MASK | KEY
                 *    Decode: KEY = K0
                 *            MASK = K0 | ~K1
                 *              (encode)  (decode)
                 *    KEY MASK   K0  K1   KEY MASK
                 *    --------   ------   --------
                 *     0   0     0   1     0   0
                 *     1   0     0   1     0   0  =====> info loss
                 *     0   1     0   0     0   1
                 *     1   1     1   1     1   1
                 * 28nm:
                 *     Encode: K0 = MASK & KEY
                 *             K1 = MASK & ~KEY
                 *     Decode: KEY = K0
                 *             MASK = K0 | K1
                 *     KEY MASK   K0  K1   KEY MASK
                 *     --------   ------   --------
                 *      0   0     0   0     0   0
                 *      1   0     0   0     0   0  =====> info loss
                 *      0   1     0   1     0   1
                 *      1   1     1   0     1   1
                 * Notes:
                 *     - Mask value of 1 means to compare against key
                 *     - K0 is in KEY field of the entry
                 *     - K1 is in MASK field of the entry
                 */
                mask[word] = key[word] | (mask[word] ^ xor_value);
            }
            if ((bit_length[i] & 0x1f) != 0) {
                if (xy_lpt) {
                    key[word - 1] &= (1 << (bit_length[i] & 0x1f)) - 1;
                }
                mask[word - 1] &= (1 << (bit_length[i] & 0x1f)) - 1;
            }
            if (xy_lpt) {
                soc_mem_field_set(unit, mem, dm_entry, key_field[i], key);
            }
            soc_mem_field_set(unit, mem, dm_entry, mask_field[i], mask);
        }

        /* Filter out the bits that is not implemented in the hardware,
         * hardware returns X=0 Y=0 which is D=0 M=1 after above conversion
         * following code will force such bits to D=0 M=0 */
        if (SOC_IS_TD_TT(unit) &&
            (mem == FP_GLOBAL_MASK_TCAM_Xm ||
             mem == FP_GLOBAL_MASK_TCAM_Ym)) {
            soc_mem_pbmp_field_get(unit, mem, dm_entry, IPBM_MASKf, &pbmp);
            if (mem == FP_GLOBAL_MASK_TCAM_Xm) {
                SOC_PBMP_AND(pbmp, PBMP_XPIPE(unit));
            } else {
                SOC_PBMP_AND(pbmp, PBMP_YPIPE(unit));
            }
            soc_mem_pbmp_field_set(unit, mem, dm_entry, IPBM_MASKf, &pbmp);
        }

        xy_entry += data_words;
        dm_entry += data_words;
    }
}

void
_soc_mem_tcam_dm_to_xy(int unit, soc_mem_t mem, int count,
                       uint32 *dm_entry, uint32 *xy_entry,
                       uint32 *cache_entry)
{
    uint32 key[SOC_MAX_MEM_FIELD_WORDS], mask[SOC_MAX_MEM_FIELD_WORDS];
    uint32 xor_value;
    uint32 converted_key, converted_mask;
    soc_field_t key_field[4], mask_field[4];
    int bit_length[4], word_length[4], field_count;
    int index, i, word, data_words, data_bytes;
    int no_trans = FALSE;
    int xy_lpt;

    if (!soc_feature(unit, soc_feature_xy_tcam_direct)) {
        /* Only clear the "don't care" key bits */
        no_trans = TRUE;
    }
    xor_value = soc_feature(unit, soc_feature_xy_tcam_28nm) ? 0xffffffff : 0;
    xy_lpt = soc_feature(unit, soc_feature_xy_tcam_lpt) &&
             (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_TCAM_ENCODING_LPT);

    if (mem == L3_DEFIPm || mem == L3_DEFIP_Xm || mem == L3_DEFIP_Ym ||
        mem == L3_DEFIP_ONLYm
        ) {
        if (SOC_MEM_FIELD_VALID(unit, mem, KEY0f)) {
            key_field[0] = KEY0f;
            key_field[1] = KEY1f;
            mask_field[0] = MASK0f;
            mask_field[1] = MASK1f;
            field_count = 2;
        } else {
            key_field[0] = KEYf;
            mask_field[0] = MASKf;
            field_count = 1;
        }
#if defined(BCM_TRIUMPH3_SUPPORT) || defined(BCM_KATANA_SUPPORT) || \
    defined(BCM_TRIDENT2_SUPPORT)
    } else if (mem == L3_DEFIP_PAIR_128m || mem == L3_DEFIP_PAIR_128_ONLYm) {
        key_field[0] = KEY0_UPRf;
        key_field[1] = KEY1_UPRf;
        key_field[2] = KEY0_LWRf;
        key_field[3] = KEY1_LWRf;
        mask_field[0] = MASK0_UPRf;
        mask_field[1] = MASK1_UPRf;
        mask_field[2] = MASK0_LWRf;
        mask_field[3] = MASK1_LWRf;
        field_count = 4;
#endif
    } else {
        if (SOC_MEM_FIELD_VALID(unit, mem, FULL_KEYf)) {
            key_field[0] = FULL_KEYf;
            mask_field[0] = FULL_MASKf;
        } else {
            key_field[0] = KEYf;
            mask_field[0] = MASKf;
        }
        field_count = 1;
#ifdef BCM_SABER2_SUPPORT
        if (SOC_IS_SABER2(unit) && (mem == FP_TCAMm) &&
             soc_feature(unit, soc_feature_xy_tcam_28nm) &&
             !SOC_MEM_FIELD_VALID(unit, mem, FULL_KEYf)) {

            /* Key field doesn't include F4 and double wide mode fields */
            if (SOC_MEM_FIELD_VALID(unit, mem, F4f)) {
                key_field[field_count] = F4f;
                mask_field[field_count] = F4_MASKf;
                field_count += 1;
            }

            if (SOC_MEM_FIELD_VALID(unit, mem, DOUBLE_WIDE_MODEf)) {
                key_field[field_count] = DOUBLE_WIDE_MODEf;
                mask_field[field_count] = DOUBLE_WIDE_MODE_MASKf;
                field_count += 1;
            }
        }
#endif
    }
    for (i = 0; i < field_count; i++) {
        bit_length[i] = soc_mem_field_length(unit, mem, key_field[i]);
        word_length[i] = (bit_length[i] + 31) / 32;
    }
    data_words = soc_mem_entry_words(unit, mem);
    data_bytes = data_words * sizeof(uint32);
    for (index = 0; index < count; index++) {
        if (xy_entry != dm_entry) {
            sal_memcpy(xy_entry, dm_entry, data_bytes);
        }
        if (cache_entry) {
            sal_memcpy(cache_entry, dm_entry, data_bytes);
        }
        for (i = 0; i < field_count; i++) {
            soc_mem_field_get(unit, mem, dm_entry, key_field[i], key);
            soc_mem_field_get(unit, mem, dm_entry, mask_field[i], mask);
            if (cache_entry) {
                for (word = 0; word < word_length[i]; word++) {
                    key[word] &= mask[word];
                }
                soc_mem_field_set(unit, mem, cache_entry, key_field[i], key);
            }
            for (word = 0; word < word_length[i]; word++) {
                /*
                 * 40nm:
                 *    Encode: K0 = MASK & KEY
                 *            K1 = ~MASK | KEY
                 *    Decode: KEY = K0
                 *            MASK = K0 | ~K1
                 *              (encode)  (decode)
                 *    KEY MASK   K0  K1   KEY MASK
                 *    --------   ------   --------
                 *     0   0     0   1     0   0
                 *     1   0     0   1     0   0  =====> info loss
                 *     0   1     0   0     0   1
                 *     1   1     1   1     1   1
                 * 28nm:
                 *     Encode: K0 = MASK & KEY
                 *             K1 = MASK & ~KEY
                 *     Decode: KEY = K0
                 *             MASK = K0 | K1
                 *     KEY MASK   K0  K1   KEY MASK
                 *     --------   ------   --------
                 *      0   0     0   0     0   0
                 *      1   0     0   0     0   0  =====> info loss
                 *      0   1     0   1     0   1
                 *      1   1     1   0     1   1
                 * Notes:
                 *     - Mask value of 1 means to compare against key
                 *     - K0 is in KEY field of the entry
                 *     - K1 is in MASK field of the entry
                 */
                converted_key = key[word] & mask[word];
                if (!no_trans) {
                    converted_mask = (key[word] | ~mask[word]) ^ xor_value;
                    mask[word] = converted_mask;
                }
                key[word] = converted_key;
                /*
                 * XY -> LPT
                 * lpt_x[1:0] = {(xy_y[1] | xy_y[0]), (xy_x[1] | xy_y[0])}
                 * lpt_y[1:0] = {(xy_y[1] | xy_x[0]), (xy_x[1] | xy_x[0])}
                 */
                if (xy_lpt) {
                    converted_key = ((mask[word] | (mask[word] << 1)) & 0xaaaaaaaa) |
                                    ((mask[word] | (key[word] >> 1)) & 0x55555555);
                    converted_mask = ((mask[word] | (key[word] << 1)) & 0xaaaaaaaa) |
                                     ((key[word] | (key[word] >> 1)) & 0x55555555);
                    key[word] = converted_key;
                    mask[word] = converted_mask;
                }
            }
            if ((bit_length[i] & 0x1f) != 0) {
                if (xy_lpt) {
                    key[word - 1] &= (1 << (bit_length[i] & 0x1f)) - 1;
                }
                mask[word - 1] &= (1 << (bit_length[i] & 0x1f)) - 1;
            }
            soc_mem_field_set(unit, mem, xy_entry, key_field[i], key);
            soc_mem_field_set(unit, mem, xy_entry, mask_field[i], mask);
        }
#ifdef BCM_TOMAHAWK2_SUPPORT
        /*
         * TH2:
         * if entry is part of IPBM lookup
         *     X=M & D (for all regular bits [159:34])
         *     X=1'b0 (for IPBM bits[33:0])
         *     Y=M & D! (for all bits)
         * if entry is not part of IPBM lookup (no change)
         *     X=M & D
         *     Y=M & D!
         */
        if (SOC_IS_TOMAHAWK2(unit) && 
            (mem == IFP_TCAM_WIDEm || 
             mem == IFP_TCAM_WIDE_PIPE0m ||
             mem == IFP_TCAM_WIDE_PIPE1m ||
             mem == IFP_TCAM_WIDE_PIPE2m ||
             mem == IFP_TCAM_WIDE_PIPE3m)) {
            sal_memset(key, 0, sizeof(key));
            soc_mem_field_set(unit, mem, xy_entry, IPBMf, key);
        }
#endif /* BCM_TOMAHAWK2_SUPPORT */
        dm_entry += data_words;
        xy_entry += data_words;
        if (cache_entry) {
            cache_entry += data_words;
        }
    }
}

STATIC void
_soc_mem_read_defip_index_map(int unit, soc_mem_t mem, int index, int *index2);

STATIC int
_soc_mem_defip_index_is_invalid(int unit, soc_mem_t mem, int index);

#define MAX_TCAM_PROTECT_RANGES 32
STATIC int
_soc_mem_tcam_entry_preserve(int unit, soc_mem_t mem, int copyno, int index,
                             int count, void *entry, soc_mem_t *aggr_mem,
                             void **buffer)
{
    int rv = SOC_E_NONE;
    int blk, entry_words, data_words, data_bytes, alloc_size, word_idx, i;
    int num_tcam_protect_ranges;
    int index_min[MAX_TCAM_PROTECT_RANGES], index_max[MAX_TCAM_PROTECT_RANGES];
    int split_count[MAX_TCAM_PROTECT_RANGES];
    uint32 read_buf[SOC_MAX_MEM_WORDS], *entry_ptr;
    int field_count, field_idx, idx, is_l3_defip = 0, wide = 0;
    soc_field_t fields[4];
    int incr;

    switch (mem) {
     case FP_GLOBAL_MASK_TCAMm:
     case FP_GLOBAL_MASK_TCAM_Xm:
     case FP_GLOBAL_MASK_TCAM_Ym:
     case FP_GM_FIELDSm:
     case FP_TCAMm:
     case FP_UDF_TCAMm:
     case EFP_TCAMm:
     case VFP_TCAMm:
#if defined BCM_TOMAHAWK_SUPPORT
     case IFP_TCAMm:
     case IFP_TCAM_PIPE0m:
     case IFP_TCAM_PIPE1m:
     case IFP_TCAM_PIPE2m:
     case IFP_TCAM_PIPE3m:
     case IFP_TCAM_WIDEm:
     case IFP_TCAM_WIDE_PIPE0m:
     case IFP_TCAM_WIDE_PIPE1m:
     case IFP_TCAM_WIDE_PIPE2m:
     case IFP_TCAM_WIDE_PIPE3m:
     case FP_UDF_TCAM_PIPE0m:
     case FP_UDF_TCAM_PIPE1m:
     case FP_UDF_TCAM_PIPE2m:
     case FP_UDF_TCAM_PIPE3m:
     case IFP_LOGICAL_TABLE_SELECTm:
     case IFP_LOGICAL_TABLE_SELECT_PIPE0m:
     case IFP_LOGICAL_TABLE_SELECT_PIPE1m:
     case IFP_LOGICAL_TABLE_SELECT_PIPE2m:
     case IFP_LOGICAL_TABLE_SELECT_PIPE3m:
     case IFP_LOGICAL_TABLE_SELECT_TCAM_ONLYm:
     case IFP_LOGICAL_TABLE_SELECT_TCAM_ONLY_PIPE0m:
     case IFP_LOGICAL_TABLE_SELECT_TCAM_ONLY_PIPE1m:
     case IFP_LOGICAL_TABLE_SELECT_TCAM_ONLY_PIPE2m:
     case IFP_LOGICAL_TABLE_SELECT_TCAM_ONLY_PIPE3m:
     case EXACT_MATCH_LOGICAL_TABLE_SELECTm:
     case EXACT_MATCH_LOGICAL_TABLE_SELECT_PIPE0m:
     case EXACT_MATCH_LOGICAL_TABLE_SELECT_PIPE1m:
     case EXACT_MATCH_LOGICAL_TABLE_SELECT_PIPE2m:
     case EXACT_MATCH_LOGICAL_TABLE_SELECT_PIPE3m:
     case EXACT_MATCH_LOGICAL_TABLE_SELECT_TCAM_ONLYm:
     case EXACT_MATCH_LOGICAL_TABLE_SELECT_TCAM_ONLY_PIPE0m:
     case EXACT_MATCH_LOGICAL_TABLE_SELECT_TCAM_ONLY_PIPE1m:
     case EXACT_MATCH_LOGICAL_TABLE_SELECT_TCAM_ONLY_PIPE2m:
     case EXACT_MATCH_LOGICAL_TABLE_SELECT_TCAM_ONLY_PIPE3m:
#endif /* BCM_TOMAHAWK_SUPPORT */
        return SOC_E_NONE;
    case CPU_COS_MAP_ONLYm:
        *aggr_mem = CPU_COS_MAPm;
        break;
    case L2_USER_ENTRY_ONLYm:
        *aggr_mem = L2_USER_ENTRYm;
        break;
    case L3_DEFIP_128_ONLYm:
        *aggr_mem = L3_DEFIP_128m; /* will miss hit bit from one pipe */
        break;
#if defined(BCM_TRIUMPH3_SUPPORT) || defined(BCM_KATANA_SUPPORT)
    case L3_DEFIP_PAIR_128_ONLYm:
        *aggr_mem = L3_DEFIP_PAIR_128m; /* will miss hit bit from one pipe */
        break;
#endif
    case L3_DEFIP_ONLYm:
        *aggr_mem = L3_DEFIPm; /* will miss hit bit from one pipe */
        break;
    case MY_STATION_TCAM_ENTRY_ONLYm:
        *aggr_mem = MY_STATION_TCAMm;
        break;
    case VLAN_SUBNET_ONLYm:
        *aggr_mem = VLAN_SUBNETm;
        break;
    default:
        *aggr_mem = mem;
        break;
    }

    if (*aggr_mem != mem && count > 1) {
        /* Can't support DMA multiple entries on tcam only table */
        return SOC_E_NONE;
    }

    if (copyno == COPYNO_ALL) {
        SOC_MEM_BLOCK_ITER(unit, *aggr_mem, blk) {
            copyno = blk;
            break;
        }
    }

    /* Check if any valid entry will be overwritten */
    if (SOC_MEM_FIELD_VALID(unit, mem, VALIDf)) {
        fields[0] = VALIDf;
        field_count = 1;
    } else if (SOC_MEM_FIELD_VALID(unit, mem, VALID0f)) {
        fields[0] = VALID0f;
        fields[1] = VALID1f;
        field_count = 2;
    } else if (SOC_MEM_FIELD_VALID(unit, mem, VALID0_LWRf)) {
        fields[0] = VALID0_LWRf;
        fields[1] = VALID1_LWRf;
        fields[2] = VALID0_UPRf;
        fields[3] = VALID1_UPRf;
        field_count = 4;
    } else {
        field_count = 0;
    }

    num_tcam_protect_ranges = (SOC_CONTROL(unit)->l3_defip_max_tcams);

    sal_memset(index_min, -1, sizeof(index_min));
    sal_memset(index_max, -1, sizeof(index_max));
    sal_memset(split_count, 0, sizeof(split_count));
    data_words = soc_mem_entry_words(unit, *aggr_mem);
    data_bytes = data_words * sizeof(uint32);
    if (mem == L3_DEFIPm || mem == L3_DEFIP_ONLYm) {
        is_l3_defip = 1;
    }

#if defined(BCM_TRIUMPH3_SUPPORT) || defined(BCM_KATANA_SUPPORT)
    if (mem == L3_DEFIP_PAIR_128m || mem == L3_DEFIP_PAIR_128_ONLYm) {
        is_l3_defip = 1;
        wide = 1;
    }
#endif
    incr = soc_mem_index_count(unit, mem) / (num_tcam_protect_ranges >> wide);
    for (i = 0; i < count; i++) {
        if (is_l3_defip) {
            idx = (index + i) / incr;
        } else {
            idx = 0;
            split_count[idx] = count;
            index_min[idx] = index;
            index_max[idx] = soc_mem_index_max(unit, *aggr_mem);
            break;
        }
        if (idx >= (num_tcam_protect_ranges >> wide)) {
            return SOC_E_INTERNAL;
        }

        split_count[idx]++;

        if (index_min[idx] != -1) {
            continue;
        }

        index_min[idx] = index + i;
        index_max[idx] = (idx + 1) * incr - 1;

        _soc_mem_read_defip_index_map(unit, mem, index_min[idx], &index_min[idx]);
        _soc_mem_read_defip_index_map(unit, mem, index_max[idx], &index_max[idx]);
    }

    entry_words = soc_mem_entry_words(unit, mem);
    entry_ptr = entry;
    for (idx = 0; idx < num_tcam_protect_ranges; idx++) {
        if (index_min[idx] == -1) {
            continue;
        }

        alloc_size = (index_max[idx] - index_min[idx] + 1) * data_bytes;
        buffer[idx] = soc_cm_salloc(unit, alloc_size + data_bytes, "shift buffer");
        if (buffer[idx] == NULL) {
            rv = SOC_E_MEMORY;
            goto cleanup;
        }

        /* After split, l3_defip tcam tables are good to use dma */
#ifdef BCM_SBUSDMA_SUPPORT
        if (soc_feature(unit, soc_feature_sbusdma)) {
            rv = _soc_mem_sbusdma_read(unit, *aggr_mem, copyno, index_min[idx],
                                       index_max[idx], 0, buffer[idx]);
        } else
#endif
        {
            rv = _soc_xgs3_mem_dma(unit, *aggr_mem, 0, copyno, index_min[idx],
                                   index_max[idx], 0, buffer[idx]);
        }
        if (SOC_FAILURE(rv)) {
            goto cleanup;
        }

        /* Zero the last entry */
        _soc_mem_tcam_dm_to_xy(unit, *aggr_mem, 1,
                               soc_mem_entry_null(unit, *aggr_mem),
                               &((uint32 *)buffer[idx])
                               [alloc_size / sizeof(uint32)], NULL);

        for (i = 0; i < split_count[idx]; i++) {
            /* Get the original entry */
            sal_memcpy(read_buf, ((uint32 *)buffer[idx]) + (data_words * i),
                       WORDS2BYTES(entry_words));

            /* Bit-wise 'and' the original entry with new entry */
            for (word_idx = 0; word_idx < entry_words; word_idx++) {
                read_buf[word_idx] &= entry_ptr[word_idx];
            }
            /* Find if original and new entry are both valid */
            for (field_idx = 0; field_idx < field_count; field_idx++) {
                if (soc_mem_field32_get(unit, *aggr_mem, read_buf,
                                        fields[field_idx])) {
                    break;
                }
            }
            if (field_idx != field_count) {
                break;
            }
            entry_ptr += entry_words;
        }
        if (i == split_count[idx]) {
            /* don't need to preserve entry */
            soc_cm_sfree(unit, buffer[idx]);
            buffer[idx] = NULL;
            continue;
        }

#ifdef BCM_SBUSDMA_SUPPORT
        if (soc_feature(unit, soc_feature_sbusdma)) {
            rv = _soc_mem_sbusdma_write(unit, *aggr_mem, copyno, index_max[idx] + 1,
                                        index_min[idx] + 1, buffer[idx], FALSE, -1);
        } else
#endif
        {
            rv = _soc_xgs3_mem_slam(unit, 0, *aggr_mem, 0, copyno, index_max[idx] + 1,
                                    index_min[idx] + 1, buffer[idx]);
        }

        if (SOC_FAILURE(rv)) {
            goto cleanup;
        }

    }

    return SOC_E_NONE;

cleanup:
    for (idx = 0; idx < num_tcam_protect_ranges; idx++) {
        if (buffer[idx]) {
            soc_cm_sfree(unit, buffer[idx]);
            buffer[idx] = NULL;
        }
    }
    return rv;
}

STATIC int
_soc_mem_tcam_entry_restore(int unit, soc_mem_t mem, int copyno, int index,
                            int count, void **buffer)
{
    int rv, rv2 = SOC_E_NONE;
    int blk, data_words;
    uint32 *buf_ptr;
    int num_tcam_protect_ranges;
    int index_min[MAX_TCAM_PROTECT_RANGES], index_max[MAX_TCAM_PROTECT_RANGES];
    int split_count[MAX_TCAM_PROTECT_RANGES];
    int i, idx, is_l3_defip = 0, wide = 0, incr;

    num_tcam_protect_ranges = (SOC_CONTROL(unit)->l3_defip_max_tcams);

    for (idx = 0; idx < num_tcam_protect_ranges; idx++) {
        if (buffer[idx]) {
            break;
        }
    }
    if (idx == num_tcam_protect_ranges) {
        return SOC_E_NONE;
    }

    if (copyno == COPYNO_ALL) {
        SOC_MEM_BLOCK_ITER(unit, mem, blk) {
            copyno = blk;
            break;
        }
    }

    data_words = soc_mem_entry_words(unit, mem);
    sal_memset(index_min, -1, sizeof(index_min));
    sal_memset(index_max, -1, sizeof(index_max));
    sal_memset(split_count, 0, sizeof(split_count));

    if (mem == L3_DEFIPm || mem == L3_DEFIP_ONLYm) {
        is_l3_defip = 1;
    }

#if defined(BCM_TRIUMPH3_SUPPORT) || defined(BCM_KATANA_SUPPORT)
    if (mem == L3_DEFIP_PAIR_128m || mem == L3_DEFIP_PAIR_128_ONLYm) {
        is_l3_defip = 1;
        wide = 1;
    }
#endif

    incr = soc_mem_index_count(unit, mem) / (num_tcam_protect_ranges >> wide);
    for (i = 0; i < count; i++) {
        if (is_l3_defip) {
            idx = (index + i) / incr;
        } else {
            idx = 0;
            split_count[idx] = count;
            index_min[idx] = index;
            index_max[idx] = soc_mem_index_max(unit, mem);
            break;
        }
        if (idx >= (num_tcam_protect_ranges >> wide)) {
            return SOC_E_INTERNAL;
        }

        split_count[idx]++;

        if (index_min[idx] != -1) {
            continue;
        }

        index_min[idx] = index + i;
        index_max[idx] = (idx + 1) * incr - 1;

        _soc_mem_read_defip_index_map(unit, mem, index_min[idx], &index_min[idx]);
        _soc_mem_read_defip_index_map(unit, mem, index_max[idx], &index_max[idx]);
    }

    for (idx = 0; idx < num_tcam_protect_ranges; idx++) {
        if (index_min[idx] == -1) {
            continue;
        }
        if (buffer[idx] == NULL) {
            continue;
        }
        buf_ptr = &((uint32 *)buffer[idx])[split_count[idx] * data_words];

#ifdef BCM_SBUSDMA_SUPPORT
        if (soc_feature(unit, soc_feature_sbusdma)) {
            /* coverity[negative_returns] */
            rv = _soc_mem_sbusdma_write(unit, mem, copyno, index_min[idx] + split_count[idx],
                                        index_max[idx] + 1, buf_ptr, FALSE, -1);
        } else
#endif
        {
            /* coverity[negative_returns] */
            rv = _soc_xgs3_mem_slam(unit, 0, mem, 0, copyno, index_min[idx] + split_count[idx],
                                    index_max[idx] + 1, buf_ptr);
        }
        LOG_INFO(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "Restore mem:%d from min+count:%-5d+%-2d to max:%-5d\n"),
                  mem,index_min[idx], split_count[idx], index_max[idx]));
        soc_cm_sfree(unit, buffer[idx]);
        if (rv < 0) { /* Don't stop if rv < 0, but record rv. */
            rv2 = rv;
        }
    }

    return rv2;
}
#endif /* BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT */

/*
 * Function:
 *     _soc_mem_dma_read
 * Purpose:
 *     DMA acceleration for soc_mem_read_range()
 *     With multiple CMC/channel support
 * Parameters:
 *     buffer -- must be pointer to sufficiently large
 *               DMA-able block of memory
 */
STATIC int
_soc_mem_dma_read(int unit, soc_mem_t mem, unsigned array_index, int copyno,
                  int index_min, int index_max, uint32 ser_flags,
                  void *buffer, int vchan)
{
    /* Remove assertions from here, because they are checked in calling
     * functions or intentionally excepted for SER correction
     * and Warm Boot.
     */

#ifdef BCM_SBUSDMA_SUPPORT
    if (soc_feature(unit, soc_feature_sbusdma)) {
        SOC_IF_ERROR_RETURN
            (_soc_mem_array_sbusdma_read(unit, mem, array_index,
                                         copyno, index_min, index_max,
                                         ser_flags, buffer, vchan));
    } else
#endif
    {
        SOC_IF_ERROR_RETURN
            (_soc_xgs3_mem_dma(unit, mem, array_index, copyno, index_min,
                               index_max, ser_flags, buffer));
    }

    if (0 != (ser_flags & _SOC_SER_FLAG_XY_READ)) {
        /* SER memscan logic must have the raw entries */
        return SOC_E_NONE;
    }

#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) \
    || defined(BCM_HURRICANE2_SUPPORT)
    if (soc_feature(unit, soc_feature_xy_tcam_direct) &&
        (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) &&
        (!(SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_EXT_CAM))) {
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
        _soc_mem_fp_global_mask_tcam_shift(unit, mem, buffer, index_min, index_max);
#endif
        _soc_mem_tcam_xy_to_dm(unit, mem, index_max - index_min + 1,
                               buffer, buffer);
    }
#endif /* BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT */

    return SOC_E_NONE;
}

/*
 * Function:
 *     _soc_mem_dma_write
 * Purpose:
 *     DMA acceleration for soc_mem_write_range()
 * Parameters:
 *     buffer -- must be pointer to sufficiently large
 *               DMA-able block of memory
 *     index_begin <= index_end - Forward direction
 *     index_begin >  index_end - Reverse direction
 */
STATIC int
_soc_mem_dma_write(int unit, uint32 flags, soc_mem_t mem, unsigned array_index, int copyno,
                   int index_begin, int index_end, void *buffer, void *cache_buffer,
                   int vchan)
{
    int           rv;
    void          *buffer_ptr;
#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) \
    || defined(BCM_HURRICANE2_SUPPORT)
    int           count, alloc_size;
    soc_mem_t     aggr_mem = INVALIDm;
    void          *shift_buffer[MAX_TCAM_PROTECT_RANGES] = {NULL};
#endif

    if (SOC_HW_ACCESS_DISABLE(unit)) {
        return SOC_E_NONE;
    }

    SOC_MEM_ALIAS_TO_ORIG(unit,mem);

    assert(SOC_MEM_BLOCK_VALID(unit, mem, copyno));
#ifndef BCM_WARM_BOOT_SUPPORT
    assert(soc_mem_index_valid(unit, mem, index_begin));
    assert(soc_mem_index_valid(unit, mem, index_end));
#endif

#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) \
    || defined(BCM_HURRICANE2_SUPPORT)
    if (soc_feature(unit, soc_feature_xy_tcam) &&
        (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) &&
        (!(SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_EXT_CAM))) {
        if (index_begin > index_end) {
            count = index_begin - index_end + 1;
        } else {
            count = index_end - index_begin + 1;
        }
        alloc_size = count * soc_mem_entry_words(unit, mem) * sizeof(uint32);
        buffer_ptr = soc_cm_salloc(unit, alloc_size, "converted buffer");
        if (buffer_ptr == NULL) {
            return SOC_E_MEMORY;
        }
        _soc_mem_tcam_dm_to_xy(unit, mem, count, buffer, buffer_ptr,
                               cache_buffer);

        if (index_begin <= index_end &&
            SOC_CONTROL(unit)->tcam_protect_write) {
            rv = _soc_mem_tcam_entry_preserve(unit, mem, copyno, index_begin,
                                              count, buffer_ptr, &aggr_mem,
                                              shift_buffer);
            if (SOC_FAILURE(rv)) {
                soc_cm_sfree(unit, buffer_ptr);
                return rv;
            }
        }
    } else
#endif /* BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT */
    {
        buffer_ptr = buffer;
    }

#ifdef BCM_SBUSDMA_SUPPORT
    if (soc_feature(unit, soc_feature_sbusdma)) {
        rv = _soc_mem_array_sbusdma_write(unit, flags, mem, array_index,
                                          array_index, copyno, index_begin,
                                          index_end, buffer_ptr, FALSE, -1,
                                          vchan);
    } else
#endif
    {
        rv = _soc_xgs3_mem_slam(unit, flags, mem, array_index, copyno, index_begin,
                                index_end, buffer_ptr);
    }

#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) \
    || defined(BCM_HURRICANE2_SUPPORT)
    if (soc_feature(unit, soc_feature_xy_tcam) &&
        (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) &&
        (!(SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_EXT_CAM))) {
#ifdef INCLUDE_MEM_SCAN
        if (NULL != cache_buffer) {
            /* Update memscan TCAM cache if necessary */
            soc_mem_scan_tcam_cache_update(unit, mem,
                                           index_begin, index_end,
                                           buffer_ptr);
        }
#endif /* INCLUDE_MEM_SCAN */
#ifdef BCM_TRIDENT2_SUPPORT
#ifdef BCM_WARM_BOOT_SUPPORT
        if (SOC_IS_TRIDENT2X(unit) || SOC_IS_TITAN2X(unit)) {
            int blk;

            SOC_MEM_BLOCK_ITER(unit, mem, blk) {
                if (copyno != COPYNO_ALL && copyno != blk) {
                    continue;
                }
                soc_mem_overlay_tcam_update(unit, mem, blk,
                                            index_begin, index_end);
            }
        }
#endif
#endif
        soc_cm_sfree(unit, buffer_ptr);
        if (index_begin <= index_end &&
            SOC_CONTROL(unit)->tcam_protect_write) {
            SOC_IF_ERROR_RETURN
                (_soc_mem_tcam_entry_restore(unit, aggr_mem, copyno,
                                             index_begin,
                                             index_end - index_begin + 1,
                                             shift_buffer));
        }
    }
#endif /* BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT */

    return rv;
}




#if defined(BCM_FIREBOLT_SUPPORT) || defined(BCM_SIRIUS_SUPPORT) || defined(BCM_CALADAN3_SUPPORT)
/*
 * Function:
 *      _soc_xgs3_mem_clear_slam
 * Purpose:
 *    soc_mem_clear acceleration using table DMA write (slam)
 */
STATIC int
_soc_xgs3_mem_clear_slam(int unit, soc_mem_t mem,
                         int copyno, void *null_entry)
{
    int       rv, chunk_size, chunk_entries, mem_size, entry_words;
    int       index, index_end, index_min, index_max;
    uint32    *buf;

    if (SOC_WARM_BOOT(unit) || SOC_IS_RELOADING(unit)) {
        return SOC_E_NONE;
    }
#ifdef BCM_SBUSDMA_SUPPORT
    if (soc_feature(unit, soc_feature_sbusdma)) {
        uint32 *null_ptr;
#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) \
    || defined(BCM_HURRICANE2_SUPPORT)
        uint32 null_buf[SOC_MAX_MEM_WORDS];
        if (soc_feature(unit, soc_feature_xy_tcam) &&
            (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) &&
            (!(SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_EXT_CAM))) {
            null_ptr = null_buf;
            _soc_mem_tcam_dm_to_xy(unit, mem, 1, null_entry, null_ptr, NULL);
        } else
#endif /* defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) */
        {
            null_ptr = null_entry;
        }
        return _soc_mem_sbusdma_clear(unit, mem, copyno, null_ptr);
    }
#endif
    chunk_size = SOC_MEM_CLEAR_CHUNK_SIZE_GET(unit);
    index_min = soc_mem_index_min(unit, mem);
    index_max = soc_mem_index_max(unit, mem);
    entry_words = soc_mem_entry_words(unit, mem);
    mem_size = (index_max - index_min + 1) * entry_words * 4;
    if (mem_size < chunk_size) {
        chunk_size = mem_size;
    }

    buf = soc_cm_salloc(unit, chunk_size, "mem_clear_buf");
    if (buf == NULL) {
        return SOC_E_MEMORY;
    }

    chunk_entries = chunk_size / (entry_words * 4);

    if (null_entry == _soc_mem_entry_null_zeroes) {
        sal_memset(buf, 0, chunk_size);
    } else {
        for (index = 0; index < chunk_entries; index++) {
            sal_memcpy(buf + (index * entry_words),
                       null_entry, entry_words * 4);
        }
    }

    rv = SOC_E_NONE;
    for (index = index_min; index <= index_max; index += chunk_entries) {
        index_end = index + chunk_entries - 1;
        if (index_end > index_max) {
            index_end = index_max;
        }

        rv = soc_mem_write_range(unit, mem, copyno, index, index_end, buf);
        if (rv < 0) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "soc_mem_write_range: "
                                  "write %s.%s[%d-%d] failed: %s\n"),
                       SOC_MEM_UFNAME(unit, mem),
                       SOC_BLOCK_NAME(unit, copyno),
                       index, index_end, soc_errmsg(rv)));
            break;
        }
    }

    soc_cm_sfree(unit, buf);
    return rv;
}

/*
 * Function:
 *      _soc_xgs3_mem_clear_pipe
 * Purpose:
 *    soc_mem_clear acceleration using pipeline table clear logic.
 *    Much faster than table slam.  Exists on XGS3.
 */
STATIC int
_soc_xgs3_mem_clear_pipe(int unit, soc_mem_t mem, int blk, void *null_entry)
{
    int rv, mementries, to_usec, to_rv;
    uint32 hwreset1, hwreset2, membase, memoffset, memstage;
    soc_timeout_t to;
#ifdef BCM_EXTND_SBUS_SUPPORT
    uint32 memnum;
#endif /* BCM_EXTND_SBUS_SUPPORT */

#ifdef BCM_XGS_SUPPORT
    if (SOC_IS_XGS(unit)) {
        if (!SOC_MEM_CLEAR_HW_ACC(unit)) {
            return SOC_E_UNAVAIL;
        }
    }
#endif /* BCM_XGS_SUPPORT */
    if (null_entry != _soc_mem_entry_null_zeroes) {
        return SOC_E_UNAVAIL;
    }

    /* Tomahawk cannot clear the vlan table with pipe for some reasons
     * Use DMA clear for now
     */
    if (SOC_IS_TOMAHAWKX(unit)) {
        if ((mem == VLAN_TABm) || (mem == VLAN_XLATEm) ||
            (mem == EGR_VLAN_XLATEm)) {
            return SOC_E_UNAVAIL;
        }
    }

    if (SOC_WARM_BOOT(unit) || SOC_IS_RELOADING(unit) || SOC_HW_ACCESS_DISABLE(unit)) {
        return SOC_E_NONE;
    }
    if (SAL_BOOT_PLISIM) {
        if (SAL_BOOT_BCMSIM || SAL_BOOT_XGSSIM) {
            if (!(SOC_IS_TRIUMPH3(unit) || SOC_IS_TD2_TT2(unit))) {
                /* h/w acceleration is not implemented in some models */
                return SOC_E_UNAVAIL;
            }
        } else {
            return SOC_E_UNAVAIL;
        }
    }

    if (!SOC_REG_IS_VALID(unit, ING_HW_RESET_CONTROL_1r)) {
        return SOC_E_UNAVAIL;
    }


#ifdef BCM_TRIUMPH_SUPPORT
    if (SOC_IS_TRIUMPH(unit) || SOC_IS_VALKYRIE(unit)) {
        switch (mem) {
        case ING_PW_TERM_COUNTERSm:
        case EGR_IPFIX_SESSION_TABLEm:
        case EGR_IPFIX_EXPORT_FIFOm:
            return SOC_E_UNAVAIL;
        default:
            break;
        }
    }
#if defined(BCM_TRIUMPH2_SUPPORT)
    else if (SOC_IS_TRIUMPH2(unit) || SOC_IS_APOLLO(unit) ||
             SOC_IS_VALKYRIE2(unit)) {
        if (mem == LMEPm) {
            return SOC_E_UNAVAIL;
        }
        if (soc_feature(unit, soc_feature_ser_parity)) {
            SOC_IF_ERROR_RETURN
                (soc_triumph2_ser_mem_clear(unit, mem));
        }
    }
#endif /* BCM_TRIUMPH2_SUPPORT */
#if defined(BCM_ENDURO_SUPPORT)
    else if (SOC_IS_ENDURO(unit)) {
        if (mem == LMEPm || mem == LMEP_1m) {
            return SOC_E_UNAVAIL;
        }
    }
#endif /* BCM_ENDURO_SUPPORT */
#if defined(BCM_TRIDENT_SUPPORT)
    else if (SOC_IS_TRIDENT(unit) || SOC_IS_TITAN(unit)) {
        if ((SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) ||
            (SOC_MEM_INFO(unit, mem).index_max == 0)) {
            return SOC_E_UNAVAIL;
        }
        switch (mem) {
        case EGR_VLANm:
            return SOC_E_UNAVAIL;
        default:
        break;
        }
    }
#endif /* BCM_TRIDENT_SUPPORT */
#ifdef BCM_TRIUMPH3_SUPPORT
    else if (SOC_IS_TRIUMPH3(unit) || SOC_IS_TD2_TT2(unit)) {
        if ((SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) ||
            (SOC_MEM_INFO(unit, mem).index_max == 0)) {
            return SOC_E_UNAVAIL;
        }
        if (SOC_IS_TRIUMPH3(unit)) {
            if (mem == EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm ||
                mem == ING_DVP_2_TABLEm) {
                return SOC_E_UNAVAIL;
            }
        }
    }
#endif /* BCM_TRIUMPH3_SUPPORT */
#ifdef BCM_KATANA2_SUPPORT
    else if (SOC_IS_KATANA2(unit)) {
        switch(mem) {
        /* WORK-AROUND:SDK-48233
           KT2 SW WAR: IP HW Mem Reset does not reset
           IARB_ING_PHYSICAL_PORT table  */
        case IARB_ING_PHYSICAL_PORTm:
            return SOC_E_UNAVAIL;
        default:
            break;
        }
        if (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) {
            return SOC_E_UNAVAIL;
        }
    }
#endif /* BCM_KATANA2_SUPPORT */
#ifdef BCM_KATANA_SUPPORT
    else if (SOC_IS_KATANA(unit)) {
        if (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) {
            return SOC_E_UNAVAIL;
        }
    }
#endif
#ifdef BCM_HURRICANE2_SUPPORT
    else if (SOC_IS_HURRICANE2(unit)) {
        switch(mem) {
        case VLAN_SUBNETm:
        case VLAN_SUBNET_ONLYm:
            return SOC_E_UNAVAIL;
        default:
            break;
        }

        if (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) {
            SOC_IF_ERROR_RETURN
                (soc_hurricane2_ser_mem_clear(unit, mem));
        }
    }
#endif /* BCM_HURRICANE2_SUPPORT */

#ifdef BCM_GREYHOUND_SUPPORT
    else if (SOC_IS_GREYHOUND(unit) || SOC_IS_HURRICANE3(unit) ||
            SOC_IS_GREYHOUND2(unit)) {
        if (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) {
            SOC_IF_ERROR_RETURN
                (soc_greyhound_ser_mem_clear(unit, mem));
        }
    }
#endif /* BCM_GREYHOUND_SUPPORT */

#ifdef BCM_SABER2_SUPPORT
    if (SOC_IS_SABER2(unit)) {
        if (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) {
            SOC_IF_ERROR_RETURN
                (soc_saber2_ser_mem_clear(unit, mem));
        }
    }
#endif /* BCM_SABER2_SUPPORT */

#endif /* BCM_TRIUMPH_SUPPORT */

    if (SAL_BOOT_SIMULATION) {
        to_usec = 10000000;
    } else {
        to_usec = 50000;
    }
#ifdef BCM_SHADOW_SUPPORT
    if (SOC_IS_SHADOW(unit) && (mem == ING_VLAN_TAG_ACTION_PROFILEm)) {
       return SOC_E_UNAVAIL;
    }

    if (SOC_IS_SHADOW(unit)) {
        /* Reset HW Reset Control */
        SOC_IF_ERROR_RETURN(WRITE_ING_HW_RESET_CONTROL_2r(unit, 0));
        SOC_IF_ERROR_RETURN(WRITE_ING_HW_RESET_CONTROL_1r(unit, 0));
    }
#endif

    rv = SOC_E_NONE;
    to_rv = SOC_E_NONE;
    membase = SOC_MEM_BASE(unit, mem);
    mementries = soc_mem_index_count(unit, mem);
#ifdef BCM_EXTND_SBUS_SUPPORT
    memnum = 0;
    if (soc_feature(unit, soc_feature_new_sbus_format)) {
        if (SOC_BLOCK_TYPE(unit, blk) == SOC_BLK_IPIPE) {
            memnum = SOC_MEM_ADDR_NUM_EXTENDED(membase);
            /* For KATANA2: ING_HW_RESET_CONTROL_1:: OFFSET
             26-bit starting offset of table for memory.
             This is {8-bit MEMSEL, 18-bit MEMIDX}
             Believe below condition should be out of "if BLK_IPIPE" condition
             */
            if (SOC_IS_KATANA2(unit)) {
                memoffset = SOC_MEM_ADDR_OFFSET_EXTENDED(membase);
            } else {
                memoffset = SOC_MEM_ADDR_OFFSET_EXTENDED_IPIPE(membase);
            }
        } else {
            memoffset = SOC_MEM_ADDR_OFFSET_EXTENDED(membase);
        }
        memstage = SOC_MEM_ADDR_STAGE_EXTENDED(membase);
    } else
#endif
    {
        memoffset = SOC_MEM_ADDR_OFFSET(membase);
        memstage = SOC_MEM_ADDR_STAGE(membase);
    }

    switch (SOC_BLOCK_TYPE(unit, blk)) {
    case SOC_BLK_IPIPE:
    case SOC_BLK_IPIPE_HI:
        hwreset1 = 0;
#ifdef BCM_EXTND_SBUS_SUPPORT
        if (soc_reg_field_valid(unit, ING_HW_RESET_CONTROL_1r,
                                MEMORY_NUMBERf)) {
            soc_reg_field_set(unit, ING_HW_RESET_CONTROL_1r, &hwreset1,
                  MEMORY_NUMBERf, memnum);
        }
#endif

         soc_reg_field_set(unit, ING_HW_RESET_CONTROL_1r, &hwreset1,
                   OFFSETf, memoffset);
         soc_reg_field_set(unit, ING_HW_RESET_CONTROL_1r, &hwreset1,
                   STAGE_NUMBERf, memstage);
         hwreset2 = 0;
         soc_reg_field_set(unit, ING_HW_RESET_CONTROL_2r, &hwreset2,
                   COUNTf, mementries);
         soc_reg_field_set(unit, ING_HW_RESET_CONTROL_2r, &hwreset2,
                   VALIDf, 1);

         rv = WRITE_ING_HW_RESET_CONTROL_1r(unit, hwreset1);
         if (rv < 0) {
             break;
         }
         rv = WRITE_ING_HW_RESET_CONTROL_2r(unit, hwreset2);
         if (rv < 0) {
             break;
         }

         soc_timeout_init(&to, to_usec, 100);
         for (;;) {
             rv = READ_ING_HW_RESET_CONTROL_2r(unit, &hwreset2);
             if (rv < 0) {
                 break;
             }
             if (soc_reg_field_get(unit, ING_HW_RESET_CONTROL_2r, hwreset2,
                                   DONEf)) {
                 break;
             }
             if (soc_timeout_check(&to)) {
                 to_rv = SOC_E_TIMEOUT;
                 break;
             }
         }
        hwreset2 = 0;
        if (soc_reg_field_valid(unit, ING_HW_RESET_CONTROL_2r, CMIC_REQ_ENABLEf)) {
            soc_reg_field_set(unit, ING_HW_RESET_CONTROL_2r, &hwreset2,
                              CMIC_REQ_ENABLEf, 1);
        }
        rv = WRITE_ING_HW_RESET_CONTROL_2r(unit, hwreset2);
        if (rv < 0) {
            break;
        }
        rv = WRITE_ING_HW_RESET_CONTROL_1r(unit, 0);
        break;

    case SOC_BLK_EPIPE:
    case SOC_BLK_EPIPE_HI:
        hwreset1 = 0;
        soc_reg_field_set(unit, EGR_HW_RESET_CONTROL_0r, &hwreset1,
                  START_ADDRESSf, memoffset);
        soc_reg_field_set(unit, EGR_HW_RESET_CONTROL_0r, &hwreset1,
                  STAGE_NUMBERf, memstage);
        hwreset2 = 0;
        soc_reg_field_set(unit, EGR_HW_RESET_CONTROL_1r, &hwreset2,
                  COUNTf, mementries);
        soc_reg_field_set(unit, EGR_HW_RESET_CONTROL_1r, &hwreset2,
                  VALIDf, 1);

        rv = WRITE_EGR_HW_RESET_CONTROL_0r(unit, hwreset1);
        if (rv < 0) {
            break;
        }
        rv = WRITE_EGR_HW_RESET_CONTROL_1r(unit, hwreset2);
        if (rv < 0) {
            break;
        }

        soc_timeout_init(&to, to_usec, 100);
        for (;;) {
            rv = READ_EGR_HW_RESET_CONTROL_1r(unit, &hwreset2);
            if (rv < 0) {
                break;
            }
            if (soc_reg_field_get(unit, EGR_HW_RESET_CONTROL_1r, hwreset2,
                                  DONEf)) {
                break;
            }
            if (soc_timeout_check(&to)) {
                to_rv = SOC_E_TIMEOUT;
                break;
            }
        }

        rv = WRITE_EGR_HW_RESET_CONTROL_1r(unit, 0);
        if (rv < 0) {
            break;
        }
        rv = WRITE_EGR_HW_RESET_CONTROL_0r(unit, 0);
        break;

    case SOC_BLK_ISM:
        hwreset1 = 0;
        soc_reg_field_set(unit, ISM_HW_RESET_CONTROL_0r, &hwreset1,
                  START_ADDRESSf, memoffset);
        soc_reg_field_set(unit, ISM_HW_RESET_CONTROL_0r, &hwreset1,
                  TABLE_IDf, memstage);
        hwreset2 = 0;
        soc_reg_field_set(unit, ISM_HW_RESET_CONTROL_1r, &hwreset2,
                  COUNTf, mementries);
        soc_reg_field_set(unit, ISM_HW_RESET_CONTROL_1r, &hwreset2,
                  VALIDf, 1);

        rv = WRITE_ISM_HW_RESET_CONTROL_0r(unit, hwreset1);
        if (rv < 0) {
            break;
        }
        rv = WRITE_ISM_HW_RESET_CONTROL_1r(unit, hwreset2);
        if (rv < 0) {
            break;
        }

        soc_timeout_init(&to, to_usec, 100);
        for (;;) {
            rv = READ_ISM_HW_RESET_CONTROL_1r(unit, &hwreset2);
            if (rv < 0) {
                break;
            }
            if (soc_reg_field_get(unit, ISM_HW_RESET_CONTROL_1r, hwreset2,
                                  DONEf)) {
                break;
            }
            if (soc_timeout_check(&to)) {
                to_rv = SOC_E_TIMEOUT;
                break;
            }
        }

        rv = WRITE_ISM_HW_RESET_CONTROL_1r(unit, 0);
        if (rv < 0) {
            break;
        }
        rv = WRITE_ISM_HW_RESET_CONTROL_0r(unit, 0);
        break;

    default:
        rv = SOC_E_UNAVAIL;
        break;
    }

    return rv == SOC_E_NONE ? to_rv : rv;
}
#endif /* defined(BCM_FIREBOLT_SUPPORT) || defined(BCM_SIRIUS_SUPPORT) */

#endif /* defined(BCM_XGS_SUPPORT) || defined(BCM_SIRIUS_SUPPORT) ... */

/************************************************************************/
/* Routines for reading/writing all tables (except no write to ARL)    */
/************************************************************************/

/*
 * Function: _soc_mem_read_tcam_is_invalid
 *
 * Purpose: Called from within _soc_mem_read.  If the read is attempting to
 *          read from a tcam that makes no sense, we return without reading.
 *          This is for cases like when a tcam is set to 512 width but the read
 *          is misconfigured to read only the 2nd 256 bytes instead of the full
 *          record.
 *
 * Returns: TRUE if the read call should terminate and return no data
 *          FALSE if the read seems to be valid (and not a no-op tcam read)
 *
 */
STATIC int
_soc_mem_read_tcam_is_invalid(int unit, soc_mem_t mem, int index)
{
    if (soc_feature(unit, soc_feature_field_stage_ingress_256_half_slice)) {
        if (mem == FP_GLOBAL_MASK_TCAMm ||  mem == FP_TCAMm) {
            if ((index / 64) % 2) {
               return TRUE;
            }
        }
    }

#ifdef BCM_TRIDENT2PLUS_SUPPORT
    if (soc_feature(unit, soc_feature_field_stage_quarter_slice)
        && soc_feature(unit, soc_feature_field_quarter_slice_single_tcam)
        && soc_feature(unit, soc_feature_field_ingress_two_slice_types)) {
        if ((FP_TCAMm == mem) || (FP_GM_FIELDSm == mem)
            || (FP_GLOBAL_MASK_TCAMm == mem)
            || (FP_GLOBAL_MASK_TCAM_Xm == mem)
            || (FP_GLOBAL_MASK_TCAM_Ym == mem)) {
            if (index < (soc_mem_index_count(unit, mem) / 2)) {
                /* Slices of 2K entries. */
                if (((index / 1024) % 2) || ((index / 512) % 2)) {
                    return (TRUE);
                }
            } else {
                /* Slices of 1K entries. */
                if (((index / 512) % 2) || ((index / 256) % 2)) {
                    return (TRUE);
                }
            }
        }
    }
#endif /* !BCM_TRIDENT2PLUS_SUPPORT */

#ifdef BCM_APACHE_SUPPORT
    if (soc_feature(unit, soc_feature_field_stage_half_slice)
        && soc_feature(unit, soc_feature_field_half_slice_single_tcam)
        && soc_feature(unit, soc_feature_field_ingress_two_slice_types)) {
        if ((FP_TCAMm == mem) || (FP_GM_FIELDSm == mem)
            || (FP_GLOBAL_MASK_TCAMm == mem)
            || (FP_GLOBAL_MASK_TCAM_Xm == mem)
            || (FP_GLOBAL_MASK_TCAM_Ym == mem)) {
            if (index < (soc_mem_index_count(unit, mem) / 2)) {
                /* Slices of 1K entries. */
                if ((index / 512) % 2) {
                    return (TRUE);
                }
            } else {
                /* Slices of 512 entries. */
                if ((index / 256) % 2) {
                    return (TRUE);
                }
            }
        }
    }
#endif /* !BCM_APACHE_SUPPORT */
    if (soc_feature(unit, soc_feature_field_ingress_two_slice_types)
        && soc_feature(unit, soc_feature_field_stage_ingress_512_half_slice)){
        if (mem == FP_GLOBAL_MASK_TCAMm ||  mem == FP_TCAMm) {
            if (index < (soc_mem_index_count(unit, mem) / 2)) {
                if ((index / 256) % 2) {
                    return TRUE;
                }
            }
        }
    }

    if (soc_feature(unit, soc_feature_field_ingress_two_slice_types)
        && soc_feature(unit, soc_feature_field_slices8)) {
        if (mem == FP_GLOBAL_MASK_TCAMm ||  mem == FP_TCAMm) {
            if (index >= (soc_mem_index_count(unit, mem) * 3 / 4)) {
                return TRUE;
            }
        }
    }

    if (soc_feature(unit, soc_feature_field_stage_egress_256_half_slice)) {
        if (mem == EFP_TCAMm) {
            if ((index / 128) % 2) {
                return TRUE;
            }
        }
    }

    if (soc_feature(unit, soc_feature_field_stage_egress_512_half_slice)) {
        if (mem == EFP_TCAMm) {
            if ((index / 256) % 2) {
                return TRUE;
            }
        }
    }



    if (soc_feature(unit, soc_feature_field_stage_lookup_512_half_slice)) {
        if (mem == VFP_TCAMm) {
            if ((index / 128) % 2) {
                return TRUE;
            }
        }
    }

    if (soc_feature(unit, soc_feature_field_stage_lookup_256_half_slice)) {
        if (mem == VFP_TCAMm) {
            if ((index / 64) % 2) {
                return TRUE;
            }
        }
    }

    if (!(soc_feature(unit,
                      soc_feature_field_stage_lookup_512_half_slice) ||
          soc_feature(unit,
                      soc_feature_field_stage_lookup_256_half_slice) ||
          soc_feature(unit,
                      soc_feature_field_stage_egress_256_half_slice) ||
          soc_feature(unit,
                      soc_feature_field_stage_ingress_256_half_slice))) {

        if (!soc_feature(unit, soc_feature_field_stage_quarter_slice)) {
            if (mem == FP_GLOBAL_MASK_TCAMm || mem == FP_TCAMm) {
                /* Do not access h/w for tcam index mapping to the holes */
                if (soc_feature(unit, soc_feature_field_stage_half_slice)
                    && (!soc_feature(unit, soc_feature_field_half_slice_single_tcam))) {
                    if ((index/128) % 2) {
                        return TRUE;
                    }
                } else if (soc_feature(unit,
                                       soc_feature_field_slice_size128)) {
                    if ((index/64) % 2) {
                        return TRUE;
                    }
                }
            } else if (mem == EFP_TCAMm) {
                /* Do not access h/w for tcam index mapping to the holes */
                if (soc_feature(unit, soc_feature_field_stage_half_slice)) {
                    if (SOC_IS_TRIUMPH2(unit) || SOC_IS_APOLLO(unit) ||
                        SOC_IS_VALKYRIE2(unit))  {
                        if ((index/128) % 2) {
                            return TRUE;
                        }
                    }
                }
            }
        }
    }

    if (soc_feature(unit, soc_feature_field_stage_quarter_slice)) {
        /* Do not access h/w for tcam index mapping to the holes */
        if (mem == EFP_TCAMm) {
            if ((SOC_IS_TRIUMPH3(unit)) && ((index/128) % 2)) {
                return TRUE;
            }
        } else if (mem == FP_GLOBAL_MASK_TCAMm || mem == FP_TCAMm ||
                   mem == VFP_TCAMm) {
             if ((SOC_IS_TRIUMPH3(unit)) && (((index/128) % 2) ||
                 ((index/64) % 2))) {
                 return TRUE;
             }
        }
    }

    return FALSE;
}

/*
 * Function: _soc_mem_read_tr3_tunnel_is_invalid
 *
 * Purpose: Called from within _soc_mem_read.  On Triumph3, if attempting to
 *          read from certain RX/TX tunnels in the XLPORT/XWPORT at index
 *          16 or above, return an indication that the read is invalid.
 *
 * Returns: TRUE if the read request is invalid and should return no data
 *          FALSE if the read request seems okay and should continue on
 *
 */
STATIC int
_soc_mem_read_tr3_tunnel_is_invalid(int unit, soc_mem_t mem, int index, int copyno)
{
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (SOC_IS_TRIUMPH3(unit)) {
        if (mem == PORT_EHG_RX_TUNNEL_DATAm || mem == PORT_EHG_RX_TUNNEL_MASKm ||
            mem == PORT_EHG_TX_TUNNEL_DATAm) {
            if ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_XLPORT) ||
                (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_XWPORT)) {
                if (index > 15) {
                    return TRUE;
                }
            }
        }
    }
#endif /* BCM_TRIUMPH3_SUPPORT */

    return FALSE;
}

/*
 * Function: _soc_mem_read_hercules
 *
 * Purpose: Called by _soc_mem_read.  If the unit is a hercules, redirect the
 *          request to the hercules mem read handler.
 *
 * Returns: TRUE if the request was intercepted and handled by hercules
 *          FALSE if the request should proceed normally
 *
 */
STATIC int
_soc_mem_read_hercules(int unit, soc_mem_t mem, int copyno, int index,
                       void *entry_data, int *rv)
{
#ifdef BCM_HERCULES_SUPPORT
    /* Handle Hercules' word read tables separately */
    if (SOC_IS_HERCULES(unit) &&
        (soc_mem_flags(unit, mem) & SOC_MEM_FLAG_WORDADR)) {
        *rv = soc_hercules_mem_read(unit, mem, copyno, index, entry_data);
        return TRUE;
    }
#endif /* BCM_HERCULES_SUPPORT */

    return FALSE;
}

/*
 * Function: _soc_mem_read_tr3_esm_unlock
 *
 * Purpose: Called by _soc_mem_read.  On Triumph3, if ESM correction is enabled
 *          and reading from the ESM or ETU blocks,  release the ESM lock.
 */
STATIC void
_soc_mem_read_tr3_esm_unlock(int unit, int copyno)
{
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_esm_correction) &&
          ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
         (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {

        SOC_ESM_UNLOCK(unit);
    }
#endif
}

/*
 * Function: _soc_mem_read_tr3_esm_lock
 *
 * Purpose: Called by _soc_mem_read.  On Triumph3, if ESM correction is enabled
 *          and reading from the ESM or ETU blocks, take the ESM lock.
 */
STATIC void
_soc_mem_read_tr3_esm_lock(int unit, int copyno)
{
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_esm_correction) &&
          ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
         (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {

        SOC_ESM_LOCK(unit);
    }
#endif
}

/*
 * Function: _soc_mem_read_cache_attempt
 *
 * Purpose: Called from within _soc_mem_read.  If this memory is cached and
 *          it makes sense to serve the read from the cache, then do so.  Also
 *          sets the variable cache_consistency_check=1 if the cache should be
 *          checked at the end of the read for consistency.
 *
 * Returns: TRUE if the read was served from the cache
 *          FALSE if it was not
 *
 */
int
_soc_mem_read_cache_attempt(int unit, uint32 flags, soc_mem_t mem, int copyno,
                            int index, int array_index, void *entry_data,
                            uint32 *entry_data_cache,
                            uint32 *cache_consistency_check)
{
    uint32 *cache;
    uint8  *vmap;
    int entry_dw = soc_mem_entry_words(unit, mem);
    int mem_array_vmap_offset = array_index * soc_mem_index_count(unit, mem);
    int mem_array_cache_offset = mem_array_vmap_offset * entry_dw;

    if (flags & SOC_MEM_DONT_USE_CACHE) {
        return FALSE;
    }

    /* Return data from cache if active */
    cache = SOC_MEM_STATE(unit, mem).cache[copyno];
    vmap = SOC_MEM_STATE(unit, mem).vmap[copyno];

    if (!(SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_SER_WRITE_CACHE_RESTORE) &&
        (cache != NULL) && CACHE_VMAP_TST(vmap, index + mem_array_vmap_offset)
        && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
#ifdef INCLUDE_TCL
        /* Check contents of cache match device table */
#ifdef BCM_TOMAHAWK2_SUPPORT
        /*
         * Due to IPBM part of key field of these tables was forced to 
         * be written to 0, these tables should be read from Cache on TH2.
         */
        if (SOC_IS_TOMAHAWK2(unit) && 
            (mem == IFP_TCAM_WIDEm || 
             mem == IFP_TCAM_WIDE_PIPE0m ||
             mem == IFP_TCAM_WIDE_PIPE1m ||
             mem == IFP_TCAM_WIDE_PIPE2m ||
             mem == IFP_TCAM_WIDE_PIPE3m)) {
            *cache_consistency_check = 0;
        } else
#endif /* BCM_TOMAHAWK2_SUPPORT */
        if ((SOC_IS_TD_TT(unit) &&
             (mem == FP_GLOBAL_MASK_TCAMm || mem == FP_GM_FIELDSm)) ||
             (!SOC_MEM_CACHE_COHERENCY_CHECK(unit)) ||
             (flags & SOC_MEM_DONT_CONVERT_XY2DM)) {
            *cache_consistency_check = 0;
        } else {
            *cache_consistency_check = 1;
        }

        if (*cache_consistency_check) {
            sal_memcpy(entry_data_cache, cache + mem_array_cache_offset +
                       index * entry_dw, entry_dw * 4);
        } else
#endif
        {
            sal_memcpy(entry_data, cache + mem_array_cache_offset +
                       index * entry_dw, entry_dw * 4);
            if (!SOC_CONTROL(unit)->force_read_through) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

int
_soc_mem_alpm_read_cache(int unit, soc_mem_t mem, int copyno,
                         int index, void *entry_data)
{
    int rv = SOC_E_NONE;
    uint32 *cache;
    uint8  *vmap;
    int entry_dw = soc_mem_entry_words(unit, mem);

    if (copyno == MEM_BLOCK_ANY || copyno == SOC_CORE_ALL) {
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }

    MEM_LOCK(unit, mem);

    /* Return data from cache if active */
    cache = SOC_MEM_STATE(unit, mem).cache[copyno];
    vmap = SOC_MEM_STATE(unit, mem).vmap[copyno];

    if (cache != NULL
        && CACHE_VMAP_TST(vmap, index)) {
        sal_memcpy(entry_data, cache + index * entry_dw, entry_dw * 4);
        rv = SOC_E_NONE;
    } else {
        rv = SOC_E_DISABLED;
    }

    MEM_UNLOCK(unit, mem);

    return rv;
}

/*
 * Function: _soc_mem_read_tr3_read_attempt
 *
 * Purpose:  Called from within _soc_mem_read.  On Triumph devices with the
 *           "always drive dbus" feature, add special case handling to reads
 *           from the ESM block.
 *
 * Returns: TRUE if the read was handled by this function
 *          FALSE if this function did nothing
 *
 */
STATIC int
_soc_mem_read_tr3_read_attempt(int unit, soc_mem_t mem, int index, int copyno,
                               void *entry_data, int *rv)
{
#ifdef BCM_TRIUMPH_SUPPORT
    if (soc_feature(unit, soc_feature_always_drive_dbus) &&
        SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM &&
        soc_mem_is_cam(unit, mem)) {

        static const soc_field_t field_mask[] =
            {TMW0f, TMW1f, TMW2f, TMW3f, TMW4f, TMW5f};
        static const soc_field_t field_data[] =
            {TDW0f, TDW1f, TDW2f, TDW3f, TDW4f, TDW5f};

        uint32 tmp_entry[SOC_MAX_MEM_WORDS];
        uint32  sdata[SOC_MAX_MEM_WORDS], src_hit, dst_hit;
        soc_mem_t real_mem, defip_data_mem, src_hit_mem, dst_hit_mem, mask_mem;
        int raw_index, valid;
        int tcam_width, i;
        uint32 mask[4], data[4];

        *rv = soc_tcam_mem_index_to_raw_index(unit, mem, index, &real_mem,
                                              &raw_index);
        if (SOC_FAILURE(*rv)) {
            return TRUE;
        }
        switch (mem) {
        case EXT_ACL360_TCAM_DATAm:
        case EXT_ACL360_TCAM_DATA_IPV6_SHORTm:
            mask_mem = EXT_ACL360_TCAM_MASKm;
            break;
        case EXT_ACL432_TCAM_DATAm:
        case EXT_ACL432_TCAM_DATA_IPV6_LONGm:
        case EXT_ACL432_TCAM_DATA_L2_IPV4m:
        case EXT_ACL432_TCAM_DATA_L2_IPV6m:
            mask_mem = EXT_ACL432_TCAM_MASKm;
            break;
        default:
            mask_mem = INVALIDm;
            break;
        }

        defip_data_mem = INVALIDm;
        src_hit_mem = INVALIDm;
        dst_hit_mem = INVALIDm;
        switch (real_mem) {
        case EXT_IPV4_DEFIPm:
            defip_data_mem = EXT_DEFIP_DATA_IPV4m;
            src_hit_mem = EXT_SRC_HIT_BITS_IPV4m;
            dst_hit_mem = EXT_DST_HIT_BITS_IPV4m;
            tcam_width = 1;
            break;
        case EXT_IPV6_64_DEFIPm:
            defip_data_mem = EXT_DEFIP_DATA_IPV6_64m;
            src_hit_mem = EXT_SRC_HIT_BITS_IPV6_64m;
            dst_hit_mem = EXT_DST_HIT_BITS_IPV6_64m;
            tcam_width = 1;
            break;
        case EXT_IPV6_128_DEFIPm:
            defip_data_mem = EXT_DEFIP_DATA_IPV6_128m;
            src_hit_mem = EXT_SRC_HIT_BITS_IPV6_128m;
            dst_hit_mem = EXT_DST_HIT_BITS_IPV6_128m;
            tcam_width = 2;
            break;
        case EXT_L2_ENTRY_TCAMm:
        case EXT_IPV4_DEFIP_TCAMm:
        case EXT_IPV6_64_DEFIP_TCAMm:
            tcam_width = 1;
            break;
        case EXT_IPV6_128_DEFIP_TCAMm:
        case EXT_ACL144_TCAMm:
            tcam_width = 2;
            break;
        case EXT_ACL288_TCAMm:
            tcam_width = 4;
            break;
        case EXT_ACL360_TCAM_DATAm:
            tcam_width = 5;
            break;
        case EXT_ACL432_TCAM_DATAm:
            tcam_width = 6;
            break;
        case EXT_L2_ENTRYm:
        default:
            tcam_width = 0;
            break;
        }

        sal_memcpy(entry_data, soc_mem_entry_null(unit, real_mem),
                   soc_mem_entry_words(unit, real_mem) * sizeof(uint32));

        if (defip_data_mem != INVALIDm) {
            *rv = soc_mem_read(unit, defip_data_mem, copyno, index, tmp_entry);
            if (SOC_FAILURE(*rv)) {
                return TRUE;
            }
            soc_mem_field_get(unit, defip_data_mem, tmp_entry, SDATAf, sdata);
            soc_mem_field_set(unit, real_mem, entry_data, SDATAf, sdata);
        }
        if (src_hit_mem != INVALIDm) {
            *rv = soc_mem_read(unit, src_hit_mem, copyno, index >> 5,
                              tmp_entry);
            if (SOC_FAILURE(*rv)) {
                return TRUE;
            }
            src_hit = (soc_mem_field32_get(unit, src_hit_mem, tmp_entry,
                                           SRC_HITf) >> (index & 0x1f)) & 1;
            soc_mem_field32_set(unit, real_mem, entry_data, SRC_HITf, src_hit);
        }
        if (dst_hit_mem != INVALIDm) {
            *rv = soc_mem_read(unit, dst_hit_mem, copyno, index >> 5,
                              tmp_entry);
            if (SOC_FAILURE(*rv)) {
                return TRUE;
            }
            dst_hit = (soc_mem_field32_get(unit, dst_hit_mem, tmp_entry,
                                           DST_HITf) >> (index & 0x1f)) & 1;
            soc_mem_field32_set(unit, real_mem, entry_data, DST_HITf, dst_hit);
        }
        if (mask_mem != INVALIDm) {
            sal_memcpy(tmp_entry, soc_mem_entry_null(unit, mask_mem),
                       soc_mem_entry_words(unit, mask_mem) * sizeof(uint32));
        }
        if (tcam_width) {
            for (i = 0; i < tcam_width; i++) {
                *rv = soc_tcam_read_entry(unit, TCAM_PARTITION_RAW,
                                         raw_index + i, mask, data, &valid);
                if (SOC_FAILURE(*rv)) {
                    return TRUE;
                }
                mask[0] = mask[3];
                mask[3] = mask[1];
                mask[1] = mask[2];
                mask[2] = mask[3];
                if (mask_mem == INVALIDm) {
                    soc_mem_field_set(unit, real_mem, entry_data,
                                      field_mask[i], mask);
                } else {
                    soc_mem_field_set(unit, mask_mem, tmp_entry,
                                      field_mask[i], mask);
                }
                data[0] = data[3];
                data[3] = data[1];
                data[1] = data[2];
                data[2] = data[3];
                soc_mem_field_set(unit, real_mem, entry_data, field_data[i],
                                  data);
            }
            if (mask_mem != INVALIDm) {
                *rv = soc_mem_write(unit, mask_mem, copyno, 0, tmp_entry);
            }
            return TRUE;
        }
    }
#endif /* BCM_TRIUMPH_SUPPORT */

    return FALSE;
}

/*
 * Function: _soc_mem_read_td2_xpipe_tbl_attempt
 *
 * Purpose:  Called within _soc_mem_read.  On Trident2, intercept reads from
 *           the X/Y pipes and serve them from memory.
 *
 * Returns:  TRUE if this function handled the read request
 *           FALSE if it did not.
 *
 */
STATIC int
_soc_mem_read_td2_xpipe_tbl_attempt(int unit, soc_mem_t mem, int index,
                                    void *entry_data, int *rv)
{
#ifdef BCM_TRIDENT2_SUPPORT
    /* For MMU_INTFI_X/YPIPE_FC_MAP_TBL0/1, do not read hardware table. */
    if (SOC_IS_TRIDENT2(unit)) {
        if (mem == MMU_INTFI_XPIPE_FC_MAP_TBL0m ||
            mem == MMU_INTFI_XPIPE_FC_MAP_TBL1m ||
            mem == MMU_INTFI_YPIPE_FC_MAP_TBL0m ||
            mem == MMU_INTFI_YPIPE_FC_MAP_TBL1m){

            *rv = soc_trident2_fc_map_shadow_entry_get(unit, mem, index,
                                                       entry_data);
            return TRUE;
        }
    }
#endif

    return FALSE;
}

/*
 * Function: _soc_mem_read_defip_index_map
 *
 * Purpose:  Called from within _soc_mem_read and write.  On many devices, we
 *           have to DEFIP table access, converting the requested index
 *           into the actual hardware value "index2."
 *
 * Returns: nothing, but index2 is set as output.
 *
 */
STATIC void
_soc_mem_read_defip_index_map(int unit, soc_mem_t mem, int index, int *index2)
{
    *index2 = index;

#if defined(BCM_FIREBOLT_SUPPORT)
#if defined(SOC_MEM_L3_DEFIP_WAR) || defined(BCM_TRIUMPH3_SUPPORT) || \
    defined(BCM_ENDURO_SUPPORT)|| defined(BCM_KATANA2_SUPPORT) || \
    defined(BCM_KATANA_SUPPORT)
    if (soc_feature(unit, soc_feature_l3_defip_map) &&
        (mem == L3_DEFIPm ||
         mem == L3_DEFIP_ONLYm ||
         mem == L3_DEFIP_DATA_ONLYm ||
         mem == L3_DEFIP_HIT_ONLY_Xm ||
         mem == L3_DEFIP_HIT_ONLY_Ym ||
         mem == L3_DEFIP_HIT_ONLYm)) {
#ifdef BCM_TRIUMPH3_SUPPORT
        if ((SOC_IS_TRIUMPH3(unit)) || ((SOC_IS_HELIX4(unit)) &&
            (!soc_feature(unit, soc_feature_l3_256_defip_table))) ) {
            *index2 = soc_tr3_l3_defip_index_map(unit, mem, index);
        } else
#endif /* BCM_TRIUMPH3_SUPPORT */
#ifdef BCM_KATANA_SUPPORT
        if (SOC_IS_KATANA(unit)) {
            *index2 = soc_kt_l3_defip_index_map(unit, mem, index);
        } else
#endif /* BCM_KATANA_SUPPORT */
#ifdef BCM_KATANA2_SUPPORT
        if (SOC_IS_KATANA2(unit)) {
            *index2 = soc_kt2_l3_defip_index_map(unit, mem, index);
        } else
#endif /* BCM_KATANA2_SUPPORT */
#if defined(BCM_TRIDENT2_SUPPORT)
        if (SOC_IS_TD2_TT2(unit)) {
            *index2 = soc_trident2_l3_defip_index_map(unit, mem, index);
        } else
#endif /* BCM_TRIDENT2_SUPPORT */
#if defined(BCM_ENDURO_SUPPORT)
        if (SOC_IS_ENDURO(unit)) {
            *index2 = soc_enduro_l3_defip_index_map(unit, mem, index);
        } else
#endif /* BCM_ENDURO_SUPPORT */
        {
#ifdef SOC_MEM_L3_DEFIP_WAR
            *index2 = soc_fb_l3_defip_index_map(unit, index);
#endif /* SOC_MEM_L3_DEFIP_WAR */
        }
    }
#endif /* SOC_MEM_L3_DEFIP_WAR || BCM_TRIUMPH3_SUPPORT */
#endif /* BCM_FIREBOLT_SUPPORT */

#ifdef BCM_TRIUMPH2_SUPPORT
    if (soc_feature(unit, soc_feature_l3_defip_hole) &&
        (mem == L3_DEFIPm ||
         mem == L3_DEFIP_ONLYm ||
         mem == L3_DEFIP_DATA_ONLYm ||
         mem == L3_DEFIP_HIT_ONLYm)) {
        *index2 = soc_tr2_l3_defip_index_map(unit, index);
    }
#endif /* BCM_TRIUMPH2_SUPPORT */
#if defined(BCM_TRIUMPH3_SUPPORT) || defined(BCM_KATANA2_SUPPORT) || \
    defined(BCM_KATANA_SUPPORT)
    if (soc_feature(unit, soc_feature_l3_defip_map) &&
        (mem == L3_DEFIP_PAIR_128m ||
         mem == L3_DEFIP_PAIR_128_ONLYm ||
         mem == L3_DEFIP_PAIR_128_DATA_ONLYm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Xm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Ym ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLYm)) {
#if defined(BCM_TRIUMPH3_SUPPORT)
        if ((SOC_IS_TRIUMPH3(unit)) || ((SOC_IS_HELIX4(unit)) &&
            (!soc_feature(unit, soc_feature_l3_256_defip_table))) ) {
            *index2 = soc_tr3_l3_defip_index_map(unit, mem, index);
        }
#endif /* BCM_TRIUMPH3_SUPPORT */
#if defined(BCM_TRIDENT2_SUPPORT)
        if SOC_IS_TD2_TT2(unit) {
            *index2 = soc_trident2_l3_defip_index_map(unit, mem, index);
        }
#endif /* BCM_TRIDENT2_SUPPORT */
#ifdef BCM_KATANA_SUPPORT
        if (SOC_IS_KATANA(unit)) {
            *index2 = soc_kt_l3_defip_index_map(unit, mem, index);
        }
#endif /* BCM_KATANA_SUPPORT */
#ifdef BCM_KATANA2_SUPPORT
        if (SOC_IS_KATANA2(unit)) {
            *index2 = soc_kt2_l3_defip_index_map(unit, mem, index);
        }
#endif /* BCM_KATANA2_SUPPORT */
    }
#endif /* BCM_TRIUMPH3_SUPPORT || BCM_KATANA2_SUPPORT*/
}

/*
 * Function: _soc_mem_defip_index_remap
 *
 * Purpose:  Called from within _soc_mem_read and write.  On many devices, we
 *           have to check if the index is valid using logical index when access DEFIP tables,
 *           this function is used to convert the physical index to logical index.
 *
 * Returns: nothing, but index2 is set as output.
 *
 */
STATIC void
_soc_mem_defip_index_remap(int unit, soc_mem_t mem, int index, int *index2)
{
    *index2 = index;

#if defined(BCM_TRIDENT2_SUPPORT)
    if SOC_IS_TD2_TT2(unit) {
        *index2 = soc_trident2_l3_defip_index_remap(unit, mem, index);
    }
#endif /* BCM_TRIDENT2_SUPPORT */
}

/*
 * Function: _soc_mem_defip_index_is_invalid
 *
 * Purpose:  Called from within _soc_mem_read and write.  This function is used to check
 *               if this physical index is invalid for this mem view.
 *
 * Returns: TRUE if the index is invalid
 *          FALSE if the index is valid
 */
STATIC int
_soc_mem_defip_index_is_invalid(int unit, soc_mem_t mem, int index)
{
    int index_max = soc_mem_index_max(unit, mem);
    int index_for_check = index;

    if (soc_feature(unit, soc_feature_l3_defip_map)) {
        if (mem == L3_DEFIPm ||
            mem == L3_DEFIP_ONLYm ||
            mem == L3_DEFIP_DATA_ONLYm ||
            mem == L3_DEFIP_HIT_ONLY_Xm ||
            mem == L3_DEFIP_HIT_ONLY_Ym ||
            mem == L3_DEFIP_HIT_ONLYm) {
            index_max = soc_mem_index_max(unit, L3_DEFIPm) +
                        soc_mem_index_max(unit, L3_DEFIP_PAIR_128m) * 2;
        } else if (mem == L3_DEFIP_PAIR_128m ||
            mem == L3_DEFIP_PAIR_128_ONLYm ||
            mem == L3_DEFIP_PAIR_128_DATA_ONLYm ||
            mem == L3_DEFIP_PAIR_128_HIT_ONLY_Xm ||
            mem == L3_DEFIP_PAIR_128_HIT_ONLY_Ym ||
            mem == L3_DEFIP_PAIR_128_HIT_ONLYm) {
            _soc_mem_defip_index_remap(unit, mem, index, &index_for_check);
        }
    }

    if (index_for_check < 0 || index_for_check > index_max) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "invalid index %d for memory %s\n"),
                   index_for_check, SOC_MEM_NAME(unit, mem)));
        return TRUE;
    }

    return FALSE;
}

/*
 * Function: _soc_mem_read_tr_response_word_update
 *
 * Purpose:  Called from within _soc_mem_read.  When reading certain tables on
 *           TRIUMPH devices with ESM support, the schan response will have a
 *           response word in it.  If so, set the resp_word variable.
 *
 * Returns: nothing, but resp_word may be updated
 *
 * Note:
 *  We probably should invent SOC_MEM_FLAG_HAS_RESP_WORD in regsfile
 *  for such tables (or define a bit in S-chan message header if
 *  hardware can do that)
 */
STATIC void
_soc_mem_read_tr_response_word_update(int unit, soc_mem_t mem,
                                      int *resp_word)
{
#ifdef BCM_TRIUMPH_SUPPORT
    if ((!SAL_BOOT_PLISIM || SAL_BOOT_BCMSIM) &&
        soc_feature(unit, soc_feature_esm_support)) {
        switch (mem) {
        case EXT_L2_ENTRYm:
        case EXT_L2_ENTRY_TCAMm:
        case EXT_L2_ENTRY_DATAm:
        case EXT_DEFIP_DATAm:
        case EXT_IPV4_DEFIP_TCAMm:
        case EXT_IPV4_DEFIPm:
        case EXT_IPV6_64_DEFIP_TCAMm:
        case EXT_IPV6_64_DEFIPm:
        case EXT_IPV6_128_DEFIP_TCAMm:
        case EXT_IPV6_128_DEFIPm:
        case EXT_FP_POLICYm:
        case EXT_FP_CNTRm:
        case EXT_FP_CNTR8m:
        case EXT_ACL144_TCAMm:
        case EXT_ACL288_TCAMm:
        case EXT_ACL360_TCAM_DATAm: /* not for MASK portion */
        case EXT_ACL432_TCAM_DATAm: /* not for MASK portion */
        /* and following software invented tables as well */
        case EXT_DEFIP_DATA_IPV4m:
        case EXT_DEFIP_DATA_IPV6_64m:
        case EXT_DEFIP_DATA_IPV6_128m:
        case EXT_FP_POLICY_ACL288_L2m:
        case EXT_FP_POLICY_ACL288_IPV4m:
        case EXT_FP_POLICY_ACL360_IPV6_SHORTm:
        case EXT_FP_POLICY_ACL432_IPV6_LONGm:
        case EXT_FP_POLICY_ACL144_L2m:
        case EXT_FP_POLICY_ACL144_IPV4m:
        case EXT_FP_POLICY_ACL144_IPV6m:
        case EXT_FP_POLICY_ACL432_L2_IPV4m:
        case EXT_FP_POLICY_ACL432_L2_IPV6m:
        case EXT_FP_CNTR_ACL288_L2m:
        case EXT_FP_CNTR_ACL288_IPV4m:
        case EXT_FP_CNTR_ACL360_IPV6_SHORTm:
        case EXT_FP_CNTR_ACL432_IPV6_LONGm:
        case EXT_FP_CNTR_ACL144_L2m:
        case EXT_FP_CNTR_ACL144_IPV4m:
        case EXT_FP_CNTR_ACL144_IPV6m:
        case EXT_FP_CNTR_ACL432_L2_IPV4m:
        case EXT_FP_CNTR_ACL432_L2_IPV6m:
        case EXT_FP_CNTR8_ACL288_L2m:
        case EXT_FP_CNTR8_ACL288_IPV4m:
        case EXT_FP_CNTR8_ACL360_IPV6_SHORTm:
        case EXT_FP_CNTR8_ACL432_IPV6_LONGm:
        case EXT_FP_CNTR8_ACL144_L2m:
        case EXT_FP_CNTR8_ACL144_IPV4m:
        case EXT_FP_CNTR8_ACL144_IPV6m:
        case EXT_FP_CNTR8_ACL432_L2_IPV4m:
        case EXT_FP_CNTR8_ACL432_L2_IPV6m:
        case EXT_ACL288_TCAM_L2m:
        case EXT_ACL288_TCAM_IPV4m:
        case EXT_ACL360_TCAM_DATA_IPV6_SHORTm:
        case EXT_ACL432_TCAM_DATA_IPV6_LONGm:
        case EXT_ACL144_TCAM_L2m:
        case EXT_ACL144_TCAM_IPV4m:
        case EXT_ACL144_TCAM_IPV6m:
        case EXT_ACL432_TCAM_DATA_L2_IPV4m:
        case EXT_ACL432_TCAM_DATA_L2_IPV6m:
#ifdef BCM_TRIUMPH3_SUPPORT
            if (SOC_IS_TRIUMPH3(unit)) {
                break;
            }
#endif /* BCM_TRIUMPH3_SUPPORT */
            *resp_word = 1;
            break;
        default:
            break;
        }
    }
#endif /* BCM_TRIUMPH_SUPPORT */
}

STATIC void 
_soc_mem_acc_type_swap(int unit, schan_header_t *header) {
    int acc_type_swap = 0, acc_type_old = 0;

    if (soc_feature(unit, soc_feature_sbus_format_v4)) {
        acc_type_old = header->v4.acc_type;
    } else if (soc_feature(unit, soc_feature_new_sbus_format)) {
        acc_type_old = header->v3.acc_type;
    } else {
        acc_type_old = header->v2.src_blk;
    }

    if (SOC_IS_TOMAHAWKX(unit)) {
        if (acc_type_old == _SOC_MEM_ADDR_ACC_TYPE_PIPE_1) {
            acc_type_swap = _SOC_MEM_ADDR_ACC_TYPE_PIPE_0;
        } else {
            acc_type_swap = _SOC_MEM_ADDR_ACC_TYPE_PIPE_1;
        }
    } else {
        if (acc_type_old == _SOC_MEM_ADDR_ACC_TYPE_PIPE_Y) {
            acc_type_swap = _SOC_MEM_ADDR_ACC_TYPE_PIPE_X;
        } else {
            acc_type_swap = _SOC_MEM_ADDR_ACC_TYPE_PIPE_Y;
        }
    }

    if (soc_feature(unit, soc_feature_sbus_format_v4)) {
        header->v4.acc_type = acc_type_swap;
    } else if (soc_feature(unit, soc_feature_new_sbus_format)) {
        header->v3.acc_type = acc_type_swap;
    } else {
        header->v2.src_blk = acc_type_swap;
    }
}

/*
 * Function: _soc_mem_read_ser_correct
 *
 * Purpose:  Called from within _soc_mem_read.  If there was a error reading,
 *           try to use SER to correct it.
 *
 * Returns:  TRUE if the error was corrected,
 *           FALSE if it was not.
 *
 */
STATIC int
_soc_mem_read_ser_correct(int unit, uint32 flags, soc_mem_t mem, int copyno,
                          int index, void *entry_data,
                          schan_msg_t *schan_msg, schan_msg_t *schan_msg_cpy,
                          int resp_word, int *rv, int *all_done)
{
    uint32 *cache = SOC_MEM_STATE(unit, mem).cache[copyno];
    uint8  *vmap  = SOC_MEM_STATE(unit, mem).vmap[copyno];
    int entry_dw = soc_mem_entry_words(unit, mem);
    int allow_intr = SOC_IS_SAND(unit) ? 1 : 0;

    if (!SOC_SER_CORRECTION_SUPPORT(unit)) {
        return FALSE;
    }

#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_HELIX4_SUPPORT) || \
        defined(BCM_KATANA2_SUPPORT) || defined(BCM_HURRICANE2_SUPPORT) || \
        defined(BCM_GREYHOUND_SUPPORT)
    do {
        uint8 *corrupt = NULL;

        /* Record error index for TCAM table */
        if((SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) &&
           (!(SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_EXT_CAM))) {
            corrupt = SOC_MEM_STATE(unit, mem).corrupt[copyno];
            if (corrupt != NULL) {
                TCAM_CORRUPT_MAP_SET(corrupt, index);
                LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
                            (BSL_META_U(unit,
                                "Mem[%s] index[%d] "
                                "TCAM parity error indicating bit is set.\n"),
                             SOC_MEM_NAME(unit, mem), index));
            }
        }
    } while (0);
#endif

    if (flags & SOC_MEM_SCHAN_ERR_RETURN) {
        return FALSE;
    }

    if (cache != NULL && CACHE_VMAP_TST(vmap, index)
        && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
        sal_memcpy(entry_data, cache + index * entry_dw, entry_dw * 4);
        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                                "Unit:%d Mem[%s] index[%d] "
                                "Force fetch data from cache.\n"),
                     unit, SOC_MEM_NAME(unit, mem), index));
    } else if (SOC_IS_TD2_TT2(unit) &&
        soc_feature(unit, soc_feature_two_ingress_pipes) &&
        (mem == L3_DEFIP_ALPM_IPV4m || mem == L3_DEFIP_ALPM_IPV4_1m ||
         mem == L3_DEFIP_ALPM_IPV6_64m || mem == L3_DEFIP_ALPM_IPV6_64_1m ||
         mem == L3_DEFIP_ALPM_IPV6_128m || mem == L3_DEFIPm ||
         mem == L3_DEFIP_PAIR_128m)) {
        /* Fetch data from other pipe */
        _soc_mem_acc_type_swap(unit, &(schan_msg_cpy->readcmd.header));

        *rv = soc_schan_op(unit, schan_msg_cpy, 2, 1 + entry_dw + resp_word, allow_intr);
        if (SOC_FAILURE(*rv)) {
            return TRUE;
        }

        sal_memcpy(schan_msg, schan_msg_cpy, sizeof(*schan_msg));
        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                                "Unit:%d Mem[%s] index[%d] "
                                "Force fetch data from other pipe.\n"),
                     unit, SOC_MEM_NAME(unit, mem), index));
        return TRUE;
    } else {
        void *null_entry = soc_mem_entry_null(unit, mem);
        sal_memcpy(entry_data, null_entry, entry_dw * 4);
        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                                "Unit:%d Mem[%s] index[%d] "
                                "Force fetch null data.\n"),
                     unit, SOC_MEM_NAME(unit, mem), index));
        return TRUE;
    }

    return FALSE;
}

/*
 * Function: _soc_mem_read_td_tt_byte_len_update
 *
 * Purpose:  Called within _soc_mem_read.  On Trident/titan chips, the expected
 *           data length from certain memories has to be set as a special case.
 *
 * Returns:  Nothing, but may alter the data_byte_len variable if necessary.
 *
 */
STATIC void
_soc_mem_read_td_tt_byte_len_update(int unit, soc_mem_t mem, int entry_dw,
                                   int *data_byte_len)
{
#ifdef BCM_TRIDENT_SUPPORT
    if (SOC_IS_TD_TT(unit)) {
        if (mem == ESBS_PORT_TO_PIPE_MAPPINGm ||
            mem == ISBS_PORT_TO_PIPE_MAPPINGm) {
            *data_byte_len = entry_dw * 4;
        }
    }
#endif /* BCM_TRIDENT_SUPPORT */
}

/*
 * Function: _soc_mem_dst_blk_update
 *
 * Purpose:  Called within _soc_mem_read/write.  In certain configurations, the
 *           destination block for schan requests must be calculated as a
 *           special case and not read directly from the regsfile.
 *
 * Returns:  Nothing, but may alter the dst_blk variable if necessary.
 *
 */
STATIC void
_soc_mem_dst_blk_update(int unit, int copyno, int maddr, int *dst_blk)
{
    *dst_blk = 0;

#if defined(BCM_EXTND_SBUS_SUPPORT) || defined(BCM_SAND_SUPPORT)

    if (soc_feature(unit, soc_feature_new_sbus_format)) {
        *dst_blk = SOC_BLOCK2SCH(unit, copyno);
    } else

#endif
    {
#if defined(BCM_XGS3_SWITCH_SUPPORT) || defined(BCM_SIRIUS_SUPPORT) ||\
    defined(BCM_SAND_SUPPORT)
        /* required on XGS3. Optional on other devices */
        *dst_blk = ((maddr >> SOC_BLOCK_BP) & 0xf) |
            (((maddr >> SOC_BLOCK_MSB_BP) & 0x3) << 4);
#endif /* BCM_XGS3_SWITCH_SUPPORT || BCM_SIRIUS_SUPPORT || BCM_SAND_SUPPORT */
    }
}

/*
 * Function: _soc_mem_read_sirius_sim_adjust
 *
 * Purpose:  Called within _soc_mem_read.  On Sirius devices, in certain cases
 *           the schan read parameters should be zero'd out.
 *
 * Returns:  Nothing, but may alter the 3 input parameters (other than unit).
 *
 */
STATIC void
_soc_mem_read_sirius_sim_adjust(int unit, int *src_blk, int *data_byte_len,
                                uint32 *maddr)
{
#ifdef BCM_SIRIUS_SUPPORT
    if (SOC_IS_SIRIUS(unit) && (!SAL_BOOT_PLISIM || SAL_BOOT_BCMSIM)) {
        *src_blk = 0;
        *data_byte_len = 0;
        /* mask off the block field */
        *maddr &= 0x3F0FFFFF;
    }
#endif
}

#ifdef HR3LITE_LPM_WAR
/*
 * Function: _soc_mem_hr3_lpm_hit_shadow_update
 *
 * Purpose:  Called by _soc_mem_read_schan_msg_send(). 
 *          This function is used to sync-up the correct LPM-HIT entry on 
 *          HR3-Lite device. LPM table in HR3-Lite support 64 entries only, 
 *          but LPM-TCAM on entry between 32~63 can only be hit with 
 *          LPM-DATA on entry between 512~543 and the hit status is updated  
 *          by HW on LPM-HIT entry 512~543 as well.
 *
 * Returns:  SOC_E_xxx
 */

STATIC int
_soc_mem_hr3_lpm_hit_shadow_update(int unit, soc_mem_t mem, int copyno,
                    int index, void *lpm_entry_data, unsigned array_index)
{
    defip_entry_t *lpm_entry;
    defip_hit_only_entry_t lpm_hit_entry;
    int defip_hit_size = 0;
    int lpm_hit_shadow_index = -1;
    schan_msg_t schan_msg;
    int opcode, err;
    int resp_word = 0;
    int entry_dw = soc_mem_entry_words(unit, L3_DEFIP_HIT_ONLYm);
    int rv = SOC_E_NONE;
    int src_blk, dst_blk, acc_type, data_byte_len;
    uint32 maddr;
    uint8 at;
    uint32 new_hit = 0, old_hit = 0;

    if (soc_feature(unit, soc_feature_hr3_lite_lpm_shadow_hit)) {
        if ((NEED_HR3LITE_LPM_SHADOW(mem)) && (mem != L3_DEFIP_DATA_ONLYm)) {
            lpm_hit_shadow_index = HR3LITE_LPM_HIT_INDEX2(index);
        }
    }

    if (lpm_hit_shadow_index == -1) {
        /* no sync-up process is required */
        return SOC_E_NONE;
    }

    LOG_INFO(BSL_LS_SOC_LPM, (BSL_META_U(unit,
            "HR3-Lite LPM_WAR:update LPM(%s,idx=%d) HIT from entry(%d)!\n"),
            SOC_MEM_NAME(unit, mem),index, lpm_hit_shadow_index));
    /* init entry data buf */
    defip_hit_size = sizeof(defip_hit_only_entry_t);
    sal_memset(&lpm_hit_entry, 0, defip_hit_size);

    sal_memset(&schan_msg, 0, sizeof(schan_msg_t));

    /* Setup S-Channel command packet */
    schan_msg_clear(&schan_msg);
    src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
    acc_type = SOC_MEM_ACC_TYPE(unit, L3_DEFIP_HIT_ONLYm);
    data_byte_len = 4;

    maddr = soc_mem_addr_get(unit, L3_DEFIP_HIT_ONLYm, array_index, 
            copyno, lpm_hit_shadow_index, &at);
    schan_msg.readcmd.address = maddr;

    _soc_mem_dst_blk_update(unit, copyno, maddr, &dst_blk);
    _soc_mem_read_tr_response_word_update(unit, mem, &resp_word);

    soc_schan_header_cmd_set(unit, &schan_msg.header, READ_MEMORY_CMD_MSG,
                             dst_blk, src_blk, acc_type, data_byte_len, 0, 0);

    /*
     * Write onto S-Channel "memory read" command packet consisting of header
     * word + address word, and read back header word + entry_dw data words.
     */
    if (2 + entry_dw > CMIC_SCHAN_WORDS(unit)) {
        LOG_WARN(BSL_LS_SOC_SOCMEM, (BSL_META_U(unit,
                "soc_mem_read: assert will fail for memory %s\n"),
                SOC_MEM_NAME(unit, L3_DEFIP_HIT_ONLYm)));
    }

    rv = soc_schan_op(unit, &schan_msg, 2, 1 + entry_dw + resp_word, 0);

    soc_schan_header_status_get(unit, &schan_msg.header, &opcode, NULL, NULL,
                                &err, NULL, NULL);
    if (opcode != READ_MEMORY_ACK_MSG || err != 0) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM, (BSL_META_U(unit,
                "soc_mem_read: Mem(%s) invalid S-Channel reply, "
                "expected READ_MEMORY_ACK:, opcode %d\n"), 
                SOC_MEM_NAME(unit,L3_DEFIP_HIT_ONLYm), opcode));
        soc_schan_dump(unit, &schan_msg, 1 + entry_dw + resp_word);
        rv = SOC_E_INTERNAL;
        return rv;
    }

    sal_memcpy(&lpm_hit_entry,
               resp_word ? schan_msg.genresp.data : schan_msg.readresp.data,
               entry_dw * sizeof (uint32));

    /* sync-up hit field */
    new_hit = soc_L3_DEFIP_HIT_ONLYm_field32_get(unit, &lpm_hit_entry, HITf);
    if (mem == L3_DEFIP_HIT_ONLYm) {
        old_hit = soc_L3_DEFIP_HIT_ONLYm_field32_get(unit, lpm_entry_data, HITf);
        /* sync-up the HIT field */
        sal_memcpy(lpm_entry_data, &lpm_hit_entry, entry_dw * sizeof (uint32));
    } else if (mem == L3_DEFIPm) {
        lpm_entry = lpm_entry_data;
        old_hit = soc_L3_DEFIPm_field32_get(unit, lpm_entry, HITf);
        /* sync-up the HIT field */
        soc_L3_DEFIPm_field32_set(unit, lpm_entry, HITf, new_hit);
    }
    LOG_INFO(BSL_LS_SOC_LPM, (BSL_META_U(unit,
            "HR3-Lite LPM_WAR:update LPM(%s,idx=%d) new_hit=%d, old_hit=%d\n"),
            SOC_MEM_NAME(unit, mem),index, new_hit, old_hit));

    return rv;
}
#endif  /* HR3LITE_LPM_WAR */

/*
 * Function: _soc_mem_read_schan_msg_send
 *
 * Purpose:  Called within _soc_mem_read.  Construct and send a schan message
 *           holding the read requests, and parse the response.  If an error
 *           happens, try to correct with SER.
 *
 * Returns:  Standard BCM_E_* code
 *
 */
STATIC int
_soc_mem_read_schan_msg_send(int unit, uint32 flags, soc_mem_t mem, int copyno,
                             int index, void *entry_data, unsigned array_index,
                             int remapped_index)
{
    schan_msg_t schan_msg, schan_msg_cpy;
    int opcode, err;
    int resp_word = 0;
    int entry_dw = soc_mem_entry_words(unit, mem);
    int rv = SOC_E_NONE;
    int allow_intr = SOC_IS_SAND(unit) ? 1 : 0;
    int src_blk, dst_blk, acc_type, data_byte_len;
    uint32 maddr;
    uint8 at;

    sal_memset(&schan_msg, 0, sizeof(schan_msg_t));

    /* Setup S-Channel command packet */
    schan_msg_clear(&schan_msg);
    src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
    acc_type = SOC_MEM_ACC_TYPE(unit, mem);
    data_byte_len = 4;

    maddr = soc_mem_addr_get(unit, mem, array_index, copyno, remapped_index,
                             &at);
    schan_msg.readcmd.address = maddr;

    _soc_mem_read_td_tt_byte_len_update(unit, mem, entry_dw, &data_byte_len);
    _soc_mem_dst_blk_update(unit, copyno, maddr, &dst_blk);
    _soc_mem_read_tr_response_word_update(unit, mem, &resp_word);
    _soc_mem_read_sirius_sim_adjust(unit, &src_blk, &data_byte_len, &maddr);

    soc_schan_header_cmd_set(unit, &schan_msg.header, READ_MEMORY_CMD_MSG,
                             dst_blk, src_blk, acc_type, data_byte_len, 0, 0);

    /*
     * Write onto S-Channel "memory read" command packet consisting of header
     * word + address word, and read back header word + entry_dw data words.
     */
    if (2 + entry_dw > CMIC_SCHAN_WORDS(unit)) {
       LOG_WARN(BSL_LS_SOC_SOCMEM,
                (BSL_META_U(unit,
                            "soc_mem_read: assert will fail for memory %s\n"),
                 SOC_MEM_NAME(unit, mem)));
    }

    if (soc_feature(unit, soc_feature_two_ingress_pipes)) {
        sal_memcpy(&schan_msg_cpy, &schan_msg, sizeof(schan_msg));
    }
    rv = soc_schan_op(unit, &schan_msg, 2, 1 + entry_dw + resp_word, allow_intr);
    if (SOC_FAILURE(rv)) {
        int all_done = FALSE;
        _soc_mem_read_ser_correct(unit, flags, mem, copyno, index, entry_data,
                                  &schan_msg, &schan_msg_cpy, resp_word, &rv,
                                  &all_done);
        if (SOC_FAILURE(rv) || all_done) {
            return rv;
        }
    }

    soc_schan_header_status_get(unit, &schan_msg.header, &opcode, NULL, NULL,
                                &err, NULL, NULL);
    if (opcode != READ_MEMORY_ACK_MSG || err != 0) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_read: "
                              "Mem(%s) "
                              "invalid S-Channel reply, expected READ_MEMORY_ACK:, opcode %d\n"),SOC_MEM_NAME(unit,mem), opcode));
        soc_schan_dump(unit, &schan_msg, 1 + entry_dw + resp_word);
        rv = SOC_E_INTERNAL;
        return rv;
    }

    sal_memcpy(entry_data,
               resp_word ? schan_msg.genresp.data : schan_msg.readresp.data,
               entry_dw * sizeof (uint32));
#ifdef USE_VALGRIND_CLIENT_REQUESTS
               VALGRIND_MAKE_MEM_DEFINED(entry_data, entry_dw * sizeof (uint32));
#endif /* USE_VALGRIND_CLIENT_REQUESTS */

#ifdef HR3LITE_LPM_WAR
    /* if target table is HR3-Lite's LPM, HIT bit is updated by HW from  
     * LPM-HIT table and for HR3-Lite's LPM entry 32~63 this HIT bit needs 
     * WAR to sync-up correct HIT from LPM-HIT entry 512~543.
     */
    if (soc_feature(unit, soc_feature_hr3_lite_lpm_shadow_hit) && 
            NEED_HR3LITE_LPM_SHADOW(mem)) {
        rv = _soc_mem_hr3_lpm_hit_shadow_update(unit, 
                mem, copyno, index, entry_data, array_index);
    }
#endif  /* HR3LITE_LPM_WAR */

    return rv;
}

/*
 * Function: _soc_mem_read_tcam_to_dm_format
 *
 * Purpose:  On certain devices, data from TCAMs and the ETU block has to be
 *           converted from X-Y format to D-M format.
 *
 */
STATIC void _soc_mem_read_tcam_to_dm_format(int unit, soc_mem_t mem, int copyno,
                                            soc_mem_info_t *meminfo, uint32 flags,
                                            void *entry_data)
{
#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) \
    || defined(BCM_HURRICANE2_SUPPORT)
    if (soc_feature(unit, soc_feature_xy_tcam_direct) &&
        (meminfo->flags & SOC_MEM_FLAG_CAM) &&
        (!(meminfo->flags & SOC_MEM_FLAG_EXT_CAM)) &&
        (!(flags & SOC_MEM_DONT_CONVERT_XY2DM))) {
#if defined(BCM_TRIDENT2PLUS_SUPPORT) 
        _soc_mem_fp_global_mask_tcam_shift(unit, mem, entry_data, 0, 0);
#endif
        _soc_mem_tcam_xy_to_dm(unit, mem, 1, entry_data, entry_data);
    }
#endif /* BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT */

#ifdef BCM_TRIUMPH3_SUPPORT
    /* convert X-Y data of esm external tcam table reads to D-M format */
    if (soc_feature(unit, soc_feature_etu_support) &&
        SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU) {

        uint32 key[SOC_MAX_MEM_FIELD_WORDS], mask[SOC_MAX_MEM_FIELD_WORDS];
        soc_field_t key_field[4], mask_field[4];
        int field_count=0, bit_length, word_length, i, word;

        switch (mem) {
        case EXT_L2_ENTRY_TCAMm:
        case EXT_L2_ENTRY_TCAM_WIDEm:
        case EXT_IPV4_TCAMm:
        case EXT_IPV6_64_TCAMm:
        case EXT_IPV4_DEFIPm:
        case EXT_IPV4_DEFIP_TCAMm:
        case EXT_IPV4_UCASTm:
        case EXT_IPV4_UCAST_TCAMm:
        case EXT_IPV4_UCAST_WIDEm:
        case EXT_IPV4_UCAST_WIDE_TCAMm:
        case EXT_IPV6_64_DEFIPm:
        case EXT_IPV6_64_DEFIP_TCAMm:
        case EXT_ACL80_TCAMm:
            key_field[0] = TDW0f;
            mask_field[0] = TMW0f;
            field_count = 1;
            break;

        case EXT_IPV6_128_TCAMm:
        case EXT_IPV6_128_DEFIPm:
        case EXT_IPV6_128_DEFIP_TCAMm:
        case EXT_IPV6_128_UCASTm:
        case EXT_IPV6_128_UCAST_TCAMm:
        case EXT_IPV6_128_UCAST_WIDEm:
        case EXT_IPV6_128_UCAST_WIDE_TCAMm:
        case EXT_ACL160_TCAMm:
        case EXT_ACL144_TCAM_L2m:
        case EXT_ACL144_TCAM_IPV4m:
        case EXT_ACL144_TCAM_IPV6m:
            key_field[0] = TDW0f;
            key_field[1] = TDW1f;
            mask_field[0] = TMW0f;
            mask_field[1] = TMW1f;
            field_count = 2;
            break;

        case EXT_ACL288_TCAM_L2m:
        case EXT_ACL288_TCAM_IPV4m:
        case EXT_ACL432_TCAM_DATA_L2_IPV4m:
        case EXT_ACL320_TCAMm:
            key_field[0] = TDW0f;
            key_field[1] = TDW1f;
            key_field[2] = TDW2f;
            key_field[3] = TDW3f;
            mask_field[0] = TMW0f;
            mask_field[1] = TMW1f;
            mask_field[2] = TMW2f;
            mask_field[3] = TMW3f;
            field_count = 4;
            break;

        
#ifdef fix_later
        case EXT_ACL360_TCAM_DATA_IPV6_SHORTm:
        case EXT_ACL432_TCAM_DATA_IPV6_LONGm:
        case EXT_ACL432_TCAM_DATA_L2_IPV6m:
        case EXT_ACL432_TCAM_MASKm:
        case EXT_ACL360_TCAM_MASK_IPV6_SHORTm:
        case EXT_ACL432_TCAM_MASK_IPV6_LONGm:
        case EXT_ACL432_TCAM_MASK_L2_IPV6m:
        case EXT_ACL480_TCAM_DATAm:
        case EXT_ACL480_TCAM_MASKm:
            key_field[0] = TDW0f;
            key_field[1] = TDW1f;
            key_field[2] = TDW2f;
            key_field[3] = TDW3f;
            mask_field[0] = TMW0f;
            mask_field[1] = TMW1f;
            mask_field[2] = TMW2f;
            mask_field[3] = TMW3f;
            field_count = 6;
            break;
#endif
        }
        for (i = 0; i < field_count; i++) {
            soc_mem_field_get(unit, mem, entry_data, key_field[i], key);
            soc_mem_field_get(unit, mem, entry_data, mask_field[i], mask);
            bit_length = soc_mem_field_length(unit, mem, key_field[i]);
            word_length = (bit_length + 31) / 32;
            for (word = 0; word < word_length; word++) {
                /* save data word first */
                mask[word] = ~(key[word] | mask[word]);
            }
            if ((bit_length & 0x1f) != 0) {
                mask[word - 1] &= (1 << (bit_length & 0x1f)) - 1;
            }
            soc_mem_field_set(unit, mem, entry_data, mask_field[i], mask);
            soc_mem_field_set(unit, mem, entry_data, key_field[i], key);
        }
    }
#endif /* BCM_TRIUMPH3_SUPPORT */

}

void soc_mem_parity_field_clear(int unit, int mem,
                            void* entry_data, void *entry_data_cache)
{
    /* Heuristic to clear parity/ecc bits */
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITYf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITYf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_Af);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_Bf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_LOWERf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_UPPERf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_P0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_P1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_P2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_P3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PBM_PARITY_P0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PBM_PARITY_P1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PBM_PARITY_P2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PBM_PARITY_P3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_EGR_VLAN_STG_PARITY_P0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_EGR_VLAN_STG_PARITY_P1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_EGR_VLAN_STG_PARITY_P2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_EGR_VLAN_STG_PARITY_P3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_4f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCPf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP4f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP5f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP6f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP7f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_4f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_4f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PBM_PARITY_P0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PBM_PARITY_P1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PBM_PARITY_P2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PBM_PARITY_P3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_P0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_P1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_P2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_PARITY_P3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_EGR_VLAN_STG_PARITY_P0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_EGR_VLAN_STG_PARITY_P1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_EGR_VLAN_STG_PARITY_P2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EVEN_EGR_VLAN_STG_PARITY_P3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_P0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_P1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_P2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_P3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_P0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_P1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_P2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_P3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_P0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_P1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_P2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_P3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_PBM_0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_PBM_1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_PBM_2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_PBM_3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_PBM_0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_PBM_1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_PBM_2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_PBM_3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_PBM_0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_PBM_1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_PBM_2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_PBM_3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, TCAM_PARITY_KEYf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, TCAM_PARITY_MASKf);

    /* Clear hit bits */
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HITf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HIT0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HIT1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HIT_0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HIT_1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HIT_2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HIT_3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HITDA_0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HITDA_1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HITDA_2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HITDA_3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HITSA_0f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HITSA_1f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HITSA_2f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HITSA_3f);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HITDAf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HITSAf); 
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, B0_HITf); 
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, B1_HITf); 
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, L3_HIT_DCMf); 
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, L3_HIT_PMf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, HIT_BITSf);
    /* Clear hardware-modified fields in meter tables  */
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, BUCKETCOUNTf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, REFRESHCOUNTf);
    
    /* Clear parity/ecc bits on svm_meter_table */
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_EXCESSf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_EXCESSf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_EXCESSf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_COMMITTEDf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_COMMITTEDf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_COMMITTEDf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_LRNf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_LRNf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_LRNf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECCP_CONFIGf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, PARITY_CONFIGf);
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, ECC_CONFIGf);
    /* Clear hardware-modified fields in svm_meter_table */
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, LRNf);         /* LAST_REFRESH_NUMBERf */
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, COMMITTEDf);   /* COMMITTED_BUCKETCOUNTf */
    _SOC_ENTRY_PARITY_CLEAR(unit, mem, EXCESSf);      /* EXCESS_BUCKETCOUNTf */
}

/*
 * Function: _soc_mem_read_cache_check_clear
 *
 * Purpose:  Called from _soc_mem_read.  After a read has accessed the cache,
 *           some cleanup may be necessary.  Perform that here.
 */
STATIC void
_soc_mem_read_cache_check_clear(int unit, int mem, int index, int copyno,
                                void *entry_data, void *entry_data_cache)
{
#ifdef INCLUDE_TCL
    int entry_dw = soc_mem_entry_words(unit, mem);

    /* Some exceptions */
    if ((SOC_IS_KATANA2(unit) || SOC_IS_TD_TT(unit) ||
        SOC_IS_TD2_TT2(unit)) && (mem == EGR_VLANm)) {
        return;
    }

    /* Heuristic to check if entry is valid */
    _SOC_ENTRY_VALID_CHECK(unit, mem, VALIDf);
    _SOC_ENTRY_VALID_CHECK(unit, mem, VALID_0f);

    soc_mem_parity_field_clear(unit, mem, entry_data, entry_data_cache);

    if (sal_memcmp(entry_data_cache, entry_data,
                   entry_dw * sizeof (uint32)) != 0) {
        if (bsl_check(bslLayerSoc, bslSourceSocmem, bslSeverityError, unit)) {
            LOG_CLI((BSL_META_U(unit,
                                "soc_mem_read unit %d: %s.%s[%d]: cache: "),
                     unit, SOC_MEM_NAME(unit, mem),
                     SOC_BLOCK_NAME(unit, copyno), index));
            soc_mem_entry_dump(unit, mem, entry_data_cache);

            LOG_CLI((BSL_META_U(unit,
                                "\n")));
            LOG_CLI((BSL_META_U(unit,
                                "soc_mem_read unit %d: %s.%s[%d]: Device: "),
                     unit, SOC_MEM_NAME(unit, mem),
                     SOC_BLOCK_NAME(unit, copyno), index));
            soc_mem_entry_dump(unit, mem, entry_data);
            LOG_CLI((BSL_META_U(unit,
                                "\n")));
        }

    }
#endif
}

/*
 * Function:
 *    _soc_mem_read
 * Purpose:
 *    Read a memory internal to the SOC.
 * Notes:
 *    GBP/CBP memory should only accessed when MMU is in DEBUG mode.
 */
STATIC int
_soc_mem_read(int unit, uint32 flags, soc_mem_t mem,
              /* in memory arrays this is the element index in the array, otherwise 0 */
              unsigned array_index,
              int copyno, int index, void *entry_data)
{
    soc_mem_info_t *meminfo;
    int index2;
    int rv, rv2 = SOC_E_NONE;
    uint32 entry_data_cache[SOC_MAX_MEM_FIELD_WORDS];
    uint32 cache_consistency_check = 0;
    int ok = TRUE;
    uint32 entry_num_max = 0;
#ifdef BCM_PETRA_SUPPORT
    int       entry_dw;
#endif

    SOC_MEM_ALIAS_TO_ORIG(unit,mem);

    if (!soc_mem_is_valid(unit, mem)) {
        return SOC_E_MEMORY;
    }

#ifdef BCM_PETRA_SUPPORT
    entry_dw = soc_mem_entry_words(unit, mem);
    /*
     * Write onto S-Channel "memory read" command packet consisting of header
     * word + address word, and read back header word + entry_dw data words.
     */
    if (SOC_IS_DPP(unit) && (entry_dw + 2 > CMIC_SCHAN_WORDS(unit))) {
        if (SOC_IS_ARADPLUS_AND_BELOW(unit)) {
            SOC_DPP_ALLOW_WARMBOOT_WRITE(_soc_arad_mem_array_wide_access(unit, flags, mem, array_index, copyno, index, entry_data, 1), rv);
            return rv;
        } else {
            SOC_DPP_ALLOW_WARMBOOT_WRITE(dcmn_mem_array_wide_access(unit, flags, mem, array_index, copyno, index, entry_data, 1), rv);
            return rv;
        }
    }
#endif

    if (mem == FP_GLOBAL_MASK_TCAMm ||  mem == FP_TCAMm ||
        mem == EFP_TCAMm || mem == VFP_TCAMm ||
        mem == FP_GLOBAL_MASK_TCAM_Xm || mem == FP_GLOBAL_MASK_TCAM_Ym ||
        mem == FP_GM_FIELDSm) {
        if (_soc_mem_read_tcam_is_invalid(unit, mem, index) == TRUE) {
            return SOC_E_NONE;
        }
    }

    meminfo = &SOC_MEM_INFO(unit, mem);

    if (copyno == MEM_BLOCK_ANY || copyno == SOC_CORE_ALL) {
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }

    if (!SOC_MEM_BLOCK_VALID(unit, mem, copyno)) {
        LOG_WARN(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "soc_mem_read: invalid block %d for memory %s\n"),
                  copyno, SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    if (_soc_mem_read_tr3_tunnel_is_invalid(unit, mem, index, copyno) == TRUE) {
        return SOC_E_NONE;
    }

    /*
     * When checking index, check for 0 instead of soc_mem_index_min.
     * Diagnostics need to read/write index 0 of Strata ARL and GIRULE.
     */
    if ((flags & SOC_MEM_DONT_MAP_INDEX) &&
        (mem == L3_DEFIPm ||
         mem == L3_DEFIP_ONLYm ||
         mem == L3_DEFIP_DATA_ONLYm ||
         mem == L3_DEFIP_HIT_ONLY_Xm ||
         mem == L3_DEFIP_HIT_ONLY_Ym ||
         mem == L3_DEFIP_HIT_ONLYm ||
         mem == L3_DEFIP_PAIR_128m ||
         mem == L3_DEFIP_PAIR_128_ONLYm ||
         mem == L3_DEFIP_PAIR_128_DATA_ONLYm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Xm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Ym ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLYm)) {
        if (_soc_mem_defip_index_is_invalid(unit, mem, index) == TRUE) {
            return SOC_E_PARAM;
        }
    } else {
        entry_num_max = soc_mem_index_max(unit, mem);

        if (index < 0 || index > entry_num_max) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "soc_mem_read: invalid index %d for memory %s\n"),
                       index, SOC_MEM_NAME(unit, mem)));
            return SOC_E_PARAM;
        }
    }

    if (_soc_mem_read_hercules(unit, mem, copyno, index,
                               entry_data, &rv) == TRUE) {
        return rv;
    }

    rv = SOC_E_NONE;

    MEM_LOCK(unit, mem);
    _soc_mem_read_tr3_esm_lock(unit, copyno);

    if (_soc_mem_read_cache_attempt(unit, flags, mem, copyno, index,
                                    array_index, entry_data,
                                    entry_data_cache,
                                    &cache_consistency_check)) {
        ok = FALSE;
    }
#ifdef CRASH_RECOVERY_SUPPORT
/*     Use crash recovery defined callback for access*/
    if (BCM_UNIT_DO_HW_READ_WRITE(unit))
    {
        if(Hw_Log_List[unit].Access_cb.mem_read)
        {
            if(ok && Hw_Log_List[unit].Access_cb.mem_read(unit, mem,
                                                                       array_index, copyno,
                                                                       index, entry_data)) {
            ok = FALSE;
            }
        }
    }
    /* enable hw reads and writes */
#endif /* CRASH_RECOVERY_SUPPORT */

    if (ok && _soc_mem_read_tr3_read_attempt(unit, mem, index, copyno,
                                             entry_data, &rv) == TRUE) {
        ok = FALSE;
    }

    if (ok && _soc_mem_read_td2_xpipe_tbl_attempt(unit, mem, index, entry_data,
                                                  &rv) == TRUE) {
        ok = FALSE;
    }

    if (ok) {
        index2 = index;
        if (!(flags & SOC_MEM_DONT_MAP_INDEX)) {
            _soc_mem_read_defip_index_map(unit, mem, index, &index2);
        }
        rv2 = _soc_mem_read_schan_msg_send(unit, flags, mem, copyno, index,
                                          entry_data, array_index, index2);
        if (SOC_SUCCESS(rv2)) {
            _soc_mem_read_tcam_to_dm_format(unit, mem, copyno, meminfo, flags,
                                            entry_data);
        }
    }

    /* All done.  Cleanup time. */

    _soc_mem_read_tr3_esm_unlock(unit, copyno);
    MEM_UNLOCK(unit, mem);

    if (cache_consistency_check) {
        _soc_mem_read_cache_check_clear(unit, mem, index, copyno, entry_data,
                                        entry_data_cache);
    }

    if (NULL != meminfo->snoop_cb &&
         (SOC_MEM_SNOOP_READ & meminfo->snoop_flags)) {
         meminfo->snoop_cb(unit, mem, SOC_MEM_SNOOP_READ, copyno, index, index,
                           entry_data, meminfo->snoop_user_data);
    }
    if (bsl_check(bslLayerSoc, bslSourceMem, bslSeverityNormal, unit)) {
        LOG_CLI((BSL_META_U(unit,
                            "soc_mem_read unit %d: %s.%s[%d]: "),
                 unit, SOC_MEM_NAME(unit, mem),
                 SOC_BLOCK_NAME(unit, copyno), index));
        soc_mem_entry_dump(unit, mem, entry_data);
        LOG_CLI((BSL_META_U(unit,
                            "\n")));
    }

    if (ok && (flags & SOC_MEM_SCHAN_ERR_RETURN)) {
        return rv2; /* we executed _soc_mem_read_schan_msg_send and we need to
                       return NACK in case of failure */
    } else {
        if ((SOC_FAILURE(rv2)) && cache_consistency_check) {
            sal_memcpy(entry_data, entry_data_cache,
                       soc_mem_entry_words(unit, mem) * sizeof (uint32));
        }
        /* we did not execute _soc_mem_read_schan_msg_send */
        return rv;
    }
}

/* Wrapper routine with a 'flags' param for better access control */
int
soc_mem_read_extended(int unit, uint32 flags, soc_mem_t mem,
                      /* in memory arrays this is the element index in the array, otherwise 0 */
                      unsigned array_index, int copyno,
                      int index, void *entry_data)
{
    return _soc_mem_read(unit, flags, mem, array_index, copyno, index,
                         entry_data);
}

/*
 * Function:
 *    soc_mem_pipe_select_read
 * Purpose:
 *    Read a memory internal to the SOC.
 * Notes:
 *    GBP/CBP memory should only accessed when MMU is in DEBUG mode.
 */
int
soc_mem_pipe_select_read(int unit,
                         uint32 flags,
                         soc_mem_t mem,
                         int copyno,
                         int acc_type,
                         int index,
                         void *entry_data)
{
    schan_msg_t schan_msg;
    int entry_dw = soc_mem_entry_words(unit, mem);
    soc_mem_info_t *meminfo;
    int index_valid;
    uint8 at;
    int index2;
    uint32 maddr;
    int rv, allow_intr = 0;
    int resp_word = 0;
    int src_blk, dst_blk, data_byte_len;
    int opcode, err;
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_HELIX4_SUPPORT) || \
    defined(BCM_KATANA2_SUPPORT) || defined(BCM_HURRICANE2_SUPPORT) || \
    defined(BCM_GREYHOUND_SUPPORT)
    uint8 *corrupt = NULL;
#endif
    uint32 entry_num_max = 0;

    if (!soc_feature(unit, soc_feature_two_ingress_pipes)) {
        /* Not a relevant device */
        return SOC_E_UNAVAIL;
    }

    if (!soc_mem_is_valid(unit, mem)) {
        return SOC_E_MEMORY;
    }

    meminfo = &SOC_MEM_INFO(unit, mem);

    if (copyno == MEM_BLOCK_ANY) {
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }

    if (!SOC_MEM_BLOCK_VALID(unit, mem, copyno)) {
        LOG_WARN(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "soc_mem_pipe_select_read: invalid block %d for memory %s\n"),
                  copyno, SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    /*
     * When checking index, check for 0 instead of soc_mem_index_min.
     * Diagnostics need to read/write index 0 of Strata ARL and GIRULE.
     */
    if ((flags & SOC_MEM_DONT_MAP_INDEX) &&
        (mem == L3_DEFIPm ||
         mem == L3_DEFIP_ONLYm ||
         mem == L3_DEFIP_DATA_ONLYm ||
         mem == L3_DEFIP_HIT_ONLY_Xm ||
         mem == L3_DEFIP_HIT_ONLY_Ym ||
         mem == L3_DEFIP_HIT_ONLYm ||
         mem == L3_DEFIP_PAIR_128m ||
         mem == L3_DEFIP_PAIR_128_ONLYm ||
         mem == L3_DEFIP_PAIR_128_DATA_ONLYm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Xm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Ym ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLYm)) {
        if (_soc_mem_defip_index_is_invalid(unit, mem, index) == TRUE) {
            return SOC_E_PARAM;
        }
    } else {
        entry_num_max = soc_mem_index_max(unit, mem);

        index_valid = (index >= 0 &&
                   index <= entry_num_max);

        if (!index_valid) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "soc_mem_pipe_select_read: invalid index %d for memory %s acc_type %d\n"),
                       index, SOC_MEM_NAME(unit, mem), acc_type));
            return SOC_E_PARAM;
        }
    }

#ifdef BCM_TRIDENT2PLUS_SUPPORT
    if (SOC_IS_TD2P_TT2P(unit)) {
        if ((FP_TCAMm == mem) || (FP_GM_FIELDSm == mem)
            || (FP_GLOBAL_MASK_TCAMm == mem)
            || (FP_GLOBAL_MASK_TCAM_Xm == mem)
            || (FP_GLOBAL_MASK_TCAM_Ym == mem)) {
            if (_soc_mem_read_tcam_is_invalid(unit, mem, index) == TRUE) {
                return SOC_E_NONE;
            }
        }
    }
#endif

    rv = SOC_E_NONE;

    MEM_LOCK(unit, mem);

    /* Setup S-Channel command packet */
    schan_msg_clear(&schan_msg);

    src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
    data_byte_len = 4;
#ifdef BCM_TRIDENT_SUPPORT
    if (SOC_IS_TD_TT(unit)) {
        if (mem == ESBS_PORT_TO_PIPE_MAPPINGm ||
            mem == ISBS_PORT_TO_PIPE_MAPPINGm) {
            data_byte_len = entry_dw * 4;
        }
    }
#endif /* BCM_TRIDENT_SUPPORT */

    index2 = index;

    
#if defined(BCM_TRIDENT2_SUPPORT)
        if (SOC_IS_TD2_TT2(unit) &&
            soc_feature(unit, soc_feature_l3_defip_map) &&
            (mem == L3_DEFIPm ||
             mem == L3_DEFIP_ONLYm ||
             mem == L3_DEFIP_DATA_ONLYm ||
             mem == L3_DEFIP_HIT_ONLY_Xm ||
             mem == L3_DEFIP_HIT_ONLY_Ym ||
             mem == L3_DEFIP_HIT_ONLYm ||
             mem == L3_DEFIP_PAIR_128m ||
             mem == L3_DEFIP_PAIR_128_ONLYm ||
             mem == L3_DEFIP_PAIR_128_DATA_ONLYm ||
             mem == L3_DEFIP_PAIR_128_HIT_ONLY_Xm ||
             mem == L3_DEFIP_PAIR_128_HIT_ONLY_Ym ||
             mem == L3_DEFIP_PAIR_128_HIT_ONLYm)) {
            if (!(flags & SOC_MEM_DONT_MAP_INDEX)) {
                index2 = soc_trident2_l3_defip_index_map(unit, mem, index);
            }
        }
#endif /* BCM_TRIDENT2_SUPPORT */
    maddr = soc_mem_addr_get(unit, mem, 0, copyno, index2, &at);
#if defined(BCM_EXTND_SBUS_SUPPORT)
    if (!soc_feature(unit, soc_feature_new_sbus_format) &&
        (acc_type != 0)) {
        /* Override ACC_TYPE in address */
        maddr &= ~(_SOC_MEM_ADDR_ACC_TYPE_MASK <<
                        _SOC_MEM_ADDR_ACC_TYPE_SHIFT);
        maddr |= (acc_type & _SOC_MEM_ADDR_ACC_TYPE_MASK) <<
            _SOC_MEM_ADDR_ACC_TYPE_SHIFT;
    }
#endif /* BCM_EXTND_SBUS_SUPPORT */
    schan_msg.readcmd.address = maddr;

    dst_blk = 0;
#if defined(BCM_EXTND_SBUS_SUPPORT)
    if (soc_feature(unit, soc_feature_new_sbus_format)) {
        dst_blk = SOC_BLOCK2SCH(unit, copyno);
    } else
#endif
    {
#if defined(BCM_XGS3_SWITCH_SUPPORT)
        /* required on XGS3. Optional on other devices */
        dst_blk = ((maddr >> SOC_BLOCK_BP) & 0xf) |
            (((maddr >> SOC_BLOCK_MSB_BP) & 0x1) << 4);
#endif /* BCM_XGS3_SWITCH_SUPPORT */
    }

    soc_schan_header_cmd_set(unit, &schan_msg.header, READ_MEMORY_CMD_MSG,
                             dst_blk, src_blk, acc_type, data_byte_len, 0, 0);

    /*
     * Write onto S-Channel "memory read" command packet consisting of header
     * word + address word, and read back header word + entry_dw data words.
     */
    if (2 + entry_dw > CMIC_SCHAN_WORDS(unit)) {
       LOG_WARN(BSL_LS_SOC_SOCMEM,
                (BSL_META_U(unit,
                            "soc_mem_read: assert will fail for memory %s\n"),
                 SOC_MEM_NAME(unit, mem)));
    }
    rv = soc_schan_op(unit, &schan_msg, 2, 1 + entry_dw + resp_word, allow_intr);
    if (SOC_FAILURE(rv)) {
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_HELIX4_SUPPORT) || \
    defined(BCM_KATANA2_SUPPORT) || defined(BCM_HURRICANE2_SUPPORT) || \
    defined(BCM_GREYHOUND_SUPPORT)
        if (SOC_SER_CORRECTION_SUPPORT(unit)) {
            /* Record error index for TCAM table */
            if((SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) &&
               (!(SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_EXT_CAM))) {
                corrupt = SOC_MEM_STATE(unit, mem).corrupt[copyno];
                if (corrupt != NULL) {
                    TCAM_CORRUPT_MAP_SET(corrupt, index);
                    LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
                                (BSL_META_U(unit,
                                            "Mem[%s] index[%d] "
                                            "TCAM parity error indicating bit is set.\n"),
                                 SOC_MEM_NAME(unit, mem), index));
                }
            }
        }
#endif
        goto done;
    }

    soc_schan_header_status_get(unit, &schan_msg.header, &opcode, NULL, NULL,
                                &err, NULL, NULL);
    if (opcode != READ_MEMORY_ACK_MSG || err != 0) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_read: "
                              "invalid S-Channel reply, expected READ_MEMORY_ACK:, opcode %d\n"), opcode));
        soc_schan_dump(unit, &schan_msg, 1 + entry_dw + resp_word);
        rv = SOC_E_INTERNAL;
        goto done;
    }

    sal_memcpy(entry_data,
               resp_word ? schan_msg.genresp.data : schan_msg.readresp.data,
               entry_dw * sizeof (uint32));

#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT)
    if (soc_feature(unit, soc_feature_xy_tcam_direct) &&
        (meminfo->flags & SOC_MEM_FLAG_CAM) &&
        (!(meminfo->flags & SOC_MEM_FLAG_EXT_CAM)) &&
        (!(flags & SOC_MEM_DONT_CONVERT_XY2DM))) {
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
        _soc_mem_fp_global_mask_tcam_shift(unit, mem, entry_data, 0, 0);
#endif
        _soc_mem_tcam_xy_to_dm(unit, mem, 1, entry_data, entry_data);
    }
#endif /* BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT */

done:

    MEM_UNLOCK(unit, mem);


    if (NULL != meminfo->snoop_cb &&
         (SOC_MEM_SNOOP_READ & meminfo->snoop_flags)) {
         meminfo->snoop_cb(unit, mem, SOC_MEM_SNOOP_READ, copyno, index, index,
                           entry_data, meminfo->snoop_user_data);
    }
    if (bsl_check(bslLayerSoc, bslSourceMem, bslSeverityNormal, unit)) {
        LOG_CLI((BSL_META_U(unit,
                            "soc_mem_read unit %d: %s.%s[%d]: "),
                 unit, SOC_MEM_NAME(unit, mem),
                 SOC_BLOCK_NAME(unit, copyno), index));
        soc_mem_entry_dump(unit, mem, entry_data);
        LOG_CLI((BSL_META_U(unit,
                            "\n")));
    }

    return rv;
}

STATIC int
_soc_mem_read_kt2(int unit,
                  soc_mem_t mem,
                  int copyno,
                  int index,
                  void *entry_data)
{
    int rv = SOC_E_FAIL;

#ifdef BCM_KATANA2_SUPPORT
    uint32 rval;
    uint16 dev_id;
    uint8 rev_id;

    soc_cm_get_id(unit, &dev_id, &rev_id);
    if (SOC_IS_KATANA2(unit) &&
        (rev_id == BCM56450_B1_REV_ID) &&
        (SOC_CONTROL(unit)->soc_flags & SOC_F_ALL_MODULES_INITED) &&
        (mem == ING_PHYSICAL_PORT_TABLEm)) {

        READ_ING_Q_BEGINr(unit, &rval);
        soc_reg_field_set(unit, ING_Q_BEGINr, &rval,
                          ING_PHYSICAL_PORT_SBUS_WITH_PKT_DISABLEf, 1);
        WRITE_ING_Q_BEGINr(unit, rval);
        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit, "READ PKT_DISABLE=1\n")));

        rv = soc_mem_array_read(unit, mem, 0, copyno, index, entry_data);
        soc_reg_field_set(unit, ING_Q_BEGINr, &rval,
                          ING_PHYSICAL_PORT_SBUS_WITH_PKT_DISABLEf, 0);
        WRITE_ING_Q_BEGINr(unit, rval);
        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit, "READ PKT_DISABLE=0\n")));
    } else {
        return soc_mem_array_read(unit, mem, 0, copyno, index, entry_data);
    }
#endif

    return rv;
}

int
soc_mem_read(int unit, soc_mem_t mem, int copyno, int index, void *entry_data)
{
    int rv;

    _SOC_MEM_REPLACE_MEM(unit, mem);

    if (SOC_IS_KATANA2(unit)) {
        rv = _soc_mem_read_kt2(unit, mem, copyno, index, entry_data);
    } else {
        rv = soc_mem_array_read(unit, mem, 0, copyno, index, entry_data);
    }

    return rv;
}

int
soc_mem_read_no_cache(int unit,
                               uint32 flags,
                               soc_mem_t mem,
                               unsigned array_index,
                               int copyno,
                               int index,
                               void *entry_data)
{
    int rv;

    _SOC_MEM_REPLACE_MEM(unit, mem);
    flags |= SOC_MEM_DONT_USE_CACHE;

    MEM_LOCK(unit, mem);
    rv = soc_mem_array_read_flags(unit, mem, array_index, copyno,
                                  index, entry_data, flags);
    MEM_UNLOCK(unit, mem);

    return rv;
}

STATIC int
_soc_mem_trident_egr_vlan_read(int unit, soc_mem_t mem, unsigned array_index,
                               int copyno, int index, void *entry_data, int *rv)
{
#ifdef BCM_TRIDENT_SUPPORT
    uint32 entry_data_y[SOC_MAX_MEM_WORDS];
    soc_pbmp_t pbmp_x, pbmp_y;

    if (!SOC_IS_TRIDENT(unit) && !SOC_IS_TITAN(unit)) {
        return FALSE;
    }

    if (mem != EGR_VLANm) {
        return FALSE;
    }

    soc_trident_pipe_select(unit, TRUE, 1); /* Y pipe */
    *rv = _soc_mem_read(unit, 0, mem, array_index, copyno, index, entry_data_y);
    if (SOC_FAILURE(*rv)) {
        return TRUE;
    }

    soc_trident_pipe_select(unit, TRUE, 0); /* X pipe */
    *rv = _soc_mem_read(unit, 0, mem, array_index, copyno, index, entry_data);
    if (SOC_FAILURE(*rv)) {
        return TRUE;
    }

    soc_mem_pbmp_field_get(unit, mem, entry_data, PORT_BITMAPf, &pbmp_x);
    soc_mem_pbmp_field_get(unit, mem, entry_data_y, PORT_BITMAPf, &pbmp_y);
    SOC_PBMP_OR(pbmp_x, pbmp_y);
    soc_mem_pbmp_field_set(unit, mem, entry_data, PORT_BITMAPf, &pbmp_x);
    soc_mem_pbmp_field_get(unit, mem, entry_data, UT_PORT_BITMAPf, &pbmp_x);
    soc_mem_pbmp_field_get(unit, mem, entry_data_y, UT_PORT_BITMAPf, &pbmp_y);
    SOC_PBMP_OR(pbmp_x, pbmp_y);
    soc_mem_pbmp_field_set(unit, mem, entry_data, UT_PORT_BITMAPf, &pbmp_x);

    return TRUE;
#endif /* BCM_TRIDENT_SUPPORT */

    return FALSE;
}

int
soc_mem_array_read_extended(int unit, uint32 flags, 
                                  soc_mem_t mem, unsigned array_index,
                                  int copyno, int index, void *entry_data)
{
    int rv = SOC_E_NONE;

    /* Use user defined callback for access */
    if (SOC_INFO(unit).reg_access.mem_read) {
        return SOC_INFO(unit).reg_access.mem_read(unit, mem, array_index,
                                                  copyno, index, entry_data);
    }

    if (_soc_mem_trident_egr_vlan_read(unit, mem, array_index, copyno, index,
                                       entry_data, &rv) == TRUE) {
        return rv;
    }

    return _soc_mem_read(unit, flags, mem, array_index, copyno, index, entry_data);
}

int
soc_mem_read_physical_index(int unit, uint32 flags, soc_mem_t mem,
                            int copyno, int index, void *entry_data)
{
    int rv;

    _SOC_MEM_REPLACE_MEM(unit, mem);

    if (SOC_IS_KATANA2(unit)) {
        rv = _soc_mem_read_kt2(unit, mem, copyno, index, entry_data);
    } else {
        rv = soc_mem_array_read_extended(unit, flags, mem, 0, copyno,
                                               index, entry_data);
    }

    return rv;
}

/*
 * Function:    soc_mem_array_read
 * Purpose:     Read a memory array internal to the SOC.
 */
int
soc_mem_array_read(int unit, soc_mem_t mem, unsigned array_index,
                   int copyno, int index, void *entry_data)
{
	int rv = SOC_E_NONE;

    /* Use user defined callback for access */
    if (SOC_INFO(unit).reg_access.mem_read) {
        return SOC_INFO(unit).reg_access.mem_read(unit, mem, array_index,
                                                  copyno, index, entry_data);
    }

    if (_soc_mem_trident_egr_vlan_read(unit, mem, array_index, copyno, index,
                                       entry_data, &rv) == TRUE) {
        return rv;
    }

    return _soc_mem_read(unit, 0, mem, array_index, copyno, index, entry_data);
}

/*
 * Function:
 *    soc_mem_read_range
 * Purpose:
 *    Read a range of chip's memory
 */

int
soc_mem_read_range(int unit, soc_mem_t mem, int copyno,
                   int index_min, int index_max, void *buffer)
{
    int rv;

    rv = soc_mem_read_range_multi_cmc(unit, mem, copyno, index_min, index_max, buffer, -1);

    return rv;
}

/*
 * Function:
 *    soc_mem_read_range_multi_cmc
 * Purpose:
 *    Read a range of chip's memory
 *    With multiple CMC/channel support
 * Parameters:
 *    buffer -- Pointer to block of sufficiently large memory.
 * Notes:
 *    Table DMA only works on tables whose entry is less than
 *    SOC_MEM_DMA_MAX_DATA_BEATS words long.
 *
 *    Table DMA has a minimum transaction size of 4 words, so if the
 *    table entry is 1 or 2 words, then the count of words is modified
 *    to keep this alignment.  For the remainder of the entries, this
 *    function reads in the remainder of the data through mem_read
 *    without using DMA.
 */

int
soc_mem_read_range_multi_cmc(int unit, soc_mem_t mem, int copyno,
                             int index_min, int index_max, void *buffer,
                             int vchan)
{
    int rc;

#ifdef SOC_MEM_DEBUG_SPEED
    int count = (index_max > index_min) ?
      (index_max - index_min + 1) :
      (index_min - index_max + 1);

    sal_usecs_t start_time;
    int diff_time,start_time = sal_time_usecs();
#endif
    /* COVERITY: Intentional, stack use of 4064 bytes */
    /* coverity[stack_use_callee_max : FALSE] */
    /* coverity[stack_use_return : FALSE] */

    if (SOC_WARM_BOOT(unit) && SOC_CONTROL(unit)->dma_from_mem_cache) {
        uint32 *cache;
        uint8 *vmap;
        int entry_dw, entry_sz, start, indexes;

        if (copyno == MEM_BLOCK_ANY) {
            copyno = SOC_MEM_BLOCK_ANY(unit, mem);
        }
        cache = SOC_MEM_STATE(unit, mem).cache[copyno];
        vmap = SOC_MEM_STATE(unit, mem).vmap[copyno];
        if (cache != NULL && CACHE_VMAP_TST(vmap, 0) &&
            !_SOC_MEM_CHK_L2_MEM(mem) && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
            entry_dw = soc_mem_entry_words(unit, mem);
            entry_sz = entry_dw * sizeof(uint32);
            start = index_min > index_max ? index_max : index_min;
            indexes = (index_max > index_min) ?
                       (index_max - index_min + 1) : (index_min - index_max + 1);
            sal_memcpy(buffer, cache + (start * entry_dw), indexes * entry_sz);
            LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
                        (BSL_META_U(unit,
                                    "Mem[%s] "
                                    "DMA data from cache.\n"),
                         SOC_MEM_NAME(unit, mem)));
            return SOC_E_NONE;
        }
    }
    rc = soc_mem_array_read_range_multi_cmc(unit, mem, 0, copyno, index_min,
                                            index_max, buffer, vchan);
#ifdef SOC_MEM_DEBUG_SPEED
    diff_time = SAL_USECS_SUB(sal_time_usecs(), start_time);
    LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
                (BSL_META_U(unit,
                            "Total dma read time: %d usecs, [%d nsecs per op]\n"),
                 diff_time, diff_time*1000/count));
#endif
    return rc;
}

/*
 * Function:
 *    soc_mem_array_read_range
 * Purpose:
 *    Read a range of chip's memory
 * Parameters:
 *    buffer -- Pointer to block of sufficiently large memory.
 *                  For Draco DMA operations, this memory must be
 *                  DMA-able
 * Notes:
 *    Table DMA only works on tables whose entry is less than
 *    SOC_MEM_DMA_MAX_DATA_BEATS words long.
 *
 *    Table DMA has a minimum transaction size of 4 words, so if the
 *    table entry is 1 or 2 words, then the count of words is modified
 *    to keep this alignment.  For the remainder of the entries, this
 *    function reads in the remainder of the data through mem_read
 *    without using DMA.
 */

int
soc_mem_array_read_range(int unit, soc_mem_t mem, unsigned array_index,
                         int copyno, int index_min, int index_max, void *buffer)
{
    int rv;

    rv = soc_mem_array_read_range_multi_cmc(unit, mem, array_index,
                                            copyno, index_min, index_max, buffer, -1);

    return rv;
}

#ifdef BROADCOM_DEBUG

static int soc_dma_debug_property_get(int unit)
{
    if (0 == _soc_dma_debug_op[unit]) {
        _soc_dma_debug_enable[unit] =
            soc_property_get(unit, "dma_mem_debug_enable", 0);
        _soc_dma_debug_op[unit] = 1;
    }
    return _soc_dma_debug_enable[unit];
}

#endif

/*
 * Function:
 *    soc_mem_array_read_range_multi_cmc
 * Purpose:
 *    Read a range of chip's memory
 *    With multiple CMC/channel support
 */

int
soc_mem_array_read_range_multi_cmc(int unit, soc_mem_t mem, unsigned array_index,
                                   int copyno, int index_min, int index_max, void *buffer, int vchan)
{
    int index;
    int count;
    void *buf;
#ifdef BROADCOM_DEBUG
    shared_block_t *p = NULL;
    int            length = 0;
#endif
#ifdef BCM_PETRA_SUPPORT
    int       rv;
    int       entry_dw;
    entry_dw = soc_mem_entry_words(unit, mem);
#endif
    if (!soc_mem_is_valid(unit, mem)) {
        return SOC_E_MEMORY;
    }

    if (copyno == MEM_BLOCK_ANY) {
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }
    if (copyno == COPYNO_ALL) {
        return SOC_E_INTERNAL;
    }

    assert(SOC_MEM_BLOCK_VALID(unit, mem, copyno));
    assert(soc_mem_index_valid(unit, mem, index_min));
    assert(soc_mem_index_valid(unit, mem, index_max));
    assert(index_min <= index_max);
    assert(buffer != NULL);
#ifdef BROADCOM_DEBUG
    if (soc_dma_debug_property_get(unit)) {
        p = (shared_block_t *) (((char*)buffer) -
            ( (((char*)&(((shared_block_t*)0)->user_data[0]))) - ((char*)(shared_block_t*)0) ));
        if (soc_cm_shared_good_range(unit, p)) {
            if (SHARED_GOOD_START(p)  && SHARED_GOOD_END_DEBUG(p)) {
                length = WORDS2BYTES(soc_mem_entry_words(unit, mem)) *
                         (index_max - index_min + 1);
                if (length > p->size) {
                    LOG_WARN(BSL_LS_SOC_SOCMEM,
                             (BSL_META_U(unit,
                                         "Suspicious DMA length: "
                                         "Desc:%s: Size:%u: length:%u\n"),
                              p->description, p->size, length));
                }
            }
        } else if (soc_mem_dmaable(unit, mem, copyno)) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "ERROR:ATTN: Address:%p:"
                                  "probably not in shared memory region \n"),
                       p));
        }
    }
#endif

    count = 0;

    /* coverity[var_tested_neg : FALSE] */
    LOG_INFO(BSL_LS_SOC_SOCMEM,
             (BSL_META_U(unit,
                         "soc_mem_array_read_range: unit %d memory %s.%s [%d:%d]\n"),
              unit, SOC_MEM_UFNAME(unit, mem),
              SOC_BLOCK_NAME(unit, copyno),
              index_min, index_max));

    /* If device is gone fill buffer with null entries. */
    if (SOC_IS_DETACHING(unit)) {
        buf = buffer;
        for (index = index_min; index <= index_max; index++, count++) {
            buf = soc_mem_table_idx_to_pointer(unit, mem, void *,
                                               buffer, count);
            sal_memcpy(buf, soc_mem_entry_null(unit, mem),
                       soc_mem_entry_bytes(unit, mem));
        }
        return (SOC_E_NONE);
    }

#ifdef BCM_PETRA_SUPPORT
    /*
     * Write onto S-Channel "memory read" command packet consisting of header
     * word + address word, and read back header word + entry_dw data words.
     */
    if (SOC_IS_DPP(unit) && (entry_dw + 2 > CMIC_SCHAN_WORDS(unit))) {
        buf = buffer;
        for (index = index_min; index <= index_max; index++, count++) {
            buf = soc_mem_table_idx_to_pointer(unit, mem, void *,
                                               buffer, count);
            SOC_DPP_ALLOW_WARMBOOT_WRITE(dcmn_mem_array_wide_access(unit, SOC_MEM_NO_FLAGS, mem, array_index, MEM_BLOCK_ALL, index, buf, 1), rv);
            if (SOC_FAILURE(rv)) {
                LOG_ERROR(BSL_LS_SOC_COMMON,
                                (BSL_META_U(unit,
                                            "Read mem %s[%d][%d] error.\n"),
                                 SOC_MEM_NAME(unit, mem), array_index, index));
            }
        }
        return (SOC_E_NONE);
    }
#endif

#if defined(BCM_XGS_SWITCH_SUPPORT) || defined(BCM_SIRIUS_SUPPORT) ||\
    defined(BCM_CALADAN3_SUPPORT) || defined(BCM_88750_SUPPORT) ||\
    defined(BCM_PETRA_SUPPORT)
    /* coverity[negative_returns : FALSE] */
    if (soc_mem_dmaable(unit, mem, copyno)) {
#if defined(BCM_SIRIUS_SUPPORT) || defined (BCM_XGS3_SWITCH_SUPPORT) ||\
    defined(BCM_CALADAN3_SUPPORT) || defined(BCM_88750_SUPPORT) ||\
    defined(BCM_SAND_SUPPORT)

        if (SOC_IS_SIRIUS(unit) || SOC_IS_XGS3_SWITCH(unit) ||
            SOC_IS_CALADAN3(unit) || SOC_IS_SAND(unit)) {
            int rv;
            soc_mem_info_t  *meminfo;

            meminfo = &SOC_MEM_INFO(unit, mem);
            rv = _soc_mem_dma_read(unit, mem, array_index, copyno, index_min,
                                   index_max, 0, buffer, vchan);

            if (SOC_FAILURE(rv)) {
                if (rv == SOC_E_FAIL && SOC_SER_CORRECTION_SUPPORT(unit)) {
                    LOG_VERBOSE(BSL_LS_SOC_COMMON,
                                (BSL_META_U(unit,
                                            "Mem[%s] "
                                            "DMA fallback to pio.\n"),
                                 SOC_MEM_NAME(unit, mem)));
                    goto _dma_fall_back_pio;
                }
                return rv;
            }
            if (NULL != meminfo->snoop_cb &&
                (SOC_MEM_SNOOP_READ & meminfo->snoop_flags)) {
                meminfo->snoop_cb(unit, mem, SOC_MEM_SNOOP_READ, copyno,
                                  index_min, index_max, buffer,
                                  meminfo->snoop_user_data);
            }
            return SOC_E_NONE;
        }
#endif /* BCM_XGS3_SWITCH_SUPPORT BCM_SIRIUS_SUPPORT BCM_CALADAN3_SUPPORT ||
          defined(BCM_88750_SUPPORT) */
    }
_dma_fall_back_pio:
#endif /* (BCM_XGS_SWITCH_SUPPORT) || defined(BCM_SIRIUS_SUPPORT) ||
          defined(BCM_CALADAN3_SUPPORT) || defined(BCM_88750_SUPPORT) ||
          defined(BCM_PETRA_SUPPORT) */
    /* For the rest of entries, do direct read */
    for (index = index_min; index <= index_max; index++, count++) {
        buf = soc_mem_table_idx_to_pointer(unit, mem, void *,
                                           buffer, count);
        SOC_IF_ERROR_RETURN(soc_mem_array_read(unit, mem, array_index, copyno,
                                               index, buf));
    }

    return SOC_E_NONE;
}

int soc_mem_array_read_flags(int unit, soc_mem_t mem, unsigned array_index,
                             int copyno, int index, void *entry_data, int flags)
{
    int rv, orig_flag;

    if (!soc_mem_is_valid(unit, mem)) {
        return SOC_E_MEMORY;
    }

    orig_flag = SOC_MEM_FORCE_READ_THROUGH(unit);

    if(flags & SOC_MEM_DONT_USE_CACHE) {
        SOC_MEM_FORCE_READ_THROUGH_SET(unit, 1);
    }

    rv = soc_mem_array_read_extended(unit, flags, mem, array_index,
                                           copyno, index, entry_data);

    SOC_MEM_FORCE_READ_THROUGH_SET(unit, orig_flag);

    return rv;
}

/*
 * Function:
 *    soc_mem_ser_read_range
 * Purpose:
 *    Read a range of chip's memory for SER detection
 * Parameters:
 *    buffer -- Pointer to block of sufficiently large memory.
 * Notes:
 *    Table DMA only works on tables whose entry is less than
 *    SOC_MEM_DMA_MAX_DATA_BEATS words long.
 *
 *    Table DMA has a minimum transaction size of 4 words, so if the
 *    table entry is 1 or 2 words, then the count of words is modified
 *    to keep this alignment.  For the remainder of the entries, this
 *    function reads in the remainder of the data through mem_read
 *    without using DMA.
 */
int
soc_mem_ser_read_range(int unit, soc_mem_t mem, int copyno,
                       int index_min, int index_max, uint32 ser_flags,
                       void *buffer)
{
    int rv, index, pipe_acc, count;
    void *buf;

    if (!soc_mem_is_valid(unit, mem)) {
        return SOC_E_MEMORY;
    }

    if (copyno == MEM_BLOCK_ANY) {
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }
    if (copyno == COPYNO_ALL) {
        return SOC_E_INTERNAL;
    }

    /* Skip assertions here because they should have been checked
     * during mem scan init. */
    assert(buffer != NULL);

    if (0 == (ser_flags & _SOC_SER_FLAG_NO_DMA)) {
        rv = _soc_mem_dma_read(unit, mem, 0, copyno, index_min,
                               index_max, ser_flags, buffer, -1);
    } else {
        /* Trigger the slow path iteration below */
        rv = SOC_E_FAIL;
    }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
    if (SOC_E_NONE == rv) {
        _soc_mem_fp_global_mask_tcam_shift(unit, mem, buffer, 0, 
                                           index_max - index_min);
    }
#endif
    
    if (SOC_E_FAIL == rv) {
        if (ser_flags & _SOC_SER_FLAG_DMA_ERR_RETURN) {
            return rv;
        }
        /* Scan the table by individual reads to trigger SER correction */
        pipe_acc = (ser_flags & _SOC_SER_FLAG_ACC_TYPE_MASK);
        count = 0;
        for (index = index_min; index <= index_max; index++, count++) {
            buf = soc_mem_table_idx_to_pointer(unit, mem, void *,
                                               buffer, count);
            if (0 != (ser_flags & _SOC_SER_FLAG_MULTI_PIPE)) {
                /* For multiple-pipe devices, soc_mem_pipe_select_read
                 * expects a non-zero acc_type parameter.
                 * If _SOC_MEM_ADDR_ACC_TYPE_PIPE_Y is not configured,
                 * A default acc_type should be specified.
                 */
                if ((pipe_acc == 0) &&
                    !(soc_feature(unit,
                                  soc_feature_unique_acc_type_access))) {

                    pipe_acc = SOC_MEM_ACC_TYPE(unit, mem);
                }
                if (0 != (ser_flags & _SOC_SER_FLAG_XY_READ)) {
                    rv = soc_mem_pipe_select_read(unit, SOC_MEM_DONT_CONVERT_XY2DM,
                                                  mem, copyno, pipe_acc, index, buf);
                } else {
                    rv = soc_mem_pipe_select_read(unit, SOC_MEM_NO_FLAGS,
                                                  mem, copyno, pipe_acc, index, buf);
                }

            } else {
#ifdef BCM_TRIDENT_SUPPORT
                if ((SOC_IS_TRIDENT(unit) || SOC_IS_TITAN(unit)) &&
                      (mem == EGR_VLANm)) {
                    rv = soc_mem_array_read(unit, mem, 0, copyno, index, buf);
                } else
#endif
                {
                    if (0 != (ser_flags & _SOC_SER_FLAG_XY_READ)) {
                        rv = soc_mem_read_extended(unit,
                                SOC_MEM_DONT_CONVERT_XY2DM | SOC_MEM_DONT_USE_CACHE,
                                mem, 0, copyno, index, buf);
                    } else {
                        rv = soc_mem_read_extended(unit, SOC_MEM_DONT_USE_CACHE,
                                                   mem, 0, copyno, index, buf);
                    }
                }
            }
            if (SOC_E_FAIL == rv) {
                /* We're expecting this to fail, that triggers the
                 * correction logic. */
                rv = SOC_E_NONE;
            } else if (SOC_FAILURE(rv)) {
                return rv;
            }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
        _soc_mem_fp_global_mask_tcam_shift(unit, mem, buf, 0, 0);
#endif
        }
        rv = SOC_E_NONE;
    }

    return rv;
}

#ifdef BCM_ESW_SUPPORT
/*
 * Function:
 *    soc_mem_is_dynamic
 * Purpose:
 *    Determine if a memory contains fields that will be updated by hardware.
 * Parameters:
 * Notes:
 *    N/A
 */
int
soc_mem_is_dynamic(int unit, soc_mem_t mem)
{
    if (SOC_MEM_FIELD_VALID(unit, mem, PARITYf)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PARITYf)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PARITY_0f)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PARITY_1f)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PARITY_2f)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PARITY_3f)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PARITY_Af)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PARITY_Bf)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PARITY_LOWERf)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PARITY_UPPERf)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PARITY_P0f)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PARITY_P1f)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PARITY_P2f)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PARITY_P3f)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PBM_PARITY_P0f)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PBM_PARITY_P1f)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PBM_PARITY_P2f)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_PBM_PARITY_P3f)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_EGR_VLAN_STG_PARITY_P0f)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_EGR_VLAN_STG_PARITY_P1f)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_EGR_VLAN_STG_PARITY_P2f)||
        SOC_MEM_FIELD_VALID(unit, mem, EVEN_EGR_VLAN_STG_PARITY_P3f)||
        SOC_MEM_FIELD_VALID(unit, mem, ECCf)||
        SOC_MEM_FIELD_VALID(unit, mem, ECC_0f)||
        SOC_MEM_FIELD_VALID(unit, mem, ECC_1f)||
        SOC_MEM_FIELD_VALID(unit, mem, ECC_2f)||
        SOC_MEM_FIELD_VALID(unit, mem, ECC_3f)||
        SOC_MEM_FIELD_VALID(unit, mem, ECC_4f)||
        SOC_MEM_FIELD_VALID(unit, mem, ECCPf)||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP0f)||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP1f)||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP2f)||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP3f)||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP4f)||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP5f)||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP7f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP_0f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP_1f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP_2f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP_3f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP_4f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECC0f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECC1f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECC2f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECC3f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY0f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY1f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY2f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY3f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY_0f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY_1f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY_2f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY_3f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY_4f) ||
        SOC_MEM_FIELD_VALID(unit, mem, HITf) ||
        SOC_MEM_FIELD_VALID(unit, mem, HIT0f) ||
        SOC_MEM_FIELD_VALID(unit, mem, HIT1f) ||
        SOC_MEM_FIELD_VALID(unit, mem, HIT_0f) ||
        SOC_MEM_FIELD_VALID(unit, mem, HIT_1f) ||
        SOC_MEM_FIELD_VALID(unit, mem, HIT_2f) ||
        SOC_MEM_FIELD_VALID(unit, mem, HIT_3f) ||
        SOC_MEM_FIELD_VALID(unit, mem, HITDA_0f) ||
        SOC_MEM_FIELD_VALID(unit, mem, HITDA_1f) ||
        SOC_MEM_FIELD_VALID(unit, mem, HITDA_2f) ||
        SOC_MEM_FIELD_VALID(unit, mem, HITDA_3f) ||
        SOC_MEM_FIELD_VALID(unit, mem, HITSA_0f) ||
        SOC_MEM_FIELD_VALID(unit, mem, HITSA_1f) ||
        SOC_MEM_FIELD_VALID(unit, mem, HITSA_2f) ||
        SOC_MEM_FIELD_VALID(unit, mem, HITSA_3f) ||
        SOC_MEM_FIELD_VALID(unit, mem, HITSAf) ||
        SOC_MEM_FIELD_VALID(unit, mem, HITDAf) ||
        SOC_MEM_FIELD_VALID(unit, mem, B0_HITf) ||
        SOC_MEM_FIELD_VALID(unit, mem, B1_HITf) ||
        SOC_MEM_FIELD_VALID(unit, mem, L3_HIT_DCMf) ||
        SOC_MEM_FIELD_VALID(unit, mem, L3_HIT_PMf) ||
        SOC_MEM_FIELD_VALID(unit, mem, HIT_BITSf) ||
        SOC_MEM_FIELD_VALID(unit, mem, BUCKETCOUNTf) ||
        SOC_MEM_FIELD_VALID(unit, mem, REFRESHCOUNTf) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP_P0f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP_P1f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP_P2f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP_P3f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECC_P0f)||
        SOC_MEM_FIELD_VALID(unit, mem, ECC_P1f)||
        SOC_MEM_FIELD_VALID(unit, mem, ECC_P2f)||
        SOC_MEM_FIELD_VALID(unit, mem, ECC_P3f)||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY_P0f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY_P1f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY_P2f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY_P3f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP_PBM_0f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP_PBM_1f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP_PBM_2f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECCP_PBM_3f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECC_PBM_0f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECC_PBM_1f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECC_PBM_2f) ||
        SOC_MEM_FIELD_VALID(unit, mem, ECC_PBM_3f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY_PBM_0f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY_PBM_1f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY_PBM_2f) ||
        SOC_MEM_FIELD_VALID(unit, mem, PARITY_PBM_3f)) {
        return TRUE;
    }
    return FALSE;
}
#endif

#if defined(BCM_XGS3_SWITCH_SUPPORT) || defined(BCM_SIRIUS_SUPPORT) || defined(BCM_CALADAN3_SUPPORT)
#ifdef BCM_WARM_BOOT_SUPPORT
/*
 * Function:
 *    soc_scache_mem_read_range
 * Purpose:
 *    Read a range of chip's memory used for L2 Warm Boot
 * Parameters:
 *    buffer -- Pointer to block of sufficiently large memory.
 *                  For DMA operations, this memory must be
 *                  DMA-able
 * Notes:
 *    This function is only used for DMA'ing the scache from the
 *      chip's internal memory.
 */

STATIC int
soc_scache_mem_read_range(int unit, soc_mem_t mem, int copyno,
                          int index_min, int index_max, void *buffer)
{
    int index;
    int count;
    void *buf;
    soc_mem_info_t  *meminfo;
    unsigned int array_index = 0;

    if (!soc_mem_is_valid(unit, mem)) {
        return SOC_E_MEMORY;
    }

    if (copyno == MEM_BLOCK_ANY) {
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }
    if (copyno == COPYNO_ALL) {
        return SOC_E_INTERNAL;
    }

    assert(SOC_MEM_BLOCK_VALID(unit, mem, copyno));
    assert(buffer != NULL);

    meminfo = &SOC_MEM_INFO(unit, mem);
    count = 0;

    /* coverity[var_tested_neg : FALSE] */
    LOG_INFO(BSL_LS_SOC_SOCMEM,
             (BSL_META_U(unit,
                         "soc_scache_mem_read_range: unit %d memory %s.%s [%d:%d]\n"),
              unit, SOC_MEM_UFNAME(unit, mem),
              SOC_BLOCK_NAME(unit, copyno),
              index_min, index_max));

    /* If device is gone fill buffer with null entries. */
    if (SOC_IS_DETACHING(unit)) {
        buf = buffer;
        for (index = index_min; index <= index_max; index++, count++) {
            buf = soc_mem_table_idx_to_pointer(unit, mem, void *,
                                               buffer, count);
            sal_memcpy(buf, soc_mem_entry_null(unit, mem),
                       soc_mem_entry_bytes(unit, mem));
        }
        return (SOC_E_NONE);
    }

    /* coverity[negative_returns : FALSE] */
    if (soc_mem_dmaable(unit, mem, copyno)) {
        if (SOC_IS_XGS3_SWITCH(unit)) {

            SOC_IF_ERROR_RETURN
                (_soc_mem_dma_read(unit, mem, array_index, copyno,
                                   index_min, index_max, 0, buffer, -1));

            if (NULL != meminfo->snoop_cb &&
                (SOC_MEM_SNOOP_READ & meminfo->snoop_flags)) {
                meminfo->snoop_cb(unit, mem, SOC_MEM_SNOOP_READ, copyno,
                                  index_min, index_max, buffer,
                                  meminfo->snoop_user_data);
            }
        }
    }
    return SOC_E_NONE;
}

/*
 * Function:
 *    soc_scache_mem_write_range
 * Purpose:
 *    Write a range of chip's memory used for L2 Warm Boot
 * Parameters:
 *    buffer -- Pointer to block of sufficiently large memory.
 *                For slam operations, this memory must be
 *                slammable
 * Notes:
 *    This function is only used for slamming the scache to the
 *      chip's internal memory.
 */
STATIC int
soc_scache_mem_write_range(int unit, soc_mem_t mem, int copyno,
                           int index_min, int index_max, void *buffer)
{
    uint32          entry_dw;
    int             i, rv = SOC_E_NONE;
    soc_mem_info_t  *meminfo;
    void            *cache_buffer = NULL;
#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) \
    || defined(BCM_HURRICANE2_SUPPORT)
    int             count, alloc_size;
#endif
    uint32          *cache;

    if (!soc_mem_is_valid(unit, mem)) {
        return SOC_E_MEMORY;
    }

    meminfo = &SOC_MEM_INFO(unit, mem);
    entry_dw = soc_mem_entry_words(unit, mem);

    LOG_INFO(BSL_LS_SOC_SOCMEM,
             (BSL_META_U(unit,
                         "soc_scache_mem_write_range: unit %d memory %s.%s [%d:%d]\n"),
              unit, SOC_MEM_UFNAME(unit, mem),
              SOC_BLOCK_NAME(unit, copyno),
              index_min, index_max));

    /*    coverity[negative_returns : FALSE]    */
    if (SOC_IS_XGS3_SWITCH(unit) && soc_mem_slamable(unit, mem, copyno)) {
        int blk;

        if (copyno == COPYNO_ALL) {
            SOC_MEM_BLOCK_ITER(unit, mem, blk) {
                copyno = blk;
                break;
            }
        }
        if (copyno == COPYNO_ALL) {
            return SOC_E_INTERNAL;
        }
        cache = SOC_MEM_STATE(unit, mem).cache[copyno];
#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) \
    || defined(BCM_HURRICANE2_SUPPORT)
        if (cache != NULL && !SOC_MEM_TEST_SKIP_CACHE(unit) &&
            (soc_feature(unit, soc_feature_xy_tcam) &&
            (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) &&
            (!(SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_EXT_CAM)))) {
            if (index_min > index_max) {
                count = index_min - index_max + 1;
            } else {
                count = index_max - index_min + 1;
            }
            alloc_size = count * entry_dw * sizeof(uint32);
            cache_buffer = sal_alloc(alloc_size, "cache buffer");
            if (cache_buffer == NULL) {
                return SOC_E_MEMORY;
            }
        }
#endif
        MEM_LOCK(unit, mem);
        rv = _soc_mem_dma_write(unit, 0, mem, 0, copyno, index_min, index_max,
                                buffer, cache_buffer, -1);
        if (rv >= 0) {
            uint8 *vmap, *vmap1 = NULL;
               /* coverity[negative_returns : FALSE]    */
               /* coverity[negative_returns] */
            vmap = SOC_MEM_STATE(unit, mem).vmap[copyno];
            if (_SOC_MEM_CHK_L2_MEM(mem)) {
                if (mem == L2_ENTRY_1m) {
                    vmap1 = SOC_MEM_STATE(unit, L2_ENTRY_2m).vmap[copyno];
                } else if (mem == L2_ENTRY_2m) {
                    vmap1 = SOC_MEM_STATE(unit, L2_ENTRY_1m).vmap[copyno];
                }
            }

            if (cache != NULL && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
                sal_memcpy(cache + index_min * entry_dw, cache_buffer ?
                           cache_buffer : buffer,
                           (index_max - index_min + 1) * entry_dw * 4);
                for (i = index_min; i <= index_max; i++) {
                    if (vmap1) {
                        CACHE_VMAP_CLR(vmap, i);
                        if (mem == L2_ENTRY_1m) {
                            CACHE_VMAP_CLR(vmap1, i/2);
                        } else {
                            CACHE_VMAP_CLR(vmap1, i*2);
                            CACHE_VMAP_CLR(vmap1, i*2 + 1);
                        }
                    } else {
                        CACHE_VMAP_SET(vmap, i);
                    }
                }
#ifdef BCM_TRX_SUPPORT
                _soc_mem_aggr_cache_update(unit, mem, copyno, 0,
                    index_min, index_max, 0, buffer);
#endif /* BCM_TRX_SUPPORT */                
            }
        }
        MEM_UNLOCK(unit, mem);
        if (NULL != meminfo->snoop_cb &&
            (SOC_MEM_SNOOP_WRITE & meminfo->snoop_flags)) {
            meminfo->snoop_cb(unit, mem, SOC_MEM_SNOOP_WRITE, copyno, index_min,
                              index_max, buffer, meminfo->snoop_user_data);
        }
        if (cache_buffer) {
            sal_free(cache_buffer);
        }
    }
    return rv;
}

int
soc_scache_esw_nh_mem_write(int unit, uint8 *buf, int offset, int nbytes)
{
    int num_entries, bytes2,  rv = SOC_E_NONE;
    int stable_size;
    uint32 stable_index_max, stable_index_min;
    int i, j, bufptr;
    uint8 *buf2;
    int start, end;
    int scache_offset;
    uint8 mask[SOC_MAX_MEM_BYTES];
    uint32 l3_next_hop_size = soc_mem_entry_bytes(unit, EGR_L3_NEXT_HOPm) *
        soc_mem_index_count(unit, EGR_L3_NEXT_HOPm);

    SOC_IF_ERROR_RETURN(
        soc_stable_tmp_access(unit, sf_index_min, &stable_index_min, TRUE));
    SOC_IF_ERROR_RETURN(
        soc_stable_tmp_access(unit, sf_index_max, &stable_index_max, TRUE));
    SOC_IF_ERROR_RETURN(
        soc_stable_size_get(unit, &stable_size));

    bytes2 = soc_mem_entry_words(unit, EGR_L3_NEXT_HOPm) * sizeof(uint32);
    num_entries = stable_index_max - stable_index_min + 1;

    start = stable_index_min + offset / (bytes2);

    scache_offset = offset % (bytes2);
    end = start + (nbytes + scache_offset + bytes2 - 1) / (bytes2);
    if (stable_index_max < end) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "Stable max index(%d) is less than buffer end(%d)\n"),
                   stable_index_max, end));   
        return SOC_E_INTERNAL;
    }
    if (stable_size > l3_next_hop_size) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "Stable size(%d) is larger than size of memory EGR_L3_NEXT_HOPm(%d)\n"),
                   stable_size, l3_next_hop_size)); 
        return SOC_E_INTERNAL;
    }
    buf2 = NULL;
    /* Allocate slammable memory */
    buf2 = soc_cm_salloc(unit, bytes2 * num_entries, "EGR_L3_NEXT_HOP buffer");
    if (NULL == buf2) {
        rv = SOC_E_MEMORY;
        goto cleanup;
    }
    sal_memset(buf2, 0, bytes2 * num_entries);

    /* Copy the buffer to the individual memories */
    bufptr = 0;
    sal_memset(mask, 0, SOC_MAX_MEM_BYTES);
    soc_mem_datamask_get(unit, EGR_L3_NEXT_HOPm, (uint32 *)mask);

    SOC_IF_ERROR_RETURN
        (soc_scache_mem_read_range(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY,
                                   start, start, buf2));
    if (start != end) {
        SOC_IF_ERROR_RETURN
            (soc_scache_mem_read_range(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY,
                                       end, end, buf2 + end * bytes2));
    }

    for (i = 0; i <= (end - start); i++) {
        if (bufptr < stable_size) {
            for (j = (i == 0) ? scache_offset : 0; j < SOC_MAX_MEM_BYTES; j++) {
                if (mask[j] != 0xff) {
                    continue;
                }
                /*
                sal_memcpy(buf2 + (i - stable_index_min) * bytes2 + j,
                           (buf + bufptr), 1);
                */
                *(buf2 + i * bytes2 + j) = *(buf + bufptr);
                bufptr++;
            }
        }
    }

    rv = soc_scache_mem_write_range(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ALL,
                                    start, end, buf2);
cleanup:
    if (buf2 != NULL) {
        soc_cm_sfree(unit, buf2);
    }
    return rv;
}

int
soc_scache_esw_nh_mem_read(int unit, uint8 *buf, int offset, int nbytes)
{
    int i, num_entries, bytes2, rv = SOC_E_NONE;
    int j, bufptr;
    int stable_size;
    uint32 stable_index_min, stable_index_max;
    uint8 *buf2;
    int start, end;
    int scache_offset;
    uint8 mask[SOC_MAX_MEM_BYTES];
    uint32 l3_next_hop_size = soc_mem_entry_bytes(unit, EGR_L3_NEXT_HOPm) *
        soc_mem_index_count(unit, EGR_L3_NEXT_HOPm);

    SOC_IF_ERROR_RETURN(
        soc_stable_tmp_access(unit, sf_index_min, &stable_index_min, TRUE));
    SOC_IF_ERROR_RETURN(
        soc_stable_tmp_access(unit, sf_index_max, &stable_index_max, TRUE));
    SOC_IF_ERROR_RETURN(
        soc_stable_size_get(unit, &stable_size));

    bytes2 = soc_mem_entry_words(unit, EGR_L3_NEXT_HOPm) * sizeof(uint32);
    num_entries = stable_index_max - stable_index_min + 1;

    buf2 = NULL;
    start = stable_index_min + offset / (bytes2);

    scache_offset = offset % (bytes2);
    end = start + (nbytes + scache_offset + bytes2 - 1) / (bytes2);
    if (stable_index_max < end) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "Stable max index(%d) is less than buffer end(%d)\n"),
                   stable_index_max, end));   
        return SOC_E_INTERNAL;
    }
    if (stable_size > l3_next_hop_size) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "Stable size(%d) is larger than size of memory EGR_L3_NEXT_HOPm(%d)\n"),
                   stable_size, l3_next_hop_size)); 
        return SOC_E_INTERNAL;
    }
    /* Allocate DMA'able memory */
    buf2 = soc_cm_salloc(unit, bytes2 * num_entries, "EGR_L3_NEXT_HOP buffer");
    if (NULL == buf2) {
        rv = SOC_E_MEMORY;
        goto cleanup;
    }
    sal_memset(buf2, 0, bytes2 * num_entries);

    /* DMA the tables */
    rv = soc_scache_mem_read_range(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY,
                                   stable_index_min, stable_index_max, buf2);

    /* Copy out the individual buffers to the scache buffer */
    bufptr = 0;
    sal_memset(mask, 0, SOC_MAX_MEM_BYTES);
    soc_mem_datamask_get(unit, EGR_L3_NEXT_HOPm, (uint32 *)mask);
    for (i = 0; i <= (end - start); i++) {
        if (bufptr < stable_size) {
            for (j = (i == 0) ? scache_offset : 0; j < SOC_MAX_MEM_BYTES; j++) {
                if (mask[j] != 0xff) {
                    continue;
                }
                /*
                sal_memcpy((buf + offset + bufptr),
                            buf2 + (i - stable_index_min) * bytes2 + j, 1);
                */
                *(buf + bufptr) = *(buf2 + i * bytes2 + j);
                bufptr++;
            }
        }
    }
cleanup:
    if (buf2 != NULL) {
        soc_cm_sfree(unit, buf2);
    }
    return rv;
}
#endif /* BCM_WARM_BOOT_SUPPORT */
#endif /* defined(BCM_XGS3_SWITCH_SUPPORT) || defined(BCM_SIRIUS_SUPPORT) */

/*
 * Function: _soc_mem_write_ext_cache_set
 *
 * Purpose:  Called by _soc_mem_write_extended().  Check for the no-cache flag
 *           and set no_cache and index accordingly.
 *
 * Returns:  Nothing, but sets no_cache and may alter index
 */
STATIC void
_soc_mem_write_ext_cache_set(uint32 flags, int *index, int *no_cache)
{
    if ((flags & SOC_MEM_DONT_USE_CACHE) == SOC_MEM_DONT_USE_CACHE) {
        *no_cache = 1;
    } else {
        *no_cache = 0;
    }

    /*Legacy  nocache handling*/
    if (*index < 0) {
        *no_cache = 1;
        *index = -*index;
    }
}

/*
 * Function: _soc_mem_hercules_write_attempt
 *
 * Purpose: Called by _soc_mem_write_extended().  If the unit is a hercules,
 *          redirect the request to the hercules mem write handler.
 *
 * Returns: TRUE if the request was intercepted and handled by hercules
 *          FALSE if the request should proceed normally
 *
 */
STATIC int
_soc_mem_hercules_write_attempt(int unit, soc_mem_t mem, int copyno, int index,
                                void *entry_data, int *rv)
{
#ifdef BCM_HERCULES_SUPPORT
    /* Handle Hercules' word read tables separately */
    if (SOC_IS_HERCULES(unit) &&
        (soc_mem_flags(unit, mem) & SOC_MEM_FLAG_WORDADR)) {
        *rv = soc_hercules_mem_write(unit, mem, copyno, index, entry_data);
        return TRUE;
    }
#endif /* BCM_HERCULES_SUPPORT */

    return FALSE;
}

STATIC void *
_soc_mem_write_tcam_to_hw_format(int unit, soc_mem_t mem, void *entry_data,
                                 uint32 *cache_entry_data,
                                 uint32 *converted_entry_data)
{
    void *entry_data_ptr = entry_data;

#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) \
    || defined(BCM_HURRICANE2_SUPPORT)

    soc_mem_info_t *meminfo = &SOC_MEM_INFO(unit, mem);
    if (soc_feature(unit, soc_feature_xy_tcam) &&
        (meminfo->flags & SOC_MEM_FLAG_CAM) &&
        (!(meminfo->flags & SOC_MEM_FLAG_EXT_CAM))) {

        entry_data_ptr = converted_entry_data;
        _soc_mem_tcam_dm_to_xy(unit, mem, 1, entry_data, entry_data_ptr,
                               cache_entry_data);
    }
#endif /* BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT */

    return entry_data_ptr;
}

/*
 * Function: _soc_mem_write_sanity_check
 *
 * Purpose:  Called by _soc_mem_write_extended().  Check mem/index parameters
 *           for basic validity.
 *
 * Returns:  SOC_E_xxx
 */
STATIC int
_soc_mem_write_sanity_check(int unit, uint32 flags, soc_mem_t mem,
                                      int index)
{
    soc_mem_info_t *meminfo = &SOC_MEM_INFO(unit, mem);
    uint32 entry_num_max = 0;

    if (!soc_mem_is_valid(unit, mem)) {
        return SOC_E_MEMORY;
    }

    /*
     * When checking index, check for 0 instead of soc_mem_index_min.
     * Diagnostics need to read/write index 0 of Strata ARL and GIRULE.
     */
    if ((flags & SOC_MEM_DONT_MAP_INDEX) &&
        (mem == L3_DEFIPm ||
         mem == L3_DEFIP_ONLYm ||
         mem == L3_DEFIP_DATA_ONLYm ||
         mem == L3_DEFIP_HIT_ONLY_Xm ||
         mem == L3_DEFIP_HIT_ONLY_Ym ||
         mem == L3_DEFIP_HIT_ONLYm ||
         mem == L3_DEFIP_PAIR_128m ||
         mem == L3_DEFIP_PAIR_128_ONLYm ||
         mem == L3_DEFIP_PAIR_128_DATA_ONLYm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Xm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Ym ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLYm)) {
        if (_soc_mem_defip_index_is_invalid(unit, mem, index) == TRUE) {
            return SOC_E_PARAM;
        }
    } else {
        entry_num_max = soc_mem_index_max(unit, mem);

        if (index < 0 || index > entry_num_max) {
            LOG_WARN(BSL_LS_SOC_SOCMEM,
                     (BSL_META_U(unit,
                                 "soc_mem_write: invalid index %d for memory %s\n"),
                      index, SOC_MEM_NAME(unit, mem)));
            return SOC_E_PARAM;
        }
    }

    if (_soc_mem_read_tcam_is_invalid(unit, mem, index) == TRUE) {
        LOG_WARN(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "soc_mem_write: invalid index %d for memory %s\n"),
                  index, SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    if (meminfo->flags & SOC_MEM_FLAG_READONLY) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                             "soc_mem_write: attempt to write R/O memory %s\n"),
                   SOC_MEM_NAME(unit, mem)));
        return SOC_E_INTERNAL;
    }

    return SOC_E_NONE;
}

/*
 * _soc_mem_write_copyno_update
 *
 * Called from within soc_mem_write.  When writing to Arad, the writer must
 * select which core is being written to.  If SOC_CORE_ALL or COPYNO_ALL is
 * selected, then override the copyno variable to use broadcast registers
 * instead of iterating and writing to each separately.
 *
 * Returns SOC_E_xxx
 */
STATIC int
_soc_mem_write_copyno_update(int unit, soc_mem_t mem, int *copyno,
                             int *copyno_override)
{
    int rv = SOC_E_NONE;

#ifdef BCM_PETRA_SUPPORT
    /* Use core broadcast writes when possible for more efficient writes */
    if (SOC_IS_ARAD(unit) && (*copyno == SOC_CORE_ALL || *copyno == COPYNO_ALL || SOC_BLOCK_IS_BROADCAST(unit, *copyno))) {
        *copyno_override = soc_arad_mem_broadcast_block_get(unit, mem);
        if (!*copyno_override) {
            *copyno = COPYNO_ALL;
        }
    } else
#endif /* BCM_PETRA_SUPPORT */

    if (*copyno != COPYNO_ALL) {
        if (!SOC_MEM_BLOCK_VALID(unit, mem, *copyno)) {
            LOG_WARN(BSL_LS_SOC_SOCMEM,
                     (BSL_META_U(unit,
                                 "soc_mem_write: invalid block %d for memory %s\n"),
                      *copyno, SOC_MEM_NAME(unit, mem)));
            rv = SOC_E_PARAM;
        }
    }

    return rv;
}

/*
 * Function: _soc_mem_write_sirius_sim_adjust
 *
 * Purpose:  Called within _soc_mem_write.  On Sirius devices, in certain cases
 *           the schan write command address should have the block masked off.
 *
 * Returns:  Nothing, but may alter the 3 input parameters (other than unit).
 *
 */
STATIC void
_soc_mem_write_sirius_sim_adjust(int unit, schan_msg_t *schan_msg)
{
#ifdef BCM_SIRIUS_SUPPORT
        if (SOC_IS_SIRIUS(unit) && (!SAL_BOOT_PLISIM || SAL_BOOT_BCMSIM)) {
            /* mask off the block field */
            schan_msg->writecmd.address &= 0x3F0FFFFF;
        }
#endif /* BCM_SIRIUS_SUPPORT */
}

#ifdef BCM_TRIDENT_SUPPORT
/*
 * Function: _soc_mem_write_instances_cache_update
 *
 * Purpose:  If this mem view has several physical instances, 
 *               they need to be updated accordingly.
 *
 * Returns:  None
 */
STATIC void
_soc_mem_write_instances_cache_update(int unit, soc_mem_t mem, 
                                      int blk, int no_cache, 
                                      int index_min, int index_max,
                                      int array_index, void *entry_data)
{
    soc_mem_t mem_cp[SOC_MEM_CP_MAX_NUM];

    /*
     * There are 2 physical instances of THDO_OFFSET_0 on TD+.
     * Writing this table writes all two physical instances.
     * The instances are THDO_OFFSET_0A, THDO_OFFSET_0B.
     */
    if (SOC_IS_TRIDENT(unit)) {
        int r = SOC_E_NONE, i;

        r = soc_trident_mem_cp_get(unit, mem, mem_cp);
        if (SOC_FAILURE(r)) {
            return;
        }

        for (i = 0; i < SOC_MEM_CP_MAX_NUM; i++) {
            if (soc_mem_is_valid(unit, mem_cp[i])) {
                _soc_mem_write_range_cache_update(unit, mem_cp[i],
                                                  array_index, blk, no_cache,
                                                  index_min, index_max,
                                                  entry_data);
            }
        }
    }
}
#endif /* BCM_TRIDENT_SUPPORT */

/*
 * Function: _soc_mem_write_cache_update
 *
 * Purpose:  Called by soc_mem_write_extended().  After a successful write,
 *           update the cache to hold the new data (if applicable).
 *
 * Returns:  Nothing
 */
void
_soc_mem_write_cache_update(int unit, soc_mem_t mem, int blk, int no_cache,
                            int index, int array_index, void *entry_data,
                            void *entry_data_ptr, uint32 *cache_entry_data,
                            uint32 *converted_entry_data)
{
    /* Write back to cache if active */
    uint32 *cache;
    uint8 *vmap;
    int entry_dw;
    int mem_array_vmap_offset;
    int mem_array_cache_offset;

    SOC_MEM_ALIAS_TO_ORIG(unit,mem);
    cache = SOC_MEM_STATE(unit, mem).cache[blk];
    vmap = SOC_MEM_STATE(unit, mem).vmap[blk];
    entry_dw = soc_mem_entry_words(unit, mem);
    mem_array_vmap_offset = array_index * soc_mem_index_count(unit, mem);
    mem_array_cache_offset = mem_array_vmap_offset * entry_dw;

    if (cache != NULL && !no_cache && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
        if (SOC_WARM_BOOT(unit) || SOC_IS_RELOADING(unit) ||
            SOC_HW_ACCESS_DISABLE(unit)) {
            if (!SOC_WARM_BOOT(unit)) {
                CACHE_VMAP_CLR(vmap, mem_array_vmap_offset + index);
            }
        } else if (_SOC_MEM_CHK_L2_MEM(mem)) {
            /* If an invalid entry is being written then clear the cache */
            if (((mem == L2_ENTRY_2m &&
                 soc_L2_ENTRY_2m_field32_get(unit, entry_data, VALID_0f) &&
                 soc_L2_ENTRY_2m_field32_get(unit, entry_data, VALID_1f)) ||
                 ((mem == L2Xm || mem == L2_ENTRY_1m) &&
                  soc_mem_field32_get(unit, mem, entry_data, VALIDf))) &&
                 ((mem == L2_ENTRY_2m &&
                  (soc_mem_field32_get(unit, mem, entry_data, STATIC_BIT_0f) &&
                   soc_mem_field32_get(unit, mem, entry_data, STATIC_BIT_1f))) ||
                   ((mem == L2Xm || mem == L2_ENTRY_1m) &&
                    soc_mem_field32_get(unit, mem, entry_data, STATIC_BITf)))) {
#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT)
                sal_memcpy(cache + index * entry_dw,
                           (entry_data_ptr == converted_entry_data) ?
                           cache_entry_data : entry_data, entry_dw * 4);
#else
                sal_memcpy(cache + index * entry_dw, entry_data, entry_dw * 4);
#endif /* BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT */
                CACHE_VMAP_SET(vmap, index);
            } else {
                CACHE_VMAP_CLR(vmap, index);
            }
            if (mem == L2_ENTRY_1m) {
                vmap = SOC_MEM_STATE(unit, L2_ENTRY_2m).vmap[blk];
                CACHE_VMAP_CLR(vmap, index/2);
            } else if (mem == L2_ENTRY_2m) {
                vmap = SOC_MEM_STATE(unit, L2_ENTRY_1m).vmap[blk];
                CACHE_VMAP_CLR(vmap, index*2);
                CACHE_VMAP_CLR(vmap, index*2 + 1);
            }
        } else {
            if (SOC_IS_DPP(unit)) {
                /* DPP devices should use this write even if they are compiled with trident devices. */
                sal_memcpy(cache + mem_array_cache_offset + index * entry_dw,
                           entry_data, entry_dw * 4);
            } else {
#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) \
        || defined(BCM_HURRICANE2_SUPPORT)
                if (entry_data_ptr == converted_entry_data) {
                    sal_memcpy(cache + index * entry_dw,
                               cache_entry_data, entry_dw * 4);
#if defined(INCLUDE_MEM_SCAN)
                    /* Update memscan TCAM cache if necessary */
                    soc_mem_scan_tcam_cache_update(unit, mem,
                                                   index, index,
                                                   entry_data_ptr);
#endif /* INCLUDE_MEM_SCAN */
#ifdef BCM_TRIDENT2_SUPPORT
#ifdef BCM_WARM_BOOT_SUPPORT
                    if (SOC_IS_TRIDENT2X(unit) || SOC_IS_TITAN2X(unit)) {
                        soc_mem_overlay_tcam_update(unit, mem, blk,
                                                    index, index);
                    }
#endif
#endif
                } else {
                    sal_memcpy(cache + index * entry_dw,
                               entry_data, entry_dw * 4);
                }
#else
                sal_memcpy(cache + mem_array_cache_offset + index * entry_dw,
                           entry_data, entry_dw * 4);
#endif /* BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT */
            }
            CACHE_VMAP_SET(vmap, (mem_array_vmap_offset + index));

            /* Clear VMAP index in cache when tunnel info is null entry */
            if (_SOC_MEM_CHK_EGR_IP_TUNNEL(mem) &&
                    SOC_MEM_IS_VALID(unit, mem)) {
                if (sal_memcmp(entry_data, soc_mem_entry_null(unit, mem),
                                       soc_mem_entry_words(unit, mem) * sizeof(uint32)) == 0) {
                    CACHE_VMAP_CLR(vmap, index);
                }
            }

#ifdef BCM_TRIDENT_SUPPORT
            _soc_mem_write_instances_cache_update(unit, mem, blk,
                                                  no_cache, index, index,
                                                  array_index, entry_data);
#endif /* BCM_TRIDENT_SUPPORT */
        }
    }
}

void
_soc_mem_alpm_write_cache(int unit, soc_mem_t mem, int copyno,
                          int index, void *entry_data)
{
    /* Write back to cache if active */
    int blk;
    uint32 *cache;
    uint8 *vmap;
    int entry_dw;
    uint32 converted_entry_data[SOC_MAX_MEM_WORDS];
    uint32 cache_entry_data[SOC_MAX_MEM_WORDS];
    void *entry_data_ptr;

    /* Currently only apply to LPM tables */
    switch (mem) {
        case L3_DEFIPm:
        case L3_DEFIP_PAIR_128m:
        case L3_DEFIP_AUX_TABLEm:
        case L3_DEFIP_ALPM_RAWm:
        case L3_DEFIP_ALPM_IPV4m:
        case L3_DEFIP_ALPM_IPV4_1m:
        case L3_DEFIP_ALPM_IPV6_64m:
        case L3_DEFIP_ALPM_IPV6_64_1m:
        case L3_DEFIP_ALPM_IPV6_128m:
            break;
        default:
            return ;
    }

    entry_data_ptr = _soc_mem_write_tcam_to_hw_format(unit, mem, entry_data,
                                                      cache_entry_data,
                                                      converted_entry_data);
    MEM_LOCK(unit, mem);

    SOC_MEM_ALIAS_TO_ORIG(unit,mem);

    SOC_MEM_BLOCK_ITER(unit, mem, blk) {
        if (copyno != COPYNO_ALL && copyno != blk) {
            continue;
        }

        cache = SOC_MEM_STATE(unit, mem).cache[blk];
        vmap = SOC_MEM_STATE(unit, mem).vmap[blk];
        entry_dw = soc_mem_entry_words(unit, mem);

        if (cache != NULL && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
            if (SOC_WARM_BOOT(unit) || SOC_IS_RELOADING(unit) ||
                SOC_HW_ACCESS_DISABLE(unit)) {
                if (!SOC_WARM_BOOT(unit)) {
                    CACHE_VMAP_CLR(vmap, index);
                }
            } else {
                if (entry_data_ptr == converted_entry_data) {
                    sal_memcpy(cache + index * entry_dw,
                               cache_entry_data, entry_dw * 4);
#if defined(INCLUDE_MEM_SCAN)
                    /* Update memscan TCAM cache if necessary */
                    soc_mem_scan_tcam_cache_update(unit, mem,
                                                   index, index,
                                                   entry_data_ptr);
#endif /* INCLUDE_MEM_SCAN */
                } else {
                    sal_memcpy(cache + index * entry_dw,
                               entry_data, entry_dw * 4);
                }
                CACHE_VMAP_SET(vmap, index);
            }
        }
    }

    MEM_UNLOCK(unit, mem);

}

#ifdef BCM_ESW_SUPPORT
STATIC uint32
_soc_l2mode_fifo_enable_memwr_field_get(int unit)
{
    uint32 rval = 0, memwr = 0;
    if (SOC_IS_TRIDENT2X(unit)) {
        if (READ_AUX_ARB_CONTROLr(unit, &rval) < 0) {
            return 0;
        }
        memwr = soc_reg_field_get(unit, AUX_ARB_CONTROLr,
                                  rval,
                                  L2_MOD_FIFO_ENABLE_MEMWRf);
    }
    return memwr;
}

STATIC int
_soc_l2mode_fifo_enable_memwr_field_set(int unit, int enable)
{
    uint32 rval = 0;
    uint32 ena = (enable==0)?0:1;

    if (SOC_IS_TRIDENT2X(unit)) {
        SOC_IF_ERROR_RETURN(
                READ_AUX_ARB_CONTROLr(unit, &rval));
        soc_reg_field_set(unit, AUX_ARB_CONTROLr, &rval,
                        L2_MOD_FIFO_ENABLE_MEMWRf, ena);
        SOC_IF_ERROR_RETURN(
                WRITE_AUX_ARB_CONTROLr(unit, rval));
    }
    return SOC_E_NONE;
}

#if defined(BCM_TRIUMPH3_SUPPORT)
STATIC uint32
_soc_l2mode_fifo_enable_delete_field_get(int unit)
{
    uint32 rval = 0, l2_delete = 0;
    int i;
    soc_reg_t fifo_reg[] = {
        L2_MOD_FIFO_ENABLEr, 
        AUX_ARB_CONTROLr
    };
    soc_reg_t fifo_fld[] = {
        L2_DELETEf,
        L2_MOD_FIFO_ENABLE_L2_DELETEf
    };

    for (i = 0; i < sizeof(fifo_fld) / sizeof(fifo_fld[0]); i++) {
        if (SOC_REG_FIELD_VALID(unit, fifo_reg[i], fifo_fld[i])) {
            if (soc_reg32_get(unit, fifo_reg[i], REG_PORT_ANY, 0, &rval) < 0) {
                return 0;
            }
            l2_delete = soc_reg_field_get(unit, fifo_reg[i], rval, fifo_fld[i]);
            break;
        }
    }
    return l2_delete;

}

STATIC int
_soc_l2mode_fifo_enable_delete_field_set(int unit, uint32 enable)
{
    uint32 rval = 0;
    int i;
    soc_reg_t fifo_reg[] = {
        L2_MOD_FIFO_ENABLEr,
        AUX_ARB_CONTROLr
    };
    soc_reg_t fifo_fld[] = {
        L2_DELETEf,
        L2_MOD_FIFO_ENABLE_L2_DELETEf
    };

    for (i = 0; i < sizeof(fifo_fld) / sizeof(fifo_fld[0]); i++) {
        if (SOC_REG_FIELD_VALID(unit, fifo_reg[i], fifo_fld[i])) {
            SOC_IF_ERROR_RETURN(
                soc_reg32_get(unit, fifo_reg[i], REG_PORT_ANY, 0, &rval));
            soc_reg_field_set(unit, fifo_reg[i], &rval, fifo_fld[i], enable);
            SOC_IF_ERROR_RETURN(
                soc_reg32_set(unit, fifo_reg[i], REG_PORT_ANY, 0, rval));
            break;
        }
    }

    return SOC_E_NONE;
}

STATIC uint32
_soc_l2mode_fifo_enable_insert_field_get(int unit)
{
    uint32 rval = 0, l2_insert = 0;
    int i;
    soc_reg_t fifo_reg[] = {
        L2_MOD_FIFO_ENABLEr,
        AUX_ARB_CONTROLr
    };
    soc_reg_t fifo_fld[] = {
        L2_INSERTf,
        L2_MOD_FIFO_ENABLE_L2_INSERTf
    };

    for (i = 0; i < sizeof(fifo_fld) / sizeof(fifo_fld[0]); i++) {
        if (SOC_REG_FIELD_VALID(unit, fifo_reg[i], fifo_fld[i])) {
            if (soc_reg32_get(unit, fifo_reg[i], REG_PORT_ANY, 0, &rval) < 0) {
                return 0;
            }
            l2_insert = soc_reg_field_get(unit, fifo_reg[i], rval, fifo_fld[i]);
            break;
        }
    }
    return l2_insert;

}

STATIC int
_soc_l2mode_fifo_enable_insert_field_set(int unit, uint32 enable)
{
    uint32 rval = 0;
    int i;
    soc_reg_t fifo_reg[] = {
        L2_MOD_FIFO_ENABLEr,
        AUX_ARB_CONTROLr
    };
    soc_reg_t fifo_fld[] = {
        L2_INSERTf,
        L2_MOD_FIFO_ENABLE_L2_INSERTf
    };

    for (i = 0; i < sizeof(fifo_fld) / sizeof(fifo_fld[0]); i++) {
        if (SOC_REG_FIELD_VALID(unit, fifo_reg[i], fifo_fld[i])) {
            SOC_IF_ERROR_RETURN(
                soc_reg32_get(unit, fifo_reg[i], REG_PORT_ANY, 0, &rval));
            soc_reg_field_set(unit, fifo_reg[i], &rval, fifo_fld[i], enable);
            SOC_IF_ERROR_RETURN(
                soc_reg32_set(unit, fifo_reg[i], REG_PORT_ANY, 0, rval));
            break;
        }
    }

    return SOC_E_NONE;
}

#endif /* defined(BCM_TRIUMPH3_SUPPORT) */

/* When L2 entries are corrected, l2mode_fifo memwr field must be '0'.
* Reason1: We need not to produce the memwr event to L2mode_fifo
* during soc_ser_sram_correction.
* Reason2: If l2mode_fifo is full and memwr_field is '1', it will lead to recursion.
* soc_ser_sram_correction -> _soc_mem_write_schan_msg_send -> soc_ser_sram_correction
* _soc_mem_write_schan_msg_send -> .......
*/
STATIC void
_soc_ser_sram_correction_pre(int unit, soc_mem_t mem, uint32 mem_val)
{
    switch(mem) {
        case L2Xm:
            if (mem_val) {
                (void)_soc_l2mode_fifo_enable_memwr_field_set(unit, 0);
            }
            break;
        default:
            return;
    }
}

STATIC void
_soc_ser_sram_correction_post(int unit, soc_mem_t mem, uint32 mem_val)
{
    switch(mem) {
        case L2Xm:
            if (mem_val) {
                (void)_soc_l2mode_fifo_enable_memwr_field_set(unit, 1);
            }
            break;
        default:
            return;
    }
}

STATIC void
_soc_ser_sram_correction_init(int unit, soc_mem_t mem, uint32 *mem_val)
{
    switch(mem) {
        case L2Xm:
            *mem_val = _soc_l2mode_fifo_enable_memwr_field_get(unit);
            break;
        default:
            return ;
    }
    return ;
}
#endif /* BCM_ESW_SUPPORT */

/*
 * Function: _soc_mem_write_schan_msg_send
 *
 * Purpose:  Called by soc_mem_write_extended().  Send the requested
 *           schan_msg_t.  On ESW chips, retry errors.  If SER is available,
 *           try to use it to correct errors.
 *
 * Returns:  SOC_E_xxx
 */
STATIC int
_soc_mem_write_schan_msg_send(int unit, schan_msg_t *schan_msg, soc_mem_t mem,
                              int blk, int index2)
{
    int rv;
    int allow_intr = SOC_IS_SAND(unit);
    int entry_dw = soc_mem_entry_words(unit, mem);

#ifdef BCM_ESW_SUPPORT
    uint32 retries_remaining = HASH_MEM_OP_RETRY_COUNT;
    int rv1 = SOC_E_NONE, rv2 = SOC_E_NONE;
    uint32 need_retry = 0;
    int pipe = 0;
    int acc_type;
    uint32 entry[SOC_MAX_MEM_WORDS];

    do {
        rv = soc_schan_op(unit, schan_msg, 2 + entry_dw, 0, allow_intr);

        if (SOC_E_FAIL == rv && soc_feature(unit, soc_feature_shared_hash_mem)) {
            if (retries_remaining == HASH_MEM_OP_RETRY_COUNT) {
                _soc_ser_sram_correction_init(unit, mem, &need_retry);
            }
            _soc_ser_sram_correction_pre(unit, mem, need_retry);

            if (need_retry){
                rv2 = soc_schan_op(unit, schan_msg, 2 + entry_dw, 0, allow_intr);
            }
            /* If rv2==SOC_E_FAIL, the failure of soc_schan_op is not due to L2_mode_fifo is filled,
            * because we diable memwr fifo events, so we need to correct the memory.
            */
            if (rv2 == SOC_E_FAIL || need_retry == 0) {
                /* For multiple pipe devices, SER error is attempted from alternative pipe
                 * before attempting for inline correction.
                 * For single pipe devices, inline SER correction is done
                 */
                if (NUM_PIPE(unit) > 1) {
                    for (pipe = 0; pipe < NUM_PIPE(unit); pipe++) {
                        /* determine 1st pipe which has the problem */
                        if (SOC_IS_TOMAHAWKX(unit) ) {
                            acc_type = (pipe == 0)? _SOC_MEM_ADDR_ACC_TYPE_PIPE_0 :
                                       (pipe == 1)? _SOC_MEM_ADDR_ACC_TYPE_PIPE_1 :
                                       (pipe == 2)? _SOC_MEM_ADDR_ACC_TYPE_PIPE_2 :
                                                    _SOC_MEM_ADDR_ACC_TYPE_PIPE_3;
                        } else if (SOC_IS_TRIDENT2X(unit) || SOC_IS_TITAN2X(unit)) {
                            acc_type = (pipe == 0) ? _SOC_MEM_ADDR_ACC_TYPE_PIPE_X:
                                _SOC_MEM_ADDR_ACC_TYPE_PIPE_Y;
                        } else {
                            return SOC_E_UNAVAIL;
                        }

                        LOG_VERBOSE(BSL_LS_SOC_SOCMEM, (BSL_META_U(unit,
                                  "unit %d: inline ALPM correction LOOP: pipe:%d index %d\n"),
                                   unit, pipe, index2));
                        rv = soc_mem_pipe_select_read(unit, SOC_MEM_NO_FLAGS, mem, MEM_BLOCK_ANY,
                                                      acc_type, index2, entry);
                        if (SOC_E_FAIL == rv) {
                            LOG_ERROR(BSL_LS_SOC_SOCMEM, (BSL_META_U(unit,
                                      "unit %d: inline ALPM correction: will try ser_correction for pipe %d, index %d\n"),
                                      unit, pipe, index2));
                            break;
                        }
                    }
                }
                if (pipe >=0 && pipe < NUM_PIPE(unit)) {
                    rv1 = soc_ser_sram_correction(unit, pipe,
                                                  schan_msg->writecmd.header.v2.dst_blk,
                                                  schan_msg->writecmd.address, mem,
                                                  blk, index2, NULL);
                }
            } 
            _soc_ser_sram_correction_post(unit, mem, need_retry);

            if (rv1 == SOC_E_NONE && retries_remaining > 0) {
                retries_remaining--;
            } else {
                retries_remaining = 0; /* Don't try if the correction fails */
            }
        } else if (SOC_E_FAIL == rv && soc_feature(unit, soc_feature_ignore_mem_write_nak)){
            /*
             * In bcm53400 and bcm56160 series, when parity error happened in
             * FP_METER_TABLE/EFP_METER_TABLE, sometimes will always get SOC_E_FAIL.
             * Add retries here to avoid infinite loop.
             */
            if ((mem == EFP_METER_TABLEm) || (mem == FP_METER_TABLEm)) {
                retries_remaining--;
            }
        } else if (SOC_FAILURE(rv)) {
            retries_remaining--;
        }
    } while (SOC_FAILURE(rv) && retries_remaining > 0);

#else
    rv = soc_schan_op(unit, schan_msg, 2 + entry_dw, 0, allow_intr);
#endif /* BCM_ESW_SUPPORT */

    return rv;
}

/*
 * Function: _soc_mem_write_alpm_bkt_view_set
 *
 * Purpose:  Called by soc_mem_write_extended().  If writing to the ALPM
 *           memories, add special processing to update the bucket view map.
 *
 * Returns:  Nothing
 */
STATIC void
_soc_mem_write_alpm_bkt_view_set(int unit, soc_mem_t mem, int index,
                                 void *entry_data)
{
#ifdef ALPM_ENABLE
    if (mem == L3_DEFIP_ALPM_IPV4m || mem == L3_DEFIP_ALPM_IPV4_1m ||
        mem == L3_DEFIP_ALPM_IPV6_64m || mem == L3_DEFIP_ALPM_IPV6_64_1m ||
        mem == L3_DEFIP_ALPM_IPV6_128m) {
        if (soc_mem_field32_get(unit, mem, entry_data, VALIDf)) {
            if (SOC_IS_TRIDENT2(unit) || SOC_IS_TRIDENT2PLUS(unit)) {
                _soc_trident2_alpm_bkt_view_set(unit, index, mem);
            } 
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_APACHE_SUPPORT)
            else {
                soc_alpm_cmn_bkt_view_set(unit, index, mem);
            }
#endif
        }
    }
#endif /* ALPM_ENABLE */
}

/*
 * Function: _soc_mem_write_td2_shadow_entry_set
 *
 * Purpose:  Called by soc_mem_write_extended().  On TD2, if writing to the
 *           X or Y pipe map tables, update the shadow entry for them.
 *
 * Returns:  SOC_E_xxx
 */
STATIC int
_soc_mem_write_td2_shadow_entry_set(int unit, soc_mem_t mem, int index,
                                    void *entry_data)
{
    int rv = SOC_E_NONE;

#ifdef BCM_TRIDENT2_SUPPORT
    /* For MMU_INTFI_X/YPIPE_FC_MAP_TBL0/1, update software shadow table. */
    if (SOC_IS_TRIDENT2(unit)) {
        if (mem == MMU_INTFI_XPIPE_FC_MAP_TBL0m ||
            mem == MMU_INTFI_XPIPE_FC_MAP_TBL1m ||
            mem == MMU_INTFI_YPIPE_FC_MAP_TBL0m ||
            mem == MMU_INTFI_YPIPE_FC_MAP_TBL1m){
            rv = soc_trident2_fc_map_shadow_entry_set(unit, mem, index,
                                                      entry_data);
        }
    }
#endif

    return rv;
}
 
#ifdef HR3LITE_LPM_WAR
/*
 * Function: _soc_mem_hr3_lpm_duplicate
 *
 * Purpose:  Called by _soc_mem_write_schan_msg_create(). 
 *          This function is used to duplicate a LPM-DATA entry on 
 *          HR3-Lite device. LPM table in HR3-Lite support 64 entries 
 *          only but this LPM TCAM for LPM entry between 32~63 can only 
 *          be hit with LPM-DATA in entry between 512~543.
 *
 * Returns:  SOC_E_xxx
 */
STATIC int
_soc_mem_hr3_lpm_duplicate(int unit, soc_mem_t mem, int blk,
            int copyno, int array_index, void *in_entry_data, int index)
{
    defip_entry_t *lpm_entry = NULL;
    defip_data_only_entry_t lpm_data_entry;
    defip_hit_only_entry_t lpm_hit_entry;
    int lpm_shadow_size = 0;
    void *entry_data = in_entry_data;
    void *lpm_showdow_entry;
    schan_msg_t schan_msg;
    int entry_dw = 0, data_byte_len = 0;
    int src_blk = 0, dst_blk = 0, acc_type = 0;
    int copyno_override = 0; /* If non-zero, will override copyno */
    uint32 maddr;
    uint8 at;
    soc_mem_t lpm_shadow[] = {L3_DEFIP_DATA_ONLYm, L3_DEFIP_HIT_ONLYm};
    soc_mem_t update_mem = INVALIDm;
    int mem_update_cnt = 0;
    int mem_update_id = -1;
    int i;
    uint32  buf;
    soc_mem_info_t      *m;
    soc_field_info_t    *fld;

    if (!soc_feature(unit, soc_feature_hr3_lite_lpm_shadow_hit)) {
        return SOC_E_NONE;
    }

    mem_update_id = 0;
    if (mem == L3_DEFIPm) {
        mem_update_cnt = 2;
        lpm_entry = entry_data;
    } else {
        mem_update_cnt = 1;
        if (mem == L3_DEFIP_HIT_ONLYm) {
            mem_update_id = 1;
        }
    }

    for (i = mem_update_id; mem_update_cnt > 0; mem_update_cnt--, i++) {
        
        /* 
        * L3_DEFIPm in HW view is constucted with three tables of 
        * LPM-TCAM, LPM-DATA and LPM_HTI.
        *  - This WAR is to duplicate LPM-DATA and LPM-HIT entry.
        */
        update_mem = lpm_shadow[i];
        LOG_INFO(BSL_LS_SOC_LPM, (BSL_META_U(unit, "HR3-Lite LPM_WAR :"
                "updateing mem=%s,index=%d\n"), 
                SOC_MEM_NAME(unit,update_mem), index));
        if (lpm_entry != NULL) {
            m = &SOC_MEM_INFO(unit, update_mem);
            if (update_mem == L3_DEFIP_DATA_ONLYm) {
                lpm_shadow_size = sizeof(defip_data_only_entry_t);
                lpm_showdow_entry = &lpm_data_entry;
            } else {
                lpm_shadow_size = sizeof(defip_hit_only_entry_t);
                lpm_showdow_entry = &lpm_hit_entry;
            }
            sal_memset(lpm_showdow_entry, 0, lpm_shadow_size);
            for (fld = &m->fields[0]; fld < &m->fields[m->nFields]; fld++) {
                
                buf = soc_L3_DEFIPm_field32_get(unit, lpm_entry, fld->field);
                soc_mem_field32_set(unit, update_mem, 
                        lpm_showdow_entry, fld->field, buf);
            }
            /* assign processing entry_data to this updating LPM shadow entry */
            entry_data = lpm_showdow_entry;
        }
        
        /* start the LPM shadow update */
        mem = update_mem;

        if (bsl_check(bslLayerSoc, bslSourceMem, bslSeverityNormal, unit)) {
            LOG_CLI((BSL_META_U(unit,
                                "soc_mem_write unit %d: %s.%s[%d]: "),
                    unit, SOC_MEM_NAME(unit, mem),
                    SOC_BLOCK_NAME(unit, copyno), index));
            soc_mem_entry_dump(unit, mem, entry_data);
            LOG_CLI((BSL_META_U(unit,
                                "\n")));
        }
        /* start on LPM-DATAm duplication */
        entry_dw = soc_mem_entry_words(unit, mem);
        data_byte_len = entry_dw * sizeof (uint32);
    
        schan_msg_clear(&schan_msg);
        acc_type = SOC_MEM_ACC_TYPE(unit, mem);
        src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
    
        sal_memcpy(schan_msg.writecmd.data, entry_data, data_byte_len);
        _soc_mem_write_copyno_update(unit, mem, &copyno, &copyno_override);
    
        maddr = soc_mem_addr_get(unit, mem, array_index, blk, index, &at);
        schan_msg.writecmd.address = maddr;
    
        _soc_mem_dst_blk_update(unit, blk, maddr, &dst_blk);
    
        soc_schan_header_cmd_set(unit, &schan_msg.header, WRITE_MEMORY_CMD_MSG,
                dst_blk, src_blk, acc_type, data_byte_len, 0, 0);
        SOC_IF_ERROR_RETURN(_soc_mem_write_schan_msg_send(unit, 
                &schan_msg, mem, blk, index));
        LOG_INFO(BSL_LS_SOC_LPM, (BSL_META_U(unit, "HR3-Lite LPM_WAR :"
                "mem=%s(%d) duplicated\n"), 
                SOC_MEM_NAME(unit,update_mem), index));
    }

    return SOC_E_NONE;
}
#endif 

/*
 * Function: _soc_mem_write_schan_msg_create
 *
 * Purpose:  Called by soc_mem_write_extended().  Create the schan_msg_t struct
 *           and send it down the line using _soc_mem_write_schan_msg_send().
 *           Set copyno=COPYNO_ALL for all copies of the memory.
 *
 * Returns:  SOC_E_xxx
 */
STATIC int
_soc_mem_write_schan_msg_create(int unit, uint32 flags, 
                                soc_mem_t mem, int copyno,
                                int array_index, void *entry_data_ptr,
                                void *entry_data,
                                uint32 *cache_entry_data,
                                uint32 *converted_entry_data,
                                int index, int no_cache)
{
    int blk;
    int index2;
    schan_msg_t schan_msg;
    int src_blk, dst_blk, acc_type;
    int copyno_override = 0; /* If non-zero, will override copyno */
    uint32 maddr;
    uint8 at;
    int entry_dw = soc_mem_entry_words(unit, mem);
    int data_byte_len = entry_dw * sizeof (uint32);
#ifdef HR3LITE_LPM_WAR
    int     lpm_data_shadow_index = -1;
#endif

    /*
     * Setup S-Channel command packet
     *
     * NOTE: the datalen field matters only for the Write Memory and
     * Write Register commands, where it is used only by the CMIC to
     * determine how much data to send, and is in units of bytes.
     */
    schan_msg_clear(&schan_msg);
    acc_type = SOC_MEM_ACC_TYPE(unit, mem);
    src_blk = SOC_IS_SIRIUS(unit) ? 0 : SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));

    sal_memcpy(schan_msg.writecmd.data, entry_data_ptr,
               entry_dw * sizeof(uint32));

    _soc_mem_write_copyno_update(unit, mem, &copyno, &copyno_override);

    if (bsl_check(bslLayerSoc, bslSourceMem, bslSeverityNormal, unit)) {
        LOG_CLI((BSL_META_U(unit,
                            "soc_mem_write unit %d: %s.%s[%d]: "),
                 unit, SOC_MEM_NAME(unit, mem),
                 SOC_BLOCK_NAME(unit, copyno), index));
        soc_mem_entry_dump(unit, mem, entry_data_ptr);
        LOG_CLI((BSL_META_U(unit,
                            "\n")));
    }

#ifdef HR3LITE_LPM_WAR
    if (soc_feature(unit, soc_feature_hr3_lite_lpm_shadow_hit) && 
            NEED_HR3LITE_LPM_SHADOW(mem)) {
        lpm_data_shadow_index = HR3LITE_LPM_HIT_INDEX2(index);
    }
#endif  /* HR3LITE_LPM_WAR */

    /* Write to one or all copies of the memory */
    SOC_MEM_BLOCK_ITER(unit, mem, blk) {
        if (copyno_override) {
            blk = copyno = copyno_override;
        } else if (copyno != COPYNO_ALL && copyno != blk) {
            continue;
        }

        index2 = index;
        if (!(flags & SOC_MEM_DONT_MAP_INDEX)) {
            _soc_mem_read_defip_index_map(unit, mem, index, &index2);
        }

        maddr = soc_mem_addr_get(unit, mem, array_index, blk, index2, &at);
        schan_msg.writecmd.address = maddr;

        _soc_mem_dst_blk_update(unit, blk, maddr, &dst_blk);
        _soc_mem_write_sirius_sim_adjust(unit, &schan_msg);

        soc_schan_header_cmd_set(unit, &schan_msg.header, WRITE_MEMORY_CMD_MSG,
                                 dst_blk, src_blk, acc_type, data_byte_len, 0,
                                 0);

        /* Write header + address + entry_dw data DWORDs */
        /* Note: The hardware does not send WRITE_MEMORY_ACK_MSG. */
        if (2 + entry_dw > CMIC_SCHAN_WORDS(unit)) {
           LOG_WARN(BSL_LS_SOC_SOCMEM,
                    (BSL_META_U(unit,
                        "soc_mem_write: assert will fail for memory %s\n"),
                    SOC_MEM_NAME(unit, mem)));
        }

#ifdef CRASH_RECOVERY_SUPPORT
        if (!(BCM_UNIT_DO_HW_READ_WRITE(unit))){
            SOC_IF_ERROR_RETURN(_soc_mem_write_schan_msg_send(unit, &schan_msg, mem,
                                                          blk, index2));
        }
#else
            SOC_IF_ERROR_RETURN(_soc_mem_write_schan_msg_send(unit, &schan_msg, mem,
                                                          blk, index2));
#endif /* CRASH_RECOVERY_SUPPORT */

#ifdef HR3LITE_LPM_WAR
        if (lpm_data_shadow_index != -1) {
            LOG_INFO(BSL_LS_SOC_LPM, (BSL_META_U(unit, "HR3-Lite LPM_WAR :"
                    "request mem=%s(%d) duplicate on LPM DATA/HIT entry(%d)\n"), 
                    SOC_MEM_NAME(unit,mem), index, lpm_data_shadow_index));
            SOC_IF_ERROR_RETURN(_soc_mem_hr3_lpm_duplicate(unit, mem, blk, 
                    copyno, array_index, entry_data, lpm_data_shadow_index));
        }
#endif  /* HR3LITE_LPM_WAR */

        if (copyno_override)
            break;

        /* we dont cache in case block is a broadcast_block*/
        _soc_mem_write_cache_update(unit, mem, blk, no_cache, index,
                                    array_index, entry_data, entry_data_ptr,
                                    cache_entry_data, converted_entry_data);

        /* Update the cache for other memory copies of this memory view */
#ifdef BCM_TRX_SUPPORT
        _soc_mem_aggr_cache_update(unit, mem, blk, no_cache,
                                index, index,
                                array_index, entry_data);
#endif /* BCM_TRX_SUPPORT */
    }
    if (copyno_override) {
        /* if block is a broadcast_block we cach seperatly for each block*/
        SOC_MEM_BLOCK_ITER(unit, mem, blk) {
            _soc_mem_write_cache_update(unit, mem, blk, no_cache, index,
                                        array_index, entry_data, entry_data_ptr,
                                        cache_entry_data, converted_entry_data);
        }

    }

    return SOC_E_NONE;
}

/*
 * Function:
 *    _soc_mem_write_extended
 * Purpose:
 *    Write a memory internal to the SOC.
 * Notes:
 *    GBP/CBP memory should only accessed when MMU is in DEBUG mode.
 */
STATIC int
_soc_mem_write_extended(int unit, uint32 flags, soc_mem_t mem,
               /* in memory arrays this is the element index in the array, otherwise 0 */
               unsigned array_index,
               int copyno, /* Use COPYNO_ALL for all */
               int index, void *entry_data)
{
    soc_mem_info_t *meminfo = &SOC_MEM_INFO(unit, mem);
    int no_cache;
    int rv = SOC_E_NONE;
    void *entry_data_ptr;
    uint32 converted_entry_data[SOC_MAX_MEM_WORDS];
    uint32 cache_entry_data[SOC_MAX_MEM_WORDS];

#ifdef CRASH_RECOVERY_SUPPORT
/* Use crash recovery defined callback for access */
    if (BCM_UNIT_DO_HW_READ_WRITE(unit))
    {
        if(Hw_Log_List[unit].Access_cb.mem_write)
        {
            Hw_Log_List[unit].Access_cb.mem_write(unit, mem, array_index, copyno, index, entry_data);
        }
    }
#endif /* CRASH_RECOVERY_SUPPORT */

    SOC_IF_ERROR_RETURN(_soc_mem_write_sanity_check(unit, flags, mem, index));

    _soc_mem_write_ext_cache_set(flags, &index, &no_cache);
    if (_soc_mem_hercules_write_attempt(unit, mem, copyno, index, entry_data,
                                        &rv)) {
        return rv;
    }

    entry_data_ptr = _soc_mem_write_tcam_to_hw_format(unit, mem, entry_data,
                                                      cache_entry_data,
                                                      converted_entry_data);

    MEM_LOCK(unit, mem);
    _soc_mem_read_tr3_esm_lock(unit, copyno);

    rv = _soc_mem_write_schan_msg_create(unit, flags, mem, copyno, array_index,
                                         entry_data_ptr, entry_data,
                                         cache_entry_data, converted_entry_data,
                                         index, no_cache);

    if (SOC_SUCCESS(rv)) {
        _soc_mem_write_alpm_bkt_view_set(unit, mem, index, entry_data);
        rv = _soc_mem_write_td2_shadow_entry_set(unit, mem, index, entry_data);
    }

    _soc_mem_read_tr3_esm_unlock(unit, copyno);
    MEM_UNLOCK(unit, mem);

    if (NULL != meminfo->snoop_cb &&
        (SOC_MEM_SNOOP_WRITE & meminfo->snoop_flags)) {

        meminfo->snoop_cb(unit, mem, SOC_MEM_SNOOP_WRITE, copyno, index, index,
                          entry_data_ptr, meminfo->snoop_user_data);
    }

    return rv;
}

#ifdef BCM_TRIUMPH2_SUPPORT
STATIC int
_soc_mem_write(int unit, soc_mem_t mem,
               /* in memory arrays this is the element index in the array, otherwise 0 */
               unsigned array_index,
               int copyno,    /* Use COPYNO_ALL for all */
               int index, void *entry_data)
{
    return _soc_mem_write_extended(unit, SOC_MEM_NO_FLAGS, mem,
                                   array_index, copyno, index, entry_data);
}
#endif /* BCM_TRIUMPH2_SUPPORT */

/*
 * Function:
 *    soc_mem_pipe_select_write
 * Purpose:
 *    Write a memory internal to the SOC with pipe selection.
 * Notes:
 *    GBP/CBP memory should only accessed when MMU is in DEBUG mode.
 */

int
soc_mem_pipe_select_write(int unit,
                          uint32 flags,
                          soc_mem_t mem,
                          int copyno,    /* Use COPYNO_ALL for all */
                          int acc_type,
                          int index,
                          void *entry_data)
{
    schan_msg_t schan_msg;
    int blk;
    soc_mem_info_t *meminfo; 
    int entry_dw;
    uint32 *cache;
    uint32 maddr;
    uint8 at, *vmap;
    int no_cache;
    int index2, allow_intr=0;
    int rv;
    int src_blk, dst_blk, data_byte_len;
    void *entry_data_ptr;
#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT)
    uint32 converted_entry_data[SOC_MAX_MEM_WORDS];
    uint32 cache_entry_data[SOC_MAX_MEM_WORDS];
#endif /* BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT */
#ifdef BCM_ESW_SUPPORT
    uint8  write_retry_attempt = 0;
    int rv1 = SOC_E_NONE, rv2 = SOC_E_NONE;
    int pipe = SOC_PIPE_ANY;
    uint32 need_retry = 0;
#endif
    int entry_num_max = 0;

    SOC_MEM_ALIAS_TO_ORIG(unit,mem);
    entry_dw = soc_mem_entry_words(unit, mem);

    if (!soc_feature(unit, soc_feature_two_ingress_pipes)) {
        /* Not a relevant device */
        return SOC_E_UNAVAIL;
    }

    if (!soc_mem_is_valid(unit, mem)) {
        return SOC_E_MEMORY;
    }
    meminfo = &SOC_MEM_INFO(unit, mem);

    if ((flags & SOC_MEM_DONT_USE_CACHE) == SOC_MEM_DONT_USE_CACHE) {
        no_cache = 1;
    } else {
        no_cache = 0;
    }

    if (meminfo->flags & SOC_MEM_FLAG_READONLY) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_write: attempt to write R/O memory %s\n"),
                   SOC_MEM_NAME(unit, mem)));
        return SOC_E_INTERNAL;
    }

#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT)
    if (soc_feature(unit, soc_feature_xy_tcam) &&
        (meminfo->flags & SOC_MEM_FLAG_CAM) &&
        (!(meminfo->flags & SOC_MEM_FLAG_EXT_CAM))) {
        entry_data_ptr = converted_entry_data;
        _soc_mem_tcam_dm_to_xy(unit, mem, 1, entry_data, entry_data_ptr,
                               cache_entry_data);
    } else
#endif /* BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT */
    {
        entry_data_ptr = entry_data;
    }

    /*
     * Setup S-Channel command packet
     *
     * NOTE: the datalen field matters only for the Write Memory and
     * Write Register commands, where it is used only by the CMIC to
     * determine how much data to send, and is in units of bytes.
     */

    schan_msg_clear(&schan_msg);
    data_byte_len = entry_dw * sizeof (uint32);
#ifdef BCM_SIRIUS_SUPPORT
    if (SOC_IS_SIRIUS(unit)) {
        src_blk = 0;
    } else
#endif /* BCM_SIRIUS_SUPPORT */
    {
        src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
    }
    sal_memcpy(schan_msg.writecmd.data,
               entry_data_ptr,
               entry_dw * sizeof (uint32));

    if (copyno != COPYNO_ALL) {
        if (!SOC_MEM_BLOCK_VALID(unit, mem, copyno)) {
            LOG_WARN(BSL_LS_SOC_SOCMEM,
                     (BSL_META_U(unit,
                                 "soc_mem_write: invalid block %d for memory %s\n"),
                      copyno, SOC_MEM_NAME(unit, mem)));
            return SOC_E_PARAM;
        }
    }

    /*
     * When checking index, check for 0 instead of soc_mem_index_min.
     * Diagnostics need to read/write index 0 of Strata ARL and GIRULE.
     */
    if ((flags & SOC_MEM_DONT_MAP_INDEX) &&
        (mem == L3_DEFIPm ||
         mem == L3_DEFIP_ONLYm ||
         mem == L3_DEFIP_DATA_ONLYm ||
         mem == L3_DEFIP_HIT_ONLY_Xm ||
         mem == L3_DEFIP_HIT_ONLY_Ym ||
         mem == L3_DEFIP_HIT_ONLYm ||
         mem == L3_DEFIP_PAIR_128m ||
         mem == L3_DEFIP_PAIR_128_ONLYm ||
         mem == L3_DEFIP_PAIR_128_DATA_ONLYm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Xm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Ym ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLYm)) {
        if (_soc_mem_defip_index_is_invalid(unit, mem, index) == TRUE) {
            return SOC_E_PARAM;
        }
    } else {
        entry_num_max = soc_mem_index_max(unit, mem);

        if (index < 0 || index > entry_num_max) {
            LOG_WARN(BSL_LS_SOC_SOCMEM,
                     (BSL_META_U(unit,
                                 "soc_mem_pipe_select_write: invalid index %d for memory %s acc_type %d\n"),
                      index, SOC_MEM_NAME(unit, mem), acc_type));
            return SOC_E_PARAM;
        }
    }

    if (_soc_mem_read_tcam_is_invalid(unit, mem, index) == TRUE) {
        LOG_WARN(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "soc_mem_pipe_select_write: invalid index %d for memory %s\n"),
                  index, SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    if (bsl_check(bslLayerSoc, bslSourceMem, bslSeverityNormal, unit)) {
        LOG_CLI((BSL_META_U(unit,
                            "soc_mem_pipe_select_write unit %d: %s.%s[%d]: "),
                 unit, SOC_MEM_NAME(unit, mem),
                 SOC_BLOCK_NAME(unit, copyno), index));
        soc_mem_entry_dump(unit, mem, entry_data_ptr);
        LOG_CLI((BSL_META_U(unit,
                            "\n")));
    }

    /* Write to one or all copies of the memory */

    rv = SOC_E_NONE;

    MEM_LOCK(unit, mem);
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_esm_correction)) {
        if (copyno == MEM_BLOCK_ANY) {
            copyno = SOC_MEM_BLOCK_ANY(unit, mem);
        }
        if ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
            (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU)) {
            SOC_ESM_LOCK(unit);
        }
    }
#endif
    SOC_MEM_BLOCK_ITER(unit, mem, blk) {
        if (copyno != COPYNO_ALL && copyno != blk) {
            continue;
        }

        index2 = index;

    
#if defined(BCM_TRIDENT2_SUPPORT)
        if (SOC_IS_TD2_TT2(unit) &&
            soc_feature(unit, soc_feature_l3_defip_map) &&
            (mem == L3_DEFIPm ||
             mem == L3_DEFIP_ONLYm ||
             mem == L3_DEFIP_DATA_ONLYm ||
             mem == L3_DEFIP_HIT_ONLY_Xm ||
             mem == L3_DEFIP_HIT_ONLY_Ym ||
             mem == L3_DEFIP_HIT_ONLYm ||
             mem == L3_DEFIP_PAIR_128m ||
             mem == L3_DEFIP_PAIR_128_ONLYm ||
             mem == L3_DEFIP_PAIR_128_DATA_ONLYm ||
             mem == L3_DEFIP_PAIR_128_HIT_ONLY_Xm ||
             mem == L3_DEFIP_PAIR_128_HIT_ONLY_Ym ||
             mem == L3_DEFIP_PAIR_128_HIT_ONLYm)) {
            if (!(flags & SOC_MEM_DONT_MAP_INDEX)) {
                index2 = soc_trident2_l3_defip_index_map(unit, mem, index);
            }
        }
#endif /* BCM_TRIDENT2_SUPPORT */
        maddr = soc_mem_addr_get(unit, mem, 0, blk, index2, &at);
#if defined(BCM_EXTND_SBUS_SUPPORT)
        if (!soc_feature(unit, soc_feature_new_sbus_format)) {
            /* Override ACC_TYPE in address */
            maddr &= ~(_SOC_MEM_ADDR_ACC_TYPE_MASK <<
                            _SOC_MEM_ADDR_ACC_TYPE_SHIFT);
            maddr |= (acc_type & _SOC_MEM_ADDR_ACC_TYPE_MASK) <<
                            _SOC_MEM_ADDR_ACC_TYPE_SHIFT;
        }
#endif /* BCM_EXTND_SBUS_SUPPORT */
        schan_msg.writecmd.address = maddr;
        dst_blk = 0;
#if defined(BCM_EXTND_SBUS_SUPPORT)
        if (soc_feature(unit, soc_feature_new_sbus_format)) {
            dst_blk = SOC_BLOCK2SCH(unit, blk);
        } else
#endif /* BCM_EXTND_SBUS_SUPPORT */
        {
#if defined(BCM_XGS3_SWITCH_SUPPORT)
            /* required on XGS3. Optional on other devices */
            dst_blk = ((maddr >> SOC_BLOCK_BP)
                 & 0xf) | (((maddr >> SOC_BLOCK_MSB_BP) & 0x3) << 4);
#endif /* BCM_XGS3_SWITCH_SUPPORT */
        }

        soc_schan_header_cmd_set(unit, &schan_msg.header, WRITE_MEMORY_CMD_MSG,
                                 dst_blk, src_blk, acc_type, data_byte_len, 0,
                                 0);

        /* Write header + address + entry_dw data DWORDs */
        /* Note: The hardware does not send WRITE_MEMORY_ACK_MSG. */
        if (2 + entry_dw > CMIC_SCHAN_WORDS(unit)) {
           LOG_WARN(BSL_LS_SOC_SOCMEM,
                    (BSL_META_U(unit,
                                "soc_mem_pipe_select_write: assert will fail for memory %s\n"), SOC_MEM_NAME(unit, mem)));
        }
#ifdef BCM_ESW_SUPPORT
_retry_op:
#endif /* BCM_ESW_SUPPORT */
        rv = soc_schan_op(unit, &schan_msg,
                           2 + entry_dw, 0, allow_intr);
        if (SOC_E_FAIL == rv) {
#ifdef BCM_ESW_SUPPORT
            if (soc_feature(unit, soc_feature_shared_hash_mem)) {

                if (0 == write_retry_attempt) {
                    _soc_ser_sram_correction_init(unit, mem, &need_retry);
                }
                _soc_ser_sram_correction_pre(unit, mem, need_retry);

                if (need_retry){
                    rv2 = soc_schan_op(unit, &schan_msg, 2 + entry_dw, 0, allow_intr);
                }
                /* If rv2==SOC_E_FAIL, the failure of soc_schan_op is not due to L2_mode_fifo is filled,
                * because we diable memwr fifo events, so we need to correct the memory.
                */
                if (rv2 == SOC_E_FAIL || need_retry == 0) {
                    if (SOC_IS_TOMAHAWKX(unit)) {
                        pipe = (acc_type == _SOC_MEM_ADDR_ACC_TYPE_PIPE_0) ? 0 :
                               (acc_type == _SOC_MEM_ADDR_ACC_TYPE_PIPE_1) ? 1 :
                               (acc_type == _SOC_MEM_ADDR_ACC_TYPE_PIPE_2) ? 2 :
                                                                             3;
                    } else if (SOC_IS_TRIDENT2X(unit)) {
                        pipe = (acc_type == _SOC_MEM_ADDR_ACC_TYPE_PIPE_X) ? 0 :
                                                                             1;
                    } else {
                        pipe = -1;
                    }
                    rv1 = soc_ser_sram_correction(unit, pipe,
                                                  schan_msg.writecmd.header.v2.dst_blk,
                                                  schan_msg.writecmd.address, mem,
                                                  blk, index2, NULL);
                }
                _soc_ser_sram_correction_post(unit, mem, need_retry);

                if (rv1 == SOC_E_NONE) {
                    if (write_retry_attempt < HASH_MEM_OP_RETRY_COUNT) {
                        write_retry_attempt++;
                        goto _retry_op;
                    }
                }
            }
#endif /* BCM_ESW_SUPPORT */
            goto done;
        }

        /* Write back to cache if active */
        cache = SOC_MEM_STATE(unit, mem).cache[blk];
        vmap = SOC_MEM_STATE(unit, mem).vmap[blk];

        if (cache != NULL && !no_cache && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
            if (SOC_WARM_BOOT(unit) || SOC_IS_RELOADING(unit) ||
                SOC_HW_ACCESS_DISABLE(unit)) {
                if (!SOC_WARM_BOOT(unit)) {
                    CACHE_VMAP_CLR(vmap, index);
                }
            } else if (_SOC_MEM_CHK_L2_MEM(mem)) {
                /* If an invalid entry is being written then clear the cache */
                if (((mem == L2_ENTRY_2m &&
                     soc_L2_ENTRY_2m_field32_get(unit, entry_data, VALID_0f) &&
                     soc_L2_ENTRY_2m_field32_get(unit, entry_data, VALID_1f)) ||
                     ((mem == L2Xm || mem == L2_ENTRY_1m) &&
                      soc_mem_field32_get(unit, mem, entry_data, VALIDf))) &&
                     ((mem == L2_ENTRY_2m &&
                      (soc_mem_field32_get(unit, mem, entry_data, STATIC_BIT_0f) &&
                       soc_mem_field32_get(unit, mem, entry_data, STATIC_BIT_1f))) ||
                       ((mem == L2Xm || mem == L2_ENTRY_1m) &&
                        soc_mem_field32_get(unit, mem, entry_data, STATIC_BITf)))) {
#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT)
                    sal_memcpy(cache + index * entry_dw,
                               (entry_data_ptr == converted_entry_data) ?
                               cache_entry_data : entry_data, entry_dw * 4);
#else
                    sal_memcpy(cache + index * entry_dw, entry_data, entry_dw * 4);
#endif /* BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT */
                    CACHE_VMAP_SET(vmap, index);
                } else {
                    CACHE_VMAP_CLR(vmap, index);
                }
                if (mem == L2_ENTRY_1m) {
                    vmap = SOC_MEM_STATE(unit, L2_ENTRY_2m).vmap[blk];
                    CACHE_VMAP_CLR(vmap, index/2);
                } else if (mem == L2_ENTRY_2m) {
                    vmap = SOC_MEM_STATE(unit, L2_ENTRY_1m).vmap[blk];
                    CACHE_VMAP_CLR(vmap, index*2);
                    CACHE_VMAP_CLR(vmap, index*2 + 1);
                }
            } else {
#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT)
                sal_memcpy(cache + index * entry_dw,
                           (entry_data_ptr == converted_entry_data) ?
                           cache_entry_data : entry_data, entry_dw * 4);
#else
                sal_memcpy(cache + index * entry_dw, entry_data, entry_dw * 4);
#endif /* BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT */
                CACHE_VMAP_SET(vmap, index);
            }
        }
    }

done:
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_esm_correction)) {
        if (copyno == MEM_BLOCK_ANY) {
            copyno = SOC_MEM_BLOCK_ANY(unit, mem);
        }
        if ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
            (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU)) {
            SOC_ESM_UNLOCK(unit);
        }
    }
#endif
    MEM_UNLOCK(unit, mem);
    if (NULL != meminfo->snoop_cb &&
        (SOC_MEM_SNOOP_WRITE & meminfo->snoop_flags)) {
        meminfo->snoop_cb(unit, mem, SOC_MEM_SNOOP_WRITE, copyno, index, index,
                          entry_data_ptr, meminfo->snoop_user_data);
    }

    return rv;
}

#ifdef BCM_TRIUMPH2_SUPPORT
STATIC int
_soc_mem_op_cpu_tdm(int unit, int enable)
{
    if (enable) {
        soc_IARB_TDM_TABLEm_field32_set(unit,
                                       &(SOC_CONTROL(unit)->iarb_tdm),
                                       PORT_NUMf, 0);
    } else {
        soc_IARB_TDM_TABLEm_field32_set(unit,
                                       &(SOC_CONTROL(unit)->iarb_tdm),
                                       PORT_NUMf, 63);
    }
    return _soc_mem_write(unit, IARB_TDM_TABLEm, 0, SOC_BLOCK_ALL,
                          SOC_CONTROL(unit)->iarb_tdm_idx,
                          &(SOC_CONTROL(unit)->iarb_tdm));
}
#endif /* BCM_TRIUMPH2_SUPPORT */

#ifdef BCM_HURRICANE2_SUPPORT
STATIC int
_soc_mem_hr2_parity_generate(int unit, soc_mem_t mem, void *entry_data)
{
    uint8   *data8_buf, data8_val;
    int     num_bytes, i;
    uint32   parity, rval;

    SOC_IF_ERROR_RETURN(READ_MISCCONFIGr(unit, &rval));
    if (soc_reg_field_get(unit, MISCCONFIGr, rval, PARITY_CHECK_ENf) == 0) {
        return SOC_E_NONE;
    }
    if ((mem == MMU_IPMC_VLAN_TBLm) || (mem == MMU_IPMC_GROUP_TBL0m)
        || (mem == MMU_IPMC_GROUP_TBL1m)) {
        data8_buf = (uint8 *)entry_data;
        num_bytes = soc_mem_entry_bytes(unit,mem);

        /* Reset the parity value */
        parity = 0;
        soc_mem_field32_set(unit, mem, entry_data, PARITYf, parity);

        /* Calculate the parity value */
        for (i = 0; i < num_bytes; i++) {
            data8_val = data8_buf[i];
            while (data8_val > 0) {
                if (data8_val & 0x1) {
                    parity++;
                }
                data8_val = data8_val >> 1;
            }
        }

        /* Set parity value */
        parity &= 0x1;
        soc_mem_field32_set(unit, mem, entry_data, PARITYf, parity);
    }
    return SOC_E_NONE;
}
#endif /* BCM_HURRICANE2_SUPPORT */

STATIC int
_soc_mem_write_kt2(int unit, soc_mem_t mem, int copyno, int index,
                   void *entry_data)
{
    int rv = SOC_E_FAIL;

#ifdef BCM_KATANA2_SUPPORT
    uint32 rval;
    uint16 dev_id;
    uint8 rev_id;

    soc_cm_get_id(unit, &dev_id, &rev_id);
    if ((rev_id == BCM56450_B1_REV_ID) &&
        (SOC_CONTROL(unit)->soc_flags & SOC_F_ALL_MODULES_INITED) &&
        (mem == ING_PHYSICAL_PORT_TABLEm)) {

        READ_ING_Q_BEGINr(unit,&rval);
        soc_reg_field_set(unit, ING_Q_BEGINr, &rval,
                          ING_PHYSICAL_PORT_SBUS_WITH_PKT_DISABLEf , 1);
        WRITE_ING_Q_BEGINr(unit,rval);
        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit, "WRITE PKT_DISABLE=1\n")));

        rv = soc_mem_array_write(unit, mem, 0, copyno, index, entry_data);
        soc_reg_field_set(unit, ING_Q_BEGINr, &rval,
                          ING_PHYSICAL_PORT_SBUS_WITH_PKT_DISABLEf , 0);
        WRITE_ING_Q_BEGINr(unit,rval);
        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit, "WRITE PKT_DISABLE=0\n")));
    } else {
        return soc_mem_array_write(unit, mem, 0, copyno, index, entry_data);
    }
#endif

    return rv;
}

void
soc_mem_watch_set(int unit, int value)
{
    SOC_CONTROL(unit)->soc_mem_watch = value;
    LOG_CLI((BSL_META("memwatch delta %s\n"),
             value ? "on" : "off"));
}

/*
 * Function:
 *    soc_mem_write
 * Purpose:
 *    Write a memory internal to the SOC.
 * Notes:
 *    GBP/CBP memory should only accessed when MMU is in DEBUG mode.
 */
int
soc_mem_write(int unit,
              soc_mem_t mem,
              int copyno,  /* Use COPYNO_ALL for all */
              int index,
              void *entry_data)
{
    int rv;

    if (SOC_CONTROL(unit)->soc_mem_watch) {
        if (SOC_CONTROL(unit)->prev_mem != mem) {
            LOG_CLI((BSL_META_U(unit, "%d:%s MEM: %s[%d]\n"),
                     unit, BSL_FUNC, SOC_MEM_NAME(unit, mem),
                     index));
            SOC_CONTROL(unit)->prev_mem = mem;
        }
    }

    _SOC_MEM_REPLACE_MEM(unit, mem);

    if (SOC_IS_KATANA2(unit)) {
        rv = _soc_mem_write_kt2(unit, mem, copyno, index, entry_data);
    } else {
        rv = soc_mem_array_write(unit, mem, 0, copyno, index, entry_data);
    }

    return rv;
}

int
soc_mem_write_extended(int unit, uint32 flags,
                         soc_mem_t mem,
                         int copyno, /* COPYNO_ALL allowed */
                         int index,
                         void *entry_data)
{
    int rv;

    _SOC_MEM_REPLACE_MEM(unit, mem);
    if (SOC_IS_KATANA2(unit)) {
        rv = _soc_mem_write_kt2(unit, mem, copyno, index, entry_data);
    } else {
        rv = soc_mem_array_write_extended(unit, flags, mem, 0, copyno,
                                            index, entry_data);
    }

    return rv;
}

/*
 * Function:    soc_mem_array_write
 * Purpose:     Write a memory array internal to the SOC.
 */
int
soc_mem_array_write(int unit, soc_mem_t mem, unsigned array_index, int copyno,/* Use COPYNO_ALL for all */
              int index, void *entry_data)
{
    return soc_mem_array_write_extended(unit, SOC_MEM_NO_FLAGS, mem,
              array_index, copyno, index, entry_data);
}

int
soc_mem_array_write_extended(int unit, uint32 flags,
              soc_mem_t mem, unsigned array_index,
              int copyno,/* Use COPYNO_ALL for all */
              int index, void *entry_data)
{
    int rv;
#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) \
    || defined(BCM_HURRICANE2_SUPPORT)
    soc_mem_t     aggr_mem = INVALIDm;
    void          *shift_buffer[MAX_TCAM_PROTECT_RANGES] = {NULL};
#endif
#ifdef BCM_HURRICANE2_SUPPORT
    uint32 try_cnt = 0, val = 0;
    int     entry_len = 0, skip_cache = 0;
    void    *entry_tmp = NULL; /* the copy for written entry */
    void    *gp_entry_chk = NULL; /* for read check */
    void    *xp_entry_chk = NULL; /* for read check */
#endif /* BCM_HURRICANE2_SUPPORT */
#ifdef BCM_PETRA_SUPPORT
    int       entry_dw;
#endif

    /* Use user defined callback for access */
    if(SOC_INFO(unit).reg_access.mem_write) {
        return SOC_INFO(unit).reg_access.mem_write(unit, mem, array_index, copyno, index, entry_data);
    }

    /* if reloading, don't write to register */
    if (SOC_IS_RELOADING(unit)){
        return SOC_E_NONE;
    }

#ifdef BCM_PETRA_SUPPORT
    entry_dw = soc_mem_entry_words(unit, mem);

    /* Write header + address + entry_dw data DWORDs */
    if (SOC_IS_DPP(unit) && (entry_dw + 2 > CMIC_SCHAN_WORDS(unit))) {
        if (SOC_IS_ARADPLUS_AND_BELOW(unit)) {
            SOC_DPP_ALLOW_WARMBOOT_WRITE(_soc_arad_mem_array_wide_access(unit, flags, mem, array_index, copyno, index, entry_data, 0), rv);
            return rv;
        } else {
            SOC_DPP_ALLOW_WARMBOOT_WRITE(dcmn_mem_array_wide_access(unit, flags, mem, array_index, copyno, index, entry_data, 0), rv);
            return rv;
        }
    }
#endif

#ifdef BCM_TRIUMPH2_SUPPORT
    if (SOC_IS_TRIUMPH2(unit) || SOC_IS_APOLLO(unit) ||
        SOC_IS_VALKYRIE2(unit) || SOC_IS_ENDURO(unit)) {
        if ((mem == LMEPm) || (mem == LMEP_1m)) {
            /* Disable CPU slot from TDM */
            SOC_IF_ERROR_RETURN(_soc_mem_op_cpu_tdm(unit, 0));
        }
    }
#endif /* BCM_TRIUMPH2_SUPPORT */

#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) \
    || defined(BCM_HURRICANE2_SUPPORT)
    if (soc_feature(unit, soc_feature_xy_tcam) &&
        SOC_CONTROL(unit)->tcam_protect_write &&
        (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) &&
        (!(SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_EXT_CAM))) {
        SOC_IF_ERROR_RETURN
            (_soc_mem_tcam_entry_preserve(unit, mem, copyno, index, 1,
                                          entry_data, &aggr_mem,
                                          shift_buffer));
    }
#endif /* BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT */

#ifdef BCM_HURRICANE2_SUPPORT
    if (SOC_IS_HURRICANE2(unit)) {
        SOC_IF_ERROR_RETURN
            (_soc_mem_hr2_parity_generate(unit, mem, entry_data));
    }
    if (SOC_IS_HURRICANE2(unit) && mem == MMU_IPMC_VLAN_TBLm) {
        entry_len = soc_mem_entry_words(unit, mem) * sizeof(uint32);
        entry_tmp = sal_alloc(entry_len, "ipmc_vlan_entry_copy");
        gp_entry_chk = sal_alloc(entry_len, "ipmc_vlan_gpentry_check");
        xp_entry_chk = sal_alloc(entry_len, "ipmc_vlan_xpentry_check");
        if (entry_tmp == NULL || gp_entry_chk == NULL ||
                xp_entry_chk == NULL) {
            rv = SOC_E_MEMORY;
            goto hr2_war_done;
        }
        sal_memcpy(entry_tmp, entry_data, entry_len);
        sal_memset(gp_entry_chk, 0, entry_len);
        sal_memset(xp_entry_chk, 0, entry_len);
    }
#endif  /* BCM_HURRICANE2_SUPPORT */

    rv = _soc_mem_write_extended(unit, flags,mem, array_index, copyno, index,
                                 entry_data);
#ifdef BCM_HURRICANE2_SUPPORT
    /* SDK-50996: HR2 WAR for sbus issue while writing to IPMC_VLAN */
    if (SOC_IS_HURRICANE2(unit) && mem == MMU_IPMC_VLAN_TBLm) {

        /* Skip cache */
        skip_cache = SOC_MEM_TEST_SKIP_CACHE(unit);
        SOC_MEM_TEST_SKIP_CACHE_SET(unit, TRUE);

        do {
            if (++try_cnt > 10) {
                rv = SOC_E_INTERNAL;
                goto hr2_war_done;
            }
            if (try_cnt > 1){
                sal_memcpy(entry_data, entry_tmp, entry_len);
                if ((rv = _soc_mem_write_extended(unit, flags, mem,
                        array_index, copyno, index, entry_data)) < 0) {
                    goto hr2_war_done;
                }
            }

            /* Force the read on XPORT's IPMC_VLANm */
            if ((rv = READ_MISCCONFIGr(unit, &val)) < 0) {
                goto hr2_war_done;
            }
            soc_reg_field_set(unit, MISCCONFIGr, &val,
                              IPMC_VLAN_TBL_RD_SELf, 1);
            if ((rv = WRITE_MISCCONFIGr(unit, val)) < 0) {
                goto hr2_war_done;
            }
            sal_memset(xp_entry_chk, 0, entry_len);
            if ((rv = soc_mem_array_read(unit, mem,
                    array_index, copyno, index, xp_entry_chk)) < 0) {
                goto hr2_war_done;
            }

            /* Force the read on GPORT's IPMC_VLANm */
            if ((rv = READ_MISCCONFIGr(unit, &val)) < 0) {
                goto hr2_war_done;
            }
            soc_reg_field_set(unit, MISCCONFIGr, &val,
                              IPMC_VLAN_TBL_RD_SELf, 0);
            if ((rv = WRITE_MISCCONFIGr(unit, val)) < 0) {
                goto hr2_war_done;
            }
            sal_memset(gp_entry_chk, 0, entry_len);
            if ((rv = soc_mem_array_read(unit, mem,
                    array_index, copyno, index, gp_entry_chk)) < 0) {
                goto hr2_war_done;
            }
        } while(sal_memcmp(gp_entry_chk, entry_tmp, entry_len) ||
                sal_memcmp(xp_entry_chk, entry_tmp, entry_len));

        /* free the allocated memory */
hr2_war_done :
        if (entry_tmp) {
            sal_free(entry_tmp);
        }
        if (gp_entry_chk) {
            sal_free(gp_entry_chk);
        }
        if (xp_entry_chk) {
            sal_free(xp_entry_chk);
        }

        SOC_MEM_TEST_SKIP_CACHE_SET(unit, skip_cache);
        /* return once the WAR got error return */
        if (SOC_FAILURE(rv)){
            return rv;
        }
    }
#endif  /* BCM_HURRICANE2_SUPPORT */

#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) \
    || defined(BCM_HURRICANE2_SUPPORT)
    if (soc_feature(unit, soc_feature_xy_tcam) &&
        SOC_CONTROL(unit)->tcam_protect_write &&
        (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) &&
        (!(SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_EXT_CAM))) {
            SOC_IF_ERROR_RETURN
                (_soc_mem_tcam_entry_restore(unit, aggr_mem, copyno, index, 1,
                                             shift_buffer));
    }
#endif /* BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT */

    SOC_IF_ERROR_RETURN(rv);

#ifdef BCM_TRIUMPH2_SUPPORT
    if (SOC_IS_TRIUMPH2(unit) || SOC_IS_APOLLO(unit) ||
        SOC_IS_VALKYRIE2(unit) || SOC_IS_ENDURO(unit)) {
        if ((mem == LMEPm) || (mem == LMEP_1m)) {
            /* Enable CPU slot from TDM */
            SOC_IF_ERROR_RETURN(_soc_mem_op_cpu_tdm(unit, 1));
        }
    }
#endif /* BCM_TRIUMPH2_SUPPORT */

    return rv;
}

/*
 * Function:
 *    soc_mem_write_range
 * Purpose:
 *    Write a range of chip's memory with multiple entries
 */
int
soc_mem_write_range(int unit, soc_mem_t mem, int copyno,
                    int index_min, int index_max, void *buffer)
{
    int rc;

    rc = soc_mem_write_range_multi_cmc(unit, mem, copyno,
                                       index_min, index_max, buffer, -1);
    return rc;
}

/*
 * Function:
 *    soc_mem_write_range_multi_cmc
 * Purpose:
 *    Write a range of chip's memory with multiple entries
 *    With multiple CMC/channel support
 */
int
soc_mem_write_range_multi_cmc(int unit, soc_mem_t mem, int copyno,
                              int index_min, int index_max, void *buffer,
                              int vchan)
{
    int rc;

#ifdef SOC_MEM_DEBUG_SPEED

    sal_usecs_t start_time;
    int diff_time, count = (index_max > index_min) ?
                           (index_max - index_min + 1) :
                           (index_min - index_max + 1);

    start_time = sal_time_usecs();
#endif

    rc = soc_mem_array_write_range_multi_cmc(unit, 0, mem, 0, copyno, index_min,
                                            index_max, buffer, vchan);

#ifdef SOC_MEM_DEBUG_SPEED
    diff_time = SAL_USECS_SUB(sal_time_usecs(), start_time);
    LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
                (BSL_META_U(unit,
                            "Total dma write time: %d usecs, [%d nsecs per op]\n"),
                 diff_time, diff_time*1000/count));
#endif
 return rc;
}

/*
 * Function:
 *    soc_mem_array_write_range
 * Purpose:
 *    Write a range of chip's memory with multiple entries
 *
 */

int
soc_mem_array_write_range(int unit, uint32 flags, soc_mem_t mem, unsigned array_index,
                          int copyno, int index_min, int index_max, void *buffer)
{
    int rv;
    rv = soc_mem_array_write_range_multi_cmc(unit, flags, mem, array_index,
                                             copyno, index_min, index_max, buffer, -1);
    return rv;
}

/*
 * Function:
 *    soc_mem_array_write_range_multi_cmc
 * Purpose:
 *    Write a range of chip's memory with multiple entries
 *
 */

int
soc_mem_array_write_range_multi_cmc(int unit, uint32 flags, soc_mem_t mem, unsigned array_index,
                                    int copyno, int index_min, int index_max, void *buffer,
                                    int vchan)
{
    int                   rv = SOC_E_NONE;
    uint32          entry_dw;
    int mem_array_vmap_offset, mem_array_cache_offset;
    int             i;
    soc_mem_info_t  *meminfo;
    void            *cache_buffer = NULL;
    int             blk, cache_copyno = copyno;
#ifdef BROADCOM_DEBUG
    shared_block_t *p = NULL;
    int            length = 0;
#endif
#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) \
    || defined(BCM_HURRICANE2_SUPPORT)

    int             count, alloc_size;
#endif
    uint32          *cache;

    /* if reloading, don't write to register
       and write to H/W is not allowed during Warm boot */
    if (SOC_IS_RELOADING(unit) || SOC_WARM_BOOT(unit))
    {
        return SOC_E_NONE;
    }

    SOC_MEM_ALIAS_TO_ORIG(unit,mem);

    if (!soc_mem_is_valid(unit, mem)) {
        return SOC_E_MEMORY;
    }
#ifdef BCM_PETRA_SUPPORT
    if (SOC_IS_ARAD(unit)){
        int copyno_override;

        copyno_override = soc_arad_mem_broadcast_block_get(unit,mem);
        if ((copyno == COPYNO_ALL || copyno == SOC_CORE_ALL) && copyno_override){
            copyno = copyno_override;
        }
        if (cache_copyno == copyno_override) {
            cache_copyno = COPYNO_ALL;
        }
    }
#endif
    if (copyno == COPYNO_ALL) {
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }
    if (copyno == COPYNO_ALL) {
        return SOC_E_INTERNAL;
    }
#ifdef BROADCOM_DEBUG
    if (soc_dma_debug_property_get(unit)) {
        p = (shared_block_t *) (((char*)buffer) -
            ( (((char*)&(((shared_block_t*)0)->user_data[0]))) - ((char*)(shared_block_t*)0) ));
        if (soc_cm_shared_good_range(unit, p)) {
            if (SHARED_GOOD_START(p)  && SHARED_GOOD_END_DEBUG(p)) {
                length = WORDS2BYTES(soc_mem_entry_words(unit, mem)) *
                         (index_max - index_min + 1);
                if (length > p->size) {
                    LOG_WARN(BSL_LS_SOC_SOCMEM,
                             (BSL_META_U(unit,
                                         "Suspicious DMA length: "
                                         "Desc:%s: Size:%u: length:%u\n"),
                              p->description, p->size, length));
                }
            }
        } else if (soc_mem_dmaable(unit, mem, copyno)) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "ERROR:ATTN: Address:%p:"
                                  "probably not in shared memory region \n"),
                       p));
        }
    }
#endif

    meminfo = &SOC_MEM_INFO(unit, mem);
    entry_dw = soc_mem_entry_words(unit, mem);
    mem_array_vmap_offset = array_index * soc_mem_index_count(unit, mem);
    mem_array_cache_offset = mem_array_vmap_offset * entry_dw;

    LOG_INFO(BSL_LS_SOC_SOCMEM,
             (BSL_META_U(unit,
                         "soc_mem_array_write_range: unit %d memory %s.%s [%d:%d]\n"),
              unit, SOC_MEM_UFNAME(unit, mem),
              SOC_BLOCK_NAME(unit, copyno),
              index_min, index_max));

    /*    coverity[negative_returns : FALSE]    */
    if (soc_mem_slamable(unit, mem, copyno)) {
        cache = SOC_MEM_STATE(unit, mem).cache[copyno];
#if defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT) || defined(BCM_HURRICANE2_SUPPORT)
        if (cache != NULL && !SOC_MEM_TEST_SKIP_CACHE(unit)
            && (soc_feature(unit, soc_feature_xy_tcam) &&
            (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) &&
            (!(SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_EXT_CAM)))) {
            if (index_min > index_max) {
                count = index_min - index_max + 1;
            } else {
                count = index_max - index_min + 1;
            }
            alloc_size = count * entry_dw * sizeof(uint32);
            cache_buffer = sal_alloc(alloc_size, "cache buffer");
            if (cache_buffer == NULL) {
                return SOC_E_MEMORY;
            }
        }
#endif

        MEM_LOCK(unit, mem);

#ifdef BCM_PETRA_SUPPORT
        /* Write header + address + entry_dw data DWORDs */
        if (!(entry_dw + 2 > CMIC_SCHAN_WORDS(unit)))
#endif
        {
            rv = _soc_mem_dma_write(unit, flags, mem, array_index, copyno, index_min,
                                    index_max, buffer, cache_buffer, vchan);
        }

        if (rv >= 0) {
            uint8 *vmap, *vmap1 = NULL;

            SOC_MEM_BLOCK_ITER(unit, mem, blk) {
                if ((cache_copyno != COPYNO_ALL) && (cache_copyno != blk)) {
                    continue;
                }
                cache = SOC_MEM_STATE(unit, mem).cache[blk];
                vmap = SOC_MEM_STATE(unit, mem).vmap[blk];
                if (_SOC_MEM_CHK_L2_MEM(mem)) {
                    if (mem == L2_ENTRY_1m) {
                        vmap1 = SOC_MEM_STATE(unit, L2_ENTRY_2m).vmap[copyno];
                    } else if (mem == L2_ENTRY_2m) {
                        vmap1 = SOC_MEM_STATE(unit, L2_ENTRY_1m).vmap[copyno];
                    }
                }
                if (cache != NULL && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
                    sal_memcpy(cache + mem_array_cache_offset + index_min * entry_dw, cache_buffer ?
                               cache_buffer : buffer,
                               (index_max - index_min + 1) * entry_dw * 4);
                    for (i = index_min; i <= index_max; i++) {
                        if (vmap1) {
                            CACHE_VMAP_CLR(vmap, i);
                            if (mem == L2_ENTRY_1m) {
                                CACHE_VMAP_CLR(vmap1, i/2);
                            } else {
                                CACHE_VMAP_CLR(vmap1, i*2);
                                CACHE_VMAP_CLR(vmap1, i*2 + 1);
                            }
                        } else {
                            CACHE_VMAP_SET(vmap, mem_array_vmap_offset + i);
                        }
                    }
               }
           }
 
#ifdef BCM_TRX_SUPPORT
           _soc_mem_aggr_cache_update(unit, mem, copyno, 0,
                    index_min, index_max, array_index, buffer);
#endif /* BCM_TRX_SUPPORT */

        }
        MEM_UNLOCK(unit, mem);
        if (NULL != meminfo->snoop_cb &&
            (SOC_MEM_SNOOP_WRITE & meminfo->snoop_flags)) {
            meminfo->snoop_cb(unit, mem, SOC_MEM_SNOOP_WRITE, copyno, index_min,
                              index_max, buffer, meminfo->snoop_user_data);
        }
        if (cache_buffer) {
            sal_free(cache_buffer);
        }
        return rv;
    }

    for (i = index_min; i <= index_max; i++) {
        SOC_IF_ERROR_RETURN(soc_mem_array_write(unit, mem, array_index, copyno,
                                                i, buffer));
        buffer = ((uint32 *)buffer + entry_dw);
    }
    return SOC_E_NONE;
}




/*
 *    Efficiently write a range of chip's memory with the same memory entry.
 *    When SLAM DMA is available, it is used, and the buffer must be allocated using soc_cm_salloc().
 */
int soc_mem_array_fill_range(
    int unit,              /* unit of the memory */
    uint32 flags,          /* flags */
    soc_mem_t mem,         /* Memory to be written to */
    unsigned min_ar_index, /* min array index to be written to, not used in memories which are not arrays */
    unsigned max_ar_index, /* max array index to be written to, not used in memories which are not arrays */
    int copyno,            /* Memory block to write to */
    int index_min,         /* first memory index to write to */
    int index_max,         /* last memory index to write to */
    const void *buffer)    /* buffer of the entry to write. If the memory can be written to by SLAM DMA,
                              then the buffer must be allocated using soc_cm_salloc(). */
{
    uint32          entry_dw;
    int             mem_array_vmap_offset, mem_array_cache_offset;
    int             i, nof_entries_to_write, tmp;
    unsigned        a_ind;
    soc_mem_info_t  *meminfo;
    uint32          *cache, *cache_cp;
    int             blk, cache_copyno = copyno;

    /* if reloading, don't write to register */
    if (SOC_IS_RELOADING(unit) || SOC_HW_ACCESS_DISABLE(unit))
    {
        return SOC_E_NONE;
    }

    SOC_MEM_ALIAS_TO_ORIG(unit,mem);

    if (!soc_mem_is_valid(unit, mem)) {
        return SOC_E_MEMORY;
    }

    meminfo = &SOC_MEM_INFO(unit, mem);
    entry_dw = soc_mem_entry_words(unit, mem);

    /* get legal values for indices, if too small/big use the memory's boundaries */
    tmp = soc_mem_index_min(unit, mem);
    if (index_min < tmp) {
      index_min = tmp;
    }
    if (index_max < index_min) {
        index_max = index_min;
    } else {
         tmp = soc_mem_index_max(unit, mem);
         if (index_max > tmp) {
             index_max = tmp;
         }
    }

    if (SOC_MEM_IS_ARRAY(unit, mem)) {
        soc_mem_array_info_t *maip = SOC_MEM_ARRAY_INFOP(unit, mem);
        if (maip) {
            if (max_ar_index >= maip->numels) {
                max_ar_index = maip->numels - 1;
            }
        } else {
            max_ar_index = 0;
        }
        if (min_ar_index > max_ar_index) {
            min_ar_index = max_ar_index;
        }
    } else {
        min_ar_index = max_ar_index = 0;
    }

    nof_entries_to_write = index_max - index_min + 1;

#ifdef BCM_PETRA_SUPPORT
    if (SOC_IS_ARAD(unit)){
        int copyno_override;

        copyno_override = soc_arad_mem_broadcast_block_get(unit,mem);
        if ((copyno == COPYNO_ALL || copyno == SOC_CORE_ALL) && copyno_override){
            copyno = copyno_override;
        }
        if (cache_copyno == copyno_override) {
            cache_copyno = COPYNO_ALL;
        }
    }
#endif
    if (copyno == COPYNO_ALL) {
        SOC_MEM_BLOCK_ITER(unit, mem, blk) {
            copyno = blk;
            break;
        }
    }
    if (copyno == COPYNO_ALL) {
        return SOC_E_INTERNAL;
    }

    LOG_INFO(BSL_LS_SOC_SOCMEM,
             (BSL_META_U(unit,
                         "memory %s[%u:%u].%s [%d:%d]\n"),
              SOC_MEM_UFNAME(unit, mem),
              min_ar_index, max_ar_index, SOC_BLOCK_NAME(unit, copyno), index_min, index_max));

    /*    coverity[negative_returns : FALSE]    */
    if (soc_mem_slamable(unit, mem, copyno)) {
        int rv = SOC_E_NONE;

#ifdef BROADCOM_DEBUG
    if (soc_dma_debug_property_get(unit)) {
        shared_block_t *p = (shared_block_t *) (((char*)buffer) -
            ( (((char*)&(((shared_block_t*)0)->user_data[0]))) - ((char*)(shared_block_t*)0) ));
        int            length = 0;
        if (soc_cm_shared_good_range(unit, p)) {
            if (SHARED_GOOD_START(p)  && SHARED_GOOD_END_DEBUG(p)) {
                length = WORDS2BYTES(soc_mem_entry_words(unit, mem)) *
                         (index_max - index_min + 1);
                if (length > p->size) {
                    LOG_WARN(BSL_LS_SOC_SOCMEM,
                             (BSL_META_U(unit,
                                         "Some problem in calling: Desc:%s: Size:%u: length:%d\n"),
                              p->description, p->size, soc_mem_entry_bytes(unit, mem)));
                }
            }
        } else {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "ERROR:ATTN: Address:%p: probably not in shared memory region\n"),
                       p));
        }
	}
#endif

        MEM_LOCK(unit, mem);

#ifdef BCM_SBUSDMA_SUPPORT
        if (soc_feature(unit, soc_feature_sbusdma)) {
#ifdef BCM_PETRA_SUPPORT
            /* Write header + address + entry_dw data DWORDs */
            if (!(entry_dw + 2 > CMIC_SCHAN_WORDS(unit)))
#endif
            {
                rv = _soc_mem_array_sbusdma_write(unit, 0, mem, min_ar_index,
                                                  max_ar_index, copyno, index_min,
                                                  index_max, (void*)buffer, TRUE,
                                                  1, -1);
            }
        } else
#endif
        {
            /**
             * the code above is intended for non-slambles dma, 
             * so we write the array table by table 
             *  
             */
            int j;
            int       chunk_size, chunk_entries, mem_size, entry_words;
            int       index, index_end;
            uint32    *buf;

            chunk_size = 2*1024*1024; /* 2MB: max DMA buffer size to allocate */
            entry_words = soc_mem_entry_words(unit, mem);
            mem_size = (index_max - index_min + 1) * entry_words * 4;
            if (mem_size < chunk_size) {
                chunk_size = mem_size;
            }

            buf = soc_cm_salloc(unit, chunk_size, "fill_table_using_table_dma");
            if (buf == NULL) {
                MEM_UNLOCK(unit, mem);
                return SOC_E_MEMORY;
            }

            chunk_entries = chunk_size / (entry_words * 4);

            for (index = 0; index < chunk_entries; index++) {
                sal_memcpy(buf + (index * entry_words),
                           buffer, entry_words * 4);
            }

            rv = SOC_E_NONE;
            for (j=min_ar_index;j<=max_ar_index;j++) {
                for (index = index_min; index <= index_max; index += chunk_entries) {
                    index_end = index + chunk_entries - 1;
                    if (index_end > index_max) {
                        index_end = index_max;
                    }

                    rv = soc_mem_array_write_range(unit, 0, mem, j, copyno, index, index_end, buf);
                    if (rv < 0) {
                        LOG_ERROR(BSL_LS_SOC_SOCMEM, (BSL_META_U(unit, "soc_mem_write_range: " "write %s.%s[%d-%d] failed: %s\n"),
                                                      SOC_MEM_UFNAME(unit, mem), SOC_BLOCK_NAME(unit, copyno), index, index_end, soc_errmsg(rv)));
                        break;
                    }
                }

            }
            soc_cm_sfree(unit, buf);
        }

        if (rv >= 0) {
            uint8 *vmap;
            SOC_MEM_BLOCK_ITER(unit, mem, blk) {
                if ((cache_copyno != COPYNO_ALL) && (cache_copyno != blk)) {
                    continue;
                }

                cache = SOC_MEM_STATE(unit, mem).cache[blk];
 
                /*    coverity[negative_returns : FALSE]    */
                /* coverity[negative_returns] */
                vmap = SOC_MEM_STATE(unit, mem).vmap[blk];
            
                if (cache != NULL && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
                    if (!SOC_MEM_IS_ARRAY(unit, mem)) {
                        min_ar_index = max_ar_index = 0;
                    }
                    for (a_ind = min_ar_index; a_ind <= max_ar_index; ++a_ind) {
                        int bytes_to_copy = entry_dw * 4;

                        mem_array_vmap_offset = a_ind * soc_mem_index_count(unit, mem);
                        mem_array_cache_offset = mem_array_vmap_offset * entry_dw;
                        cache_cp = cache;
                        cache_cp +=  mem_array_cache_offset + (index_min * entry_dw);
                        for (i = 0; i < nof_entries_to_write; i++) { /* update cache */
                            sal_memcpy(cache_cp, buffer, bytes_to_copy);
                            cache_cp += entry_dw;
                        }
                        for (i = index_min; i <= index_max; i++) {
                            CACHE_VMAP_SET(vmap, mem_array_vmap_offset + i);
                        }
                    }
                }
            }
        }
        MEM_UNLOCK(unit, mem);
        if (NULL != meminfo->snoop_cb &&
            (SOC_MEM_SNOOP_WRITE & meminfo->snoop_flags)) {
            meminfo->snoop_cb(unit, mem, SOC_MEM_SNOOP_WRITE, copyno, index_min,
                              index_max, (void*)buffer, meminfo->snoop_user_data);
        }
        return rv;
    }

    /* If SLAM DMA is not supported, fill the table use per entry access */
    for (a_ind = min_ar_index; a_ind <= max_ar_index; ++a_ind) {
        for (i = index_min; i <= index_max; i++) {
            SOC_IF_ERROR_RETURN(soc_mem_array_write(unit, mem, a_ind, copyno, i, (void*)buffer));
        }
    }
    return SOC_E_NONE;
}

/*
 *    Efficiently write to a whole memory with a repeating entry.
 *    When SLAM DMA is available, it is used, and the buffer must be allocated using soc_cm_salloc().
 */

int
soc_mem_fill(
    int unit,           /* unit of the memory */
    soc_mem_t mem,      /* Memory to be written to */
    int copyno,         /* Memory block to write to */
    const void *buffer) /* buffer of the entry to repeatedly write. If the memory can be written to by SLAM DMA,
                           then the buffer must be allocated using soc_cm_salloc(). */
{
    int rc;
    unsigned max_array_index = 0;
#ifdef SOC_MEM_DEBUG_SPEED
    sal_usecs_t start_time = sal_time_usecs();
    int diff_time;
#endif

    if (!soc_mem_is_valid(unit, mem)) {
        return SOC_E_MEMORY;
    }
    if (SOC_MEM_IS_ARRAY(unit, mem)) {
        soc_mem_array_info_t *maip = SOC_MEM_ARRAY_INFOP(unit, mem);
        if (maip) {
            max_array_index = maip->numels - 1;
        }
    }
    rc = soc_mem_array_fill_range(unit, 0, mem, 0, max_array_index, copyno, soc_mem_index_min(unit, mem), soc_mem_index_max(unit, mem), buffer);

#ifdef SOC_MEM_DEBUG_SPEED
    diff_time = SAL_USECS_SUB(sal_time_usecs(), start_time);
    LOG_VERBOSE(BSL_LS_SOC_SOCMEM, (BSL_META_U(unit, "Total dma write time: %d usecs\n"), diff_time));
#endif
 return rc;
}

#ifdef BCM_TRIDENT_SUPPORT
STATIC int _soc_mem_clear_require_pipe_select(int unit, soc_mem_t mem)
{
    if (!(soc_mem_is_valid(unit, mem))) {
        return FALSE;
    } else {
        switch (mem) {
        case L3_ENTRY_HIT_ONLYm:
        case L3_DEFIP_HIT_ONLYm:
        case FP_COUNTER_TABLEm:
        case FP_STORM_CONTROL_METERSm:
        case ING_PW_TERM_SEQ_NUMm:
        case ING_VINTF_COUNTER_TABLEm:
        case ING_SERVICE_COUNTER_TABLEm:
        case TRILL_DROP_STATSm:
        case DLB_HGT_GROUP_CONTROLm:
        case DLB_HGT_GROUP_STATSm:
        case DLB_HGT_FLOWSET_TIMESTAMP_PAGEm:
        case DLB_HGT_OPTIMAL_CANDIDATEm:
        case DLB_HGT_FLOWSET_PORTm:
        case DLB_HGT_FLOWSET_TIMESTAMPm:
        case EGR_FRAGMENT_ID_TABLEm:
        case EFP_METER_TABLEm:
        case EGR_PW_INIT_COUNTERSm:
        case EFP_COUNTER_TABLEm:
        case EGR_PERQ_XMT_COUNTERSm:
        case EGR_VINTF_COUNTER_TABLEm:
        case EGR_SERVICE_COUNTER_TABLEm:
        case EGR_PORT_REQUESTSm:
        case EGR_MMU_REQUESTSm:
        case EGR_MAX_USED_ENTRIESm:
            return TRUE;
        default:
            return FALSE;
        }
    }
}
#endif

/************************************************************************
 * Routines for reading/writing sorted tables except ARL,        *
 * and searching sorted tables including the ARL.            *
 * Sorted tables are:  IRULE, L3, MCAST, PVLAN                *
 ************************************************************************/

#if defined(BCM_TRIDENT2_SUPPORT)
STATIC int
_soc_mem_clear_tcam_protect_clear_restore(int unit,
                                          soc_mem_t mem,
                                          int clear_restore, /* 0:clear, 1:restore*/
                                          int *tcam_protect_write,
                                          int *config_128b_entries)
{
    int qualified =
        soc_feature(unit, soc_feature_l3_defip_map) &&
        (mem == L3_DEFIPm ||
         mem == L3_DEFIP_ONLYm ||
         mem == L3_DEFIP_DATA_ONLYm ||
         mem == L3_DEFIP_HIT_ONLY_Xm ||
         mem == L3_DEFIP_HIT_ONLY_Ym ||
         mem == L3_DEFIP_HIT_ONLYm ||
         mem == L3_DEFIP_PAIR_128m ||
         mem == L3_DEFIP_PAIR_128_ONLYm ||
         mem == L3_DEFIP_PAIR_128_DATA_ONLYm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Xm ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLY_Ym ||
         mem == L3_DEFIP_PAIR_128_HIT_ONLYm);
    if (!qualified) {
        return SOC_E_NONE;
    }

    if (clear_restore == 0) { /* clear */
        *tcam_protect_write = SOC_CONTROL(unit)->tcam_protect_write;
        SOC_CONTROL(unit)->tcam_protect_write = FALSE;
        if (*tcam_protect_write) {
            if (soc_mem_index_count(unit, L3_DEFIPm)) {
                SOP_MEM_STATE(unit, L3_DEFIPm).index_max += 8;
                SOC_CONTROL(unit)->l3_defip_tcam_size++;
            }
            if (soc_mem_index_count(unit, L3_DEFIP_PAIR_128m)) {
                SOP_MEM_STATE(unit, L3_DEFIP_PAIR_128m).index_max += 4;
                SOC_CONTROL(unit)->l3_defip_tcam_size++;
            }
            *config_128b_entries = SOC_L3_DEFIP_MAX_128B_ENTRIES(unit) + 4;

            SOC_CONTROL(unit)->l3_defip_index_remap =
                soc_mem_index_count(unit, L3_DEFIP_PAIR_128m);
            return soc_l3_defip_indexes_init(unit, *config_128b_entries);
        }
    } else {  /* restore */
        SOC_CONTROL(unit)->tcam_protect_write = *tcam_protect_write;
        if (*tcam_protect_write) {
            if (soc_mem_index_count(unit, L3_DEFIPm)) {
                SOP_MEM_STATE(unit, L3_DEFIPm).index_max -= 8;
                SOC_CONTROL(unit)->l3_defip_tcam_size--;
            }
            if (soc_mem_index_count(unit, L3_DEFIP_PAIR_128m)) {
                SOP_MEM_STATE(unit, L3_DEFIP_PAIR_128m).index_max -= 4;
                SOC_CONTROL(unit)->l3_defip_tcam_size--;
            }
            SOC_CONTROL(unit)->l3_defip_index_remap =
                soc_mem_index_count(unit, L3_DEFIP_PAIR_128m);
            *config_128b_entries -= 4;
            return soc_l3_defip_indexes_init(unit, *config_128b_entries);
        }
    }
    return SOC_E_NONE;
}
#endif /* BCM_TRIDENT2_SUPPORT */

/*
 * Function:
 *    soc_mem_clearable_on_reset
 * Purpose:
 *    Determine whether a table can be cleared during resetting
 * Returns:
 *    0 if not, 1 otherwise
 */

int
soc_mem_clearable_on_reset(int unit, soc_mem_t mem, int copyno)
{
    int blk;
#ifdef BCM_TOMAHAWK_SUPPORT
    if (SOC_IS_TOMAHAWKX(unit)) {
        if (soc_mem_is_readonly(unit, mem)) {
            return FALSE;
        }
        switch (mem) {
        /* L3_MTU_VALUESm should always be cleared during initialization.
         * After clearing, the MTU_SIZEf was set to default value (0x3fff).
         * After "init soc;init misc", the MTU_SIZEf was set to zero. 
         * Zero will cause L3 cases failure. 
         */
        case L3_MTU_VALUESm:
        case IS_TDM_CALENDAR0m:
        case IS_TDM_CALENDAR1m:
        case IS_TDM_CALENDAR0_PIPE0m:
        case IS_TDM_CALENDAR0_PIPE1m:
        case IS_TDM_CALENDAR0_PIPE2m:
        case IS_TDM_CALENDAR0_PIPE3m:
        case IS_TDM_CALENDAR1_PIPE0m:
        case IS_TDM_CALENDAR1_PIPE1m:
        case IS_TDM_CALENDAR1_PIPE2m:
        case IS_TDM_CALENDAR1_PIPE3m:
        case LINK_STATUSm:
#ifdef BCM_TOMAHAWK2_SUPPORT
        case PKT_SCH_CALENDAR0_PIPE0m:
        case PKT_SCH_CALENDAR0_PIPE1m:
        case PKT_SCH_CALENDAR0_PIPE2m:
        case PKT_SCH_CALENDAR0_PIPE3m:
        case PKT_SCH_CALENDAR1_PIPE0m:
        case PKT_SCH_CALENDAR1_PIPE1m:
        case PKT_SCH_CALENDAR1_PIPE2m:
        case PKT_SCH_CALENDAR1_PIPE3m:
#endif
            return FALSE;
        default:
            break;
        }
        SOC_MEM_BLOCK_ITER(unit, mem, blk) {
            if (copyno != COPYNO_ALL && copyno != blk) {
                continue;
            }
            switch(SOC_BLOCK_TYPE(unit, blk)) {
            case SOC_BLK_IPIPE:
            case SOC_BLK_EPIPE:
                return TRUE;
            default:
                break;
            }
        }
    } else
#endif
    {
        COMPILER_REFERENCE(unit);
        COMPILER_REFERENCE(mem);
        COMPILER_REFERENCE(copyno);
        COMPILER_REFERENCE(blk);
    }

    return FALSE;
}

/*
 * Function:
 *    soc_mem_clear
 * Purpose:
 *    Clears a memory.
 *    Operates on all copies of the table if copyno is COPYNO_ALL.
 * Notes:
 *    For non-sorted tables, all entries are cleared.  For sorted
 *    tables, only the entries known to contain data are cleared,
 *    unless the force_all flag is on, in which case all entries
 *    are cleared.  The force_all flag is mainly intended for the first
 *    initialization after a chip reset.
 */

int
soc_mem_clear(int unit,
              soc_mem_t mem,
              int copyno,
              int force_all)
{
    int           rv = SOC_E_NONE;
#if defined(BCM_ESW_SUPPORT) || defined(BCM_SIRIUS_SUPPORT) || defined(BCM_CALADAN3_SUPPORT)
    int           blk, index_min, index_max, index;
#endif
#if defined(BCM_FIREBOLT_SUPPORT) || defined(BCM_SIRIUS_SUPPORT) || defined(BCM_CALADAN3_SUPPORT)
    uint8         *vmap, *vmap1 = NULL;
    uint32        entry_dw, *cache;
    void          *null_entry = soc_mem_entry_null(unit, mem);
    uint32        empty_entry[SOC_MAX_MEM_WORDS] = {0};
#if defined(INCLUDE_MEM_SCAN) && \
    (defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT))
    uint32        xy_null_entry[SOC_MAX_MEM_WORDS] = {0};
#endif /* INCLUDE_MEM_SCAN && (BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT) */
#endif /* BCM_FIREBOLT_SUPPORT || BCM_SIRIUS_SUPPORT */

#ifdef BCM_TRIDENT_SUPPORT
    int(*pipe_select)(int, int, int) = NULL;
    int           require_pipe_select = FALSE;
#ifdef BCM_TRIDENT2_SUPPORT
    uint8         acc_type = 0;
    int           tcam_protect_write = 0;
    int           config_128b_entries = 0;
#endif
    if (SOC_IS_TRIDENT(unit) || SOC_IS_TITAN(unit)) {
        pipe_select = soc_trident_pipe_select;
        require_pipe_select = _soc_mem_clear_require_pipe_select(unit, mem);
    }
#ifdef BCM_TRIDENT2_SUPPORT
    else if (SOC_IS_TRIDENT2X(unit) || SOC_IS_TITAN2X(unit)) {
        pipe_select = soc_trident2_pipe_select;
        acc_type = SOC_MEM_ACC_TYPE(unit, mem);
        require_pipe_select = (acc_type == _SOC_ACC_TYPE_PIPE_SBS) ? TRUE : FALSE;
    }
#endif
    else {
        pipe_select = NULL;
    }
#endif
    _SOC_MEM_REPLACE_MEM(unit, mem);
    if (soc_mem_index_count(unit, mem) == 0) {
        return SOC_E_NONE;
    }

    if (SOC_HW_RESET(unit) &&
        soc_mem_clearable_on_reset(unit, mem, copyno)) {
        return SOC_E_NONE;
    }

/* DISABLE_SBUS_MEMWR_PARITY_CHECK bit has to been cleared before return */
#if defined(BCM_TRIDENT2_SUPPORT)
    if (SOC_IS_TD2_TT2(unit) && (!SOC_IS_TOMAHAWKX(unit))) {
        soc_trident2_l3_memwr_parity_check(unit, mem, TRUE);
    }
    _soc_mem_clear_tcam_protect_clear_restore(unit, mem, 0,
        &tcam_protect_write, &config_128b_entries);
#endif /* BCM_TRIDENT2_SUPPORT */

#if defined(BCM_FIREBOLT_SUPPORT) || defined(BCM_SIRIUS_SUPPORT) || defined(BCM_CALADAN3_SUPPORT)
    if (SOC_IS_FBX(unit) || SOC_IS_SIRIUS(unit) || SOC_IS_CALADAN3(unit)) {
        if (!null_entry) {
            null_entry = empty_entry;
        }
        MEM_LOCK(unit, mem);
        SOC_MEM_BLOCK_ITER(unit, mem, blk) {
            if (copyno != COPYNO_ALL && copyno != blk) {
                continue;
            }
            index_min = soc_mem_index_min(unit, mem);
            index_max = soc_mem_index_max(unit, mem);
            if (SOC_MEM_CLEAR_USE_DMA(unit)) {
                rv = _soc_xgs3_mem_clear_pipe(unit, mem, blk, null_entry);
                if (rv == SOC_E_UNAVAIL) {
                    if (soc_mem_slamable(unit, mem, blk)) {
                        rv = _soc_xgs3_mem_clear_slam(unit, mem, blk, null_entry);
                        if (rv < 0) {
                            goto check_cache;
                        }
#ifdef BCM_TRIDENT_SUPPORT
                        if ((SOC_IS_TD_TT(unit) && !SOC_IS_TOMAHAWKX(unit)) && 
                        	  (require_pipe_select == TRUE) && (pipe_select != NULL)) {
                            IP_ARBITER_LOCK(unit);
                            /* Switch to Y-pipe */
                            (void)pipe_select(unit, TRUE, 1);
                            (void)pipe_select(unit, FALSE, 1);
                            rv = _soc_xgs3_mem_clear_slam(unit, mem, blk, null_entry);
                            /* Restore to X-pipe */
                            (void)pipe_select(unit, TRUE, 0);
                            (void)pipe_select(unit, FALSE, 0);
                            IP_ARBITER_UNLOCK(unit);
                            if (rv < 0) {
                                goto check_cache;
                            }
                        }
#endif
                    } else {
                        goto _clear_use_pio;
                    }
                }
                if (rv < 0) {
                    goto check_cache;
                }
            } else {
_clear_use_pio:
                for (index = index_min; index <= index_max; index++) {
                    if ((rv = soc_mem_write(unit, mem, blk, index,
                                            null_entry)) < 0) {
                        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                                  (BSL_META_U(unit,
                                              "soc_mem_clear: "
                                              "write %s.%s[%d] failed: %s\n"),
                                   SOC_MEM_UFNAME(unit, mem),
                                   SOC_BLOCK_NAME(unit, blk),
                                   index, soc_errmsg(rv)));
                        goto done;
                    }
                }
#ifdef BCM_TRIDENT_SUPPORT
                if ((SOC_IS_TD_TT(unit) && !SOC_IS_TOMAHAWKX(unit)) && 
                    (require_pipe_select == TRUE) && (pipe_select != NULL)) {
                    IP_ARBITER_LOCK(unit);
                    /* Switch to Y-pipe */
                    (void)pipe_select(unit, TRUE, 1);
                    (void)pipe_select(unit, FALSE, 1);
                    for (index = index_min; index <= index_max; index++) {
                        if ((rv = soc_mem_write(unit, mem, blk, index,
                                                null_entry)) < 0) {
                            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                                  (BSL_META_U(unit,
                                              "soc_mem_clear: "
                                              "write %s.%s[%d] failed: %s\n"),
                                   SOC_MEM_UFNAME(unit, mem),
                                   SOC_BLOCK_NAME(unit, blk),
                                   index, soc_errmsg(rv)));
                            /* Restore to X-pipe */
                            (void)pipe_select(unit, TRUE, 0);
                            (void)pipe_select(unit, FALSE, 0);
                            IP_ARBITER_UNLOCK(unit);
                            goto done;
                        }
                    }
                    /* Restore to X-pipe */
                    (void)pipe_select(unit, TRUE, 0);
                    (void)pipe_select(unit, FALSE, 0);
                    IP_ARBITER_UNLOCK(unit);
                }
#endif
            }
check_cache:
            /* Update cache if active */
            cache = SOC_MEM_STATE(unit, mem).cache[blk];
            vmap = SOC_MEM_STATE(unit, mem).vmap[blk];
            if (_SOC_MEM_CHK_L2_MEM(mem)) {
                if (mem == L2_ENTRY_1m) {
                    vmap1 = SOC_MEM_STATE(unit, L2_ENTRY_2m).vmap[blk];
                } else if (mem == L2_ENTRY_2m) {
                    vmap1 = SOC_MEM_STATE(unit, L2_ENTRY_1m).vmap[blk];
                }
            }
            entry_dw = soc_mem_entry_words(unit, mem);
            if (cache != NULL && !SOC_MEM_TEST_SKIP_CACHE(unit) &&
                !SOC_WARM_BOOT(unit)) {
#if defined(INCLUDE_MEM_SCAN) && \
    (defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT))
                if (soc_feature(unit, soc_feature_xy_tcam) &&
                    (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_CAM) &&
                    (!(SOC_MEM_INFO(unit, mem).flags &
                               SOC_MEM_FLAG_EXT_CAM))) {
                    _soc_mem_tcam_dm_to_xy(unit, mem, 1, null_entry,
                                           xy_null_entry, NULL);
                }
#endif /* INCLUDE_MEM_SCAN && (BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT) */
                for (index = index_min; index <= index_max; index++) {
                    if (SOC_WARM_BOOT(unit) || SOC_IS_RELOADING(unit) ||
                        SOC_HW_ACCESS_DISABLE(unit)) {
                        CACHE_VMAP_CLR(vmap, index);
                    } else if (vmap1) {
                        CACHE_VMAP_CLR(vmap, index);
                        if (mem == L2_ENTRY_1m) {
                            CACHE_VMAP_CLR(vmap1, index/2);
                        } else {
                            CACHE_VMAP_CLR(vmap1, index*2);
                            CACHE_VMAP_CLR(vmap1, index*2 + 1);
                        }
                    } else {
                        sal_memcpy(cache + index * entry_dw, null_entry,
                                   entry_dw * 4);
                        CACHE_VMAP_CLR(vmap, index);
#if defined(INCLUDE_MEM_SCAN) && \
    (defined(BCM_TRIDENT_SUPPORT) || defined(BCM_SHADOW_SUPPORT))
                        if (soc_feature(unit, soc_feature_xy_tcam) &&
                            (SOC_MEM_INFO(unit, mem).flags &
                             SOC_MEM_FLAG_CAM) &&
                            (!(SOC_MEM_INFO(unit, mem).flags &
                               SOC_MEM_FLAG_EXT_CAM))) {
                            /* Update memscan TCAM cache if necessary */
                            soc_mem_scan_tcam_cache_update(unit, mem,
                                                           index, index,
                                                           xy_null_entry);
                        }
#endif /* INCLUDE_MEM_SCAN && (BCM_TRIDENT_SUPPORT || BCM_SHADOW_SUPPORT) */
#ifdef BCM_TRIDENT2_SUPPORT
#ifdef BCM_WARM_BOOT_SUPPORT
                        if (SOC_IS_TRIDENT2X(unit) || SOC_IS_TITAN2X(unit)) {
                            soc_mem_overlay_tcam_update(unit, mem, blk,
                                                        index, index);
                        }
#endif
#endif
                    }
                }
            }
        }
#ifdef BCM_TRIDENT2_SUPPORT
    /* For MMU_INTFI_X/YPIPE_FC_MAP_TBL0/1, update software shadow table. */
    if (SOC_IS_TRIDENT2(unit)) {
        if (mem == MMU_INTFI_XPIPE_FC_MAP_TBL0m ||
            mem == MMU_INTFI_XPIPE_FC_MAP_TBL1m ||
            mem == MMU_INTFI_YPIPE_FC_MAP_TBL0m ||
            mem == MMU_INTFI_YPIPE_FC_MAP_TBL1m){
            rv = soc_trident2_fc_map_shadow_clear(unit, mem);
            goto done;
        }
    }
#endif
        goto done; /* Required if block iter does not execute even once */
    }
#endif /* BCM_FIREBOLT_SUPPORT || BCM_SIRIUS_SUPPORT */

#if defined(BCM_XGS12_FABRIC_SUPPORT)
    if (copyno != COPYNO_ALL) {
        assert(SOC_MEM_BLOCK_VALID(unit, mem, copyno));
    }

    MEM_LOCK(unit, mem);

    SOC_MEM_BLOCK_ITER(unit, mem, blk) {
        if (copyno != COPYNO_ALL && copyno != blk) {
            continue;
        }

        index_min = soc_mem_index_min(unit, mem);
        index_max = soc_mem_index_max(unit, mem);

        switch (mem) {
#if defined(BCM_XGS_SWITCH_SUPPORT)
        case L3_IPMCm:
#endif
#ifdef BCM_XGS12_FABRIC_SUPPORT
        case MEM_UCm:
        case MEM_MCm:
        case MEM_VIDm:
        case MEM_IPMCm:
#ifdef BCM_HERCULES15_SUPPORT
        case MEM_EGR_MODMAPm:
        case MEM_ING_MODMAPm:
        case MEM_ING_SRCMODBLKm:
        case MEM_TRUNK_PORT_POOLm:
#endif /* BCM_HERCULES15_SUPPORT */
#endif /* BCM_XGS12_FABRIC_SUPPORT */
#ifdef BCM_XGS_SWITCH_SUPPORT
        case EGR_MASKm:
        case MAC_BLOCKm:
        case PORT_TABm:
        case STG_TABm:
        case TRUNK_GROUPm:
        case TRUNK_BITMAPm:
        case TRUNK_EGR_MASKm:
        case VLAN_TABm:
#endif /* BCM_XGS_SWITCH_SUPPORT */
            /* NOTE: fall through from above */
            /* Tables where all entries should be cleared to null value */
            for (index = index_min; index <= index_max; index++) {
                if ((rv = soc_mem_write(unit, mem, blk, index,
                                        soc_mem_entry_null(unit, mem))) < 0) {
                    LOG_ERROR(BSL_LS_SOC_SOCMEM,
                              (BSL_META_U(unit,
                                          "soc_mem_clear: "
                                          "write %s.%s[%d] failed: %s\n"),
                               SOC_MEM_UFNAME(unit, mem),
                               SOC_BLOCK_NAME(unit, blk),
                               index, soc_errmsg(rv)));
                    goto done;
                }
            }
            break;
#if defined(BCM_5690) || defined(BCM_5665)
        case GFILTER_FFPCOUNTERSm:
        case GFILTER_FFPPACKETCOUNTERSm:
#if defined(BCM_5695)
        case GFILTER_FFP_IN_PROFILE_COUNTERSm:
        case GFILTER_FFP_OUT_PROFILE_COUNTERSm:
#endif
        case GFILTER_METERINGm:
#endif
#if defined(BCM_5673) || defined(BCM_5674)
        case XFILTER_FFPCOUNTERSm:
        case XFILTER_FFPOPPACKETCOUNTERSm:
        case XFILTER_FFPIPPACKETCOUNTERSm:
        case XFILTER_FFPOPBYTECOUNTERSm:
        case XFILTER_FFPIPBYTECOUNTERSm:
        case XFILTER_METERINGm:
#endif
#if defined(BCM_5690) || defined(BCM_5665) || \
    defined(BCM_5673) || defined(BCM_5674)
            if (soc_feature(unit, soc_feature_filter_metering)) {
                /* NOTE: fall through from above */
                /* Tables where all entries should be cleared to null value */
                for (index = index_min; index <= index_max; index++) {
                    if ((rv = soc_mem_write(unit, mem, blk, index,
                                            soc_mem_entry_null(unit,
                                                               mem))) < 0) {
                        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                                  (BSL_META_U(unit,
                                              "soc_mem_clear: "
                                              "write %s.%s[%d] failed: %s\n"),
                                   SOC_MEM_UFNAME(unit, mem),
                                   SOC_BLOCK_NAME(unit, blk),
                                   index, soc_errmsg(rv)));
                        goto done;
                    }
                }
                break;
            } /* else fall thru to default */
#endif /* BCM_XGS_SWITCH_SUPPORT */
        default:
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "soc_mem_clear: don't know how to clear %s\n"),
                       SOC_MEM_UFNAME(unit, mem)));
            rv = SOC_E_PARAM;
            goto done;
        }
    }
#endif /* XGS12_FABRIC */

    rv = SOC_E_NONE;
    goto done;

done:
    MEM_UNLOCK(unit, mem);

/* DISABLE_SBUS_MEMWR_PARITY_CHECK bit has to be cleared before return */
#if defined(BCM_TRIDENT2_SUPPORT)
    if (SOC_IS_TD2_TT2(unit) && (!SOC_IS_TOMAHAWKX(unit))) {
        soc_trident2_l3_memwr_parity_check(unit, mem, FALSE);
    }
    _soc_mem_clear_tcam_protect_clear_restore(unit, mem, 1,
        &tcam_protect_write, &config_128b_entries);
#endif /* BCM_TRIDENT2_SUPPORT */

    LOG_INFO(BSL_LS_SOC_SOCMEM,
             (BSL_META_U(unit,
                         "soc_mem_clear: unit %d memory %s.%s returns %s\n"),
              unit, SOC_MEM_UFNAME(unit, mem),
              SOC_BLOCK_NAME(unit, copyno), soc_errmsg(rv)));

    return rv;
}

/*
 * Function:
 *      _soc_mem_search
 * Purpose:
 *      Helper function for soc_mem_search.  Does the raw search
 *      without worrying about special memory cases.
 * Parameters:
 *      See soc_mem_search, below.
 * Returns:
 *  SOC_E_NOT_FOUND
 *  Entry not found:  in this case, if
 *                           table is sorted, then index_ptr gets the
 *                           location in which the entry should be
 *                           inserted.
 *  SOC_E_NONE       Entry is found:  index_ptr gets location
 *  SOC_E_XXX        If internal error occurs
 * Notes:
 *  See soc_mem_search, below.
 */

STATIC int
_soc_mem_search(int unit,
               soc_mem_t mem,
               int copyno,
               int *index_ptr,
               void *key_data,
               void *entry_data,
               int lowest_match)
{
    int base, count, mid, last_read = -1;
    int rv, r;

    /*
     * Establish search range within table.  For filter, optimize using
     * cached start/count of rule set for mask specified by FSEL.  Note
     * that all rules using a given FSEL are necessarily contiguous.
     */
    base = soc_mem_index_min(unit, mem);
    count = soc_mem_entries(unit, mem, copyno);

    while (count > 0) {
        mid = base + count / 2;

        if ((rv = soc_mem_read(unit, mem, copyno, mid, entry_data)) < 0) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "soc_mem_search: read %s.%s[%d] failed\n"),
                       SOC_MEM_UFNAME(unit, mem),
                       SOC_BLOCK_NAME(unit, copyno), mid));
            return rv;
        }

        last_read = mid;

        r = soc_mem_compare_key(unit, mem, key_data, entry_data);

        if (r == 0) {
            if (lowest_match) {
                count = count / 2;      /* Must treat same as < 0 */
            } else {
                if (index_ptr != NULL) {
                    *index_ptr = mid;   /* Don't care if it's lowest match */
                }
                return SOC_E_NONE;              /* Found */
            }
        } else if (r < 0) {
            count = count / 2;
        } else {
            base = mid + 1;
            count = (count - 1) / 2;
        }
    }

    *index_ptr = base;

    /*
     * base is the expected position of the entry (insertion point).
     * If lowest_match is true, we may have actually found it, and
     * we have to check again here.
     */

    if (lowest_match && base <= soc_mem_index_max(unit, mem)) {
        if (last_read != base &&        /* Optimize out read if possible */
            (rv = soc_mem_read(unit, mem, copyno, base, entry_data)) < 0) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "soc_mem_search: read %s.%s[%d] failed\n"),
                       SOC_MEM_UFNAME(unit, mem),
                       SOC_BLOCK_NAME(unit, copyno), base));
            return rv;
        }

        if (soc_mem_compare_key(unit, mem, key_data, entry_data) == 0) {
            return SOC_E_NONE;                  /* Found */
        }
    }

    /* If bitmap conflict found, return SOC_E_CONFIG */
    return SOC_E_NOT_FOUND;       /* No error; not found */
}

/* Special hash mem lookup routine used only in context of generic lookup failure
   due to parity/ecc error in order to do bank specific lookups or accelerate
   mem cache indexing by fetching bucket index from h/w */
STATIC int
_soc_mem_bank_lookup(int unit, soc_mem_t mem, int copyno, int32 banks,
                     void *key, void *result, int *index_ptr)
{
    schan_msg_t schan_msg;
    int         rv, entry_dw, allow_intr=0;
    uint8       at;
    int         type, index, err_info=0;
    uint32      *data;
    int         src_blk, dst_blk, acc_type, data_byte_len;
    int         opcode, nack;
    uint32      bank_ignore_mask;

    entry_dw = soc_mem_entry_words(unit, mem);

    schan_msg_clear(&schan_msg);
    acc_type  = SOC_MEM_ACC_TYPE(unit, mem);
    src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
    dst_blk = SOC_BLOCK2SCH(unit, copyno);
    if ((soc_feature(unit, soc_feature_ism_memory) && soc_mem_is_mview(unit, mem)) ||
        (soc_feature(unit, soc_feature_shared_hash_mem) && ((mem == L2Xm) ||
         (mem == L3_ENTRY_ONLYm) || (mem == L3_ENTRY_IPV4_UNICASTm) ||
         (mem == L3_ENTRY_IPV4_MULTICASTm) || (mem == L3_ENTRY_IPV6_UNICASTm) ||
         (mem == L3_ENTRY_IPV6_MULTICASTm)
#if defined(BCM_TOMAHAWK_SUPPORT)
         || _SOC_MEM_CHK_FPEM_MEM(mem)
#endif
       ))) {
        /* don't set bank id in cos */
        bank_ignore_mask = 0;
    } else {
        bank_ignore_mask = banks & 0x3;
    }
    data_byte_len = entry_dw * 4;

    schan_msg.gencmd.address = soc_mem_addr_get(unit, mem, 0, copyno, 0, &at);
    if ((soc_feature(unit, soc_feature_ism_memory) && soc_mem_is_mview(unit, mem)) ||
        (soc_feature(unit, soc_feature_shared_hash_mem) && ((mem == L2Xm) ||
         (mem == L3_ENTRY_ONLYm) || (mem == L3_ENTRY_IPV4_UNICASTm) ||
         (mem == L3_ENTRY_IPV4_MULTICASTm) || (mem == L3_ENTRY_IPV6_UNICASTm) ||
         (mem == L3_ENTRY_IPV6_MULTICASTm)
#if defined(BCM_TOMAHAWK_SUPPORT)
         || _SOC_MEM_CHK_FPEM_MEM(mem)
#endif
       ))) {
        /* set bank ignore mask in lower 20 bits */
        if (banks && banks != SOC_MEM_HASH_BANK_ALL) {
            if (soc_feature(unit, soc_feature_shared_hash_mem)) {
                schan_msg.gencmd.address |= (~banks & SOC_HASH_BANK_MASK_SHARED);
            } else {
#ifdef BCM_TRIUMPH3_SUPPORT
                uint32 phy_banks = soc_ism_get_phy_bank_mask(unit, banks);
                schan_msg.gencmd.address |= (~phy_banks & SOC_HASH_BANK_MASK_ISM);
#endif /* BCM_TRIUMPH3_SUPPORT */
            }
        }
    }

    soc_schan_header_cmd_set(unit, &schan_msg.header, TABLE_LOOKUP_CMD_MSG,
                             dst_blk, src_blk, acc_type, data_byte_len, 0,
                             bank_ignore_mask);

    /* Fill in entry data */
    sal_memcpy(schan_msg.gencmd.data, key, entry_dw * 4);

    if (SOC_IS_SAND(unit)) {
        allow_intr = 1;
    }

    /*
     * Execute S-Channel "lookup insert" command packet consisting of
     * (header word + address word + entry_dw), and read back
     * (header word + response word + entry_dw) data words.
     */

    rv = soc_schan_op(unit, &schan_msg, entry_dw + 2, entry_dw + 2, allow_intr);

    /* Check result */
    soc_schan_header_status_get(unit, &schan_msg.header, &opcode, NULL, NULL,
                                NULL, NULL, &nack);
    if (opcode != TABLE_LOOKUP_DONE_MSG) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_bank_lookup: "
                              "invalid S-Channel reply, expected TABLE_LOOKUP_DONE_MSG:\n")));
        soc_schan_dump(unit, &schan_msg, entry_dw + 2);
        return SOC_E_INTERNAL;
    }

    if (soc_feature(unit, soc_feature_new_sbus_format) &&
        !soc_feature(unit, soc_feature_new_sbus_old_resp) ) {
        type = schan_msg.genresp_v2.response.type;
        err_info = schan_msg.genresp_v2.response.err_info;
        index = schan_msg.genresp_v2.response.index;
        data = schan_msg.genresp_v2.data;
    } else {
        type = schan_msg.genresp.response.type;
        index = schan_msg.genresp.response.index;
        data = schan_msg.genresp.data;
    }
    if ((nack != 0) || (rv == SOC_E_FAIL)) {
        if (type == SCHAN_GEN_RESP_TYPE_NOT_FOUND) {
            rv = SOC_E_NOT_FOUND;
        } else {
            rv = SOC_E_FAIL;
            
            if (!(SOC_IS_TRIUMPH3(unit) || (SOC_IS_TRIDENT2(unit)
                || (SOC_IS_TRIDENT2PLUS(unit) || SOC_IS_TITAN2PLUS(unit)))
                || (SOC_IS_TOMAHAWKX(unit)) || SOC_IS_APACHE(unit))) {
                index = schan_msg.dwords[2] & 0xffff;
            }
            if (err_info == SCHAN_GEN_RESP_ERR_INFO_ERROR_IN_LP_TABLE) {
                LOG_ERROR(BSL_LS_SOC_SOCMEM, (BSL_META_U(unit,
                    "_soc_mem_bank_lookup: Encountered error on LP table "
                    "for memory %s, banks[0x%0x], err_rsp_index[%d] !!\n"),
                    SOC_MEM_NAME(unit, mem), banks, index));
#if defined(BCM_TOMAHAWK_SUPPORT)
                if (SOC_IS_TOMAHAWKX(unit)) {
                    if (SOC_FAILURE(soc_th_lp_to_fv_index_remap(unit, mem, &index))) {
                        return SOC_E_INTERNAL; 
                    }
                }
#endif
            }
        }
    } else {
        if (result != NULL) {
            sal_memcpy(result, data, entry_dw * sizeof(uint32));
        }
    }
    *index_ptr = index;

    if (bsl_check(bslLayerSoc, bslSourceSocmem, bslSeverityNormal, unit)) {
        LOG_CLI((BSL_META_U(unit,
                            "Bank lookup table[%s]: banks=%d"), SOC_MEM_NAME(unit, mem),
                 banks));
        if (bsl_check(bslLayerSoc, bslSourceCommon, bslSeverityVerbose, unit)) {
            soc_mem_entry_dump(unit, mem, data);
        }
        if (SOC_FAILURE(rv)) {
            if (type == SCHAN_GEN_RESP_TYPE_NOT_FOUND) {
                LOG_CLI((BSL_META_U(unit,
                                    " Not found\n")));
            } else {
                LOG_CLI((BSL_META_U(unit,
                                    " Fail, bkt:%d\n"), index));
            }
        } else {
            LOG_CLI((BSL_META_U(unit,
                                " (index=%d)\n"), index));
        }
    }

    return rv;
}

#ifdef BCM_TRIDENT2_SUPPORT
STATIC INLINE int
_soc_mem_shared_hash_entries_per_bkt(soc_mem_t mem)
{
    switch(mem) {
    case L2Xm:
    case L3_ENTRY_ONLYm:
    case L3_ENTRY_IPV4_UNICASTm:
        return 4;
    case L3_ENTRY_IPV6_UNICASTm:
    case L3_ENTRY_IPV4_MULTICASTm:
        return 2;
    case L3_ENTRY_IPV6_MULTICASTm:
        return 1;
    case EXACT_MATCH_2m:
    case EXACT_MATCH_2_PIPE0m:
    case EXACT_MATCH_2_PIPE1m:
    case EXACT_MATCH_2_PIPE2m:
    case EXACT_MATCH_2_PIPE3m:        
        return 4;
    case EXACT_MATCH_4m:
    case EXACT_MATCH_4_PIPE0m:
    case EXACT_MATCH_4_PIPE1m:
    case EXACT_MATCH_4_PIPE2m:
    case EXACT_MATCH_4_PIPE3m:        
        return 2;         
    default:
        return 4;
    }
}
#endif /* BCM_TRIDENT2_SUPPORT */

STATIC int
_soc_mem_hash_entries_per_bkt(int unit, soc_mem_t mem)
{
    if (soc_feature(unit, soc_feature_ism_memory) && soc_mem_is_mview(unit, mem)) {
        if (mem == L2_ENTRY_2m || mem == L3_ENTRY_2m || mem == VLAN_XLATE_EXTDm
            || mem == MPLS_ENTRY_EXTDm) {
            return 2;
        } else if (mem == L3_ENTRY_4m) {
            return 1;
        } else {
            return 4;
        }
    }
#if defined(BCM_HURRICANE2_SUPPORT) || defined(BCM_KATANA2_SUPPORT) || defined(BCM_TRIDENT_SUPPORT)
    else if (soc_feature(unit, soc_feature_hr2_dual_hash) ||
             SOC_IS_KATANA2(unit) || SOC_IS_TRIDENT(unit)) {
        switch (mem) {
            case L3_ENTRY_ONLYm:
            case L3_ENTRY_IPV4_UNICASTm:
                return 8;
            case L2Xm:
            case L3_ENTRY_IPV6_UNICASTm:
            case L3_ENTRY_IPV4_MULTICASTm:
                return 4;
            case L3_ENTRY_IPV6_MULTICASTm:
                return 2;
            default:
                return 4;
        }
    }
#endif /* BCM_HURRICANE2_SUPPORT */
#ifdef BCM_TRIDENT2_SUPPORT
    else {
        return _soc_mem_shared_hash_entries_per_bkt(mem);
    }
#endif /* BCM_TRIDENT2_SUPPORT */
    return 4;
}

STATIC int
_soc_hash_mem_entry_base_get(int unit, soc_mem_t mem, int bank, int base_index,
                             int num_entries)
{
    if (soc_feature(unit, soc_feature_ism_memory) && soc_mem_is_mview(unit, mem)) {
        return base_index * num_entries;
    } else if (soc_feature(unit, soc_feature_shared_hash_mem)) {
         if (((mem == L2Xm) ||
             (mem == L3_ENTRY_ONLYm) || (mem == L3_ENTRY_IPV4_UNICASTm) ||
             (mem == L3_ENTRY_IPV4_MULTICASTm) || (mem == L3_ENTRY_IPV6_UNICASTm) ||
             (mem == L3_ENTRY_IPV6_MULTICASTm))) {
            return base_index * num_entries;
         } else {
            return (base_index/num_entries) * num_entries;
         }
    } else {
        num_entries *= 2;
        if (bank) {
            return ((base_index * num_entries ) + 4);
        } else {
            return base_index * num_entries;
        }
    }
}

STATIC void
_soc_mem_entry_get_key(int unit, soc_mem_t mem, void *entry, void *key)
{
#ifdef BCM_ISM_SUPPORT
    if (soc_feature(unit, soc_feature_ism_memory) && soc_mem_is_mview(unit, mem)) {
        /* Use ism routines */
        (void)soc_gen_key_from_entry(unit, mem, entry, key);
    } else
#endif /* BCM_ISM_SUPPORT */
    {
        /* Else use key field */
        if (SOC_MEM_FIELD_VALID(unit, mem, KEYf)) {
            uint32 key_buff[SOC_MAX_MEM_FIELD_WORDS];
            soc_mem_field_get(unit, mem, entry, KEYf, key_buff);
            soc_mem_field_set(unit, mem, key, KEYf, key_buff);
        } else {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "Unable to retreive key for %s.\n"),
                       SOC_MEM_NAME(unit, mem)));
        }
    }
}

/* NOTE: index1 is used for legacy dual hash only */
STATIC int
_soc_mem_cache_lookup(int unit, soc_mem_t mem, int copyno, int32 banks,
                      void *key, void *result, int index0, int index1,
                      int *index_ptr, uint32 *cache, uint8 *vmap)
{
    int i, rc;
    uint32 bmap = 0;
    uint32 *entry_ptr, match_key[SOC_MAX_MEM_WORDS];
    uint32 entry_dw = soc_mem_entry_words(unit, mem);
#ifdef BCM_ISM_SUPPORT
    _soc_ism_mem_banks_t ism_banks;
#endif /* BCM_ISM_SUPPORT */

#ifdef BCM_ISM_SUPPORT
    if (soc_feature(unit, soc_feature_ism_memory) && soc_mem_is_mview(unit, mem)) {
        rc = soc_ism_get_banks_for_mem(unit, mem, ism_banks.banks,
                                       ism_banks.bank_size, &ism_banks.count);
        if (SOC_FAILURE(rc) || ism_banks.count == 0) {
            if (index_ptr) {
                *index_ptr = -1;
            }
            return SOC_E_NOT_FOUND;
        }
        for (i = 0; i < ism_banks.count; i++) {
            bmap |= 1 << ism_banks.banks[i];
        }
        bmap &= banks;
    } else
#endif /* BCM_ISM_SUPPORT */
#ifdef BCM_TRIDENT2_SUPPORT
    if (soc_feature(unit, soc_feature_shared_hash_mem) && ((mem == L2Xm) ||
         (mem == L3_ENTRY_ONLYm) || (mem == L3_ENTRY_IPV4_UNICASTm) ||
         (mem == L3_ENTRY_IPV4_MULTICASTm) || (mem == L3_ENTRY_IPV6_UNICASTm) ||
         (mem == L3_ENTRY_IPV6_MULTICASTm)
#if defined(BCM_TOMAHAWK_SUPPORT)
         || _SOC_MEM_CHK_FPEM_MEM(mem)
#endif
       )) {
#ifdef BCM_APACHE_SUPPORT
        if (SOC_IS_APACHE(unit)) {
            SOC_IF_ERROR_RETURN
                (soc_apache_hash_bank_phy_bitmap_get(unit, mem, &bmap));
        } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
        if (SOC_IS_TOMAHAWKX(unit)) {
            SOC_IF_ERROR_RETURN
                (soc_tomahawk_hash_bank_phy_bitmap_get(unit, mem, &bmap));
        } else
#endif /* BCM_TOMAHAWK_SUPPORT */
        {
            SOC_IF_ERROR_RETURN
                (soc_trident2_hash_bank_bitmap_get(unit, mem, &bmap));
        }
        /* For L2X and L3_ENTRY tables, if banks is zero, it means all banks
         * banks should be either specific bank bitmap or an indication that
         * specifying all banks.
         */
        (banks) ? bmap &= banks : bmap;
    } else
#endif /* BCM_TRIDENT2_SUPPORT */
    { /* Dual hash */
        bmap = 0x3;
        (banks) ? bmap &= banks : bmap;
    }
    /* Issue lookups on all specified & applicable banks */
    if (bmap) {
        int e, entries, entry_base, match, index = -1;

        entries = _soc_mem_hash_entries_per_bkt(unit, mem);
        for (i = 0; i < 32; i++) {
            if (bmap & (1 << i)) {
                if (index1 == -1) {
                    rc = _soc_mem_bank_lookup(unit, mem, copyno, 1 << i, key,
                                              result, &index);
                } else { /* Legacy dual hash: bucket indexes are already known */
                    index = i ? index1 : index0;
                    rc = SOC_E_FAIL;
                }
                if (SOC_FAILURE(rc)) { /* Case where correction has not yet happened */
                    if (rc == SOC_E_NOT_FOUND) {
                        continue;
                    } else if (rc == SOC_E_INTERNAL) {
                        return rc;
                    } else if (rc == SOC_E_FAIL) {
                        /* Check and compare key with all entries in the bucket */
                        if (!SOC_IS_TD2_TT2(unit)) {
                            /* All other devices return the bucket */
                            LOG_VERBOSE(BSL_LS_SOC_COMMON,
                                        (BSL_META_U(unit,
                                                    "Check in bucket: %d\n"),
                                         index));
                            entry_base = _soc_hash_mem_entry_base_get(unit, mem, i,
                                                                      index, entries);
                        } else {
                            /* TD and TD2 return the index */
                            if (mem == L3_ENTRY_IPV4_MULTICASTm || mem == L3_ENTRY_IPV6_UNICASTm
#if defined(BCM_TOMAHAWK_SUPPORT)
                                || mem == EXACT_MATCH_2m
                                || mem == EXACT_MATCH_2_PIPE0m
                                || mem == EXACT_MATCH_2_PIPE1m
                                || mem == EXACT_MATCH_2_PIPE2m
                                || mem == EXACT_MATCH_2_PIPE3m
#endif /* BCM_TOMAHAWK_SUPPORT */
                               ) {
                                index = index/2;
                            } else if (mem == L3_ENTRY_IPV6_MULTICASTm
#if defined(BCM_TOMAHAWK_SUPPORT)
                                || mem == EXACT_MATCH_4m
                                || mem == EXACT_MATCH_4_PIPE0m
                                || mem == EXACT_MATCH_4_PIPE1m
                                || mem == EXACT_MATCH_4_PIPE2m
                                || mem == EXACT_MATCH_4_PIPE3m
#endif /* BCM_TOMAHAWK_SUPPORT */
                               ) {
                                index = index/4;
                            }
                            LOG_VERBOSE(BSL_LS_SOC_COMMON,
                                        (BSL_META_U(unit,
                                                    "Check at index: %d\n"),
                                         index));
                            entry_base = index;
                        }
                        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                                    (BSL_META_U(unit,
                                                "Base entry: %d\n"), entry_base));
                        for (e = 0; e < entries; e++) {
                            /* Try with only cached entries */
                            if (CACHE_VMAP_TST(vmap, entry_base+e)) {
                                match = 0;
                                entry_ptr = cache + ((entry_base+e) * entry_dw);
                                sal_memset(match_key, 0, sizeof(match_key));
                                /* If match found then return else continue */
                                LOG_VERBOSE(BSL_LS_SOC_COMMON,
                                            (BSL_META_U(unit,
                                                        "Check cached entry at "
                                                        "index: %d\n"), entry_base+e));
                                if (soc_feature(unit, soc_feature_ism_memory) &&
                                    soc_mem_is_mview(unit, mem) && !(mem == L2_ENTRY_1m ||
                                    mem == L2_ENTRY_2m)) { /* L2 key compare routine exists for TR3 */
                                    uint32 entry_key[SOC_MAX_MEM_WORDS];

                                    sal_memset(entry_key, 0, sizeof(entry_key));
                                    _soc_mem_entry_get_key(unit, mem, entry_ptr, match_key);
                                    _soc_mem_entry_get_key(unit, mem, key, entry_key);
                                    if (sal_memcmp(entry_key, match_key, entry_dw * 4)) {
                                        continue;
                                    } else {
                                        match = 1;
                                    }
                                } else if (soc_mem_cmp_fn(unit, mem)) {
                                    if (soc_mem_compare_key(unit, mem, entry_ptr, key)) {
                                        continue;
                                    } else {
                                        match = 1;
                                    }
                                } else {
                                    uint32 entry_key[SOC_MAX_MEM_WORDS];

                                    sal_memset(entry_key, 0, sizeof(entry_key));
                                    _soc_mem_entry_get_key(unit, mem, entry_ptr, match_key);
                                    _soc_mem_entry_get_key(unit, mem, key, entry_key);
                                    if (sal_memcmp(entry_key, match_key, entry_dw * 4)) {
                                        continue;
                                    } else {
                                        match = 1;
                                    }
                                }
                                if (match) {
                                    if (result) {
                                        sal_memcpy(result, entry_ptr, entry_dw * 4);
                                    }
                                    if (index_ptr) {
                                        *index_ptr = entry_base+e;
                                    }
                                    return SOC_E_NONE;
                                }
                            }
                        }
                    }
                } else { /* Case where correction completes before this routine */
                    if (index_ptr) {
                        *index_ptr = index;
                    }
                    return SOC_E_NONE;
                }
            }
        }
    }
    if (index_ptr) {
        *index_ptr = -1;
    }
    return SOC_E_NOT_FOUND;
}

/*
 * Function:
 *      soc_mem_generic_lookup
 * Purpose:
 *      Send a lookup message over the S-Channel and receive the response.
 * Parameters:
 *      unit - StrataSwitch unit #
 *      banks - For dual hashing, which halves are selected (inverted)
 *              Note: used to create a bitmap for ISM mems. (All banks: -1)
 *      key - entry to look up
 *      result - entry to receive entire found entry
 *      index_ptr (OUT) - If found, receives table index where found
 * Returns:
 *      SOC_E_INTERNAL if retries exceeded or other internal error
 *      SOC_E_NOT_FOUND if the entry is not found.
 *      SOC_E_NONE (0) on success (entry found):
 */

int
soc_mem_generic_lookup(int unit, soc_mem_t mem, int copyno, int32 banks,
                       void *key, void *result, int *index_ptr)
{
    schan_msg_t schan_msg;
    int         rv, entry_dw, allow_intr=0;
    uint8       at;
    int         type, index, index_valid, err_info=0;
    uint32      *data;
    int         src_blk, dst_blk, acc_type, data_byte_len;
    int         opcode, nack;
    uint32      bank_ignore_mask;
#ifdef BCM_TRIUMPH_SUPPORT
    int         rv1 = SOC_E_NONE;
#endif

    _SOC_MEM_REPLACE_MEM(unit, mem);
    SOC_MEM_ALIAS_TO_ORIG(unit,mem);
    assert(SOC_MEM_IS_VALID(unit, mem));
    assert(soc_attached(unit));

#ifdef BCM_SIRIUS_SUPPORT
    if (SOC_IS_SIRIUS(unit)) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_generic_lookup: not supported on %s\n"),
                   SOC_CHIP_STRING(unit)));
        return SOC_E_UNAVAIL;
    }
#endif

    if (copyno == MEM_BLOCK_ANY) {
        /* coverity[overrun-local : FALSE] */
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }
    if (!SOC_MEM_BLOCK_VALID(unit, mem, copyno)) {
        LOG_WARN(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "soc_mem_generic_lookup: invalid block %d for memory %s\n"),
                  copyno, SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    entry_dw = soc_mem_entry_words(unit, mem);

    MEM_LOCK(unit, mem);
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_esm_correction) &&
          ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
         (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
        SOC_ESM_LOCK(unit);
    }
#endif
    schan_msg_clear(&schan_msg);
    acc_type = SOC_MEM_ACC_TYPE(unit, mem);
    src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
    dst_blk = SOC_BLOCK2SCH(unit, copyno);
    if ((soc_feature(unit, soc_feature_ism_memory) && soc_mem_is_mview(unit, mem)) ||
        (soc_feature(unit, soc_feature_shared_hash_mem) && ((mem == L2Xm) ||
         (mem == L3_ENTRY_ONLYm) || (mem == L3_ENTRY_IPV4_UNICASTm) ||
         (mem == L3_ENTRY_IPV4_MULTICASTm) || (mem == L3_ENTRY_IPV6_UNICASTm) ||
         (mem == L3_ENTRY_IPV6_MULTICASTm)
#if defined(BCM_TOMAHAWK_SUPPORT)
         || _SOC_MEM_CHK_FPEM_MEM(mem)
#endif
       ))) {
        /* don't set bank id in cos */
        bank_ignore_mask = 0;
    } else {
        bank_ignore_mask = banks & 0x3;
    }
    data_byte_len = entry_dw * 4;

    schan_msg.gencmd.address = soc_mem_addr_get(unit, mem, 0, copyno, 0, &at);
    if ((soc_feature(unit, soc_feature_ism_memory) && soc_mem_is_mview(unit, mem)) ||
        (soc_feature(unit, soc_feature_shared_hash_mem) && ((mem == L2Xm) ||
         (mem == L3_ENTRY_ONLYm) || (mem == L3_ENTRY_IPV4_UNICASTm) ||
         (mem == L3_ENTRY_IPV4_MULTICASTm) || (mem == L3_ENTRY_IPV6_UNICASTm) ||
         (mem == L3_ENTRY_IPV6_MULTICASTm)
#if defined(BCM_TOMAHAWK_SUPPORT)
         || _SOC_MEM_CHK_FPEM_MEM(mem)
#endif
       ))) {
        /* set bank ignore mask in lower 20 bits */
        if (banks && banks != SOC_MEM_HASH_BANK_ALL) {
            if (soc_feature(unit, soc_feature_shared_hash_mem)) {
                schan_msg.gencmd.address |= (~banks & SOC_HASH_BANK_MASK_SHARED);
            } else {
#ifdef BCM_TRIUMPH3_SUPPORT
                uint32 phy_banks = soc_ism_get_phy_bank_mask(unit, banks);
                schan_msg.gencmd.address |= (~phy_banks & SOC_HASH_BANK_MASK_ISM);
#endif /* BCM_TRIUMPH3_SUPPORT */
            }
        }
    }

    /* BCM56960 supports 2 different modes for lookup
     * Any width match mode - Bit 17 of SBUS address set to 0
     * Exact width match mode - Bit 17 of SBUS address set to 1
     * Below code sets lookup mode to exact width match mode for BCM56960
     */
    if (soc_feature(unit, soc_feature_exact_width_match_mode)) {
        if ((mem == L3_ENTRY_IPV4_UNICASTm)   ||
            (mem == L3_ENTRY_IPV4_MULTICASTm) ||
            (mem == L3_ENTRY_IPV6_UNICASTm)   ||
            (mem == L3_ENTRY_IPV6_MULTICASTm)
#if defined(BCM_TOMAHAWK_SUPPORT)
            || _SOC_MEM_CHK_FPEM_MEM(mem)
#endif
           ) {
            /* Set bit 17 of SBUS address to select exact width match mode */
            schan_msg.gencmd.address |= (1 << 17);
        }
    }
    
    soc_schan_header_cmd_set(unit, &schan_msg.header, TABLE_LOOKUP_CMD_MSG,
                             dst_blk, src_blk, acc_type, data_byte_len, 0,
                             bank_ignore_mask);

    /* Fill in entry data */
    sal_memcpy(schan_msg.gencmd.data, key, entry_dw * 4);

    if (SOC_IS_SAND(unit)) {
        allow_intr = 1;
    }

    /*
     * Execute S-Channel "lookup insert" command packet consisting of
     * (header word + address word + entry_dw), and read back
     * (header word + response word + entry_dw) data words.
     */

    rv = soc_schan_op(unit, &schan_msg, entry_dw + 2, entry_dw + 2, allow_intr);

    /* Check result */
    soc_schan_header_status_get(unit, &schan_msg.header, &opcode, NULL, NULL,
                                NULL, NULL, &nack);
    if (opcode != TABLE_LOOKUP_DONE_MSG) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_generic_lookup: "
                              "invalid S-Channel reply, expected TABLE_LOOKUP_DONE_MSG:\n")));
        soc_schan_dump(unit, &schan_msg, entry_dw + 2);
#if defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit, soc_feature_esm_correction) &&
              ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
             (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
            SOC_ESM_UNLOCK(unit);
        }
#endif
        MEM_UNLOCK(unit, mem);
        return SOC_E_INTERNAL;
    }

    if (soc_feature(unit, soc_feature_new_sbus_format) &&
        !soc_feature(unit, soc_feature_new_sbus_old_resp)) {
        type = schan_msg.genresp_v2.response.type;
        err_info = schan_msg.genresp_v2.response.err_info;
        index = schan_msg.genresp_v2.response.index;
        data = schan_msg.genresp_v2.data;
    } else {
        type = schan_msg.genresp.response.type;
        index = schan_msg.genresp.response.index;
        data = schan_msg.genresp.data;
    }
    if ((nack != 0) || (rv == SOC_E_FAIL)) {
        if (index_ptr) {
            *index_ptr = -1;
        }
        if (type == SCHAN_GEN_RESP_TYPE_NOT_FOUND) {
            rv = SOC_E_NOT_FOUND;
#if defined(BCM_TRIDENT2_SUPPORT)
            if (soc_feature(unit, soc_feature_shared_hash_ins) &&
                (mem == L3_ENTRY_IPV4_MULTICASTm || mem == L3_ENTRY_IPV6_UNICASTm)) {
                /* Check for banks != 'All' to distinguish from normal lookup.
                 * The special handling code always passes unique bank numbers.
                 */
                if (index_ptr && (banks != SOC_MEM_HASH_BANK_ALL)) {
                    *index_ptr = schan_msg.genresp.response.index;
                }
            }
#endif /* BCM_TRIDENT2_SUPPORT */
        } else {
            if (SOC_SER_CORRECTION_SUPPORT(unit)) {
                uint32 *cache;
                uint8 *vmap;
                LOG_VERBOSE(BSL_LS_SOC_SER,(BSL_META_U(unit,
                    ">>>>> soc_mem_generic_lookup: rv=%d, nack=%d, type=%d, index=%d\n"),
                    rv, nack, type, index));
                if (err_info == SCHAN_GEN_RESP_ERR_INFO_ERROR_IN_LP_TABLE) {
                    LOG_ERROR(BSL_LS_SOC_SOCMEM, (BSL_META_U(unit,
                        "soc_mem_generic_lookup: Encountered error on LP table "
                        "for memory %s, banks[0x%0x], err_rsp_index[%d] !!\n"),
                        SOC_MEM_NAME(unit, mem), banks, index));
#if defined(BCM_TOMAHAWK_SUPPORT)
                    if (SOC_IS_TOMAHAWKX(unit)) {
                        if (SOC_FAILURE(soc_th_lp_to_fv_index_remap(unit, mem, &index))) {
                            MEM_UNLOCK(unit, mem);
                            return SOC_E_INTERNAL; 
                        }
                    }
#endif
                }
                cache = SOC_MEM_STATE(unit, mem).cache[copyno];
                vmap = SOC_MEM_STATE(unit, mem).vmap[copyno];
                if (cache != NULL && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
                    int index0 = index, index1 = -1;
                    if (SOC_IS_TRIDENT(unit) || SOC_IS_TRIUMPH2(unit)
                         || SOC_IS_KATANA2(unit)) {
                        
                        index0 = schan_msg.dwords[2] & 0xffff;
                        index1 = schan_msg.dwords[2] >> 16;
                    }
                    rv = _soc_mem_cache_lookup(unit, mem, copyno, banks, key,
                                               result, index0, index1,
                                               index_ptr, cache, vmap);
                }
            } else {
                rv = SOC_E_FAIL;
            }
        }
    } else {
        if (result != NULL) {
            sal_memcpy(result, data, entry_dw * sizeof(uint32));
        }
#ifdef BCM_TRIUMPH_SUPPORT
        if (soc_feature(unit, soc_feature_etu_support) &&
            mem == EXT_L2_ENTRY_2m) {
            soc_mem_t ext_mem;
            int ext_index;

            rv1 = soc_tcam_raw_index_to_mem_index(unit, index,
                                &ext_mem, &ext_index);
            if (SOC_FAILURE(rv1)) {
#if defined(BCM_TRIUMPH3_SUPPORT)
                if (soc_feature(unit, soc_feature_esm_correction) &&
                      ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
                     (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
                    SOC_ESM_UNLOCK(unit);
                }
#endif
                MEM_UNLOCK(unit, mem);
                return rv1;
            }
            if (mem != ext_mem) {
#if defined(BCM_TRIUMPH3_SUPPORT)
                if (soc_feature(unit, soc_feature_esm_correction) &&
                      ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
                     (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
                    SOC_ESM_UNLOCK(unit);
                }
#endif
                MEM_UNLOCK(unit, mem);
                return SOC_E_INTERNAL;
            }
            index = ext_index;
        }
#endif /* BCM_TRIUMPH_SUPPORT */
        if (index_ptr != NULL) {
            *index_ptr = index;
        }
        index_valid = (index >= 0 &&
                           index <= soc_mem_index_max(unit, mem));
        if (!index_valid) {
#if defined(BCM_HURRICANE3_SUPPORT)
            if ((SOC_IS_HURRICANE3(unit) || SOC_IS_GREYHOUND2(unit)) &&
                (mem == L2Xm)) {
                if (index & SCHAN_GEN_RESP_INDEX_L2_OVERFLOW) {
                    /* Check whether the entry is located at L2_OVERFLOW_ENTRY */
                    if ((index & ~SCHAN_GEN_RESP_INDEX_L2_OVERFLOW) <=
                        soc_mem_index_max(unit, L2_ENTRY_OVERFLOWm)) {
                        rv = SOC_E_NONE;
                    }
                }
            } else 
#endif /* BCM_HURRICANE3_SUPPORT */
            {
                LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "soc_mem_generic_lookup: "
                                  "invalid index %d for memory %s\n"),
                       index, SOC_MEM_NAME(unit, mem)));
                rv = SOC_E_INTERNAL;
            }
        }
    }
    if (bsl_check(bslLayerSoc, bslSourceSocmem, bslSeverityNormal, unit)) {
        LOG_CLI((BSL_META_U(unit,
                            "Lookup table[%s]: banks=%d"), SOC_MEM_NAME(unit, mem),
                 banks));
        if (bsl_check(bslLayerSoc, bslSourceCommon, bslSeverityVerbose, unit)) {
            soc_mem_entry_dump(unit, mem, data);
        }
        if (SOC_FAILURE(rv)) {
            if (type == SCHAN_GEN_RESP_TYPE_NOT_FOUND) {
                LOG_CLI((BSL_META_U(unit,
                                    " Not found\n")));
            } else {
                LOG_CLI((BSL_META_U(unit,
                                    " Fail\n")));
            }
        } else {
            LOG_CLI((BSL_META_U(unit,
                                " (index=%d)\n"), index));
        }
    }
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_esm_correction) &&
        ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
         (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
        SOC_ESM_UNLOCK(unit);
    }
#endif
    MEM_UNLOCK(unit, mem);
    return rv;
}

#ifdef ALPM_ENABLE
#define _SOC_ALPM_MEM_OP_RETRY_TO          1000000
#define _SOC_ALPM_MEM_OP_RETRY_COUNT       5

STATIC int
_soc_mem_alpm_ser_correction(int unit, soc_mem_t mem, int blk,
                             int addr, int copyno, int index,
                             int bkt_ptr)
{
    int rv, pipe;
    int bank = 0, ent = 0;
    int acc_type = 0;
    uint32 entry[SOC_MAX_MEM_WORDS];
    int ent_cnt = 1, bank_cnt = 1;

    if (SOC_IS_TOMAHAWKX(unit)) {
        mem = L3_DEFIP_ALPM_ECCm;
        bkt_ptr = -1;
    } else if (SOC_IS_TRIDENT2PLUS(unit)) {
        mem = L3_DEFIP_ALPM_ECCm;
    } else if (SOC_IS_APACHE(unit)) {
        mem = L3_DEFIP_ALPM_ECCm;
        bkt_ptr = -1;
    }

    if (bkt_ptr != -1) {
        /* Need traverse to find actual parity error index*/
        switch (mem) {
        case L3_DEFIP_ALPM_ECCm:
        case L3_DEFIP_ALPM_IPV4_1m:
        case L3_DEFIP_ALPM_IPV6_64m:
            ent_cnt  = 4;
            break;
        case L3_DEFIP_ALPM_IPV6_64_1m:
            ent_cnt  = 3;
            break;
        case L3_DEFIP_ALPM_IPV6_128m:
            ent_cnt  = 2;
            break;
        case L3_DEFIP_ALPM_IPV4m:
        default:
            ent_cnt  = 6;
            break;
        }
        bank_cnt = 4;
    }

    for (pipe = 0; pipe < NUM_PIPE(unit); pipe++) {
        /* determine 1st pipe which has the problem */
        if (NUM_PIPE(unit) == 4) {
            acc_type = (pipe == 0)? _SOC_MEM_ADDR_ACC_TYPE_PIPE_0 :
                       (pipe == 1)? _SOC_MEM_ADDR_ACC_TYPE_PIPE_1 :
                       (pipe == 2)? _SOC_MEM_ADDR_ACC_TYPE_PIPE_2 :
                                    _SOC_MEM_ADDR_ACC_TYPE_PIPE_3;
        } else if (NUM_PIPE(unit) == 2) {
            acc_type = (pipe == 0) ? _SOC_MEM_ADDR_ACC_TYPE_PIPE_X:
                _SOC_MEM_ADDR_ACC_TYPE_PIPE_Y;
        }

        for (ent = 0; ent < ent_cnt; ent++) {
            for (bank = 0; bank < bank_cnt; bank ++) {
                if (bkt_ptr != -1) {
                    index = (ent << 16) | (bkt_ptr << 2) | (bank);
                }
                LOG_VERBOSE(BSL_LS_SOC_SOCMEM, (BSL_META_U(unit,
                          "unit %d: inline ALPM correction LOOP: pipe:%d index %d\n"),
                           unit, pipe, index));
                /* Only multi-pipe need to confirm which pipe has the parity error */
                if (NUM_PIPE(unit) > 1) {
                    rv = soc_mem_pipe_select_read(unit, SOC_MEM_NO_FLAGS, mem, copyno,
                                                  acc_type, index, entry);
                    if (SOC_E_FAIL == rv) {
                        LOG_ERROR(BSL_LS_SOC_SOCMEM, (BSL_META_U(unit,
                                  "unit %d: inline ALPM correction: will try ser_correction for pipe %d, index %d\n"),
                                  unit, pipe, index));

                        /* Once we found the bad pipe on TD2, restore the entire bucket
                         * across 4 banks. This could be done via call soc_ser_sram_correction
                         * 4 times with 4 indexes on 4 banks.
                         */
                        if (SOC_IS_TRIDENT2X(unit)) {
                            for (bank = 0; bank < 4; bank ++) {
                                index = (index & (~0x3)) | (bank);
                                rv = soc_ser_sram_correction(unit, pipe, blk, addr, mem, copyno,
                                                             index, NULL);
                            }
                        } else {
                            rv = soc_ser_sram_correction(unit, pipe, blk, addr, mem, copyno,
                                                         index, NULL);
                        }
                        if (SOC_FAILURE(rv)) {
                            LOG_ERROR(BSL_LS_SOC_SOCMEM, (BSL_META_U(unit,
                                "unit %d: inline ALPM correction: ser_correction FAILED for pipe %d, index %d\n"),
                                unit, pipe, index));
                        } else {
                            LOG_VERBOSE(BSL_LS_SOC_SOCMEM, (BSL_META_U(unit,
                                "unit %d: inline ALPM correction: ser_correction passed for pipe %d, index %d\n"),
                                unit, pipe, index));
                        }
                        return rv;
                    }
                }else {
                    rv = soc_ser_sram_correction(unit, pipe, blk, addr, mem, copyno,
                                                 index, NULL);
                    if (SOC_FAILURE(rv)) {
                        LOG_ERROR(BSL_LS_SOC_SOCMEM, (BSL_META_U(unit,
                            "unit %d: inline ALPM correction: ser_correction FAILED for pipe %d, index %d\n"),
                            unit, pipe, index));
                    } else {
                        LOG_VERBOSE(BSL_LS_SOC_SOCMEM, (BSL_META_U(unit,
                            "unit %d: inline ALPM correction: ser_correction passed for pipe %d, index %d\n"),
                            unit, pipe, index));
                    }
                    return rv;
                }
            }
        }
    }
    /* Means we could not find any pipe which had problem
     * Should not happen, just let caller retry operation */
    LOG_WARN(BSL_LS_SOC_SOCMEM, (BSL_META_U(unit,
        "unit %d: inline ALPM correction: ser_correction illegal pipe %d for index %d !! \n"),
        unit, pipe, index));
    return SOC_E_NONE;
}

/*
 * Function:
 *      soc_mem_alpm_lookup
 * Purpose:
 *      Send a lookup message over the S-Channel and receive the response
 *      from ALPM memories. ALPM memories have special needs bcoz the entries
 *      are not hashed.
 * Parameters:
 *      unit - StrataSwitch unit #
 *      banks - Banks
 *              Note: used to create a bitmap for ISM mems. (All banks: -1)
 *      key - entry to look up
 *      result - entry to receive entire found entry
 *      index_ptr (OUT) - If found, receives table index where found
 * Returns:
 *      SOC_E_INTERNAL if retries exceeded or other internal error
 *      SOC_E_NOT_FOUND if the entry is not found.
 *      SOC_E_NONE (0) on success (entry found):
 */

int
soc_mem_alpm_lookup(int unit, soc_mem_t mem, int bucket_index,
                    int copyno, uint32 banks_disable,
                    void *key, void *result, int *index_ptr)
{
    schan_msg_t schan_msg;
    uint8       at;
    uint32      *data;
    int         rv, entry_dw, allow_intr=0;
    int         type, index;
    int         _soc_alpm_lookup_retry = 0;
    int         src_blk, dst_blk, acc_type, data_byte_len;
    int         opcode, nack;
    uint32      bank_ignore_mask;
    int         bkt_ptr = -1;

    assert(SOC_MEM_IS_VALID(unit, mem));
    assert(soc_attached(unit));

    if (SAL_BOOT_SIMULATION) {
#if defined(ALPM_ENABLE)
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_APACHE_SUPPORT)
        if (index_ptr) {
            *index_ptr = bucket_index << (soc_alpm_cmn_banks_get(unit) / 2);
        }
#endif
#endif
        return SOC_E_NOT_FOUND;
    }

    /* Only applicable to ALPM memories */
    if ((mem != L3_DEFIP_ALPM_IPV6_128m) &&
        (mem != L3_DEFIP_ALPM_IPV6_64m) &&
        (mem != L3_DEFIP_ALPM_IPV6_64_1m) &&
        (mem != L3_DEFIP_ALPM_IPV4m) &&
        (mem != L3_DEFIP_ALPM_IPV4_1m)) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_alpm_lookup: not supported non-ALPM memories %s\n"),
                   SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

#ifdef BCM_SIRIUS_SUPPORT
    if (SOC_IS_SIRIUS(unit)) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_generic_lookup: not supported on %s\n"),
                   SOC_CHIP_STRING(unit)));
        return SOC_E_UNAVAIL;
    }
#endif

    if (copyno == MEM_BLOCK_ANY) {
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }
    if (!SOC_MEM_BLOCK_VALID(unit, mem, copyno)) {
        LOG_WARN(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "soc_mem_alpm_lookup: invalid block %d for memory %s\n"),
                  copyno, SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    entry_dw = soc_mem_entry_words(unit, mem);

    SOC_CONTROL_LOCK(unit);
    SHR_BITSET(&(SOC_CONTROL(unit)->alpm_mem_ops), _SOC_ALPM_LOOKUP);
    SOC_CONTROL_UNLOCK(unit);
_retry:
    schan_msg_clear(&schan_msg);
    acc_type = SOC_MEM_ACC_TYPE(unit, mem);
    src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
    dst_blk = SOC_BLOCK2SCH(unit, copyno);
#ifdef BCM_ISM_SUPPORT
    if (soc_feature(unit, soc_feature_ism_memory)) {
        /* don't set bank id in cos */
        bank_ignore_mask = 0;
    } else
#endif /* BCM_ISM_SUPPORT */
    {
        bank_ignore_mask = banks_disable & 0x3;
    }
    data_byte_len = entry_dw * 4;

    /* ALPM entries are not hashed */
    schan_msg.gencmd.address = soc_mem_addr_get(unit, mem, 0, copyno, 0, &at);
    /* Address contains bank disable, and bucket pointer */
    /* Bucket index and bank disable
     * addr[TD2: 13:0,  TH: 12:0,  TH2: 13:0]   - bucket
     * addr[TD2: 17:14, TH: 16:13, TH2: 17:14]  - bank disable[3:0]
     */
#if defined(BCM_TOMAHAWK_SUPPORT)
    if (SOC_IS_TOMAHAWK(unit)) {
        schan_msg.gencmd.address &= ~(0x0001ffff);
        schan_msg.gencmd.address |=
            (((banks_disable & 0xf) << 13) | (bucket_index & SOC_TH_ALPM_BKT_MASK(unit)));
    } else
#endif /* BCM_TOMAHAWK_SUPPORT */
    {
        schan_msg.gencmd.address &= ~(0x0003ffff);
        schan_msg.gencmd.address |= 
            (((banks_disable & 0xf) << 14) | (bucket_index & SOC_TD2_ALPM_BKT_MASK));
    }

    soc_schan_header_cmd_set(unit, &schan_msg.header, TABLE_LOOKUP_CMD_MSG,
                             dst_blk, src_blk, acc_type, data_byte_len, 0,
                             bank_ignore_mask);

    /* Fill in entry data */
    sal_memcpy(schan_msg.gencmd.data, key, entry_dw * 4);

    if (SOC_IS_SAND(unit)) {
        allow_intr = 1;
    }

    /*
     * Execute S-Channel "lookup insert" command packet consisting of
     * (header word + address word + entry_dw), and read back
     * (header word + response word + entry_dw) data words.
     */
    rv = soc_schan_op(unit, &schan_msg, entry_dw + 2, entry_dw + 2, allow_intr);

    /* Check result */
    soc_schan_header_status_get(unit, &schan_msg.header, &opcode, NULL, NULL,
                                NULL, NULL, &nack);
    if (opcode != TABLE_LOOKUP_DONE_MSG) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_generic_lookup: "
                              "invalid S-Channel reply, expected TABLE_LOOKUP_DONE_MSG:\n")));
        soc_schan_dump(unit, &schan_msg, entry_dw + 2);
        return SOC_E_INTERNAL;
    }

    type = schan_msg.genresp_v2.response.type;
    index = schan_msg.genresp_v2.response.index;
    data = schan_msg.genresp_v2.data;

    if ((nack != 0) || (rv == SOC_E_FAIL)) {
        if (index_ptr) {
            *index_ptr = -1;
        }
        if (type == SCHAN_GEN_RESP_TYPE_NOT_FOUND) {
            rv = SOC_E_NOT_FOUND;
        } else {
            LOG_WARN(BSL_LS_SOC_SOCMEM,
                     (BSL_META_U(unit,
                                 "unit %d: ALPM lookup operation[%s] encountered "
                                 "parity error !!\n"), unit, SOC_MEM_NAME(unit, mem)));
            _soc_alpm_lookup_retry++;
            if (type == SCHAN_GEN_RESP_TYPE_FOUND) {
                bkt_ptr = bucket_index;
            }
            if (SOC_CONTROL(unit)->alpm_lookup_retry) {
                (void)_soc_mem_alpm_ser_correction(unit,
                                                   mem,
                                                   schan_msg.gencmd.header.v2.dst_blk,
                                                   schan_msg.gencmd.address,
                                                   copyno, index,
                                                   bkt_ptr);
                

                if (_soc_alpm_lookup_retry < _SOC_ALPM_MEM_OP_RETRY_COUNT) {
                    if (sal_sem_take(SOC_CONTROL(unit)->alpm_lookup_retry,
                                     _SOC_ALPM_MEM_OP_RETRY_TO) < 0) {
                        LOG_WARN(BSL_LS_SOC_SOCMEM,
                                 (BSL_META_U(unit,
                                             "unit %d: Retry ALPM lookup operation..\n"), unit));
                        goto _retry;
                    } else {
                        LOG_WARN(BSL_LS_SOC_SOCMEM,
                                 (BSL_META_U(unit,
                                             "unit %d: Retry ALPM lookup operation..\n"), unit));
                        goto _retry;
                    }
                } else {
                    LOG_ERROR(BSL_LS_SOC_SOCMEM,
                              (BSL_META_U(unit,
                                          "unit %d: Aborting ALPM lookup operation "
                                          "due to un-correctable error !!\n"), unit));
                    rv = SOC_E_INTERNAL;
                }
            } else {
                rv = SOC_E_INTERNAL;
            }
        }
    } else {
        if (result != NULL) {
            sal_memcpy(result, data, entry_dw * sizeof(uint32));
        }
        if (index_ptr != NULL) {
            *index_ptr = index;
        }
    }
    SOC_CONTROL_LOCK(unit);
    SHR_BITCLR(&(SOC_CONTROL(unit)->alpm_mem_ops), _SOC_ALPM_LOOKUP);
    SOC_CONTROL_UNLOCK(unit);

    if (bsl_check(bslLayerSoc, bslSourceSocmem, bslSeverityNormal, unit)) {
        LOG_CLI((BSL_META_U(unit,
                            "Lookup table[%s]: banks=%d"), SOC_MEM_NAME(unit, mem),
                 banks_disable));
        if (bsl_check(bslLayerSoc, bslSourceCommon, bslSeverityVerbose, unit)) {
            soc_mem_entry_dump(unit, mem, data);
        }
        if (SOC_FAILURE(rv)) {
            if (type == SCHAN_GEN_RESP_TYPE_NOT_FOUND) {
                LOG_CLI((BSL_META_U(unit,
                                    " Not found\n")));
            } else {
                LOG_CLI((BSL_META_U(unit,
                                    " Fail\n")));
            }
        } else {
            LOG_CLI((BSL_META_U(unit,
                                " (index=%d)\n"), index));
        }
    }

    return rv;
}

/*
 * Function:
 *      soc_mem_alpm_insert
 * Purpose:
 *      Insert an entry into ALPM memory.
 * Parameters:
 *      unit - StrataSwitch unit #
 *      banks - For dual hashing, which halves are selected (inverted)
 *              Note: used to create a bitmap for ISM mems. (All banks: -1)
 *      entry - entry to insert
 *      old_entry - old entry if existing entry was replaced
 * Returns:
 *      SOC_E_NONE - success
 *      SOC_E_EXISTS - existing entry was replaced
 *      SOC_E_FULL - full
 *      SOC_E_BUSY - modfifo full
 * Notes:
 *      Uses hardware insertion; sends an INSERT message over the
 *      S-Channel.
 */

int
soc_mem_alpm_insert(int unit, soc_mem_t mem, int bucket,
                    int copyno, int32 banks_disable,
                    void *entry, void *old_entry, int *index_ptr)
{
    schan_msg_t schan_msg;
    uint8       at, *vmap;
    uint32      *data, *cache;
    int         rv, entry_dw, index_valid;
    int         type, index, allow_intr=0;
    int         _soc_alpm_insert_retry = 0;
    int         src_blk, dst_blk, acc_type, data_byte_len;
    int         opcode, nack;
    int         type2 = -1, index2 = 0;
    uint32      data2[CMIC_SCHAN_WORDS_ALLOC - 2];
    int         bkt_ptr = -1;

    SOC_MEM_ALIAS_TO_ORIG(unit,mem);
    assert(SOC_MEM_IS_VALID(unit, mem));
    assert(soc_attached(unit));

    if (SAL_BOOT_SIMULATION) {
#if defined(ALPM_ENABLE)
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_APACHE_SUPPORT)
        if (index_ptr) {
            *index_ptr = bucket << (soc_alpm_cmn_banks_get(unit) / 2);
        }
#endif
#endif

        return SOC_E_NONE;
    }

    /* Only applicable to ALPM memories */
    if ((mem != L3_DEFIP_ALPM_IPV6_128m) &&
        (mem != L3_DEFIP_ALPM_IPV6_64m) &&
        (mem != L3_DEFIP_ALPM_IPV6_64_1m) &&
        (mem != L3_DEFIP_ALPM_IPV4m) &&
        (mem != L3_DEFIP_ALPM_IPV4_1m)) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_alpm_insert: not supported non-ALPM memories %s\n"),
                   SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    if (copyno == MEM_BLOCK_ANY) {
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }
    if (!SOC_MEM_BLOCK_VALID(unit, mem, copyno)) {
        LOG_WARN(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "soc_mem_alpm_insert: invalid block %d for memory %s\n"),
                  copyno, SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    if (bsl_check(bslLayerSoc, bslSourceSocmem, bslSeverityNormal, unit)) {
        LOG_CLI((BSL_META_U(unit,
                            "Insert table[%s]: banks=%d"), SOC_MEM_NAME(unit, mem),
                 banks_disable));
        if (bsl_check(bslLayerSoc, bslSourceCommon, bslSeverityVerbose, unit)) {
            soc_mem_entry_dump(unit, mem, entry);
        }
        LOG_CLI((BSL_META_U(unit,
                            "\n")));
    }

    entry_dw = soc_mem_entry_words(unit, mem);

    SOC_CONTROL_LOCK(unit);
    SHR_BITSET(&(SOC_CONTROL(unit)->alpm_mem_ops), _SOC_ALPM_INSERT);
    SOC_CONTROL_UNLOCK(unit);

_retry:
    schan_msg_clear(&schan_msg);
    acc_type = SOC_MEM_ACC_TYPE(unit, mem);
    src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
    dst_blk = SOC_BLOCK2SCH(unit, copyno);
    data_byte_len = entry_dw * 4;

    schan_msg.gencmd.address = soc_mem_addr_get(unit, mem, 0, copyno, 0, &at);
    /* Address contains bank disable, and bucket pointer */
    /* Bucket index and bank disable
     * addr[TD2: 13:0,  TH: 12:0,  TH2: 13:0]   - bucket
     * addr[TD2: 17:14, TH: 16:13, TH2: 17:14]  - bank disable[3:0]
     */
#if defined(BCM_TOMAHAWK_SUPPORT)
    if (SOC_IS_TOMAHAWK(unit)) {
        schan_msg.gencmd.address &= ~(0x0001ffff);
        schan_msg.gencmd.address |=
            (((banks_disable & 0xf) << 13) | (bucket & SOC_TH_ALPM_BKT_MASK(unit)));
    } else
#endif
    {
        schan_msg.gencmd.address &= ~(0x0003ffff);
        schan_msg.gencmd.address |= 
            (((banks_disable & 0xf) << 14) | (bucket & SOC_TD2_ALPM_BKT_MASK));
    }

    soc_schan_header_cmd_set(unit, &schan_msg.header, TABLE_INSERT_CMD_MSG,
                             dst_blk, src_blk, acc_type, data_byte_len, 0, 0);

    /* Fill in packet data */
    sal_memcpy(schan_msg.gencmd.data, entry, entry_dw * 4);

    /*
     * Execute S-Channel "table insert" command packet consisting of
     * (header word + address word + entry_dw), and read back
     * (header word + response word + entry_dw) data words.
     */
    SOC_ALPM_LPM_LOCK(unit);
    rv = soc_schan_op(unit, &schan_msg, entry_dw + 2, entry_dw + 2, allow_intr);

    soc_schan_header_status_get(unit, &schan_msg.header, &opcode, NULL, NULL,
                                NULL, NULL, &nack);
    if (opcode != TABLE_INSERT_DONE_MSG) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_alpm_insert: "
                              "invalid S-Channel reply, expected TABLE_INSERT_DONE_MSG:\n")));
        soc_schan_dump(unit, &schan_msg, 1);
        SOC_ALPM_LPM_UNLOCK(unit);
        return SOC_E_INTERNAL;
    }
    type = schan_msg.genresp_v2.response.type;
    index = schan_msg.genresp_v2.response.index;
    data = schan_msg.genresp_v2.data;

    LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
               (BSL_META_U(unit,
                          "SBUS INSERT: nack:%d rv:%d type:%d index:%d \n "),
                           nack, rv, type, index));

    if (_soc_alpm_insert_retry && (nack == 0 && rv == SOC_E_NONE)) {
        if (type == SCHAN_GEN_RESP_TYPE_REPLACED) {
            LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                      "SBUS INSERT failed, replaced at retry %d \n"),
                      _soc_alpm_insert_retry));
            /* After SER actions, the entry in bad pipe could be
             * inserted/replaced, we too consider that as success.
             */
            if (type2 == SCHAN_GEN_RESP_TYPE_INSERTED ||
                type2 == SCHAN_GEN_RESP_TYPE_REPLACED) {
                /* recover the values needed for success ops */
                type  = type2;
                index = index2;
                if (type == SCHAN_GEN_RESP_TYPE_REPLACED) {
                    data  = data2;
                }
            } else {
                /* SBUS fails at the first time (type is error).
                 * We still get index, but lose info of being inserted/replaced
                 * and old entry if it's replaced.
                 */
                LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
                           (BSL_META_U(unit,
                           "SBUS INSERT fails 1st with error. The old entry is lost \n ")));
            }
        }
    }
    if ((nack != 0) || (rv == SOC_E_FAIL)) {
        if (index_ptr) {
            *index_ptr = -1;
        }
        if (type == SCHAN_GEN_RESP_TYPE_FULL) {
            LOG_INFO(BSL_LS_SOC_SOCMEM,
                     (BSL_META_U(unit,
                                 "Insert table[%s]: hash bucket full\n"),
                      SOC_MEM_NAME(unit, mem)));
            rv = SOC_E_FULL;
        } else if (type == SCHAN_GEN_RESP_TYPE_ERROR ||
                   type == SCHAN_GEN_RESP_TYPE_INSERTED ||
                   type == SCHAN_GEN_RESP_TYPE_REPLACED) {
            _soc_alpm_insert_retry++;
            if (type == SCHAN_GEN_RESP_TYPE_INSERTED ||
                type == SCHAN_GEN_RESP_TYPE_REPLACED) {
                type2  = type;
                index2 = index;
                if (type == SCHAN_GEN_RESP_TYPE_REPLACED) {
                    sal_memcpy(data2, schan_msg.genresp_v2.data, entry_dw * 4);
                }
                bkt_ptr = bucket;
            }

            if (SOC_CONTROL(unit)->alpm_insert_retry) {
                (void)_soc_mem_alpm_ser_correction(unit,
                                                   mem,
                                                   schan_msg.gencmd.header.v2.dst_blk,
                                                   schan_msg.gencmd.address,
                                                   copyno, index,
                                                   bkt_ptr);
                if (_soc_alpm_insert_retry < _SOC_ALPM_MEM_OP_RETRY_COUNT) {
                    
                    if (sal_sem_take(SOC_CONTROL(unit)->alpm_insert_retry,
                                 _SOC_ALPM_MEM_OP_RETRY_TO) < 0) {
                        LOG_WARN(BSL_LS_SOC_SOCMEM,
                                 (BSL_META_U(unit,
                                             "unit %d: Retry ALPM insert operation..\n"), unit));
                        goto _retry;
                    } else {
                        LOG_WARN(BSL_LS_SOC_SOCMEM,
                                 (BSL_META_U(unit,
                                             "unit %d: Retry ALPM insert operation..\n"), unit));
                        goto _retry;
                    }
                } else {
                    LOG_ERROR(BSL_LS_SOC_SOCMEM,
                              (BSL_META_U(unit,
                                          "unit %d: Aborting ALPM insert operation "
                                          "due to un-correctable error !!\n"), unit));
                    rv = SOC_E_INTERNAL;
                }
            } else {
                rv = SOC_E_INTERNAL;
            }
        } else {
            rv = SOC_E_FAIL;
        }
    } else {
        if (index_ptr != NULL) {
            *index_ptr = index;
        }
        if (type == SCHAN_GEN_RESP_TYPE_REPLACED) {
            if (old_entry != NULL) {
                sal_memcpy(old_entry, data, entry_dw * sizeof(uint32));
            }
            rv = SOC_E_EXISTS;
        }
        /* Update cache if active */
        cache = SOC_MEM_STATE(unit, mem).cache[copyno];
        vmap = SOC_MEM_STATE(unit, mem).vmap[copyno];
        index_valid = (index >= 0 &&
                       index <= soc_mem_index_max(unit, mem));
        if (index_valid && cache != NULL && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
            sal_memcpy(cache + index * entry_dw, entry, entry_dw * 4);
            CACHE_VMAP_SET(vmap, index);
        }
    }
    SOC_ALPM_LPM_UNLOCK(unit);

    if (!SOC_FAILURE(rv) || (rv == SOC_E_EXISTS)) {
        if (SOC_IS_TRIDENT2(unit) || SOC_IS_TRIDENT2PLUS(unit)) {
            _soc_trident2_alpm_bkt_view_set(unit, index, mem);
        }
#if defined(ALPM_ENABLE)
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_APACHE_SUPPORT)
        else {
            soc_alpm_cmn_bkt_view_set(unit, index, mem);
        }
#endif
#endif
    }
    SOC_CONTROL_LOCK(unit);
    SHR_BITCLR(&(SOC_CONTROL(unit)->alpm_mem_ops), _SOC_ALPM_INSERT);
    SOC_CONTROL_UNLOCK(unit);

    return rv;
}

/*
 * Function:
 *      soc_mem_alpm_delete
 * Purpose:
 *      Delete an ALPM entry
 * Parameters:
 *      unit - StrataSwitch unit #
 *      banks - For dual hashing, which halves are selected (inverted)
 *              Note: used to create a bitmap for ISM mems. (All banks: -1)
 *      entry - entry to delete
 *      old_entry - old entry if entry was found and deleted
 * Returns:
 *      SOC_E_NONE - success
 *      SOC_E_NOT_FOUND - full
 *      SOC_E_BUSY - modfifo full
 * Notes:
 *      Uses hardware deletion; sends an DELETE message over the
 *      S-Channel.
 */

int
soc_mem_alpm_delete(int unit, soc_mem_t mem, int bucket,
                    int copyno, int32 banks_disable,
                    void *entry, void *old_entry, int *index_ptr)
{
    schan_msg_t schan_msg;
    uint8       at, *vmap;
    uint32      *data, *cache;
    int         rv, entry_dw, index_valid;
    int         type, index, allow_intr=0;
    int         _soc_alpm_delete_retry = 0;
    void        *null_entry = soc_mem_entry_null(unit, mem);
    int         src_blk, dst_blk, acc_type, data_byte_len;
    int         opcode, nack;
    uint32      bank_ignore_mask;
    int         type2 = -1, index2 = 0;
    uint32      data2[CMIC_SCHAN_WORDS_ALLOC - 2];
    int         bkt_ptr = -1;

    SOC_MEM_ALIAS_TO_ORIG(unit,mem);
    assert(SOC_MEM_IS_VALID(unit, mem));
    assert(soc_attached(unit));

    if (SAL_BOOT_SIMULATION) {
#if defined(ALPM_ENABLE)
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_APACHE_SUPPORT)
        if (index_ptr) {
            *index_ptr = bucket << (soc_alpm_cmn_banks_get(unit) / 2);
        }
#endif
#endif
        return SOC_E_NONE;
    }

    /* Only applicable to ALPM memories */
    if ((mem != L3_DEFIP_ALPM_IPV6_128m) &&
        (mem != L3_DEFIP_ALPM_IPV6_64m) &&
        (mem != L3_DEFIP_ALPM_IPV6_64_1m) &&
        (mem != L3_DEFIP_ALPM_IPV4m) &&
        (mem != L3_DEFIP_ALPM_IPV4_1m)) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_alpm_delete: not supported non-ALPM memories %s\n"),
                   SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }
#ifdef BCM_SIRIUS_SUPPORT
    if (SOC_IS_SIRIUS(unit)) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_generic_delete: not supported on %s\n"),
                   SOC_CHIP_STRING(unit)));
        return SOC_E_UNAVAIL;
    }
#endif

    if (copyno == MEM_BLOCK_ANY) {
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }
    if (!SOC_MEM_BLOCK_VALID(unit, mem, copyno)) {
        LOG_WARN(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "soc_mem_alpm_delete: invalid block %d for memory %s\n"),
                  copyno, SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    if (bsl_check(bslLayerSoc, bslSourceSocmem, bslSeverityNormal, unit)) {
        LOG_CLI((BSL_META_U(unit,
                            "Delete table[%s]: banks=%d"), SOC_MEM_NAME(unit, mem),
                 banks_disable));
        if (bsl_check(bslLayerSoc, bslSourceCommon, bslSeverityVerbose, unit)) {
            soc_mem_entry_dump(unit, mem, entry);
        }
        LOG_CLI((BSL_META_U(unit,
                            "\n")));
    }

    entry_dw = soc_mem_entry_words(unit, mem);

    SOC_CONTROL_LOCK(unit);
    SHR_BITSET(&(SOC_CONTROL(unit)->alpm_mem_ops), _SOC_ALPM_DELETE);
    SOC_CONTROL_UNLOCK(unit);

_retry:
    schan_msg_clear(&schan_msg);
    acc_type = SOC_MEM_ACC_TYPE(unit, mem);
    src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
    dst_blk = SOC_BLOCK2SCH(unit, copyno);
#ifdef BCM_ISM_SUPPORT
    if (soc_feature(unit, soc_feature_ism_memory)) {
        /* don't set bank id in cos */
        bank_ignore_mask = 0;
    } else
#endif /* BCM_ISM_SUPPORT */
    {
        bank_ignore_mask = banks_disable & 0x3;
    }
    data_byte_len = entry_dw * 4;

    schan_msg.gencmd.address = soc_mem_addr_get(unit, mem, 0, copyno, 0, &at);

    /* Address contains bank disable, and bucket pointer */
    /* Bucket index and bank disable
     * addr[TD2: 13:0,  TH: 12:0,  TH2: 13:0]   - bucket
     * addr[TD2: 17:14, TH: 16:13, TH2: 17:14]  - bank disable[3:0]
     */
#if defined(BCM_TOMAHAWK_SUPPORT)
    if (SOC_IS_TOMAHAWK(unit)) {
        schan_msg.gencmd.address &= ~(0x0001ffff);
        schan_msg.gencmd.address |=
            (((banks_disable & 0xf) << 13) | (bucket & SOC_TH_ALPM_BKT_MASK(unit)));
    } else
#endif
    {
        schan_msg.gencmd.address &= ~(0x0003ffff);
        schan_msg.gencmd.address |= 
            (((banks_disable & 0xf) << 14) | (bucket & SOC_TD2_ALPM_BKT_MASK));
    }

    soc_schan_header_cmd_set(unit, &schan_msg.header, TABLE_DELETE_CMD_MSG,
                             dst_blk, src_blk, acc_type, data_byte_len, 0,
                             bank_ignore_mask);

    /* Fill in packet data */
    sal_memcpy(schan_msg.gencmd.data, entry, entry_dw * 4);

    if (SOC_IS_SAND(unit)) {
        allow_intr = 1;
    }

    /*
     * Execute S-Channel "table delete" command packet consisting of
     * (header word + address word + entry_dw), and read back
     * (header word + response word + entry_dw) data words.
     */
    rv = soc_schan_op(unit, &schan_msg, entry_dw + 2, entry_dw + 2, allow_intr);

    soc_schan_header_status_get(unit, &schan_msg.header, &opcode, NULL, NULL,
                                NULL, NULL, &nack);
    if (opcode != TABLE_DELETE_DONE_MSG) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_generic_delete: "
                              "invalid S-Channel reply, expected TABLE_DELETE_DONE_MSG:\n")));
        soc_schan_dump(unit, &schan_msg, 1);
        return SOC_E_INTERNAL;
    }

    type = schan_msg.genresp_v2.response.type;
    index = schan_msg.genresp_v2.response.index;
    data = schan_msg.genresp_v2.data;

    LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
              (BSL_META_U(unit,
                          "SBUS DELETE: nack:%d rv:%d type:%d index:%d \n"),
                          nack, rv, type, index));

    if (_soc_alpm_delete_retry && (nack != 0 || rv == SOC_E_FAIL)) {
        if (type == SCHAN_GEN_RESP_TYPE_NOT_FOUND) {
            LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                      "SBUS DELETE failed, not found at retry %d \n"),
                      _soc_alpm_delete_retry));
            /* After SER actions, the entry in bad pipe could be deleted.
             * We too consider that as success.
             */
            nack = 0;
            rv = SOC_E_NONE;
            if (type2 == SCHAN_GEN_RESP_TYPE_DELETED) {
                /* recover the values needed for success ops */
                type  = type2;
                index = index2;
                data  = data2;
            } else {
                /* SBUS fails at the first time (type is error).
                 * We lose the deleted entry's contect and its index.
                 */
                LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
                          (BSL_META_U(unit,
                          "SBUS INSERT fails 1st with error. The deleted index"
                          " and old entry is lost \n")));
            }
        }
    }
    if ((nack != 0) || (rv == SOC_E_FAIL)) {
        if (index_ptr) {
            *index_ptr = -1;
        }
        if (type == SCHAN_GEN_RESP_TYPE_NOT_FOUND) {
            LOG_INFO(BSL_LS_SOC_SOCMEM,
                     (BSL_META_U(unit,
                                 "Delete table[%s]: Not found\n"),
                      SOC_MEM_NAME(unit, mem)));
            rv = SOC_E_NOT_FOUND;
        } else if (type == SCHAN_GEN_RESP_TYPE_ERROR ||
                   type == SCHAN_GEN_RESP_TYPE_DELETED) {

            _soc_alpm_delete_retry++;
            if (type == SCHAN_GEN_RESP_TYPE_DELETED) {
                type2  = type;
                index2 = index;
                sal_memcpy(data2, schan_msg.genresp_v2.data, entry_dw * 4);
                bkt_ptr = bucket;
            }
            if (SOC_CONTROL(unit)->alpm_delete_retry) {
                (void)_soc_mem_alpm_ser_correction(unit,
                                                   mem,
                                                   schan_msg.gencmd.header.v2.dst_blk,
                                                   schan_msg.gencmd.address,
                                                   copyno, index,
                                                   bkt_ptr);
                if (_soc_alpm_delete_retry < _SOC_ALPM_MEM_OP_RETRY_COUNT) {
                    
                    if (sal_sem_take(SOC_CONTROL(unit)->alpm_delete_retry,
                                     _SOC_ALPM_MEM_OP_RETRY_TO) < 0) {
                        LOG_WARN(BSL_LS_SOC_SOCMEM,
                                 (BSL_META_U(unit,
                                             "unit %d: Retry ALPM delete operation..\n"), unit));
                        goto _retry;
                    } else {
                        LOG_WARN(BSL_LS_SOC_SOCMEM,
                                 (BSL_META_U(unit,
                                             "unit %d: Retry ALPM delete operation..\n"), unit));
                        goto _retry;
                    }
                } else {
                    LOG_ERROR(BSL_LS_SOC_SOCMEM,
                              (BSL_META_U(unit,
                                          "unit %d: Aborting ALPM delete operation "
                                          "due to un-correctable error !!\n"), unit));
                    rv = SOC_E_INTERNAL;
                }
            } else {
                rv = SOC_E_INTERNAL;
            }
        } else {
            rv = SOC_E_FAIL;
        }
    } else {
        if (old_entry != NULL) {
            sal_memcpy(old_entry, data, entry_dw * sizeof(uint32));
        }
        if (index_ptr != NULL) {
            *index_ptr = index;
        }
        /* Update cache if active */
        cache = SOC_MEM_STATE(unit, mem).cache[copyno];
        vmap = SOC_MEM_STATE(unit, mem).vmap[copyno];
        index_valid = (index >= 0 &&
                       index <= soc_mem_index_max(unit, mem));

        if (index_valid && cache != NULL && CACHE_VMAP_TST(vmap, index)
            && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
            sal_memcpy(cache + index * entry_dw, null_entry, entry_dw * 4);
            CACHE_VMAP_SET(vmap, index);
        }
    }

    SOC_CONTROL_LOCK(unit);
    SHR_BITCLR(&(SOC_CONTROL(unit)->alpm_mem_ops), _SOC_ALPM_DELETE);
    SOC_CONTROL_UNLOCK(unit);

    return rv;
}
#endif /* ALPM_ENABLE */

#ifdef BCM_TRX_SUPPORT
#define HASH_DUAL   0
#define HASH_SHARED 1
#define HASH_MULTI  2
STATIC int
_soc_mem_dual_hash_get(int unit, soc_mem_t mem, int hash_sel, int bank,
                       void *entry_data);

STATIC int
soc_hash_mem_bank_type_get (int unit, soc_mem_t mem, uint32* type)
{
#ifdef BCM_TRX_SUPPORT
    if (SOC_IS_TRX(unit)) {
        _SOC_MEM_REPLACE_MEM(unit, mem);
        switch (mem) {
            /* Intentional fall through */
            /* coverity[unterminated_case] */
            case MPLS_ENTRYm:
                if (SOC_IS_SC_CQ(unit)) {
                    return SOC_E_UNAVAIL;
                }
            case VLAN_XLATEm:
            /* Intentional fall through */
            /* coverity[unterminated_case] */
            case EGR_VLAN_XLATEm:
#ifdef BCM_ISM_SUPPORT
                if (soc_feature(unit, soc_feature_ism_memory) &&
                      soc_mem_is_mview(unit, mem)) {
                    *type = HASH_MULTI;
                    return SOC_E_NONE;
                }
#endif
            case VLAN_MACm:
            case AXP_WRX_WCDm:
            case AXP_WRX_SVP_ASSIGNMENTm:
#ifdef BCM_TRIDENT2_SUPPORT
            case ING_VP_VLAN_MEMBERSHIPm:
            case EGR_VP_VLAN_MEMBERSHIPm:
            case ING_DNAT_ADDRESS_TYPEm:
            case L2_ENDPOINT_IDm:
            case ENDPOINT_QUEUE_MAPm:
#endif
#ifdef BCM_KATANA2_SUPPORT
            case EGR_MP_GROUPm:
#endif 
#ifdef BCM_SABER2_SUPPORT
            case MP_GROUPm:
#endif
                *type = HASH_DUAL;
                return SOC_E_NONE;
#ifdef BCM_ISM_SUPPORT
            case L2_ENTRY_1m:
            case L2_ENTRY_2m:
            case L3_ENTRY_1m:
            case L3_ENTRY_2m:
            case L3_ENTRY_4m:
            case VLAN_XLATE_EXTDm:
            case MPLS_ENTRY_EXTDm:
                if (soc_feature(unit, soc_feature_ism_memory) &&
                      soc_mem_is_mview(unit, mem)) {
                    *type = HASH_MULTI;
                    return SOC_E_NONE;
                }
                break;
#endif /* BCM_ISM_SUPPORT */
            default:
                break;
            }
        }
#endif /* BCM_TRX_SUPPORT */

#ifdef BCM_FIREBOLT_SUPPORT
    if (SOC_IS_FBX(unit)) {
        switch (mem) {
        case L2Xm:
#ifdef BCM_TRIDENT2_SUPPORT
            if (soc_feature(unit, soc_feature_shared_hash_mem)) {
                *type = HASH_SHARED;
                return SOC_E_NONE;
            }
#endif /* BCM_TRIDENT2_SUPPORT */
#if defined(BCM_FIREBOLT2_SUPPORT) || defined(BCM_RAVEN_SUPPORT) || \
    defined(BCM_TRX_SUPPORT)
            if (soc_feature(unit, soc_feature_dual_hash)) {
                *type = HASH_DUAL;
                return SOC_E_NONE;
            }
#endif
            /* Insert via ARL_INSERT_CMD_MSG, not TABLE_INSERT_CMD_MSG */
            break;
#if defined(INCLUDE_L3)
        case L3_ENTRY_ONLYm:
        case L3_ENTRY_IPV4_UNICASTm:
        case L3_ENTRY_IPV4_MULTICASTm:
        case L3_ENTRY_IPV6_UNICASTm:
        case L3_ENTRY_IPV6_MULTICASTm:
#ifdef BCM_TRIDENT2_SUPPORT
            if (soc_feature(unit, soc_feature_shared_hash_mem)) {
                *type = HASH_SHARED;
                return SOC_E_NONE;
            }
#endif /* BCM_TRIDENT2_SUPPORT */
#if defined(BCM_FIREBOLT2_SUPPORT) || defined(BCM_TRX_SUPPORT) || \
    defined(BCM_RAVEN_SUPPORT)
            if (soc_feature(unit, soc_feature_dual_hash)) {
                *type = HASH_DUAL;
                return SOC_E_NONE;
            }
#endif /* BCM_FIREBOLT2_SUPPORT || BCM_RAVEN_SUPPORT || BCM_TRX_SUPPORT */
#endif /* INCLUDE_L3 */
            /* Insert via L3_INSERT_CMD_MSG, not TABLE_INSERT_CMD_MSG */
            break;
        case VLAN_MACm:
#if defined(BCM_RAVEN_SUPPORT)
            if (soc_feature(unit, soc_feature_dual_hash) &&
                (SOC_IS_RAVEN(unit) || SOC_IS_HAWKEYE(unit))) {
                *type = HASH_DUAL;
                return SOC_E_NONE;
            } else
#endif
            {
                /* Via memory read/write */
            }
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
            if (soc_feature(unit, soc_feature_shared_hash_mem)) {
                *type = HASH_SHARED;
                return SOC_E_NONE;
            }
#endif
        default:
            break;
        }
    }
#endif
#ifdef BCM_ISM_SUPPORT
        if (soc_feature(unit, soc_feature_ism_memory) &&
              soc_mem_is_hashed(unit, mem) &&
            soc_mem_is_mview(unit, mem)) {
            *type = HASH_MULTI;
            return SOC_E_NONE;
        }
#endif /* BCM_ISM_SUPPORT */

    return SOC_E_INTERNAL;
}


STATIC int
soc_hash_mem_dual_hash_info_get (int unit, soc_mem_t mem, dual_hash_info_t *hash_info)
{
    int rv = SOC_E_INTERNAL;

    switch (mem) {
    case L2Xm:
        SOC_IF_ERROR_RETURN
            (soc_fb_l2x_entry_bank_hash_sel_get(unit, 0,
                                                &(hash_info->hash_sel0)));
        SOC_IF_ERROR_RETURN
            (soc_fb_l2x_entry_bank_hash_sel_get(unit, 1,
                                                &(hash_info->hash_sel1)));
        hash_info->bucket_size = SOC_L2X_BUCKET_SIZE;
        hash_info->base_mem = mem;
        rv = SOC_E_NONE;
        break;
#ifdef BCM_TRX_SUPPORT
    case VLAN_XLATEm:
        if (!SOC_IS_TRX(unit)) {
            break;
        }
#endif
#if defined(BCM_RAVEN_SUPPORT) || defined(BCM_TRX_SUPPORT)
    case VLAN_MACm:
        if (SOC_IS_FIREBOLT2(unit)) {
            break;
        }
#ifdef BCM_TRX_SUPPORT
#ifdef BCM_TRIDENT2_SUPPORT
        if (SOC_IS_TD2_TT2(unit)) {
            SOC_IF_ERROR_RETURN
                (soc_td2_hash_sel_get(unit, mem, 0, &hash_info->hash_sel0));
            SOC_IF_ERROR_RETURN
                (soc_td2_hash_sel_get(unit, mem, 1, &hash_info->hash_sel1));
            hash_info->bucket_size = SOC_VLAN_XLATE_BUCKET_SIZE;
            hash_info->base_mem = mem;
            rv = SOC_E_NONE;
        } else
#endif
        {
            if (mem == VLAN_XLATEm) {
                SOC_IF_ERROR_RETURN
                    (soc_tr_hash_sel_get(unit, mem, 0, &hash_info->hash_sel0));
                SOC_IF_ERROR_RETURN
                    (soc_tr_hash_sel_get(unit, mem, 1, &hash_info->hash_sel1));
                hash_info->bucket_size = SOC_VLAN_XLATE_BUCKET_SIZE;
                hash_info->base_mem = mem;
                rv = SOC_E_NONE;
            } else
#endif
            {
                SOC_IF_ERROR_RETURN
                    (soc_fb_rv_vlanmac_hash_sel_get(unit, 0, &(hash_info->hash_sel0)));

                SOC_IF_ERROR_RETURN
                    (soc_fb_rv_vlanmac_hash_sel_get(unit, 1, &(hash_info->hash_sel1)));
                hash_info->bucket_size = SOC_VLAN_MAC_BUCKET_SIZE;
                hash_info->base_mem = mem;
                rv = SOC_E_NONE;
            }
        }
        break;
#endif

#if defined(BCM_TRX_SUPPORT)
    case MPLS_ENTRYm:
        SOC_IF_ERROR_RETURN
            (soc_tr_hash_sel_get(unit, mem, 0, &hash_info->hash_sel0));
        SOC_IF_ERROR_RETURN
            (soc_tr_hash_sel_get(unit, mem, 1, &hash_info->hash_sel1));
        hash_info->bucket_size = SOC_MPLS_ENTRY_BUCKET_SIZE;
        hash_info->base_mem = mem;
        rv = SOC_E_NONE;
        break;
    case EGR_VLAN_XLATEm:
        SOC_IF_ERROR_RETURN
            (soc_tr_hash_sel_get(unit, mem, 0, &hash_info->hash_sel0));
        SOC_IF_ERROR_RETURN
            (soc_tr_hash_sel_get(unit, mem, 1, &hash_info->hash_sel1));
        hash_info->bucket_size = SOC_EGR_VLAN_XLATE_BUCKET_SIZE;
        hash_info->base_mem = mem;
        rv = SOC_E_NONE;
        break;
    case AXP_WRX_WCDm:
    case AXP_WRX_SVP_ASSIGNMENTm:
        SOC_IF_ERROR_RETURN
            (soc_tr_hash_sel_get(unit, mem, 0, &hash_info->hash_sel0));
        SOC_IF_ERROR_RETURN
            (soc_tr_hash_sel_get(unit, mem, 1, &hash_info->hash_sel1));
        hash_info->bucket_size = SOC_WLAN_BUCKET_SIZE;
        hash_info->base_mem = mem;
        rv = SOC_E_NONE;
        break;
#ifdef BCM_TRIDENT2_SUPPORT
    case ING_VP_VLAN_MEMBERSHIPm:
        SOC_IF_ERROR_RETURN
            (soc_td2_hash_sel_get(unit, mem, 0, &hash_info->hash_sel0));
        SOC_IF_ERROR_RETURN
            (soc_td2_hash_sel_get(unit, mem, 1, &hash_info->hash_sel1));
        hash_info->bucket_size = SOC_ING_VP_VLAN_MEMBER_BUCKET_SIZE;
        hash_info->base_mem = mem;
        rv = SOC_E_NONE;
        break;
    case EGR_VP_VLAN_MEMBERSHIPm:
        SOC_IF_ERROR_RETURN
            (soc_td2_hash_sel_get(unit, mem, 0, &hash_info->hash_sel0));
        SOC_IF_ERROR_RETURN
            (soc_td2_hash_sel_get(unit, mem, 1, &hash_info->hash_sel1));
        hash_info->bucket_size = SOC_EGR_VP_VLAN_MEMBER_BUCKET_SIZE;
        hash_info->base_mem = mem;
        rv = SOC_E_NONE;
        break;
    case ING_DNAT_ADDRESS_TYPEm:
        SOC_IF_ERROR_RETURN
            (soc_td2_hash_sel_get(unit, mem, 0, &hash_info->hash_sel0));
        SOC_IF_ERROR_RETURN
            (soc_td2_hash_sel_get(unit, mem, 1, &hash_info->hash_sel1));
        hash_info->bucket_size = SOC_ING_DNAT_ADDRESS_TYPE_BUCKET_SIZE;
        hash_info->base_mem = mem;
        rv = SOC_E_NONE;
        break;
    case L2_ENDPOINT_IDm:
        SOC_IF_ERROR_RETURN
            (soc_td2_hash_sel_get(unit, mem, 0, &hash_info->hash_sel0));
        SOC_IF_ERROR_RETURN
            (soc_td2_hash_sel_get(unit, mem, 1, &hash_info->hash_sel1));
        hash_info->bucket_size = SOC_L2_ENDPOINT_ID_BUCKET_SIZE;
        hash_info->base_mem = mem;
        rv = SOC_E_NONE;
        break;
    case ENDPOINT_QUEUE_MAPm:
        SOC_IF_ERROR_RETURN
            (soc_td2_hash_sel_get(unit, mem, 0, &hash_info->hash_sel0));
        SOC_IF_ERROR_RETURN
            (soc_td2_hash_sel_get(unit, mem, 1, &hash_info->hash_sel1));
        hash_info->bucket_size = SOC_ENDPOINT_QUEUE_MAP_BUCKET_SIZE;
        hash_info->base_mem = mem;
        rv = SOC_E_NONE;
        break;
#endif
#ifdef BCM_KATANA2_SUPPORT
    case EGR_MP_GROUPm:
        
        SOC_IF_ERROR_RETURN
            (soc_tr_hash_sel_get(unit, mem, 0, &hash_info->hash_sel0));
        SOC_IF_ERROR_RETURN
            (soc_tr_hash_sel_get(unit, mem, 1, &hash_info->hash_sel1));
        hash_info->bucket_size = SOC_EGR_MP_GROUP_BUCKET_SIZE;
        hash_info->base_mem = mem;
        rv = SOC_E_NONE;
        break;
#endif
#ifdef BCM_SABER2_SUPPORT
    case MP_GROUPm:
        
        SOC_IF_ERROR_RETURN
            (soc_tr_hash_sel_get(unit, mem, 0, &hash_info->hash_sel0));
        SOC_IF_ERROR_RETURN
            (soc_tr_hash_sel_get(unit, mem, 1, &hash_info->hash_sel1));
        hash_info->bucket_size = SOC_EGR_MP_GROUP_BUCKET_SIZE;
        hash_info->base_mem = mem;
        rv = SOC_E_NONE;
        break;
#endif
#endif /* BCM_TRX_SUPPORT */
#if defined(INCLUDE_L3)
    case L3_ENTRY_ONLYm:
    case L3_ENTRY_IPV4_UNICASTm:
    case L3_ENTRY_IPV4_MULTICASTm:
    case L3_ENTRY_IPV6_UNICASTm:
    case L3_ENTRY_IPV6_MULTICASTm:
#if defined(BCM_FIREBOLT2_SUPPORT) || defined(BCM_TRX_SUPPORT) || \
    defined(BCM_RAVEN_SUPPORT)
        if (soc_feature(unit, soc_feature_dual_hash)) {
            SOC_IF_ERROR_RETURN
                (soc_fb_l3x_entry_bank_hash_sel_get(unit, 0,
                                            &(hash_info->hash_sel0)));
            SOC_IF_ERROR_RETURN
                (soc_fb_l3x_entry_bank_hash_sel_get(unit, 1,
                                            &(hash_info->hash_sel1)));
            hash_info->bucket_size = SOC_L3X_BUCKET_SIZE(unit);
            hash_info->base_mem = L3_ENTRY_IPV4_UNICASTm;
#if defined(BCM_HURRICANE2_SUPPORT) || defined(BCM_KATANA2_SUPPORT) || defined(BCM_TRIDENT_SUPPORT)
            if (soc_feature(unit, soc_feature_hr2_dual_hash) ||
                SOC_IS_KATANA2(unit) || SOC_IS_TRIDENT(unit)) {
                hash_info->bucket_size =
                    _soc_mem_hash_entries_per_bkt(unit, mem);
            }
#endif /* BCM_HURRICANE2_SUPPORT */
            rv = SOC_E_NONE;
        }
#endif
        break;
#endif
    default:
        break;
    }
    return rv;
}

STATIC int
soc_ser_hash_mem_bank_correction(int unit, soc_mem_t mem, schan_msg_t* schan_msg,
                                 uint32 base_index, uint32 size)
{
    int i;
    uint32 recovery_view_index;
    _soc_ser_correct_info_t spci;
    sal_memset(&spci, 0, sizeof(spci));

    spci.flags = SOC_SER_SRC_MEM | SOC_SER_REG_MEM_KNOWN;
    spci.reg = INVALIDr;
    spci.mem = mem;
    spci.pipe_num = SOC_PIPE_ANY;
    spci.blk_type = SOC_BLOCK_ANY;
    spci.sblk = 0;
    spci.addr = schan_msg->gencmd.address;
    recovery_view_index = base_index;

#if defined(BCM_TOMAHAWK_SUPPORT)
    if (SOC_IS_TOMAHAWKX(unit)) {
        switch (mem) {
        case L3_ENTRY_ONLYm:
        case L3_ENTRY_IPV4_UNICASTm:
            spci.mem = L3_ENTRY_ONLY_ECCm;
            recovery_view_index = base_index;
            break;
        case L3_ENTRY_IPV4_MULTICASTm:
        case L3_ENTRY_IPV6_UNICASTm:
            spci.mem = L3_ENTRY_ONLY_ECCm;
            recovery_view_index = base_index * 2;
            break;
        case L3_ENTRY_IPV6_MULTICASTm:
            spci.mem = L3_ENTRY_ONLY_ECCm;
            recovery_view_index = base_index * 4;
            break;
        case L2Xm:
            spci.mem = L2_ENTRY_ONLY_ECCm;
            recovery_view_index = base_index;
            break;
        case VLAN_MACm:
        case VLAN_XLATEm:
            spci.mem = VLAN_XLATE_ECCm;
            recovery_view_index = base_index;
            break;
        case EGR_VLAN_XLATEm:
            spci.mem = EGR_VLAN_XLATE_ECCm;
            recovery_view_index = base_index;
            break;
        case MPLS_ENTRYm:
            spci.mem = MPLS_ENTRY_ECCm;
            recovery_view_index = base_index;
            break;
        case EXACT_MATCH_2m:
            spci.mem = FPEM_ECCm;
            recovery_view_index = base_index/2;
            break;
        case EXACT_MATCH_2_PIPE0m:
            spci.pipe_num = 0;
            spci.mem = FPEM_ECCm;
            recovery_view_index = base_index/2;
            break;
        case EXACT_MATCH_2_PIPE1m:
            spci.pipe_num = 1;
            spci.mem = FPEM_ECCm;
            recovery_view_index = base_index/2;
            break;
        case EXACT_MATCH_2_PIPE2m:
            spci.pipe_num = 2;
            spci.mem = FPEM_ECCm;
            recovery_view_index = base_index/2;
            break;
        case EXACT_MATCH_2_PIPE3m:
            spci.pipe_num = 3;
            spci.mem = FPEM_ECCm;
            recovery_view_index = base_index/2;
            break;
        case EXACT_MATCH_4m:
            spci.mem = FPEM_ECCm;
            recovery_view_index = base_index/4;
            break;
        case EXACT_MATCH_4_PIPE0m:
            spci.pipe_num = 0;
            spci.mem = FPEM_ECCm;
            recovery_view_index = base_index/4;
            break;
        case EXACT_MATCH_4_PIPE1m:
            spci.pipe_num = 1;
            spci.mem = FPEM_ECCm;
            recovery_view_index = base_index/4;
            break;
        case EXACT_MATCH_4_PIPE2m:
            spci.pipe_num = 2;
            spci.mem = FPEM_ECCm;
            recovery_view_index = base_index/4;
            break;
        case EXACT_MATCH_4_PIPE3m:
            spci.pipe_num = 3;
            spci.mem = FPEM_ECCm;
            recovery_view_index = base_index/4;
            break;
        default:
            break;
        }
    } else
#endif
    {
        switch (mem) {
        case L3_ENTRY_ONLYm:
        case L3_ENTRY_IPV4_UNICASTm:
            spci.mem = L3_ENTRY_ONLYm;
            recovery_view_index = base_index;
            break;
        case L3_ENTRY_IPV4_MULTICASTm:
        case L3_ENTRY_IPV6_UNICASTm:
            spci.mem = L3_ENTRY_ONLYm;
            recovery_view_index = base_index * 2;
            break;
        case L3_ENTRY_IPV6_MULTICASTm:
            spci.mem = L3_ENTRY_ONLYm;
            recovery_view_index = base_index * 4;
            break;
        default:
            break;
        }
    }

    /* If one entry has parity error, mem access in other
     * entry of same bucket will trigger SBUS NACK. So
     * all entties need to be corrected. */
    for (i = 0; i < size; i++) {
        spci.index = recovery_view_index + i;
        (void) soc_ser_correction(unit, &spci);
    }
    return SOC_E_NONE;
}


STATIC int
soc_hash_mem_inline_recovery(int unit, soc_mem_t mem, schan_msg_t* schan_msg,
                             int32 bank, void *entry)
{
    uint32 type, bank_idx, bucket_index;
    int half_bucket, hash_sel, hash_base;
#if defined(BCM_TRIDENT2_SUPPORT)
    uint32 bucket = 0;
    int index, num_banks, bank_ids[7];
#endif
#ifdef BCM_ISM_SUPPORT
    int ism_index;
    uint8  num_ent;
    uint32 result;
    _soc_ism_mem_banks_t mem_banks;
#endif
    dual_hash_info_t hash_info = {0};

    SOC_IF_ERROR_RETURN(soc_hash_mem_bank_type_get(unit, mem, &type));

    switch (type) {
    case HASH_DUAL:
        SOC_IF_ERROR_RETURN(soc_hash_mem_dual_hash_info_get(unit, mem, &hash_info));
        half_bucket = hash_info.bucket_size / 2;
        /* For dual bank hash table, one entry may be located in one of two
         * different hash buckets, thus different entry index in two banks,
         * any parity error in these two buckets will cause insert/delete fail.
         * We have to restore both buckets.
         */
        for (bank_idx = 0; bank_idx < 2; bank_idx++) {
            hash_sel = (bank_idx == 0) ? hash_info.hash_sel0 : hash_info.hash_sel1;
            hash_base = _soc_mem_dual_hash_get(unit, mem, hash_sel, bank_idx,
                                               entry);
            if (hash_base == -1) {
                return SOC_E_INTERNAL;
            }
            bucket_index = hash_base * hash_info.bucket_size + bank_idx * half_bucket;
            SOC_IF_ERROR_RETURN(soc_ser_hash_mem_bank_correction(unit, mem,
                                                     schan_msg, bucket_index,
                                                     half_bucket));
        }
        break;
    case HASH_SHARED:
#if defined(BCM_TRIDENT2_SUPPORT)

#ifdef BCM_APACHE_SUPPORT
        if (SOC_IS_APACHE(unit)) {
            SOC_IF_ERROR_RETURN
                (soc_apache_hash_bank_count_get(unit, mem, &num_banks));
        } else
#endif /* APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
        if (SOC_IS_TOMAHAWKX(unit)) {
            SOC_IF_ERROR_RETURN
                (soc_tomahawk_hash_bank_count_get(unit, mem, &num_banks));
        } else
#endif /* BCM_TOMAHAWK_SUPPORT */
        {
            SOC_IF_ERROR_RETURN
                (soc_trident2_hash_bank_count_get(unit, mem, &num_banks));
        }
        for (bank_idx = 0; bank_idx < num_banks; bank_idx++) {

#ifdef BCM_APACHE_SUPPORT
            if (SOC_IS_APACHE(unit)) {
                SOC_IF_ERROR_RETURN
                    (soc_apache_hash_bank_number_get(unit, mem, bank_idx,
                                                       &bank_ids[bank_idx]));
                SOC_IF_ERROR_RETURN
                    (soc_ap_hash_bucket_get(unit, mem, bank_ids[bank_idx],
                                            entry, &bucket));
                index = soc_ap_hash_index_get(unit, mem, bank_ids[bank_idx], bucket);
            } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
            if (SOC_IS_TOMAHAWKX(unit)) {
                SOC_IF_ERROR_RETURN
                    (soc_tomahawk_hash_bank_number_get(unit, mem, bank_idx,
                                                       &bank_ids[bank_idx]));
                SOC_IF_ERROR_RETURN
                    (soc_th_hash_bucket_get(unit, mem, bank_ids[bank_idx],
                                            entry, &bucket));
                index = soc_th_hash_index_get(unit, mem, bank_ids[bank_idx], bucket);
            } else
#endif /* BCM_TOMAHAWK_SUPPORT */
            {
                SOC_IF_ERROR_RETURN
                    (soc_trident2_hash_bank_number_get(unit, mem, bank_idx,
                                                       &bank_ids[bank_idx]));
                SOC_IF_ERROR_RETURN
                    (soc_hash_bucket_get(unit, mem, bank_ids[bank_idx],
                                         entry, &bucket));
                index = soc_hash_index_get(unit, mem, bank_ids[bank_idx], bucket);
            }
            SOC_IF_ERROR_RETURN(soc_ser_hash_mem_bank_correction(unit, mem,
                                                     schan_msg, index, 4));
        }
#endif
        break;
    case HASH_MULTI:
#ifdef BCM_ISM_SUPPORT
        sal_memset(&mem_banks, 0, sizeof(mem_banks));
        SOC_IF_ERROR_RETURN(soc_ism_get_banks_for_mem(unit, mem, mem_banks.banks,
                                       mem_banks.bank_size, &mem_banks.count));
        for (bank_idx = 0; bank_idx < mem_banks.count; bank_idx++) {
            bank = (uint32)1 << mem_banks.banks[bank_idx];
            SOC_IF_ERROR_RETURN(soc_generic_hash(unit, mem, entry, bank, 0, &ism_index,
                                                 &result, &bucket_index, &num_ent));
            SOC_IF_ERROR_RETURN(soc_ser_hash_mem_bank_correction(unit, mem,
                                                     schan_msg, bucket_index * num_ent,
                                                     num_ent));
        }
#endif
        break;
    default:
        return SOC_E_FAIL;
    }
    return SOC_E_NONE;
}
#endif

#if defined(BCM_TOMAHAWK_SUPPORT)
/* TH:
 * For insert, delete operations to all hash tables, SER event could be only for 
 * functional view (fv). rsp_word indicates index to _ECC view and not to funct
 * view - and ser_correction should be invoked with correct _ECC view.
 */
STATIC int
_soc_inline_ser_mem_remap(int unit, soc_mem_t *mem, int *pipe, int *iratio)
{
    int out_pipe = SOC_PIPE_ANY, out_iratio = 1;
    soc_mem_t out_mem;

    if (NULL == mem || NULL == pipe || NULL == iratio) {
        return SOC_E_PARAM;
    }
    switch (*mem) {
    case L3_ENTRY_IPV4_UNICASTm:
    case L3_ENTRY_ONLYm:
        out_mem = L3_ENTRY_ONLY_ECCm;
        break;
    case L3_ENTRY_IPV4_MULTICASTm:
    case L3_ENTRY_IPV6_UNICASTm:
        out_mem = L3_ENTRY_ONLY_ECCm;
        out_iratio = 2;
        break;
    case L3_ENTRY_IPV6_MULTICASTm:
        out_mem = L3_ENTRY_ONLY_ECCm;
        out_iratio = 4;
        break;
    case L2Xm:
        out_mem = L2_ENTRY_ONLY_ECCm;
        break;
    case VLAN_MACm:
    case VLAN_XLATEm:
        out_mem = VLAN_XLATE_ECCm;
        break;
    case EGR_VLAN_XLATEm:
        out_mem = EGR_VLAN_XLATE_ECCm;
        break;
    case MPLS_ENTRYm:
        out_mem = MPLS_ENTRY_ECCm;
        break;
    case EXACT_MATCH_2m:
        out_mem = FPEM_ECCm;
        out_iratio = 2;
        break;
    case EXACT_MATCH_2_PIPE0m:
        out_mem = FPEM_ECCm;
        out_iratio = 2;
        out_pipe = 0;
        break;
    case EXACT_MATCH_2_PIPE1m:
        out_mem = FPEM_ECCm;
        out_iratio = 2;
        out_pipe = 1;
        break;
    case EXACT_MATCH_2_PIPE2m:
        out_mem = FPEM_ECCm;
        out_iratio = 2;
        out_pipe = 2;
        break;
    case EXACT_MATCH_2_PIPE3m:
        out_mem = FPEM_ECCm;
        out_iratio = 2;
        out_pipe = 3;
        break;
    case EXACT_MATCH_4m:
        out_mem = FPEM_ECCm;
        out_iratio = 4;
        break;
    case EXACT_MATCH_4_PIPE0m:
        out_mem = FPEM_ECCm;
        out_iratio = 4;
        out_pipe = 0;
        break;
    case EXACT_MATCH_4_PIPE1m:
        out_mem = FPEM_ECCm;
        out_iratio = 4;
        out_pipe = 1;
        break;
    case EXACT_MATCH_4_PIPE2m:
        out_mem = FPEM_ECCm;
        out_iratio = 4;
        out_pipe = 2;
        break;
    case EXACT_MATCH_4_PIPE3m:
        out_mem = FPEM_ECCm;
        out_iratio = 4;
        out_pipe = 3;
        break;
    default:
        out_mem = *mem;
        break;
    }
    *mem = out_mem;
    *pipe = out_pipe;
    *iratio = out_iratio;

    return SOC_E_NONE;
}
#endif /* BCM_TOMAHAWK_SUPPORT */
/*
 * Function:
 *      soc_mem_generic_insert
 * Purpose:
 *      Insert an entry
 * Parameters:
 *      unit - StrataSwitch unit #
 *      banks - For dual hashing, which halves are selected (inverted)
 *              Note: used to create a bitmap for ISM mems. (All banks: -1)
 *      entry - entry to insert
 *      old_entry - old entry if existing entry was replaced
 * Returns:
 *      SOC_E_NONE - success
 *      SOC_E_EXISTS - existing entry was replaced
 *      SOC_E_FULL - full
 *      SOC_E_BUSY - modfifo full
 * Notes:
 *      Uses hardware insertion; sends an INSERT message over the
 *      S-Channel.
 */

int
soc_mem_generic_insert(int unit, soc_mem_t mem, int copyno, int32 banks,
                       void *entry, void *old_entry, int *index_ptr)
{
    schan_msg_t schan_msg;
#ifdef BCM_ESW_SUPPORT
    schan_msg_t schan_msg_cpy;
#endif /* BCM_ESW_SUPPORT */
    int         rv, index_valid;
    int         entry_dw = 0;
    uint8       at, *vmap;
    int         type, err_info, index, allow_intr=0;
    uint32      *data, *cache;
    int         src_blk, dst_blk, acc_type, data_byte_len;
    int         opcode, nack;
    uint32      bank_ignore_mask;
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_TRIUMPH_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
    int         rv1 = SOC_E_NONE;
#endif

#ifdef BCM_ESW_SUPPORT
    uint8       insert_retry_attempt = 0;
#endif

#ifdef BCM_TRIDENT2PLUS_SUPPORT
    uint8       insert_full_retry_attempt = 0;
#endif /* BCM_TRIDENT2PLUS_SUPPORT */

#if defined(BCM_TRIUMPH3_SUPPORT)
    int             ins_disable = FALSE, try_one_time = 1;
    uint32          l2_insert = 0;
#endif

#if defined(BCM_HURRICANE3_SUPPORT)
    uint32      overflow_index = 0;
#endif /* BCM_HURRICANE3_SUPPORT */


    _SOC_MEM_REPLACE_MEM(unit, mem);
    SOC_MEM_ALIAS_TO_ORIG(unit,mem);
    assert(SOC_MEM_IS_VALID(unit, mem));
    assert(soc_attached(unit));

#ifdef BCM_SIRIUS_SUPPORT
    if (SOC_IS_SIRIUS(unit)) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_generic_insert: not supported on %s\n"),
                   SOC_CHIP_STRING(unit)));
        return SOC_E_UNAVAIL;
    }
#endif

    if (copyno == MEM_BLOCK_ANY) {
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }
    if (!SOC_MEM_BLOCK_VALID(unit, mem, copyno)) {
        LOG_WARN(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "soc_mem_generic_insert: invalid block %d for memory %s\n"),
                  copyno, SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    if (bsl_check(bslLayerSoc, bslSourceSocmem, bslSeverityNormal, unit)) {
        LOG_CLI((BSL_META_U(unit,
                            "Insert table[%s]: banks=%d"), SOC_MEM_NAME(unit, mem),
                 banks));
        if (bsl_check(bslLayerSoc, bslSourceCommon, bslSeverityVerbose, unit)) {
            soc_mem_entry_dump(unit, mem, entry);
        }
        LOG_CLI((BSL_META_U(unit,
                            "\n")));
    }

    entry_dw = soc_mem_entry_words(unit, mem);


#if defined(BCM_TRIDENT2_SUPPORT)
    if (soc_feature(unit, soc_feature_shared_hash_ins) &&
        !SOC_IS_TOMAHAWKX(unit) &&
        (mem == L3_ENTRY_IPV4_MULTICASTm || mem == L3_ENTRY_IPV6_UNICASTm)) {
        int ok = 0, index, bix;
        int num_banks, bank_ids[7];
        int bank_indexes[7] = {-1, -1, -1, -1, -1, -1, -1};

        /* Retreive applicable bank map and first try replace on each */

        {
            SOC_IF_ERROR_RETURN
                (soc_trident2_hash_bank_count_get(unit, mem, &num_banks));
        }

        if (banks != SOC_MEM_HASH_BANK_ALL) {
            num_banks = 1;
        }
        MEM_LOCK(unit, mem);
        for (bix = 0; bix < num_banks; bix++) {
            int32 bid;

            {
                rv1 = (soc_trident2_hash_bank_number_get(unit, mem, bix,
                                                         &bank_ids[bix]));
            }
            if (SOC_FAILURE(rv1)) {
                MEM_UNLOCK(unit, mem);
                return rv1;
            }
            bid = 1 << bank_ids[bix];
            if (banks != SOC_MEM_HASH_BANK_ALL) {
                bid = banks;
            }
            rv1 = soc_mem_generic_lookup(unit, mem, copyno, bid,
                                         entry, NULL, &index);
            LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
                        (BSL_META_U(unit,
                                    "Sh-Ins-Lukp: bank[%d] "
                                    "rv[%d] index[%d]\n"),
                         bid, rv1, index));
            if (SOC_FAILURE(rv1)) {
                if (rv1 == SOC_E_NOT_FOUND) {
                    /* Entry not found. Save index and move on to the next bank */
                    bank_indexes[bix] = index;
                    continue;
                } else {
                    MEM_UNLOCK(unit, mem);
                    return rv1;
                }
            }
            /* Replace entry */
            if (old_entry) {
                rv1 = soc_mem_read(unit, mem, copyno, index, old_entry);
                if (SOC_FAILURE(rv1)) {
                    MEM_UNLOCK(unit, mem);
                    return rv1;
                }
            }
            rv1 = soc_mem_write(unit, mem, copyno, index, entry);
            if (SOC_FAILURE(rv1)) {
                MEM_UNLOCK(unit, mem);
                return rv1;
            }
            if (index_ptr) {
                *index_ptr = index;
            }
            ok = 1; 
           /* rv = SOC_E_EXISTS; Overwritten later. Coverity complains */
            LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
                        (BSL_META_U(unit,
                                    "Sh-Ins-Lukp: "
                                    "replace at index:%d\n"), index));
            break;
        }
        if (!ok) {
            /* If replace did not happen then try a fresh insert */
            for (bix = 0; bix < num_banks; bix++) {
                uint32 bkt[SOC_MAX_MEM_WORDS];

                index = bank_indexes[bix];
                if (index < 0) {
                    continue;
                }
                rv1 = soc_mem_read(unit, L3_ENTRY_IPV6_MULTICASTm, copyno,
                                   index, bkt);
                if (SOC_FAILURE(rv1)) {
                    MEM_UNLOCK(unit, mem);
                    return rv1;
                } else {
                    uint32 v0, v1;

                    soc_mem_field_get(unit, L3_ENTRY_IPV6_MULTICASTm, bkt,
                                      VALID_0f, &v0);
                    soc_mem_field_get(unit, L3_ENTRY_IPV6_MULTICASTm, bkt,
                                      VALID_1f, &v1);
                    if ((v0 == 0) && (v1 == 0)) {
                        index *= 2;
                        rv1 = soc_mem_write(unit, mem, copyno, index, entry);
                        if (SOC_FAILURE(rv1)) {
                            MEM_UNLOCK(unit, mem);
                            return rv1;
                        }
                        if (index_ptr) {
                            *index_ptr = index;
                        }
                        ok = 1; rv = SOC_E_NONE;
                        LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
                                    (BSL_META_U(unit,
                                                "Sh-Ins-Lukp: "
                                                "insert at index:%d\n"), index));
                        break;
                    } else {
                        soc_mem_field_get(unit, L3_ENTRY_IPV6_MULTICASTm, bkt,
                                          VALID_2f, &v0);
                        soc_mem_field_get(unit, L3_ENTRY_IPV6_MULTICASTm, bkt,
                                          VALID_3f, &v1);
                        if ((v0 == 0) && (v1 == 0)) {
                            index = index*2 + 1;
                            rv1 = soc_mem_write(unit, mem, copyno, index, entry);
                            if (SOC_FAILURE(rv1)) {
                                MEM_UNLOCK(unit, mem);
                                return rv1;
                            }
                            if (index_ptr) {
                                *index_ptr = index;
                            }
                            ok = 1; rv = SOC_E_NONE;
                            LOG_VERBOSE(BSL_LS_SOC_SOCMEM,
                                        (BSL_META_U(unit,
                                                    "Sh-Ins-Lukp: "
                                                    "insert at index:%d\n"), index));
                            break;
                        }
                    }
                }
            }
        }
        if (!ok) {
            MEM_UNLOCK(unit, mem);
            return SOC_E_FULL;
        }

        SOP_MEM_STATE(unit, mem).count[copyno]++;

        /* Update cache if active */
        cache = SOC_MEM_STATE(unit, mem).cache[copyno];
        vmap = SOC_MEM_STATE(unit, mem).vmap[copyno];
        index_valid = (index >= 0 &&
                       index <= soc_mem_index_max(unit, mem));
        if (!index_valid) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "soc_mem_generic_insert: "
                                  "invalid index %d for memory %s\n"),
                       index, SOC_MEM_NAME(unit, mem)));
            rv = SOC_E_INTERNAL;
        } else if (cache != NULL && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
            sal_memcpy(cache + index * entry_dw, entry, entry_dw * 4);
            CACHE_VMAP_SET(vmap, index);
        }
        MEM_UNLOCK(unit, mem);
        return SOC_E_NONE;
    }
#endif /* BCM_TRIDENT2_SUPPORT */

    MEM_LOCK(unit, mem);
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_esm_correction) &&
          ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
         (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
        SOC_ESM_LOCK(unit);
    }
#endif
    schan_msg_clear(&schan_msg);
    acc_type = SOC_MEM_ACC_TYPE(unit, mem);
    src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
    dst_blk = SOC_BLOCK2SCH(unit, copyno);
    if ((soc_feature(unit, soc_feature_ism_memory) && soc_mem_is_mview(unit, mem)) ||
        (soc_feature(unit, soc_feature_shared_hash_mem) && ((mem == L2Xm) ||
         (mem == L3_ENTRY_ONLYm) || (mem == L3_ENTRY_IPV4_UNICASTm) ||
         (mem == L3_ENTRY_IPV4_MULTICASTm) || (mem == L3_ENTRY_IPV6_UNICASTm) ||
         (mem == L3_ENTRY_IPV6_MULTICASTm)
#if defined(BCM_TOMAHAWK_SUPPORT)
         || _SOC_MEM_CHK_FPEM_MEM(mem)
#endif
       ))) {
        /* don't set bank id in cos */
        bank_ignore_mask = 0;
    } else {
        bank_ignore_mask = banks & 0x3;
    }
    data_byte_len = entry_dw * 4;

    schan_msg.gencmd.address = soc_mem_addr_get(unit, mem, 0, copyno, 0, &at);
    if ((soc_feature(unit, soc_feature_ism_memory) && soc_mem_is_mview(unit, mem)) ||
        (soc_feature(unit, soc_feature_shared_hash_mem) && ((mem == L2Xm) ||
         (mem == L3_ENTRY_ONLYm) || (mem == L3_ENTRY_IPV4_UNICASTm) ||
         (mem == L3_ENTRY_IPV4_MULTICASTm) || (mem == L3_ENTRY_IPV6_UNICASTm) ||
         (mem == L3_ENTRY_IPV6_MULTICASTm)
#if defined(BCM_TOMAHAWK_SUPPORT)
         || _SOC_MEM_CHK_FPEM_MEM(mem)
#endif
       ))) {
        /* set bank ignore mask in lower 20 bits */
        if (banks && banks != SOC_MEM_HASH_BANK_ALL) {
            if (soc_feature(unit, soc_feature_shared_hash_mem)) {
                schan_msg.gencmd.address |= (~banks & SOC_HASH_BANK_MASK_SHARED);
            } else {
#ifdef BCM_TRIUMPH3_SUPPORT
                uint32 phy_banks = soc_ism_get_phy_bank_mask(unit, banks);
                schan_msg.gencmd.address |= (~phy_banks & SOC_HASH_BANK_MASK_ISM);
#endif /* BCM_TRIUMPH3_SUPPORT */
            }
        }
    }

    soc_schan_header_cmd_set(unit, &schan_msg.header, TABLE_INSERT_CMD_MSG,
                             dst_blk, src_blk, acc_type, data_byte_len, 0,
                             bank_ignore_mask);

    /* Fill in packet data */
    sal_memcpy(schan_msg.gencmd.data, entry, entry_dw * 4);

    if (SOC_IS_SAND(unit)) {
        allow_intr = 1;
    }

    /*
     * Execute S-Channel "table insert" command packet consisting of
     * (header word + address word + entry_dw), and read back
     * (header word + response word + entry_dw) data words.
     */
    if (2 + entry_dw > CMIC_SCHAN_WORDS(unit)) {
       LOG_WARN(BSL_LS_SOC_SOCMEM,
                (BSL_META_U(unit,
                            "soc_mem_generic_insert: assert will fail for memory %s\n"),
                 SOC_MEM_NAME(unit, mem)));
    }
#ifdef BCM_ESW_SUPPORT
    if (soc_feature(unit, soc_feature_shared_hash_mem) ||
          soc_feature(unit, soc_feature_dual_hash) ||
          soc_feature(unit, soc_feature_ism_memory)) {
        sal_memcpy(&schan_msg_cpy, &schan_msg, sizeof(schan_msg));
    }
_retry_op:
#endif /* BCM_ESW_SUPPORT */
    rv = soc_schan_op(unit, &schan_msg, entry_dw + 2, entry_dw + 2, allow_intr);

    soc_schan_header_status_get(unit, &schan_msg.header, &opcode, NULL, NULL,
                                NULL, NULL, &nack);
    if (opcode != TABLE_INSERT_DONE_MSG) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_generic_insert: "
                              "invalid S-Channel reply, expected TABLE_INSERT_DONE_MSG:\n")));
        soc_schan_dump(unit, &schan_msg, 1);
#if defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit, soc_feature_esm_correction) &&
            ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
             (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
            SOC_ESM_UNLOCK(unit);
        }
#endif
#if defined(BCM_TRIUMPH3_SUPPORT)
        if (ins_disable) {
            _soc_l2mode_fifo_enable_insert_field_set(unit, l2_insert);
            ins_disable = FALSE;
        }
#endif
        MEM_UNLOCK(unit, mem);
        return SOC_E_INTERNAL;
    }

    if (soc_feature(unit, soc_feature_new_sbus_format) &&
        !soc_feature(unit, soc_feature_new_sbus_old_resp) ) {
        type = schan_msg.genresp_v2.response.type;
        err_info = schan_msg.genresp_v2.response.err_info;
        index = schan_msg.genresp_v2.response.index;
        data = schan_msg.genresp_v2.data;
    } else {
        type = schan_msg.genresp.response.type;
        err_info = schan_msg.genresp.response.err_info;
        index = schan_msg.genresp.response.index;
        data = schan_msg.genresp.data;
    }
    
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (ins_disable) {
        _soc_l2mode_fifo_enable_insert_field_set(unit, l2_insert);
        ins_disable = FALSE;
        if (type == SCHAN_GEN_RESP_TYPE_INSERTED) {
            soc_l2_entry_callback(unit, 0, mem, NULL, entry);
        }       
    }
#endif

    if ((nack != 0) || (rv == SOC_E_FAIL)) {
        if (index_ptr) {
            *index_ptr = -1;
        }
#if defined(BCM_TRIUMPH3_SUPPORT)
        if ((SOC_IS_TRIUMPH3(unit) || SOC_IS_TRIDENT(unit) ||
             SOC_IS_TRIDENT2X(unit) || SOC_IS_TOMAHAWKX(unit)) &&
            ((type == SCHAN_GEN_RESP_TYPE_L2_FIFO_FULL) ||
             (type == SCHAN_GEN_RESP_L2_MOD_FIFO_FULL))) {
            /*This is a Software Workaround for L2_MOD_FIFO. Sometimes an entry may be
              unable to be inserted into L2 table when L2_MOD_FIFO is too busy. In this situation
              the L2_INSERT field in L2_MOD_FIFO_ENABLE will be disabled and retry the insert
              operation. After the insertion the L2_INSERT field will be enabled.*/
            l2_insert = _soc_l2mode_fifo_enable_insert_field_get(unit);
            _soc_l2mode_fifo_enable_insert_field_set(unit, 0);
            ins_disable = TRUE;
            if (l2_insert && try_one_time) {
                sal_memcpy(&schan_msg, &schan_msg_cpy, sizeof(schan_msg));
                LOG_INFO(BSL_LS_SOC_SOCMEM,
                         (BSL_META_U(unit,
                                     "L2_MOD_FIFO may be full, error type [%d], will insert again!\n"),
                                     type));
                try_one_time--;
                goto _retry_op;
            } else {
                _soc_l2mode_fifo_enable_insert_field_set(unit, l2_insert);
            }
        }
#endif
#if defined(BCM_ISM_SUPPORT)
        if ((type == SCHAN_GEN_RESP_MAC_LIMIT_THRESHOLD) ||
            (type == SCHAN_GEN_RESP_MAC_LIMIT_DELETE) ||
            (type == SCHAN_GEN_RESP_L2_MOD_FIFO_FULL)) {
            LOG_INFO(BSL_LS_SOC_SOCMEM,
                     (BSL_META_U(unit,
                                 "Insert table[%s]: hash bucket full\n"),
                      SOC_MEM_NAME(unit, mem)));

            rv = SOC_E_FULL;
        }  else {
#endif
#ifdef BCM_TRIDENT2PLUS_SUPPORT
            if (soc_feature(unit, soc_feature_td2p_a0_sw_war) &&
                ((mem == EGR_VLAN_XLATEm) || (mem == VLAN_XLATEm)) &&
                (type == SCHAN_GEN_RESP_TYPE_FULL)) {
                if (insert_full_retry_attempt < HASH_MEM_OP_RETRY_COUNT) {
                    insert_full_retry_attempt++;
                    sal_memcpy(&schan_msg, &schan_msg_cpy, sizeof(schan_msg));
                    goto _retry_op;
                }
            }
#endif /* BCM_TRIDENT2PLUS_SUPPORT */
            if ((type == SCHAN_GEN_RESP_TYPE_FULL) ||
                (type == SCHAN_GEN_RESP_TYPE_L2_FIFO_FULL) ||
                (type == SCHAN_GEN_RESP_TYPE_MAC_LIMIT_THRE) ||
                (type == SCHAN_GEN_RESP_TYPE_MAC_LIMIT_DEL)) {
                LOG_INFO(BSL_LS_SOC_SOCMEM,
                         (BSL_META_U(unit,
                                     "Insert table[%s]: hash bucket full\n"),
                          SOC_MEM_NAME(unit, mem)));
                rv = SOC_E_FULL;
            } else if (type == SCHAN_GEN_RESP_TYPE_ERROR) {
#ifdef BCM_ESW_SUPPORT
                LOG_VERBOSE(BSL_LS_SOC_SER,(BSL_META_U(unit,
                    ">>>>> soc_mem_generic_insert: rv=%d, nack=%d, type=%d, index=%d\n"),
                    rv, nack, type, index));
                if (soc_feature(unit, soc_feature_shared_hash_mem)) {
                    int i, rv1 = SOC_E_NONE, index1 = index, ir1 = 1,
                        pipe1 = SOC_PIPE_ANY;
                    soc_mem_t mem1 = mem;
#if defined(BCM_TOMAHAWK_SUPPORT)
                    if (SOC_IS_TOMAHAWKX(unit)) {
                        (void)_soc_inline_ser_mem_remap(unit, &mem1, &pipe1,
                                                        &ir1);
                        index1 = (index / ir1) * ir1; /* base_index for entry */
                    }
#endif /* BCM_TOMAHAWK_SUPPORT */
                    for (i = 0; i < ir1; i++) {
                        rv1 = soc_ser_sram_correction(unit, pipe1,
                                                  schan_msg.gencmd.header.v2.dst_blk,
                                                      schan_msg.gencmd.address, mem1,
                                                      copyno, index1 + i, NULL);
                        if (SOC_FAILURE(rv1)) {
                            break;
                        }
                    }
                    if (rv1 == SOC_E_NONE) {
                        if (insert_retry_attempt < HASH_MEM_OP_RETRY_COUNT) {
                            insert_retry_attempt++;
                            sal_memcpy(&schan_msg, &schan_msg_cpy,
                                       sizeof(schan_msg));
                            goto _retry_op;
                        }
                    }
                }
#endif /* BCM_ESW_SUPPORT */
                if (err_info == SCHAN_GEN_RESP_ERROR_BUSY) {
                    LOG_INFO(BSL_LS_SOC_SOCMEM,
                             (BSL_META_U(unit,
                                         "Insert table[%s]: Modfifo full\n"),
                              SOC_MEM_NAME(unit, mem)));
                    rv = SOC_E_BUSY;
                } else if (err_info == SCHAN_GEN_RESP_ERROR_PARITY) {
                    LOG_ERROR(BSL_LS_SOC_SOCMEM,
                              (BSL_META_U(unit,
                                          "Insert table[%s]: Parity Error Index %d\n"),
                               SOC_MEM_NAME(unit, mem), index));
                    rv = SOC_E_INTERNAL;
                }
#if defined(BCM_TRX_SUPPORT)
                /* Hash table response with SCHAN_GEN_RESP_TYPE_ERROR */
                if (SOC_IS_TRX(unit) && (soc_mem_is_hashed(unit, mem))) {
                    /* Try to recover */
                    rv = soc_hash_mem_inline_recovery(unit, mem, &schan_msg,
                                                        banks, entry);
                    if (rv == SOC_E_NONE) {
                        if (insert_retry_attempt < HASH_MEM_OP_RETRY_COUNT) {
                            insert_retry_attempt++;
                            sal_memcpy(&schan_msg, &schan_msg_cpy, sizeof(schan_msg));
                            LOG_WARN(BSL_LS_SOC_SOCMEM,
                                     (BSL_META_U(unit,
                                                 "unit %d: Retry Hash insert operation..\n"),
                                      unit));
                            goto _retry_op;
                        }

                    } else {
                        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                                  (BSL_META_U(unit,
                                              "unit %d, table[%s]: Parity error recovery failed, return value: %d\n"),
                                   unit, SOC_MEM_NAME(unit, mem), rv));
                    }
                }
#endif
            } else {
#if defined(BCM_TRIDENT2_SUPPORT)
                if (SOC_IS_TD2_TT2(unit) && soc_mem_is_hashed(unit, mem) &&
                    (type == SCHAN_GEN_RESP_TYPE_INSERTED ||
                    type == SCHAN_GEN_RESP_TYPE_REPLACED)) {
                    rv = soc_hash_mem_inline_recovery(unit, mem, &schan_msg,
                                                        banks, entry);
                    if (rv == SOC_E_NONE) {
                        if (insert_retry_attempt < HASH_MEM_OP_RETRY_COUNT) {
                            insert_retry_attempt++;
                            sal_memcpy(&schan_msg, &schan_msg_cpy, sizeof(schan_msg));
                            LOG_WARN(BSL_LS_SOC_SOCMEM,
                                     (BSL_META_U(unit,
                                                 "unit %d: Retry Hash insert operation..\n"),
                                      unit));
                            goto _retry_op;
                        }
                    } else {
                        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                                  (BSL_META_U(unit,
                                              "unit %d, table[%s]: Parity error recovery failed, return value: %d\n"),
                                   unit, SOC_MEM_NAME(unit, mem), rv));
                    }
                }
#endif
                rv = SOC_E_FAIL;
            }
#if defined(BCM_TRIUMPH3_SUPPORT) && defined(BCM_ISM_SUPPORT)
        }
#endif
    } else {
#ifdef BCM_TRIUMPH_SUPPORT
        if (soc_feature(unit, soc_feature_etu_support) &&
            mem == EXT_L2_ENTRY_2m) {
            soc_mem_t ext_mem;
            int ext_index;

            rv1 = soc_tcam_raw_index_to_mem_index(unit, index,
                                &ext_mem, &ext_index);
            if (SOC_FAILURE(rv1)) {
#if defined(BCM_TRIUMPH3_SUPPORT)
                if (soc_feature(unit, soc_feature_esm_correction) &&
                      ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
                     (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
                    SOC_ESM_UNLOCK(unit);
                }
#endif
                MEM_UNLOCK(unit, mem);
                return rv1;
            }
            if (mem != ext_mem) {
#if defined(BCM_TRIUMPH3_SUPPORT)
                if (soc_feature(unit, soc_feature_esm_correction) &&
                      ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
                     (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
                    SOC_ESM_UNLOCK(unit);
                }
#endif
                MEM_UNLOCK(unit, mem);
                return SOC_E_INTERNAL;
            }
            index = ext_index;
        }
#endif /* BCM_TRIUMPH_SUPPORT */
        if (index_ptr != NULL) {
            *index_ptr = index;
        }
        if (type == SCHAN_GEN_RESP_TYPE_REPLACED) {
            if (old_entry != NULL) {
                /* coverity[overrun-buffer-arg] */
                sal_memcpy(old_entry, data, entry_dw * sizeof(uint32));
            }
            rv = SOC_E_EXISTS;
        }
        SOP_MEM_STATE(unit, mem).count[copyno]++;
#if defined(BCM_TRIUMPH_SUPPORT)
        if (mem == EXT_L2_ENTRYm) {
            rv1 = soc_triumph_ext_l2_entry_update(unit, index, entry);
            if (SOC_FAILURE(rv1)) {
#if defined(BCM_TRIUMPH3_SUPPORT)
                if (soc_feature(unit, soc_feature_esm_correction) &&
                      ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
                     (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
                    SOC_ESM_UNLOCK(unit);
                }
#endif
                MEM_UNLOCK(unit, mem);
                return rv1;
            }
        }
#endif /* BCM_TRIUMPH_SUPPORT */
#if defined(BCM_TRIUMPH3_SUPPORT)
        if (mem == EXT_L2_ENTRY_1m || mem == EXT_L2_ENTRY_2m) {
            rv1 = soc_triumph3_ext_l2_entry_update(unit, mem, index, entry);
            if (SOC_FAILURE(rv1)) {
#if defined(BCM_TRIUMPH3_SUPPORT)
                if (soc_feature(unit, soc_feature_esm_correction) &&
                      ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
                     (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
                    SOC_ESM_UNLOCK(unit);
                }
#endif
                MEM_UNLOCK(unit, mem);
                return rv1;
            }
        }
#endif /* BCM_TRIUMPH3_SUPPORT */
        /* Update cache if active */
        cache = SOC_MEM_STATE(unit, mem).cache[copyno];
        vmap = SOC_MEM_STATE(unit, mem).vmap[copyno];
        index_valid = (index >= 0 &&
                       index <= soc_mem_index_max(unit, mem));
        if (!index_valid) {
#if defined(BCM_HURRICANE3_SUPPORT)
            if ((SOC_IS_HURRICANE3(unit) || SOC_IS_GREYHOUND2(unit)) &&
                (mem == L2Xm) && 
                (index & SCHAN_GEN_RESP_INDEX_L2_OVERFLOW)) {
                overflow_index = index & ~SCHAN_GEN_RESP_INDEX_L2_OVERFLOW; 
                /* Check whether the entry is located at L2_OVERFLOW_ENTRY */
                if ((overflow_index) <=
                    soc_mem_index_max(unit, L2_ENTRY_OVERFLOWm)) {
                    rv = SOC_E_NONE;
                    /* update cache */
                    cache = SOC_MEM_STATE(unit, 
                                    L2_ENTRY_OVERFLOWm).cache[copyno];
                    vmap = SOC_MEM_STATE(unit, 
                                    L2_ENTRY_OVERFLOWm).vmap[copyno];
                    if (cache != NULL && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
                        /* 
                         * If an invalid or non-static entry 
                          * is being written then clear the cache 
                          */
                        if (soc_mem_field32_get(unit, mem, entry, VALIDf) &&
                           soc_mem_field32_get(unit, mem, entry, STATIC_BITf)) {
                            sal_memcpy(cache + overflow_index * entry_dw, 
                                entry, entry_dw * 4);
                            CACHE_VMAP_SET(vmap, overflow_index);
                         } else {
                            CACHE_VMAP_CLR(vmap, overflow_index);
                         }
                    }
                } else {
                    LOG_ERROR(BSL_LS_SOC_SOCMEM, 
                        (BSL_META_U(unit, "soc_mem_generic_insert: "
                         "invalid index %d for memory L2_ENTRY_OVERFLOW\n"),
                         overflow_index));
                    rv = SOC_E_INTERNAL;
                }
            } else 
#endif /* BCM_HURRICANE3_SUPPORT */
            {            
                LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "soc_mem_generic_insert: "
                                  "invalid index %d for memory %s\n"),
                       index, SOC_MEM_NAME(unit, mem)));
                rv = SOC_E_INTERNAL;
            }
        } else if (cache != NULL && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
            if (_SOC_MEM_CHK_L2_MEM(mem)) {
                /* If an invalid or non-static entry is being written then clear the cache */
                if (((mem == L2_ENTRY_2m &&
                     soc_L2_ENTRY_2m_field32_get(unit, entry, VALID_0f) &&
                     soc_L2_ENTRY_2m_field32_get(unit, entry, VALID_1f)) ||
                     ((mem == L2Xm || mem == L2_ENTRY_1m) &&
                      soc_mem_field32_get(unit, mem, entry, VALIDf))) &&
                     (((mem == L2Xm || mem == L2_ENTRY_1m) &&
                       soc_mem_field32_get(unit, mem, entry, STATIC_BITf)) ||
                     (mem == L2_ENTRY_2m &&
                      (soc_mem_field32_get(unit, mem, entry, STATIC_BIT_0f) &&
                       soc_mem_field32_get(unit, mem, entry, STATIC_BIT_1f))))) {
                    sal_memcpy(cache + index * entry_dw, entry, entry_dw * 4);
                    CACHE_VMAP_SET(vmap, index);
                    if (mem == L2_ENTRY_1m) {
                        vmap = SOC_MEM_STATE(unit, L2_ENTRY_2m).vmap[copyno];
                        CACHE_VMAP_CLR(vmap, index/2);
                    } else if (mem == L2_ENTRY_2m) {
                        vmap = SOC_MEM_STATE(unit, L2_ENTRY_1m).vmap[copyno];
                        CACHE_VMAP_CLR(vmap, index*2);
                        CACHE_VMAP_CLR(vmap, index*2 + 1);
                    }
                } else {
                    CACHE_VMAP_CLR(vmap, index);
                }
            } else {
                sal_memcpy(cache + index * entry_dw, entry, entry_dw * 4);
                CACHE_VMAP_SET(vmap, index);
            }
        }
    }
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_esm_correction) &&
                      ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
                     (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
        SOC_ESM_UNLOCK(unit);
    }
#endif
    MEM_UNLOCK(unit, mem);

    return rv;
}

/*
 * Function:
 *      soc_mem_generic_delete
 * Purpose:
 *      Delete an entry
 * Parameters:
 *      unit - StrataSwitch unit #
 *      banks - For dual hashing, which halves are selected (inverted)
 *              Note: used to create a bitmap for ISM mems. (All banks: -1)
 *      entry - entry to delete
 *      old_entry - old entry if entry was found and deleted
 * Returns:
 *      SOC_E_NONE - success
 *      SOC_E_NOT_FOUND - full
 *      SOC_E_BUSY - modfifo full
 * Notes:
 *      Uses hardware deletion; sends an DELETE message over the
 *      S-Channel.
 */

int
soc_mem_generic_delete(int unit, soc_mem_t mem, int copyno, int32 banks,
                       void *entry, void *old_entry, int *index_ptr)
{
    schan_msg_t schan_msg;
#ifdef BCM_ESW_SUPPORT
    schan_msg_t schan_msg_cpy;
#endif /* BCM_ESW_SUPPORT */
    int         rv, entry_dw, index_valid;
    uint8       at, *vmap;
    int         type, err_info, index, allow_intr=0;
    uint32      *data, *cache;
    void        *null_entry;
    int         src_blk, dst_blk, acc_type, data_byte_len;
    int         opcode, nack;
    uint32      bank_ignore_mask;
#if defined(BCM_TRIUMPH_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
    int         rv1 = SOC_E_NONE;
#endif
#if defined(BCM_TRX_SUPPORT)
    uint8       delete_retry_attempt = 0;
#endif
#if defined(BCM_TRIUMPH3_SUPPORT)
    int             del_disable = FALSE, try_one_time = 1;
    uint32          l2_delete = 0;
#endif

#if defined(BCM_HURRICANE3_SUPPORT)
    uint32      overflow_index = 0;
#endif /* BCM_HURRICANE3_SUPPORT */


    _SOC_MEM_REPLACE_MEM(unit, mem);
    SOC_MEM_ALIAS_TO_ORIG(unit,mem);
    null_entry = soc_mem_entry_null(unit, mem);

    assert(SOC_MEM_IS_VALID(unit, mem));
    assert(soc_attached(unit));

#ifdef BCM_SIRIUS_SUPPORT
    if (SOC_IS_SIRIUS(unit)) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_generic_delete: not supported on %s\n"),
                   SOC_CHIP_STRING(unit)));
    return SOC_E_UNAVAIL;
    }
#endif

    if (copyno == MEM_BLOCK_ANY) {
        /* coverity[overrun-local : FALSE] */
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }
    if (!SOC_MEM_BLOCK_VALID(unit, mem, copyno)) {
        LOG_WARN(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "soc_mem_generic_delete: invalid block %d for memory %s\n"),
                  copyno, SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    if (bsl_check(bslLayerSoc, bslSourceSocmem, bslSeverityNormal, unit)) {
        LOG_CLI((BSL_META_U(unit,
                            "Delete table[%s]: banks=%d"), SOC_MEM_NAME(unit, mem),
                 banks));
        if (bsl_check(bslLayerSoc, bslSourceCommon, bslSeverityVerbose, unit)) {
            soc_mem_entry_dump(unit, mem, entry);
        }
        LOG_CLI((BSL_META_U(unit,
                            "\n")));
    }

    entry_dw = soc_mem_entry_words(unit, mem);

    MEM_LOCK(unit, mem);
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_esm_correction) &&
          ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
         (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
        SOC_ESM_LOCK(unit);
    }
#endif
    schan_msg_clear(&schan_msg);
    acc_type = SOC_MEM_ACC_TYPE(unit, mem);
    src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
    dst_blk = SOC_BLOCK2SCH(unit, copyno);
    if ((soc_feature(unit, soc_feature_ism_memory) && soc_mem_is_mview(unit, mem)) ||
        (soc_feature(unit, soc_feature_shared_hash_mem) && ((mem == L2Xm) ||
         (mem == L3_ENTRY_ONLYm) || (mem == L3_ENTRY_IPV4_UNICASTm) ||
         (mem == L3_ENTRY_IPV4_MULTICASTm) || (mem == L3_ENTRY_IPV6_UNICASTm) ||
         (mem == L3_ENTRY_IPV6_MULTICASTm)
#if defined(BCM_TOMAHAWK_SUPPORT)
         || _SOC_MEM_CHK_FPEM_MEM(mem)
#endif
       ))) {
        /* don't set bank id in cos */
        bank_ignore_mask = 0;
    } else {
        bank_ignore_mask = banks & 0x3;
    }
    data_byte_len = entry_dw * 4;

    schan_msg.gencmd.address = soc_mem_addr_get(unit, mem, 0, copyno, 0, &at);
    if ((soc_feature(unit, soc_feature_ism_memory) && soc_mem_is_mview(unit, mem)) ||
        (soc_feature(unit, soc_feature_shared_hash_mem) && ((mem == L2Xm) ||
         (mem == L3_ENTRY_ONLYm) || (mem == L3_ENTRY_IPV4_UNICASTm) ||
         (mem == L3_ENTRY_IPV4_MULTICASTm) || (mem == L3_ENTRY_IPV6_UNICASTm) ||
         (mem == L3_ENTRY_IPV6_MULTICASTm)
#if defined(BCM_TOMAHAWK_SUPPORT)
         || _SOC_MEM_CHK_FPEM_MEM(mem)
#endif
       ))) {
        /* set bank ignore mask in lower 20 bits */
        if (banks && banks != SOC_MEM_HASH_BANK_ALL) {
            if (soc_feature(unit, soc_feature_shared_hash_mem)) {
                schan_msg.gencmd.address |= (~banks & SOC_HASH_BANK_MASK_SHARED);
            } else {
#ifdef BCM_TRIUMPH3_SUPPORT
                uint32 phy_banks = soc_ism_get_phy_bank_mask(unit, banks);
                schan_msg.gencmd.address |= (~phy_banks & SOC_HASH_BANK_MASK_ISM);
#endif /* BCM_TRIUMPH3_SUPPORT */
            }
        }
    }

    soc_schan_header_cmd_set(unit, &schan_msg.header, TABLE_DELETE_CMD_MSG,
                             dst_blk, src_blk, acc_type, data_byte_len, 0,
                             bank_ignore_mask);

    /* Fill in packet data */
    sal_memcpy(schan_msg.gencmd.data, entry, entry_dw * 4);

    if (SOC_IS_SAND(unit)) {
        allow_intr = 1;
    }

    /*
     * Execute S-Channel "table delete" command packet consisting of
     * (header word + address word + entry_dw), and read back
     * (header word + response word + entry_dw) data words.
     */
#ifdef BCM_ESW_SUPPORT
    if (soc_feature(unit, soc_feature_shared_hash_mem) ||
          soc_feature(unit, soc_feature_dual_hash) ||
          soc_feature(unit, soc_feature_ism_memory)) {
        sal_memcpy(&schan_msg_cpy, &schan_msg, sizeof(schan_msg));
    }
#endif
#if defined(BCM_TRX_SUPPORT)
_retry_op_delete:
#endif
    rv = soc_schan_op(unit, &schan_msg, entry_dw + 2, entry_dw + 2, allow_intr);

    soc_schan_header_status_get(unit, &schan_msg.header, &opcode, NULL, NULL,
                                NULL, NULL, &nack);
    if (opcode != TABLE_DELETE_DONE_MSG) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_generic_delete: "
                              "invalid S-Channel reply, expected TABLE_DELETE_DONE_MSG:\n")));
        soc_schan_dump(unit, &schan_msg, 1);
#if defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit, soc_feature_esm_correction) &&
              ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
             (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
            SOC_ESM_UNLOCK(unit);
        }
#endif
#if defined(BCM_TRIUMPH3_SUPPORT)
        if (del_disable) {
            _soc_l2mode_fifo_enable_delete_field_set(unit, l2_delete);
            del_disable = FALSE;
        }
#endif
        MEM_UNLOCK(unit, mem);
        return SOC_E_INTERNAL;
    }

    if (soc_feature(unit, soc_feature_new_sbus_format) &&
        !soc_feature(unit, soc_feature_new_sbus_old_resp) ) {
        type = schan_msg.genresp_v2.response.type;
        err_info = schan_msg.genresp_v2.response.err_info;
        index = schan_msg.genresp_v2.response.index;
        data = schan_msg.genresp_v2.data;
    } else {
        type = schan_msg.genresp.response.type;
        err_info = schan_msg.genresp.response.err_info;
        index = schan_msg.genresp.response.index;
        data = schan_msg.genresp.data;
    }
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (del_disable) {
        _soc_l2mode_fifo_enable_delete_field_set(unit, l2_delete);
        del_disable = FALSE;
        if (type == SCHAN_GEN_RESP_TYPE_DELETED) {        
            soc_l2_entry_callback(unit, 0, mem, entry, NULL);
        }
    }
#endif

    if ((nack != 0) || (rv == SOC_E_FAIL)) {
        if (index_ptr) {
            *index_ptr = -1;
        }
        if (type == SCHAN_GEN_RESP_TYPE_NOT_FOUND) {
            LOG_INFO(BSL_LS_SOC_SOCMEM,
                     (BSL_META_U(unit,
                                 "Delete table[%s]: Not found\n"),
                      SOC_MEM_NAME(unit, mem)));
            rv = SOC_E_NOT_FOUND;
        } else if (type == SCHAN_GEN_RESP_TYPE_ERROR) {
                LOG_VERBOSE(BSL_LS_SOC_SER,(BSL_META_U(unit,
                    ">>>>> soc_mem_generic_delete: rv=%d, nack=%d, type=%d, index=%d\n"),
                    rv, nack, type, index));
#if defined(BCM_TOMAHAWK_SUPPORT)
                if (soc_feature(unit, soc_feature_shared_hash_mem) &&
                    SOC_IS_TOMAHAWKX(unit)) {
                    int i, rv1 = SOC_E_NONE, index1 = index, ir1 = 1,
                        pipe1 = SOC_PIPE_ANY;
                    soc_mem_t mem1 = mem;

                    (void)_soc_inline_ser_mem_remap(unit, &mem1, &pipe1, &ir1);
                    index1 = (index / ir1) * ir1; /* base_index for entry */

                    for (i = 0; i < ir1; i++) { 
                        rv1 = soc_ser_sram_correction(unit, pipe1,
                                                      schan_msg.gencmd.header.v2.dst_blk,
                                                      schan_msg.gencmd.address, mem1,
                                                      copyno, index1 + i, NULL);
                        if (SOC_FAILURE(rv1)) {
                            break;
                        }
                    }
                    if (rv1 == SOC_E_NONE) {
                        if (delete_retry_attempt < HASH_MEM_OP_RETRY_COUNT) {
                            delete_retry_attempt++;
                            sal_memcpy(&schan_msg, &schan_msg_cpy,
                                       sizeof(schan_msg));
                            goto _retry_op_delete;
                        }
                    }
                }
#endif /* BCM_TOMAHAWK_SUPPORT */
            if (err_info == SCHAN_GEN_RESP_ERROR_BUSY) {
                LOG_INFO(BSL_LS_SOC_SOCMEM,
                         (BSL_META_U(unit,
                                     "Delete table[%s]: Modfifo full\n"),
                          SOC_MEM_NAME(unit, mem)));
                rv = SOC_E_BUSY;
            } else if (err_info == SCHAN_GEN_RESP_ERROR_PARITY) {
                LOG_ERROR(BSL_LS_SOC_SOCMEM,
                          (BSL_META_U(unit,
                                      "Delete table[%s]: Parity Error Index %d\n"),
                           SOC_MEM_NAME(unit, mem), index));
                rv = SOC_E_INTERNAL;
            }
            /* For Trident2, type is SCHAN_GEN_RESP_TYPE_ERROR, err_info is 0 */
#if defined(BCM_TRX_SUPPORT)
            if (SOC_IS_TRX(unit) && (soc_mem_is_hashed(unit, mem))) {
                /* Try to recover */
                rv = soc_hash_mem_inline_recovery(unit, mem, &schan_msg,
                                                    banks, entry);
                if (rv == SOC_E_NONE) {
                      if (delete_retry_attempt < HASH_MEM_OP_RETRY_COUNT) {
                          delete_retry_attempt++;
                          if (soc_mem_generic_lookup(unit, mem, copyno, banks,
                                                     entry, NULL, index_ptr) ==
                                                     SOC_E_NOT_FOUND) {
                            rv = SOC_E_NONE;
                        } else {
                            sal_memcpy(&schan_msg, &schan_msg_cpy, sizeof(schan_msg));
                            LOG_WARN(BSL_LS_SOC_SOCMEM,
                                     (BSL_META_U(unit,
                                                 "unit %d: Retry Hash delete operation..\n"),
                                      unit));
                            goto _retry_op_delete;
                        }
                      }
                } else {
                    LOG_ERROR(BSL_LS_SOC_SOCMEM,
                              (BSL_META_U(unit,
                                          "unit %d, table[%s]: Parity error recovery failed, return value: %d\n"),
                               unit, SOC_MEM_NAME(unit, mem), rv));
                }
            }
#endif
        } else {

            rv = SOC_E_FAIL;
#if defined(BCM_TRIDENT2_SUPPORT)
            if (SOC_IS_TD2_TT2(unit) && soc_mem_is_hashed(unit, mem) &&
                (type == SCHAN_GEN_RESP_TYPE_DELETED)) {
                rv = soc_hash_mem_inline_recovery(unit, mem, &schan_msg,
                                                  banks, entry);
                if (rv == SOC_E_NONE) {
                    if (delete_retry_attempt < HASH_MEM_OP_RETRY_COUNT) {
                        delete_retry_attempt++;
                        if (soc_mem_generic_lookup(unit, mem, copyno, banks,
                                                     entry, NULL, index_ptr) ==
                                                     SOC_E_NOT_FOUND) {
                            rv = SOC_E_NONE;
                        } else {
                            sal_memcpy(&schan_msg, &schan_msg_cpy, sizeof(schan_msg));
                            LOG_WARN(BSL_LS_SOC_SOCMEM,
                                     (BSL_META_U(unit,
                                                 "unit %d: Retry Hash delete operation..\n"),
                                      unit));
                            goto _retry_op_delete;
                        }
                    }
                } else {
                    LOG_ERROR(BSL_LS_SOC_SOCMEM,
                              (BSL_META_U(unit,
                                          "unit %d, table[%s]: Parity error recovery failed, return value: %d\n"),
                               unit, SOC_MEM_NAME(unit, mem), rv));
                }
            }
#endif
#if defined(BCM_TRIUMPH3_SUPPORT)
            if ((SOC_IS_TRIUMPH3(unit) || SOC_IS_TRIDENT(unit) ||
                  SOC_IS_TRIDENT2X(unit) || SOC_IS_TOMAHAWKX(unit)) &&
                 ((type == SCHAN_GEN_RESP_TYPE_L2_FIFO_FULL) ||
                  (type == SCHAN_GEN_RESP_L2_MOD_FIFO_FULL))) {
                /*This is a Software Workaround for L2_MOD_FIFO. Sometimes an entry may be
                  unable to be deleted when L2_MOD_FIFO is too busy. In this situation
                  the L2_DELETE field in L2_MOD_FIFO_ENABLE will be disabled and retry the delete
                  operation. After the deletion the L2_DELETE field will be enabled.*/
                if (soc_mem_generic_lookup(unit, mem, copyno, banks,
                                           entry, NULL, index_ptr) ==
                                           SOC_E_NOT_FOUND) {
                    rv = SOC_E_NONE;
                } else {
                    l2_delete = _soc_l2mode_fifo_enable_delete_field_get(unit);
                    _soc_l2mode_fifo_enable_delete_field_set(unit, 0);
                    del_disable = TRUE;
                    sal_memcpy(&schan_msg, &schan_msg_cpy, sizeof(schan_msg));
                    if (l2_delete && try_one_time) {
                        LOG_INFO(BSL_LS_SOC_SOCMEM,
                                 (BSL_META_U(unit,
                                             "L2_MOD_FIFO may be full, error type [%d], will delete again!\n"),
                                             type));
                        try_one_time--;
                        goto _retry_op_delete;
                    } else {
                        _soc_l2mode_fifo_enable_delete_field_set(unit, l2_delete);
                    }
                }                
            }
#endif
        }
    } else {
        if (old_entry != NULL) {
            sal_memcpy(old_entry, data, entry_dw * sizeof(uint32));
        }
#ifdef BCM_TRIUMPH_SUPPORT
        if (soc_feature(unit, soc_feature_etu_support) &&
            mem == EXT_L2_ENTRY_2m) {
            soc_mem_t ext_mem;
            int ext_index;

            rv1 = soc_tcam_raw_index_to_mem_index(unit, index,
                                                &ext_mem, &ext_index);
            if (SOC_FAILURE(rv1)) {
#if defined(BCM_TRIUMPH3_SUPPORT)
                if (soc_feature(unit, soc_feature_esm_correction) &&
                      ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
                     (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
                    SOC_ESM_UNLOCK(unit);
                }
#endif
                MEM_UNLOCK(unit, mem);
                return rv1;
            }
            if (mem != ext_mem) {
#if defined(BCM_TRIUMPH3_SUPPORT)
                if (soc_feature(unit, soc_feature_esm_correction) &&
                      ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
                     (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
                    SOC_ESM_UNLOCK(unit);
                }
#endif
                MEM_UNLOCK(unit, mem);
                return SOC_E_INTERNAL;
            }
            index = ext_index;
        }
#endif /* BCM_TRIUMPH_SUPPORT */
        if (index_ptr != NULL) {
            *index_ptr = index;
        }
        SOP_MEM_STATE(unit, mem).count[copyno]--;
#if defined(BCM_TRIUMPH_SUPPORT)
        if (mem == EXT_L2_ENTRYm) {
            rv1 = soc_triumph_ext_l2_entry_update(unit, index, NULL);
            if (SOC_FAILURE(rv1)) {
#if defined(BCM_TRIUMPH3_SUPPORT)
                if (soc_feature(unit, soc_feature_esm_correction) &&
                      ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
                     (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
                    SOC_ESM_UNLOCK(unit);
                }
#endif
                MEM_UNLOCK(unit, mem);
                return rv1;
            }
        }
#endif /* BCM_TRIUMPH_SUPPORT */
#if defined(BCM_TRIUMPH3_SUPPORT)
        if (mem == EXT_L2_ENTRY_1m || mem == EXT_L2_ENTRY_2m) {
            rv1 = soc_triumph3_ext_l2_entry_update(unit, mem, index, NULL);
            if (SOC_FAILURE(rv1)) {
#if defined(BCM_TRIUMPH3_SUPPORT)
                if (soc_feature(unit, soc_feature_esm_correction) &&
                      ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
                     (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
                    SOC_ESM_UNLOCK(unit);
                }
#endif
                MEM_UNLOCK(unit, mem);
                return rv1;
            }
        }
#endif /* BCM_TRIUMPH3_SUPPORT */
        if (!SOC_WARM_BOOT(unit)) {
            /* Update cache if active */
            cache = SOC_MEM_STATE(unit, mem).cache[copyno];
            vmap = SOC_MEM_STATE(unit, mem).vmap[copyno];
            index_valid = (index >= 0 &&
                           index <= soc_mem_index_max(unit, mem));
            if (!index_valid) {
#if defined(BCM_HURRICANE3_SUPPORT)
                if ((SOC_IS_HURRICANE3(unit) || SOC_IS_GREYHOUND2(unit)) && 
                    (mem == L2Xm) && 
                    (index & SCHAN_GEN_RESP_INDEX_L2_OVERFLOW)) {
                    overflow_index = index & ~SCHAN_GEN_RESP_INDEX_L2_OVERFLOW; 
                    /* Check whether the entry is located at L2_OVERFLOW_ENTRY */
                    if ((overflow_index) <=
                        soc_mem_index_max(unit, L2_ENTRY_OVERFLOWm)) {
                        /* update cache */
                        cache = SOC_MEM_STATE(unit, 
                                        L2_ENTRY_OVERFLOWm).cache[copyno];
                        vmap = SOC_MEM_STATE(unit, 
                                        L2_ENTRY_OVERFLOWm).vmap[copyno];
                        if (cache != NULL && !SOC_MEM_TEST_SKIP_CACHE(unit) &&
                            CACHE_VMAP_TST(vmap, overflow_index)) {
                            CACHE_VMAP_CLR(vmap, index);
                        }
                        MEM_UNLOCK(unit, mem);
                        return rv;
                    } else {
                        LOG_ERROR(BSL_LS_SOC_SOCMEM, 
                            (BSL_META_U(unit, 
                             "soc_mem_generic_delete: "
                             "invalid index %d for memory L2_ENTRY_OVERFLOW\n"),
                             overflow_index));
                    }
                } else 
#endif /* BCM_HURRICANE3_SUPPORT */ 
                {                
                    LOG_ERROR(BSL_LS_SOC_SOCMEM,
                          (BSL_META_U(unit,
                                      "soc_mem_generic_delete: "
                                      "invalid index %d for memory %s\n"),
                           index, SOC_MEM_NAME(unit, mem)));
                }
#if defined(BCM_TRIUMPH3_SUPPORT)
                if (soc_feature(unit, soc_feature_esm_correction) &&
                      ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
                     (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
                    SOC_ESM_UNLOCK(unit);
                }
#endif
                MEM_UNLOCK(unit, mem);
                return SOC_E_INTERNAL;
            }
            if (index_valid && cache != NULL && CACHE_VMAP_TST(vmap, index)
                && !SOC_MEM_TEST_SKIP_CACHE(unit)) {
                if (_SOC_MEM_CHK_L2_MEM(mem)) {
                    CACHE_VMAP_CLR(vmap, index);
                    if (mem == L2_ENTRY_1m) {
                        vmap = SOC_MEM_STATE(unit, L2_ENTRY_2m).vmap[copyno];
                        CACHE_VMAP_CLR(vmap, index/2);
                    } else if (mem == L2_ENTRY_2m) {
                        vmap = SOC_MEM_STATE(unit, L2_ENTRY_1m).vmap[copyno];
                        CACHE_VMAP_CLR(vmap, index*2);
                        CACHE_VMAP_CLR(vmap, index*2 + 1);
                    }
                } else {
                    sal_memcpy(cache + index * entry_dw, null_entry, entry_dw * 4);
                    CACHE_VMAP_CLR(vmap, index);
                }
            }
        }
    }
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_esm_correction) &&
        ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
         (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ETU))) {
        SOC_ESM_UNLOCK(unit);
    }
#endif
    MEM_UNLOCK(unit, mem);
    return rv;
}

/*
 * Function:
 *      soc_mem_search
 * Purpose:
 *      Search a memory for a key.
 * Parameters:
 *      mem               Memory to search
 *      copyno               Which copy to search (if multiple copies)
 *      index_ptr            OUT:
 *                           If entry found gets the location of the entry.
 *                           If not found, and table is sorted, gets the
 *                           location of the insertion point.
 *                           CAN NO LONGER BE NULL.
 *      key_data                IN:  Data to search for
 *      entry_data,             OUT: Data if found
 *                           CAN NO LONGER BE NULL.  Must be big enough
 *                           to handle the appropriate data.
 *      lowest_match         IN:  For sorted tables only.
 *                           If there are duplicate entries in the
 *                           table, and lowest_match is 1, then the
 *                           matching entry at the lowest index is
 *                           returned.  If lowest_match is 0, then any
 *                           of the matching entries may be picked at
 *                           random, which can be faster.
 * Returns:
 *    SOC_E_NOT_FOUND
 *                 Entry not found:  in this case, if
 *                           table is sorted, then index_ptr gets the
 *                           location in which the entry should be
 *                           inserted.
 *    SOC_E_NONE         Entry is found:  index_ptr gets location
 *    SOC_E_XXX         If internal error occurs
 * Notes:
 *    A binary search is performed for a matching table entry.
 *        The appropriate field(s) of entry_data are used for the key.
 *        All other fields in entry_data are ignored.
 *
 *    If found, the index is stored in index_ptr, and if entry_data is
 *        non-NULL, the contents of the found entry are written into it,
 *        and 1 is returned.
 *    If not found, the index of a correct insertion point for the
 *        entry is stored in index_ptr and 0 is returned (unless this
 *        is a hashed table).
 *    If a table read error occurs, SOC_E_XXX is returned.
 *
 *    For the ARL table, all entries are searched.  On revision GSL and
 *        higher, a hardware lookup feature is used.  For earlier
 *        revisions, a software search is used, which is not
 *        recommended since the ARL table may be under constant
 *        update by the hardware.  For hardware lookup, the index_ptr
 *        is written with -1.
 *    For non-ARL sorted tables, only the entries known to contain data
 *        are searched:
 *        (index_min <= search_index < index_min + memInfo.count).
 */

int
soc_mem_search(int unit,
               soc_mem_t mem,
               int copyno,
               int *index_ptr,
               void *key_data,
               void *entry_data,
               int lowest_match)
{
    int rv = 0;

    COMPILER_REFERENCE(rv);
    assert(soc_mem_is_sorted(unit, mem) ||
           soc_mem_is_hashed(unit, mem) ||
           soc_mem_is_cam(unit, mem) ||
           soc_mem_is_cmd(unit, mem));
    assert(entry_data);
    assert(index_ptr);

    if (copyno == MEM_BLOCK_ANY) {
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }
    assert(SOC_MEM_BLOCK_VALID(unit, mem, copyno));

#ifdef BCM_TRX_SUPPORT
    if (soc_feature(unit, soc_feature_generic_table_ops)) {
        int32 banks = 0;
        _SOC_MEM_REPLACE_MEM(unit, mem);
        switch (mem) {
        case L2Xm:
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
#ifdef BCM_ISM_SUPPORT
        case L2_ENTRY_1m:
        case L2_ENTRY_2m:
        case L3_ENTRY_1m:
        case L3_ENTRY_2m:
        case L3_ENTRY_4m:
        case VLAN_XLATE_EXTDm:
        case MPLS_ENTRY_EXTDm:
#endif /* BCM_ISM_SUPPORT */
        case EGR_VLAN_XLATEm:
        case VLAN_XLATEm:
        case MPLS_ENTRYm:
        /* passthru */
        /* coverity[fallthrough: FALSE] */
#ifdef BCM_TRIUMPH3_SUPPORT
        case AXP_WRX_WCDm:
        case AXP_WRX_SVP_ASSIGNMENTm:
        case EXT_L2_ENTRY_1m:
        case EXT_L2_ENTRY_2m:
        case FT_SESSIONm:
        case FT_SESSION_IPV6m:
#endif /* BCM_TRIUMPH3_SUPPORT */
        case VLAN_MACm:
#ifdef BCM_TRIDENT2_SUPPORT
        case ING_VP_VLAN_MEMBERSHIPm:
        case EGR_VP_VLAN_MEMBERSHIPm:
        case ING_DNAT_ADDRESS_TYPEm:
        case L2_ENDPOINT_IDm:
        case ENDPOINT_QUEUE_MAPm:
#endif /* BCM_TRIDENT2_SUPPORT */
#ifdef BCM_KATANA2_SUPPORT
        case EGR_MP_GROUPm:
#endif
#ifdef BCM_SABER2_SUPPORT
        case MP_GROUPm:
#endif
        case EXT_L2_ENTRYm:
            if (soc_feature(unit, soc_feature_ism_memory) &&
                soc_mem_is_mview(unit, mem)) {
                banks = -1;
            }
            return soc_mem_generic_lookup(unit, mem, copyno, banks,
                                          key_data, entry_data, index_ptr);
        default:
            break;
        }
    }
#endif /* BCM_TRX_SUPPORT */

#ifdef BCM_FIREBOLT_SUPPORT
    if (SOC_IS_FBX(unit)) {
        switch (mem) {
        case L2Xm:
            return soc_fb_l2x_lookup(unit, key_data,
                                     entry_data, index_ptr);
#if defined(INCLUDE_L3)
        case L3_DEFIPm:
            return soc_fb_lpm_match(unit, key_data, entry_data, index_ptr);
        case L3_ENTRY_IPV4_UNICASTm:
        case L3_ENTRY_IPV4_MULTICASTm:
        case L3_ENTRY_IPV6_UNICASTm:
        case L3_ENTRY_IPV6_MULTICASTm:
            return soc_fb_l3x_lookup(unit, key_data, entry_data, index_ptr);
#endif /* INCLUDE_L3 */
        case VLAN_MACm:
            return soc_fb_vlanmac_entry_lkup(unit, key_data, entry_data, index_ptr);
        default:
            break;
        }
    }
#endif /* BCM_FIREBOLT_SUPPORT */

    return _soc_mem_search(unit, mem, copyno, index_ptr, key_data,
                           entry_data, lowest_match);
}


#if defined(BCM_FIREBOLT2_SUPPORT) || defined(BCM_TRX_SUPPORT) \
         || defined(BCM_RAVEN_SUPPORT) || defined(BCM_HAWKEYE_SUPPORT)

/*
 * Function:
 *    soc_mem_bank_insert
 * Purpose:
 *    Do a single insert to a banked hash table with bank selection
 */
int
soc_mem_bank_insert(int unit,
                    soc_mem_t mem,
                    int32 banks,
                    int copyno,
                    void *entry_data,
                    void *old_entry_data)
{
    if (SOC_CONTROL(unit)->soc_mem_watch) {
        if (SOC_CONTROL(unit)->prev_mem != mem) {
            LOG_CLI((BSL_META_U(unit, "%d:%s MEM: %s\n"),
                     unit, BSL_FUNC, SOC_MEM_NAME(unit, mem)));
            SOC_CONTROL(unit)->prev_mem = mem;
        }
    }
    if (soc_feature(unit, soc_feature_generic_table_ops)) {
        return soc_mem_generic_insert(unit, mem, MEM_BLOCK_ANY,
                                      banks, entry_data, old_entry_data, 0);
    }

    switch (mem) {
    case L2Xm:
        return soc_fb_l2x_bank_insert(unit, banks,
                                      (l2x_entry_t *)entry_data);
#if defined(INCLUDE_L3)
    case L3_ENTRY_IPV4_UNICASTm:
    case L3_ENTRY_IPV4_MULTICASTm:
    case L3_ENTRY_IPV6_UNICASTm:
    case L3_ENTRY_IPV6_MULTICASTm:
        return soc_fb_l3x_bank_insert(unit, banks,
                        (l3_entry_ipv6_multicast_entry_t *)entry_data);
#endif
#if defined(BCM_RAVEN_SUPPORT)
    case VLAN_MACm:
    if (SOC_IS_RAVEN(unit) || SOC_IS_HAWKEYE(unit)) {
        return soc_fb_vlanmac_entry_bank_ins(unit, banks,
            (vlan_mac_entry_t *)entry_data);
    } else {
        return SOC_E_UNAVAIL;
    }
#endif
    default:
        break;
    }

    return SOC_E_UNAVAIL;
}

int soc_fb_vlanmac_entry_bank_delete(int unit,
                                     int32 banks,
                                     vlan_mac_entry_t *entry_data)
{
    return soc_fb_vlanmac_entry_del(unit, (vlan_mac_entry_t *)entry_data);
}

/*
 * Function:
 *      soc_mem_bank_delete
 * Purpose:
 *      Do a single delete to a banked hash table with bank selection
 * Parameters:
 *      unit - StrataSwitch unit #
 *      mem -  Memory in which the entry has to be deleted.
 *      banks - For dual hashing, which halves are selected (inverted)
 *      copyno - Specific copy of table/memory or all if -1.
 *      entry_data - entry to be deleted.
 *      old_entry_data (OUT) - If found, receives table index where
 *                  entry was deleted.
 * Returns:
 *      SOC_E_INTERNAL if retries exceeded or other internal error
 *      SOC_E_NOT_FOUND if the entry is not found.
 *      SOC_E_NONE (0) on success (entry found):
 */
int
soc_mem_bank_delete(int unit,
                    soc_mem_t mem,
                    int32 banks,
                    int copyno,
                    void *entry_data,
                    void *old_entry_data)
{
    if (soc_feature(unit, soc_feature_generic_table_ops)) {
        return soc_mem_generic_delete(unit, mem, MEM_BLOCK_ANY,
                                      banks, entry_data, old_entry_data, 0);
    }

    switch (mem) {
    case L2Xm:
        return soc_fb_l2x_bank_delete(unit, banks,
                                      (l2x_entry_t *)entry_data);
#if defined(INCLUDE_L3)
    case L3_ENTRY_IPV4_UNICASTm:
    case L3_ENTRY_IPV4_MULTICASTm:
    case L3_ENTRY_IPV6_UNICASTm:
    case L3_ENTRY_IPV6_MULTICASTm:
        return soc_fb_l3x_bank_delete(unit, banks,
                        (l3_entry_ipv6_multicast_entry_t *)entry_data);
#endif
#if defined(BCM_RAVEN_SUPPORT)
    case VLAN_MACm:
    if (SOC_IS_RAVEN(unit) || SOC_IS_HAWKEYE(unit)) {
        return soc_fb_vlanmac_entry_bank_delete(unit, banks,
            (vlan_mac_entry_t *)entry_data);
    } else {
        return SOC_E_UNAVAIL;
    }
#endif
    default:
        break;
    }

    return SOC_E_UNAVAIL;
}


/*
 * Function:
 *    soc_mem_bank_search
 * Purpose:
 *    Search a banked hash table with bank selection
 */
int
soc_mem_bank_search(int unit,
                    soc_mem_t mem,
                    uint8 banks,
                    int copyno,
                    int *index_ptr,
                    void *key_data,
                    void *entry_data)
{
    switch (mem) {
    case L2Xm:
        return soc_fb_l2x_bank_lookup(unit, banks, (l2x_entry_t *)key_data,
                                      (l2x_entry_t *)entry_data, index_ptr);
#if defined(INCLUDE_L3)
    case L3_ENTRY_IPV4_UNICASTm:
    case L3_ENTRY_IPV4_MULTICASTm:
    case L3_ENTRY_IPV6_UNICASTm:
    case L3_ENTRY_IPV6_MULTICASTm:
        return soc_fb_l3x_bank_lookup(unit, banks,
                        (l3_entry_ipv6_multicast_entry_t *)key_data,
                        (l3_entry_ipv6_multicast_entry_t *)entry_data,
                                        index_ptr);
#endif
    default:
        break;
    }

    return SOC_E_UNAVAIL;
}

/*
 * Function:
 *    _soc_mem_dual_hash_get
 * Purpose:
 *    Calcualte hash for dual hashed memories
 */

STATIC int
_soc_mem_dual_hash_get(int unit, soc_mem_t mem, int hash_sel, int bank,
                       void *entry_data)
{
    switch (mem) {
    case L2Xm:
#if defined(BCM_TRX_SUPPORT)
    if (SOC_IS_TRX(unit)) {
        return soc_tr_l2x_entry_hash(unit, hash_sel, entry_data);
    } else
#endif
    {
        return soc_fb_l2x_entry_hash(unit, hash_sel, entry_data);
    }
#if defined(INCLUDE_L3)
    case L3_ENTRY_IPV4_UNICASTm:
    case L3_ENTRY_IPV4_MULTICASTm:
    case L3_ENTRY_IPV6_UNICASTm:
    case L3_ENTRY_IPV6_MULTICASTm:
        return soc_fb_l3x_entry_hash(unit, hash_sel, entry_data);
#endif /* INCLUDE_L3 */
#if defined(BCM_RAVEN_SUPPORT) || defined(BCM_TRIDENT2_SUPPORT)
    case VLAN_MACm:
#if defined(BCM_TRIDENT2_SUPPORT)
        if (SOC_IS_TD2_TT2(unit)) {
            return soc_td2_vlan_xlate_entry_hash(unit, hash_sel, bank, entry_data);
        } else
#endif
        if (SOC_IS_RAVEN(unit)  || SOC_IS_HAWKEYE(unit)) {
            return soc_fb_vlan_mac_entry_hash(unit, hash_sel, entry_data);
        } else {
            return -1;
        }
#endif

#if defined(BCM_TRX_SUPPORT)
    case VLAN_XLATEm:
#if defined(BCM_TRIDENT2_SUPPORT)
        if (SOC_IS_TD2_TT2(unit)) {
#ifdef BCM_APACHE_SUPPORT
            if (SOC_IS_APACHE(unit)) {
                return soc_ap_vlan_xlate_entry_hash(unit, hash_sel, bank,
                                                    entry_data);
            } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
            if (SOC_IS_TOMAHAWKX(unit)) {
                return soc_th_vlan_xlate_entry_hash(unit, hash_sel, bank,
                                                    entry_data);
            } else
#endif /* BCM_TOMAHAWK_SUPPORT */
            {
                return soc_td2_vlan_xlate_entry_hash(unit, hash_sel, bank,
                                                     entry_data);
            }
        } else
#endif /* BCM_TRIDENT2_SUPPORT */
        if (SOC_IS_TRX(unit)) {
            return soc_tr_vlan_xlate_entry_hash(unit, hash_sel, entry_data);
        }
        return -1;

    case MPLS_ENTRYm:
#if defined(BCM_TRIDENT2_SUPPORT)
        if (SOC_IS_TD2_TT2(unit)) {
 #ifdef BCM_APACHE_SUPPORT
            if (SOC_IS_APACHE(unit)) {
                return soc_ap_mpls_entry_hash(unit, hash_sel, bank,
                                              entry_data);
            } else
#endif /* BCM_APACHE_SUPPORT */
 #ifdef BCM_TOMAHAWK_SUPPORT
            if (SOC_IS_TOMAHAWKX(unit)) {
                return soc_th_mpls_entry_hash(unit, hash_sel, bank,
                                              entry_data);
            } else
#endif /* BCM_TOMAHAWK_SUPPORT */
            {
                return soc_td2_mpls_entry_hash(unit, hash_sel, bank, entry_data);
            }
        } else
#endif /* BCM_TRIDENT2_SUPPORT */
        if (SOC_IS_TR_VL(unit)) {
            return soc_tr_mpls_entry_hash(unit, hash_sel, entry_data);
        }
        return -1;

    case EGR_VLAN_XLATEm:
#if defined(BCM_TRIDENT2_SUPPORT)
        if (SOC_IS_TD2_TT2(unit)) {
#ifdef BCM_APACHE_SUPPORT
            if (SOC_IS_APACHE(unit)) {
                return soc_ap_egr_vlan_xlate_entry_hash(unit, hash_sel,
                                                        bank, entry_data);
            } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
            if (SOC_IS_TOMAHAWKX(unit)) {
                return soc_th_egr_vlan_xlate_entry_hash(unit, hash_sel,
                                                        bank, entry_data);
            } else
#endif /* BCM_TOMAHAWK_SUPPORT */
            {
                return soc_td2_egr_vlan_xlate_entry_hash(unit, hash_sel,
                                                         bank, entry_data);
            }
        } else
#endif /* BCM_TRIDENT2_SUPPORT */
        if (SOC_IS_TRX(unit)) {
            return soc_tr_egr_vlan_xlate_entry_hash(unit, hash_sel, entry_data);
        }
        return -1;
#endif

#ifdef BCM_TRIUMPH3_SUPPORT
    case AXP_WRX_WCDm:
    case AXP_WRX_SVP_ASSIGNMENTm:
        if (SOC_IS_TRIUMPH3(unit)) {
            return soc_tr3_wlan_entry_hash(unit, mem, hash_sel, entry_data);
        }
        return -1;
    case FT_SESSIONm:
    case FT_SESSION_IPV6m:
        if (SOC_IS_TRIUMPH3(unit)) {
            return soc_tr3_ft_session_entry_hash(unit, mem, hash_sel, entry_data);
        }
        return -1;
#endif /* BCM_TRIUMPH3_SUPPORT */

#ifdef BCM_TRIDENT2_SUPPORT
    case ING_VP_VLAN_MEMBERSHIPm:
        if (SOC_IS_TD2_TT2(unit)) {
            return soc_td2_ing_vp_vlan_member_entry_hash(unit, hash_sel, bank,
                                                         entry_data);
        }
        return -1;
    case EGR_VP_VLAN_MEMBERSHIPm:
        if (SOC_IS_TD2_TT2(unit)) {
            return soc_td2_egr_vp_vlan_member_entry_hash(unit, hash_sel, bank,
                                                         entry_data);
        }
        return -1;
    case ING_DNAT_ADDRESS_TYPEm:
        if (SOC_IS_TD2_TT2(unit)) {
            return soc_td2_ing_dnat_address_type_entry_hash(unit, hash_sel, bank,
                                                            entry_data);
        }
        return -1;
    case L2_ENDPOINT_IDm:
        if (SOC_IS_TD2_TT2(unit)) {
#ifdef BCM_APACHE_SUPPORT
            if (SOC_IS_APACHE(unit)) {
                return soc_ap_l2_endpoint_id_entry_hash(unit, hash_sel, bank,
                        entry_data);
            } else
#endif /* BCM_APACHE_SUPPORT */
            {
                return soc_td2_l2_endpoint_id_entry_hash(unit, hash_sel, bank,
                        entry_data);
            }
        }
        return -1;
    case ENDPOINT_QUEUE_MAPm:
        if (SOC_IS_TD2_TT2(unit)) {
#ifdef BCM_APACHE_SUPPORT
            if (SOC_IS_APACHE(unit)) {
                return soc_ap_endpoint_queue_map_entry_hash(unit, hash_sel,
                        bank, entry_data);
            } else
#endif /* BCM_APACHE_SUPPORT */
            {
                return soc_td2_endpoint_queue_map_entry_hash(unit, hash_sel,
                        bank, entry_data);
            }
        }
        return -1;
#endif /* BCM_TRIDENT2_SUPPORT */

    default:
        return -1;
    }
}

/*
 * Function:
 *    _soc_mem_dual_hash_move
 * Purpose:
 *    Recursive move routine for dual hash auto-move inserts
 *      Assumes feature already checked, mem locks taken.
 */

STATIC int
_soc_mem_dual_hash_move(int unit, soc_mem_t mem, /* Assumes memory locked */
                        uint8 banks, int copyno, void *entry_data,
                        dual_hash_info_t *hash_info,
                        SHR_BITDCL *bucket_trace, int recurse_depth,
                        int recurse_in )
{
    int hash_base, cur_index = -1, base_index, bucket_index;
    int dest_hash_base, dest_bucket_index = -1;
    int half_bucket, hw_war = 0, trace_size = 0;
    SHR_BITDCL *trace;               /* Buckets involved in recursion.   */
    int rv = SOC_E_NONE, bix, i, found = FALSE;
    uint8 this_bank_bit, this_bank_only, that_bank_only, this_hash, that_hash;
    uint32 move_entry[SOC_MAX_MEM_WORDS];
    int recurse = recurse_in;

    if (recurse_depth < 0) {
        return SOC_E_FULL;
    }
    if (((SOC_IS_TRIUMPH(unit) || SOC_IS_VALKYRIE(unit) ||
          SOC_IS_SCORPION(unit) || SOC_IS_ENDURO(unit)) &&
         (mem == EGR_VLAN_XLATEm)) ||
         soc_feature(unit, soc_feature_hr2_dual_hash)) {
        /* Handle h/w limitation for this memory */
        hw_war = TRUE;
    }
    /* Stack variables initialization & memory allocations.*/
    half_bucket = hash_info->bucket_size / 2;

    /* Keep back trace of all buckets affected by recursion. */
    if (NULL == bucket_trace) {
        trace_size =
            SHR_BITALLOCSIZE(soc_mem_index_count(unit, hash_info->base_mem)/
                             half_bucket);
        trace =  sal_alloc(trace_size, "Dual hash");
        if (NULL == trace) {
            return (SOC_E_MEMORY);
        }
        recurse = 0;
    } else {
        trace = bucket_trace;
        recurse++;
    }

    /* Iterate over banks. */
    for (bix = 0; bix < 2; bix++) {
        this_bank_bit =
            (bix == 0) ? SOC_MEM_HASH_BANK0_BIT : SOC_MEM_HASH_BANK1_BIT;
        this_bank_only =
            (bix == 0) ? SOC_MEM_HASH_BANK0_ONLY : SOC_MEM_HASH_BANK1_ONLY;
        that_bank_only =
            (bix == 0) ? SOC_MEM_HASH_BANK1_ONLY : SOC_MEM_HASH_BANK0_ONLY;
        this_hash = (bix == 0) ? hash_info->hash_sel0 : hash_info->hash_sel1;
        that_hash = (bix == 0) ? hash_info->hash_sel1 : hash_info->hash_sel0;

        if (banks & this_bank_bit) {
            /* Not this bank */
            continue;
        }
        hash_base = _soc_mem_dual_hash_get(unit, mem, this_hash, bix,
                                           entry_data);
        if (hash_base == -1) {
            rv = SOC_E_INTERNAL;
            break;
        }

        /* Bucket half is based on bank id.      */
        bucket_index = hash_base * hash_info->bucket_size + bix * half_bucket;

        /* Recursion trace initialization. */
        if (NULL == bucket_trace) {
            sal_memset(trace, 0, trace_size);
        }
        SHR_BITSET(trace, bucket_index/half_bucket);

        /* Iterate over entries in the half-bucket.. */
        for (i = 0; i < (hash_info->bucket_size / 2); i++) {
            cur_index = base_index = bucket_index + i;
            /* Read bucket entry. */
            rv = soc_mem_read(unit, hash_info->base_mem, copyno,
                              base_index, move_entry);
            if (SOC_FAILURE(rv)) {
                rv = SOC_E_MEMORY;
                break;
            }
            /* Is this an exception mem OR
               Are we already in recursion or will we be in recursion */
            if (hw_war || recurse || recurse_depth) {
                /* Calculate destination entry hash value. */
                dest_hash_base = _soc_mem_dual_hash_get(unit, mem, that_hash,
                                                        !bix, move_entry);
                if (dest_hash_base == -1) {
                    rv = SOC_E_INTERNAL;
                    break;
                }
                dest_bucket_index =
                    dest_hash_base * hash_info->bucket_size + (!bix) * half_bucket;

                /* Make sure we are not touching buckets in bucket trace. */
                if(SHR_BITGET(trace, dest_bucket_index/half_bucket)) {
                    continue;
                }
            }

            /* Attempt to insert it into the other bank. */
            if (hw_war) {
                int e, notfound = TRUE;
                uint32 tmp_entry[SOC_MAX_MEM_WORDS];

                for (e = 0; e < (hash_info->bucket_size / 2); e++) {
                    rv = soc_mem_read(unit, mem, copyno,
                                      dest_bucket_index + e, tmp_entry);
                    if (SOC_FAILURE(rv)) {
                        rv = SOC_E_MEMORY;
                        break;
                    }
                    if (soc_mem_field32_get(unit, mem, tmp_entry, VALIDf)) {
                        continue;
                    } else {
                        rv = soc_mem_write(unit, mem, copyno, dest_bucket_index + e,
                                           move_entry);
                        if (SOC_FAILURE(rv)) {
                            rv = SOC_E_MEMORY;
                            break;
                        }
                        notfound = FALSE;
                        break;
                    }
                }
                if ((rv != SOC_E_MEMORY) && (notfound == TRUE)) {
                    rv = SOC_E_FULL;
                }
            } else {
                rv = soc_mem_bank_insert(unit, mem, that_bank_only,
                                         copyno, move_entry, NULL);
            }
            if (SOC_FAILURE(rv)) {
                if (rv != SOC_E_FULL) {
                    if ((rv == SOC_E_EXISTS) &&
                        bsl_check(bslLayerSoc, bslSourceCommon,
                                  bslSeverityVerbose, unit)) {
                        soc_mem_entry_dump(unit, mem, move_entry);
                    }
                    break;
                }
                /* Recursive call - attempt to create a slot
                   in other bank bucket. */
                rv = _soc_mem_dual_hash_move(unit, mem, that_bank_only,
                                             copyno, move_entry, hash_info,
                                             trace, recurse_depth - 1,
                                             recurse);
                if (SOC_FAILURE(rv)) {
                    if (rv != SOC_E_FULL) {
                        if ((rv == SOC_E_EXISTS) &&
                            bsl_check(bslLayerSoc, bslSourceCommon,
                                      bslSeverityVerbose, unit)) {
                            soc_mem_entry_dump(unit, mem, move_entry);
                        }
                        break;
                    }
                    continue;
                }
            }
            /* Entry was moved successfully. */
            found = TRUE;
            /* Delete old entry from original location */
            if (hw_war) {
                /* A entry clear is not needed as the entry is written over later on.
                   soc_mem_field32_set(unit, mem, &move_entry, VALIDf, 0);
                   rv = soc_mem_write(unit, mem, copyno, cur_index, move_entry);
                */
            } else {
                rv = soc_mem_bank_delete(unit, mem, this_bank_only, MEM_BLOCK_ANY,
                                            move_entry, NULL);
            }
            break;
        }  /* Bucket iteration loop. */

        if (found || ((rv < 0) && (rv != SOC_E_FULL))) {
            break;
        }
    } /* Bank iteration loop. */

    if ((rv < 0) && (rv != SOC_E_FULL)) {
        if (NULL == bucket_trace) sal_free(trace);
        return rv;
    }

    if (!found) {
        if (NULL == bucket_trace) sal_free(trace);
        return SOC_E_FULL;
    }
    if (hw_war) {
        rv = soc_mem_write(unit, mem, copyno, cur_index, entry_data);
    } else {
            rv = soc_mem_bank_insert(unit, mem, this_bank_only,copyno,
                                        entry_data, NULL);
    }
    if (NULL == bucket_trace) sal_free(trace);
    return (rv);
}

/*
 * Function:
 *    _soc_mem_dual_hash_insert
 * Purpose:
 *    Dual hash auto-move inserts
 *      Assumes feature already checked
 */

int
_soc_mem_dual_hash_insert(int unit,
                          soc_mem_t mem,     /* Assumes memory locked */
                          int copyno,
                          void *entry_data,
                          void *old_entry_data,
                          int recurse_depth)
{
    int rv = SOC_E_NONE;
    dual_hash_info_t hash_info = {0};
#ifdef BCM_TRIUMPH3_SUPPORT
    uint32 regval;
#endif /* BCM_TRIUMPH3_SUPPORT */

    switch (mem) {
    case L2Xm:
        rv = soc_mem_bank_insert(unit, mem, 0, copyno,
                                 entry_data, old_entry_data);
        if (rv != SOC_E_FULL || recurse_depth == 0) {
            return rv;
        }
        SOC_IF_ERROR_RETURN
            (soc_fb_l2x_entry_bank_hash_sel_get(unit, 0,
                                                &(hash_info.hash_sel0)));
        SOC_IF_ERROR_RETURN
            (soc_fb_l2x_entry_bank_hash_sel_get(unit, 1,
                                                &(hash_info.hash_sel1)));
        if (hash_info.hash_sel0 == hash_info.hash_sel1) {
            /* Can't juggle the entries */
            return SOC_E_FULL;
        }
        hash_info.bucket_size = SOC_L2X_BUCKET_SIZE;
        hash_info.base_mem = mem;

        /* Time to shuffle the entries */
        SOC_IF_ERROR_RETURN(soc_l2x_freeze(unit));
        rv = _soc_mem_dual_hash_move(unit, mem, SOC_MEM_HASH_BANK_BOTH,
                                     copyno, entry_data, &hash_info,
                                     NULL, recurse_depth - 1,0);
        SOC_IF_ERROR_RETURN(soc_l2x_thaw(unit));
        return rv;
#if defined(BCM_TRIUMPH_SUPPORT)
#endif /* BCM_TRIUMPH_SUPPORT */
#ifdef BCM_TRX_SUPPORT
    case VLAN_XLATEm:
        if (!SOC_IS_TRX(unit)) {
            break;
        }
        /* Fall thru for TRX (VLAN_XLATE & VLAN_MAC are a shared table) */
#endif /* BCM_TRX_SUPPORT */
#if defined(BCM_RAVEN_SUPPORT) || defined(BCM_TRX_SUPPORT)
    case VLAN_MACm:
        if (SOC_IS_FIREBOLT2(unit)) {
            break;
        }
    rv = soc_mem_bank_insert(unit, mem, 0, copyno,
                                 entry_data, old_entry_data);
        if (rv != SOC_E_FULL || recurse_depth == 0) {
            return rv;
        }
#ifdef BCM_TRX_SUPPORT
        if (mem == VLAN_XLATEm) {
            SOC_IF_ERROR_RETURN
                (soc_tr_hash_sel_get(unit, mem, 0, &hash_info.hash_sel0));
            SOC_IF_ERROR_RETURN
                (soc_tr_hash_sel_get(unit, mem, 1, &hash_info.hash_sel1));
            hash_info.bucket_size = SOC_VLAN_XLATE_BUCKET_SIZE;
        } else
#endif /* BCM_TRX_SUPPORT */
        {
            SOC_IF_ERROR_RETURN
                (soc_fb_rv_vlanmac_hash_sel_get(unit, 0, &(hash_info.hash_sel0)));

            SOC_IF_ERROR_RETURN
                (soc_fb_rv_vlanmac_hash_sel_get(unit, 1, &(hash_info.hash_sel1)));
            hash_info.bucket_size = SOC_VLAN_MAC_BUCKET_SIZE;
        }
        if (hash_info.hash_sel0 == hash_info.hash_sel1) {
            /* Can't juggle the entries */
            return SOC_E_FULL;
        }
        hash_info.base_mem = mem;
        /* Time to shuffle the entries */
        if (SOC_IS_TRX(unit)) {
            soc_mem_lock(unit, VLAN_XLATEm);
        }
        soc_mem_lock(unit, VLAN_MACm);
        rv = _soc_mem_dual_hash_move(unit, mem, SOC_MEM_HASH_BANK_BOTH,
                                     copyno, entry_data, &hash_info,
                                     NULL, recurse_depth - 1,0);
        soc_mem_unlock(unit, VLAN_MACm);
        if (SOC_IS_TRX(unit)) {
            soc_mem_unlock(unit, VLAN_XLATEm);
        }
        return rv;
#endif /* BCM_RAVEN_SUPPORT || BCM_TRX_SUPPORT */
#if defined(INCLUDE_L3)
    case L3_ENTRY_ONLYm:
    case L3_ENTRY_IPV4_UNICASTm:
    case L3_ENTRY_IPV4_MULTICASTm:
    case L3_ENTRY_IPV6_UNICASTm:
    case L3_ENTRY_IPV6_MULTICASTm:
        return soc_fb_l3x_insert(unit, entry_data);
#endif /* INCLUDE_L3 */
#if defined(BCM_TRX_SUPPORT)
    case MPLS_ENTRYm:
        hash_info.bucket_size = SOC_MPLS_ENTRY_BUCKET_SIZE;
        break;
    case EGR_VLAN_XLATEm:
        hash_info.bucket_size = SOC_EGR_VLAN_XLATE_BUCKET_SIZE;
        break;
    case AXP_WRX_WCDm:
    case AXP_WRX_SVP_ASSIGNMENTm:
        hash_info.bucket_size = SOC_WLAN_BUCKET_SIZE;
        break;
#ifdef BCM_TRIUMPH3_SUPPORT
    case FT_SESSIONm:
    case FT_SESSION_IPV6m:
        SOC_IF_ERROR_RETURN(READ_FT_CONFIGr(unit, &regval));
        hash_info.bucket_size = 1 << soc_reg_field_get(unit,
                                            FT_CONFIGr, regval, TABLE_SIZEf);
        break;
#endif /* BCM_TRIUMPH3_SUPPORT */
#ifdef BCM_TRIDENT2_SUPPORT
    case ING_VP_VLAN_MEMBERSHIPm:
        hash_info.bucket_size = SOC_ING_VP_VLAN_MEMBER_BUCKET_SIZE;
        break;
    case EGR_VP_VLAN_MEMBERSHIPm:
        hash_info.bucket_size = SOC_EGR_VP_VLAN_MEMBER_BUCKET_SIZE;
        break;
    case ING_DNAT_ADDRESS_TYPEm:
        hash_info.bucket_size = SOC_ING_DNAT_ADDRESS_TYPE_BUCKET_SIZE;
        break;
    case L2_ENDPOINT_IDm:
        hash_info.bucket_size = SOC_L2_ENDPOINT_ID_BUCKET_SIZE;
        break;
    case ENDPOINT_QUEUE_MAPm:
        hash_info.bucket_size = SOC_ENDPOINT_QUEUE_MAP_BUCKET_SIZE;
        break;
#endif /* BCM_TRIDENT2_SUPPORT */
#ifdef BCM_KATANA2_SUPPORT
    case EGR_MP_GROUPm:
        hash_info.bucket_size = SOC_EGR_MP_GROUP_BUCKET_SIZE;
        break;
#endif
#ifdef BCM_SABER2_SUPPORT
    case MP_GROUPm:
        hash_info.bucket_size = SOC_EGR_MP_GROUP_BUCKET_SIZE;
        break;
#endif
#endif /* BCM_TRX_SUPPORT */
    default:
        return SOC_E_UNAVAIL;
    }

#if defined(BCM_TRX_SUPPORT)
    /*
     * If a block of physical memory is not shared by multiple table and
     * it is not modified by hardware, we should be able to use general case
     * for dual hash insert
     */
    rv = soc_mem_bank_insert(unit, mem, 0, copyno, entry_data, old_entry_data);
    if (rv != SOC_E_FULL || recurse_depth == 0) {
        return rv;
    }
#ifdef BCM_TRIDENT2_SUPPORT
    if (SOC_IS_TD2_TT2(unit)) {
        SOC_IF_ERROR_RETURN
            (soc_td2_hash_sel_get(unit, mem, 0, &hash_info.hash_sel0));
        SOC_IF_ERROR_RETURN
            (soc_td2_hash_sel_get(unit, mem, 1, &hash_info.hash_sel1));
    } else
#endif /* BCM_TRIDENT2_SUPPORT */
    {
        SOC_IF_ERROR_RETURN
            (soc_tr_hash_sel_get(unit, mem, 0, &hash_info.hash_sel0));
        SOC_IF_ERROR_RETURN
            (soc_tr_hash_sel_get(unit, mem, 1, &hash_info.hash_sel1));
    }
    if (hash_info.hash_sel0 == hash_info.hash_sel1) {
        /* Can't juggle the entries */
        return SOC_E_FULL;
    }
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (mem == FT_SESSIONm || mem == FT_SESSION_IPV6m) {
        /* Can't juggle the entries - FT_SESSION entries don't move */
        return SOC_E_FULL;
    }
#endif /* BCM_TRIUMPH3_SUPPORT */
    hash_info.base_mem = mem;
    /* Time to shuffle the entries */
    soc_mem_lock(unit, mem);
    rv = _soc_mem_dual_hash_move(unit, mem, SOC_MEM_HASH_BANK_BOTH, copyno,
                                 entry_data, &hash_info, NULL,
                                 recurse_depth - 1,0);
    soc_mem_unlock(unit, mem);
#endif /* BCM_TRX_SUPPORT */

    return rv;
}

#ifdef BCM_ISM_SUPPORT
/*
 * Function:
 *    _soc_mem_multi_hash_insert
 * Purpose:
 *    N hash auto-move inserts
 *      Assumes feature already checked
 */

STATIC int
_soc_mem_multi_hash_insert(int unit, soc_mem_t mem, int copyno,
                           void *entry_data, void *old_entry_data,
                           int recurse_depth)
{
    int rv = SOC_E_NONE;
    rv = soc_mem_bank_insert(unit, mem, SOC_MEM_HASH_BANK_ALL, copyno,
                             entry_data, old_entry_data);
    if (rv != SOC_E_FULL) {
        return rv;
    }
    /* Time to shuffle the entries */
    soc_mem_lock(unit, mem);
    rv = soc_mem_multi_hash_move(unit, mem, SOC_MEM_HASH_BANK_ALL, copyno,
                                 entry_data, NULL, NULL, recurse_depth-1);
    soc_mem_unlock(unit, mem);
    return rv;
}
#endif /* BCM_ISM_SUPPORT */

#ifdef BCM_TRIDENT2_SUPPORT
/*Function declaration */

#define NUM_ENT        4

typedef struct _soc_breadth_bucket2_s {
    int8 src_bix;
    int8 level;
    uint8 status;           /* l3 only */
    uint8 src_ent;

    uint8 width[NUM_ENT];   /* l3 only */
    soc_mem_t mem[NUM_ENT]; /*l3 only */
    uint32 bucket;
    uint32 p_bucket;        /* l3 only */
    uint32 s_bucket;
    int    n_bucket;
    union {
        l2x_entry_t l2[4];
        l3_entry_only_entry_t l3[4];
        exact_match_2_entry_t em[4];
    } e;
} _soc_breadth_bucket2_t;

typedef struct _soc_breadth_bucket_s {
    soc_mem_t src_mem;
    uint8 src_bix;
    uint8 status;
    uint8 src_width;
    uint8 src_ent;
    uint32 src_bkt_idx;
    uint32 bucket;
    union {
        l2x_entry_t l2[4];
        l3_entry_only_entry_t l3[4];
        exact_match_2_entry_t em[4];
    } e;
} _soc_breadth_bucket_t;

STATIC INLINE int
_soc_mem_shared_hash_init(int unit, soc_mem_t mem,
                          SHR_BITDCL **trace,
                          _soc_hash_bank_info_t *bank_info,
                          int num_levels,
                          _soc_breadth_bucket_t **buckets,
                          _soc_breadth_bucket2_t **buckets2);

STATIC int
_soc_mem_shared_hash_incr_per_entry(int unit, soc_mem_t mem, void *old_entry,
                                    void *new_entry)
{
    if (mem == L2Xm) {
        return 1;
    } else {
        int oi = 1;
        if (SOC_MEM_FIELD_VALID(unit, L3_ENTRY_ONLYm, KEY_TYPEf)) {
            int key_type;

            key_type = soc_mem_field32_get(unit, L3_ENTRY_ONLYm,
                                           old_entry, KEY_TYPEf);
            switch (key_type) {
            case TD2_L3_HASH_KEY_TYPE_V4UC:
            case TD2_L3_HASH_KEY_TYPE_TRILL:
            case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN:
            case TD2_L3_HASH_KEY_TYPE_FCOE_HOST:
            case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP:
                oi = 1;
                break;
            case TD2_L3_HASH_KEY_TYPE_V4UC_EXT:
            case TD2_L3_HASH_KEY_TYPE_V4MC:
            case TD2_L3_HASH_KEY_TYPE_V4L2MC:
            case TD2_L3_HASH_KEY_TYPE_V4L2VPMC:
            case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN_EXT:
            case TD2_L3_HASH_KEY_TYPE_FCOE_HOST_EXT:
            case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP_EXT:
            case TD2_L3_HASH_KEY_TYPE_DST_NAT:
            case TD2_L3_HASH_KEY_TYPE_DST_NAPT:
            case TD2_L3_HASH_KEY_TYPE_V6UC:
                oi = 2;
                break;
            case TD2_L3_HASH_KEY_TYPE_V6UC_EXT:
            case TD2_L3_HASH_KEY_TYPE_V6MC:
            case TD2_L3_HASH_KEY_TYPE_V6L2MC:
            case TD2_L3_HASH_KEY_TYPE_V6L2VPMC:
                oi = 4;
                break;
            default:
                oi = 1;
            }
            key_type = soc_mem_field32_get(unit, L3_ENTRY_ONLYm,
                                           new_entry, KEY_TYPEf);
            switch (key_type) {
            case TD2_L3_HASH_KEY_TYPE_V4UC:
            case TD2_L3_HASH_KEY_TYPE_TRILL:
            case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN:
            case TD2_L3_HASH_KEY_TYPE_FCOE_HOST:
            case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP:
                return oi;
            case TD2_L3_HASH_KEY_TYPE_V4UC_EXT:
            case TD2_L3_HASH_KEY_TYPE_V4MC:
            case TD2_L3_HASH_KEY_TYPE_V4L2MC:
            case TD2_L3_HASH_KEY_TYPE_V4L2VPMC:
            case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN_EXT:
            case TD2_L3_HASH_KEY_TYPE_FCOE_HOST_EXT:
            case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP_EXT:
            case TD2_L3_HASH_KEY_TYPE_DST_NAT:
            case TD2_L3_HASH_KEY_TYPE_DST_NAPT:
            case TD2_L3_HASH_KEY_TYPE_V6UC:
                return (oi > 2) ? 2 : 1;
            case TD2_L3_HASH_KEY_TYPE_V6UC_EXT:
            case TD2_L3_HASH_KEY_TYPE_V6MC:
            case TD2_L3_HASH_KEY_TYPE_V6L2MC:
            case TD2_L3_HASH_KEY_TYPE_V6L2VPMC:
                return 1;
            default:
                return (oi > 1) ? oi : 1;
            }
        } else {
            return 1;
        }
    }
}

/* return values:
 *   0: equals,
 *   1, 2, 4, wider times, old_entry>new_entry .
 *   -1, old_entry<new_entry
 */
STATIC int
_soc_mem_shared_hash_entry_wide_cmp(int unit, soc_mem_t mem, void *old_entry,
                                void *new_entry, soc_mem_t *wider_mem)
{
    int oi = 1;
    int ni = 1;

    *wider_mem = mem;
    if (mem == L2Xm) {
        return 0;
    } 

    if (SOC_MEM_FIELD_VALID(unit, L3_ENTRY_ONLYm, KEY_TYPEf)) {
        int key_type, key_type2;

        key_type = soc_mem_field32_get(unit, L3_ENTRY_ONLYm,
                                       old_entry, KEY_TYPEf);
        switch (key_type) {
        case TD2_L3_HASH_KEY_TYPE_V4UC:
        case TD2_L3_HASH_KEY_TYPE_TRILL:
        case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN:
        case TD2_L3_HASH_KEY_TYPE_FCOE_HOST:
        case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP:
            oi = 1;
            break;
        case TD2_L3_HASH_KEY_TYPE_V4UC_EXT:
        case TD2_L3_HASH_KEY_TYPE_V4MC:
        case TD2_L3_HASH_KEY_TYPE_V4L2MC:
        case TD2_L3_HASH_KEY_TYPE_V4L2VPMC:
        case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN_EXT:
        case TD2_L3_HASH_KEY_TYPE_FCOE_HOST_EXT:
        case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP_EXT:
        case TD2_L3_HASH_KEY_TYPE_DST_NAT:
        case TD2_L3_HASH_KEY_TYPE_DST_NAPT:
        case TD2_L3_HASH_KEY_TYPE_V6UC:
            oi = 2;
            break;
        case TD2_L3_HASH_KEY_TYPE_V6UC_EXT:
        case TD2_L3_HASH_KEY_TYPE_V6MC:
        case TD2_L3_HASH_KEY_TYPE_V6L2MC:
        case TD2_L3_HASH_KEY_TYPE_V6L2VPMC:
            oi = 4;
            break;
        default:
            oi = 1;
        }
        key_type2 = soc_mem_field32_get(unit, L3_ENTRY_ONLYm,
                                       new_entry, KEY_TYPEf);
        switch (key_type2) {
        case TD2_L3_HASH_KEY_TYPE_V4UC:
        case TD2_L3_HASH_KEY_TYPE_TRILL:
        case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN:
        case TD2_L3_HASH_KEY_TYPE_FCOE_HOST:
        case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP:
            ni = 1;
            break;
        case TD2_L3_HASH_KEY_TYPE_V4UC_EXT:
        case TD2_L3_HASH_KEY_TYPE_V4MC:
        case TD2_L3_HASH_KEY_TYPE_V4L2MC:
        case TD2_L3_HASH_KEY_TYPE_V4L2VPMC:
        case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN_EXT:
        case TD2_L3_HASH_KEY_TYPE_FCOE_HOST_EXT:
        case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP_EXT:
        case TD2_L3_HASH_KEY_TYPE_DST_NAT:
        case TD2_L3_HASH_KEY_TYPE_DST_NAPT:
        case TD2_L3_HASH_KEY_TYPE_V6UC:
            ni = 2;
            break;
        case TD2_L3_HASH_KEY_TYPE_V6UC_EXT:
        case TD2_L3_HASH_KEY_TYPE_V6MC:
        case TD2_L3_HASH_KEY_TYPE_V6L2MC:
        case TD2_L3_HASH_KEY_TYPE_V6L2VPMC:
            ni = 4;
            break;
        default:
            ni = 1;
        }

        if (oi == ni) {
            return 0;
        } else if (oi < ni) {
            return -1;
        } else { /* (oi > ni) */
            if (oi == 2) {
                *wider_mem = L3_ENTRY_IPV4_MULTICASTm;
            } else if (oi == 4) {
                *wider_mem = L3_ENTRY_IPV6_MULTICASTm;
            }
            return oi / ni;
        }
    }

    return 0;
}

STATIC int
_soc_mem_shared_hash_narrower_move_allowed(int v0, int v1, int v2, int v3,
                                        soc_mem_t *mem, int *index, int offset)
{
    int entry_num = v0 + v1 + v2 + v3;
    int double_wide_upper_num = v0 + v1;
    int double_wide_lower_num = v2 + v3;
    soc_mem_t ori_mem = *mem;
    int ori_idx = *index + offset;
    int index_offset = 0;

    switch (ori_mem) {
    case L3_ENTRY_IPV6_MULTICASTm:
        if (entry_num == 1) {
            index_offset = v0 ? 0 : (v1 ? 1 : (v2 ? 2 : (v3 ? 3 : 0)));
            *mem = L3_ENTRY_IPV4_UNICASTm;
            *index = ori_idx * 4 + index_offset;
            return 1;
        } else if (entry_num == 2) {
            if (double_wide_upper_num == 2 || double_wide_lower_num == 2) {
                index_offset = double_wide_upper_num ? 0 : 1;
                /* L3_ENTRY_IPV4_UNICASTm can be treated as V4_MULTICAST */
                *mem = L3_ENTRY_IPV4_MULTICASTm;
                *index = ori_idx * 2 + index_offset;
                return 1;
            }
        }
        break;
    case L3_ENTRY_IPV6_UNICASTm:
    case L3_ENTRY_IPV4_MULTICASTm:
        if (entry_num == 1) {
            index_offset = v0 ? 0 : 1;
            *mem = L3_ENTRY_IPV4_UNICASTm;
            *index = ori_idx * 2 + index_offset;
            return 1;
        }
        break;
    default:
        /* no entry narrower than L3_ENTRY_IPV4_UNICASTm */
        return 0;
    }

    return 0;
}

STATIC int
_soc_mem_shared_hash_move(int unit, soc_mem_t mem, int32 banks, int copyno,
                          void *entry, SHR_BITDCL *bucket_trace,
                          int recurse_depth,_soc_hash_bank_info_t *bank_info)
{
    SHR_BITDCL *trace = NULL;
    _soc_hash_bank_info_t bank_inf = {0};
    uint8 num_ent = 0;
    int32 nbix, cbix;
    int rv = SOC_E_NONE, cmp_rv, index, ori_index;
    uint8 i, bix, found = 0, cb = 0;
    uint32 db, dest_bucket;
    uint32 bucket, move_entry[SOC_MAX_MEM_WORDS];
    soc_mem_t wider_mem, ori_mem = mem;
    int banks_seq_num = 0;

    if (recurse_depth < 0) {
        return SOC_E_FULL;
    }
    /* Get the entry number per each bucket */
    num_ent = _soc_mem_shared_hash_entries_per_bkt(mem);

    /* Stack variables initialization & memory allocations */
    if (NULL == bucket_trace) {
        SOC_IF_ERROR_RETURN(
            _soc_mem_shared_hash_init(unit, mem, &trace, &bank_inf, 0, NULL, NULL));
    } else {
        trace = bucket_trace;
        if (NULL != bank_info) {
            sal_memcpy(&bank_inf, bank_info, sizeof(_soc_hash_bank_info_t));
        } else {
            rv = SOC_E_INTERNAL;
            goto clean_up;
        }
    }

    if (banks != SOC_MEM_HASH_BANK_ALL) {
#ifdef BCM_APACHE_SUPPORT
        if (SOC_IS_APACHE(unit)) {
            rv = soc_apache_hash_seq_number_get(unit, mem, banks,
                                                  &banks_seq_num);
        } else 
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
        if (SOC_IS_TOMAHAWKX(unit)) {
            rv = soc_tomahawk_hash_seq_number_get(unit, mem, banks,
                                                  &banks_seq_num);
        } else 
#endif /* BCM_TOMAHAWK_SUPPORT */
        {
            rv = soc_trident2_hash_seq_number_get(unit, mem, banks,
                                                  &banks_seq_num);
        }
        if (SOC_FAILURE(rv)) {
            goto clean_up;
        }
    } else {
        /* Make sure we will try from the first bank */
        banks_seq_num = -1;
    }

    /* Iterate over banks. */
    for (bix = 0; bix < bank_inf.num_banks; bix++) {
        /* if banks is present, try interate from next bank */
        cbix = banks_seq_num + bix + 1;
        cbix = cbix % bank_inf.num_banks;
        cb = bank_inf.bank_nums[cbix];
        if (banks == cb) {
            /* Not this bank */
            continue;
        }
        nbix = (cbix + 1) % bank_inf.num_banks; /* next bank index */
        db = bank_inf.bank_nums[nbix]; /* next destination bank */
#ifdef BCM_APACHE_SUPPORT
        if (SOC_IS_APACHE(unit)) {
            rv = soc_ap_hash_bucket_get(unit, mem, cb, entry, &bucket);
        } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
        if (SOC_IS_TOMAHAWKX(unit)) {
            rv = soc_th_hash_bucket_get(unit, mem, cb, entry, &bucket);
        } else
#endif /* BCM_TOMAHAWK_SUPPORT */
        {
            rv = soc_hash_bucket_get(unit, mem, cb, entry, &bucket);
        }
        if (SOC_FAILURE(rv)) {
            break;
        }
        SHR_BITSET(trace, (bank_inf.numb * cbix) + bucket);
#ifdef BCM_APACHE_SUPPORT
        if (SOC_IS_APACHE(unit)) {
            index = soc_ap_hash_index_get(unit, mem, cb, bucket);
        } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
        if (SOC_IS_TOMAHAWKX(unit)) {
            index = soc_th_hash_index_get(unit, mem, cb, bucket);
        } else
#endif /* BCM_TOMAHAWK_SUPPORT */
        {
            index = soc_hash_index_get(unit, mem, cb, bucket);
        }
        ori_index = index;
        for (i = 0; i < num_ent;) {
            uint32 v0, v1, v2, v3;
            rv = soc_mem_read(unit, mem, copyno, index+i, move_entry);
            if (SOC_FAILURE(rv)) {
                rv = SOC_E_MEMORY;
                break;
            }

            /* Could be an empty slot here */
            v0 = v1 = v2 = v3 = 0;
            switch (mem) {
            case L3_ENTRY_IPV6_MULTICASTm:
                soc_mem_field_get(unit, mem, move_entry, VALID_3f, &v3);
                soc_mem_field_get(unit, mem, move_entry, VALID_2f, &v2);
                /* pass through */
            case L3_ENTRY_IPV6_UNICASTm:
            case L3_ENTRY_IPV4_MULTICASTm:
                soc_mem_field_get(unit, mem, move_entry, VALID_1f, &v1);
                soc_mem_field_get(unit, mem, move_entry, VALID_0f, &v0);
                break;
            case L3_ENTRY_IPV4_UNICASTm:
            case L3_ENTRY_ONLYm:
            case L2Xm:
                soc_mem_field_get(unit, mem, move_entry, VALIDf, &v0);
                break;
            default:
                v0 = v1 = v2 = v3 = 1;
                break;
            }
            LOG_VERBOSE(BSL_LS_SOC_COMMON,
                        (BSL_META_U(unit,
                                    "Depth %d, 1st read %d index %d from bank %d bucket %d(%d %d %d %d)\n"),
                         recurse_depth, mem, index+i, cb, bucket,
                         v0, v1, v2, v3));

            if (!(v0 || v1 || v2 || v3)) {
                found = TRUE;
                break;
            }

            cmp_rv = _soc_mem_shared_hash_entry_wide_cmp(unit, mem, move_entry,
                                                         entry, &wider_mem);
            if (cmp_rv == 0) {
                /* Are we already in recursion or will we be in recursion */
                if (recurse_depth) {
                    /* Calculate destination entry hash value. */
#ifdef BCM_APACHE_SUPPORT
                    if (SOC_IS_APACHE(unit)) {
                        (void)soc_ap_hash_bucket_get(unit, mem, db, move_entry,
                                                    &dest_bucket);
                    } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
                    if (SOC_IS_TOMAHAWKX(unit)) {
                        (void)soc_th_hash_bucket_get(unit, mem, db, move_entry,
                                                    &dest_bucket);
                    } else
#endif /* BCM_TOMAHAWK_SUPPORT */
                    {
                        (void)soc_hash_bucket_get(unit, mem, db, move_entry, &dest_bucket);
                    }

                    if (SHR_BITGET(trace, (bank_inf.numb * nbix) + dest_bucket)) {
                        /* Try next entry in the same bucket */
                        i++;
                        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                                    (BSL_META_U(unit,
                                                "  Skip(1) bank %d bucket %d, go to bank %d index %d\n"),
                                     db, dest_bucket, cb, index+i));
                        continue;
                    }
                }
            }

            if (cmp_rv < 0) {
                if (_soc_mem_shared_hash_narrower_move_allowed(v0, v1, v2, v3,
                                                            &mem, &index, i)) {
                    if (SOC_FAILURE(soc_mem_read(unit, mem, copyno, index,
                                                 move_entry))) {
                        rv = SOC_E_MEMORY;
                        mem = ori_mem;
                        break;
                    }
                    LOG_VERBOSE(BSL_LS_SOC_COMMON,
                                (BSL_META_U(unit,
                                            "Depth %d, L-2nd read %d index %d from bank %d bucket %d\n"),
                                 recurse_depth, mem, index, cb, bucket));
                    /* Are we already in recursion or will we be in recursion */
                    if (recurse_depth) {
                        /* Calculate destination entry hash value. */
#ifdef BCM_APACHE_SUPPORT
                        if (SOC_IS_APACHE(unit)) {
                            (void)soc_ap_hash_bucket_get(unit, mem, db, move_entry,
                                                         &dest_bucket);
                        } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
                        if (SOC_IS_TOMAHAWKX(unit)) {
                            (void)soc_th_hash_bucket_get(unit, mem, db, move_entry,
                                                         &dest_bucket);
                        } else
#endif /* BCM_TOMAHAWK_SUPPORT */
                        {
                            (void)soc_hash_bucket_get(unit, mem, db, move_entry,
                                                      &dest_bucket);
                        }
                        if (SHR_BITGET(trace, (bank_inf.numb * nbix) + dest_bucket)) {
                            /* Try next entry in the same bucket in original
                             * memory view */
                            mem = ori_mem;
                            index = ori_index;
                            i++;
                            LOG_VERBOSE(BSL_LS_SOC_COMMON,
                                        (BSL_META_U(unit,
                                                    "  Skip(2) bank %d bucket %d, go to mem %d " \
                                                    "bank %d index %d \n"),
                                         db, dest_bucket, mem, i >= num_ent ? db : cb,
                                         i >= num_ent ? -1 : (index+i)));
                            continue;
                        }
                    }
                } else {
                    LOG_VERBOSE(BSL_LS_SOC_COMMON,
                                (BSL_META_U(unit,
                                            "Depth %d, Existing entry width is narrower, but can't move\n"),
                                 recurse_depth));
                    rv = SOC_E_NONE;
                    break;
                }
            }

            /* wider entry */
            if (cmp_rv > 0) {
                mem = wider_mem;
                index = (index + i) / cmp_rv;
                rv = soc_mem_read(unit, mem, copyno, index, move_entry);
                LOG_VERBOSE(BSL_LS_SOC_COMMON,
                            (BSL_META_U(unit,
                                        "Depth %d, G-2nd read %d index %d from bank %d bucket %d\n"),
                             recurse_depth, mem, index, cb, bucket));
                if (SOC_FAILURE(rv)) {
                    rv = SOC_E_MEMORY;
                    mem = ori_mem;
                    break;
                }
                /* Are we already in recursion or will we be in recursion */
                if (recurse_depth) {
                    /* Calculate destination entry hash value. */
#ifdef BCM_APACHE_SUPPORT
                    if (SOC_IS_APACHE(unit)) {
                        (void)soc_ap_hash_bucket_get(unit, mem, db, move_entry,
                                                     &dest_bucket);
                    } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
                    if (SOC_IS_TOMAHAWKX(unit)) {
                        (void)soc_th_hash_bucket_get(unit, mem, db, move_entry,
                                                     &dest_bucket);
                    } else
#endif /* BCM_TOMAHAWK_SUPPORT */
                    {
                        (void)soc_hash_bucket_get(unit, mem, db, move_entry,
                                                  &dest_bucket);
                    }
                    if (SHR_BITGET(trace, (bank_inf.numb * nbix) + dest_bucket)) {
                        /* Determine bucket offset increment based upon
                         * existing entry type/size */
                        mem = ori_mem;
                        index = ori_index;
                        i += _soc_mem_shared_hash_incr_per_entry(unit, mem,
                                                         move_entry, entry);
                        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                                    (BSL_META_U(unit,
                                                "  Skip(3) bank %d bucket %d, go to bank %d index %d\n"),
                                     db, dest_bucket, cb, index+i));
                        continue;
                    }
                }
            }

            /* Attempt to insert it into the other bank. */
            rv = soc_mem_bank_insert(unit, mem, (uint32)1<<db, copyno, move_entry, NULL);
            LOG_VERBOSE(BSL_LS_SOC_COMMON,
                        (BSL_META_U(unit,
                                    "Depth %d, trying to insert %d index %d to bank %d\n"),
                         recurse_depth, mem, index, db));
            if (SOC_FAILURE(rv)) {
                if (rv != SOC_E_FULL) {
                    LOG_VERBOSE(BSL_LS_SOC_COMMON,
                                (BSL_META_U(unit,
                                            "  But failed, rv %s, ori_mem %d\n"), soc_errmsg(rv), ori_mem));
                    mem = ori_mem;
                    break;
                }
                LOG_VERBOSE(BSL_LS_SOC_COMMON,
                            (BSL_META_U(unit,
                                        "  But bank is full, insert failed, will do recurse\n")));
                /* Recursive call - attempt to create a slot
                   in another bank's bucket. */
                rv = _soc_mem_shared_hash_move(unit, mem, cb, copyno,
                                               move_entry, trace,
                                               recurse_depth - 1,&bank_inf);
                if (SOC_FAILURE(rv)) {
                    if (rv != SOC_E_FULL) {
                        break;
                    }
                    mem = ori_mem;
                    index = ori_index;
                    i += _soc_mem_shared_hash_incr_per_entry(unit, mem,
                                                            move_entry, entry);
                    continue;
                }
            }
            LOG_VERBOSE(BSL_LS_SOC_COMMON,
                        (BSL_META_U(unit,
                                    "  Insert successfully\n")));
            /* Entry was moved successfully. */
            found = TRUE;
            /* Delete old entry from original location */
            LOG_VERBOSE(BSL_LS_SOC_COMMON,
                        (BSL_META_U(unit,
                                    "Depth %d, trying to delete %d index %d from bank %d\n"),
                         recurse_depth, mem, index, cb));
            rv = soc_mem_generic_delete(unit, mem, MEM_BLOCK_ANY, (uint32)1<<cb,
                                        move_entry, NULL, NULL);
            mem = ori_mem;
            break;
        }  /* Bucket iteration loop. */
        if (found || ((rv < 0) && (rv != SOC_E_FULL))) {
            break;
        }
    } /* Bank iteration loop. */
    if ((rv < 0) && (rv != SOC_E_FULL)) {
        if (NULL == bucket_trace) {
            sal_free(trace);
        }
        return rv;
    }
    if (!found) {
        if (NULL == bucket_trace) {
            sal_free(trace);
        }
        return SOC_E_FULL;
    }
    rv = soc_mem_generic_insert(unit, mem, copyno, (uint32)1<<cb, entry, entry, NULL);
    if (rv) {
        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                                "Insert entry: %d\n"), rv));
    }
clean_up:
    if (NULL == bucket_trace) {
        sal_free(trace);
    }
    return rv;
}


STATIC INLINE void
_soc_mem_shared_hash_l3_bucket_read(int unit, int ent, int key_type, int *width,
                                    soc_mem_t *mem, void *bkt_entry, void *entry)
{

    soc_mem_info_t meminfo;
    soc_field_info_t fieldinfo;
    /* Exclude HIT bit */
    int entry_bits;

    switch (key_type) {
    case TD2_L3_HASH_KEY_TYPE_V4UC:
    case TD2_L3_HASH_KEY_TYPE_TRILL:
    case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN:
    case TD2_L3_HASH_KEY_TYPE_FCOE_HOST:
    case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP:
        *width = 1;
        *mem = L3_ENTRY_IPV4_UNICASTm;
        break;
    case TD2_L3_HASH_KEY_TYPE_V4UC_EXT:
    case TD2_L3_HASH_KEY_TYPE_V4MC:
    case TD2_L3_HASH_KEY_TYPE_V4L2MC:
    case TD2_L3_HASH_KEY_TYPE_V4L2VPMC:
    case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN_EXT:
    case TD2_L3_HASH_KEY_TYPE_FCOE_HOST_EXT:
    case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP_EXT:
    case TD2_L3_HASH_KEY_TYPE_DST_NAT:
    case TD2_L3_HASH_KEY_TYPE_DST_NAPT:
    case TD2_L3_HASH_KEY_TYPE_V6UC:
        *width = 2;
        *mem = L3_ENTRY_IPV4_MULTICASTm;
        break;
    case TD2_L3_HASH_KEY_TYPE_V6UC_EXT:
    case TD2_L3_HASH_KEY_TYPE_V6MC:
    case TD2_L3_HASH_KEY_TYPE_V6L2MC:
    case TD2_L3_HASH_KEY_TYPE_V6L2VPMC:
    default:
        *width = 4;
        *mem = L3_ENTRY_IPV6_MULTICASTm;
        sal_memcpy(entry, bkt_entry, sizeof(l3_entry_ipv6_multicast_entry_t));
        return;
    }
    entry_bits = (soc_mem_entry_bits(unit, L3_ENTRY_IPV4_UNICASTm) - 1) * (*width);

    meminfo.flags = 0;
    meminfo.bytes = SOC_MEM_WORDS(unit, L3_ENTRY_IPV6_MULTICASTm) * 4;
    fieldinfo.flags = SOCF_LE;
    fieldinfo.bp =  entry_bits * (ent / *width);
    fieldinfo.len = entry_bits;

    (void)soc_meminfo_fieldinfo_field_get(bkt_entry, &meminfo,
                                          &fieldinfo, entry);

}

#define SRAM_ENTRIES_PER_BUCKET 2
#define MEM_IS_EXACT_MATCH(mem) (EXACT_MATCH_4m == mem || \
                             EXACT_MATCH_4_PIPE0m == mem || \
                             EXACT_MATCH_4_PIPE1m == mem || \
                             EXACT_MATCH_4_PIPE2m == mem || \
                             EXACT_MATCH_4_PIPE3m == mem || \
                             EXACT_MATCH_2m == mem || \
                             EXACT_MATCH_2_PIPE0m == mem || \
                             EXACT_MATCH_2_PIPE1m == mem || \
                             EXACT_MATCH_2_PIPE2m == mem || \
                             EXACT_MATCH_2_PIPE3m == mem)

#if defined(BCM_TOMAHAWK_SUPPORT)
STATIC INLINE void
_soc_mem_shared_hash_exact_match_bucket_read(int unit, int ent, int key_type, int *width,
                                    soc_mem_t *mem, void *bkt_entry, void *entry, int pipenum)
{
    soc_mem_info_t meminfo;
    soc_field_info_t fieldinfo;
    /* Exclude HIT bit */
    int entry_bits;

    switch (key_type) {
    case TH_FPEM_HASH_KEY_TYPE_128B:
    case TH_FPEM_HASH_KEY_TYPE_160B:
        *width = 1;
        switch (pipenum) {
        case 0:
            *mem = EXACT_MATCH_2_PIPE0m;
            break;
        case 1:
            *mem = EXACT_MATCH_2_PIPE1m;
            break;
        case 2:
            *mem = EXACT_MATCH_2_PIPE2m;
            break;
        case 3:
            *mem = EXACT_MATCH_2_PIPE3m;
            break;
        default:
            *mem = EXACT_MATCH_2m;
            break;            
        }
        break;
    case TH_FPEM_HASH_KEY_TYPE_320B:
    default:
        switch (pipenum) {
        case 0:
            *mem = EXACT_MATCH_4_PIPE0m;
            break;
        case 1:
            *mem = EXACT_MATCH_4_PIPE1m;
            break;
        case 2:
            *mem = EXACT_MATCH_4_PIPE2m;
            break;
        case 3:
            *mem = EXACT_MATCH_4_PIPE3m;
            break;
        default:
            *mem = EXACT_MATCH_4m;
            break;            
        }        
        *width = 2;
    }
    entry_bits = (soc_mem_entry_bits(unit, EXACT_MATCH_2m) - 1) * (*width);

    meminfo.flags = 0;
    meminfo.bytes = SOC_MEM_WORDS(unit, EXACT_MATCH_4m) * 4;
    fieldinfo.flags = SOCF_LE;
    fieldinfo.bp =  entry_bits * ((ent % SRAM_ENTRIES_PER_BUCKET) / *width);
    fieldinfo.len = entry_bits;

    (void)soc_meminfo_fieldinfo_field_get((uint32 *)bkt_entry, &meminfo,
                                          &fieldinfo, entry);
}
#endif


#define _SOC_MEM_SHARED_HASH_STATUS_CHECK(free, mem, status)\
    switch(mem) {                                           \
    case L3_ENTRY_IPV6_MULTICASTm:                          \
    case EXACT_MATCH_4m:                                    \
        free = (status == 0x0);                             \
        break;                                              \
    case L3_ENTRY_IPV6_UNICASTm:                            \
    case L3_ENTRY_IPV4_MULTICASTm:                          \
    case EXACT_MATCH_2m:                                    \
        free = (status & 0x3) == 0 || (status >> 2) == 0;   \
        break;                                              \
    case L2Xm:                                              \
    case L3_ENTRY_ONLYm:                                    \
    case L3_ENTRY_IPV4_UNICASTm:                            \
    default:                                                \
        free = (status != 0xf);                             \
        break;                                              \
    }


STATIC INLINE int
_soc_mem_shared_hash_bucket(int unit, int mem, int bank, uint32 *entry)
{
    int index;
    uint32 bucket;

#ifdef BCM_APACHE_SUPPORT
    if (SOC_IS_APACHE(unit)) {
        (void)soc_ap_hash_bucket_get(unit, mem, bank, entry, &bucket);
    } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
    if (SOC_IS_TOMAHAWKX(unit)) {
        (void)soc_th_hash_bucket_get(unit, mem, bank, entry, &bucket);
    } else
#endif /* BCM_TOMAHAWK_SUPPORT */
    {
        (void)soc_hash_bucket_get(unit, mem, bank, entry, &bucket);
    }

#ifdef BCM_APACHE_SUPPORT
    if (SOC_IS_APACHE(unit)) {
        index = soc_ap_hash_index_get(unit, mem, bank, bucket);
    } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
    if (SOC_IS_TOMAHAWKX(unit)) {
        index = soc_th_hash_index_get(unit, mem, bank, bucket);
    } else
#endif /* BCM_TOMAHAWK_SUPPORT */
    {
        index = soc_hash_index_get(unit, mem, bank, bucket);
    }

    return (index / _soc_mem_shared_hash_entries_per_bkt(mem));
}



#define INVALID_BUCKET      -1

/*
 * this breadth algorithm works as backup as it uses fixed memory which
 * will be less than the default one when depth exceeds a threshold
 */

STATIC int
_soc_mem_shared_hash_breadth_l2_move2(int unit, soc_mem_t mem, int copyno,
                                     void *entry, int num_levels)
{
    _soc_breadth_bucket2_t *buckets = NULL;
    _soc_breadth_bucket2_t *cb, *cbi, *pbi, *pb, *sb;
    SHR_BITDCL *bkt_trace = NULL;
    _soc_hash_bank_info_t bi = {0};
    int rv;
    int job_done = 0;
    int bank_num, ibank_num = 0;
    int bix, cbix, num_banks;
    int free = 0;
    int ent, sent, bucket;
    int curr_bucket;
    void *se, *me;

    if (num_levels <= 0) {
        return SOC_E_FULL;
    }

    SOC_IF_ERROR_RETURN(_soc_mem_shared_hash_init
        (unit, mem, &bkt_trace, &bi, 0, NULL, &buckets));

    /* the last bucket, which is also the head */
    curr_bucket = bi.numb;
    pb = &buckets[curr_bucket];
    pb->bucket      = curr_bucket;
    pb->src_bix     = -1;
    pb->level       = -1;
    pb->n_bucket    = -1;

    do {
        /* This bucket's entries have been tried to move */
        cb = &buckets[curr_bucket];

        /* all levels have been tried */
        if (cb->level == num_levels - 1) {
            goto done;
        }

        /* now generate the sub buckets under this bucket
         * and try to move the sub buckets' entries
         */

        if (curr_bucket == bi.numb) {
            num_banks = bi.num_banks;
        } else {
            num_banks = bi.num_banks - 1;
        }

        for (ent = 0; ent < NUM_ENT; ent++) {
            if (curr_bucket == bi.numb) {
                se = entry;
            } else {
                se = &cb->e.l2[ent];
            }
            for (bix = 0; bix < num_banks; bix++) {
                cbix = cb->src_bix + bix + 1;
                cbix = cbix % bi.num_banks;

                bank_num = bi.bank_nums[cbix];
                bucket = _soc_mem_shared_hash_bucket(unit, mem,
                                                     bank_num, se);

                /* do not visit a bucket twice */
                if (SHR_BITGET(bkt_trace, bucket)) {
                    continue;
                }

                sb = &buckets[bucket];
                SHR_BITSET(bkt_trace, bucket);
                sb->level     = cb->level + 1;
                sb->src_ent   = ent;
                sb->bucket    = bucket;
                sb->s_bucket  = curr_bucket;
                sb->src_bix   = cbix;

                /* connect */
                pb->n_bucket  = bucket;
                sb->n_bucket  = INVALID_BUCKET;

                /* loop sub bucket's entries and try move */
                for (sent = 0; sent < NUM_ENT; sent++) {
                    me = &sb->e.l2[sent];
                    rv = soc_mem_read(unit, mem, copyno, (bucket << 2) + sent, me);
                    if (SOC_FAILURE(rv)) {
                        goto cleanup;
                    }

                    rv = soc_mem_generic_insert(unit, mem, copyno,
                                                bi.banks & ~(1 << bank_num),
                                                me, NULL, NULL);

                    free = 0;
                    if (SOC_SUCCESS(rv)) {

                        rv = soc_mem_generic_delete(unit, mem, MEM_BLOCK_ANY,
                                                    1 << bank_num, me, NULL, NULL);
                        if (SOC_FAILURE(rv)) {
                            goto cleanup;
                        }
                        free = 1;

                    }

                    /* sb is not freed, try move next entry in sb */
                    if (!free) {
                        continue;
                    }

                    /* now sb is freed.
                     * cb's entry can be moved now. move it and
                     * more if possible
                     */
                    ibank_num = bank_num;
                    pbi = sb;
                    while (pbi->s_bucket != bi.numb) {
                        cbi = pbi;
                        pbi = &buckets[pbi->s_bucket];
                        me = &pbi->e.l2[cbi->src_ent];
                        rv = soc_mem_generic_insert(unit, mem, copyno,
                                                    (1 << ibank_num),
                                                    me, NULL, NULL);
                        if (SOC_FAILURE(rv)) {
                            goto cleanup;
                        }

                        ibank_num = bi.bank_nums[pbi->src_bix];

                        rv = soc_mem_generic_delete(unit, mem,
                                                    MEM_BLOCK_ANY,
                                                    (1 << ibank_num),
                                                    me, NULL, NULL);
                        if (SOC_FAILURE(rv)) {
                            goto cleanup;
                        }
                    }

                    job_done = 1;
                    goto done;
                } /* sub bucket loop and move done. */

                pb = sb;
            }
        }
        curr_bucket = cb->n_bucket;
    } while (curr_bucket != INVALID_BUCKET);

done:
    if (job_done) {
        rv = soc_mem_generic_insert(unit, mem, copyno, 1 << ibank_num,
                                    entry, entry, NULL);
    } else {
        rv = SOC_E_FULL;
    }

cleanup:
    if (bkt_trace) {
        sal_free(bkt_trace);
    }

    if (buckets) {
        sal_free(buckets);
    }

    if (SOC_FAILURE(rv)) {
        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                                "Insert entry: %d\n"), rv));
    }

    return rv;
}

STATIC int
_soc_mem_shared_hash_breadth_mix_move2(int unit, soc_mem_t mem, int copyno,
                                      void *entry, int num_levels)
{
    _soc_breadth_bucket2_t *buckets = NULL;
    _soc_breadth_bucket2_t *cb, *nb, *cbi = NULL, *pbi, *pb, *sb, *ib;
    SHR_BITDCL *bkt_trace = NULL;
    _soc_hash_bank_info_t bi = {0};
    uint32 bkt_entry[2][SOC_MAX_MEM_WORDS];
    uint32 v[NUM_ENT], k[NUM_ENT];
    int rv;
    int job_done = 0;
    int bank_num, ibank_num = 0;
    int bix, cbix, num_banks;
    int move_bkt;
    int free = 0;
    int ent, ent_incr, sent, sent_incr, bucket;
    uint8 skip = 0, src_width;
    soc_mem_t orig_mem, base_mem, src_mem;
    int curr_bucket;
    void *se, *me;
#if defined(BCM_TOMAHAWK_SUPPORT)
    int pipenum = -1;
#endif

    if (num_levels <= 0) {
        return SOC_E_FULL;
    }

    base_mem = MEM_IS_EXACT_MATCH(mem) ? EXACT_MATCH_4m : L3_ENTRY_IPV6_MULTICASTm;
#if defined(BCM_TOMAHAWK_SUPPORT)
    if (base_mem == EXACT_MATCH_4m) {
        switch (mem) {
        case EXACT_MATCH_4_PIPE0m:
        case EXACT_MATCH_2_PIPE0m:
            base_mem = EXACT_MATCH_4_PIPE0m;
            pipenum = 0;
            break;
        case EXACT_MATCH_4_PIPE1m:
        case EXACT_MATCH_2_PIPE1m:
            base_mem = EXACT_MATCH_4_PIPE1m;
            pipenum = 1;
            break;
        case EXACT_MATCH_4_PIPE2m:
        case EXACT_MATCH_2_PIPE2m:
            base_mem = EXACT_MATCH_4_PIPE2m;
            pipenum = 2;
            break;
        case EXACT_MATCH_4_PIPE3m:
        case EXACT_MATCH_2_PIPE3m:
            base_mem = EXACT_MATCH_4_PIPE3m;
            pipenum = 3;
            break;
        default:
            break;
        }
    }  
#endif
    SOC_IF_ERROR_RETURN(_soc_mem_shared_hash_init
        (unit, base_mem, &bkt_trace, &bi, 0, NULL, &buckets));

    /* the last bucket, which is also the head */
    curr_bucket = bi.numb;
    pb = &buckets[curr_bucket];
    pb->bucket      = curr_bucket;
    pb->status      = 0x1;
    pb->src_bix     = -1;
    pb->level       = -1;
    pb->n_bucket    = -1;
    pb->mem[0]      = mem;

    do {
        /* This bucket's entries have been tried to move */
        cb = &buckets[curr_bucket];

        /* all levels have been tried */
        if (cb->level == num_levels - 1) {
            goto done;
        }

        /* now generate the sub buckets under this bucket
         * and try to move the sub buckets' entries
         */

        if (curr_bucket == bi.numb) {
            num_banks = bi.num_banks;
        } else {
            num_banks = bi.num_banks - 1;
        }

        for (ent = 0; ent < NUM_ENT; ent += ent_incr) {
            if (!(cb->status & (1 << ent))) {
                ent_incr = 1;
                continue;
            }

            if (curr_bucket == bi.numb) {
                se        = entry;
                ent_incr  = NUM_ENT;
            } else if (MEM_IS_EXACT_MATCH(mem)) {
                se        = &cb->e.em[ent];
                ent_incr  = cb->width[ent];
            } else {
                se        = &cb->e.l3[ent];
                ent_incr  = cb->width[ent];
            }

            src_mem = cb->mem[ent];
            skip = 0;
            for (bix = 0; bix < num_banks; bix++) {
                cbix = cb->src_bix + bix + 1;
                cbix = cbix % bi.num_banks;

                bank_num = bi.bank_nums[cbix];
                bucket = _soc_mem_shared_hash_bucket(unit, src_mem, bank_num, se);

                /* do not visit a bucket twice */
                if (SHR_BITGET(bkt_trace, bucket)) {
                    continue;
                }

                sb = &buckets[bucket];
                SHR_BITSET(bkt_trace, bucket);
                sb->level     = cb->level + 1;
                sb->src_ent   = ent;
                sb->bucket    = bucket;
                sb->s_bucket  = curr_bucket;
                sb->src_bix   = cbix;

                /* connect */
                pb->n_bucket  = bucket;
                sb->p_bucket  = pb->bucket;
                sb->n_bucket  = INVALID_BUCKET;
                if (MEM_IS_EXACT_MATCH(mem)) {
                    rv = soc_mem_read(unit, base_mem, copyno, 
                                      bucket * SRAM_ENTRIES_PER_BUCKET, bkt_entry[0]);
                    if (SOC_FAILURE(rv)) {
                        goto cleanup;
                    }
                    rv = soc_mem_read(unit, base_mem, copyno, 
                                      bucket * SRAM_ENTRIES_PER_BUCKET + 1, 
                                      bkt_entry[1]);
                    if (SOC_FAILURE(rv)) {
                        goto cleanup;
                    }            
                    soc_mem_field_get(unit, base_mem, bkt_entry[1], VALID_2f, &v[3]);
                    soc_mem_field_get(unit, base_mem, bkt_entry[1], VALID_0f, &v[2]);
                    soc_mem_field_get(unit, base_mem, bkt_entry[0], VALID_2f, &v[1]);
                    soc_mem_field_get(unit, base_mem, bkt_entry[0], VALID_0f, &v[0]);
                    soc_mem_field_get(unit, base_mem, bkt_entry[1], KEY_TYPE_2f, &k[3]);
                    soc_mem_field_get(unit, base_mem, bkt_entry[1], KEY_TYPE_0f, &k[2]);
                    soc_mem_field_get(unit, base_mem, bkt_entry[0], KEY_TYPE_2f, &k[1]);
                    soc_mem_field_get(unit, base_mem, bkt_entry[0], KEY_TYPE_0f, &k[0]);

                } else {
                    /* loop sub bucket's entries and try move */
                    rv = soc_mem_read(unit, base_mem, copyno, bucket, bkt_entry[0]);
                    if (SOC_FAILURE(rv)) {
                        goto cleanup;
                    }

                    soc_mem_field_get(unit, base_mem, bkt_entry[0], VALID_3f, &v[3]);
                    soc_mem_field_get(unit, base_mem, bkt_entry[0], VALID_2f, &v[2]);
                    soc_mem_field_get(unit, base_mem, bkt_entry[0], VALID_1f, &v[1]);
                    soc_mem_field_get(unit, base_mem, bkt_entry[0], VALID_0f, &v[0]);
                    soc_mem_field_get(unit, base_mem, bkt_entry[0], KEY_TYPE_3f, &k[3]);
                    soc_mem_field_get(unit, base_mem, bkt_entry[0], KEY_TYPE_2f, &k[2]);
                    soc_mem_field_get(unit, base_mem, bkt_entry[0], KEY_TYPE_1f, &k[1]);
                    soc_mem_field_get(unit, base_mem, bkt_entry[0], KEY_TYPE_0f, &k[0]);                
                }
                sb->status = v[3] << 3 | v[2] << 2 | v[1] << 1 | v[0];

                /* loop sub bucket's entries and try insert */
                for (sent = 0; sent < NUM_ENT; sent += sent_incr) {
                    if (!v[sent]) {
                        sent_incr = 1;
                        continue;
                    }
#if defined(BCM_TOMAHAWK_SUPPORT)
                    if (MEM_IS_EXACT_MATCH(mem)) {
                        me = &sb->e.em[sent];
                        _soc_mem_shared_hash_exact_match_bucket_read(unit, sent, k[sent],
                                &sent_incr, &orig_mem, bkt_entry[sent / SRAM_ENTRIES_PER_BUCKET], me, pipenum);                        
                    } else 
#endif                    
                    {
                        me = &sb->e.l3[sent];
                        _soc_mem_shared_hash_l3_bucket_read(unit, sent, k[sent],
                                &sent_incr, &orig_mem, bkt_entry[0], me);
                    }
                    sb->width[sent] = sent_incr;
                    sb->mem[sent] = orig_mem;

                    rv = soc_mem_generic_insert(unit, orig_mem, copyno,
                                                bi.banks & ~(1 << bank_num),
                                                me, NULL, &move_bkt);

                    free = 0;
                    if (SOC_SUCCESS(rv)) {

                        /* modifying a visited a bucket may
                         * corrupt the structure (entry, status ...)
                         */
                        move_bkt /= _soc_mem_shared_hash_entries_per_bkt(orig_mem);
                        if (SHR_BITGET(bkt_trace, move_bkt)) {
                            rv = soc_mem_generic_delete(unit, orig_mem, MEM_BLOCK_ANY,
                                                        bi.banks & ~(1 << bank_num),
                                                        me, NULL, NULL);

                            if (SOC_FAILURE(rv)) {
                                goto cleanup;
                            }
                        } else {
                            rv = soc_mem_generic_delete(unit, orig_mem, MEM_BLOCK_ANY,
                                                        1 << bank_num, me, NULL, NULL);
                            if (SOC_FAILURE(rv)) {
                                goto cleanup;
                            }
                            sb->status &= ~(((1 << sent_incr) - 1) << sent);

                            _SOC_MEM_SHARED_HASH_STATUS_CHECK(free, src_mem, sb->status);
                        }
                    }
                    /* sb is not freed, try move next entry in sb */
                    if (!free) {
                        continue;
                    }

                    /* now sb is freed.
                     * cb's entry can be moved now. move it and
                     * more if possible
                     */
                    ibank_num = bank_num;
                    pbi = sb;

                    while (free && pbi->s_bucket != bi.numb) {
                        cbi = pbi;
                        pbi = &buckets[pbi->s_bucket];
                        if (MEM_IS_EXACT_MATCH(mem)) {
                            me = &pbi->e.em[cbi->src_ent];
                        } else {
                            me = &pbi->e.l3[cbi->src_ent];
                        }
                        orig_mem = pbi->mem[cbi->src_ent];
                        rv = soc_mem_generic_insert(unit, orig_mem, copyno,
                                                    (1 << ibank_num),
                                                    me, NULL, NULL);
                        if (SOC_FAILURE(rv)) {
                            goto cleanup;
                        }

                        ibank_num = bi.bank_nums[pbi->src_bix];

                        rv = soc_mem_generic_delete(unit, orig_mem,
                                                    MEM_BLOCK_ANY,
                                                    (1 << ibank_num),
                                                    me, NULL, NULL);
                        if (SOC_FAILURE(rv)) {
                            goto cleanup;
                        }

                        src_width = (L3_ENTRY_IPV6_MULTICASTm == orig_mem) ? 4 :
                                    (L3_ENTRY_IPV4_MULTICASTm == orig_mem ||
                                     L3_ENTRY_IPV6_UNICASTm   == orig_mem || 
                                     EXACT_MATCH_4m == mem ||
                                     EXACT_MATCH_4_PIPE0m == mem || 
                                     EXACT_MATCH_4_PIPE1m == mem ||
                                     EXACT_MATCH_4_PIPE2m == mem || 
                                     EXACT_MATCH_4_PIPE3m == mem) ? 2 : 1;
                        pbi->status &= ~(((1 << src_width) - 1) << cbi->src_ent);

                        _SOC_MEM_SHARED_HASH_STATUS_CHECK(free,
                            buckets[pbi->s_bucket].mem[pbi->src_ent], pbi->status);
                    }

                    if (free) {
                        job_done = 1;
                        goto done;
                    } else {
                        /* re-connect, because some of sbs which are linked
                         * just now, may need to be removed from the link.
                         */

                        /* taking care of the sub buckets at next level */
                        while (pb->level == sb->level) {
                            ib = pb;
                            while (ib->level > cbi->level) {
                                ib = &buckets[ib->s_bucket];
                            }
                            if (ib->s_bucket != cbi->s_bucket ||
                                ib->src_ent != cbi->src_ent) {
                                break;
                            }
                            /* this could be further optimized to clear
                             * those skipped sbs and free the bucket.
                             */
                            pb = &buckets[pb->p_bucket];
                        }

                        /* taking care of the current buckets at this level */
                        if (pbi == cb) {
                            /* moved 1 entry. entry of cb is moved, but cb is
                             * still not freed. continue next entry's sub bucket
                             */
                            skip = 1;
                            /* assert (cb->level == cbi->level - 1); */
                        } else {

                            /* moved at least 2 entries, now it's time
                             * to advance current bucket.
                             */

                            /* assert (cb->level >= cbi->level); */
                            skip = 2;

                            nb = &buckets[cb->n_bucket];
                            while (nb->level != sb->level) {
                                ib = nb;
                                while (ib->level > cbi->level) {
                                    ib = &buckets[ib->s_bucket];
                                }
                                if (ib->s_bucket != cbi->s_bucket ||
                                    ib->src_ent != cbi->src_ent) {
                                    break;
                                }
                                nb = &buckets[nb->n_bucket];
                            }

                            cb = &buckets[nb->p_bucket];
                        }
                        break;
                    }
                } /* sub bucket loop and move done. */
                if (skip) {
                    break;
                }
                pb = sb;
            }
            if (skip == 2) {
                break;
            }
        }
        curr_bucket = cb->n_bucket;
    } while (curr_bucket != INVALID_BUCKET);

done:
    if (job_done) {
        rv = soc_mem_generic_insert(unit, mem, copyno, 1 << ibank_num,
                                    entry, entry, NULL);
    } else {
        rv = SOC_E_FULL;
    }

cleanup:
    if (bkt_trace) {
        sal_free(bkt_trace);
    }

    if (buckets) {
        sal_free(buckets);
    }

    if (SOC_FAILURE(rv)) {
        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                                "Insert entry: %d\n"), rv));
    }

    return rv;
}

/*
 * this breadth algorithm is the default one which is faster and
 * consume less memory.
 */
#define MAX_MIX_LEVELS  4
#define MAX_L2_LEVELS  6

STATIC int
_soc_mem_shared_hash_breadth_l2_move(int unit, soc_mem_t mem, int copyno,
                                     void *entry, int num_levels)
{
    SHR_BITDCL *bkt_trace = NULL;
    _soc_breadth_bucket_t *buckets[MAX_L2_LEVELS + 1] = {0};
    _soc_breadth_bucket_t *cb, *tb, *cbi = NULL, *pbi;
    uint32 *se;
    int rv;
    int level, job_done = 0;
    int bank_num, ibank_num = 0;
    int bix, cbix;
    int bkt_idx;
    int nl_bkt_idx, num_cl_bkts;
    int ent, bucket;
    int i;
    _soc_hash_bank_info_t bi = {0};

    if (num_levels <= 0) {
        return SOC_E_FULL;
    } else if (num_levels > MAX_L2_LEVELS) {
        return _soc_mem_shared_hash_breadth_l2_move2(unit, mem, copyno,
                                                     entry, num_levels);
    }

    SOC_IF_ERROR_RETURN(_soc_mem_shared_hash_init
        (unit, mem, &bkt_trace, &bi, num_levels, &buckets[0], NULL));

    num_cl_bkts = bi.num_banks;

    for (level = 0; level < num_levels; level++) {
        if (level + 1 < num_levels) {
            buckets[level + 1] = buckets[level] + num_cl_bkts;
        } else {
            buckets[level + 1] = 0;
        }
        for (bkt_idx = 0; bkt_idx < num_cl_bkts; bkt_idx++) {
            cb = &buckets[level][bkt_idx];
            if (level == 0) {
                cb->src_bix = bkt_idx;
                cb->src_mem  = mem;
                se = entry;
            } else {
                se = (uint32 *)&buckets[level - 1][cb->src_bkt_idx].e.l2[cb->src_ent];
            }

            if (cb->src_mem == 0) {
                continue;
            }

            bank_num = bi.bank_nums[cb->src_bix];
            bucket = _soc_mem_shared_hash_bucket(unit, cb->src_mem, bank_num, se);

            if (SHR_BITGET(bkt_trace, bucket)) {
                continue;
            }
            SHR_BITSET(bkt_trace, bucket);
            cb->bucket = bucket;

            for (ent = 0; ent < NUM_ENT; ent++) {
                /* Read current entry */
                rv = soc_mem_read(unit, mem, copyno, (bucket << 2) + ent, &cb->e.l2[ent]);
                if (SOC_FAILURE(rv)) {
                    goto cleanup;
                }

                /* Try insert */
                rv = soc_mem_generic_insert(unit, mem, copyno,
                                            bi.banks & ~(1 << bank_num),
                                            &cb->e.l2[ent], NULL, NULL);

                if (SOC_SUCCESS(rv)) {
                    rv = soc_mem_generic_delete(unit, mem, MEM_BLOCK_ANY,
                                                (uint32)1<<bank_num,
                                                &cb->e.l2[ent], NULL, NULL);
                    if (SOC_FAILURE(rv)) {
                        goto cleanup;
                    }
                    break;
                }

                /* Construct buckets in next level under this entry */
                nl_bkt_idx = bkt_idx * (bi.num_banks - 1) * NUM_ENT +
                                 ent * (bi.num_banks - 1);

                for (bix = 0; bix < bi.num_banks && buckets[level + 1]; bix++) {
                    cbix = cb->src_bix + bix + 1;
                    cbix = cbix % bi.num_banks;
                    if (bank_num == bi.bank_nums[cbix]) {
                        continue;
                    }

                    tb = &buckets[level + 1][nl_bkt_idx++];
                    tb->src_bkt_idx   = bkt_idx;
                    tb->src_bix  = cbix;
                    tb->src_mem  = mem;
                    tb->src_ent  = ent;
                }
            }

            if (ent < NUM_ENT) {
                ibank_num = bank_num;
                pbi = cb;
                for (i = level; i > 0; i--) {
                    cbi = pbi;
                    pbi = &buckets[i - 1][cbi->src_bkt_idx];

                    rv = soc_mem_bank_insert(unit, cbi->src_mem,
                                             (1 << ibank_num), copyno,
                                             &pbi->e.l2[cbi->src_ent], NULL);
                    if (SOC_FAILURE(rv)) {
                        goto cleanup;
                    }

                    ibank_num = bi.bank_nums[pbi->src_bix];

                    rv = soc_mem_generic_delete(unit, cbi->src_mem, MEM_BLOCK_ANY,
                                                (1 << ibank_num),
                                                &pbi->e.l2[cbi->src_ent], NULL, NULL);
                    if (SOC_FAILURE(rv)) {
                        goto cleanup;
                    }
                }
                job_done = 1;
                goto done;
            }
        }
        num_cl_bkts *= (bi.num_banks - 1) * NUM_ENT;
    }

done:
    if (job_done) {
        rv = soc_mem_generic_insert(unit, mem, copyno, 1 << ibank_num,
                                    entry, entry, NULL);
    } else {
        rv = SOC_E_FULL;
    }

cleanup:
    if (bkt_trace) {
        sal_free(bkt_trace);
    }

    if (buckets[0]) {
        sal_free(buckets[0]);
    }

    if (SOC_FAILURE(rv)) {
        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                                "Insert entry: %d\n"), rv));
    }

    return rv;
}

STATIC int
_soc_mem_shared_hash_breadth_mix_move(int unit, soc_mem_t mem, int copyno,
                                     void *entry, int num_levels)
{
    SHR_BITDCL *bkt_trace = NULL;
    _soc_breadth_bucket_t *buckets[MAX_MIX_LEVELS + 1] = {0};
    _soc_breadth_bucket_t *cb, *tb, *cbi = NULL, *pbi;
    uint32 bkt_entry[2][SOC_MAX_MEM_WORDS];
    uint32 v[NUM_ENT], k[NUM_ENT];
    void *se, *me;
    int rv;
    int level, job_done = 0;
    int move_bkt;
    int bank_num, ibank_num = 0;
    int bix, cbix;
    int free = 0, moved = 0;
    int bkt_idx, bkt_idx1 = 0, bkt_idx2 = 0;
    int nl_bkt_idx, num_cl_bkts;
    int ent, ent_incr, bucket;
    int num = 0;
    int i, j;
    _soc_hash_bank_info_t bi = {0};
    soc_mem_t src_mem, base_mem;
#if defined(BCM_TOMAHAWK_SUPPORT)
    int pipenum = -1;
#endif
    if (num_levels <= 0) {
        return SOC_E_FULL;
    } else if (num_levels > MAX_MIX_LEVELS) {
        return _soc_mem_shared_hash_breadth_mix_move2(unit, mem, copyno,
                                                     entry, num_levels);
    }

    base_mem = MEM_IS_EXACT_MATCH(mem) ? EXACT_MATCH_4m : L3_ENTRY_IPV6_MULTICASTm;
#if defined(BCM_TOMAHAWK_SUPPORT)
    if (base_mem == EXACT_MATCH_4m) {
        switch (mem) {
        case EXACT_MATCH_4_PIPE0m:
        case EXACT_MATCH_2_PIPE0m:
            base_mem = EXACT_MATCH_4_PIPE0m;
            pipenum = 0;
            break;
        case EXACT_MATCH_4_PIPE1m:
        case EXACT_MATCH_2_PIPE1m:
            base_mem = EXACT_MATCH_4_PIPE1m;
            pipenum = 1;
            break;
        case EXACT_MATCH_4_PIPE2m:
        case EXACT_MATCH_2_PIPE2m:
            base_mem = EXACT_MATCH_4_PIPE2m;
            pipenum = 2;
            break;
        case EXACT_MATCH_4_PIPE3m:
        case EXACT_MATCH_2_PIPE3m:
            base_mem = EXACT_MATCH_4_PIPE3m;
            pipenum = 3;
            break;
        default:
            break;
        }
    }
#endif
    SOC_IF_ERROR_RETURN(_soc_mem_shared_hash_init
        (unit, base_mem, &bkt_trace, &bi, num_levels, &buckets[0], NULL));

    num_cl_bkts = bi.num_banks;

    for (level = 0; level < num_levels; level++) {
        if (level + 1 < num_levels) {
            buckets[level + 1] = buckets[level] + num_cl_bkts;
        } else {
            buckets[level + 1] = 0;
        }

        for (bkt_idx = 0; bkt_idx < num_cl_bkts; bkt_idx++) {
            cb = &buckets[level][bkt_idx];
            if (level == 0) {
                cb->src_bix = bkt_idx;
                cb->src_mem  = mem;
                cb->src_width = (L3_ENTRY_IPV6_MULTICASTm == mem) ? 4 :
                                (L3_ENTRY_IPV4_MULTICASTm == mem ||
                                 L3_ENTRY_IPV6_UNICASTm   == mem ||
                                 EXACT_MATCH_4m == mem ||
                                 EXACT_MATCH_4_PIPE0m == mem || 
                                 EXACT_MATCH_4_PIPE1m == mem ||
                                 EXACT_MATCH_4_PIPE2m == mem || 
                                 EXACT_MATCH_4_PIPE3m == mem) ? 2 : 1;
                se = entry;
            } else {
                if (cb->src_mem == 0) {
                    continue;
                }
                if (MEM_IS_EXACT_MATCH(mem)) {
                    se = &buckets[level - 1][cb->src_bkt_idx].e.em[cb->src_ent];
                } else {
                    se = &buckets[level - 1][cb->src_bkt_idx].e.l3[cb->src_ent];
                }
            }

            bank_num = bi.bank_nums[cb->src_bix];
            bucket = _soc_mem_shared_hash_bucket(unit, cb->src_mem, bank_num, se);

            if (SHR_BITGET(bkt_trace, bucket)) {
                continue;
            }
            SHR_BITSET(bkt_trace, bucket);
            cb->bucket = bucket;
            if (MEM_IS_EXACT_MATCH(mem)) {
                rv = soc_mem_read(unit, base_mem, copyno, 
                                  bucket * SRAM_ENTRIES_PER_BUCKET, bkt_entry[0]);
                if (SOC_FAILURE(rv)) {
                    goto cleanup;
                }
                rv = soc_mem_read(unit, base_mem, copyno, 
                                  bucket * SRAM_ENTRIES_PER_BUCKET + 1, 
                                  bkt_entry[1]);
                if (SOC_FAILURE(rv)) {
                    goto cleanup;
                }
                soc_mem_field_get(unit, base_mem, bkt_entry[1], VALID_2f, &v[3]);
                soc_mem_field_get(unit, base_mem, bkt_entry[1], VALID_0f, &v[2]);
                soc_mem_field_get(unit, base_mem, bkt_entry[0], VALID_2f, &v[1]);
                soc_mem_field_get(unit, base_mem, bkt_entry[0], VALID_0f, &v[0]);
                soc_mem_field_get(unit, base_mem, bkt_entry[1], KEY_TYPE_2f, &k[3]);
                soc_mem_field_get(unit, base_mem, bkt_entry[1], KEY_TYPE_0f, &k[2]);
                soc_mem_field_get(unit, base_mem, bkt_entry[0], KEY_TYPE_2f, &k[1]);
                soc_mem_field_get(unit, base_mem, bkt_entry[0], KEY_TYPE_0f, &k[0]);

            } else {
                rv = soc_mem_read(unit, base_mem, copyno, bucket, bkt_entry[0]);
                if (SOC_FAILURE(rv)) {
                    goto cleanup;
                }
                soc_mem_field_get(unit, base_mem, bkt_entry[0], VALID_3f, &v[3]);
                soc_mem_field_get(unit, base_mem, bkt_entry[0], VALID_2f, &v[2]);
                soc_mem_field_get(unit, base_mem, bkt_entry[0], VALID_1f, &v[1]);
                soc_mem_field_get(unit, base_mem, bkt_entry[0], VALID_0f, &v[0]);
                soc_mem_field_get(unit, base_mem, bkt_entry[0], KEY_TYPE_3f, &k[3]);
                soc_mem_field_get(unit, base_mem, bkt_entry[0], KEY_TYPE_2f, &k[2]);
                soc_mem_field_get(unit, base_mem, bkt_entry[0], KEY_TYPE_1f, &k[1]);
                soc_mem_field_get(unit, base_mem, bkt_entry[0], KEY_TYPE_0f, &k[0]);                
            }
            cb->status = v[3] << 3 | v[2] << 2 | v[1] << 1 | v[0];

            for (ent = 0; ent < NUM_ENT; ent += ent_incr) {
                if (!v[ent]) {
                    ent_incr = 1;
                    continue;
                }
#if defined(BCM_TOMAHAWK_SUPPORT) 
                if (MEM_IS_EXACT_MATCH(mem)) {
                    me = &cb->e.em[ent];
                    _soc_mem_shared_hash_exact_match_bucket_read(unit, ent, k[ent],
                                                                 &ent_incr, &src_mem, 
                                                                 bkt_entry[ent / SRAM_ENTRIES_PER_BUCKET], 
                                                                 me, pipenum);
                } else 
#endif                
                {
                    me = &cb->e.l3[ent];
                    _soc_mem_shared_hash_l3_bucket_read(unit, ent, k[ent],
                                                        &ent_incr, &src_mem, 
                                                        bkt_entry[0], me);
                }

                rv = soc_mem_generic_insert(unit, src_mem, copyno,
                                            bi.banks & ~(1 << bank_num),
                                            me, NULL, &move_bkt);
                moved = 0;
                free = 0;
                if (SOC_SUCCESS(rv)) {

                    move_bkt /= _soc_mem_shared_hash_entries_per_bkt(src_mem);
                    if (SHR_BITGET(bkt_trace, move_bkt)) {
                        rv = soc_mem_generic_delete(unit, src_mem, MEM_BLOCK_ANY,
                                                    bi.banks & ~(1 << bank_num),
                                                    me, NULL, NULL);

                        if (SOC_FAILURE(rv)) {
                            goto cleanup;
                        }
                    } else {
                        rv = soc_mem_generic_delete(unit, src_mem, MEM_BLOCK_ANY,
                                                    1 << bank_num, me, NULL, NULL);
                        if (SOC_FAILURE(rv)) {
                            goto cleanup;
                        }
                        moved = 1;
                        cb->status &= ~(((1 << ent_incr) - 1) << ent);

                        _SOC_MEM_SHARED_HASH_STATUS_CHECK(free, cb->src_mem, cb->status);

                    }
                }

                if (!free) {
                    if (!moved) {
                        /* Construct buckets in next level for this entry */
                        nl_bkt_idx = bkt_idx * (bi.num_banks - 1) * NUM_ENT +
                                         ent * (bi.num_banks - 1);

                        for (bix = 0; bix < bi.num_banks && buckets[level + 1]; bix++) {
                            cbix = cb->src_bix + bix + 1;
                            cbix = cbix % bi.num_banks;
                            if (bank_num == bi.bank_nums[cbix]) {
                                continue;
                            }

                            tb = &buckets[level + 1][nl_bkt_idx++];
                            tb->src_bkt_idx = bkt_idx;
                            tb->src_bix  = cbix;
                            tb->src_mem  = src_mem;
                            tb->src_width= ent_incr;
                            tb->src_ent  = ent;
                        }
                    }
                    continue;
                }

                ibank_num = bank_num;
                pbi = cb;
                for (i = level; i > 0; i--) {
                    cbi = pbi;
                    pbi = &buckets[i - 1][cbi->src_bkt_idx];
                    if (MEM_IS_EXACT_MATCH(mem)) {
                        me = &pbi->e.em[cbi->src_ent];
                    } else {
                        me = &pbi->e.l3[cbi->src_ent];
                    }
                    rv = soc_mem_bank_insert(unit, cbi->src_mem,
                                             (1 << ibank_num), copyno,
                                             me, NULL);
                    if (SOC_FAILURE(rv)) {
                        goto cleanup;
                    }

                    ibank_num = bi.bank_nums[pbi->src_bix];

                    rv = soc_mem_generic_delete(unit, cbi->src_mem,
                                                MEM_BLOCK_ANY,
                                                (1 << ibank_num),
                                                me, NULL, NULL);
                    if (SOC_FAILURE(rv)) {
                        goto cleanup;
                    }

                    pbi->status &= ~(((1 << cbi->src_width) - 1) << cbi->src_ent);

                    _SOC_MEM_SHARED_HASH_STATUS_CHECK(free, pbi->src_mem, pbi->status);

                    if (!free) {
                        break;
                    }
                }

                /* now i is the toppest level with free slot available */

                if (free) {
                    job_done = 1;
                    goto done;
                } else {
                    /* cbi --> level i; pbi --> level i - 1.
                     * cbi is the toppest bucket with entry moved.
                     * pbi is the bucket with entry removed.
                     * Entire sub tree of pbi's removed entry needs be skipped.
                     */
                    for (j = i; j <= level; j++) {
                        if (j == i) {
                            num      = (bi.num_banks - 1);
                            bkt_idx1 = (cbi->src_bkt_idx * ((bi.num_banks - 1) * NUM_ENT)) +
                                       (cbi->src_ent * (bi.num_banks - 1));
                            bkt_idx2 = bkt_idx1 + (cbi->src_width * (bi.num_banks - 1));
                        } else {
                            num      *= (bi.num_banks - 1) * NUM_ENT;
                            bkt_idx1 *= (bi.num_banks - 1) * NUM_ENT;
                            bkt_idx2 *= (bi.num_banks - 1) * NUM_ENT;
                        }
                    }

                    /* assert (bkt_idx2 > bkt_idx && bkt_idx >= bkt_idx1); */
                    if (buckets[j]) {
                        num      *= (bi.num_banks - 1) * NUM_ENT;
                        bkt_idx1 *= (bi.num_banks - 1) * NUM_ENT;
                        sal_memset(&buckets[j][bkt_idx1], 0,
                                   num * sizeof(_soc_breadth_bucket_t));
                    }
                    bkt_idx = bkt_idx2 - 1;
                    break;
                }
            }
        }
        num_cl_bkts *= (bi.num_banks - 1) * NUM_ENT;
    }

done:
    if (job_done) {
        rv = soc_mem_generic_insert(unit, mem, copyno, 1 << ibank_num,
                                    entry, entry, NULL);
    } else {
        rv = SOC_E_FULL;
    }

cleanup:
    if (bkt_trace) {
        sal_free(bkt_trace);
    }

    if (buckets[0]) {
        sal_free(buckets[0]);
    }

    if (SOC_FAILURE(rv)) {
        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                                "Insert entry: %d\n"), rv));
    }

    return rv;
}


STATIC int
_soc_mem_shared_hash_depth_move(int unit, soc_mem_t mem,
                                int32 bank_num_start, int copyno,
                                void *entry, int recurse_depth,
                                _soc_hash_bank_info_t *bi_ptr,
                                SHR_BITDCL *trace)
{
    _soc_hash_bank_info_t bi;
    SHR_BITDCL *bkt_trace = NULL;
    SHR_BITDCL *bkt_trace2 = NULL;
    soc_mem_t l3_mem = L3_ENTRY_IPV6_MULTICASTm;
    soc_mem_t em_mem = EXACT_MATCH_4m;
    soc_mem_t orig_mem = 0;
    uint32 orig_entry[NUM_ENT][SOC_MAX_MEM_WORDS];
    uint32 bkt_entry[2][SOC_MAX_MEM_WORDS];
    uint8 v[NUM_ENT], k[NUM_ENT];
    int bank_num = 0;
    int cbix, bix;
    int rv = SOC_E_NONE;
    int ent, ent_incr, bucket, retry;
    int found = 0, status = 0, moved, move_bkt;
    int buckets[7] = {0}, mark = 0;
#if defined(BCM_TOMAHAWK_SUPPORT)
    int pipenum = -1;
    if (MEM_IS_EXACT_MATCH(mem)) {
        switch (mem) {
        case EXACT_MATCH_4_PIPE0m:
        case EXACT_MATCH_2_PIPE0m:
            pipenum = 0;
            break;
        case EXACT_MATCH_4_PIPE1m:
        case EXACT_MATCH_2_PIPE1m:
            pipenum = 1;
            break;
        case EXACT_MATCH_4_PIPE2m:
        case EXACT_MATCH_2_PIPE2m:
            pipenum = 2;
            break;
        case EXACT_MATCH_4_PIPE3m:
        case EXACT_MATCH_2_PIPE3m:
            pipenum = 3;
            break;
        default:
            break;
        }
    }  
#endif

    sal_memset(&bi, 0, sizeof(bi));
    if (bank_num_start == SOC_MEM_HASH_BANK_ALL) {
        if (recurse_depth < 0) {
            return SOC_E_FULL;
        }
        SOC_IF_ERROR_RETURN(
            _soc_mem_shared_hash_init(unit, mem, &bkt_trace,
                                      &bi, 0, NULL, NULL));
    } else {
        if (trace) {
            bkt_trace = trace;
        } else {
            return SOC_E_INTERNAL;
        }

        if (bi_ptr) {
            sal_memcpy(&bi, bi_ptr, sizeof(_soc_hash_bank_info_t));
        } else {
            sal_free(bkt_trace);
            return SOC_E_INTERNAL;
        }
    }

    bkt_trace2 = bkt_trace;

    if (SOC_MULTI_HASH_MOVE_ALGORITHM(unit) == MULTI_MOVE_MODE_DEPTH_FAST) {
        bkt_trace2 = bkt_trace + _SHR_BITDCLSIZE(bi.numb);
    }

    for (bix = 0; bix < bi.num_banks; bix++) {
        cbix = bix % bi.num_banks;
        bank_num = bi.bank_nums[cbix];
        if (bank_num_start == bank_num) {
            continue;
        }

        bucket = _soc_mem_shared_hash_bucket(unit, mem, bank_num, entry);

        if (SHR_BITGET(bkt_trace, bucket)) {
            continue;
        }
        SHR_BITSET(bkt_trace, bucket);
        SHR_BITSET(bkt_trace2, bucket);
        buckets[mark++] = bucket;

        if (mem != L2Xm) {
            if (mem == L3_ENTRY_ONLYm || 
                mem == L3_ENTRY_IPV4_UNICASTm || 
                mem == L3_ENTRY_IPV4_MULTICASTm || 
                mem == L3_ENTRY_IPV6_UNICASTm ||
                mem == L3_ENTRY_IPV6_MULTICASTm) {
                rv = soc_mem_read(unit, l3_mem, copyno, bucket, bkt_entry[0]);
                if (SOC_FAILURE(rv)) {
                    goto cleanup;
                }

                v[3] = soc_mem_field32_get(unit, l3_mem, bkt_entry[0], VALID_3f);
                v[2] = soc_mem_field32_get(unit, l3_mem, bkt_entry[0], VALID_2f);
                v[1] = soc_mem_field32_get(unit, l3_mem, bkt_entry[0], VALID_1f);
                v[0] = soc_mem_field32_get(unit, l3_mem, bkt_entry[0], VALID_0f);
                k[3] = soc_mem_field32_get(unit, l3_mem, bkt_entry[0], KEY_TYPE_3f);
                k[2] = soc_mem_field32_get(unit, l3_mem, bkt_entry[0], KEY_TYPE_2f);
                k[1] = soc_mem_field32_get(unit, l3_mem, bkt_entry[0], KEY_TYPE_1f);
                k[0] = soc_mem_field32_get(unit, l3_mem, bkt_entry[0], KEY_TYPE_0f);
            } else {
                rv = soc_mem_read(unit, em_mem, copyno, 
                                  bucket * SRAM_ENTRIES_PER_BUCKET, bkt_entry[0]);
                if (SOC_FAILURE(rv)) {
                    goto cleanup;
                }
                rv = soc_mem_read(unit, em_mem, copyno, 
                                  bucket * SRAM_ENTRIES_PER_BUCKET + 1, 
                                  bkt_entry[1]);
                if (SOC_FAILURE(rv)) {
                    goto cleanup;
                }
                v[3] = soc_mem_field32_get(unit, em_mem, &bkt_entry[1], VALID_2f);
                v[2] = soc_mem_field32_get(unit, em_mem, &bkt_entry[1], VALID_0f);
                v[1] = soc_mem_field32_get(unit, em_mem, &bkt_entry[0], VALID_2f);
                v[0] = soc_mem_field32_get(unit, em_mem, &bkt_entry[0], VALID_0f);
                k[3] = soc_mem_field32_get(unit, em_mem, &bkt_entry[1], KEY_TYPE_2f);
                k[2] = soc_mem_field32_get(unit, em_mem, &bkt_entry[1], KEY_TYPE_0f);
                k[1] = soc_mem_field32_get(unit, em_mem, &bkt_entry[0], KEY_TYPE_2f);
                k[0] = soc_mem_field32_get(unit, em_mem, &bkt_entry[0], KEY_TYPE_0f);
            }
            status = v[3] << 3 | v[2] << 2 | v[1] << 1 | v[0];
        }

        ent_incr = 1;
        retry = 0;
        for (ent = 0; ent < NUM_ENT; ent += ent_incr) {
            if (retry) {
                retry = 0;
            } else {
                if (mem == L2Xm) {
                    rv = soc_mem_read(unit, mem, copyno,
                                      (bucket << 2) + ent, &orig_entry[ent]);
                    if (SOC_FAILURE(rv)) {
                        goto cleanup;
                    }
                    orig_mem = mem;
                    ent_incr = 1;
                } else if (mem == L3_ENTRY_ONLYm || 
                           mem == L3_ENTRY_IPV4_UNICASTm || 
                           mem == L3_ENTRY_IPV4_MULTICASTm || 
                           mem == L3_ENTRY_IPV6_UNICASTm ||
                           mem == L3_ENTRY_IPV6_MULTICASTm) {
                    if (!v[ent]) {
                        ent_incr = 1;
                        continue;
                    }
                    _soc_mem_shared_hash_l3_bucket_read(unit, ent, k[ent],
                          &ent_incr, &orig_mem, bkt_entry[0], &orig_entry[ent]);
                } else {
#if defined(BCM_TOMAHAWK_SUPPORT) 
                    if (!v[ent]) {
                        ent_incr = 1;
                        continue;
                    }
                    _soc_mem_shared_hash_exact_match_bucket_read(unit, ent, k[ent],
                          &ent_incr, &orig_mem, bkt_entry[ent / SRAM_ENTRIES_PER_BUCKET], 
                          &orig_entry[ent], pipenum);
#endif                    
                }
            }

            rv = soc_mem_generic_insert(unit, orig_mem, copyno,
                                        bi.banks & ~(1 << bank_num),
                                        orig_entry[ent], NULL, &move_bkt);

            moved = 0;
            if (SOC_SUCCESS(rv)) {
                move_bkt /= _soc_mem_shared_hash_entries_per_bkt(orig_mem);
                if (SHR_BITGET(bkt_trace2, move_bkt)) {
                    rv = soc_mem_generic_delete(unit, orig_mem, MEM_BLOCK_ANY,
                                                bi.banks & ~(1 << bank_num),
                                                orig_entry[ent], NULL, NULL);
                    if (SOC_FAILURE(rv)) {
                        goto cleanup;
                    }
                } else {
                    rv = soc_mem_generic_delete(unit, orig_mem, MEM_BLOCK_ANY,
                                                (uint32)1<<bank_num,
                                                orig_entry[ent], NULL, NULL);
                    if (SOC_FAILURE(rv)) {
                        goto cleanup;
                    }
                    moved = 1;

                    if (mem == L2Xm) {
                        found = 1;
                        break;
                    } else {
                        status &= ~(((1 << ent_incr) - 1) << ent);
                        _SOC_MEM_SHARED_HASH_STATUS_CHECK(found, mem, status);
                    }
                }
            }

            if (found) {
                break;
            } else if (!moved && recurse_depth) {

                rv = _soc_mem_shared_hash_depth_move(unit, orig_mem,
                                                     bank_num, copyno,
                                                     orig_entry[ent],
                                                     recurse_depth - 1,
                                                     &bi, bkt_trace);
                if (SOC_SUCCESS(rv)) {
                    /* Deeper entry is moved, try again */
                    retry = 1;
                    ent -= ent_incr;
                } else if (rv != SOC_E_FULL) {
                    goto cleanup;
                }
            }
        }
        if (found) {
            break;
        }
    }

    if (!found) {
        rv = SOC_E_FULL;
    }

    if (bank_num_start == SOC_MEM_HASH_BANK_ALL) {
        if (found) {
            rv = soc_mem_generic_insert(unit, mem, copyno, (uint32)1<<bank_num,
                                        entry, entry, NULL);
        }
        if (rv) {
            LOG_VERBOSE(BSL_LS_SOC_COMMON,
                        (BSL_META_U(unit,
                                    "Insert entry: %d\n"), rv));
        }
    }

cleanup:
    for (ent_incr = 0; ent_incr < mark; ent_incr++) {
        SHR_BITCLR(bkt_trace2, buckets[ent_incr]);
    }
    if (bank_num_start == SOC_MEM_HASH_BANK_ALL) {
        sal_free(bkt_trace);
    }
    return rv;
}


/*
 * Function:
 *    _soc_mem_shared_hash_init
 * Purpose:
 *    Stack variables initialization & memory allocations for recursion algorithm
 * Param:
 *    mem  -  memory for new insert
 *    *num_ent   -  how may entries in each bucket
 *    **trace    -  bucket bitmap used to trace which buckets have been moved during recursion
 *    bank_info  -  bank properties
 *          num_bkt   -  how may buckets we need trace in recursion
 *          num_banks -  how may banks for this type of entry
 *          bank_ids  -  mapping for logical bank id to physical bank id for insert mem
 *          bank_bmp  -  physical bank id bitmap for this kind of mem table
 * Notes:
 *    None.
 */

STATIC INLINE int
_soc_mem_shared_hash_init(int unit, soc_mem_t mem,
                          SHR_BITDCL **trace,
                          _soc_hash_bank_info_t *bank_info,
                          int num_levels,
                          _soc_breadth_bucket_t **buckets,
                          _soc_breadth_bucket2_t **buckets2)
{
    uint8 bix;
    int alloc_size;
    SHR_BITDCL *bkt_trace = NULL;

    if (NULL == bank_info) {
        return SOC_E_PARAM;
    }

    /* Get the bank number of this mem table */
#ifdef BCM_APACHE_SUPPORT
    if (SOC_IS_APACHE(unit)) {
        SOC_IF_ERROR_RETURN
            (soc_apache_hash_bank_count_get(unit, mem, &bank_info->num_banks));
    } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
    if (SOC_IS_TOMAHAWKX(unit)) {
        SOC_IF_ERROR_RETURN
            (soc_tomahawk_hash_bank_count_get(unit, mem, &bank_info->num_banks));
    } else
#endif /* BCM_TOMAHAWK_SUPPORT */
    {
        SOC_IF_ERROR_RETURN
            (soc_trident2_hash_bank_count_get(unit, mem, &bank_info->num_banks));
    }

    /* Clear banks bitmap. */
    bank_info->banks = 0;

    for (bix = 0; bix < bank_info->num_banks; bix++) {
#ifdef BCM_APACHE_SUPPORT
        if (SOC_IS_APACHE(unit)) {
            SOC_IF_ERROR_RETURN
                (soc_apache_hash_bank_number_get(unit, mem, bix,
                                                   &bank_info->bank_nums[bix]));
        } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
        if (SOC_IS_TOMAHAWKX(unit)) {
            SOC_IF_ERROR_RETURN
                (soc_tomahawk_hash_bank_number_get(unit, mem, bix,
                                                   &bank_info->bank_nums[bix]));
        } else
#endif /* BCM_TOMAHAWK_SUPPORT */
        {
            SOC_IF_ERROR_RETURN
                (soc_trident2_hash_bank_number_get(unit, mem, bix,
                                                   &bank_info->bank_nums[bix]));
        }

        /* Get the all banks bitmap for hash insert. */
        bank_info->banks |= 1 << bank_info->bank_nums[bix];
    }

    if (SOC_MULTI_HASH_MOVE_ALGORITHM(unit) == MULTI_MOVE_MODE_LEGACY ||
        SOC_MULTI_HASH_MOVE_ALGORITHM(unit) == MULTI_MOVE_MODE_MIX)  {
        bank_info->numb = 65536;
        /* Keep back trace of all buckets affected by recursion. */
        alloc_size = SHR_BITALLOCSIZE(bank_info->numb * 10);
        /* For simplicity, allocate same number of bits for buckets per bank */
    } else {
        bank_info->numb = soc_mem_index_count(unit, mem) >> (L2Xm == mem ? 2 : 0);
        alloc_size = SHR_BITALLOCSIZE(bank_info->numb);
        if (SOC_MULTI_HASH_MOVE_ALGORITHM(unit) ==
            MULTI_MOVE_MODE_DEPTH_FAST) {
            alloc_size *= 2;
        }
    }

    bkt_trace = sal_alloc(alloc_size, "Shared hash");
    if (NULL == bkt_trace) {
        return SOC_E_MEMORY;
    }
    sal_memset(bkt_trace, 0, alloc_size);
    *trace = bkt_trace;


    if (buckets) {
        int num_cl_buckets;
        int level;

        alloc_size = 0;
        num_cl_buckets = bank_info->num_banks;
        for (level = 0; level < num_levels; level++) {
            alloc_size += num_cl_buckets;
            num_cl_buckets *= (bank_info->num_banks - 1) * 4;
        }
        alloc_size *= sizeof(_soc_breadth_bucket_t);

        *buckets = sal_alloc(alloc_size, "Buckets");
        if (*buckets == NULL) {
            sal_free(bkt_trace);
            return SOC_E_MEMORY;
        }
        sal_memset(*buckets, 0, alloc_size);
    }

    if (buckets2) {
        alloc_size = (bank_info->numb + 1) * sizeof(_soc_breadth_bucket2_t);
        *buckets2 = sal_alloc(alloc_size, "Buckets");
        if (*buckets2 == NULL) {
            sal_free(bkt_trace);
            if (buckets && *buckets) {
                sal_free(*buckets);
            }
            return SOC_E_MEMORY;
        }
        sal_memset(*buckets2, 0, alloc_size);
    }

    return SOC_E_NONE;
}

/*
 * Function:
 *    _soc_mem_shared_hash_move_entry_resolve
 * Purpose:
 *    Resolve the move mem and entry from current slot/index(with new mem view),
 *    1. If there is enough space for new mem insert (!(v0 || v1 || v2 || v3)),
 *        return free_slot = TRUE directly.
 *    2. If there is no enough space, resolve the move mem type and
 *        entry content for hash move.
 * Param:
 *    mem    -  memory for new insert
 *    copyno -  copy number
 *    index  -  Hash calculated HW index for this mem view in current bank
 *    *free_slot  -  If have enough spaces for hash move return TRUE; else FALSE
 *    *move_mem   -  mem type which will be moved.
 *    *move_entry -  mem entry which will be moved.
 * Notes:
 *    None.
 */

STATIC INLINE int
_soc_mem_shared_hash_move_entry_resolve(int unit, soc_mem_t mem, int copyno,
                                        int index, int *free_slot,
                                        soc_mem_t *move_mem, void *move_entry)
{
    int rv;
    uint32 v0, v1, v2, v3, k0, k1, k2, k3;
    soc_mem_t old_mem;
    int key_type, offset, l3_entry_only_index;

    /* Read entry information from current slot/index with new mem view */
    rv = soc_mem_read(unit, mem, copyno, index, move_entry);
    if (SOC_FAILURE(rv)) {
        return SOC_E_MEMORY;
    }
    /* Could be an empty slot here */
    v0 = v1 = v2 = v3 = 0;
    k0 = k1 = k2 = k3 = 0;
    *free_slot = FALSE;

    switch (mem) {
    case L3_ENTRY_IPV6_MULTICASTm:
        soc_mem_field_get(unit, mem, move_entry, VALID_3f, &v3);
        soc_mem_field_get(unit, mem, move_entry, VALID_2f, &v2);
        soc_mem_field_get(unit, mem, move_entry, KEY_TYPE_3f, &k3);
        soc_mem_field_get(unit, mem, move_entry, KEY_TYPE_2f, &k2);
        /* pass through */
    case L3_ENTRY_IPV6_UNICASTm:
    case L3_ENTRY_IPV4_MULTICASTm:
        soc_mem_field_get(unit, mem, move_entry, VALID_1f, &v1);
        soc_mem_field_get(unit, mem, move_entry, VALID_0f, &v0);
        soc_mem_field_get(unit, mem, move_entry, KEY_TYPE_1f, &k1);
        soc_mem_field_get(unit, mem, move_entry, KEY_TYPE_0f, &k0);
        break;
    case L3_ENTRY_IPV4_UNICASTm:
    case L3_ENTRY_ONLYm:
    case L2Xm:
        soc_mem_field_get(unit, mem, move_entry, VALIDf, &v0);
        soc_mem_field_get(unit, mem, move_entry, KEY_TYPEf, &k0);
        break;
    default:
        v0 = v1 = v2 = v3 = 1;
        break;
    }

    LOG_VERBOSE(BSL_LS_SOC_COMMON,
                (BSL_META_U(unit,
                            "Read %s from index %d (%d %d %d %d)\n"),
                 SOC_MEM_NAME(unit, mem), index, v0, v1, v2, v3));

    /* If it is an emty bucket or there are enough spaces for hash insert,
     * v0, v1, v2, v3 are all zero. */
    if (!(v0 || v1 || v2 || v3)) {
        *free_slot = TRUE;
        return SOC_E_NONE;
    }

    /* If no enough spaces, resolve move mem and move entry */
    if (mem == L2Xm) {
        *move_mem = L2Xm;
    } else {
        /* what is the key_type of old entry */
        key_type = v0 ? k0 : (v1 ? k1 : (v2 ? k2 : (v3 ? k3 : 0)));

        /* what is the mem table type of old entry */
        switch (key_type) {
        case TD2_L3_HASH_KEY_TYPE_V4UC:
        case TD2_L3_HASH_KEY_TYPE_TRILL:
        case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN:
        case TD2_L3_HASH_KEY_TYPE_FCOE_HOST:
        case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP:
            old_mem = L3_ENTRY_IPV4_UNICASTm;
            break;
        case TD2_L3_HASH_KEY_TYPE_V4UC_EXT:
        case TD2_L3_HASH_KEY_TYPE_V4MC:
        case TD2_L3_HASH_KEY_TYPE_V4L2MC:
        case TD2_L3_HASH_KEY_TYPE_V4L2VPMC:
        case TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN_EXT:
        case TD2_L3_HASH_KEY_TYPE_FCOE_HOST_EXT:
        case TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP_EXT:
        case TD2_L3_HASH_KEY_TYPE_DST_NAT:
        case TD2_L3_HASH_KEY_TYPE_DST_NAPT:
            old_mem = L3_ENTRY_IPV4_MULTICASTm;
            break;
        case TD2_L3_HASH_KEY_TYPE_V6UC:
            old_mem = L3_ENTRY_IPV6_UNICASTm;
            break;
        case TD2_L3_HASH_KEY_TYPE_V6UC_EXT:
        case TD2_L3_HASH_KEY_TYPE_V6MC:
        case TD2_L3_HASH_KEY_TYPE_V6L2MC:
        case TD2_L3_HASH_KEY_TYPE_V6L2VPMC:
            old_mem = L3_ENTRY_IPV6_MULTICASTm;
            break;
        default:
            return SOC_E_INTERNAL;
        }

        /* If different mem table, re-calculate the index and re-read the move entry */
        if (mem != old_mem) {
            /* coverity[dead_error_line] */
            offset = v0 ? 0 : (v1 ? 1 : (v2 ? 2 : (v3 ? 3 : 0)));
            switch (mem) {
            case L3_ENTRY_IPV6_MULTICASTm:
                l3_entry_only_index = index * 4 + offset;
                break;
            case L3_ENTRY_IPV6_UNICASTm:
            case L3_ENTRY_IPV4_MULTICASTm:
                l3_entry_only_index = index * 2 + offset;
                break;
            case L3_ENTRY_IPV4_UNICASTm:
            default:
                l3_entry_only_index = index + offset;
                break;
            }
            /* re-calculate the move entry index */
            switch (old_mem) {
            case L3_ENTRY_IPV6_MULTICASTm:
                index = l3_entry_only_index / 4;
                break;
            case L3_ENTRY_IPV6_UNICASTm:
            case L3_ENTRY_IPV4_MULTICASTm:
                index = l3_entry_only_index / 2;
                break;
            case L3_ENTRY_IPV4_UNICASTm:
            default:
                index = l3_entry_only_index;
                break;
            }
            /* re-read the move entry */
            rv = soc_mem_read(unit, old_mem, copyno, index, move_entry);
            if (SOC_FAILURE(rv)) {
                return SOC_E_MEMORY;
            }
        }

        /* return move mem table type */
        *move_mem = old_mem;
    }

    return SOC_E_NONE;
}

/*
 * Function:
 *    _soc_mem_shared_hash_breadth_move
 * Purpose:
 *    Breadth recursion algorithm for shared hash move
 * Notes:
 *    None.
 */

STATIC int
_soc_mem_shared_hash_breadth_move(int unit, soc_mem_t mem,
                                  int32 banks, int copyno,
                                  void *entry, SHR_BITDCL *bucket_trace,
                                  int recurse, int recurse_depth,
                                  _soc_hash_bank_info_t *bank_info)
{
    SHR_BITDCL *trace = NULL;
    _soc_hash_bank_info_t bank_inf = {0};
    uint8 num_ent = 0;
    int nbix, cbix;
    int rv = SOC_E_NONE, index;
    uint8 i, bix, found = FALSE, cb = 0;
    uint32 db, dest_bucket, dbs;
    uint32 bucket, move_entry[SOC_MAX_MEM_WORDS];
    soc_mem_t ori_mem = mem;
    int banks_seq_num = 0;

    if (recurse_depth < 0) {
        return SOC_E_FULL;
    }
    /* Get the entry number per each bucket */
    num_ent = _soc_mem_shared_hash_entries_per_bkt(mem);

    /* Stack variables initialization & memory allocations */
    if (NULL == bucket_trace) {
        SOC_IF_ERROR_RETURN(
            _soc_mem_shared_hash_init(unit, mem, &trace, &bank_inf, 0, NULL, NULL));
    } else {
        trace = bucket_trace;
        if (NULL != bank_info) {
            sal_memcpy(&bank_inf, bank_info, sizeof(_soc_hash_bank_info_t));
        } else {
            rv = SOC_E_INTERNAL;
            goto clean_up;
        }
    }
    if (banks != SOC_MEM_HASH_BANK_ALL) {
#ifdef BCM_APACHE_SUPPORT
        if (SOC_IS_APACHE(unit)) {
            rv = soc_apache_hash_seq_number_get(unit, mem, banks,
                                                  &banks_seq_num);
        } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
        if (SOC_IS_TOMAHAWKX(unit)) {
            rv = soc_tomahawk_hash_seq_number_get(unit, mem, banks,
                                                  &banks_seq_num);
        } else
#endif /* BCM_TOMAHAWK_SUPPORT */
        {
            rv = soc_trident2_hash_seq_number_get(unit, mem, banks,
                                                  &banks_seq_num);
        }
        if (SOC_FAILURE(rv)) {
            goto clean_up;
        }
    } else {
        /* Make sure we will try from the first bank */
        banks_seq_num = -1;
    }
    /* Iterate over banks. */
    for (bix = 0; bix < bank_inf.num_banks; bix++) {
        /* if banks is present, try interate from next bank */
        cbix = banks_seq_num + bix + 1;
        cbix = cbix % bank_inf.num_banks;
        cb = bank_inf.bank_nums[cbix];
        if (banks == cb) {
            /* Not this bank */
            continue;
        }
#ifdef BCM_APACHE_SUPPORT
        if (SOC_IS_APACHE(unit)) {
            rv = soc_ap_hash_bucket_get(unit, mem, cb, entry, &bucket);
        } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
        if (SOC_IS_TOMAHAWKX(unit)) {
            rv = soc_th_hash_bucket_get(unit, mem, cb, entry, &bucket);
        } else
#endif /* BCM_TOMAHAWK_SUPPORT */
        {
            rv = soc_hash_bucket_get(unit, mem, cb, entry, &bucket);
        }

        if (SOC_FAILURE(rv)) {
            break;
        }
        nbix = (cbix + 1) % bank_inf.num_banks; /* next bank index */
        db = bank_inf.bank_nums[nbix]; /* next destination bank */
        /* For simplicity, use dbs instead of db for hash move.
        * The dbs only used in breadth iteration loop */
        dbs = bank_inf.banks & ~(1 << cb);
        /* Only record the buckets in breadth iteration loop */
        if (!recurse) {
            /* If this bucket was already iterated, Try next */
            if (SHR_BITGET(trace, (bank_inf.numb * cbix) + bucket)) {
                continue;
            }
            SHR_BITSET(trace, (bank_inf.numb * cbix) + bucket);
        }
#ifdef BCM_APACHE_SUPPORT
        if (SOC_IS_APACHE(unit)) {
            index = soc_ap_hash_index_get(unit, mem, cb, bucket);
        } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
        if (SOC_IS_TOMAHAWKX(unit)) {
            index = soc_th_hash_index_get(unit, mem, cb, bucket);
        } else
#endif /* BCM_TOMAHAWK_SUPPORT */
        {
            index = soc_hash_index_get(unit, mem, cb, bucket);
        }
        for (i = 0; i < num_ent;) {
            int free_slot = FALSE;
            /* Is there free slot to insert this new entry?
               If no, resolve the move_mem and move_entry from current index */
            rv = _soc_mem_shared_hash_move_entry_resolve(unit, mem, copyno,
                                                         index + i, &free_slot,
                                                         &mem, move_entry);
            if (SOC_FAILURE(rv)) {
                break;
            }
            /* If have free slot for move entry, insert directly */
            if (free_slot) {
                found = TRUE;
                break;
            }

            /* Are we already in recursion */
            if (recurse) {
                /* Will we be in recursion */
                if (recurse_depth) {
                    /* Calculate destination entry hash value. */
#ifdef BCM_APACHE_SUPPORT
                    if (SOC_IS_APACHE(unit)) {
                        (void)soc_ap_hash_bucket_get(unit, mem, db, move_entry,
                                                     &dest_bucket);
                    } else
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
                    if (SOC_IS_TOMAHAWKX(unit)) {
                        (void)soc_th_hash_bucket_get(unit, mem, db, move_entry,
                                                     &dest_bucket);
                    } else
#endif /* BCM_TOMAHAWK_SUPPORT */
                    {
                        (void)soc_hash_bucket_get(unit, mem, db, move_entry,
                                                  &dest_bucket);
                    }
                    if (SHR_BITGET(trace, (bank_inf.numb * nbix) + dest_bucket)) {
                        /* Try next entry in the same bucket in original
                         * memory view */
                        mem = ori_mem;
                        i += _soc_mem_shared_hash_incr_per_entry(unit, mem,
                                                                 move_entry,
                                                                 entry);
                        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                                    (BSL_META_U(unit,
                                                "  Skip bank %d bucket %d\n"),
                                     db, dest_bucket));
                        continue;
                    }
                }

                /* Recursive call - attempt to create a slot
                   in another bank's bucket. */
                rv = _soc_mem_shared_hash_breadth_move(unit, mem, cb, copyno,
                                                       move_entry, trace, FALSE,
                                                       recurse_depth - 1,
                                                       &bank_inf);
            } else {
                /* Attempt to insert it into the other banks. Use dbs instead of db. */
                rv = soc_mem_bank_insert(unit, mem, dbs, copyno,
                                         move_entry, NULL);
            }

            if (SOC_FAILURE(rv)) {
                if (rv != SOC_E_FULL) {
                    break;
                }
                mem = ori_mem;
                i += _soc_mem_shared_hash_incr_per_entry(unit, mem,
                                                         move_entry, entry);
                continue;
            }
            /* Delete old entry from original location */
            rv = soc_mem_generic_delete(unit, mem, MEM_BLOCK_ANY, (uint32)1<<cb,
                                        move_entry, NULL, NULL);
            mem = ori_mem;
            if ((rv < 0) && (rv != SOC_E_FULL)) {
                break;
            }
        }  /* Bucket iteration loop. */

        if (found || ((rv < 0) && (rv != SOC_E_FULL))) {
            break;
        }
    }

    if (!recurse && !found && (rv == SOC_E_NONE || rv == SOC_E_FULL)) {
        /* Recursive call - attempt to go deeper in the hierarchy. */
        rv = _soc_mem_shared_hash_breadth_move(unit, mem, banks, copyno, entry,
                                               trace, TRUE, recurse_depth,
                                               &bank_inf);
        if (SOC_SUCCESS(rv)) {
            goto clean_up;
        }
    }

    /* Bank iteration loop. */
    if ((rv < 0) && (rv != SOC_E_FULL)) {
        goto clean_up;
    }
    if (!found) {
        rv = SOC_E_FULL;
        goto clean_up;
    }

    rv = soc_mem_generic_insert(unit, mem, copyno, (uint32)1<<cb,
                                entry, entry, NULL);
    if (rv) {
        LOG_VERBOSE(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                                "Insert entry: %d\n"), rv));
    }

clean_up:
    if (NULL == bucket_trace) {
        sal_free(trace);
    }

    return rv;
}


STATIC int
_soc_mem_shared_hash_insert(int unit, soc_mem_t mem, int copyno,
                            void *entry_data, void *old_entry_data,
                            int recurse_depth)
{
    int rv = SOC_E_NONE;
    rv = soc_mem_bank_insert(unit, mem, SOC_MEM_HASH_BANK_ALL, copyno,
                             entry_data, old_entry_data);
    if (rv != SOC_E_FULL) {
        return rv;
    }

    /* Time to shuffle the entries */
    soc_mem_lock(unit, mem);
    switch (SOC_MULTI_HASH_MOVE_ALGORITHM(unit)) {
        case MULTI_MOVE_MODE_LEGACY:
            rv = _soc_mem_shared_hash_move(unit, mem, SOC_MEM_HASH_BANK_ALL,
                                           copyno, entry_data, NULL,
                                           recurse_depth - 1, NULL);
            break;
        case MULTI_MOVE_MODE_MIX:
            rv = _soc_mem_shared_hash_breadth_move(unit, mem, SOC_MEM_HASH_BANK_ALL,
                                                   copyno, entry_data, NULL, 0,
                                                   recurse_depth - 1, NULL);
            break;
        case MULTI_MOVE_MODE_DEPTH_FAST:
        case MULTI_MOVE_MODE_DEPTH:
            rv = _soc_mem_shared_hash_depth_move(unit, mem, SOC_MEM_HASH_BANK_ALL,
                                                 copyno, entry_data,
                                                 recurse_depth - 1, NULL, NULL);
            break;
        default:
            if (mem == L2Xm) {
                rv = _soc_mem_shared_hash_breadth_l2_move(unit, mem, copyno,
                                                          entry_data, recurse_depth);
            } else {
                rv = _soc_mem_shared_hash_breadth_mix_move(unit, mem, copyno,
                                                           entry_data, recurse_depth);
            }
            break;
    }
    soc_mem_unlock(unit, mem);
    return rv;
}
#endif /* BCM_TRIDENT2_SUPPORT */

#endif /* BCM_FIREBOLT2_SUPPORT || BCM_TRX_SUPPORT \
          || BCM_RAVEN_SUPPORT */

/*
 * Function:
 *    _soc_mem_insert
 * Purpose:
 *    Helper routine for soc_mem_insert
 */

STATIC int
_soc_mem_insert(int unit,
                soc_mem_t mem,        /* Assumes memory locked */
                int copyno,
                void *entry_data)
{
    uint32        entry_tmp[SOC_MAX_MEM_WORDS];
    int            max, ins_index, index, last, rv;

    max = soc_mem_index_max(unit, mem);
    last = soc_mem_index_last(unit, mem, copyno);

    rv = soc_mem_search(unit, mem, copyno,
                        &ins_index, entry_data, entry_tmp, 0);

    if (rv == SOC_E_NONE) {
        /* If entry with matching key already exists, overwrite it. */

        if ((rv = soc_mem_write(unit, mem, copyno,
                                ins_index, entry_data)) < 0) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "soc_mem_insert: write %s.%s[%d] failed\n"),
                       SOC_MEM_UFNAME(unit, mem),
                       SOC_BLOCK_NAME(unit, copyno), ins_index));
            return rv;
        }

        return SOC_E_NONE;
    }

    if (rv != SOC_E_NOT_FOUND) {
        return rv;
    }

    /* Point to first unused entry; fail if table is already full. */

    if ((index = last + 1) > max) {
        return SOC_E_FULL;
    }

    while (index > ins_index) {
        /* Move up one entry */

        if ((rv = soc_mem_read(unit, mem, copyno,
                               index - 1, entry_tmp)) < 0) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "soc_mem_insert: read %s.%s[%d] failed\n"),
                       SOC_MEM_UFNAME(unit, mem),
                       SOC_BLOCK_NAME(unit, copyno), index - 1));
            return rv;
        }

        if ((rv = soc_mem_write(unit, mem, copyno,
                                index, entry_tmp)) < 0) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "soc_mem_insert: write %s.%s[%d] failed\n"),
                       SOC_MEM_UFNAME(unit, mem),
                       SOC_BLOCK_NAME(unit, copyno), index));
            return rv;
        }

        index--;
    }

    /* Write entry data at insertion point */

    if ((rv = soc_mem_write(unit, mem, copyno,
                            ins_index, entry_data)) < 0) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_insert: write %s.%s[%d] failed\n"),
                   SOC_MEM_UFNAME(unit, mem),
                   SOC_BLOCK_NAME(unit, copyno), ins_index));
        return rv;
    }
    _SOC_MEM_REUSE_MEM_STATE(unit, mem);
    SOP_MEM_STATE(unit, mem).count[copyno]++;

    return SOC_E_NONE;
}

/*
 * Function:
 *    soc_mem_insert
 * Purpose:
 *    Insert an entry into a structured table based on key.
 * Returns:
 *    SOC_E_NONE - insertion succeeded
 *    SOC_E_FULL - table full
 *    SOC_E_XXX - other error
 * Notes:
 *      Tables may be binary, hashed, CAM, or command.
 *
 *      Binary tables:
 *    A binary search is made for the entry, and if a matching key is
 *    found, the entry is overwritten with new data, and 0 is returned.
 *    If key is not found:
 *    Starting with the last entry (kept in soc_control_t structure),
 *    entries are moved down until a space is made for the required
 *    index.   The moving process results in temporary duplicates but not
 *    temporarily-out-of-order entries.
 *    Fails if the table is already full before the insert.
 *
 *      Hashed tables:
 *      The entry is passed to the hardware, which performs a hash
 *      calculation on the key.  The entry will be added to the
 *      corresponding hash bucket, if an available slot exists.
 *      Fails if the hash bucket for that key is full.
 *
 *      CAM tables:
 *      Entries are added to the CAM based on the device's
 *      implementation-specific algorithm.
 *      Fails if table resources are exhausted.
 *
 *      Command supported tables:
 *      Uses memory command hardware support to handle table state,
 *      based on the entry key.
 *      Fails if hardware insert operation is unsuccessful.
 *
 *    If COPYNO_ALL is specified, the insertion is repeated for each
 *    copy of the table.
 */

int
soc_mem_insert(int unit,
               soc_mem_t mem,
               int copyno,
               void *entry_data)
{
    int rv = SOC_E_NONE;

    assert(soc_mem_is_sorted(unit, mem) ||
           soc_mem_is_hashed(unit, mem) ||
           soc_mem_is_cam(unit, mem) ||
           soc_mem_is_cmd(unit, mem));
    assert(entry_data);

    if (copyno == COPYNO_ALL) {

#ifdef BCM_TRX_SUPPORT
        if (SOC_IS_TRX(unit)) {
            _SOC_MEM_REPLACE_MEM(unit, mem);
            switch (mem) {
            case MPLS_ENTRYm:
                if (SOC_IS_SC_CQ(unit)) {
                    break;
                }
            case VLAN_XLATEm:
            case EGR_VLAN_XLATEm:
#ifdef BCM_ISM_SUPPORT
                if (soc_feature(unit, soc_feature_ism_memory)) {
                    break;
                }
#endif /* BCM_ISM_SUPPORT */
            case VLAN_MACm:
            case AXP_WRX_WCDm:
            case AXP_WRX_SVP_ASSIGNMENTm:
#ifdef BCM_TRIUMPH3_SUPPORT
            case FT_SESSIONm:
            case FT_SESSION_IPV6m:
#endif /* BCM_TRIUMPH3_SUPPORT */
#ifdef BCM_TRIDENT2_SUPPORT
            case ING_VP_VLAN_MEMBERSHIPm:
            case EGR_VP_VLAN_MEMBERSHIPm:
            case ING_DNAT_ADDRESS_TYPEm:
            case L2_ENDPOINT_IDm:
            case ENDPOINT_QUEUE_MAPm:
#endif /* BCM_TRIDENT2_SUPPORT */
#ifdef BCM_KATANA2_SUPPORT
            case EGR_MP_GROUPm:
#endif
#ifdef BCM_SABER2_SUPPORT
            case MP_GROUPm:
#endif
                return _soc_mem_dual_hash_insert(unit, mem, copyno,
                                                 entry_data, NULL,
                           soc_dual_hash_recurse_depth_get(unit, mem));
            default:
                break;
            }
        }
#endif /* BCM_TRX_SUPPORT */

#ifdef BCM_FIREBOLT_SUPPORT
        if (SOC_IS_FBX(unit)) {
            switch (mem) {
            case L2Xm:
#ifdef BCM_TRIDENT2_SUPPORT
                if (soc_feature(unit, soc_feature_shared_hash_mem)) {
                    return _soc_mem_shared_hash_insert(unit, mem, copyno,
                                                       entry_data, NULL,
                               soc_multi_hash_recurse_depth_get(unit, mem));
                }
#endif /* BCM_TRIDENT2_SUPPORT */
                return soc_fb_l2x_insert(unit, entry_data);
#if defined(INCLUDE_L3)
            case L3_DEFIPm:
                return soc_fb_lpm_insert(unit, entry_data);
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
#ifdef BCM_TRIDENT2_SUPPORT
                if (soc_feature(unit, soc_feature_shared_hash_mem)) {
                    return _soc_mem_shared_hash_insert(unit, mem, copyno,
                                                       entry_data, NULL,
                               soc_multi_hash_recurse_depth_get(unit, mem));
                }
#endif /* BCM_TRIDENT2_SUPPORT */
                return soc_fb_l3x_insert(unit, entry_data);
#endif /* INCLUDE_L3 */
            case VLAN_MACm:
#if defined(BCM_RAVEN_SUPPORT)
                if (soc_feature(unit, soc_feature_dual_hash) &&
                    (SOC_IS_RAVEN(unit) || SOC_IS_HAWKEYE(unit))) {
                   return _soc_mem_dual_hash_insert(unit, mem, copyno,
                                                    entry_data, NULL,
                              soc_dual_hash_recurse_depth_get(unit, mem));
                } else
#endif
            {
                return soc_fb_vlanmac_entry_ins(unit, entry_data);
            }
#ifdef BCM_TRIUMPH3_SUPPORT
            case EXT_L2_ENTRY_1m:
            case EXT_L2_ENTRY_2m:
               return soc_mem_generic_insert(unit, mem, copyno, -1,
                                              entry_data, NULL, NULL);
#endif /* BCM_TRIUMPH3_SUPPORT */
            default:
                break;
            }
        }
#endif /* BCM_FIREBOLT_SUPPORT */

#ifdef BCM_ISM_SUPPORT
        if (soc_feature(unit, soc_feature_ism_memory) &&
            soc_mem_is_mview(unit, mem)) {
            return _soc_mem_multi_hash_insert(unit, mem, copyno, entry_data,
                       NULL, soc_multi_hash_recurse_depth_get(unit, mem));
        }
#endif /* BCM_ISM_SUPPORT */

        MEM_LOCK(unit, mem);
        SOC_MEM_BLOCK_ITER(unit, mem, copyno) {
            if ((rv = _soc_mem_insert(unit, mem, copyno, entry_data)) < 0) {
                break;
            }
        }
    } else {
        assert(SOC_MEM_BLOCK_VALID(unit, mem, copyno));
        MEM_LOCK(unit, mem);
        rv = _soc_mem_insert(unit, mem, copyno, entry_data);
    }

    MEM_UNLOCK(unit, mem);

    return rv;
}

/*
 * Function:
 *    soc_mem_insert_return_old
 * Purpose:
 *      Same as soc_mem_insert() with the additional feature
 *      that if an exiting entry was replaced, the old entry data
 *      is returned back.
 * Returns:
 *    SOC_E_NONE - insertion succeeded
 *    SOC_E_EXISTS - insertion succeeded, old_entry_data is valid
 *    SOC_E_FULL - table full
 *    SOC_E_UNAVAIL - not supported on the specified unit, mem
 *    SOC_E_XXX - other error
 */
int
soc_mem_insert_return_old(int unit,
                          soc_mem_t mem,
                          int copyno,
                          void *entry_data,
                          void *old_entry_data)
{
#ifdef BCM_TRX_SUPPORT
    if (SOC_IS_TRX(unit)) {
        _SOC_MEM_REPLACE_MEM(unit, mem);
        switch (mem) {
        case L2Xm: /* Note: the order of the case statements should be
                            changed with care */
#if defined(INCLUDE_L3)
        case L3_ENTRY_ONLYm:
        case L3_ENTRY_IPV4_UNICASTm:
        case L3_ENTRY_IPV4_MULTICASTm:
        case L3_ENTRY_IPV6_UNICASTm:
        case L3_ENTRY_IPV6_MULTICASTm:
#endif /* INCLUDE_L3 */
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
#ifdef BCM_TRIDENT2_SUPPORT
            if (soc_feature(unit, soc_feature_shared_hash_mem)) {
                return _soc_mem_shared_hash_insert(unit, mem, copyno, entry_data,
                           NULL, soc_multi_hash_recurse_depth_get(unit, mem));
            } else
#endif /* BCM_TRIDENT2_SUPPORT */
            {
                return _soc_mem_dual_hash_insert(unit, mem, copyno,
                                                 entry_data, old_entry_data,
                           soc_dual_hash_recurse_depth_get(unit, mem));
            }
        case MPLS_ENTRYm: /* Note: the order of the case statements should be
                                   changed with care */
            if (SOC_IS_SC_CQ(unit)) {
                break;
            }
        /* passthru */
        /* coverity[fallthrough: FALSE] */
        case EGR_VLAN_XLATEm:
        case VLAN_XLATEm:
        case VLAN_MACm:
        case AXP_WRX_WCDm:
        case AXP_WRX_SVP_ASSIGNMENTm:
#ifdef BCM_TRIUMPH3_SUPPORT
        case FT_SESSIONm:
        case FT_SESSION_IPV6m:
#endif /* BCM_TRIUMPH3_SUPPORT */
#ifdef BCM_TRIDENT2_SUPPORT
        case ING_VP_VLAN_MEMBERSHIPm:
        case EGR_VP_VLAN_MEMBERSHIPm:
        case ING_DNAT_ADDRESS_TYPEm:
        case L2_ENDPOINT_IDm:
        case ENDPOINT_QUEUE_MAPm:
#endif /* BCM_TRIDENT2_SUPPORT */
#ifdef BCM_ISM_SUPPORT
            if (soc_feature(unit, soc_feature_ism_memory) &&
                soc_mem_is_mview(unit, mem)) {
                return _soc_mem_multi_hash_insert(unit, mem, copyno,
                                                  entry_data, old_entry_data,
                           soc_multi_hash_recurse_depth_get(unit, mem));
            } else
#endif /* BCM_ISM_SUPPORT */
            {
                return _soc_mem_dual_hash_insert(unit, mem, copyno,
                                                 entry_data, old_entry_data,
                           soc_dual_hash_recurse_depth_get(unit, mem));
            }
#ifdef BCM_TRIUMPH3_SUPPORT
        case EXT_L2_ENTRY_1m:
        case EXT_L2_ENTRY_2m:
            return soc_mem_generic_insert(unit, mem, copyno, -1,
                                          entry_data, old_entry_data, NULL);
#endif /* BCM_TRIUMPH3_SUPPORT */
#ifdef BCM_ISM_SUPPORT
        case L2_ENTRY_1m:
        case L2_ENTRY_2m:
        case L3_ENTRY_1m:
        case L3_ENTRY_2m:
        case L3_ENTRY_4m:
        case VLAN_XLATE_EXTDm:
        case MPLS_ENTRY_EXTDm:
            return _soc_mem_multi_hash_insert(unit, mem, copyno,
                                              entry_data, old_entry_data,
                       soc_multi_hash_recurse_depth_get(unit, mem));
#endif /* BCM_ISM_SUPPORT */
            break;
        }
    }
#endif
    return SOC_E_UNAVAIL;
}

/*
 * Function:
 *    _soc_mem_delete_index
 * Purpose:
 *    Helper function for soc_mem_delete and soc_mem_delete_index.
 */

STATIC int
_soc_mem_delete_index(int unit,
                      soc_mem_t mem,        /* Assumes memory locked */
                      int copyno,
                      int del_index)
{
    uint32        entry_tmp[SOC_MAX_MEM_WORDS];
    int            min, index, eot_index, rv;

    /* eot_index points one past last entry in use */

    if (soc_mem_is_cam(unit, mem)) {
        index = del_index;
    } else {
        min = soc_mem_index_min(unit, mem);
        eot_index = soc_mem_index_last(unit, mem, copyno) + 1;

        if (del_index < min || del_index >= eot_index) {
            return SOC_E_NOT_FOUND;
        }

        for (index = del_index; index + 1 < eot_index; index++) {
            /* Move down one entry */

            if ((rv = soc_mem_read(unit, mem, copyno,
                                   index + 1, entry_tmp)) < 0) {
                LOG_ERROR(BSL_LS_SOC_SOCMEM,
                          (BSL_META_U(unit,
                                      "soc_mem_delete_index: "
                                      "read %s.%s[%d] failed\n"),
                           SOC_MEM_UFNAME(unit, mem),
                           SOC_BLOCK_NAME(unit, copyno), index + 1));
                return rv;
            }

            if ((rv = soc_mem_write(unit, mem, copyno,
                                    index, entry_tmp)) < 0) {
                LOG_ERROR(BSL_LS_SOC_SOCMEM,
                          (BSL_META_U(unit,
                                      "soc_mem_delete_index: "
                                      "write %s.%s[%d] failed\n"),
                           SOC_MEM_UFNAME(unit, mem),
                           SOC_BLOCK_NAME(unit, copyno), index));
                return rv;
            }
        }
    }

    if ((rv = soc_mem_write(unit, mem, copyno, index,
                            soc_mem_entry_null(unit, mem))) < 0) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_delete_index: "
                              "write %s.%s[%d] failed\n"),
                   SOC_MEM_UFNAME(unit, mem),
                   SOC_BLOCK_NAME(unit, copyno), index + 1));
        return rv;
    }
    _SOC_MEM_REUSE_MEM_STATE(unit, mem);
    SOP_MEM_STATE(unit, mem).count[copyno]--;

    return SOC_E_NONE;
}

/*
 * Function:
 *    soc_mem_delete_index
 * Purpose:
 *    Delete an entry from a sorted table based on index,
 *    moving up all successive entries.
 * Notes:
 *    If COPYNO_ALL is specified, the deletion is repeated for each
 *    copy of the table.
 *
 *    For deletes from the ARL, reads the entry and requests hardware
 *    deletion by key.  This shouldn't generally be done, because the
 *    hardware may change the ARL table at any time.
 */

int
soc_mem_delete_index(int unit,
                     soc_mem_t mem,
                     int copyno,
                     int index)
{
    int rv = SOC_E_NONE;

    assert(soc_mem_is_sorted(unit, mem) ||
           soc_mem_is_hashed(unit, mem) ||
           soc_mem_is_cam(unit, mem) ||
           soc_mem_is_cmd(unit, mem));

#ifdef BCM_TRX_SUPPORT
    if (soc_feature(unit, soc_feature_generic_table_ops)) {
        uint32 entry_tmp[SOC_MAX_MEM_WORDS];
        _SOC_MEM_REPLACE_MEM(unit, mem);
        switch (mem) {
        case L2Xm:
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
        case EGR_VLAN_XLATEm:
        case VLAN_XLATEm:
        case VLAN_MACm:
        case MPLS_ENTRYm:
        case AXP_WRX_WCDm:
        case AXP_WRX_SVP_ASSIGNMENTm:
#ifdef BCM_TRIDENT2_SUPPORT
        case ING_VP_VLAN_MEMBERSHIPm:
        case EGR_VP_VLAN_MEMBERSHIPm:
        case ING_DNAT_ADDRESS_TYPEm:
        case L2_ENDPOINT_IDm:
        case ENDPOINT_QUEUE_MAPm:
#endif /* BCM_TRIDENT2_SUPPORT */
        case EXT_L2_ENTRYm:
#ifdef BCM_ISM_SUPPORT
        case L2_ENTRY_1m:
        case L2_ENTRY_2m:
        case L3_ENTRY_1m:
        case L3_ENTRY_2m:
        case L3_ENTRY_4m:
        case VLAN_XLATE_EXTDm:
        case MPLS_ENTRY_EXTDm:
#endif /* BCM_ISM_SUPPORT */
#ifdef BCM_TRIUMPH3_SUPPORT
        case EXT_L2_ENTRY_1m:
        case EXT_L2_ENTRY_2m:
        case FT_SESSIONm:
        case FT_SESSION_IPV6m:
#endif /* BCM_TRIUMPH3_SUPPORT */
#if defined(BCM_KATANA2_SUPPORT)
        case EGR_MP_GROUPm:
#endif
#ifdef BCM_SABER2_SUPPORT
        case MP_GROUPm:
#endif
            SOC_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ANY,
                                             index, entry_tmp));
            return soc_mem_delete(unit, mem, copyno, entry_tmp);
        default:
            break;
        }
    }
#endif /* BCM_TRX_SUPPORT */

#ifdef BCM_FIREBOLT_SUPPORT
    if (SOC_IS_FBX(unit)) {
        uint32 entry_tmp[SOC_MAX_MEM_WORDS];

        switch (mem) {
        case L3_ENTRY_IPV4_UNICASTm:
        case L3_ENTRY_IPV4_MULTICASTm:
        case L3_ENTRY_IPV6_UNICASTm:
        case L3_ENTRY_IPV6_MULTICASTm:
        case L2Xm:
        case VLAN_MACm:
            SOC_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ANY,
                                             index, entry_tmp));
            return soc_mem_delete(unit, mem, copyno, entry_tmp);
        case L3_DEFIPm:
        default:
            return (SOC_E_UNAVAIL);
        }
    }
#endif /* BCM_FIREBOLT_SUPPORT */

    if (copyno != COPYNO_ALL){
        assert(SOC_MEM_BLOCK_VALID(unit, mem, copyno));
        MEM_LOCK(unit, mem);
        rv = _soc_mem_delete_index(unit, mem, copyno, index);
    }

    MEM_UNLOCK(unit, mem);

    return rv;
}

/*
 * Function:
 *    soc_mem_delete
 * Purpose:
 *    Delete an entry from a sorted table based on key,
 *    moving up all successive entries.
 * Returns:
 *    0 on success (whether or not entry was found),
 *    SOC_E_XXX on read or write error.
 * Notes:
 *    A binary search for the key is performed.  If the key is found,
 *    the entry is deleted by moving up successive entries until
 *    the last entry is moved up.  The previously last entry is
 *    overwritten with the null key.
 *
 *    If COPYNO_ALL is specified, the deletion is repeated for each copy
 *    of the table.
 */

int
soc_mem_delete(int unit,
               soc_mem_t mem,
               int copyno,
               void *key_data)
{
    int            index, rv;
    uint32        entry_tmp[SOC_MAX_MEM_WORDS];

    assert(soc_mem_is_sorted(unit, mem) ||
           soc_mem_is_hashed(unit, mem) ||
           soc_mem_is_cam(unit, mem) ||
           soc_mem_is_cmd(unit, mem));
    assert(key_data);

#ifdef BCM_TRX_SUPPORT
    if (soc_feature(unit, soc_feature_generic_table_ops)) {
        int32 banks = 0;
        _SOC_MEM_REPLACE_MEM(unit, mem);
        switch (mem) {
        case L2Xm:
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
#ifdef BCM_ISM_SUPPORT
        case L2_ENTRY_1m:
        case L2_ENTRY_2m:
        case L3_ENTRY_1m:
        case L3_ENTRY_2m:
        case L3_ENTRY_4m:
        case VLAN_XLATE_EXTDm:
        case MPLS_ENTRY_EXTDm:
#endif /* BCM_ISM_SUPPORT */
        case EGR_VLAN_XLATEm:
        case VLAN_XLATEm:
        case MPLS_ENTRYm:
        /* passthru */
        /* coverity[fallthrough: FALSE] */
#ifdef BCM_TRIUMPH3_SUPPORT
        case AXP_WRX_WCDm:
        case AXP_WRX_SVP_ASSIGNMENTm:
        case EXT_L2_ENTRY_1m:
        case EXT_L2_ENTRY_2m:
        case FT_SESSIONm:
        case FT_SESSION_IPV6m:
#endif /* BCM_TRIUMPH3_SUPPORT */
        case VLAN_MACm:
#ifdef BCM_TRIDENT2_SUPPORT
        case ING_VP_VLAN_MEMBERSHIPm:
        case EGR_VP_VLAN_MEMBERSHIPm:
        case ING_DNAT_ADDRESS_TYPEm:
        case L2_ENDPOINT_IDm:
        case ENDPOINT_QUEUE_MAPm:
#endif /* BCM_TRIDENT2_SUPPORT */
#if defined(BCM_KATANA2_SUPPORT)
        case EGR_MP_GROUPm:
#endif
#ifdef BCM_SABER2_SUPPORT
        case MP_GROUPm:
#endif
        case EXT_L2_ENTRYm:
            if (soc_feature(unit, soc_feature_ism_memory) && soc_mem_is_mview(unit, mem)) {
                banks = -1;
            }
            return soc_mem_generic_delete(unit, mem, MEM_BLOCK_ANY,
                                          banks, key_data, NULL, 0);
        default:
            break;
        }
    }
#endif /* BCM_TRX_SUPPORT */

#ifdef BCM_FIREBOLT_SUPPORT
    if (SOC_IS_FBX(unit)) {
        switch (mem) {
        case L2Xm:
            return soc_fb_l2x_delete(unit, key_data);
#if defined(INCLUDE_L3)
        case L3_DEFIPm:
            return soc_fb_lpm_delete(unit, key_data);
        case L3_ENTRY_IPV4_UNICASTm:
        case L3_ENTRY_IPV4_MULTICASTm:
        case L3_ENTRY_IPV6_UNICASTm:
        case L3_ENTRY_IPV6_MULTICASTm:
            return soc_fb_l3x_delete(unit, key_data);
#endif
        case VLAN_MACm:
            return soc_fb_vlanmac_entry_del(unit, key_data);
        default:
            break;
        }
    }
#endif /* BCM_FIREBOLT_SUPPORT */

    rv = SOC_E_NONE;
    if (copyno == COPYNO_ALL) {
        MEM_LOCK(unit, mem);
        SOC_MEM_BLOCK_ITER(unit, mem, copyno) {
            if ((rv = soc_mem_search(unit, mem, copyno,
                                     &index, key_data, entry_tmp, 0)) < 0) {
                break;
            }
            if ((rv = soc_mem_delete_index(unit, mem, copyno, index)) < 0) {
                break;
            }
        }
    } else {
        assert(SOC_MEM_BLOCK_VALID(unit, mem, copyno));
        MEM_LOCK(unit, mem);
        rv = soc_mem_search(unit, mem, copyno, &index, key_data, entry_tmp, 0);
        if (rv >= 0) {
            rv = soc_mem_delete_index(unit, mem, copyno, index);
        }
    }

    MEM_UNLOCK(unit, mem);

    return rv;
}

/*
 * Function:
 *      soc_mem_delete_return_old
 * Purpose:
 *      Same as soc_mem_delete() with the additional feature
 *      that if an exiting entry was deleted, the old entry data
 *      is returned back.
 * Returns:
 *      SOC_E_NONE - delete succeeded
 *      SOC_E_UNAVAIL - not supported on the specified unit, mem
 *      SOC_E_XXX - other error
 */
int
soc_mem_delete_return_old(int unit,
                          soc_mem_t mem,
                          int copyno,
                          void *key_data,
                          void *old_entry_data)
{
#ifdef BCM_TRX_SUPPORT
    if (SOC_IS_TRX(unit)) {
        int32 banks = 0;
        _SOC_MEM_REPLACE_MEM(unit, mem);
        switch (mem) {
        case MPLS_ENTRYm: /* Note: the order of the case statements should
                                   be changed with care */
            if (SOC_IS_SC_CQ(unit)) {
                break;
            }
        /* passthru */
        /* coverity[fallthrough: FALSE] */
        case L2Xm:
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
#ifdef BCM_ISM_SUPPORT
        case L2_ENTRY_1m:
        case L2_ENTRY_2m:
        case L3_ENTRY_1m:
        case L3_ENTRY_2m:
        case L3_ENTRY_4m:
        case VLAN_XLATE_EXTDm:
        case MPLS_ENTRY_EXTDm:
#endif /* BCM_ISM_SUPPORT */
        case EGR_VLAN_XLATEm:
        case VLAN_XLATEm:
        /* passthru */
        /* coverity[fallthrough: FALSE] */
#ifdef BCM_TRIUMPH3_SUPPORT
        case AXP_WRX_WCDm:
        case AXP_WRX_SVP_ASSIGNMENTm:
        case EXT_L2_ENTRY_1m:
        case EXT_L2_ENTRY_2m:
        case FT_SESSIONm:
        case FT_SESSION_IPV6m:
#endif /* BCM_TRIUMPH3_SUPPORT */
        case VLAN_MACm:
#ifdef BCM_TRIDENT2_SUPPORT
        case ING_VP_VLAN_MEMBERSHIPm:
        case EGR_VP_VLAN_MEMBERSHIPm:
        case ING_DNAT_ADDRESS_TYPEm:
        case L2_ENDPOINT_IDm:
        case ENDPOINT_QUEUE_MAPm:
#endif /* BCM_TRIDENT2_SUPPORT */
#ifdef BCM_TRIDENT2PLUS_SUPPORT
            if (soc_feature(unit, soc_feature_td2p_a0_sw_war) &&
                ((mem == EGR_VLAN_XLATEm) || (mem == VLAN_XLATEm))) {
                int rv, index;
                rv = soc_mem_generic_lookup(unit, mem, copyno, banks, key_data,
                                            old_entry_data, &index);
                if (rv == SOC_E_NONE) {
                    rv = soc_mem_write(unit, mem, copyno, index,
                                       soc_mem_entry_null(unit, mem));
                }
                return rv;
            }
#endif /* BCM_TRIDENT2PLUS_SUPPORT */
            if (soc_feature(unit, soc_feature_ism_memory) && soc_mem_is_mview(unit, mem)) {
                banks = -1;
            }
            return soc_mem_generic_delete(unit, mem, MEM_BLOCK_ANY,
                                          banks, key_data, old_entry_data, 0);
        }
    }
#endif
    return SOC_E_UNAVAIL;

}

#if defined(BCM_TRX_SUPPORT)
/*
 * Function:
 *    _soc_mem_pop
 * Purpose:
 *    Helper function for soc_mem_pop.
 */
STATIC int
_soc_mem_pop(int unit, soc_mem_t mem, int copyno, void *entry_data)
{
    schan_msg_t schan_msg;
    int         rv, entry_dw, allow_intr=0;
    uint8       at;
    int         src_blk, dst_blk, acc_type;
    int         opcode, nack;

    assert(SOC_MEM_IS_VALID(unit, mem));
    assert(soc_attached(unit));

    if (copyno == MEM_BLOCK_ANY) {
        /* Overrun of "mem_block_any" is not possible as it is of MAX "mem" value NUM_SOC_MEM */
        /* coverity[overrun-local : FALSE] */
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }
    if (!SOC_MEM_BLOCK_VALID(unit, mem, copyno)) {
        LOG_WARN(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "soc_mem_pop: invalid block %d for memory %s\n"),
                  copyno, SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    entry_dw = soc_mem_entry_words(unit, mem);

    schan_msg_clear(&schan_msg);
    acc_type = SOC_MEM_ACC_TYPE(unit, mem);
    src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
    dst_blk = SOC_BLOCK2SCH(unit, copyno);

    schan_msg.gencmd.address = soc_mem_addr_get(unit, mem, 0, copyno, 0, &at);

#ifdef BCM_SIRIUS_SUPPORT
    if (SOC_IS_SIRIUS(unit)) {
        src_blk = 0;
    }
    if (SOC_IS_SIRIUS(unit) && (!SAL_BOOT_PLISIM || SAL_BOOT_BCMSIM)) {
    /* mask off the block field */
    schan_msg.popcmd.address &= 0x3F0FFFFF;
    }
#endif

    soc_schan_header_cmd_set(unit, &schan_msg.header, FIFO_POP_CMD_MSG,
                             dst_blk, src_blk, acc_type, 0, 0, 0);

    if (SOC_IS_SAND(unit)) {
        allow_intr = 1;
    }

    /*
     * Execute S-Channel "fifo pop" command packet consisting of
     * (header word + address word), and read back
     * (header word + entry_dw) data words.
     */

    rv = soc_schan_op(unit, &schan_msg, 2, entry_dw + 1, allow_intr);

    /* Check result */
    soc_schan_header_status_get(unit, &schan_msg.header, &opcode, NULL, NULL,
                                NULL, NULL, &nack);
    if (opcode != FIFO_POP_DONE_MSG) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_pop: "
                              "invalid S-Channel reply, expected FIFO_POP_DONE_MSG:\n")));
        soc_schan_dump(unit, &schan_msg, entry_dw + 2);
        return SOC_E_INTERNAL;
    }

    if (rv == SOC_E_NONE) {
        if (nack != 0) {
            rv = SOC_E_NOT_FOUND;
        } else {
            sal_memcpy(entry_data, schan_msg.popresp.data, entry_dw * 4);
        }
    }

    if (bsl_check(bslLayerSoc, bslSourceSocmem, bslSeverityNormal, unit)) {
        LOG_INFO(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "Fifo pop: ")));
        soc_mem_entry_dump(unit, mem, entry_data);
    }

    return rv;
}


/*
 * Function:
 *    _soc_mem_push
 * Purpose:
 *    Helper function for soc_mem_push.
 */

STATIC int
_soc_mem_push(int unit, soc_mem_t mem, int copyno, void *entry_data)
{
    schan_msg_t schan_msg;
    int         rv, entry_dw, allow_intr=0;
    uint8       at;
    int         src_blk, dst_blk, acc_type, data_byte_len;
    int         opcode, nack;

    assert(SOC_MEM_IS_VALID(unit, mem));
    assert(soc_attached(unit));

    if (copyno == MEM_BLOCK_ANY) {
        /* Overrun of "mem_block_any" is not possible as it is of MAX "mem" value NUM_SOC_MEM */
        /* coverity[overrun-local : FALSE] */
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }
    if (!SOC_MEM_BLOCK_VALID(unit, mem, copyno)) {
        LOG_WARN(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "soc_mem_push: invalid block %d for memory %s\n"),
                  copyno, SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    entry_dw = soc_mem_entry_words(unit, mem);

    schan_msg_clear(&schan_msg);
    acc_type = SOC_MEM_ACC_TYPE(unit, mem);
    src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
    dst_blk = SOC_BLOCK2SCH(unit, copyno);
    data_byte_len = entry_dw * 4;

    schan_msg.gencmd.address = soc_mem_addr_get(unit, mem, 0, copyno, 0, &at);

    /* Fill in packet data */
    sal_memcpy(schan_msg.pushcmd.data, entry_data, data_byte_len);

    soc_schan_header_cmd_set(unit, &schan_msg.header, FIFO_PUSH_CMD_MSG,
                         dst_blk, src_blk, acc_type, 0, 0, 0);

    if(SOC_IS_SAND(unit)) {
        allow_intr = 1;
    }

    /*
     * Execute S-Channel "fifo push" command packet consisting of
     * (header word + address word + entry_dw), and read back
     * (header word + entry_dw) data words.
     */

    rv = soc_schan_op(unit, &schan_msg, entry_dw + 2, entry_dw + 1, allow_intr);

    /* Check result */
    soc_schan_header_status_get(unit, &schan_msg.header, &opcode, NULL, NULL,
                                NULL, NULL, &nack);
    if (opcode != FIFO_PUSH_DONE_MSG) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_push: "
                              "invalid S-Channel reply, expected FIFO_PUSH_DONE_MSG:\n")));
        soc_schan_dump(unit, &schan_msg, entry_dw + 2);
        return SOC_E_INTERNAL;
    }

    if ((rv == SOC_E_NONE) && (nack != 0)) {
        rv = SOC_E_FULL;
    }

    if (bsl_check(bslLayerSoc, bslSourceSocmem, bslSeverityNormal, unit)) {
        LOG_INFO(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "Fifo push: ")));
        soc_mem_entry_dump(unit, mem, entry_data);
    }

    return rv;
}

#endif /* defined(BCM_TRX_SUPPORT)  */

#if defined(BCM_TRX_SUPPORT) || defined(BCM_SIRIUS_SUPPORT)

static const struct {
    soc_reg_t   cfg;
    soc_reg_t   sbus_addr;
    soc_reg_t   hostmem_addr;
    soc_reg_t   read_ptr;
    soc_reg_t   write_ptr;
    soc_reg_t   threshold;
    soc_field_t overflow_fld;
    int         overflow_bit;
} _soc_mem_fifo_dma[] = {
    {
        CMIC_FIFO_CH0_RD_DMA_CFGr,
        CMIC_FIFO_CH0_RD_DMA_SBUS_START_ADDRESSr,
        CMIC_FIFO_CH0_RD_DMA_HOSTMEM_START_ADDRESSr,
        CMIC_FIFO_CH0_RD_DMA_HOSTMEM_READ_PTRr,
        CMIC_FIFO_CH0_RD_DMA_HOSTMEM_WRITE_PTRr,
        CMIC_FIFO_CH0_RD_DMA_HOSTMEM_THRESHOLDr,
        FIFO_CH0_DMA_HOSTMEM_OVERFLOWf,
        0,
    },
    {
        CMIC_FIFO_CH1_RD_DMA_CFGr,
        CMIC_FIFO_CH1_RD_DMA_SBUS_START_ADDRESSr,
        CMIC_FIFO_CH1_RD_DMA_HOSTMEM_START_ADDRESSr,
        CMIC_FIFO_CH1_RD_DMA_HOSTMEM_READ_PTRr,
        CMIC_FIFO_CH1_RD_DMA_HOSTMEM_WRITE_PTRr,
        CMIC_FIFO_CH1_RD_DMA_HOSTMEM_THRESHOLDr,
        FIFO_CH1_DMA_HOSTMEM_OVERFLOWf,
        2,
    },
    {
        CMIC_FIFO_CH2_RD_DMA_CFGr,
        CMIC_FIFO_CH2_RD_DMA_SBUS_START_ADDRESSr,
        CMIC_FIFO_CH2_RD_DMA_HOSTMEM_START_ADDRESSr,
        CMIC_FIFO_CH2_RD_DMA_HOSTMEM_READ_PTRr,
        CMIC_FIFO_CH2_RD_DMA_HOSTMEM_WRITE_PTRr,
        CMIC_FIFO_CH2_RD_DMA_HOSTMEM_THRESHOLDr,
        FIFO_CH2_DMA_HOSTMEM_OVERFLOWf,
        4,
    },
    {
        CMIC_FIFO_CH3_RD_DMA_CFGr,
        CMIC_FIFO_CH3_RD_DMA_SBUS_START_ADDRESSr,
        CMIC_FIFO_CH3_RD_DMA_HOSTMEM_START_ADDRESSr,
        CMIC_FIFO_CH3_RD_DMA_HOSTMEM_READ_PTRr,
        CMIC_FIFO_CH3_RD_DMA_HOSTMEM_WRITE_PTRr,
        CMIC_FIFO_CH3_RD_DMA_HOSTMEM_THRESHOLDr,
        FIFO_CH3_DMA_HOSTMEM_OVERFLOWf,
        6,
    },
};
STATIC int
_soc_mem_fifo_dma_start(int unit, int chan, soc_mem_t mem, int copyno,
                        int host_entries, void *host_buf)
{
    soc_control_t  *soc = SOC_CONTROL(unit);
    soc_reg_t cfg_reg;
    uint32 addr, rval, data_beats, sel, spacing;
    uint8 at;

    if (chan < 0 || chan > 3 || host_buf == NULL) {
        return SOC_E_PARAM;
    }

    cfg_reg = _soc_mem_fifo_dma[chan].cfg;

    switch (host_entries) {
    case 64:    sel = 0; break;
    case 128:   sel = 1; break;
    case 256:   sel = 2; break;
    case 512:   sel = 3; break;
    case 1024:  sel = 4; break;
    case 2048:  sel = 5; break;
    case 4096:  sel = 6; break;
    case 8192:  sel = 7; break;
    case 16384: sel = 8; break;
    case 32768: sel = 9; break;
    case 65536: sel = 10; break;
    default:
        return SOC_E_PARAM;
    }

    if (mem != ING_IPFIX_EXPORT_FIFOm && mem != EGR_IPFIX_EXPORT_FIFOm &&
        mem != EXT_L2_MOD_FIFOm && mem != L2_MOD_FIFOm &&
        mem != CS_EJECTION_MESSAGE_TABLEm) {
        return SOC_E_BADID;
    }

    if (copyno == MEM_BLOCK_ANY) {
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }

    data_beats = soc_mem_entry_words(unit, mem);

    addr = soc_reg_addr(unit, _soc_mem_fifo_dma[chan].sbus_addr, REG_PORT_ANY,
                        0);
    rval = soc_mem_addr_get(unit, mem, 0, copyno, 0, &at);
    soc_pci_write(unit, addr, rval);

    addr = soc_reg_addr(unit, _soc_mem_fifo_dma[chan].hostmem_addr,
                        REG_PORT_ANY, 0);
    rval = soc_cm_l2p(unit, host_buf);
    soc_pci_write(unit, addr, rval);

    addr = soc_reg_addr(unit, _soc_mem_fifo_dma[chan].read_ptr, REG_PORT_ANY,
                        0);
    soc_pci_write(unit, addr, rval);

    addr = soc_reg_addr(unit, _soc_mem_fifo_dma[chan].threshold, REG_PORT_ANY,
                        0);
    rval = 0;
    soc_reg_field_set(unit, _soc_mem_fifo_dma[chan].threshold, &rval, ADDRESSf,
                      host_entries / 16 * data_beats * sizeof(uint32));
    soc_pci_write(unit, addr, rval);

    addr = soc_reg_addr(unit, cfg_reg, REG_PORT_ANY, 0);
    rval = 0;
    soc_reg_field_set(unit, cfg_reg, &rval, BEAT_COUNTf, data_beats);
    soc_reg_field_set(unit, cfg_reg, &rval, HOST_NUM_ENTRIES_SELf, sel);
    soc_reg_field_set(unit, cfg_reg, &rval, TIMEOUT_COUNTf, 200);
    if (soc_feature(unit, soc_feature_multi_sbus_cmds)) {
        
        if (soc->sbusCmdSpacing < 0) {
            spacing = data_beats > 7 ? data_beats + 1 : 8;
        } else {
            spacing = soc->sbusCmdSpacing;
        }
        if ((SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_ESM) ||
            (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_XQPORT) ||
            (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_GXPORT) ||
            (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_SPORT) ||
            (SOC_BLOCK_TYPE(unit, copyno) == SOC_BLK_GPORT)) {
            spacing = 0;
        }
        if (spacing) {
            soc_reg_field_set(unit, cfg_reg, &rval,
                              MULTIPLE_SBUS_CMD_SPACINGf, spacing);
            soc_reg_field_set(unit, cfg_reg, &rval,
                              ENABLE_MULTIPLE_SBUS_CMDSf, 1);
        }
    }
    soc_pci_write(unit, addr, rval);

    soc_reg_field_set(unit, cfg_reg, &rval, ENABLEf, 1);
    soc_reg_field_set(unit, cfg_reg, &rval, ENABLE_VALf, 1);
    soc_pci_write(unit, addr, rval);

    return SOC_E_NONE;
}

STATIC int
_soc_mem_fifo_dma_stop(int unit, int chan)
{
    uint32 addr, rval;

    if (chan < 0 || chan > 3) {
        return SOC_E_PARAM;
    }

    addr = soc_reg_addr(unit, _soc_mem_fifo_dma[chan].cfg, REG_PORT_ANY, 0);
    rval = 0;
    soc_reg_field_set(unit, _soc_mem_fifo_dma[chan].cfg, &rval, ENABLEf, 1);
    soc_pci_write(unit, addr, rval);

    return SOC_E_NONE;
}

STATIC int
_soc_mem_fifo_dma_get_read_ptr(int unit, int chan, void **host_ptr, int *count)
{
    soc_reg_t cfg_reg;
    int host_entries, data_beats;
    uint32 addr, rval, debug, hostmem_addr, read_ptr, write_ptr;

    if (chan < 0 || chan > 3 || host_ptr == NULL) {
        return SOC_E_PARAM;
    }

    /* read CFG reg first so that write pointer is valid later on */
    cfg_reg = _soc_mem_fifo_dma[chan].cfg;
    addr = soc_reg_addr(unit, cfg_reg, REG_PORT_ANY, 0);
    soc_pci_getreg(unit, addr, &rval);

    addr = soc_reg_addr(unit, _soc_mem_fifo_dma[chan].read_ptr, REG_PORT_ANY,
                        0);
    soc_pci_getreg(unit, addr, &read_ptr);

    addr = soc_reg_addr(unit, _soc_mem_fifo_dma[chan].write_ptr, REG_PORT_ANY,
                        0);
    soc_pci_getreg(unit, addr, &write_ptr);

    if (write_ptr == 0 ||
        (soc_feature(unit, soc_feature_fifo_dma_active) &&
         !soc_reg_field_get(unit, cfg_reg, rval, ACTIVEf))) {
        return SOC_E_EMPTY;
    }

    if (read_ptr == write_ptr) {
        addr = soc_reg_addr(unit, CMIC_FIFO_RD_DMA_DEBUGr, REG_PORT_ANY, 0);
        soc_pci_getreg(unit, addr, &debug);
        if (!soc_reg_field_get(unit, CMIC_FIFO_RD_DMA_DEBUGr,
                               debug, _soc_mem_fifo_dma[chan].overflow_fld)) {
            return SOC_E_EMPTY;
        }
        /* Re-read write pointer */
        addr = soc_reg_addr(unit, _soc_mem_fifo_dma[chan].write_ptr,
                            REG_PORT_ANY, 0);
        soc_pci_getreg(unit, addr, &write_ptr);
    }

    addr = soc_reg_addr(unit, _soc_mem_fifo_dma[chan].hostmem_addr,
                        REG_PORT_ANY, 0);
    soc_pci_getreg(unit, addr, &hostmem_addr);

    data_beats = soc_reg_field_get(unit, cfg_reg, rval, BEAT_COUNTf);
    if (data_beats <= 0) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "Invalid BEAT_COUNT (%d) in "
                              "CMIC_FIFO_CH%d_RD_DMA_CFG \n"), data_beats, chan));
        return SOC_E_CONFIG;
    }

    switch (soc_reg_field_get(unit, cfg_reg, rval, HOST_NUM_ENTRIES_SELf)) {
    case 0:  host_entries = 64;    break;
    case 1:  host_entries = 128;   break;
    case 2:  host_entries = 256;   break;
    case 3:  host_entries = 512;   break;
    case 4:  host_entries = 1024;  break;
    case 5:  host_entries = 2048;  break;
    case 6:  host_entries = 4096;  break;
    case 7:  host_entries = 8192;  break;
    case 8:  host_entries = 16384; break;
    case 9:  host_entries = 32768; break;
    case 10: host_entries = 65536; break;
    default: return SOC_E_CONFIG;
    }

    *host_ptr = soc_cm_p2l(unit, read_ptr);
    if (read_ptr >= write_ptr) {
        *count = host_entries -
            (read_ptr - hostmem_addr) / data_beats / sizeof(uint32);
    } else {
        *count = (write_ptr - read_ptr) / data_beats / sizeof(uint32);
    }

    if (SAL_BOOT_QUICKTURN) {
        /* Delay to ensure PCI DMA tranfer to host memory completes */
        sal_usleep(soc_mem_fifo_delay_value);
    }
    return (*count) ? SOC_E_NONE : SOC_E_EMPTY;
}

STATIC int
_soc_mem_fifo_dma_advance_read_ptr(int unit, int chan, int count)
{
    soc_reg_t cfg_reg;
    int host_entries, data_beats;
    uint32 addr, rval, debug;
    uint32 *host_buf, *read_ptr;

    if (chan < 0 || chan > 3) {
        return SOC_E_PARAM;
    }

    cfg_reg = _soc_mem_fifo_dma[chan].cfg;

    addr = soc_reg_addr(unit, cfg_reg, REG_PORT_ANY, 0);
    soc_pci_getreg(unit, addr, &rval);
    data_beats = soc_reg_field_get(unit, cfg_reg, rval, BEAT_COUNTf);

    switch (soc_reg_field_get(unit, cfg_reg, rval, HOST_NUM_ENTRIES_SELf)) {
    case 0:  host_entries = 64;    break;
    case 1:  host_entries = 128;   break;
    case 2:  host_entries = 256;   break;
    case 3:  host_entries = 512;   break;
    case 4:  host_entries = 1024;  break;
    case 5:  host_entries = 2048;  break;
    case 6:  host_entries = 4096;  break;
    case 7:  host_entries = 8192;  break;
    case 8:  host_entries = 16384; break;
    case 9:  host_entries = 32768; break;
    case 10: host_entries = 65536; break;
    default: return SOC_E_CONFIG;
    }

    if (count < 0 || count >= host_entries) {
        return SOC_E_PARAM;
    }

    /* Clear threshold overflow bit */
    addr = soc_reg_addr(unit, CMIC_FIFO_RD_DMA_DEBUGr, REG_PORT_ANY, 0);
    debug = 0;
    soc_reg_field_set(unit, CMIC_FIFO_RD_DMA_DEBUGr,
                      &debug, BIT_POSf, _soc_mem_fifo_dma[chan].overflow_bit);
    soc_pci_write(unit, addr, debug);

    addr = soc_reg_addr(unit, _soc_mem_fifo_dma[chan].hostmem_addr,
                        REG_PORT_ANY, 0);
    soc_pci_getreg(unit, addr, &rval);
    host_buf = soc_cm_p2l(unit, rval);

    addr = soc_reg_addr(unit, _soc_mem_fifo_dma[chan].read_ptr, REG_PORT_ANY,
                        0);
    soc_pci_getreg(unit, addr, &rval);
    read_ptr = soc_cm_p2l(unit, rval);

    read_ptr += count * data_beats;
    if (read_ptr >= &host_buf[host_entries * data_beats]) {
        read_ptr -= host_entries * data_beats;
    }
    rval = soc_cm_l2p(unit, read_ptr);
    soc_pci_write(unit, addr, rval);

    return SOC_E_NONE;
}

#endif /* BCM_TRX_SUPPORT || BCM_SIRIUS_SUPPORT*/

/*
 * Function:
 *      soc_mem_pop
 * Purpose:
 *      Pop an entry from a FIFO.
 * Parameters:
 *      mem               Memory to search
 *      copyno               Which copy to search (if multiple copies)
 *      entry_data              OUT: Data if found
 *                           CAN NO LONGER BE NULL.  Must be big enough
 *                           to handle the appropriate data.
 * Returns:
 *    SOC_E_NOT_FOUND      Entry not found: Fifo is empty
 *    SOC_E_NONE         Entry is found
 *    SOC_E_XXX         If internal error occurs
 */

int
soc_mem_pop(int unit,
            soc_mem_t mem,
            int copyno,
            void *entry_data)
{
#ifdef BCM_TRX_SUPPORT
    if (soc_feature(unit, soc_feature_mem_push_pop)) {
        switch (mem) {
        case ING_IPFIX_EXPORT_FIFOm:
        case EGR_IPFIX_EXPORT_FIFOm:
        case EXT_L2_MOD_FIFOm:
        case L2_MOD_FIFOm:
#ifdef BCM_TRIUMPH3_SUPPORT
        case ING_SER_FIFOm:
        case ING_SER_FIFO_Xm:
        case ING_SER_FIFO_Ym:
        case EGR_SER_FIFOm:
        case EGR_SER_FIFO_Xm:
        case EGR_SER_FIFO_Ym:
        case ISM_SER_FIFOm:
#endif /* BCM_TRIUMPH3_SUPPORT */
#ifdef BCM_TOMAHAWK_SUPPORT
        case ING_SER_FIFO_PIPE0m:
        case ING_SER_FIFO_PIPE1m:
        case ING_SER_FIFO_PIPE2m:
        case ING_SER_FIFO_PIPE3m:
        case EGR_SER_FIFO_PIPE0m:
        case EGR_SER_FIFO_PIPE1m:
        case EGR_SER_FIFO_PIPE2m:
        case EGR_SER_FIFO_PIPE3m:
        case L2_MGMT_SER_FIFOm:
        case MMU_GCFG_MEM_FAIL_ADDR_64m:
        case MMU_XCFG_MEM_FAIL_ADDR_64m:
        case MMU_XCFG_MEM_FAIL_ADDR_64_XPE0m:
        case MMU_XCFG_MEM_FAIL_ADDR_64_XPE1m:
        case MMU_XCFG_MEM_FAIL_ADDR_64_XPE2m:
        case MMU_XCFG_MEM_FAIL_ADDR_64_XPE3m:
        case MMU_SCFG_MEM_FAIL_ADDR_64m:
        case MMU_SCFG_MEM_FAIL_ADDR_64_SC0m:
        case MMU_SCFG_MEM_FAIL_ADDR_64_SC1m:
        case CENTRAL_CTR_EVICTION_FIFOm:
#endif /* BCM_TOMAHAWK_SUPPORT */
#ifdef BCM_TOMAHAWK2_SUPPORT
        case MMU_SEDCFG_MEM_FAIL_ADDR_64m:
        case MMU_SEDCFG_MEM_FAIL_ADDR_64_SC0m:
        case MMU_SEDCFG_MEM_FAIL_ADDR_64_SC1m:
#endif /* BCM_TOMAHAWK2_SUPPORT */
            return _soc_mem_pop(unit, mem, copyno, entry_data);
        default:
            return SOC_E_BADID;
            break;
        }
    }
#endif /* BCM_TRX_SUPPORT */

#ifdef BCM_SIRIUS_SUPPORT
    
    if (soc_feature(unit, soc_feature_mem_push_pop)) {
        switch (mem) {
        /* return _soc_mem_pop(unit, mem, copyno, entry_data); */
            default:
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_pop: implement mem POP\n")));
                return SOC_E_BADID;
                break;
        }
    }
#endif /* BCM_SIRIUS_SUPPORT */

    return SOC_E_UNAVAIL;
}

/*
 * Function:
 *      soc_mem_push
 * Purpose:
 *      Push an entry to a FIFO.
 * Parameters:
 *      mem                  Memory to search
 *      copyno               Which copy to search (if multiple copies)
 *      entry_data           In: Data to be pushed
 *
 * Returns:
 *      SOC_E_FULL           Fifo is full
 *      SOC_E_NONE           Entry is pushed successfully
 *      SOC_E_XXX            If internal error occurs
 */

int
soc_mem_push(int unit,
             soc_mem_t mem,
             int copyno,
             void *entry_data)
{
#ifdef BCM_SIRIUS_SUPPORT
    if (SOC_IS_SIRIUS(unit)) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_push: not supported on %s\n"),
                   SOC_CHIP_STRING(unit)));
    return SOC_E_UNAVAIL;
    }
#endif

#ifdef BCM_TRX_SUPPORT
    if (soc_feature(unit, soc_feature_mem_push_pop)) {
        switch (mem) {
        case ING_IPFIX_EXPORT_FIFOm:
        case EGR_IPFIX_EXPORT_FIFOm:
        case EXT_L2_MOD_FIFOm:
        case L2_MOD_FIFOm:
            return _soc_mem_push(unit, mem, copyno, entry_data);
        default:
            return SOC_E_BADID;
            break;
        }
    }
#endif /* BCM_TRX_SUPPORT */

    return SOC_E_UNAVAIL;
}

int
soc_mem_fifo_dma_start(int unit, int chan, soc_mem_t mem, int copyno,
                       int host_entries, void *host_buf)
{
#if defined(BCM_KATANA_SUPPORT) && defined(BCM_CMICM_SUPPORT)
    if (SOC_IS_KATANA(unit) && soc_feature(unit, soc_feature_cmicm)) {
        return _soc_mem_kt_fifo_dma_start(unit, chan, mem, copyno,
                                          host_entries, host_buf);
    }
#endif
#ifdef BCM_CMICM_SUPPORT
    if (soc_feature(unit, soc_feature_cmicm)) {
        return _soc_mem_sbus_fifo_dma_start(unit, chan, mem, copyno,
                                            host_entries, host_buf);
    }
#endif /* BCM_CMICM_SUPPORT */
#if defined(BCM_TRX_SUPPORT) || defined(BCM_SIRIUS_SUPPORT)
    if (soc_feature(unit, soc_feature_fifo_dma)) {
        return _soc_mem_fifo_dma_start(unit, chan, mem, copyno,
                                       host_entries, host_buf);
    }
#endif /* BCM_TRX_SUPPORT || BCM_SIRIUS_SUPPORT */

    return SOC_E_UNAVAIL;
}

int
soc_mem_fifo_dma_stop(int unit, int chan)
{
#if defined(BCM_KATANA_SUPPORT) && defined(BCM_CMICM_SUPPORT)
    if (SOC_IS_KATANA(unit) && soc_feature(unit, soc_feature_cmicm)) {
        return _soc_mem_kt_fifo_dma_stop(unit, chan);
    }
#endif
#ifdef BCM_CMICM_SUPPORT
    if (soc_feature(unit, soc_feature_cmicm)) {
        return _soc_mem_sbus_fifo_dma_stop(unit, chan);
    }
#endif /* BCM_CMICM_SUPPORT */
#if defined(BCM_TRX_SUPPORT) || defined(BCM_SIRIUS_SUPPORT)
    if (soc_feature(unit, soc_feature_fifo_dma)) {
        return _soc_mem_fifo_dma_stop(unit, chan);
    }
#endif /* BCM_TRX_SUPPORT || BCM_SIRIUS_SUPPORT */

    return SOC_E_UNAVAIL;
}

int
soc_mem_fifo_dma_get_read_ptr(int unit, int chan, void **host_ptr, int *count)
{

#if defined(BCM_HURRICANE2_SUPPORT) && defined(BCM_CMICM_SUPPORT)
    if (soc_feature(unit, soc_feature_cmicm) &&
            soc_feature(unit, soc_feature_fifo_dma_hu2)) {
        return _soc_mem_hu2_fifo_dma_get_read_ptr(unit, chan, host_ptr, count);
    }
#endif
#if defined(BCM_KATANA_SUPPORT) && defined(BCM_CMICM_SUPPORT)
    if (soc_feature(unit, soc_feature_cmicm)) {
        return _soc_mem_kt_fifo_dma_get_read_ptr(unit, chan, host_ptr, count);
    }
#endif
#if defined(BCM_TRX_SUPPORT) || defined(BCM_SIRIUS_SUPPORT)
    if (soc_feature(unit, soc_feature_fifo_dma)) {
        return _soc_mem_fifo_dma_get_read_ptr(unit, chan, host_ptr, count);
    }
#endif /* BCM_TRX_SUPPORT || BCM_SIRIUS_SUPPORT */

    return SOC_E_UNAVAIL;
}

int
soc_mem_fifo_dma_advance_read_ptr(int unit, int chan, int count)
{

#if defined(BCM_HURRICANE2_SUPPORT) && defined(BCM_CMICM_SUPPORT)
    if (soc_feature(unit, soc_feature_cmicm) && SOC_IS_HURRICANE2(unit)) {
        return _soc_mem_hu2_fifo_dma_advance_read_ptr(unit, chan, count);
    }
#endif
#if defined(BCM_KATANA_SUPPORT) && defined(BCM_CMICM_SUPPORT)
    if (soc_feature(unit, soc_feature_cmicm)) {
        return _soc_mem_kt_fifo_dma_advance_read_ptr(unit, chan, count);
    }
#endif
#if defined(BCM_TRX_SUPPORT) || defined(BCM_SIRIUS_SUPPORT)
    if (soc_feature(unit, soc_feature_fifo_dma)) {
        return _soc_mem_fifo_dma_advance_read_ptr(unit, chan, count);
    }
#endif /* BCM_TRX_SUPPORT || BCM_SIRIUS_SUPPORT */
    return SOC_E_UNAVAIL;
}

int
soc_mem_fifo_dma_get_num_entries(int unit, int chan, int *count)
{
#ifdef BCM_CMICM_SUPPORT
    if (soc_feature(unit, soc_feature_cmicm)) {
        return _soc_mem_sbus_fifo_dma_get_num_entries(unit, chan, count);
    }
#endif /* BCM_CMICM_SUPPORT */
    return SOC_E_UNAVAIL;
}

int
soc_mem_fifo_dma_set_entries_read(int unit, int chan, uint32 num)
{
#ifdef BCM_CMICM_SUPPORT
    if (soc_feature(unit, soc_feature_cmicm)) {
        return _soc_mem_sbus_fifo_dma_set_entries_read(unit, chan, num);
    }
#endif /* BCM_CMICM_SUPPORT */
    return SOC_E_UNAVAIL;
}

#ifdef BCM_XGS_SWITCH_SUPPORT

/*
 * Function:
 *    soc_vlan_entries
 * Purpose:
 *    Return the number of entries in VLAN_TAB with the VALID bit set.
 */

STATIC int
soc_vlan_entries(int unit)
{
    int            index_min, index_max, index;
    vlan_tab_entry_t    ve;
    int            total;

    index_min = soc_mem_index_min(unit, VLAN_TABm);
    index_max = soc_mem_index_max(unit, VLAN_TABm);

    total = 0;

    for (index = index_min; index <= index_max; index++) {
        SOC_IF_ERROR_RETURN
            (soc_mem_read(unit, VLAN_TABm, MEM_BLOCK_ANY, index, &ve));

        total += soc_VLAN_TABm_field32_get(unit, &ve, VALIDf);
    }

    return total;
}

#endif /* BCM_XGS_SWITCH_SUPPORT */

/*
 * Function:
 *    soc_mem_entries
 * Purpose:
 *    Return the number of entries in a sorted table.
 * Notes:
 *    Performs special functions for some tables.
 */

int
soc_mem_entries(int unit,
                soc_mem_t mem,
                int copyno)
{
    int            entries = 0;
    assert(SOC_MEM_IS_VALID(unit, mem));
    assert(soc_attached(unit));
    assert(soc_mem_is_sorted(unit, mem) ||
           soc_mem_is_hashed(unit, mem) ||
           soc_mem_is_cam(unit, mem) ||
           soc_mem_is_cmd(unit, mem) ||
           mem == VLAN_TABm);
    if (copyno == MEM_BLOCK_ANY) {
        /* Overrun of "mem_block_any" is not possible as it is of MAX "mem" value NUM_SOC_MEM */
        /* coverity[overrun-local : FALSE] */
        copyno = SOC_MEM_BLOCK_ANY(unit, mem);
    }
    if (!SOC_MEM_BLOCK_VALID(unit, mem, copyno)) {
        LOG_WARN(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "soc_mem_entries: invalid block %d for memory %s\n"),
                  copyno, SOC_MEM_NAME(unit, mem)));
        return SOC_E_PARAM;
    }

    switch (mem) {
#ifdef BCM_XGS_SWITCH_SUPPORT
    case VLAN_TABm:
        entries = soc_vlan_entries(unit);    /* Warning: very slow */
        break;
#endif

    default:
        _SOC_MEM_REUSE_MEM_STATE(unit, mem);
        entries = SOP_MEM_STATE(unit, mem).count[copyno];
        break;
    }

    return entries;
}

/*
 * Function:
 *    soc_mem_debug_set
 * Purpose:
 *    Enable or disable MMU debug mode.
 * Returns:
 *    Previous enable state, or SOC_E_XXX on error.
 */

int
soc_mem_debug_set(int unit, int enable)
{
    schan_msg_t schan_msg;
    int    msg, rv = SOC_E_NONE, old_enable, allow_intr=0;
    int    dst_blk, src_blk;

    old_enable = SOC_PERSIST(unit)->debugMode;

    if (enable && !old_enable) {
        msg = ENTER_DEBUG_MODE_MSG;
    } else if (!enable && old_enable) {
        msg = EXIT_DEBUG_MODE_MSG;
    } else {
        msg = -1;
    }

    if (msg >= 0) {
        schan_msg_clear(&schan_msg);

        dst_blk = SOC_BLOCK2SCH(unit, MMU_BLOCK(unit));
#ifdef BCM_SIRIUS_SUPPORT
        if (SOC_IS_SIRIUS(unit)) {
            src_blk = 0;
        } else
#endif
        {
            src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
        }

        soc_schan_header_cmd_set(unit, &schan_msg.header, msg, dst_blk,
                                 src_blk, 0, 0, 0, 0);

        if (SOC_IS_SAND(unit)) {
            allow_intr = 1;
        }

        if ((rv = soc_schan_op(unit, &schan_msg, 1, 0, allow_intr)) >= 0) {
            SOC_PERSIST(unit)->debugMode = enable;
        }

        if (!enable) {
            /* Allow packet transfers in progress to drain */
            sal_usleep(100000);
        }
    }

    return (rv < 0) ? rv : old_enable;
}

/*
 * Function:
 *    soc_mem_debug_get
 * Purpose:
 *    Return current MMU debug mode status
 */

int
soc_mem_debug_get(int unit, int *enable)
{
    *enable = SOC_PERSIST(unit)->debugMode;
    return SOC_E_NONE;
}

/*
 * Function:
 *    soc_mem_iterate
 * Purpose:
 *    Iterate over all the valid memories and call user
 *    passed callback function (do_it) for each valid memory.
 */
int
soc_mem_iterate(int unit, soc_mem_iter_f do_it, void *data)
{
    soc_mem_t mem, mem_iter;
    int       rv = SOC_E_NONE;

    if (!do_it) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "soc_mem_iterate: Callback function is NULL")));
        return SOC_E_PARAM;
    }

    for (mem_iter = 0; mem_iter < NUM_SOC_MEM; mem_iter++) {
        mem = mem_iter;
        _SOC_MEM_REPLACE_MEM(unit, mem);
        if (!SOC_MEM_IS_VALID(unit, mem)) {
            continue;
        }

#if defined(BCM_HAWKEYE_SUPPORT)
        if (SOC_IS_HAWKEYE(unit) && (soc_mem_index_max(unit, mem) <= 0)) {
            continue;
        }
#endif /* BCM_HAWKEYE_SUPPORT */

        /*
         * Call user provided callback function.
         */
        if ((rv = do_it(unit, mem, data)) < 0) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "soc_mem_iterate: Failed on memory (%s)\n"),
                       SOC_MEM_NAME(unit, mem)));
           /* break; */
        }
    }
    return rv;
}

#ifdef BCM_CMICM_SUPPORT
/*
 * Function:   soc_host_ccm_copy
 * Purpose:    Copy a range of memory from one host location to another
 *             Primarily for Cross-Coupled Memories.
 * Parameters:
 *       unit      - (IN) SOC unit number.
 *       srcbuf   - (IN) Source Buffer.
 *       dstbuf   - (IN) Destination Buffer.
 *       count    - (IN) Number of dwords to transfer.
 *       flags    - (IN)
 *                  0x01  Source and destination endian to be changed.
 *                  0x02  Source is PCI
 * Returns:
 *       SOC_E_XXX
 */
int
soc_host_ccm_copy(int unit, void *srcbuf, void *dstbuf,
                  int count, int flags)
{
    int rv;

    rv = soc_host_ccm_copy_multi_cmc(unit, srcbuf, dstbuf,
                                     count, flags, SOC_PCI_CMC(unit));
    return rv;
}

/*
 * Function:   soc_host_ccm_copy_multi_cmc
 * Purpose:    Copy a range of memory from one host location to another
 *             Primarily for Cross-Coupled Memories.
 *             With multiple CMC/channel support
 * Parameters:
 *       unit      - (IN) SOC unit number.
 *       srcbuf   - (IN) Source Buffer.
 *       dstbuf   - (IN) Destination Buffer.
 *       count    - (IN) Number of dwords to transfer.
 *       flags    - (IN)
 *                  0x01  Source and destination endian to be changed.
 *                  0x02  Source is PCI
 *       cmc      - (IN) CMC to be used.
 * Returns:
 *       SOC_E_XXX
 */
int
soc_host_ccm_copy_multi_cmc(int unit, void *srcbuf, void *dstbuf,
                            int count, int flags, int cmc)
{
    int i, rv;
    soc_control_t *soc = SOC_CONTROL(unit);
    uint32 *srcptr = (uint32 *)srcbuf;
    uint32 *dstptr = (uint32 *)dstbuf;
    uint32 reg, reg2;
    assert (srcptr && dstptr);

    if (SOC_CONTROL(unit)->ccmDmaMutex[cmc] == 0) {
        /* If DMA not enabled, or short length use PIO to copy */
        for (i = 0; i < count; i ++ ) {
            if (flags & 2) {            /* Read from PCI */
                reg = soc_pci_mcs_read(unit, PTR_TO_INT(srcptr));
            } else {
                reg = *srcptr;
            }

            if (flags & 1) {
                reg = ((reg & 0xff000000) >> 24) |
                    ((reg & 0x00ff0000) >> 8) |
                    ((reg & 0x0000ff00) << 8) |
                    ((reg & 0x000000ff) << 24);
            }

            if (flags & 2) {
                *dstptr = reg;
            } else {
                soc_pci_mcs_write(unit, PTR_TO_INT(dstptr), reg);
                reg2 = soc_pci_mcs_read(unit, PTR_TO_INT(dstptr));
                if (reg2 != reg) {
                    LOG_ERROR(BSL_LS_SOC_SOCMEM,
                              (BSL_META_U(unit,
                                          "ccm_dma: compare error %x (%x %x)\n"),
                               PTR_TO_INT(dstptr), reg, reg2));
                }
            }

            dstptr++;
            srcptr++;
        }
        return SOC_E_NONE;
    }

    CCM_DMA_LOCK(unit, cmc);

    soc_pci_write(unit, CMIC_CMCx_CCM_DMA_HOST0_MEM_START_ADDR_OFFSET(cmc), soc_cm_l2p(unit, srcbuf));
    soc_pci_write(unit, CMIC_CMCx_CCM_DMA_HOST1_MEM_START_ADDR_OFFSET(cmc), soc_cm_l2p(unit, dstbuf));
    soc_pci_write(unit, CMIC_CMCx_CCM_DMA_ENTRY_COUNT_OFFSET(cmc), count);
    /* Keep endianess default... */
    reg = soc_pci_read(unit, CMIC_CMCx_CCM_DMA_CFG_OFFSET(cmc));
    soc_reg_field_set(unit, CMIC_CMC0_CCM_DMA_CFGr, &reg, ABORTf, 0);
    soc_reg_field_set(unit, CMIC_CMC0_CCM_DMA_CFGr, &reg, ENf, 0);
    soc_pci_write(unit, CMIC_CMCx_CCM_DMA_CFG_OFFSET(cmc), reg);  /* Clearing EN clears stats */
    /* Start DMA */
    soc_reg_field_set(unit, CMIC_CMC0_CCM_DMA_CFGr, &reg, ENf, 1);
    soc_pci_write(unit, CMIC_CMCx_CCM_DMA_CFG_OFFSET(cmc), reg);

    rv = SOC_E_TIMEOUT;
    if (soc->ccmDmaIntrEnb) {
            (void)soc_cmicm_cmcx_intr0_enable(unit, cmc, IRQ_CMCx_CCMDMA_DONE);
            if (sal_sem_take(soc->ccmDmaIntr[cmc], soc->ccmDmaTimeout) < 0) {
                rv = SOC_E_TIMEOUT;
            }
           (void)soc_cmicm_cmcx_intr0_disable(unit, cmc, IRQ_CMCx_CCMDMA_DONE);

            reg = soc_pci_read(unit, CMIC_CMCx_CCM_DMA_STAT_OFFSET(cmc));
            if (soc_reg_field_get(unit, CMIC_CMC0_CCM_DMA_STATr,
                                                        reg, DONEf)) {
                rv = SOC_E_NONE;
                if (soc_reg_field_get(unit, CMIC_CMC0_CCM_DMA_STATr,
                                                        reg, ERRORf)) {
                    rv = SOC_E_FAIL;
                }
            }
    } else {
        soc_timeout_t to;
        LOG_WARN(BSL_LS_SOC_SOCMEM,
                 (BSL_META_U(unit,
                             "using Polling mode for CCM DMA\n")));
        soc_timeout_init(&to, soc->ccmDmaTimeout, 10000);
        do {
            reg = soc_pci_read(unit, CMIC_CMCx_CCM_DMA_STAT_OFFSET(cmc));
            if (soc_reg_field_get(unit, CMIC_CMC0_CCM_DMA_STATr,
                                                    reg, DONEf)) {
                rv = SOC_E_NONE;
                if (soc_reg_field_get(unit, CMIC_CMC0_CCM_DMA_STATr,
                                                    reg, ERRORf)) {
                    rv = SOC_E_FAIL;
                }
                break;
            }
        } while(!(soc_timeout_check(&to)));
    }

    if (rv == SOC_E_TIMEOUT) {
        LOG_ERROR(BSL_LS_SOC_SOCMEM,
                  (BSL_META_U(unit,
                              "CcmDmaTimeout: unit %d, ccm_dma timeout\n"), unit));

        /* Abort CCM DMA */

        /* Dummy read to allow abort to finish */

        /* Disable CCM DMA */

        /* Clear ccm DMA abort bit */
    }

    CCM_DMA_UNLOCK(unit, cmc);
    return rv;
}
#endif

/*
 * @name   - _soc_l3_defip_urpf_index_remap
 * @param  - unit: unit number
 * @param  - wide: 0 - L3_DEFIP, 1 - L3_DEFIP_PAIR_128
 * @param  - index: Physical index
 *
 * @purpose - Remap physical index to logical index
 */
STATIC int
_soc_l3_defip_urpf_index_remap(int unit, int wide, int index)
{
    int new_index = index;
    int num_tcams = 0;
    int tcam_size = SOC_L3_DEFIP_TCAM_DEPTH_GET(unit);
    int entries_left_in_paired_tcam = 0;
    int soc_defip_index_remap = SOC_L3_DEFIP_INDEX_REMAP_GET(unit) / 2;
    int half_defip_size = soc_mem_index_count(unit, L3_DEFIPm) / 2;
    int max_tcams = SOC_L3_DEFIP_MAX_TCAMS_GET(unit);
    int half_defip_index = (max_tcams * tcam_size) /2;

    if (soc_feature(unit, soc_feature_l3_lpm_scaling_enable)) {
        return index;
    }

    assert(tcam_size);

    if (wide == 0) {
        num_tcams = soc_defip_index_remap / tcam_size;
        entries_left_in_paired_tcam = tcam_size -
            (soc_defip_index_remap % tcam_size);
        new_index = index - (num_tcams * 2 * tcam_size);

        if (new_index >= half_defip_index) {
            new_index = new_index - half_defip_index;
        }

        if (new_index - (soc_defip_index_remap % tcam_size) <
            entries_left_in_paired_tcam) {
            new_index = new_index - (soc_defip_index_remap % tcam_size);
        } else {
            new_index = new_index - ((soc_defip_index_remap % tcam_size) * 2);
        }

        if (index >= half_defip_index) {
            new_index = new_index + half_defip_size;
        }
        return new_index;
    }

    new_index = index;
    if (index >= (half_defip_index / 2)) {
        new_index = index - (half_defip_index / 2) + soc_defip_index_remap;
    }
    return new_index;
}

/*
 * @name   - soc_l3_defip_urpf_index_remap
 * @param  - unit: unit number
 * @param  - wide: 0 - L3_DEFIP, 1 - L3_DEFIP_PAIR_128
 * @param  - index: Physical index
 *
 * @purpose - Remap physical index to logical index
 */
int
soc_l3_defip_urpf_index_remap(int unit, int wide, int index)
{
    if (soc_feature(unit, soc_feature_l3_lpm_scaling_enable)) {
        return index;
    }

    assert(SOC_L3_DEFIP_INDEX_INIT(unit));
    if (wide == 0) {
        return SOC_L3_DEFIP_URPF_PHY_TO_LOG_INDEX(unit, index);
    }
    return SOC_L3_DEFIP_PAIR_URPF_PHY_TO_LOG_INDEX(unit, index);
}

/*
 * @name   - _soc_l3_defip_index_remap
 * @param  - unit: unit number
 * @param  - wide: 0 - L3_DEFIP, 1 - L3_DEFIP_PAIR_128
 * @param  - index: Physical index
 *
 * @purpose - Remap physical index to logical index
 */

STATIC int
_soc_l3_defip_index_remap(int unit, int wide, int index)
{
    int new_index = index;
    int num_tcams = 0;
    int tcam_size = SOC_L3_DEFIP_TCAM_DEPTH_GET(unit);
    int entries_left_in_paired_tcam = 0;
    int soc_defip_index_remap = SOC_L3_DEFIP_INDEX_REMAP_GET(unit);

    if (soc_feature(unit, soc_feature_l3_lpm_scaling_enable)) {
        return index;
    }

    assert(tcam_size);

    if (wide == 0) {
        num_tcams = soc_defip_index_remap / tcam_size;
        entries_left_in_paired_tcam = tcam_size -
            (soc_defip_index_remap % tcam_size);
        if (index - (num_tcams * 2 * tcam_size) -
            (soc_defip_index_remap % tcam_size) < entries_left_in_paired_tcam) {
            new_index = index - (num_tcams * 2 * tcam_size) -
                        (soc_defip_index_remap % tcam_size);
        } else {
            new_index = index - (num_tcams * 2 * tcam_size) -
                        ((soc_defip_index_remap % tcam_size) * 2);
        }
        return new_index;
    }
    return new_index;
}

/*
 * @name   - soc_l3_defip_index_remap
 * @param  - unit: unit number
 * @param  - wide: 0 - L3_DEFIP, 1 - L3_DEFIP_PAIR_128
 * @param  - index: Physical index
 *
 * @purpose - Remap physical index to logical index
 */
int
soc_l3_defip_index_remap(int unit, int wide, int index)
{
    if (soc_feature(unit, soc_feature_l3_lpm_scaling_enable)) {
        return index;
    }

    assert(SOC_L3_DEFIP_INDEX_INIT(unit));
    if (wide == 0) {
        return SOC_L3_DEFIP_PHY_TO_LOG_INDEX(unit, index);
    }
    return SOC_L3_DEFIP_PAIR_PHY_TO_LOG_INDEX(unit, index);
}

/*
 * @name   - _soc_l3_reduced_defip_urpf_index_map
 * @param  - unit: unit number
 * @param  - wide: 0 - L3_DEFIP, 1 - L3_DEFIP_PAIR_128
 * @param  - index: logical index
 *
 * @purpose - Map logical to physical index
 */

STATIC int
_soc_l3_reduced_defip_urpf_index_map(int unit, int wide, int index)
{
    int new_index = index;
    int num_tcams = 0;
    int tcam_size = SOC_L3_DEFIP_TCAM_DEPTH_GET(unit);
    int entries_left_in_paired_tcam = 0;
    int soc_defip_index_remap = SOC_L3_DEFIP_INDEX_REMAP_GET(unit) / 2;
    int half_table_size = (SOC_CONTROL(unit)->l3_defip_max_tcams *
                           tcam_size) / 2;
    int half_defip_size = soc_mem_index_count(unit, L3_DEFIPm) / 2;

    if (wide == 0) {
        num_tcams = soc_defip_index_remap / tcam_size;
        entries_left_in_paired_tcam = tcam_size -
                                      (soc_defip_index_remap % tcam_size);
        if (index < entries_left_in_paired_tcam) {
            new_index = index + (num_tcams * 2 * tcam_size) +
                        (soc_defip_index_remap % tcam_size);
        } else {
            if (index < half_defip_size) {
                /* Apollo 2 has 6 tcams so the v6 rpf tcams will be last two.
                 * Therefore we will setup the defip indexes contiguous
                 */
                new_index = index + (num_tcams * 2 * tcam_size) +
                            ((soc_defip_index_remap % tcam_size) * 2);
            } else {
                if ((index - half_defip_size) < tcam_size) {
                    new_index = half_table_size + (index - half_defip_size);
                } else if ((index - half_defip_size - tcam_size) < entries_left_in_paired_tcam) {
                    new_index = half_table_size + (index - half_defip_size) +
                        (soc_defip_index_remap % tcam_size);
                } else {
                    new_index = half_table_size + (index - half_defip_size) +
                        ((soc_defip_index_remap % tcam_size) * 2);
                }
           }
        }
        return new_index;
    }

    new_index = index;
    if (index >= soc_defip_index_remap) {
        new_index = index + LPM_INDEX_2K_ENTRIES - soc_defip_index_remap;
    }
    return new_index;
}

/*
 * @name   - _soc_l3_defip_urpf_index_map
 * @param  - unit: unit number
 * @param  - wide: 0 - L3_DEFIP, 1 - L3_DEFIP_PAIR_128
 * @param  - index: Logical index
 *
 * @purpose - Map logical index to physical index
 */
STATIC int
_soc_l3_defip_urpf_index_map(int unit, int wide, int index)
{
    int new_index = index;
    int num_tcams = 0;
    int tcam_size = SOC_L3_DEFIP_TCAM_DEPTH_GET(unit);
    int max_tcams = SOC_L3_DEFIP_MAX_TCAMS_GET(unit);
    int entries_left_in_paired_tcam = 0;
    int soc_defip_index_remap = SOC_L3_DEFIP_INDEX_REMAP_GET(unit) / 2;
    int half_defip_size = soc_mem_index_count(unit, L3_DEFIPm) / 2;
    int half_defip_index = (max_tcams * tcam_size) / 2;

    if (soc_feature(unit, soc_feature_l3_lpm_scaling_enable)) {
        return index;
    }

    assert(tcam_size);

    if (wide == 0) {
        num_tcams = soc_defip_index_remap / tcam_size;
        entries_left_in_paired_tcam = tcam_size -
                                      (soc_defip_index_remap % tcam_size);
        if (index < entries_left_in_paired_tcam) {
            new_index = index + (num_tcams * 2 * tcam_size) +
                        (soc_defip_index_remap % tcam_size);
        } else if (index >= (half_defip_size)) {
            if ((index - half_defip_size)  < entries_left_in_paired_tcam) {
                new_index = (index - half_defip_size) +
                            (num_tcams * 2 * tcam_size) +
                            (soc_defip_index_remap % tcam_size) +
                            half_defip_index;
            } else {
                new_index = (index - half_defip_size) +
                            (num_tcams * 2 * tcam_size) +
                            ((soc_defip_index_remap % tcam_size) * 2) +
                            half_defip_index;
            }
        } else {
            new_index = index + (num_tcams * 2 * tcam_size) +
                        ((soc_defip_index_remap % tcam_size) * 2);
        }
        return new_index;
    }

    new_index = index;
    if (index >= soc_defip_index_remap) {
        new_index = index + (half_defip_index / 2) - soc_defip_index_remap;
    }
    return new_index;
}

/*
 * @name   - _soc_l3_reduced_defip_urpf_index_remap
 * @param  - unit: unit number
 * @param  - wide: 0 - L3_DEFIP, 1 - L3_DEFIP_PAIR_128
 * @param  - index: Physical index
 *
 * @purpose - Remap physical index to logical index
 */
STATIC int
_soc_l3_reduced_defip_urpf_index_remap(int unit, int wide, int index)
{
    int new_index = index;
    int num_tcams = 0;
    int tcam_size = SOC_L3_DEFIP_TCAM_DEPTH_GET(unit);
    int entries_left_in_paired_tcam = 0;
    int soc_defip_index_remap = SOC_L3_DEFIP_INDEX_REMAP_GET(unit) / 2;

    if (wide == 0) {
        num_tcams = soc_defip_index_remap / tcam_size;
        entries_left_in_paired_tcam = tcam_size -
            (soc_defip_index_remap % tcam_size);
        new_index = index - (num_tcams * 2 * tcam_size);

        if (new_index < LPM_INDEX_4K_ENTRIES) {

            if (new_index - (soc_defip_index_remap % tcam_size) <
                entries_left_in_paired_tcam) {
                new_index = new_index - (soc_defip_index_remap % tcam_size);
            } else {
                new_index = new_index - ((soc_defip_index_remap % tcam_size) * 2);
            }
        } else {
            new_index = index - LPM_INDEX_4K_ENTRIES;
            if (new_index - (soc_defip_index_remap % tcam_size) <
                entries_left_in_paired_tcam) {
                new_index = new_index - ((soc_defip_index_remap % tcam_size) * 3) ;
            } else {
                new_index = new_index - ((soc_defip_index_remap % tcam_size) * 4);
            }
            new_index += LPM_INDEX_4K_ENTRIES;
        }
        return new_index;
    }

    new_index = index;
    if (index >= LPM_INDEX_2K_ENTRIES) {
        new_index = index - LPM_INDEX_2K_ENTRIES + soc_defip_index_remap;
    }
    return new_index;
}

/*
 * @name   - soc_l3_defip_urpf_index_map
 * @param  - unit: unit number
 * @param  - wide: 0 - L3_DEFIP, 1 - L3_DEFIP_PAIR_128
 * @param  - index: Logical index
 *
 * @purpose - Map logical index to physical index
 */

int
soc_l3_defip_urpf_index_map(int unit, int wide, int index)
{
    if (soc_feature(unit, soc_feature_l3_lpm_scaling_enable)) {
        return index;
    }

    assert(SOC_L3_DEFIP_INDEX_INIT(unit));

    if (wide == 0) {
        return SOC_L3_DEFIP_URPF_LOG_TO_PHY_INDEX(unit, index);
    }
    return SOC_L3_DEFIP_PAIR_URPF_LOG_TO_PHY_INDEX(unit, index);
}

/*
 * @name   - _soc_l3_defip_index_map
 * @param  - unit: unit number
 * @param  - wide: 0 - L3_DEFIP, 1 - L3_DEFIP_PAIR_128
 * @param  - index: Logical index
 *
 * @purpose - Map logical index to physical index
 */
STATIC int
_soc_l3_defip_index_map(int unit, int wide, int index)
{
    int new_index = index;
    int num_tcams = 0;
    int tcam_size = SOC_L3_DEFIP_TCAM_DEPTH_GET(unit);
    int entries_left_in_paired_tcam = 0;
    int soc_defip_index_remap = SOC_L3_DEFIP_INDEX_REMAP_GET(unit);

    if (soc_feature(unit, soc_feature_l3_lpm_scaling_enable)) {
        return index;
    }
    if (wide == 0) {
        num_tcams = soc_defip_index_remap / tcam_size;
        entries_left_in_paired_tcam = tcam_size -
            (soc_defip_index_remap % tcam_size);
        if (index < entries_left_in_paired_tcam) {
            new_index = index + (num_tcams * 2 * tcam_size) +
                        soc_defip_index_remap % tcam_size;
        } else {
            new_index = index + (num_tcams * 2 * tcam_size) +
                        ((soc_defip_index_remap % tcam_size) * 2);
        }
        return new_index;
    }
    return index;
}


/*
 * @name   - soc_l3_defip_index_map
 * @param  - unit: unit number
 * @param  - wide: 0 - L3_DEFIP, 1 - L3_DEFIP_PAIR_128
 * @param  - index: Logical index
 *
 * @purpose - Map logical index to physical index
 */
int
soc_l3_defip_index_map(int unit, int wide, int index)
{
    if (soc_feature(unit, soc_feature_l3_lpm_scaling_enable)) {
        return index;
    }

    assert(SOC_L3_DEFIP_INDEX_INIT(unit));
    if (wide == 0) {
        return SOC_L3_DEFIP_LOG_TO_PHY_INDEX(unit, index);
    }
    return SOC_L3_DEFIP_PAIR_LOG_TO_PHY_INDEX(unit, index);
}

/*
 * @name   - _soc_l3_defip_alpm_urpf_index_remap
 * @param  - unit: unit number
 * @param  - wide: 0 - L3_DEFIP, 1 - L3_DEFIP_PAIR_128
 * @param  - index: Physical index
 *
 * @purpose - Remap physical index to logical index
 */
STATIC int
_soc_l3_defip_alpm_urpf_index_remap(int unit, int wide, int index)
{
    uint32 tcam_size = SOC_L3_DEFIP_TCAM_DEPTH_GET(unit);
    uint32 num_chunks, chunk_offset;
    int max_tcams = SOC_L3_DEFIP_MAX_TCAMS_GET(unit);
    int defip128_tbl_sz = SOC_CONTROL(unit)->l3_defip_index_remap;
    if (wide) {
        num_chunks = index / tcam_size;
        chunk_offset = index % tcam_size;
        return num_chunks * defip128_tbl_sz / (max_tcams / 2) +
               chunk_offset;
    }
    num_chunks = index / tcam_size;
    chunk_offset = index % tcam_size;
    chunk_offset -= (defip128_tbl_sz / (max_tcams / 2));
    return num_chunks * soc_mem_index_count(unit, L3_DEFIPm) / max_tcams + chunk_offset;
}


/*
 * @name   - soc_l3_defip_alpm_urpf_index_remap
 * @param  - unit: unit number
 * @param  - wide: 0 - L3_DEFIP, 1 - L3_DEFIP_PAIR_128
 * @param  - index: Physical index
 *
 * @purpose - Remap physical index to logical index
 */
int
soc_l3_defip_alpm_urpf_index_remap(int unit, int wide, int index)
{
    assert(SOC_L3_DEFIP_INDEX_INIT(unit));
    if (wide == 0) {
        return SOC_L3_DEFIP_ALPM_URPF_PHY_TO_LOG_INDEX(unit, index);
    }
    return SOC_L3_DEFIP_PAIR_ALPM_URPF_PHY_TO_LOG_INDEX(unit, index);
}
/*
 * @name   - _soc_l3_defip_alpm_urpf_index_map
 * @param  - unit: unit number
 * @param  - wide: 0 - L3_DEFIP, 1 - L3_DEFIP_PAIR_128
 * @param  - index: Logical index
 *
 * @purpose - Map logical index to physical index
 */
STATIC int
_soc_l3_defip_alpm_urpf_index_map(int unit, int wide, int index)
{
    uint32 tcam_size = SOC_L3_DEFIP_TCAM_DEPTH_GET(unit);
    uint32 tcam_num, tcam_idx;
    int max_tcams = SOC_L3_DEFIP_MAX_TCAMS_GET(unit);
    uint32 incr;
    int defip128_tbl_sz = SOC_CONTROL(unit)->l3_defip_index_remap;
    if (wide) {
        incr = defip128_tbl_sz / (max_tcams / 2);
        tcam_num = index / incr;
        tcam_idx = index % incr;
        return tcam_num * tcam_size + tcam_idx;
    }
    incr = soc_mem_index_count(unit, L3_DEFIPm) / (max_tcams / 2);
    incr = incr / 2;
    tcam_num = index / incr;
    tcam_idx = index % incr;
    return tcam_num * tcam_size + tcam_idx + defip128_tbl_sz / (max_tcams / 2);
}


/*
 * @name   - soc_l3_defip_alpm_urpf_index_map
 * @param  - unit: unit number
 * @param  - wide: 0 - L3_DEFIP, 1 - L3_DEFIP_PAIR_128
 * @param  - index: Logical index
 *
 * @purpose - Map logical index to physical index
 */

int
soc_l3_defip_alpm_urpf_index_map(int unit, int wide, int index)
{
    assert(SOC_L3_DEFIP_INDEX_INIT(unit));

    if (wide == 0) {
        return SOC_L3_DEFIP_ALPM_URPF_LOG_TO_PHY_INDEX(unit, index);
    }
    return SOC_L3_DEFIP_PAIR_ALPM_URPF_LOG_TO_PHY_INDEX(unit, index);
}

/*
 * @name   - soc_l3_defip_index_mem_map
 * @param  - unit: unit number
 * @param  - index: physical index
 * @param  - (OUT)mem: name of the tcam
 *
 * @purpose - Map L3_DEFIP view using physical index
 */

int
soc_l3_defip_index_mem_map(int unit, int index, soc_mem_t* mem)
{
    int logical_index =  -1;
    int defip_pair_index = -1;
    int cam_size = SOC_L3_DEFIP_TCAM_DEPTH_GET(unit);

    *mem = L3_DEFIPm;

    if (soc_feature(unit, soc_feature_l3_lpm_scaling_enable)) {
        return index;
    }

    if (SOC_CONTROL(unit)->l3_defip_urpf &&
        !soc_feature(unit, soc_feature_l3_defip_advanced_lookup)) {
        logical_index = soc_l3_defip_urpf_index_remap(unit, 0, index);

        if (logical_index != -1) {
            *mem = L3_DEFIPm;
        } else {
            *mem = L3_DEFIP_PAIR_128m;
            defip_pair_index = ((index / (cam_size * 2)) * cam_size) +
                               (index % cam_size);
            logical_index = soc_l3_defip_urpf_index_remap(unit, 1,
                                                          defip_pair_index);
        }
        return logical_index;
    }

    logical_index = soc_l3_defip_index_remap(unit, 0, index);

    if (logical_index != -1) {
        *mem = L3_DEFIPm;
    } else {
        *mem = L3_DEFIP_PAIR_128m;
        defip_pair_index = ((index / (cam_size * 2)) * cam_size) +
                           (index % cam_size);
        logical_index = soc_l3_defip_index_remap(unit, 1, defip_pair_index);
    }

    return logical_index;
}

int
soc_l3_defip_indexes_deinit(int unit)
{
    if (SOC_L3_DEFIP_INDEX_INIT(unit) == NULL) {
        return SOC_E_NONE;
    }

    if (SOC_L3_DEFIP_LOG_TO_PHY_ARRAY(unit)) {
        sal_free(SOC_L3_DEFIP_LOG_TO_PHY_ARRAY(unit));
        SOC_L3_DEFIP_LOG_TO_PHY_ARRAY(unit) = NULL;
    }

    if (SOC_L3_DEFIP_URPF_LOG_TO_PHY_ARRAY(unit)) {
        sal_free(SOC_L3_DEFIP_URPF_LOG_TO_PHY_ARRAY(unit));
        SOC_L3_DEFIP_URPF_LOG_TO_PHY_ARRAY(unit) = NULL;
    }

    if (SOC_L3_DEFIP_PHY_TO_LOG_ARRAY(unit)) {
        sal_free(SOC_L3_DEFIP_PHY_TO_LOG_ARRAY(unit));
        SOC_L3_DEFIP_PHY_TO_LOG_ARRAY(unit) = NULL;
    }

    if (SOC_L3_DEFIP_URPF_PHY_TO_LOG_ARRAY(unit)) {
        sal_free(SOC_L3_DEFIP_URPF_PHY_TO_LOG_ARRAY(unit));
        SOC_L3_DEFIP_URPF_PHY_TO_LOG_ARRAY(unit) = NULL;
    }

    if (SOC_L3_DEFIP_PAIR_LOG_TO_PHY_ARRAY(unit)) {
        sal_free(SOC_L3_DEFIP_PAIR_LOG_TO_PHY_ARRAY(unit));
        SOC_L3_DEFIP_PAIR_LOG_TO_PHY_ARRAY(unit) = NULL;
    }

    if (SOC_L3_DEFIP_PAIR_URPF_LOG_TO_PHY_ARRAY(unit)) {
        sal_free(SOC_L3_DEFIP_PAIR_URPF_LOG_TO_PHY_ARRAY(unit));
        SOC_L3_DEFIP_PAIR_URPF_LOG_TO_PHY_ARRAY(unit) = NULL;
    }

    if (SOC_L3_DEFIP_PAIR_PHY_TO_LOG_ARRAY(unit)) {
        sal_free(SOC_L3_DEFIP_PAIR_PHY_TO_LOG_ARRAY(unit));
        SOC_L3_DEFIP_PAIR_PHY_TO_LOG_ARRAY(unit) = NULL;
    }

    if (SOC_L3_DEFIP_PAIR_URPF_PHY_TO_LOG_ARRAY(unit)) {
        sal_free(SOC_L3_DEFIP_PAIR_URPF_PHY_TO_LOG_ARRAY(unit));
        SOC_L3_DEFIP_PAIR_URPF_PHY_TO_LOG_ARRAY(unit) = NULL;
    }

    if (SOC_L3_DEFIP_ALPM_URPF_LOG_TO_PHY_ARRAY(unit)) {
        sal_free(SOC_L3_DEFIP_ALPM_URPF_LOG_TO_PHY_ARRAY(unit));
        SOC_L3_DEFIP_ALPM_URPF_LOG_TO_PHY_ARRAY(unit) = NULL;
    }
    if (SOC_L3_DEFIP_ALPM_URPF_PHY_TO_LOG_ARRAY(unit)) {
        sal_free(SOC_L3_DEFIP_ALPM_URPF_PHY_TO_LOG_ARRAY(unit));
        SOC_L3_DEFIP_ALPM_URPF_PHY_TO_LOG_ARRAY(unit) = NULL;
    }
    if (SOC_L3_DEFIP_PAIR_ALPM_URPF_LOG_TO_PHY_ARRAY(unit)) {
        sal_free(SOC_L3_DEFIP_PAIR_ALPM_URPF_LOG_TO_PHY_ARRAY(unit));
        SOC_L3_DEFIP_PAIR_ALPM_URPF_LOG_TO_PHY_ARRAY(unit) = NULL;
    }
    if (SOC_L3_DEFIP_PAIR_ALPM_URPF_PHY_TO_LOG_ARRAY(unit)) {
        sal_free(SOC_L3_DEFIP_PAIR_ALPM_URPF_PHY_TO_LOG_ARRAY(unit));
        SOC_L3_DEFIP_PAIR_ALPM_URPF_PHY_TO_LOG_ARRAY(unit) = NULL;
    }

    if (SOC_L3_DEFIP_INDEX_INIT(unit)) {
        sal_free(SOC_L3_DEFIP_INDEX_INIT(unit));
        SOC_L3_DEFIP_INDEX_INIT(unit) = NULL;
    }

    return SOC_E_NONE;
}

/*
 * @name   - soc_l3_defip_indexes_init
 * @param  - unit: unit number
 * @returns -  SOC_E_MEMORY, SOC_E_NONE
 *
 * @purpose - generate a map of logical to physical and
 *            physical to logical indexes
 */
int
soc_l3_defip_indexes_init(int unit, int config_ipv6_entries)
{
    int log_index;
    int phy_index;
    int temp_index;

    int max_index = SOC_L3_DEFIP_TCAM_DEPTH_GET(unit) *
                    SOC_L3_DEFIP_MAX_TCAMS_GET(unit);
    int defip_max_index = soc_mem_index_count(unit, L3_DEFIPm);
    int size;
    int defip_table_present;
    int defip_pair_128_table_present;

    if (soc_feature(unit, soc_feature_l3_lpm_scaling_enable)) {
        SOC_L3_DEFIP_MAX_128B_ENTRIES(unit) = config_ipv6_entries;
        return SOC_E_NONE;
    }

    assert(SOC_L3_DEFIP_TCAM_DEPTH_GET(unit));
    assert(SOC_L3_DEFIP_MAX_TCAMS_GET(unit));

    SOC_IF_ERROR_RETURN(soc_l3_defip_indexes_deinit(unit));

    SOC_L3_DEFIP_INDEX_INIT(unit) =
        sal_alloc(sizeof(_soc_l3_defip_index_table_t), "defip map table");
    if (SOC_L3_DEFIP_INDEX_INIT(unit) == NULL) {
        return SOC_E_MEMORY;
    }

    sal_memset(SOC_L3_DEFIP_INDEX_INIT(unit), 0x0,
                                       sizeof(_soc_l3_defip_index_table_t));
    defip_table_present =
        soc_mem_index_count(unit, L3_DEFIPm) ? 1 : 0;

    if (defip_table_present) {
        size = defip_max_index * sizeof(int);
        SOC_L3_DEFIP_LOG_TO_PHY_ARRAY(unit) =
                                sal_alloc(size, "defip log to phy index mapping");
        if (SOC_L3_DEFIP_LOG_TO_PHY_ARRAY(unit) == NULL) {
            return SOC_E_MEMORY;
        }
        sal_memset(SOC_L3_DEFIP_LOG_TO_PHY_ARRAY(unit), -1, size);

        SOC_L3_DEFIP_URPF_LOG_TO_PHY_ARRAY(unit) =
                             sal_alloc(size, "urpf defip log to phy index mapping");
        if (SOC_L3_DEFIP_URPF_LOG_TO_PHY_ARRAY(unit) == NULL) {
            return SOC_E_MEMORY;
        }
        sal_memset(SOC_L3_DEFIP_URPF_LOG_TO_PHY_ARRAY(unit), -1, size);

        SOC_L3_DEFIP_ALPM_URPF_LOG_TO_PHY_ARRAY(unit) =
                 sal_alloc(size, "alpm urpf defip log to phy index mapping");
        if (SOC_L3_DEFIP_ALPM_URPF_LOG_TO_PHY_ARRAY(unit) == NULL) {
            return SOC_E_MEMORY;
        }
        sal_memset(SOC_L3_DEFIP_ALPM_URPF_LOG_TO_PHY_ARRAY(unit), -1, size);

        size = max_index * sizeof(int);
        SOC_L3_DEFIP_PHY_TO_LOG_ARRAY(unit) =
                              sal_alloc(size, "defip phy to log index mapping");
        if (SOC_L3_DEFIP_PHY_TO_LOG_ARRAY(unit) == NULL) {
            return SOC_E_MEMORY;
        }
        sal_memset(SOC_L3_DEFIP_PHY_TO_LOG_ARRAY(unit), -1, size);

        SOC_L3_DEFIP_URPF_PHY_TO_LOG_ARRAY(unit) =
                                 sal_alloc(size, "defip phy to log index mapping");
        if (SOC_L3_DEFIP_URPF_PHY_TO_LOG_ARRAY(unit) == NULL) {
            return SOC_E_MEMORY;
        }
        sal_memset(SOC_L3_DEFIP_URPF_PHY_TO_LOG_ARRAY(unit), -1, size);

        SOC_L3_DEFIP_ALPM_URPF_PHY_TO_LOG_ARRAY(unit) =
                 sal_alloc(size, "alpm urpf defip phy to log index mapping");
        if (SOC_L3_DEFIP_ALPM_URPF_PHY_TO_LOG_ARRAY(unit) == NULL) {
            return SOC_E_MEMORY;
        }
        sal_memset(SOC_L3_DEFIP_ALPM_URPF_PHY_TO_LOG_ARRAY(unit), -1, size);
    }

    defip_pair_128_table_present =
        soc_mem_index_count(unit, L3_DEFIP_PAIR_128m) ? 1 : 0;

    if (defip_pair_128_table_present) {
        size = soc_mem_index_count(unit, L3_DEFIP_PAIR_128m) * sizeof(int);

        SOC_L3_DEFIP_PAIR_LOG_TO_PHY_ARRAY(unit) =
                                sal_alloc(size, "defip log to phy index mapping");
        if (SOC_L3_DEFIP_PAIR_LOG_TO_PHY_ARRAY(unit) == NULL) {
            return SOC_E_MEMORY;
        }
        sal_memset(SOC_L3_DEFIP_PAIR_LOG_TO_PHY_ARRAY(unit), -1, size);

        SOC_L3_DEFIP_PAIR_URPF_LOG_TO_PHY_ARRAY(unit) =
                             sal_alloc(size, "urpf defip log to phy index mapping");
        if (SOC_L3_DEFIP_PAIR_URPF_LOG_TO_PHY_ARRAY(unit) == NULL) {
            return SOC_E_MEMORY;
        }
        sal_memset(SOC_L3_DEFIP_PAIR_URPF_LOG_TO_PHY_ARRAY(unit), -1, size);

        SOC_L3_DEFIP_PAIR_ALPM_URPF_LOG_TO_PHY_ARRAY(unit) =
                 sal_alloc(size, "alpm urpf defip log to phy index mapping");
        if (SOC_L3_DEFIP_PAIR_ALPM_URPF_LOG_TO_PHY_ARRAY(unit) == NULL) {
            return SOC_E_MEMORY;
        }
        sal_memset(SOC_L3_DEFIP_PAIR_ALPM_URPF_LOG_TO_PHY_ARRAY(unit), -1, size);

        size = max_index * sizeof(int);
        SOC_L3_DEFIP_PAIR_PHY_TO_LOG_ARRAY(unit) =
                              sal_alloc(size, "defip phy to log index mapping");
        if (SOC_L3_DEFIP_PAIR_PHY_TO_LOG_ARRAY(unit) == NULL) {
            return SOC_E_MEMORY;
        }
        sal_memset(SOC_L3_DEFIP_PAIR_PHY_TO_LOG_ARRAY(unit), -1, size);

        SOC_L3_DEFIP_PAIR_URPF_PHY_TO_LOG_ARRAY(unit) =
                                 sal_alloc(size, "defip phy to log index mapping");
        if (SOC_L3_DEFIP_PAIR_URPF_PHY_TO_LOG_ARRAY(unit) == NULL) {
            return SOC_E_MEMORY;
        }
        sal_memset(SOC_L3_DEFIP_PAIR_URPF_PHY_TO_LOG_ARRAY(unit), -1, size);

        SOC_L3_DEFIP_PAIR_ALPM_URPF_PHY_TO_LOG_ARRAY(unit) =
                 sal_alloc(size, "alpm urpf defip phy to log index mapping");
        if (SOC_L3_DEFIP_PAIR_ALPM_URPF_PHY_TO_LOG_ARRAY(unit) == NULL) {
            return SOC_E_MEMORY;
        }
        sal_memset(SOC_L3_DEFIP_PAIR_ALPM_URPF_PHY_TO_LOG_ARRAY(unit), -1, size);
    }

    for (log_index = 0; log_index < defip_max_index; log_index++) {
        phy_index = _soc_l3_defip_index_map(unit, 0, log_index);
        SOC_L3_DEFIP_LOG_TO_PHY_INDEX(unit, log_index) = phy_index;
        temp_index = _soc_l3_defip_index_remap(unit, 0, phy_index);
        SOC_L3_DEFIP_PHY_TO_LOG_INDEX(unit, phy_index) = temp_index;
        if (log_index != temp_index || phy_index >= max_index) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "defip map/remap failed 1: "
                                  "lidx %d tidx %d pidx %d midx:%d\n"),
                                  log_index, temp_index, phy_index, max_index));
        }

        if (SOC_IS_TRIUMPH3(unit) &&
               soc_feature(unit, soc_feature_l3_reduced_defip_table)) {
            phy_index = _soc_l3_reduced_defip_urpf_index_map(unit, 0, log_index);
            SOC_L3_DEFIP_URPF_LOG_TO_PHY_INDEX(unit, log_index) = phy_index;
            temp_index = _soc_l3_reduced_defip_urpf_index_remap(unit, 0, phy_index);
            SOC_L3_DEFIP_URPF_PHY_TO_LOG_INDEX(unit, phy_index) = temp_index;
        } else {
            phy_index = _soc_l3_defip_urpf_index_map(unit, 0, log_index);
            SOC_L3_DEFIP_URPF_LOG_TO_PHY_INDEX(unit, log_index) = phy_index;
            temp_index = _soc_l3_defip_urpf_index_remap(unit, 0, phy_index);
            SOC_L3_DEFIP_URPF_PHY_TO_LOG_INDEX(unit, phy_index) = temp_index;
        }
        if (log_index != temp_index || phy_index >= max_index) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "defip map/remap failed 2: "
                                  "lidx %d tidx %d pidx %d midx:%d\n"),
                                  log_index, temp_index, phy_index, max_index));
        }

        if (SOC_CONTROL(unit)->l3_defip_index_remap % 4 ||
            SOC_L3_DEFIP_MAX_TCAMS_GET(unit) % 4 ||
            !soc_feature(unit, soc_feature_alpm)) {
            /* alpm urpf will not be used */
            continue;
        }

        phy_index = _soc_l3_defip_alpm_urpf_index_map(unit, 0, log_index);
        SOC_L3_DEFIP_ALPM_URPF_LOG_TO_PHY_INDEX(unit, log_index) = phy_index;
        temp_index = _soc_l3_defip_alpm_urpf_index_remap(unit, 0, phy_index);
        SOC_L3_DEFIP_ALPM_URPF_PHY_TO_LOG_INDEX(unit, phy_index) = temp_index;
        if (log_index != temp_index || phy_index >= max_index) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "defip map/remap failed 3: "
                                  "lidx %d tidx %d pidx %d midx:%d\n"),
                                  log_index, temp_index, phy_index, max_index));
        }
    }

    defip_max_index = soc_mem_index_count(unit, L3_DEFIP_PAIR_128m);
    for (log_index = 0; log_index < defip_max_index; log_index++) {
        phy_index = _soc_l3_defip_index_map(unit, 1, log_index);
        SOC_L3_DEFIP_PAIR_LOG_TO_PHY_INDEX(unit, log_index) = phy_index;
        temp_index = _soc_l3_defip_index_remap(unit, 1, phy_index);
        SOC_L3_DEFIP_PAIR_PHY_TO_LOG_INDEX(unit, phy_index) = temp_index;
        if (log_index != temp_index || phy_index >= max_index) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "defip map/remap failed 4: "
                                  "lidx %d tidx %d pidx %d midx:%d\n"),
                                  log_index, temp_index, phy_index, max_index));
        }

        if (SOC_IS_TRIUMPH3(unit) &&
               soc_feature(unit, soc_feature_l3_reduced_defip_table)) {
            phy_index = _soc_l3_reduced_defip_urpf_index_map(unit, 1, log_index);
            SOC_L3_DEFIP_PAIR_URPF_LOG_TO_PHY_INDEX(unit, log_index) = phy_index;
            temp_index = _soc_l3_reduced_defip_urpf_index_remap(unit, 1, phy_index);
            SOC_L3_DEFIP_PAIR_URPF_PHY_TO_LOG_INDEX(unit, phy_index) = temp_index;
        } else {
            phy_index = _soc_l3_defip_urpf_index_map(unit, 1, log_index);
            SOC_L3_DEFIP_PAIR_URPF_LOG_TO_PHY_INDEX(unit, log_index) = phy_index;
            temp_index = _soc_l3_defip_urpf_index_remap(unit, 1, phy_index);
            SOC_L3_DEFIP_PAIR_URPF_PHY_TO_LOG_INDEX(unit, phy_index) = temp_index;
        }
        if (log_index != temp_index || phy_index >= max_index) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "defip map/remap failed 5: "
                                  "lidx %d tidx %d pidx %d midx:%d\n"),
                                  log_index, temp_index, phy_index, max_index));
        }

        if (SOC_CONTROL(unit)->l3_defip_index_remap % 4 ||
            SOC_L3_DEFIP_MAX_TCAMS_GET(unit) % 4 ||
            !soc_feature(unit, soc_feature_alpm)) {
            /* alpm urpf will not be used */
            continue;
        }

        phy_index = _soc_l3_defip_alpm_urpf_index_map(unit, 1, log_index);
        SOC_L3_DEFIP_PAIR_ALPM_URPF_LOG_TO_PHY_INDEX(unit, log_index) = phy_index;
        temp_index = _soc_l3_defip_alpm_urpf_index_remap(unit, 1, phy_index);
        SOC_L3_DEFIP_PAIR_ALPM_URPF_PHY_TO_LOG_INDEX(unit, phy_index) = temp_index;
        if (log_index != temp_index || phy_index >= max_index) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "defip map/remap failed 6: "
                                  "lidx %d tidx %d pidx %d midx:%d\n"),
                                  log_index, temp_index, phy_index, max_index));
        }
    }

    SOC_L3_DEFIP_MAX_128B_ENTRIES(unit) = config_ipv6_entries;
    return SOC_E_NONE;
}

/*
 * This function assumes that the lock to L3_DEFIP and
 * L3_DEFIP_PAIR_128 tables are already taken
*/
int
soc_defip_tables_resize(int unit, int num_ipv6_128b_entries)
{
    soc_persist_t *sop;
    int config_v6_entries = 0;
    int num_defip_entries = 0;
    int max_tcams = 0, tcam_size = 0;

    num_ipv6_128b_entries = num_ipv6_128b_entries +
                            (num_ipv6_128b_entries % 2);

#if defined(BCM_TRIDENT2_SUPPORT)
    if (soc_feature(unit, soc_feature_alpm) &&
        (soc_trident2_alpm_mode_get(unit) == 1 ||
         soc_trident2_alpm_mode_get(unit) == 3)) {
        num_ipv6_128b_entries = (num_ipv6_128b_entries + 3) / 4 * 4;
    }
#endif /* BCM_TRIDENT2_SUPPORT */

    if (SOC_CONTROL(unit)->tcam_protect_write) {
        num_ipv6_128b_entries = (num_ipv6_128b_entries + 3) / 4 * 4;
    }
    config_v6_entries = num_ipv6_128b_entries;

    max_tcams = SOC_L3_DEFIP_MAX_TCAMS_GET(unit);
    tcam_size = SOC_CONTROL(unit)->l3_defip_tcam_size;
    if (SOC_CONTROL(unit)->tcam_protect_write) {
        if (soc_mem_index_count(unit, L3_DEFIP_PAIR_128m)) {
            tcam_size++;
        }
        if (soc_mem_index_count(unit, L3_DEFIPm)) {
            tcam_size++;
        }
    }

    if (soc_feature(unit, soc_feature_l3_lpm_scaling_enable)) {
        num_ipv6_128b_entries = 0;
    }

    num_defip_entries = (max_tcams * tcam_size) -
                        (num_ipv6_128b_entries * 2);
    if (SOC_CONTROL(unit)->tcam_protect_write) {
        if (num_defip_entries) {
            num_defip_entries -= SOC_CONTROL(unit)->l3_defip_max_tcams;
            /* Don't use up all entries as protect entries */
            if (num_defip_entries <= 0) {
                return SOC_E_CONFIG;
            }
            tcam_size--;
        }
        if (num_ipv6_128b_entries) {
            num_ipv6_128b_entries -= SOC_CONTROL(unit)->l3_defip_max_tcams/2;
            /* Don't use up all entries as protect entries */
            if (num_ipv6_128b_entries <= 0) {
                return SOC_E_CONFIG;
            }
            tcam_size--;
        }
        if (config_v6_entries) {
            config_v6_entries -= SOC_CONTROL(unit)->l3_defip_max_tcams/2;
            if (config_v6_entries < 0) {
                return SOC_E_CONFIG;
            }
        }
    }
    SOC_CONTROL(unit)->l3_defip_tcam_size = tcam_size;

    if (soc_feature(unit, soc_feature_l3_lpm_scaling_enable)) {
        if(!soc_feature(unit, soc_feature_l3_lpm_128b_entries_reserved)) {
            config_v6_entries = ((config_v6_entries /
                                   SOC_CONTROL(unit)->l3_defip_tcam_size) +
                                   ((config_v6_entries %
                                   SOC_CONTROL(unit)->l3_defip_tcam_size)
                              ? 1 : 0)) * SOC_CONTROL(unit)->l3_defip_tcam_size;
        }
        SOC_L3_DEFIP_MAX_128B_ENTRIES(unit) = config_v6_entries;
        return SOC_E_NONE;
    }

    sop = SOC_PERSIST(unit);
    sop->memState[L3_DEFIP_PAIR_128m].index_max =
                                             num_ipv6_128b_entries - 1;
    sop->memState[L3_DEFIP_PAIR_128_ONLYm].index_max =
                                  num_ipv6_128b_entries - 1;
    sop->memState[L3_DEFIP_PAIR_128_DATA_ONLYm].index_max =
                                      num_ipv6_128b_entries - 1;
    sop->memState[L3_DEFIP_PAIR_128_HIT_ONLYm].index_max =
                                      num_ipv6_128b_entries - 1;
    sop->memState[L3_DEFIP_PAIR_128_HIT_ONLY_Xm].index_max =
                                      num_ipv6_128b_entries - 1;
    sop->memState[L3_DEFIP_PAIR_128_HIT_ONLY_Ym].index_max =
                                      num_ipv6_128b_entries - 1;
    sop->memState[L3_DEFIPm].index_max = num_defip_entries - 1;

    sop->memState[L3_DEFIP_ONLYm].index_max =
                                  sop->memState[L3_DEFIPm].index_max;
    sop->memState[L3_DEFIP_DATA_ONLYm].index_max =
                                  sop->memState[L3_DEFIPm].index_max;
    sop->memState[L3_DEFIP_HIT_ONLYm].index_max =
                                  sop->memState[L3_DEFIPm].index_max;
    sop->memState[L3_DEFIP_HIT_ONLY_Xm].index_max =
                                  sop->memState[L3_DEFIPm].index_max;
    sop->memState[L3_DEFIP_HIT_ONLY_Ym].index_max =
                                  sop->memState[L3_DEFIPm].index_max;
    SOC_CONTROL(unit)->l3_defip_index_remap = num_ipv6_128b_entries;
    return soc_l3_defip_indexes_init(unit, config_v6_entries);
}
/*
 * This function makes config variables unit specific and
 * either updates config.bcm or display on the terminal
*/
void
soc_mem_config_save(int unit, char *config_str, char *config_value)
{
     char local_config_str[80];
     sal_sprintf(local_config_str,"%s.%d",config_str,unit);

     if (soc_mem_config_set != NULL) {
         soc_mem_config_set(local_config_str,config_value);
     } else {
         if (config_value != NULL) {
             LOG_CLI((BSL_META_U(unit,
                                 "%s=%s #Add in config.bcm\n"),
                      local_config_str,config_value));
         }
     }
}

#if defined(BCM_TRIUMPH3_SUPPORT)
/*
 * Function:
 *    soc_mem_field_clear_all
 * Purpose:
 *    Clears a field in all entries for a memory .
 *    Operates on all copies of the table if copyno is COPYNO_ALL.
 * Notes:
 */

int
soc_mem_field_clear_all(int unit,
                        soc_mem_t mem,
                        soc_field_t field,
                        int copyno,
                        int force_all)
{
    int           rv = SOC_E_NONE;
    int       chunk_size, chunk_entries, mem_size, entry_words;
    int       index, index_end, index_min, index_max;
    uint32    *buf;
    uint32    fval[SOC_MAX_MEM_FIELD_WORDS];
    int       i;


    /* Implemented for TR3 and HX4 */
    if (!(SOC_IS_TRIUMPH3(unit) || SOC_IS_HELIX4(unit))) {
        return SOC_E_UNAVAIL;
    }

    if (SOC_WARM_BOOT(unit) || SOC_IS_RELOADING(unit)) {
        return SOC_E_NONE;
    }

    _SOC_MEM_REPLACE_MEM(unit, mem);
    if (soc_mem_index_count(unit, mem) == 0) {
        return SOC_E_NONE;
    }

    sal_memset(fval, 0, sizeof (fval));

    /* check is the field is valid */
    if (!SOC_MEM_FIELD_VALID(unit, mem, field)) {
        return SOC_E_PARAM;
    }

    chunk_size = SOC_MEM_CLEAR_CHUNK_SIZE_GET(unit);
    index_min = soc_mem_index_min(unit, mem);
    index_max = soc_mem_index_max(unit, mem);
    entry_words = soc_mem_entry_words(unit, mem);
    mem_size = (index_max - index_min + 1) * entry_words * 4;

    if (mem_size < chunk_size) {
        chunk_size = mem_size;
    }

    buf = soc_cm_salloc(unit, chunk_size, "mem_clear_buf");
    if (buf == NULL) {
        return SOC_E_MEMORY;
    }

    chunk_entries = chunk_size / (entry_words * 4);

    rv = SOC_E_NONE;
    MEM_LOCK(unit, mem);
    for (index = index_min; index <= index_max; index += chunk_entries) {
        index_end = index + chunk_entries - 1;
        if (index_end > index_max) {
            LOG_CLI((BSL_META_U(unit,
                                "index_end = %d\n"), index_end));
            index_end = index_max;
        }

        /* read a chunk of data from mem */
        rv = soc_mem_read_range(unit, mem, copyno, index, index_end, buf);
        if (rv < 0) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "soc_mem_read_range: "
                                  "read %s.%s[%d-%d] failed: %s\n"),
                       SOC_MEM_UFNAME(unit, mem),
                       SOC_BLOCK_NAME(unit, copyno),
                       index, index_end, soc_errmsg(rv)));
            break;
        }


        /* Clear the field in each entry */
        for (i = 0; i < chunk_entries; i++) {
             soc_mem_field_set(unit, mem, &buf[i * entry_words],
                               field, fval);
        }

        /* Write back the entries to mem */
        rv = soc_mem_write_range(unit, mem, copyno, index, index_end, buf);
        if (rv < 0) {
            LOG_ERROR(BSL_LS_SOC_SOCMEM,
                      (BSL_META_U(unit,
                                  "soc_mem_write_range: "
                                  "write %s.%s[%d-%d] failed: %s\n"),
                       SOC_MEM_UFNAME(unit, mem),
                       SOC_BLOCK_NAME(unit, copyno),
                       index, index_end, soc_errmsg(rv)));
            break;
        }
    }

    MEM_UNLOCK(unit, mem);
    soc_cm_sfree(unit, buf);
    return rv;
}
#endif /* BCM_TRIUMPH3_SUPPORT */

/* to setup L3_IIF default entries */
#if defined(BCM_HURRICANE3_SUPPORT)
int
_soc_hr3_l3iif_hw_mem_init(int unit, int force_init_all) 
{
    schan_msg_t schan_msg;
    int src_blk, dst_blk, blk, acc_type;
    iif_entry_t def_entry;
    uint8 at;
    uint32 maddr;
    int entry_dw, data_byte_len, idx;
    int bit_num = 0, val = 0x3f;
    int rv = SOC_E_NONE;

    /* 
     * GH2 leverage design from HR3 with the same L3_IIFm design.
     *  - both L3_IIFm size are 512 only.
     */
    if (!(SOC_IS_HURRICANE3(unit) || SOC_IS_GREYHOUND2(unit)) || 
            !(SOC_MEM_IS_VALID(unit, L3_IIFm))) { 
        return SOC_E_UNAVAIL;
    }
    sal_memcpy(&def_entry,  soc_mem_entry_null(unit, L3_IIFm),
               sizeof(iif_entry_t));

    entry_dw = soc_mem_entry_words(unit, L3_IIFm);
    data_byte_len = entry_dw * sizeof (uint32);
    
    /* 
     * set default L3_IIFm.TRUST_DSCP_PTRf to maximum to prevent the 
     * internal priority been changed unexpectedly.
     *  - The L3_IIFm.TRUST_DSCP_PTRf in different ESW 
     */
    bit_num = soc_mem_field_length(unit, L3_IIFm, TRUST_DSCP_PTRf);
    switch (bit_num) {
        case 6:
            val= 0x3f;
            break;
        case 7:
            val = 0x7f;
            break;
        default :
            break;
    }
    soc_mem_field32_set(unit, L3_IIFm, 
            (uint32 *)&def_entry, TRUST_DSCP_PTRf, val);

    /* 
     * Performing the low level mem write operation to avoid MEM and index 
     * validation. In many HR3 SKUs, L3_IIFm is valid but entry count is 0.
     * - that means "index_max" in SOC info for L3_IIFm will be -1
     * - soc_mem_index_max() for this case can't report HW table size.
     */
    schan_msg_clear(&schan_msg);
    acc_type = SOC_MEM_ACC_TYPE(unit, L3_IIFm);
    src_blk = SOC_BLOCK2SCH(unit, CMIC_BLOCK(unit));
    sal_memcpy(schan_msg.writecmd.data, &def_entry, 
            entry_dw * sizeof(uint32));

    LOG_INFO(BSL_LS_SOC_L3, (BSL_META_U(unit, 
            "Installing default L3_IIFm entry..\n")));

    MEM_LOCK(unit, L3_IIFm);
    for (idx = 0; idx < HR3_L3_IIF_HW_SIZE; idx ++){
    
        /* Write to one or all copies of the memory */
        SOC_MEM_BLOCK_ITER(unit, L3_IIFm, blk) {
    
            maddr = soc_mem_addr_get(unit, L3_IIFm, 0, blk, idx, &at);
            schan_msg.writecmd.address = maddr;
    
            _soc_mem_dst_blk_update(unit, blk, maddr, &dst_blk);
    
            soc_schan_header_cmd_set(unit, &schan_msg.header, WRITE_MEMORY_CMD_MSG,
                                    dst_blk, src_blk, acc_type, data_byte_len, 0,
                                    0);
    
            /* Write header + address + entry_dw data DWORDs */
            /* Note: The hardware does not send WRITE_MEMORY_ACK_MSG. */
            if (2 + entry_dw > CMIC_SCHAN_WORDS(unit)) {
                LOG_WARN(BSL_LS_SOC_SOCMEM,
                        (BSL_META_U(unit,
                            "soc_mem_write: assert will fail for memory %s\n"),
                        SOC_MEM_NAME(unit, L3_IIFm)));
            }
    
            rv = _soc_mem_write_schan_msg_send(unit, 
                    &schan_msg, L3_IIFm, blk, idx);
            if (SOC_FAILURE(rv)) {
                break;
            }
        }

        if (!force_init_all) {
            break;
        }
    }
    MEM_UNLOCK(unit, L3_IIFm);

    LOG_INFO(BSL_LS_SOC_L3, (BSL_META_U(unit, 
            "L3_IIFm %d entries is installed(rv=%d)!\n") , idx, rv));

    return rv;
}
#endif  /* BCM_HURRICANE3_SUPPORT */

#endif /* defined(BCM_ESW_SUPPORT) || defined(BCM_SIRIUS_SUPPORT) ||%%%
          defined(BCM_SAND_SUPPORT) */
