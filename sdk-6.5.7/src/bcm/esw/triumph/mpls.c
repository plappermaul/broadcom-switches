/*
 * $Id: mpls.c,v 1.556 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:    mpls.c
 * Purpose: Manages MPLS functions
 */
#include <soc/defs.h>
#include <sal/core/libc.h>
#include <shared/bsl.h>
#if defined(BCM_TRIUMPH_SUPPORT) && defined(BCM_MPLS_SUPPORT) && \
    defined(INCLUDE_L3)

#include <soc/drv.h>
#include <soc/scache.h>
#include <soc/hash.h>
#include <soc/util.h>
#include <soc/triumph.h>

#include <bcm/error.h>
#include <bcm/mpls.h>

#include <bcm_int/esw/mbcm.h>
#include <bcm_int/esw/l3.h>
#include <bcm_int/esw/firebolt.h>
#include <bcm_int/esw/triumph.h>
#include <bcm_int/esw/trx.h>
#include <bcm_int/esw/xgs3.h>
#include <bcm_int/esw/mpls.h>
#include <bcm_int/esw/stack.h>
#include <bcm_int/common/multicast.h>
#include <bcm_int/esw/virtual.h>
#include <bcm_int/esw/port.h>
#if defined(BCM_TOMAHAWK_SUPPORT)
#include <bcm_int/esw/ecn.h>
#endif
#include <bcm_int/esw/vlan.h>
#ifdef BCM_TRIUMPH2_SUPPORT
#include <bcm_int/esw/triumph2.h>
#endif
#if defined(BCM_TRIUMPH3_SUPPORT)
#include <bcm_int/esw/triumph3.h>
#endif /* BCM_TRIUMPH3_SUPPORT*/
#ifdef BCM_KATANA2_SUPPORT
#include <bcm_int/esw/katana2.h>
#include <soc/katana2.h>
#endif
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
#include <bcm_int/esw/trident2plus.h>
#endif
#if defined(BCM_TRIDENT2PLUS_SUPPORT) || defined(BCM_TOMAHAWK_SUPPORT)
#include <bcm_int/esw/xgs5.h>
#endif
#if defined(BCM_SABER2_SUPPORT)
#include <soc/saber2.h>
#endif /* BCM_SABER2_SUPPORT */

#include <bcm_int/esw_dispatch.h>
#include <bcm_int/api_xlate_port.h>
#include <bcm_int/esw/failover.h>

#ifdef BCM_WARM_BOOT_SUPPORT
#include <bcm_int/esw/switch.h>

#define BCM_WB_VERSION_1_10               SOC_SCACHE_VERSION(1,10)
#define BCM_WB_VERSION_1_9                SOC_SCACHE_VERSION(1,9)
#define BCM_WB_VERSION_1_8                SOC_SCACHE_VERSION(1,8)
#define BCM_WB_VERSION_1_7                SOC_SCACHE_VERSION(1,7)
#define BCM_WB_VERSION_1_6                SOC_SCACHE_VERSION(1,6)
#define BCM_WB_VERSION_1_5                SOC_SCACHE_VERSION(1,5)
#define BCM_WB_VERSION_1_4                SOC_SCACHE_VERSION(1,4)
#define BCM_WB_VERSION_1_3                SOC_SCACHE_VERSION(1,3)
#define BCM_WB_VERSION_1_2                SOC_SCACHE_VERSION(1,2)
#define BCM_WB_VERSION_1_1                SOC_SCACHE_VERSION(1,1)
#define BCM_WB_VERSION_1_0                SOC_SCACHE_VERSION(1,0)
#define BCM_WB_DEFAULT_VERSION            BCM_WB_VERSION_1_10
#endif /* BCM_WARM_BOOT_SUPPORT */

#if defined(BCM_KATANA_SUPPORT)
#include <bcm_int/esw/katana.h>
#endif
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
#include <bcm_int/esw/policer.h>
#include <bcm_int/esw/flex_ctr.h>
#endif

#if defined(BCM_TOMAHAWK2_SUPPORT)
#define ECN_MAP_ID_INVALID 0xffffffff
#endif


_bcm_tr_mpls_bookkeeping_t  _bcm_tr_mpls_bk_info[BCM_MAX_NUM_UNITS] = {{ 0 }};
STATIC _bcm_mpls_egr_nhopList_t  *_bcm_tr_mpls_nhop_headPtr[BCM_MAX_NUM_UNITS][_BCM_TR_MPLS_HASH_ELEMENTS];
bcm_tr_mpls_vc_swap_table_hash_t *bcmi_vc_swap_label_hash[SOC_MAX_NUM_DEVICES];

#define MPLS_INFO(_unit_)   (&_bcm_tr_mpls_bk_info[_unit_])
#define L3_INFO(_unit_)   (&_bcm_l3_bk_info[_unit_])

/* for _bcm_tr_mpls_get_vc_and_swap_table_index() function */
#define ENTRIES_PER_CHUNK  0x400
#define CHUNK_NUM(_ix) (((_ix) - imin)/ENTRIES_PER_CHUNK)

#define SOC_VC_SWAP_LABLE_FIELD(m, f) soc_field_info_t * m##f
#define SOC_VC_SWAP_LABLE_FIELD_ASSIGN(m_u, s, m, f) \
                (s)->m##f = soc_mem_fieldinfo_get(m_u, m, f)

#if defined(BCM_KATANA_SUPPORT) || defined(BCM_APACHE_SUPPORT)
#define _BCM_QOS_MAP_TYPE_MASK                     0x3ff
#define _BCM_QOS_MAP_SHIFT                            10
#define _BCM_QOS_MAP_TYPE_ING_QUEUE_OFFSET_MAP_TABLE   5
#define _BCM_QOS_MAP_ING_QUEUE_OFFSET_MAX              7
#define EH_TAG_TYPE_NONE                               0
#define EH_TAG_TYPE_EXPLICT_QUEUE                      1
#define EH_TAG_TYPE_ING_QUEUE_MAP                      2

#endif

#ifdef BCM_APACHE_SUPPORT
#define _BCM_MPLS_FWD_LABEL1_PHB_FROM_TOS            0
#define _BCM_MPLS_FWD_LABEL2_PHB_FROM_TOS            1
#define _BCM_MPLS_FWD_LABEL3_PHB_FROM_TOS            2
#endif

typedef struct soc_emvasltm_field_cache_s {
    SOC_VC_SWAP_LABLE_FIELD(EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, MPLS_LABELf);
    SOC_VC_SWAP_LABLE_FIELD(EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, MPLS_LABEL_ACTIONf);
    SOC_VC_SWAP_LABLE_FIELD(EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, MPLS_TTLf);
    SOC_VC_SWAP_LABLE_FIELD(EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, CW_INSERT_FLAGf);
    SOC_VC_SWAP_LABLE_FIELD(EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, MPLS_EXPf);
    SOC_VC_SWAP_LABLE_FIELD(EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, NEW_PRIf);
    SOC_VC_SWAP_LABLE_FIELD(EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, NEW_CFIf);
    SOC_VC_SWAP_LABLE_FIELD(EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, SD_TAG_VIDf);
    SOC_VC_SWAP_LABLE_FIELD(EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, SD_TAG_ACTION_IF_PRESENTf);
    SOC_VC_SWAP_LABLE_FIELD(EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, SD_TAG_ACTION_IF_NOT_PRESENTf);
    SOC_VC_SWAP_LABLE_FIELD(EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, MPLS_EXP_SELECTf);
    SOC_VC_SWAP_LABLE_FIELD(EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, MPLS_EXP_MAPPING_PTRf);
} soc_emvasltm_field_cache_t;

STATIC soc_emvasltm_field_cache_t soc_emvasltm_field_cache[SOC_MAX_NUM_DEVICES];

#define SOC_MEM_VC_SWAP_F32_GET(m_unit, m_mem, m_entry_data, m_field) \
        soc_meminfo_fieldinfo_field32_get((&SOC_MEM_INFO(m_unit, m_mem)), (m_entry_data), \
	 (soc_emvasltm_field_cache[(m_unit)].m_mem##m_field))

#define SOC_MEM_VC_SWAP_F32_SET(m_unit, m_mem, m_entry_data, m_field, m_val) \
        soc_meminfo_fieldinfo_field32_set((&SOC_MEM_INFO(m_unit, m_mem)), (m_entry_data), \
	(soc_emvasltm_field_cache[(m_unit)].m_mem##m_field), (m_val))


#define SOC_MEM_VC_SWAP_FIELD_VALID(m_unit, m_mem, m_field) \
                ((soc_emvasltm_field_cache[(m_unit)].m_mem##m_field) != NULL)

#define VC_AND_SWAP_TABLE_RESV_INDEX 0

#define _BCM_MPLS_MEPID_IN_VALID_RANGE(mep_id) (mep_id > 0 && mep_id < 8192)

STATIC int _bcm_tr_mpls_add_vp_nh(int unit, bcm_if_t  egr_if, int nh_index);
STATIC int _bcm_tr_mpls_add_egrNhop ( int unit, bcm_if_t egress_if);
STATIC void  _bcm_tr_mpls_cleanup_egrNhop (int unit);
STATIC int  _bcm_tr_mpls_l2_nh_info_delete(int unit, int nh_index);
STATIC void _bcm_tr_mpls_vpws_vp_map_clear (int unit, bcm_vpn_t vpn);
STATIC void  _bcm_tr_mpls_egr_tunnel_ref_count_adjust (int unit, int index, int step);
STATIC void _bcm_tr_mpls_ing_queue_map_ref_count_adjust (int unit, int index, int step);
STATIC void bcmi_tr_mpls_vc_swap_hash_cleanup(int unit);
STATIC int _bcm_tr_mpls_port_get(int unit, bcm_vpn_t vpn, int vp, bcm_mpls_port_t *mpls_port);
#if defined(BCM_TRIUMPH3_SUPPORT)
STATIC int _bcm_tr_mpls_vc_and_swap_hash_nhop_add (int unit, int vc_swap_index,
                                                   int nh_index);
#endif

/*
 * Function:
 *      bcm_mpls_enable
 * Purpose:
 *      Enable/disable MPLS function.
 * Parameters:
 *      unit - SOC unit number.
 *      enable - TRUE: enable MPLS support; FALSE: disable MPLS support.
 * Returns:
 *      BCM_E_XXX.
 */
STATIC int
bcm_tr_mpls_enable(int unit, int enable)
{
    int port, rv = BCM_E_NONE;
    bcm_pbmp_t port_pbmp;

    BCM_PBMP_CLEAR(port_pbmp);
    BCM_PBMP_ASSIGN(port_pbmp, PBMP_PORT_ALL(unit));
#ifdef BCM_KATANA2_SUPPORT
    if (SOC_IS_KATANA2(unit) && soc_feature(unit, soc_feature_flex_port)) {
        BCM_IF_ERROR_RETURN(_bcm_kt2_flexio_pbmp_update(unit, &port_pbmp));
    }
    if (soc_feature(unit, soc_feature_linkphy_coe) ||
        soc_feature(unit, soc_feature_subtag_coe)) {
        _bcm_kt2_subport_pbmp_update(unit, &port_pbmp);
    }
#endif

    PBMP_ITER(port_pbmp, port) {
        /* No mpls lookup on stacking ports. */
        if (IS_ST_PORT(unit, port)) {
            continue;
        }
        BCM_IF_ERROR_RETURN
            (bcm_esw_port_control_set(unit, port, bcmPortControlMpls, 
                                      (enable) ? 1 : 0));
    }
    return rv;
}

/*
 * Function:
 *      _bcm_tr_mpls_free_resource
 * Purpose:
 *      Free all allocated tables and memory
 * Parameters:
 *      unit - SOC unit number
 * Returns:
 *      Nothing
 */
STATIC void
_bcm_tr_mpls_free_resource(int unit, _bcm_tr_mpls_bookkeeping_t *mpls_info)
{
    if (!mpls_info) {
        return;
    }

    if (mpls_info->vrf_bitmap) {
        sal_free(mpls_info->vrf_bitmap);
        mpls_info->vrf_bitmap = NULL;
    }

    if (mpls_info->vpws_bitmap) {
        sal_free(mpls_info->vpws_bitmap);
        mpls_info->vpws_bitmap = NULL;
    }

    if (mpls_info->vpws_vp_map) {
        sal_free(mpls_info->vpws_vp_map);
        mpls_info->vpws_vp_map = NULL;
    }

    if (mpls_info->trunk_vp_map) {
        sal_free(mpls_info->trunk_vp_map);
        mpls_info->trunk_vp_map = NULL;
    }

    if (mpls_info->vc_c_bitmap) {
        sal_free(mpls_info->vc_c_bitmap);
        mpls_info->vc_c_bitmap = NULL;
    }

    if (mpls_info->vc_nc_bitmap) {
        sal_free(mpls_info->vc_nc_bitmap);
        mpls_info->vc_nc_bitmap = NULL;
    }

    if (mpls_info->pw_term_bitmap) {
        sal_free(mpls_info->pw_term_bitmap);
        mpls_info->pw_term_bitmap = NULL;
    }

    if (mpls_info->pw_init_bitmap) {
        sal_free(mpls_info->pw_init_bitmap);
        mpls_info->pw_init_bitmap = NULL;
    }

    if (mpls_info->match_key) {
        sal_free(mpls_info->match_key);
        mpls_info->match_key = NULL;
    }

    if (mpls_info->tnl_bitmap) {
        sal_free(mpls_info->tnl_bitmap);
        mpls_info->tnl_bitmap = NULL;
    }

    if (mpls_info->ip_tnl_bitmap) {
        sal_free(mpls_info->ip_tnl_bitmap);
        mpls_info->ip_tnl_bitmap = NULL;
    }

    if (mpls_info->egr_mpls_bitmap) {
        sal_free(mpls_info->egr_mpls_bitmap);
        mpls_info->egr_mpls_bitmap = NULL;
    }
    if (mpls_info->egr_mpls_hw_idx) {
        sal_free(mpls_info->egr_mpls_hw_idx);
        mpls_info->egr_mpls_hw_idx = NULL;
    }

    if (mpls_info->ing_exp_map_bitmap) {
        sal_free(mpls_info->ing_exp_map_bitmap);
        mpls_info->ing_exp_map_bitmap = NULL;
    }

    if (mpls_info->egr_l2_exp_map_bitmap) {
        sal_free(mpls_info->egr_l2_exp_map_bitmap);
        mpls_info->egr_l2_exp_map_bitmap = NULL;
    }
#ifdef BCM_APACHE_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_exp_to_phb_cng_map)) {
        if (mpls_info->ing_exp_to_phb_cng_map_bitmap) {
            sal_free(mpls_info->ing_exp_to_phb_cng_map_bitmap);
            mpls_info->ing_exp_to_phb_cng_map_bitmap = NULL;
        }

        if (mpls_info->egr_exp_to_phb_cng_map_bitmap) {
            sal_free(mpls_info->egr_exp_to_phb_cng_map_bitmap);
            mpls_info->egr_exp_to_phb_cng_map_bitmap = NULL;
        }
    }
#endif
    if (mpls_info->vc_swap_ref_count) {
        sal_free(mpls_info->vc_swap_ref_count);
        mpls_info->vc_swap_ref_count = NULL;
    }

    if (mpls_info->egr_tunnel_ref_count) {
        sal_free(mpls_info->egr_tunnel_ref_count);
        mpls_info->egr_tunnel_ref_count = NULL;
    }

    if (mpls_info->ing_qmap_ref_count) {
        sal_free(mpls_info->ing_qmap_ref_count);
        mpls_info->ing_qmap_ref_count = NULL;
    }

    if (mpls_info->svp_profile_set_bitmap) {
        sal_free(mpls_info->svp_profile_set_bitmap);
        mpls_info->svp_profile_set_bitmap = NULL;
    }

    if (mpls_info->network_vp_bitmap) {
        sal_free(mpls_info->network_vp_bitmap);
        mpls_info->network_vp_bitmap = NULL;
    }

    if (mpls_info->failover_vp) {
        sal_free(mpls_info->failover_vp);
        mpls_info->failover_vp = NULL;
    }

    (void) _bcm_tr_mpls_cleanup_egrNhop(unit);
    bcmi_tr_mpls_vc_swap_hash_cleanup(unit);
#if defined(BCM_APACHE_SUPPORT) || defined(BCM_TOMAHAWK2_SUPPORT)
    if (soc_feature(unit, soc_feature_egr_ip_tnl_mpls_double_wide)) {
        bcmi_egr_ip_tunnel_mpls_sw_cleanup(unit);
    }
#endif

}

/*
 * Function:
 *      bcm_tr_mpls_check_init
 * Purpose:
 *      Check if MPLS is initialized
 * Parameters:
 *      unit     - Device Number
 * Returns:
 *      BCM_E_XXX
 */

STATIC int 
_bcm_tr_mpls_check_init (int unit)
{
    if ((unit < 0) || (unit >= BCM_MAX_NUM_UNITS)) {
        return BCM_E_UNIT;
    }

    if (!_bcm_tr_mpls_bk_info[unit].initialized) { 
        return BCM_E_INIT;
    } else {
        return BCM_E_NONE;
    }
}

/*
 * Function:
 *      bcm_tr_mpls_lock
 * Purpose:
 *      Take the MPLS software module Lock Sempahore
 * Parameters:
 *      unit     - Device Number
 * Returns:
 *      BCM_E_XXX
 */

int 
bcm_tr_mpls_lock (int unit)
{
   int rv;

   rv = _bcm_tr_mpls_check_init (unit);
   
   if ( rv == BCM_E_NONE ) {
        sal_mutex_take(MPLS_INFO((unit))->mpls_mutex, sal_mutex_FOREVER);
   }
   return rv; 
}


/*
 * Function:
 *      bcm_tr_mpls_unlock
 * Purpose:
 *      Release  the MPLS software module Lock Semaphore
 * Parameters:
 *      unit     - Device Number
 * Returns:
 *      BCM_E_XXX
 */

void
bcm_tr_mpls_unlock(int unit)
{
    int rv;

    rv = _bcm_tr_mpls_check_init (unit);

    if ( rv == BCM_E_NONE ) {
        sal_mutex_give(MPLS_INFO((unit))->mpls_mutex);
    }
}

/*
 * Function:
 *      bcm_tr_mpls_stat_clear
 * Purpose:
 *      Clear L2 MPLS PW Stats
 * Parameters:
 *      unit   - (IN) SOC unit #
 * Returns:
 *      BCM_E_XXX
 */     

STATIC int
_bcm_tr_mpls_stat_clear(int unit)
{
    int num_pw_term, num_pw_init;
    int index, rv=BCM_E_NONE;
    ing_pw_term_counters_entry_t cent;
    ing_pw_term_seq_num_entry_t  snent;
    egr_pw_init_counters_entry_t pw_init_entry;
    egr_pw_init_counters_x_entry_t pw_init_entry_x;
    egr_pw_init_counters_y_entry_t pw_init_entry_y;

    if (SOC_HW_RESET(unit)) {
        return BCM_E_NONE;
    }
    num_pw_term = 0;
    if (SOC_MEM_IS_VALID(unit, ING_PW_TERM_COUNTERSm)) {
        num_pw_term = soc_mem_index_count(unit, ING_PW_TERM_COUNTERSm);
        /* Clear the ING Counters */
        sal_memset(&cent, 0, sizeof(ing_pw_term_counters_entry_t));
        for (index=0; index<num_pw_term; index++) {
            rv = WRITE_ING_PW_TERM_COUNTERSm(unit, MEM_BLOCK_ANY, index, &cent);
            if (rv < 0) {
                return rv;
            }
        }
    }

    if (SOC_MEM_IS_VALID(unit,  ING_PW_TERM_SEQ_NUMm)) {
        num_pw_term = soc_mem_index_count(unit,  ING_PW_TERM_SEQ_NUMm);
        /* Clear the ING Counters */
        sal_memset(&snent, 0, sizeof(ing_pw_term_seq_num_entry_t));
        for (index = 0; index < num_pw_term; index++) {
            rv = WRITE_ING_PW_TERM_SEQ_NUMm(unit, MEM_BLOCK_ANY, index, &snent);
            if (rv < 0) {
                return rv;
            }
        }
    }

    num_pw_init = 0;
    if (SOC_MEM_IS_VALID(unit, EGR_PW_INIT_COUNTERSm)) {
        num_pw_init = soc_mem_index_count(unit, EGR_PW_INIT_COUNTERSm);
        /* Clear the EGR Counters */
        sal_memset(&pw_init_entry, 0, sizeof(egr_pw_init_counters_entry_t));    
        for (index = 0; index < num_pw_init; index++) {
            rv = WRITE_EGR_PW_INIT_COUNTERSm(unit, MEM_BLOCK_ANY, index, 
                                             &pw_init_entry);
            if (rv < 0) {
                return rv;
            }
        }
        if (SOC_MEM_IS_VALID(unit, EGR_PW_INIT_COUNTERS_Xm)) {
            num_pw_init = soc_mem_index_count(unit, EGR_PW_INIT_COUNTERS_Xm);
            sal_memset(&pw_init_entry_x, 0, sizeof(egr_pw_init_counters_x_entry_t));
            for (index = 0; index < num_pw_init; index++) {
                rv = WRITE_EGR_PW_INIT_COUNTERS_Xm(unit, MEM_BLOCK_ANY, index, 
                                                   &pw_init_entry_x);
                if (rv < 0) {
                    return rv;
                }
            }
        }
        if (SOC_MEM_IS_VALID(unit, EGR_PW_INIT_COUNTERS_Ym)) {
            num_pw_init = soc_mem_index_count(unit, EGR_PW_INIT_COUNTERS_Ym);
            sal_memset(&pw_init_entry_y, 0, sizeof(egr_pw_init_counters_y_entry_t));
            for (index = 0; index < num_pw_init; index++) {
                rv = WRITE_EGR_PW_INIT_COUNTERS_Ym(unit, MEM_BLOCK_ANY, index, 
                                                   &pw_init_entry_y);
                if (rv < 0) {
                    return rv;
                }
            }
        }
    }
    return rv;
}


/*
* Purpose:
*	   Entry-0 VC-AND-SWAP Entry is reserved for default
* Parameters:
*	   unit 	 -	(IN) StrataSwitch #.
* Returns:
*	   BCM_E_XXX
*/
STATIC int
_bcm_tr_mpls_default_entry_setup(int unit)
{
    egr_mpls_vc_and_swap_label_table_entry_t  vc_entry;
    int rv = BCM_E_NONE;
    uint32 vc_swap_index = VC_AND_SWAP_TABLE_RESV_INDEX;

    sal_memset(&vc_entry, 0,
              sizeof(egr_mpls_vc_and_swap_label_table_entry_t));

    _BCM_MPLS_VC_COUNT_USED_SET(unit, vc_swap_index);

#if defined(BCM_SWAP_TO_SELF_SUPPORT)
    if (soc_feature(unit, soc_feature_mpls_swap_label_preserve)) {

        soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                            &vc_entry, MPLS_LABEL_ACTIONf,
                            _BCM_MPLS_ACTION_NOOP);

    } else
#endif
    {
        soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                            &vc_entry, MPLS_LABEL_ACTIONf,
                            _BCM_MPLS_ACTION_RESERVED);
    }

    rv = soc_mem_write(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
              MEM_BLOCK_ALL, vc_swap_index,
              &vc_entry);

    return rv;
}

/*
 * Function:
 *      bcm_tr_mpls_vrf_used_get
 */     

int
bcm_tr_mpls_vrf_used_get(int unit, int vrf)
{
    return _BCM_MPLS_VRF_USED_GET(unit, vrf);
}

/*
 * Function:
 *      bcm_tr_mpls_pw_used_clear
 */     

void
bcm_tr_mpls_pw_used_clear(int unit, int pw_cnt)
{
    _BCM_MPLS_PW_TERM_USED_CLR(unit, pw_cnt);
}

/*
 * Function:
 *      bcm_tr_mpls_pw_used_get
 */     
int
bcm_tr_mpls_pw_used_get(int unit, int pw_cnt)
{
   return _BCM_MPLS_PW_TERM_USED_GET(unit, pw_cnt);
}

/*
 * Function:
 *      bcm_tr_mpls_pw_used_set
 */     
void
bcm_tr_mpls_pw_used_set(int unit, int pw_cnt)
{
    _BCM_MPLS_PW_TERM_USED_SET(unit, pw_cnt);
}
/*
 * Function:
 *      bcm_tr_mpls_port_independent_range
 * Purpose:
 *      Check if the incoming Label is within the Port Independent Range
 * Parameters:
 *      label - Label
 * Returns:
 *      BCM_E_XXX
 */

int
bcm_tr_mpls_port_independent_range (int unit, bcm_mpls_label_t label, bcm_gport_t port )
{
   uint32 reg_val;         /* Register value */
   uint32 label_1_low=0, label_1_high=0;
   uint32 label_2_low=0, label_2_high=0;


    if (SOC_REG_IS_VALID(unit, GLOBAL_MPLS_RANGE_1_LOWERr)) {
        BCM_IF_ERROR_RETURN(READ_GLOBAL_MPLS_RANGE_1_LOWERr(unit, &reg_val));
        label_1_low =
            soc_reg_field_get(unit, GLOBAL_MPLS_RANGE_1_LOWERr, reg_val, LABELf);
    }

    if (SOC_REG_IS_VALID(unit, GLOBAL_MPLS_RANGE_1_UPPERr)) {
        BCM_IF_ERROR_RETURN(READ_GLOBAL_MPLS_RANGE_1_UPPERr(unit, &reg_val));
        label_1_high =
            soc_reg_field_get(unit, GLOBAL_MPLS_RANGE_1_UPPERr, reg_val, LABELf);
    }

    if (SOC_REG_IS_VALID(unit, GLOBAL_MPLS_RANGE_2_LOWERr)) {
        BCM_IF_ERROR_RETURN(READ_GLOBAL_MPLS_RANGE_2_LOWERr(unit, &reg_val));
        label_2_low =
            soc_reg_field_get(unit, GLOBAL_MPLS_RANGE_2_LOWERr, reg_val, LABELf);
    }

    if (SOC_REG_IS_VALID(unit, GLOBAL_MPLS_RANGE_2_UPPERr)) {
        BCM_IF_ERROR_RETURN(READ_GLOBAL_MPLS_RANGE_2_UPPERr(unit, &reg_val));
        label_2_high =
            soc_reg_field_get(unit, GLOBAL_MPLS_RANGE_2_UPPERr, reg_val, LABELf);
    }

    if (  (( label >= label_1_low) && ( label <= label_1_high)) || 
          ((  label >= label_2_low) && ( label <= label_2_high)) ){
        if (port == BCM_GPORT_INVALID) {
            return BCM_E_NONE;
        } else {
            return BCM_E_CONFIG;
        }
    } else {
        if (port == BCM_GPORT_INVALID) {
            return BCM_E_CONFIG;
        } else {
            return BCM_E_NONE;
        }
    }
}



#ifdef BCM_WARM_BOOT_SUPPORT

int
_bcm_tr_mpls_scache_size_get(int unit, int *size)
{
    int num_vrf, num_vp, num_vc, num_ip_tnl, num_mpls_map;
    int num_ing_exp_map, num_egr_l2_exp_map, num_pw_term, num_pw_init;
#ifdef BCM_APACHE_SUPPORT
    int num_ing_mpls_exp_map1 = 0, num_egr_mpls_exp_map3 = 0;
#endif
    int num_nh, alloc_sz = 0;

    num_vrf = SOC_CONTROL(unit)->max_vrf_id + 1;
    num_vp = soc_mem_index_count(unit, SOURCE_VPm);
    num_vc = soc_mem_index_count(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);
    num_ip_tnl = soc_mem_index_count(unit, EGR_IP_TUNNEL_MPLSm);
    /* EGR_MPLS_EXP_MAPPING_1 profiles indexed by ((int_pri << 2) + cng) */
    num_mpls_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 64;
    num_ing_exp_map = soc_mem_index_count(unit, ING_MPLS_EXP_MAPPINGm) / 8;
#ifdef BCM_APACHE_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_exp_to_phb_cng_map)) {
        num_ing_mpls_exp_map1 = soc_mem_index_count(unit, 
                ING_MPLS_EXP_MAPPING_1m) / 16;
        num_egr_mpls_exp_map3 = soc_mem_index_count(unit, 
                EGR_MPLS_EXP_MAPPING_3m) / 8;
    }
#endif
    num_egr_l2_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_PRI_MAPPINGm) / 8;
    num_pw_term = 0;
    if (SOC_MEM_IS_VALID(unit, ING_PW_TERM_COUNTERSm)) {
        num_pw_term = soc_mem_index_count(unit, ING_PW_TERM_COUNTERSm);
    }
    num_pw_init = 0;
    if (SOC_MEM_IS_VALID(unit, EGR_PW_INIT_COUNTERSm)) {
        num_pw_init = soc_mem_index_count(unit, EGR_PW_INIT_COUNTERSm);
    }
    num_nh = soc_mem_index_count(unit, EGR_L3_NEXT_HOPm);

    alloc_sz = 0;
    alloc_sz += SOC_WB_SCACHE_CONTROL_SIZE; /* For version number */
    alloc_sz += SHR_BITALLOCSIZE(num_vrf);
    alloc_sz += SHR_BITALLOCSIZE(num_vp / 2);
    alloc_sz += SHR_BITALLOCSIZE(num_vc) * 2;
    alloc_sz += SHR_BITALLOCSIZE(num_pw_term);
    alloc_sz += SHR_BITALLOCSIZE(num_pw_init);
    alloc_sz += SHR_BITALLOCSIZE(num_mpls_map);
    alloc_sz += SHR_BITALLOCSIZE(num_ing_exp_map);
    alloc_sz += SHR_BITALLOCSIZE(num_egr_l2_exp_map);
    alloc_sz += SHR_BITALLOCSIZE(num_ip_tnl *
            _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit));
    alloc_sz += SHR_BITALLOCSIZE(num_ip_tnl);

    alloc_sz += sizeof(uint32) * (num_nh + 1);
#ifdef BCM_APACHE_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_exp_to_phb_cng_map)) {
        alloc_sz += SHR_BITALLOCSIZE(num_ing_mpls_exp_map1);
        alloc_sz += SHR_BITALLOCSIZE(num_egr_mpls_exp_map3);
    }
#endif
    if (size != NULL) {
        *size = alloc_sz;
    }

    return BCM_E_NONE;
}

/* 
 * Function:
 *      _bcm_tr_mpls_unsynchronized_reinit
 * Purpose:
 *      - Recover MPLS related state from h/w.
 *      - For MPLS module, the case where limited scache is 
 *          available is the same as unsynchronized init.
 *      - In Cold Boot sequence, just returns to calling func.
 * Parameters:
 *      unit    : (IN) Device Unit Number
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_unsynchronized_reinit(int unit) 
{
    int rv = BCM_E_NONE;
    if (!SOC_WARM_BOOT(unit)) {
        return rv; 
    }
    
    return rv;
}

/* 
 * Function:
 *      _bcm_tr_mpls_limited_reinit
 * Purpose:
 *      This is called when only limited scache space 
 *      is available. Please see comments for func
 *      '_bcm_tr_mpls_ingexpbitmap_sync' in relation to this.
 * Parameters:
 *      unit    : (IN) Device Unit Number
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_limited_reinit(int unit)
{
    int    rv = BCM_E_NONE;
    int    stable_size;
    uint8  *mpls_state;
    int    num_ing_exp_map;
    soc_scache_handle_t scache_handle;
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);

    SOC_SCACHE_HANDLE_SET(scache_handle, unit, BCM_MODULE_MPLS, 0);

    if (SOC_WARM_BOOT(unit)) {
    /* Warm Boot */
        SOC_IF_ERROR_RETURN(soc_stable_size_get(unit, &stable_size));

        if (stable_size > sizeof(int32)) {
            rv = _bcm_esw_scache_ptr_get(unit, scache_handle,
                                    FALSE, sizeof(int32), &mpls_state,
                                    BCM_WB_DEFAULT_VERSION, NULL);
    
            if (BCM_FAILURE(rv) && (rv != BCM_E_NOT_FOUND)) {
                return rv;
            }

            if (NULL == mpls_state) {
                return BCM_E_MEMORY;
            }
            num_ing_exp_map = soc_mem_index_count(unit,
                                                     ING_MPLS_EXP_MAPPINGm) / 8;
            SHR_BITCOPY_RANGE(mpls_info->ing_exp_map_bitmap, 0,
                              (SHR_BITDCL *)mpls_state, 0, num_ing_exp_map);
        }
    } else {
    /* Cold Boot */
        rv = _bcm_esw_scache_ptr_get(unit, scache_handle, TRUE,
                                     sizeof(int32), &mpls_state, 
                                     BCM_WB_DEFAULT_VERSION, NULL);
        if (BCM_FAILURE(rv) && (rv != BCM_E_NOT_FOUND)) {
            return rv;
        }
    }
    
    return BCM_E_NONE;
}

#ifdef BCM_TRIUMPH2_SUPPORT
/* 
 * Function:
 *      _bcm_tr_mpls_flex_stat_recover
 * Purpose:
 *      - Recover flex stats for given VP
 * Parameters:
 *      unit    : (IN) Device Unit Number
 *      vp      : (IN) Index into SOURCE_VPm table
 * Returns:
 *      BCM_E_XXX
 */
int
_bcm_tr_mpls_flex_stat_recover(int unit, int vp)
{
    int fs_index;
    bcm_gport_t gport;
    source_vp_entry_t svp_entry;

    SOC_IF_ERROR_RETURN(soc_mem_read(unit, SOURCE_VPm,
                                  MEM_BLOCK_ANY, vp, &svp_entry));

    if (soc_mem_field_valid(unit, SOURCE_VPm, VINTF_CTR_IDXf)) {
        fs_index = soc_SOURCE_VPm_field32_get(unit, &svp_entry,
                                                VINTF_CTR_IDXf);
        if (fs_index) {
            BCM_GPORT_MPLS_PORT_ID_SET(gport, vp);
            _bcm_esw_flex_stat_reinit_add(unit,
                                        _bcmFlexStatTypeGport, fs_index, gport);
        }
    }

    return BCM_E_NONE;
}

/* 
 * Function:
 *      _bcm_tr_mpls_flex_stat_recover
 * Purpose:
 *      - Recover flex stats for given VP
 * Parameters:
 *      unit    : (IN) Device Unit Number
 *      ment    : (IN) Pointer to MPLS_ENTRYm table entry
 * Returns:
 *      BCM_E_XXX
 */
void
_bcm_tr_mpls_label_flex_stat_recover(int unit, mpls_entry_entry_t *ment)
{
    int fs_index;
    _bcm_flex_stat_handle_t mpls_fsh;
    mpls_entry_entry_t fs_ment;
    uint32 key[2];

    fs_index = soc_MPLS_ENTRYm_field32_get(unit, ment,
                                           VINTF_CTR_IDXf);

    if ((0 != soc_MPLS_ENTRYm_field32_get(unit, ment,
                                          USE_VINTF_CTR_IDXf)) &&
        (0 != fs_index)) {
        sal_memset(&fs_ment, 0, sizeof(fs_ment));
        /* Construct key-only entry, copy to FS handle */
        soc_MPLS_ENTRYm_field32_set(unit, &fs_ment, VALIDf,
                 soc_MPLS_ENTRYm_field32_get(unit, ment, VALIDf));
        soc_mem_field_get(unit, MPLS_ENTRYm, (uint32 *) ment,
                          KEYf, (uint32 *) key);
        soc_mem_field_set(unit, MPLS_ENTRYm, (uint32 *) &fs_ment,
                          KEYf, (uint32 *) key);
        /* Technically, we should copy the KEY_TYPE field here,
         * but the rest of the MPLS code appears to abuse the fact that
         * the key type value is 0, so it was handled when we
         * cleared the entry above.
         */

        _BCM_FLEX_STAT_HANDLE_CLEAR(mpls_fsh);
        _BCM_FLEX_STAT_HANDLE_COPY(mpls_fsh, fs_ment);

        _bcm_esw_flex_stat_ext_reinit_add(unit,
                                          _bcmFlexStatTypeMplsLabel,
                                          fs_index, mpls_fsh);
    }
}

int
_bcm_tr_mpls_source_vp_recover(int unit)
{
    source_vp_entry_t svp;
    int rv = BCM_E_NONE;
    int value = 0, vp;
    int dscp_map_chunk = 64, ing_pri_cng_chunk = 16;
    int vp_count = soc_mem_index_count(unit, SOURCE_VPm);

    for (vp = 0; vp < vp_count; vp++) {
        if (!_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
            continue;
        }

        if (_BCM_MPLS_SVP_PROFILE_SET_USED_GET(unit, vp)) {
            /*  Read PORT Table */
            if (!soc_mem_field_valid(unit, SOURCE_VPm, TRUST_DSCP_PTRf) ||
                !soc_mem_field_valid(unit, PORT_TABm, TRUST_DOT1P_PTRf) ||
                !soc_mem_field_valid(unit, SOURCE_VPm, TRUST_DOT1P_PTRf))  {
                return BCM_E_NONE;
            }

            rv = READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp); 
            if (rv < 0) {
                return rv;
            }
            if (soc_feature(unit, soc_feature_dscp_map_per_port)) {
                if (soc_mem_field_valid(unit, SOURCE_VPm, TRUST_DSCP_PTRf)) {
                    value = soc_SOURCE_VPm_field32_get(unit, 
                                                       &svp, 
                                                       TRUST_DSCP_PTRf);
                    rv = _bcm_dscp_table_entry_reference(unit, 
                                                         (value * dscp_map_chunk),
                                                         dscp_map_chunk);
                    if (rv != BCM_E_NONE) {
                        return rv;
                    } else {
                        rv = BCM_E_NONE;
                    }
                }
            }
            if (soc_feature(unit, soc_feature_color_prio_map_profile)) {
                if (soc_mem_field_valid(unit, SOURCE_VPm, TRUST_DOT1P_PTRf)) {
                    value = soc_SOURCE_VPm_field32_get(unit, 
                                                       &svp, 
                                                       TRUST_DOT1P_PTRf);
                    rv = _bcm_ing_pri_cng_map_entry_reference(
                                            unit,
                                            (value * ing_pri_cng_chunk),
                                            ing_pri_cng_chunk);
                    if (rv != BCM_E_NONE) {
                        return rv;
                    } else {
                        rv = BCM_E_NONE;
                    }
                } 
            }
        }
    }
    return rv;
}
#endif /* BCM_TRIUMPH2_SUPPORT */

/* 
 * Function:
 *      _bcm_tr_mpls_source_vp_tpid_recover
 * Purpose:
 *      - Recover SVP TPIDs
 * Parameters:
 *      unit    : (IN) Device Unit Number
 *      vp      : (IN) Index into SOURCE_VPm table
 * Returns:
 *      BCM_E_XXX
 */
int
_bcm_tr_mpls_source_vp_tpid_recover(int unit, int vp)
{
    int index, tpid_enable;
    source_vp_entry_t svp_entry;

    SOC_IF_ERROR_RETURN(soc_mem_read(unit, SOURCE_VPm,
                                  MEM_BLOCK_ANY, vp, &svp_entry));
    if (soc_SOURCE_VPm_field32_get(unit, &svp_entry, SD_TAG_MODEf) == 1) {
        tpid_enable = soc_SOURCE_VPm_field32_get(unit, &svp_entry,
                                                              TPID_ENABLEf);
        for (index = 0; index < 4; index++) {
            if (tpid_enable & (1 << index)) {
                BCM_IF_ERROR_RETURN(_bcm_fb2_outer_tpid_tab_ref_count_add(
                                                           unit, index, 1));
                break;
            }
        }
    }

    return BCM_E_NONE;
}

int
_bcm_tr_mpls_associated_data_recover(int unit)
{
    int macda_idx;
    int i, idx_min, idx_max;
    egr_l3_next_hop_entry_t nh_entry;

    idx_min = soc_mem_index_min(unit, EGR_L3_NEXT_HOPm);
    idx_max = soc_mem_index_max(unit, EGR_L3_NEXT_HOPm);

    for (i = idx_min; i <= idx_max; i++) {
        SOC_IF_ERROR_RETURN(soc_mem_read(unit, EGR_L3_NEXT_HOPm,
                                         MEM_BLOCK_ANY, i, &nh_entry));
        if (soc_EGR_L3_NEXT_HOPm_field32_get(unit, &nh_entry, ENTRY_TYPEf) != 1) {
            continue; /* MPLS entries only */
        }

        /* Get Index to MAC-DA-PROFILEm */
        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
             macda_idx = soc_EGR_L3_NEXT_HOPm_field32_get
                              (unit, &nh_entry, MPLS__MAC_DA_PROFILE_INDEXf);
        } else {
             macda_idx = soc_EGR_L3_NEXT_HOPm_field32_get
                              (unit, &nh_entry, MAC_DA_PROFILE_INDEXf);
        }

        _bcm_common_profile_mem_ref_cnt_update(unit, EGR_MAC_DA_PROFILEm, 
                                               macda_idx, 1);
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *       bcmi_tr_mpls_vc_swap_hash_dump 
 * Purpose:
 *       dumps the whole hash table
 * Parameters:
 *           IN :  Unit
 * Returns:
 *        void
 */
void bcmi_tr_mpls_vc_swap_hash_dump(int unit)
{   

    bcm_tr_mpls_vc_swap_table_hash_entry_t *temp;
    bcm_tr_mpls_vc_swap_table_hash_t *hash;
    int i, count = 0;


    LOG_ERROR(BSL_LS_SOC_COMMON, (BSL_META_U(unit,
        "printing VC and SWAP hash table\n")));

    if (bcmi_vc_swap_label_hash[unit] != NULL) {
        for (i=0; i< BCM_TR_MPLS_VC_SWAP_HASH_BUCKETS ; i++) {
            hash =  &(bcmi_vc_swap_label_hash[unit][i]);
            temp = hash->head_link;
            if (temp != NULL) {
                while (temp != NULL) {
                    LOG_ERROR(BSL_LS_SOC_COMMON, (BSL_META_U(unit,
                        "hash index = %d : table index = %d \n"), 
                        i, temp->index));
                    temp = temp->link;
                    count++;
                }
            }
        }
        LOG_ERROR(BSL_LS_SOC_COMMON, (BSL_META_U(unit,"\ntotal counts = %d\n"),
             count));
    }
}

/* 
 * Function:
 *     bcmi_tr_mpls_egr_vc_and_swap_table_hash_recover 
 * Purpose:
 *      - Recover MPLS VC Label table sw state.
 * Parameters:
 *      unit    : (IN) Device Unit Number
 * Returns:
 *      BCM_E_XXX
 */
STATIC int 
bcmi_tr_mpls_egr_vc_and_swap_table_hash_recover(int unit)
{
    int idx, hash_idx, vc_swap_index, table_idx;
    int imin, num_vc;
    egr_mpls_vc_and_swap_label_table_entry_t vc_entry;
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);

    imin = soc_mem_index_min(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);
    num_vc = soc_mem_index_count(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);
    num_vc /= 2;
    /* we are diving the table in half to manage counted and non-counted entries. */
    for (idx = imin; idx < num_vc + imin; idx++) {
        if ((!_BCM_MPLS_VC_COUNT_USED_GET(unit, idx - imin)) &&
            (!_BCM_MPLS_VC_NON_COUNT_USED_GET(unit, idx - imin))) {
            continue;
        }

        table_idx = idx;
         if (_BCM_MPLS_VC_NON_COUNT_USED_GET(unit, idx - imin)) {
            table_idx += num_vc;
        }

        if (mpls_info->vc_swap_ref_count[table_idx - imin] <= 0) {
            continue;
        }

        BCM_IF_ERROR_RETURN(soc_mem_read(unit,
            EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
            MEM_BLOCK_ALL, table_idx, &vc_entry));
    
        BCM_IF_ERROR_RETURN(bcmi_tr_mpls_vc_swap_hash_index_get(unit, 
            NULL , 0, NULL, NULL, &vc_entry, &hash_idx));
 
        BCM_IF_ERROR_RETURN(bcmi_tr_mpls_vc_swap_hash_insert(unit, 
            &(bcmi_vc_swap_label_hash[unit][hash_idx]), 
            table_idx, NULL, NULL, NULL, 0, &vc_swap_index, FALSE));
    }
    return BCM_E_NONE;
}
#if defined(BCM_TRIUMPH3_SUPPORT)
STATIC int
_bcm_tr_mpls_egr_vc_and_swap_table_hash_nhop_recover(int unit)
{
    egr_l3_next_hop_entry_t egr_nh;
    int nh_count, idx, entry_type, vc_swap_idx, vc_count;
    int rv = BCM_E_NONE;
    soc_field_t vc_swap_idxf;

    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
        vc_swap_idxf = MPLS__VC_AND_SWAP_INDEXf ;
    } else {
        vc_swap_idxf = VC_AND_SWAP_INDEXf;
    }

    sal_memset(&egr_nh, 0, sizeof(egr_l3_next_hop_entry_t));
    nh_count = soc_mem_index_count(unit, EGR_L3_NEXT_HOPm);
    vc_count = soc_mem_index_count(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);

    for (idx = 0; idx < nh_count; idx++) {
        BCM_IF_ERROR_RETURN (
            soc_mem_read(unit, EGR_L3_NEXT_HOPm,
                         MEM_BLOCK_ANY,  idx, &egr_nh));
        entry_type = soc_EGR_L3_NEXT_HOPm_field32_get(unit,
                                                      &egr_nh, ENTRY_TYPEf);
        if (entry_type == 0x1) {
            vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                           vc_swap_idxf);
            if ((vc_swap_idx) && (vc_swap_idx < vc_count)) {
                (void)_bcm_tr_mpls_vc_and_swap_hash_nhop_add (unit,
                                                              vc_swap_idx,
                                                              idx);
            }
        }
    }
    return rv;
}
#endif
/* 
 * Function:
 *      _bcm_tr_mpls_vc_refcount_recover
 * Purpose:
 *      - Recover MPLS VC Label Reference Count  from HW
 * Parameters:
 *      unit    : (IN) Device Unit Number
 * Returns:
 *      BCM_E_XXX
 */

STATIC int
_bcm_tr_mpls_vc_refcount_recover (int unit)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    egr_l3_next_hop_entry_t egr_nh;
    int nh_count, idx, entry_type, vc_swap_idx, vc_count;
    int rv=BCM_E_NONE;
    soc_field_t vc_swap_idxf;    

    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
        vc_swap_idxf = MPLS__VC_AND_SWAP_INDEXf ;
    } else {
        vc_swap_idxf = VC_AND_SWAP_INDEXf;
    }

    sal_memset(&egr_nh, 0, sizeof(egr_l3_next_hop_entry_t));
    nh_count = soc_mem_index_count(unit, EGR_L3_NEXT_HOPm);
    vc_count = soc_mem_index_count(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);

    for (idx=0; idx<nh_count; idx++) {
         BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm,
                                      MEM_BLOCK_ANY,  idx, &egr_nh));
         entry_type = soc_EGR_L3_NEXT_HOPm_field32_get(unit, 
                                      &egr_nh, ENTRY_TYPEf);
         if (entry_type == 0x1) {
             vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                            vc_swap_idxf);
            if ((vc_swap_idx) && (vc_swap_idx < vc_count)) {
               mpls_info->vc_swap_ref_count[vc_swap_idx]++;
            }
         }
    }
    return rv;
}


/* 
 * Function:
 *      _bcm_tr_mpls_egr_tunnel_refcount_recover
 * Purpose:
 *      - Recover MPLS Egress Tunnel-Label Reference Count  from HW
 * Parameters:
 *      unit    : (IN) Device Unit Number
 * Returns:
 *      BCM_E_XXX
 */

STATIC int
_bcm_tr_mpls_egr_tunnel_refcount_recover (int unit)
{
    egr_ip_tunnel_mpls_entry_t tnl_entry;
    egr_l3_intf_entry_t if_entry;
    int intf_count, idx;
    int mpls_index, tnl_index;
    int rv=BCM_E_NONE;

    sal_memset(&if_entry, 0, sizeof(egr_l3_intf_entry_t));
    intf_count = soc_mem_index_count(unit, EGR_L3_INTFm);

    for (idx=0; idx<intf_count; idx++) {
         BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_INTFm,
                                      MEM_BLOCK_ANY,  idx, &if_entry));
         mpls_index = soc_EGR_L3_INTFm_field32_get(unit, &if_entry, 
                                              MPLS_TUNNEL_INDEXf);
         tnl_index = mpls_index / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);
         rv = READ_EGR_IP_TUNNEL_MPLSm(unit, MEM_BLOCK_ANY, 
                                      tnl_index, &tnl_entry);
         if (rv < 0) {
            return rv;
         }
         if (soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                                ENTRY_TYPEf) == 0x3) {
             /* Increment Ref count */
             _bcm_tr_mpls_egr_tunnel_ref_count_adjust (unit, 
                                                   mpls_index, 1);
        }
    }
    return rv;
}

/* 
 * Function:
 *      _bcm_tr_mpls_match_key_recover
 * Purpose:
 *      - Recover MPLS match key from h/w tables
 * Parameters:
 *      unit    : (IN) Device Unit Number
 * Returns:
 *      BCM_E_XXX
 */
int
_bcm_tr_mpls_match_key_recover(int unit)
{
    int i, vp, port_type;
    int index_min, index_max;
    vlan_xlate_entry_t vent;
    mpls_entry_entry_t mpls_entry;
    source_trunk_map_table_entry_t trunk_entry;
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);

    
    /* Recover match keys from VLAN_XLATEm table */
    index_min = soc_mem_index_min(unit, VLAN_XLATEm);
    index_max = soc_mem_index_max(unit, VLAN_XLATEm);
    for (i = index_min; i <= index_max; i++) {
        SOC_IF_ERROR_RETURN(soc_mem_read(unit,
                                         VLAN_XLATEm, MEM_BLOCK_ANY, i, &vent));
        if (soc_VLAN_XLATEm_field32_get(unit, &vent, VALIDf) == 0) {
            continue;
        }
        if ((soc_VLAN_XLATEm_field32_get(unit, &vent, MPLS_ACTIONf) & 0x1) != 0x1) {
            continue;
        }    

        vp = soc_VLAN_XLATEm_field32_get(unit, &vent, SOURCE_VPf);

        /* Proceed only if this VP belongs to MPLS */
        if (_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
            _bcm_vp_info_t vp_info;
            BCM_IF_ERROR_RETURN(_bcm_vp_info_get(unit, vp, &vp_info));

            if (vp_info.flags & _BCM_VP_INFO_SHARED_PORT) {
                mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_SHARE;
                bcm_tr_mpls_port_match_count_adjust(unit, vp, 1);
                continue;
            }
#endif
            if (soc_VLAN_XLATEm_field32_get(unit, &vent, KEY_TYPEf) ==
             TR_VLXLT_HASH_KEY_TYPE_OVID) {

                mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_VLAN;
                mpls_info->match_key[vp].match_vlan =
                    soc_VLAN_XLATEm_field32_get(unit, &vent, OVIDf);
                if (soc_VLAN_XLATEm_field32_get(unit, &vent, Tf) == 0x1) {
                    mpls_info->match_key[vp].trunk_id = 
                              soc_VLAN_XLATEm_field32_get(unit, &vent, TGIDf);
                } else {
                    mpls_info->match_key[vp].port = 
                            soc_VLAN_XLATEm_field32_get(unit, &vent, PORT_NUMf);
                    mpls_info->match_key[vp].modid = 
                           soc_VLAN_XLATEm_field32_get(unit, &vent, MODULE_IDf);
                }
                bcm_tr_mpls_port_match_count_adjust(unit, vp, 1);
        } else if (soc_VLAN_XLATEm_field32_get(unit, &vent, KEY_TYPEf) ==
             TR_VLXLT_HASH_KEY_TYPE_IVID) {

                mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_INNER_VLAN;
                mpls_info->match_key[vp].match_inner_vlan =
                    soc_VLAN_XLATEm_field32_get(unit, &vent, IVIDf);
                if (soc_VLAN_XLATEm_field32_get(unit, &vent, Tf) == 0x1) {
                    mpls_info->match_key[vp].trunk_id = 
                              soc_VLAN_XLATEm_field32_get(unit, &vent, TGIDf);
                } else {
                    mpls_info->match_key[vp].port = 
                            soc_VLAN_XLATEm_field32_get(unit, &vent, PORT_NUMf);
                    mpls_info->match_key[vp].modid = 
                           soc_VLAN_XLATEm_field32_get(unit, &vent, MODULE_IDf);
                }
                bcm_tr_mpls_port_match_count_adjust(unit, vp, 1);
            } else if (soc_VLAN_XLATEm_field32_get(unit, &vent, KEY_TYPEf) == 
                TR_VLXLT_HASH_KEY_TYPE_IVID_OVID) {

                mpls_info->match_key[vp].flags =
                                         _BCM_MPLS_PORT_MATCH_TYPE_VLAN_STACKED;
                mpls_info->match_key[vp].match_vlan =
                              soc_VLAN_XLATEm_field32_get(unit, &vent, OVIDf);
                mpls_info->match_key[vp].match_inner_vlan = 
                              soc_VLAN_XLATEm_field32_get(unit, &vent, IVIDf);
                if (soc_VLAN_XLATEm_field32_get(unit, &vent, Tf) == 0x1) {
                    mpls_info->match_key[vp].trunk_id =
                              soc_VLAN_XLATEm_field32_get(unit, &vent, TGIDf);
                } else {
                    mpls_info->match_key[vp].port = 
                            soc_VLAN_XLATEm_field32_get(unit, &vent, PORT_NUMf);
                    mpls_info->match_key[vp].modid =
                           soc_VLAN_XLATEm_field32_get(unit, &vent, MODULE_IDf);
                }
                bcm_tr_mpls_port_match_count_adjust(unit, vp, 1);
            } 
#ifdef BCM_KATANA2_SUPPORT
            else if ((soc_VLAN_XLATEm_field32_get(unit, &vent, KEY_TYPEf) == 
                KT2_PT_XLT_KEY_TYPE_LLVID) && soc_feature(unit, soc_feature_lltag)) {

                mpls_info->match_key[vp].flags =
                                         _BCM_MPLS_PORT_MATCH_TYPE_PORT_SUBPORT_PKT_VID;
                mpls_info->match_key[vp].match_subport_pkt_vid =
                              soc_VLAN_XLATEm_field32_get(unit, &vent,
                              LLTAG__LLVIDf);
               
                if (soc_VLAN_XLATEm_field32_get(unit, &vent, Tf) == 0x1) {
                    mpls_info->match_key[vp].trunk_id =
                              soc_VLAN_XLATEm_field32_get(unit, &vent, TGIDf);
                } else {
                    mpls_info->match_key[vp].port = 
                            soc_VLAN_XLATEm_field32_get(unit, &vent, PORT_NUMf);
                    mpls_info->match_key[vp].modid =
                           soc_VLAN_XLATEm_field32_get(unit, &vent, MODULE_IDf);
                }
                bcm_tr_mpls_port_match_count_adjust(unit, vp, 1);
            } else if (soc_VLAN_XLATEm_field32_get(unit, &vent, KEY_TYPEf) == 
                KT2_PT_XLT_KEY_TYPE_LLVID_IVID && soc_feature(unit, soc_feature_lltag)) {

                mpls_info->match_key[vp].flags =
                                         _BCM_MPLS_PORT_MATCH_TYPE_PORT_SUBPORT_PKT_VID_INNER_VLAN;
                mpls_info->match_key[vp].match_subport_pkt_vid =
                              soc_VLAN_XLATEm_field32_get(unit, &vent, 
                              LLTAG__LLVIDf);
                mpls_info->match_key[vp].match_inner_vlan = 
                              soc_VLAN_XLATEm_field32_get(unit, &vent, LLTAG__IVIDf);
                if (soc_VLAN_XLATEm_field32_get(unit, &vent, Tf) == 0x1) {
                    mpls_info->match_key[vp].trunk_id =
                              soc_VLAN_XLATEm_field32_get(unit, &vent, TGIDf);
                } else {
                    mpls_info->match_key[vp].port = 
                            soc_VLAN_XLATEm_field32_get(unit, &vent, PORT_NUMf);
                    mpls_info->match_key[vp].modid =
                            soc_VLAN_XLATEm_field32_get(unit, &vent, MODULE_IDf);
                }
                bcm_tr_mpls_port_match_count_adjust(unit, vp, 1);
            } else if (soc_VLAN_XLATEm_field32_get(unit, &vent, KEY_TYPEf) == 
                KT2_PT_XLT_KEY_TYPE_LLVID_OVID && soc_feature(unit, soc_feature_lltag)) {

                mpls_info->match_key[vp].flags =
                                         _BCM_MPLS_PORT_MATCH_TYPE_PORT_SUBPORT_PKT_VID_OUTER_VLAN;
                mpls_info->match_key[vp].match_subport_pkt_vid =
                              soc_VLAN_XLATEm_field32_get(unit, &vent, 
                              LLTAG__LLVIDf);
                mpls_info->match_key[vp].match_vlan = 
                              soc_VLAN_XLATEm_field32_get(unit, &vent, LLTAG__OVIDf);
                if (soc_VLAN_XLATEm_field32_get(unit, &vent, Tf) == 0x1) {
                    mpls_info->match_key[vp].trunk_id =
                              soc_VLAN_XLATEm_field32_get(unit, &vent, TGIDf);
                } else {
                    mpls_info->match_key[vp].port = 
                            soc_VLAN_XLATEm_field32_get(unit, &vent, PORT_NUMf);
                    mpls_info->match_key[vp].modid =
                           soc_VLAN_XLATEm_field32_get(unit, &vent, MODULE_IDf);
                }
                bcm_tr_mpls_port_match_count_adjust(unit, vp, 1);
            }
#endif /* BCM_KATANA2_SUPPORT */ 

#ifdef BCM_TRIUMPH2_SUPPORT    /* Recover flex stats */
            if (soc_feature(unit, soc_feature_gport_service_counters)) {
                BCM_IF_ERROR_RETURN(_bcm_tr_mpls_flex_stat_recover(unit, vp));
            }
#endif /* BCM_TRIUMPH2_SUPPORT */

            /* Recover TPID ref count */
            BCM_IF_ERROR_RETURN(_bcm_tr_mpls_source_vp_tpid_recover(unit, vp));
        }
    }

    /* Recover misc associated data */
    BCM_IF_ERROR_RETURN(_bcm_tr_mpls_associated_data_recover(unit));

    /* Recover match keys from SOURCE_TRUNK_MAP_TABLEm table */
    index_min = soc_mem_index_min(unit, SOURCE_TRUNK_MAP_TABLEm);
    index_max = soc_mem_index_max(unit, SOURCE_TRUNK_MAP_TABLEm);
    for (i = index_min; i <= index_max; i++) {
        SOC_IF_ERROR_RETURN(soc_mem_read(unit, SOURCE_TRUNK_MAP_TABLEm,
                                               MEM_BLOCK_ANY, i, &trunk_entry));

        vp = soc_SOURCE_TRUNK_MAP_TABLEm_field32_get(unit,
                                             &trunk_entry, SOURCE_VPf);

        /* Proceed only if this VP belongs to MPLS */
        if (_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {

            port_type = soc_SOURCE_TRUNK_MAP_TABLEm_field32_get(unit,
                                                     &trunk_entry, PORT_TYPEf); 
            if (0 == port_type) { /* Normal port */
                mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_PORT;
                mpls_info->match_key[vp].index = i;
            } else if(1 == port_type) { /* Trunk port */
                mpls_info->match_key[vp].flags =_BCM_MPLS_PORT_MATCH_TYPE_TRUNK;
                mpls_info->match_key[vp].trunk_id = 
                soc_SOURCE_TRUNK_MAP_TABLEm_field32_get(unit, &trunk_entry,
                                                                         TGIDf);
                mpls_info->trunk_vp_map[mpls_info->match_key[vp].trunk_id] = vp;
            }

#ifdef BCM_TRIUMPH2_SUPPORT    /* Recover flex stats */
            if (soc_feature(unit, soc_feature_gport_service_counters)) {
                BCM_IF_ERROR_RETURN(_bcm_tr_mpls_flex_stat_recover(unit, vp));
            }
#endif /* BCM_TRIUMPH2_SUPPORT */

            /* Recover TPID ref count */
            BCM_IF_ERROR_RETURN(_bcm_tr_mpls_source_vp_tpid_recover(unit, vp));
        }
    }

    /* Recover match keys from MPLS_ENTRYm table */
    index_min = soc_mem_index_min(unit, MPLS_ENTRYm);
    index_max = soc_mem_index_max(unit, MPLS_ENTRYm);
    for (i = index_min; i <= index_max; i++) {
        SOC_IF_ERROR_RETURN(soc_mem_read(unit, MPLS_ENTRYm,
                                                MEM_BLOCK_ANY, i, &mpls_entry));
        if (soc_MPLS_ENTRYm_field32_get(unit, &mpls_entry, VALIDf) == 0) {
            continue;
        }
        if ((soc_MPLS_ENTRYm_field32_get(unit,
               &mpls_entry, MPLS_ACTION_IF_BOSf) != 0x1) || 
            (soc_MPLS_ENTRYm_field32_get(unit,
               &mpls_entry, MPLS_ACTION_IF_NOT_BOSf) != 0x0)) {
#if defined(BCM_SABER2_SUPPORT)
            /* SB2: IF not BOS field is set for VCCV type 4.
             * It needs to be recovered.
             */
            if (!(SOC_IS_SABER2(unit) &&
                (soc_MPLS_ENTRYm_field32_get(unit,
                &mpls_entry, MPLS_ACTION_IF_NOT_BOSf) == 0x5)))
#endif
            {
                continue;
            }
        }

        vp = soc_MPLS_ENTRYm_field32_get(unit, &mpls_entry, SOURCE_VPf);

        /* Proceed only if this VP belongs to MPLS */
        if (_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
            bcm_module_t  modid;
            bcm_port_t    local_port;
            bcm_trunk_t   trunk_id;
            int           local_id;
            bcm_gport_t   mpls_port_id;
            bcm_trunk_t   mpls_tid = 0;
            bcm_port_t    mpls_local_port = 0;
            int           mpls_local_modid = 0;
            int           rv;

            BCM_GPORT_MPLS_PORT_ID_SET(mpls_port_id,vp);
            rv = _bcm_esw_gport_resolve(unit, mpls_port_id, &modid,
                                           &local_port, &trunk_id,
                                           &local_id);
            if (rv != BCM_E_NONE) {
                continue;
            }

            if (soc_MPLS_ENTRYm_field32_get(unit, &mpls_entry, Tf) == 0x1) {
                mpls_tid = soc_MPLS_ENTRYm_field32_get(unit, &mpls_entry, TGIDf);
                if (mpls_tid == trunk_id) {
                    mpls_info->match_key[vp].trunk_id = mpls_tid;
                } else {
                    mpls_info->match_key[vp].fo_trunk_id = mpls_tid;
                }
                mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_LABEL_PORT;
            } else {
                mpls_local_port =
                        soc_MPLS_ENTRYm_field32_get(unit, &mpls_entry, PORT_NUMf);
                mpls_local_modid =
                           soc_MPLS_ENTRYm_field32_get(unit, &mpls_entry, MODULE_IDf);
               
                if ((mpls_local_port == 0) && (mpls_local_modid == 0)) {
                    mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_LABEL;
                    mpls_info->match_key[vp].port  = mpls_local_port;
                    mpls_info->match_key[vp].modid = mpls_local_modid;
                } else {
                    mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_LABEL_PORT;
                    if (mpls_local_port == local_port && mpls_local_modid == modid) {
                        mpls_info->match_key[vp].port  = mpls_local_port;
                        mpls_info->match_key[vp].modid = mpls_local_modid;
                    } else {
                        mpls_info->match_key[vp].fo_port  = mpls_local_port;
                        mpls_info->match_key[vp].fo_modid = mpls_local_modid;
                    }
                }
            } 
            mpls_info->match_key[vp].match_label = 
                    soc_MPLS_ENTRYm_field32_get(unit, &mpls_entry, MPLS_LABELf);
            
#ifdef BCM_TRIUMPH2_SUPPORT    /* Recover flex stats */
            if (soc_feature(unit, soc_feature_gport_service_counters)) {
                BCM_IF_ERROR_RETURN
                    (_bcm_tr_mpls_flex_stat_recover(unit, vp));
                _bcm_tr_mpls_label_flex_stat_recover(unit, &mpls_entry);
            }
#endif /* BCM_TRIUMPH2_SUPPORT */

            /* Recover TPID ref count */
            BCM_IF_ERROR_RETURN(_bcm_tr_mpls_source_vp_tpid_recover(unit, vp));
        }
    }

#ifdef BCM_TRIDENT2PLUS_SUPPORT
    index_min = soc_mem_index_min(unit, SOURCE_VPm);
    index_max = soc_mem_index_max(unit, SOURCE_VPm);
    for (i = index_min; i <= index_max; i++) {
        if (_bcm_vp_used_get(unit, i, _bcmVpTypeMpls)) {
            _bcm_vp_info_t vp_info;
            BCM_IF_ERROR_RETURN(_bcm_vp_info_get(unit, i, &vp_info));

            if (vp_info.flags & _BCM_VP_INFO_SHARED_PORT) {
                mpls_info->match_key[i].flags = _BCM_MPLS_PORT_MATCH_TYPE_SHARE;
            }
        }
    }
#endif /* BCM_TRIDENT2PLUS_SUPPORT */

#ifdef BCM_TRIUMPH2_SUPPORT    /* Recover flex stats */
    if (soc_feature(unit, soc_feature_gport_service_counters)) {
        index_min = soc_mem_index_min(unit, SOURCE_VPm);
        index_max = soc_mem_index_max(unit, SOURCE_VPm);
        for (i = index_min; i <= index_max; i++) {
            if ((!_bcm_vp_used_get(unit, i, _bcmVpTypeMpls)) ||
                (mpls_info->match_key[i].flags != 0)){
                continue;
            }

            BCM_IF_ERROR_RETURN(_bcm_tr_mpls_flex_stat_recover(unit, i));
        }
    }
#endif /* BCM_TRIUMPH2_SUPPORT */

    return BCM_E_NONE;
}

/* 
 * Function:
 *      _bcm_tr_mpls_vpws_vp_map_recover
 * Purpose: 
 *     Reconstruct the VPWS VP to VPN mapping info 
 *     from h/w by traversing the source VP table.
 *     NOTE: This is possible oly for sdk versions 
 *     where the VPWS VPS are vp and vp+1.
 * Parameters:
 *      unit    : (IN) Device Unit Number
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_vpws_vp_map_recover(int unit)
{
    int i, idx_min, idx_max;
    source_vp_entry_t svp_entry; 
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);

    idx_min = soc_mem_index_min(unit, SOURCE_VPm);
    idx_max = soc_mem_index_max(unit, SOURCE_VPm);

    for (i = idx_min; i <=idx_max; i++) {
        SOC_IF_ERROR_RETURN(soc_mem_read(unit,
                            SOURCE_VPm, MEM_BLOCK_ANY, i, &svp_entry));
        if (soc_SOURCE_VPm_field32_get(unit, &svp_entry, ENTRY_TYPEf) != 2) {
            continue; /* Looking for VPWS VPs only */
        }
        mpls_info->vpws_vp_map[(i>>1)].vp1 = i;
        mpls_info->vpws_vp_map[(i>>1)].vp2 = (i+1);
        i++; /* Skip past the immediate next VP as it belongs to this VPWS */
    }
    return BCM_E_NONE;
}

/* 
 * Function:
 *      _bcm_tr_mpls_extended_reinit
 * Purpose:
 *      - Called when persistent memory adequate for
 *            MPLS state recover is available.
 *      - In Cold Boot sequence, scache space is allocated 
 *            in persistent memory.
 *      - In Warm Boot sequence, state stored in persistent 
 *            memory is retrieved to reconstrunct MPLS 
 *            book keeping structure.       
 * Parameters:
 *      unit    : (IN) Device Unit Number
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_extended_reinit(int unit)
{
    int       i, rv = BCM_E_NONE;
    int       stable_size, alloc_sz;
    int       num_pw_term, num_pw_init;
    int       num_ip_tnl, num_mpls_map, num_nh;
    int       num_vrf, num_vp, num_vc;
    int       num_ing_exp_map, num_egr_l2_exp_map;
#ifdef BCM_APACHE_SUPPORT
    int       num_ing_exp_to_phb_cng_map = 0, num_egr_exp_to_phb_cng_map = 0;
#endif
    int       vpws_vp_map_size;
    int       idx_min, idx_max;
    uint8     *mpls_state, *mpls_state_marker;
    uint16    recovered_ver;
    soc_scache_handle_t scache_handle;
    uint8*    egr_nh_tbl_buf = NULL;
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    uint32 nh_if, primary_nh_if, bitmask;
    int       additional_scache_sz = 0;
    int vp3_recovery_reqd = 0;
    uint32 recovered_sz = 0;
    int alloc_ver1_3_sz = 0;
    int        old_num_vp = 0;
#if defined(BCM_SABER2_SUPPORT)
    uint32     source_vp_sb2_5626x = 0;
#endif /* BCM_SABER2_SUPPORT */

    num_mpls_map = soc_mem_index_count(unit,
                        EGR_MPLS_EXP_MAPPING_1m) / 64;

    if (SOC_WARM_BOOT(unit)) {
    /* Warm Boot */
        alloc_sz = 0;
        alloc_ver1_3_sz = 0;
        SOC_SCACHE_HANDLE_SET(scache_handle, unit, BCM_MODULE_MPLS, 0);
        BCM_IF_ERROR_RETURN(_bcm_tr_mpls_scache_size_get(unit, &alloc_sz));
        if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_1)  {
            /* Although the sync and recover is changed to individual fields,
             * we are still allocating full structure to start with.
             * At this point, we do not know from which version we are 
             * coming from so if we are coming from a version which had
             * more space then we will reduce alloc space here.
             * We will anyways reduce the space when we will recover
             * each field based on versions below.
             */
            alloc_sz += sizeof(_bcm_mpls_vpws_vp_map_info_t) *
                     (soc_mem_index_count(unit, SOURCE_VPm)/2);
        }

        if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_2) {
            alloc_sz += SHR_BITALLOCSIZE(num_mpls_map);
        }

        if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_3) {
            alloc_sz += (num_mpls_map * sizeof(uint32) -
                         SHR_BITALLOCSIZE(num_mpls_map));
        }
        alloc_ver1_3_sz = alloc_sz;

        SOC_IF_ERROR_RETURN(soc_stable_size_get(unit, &stable_size));
        SOC_IF_ERROR_RETURN(soc_scache_ptr_get(unit, scache_handle,
                                                &mpls_state, &recovered_sz));
        recovered_sz -= SOC_WB_SCACHE_CONTROL_SIZE;
        vp3_recovery_reqd = (recovered_sz == alloc_ver1_3_sz) ? 1 : 0;

        if (stable_size > alloc_sz) {
            rv = _bcm_esw_scache_ptr_get(unit,
                                    scache_handle, FALSE,
                                    alloc_sz, &mpls_state,
                                    BCM_WB_DEFAULT_VERSION, &recovered_ver);

            if (BCM_FAILURE(rv) && (rv != BCM_E_NOT_FOUND)) {
                return rv;
            }

            if (NULL == mpls_state) {
                return BCM_E_MEMORY;
            }
   
            /* Maximum virtual port supported on saber2 5626x devices are 4K
             * This fix is related to scaling down of vpn from 8K to 4K
             */
#if defined(BCM_SABER2_SUPPORT)
            if (soc_sb2_5626x_devtype(unit)) {
                sal_memcpy(&source_vp_sb2_5626x, mpls_state,
                        sizeof(source_vp_sb2_5626x));
                if (source_vp_sb2_5626x == 0xFED5ABC7) {
                    mpls_state += sizeof(source_vp_sb2_5626x);
                } else {
                    old_num_vp = 1;
                }
            }
#endif /* BCM_SABER2_SUPPORT */
            num_vrf = SOC_CONTROL(unit)->max_vrf_id + 1;
            num_vp = soc_mem_index_count(unit, SOURCE_VPm);
            num_vc = soc_mem_index_count(unit,
                                             EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);
            num_ip_tnl = soc_mem_index_count(unit, EGR_IP_TUNNEL_MPLSm);
            /*EGR_MPLS_EXP_MAPPING_1 profiles indexed by ((int_pri <<2) + cng)*/
            num_ing_exp_map = soc_mem_index_count(unit,
                                                     ING_MPLS_EXP_MAPPINGm) / 8;
            num_egr_l2_exp_map = soc_mem_index_count(unit,
                                                 EGR_MPLS_EXP_PRI_MAPPINGm) / 8;
#ifdef BCM_APACHE_SUPPORT
            if (soc_feature(unit, soc_feature_mpls_exp_to_phb_cng_map)) {
                num_ing_exp_to_phb_cng_map = soc_mem_index_count(unit,
                        ING_MPLS_EXP_MAPPING_1m) / 16;
                num_egr_exp_to_phb_cng_map = soc_mem_index_count(unit,
                        EGR_MPLS_EXP_MAPPING_3m) / 8;
            }
#endif
            num_pw_term = 0;
            if (SOC_MEM_IS_VALID(unit, ING_PW_TERM_COUNTERSm)) {
                num_pw_term = soc_mem_index_count(unit, ING_PW_TERM_COUNTERSm);
            }
            num_pw_init = 0;
            if (SOC_MEM_IS_VALID(unit, EGR_PW_INIT_COUNTERSm)) {
                num_pw_init = soc_mem_index_count(unit, EGR_PW_INIT_COUNTERSm);
            }
            num_nh = soc_mem_index_count(unit, EGR_L3_NEXT_HOPm);
            SHR_BITCOPY_RANGE(mpls_info->vrf_bitmap, 0,
                              (SHR_BITDCL *)mpls_state, 0, num_vrf);
            mpls_state += SHR_BITALLOCSIZE(num_vrf);
            SHR_BITCOPY_RANGE(mpls_info->vpws_bitmap, 0,
                              (SHR_BITDCL *)mpls_state, 0, (num_vp/2));
            if (old_num_vp) {
                mpls_state += SHR_BITALLOCSIZE(num_vp);
            } else {
                mpls_state += SHR_BITALLOCSIZE(num_vp/2);
            }
            SHR_BITCOPY_RANGE(mpls_info->vc_c_bitmap, 0,
                              (SHR_BITDCL *)mpls_state, 0, num_vc);
            mpls_state += SHR_BITALLOCSIZE(num_vc);
            SHR_BITCOPY_RANGE(mpls_info->vc_nc_bitmap, 0,
                              (SHR_BITDCL *)mpls_state, 0, num_vc);
            mpls_state += SHR_BITALLOCSIZE(num_vc);
            if (num_pw_term) {
                SHR_BITCOPY_RANGE(mpls_info->pw_term_bitmap, 0,
                                  (SHR_BITDCL *)mpls_state, 0, num_pw_term);
                mpls_state += SHR_BITALLOCSIZE(num_pw_term);
            }
         
            if (num_pw_init) {
                SHR_BITCOPY_RANGE(mpls_info->pw_init_bitmap, 0,
                                  (SHR_BITDCL *)mpls_state, 0, num_pw_init); 
                mpls_state += SHR_BITALLOCSIZE(num_pw_init);
            }
            SHR_BITCOPY_RANGE(mpls_info->egr_mpls_bitmap, 0,
                              (SHR_BITDCL *)mpls_state, 0, num_mpls_map);
            mpls_state += SHR_BITALLOCSIZE(num_mpls_map);
            SHR_BITCOPY_RANGE(mpls_info->ing_exp_map_bitmap, 0,
                              (SHR_BITDCL *)mpls_state, 0, num_ing_exp_map);
            mpls_state += SHR_BITALLOCSIZE(num_ing_exp_map);
            SHR_BITCOPY_RANGE(mpls_info->egr_l2_exp_map_bitmap, 0,
                              (SHR_BITDCL *)mpls_state, 0, num_egr_l2_exp_map);
            mpls_state += SHR_BITALLOCSIZE(num_egr_l2_exp_map);
            
            SHR_BITCOPY_RANGE(mpls_info->tnl_bitmap, 0,
                              (SHR_BITDCL *)mpls_state, 0, (num_ip_tnl *
                                  _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit)));
            mpls_state += SHR_BITALLOCSIZE(num_ip_tnl *
                    _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit));
            SHR_BITCOPY_RANGE(mpls_info->ip_tnl_bitmap, 0,
                              (SHR_BITDCL *)mpls_state, 0, num_ip_tnl);
            mpls_state += SHR_BITALLOCSIZE(num_ip_tnl);
            mpls_state_marker = mpls_state;
       
            /* Reconstruct match_index by reading tables */
            rv = _bcm_esw_mpls_match_key_recover(unit);
            if (BCM_FAILURE(rv)) {
                return rv;
            }

            /* Reconstruct VC RefCount */
            rv = _bcm_tr_mpls_vc_refcount_recover(unit);
            if (BCM_FAILURE(rv)) {
                return rv;
            }

            /* Reconstruct Egress MPLS Tunnel-Label RefCount */
            rv = _bcm_tr_mpls_egr_tunnel_refcount_recover(unit);
            if (BCM_FAILURE(rv)) {
                return rv;
            }

            /* Reconstruct hash for egr VC and swap table */
            rv = bcmi_tr_mpls_egr_vc_and_swap_table_hash_recover(unit);
            if (BCM_FAILURE(rv)) {
                return rv;
            }

#if defined(BCM_TRIUMPH3_SUPPORT)
            /* Reconstruct intf hash for egr VC and swap table */
            if (soc_feature(unit, soc_feature_vc_and_swap_table_overlaid)) {
                rv = _bcm_tr_mpls_egr_vc_and_swap_table_hash_nhop_recover(unit);
                if (BCM_FAILURE(rv)) {
                    return rv;
                }
            }
#endif
#if defined(BCM_APACHE_SUPPORT) || defined(BCM_TOMAHAWK2_SUPPORT)
            if (soc_feature(unit, soc_feature_egr_ip_tnl_mpls_double_wide)) {
                rv = bcmi_xgs5_mpls_tunnel_initiator_reinit(unit);
                if (BCM_FAILURE(rv)) {
                    return rv;
                }
            }
#endif
            /* reconstruct the hash and linked lists */
            idx_min = soc_mem_index_min(unit, EGR_L3_NEXT_HOPm);
            idx_max = soc_mem_index_max(unit, EGR_L3_NEXT_HOPm);
            egr_nh_tbl_buf = soc_cm_salloc(unit, SOC_MEM_TABLE_BYTES(unit,
                                           EGR_L3_NEXT_HOPm),
                                           "egr_l3_next_hop");
            if (egr_nh_tbl_buf == NULL) {
                return BCM_E_MEMORY;
            }
                                            
            rv = soc_mem_read_range(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY,
                            idx_min, idx_max, egr_nh_tbl_buf);
            if (SOC_FAILURE(rv)) {
                goto cleanup;
            }

            primary_nh_if = 0;
            bitmask = 1 << ((8 * sizeof(uint32)) - 1);
            sal_memcpy(&nh_if, mpls_state, sizeof(uint32));
            while (nh_if != 0xdeadbeef) {
                if (nh_if & bitmask) {
                    primary_nh_if = (nh_if & ~bitmask);
                }
                if (primary_nh_if != (nh_if & ~bitmask)) {
                    BCM_IF_ERROR_RETURN(_bcm_tr_mpls_add_egrNhop(unit, primary_nh_if));
                    BCM_IF_ERROR_RETURN(_bcm_tr_mpls_add_vp_nh(unit, primary_nh_if, nh_if));
                    BCM_XGS3_L3_ENT_REF_CNT_INC(BCM_XGS3_L3_TBL_PTR(unit, next_hop),
                       (primary_nh_if) ? (primary_nh_if - BCM_XGS3_EGRESS_IDX_MIN) : 0,
                                                                     _BCM_SINGLE_WIDE);
                }
                mpls_state += sizeof(uint32);
                sal_memcpy(&nh_if, mpls_state, sizeof(uint32));
            }
            mpls_state = mpls_state_marker + (sizeof(uint32) * num_nh);

            /* Reconstruct VPWS VP map from the scache */
            if (recovered_ver >= BCM_WB_VERSION_1_1) {
                mpls_state += sizeof(int);
                for (i=0; i<(num_vp/2); i++) {
                    if(recovered_ver <= BCM_WB_VERSION_1_3) {
                        sal_memcpy(&mpls_info->vpws_vp_map[i].vp1, mpls_state,
                                   sizeof(int));
                        mpls_state += sizeof(int);
                        sal_memcpy(&mpls_info->vpws_vp_map[i].vp2, mpls_state,
                                   sizeof(int));
                        mpls_state += sizeof(int);
                        if(vp3_recovery_reqd) {
                            sal_memcpy(&mpls_info->vpws_vp_map[i].vp3,
                                       mpls_state, sizeof(int));
                            mpls_state += sizeof(int);
                        }
                    } else { /* if recovered_ver > BCM_WB_VERSION_1_3 */
                        sal_memcpy(&mpls_info->vpws_vp_map[i].vp1, mpls_state,
                                   sizeof(int));
                        mpls_state += sizeof(int);
                        sal_memcpy(&mpls_info->vpws_vp_map[i].vp2, mpls_state,
                                   sizeof(int));
                        mpls_state += sizeof(int);
                        sal_memcpy(&mpls_info->vpws_vp_map[i].vp3, mpls_state,
                                   sizeof(int));
                        mpls_state += sizeof(int);

                        if ((recovered_ver >= BCM_WB_VERSION_1_8) &&
                            soc_feature(unit, soc_feature_hierarchical_protection)) {

                            sal_memcpy(&mpls_info->vpws_vp_map[i].vp_b_1, mpls_state,
                                   sizeof(int));
                            mpls_state += sizeof(int);
                            sal_memcpy(&mpls_info->vpws_vp_map[i].vp_b_2, mpls_state,
                                   sizeof(int));
                            mpls_state += sizeof(int);
                            sal_memcpy(&mpls_info->vpws_vp_map[i].vp_b_3, mpls_state,
                                   sizeof(int));
                            mpls_state += sizeof(int);
                        } else if (recovered_ver == BCM_WB_VERSION_1_8) {
                            /*  As part of version 8, we allocated space common
                             *  for all devices without any soc checks because
                             *  whole structure was synced. as part of 1.9
                             *  version, the structure is broken to sync/alloc
                             *  each field separately.
                             */
                            mpls_state += (3 * sizeof(int));
                        }
                    }
                }
                if (old_num_vp) {
                    if(recovered_ver <= BCM_WB_VERSION_1_3) {
                        mpls_state += ((sizeof(int) + sizeof(int)) *
                                (num_vp - (num_vp/2)));
                        if(vp3_recovery_reqd) {
                            mpls_state += (sizeof(int) *
                                    (num_vp - (num_vp/2)));
                        }
                    } else {
                        mpls_state += ((sizeof(int) * 3) *
                                (num_vp - (num_vp/2)));
                        if (recovered_ver >= BCM_WB_VERSION_1_8) {
                            mpls_state += ((sizeof(int) * 3) *
                                    (num_vp - (num_vp/2)));
                        }
                    }
                }
            } else { /* Reconstruct VPWS VP map from h/w */
                rv = _bcm_tr_mpls_vpws_vp_map_recover(unit);
                if (BCM_FAILURE(rv)) {
                    goto cleanup;
                }
                if (old_num_vp) {
                    vpws_vp_map_size = sizeof(_bcm_mpls_vpws_vp_map_info_t) *
                        num_vp;
                } else {
                    vpws_vp_map_size = sizeof(_bcm_mpls_vpws_vp_map_info_t) *
                        (soc_mem_index_count(unit, SOURCE_VPm)/2);
                }
                rv = soc_scache_realloc(unit, scache_handle, vpws_vp_map_size);
                if (SOC_FAILURE(rv)) {
                    goto cleanup;
                }
            }

            /* Recover the MPLS map entry */
            if (recovered_ver >= BCM_WB_VERSION_1_3) {
                sal_memcpy(mpls_info->egr_mpls_hw_idx, mpls_state,
                           (num_mpls_map * sizeof(uint32)));
                mpls_state += (num_mpls_map * sizeof(uint32));

                for (i = 0; i < num_mpls_map; i++) {
                    if (_BCM_EGR_MPLS_MAP_USED_GET(unit, i)) {
                        rv = _bcm_egr_mpls_combo_map_entry_reference(unit, 
                                ((mpls_info->egr_mpls_hw_idx[i]) * 64), 64);
                        if (BCM_FAILURE(rv)) {
                            goto cleanup;
                        }
                    }
                }
            } else if (recovered_ver >= BCM_WB_VERSION_1_2) {
                /* Can not get correct data from VERSION_1_2 */
                additional_scache_sz = num_mpls_map * sizeof(uint32) -
                    SHR_BITALLOCSIZE(num_mpls_map);
            } else {
                additional_scache_sz = num_mpls_map * sizeof(uint32);
            }

            if ( (recovered_ver >= BCM_WB_VERSION_1_1) && 
                 (recovered_ver <= BCM_WB_VERSION_1_3) ){
                if (!vp3_recovery_reqd) {
                    if (old_num_vp) {
                        additional_scache_sz += sizeof(int) * num_vp;
                    } else {
                        additional_scache_sz += sizeof(int) * (num_vp/2);
                    }
                }
            }
            if (!(soc_feature(unit, soc_feature_hierarchical_protection))) {

                /* if 2 level protection is not enabled then we
                 * will have to allocate space only for version 1.8 as we
                 * sync the whole structure there. from 1.9 we changed
                 * sync of whole strcuture to indivisual fields.
                 * going from 1.8 to 1.9 we will have to remove the space also.
                 */
                if (recovered_ver == BCM_WB_VERSION_1_8) {
                   if (old_num_vp) {
                        additional_scache_sz -= (3 *sizeof(int)) * num_vp;
                    } else {
                        additional_scache_sz -= (3 *sizeof(int)) * (num_vp/2);
                    }
                }
            } else {
                /* Adding state for 3 more backup VPs */
                if (recovered_ver < BCM_WB_VERSION_1_8) {
                    if (old_num_vp) {
                        additional_scache_sz += (3 *sizeof(int)) * num_vp;
                    } else {
                        additional_scache_sz += (3 *sizeof(int)) * (num_vp/2);
                    }
                }
            }

            /* Recover the MPLS svp profile set bitmap */
            if (recovered_ver >= BCM_WB_VERSION_1_5) {
                SHR_BITCOPY_RANGE(mpls_info->svp_profile_set_bitmap, 0,
                                  (SHR_BITDCL *)mpls_state, 0, num_vp);
                if (old_num_vp) {
                    mpls_state += SHR_BITALLOCSIZE((num_vp * 2));
                } else {
                    mpls_state += SHR_BITALLOCSIZE(num_vp);
                }
#ifdef BCM_TRIUMPH2_SUPPORT 
                rv = _bcm_tr_mpls_source_vp_recover(unit);
                if (BCM_FAILURE(rv)) {
                    goto cleanup;
                }
#endif
            } else {
                if (old_num_vp) {
                    additional_scache_sz += SHR_BITALLOCSIZE((num_vp * 2));
                } else {
                    additional_scache_sz += SHR_BITALLOCSIZE(num_vp);
                }
            }

            /* Recover the network VP bitmap */
            if (recovered_ver >= BCM_WB_VERSION_1_6) {
                SHR_BITCOPY_RANGE(mpls_info->network_vp_bitmap, 0,
                        (SHR_BITDCL *)mpls_state, 0, num_vp);
                if (old_num_vp) {
                    mpls_state += SHR_BITALLOCSIZE((num_vp * 2));
                } else {
                    mpls_state += SHR_BITALLOCSIZE(num_vp);
                }
             } else {
                if (old_num_vp) {
                    additional_scache_sz += SHR_BITALLOCSIZE((num_vp * 2));
                } else {
                    additional_scache_sz += SHR_BITALLOCSIZE(num_vp);
                }
            }
#ifdef BCM_APACHE_SUPPORT
            if (soc_feature(unit, soc_feature_mpls_exp_to_phb_cng_map)) {
                /* Recover MPLS EXP to PHB and CNG maps */ 
                if (recovered_ver >= BCM_WB_VERSION_1_7) {
                    SHR_BITCOPY_RANGE(mpls_info->ing_exp_to_phb_cng_map_bitmap, 0,
                            (SHR_BITDCL *)mpls_state, 0, 
                            num_ing_exp_to_phb_cng_map);
                    mpls_state += SHR_BITALLOCSIZE(num_ing_exp_to_phb_cng_map);
                    SHR_BITCOPY_RANGE(mpls_info->egr_exp_to_phb_cng_map_bitmap, 0,
                            (SHR_BITDCL *)mpls_state, 0, 
                            num_egr_exp_to_phb_cng_map);
                    mpls_state += SHR_BITALLOCSIZE(num_egr_exp_to_phb_cng_map);
                } else {
                    additional_scache_sz +=
                            (SHR_BITALLOCSIZE(num_ing_exp_to_phb_cng_map) +
                                SHR_BITALLOCSIZE(num_egr_exp_to_phb_cng_map));
                }
            }
#endif
#ifdef BCM_TRIUMPH2_SUPPORT
            if (soc_feature(unit, soc_feature_mpls_failover)) {
                /* Recover failover vp */ 
                if (recovered_ver >= BCM_WB_VERSION_1_10) {
                    sal_memcpy(mpls_info->failover_vp, (SHR_BITDCL *)mpls_state, 
                               num_vp * sizeof(uint32));
                    mpls_state += num_vp * sizeof(uint32);
                } else {
                    additional_scache_sz += num_vp * sizeof(uint32);
                }
            }
#endif

            if (additional_scache_sz != 0) {
                rv = soc_scache_realloc(unit, scache_handle, additional_scache_sz);
                if (SOC_FAILURE(rv)) {
                    goto cleanup;
                }
            }
        }
        /* Egress Mpls Tunnel related flex stat reloaded after Mpls
         * SW structures are populated since the flow require Mpls
         * book-keeping information.
         */
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
        if (soc_feature(unit,soc_feature_advanced_flex_counter)) {
            _bcm_esw_stat_flex_check_egress_mpls_tunnel_table(unit);
        }
#endif
    } else {
    /* Cold Boot */
        alloc_sz = 0;
        BCM_IF_ERROR_RETURN(_bcm_tr_mpls_scache_size_get(unit, &alloc_sz));
        /* Allocate MPLS scache for the VPWS VP map */
        if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_1) {
            alloc_sz += sizeof(_bcm_mpls_vpws_vp_map_info_t) * 
                     (soc_mem_index_count(unit, SOURCE_VPm)/2);  
        }

        /* Allocate scache for the MPLS map entry */
        if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_2) {
            alloc_sz += SHR_BITALLOCSIZE(num_mpls_map);  
        }

        if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_3) {
            alloc_sz += (num_mpls_map * sizeof(uint32) -
                         SHR_BITALLOCSIZE(num_mpls_map));
        }
        
        if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_5) {
            alloc_sz += SHR_BITALLOCSIZE(soc_mem_index_count(unit, SOURCE_VPm));
        }

        /* For network_vp_bitmap */
        if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_6) {
            alloc_sz += SHR_BITALLOCSIZE(soc_mem_index_count(unit, SOURCE_VPm));
        }
#ifdef BCM_APACHE_SUPPORT
        /* For MPLS EXP to PHB and CNG maps */
        if (soc_feature(unit, soc_feature_mpls_exp_to_phb_cng_map)) {
            if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_7) {
                alloc_sz += SHR_BITALLOCSIZE(soc_mem_index_count(unit, 
                            ING_MPLS_EXP_MAPPING_1m) / 16);
                alloc_sz += SHR_BITALLOCSIZE(soc_mem_index_count(unit, 
                            EGR_MPLS_EXP_MAPPING_3m) / 8);
            }
        }
#endif
        /* Maximum virtual port supported on saber2 5626x devices are 4K
         * This fix is related to scaling down of vpn from 8K to 4K
         */
#if defined(BCM_SABER2_SUPPORT)
        if (soc_sb2_5626x_devtype(unit)) {
            alloc_sz += sizeof(source_vp_sb2_5626x);
        }
#endif /* BCM_SABER2_SUPPORT */
#ifdef BCM_TRIUMPH2_SUPPORT
        if (soc_feature(unit, soc_feature_mpls_failover)) {
            num_vp = soc_mem_index_count(unit, SOURCE_VPm);
            alloc_sz += num_vp * sizeof(uint32);
        }
#endif

        SOC_SCACHE_HANDLE_SET(scache_handle, unit, BCM_MODULE_MPLS, 0);

        rv = _bcm_esw_scache_ptr_get(unit, scache_handle, TRUE,
                                     alloc_sz, &mpls_state, 
                                     BCM_WB_DEFAULT_VERSION, NULL);

        if (BCM_FAILURE(rv) && (rv != BCM_E_NOT_FOUND)) {
            return rv;
        }
    
    }

cleanup:
    if (egr_nh_tbl_buf) {
        soc_cm_sfree(unit, egr_nh_tbl_buf);
    }

    return rv;
}

/* 
 * Function:
 *      _bcm_tr_mpls_nexthop_ref_inc
 * Purpose:
 *      Recover the nexthop refcount for mpls entry from hw.
 * Parameters:
 *      unit    : (IN) Device Unit Number
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_nexthop_ref_inc(
    int unit,
    bcm_mpls_tunnel_switch_t *info,
    void *user_data)
{
    int rv = BCM_E_NONE;
    int nexthop = 0, nh_idx = 0;
    uint32 mpath_flag = 0;

    COMPILER_REFERENCE(user_data);
    if (info == NULL) {
        return BCM_E_PARAM;
    }

    nexthop = info->egress_if;
    if (nexthop) {
        /* figure out hw index from egress object */
        rv = bcm_xgs3_get_nh_from_egress_object(unit,
                 nexthop, &mpath_flag, 1, &nh_idx);
        if (BCM_FAILURE(rv)) {
            return rv;
        }
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_tr_mpls_reinit
 * Purpose:
 *      Top level Warm Boot init for MPLS 
 * Parameters:
 *      unit    : (IN) Device Unit Number
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_reinit(int unit)
{
    int rv = BCM_E_NONE;
    int stable_size = 0;

    if (SOC_WARM_BOOT(unit)) {
        (void)_bcm_esw_mpls_tunnel_switch_traverse_no_lock(unit,
                  _bcm_tr_mpls_nexthop_ref_inc, NULL);
    }
    SOC_IF_ERROR_RETURN(soc_stable_size_get(unit, &stable_size));

    if (stable_size == 0) {
        rv = _bcm_tr_mpls_unsynchronized_reinit(unit);
    } else if (SOC_WARM_BOOT_SCACHE_IS_LIMITED(unit)) {
        rv = _bcm_tr_mpls_limited_reinit(unit);
    } else {
        rv = _bcm_tr_mpls_extended_reinit(unit);
    }
    return rv;
}

/* 
 * Function:
 *      _bcm_tr_mpls_ingexpbitmap_sync
 * Purpose:
 *      This is called only in the Cold Boot sequence when 
 *      limited scache is available. The 'ing_exp_map_bitmap' field of MPLS bk 
 *      structure is needed by the QoS module during Warm Boot with limited and 
 *      extended scache support. When extended scache support is available,
 *      this field is retreived by the '_bcm_tr_mpls_extended_reinit' sequence.
 * Parameters:
 *      unit    : (IN) Device Unit Number
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_ingexpbitmap_sync(int unit)
{
    int rv = BCM_E_NONE;
    uint8  *mpls_state;
    int    num_ing_exp_map;
    soc_scache_handle_t scache_handle;
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);

    SOC_SCACHE_HANDLE_SET(scache_handle, unit, BCM_MODULE_MPLS, 0);

    rv = _bcm_esw_scache_ptr_get(unit, scache_handle, FALSE,
                                 sizeof(int32), &mpls_state, 
                                 BCM_WB_DEFAULT_VERSION, NULL);

    if (BCM_FAILURE(rv) && (rv != BCM_E_INTERNAL)) {
        return rv;
    }

    num_ing_exp_map = soc_mem_index_count(unit, ING_MPLS_EXP_MAPPINGm) / 8;

    BCM_IF_ERROR_RETURN(bcm_tr_mpls_lock(unit));
    SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                      mpls_info->ing_exp_map_bitmap, 0, num_ing_exp_map);
    bcm_tr_mpls_unlock(unit);
    return rv;
}

/* 
 * Function:
 *      bcm_tr_mpls_sync
 * Purpose:
 *      Store the s/w maintained MPLS book keeping structure 
 *      onto persistent memory.
 * Parameters:
 *      unit    : (IN) Device Unit Number
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_sync(int unit)
{
    int num_vrf, num_vp, num_vc, num_ip_tnl, num_mpls_map;
    int num_ing_exp_map, num_egr_l2_exp_map, num_pw_term, num_pw_init;
#ifdef BCM_APACHE_SUPPORT
    int num_ing_exp_to_phb_cng_map = 0;
    int num_egr_exp_to_phb_cng_map = 0;
#endif
    int num_nh, alloc_sz = 0;
    int i, vp_count, rv = BCM_E_NONE;
    uint8 *mpls_state, *mpls_state_marker;
    soc_scache_handle_t scache_handle;
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    _bcm_mpls_egr_nhopList_t  *nhop_ptr;
    _bcm_mpls_vp_nh_list_t  *vp_ptr;
    uint32 primary_nh_if, bitmask;
#if defined(BCM_SABER2_SUPPORT)
    uint32     source_vp_sb2_5626x = 0xFED5ABC7;
#endif /* BCM_SABER2_SUPPORT */

    if (SOC_WARM_BOOT_SCACHE_IS_LIMITED(unit)) {
        return _bcm_tr_mpls_ingexpbitmap_sync(unit);
    }

    num_mpls_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 64;
    
    BCM_IF_ERROR_RETURN(_bcm_tr_mpls_scache_size_get(unit, &alloc_sz));
    if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_1) {
        alloc_sz += (3 * sizeof(int)) *
                    (soc_mem_index_count(unit, SOURCE_VPm)/2);

        if ((BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_8) &&
            (soc_feature(unit, soc_feature_hierarchical_protection))) {

            alloc_sz += (3 * sizeof(int)) *
                    (soc_mem_index_count(unit, SOURCE_VPm)/2);
        }

    }

    if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_2) {
         alloc_sz += SHR_BITALLOCSIZE(num_mpls_map);
    }

    if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_3) {
        alloc_sz += (num_mpls_map * sizeof(uint32) -
                     SHR_BITALLOCSIZE(num_mpls_map));
    }
	
    if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_5) {
        alloc_sz += SHR_BITALLOCSIZE(soc_mem_index_count(unit, SOURCE_VPm));
    }

    /*
     * For network_vp_bitmap
     */
    if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_6) {
        alloc_sz += SHR_BITALLOCSIZE(soc_mem_index_count(unit, SOURCE_VPm));
    }

    /* Maximum virtual port supported on saber2 5626x devices are 4K
     * This fix is related to scaling down of vpn from 8K to 4K
     */
#if defined(BCM_SABER2_SUPPORT)
        if (soc_sb2_5626x_devtype(unit)) {
            alloc_sz += sizeof(source_vp_sb2_5626x);
        }
#endif /* BCM_SABER2_SUPPORT */
/*
 * For failover_vp
 */
#ifdef BCM_TRIUMPH2_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_failover)) {
        if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_10) {
            alloc_sz += sizeof(uint32) * soc_mem_index_count(unit, SOURCE_VPm);
        }
    }
#endif /* BCM_TRIUMPH2_SUPPORT */

    SOC_SCACHE_HANDLE_SET(scache_handle, unit, BCM_MODULE_MPLS, 0);
    rv = _bcm_esw_scache_ptr_get(unit, scache_handle, FALSE,
                                 alloc_sz, &mpls_state, 
                                 BCM_WB_DEFAULT_VERSION, NULL);
    if (BCM_FAILURE(rv) && (rv != BCM_E_INTERNAL)) {
        return rv;
    }

    /* Maximum virtual port supported on saber2 5626x devices are 4K
     * This fix is related to scaling down of vpn from 8K to 4K
     */
#if defined(BCM_SABER2_SUPPORT)
        if (soc_sb2_5626x_devtype(unit)) {
            sal_memcpy(mpls_state, &source_vp_sb2_5626x, sizeof(source_vp_sb2_5626x));
            mpls_state += sizeof(source_vp_sb2_5626x);
        }
#endif /* BCM_SABER2_SUPPORT */
    num_vrf = SOC_CONTROL(unit)->max_vrf_id + 1;
    num_vp = soc_mem_index_count(unit, SOURCE_VPm);
    num_vc = soc_mem_index_count(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);
    num_ip_tnl = soc_mem_index_count(unit, EGR_IP_TUNNEL_MPLSm);
    /* EGR_MPLS_EXP_MAPPING_1 profiles indexed by ((int_pri << 2) + cng) */
    num_ing_exp_map = soc_mem_index_count(unit, ING_MPLS_EXP_MAPPINGm) / 8;
    num_egr_l2_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_PRI_MAPPINGm) / 8;
#ifdef BCM_APACHE_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_exp_to_phb_cng_map)) {
        num_ing_exp_to_phb_cng_map = soc_mem_index_count(unit, 
                ING_MPLS_EXP_MAPPING_1m) / 16;
        num_egr_exp_to_phb_cng_map = soc_mem_index_count(unit, 
                EGR_MPLS_EXP_MAPPING_3m) / 8;
    }
#endif
    num_pw_term = 0;
    if (SOC_MEM_IS_VALID(unit, ING_PW_TERM_COUNTERSm)) {
        num_pw_term = soc_mem_index_count(unit, ING_PW_TERM_COUNTERSm);
    }
    num_pw_init = 0;
    if (SOC_MEM_IS_VALID(unit, EGR_PW_INIT_COUNTERSm)) {
        num_pw_init = soc_mem_index_count(unit, EGR_PW_INIT_COUNTERSm);
    }
    num_nh = soc_mem_index_count(unit, EGR_L3_NEXT_HOPm);

    BCM_IF_ERROR_RETURN(bcm_tr_mpls_lock(unit));

    rv = BCM_E_NONE;
    SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                      mpls_info->vrf_bitmap, 0, num_vrf);
    mpls_state += SHR_BITALLOCSIZE(num_vrf);
    SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                      mpls_info->vpws_bitmap, 0, (num_vp/2));
    mpls_state += SHR_BITALLOCSIZE(num_vp/2);
    SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                      mpls_info->vc_c_bitmap, 0, num_vc);
    mpls_state += SHR_BITALLOCSIZE(num_vc);
    SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                      mpls_info->vc_nc_bitmap, 0, num_vc);
    mpls_state += SHR_BITALLOCSIZE(num_vc);
    if (num_pw_term) {
        SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                          mpls_info->pw_term_bitmap, 0, num_pw_term);
        mpls_state += SHR_BITALLOCSIZE(num_pw_term);
    }
    if (num_pw_init) {
        SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                          mpls_info->pw_init_bitmap, 0, num_pw_init);
        mpls_state += SHR_BITALLOCSIZE(num_pw_init);
    }
    
    SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                      mpls_info->egr_mpls_bitmap, 0, num_mpls_map);
    mpls_state += SHR_BITALLOCSIZE(num_mpls_map);
    SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                      mpls_info->ing_exp_map_bitmap, 0, num_ing_exp_map);
    mpls_state += SHR_BITALLOCSIZE(num_ing_exp_map);
    SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                      mpls_info->egr_l2_exp_map_bitmap, 0, num_egr_l2_exp_map);
    mpls_state += SHR_BITALLOCSIZE(num_egr_l2_exp_map);
    SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                      mpls_info->tnl_bitmap, 0,
                      (num_ip_tnl * _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit)));
    mpls_state += SHR_BITALLOCSIZE(num_ip_tnl *
            _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit));
    SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                      mpls_info->ip_tnl_bitmap, 0, num_ip_tnl);
    mpls_state += SHR_BITALLOCSIZE(num_ip_tnl);
    mpls_state_marker = mpls_state;

    bitmask = 1 << ((8 * sizeof(uint32)) - 1);
    for (i = 0; i < _BCM_TR_MPLS_HASH_ELEMENTS; i++) {
        if (_bcm_tr_mpls_nhop_headPtr[unit][i] != NULL) {
            for (nhop_ptr = _bcm_tr_mpls_nhop_headPtr[unit][i];
                 nhop_ptr != NULL;
                 nhop_ptr = nhop_ptr->link) {
                primary_nh_if = nhop_ptr->egr_if;
                primary_nh_if |= bitmask;
                sal_memcpy(mpls_state, &primary_nh_if, sizeof(uint32));
                mpls_state += sizeof(uint32);
                for (vp_ptr = nhop_ptr->vp_head_ptr;
                     vp_ptr != NULL;
                     vp_ptr = vp_ptr->link) {
                    sal_memcpy(mpls_state, &(vp_ptr->vp_nh_idx), sizeof(uint32));
                    mpls_state += sizeof(uint32);
                } /* for each vp on nh */
            } /* for each nh */
        }
    }
    vp_count = 0xdeadbeef;
    sal_memcpy(mpls_state, &vp_count, sizeof(uint32));
    mpls_state = mpls_state_marker + (sizeof(uint32) * num_nh);

    /* Sync the MPLS scache for the VPWS VP map */
    if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_1) {
        mpls_state += sizeof(int);
        /* Copy VPWS map table to scache */
        for (i=0; i<(num_vp/2); i++) {
            sal_memcpy(mpls_state, &mpls_info->vpws_vp_map[i].vp1,
                       sizeof(int));
            mpls_state += sizeof(int);
            sal_memcpy(mpls_state, &mpls_info->vpws_vp_map[i].vp2,
                       sizeof(int));
            mpls_state += sizeof(int);
            sal_memcpy(mpls_state, &mpls_info->vpws_vp_map[i].vp3,
                       sizeof(int));
            mpls_state += sizeof(int);

            if ((BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_8) &&
                (soc_feature(unit, soc_feature_hierarchical_protection))) {
                sal_memcpy(mpls_state, &mpls_info->vpws_vp_map[i].vp_b_1,
                       sizeof(int));
                mpls_state += sizeof(int);
                sal_memcpy(mpls_state, &mpls_info->vpws_vp_map[i].vp_b_2,
                       sizeof(int));
                mpls_state += sizeof(int);
                sal_memcpy(mpls_state, &mpls_info->vpws_vp_map[i].vp_b_3,
                       sizeof(int));
                mpls_state += sizeof(int);
            }
        }
    }

    /* Sync the scache for the MPLS map */
    if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_2) {
        sal_memcpy(mpls_state, mpls_info->egr_mpls_hw_idx,
                   (num_mpls_map * sizeof(uint32)));
        mpls_state += (num_mpls_map * sizeof(uint32));
    }

    /* Sync the scache for the MPLS svp profile set */
    if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_5) {
        SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                          mpls_info->svp_profile_set_bitmap, 0, num_vp);
        mpls_state += SHR_BITALLOCSIZE(num_vp);
    }

    /* Sync the scache for the MPLS network VP bitmap */
    if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_6) {
        SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                          mpls_info->network_vp_bitmap, 0, num_vp);
        mpls_state += SHR_BITALLOCSIZE(num_vp);
    }
#ifdef BCM_APACHE_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_exp_to_phb_cng_map)) {
        /* Sync the scache for the MPLS EXP to PHB and CNG maps */
        
        SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                mpls_info->ing_exp_to_phb_cng_map_bitmap, 0, 
                num_ing_exp_to_phb_cng_map);
        mpls_state += SHR_BITALLOCSIZE(num_ing_exp_to_phb_cng_map);
        SHR_BITCOPY_RANGE((SHR_BITDCL *)mpls_state, 0,
                mpls_info->egr_exp_to_phb_cng_map_bitmap, 0, 
                num_egr_exp_to_phb_cng_map);
        mpls_state += SHR_BITALLOCSIZE(num_egr_exp_to_phb_cng_map);
    }
#endif
#ifdef BCM_TRIUMPH2_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_failover)) {
        if (BCM_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_10) {
            sal_memcpy(mpls_state, mpls_info->failover_vp, 
                       (num_vp * sizeof(uint32)));
            mpls_state += sizeof(uint32) * soc_mem_index_count(unit, SOURCE_VPm);
        }
    }
#endif /* BCM_TRIUMPH2_SUPPORT */

    bcm_tr_mpls_unlock(unit);

    return rv;
}

#endif

int
_egr_qos_id2hw_idx (int unit,int qos_id,int *hw_inx)
{
    int rv = BCM_E_NOT_FOUND;
    int table_num;

    if (SOC_IS_TRIUMPH(unit) || SOC_IS_VALKYRIE(unit)) {
        rv = _bcm_tr_qos_id2idx(unit, qos_id, hw_inx);
    } else {
#ifdef BCM_TRIUMPH2_SUPPORT
        rv = _bcm_tr2_qos_id2idx(unit, qos_id, hw_inx);
#endif /* BCM_TRIUMPH2_SUPPORT */
    }

    if (rv == BCM_E_NOT_FOUND) {
        /* created from API bcm_mpls_exp_map_create() */
        if ((qos_id & (~_BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK)) ==
            _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS) {
            table_num = qos_id & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK;
            if (!_BCM_EGR_MPLS_MAP_USED_GET(unit, table_num)) {
                return BCM_E_PARAM;
            }
            *hw_inx = MPLS_INFO(unit)->egr_mpls_hw_idx[table_num];
            rv = BCM_E_NONE;
           } else if ((qos_id & (~_BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK)) ==
            _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS_L2) {
            table_num = qos_id & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK;
            if (!_BCM_MPLS_EGR_EXP_MAP_USED_GET(unit, table_num)) {
                return BCM_E_PARAM;
            } 
            *hw_inx = table_num;
            rv = BCM_E_NONE;
          } else {
            rv = BCM_E_NOT_FOUND;
          } 
    }
    if (rv == BCM_E_NOT_FOUND) {
        rv = BCM_E_PARAM;
    }
    return rv;
}

#if !defined(_BCM_QOS_MAP_TYPE_EGR_MPLS_MAPS) 
#define _BCM_QOS_MAP_TYPE_EGR_MPLS_MAPS      2
#endif

int
_egr_qos_hw_idx2id (int unit, int hw_idx, int *map_id)
{
    int rv = BCM_E_NOT_FOUND;
    int num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 64;
    int id;

    if (SOC_IS_TRIUMPH(unit) || SOC_IS_VALKYRIE(unit)) {
        rv = _bcm_tr_qos_idx2id(unit, hw_idx, _BCM_QOS_MAP_TYPE_EGR_MPLS_MAPS, map_id);
    } else {
#ifdef BCM_TRIUMPH2_SUPPORT
        rv = _bcm_tr2_qos_idx2id(unit, hw_idx, _BCM_QOS_MAP_TYPE_EGR_MPLS_MAPS, map_id);
#endif /* BCM_TRIUMPH2_SUPPORT */
    }


    if (rv == BCM_E_NOT_FOUND) {
        /* created from API bcm_mpls_exp_map_create() */

        for (id = 0; id < num_exp_map; id++) {
            if (_BCM_EGR_MPLS_MAP_USED_GET(unit, id)) {
                if (MPLS_INFO(unit)->egr_mpls_hw_idx[id] == (uint32)hw_idx) {
                    *map_id = id | _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS;
                    rv = BCM_E_NONE;
                    break;
                }
            }
        }
    }

    if (rv == BCM_E_NOT_FOUND) {
        for (id = 0; id < num_exp_map; id++) { 
            if (_BCM_MPLS_EGR_EXP_MAP_USED_GET(unit, id)) { 
                if (id == hw_idx) { 
                    *map_id = id | _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS_L2; 
                    rv = BCM_E_NONE; 
                    break; 
                } 
            } 
        } 
    }  
    if (rv == BCM_E_NOT_FOUND) {
        if (hw_idx == 0) {
            *map_id = 0;
            return BCM_E_NONE;
        }
        rv = BCM_E_PARAM;
    }
    return rv;
}
#ifdef BCM_APACHE_SUPPORT
STATIC int bcm_apache_mpls_effective_phb_select_init(int unit)
{
    int num_ing_phb_select_entries, num_egr_phb_select_entries, index;
    effective_phb_select_entry_t ing_phb_select_entry;
    egr_effective_phb_select_entry_t egr_phb_select_entry;
   
    num_ing_phb_select_entries = soc_mem_index_count(unit, 
                                                     EFFECTIVE_PHB_SELECTm);
    num_egr_phb_select_entries = soc_mem_index_count(unit, 
                                                     EGR_EFFECTIVE_PHB_SELECTm);

    /*Use PHB for INNER label  in Ingress pipeline(Decapsulation) */
    for (index = 0; index < num_ing_phb_select_entries; index++) {
        BCM_IF_ERROR_RETURN(READ_EFFECTIVE_PHB_SELECTm(unit, MEM_BLOCK_ANY,
                                                       index, 
                                                       &ing_phb_select_entry));
        if((index & 1) == 0) {
            soc_EFFECTIVE_PHB_SELECTm_field32_set(unit, &ing_phb_select_entry,
                    EFFECTIVE_PHB3_SELECTf, 
                    _BCM_MPLS_FWD_LABEL3_PHB_FROM_TOS);
        } else {
            soc_EFFECTIVE_PHB_SELECTm_field32_set(unit, &ing_phb_select_entry,
                    EFFECTIVE_PHB3_SELECTf, 
                    _BCM_MPLS_FWD_LABEL2_PHB_FROM_TOS);
        }
        if((index & 2) == 0) {
            soc_EFFECTIVE_PHB_SELECTm_field32_set(unit, &ing_phb_select_entry,
                    EFFECTIVE_PHB2_SELECTf, 
                    _BCM_MPLS_FWD_LABEL2_PHB_FROM_TOS);
        } else {
            soc_EFFECTIVE_PHB_SELECTm_field32_set(unit, &ing_phb_select_entry,
                    EFFECTIVE_PHB2_SELECTf, 
                    _BCM_MPLS_FWD_LABEL1_PHB_FROM_TOS);
        }
        /*Use PHB of label 1 both for pipe and uniform tunnels*/
        soc_EFFECTIVE_PHB_SELECTm_field32_set(unit, &ing_phb_select_entry,
                EFFECTIVE_PHB1_SELECTf, 
                _BCM_MPLS_FWD_LABEL1_PHB_FROM_TOS);
        
        BCM_IF_ERROR_RETURN(WRITE_EFFECTIVE_PHB_SELECTm(unit, MEM_BLOCK_ALL,
                                                        index, 
                                                        &ing_phb_select_entry));
    }

    /*Use PHB from INNER label in the Egress pipeline (Encapsulation)*/
    for (index = 0; index < num_egr_phb_select_entries; index++) {
        BCM_IF_ERROR_RETURN(READ_EGR_EFFECTIVE_PHB_SELECTm(unit, MEM_BLOCK_ANY,
                                                       index, 
                                                       &egr_phb_select_entry));
        /*Use PHB of label 3 both for pipe and uniform tunnels*/
        soc_EGR_EFFECTIVE_PHB_SELECTm_field32_set(unit, &egr_phb_select_entry,
                EFFECTIVE_PHB3_SELECTf, 
                _BCM_MPLS_FWD_LABEL3_PHB_FROM_TOS);
        
        if(((index & 0xc) >> 2) == 2) {
            soc_EGR_EFFECTIVE_PHB_SELECTm_field32_set(unit, &egr_phb_select_entry,
                    EFFECTIVE_PHB2_SELECTf, 
                    _BCM_MPLS_FWD_LABEL2_PHB_FROM_TOS);
        } else {
            soc_EGR_EFFECTIVE_PHB_SELECTm_field32_set(unit, &egr_phb_select_entry,
                    EFFECTIVE_PHB2_SELECTf, 
                    _BCM_MPLS_FWD_LABEL3_PHB_FROM_TOS);
        }
        if(((index & 0x30) >> 4) == 2) {
            soc_EGR_EFFECTIVE_PHB_SELECTm_field32_set(unit, &egr_phb_select_entry,
                    EFFECTIVE_PHB1_SELECTf, 
                    _BCM_MPLS_FWD_LABEL1_PHB_FROM_TOS);
        } else {
            soc_EGR_EFFECTIVE_PHB_SELECTm_field32_set(unit, &egr_phb_select_entry,
                    EFFECTIVE_PHB1_SELECTf, 
                    _BCM_MPLS_FWD_LABEL2_PHB_FROM_TOS);
        }
        BCM_IF_ERROR_RETURN(WRITE_EGR_EFFECTIVE_PHB_SELECTm(unit, MEM_BLOCK_ALL,
                                                        index, 
                                                        &egr_phb_select_entry));
    }
    return BCM_E_NONE;  
}
#endif

#if defined(BCM_TRIDENT2_SUPPORT)
/*
 * Function:
 *      _bcm_tr_l3_ingress_interface_dma_init
 * Purpose:
 *      Perform L3_IIF Table Initialization using DMA Operation
 * Parameters:
 *      unit      : (IN) Device Unit Number
 *      start_idx : (IN) Start index for Read/Write
 *      end_idx : (IN) End index for Read/Write
 * Returns:
 *      BCM_E_XXX
 *
 * Note:
 * Function to perform DMA Read / Write of L3_IIF during initialization
 * and perform all init related operations without read / writes to H/W
 */
int _bcm_tr_l3_ingress_interface_dma_init(int unit,
                                          int start_idx,
                                          int end_idx)
{
    uint32 *l3iif_buf = NULL;
    iif_entry_t *hw_entry = NULL;
    iif_entry_t write_entry;
    _bcm_l3_ingress_intf_t iif;
    int idx, size, count;
    int rv = BCM_E_INTERNAL;

    /* Determine size of buffer to be allocated.
     * Ideally buffer allocation should be done in chunks in case chunk size
     * is defined.
     */
    count = (end_idx - start_idx +1);
    size = count * WORDS2BYTES(soc_mem_entry_words(unit, L3_IIFm));

    l3iif_buf = soc_cm_salloc(unit, size, "l3_iif_buffer");
    if (l3iif_buf == NULL) {
        return BCM_E_MEMORY;
    }

    sal_memset((void *)l3iif_buf, 0, size);
    /* Read L3_IIF Table */
    rv = soc_mem_read_range(unit, L3_IIFm, MEM_BLOCK_ANY,
                           start_idx, end_idx, l3iif_buf);
    if (SOC_FAILURE(rv)) {
        LOG_ERROR(BSL_LS_BCM_MPLS,
                  (BSL_META_U(unit,
                              "Mpls Init: DMA Read Failed for L3_IIF\n")));
        soc_cm_sfree(unit, l3iif_buf);
        return rv;
    }

    /* Iterate over each entry to get and set the S/W structure */
    for (idx = 0; idx < count; idx++) {
        sal_memset(&iif, 0, sizeof(_bcm_l3_ingress_intf_t));
        iif.intf_id = idx + start_idx;
        iif.vrf = 0;
        sal_memcpy(&write_entry,  soc_mem_entry_null(unit, L3_IIFm),
                   sizeof(iif_entry_t));

        /* Read entry from buffer and pass it to interface function in order
         * to avoid a PIO read */
        hw_entry = soc_mem_table_idx_to_pointer(unit, L3_IIFm, iif_entry_t *,
                                               l3iif_buf, idx);

        /* Populate iif structure to using a _get API , No PIO Reads performed*/
        rv = _bcm_tr_l3_ingress_interface_get(unit, hw_entry, &iif);
        if (BCM_FAILURE(rv)) {
            LOG_ERROR(BSL_LS_BCM_MPLS,
                (BSL_META_U(unit,
                    "Mpls Init: Ingress interface get failed for index=(%d)\n"),
                    idx));
            soc_cm_sfree(unit, l3iif_buf);
            return rv;
        }

        /* Get the new entry to be written to hw , pass the hw_entry as an argument
         * to the function to avoid PIO read & write.
         * Profile function is also updated to not read from H/W but use the hw_entry
         * provided
         */
        rv = _bcm_tr_l3_ingress_interface_set(unit, &iif, hw_entry,
                                                     &write_entry);
        if (BCM_FAILURE(rv)) {
            LOG_ERROR(BSL_LS_BCM_MPLS,
                (BSL_META_U(unit,
                    "Mpls Init: Ingress interface set failed for index=(%d)\n"),
                    idx));
            soc_cm_sfree(unit, l3iif_buf);
            return rv;
        }

        /* Update the entry copy in the DMAed buffer with the new contents
         * of write_entry modified by interface_set.
         */
        sal_memcpy(hw_entry, &write_entry, sizeof(iif_entry_t));
    }

    /* Write to H/W */
    rv = soc_mem_write_range(unit, L3_IIFm, MEM_BLOCK_ALL,
                             start_idx, end_idx, l3iif_buf);
    soc_cm_sfree(unit, l3iif_buf);
    return rv;
}
#endif

/*
 * Function:
 *      bcm_mpls_init
 * Purpose:
 *      Initialize the MPLS software module
 * Parameters:
 *      unit     - Device Number
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_init(int unit)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    bcm_tr_mpls_vc_swap_table_hash_t *vc_swap_hash;
    int rv=0, i, num_vfi, num_vp, num_vpws, num_vc, num_vrf, num_iif, num_trunk;
    int num_pw_term, num_pw_init, num_ip_tnl, num_mpls_map, num_ing_exp_map, num_egr_l2_exp_map, pkt_cfi, pkt_pri;
#ifdef BCM_APACHE_SUPPORT
    int num_egr_mpls_exp_to_phb_cng_map = 0;
    int num_ing_mpls_exp_to_phb_cng_map = 0;
#endif
    ing_pri_cng_map_entry_t pri_map;
    vfi_entry_t vfi_entry;
    _bcm_l3_ingress_intf_t iif;
    uint32 label;
#ifdef BCM_KATANA_SUPPORT
    int num_ing_qmap = 0;
#endif /* BCM_KATANA_SUPPORT */

    if (!L3_INFO(unit)->l3_initialized) {
        LOG_ERROR(BSL_LS_BCM_MPLS,
                  (BSL_META_U(unit,
                              "L3 module must be initialized prior to MPLS_init\n")));
        return BCM_E_CONFIG;
    }

    /* cache the EGR_MPLS_VC_AND_SWAP_LABEL_TABLE field info for fast performance in the
     * table serarch function.
     */
    SOC_VC_SWAP_LABLE_FIELD_ASSIGN(unit, &soc_emvasltm_field_cache[unit],
                                    EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, MPLS_LABELf);
    SOC_VC_SWAP_LABLE_FIELD_ASSIGN(unit, &soc_emvasltm_field_cache[unit],
                             EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, MPLS_LABEL_ACTIONf);
    SOC_VC_SWAP_LABLE_FIELD_ASSIGN(unit, &soc_emvasltm_field_cache[unit],
                             EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, MPLS_TTLf);
    SOC_VC_SWAP_LABLE_FIELD_ASSIGN(unit, &soc_emvasltm_field_cache[unit],
                             EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, CW_INSERT_FLAGf);
    SOC_VC_SWAP_LABLE_FIELD_ASSIGN(unit, &soc_emvasltm_field_cache[unit],
                             EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, MPLS_EXPf);
    SOC_VC_SWAP_LABLE_FIELD_ASSIGN(unit, &soc_emvasltm_field_cache[unit],
                             EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, NEW_PRIf);
    SOC_VC_SWAP_LABLE_FIELD_ASSIGN(unit, &soc_emvasltm_field_cache[unit],
                             EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, NEW_CFIf);
    SOC_VC_SWAP_LABLE_FIELD_ASSIGN(unit, &soc_emvasltm_field_cache[unit],
                             EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, SD_TAG_VIDf);
    SOC_VC_SWAP_LABLE_FIELD_ASSIGN(unit, &soc_emvasltm_field_cache[unit],
                EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, SD_TAG_ACTION_IF_PRESENTf);
    SOC_VC_SWAP_LABLE_FIELD_ASSIGN(unit, &soc_emvasltm_field_cache[unit],
                EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, SD_TAG_ACTION_IF_NOT_PRESENTf);
    SOC_VC_SWAP_LABLE_FIELD_ASSIGN(unit, &soc_emvasltm_field_cache[unit],
                EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, MPLS_EXP_SELECTf);
    SOC_VC_SWAP_LABLE_FIELD_ASSIGN(unit, &soc_emvasltm_field_cache[unit],
                EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, MPLS_EXP_MAPPING_PTRf);
    
    /* 
     * Include flex stat hooks when implemented
     */

    num_vfi = soc_mem_index_count(unit, VFIm);
    num_vrf = SOC_CONTROL(unit)->max_vrf_id + 1;
    num_iif = soc_mem_index_count(unit, L3_IIFm);
    num_vp = soc_mem_index_count(unit, SOURCE_VPm);
    num_vpws = num_vp / 2;
    num_vc = soc_mem_index_count(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);
    num_ip_tnl = soc_mem_index_count(unit, EGR_IP_TUNNEL_MPLSm);
    num_trunk = soc_mem_index_count(unit, TRUNK_GROUPm);
#ifdef BCM_KATANA_SUPPORT
    if (soc_feature(unit, soc_feature_extended_queueing)) {
        num_ing_qmap = soc_mem_index_count(unit, ING_QUEUE_MAPm);
    }
#endif /* BCM_KATANA_SUPPORT */
    /* EGR_MPLS_EXP_MAPPING_1 profiles indexed by ((int_pri << 2) + cng) */
    num_mpls_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 64;
    num_ing_exp_map = soc_mem_index_count(unit, ING_MPLS_EXP_MAPPINGm) / 8;
#ifdef BCM_APACHE_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_exp_to_phb_cng_map)) {
        num_egr_mpls_exp_to_phb_cng_map = soc_mem_index_count(unit, 
                EGR_MPLS_EXP_MAPPING_3m) / 8;
        num_ing_mpls_exp_to_phb_cng_map = soc_mem_index_count(unit, 
                ING_MPLS_EXP_MAPPING_1m) / 16;
    }
#endif
    num_egr_l2_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_PRI_MAPPINGm) / 8;
    num_pw_term = 0;
    if (SOC_MEM_IS_VALID(unit, ING_PW_TERM_COUNTERSm)) {
        num_pw_term = soc_mem_index_count(unit, ING_PW_TERM_COUNTERSm);
    } else if (SOC_MEM_IS_VALID(unit, ING_PW_TERM_SEQ_NUMm)) {
        num_pw_term = soc_mem_index_count(unit, ING_PW_TERM_SEQ_NUMm);
    }
    num_pw_init = 0;
    if (SOC_MEM_IS_VALID(unit, EGR_PW_INIT_COUNTERSm)) {
        num_pw_init = soc_mem_index_count(unit, EGR_PW_INIT_COUNTERSm);
    }

    /*
     * allocate resources
     */
    if (mpls_info->initialized) {
        /* coverity[stack_use_overflow : FALSE] */
        rv = bcm_tr_mpls_cleanup(unit);
        BCM_IF_ERROR_RETURN(rv);
    }

    mpls_info->vrf_bitmap =
        sal_alloc(SHR_BITALLOCSIZE(num_vrf), "vrf_bitmap");
    if (mpls_info->vrf_bitmap == NULL) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return BCM_E_MEMORY;
    }

    mpls_info->vpws_bitmap =
        sal_alloc(SHR_BITALLOCSIZE(num_vpws), "vpws_bitmap");
    if (mpls_info->vpws_bitmap == NULL) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return BCM_E_MEMORY;
    }


    mpls_info->vpws_vp_map =
        sal_alloc(sizeof(_bcm_mpls_vpws_vp_map_info_t) * (num_vpws), "vpws_vp_map");
    if (mpls_info->vpws_vp_map == NULL) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return BCM_E_MEMORY;
    }

    mpls_info->trunk_vp_map =
        sal_alloc(sizeof(int) * (num_trunk), "trunk_vp_map");
    if (mpls_info->trunk_vp_map == NULL) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return BCM_E_MEMORY;
    }

    mpls_info->match_key =
        sal_alloc(sizeof(_bcm_tr_mpls_match_port_info_t) * num_vp, "match_key");
    if (mpls_info->match_key == NULL) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return BCM_E_MEMORY;
    }

    mpls_info->vc_c_bitmap =
        sal_alloc(SHR_BITALLOCSIZE(num_vc), "vc_c_bitmap"); 
    if (mpls_info->vc_c_bitmap == NULL) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return BCM_E_MEMORY;
    }

    mpls_info->vc_nc_bitmap =
        sal_alloc(SHR_BITALLOCSIZE(num_vc), "vc_nc_bitmap"); 
    if (mpls_info->vc_nc_bitmap == NULL) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return BCM_E_MEMORY;
    }

    if (num_pw_term) {
       mpls_info->pw_term_bitmap =
          sal_alloc(SHR_BITALLOCSIZE(num_pw_term), "pw_term_bitmap");
       if (mpls_info->pw_term_bitmap == NULL) {
           _bcm_tr_mpls_free_resource(unit, mpls_info);
           return BCM_E_MEMORY;
       }
    }

    if(num_pw_init) {
        mpls_info->pw_init_bitmap =
            sal_alloc(SHR_BITALLOCSIZE(num_pw_init), "pw_init_bitmap");
        if (mpls_info->pw_init_bitmap == NULL) {
            _bcm_tr_mpls_free_resource(unit, mpls_info);
            return BCM_E_MEMORY;
        }
    }

    mpls_info->egr_mpls_bitmap =
        sal_alloc(SHR_BITALLOCSIZE(num_mpls_map), "egr_mpls_bitmap");
    if (mpls_info->egr_mpls_bitmap == NULL) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return BCM_E_MEMORY;
    }
    mpls_info->egr_mpls_hw_idx = 
        sal_alloc(sizeof(uint32) * num_mpls_map, "egr_mpls_hw_idx");
    if (mpls_info->egr_mpls_hw_idx == NULL) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return BCM_E_MEMORY;
    }    

    mpls_info->ing_exp_map_bitmap =
        sal_alloc(SHR_BITALLOCSIZE(num_ing_exp_map), "ing_exp_map_bitmap");
    if (mpls_info->ing_exp_map_bitmap == NULL) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return BCM_E_MEMORY;
    }

    mpls_info->egr_l2_exp_map_bitmap =
        sal_alloc(SHR_BITALLOCSIZE(num_egr_l2_exp_map), "egr_l2_exp_map_bitmap");
    if (mpls_info->egr_l2_exp_map_bitmap == NULL) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return BCM_E_MEMORY;
    }
#ifdef BCM_APACHE_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_exp_to_phb_cng_map)) {
        mpls_info->ing_exp_to_phb_cng_map_bitmap =
            sal_alloc(SHR_BITALLOCSIZE(num_ing_mpls_exp_to_phb_cng_map), 
                    "ing_exp_to_phb_cng_map_bitmap");
        if (mpls_info->ing_exp_to_phb_cng_map_bitmap == NULL) {
            _bcm_tr_mpls_free_resource(unit, mpls_info);
            return BCM_E_MEMORY;
        }

        mpls_info->egr_exp_to_phb_cng_map_bitmap =
            sal_alloc(SHR_BITALLOCSIZE(num_egr_mpls_exp_to_phb_cng_map), 
                    "egr_exp_to_phb_cng_map_bitmap");
        if (mpls_info->egr_exp_to_phb_cng_map_bitmap == NULL) {
            _bcm_tr_mpls_free_resource(unit, mpls_info);
            return BCM_E_MEMORY;
        }
    } 
#endif
    /* 4 MPLS entries for each EGR_IP_TUNNEL_MPLS entry */
    mpls_info->tnl_bitmap =
        sal_alloc(SHR_BITALLOCSIZE(num_ip_tnl * 
                    _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit)),
                "tnl_bitmap");
    if (mpls_info->tnl_bitmap == NULL) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return BCM_E_MEMORY;
    }

    mpls_info->ip_tnl_bitmap =
        sal_alloc(SHR_BITALLOCSIZE(num_ip_tnl), "ip_tnl_bitmap");
    if (mpls_info->ip_tnl_bitmap == NULL) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return BCM_E_MEMORY;
    }

    if (NULL == mpls_info->vc_swap_ref_count) {
         mpls_info->vc_swap_ref_count = sal_alloc(sizeof(uint16) *
                                     num_vc, "vc_swap_refcount");
         if (mpls_info->vc_swap_ref_count == NULL) {
              _bcm_tr_mpls_free_resource(unit, mpls_info);
              return BCM_E_MEMORY;
         }
        sal_memset(mpls_info->vc_swap_ref_count, 0, 
                sizeof(uint16) * num_vc);
    }

    if (NULL == mpls_info->egr_tunnel_ref_count) {
         mpls_info->egr_tunnel_ref_count = sal_alloc(sizeof(uint16) *
                 (num_ip_tnl * _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit)), 
                 "egr_tunnel_ref_count");
         if (mpls_info->egr_tunnel_ref_count == NULL) {
              _bcm_tr_mpls_free_resource(unit, mpls_info);
              return BCM_E_MEMORY;
         }
         sal_memset(mpls_info->egr_tunnel_ref_count, 0, 
                 sizeof(uint16) * (num_ip_tnl *
                     _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit)));
    }

    if (NULL == bcmi_vc_swap_label_hash[unit]) {
        bcmi_vc_swap_label_hash[unit] = sal_alloc(
            BCM_TR_MPLS_VC_SWAP_HASH_BUCKETS * 
            sizeof (bcm_tr_mpls_vc_swap_table_hash_t), "vc_swap_hash");

        if (bcmi_vc_swap_label_hash[unit] == NULL) {
            _bcm_tr_mpls_free_resource(unit, mpls_info);
            return SOC_E_MEMORY;
        }

        for (i = 0; i < BCM_TR_MPLS_VC_SWAP_HASH_BUCKETS; i++) {
            vc_swap_hash = &(bcmi_vc_swap_label_hash[unit][i]);
            vc_swap_hash->head_link = NULL;
        }
    }

    mpls_info->svp_profile_set_bitmap =
        sal_alloc(SHR_BITALLOCSIZE(num_vp), "svp_profile_set_bitmap");
    if (mpls_info->svp_profile_set_bitmap == NULL) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return BCM_E_MEMORY;
    }

    mpls_info->network_vp_bitmap =
        sal_alloc(SHR_BITALLOCSIZE(num_vp), "network_vp_bitmap");
    if (mpls_info->network_vp_bitmap == NULL) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return BCM_E_MEMORY;
    }

#ifdef BCM_KATANA_SUPPORT
    if (soc_feature(unit, soc_feature_extended_queueing)) {
        if (NULL == mpls_info->ing_qmap_ref_count) {
             mpls_info->ing_qmap_ref_count = sal_alloc(sizeof(uint16) *
                                         num_ing_qmap, "ing_qmap_ref_count");
             if (mpls_info->ing_qmap_ref_count == NULL) {
                  _bcm_tr_mpls_free_resource(unit, mpls_info);
                  return BCM_E_MEMORY;
             }
            sal_memset(mpls_info->ing_qmap_ref_count, 0,
                       sizeof(uint16) * num_ing_qmap);
        }
    }
#endif /* BCM_KATANA_SUPPORT */
#ifdef BCM_TRIUMPH2_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_failover)) {
        mpls_info->failover_vp =
            sal_alloc(num_vp * sizeof(uint32), "failover vp");
        if (mpls_info->failover_vp == NULL) {
            _bcm_tr_mpls_free_resource(unit, mpls_info);
            return BCM_E_MEMORY;
        }
    }
#endif /* BCM_TRIUMPH2_SUPPORT */


    sal_memset(mpls_info->vrf_bitmap, 0, SHR_BITALLOCSIZE(num_vrf));
    sal_memset(mpls_info->vpws_bitmap, 0, SHR_BITALLOCSIZE(num_vpws));
    sal_memset(mpls_info->vc_c_bitmap, 0, SHR_BITALLOCSIZE(num_vc));
    sal_memset(mpls_info->vc_nc_bitmap, 0, SHR_BITALLOCSIZE(num_vc));
    if (num_pw_term) {
        sal_memset(mpls_info->pw_term_bitmap, 0, SHR_BITALLOCSIZE(num_pw_term));
    }
    sal_memset(mpls_info->pw_init_bitmap, 0, SHR_BITALLOCSIZE(num_pw_init));	
    sal_memset(mpls_info->egr_mpls_bitmap, 0, SHR_BITALLOCSIZE(num_mpls_map));
    sal_memset(mpls_info->egr_mpls_hw_idx, 0, (sizeof(uint32) * num_mpls_map));
    sal_memset(mpls_info->ing_exp_map_bitmap, 0, SHR_BITALLOCSIZE(num_ing_exp_map));
    sal_memset(mpls_info->egr_l2_exp_map_bitmap, 0, SHR_BITALLOCSIZE(num_egr_l2_exp_map));
#ifdef BCM_APACHE_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_exp_to_phb_cng_map)) {
        sal_memset(mpls_info->ing_exp_to_phb_cng_map_bitmap, 0, 
                SHR_BITALLOCSIZE(num_ing_mpls_exp_to_phb_cng_map));
        sal_memset(mpls_info->egr_exp_to_phb_cng_map_bitmap, 0, 
                SHR_BITALLOCSIZE(num_egr_mpls_exp_to_phb_cng_map));
    }
#endif
    sal_memset(mpls_info->match_key, 0, sizeof(_bcm_tr_mpls_match_port_info_t) * num_vp);
    sal_memset(mpls_info->tnl_bitmap, 0,
            SHR_BITALLOCSIZE(num_ip_tnl *
                _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit)));
    sal_memset(mpls_info->ip_tnl_bitmap, 0, SHR_BITALLOCSIZE(num_ip_tnl));
    sal_memset(mpls_info->svp_profile_set_bitmap, 0, SHR_BITALLOCSIZE(num_vp));
#ifdef BCM_TRIUMPH2_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_failover)) {
       sal_memset(mpls_info->failover_vp, 0, num_vp * sizeof(uint32));    
    }
#endif /* BCM_TRIUMPH2_SUPPORT */


    if (!SOC_WARM_BOOT(unit)) {
    /*
     * Initialize the VFI table by setting L3MC_INDEX to 0 and
     * PFM to 3. The PFM==3 (invalid) indicates that the VFI entry is not used.
     * This can be used to recover SW state during warm reboot.
     */
#ifdef BCM_TRIUMPH2_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
        /* Initialize VFIm entries need to have:
         * { BC_INDEX == 0,  UUC_INDEX == 0, UMC_INDEX == 0 } 
         * Just same to clear it.
         */
        rv = soc_mem_clear(unit, VFIm, MEM_BLOCK_ANY, TRUE);
        if (rv < 0) {
            _bcm_tr_mpls_free_resource(unit, mpls_info);
            return rv;
        }
    } else
#endif /* BCM_TRIUMPH2_SUPPORT */
    {
        if (soc_mem_field_valid(unit, VFIm, PFMf)) { 
           sal_memset(&vfi_entry, 0, sizeof(vfi_entry_t));
        
           soc_VFIm_field32_set(unit, &vfi_entry, L3MC_INDEXf, 0);
           soc_VFIm_field32_set(unit, &vfi_entry, PFMf, 3);
           for (i = 0; i < num_vfi; i++) {
                  rv = WRITE_VFIm(unit, MEM_BLOCK_ALL, i, &vfi_entry);
                  if (rv < 0) {
                        _bcm_tr_mpls_free_resource(unit, mpls_info);
                        return rv;
                  }
           }
        }
    }

    /* 
     * Initialize L3_IIF entries starting from 
     * _BCM_TR_MPLS_L3_IIF_BASE for each valid IIF value.
     */
#if defined(BCM_TRIDENT2_SUPPORT)
    if (soc_feature(unit, soc_feature_l3_iif_profile)) {
        rv = _bcm_tr_l3_ingress_interface_dma_init(unit,
                _BCM_TR_MPLS_L3_IIF_BASE+1,
                soc_mem_index_max(unit, L3_IIFm));

        if (BCM_FAILURE(rv)) {
            _bcm_tr_mpls_free_resource(unit, mpls_info);
            return rv;
        }
    } else
#endif /* BCM_TRIDENT2_SUPPORT */
    {
        sal_memset(&iif, 0, sizeof(_bcm_l3_ingress_intf_t));
        for (i = _BCM_TR_MPLS_L3_IIF_BASE+1; i < num_iif; i++) {
            iif.intf_id = i;
            iif.vrf = 0;

            rv = _bcm_tr_l3_ingress_interface_set(unit, &iif,
                    NULL, NULL);
            if (BCM_FAILURE(rv)) {
                _bcm_tr_mpls_free_resource(unit, mpls_info);
                return rv;
            }
        }
    }

    /* 
     * Reserve one mapping instance in ING_PRI_CNG_MAP for
     * identity mapping of packet pri/cfi to internal pri/color.
     */
    sal_memset(&pri_map, 0, sizeof(ing_pri_cng_map_entry_t));
    for (pkt_cfi = 0; pkt_cfi <= 1; pkt_cfi++) {
        for (pkt_pri = 0; pkt_pri <= 7; pkt_pri++) {
            /* ING_PRI_CNG_MAP table is indexed with
             * port[0:4] incoming priority[2:0] incoming CFI[0]
             */
            i = (_BCM_TR_MPLS_PRI_CNG_MAP_IDENTITY << 4) | (pkt_pri << 1) | pkt_cfi;
            soc_mem_field32_set(unit, ING_PRI_CNG_MAPm, &pri_map, PRIf,
                                pkt_pri);
            soc_mem_field32_set(unit, ING_PRI_CNG_MAPm, &pri_map, CNGf,
                                pkt_cfi);
            rv = WRITE_ING_PRI_CNG_MAPm(unit, MEM_BLOCK_ALL, i, &pri_map);
            if (rv < 0) {
                _bcm_tr_mpls_free_resource(unit, mpls_info);
                return rv;
            }
        }
    }

    /* Clear L2 PW Counters */
    rv = _bcm_tr_mpls_stat_clear(unit);
    if (rv < 0) {
        return rv;
    }

    /* Initialize the entire label space "Port Independent"
      * Use Switch Controls to re-configure the Label Space 
      * for Port-Dependant and Port-Independant 
      */
 
    label = 0;
    rv = soc_reg_field32_modify(unit, GLOBAL_MPLS_RANGE_1_LOWERr,
                                REG_PORT_ANY, LABELf, label);
    if (rv < 0) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return rv;
    }
    rv = soc_reg_field32_modify(unit, GLOBAL_MPLS_RANGE_2_LOWERr,
                                REG_PORT_ANY, LABELf, label);
    if (rv < 0) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return rv;
    }

    label = (1 << 20) - 1;
    rv = soc_reg_field32_modify(unit, GLOBAL_MPLS_RANGE_1_UPPERr,
                                REG_PORT_ANY, LABELf, label);
    if (rv < 0) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return rv;
    }
    rv = soc_reg_field32_modify(unit, GLOBAL_MPLS_RANGE_2_UPPERr,
                                REG_PORT_ANY, LABELf, label);
    if (rv < 0) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return rv;
    }

    /* Enable MPLS */
    rv = bcm_tr_mpls_enable(unit, TRUE);
    if (rv < 0) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return rv;
    }
#ifdef BCM_APACHE_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_exp_to_phb_cng_map)) {
        rv = bcm_apache_mpls_effective_phb_select_init(unit);
        if (rv < 0) {
            _bcm_tr_mpls_free_resource(unit, mpls_info);
            return rv;
        }
    }
#endif
    } /* !WARM_BOOT */

    /* Init Software State */
    for (i=0; i < num_vp; i++) {
         mpls_info->match_key[i].modid = -1;
         mpls_info->match_key[i].trunk_id = -1;
         mpls_info->match_key[i].fo_modid = -1;
         mpls_info->match_key[i].fo_trunk_id = -1;
         mpls_info->match_key[i].match_count = 0;
    }

    for (i=0; i < num_vpws; i++) {
         mpls_info->vpws_vp_map[i].vp1 = -1;
         mpls_info->vpws_vp_map[i].vp2 = -1;
         mpls_info->vpws_vp_map[i].vp3 = -1;
#ifdef BCM_MULTI_LEVEL_FAILOVER_SUPPORT
        if ((soc_feature(unit, soc_feature_hierarchical_protection))) {
             mpls_info->vpws_vp_map[i].vp_b_1 = -1;
             mpls_info->vpws_vp_map[i].vp_b_2 = -1;
             mpls_info->vpws_vp_map[i].vp_b_3 = -1;
        }
#endif
    }
    for (i = 0; i < num_trunk; i++) {
         mpls_info->trunk_vp_map[i] = -1;
    }
    /* Reserve  Default-VC_AND_SWAP_LABEL_TABLE entry */
    rv = _bcm_tr_mpls_default_entry_setup(unit);
    if (rv < 0) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return rv;
    }

    /* Create MPLS protection mutex. */
    MPLS_INFO((unit))->mpls_mutex = sal_mutex_create("mpls_mutex");
    if (!MPLS_INFO((unit))->mpls_mutex) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
        return rv;
    }

    for(i=0; i < _BCM_TR_MPLS_HASH_ELEMENTS; ++i) {
        _bcm_tr_mpls_nhop_headPtr[unit][i] = NULL;
    }

#if defined(BCM_APACHE_SUPPORT) || defined(BCM_TOMAHAWK2_SUPPORT)
    if (soc_feature(unit, soc_feature_egr_ip_tnl_mpls_double_wide)) {
        rv = bcmi_egr_ip_tunnel_mpls_sw_init(unit);
        if (rv != BCM_E_NONE) {
            _bcm_tr_mpls_free_resource(unit, mpls_info);
        }
    }
#endif

#ifdef BCM_WARM_BOOT_SUPPORT
    rv = _bcm_tr_mpls_reinit(unit);
    if (rv != BCM_E_NONE) {
        _bcm_tr_mpls_free_resource(unit, mpls_info);
    }
#endif /* BCM_WARM_BOOT_SUPPORT */

    /* Mark the state as initialized */
    mpls_info->initialized = TRUE;

    return rv;
}

#ifdef BCM_APACHE_SUPPORT
/*
 * Function:
 *      bcm_tr_mpls_exp_to_phb_cng_map_destroy_all
 * Purpose:
 *      Destroy all existing MPLS EXP to PBH and CNG map instances.
 * Parameters:
 *      unit       - (IN) SOC unit #
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
bcm_tr_mpls_exp_to_phb_cng_map_destroy_all(int unit)
{
    int i, num_exp_map;

    /* ING_MPLS_EXP_MAPPING_1 profiles indexed by EXP */
    num_exp_map = soc_mem_index_count(unit, ING_MPLS_EXP_MAPPING_1m) / 16;
    for (i = 0; i < num_exp_map; i++) {
        if (_BCM_MPLS_ING_EXP_TO_PHB_CNG_MAP_USED_GET(unit, i)) {
            _BCM_MPLS_ING_EXP_TO_PHB_CNG_MAP_USED_CLR(unit, i);
        }
    }

    /* EGR_MPLS_EXP_MAPPING_3m profiles indexed by EXP */
    num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_3m) / 8;
    for (i = 0; i < num_exp_map; i++) {
        if (_BCM_MPLS_EGR_EXP_TO_PHB_CNG_MAP_USED_GET(unit, i)) {
            _BCM_MPLS_EGR_EXP_TO_PHB_CNG_MAP_USED_CLR(unit, i);
        }
    }

    return BCM_E_NONE;
}
#endif
/*
 * Function:
 *      _bcm_tr_mpls_hw_clear
 * Purpose:
 *     Perform hw tables clean up for mpls module. 
 * Parameters:
 *      unit     - Device Number
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_hw_clear(int unit)
{
    int rv=BCM_E_NONE, rv_error = BCM_E_NONE;

    rv = bcm_tr_mpls_enable(unit, FALSE);
    if (BCM_FAILURE(rv)) {
        rv_error = rv;
    }

    /* Clear L2 PW Counters */
    rv = _bcm_tr_mpls_stat_clear(unit);
    if (rv<0) {
       rv_error = rv;
    }

    /* Destroy all VPNs */
    rv = bcm_tr_mpls_vpn_id_destroy_all(unit);
    if (BCM_FAILURE(rv) && (BCM_E_NONE == rv_error)) {
        rv_error = rv;
    }

    /* Delete all L2 tunnel entries */
    rv = bcm_esw_l2_tunnel_delete_all(unit);
    if (BCM_FAILURE(rv) && (BCM_E_NONE == rv_error)) {
        rv_error = rv;
    }

    /* Clear all MPLS tunnel initiators */
    rv = bcm_tr_mpls_tunnel_initiator_clear_all(unit);
    if (BCM_FAILURE(rv) && (BCM_E_NONE == rv_error)) {
        rv_error = rv;
    }

    /* Delete all MPLS switch entries */
    rv = _bcm_esw_mpls_tunnel_switch_delete_all(unit);
    if (BCM_FAILURE(rv) && (BCM_E_NONE == rv_error)) {
        rv_error = rv;
    }

    /* Destroy all EXP maps */
    rv = bcm_tr_mpls_exp_map_destroy_all(unit);
    if (BCM_FAILURE(rv) && (BCM_E_NONE == rv_error)) {
        rv_error = rv;
    }

#ifdef BCM_APACHE_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_exp_to_phb_cng_map)) {
        rv = bcm_tr_mpls_exp_to_phb_cng_map_destroy_all(unit);
        if (BCM_FAILURE(rv) && (BCM_E_NONE == rv_error)) {
            rv_error = rv;
        }
    }
#endif
    return rv_error;
}

/*
 * Function:
 *      bcm_mpls_cleanup
 * Purpose:
 *      Detach the MPLS software module
 * Parameters:
 *      unit     - Device Number
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_cleanup(int unit)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int rv=BCM_E_NONE;

    if (FALSE == mpls_info->initialized) {
        return (BCM_E_NONE);
    } 

    rv = bcm_tr_mpls_lock (unit);
    if (BCM_FAILURE(rv)) {
        return rv;
    }

    if (0 == SOC_HW_ACCESS_DISABLE(unit)) { 
        rv = _bcm_tr_mpls_hw_clear(unit);
    }

    /* Free software resources */
    (void) _bcm_tr_mpls_free_resource(unit, mpls_info);

    bcm_tr_mpls_unlock (unit);

    /* Destroy MPLS protection mutex. */
    sal_mutex_destroy(MPLS_INFO((unit))->mpls_mutex );

    /* Mark the state as uninitialized */
    mpls_info->initialized = FALSE;

    return rv;
}

/*
 * Function:
 *      _bcm_tr_vpws_vpn_alloc
 * Purpose:
 *      Internal function for allocating a VPWS Vpn_id
 * Parameters:
 *      unit    -  (IN) Device number.
 *      vpn_id     -  (OUT) VPWS Vpn_id
 * Returns:
 *      BCM_X_XXX
 */

int
_bcm_tr_vpws_vpn_alloc(int unit, bcm_vpn_t  *vpn_id)
{
    int i, num_vpws;

    num_vpws = soc_mem_index_count(unit, SOURCE_VPm) / 2;

    for (i = 0; i < num_vpws; i++) {
        if (!_BCM_MPLS_VPWS_USED_GET(unit, i)) {
            break;
        }
    }
    if (i == num_vpws) {
        return BCM_E_RESOURCE;
    }

    *vpn_id = i;
    _BCM_MPLS_VPWS_USED_SET(unit, *vpn_id);

    return BCM_E_NONE;
}


/*
 * Function:
 *      bcm_mpls_vpn_id_create
 * Purpose:
 *      Create a VPN instance
 * Parameters:
 *      unit  - (IN)  Device Number
 *      info  - (IN/OUT) VPN configuration info
 * Returns:
 *      BCM_E_XXX
 * Note:
 *      No hardware action
 */
int
bcm_tr_mpls_vpn_id_create(int unit, bcm_mpls_vpn_config_t *info)
{
    int rv = BCM_E_PARAM;
    bcm_vpn_t  vpn_id;
    int proto_pkt_inx;
    int num_vpws;

    if (info->flags & BCM_MPLS_VPN_REPLACE) {
        if (!(info->flags & BCM_MPLS_VPN_WITH_ID)) {
            return BCM_E_PARAM;
        }
    }

    if (info->flags & BCM_MPLS_VPN_VPWS) {
        if (info->flags & BCM_MPLS_VPN_WITH_ID) {
            if (!_BCM_MPLS_VPN_IS_VPWS(info->vpn)) {
                return BCM_E_PARAM;
            }          
            _BCM_MPLS_VPN_GET(vpn_id, _BCM_MPLS_VPN_TYPE_VPWS, info->vpn);
            num_vpws = soc_mem_index_count(unit, SOURCE_VPm) / 2;
            if (vpn_id >= num_vpws) {
                return BCM_E_PARAM;
            } else if (_BCM_MPLS_VPWS_USED_GET(unit, vpn_id)) {
                /* Already used */
                return BCM_E_EXISTS;
            }
            _BCM_MPLS_VPWS_USED_SET(unit, vpn_id);
            rv = BCM_E_NONE;
        } else {
           /* Get vpn_id */
           rv = _bcm_tr_vpws_vpn_alloc(unit, &vpn_id);
           if (BCM_SUCCESS(rv)) {
                 _BCM_MPLS_VPN_SET(info->vpn, _BCM_MPLS_VPN_TYPE_VPWS, vpn_id);
           }
        }
    } else if (info->flags & BCM_MPLS_VPN_VPLS) {
        vfi_entry_t vfi_entry;
        int vfi_index, mc_group=0, mc_group_type, num_vfi;
        bcm_vpn_t vpls_vpn_min=0;

        bcm_vlan_mcast_flood_t mode;
#if defined(BCM_TRIUMPH2_SUPPORT)
        int bc_group=0, umc_group=0, uuc_group=0;
        int bc_group_type, umc_group_type, uuc_group_type;
#endif /* BCM_TRIUMPH2_SUPPORT  */

        num_vfi = soc_mem_index_count(unit, VFIm);
#ifdef BCM_TRIUMPH2_SUPPORT
        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
            /* Check that the groups are valid. */
            bc_group_type = _BCM_MULTICAST_TYPE_GET(info->broadcast_group);
            bc_group = _BCM_MULTICAST_ID_GET(info->broadcast_group);
            umc_group_type = _BCM_MULTICAST_TYPE_GET(info->unknown_multicast_group);
            umc_group = _BCM_MULTICAST_ID_GET(info->unknown_multicast_group);
            uuc_group_type = _BCM_MULTICAST_TYPE_GET(info->unknown_unicast_group);
            uuc_group = _BCM_MULTICAST_ID_GET(info->unknown_unicast_group);

            if ((bc_group_type != _BCM_MULTICAST_TYPE_VPLS) ||
                (umc_group_type != _BCM_MULTICAST_TYPE_VPLS) ||
                (uuc_group_type != _BCM_MULTICAST_TYPE_VPLS) ||
                (bc_group >= soc_mem_index_count(unit, L3_IPMCm)) ||
                (umc_group >= soc_mem_index_count(unit, L3_IPMCm)) ||
                (uuc_group >= soc_mem_index_count(unit, L3_IPMCm))) {
                return BCM_E_PARAM;
            }
        } else
#endif
        {
            /* Check that the broadcast group is valid. Also check that
             * broadcast, unknown_unicast, and unknown_multicast groups
             * are the same (this device only supports on multicast group
             * for all 3 types of multicast traffic.
             */

            mc_group_type = _BCM_MULTICAST_TYPE_GET(info->broadcast_group);
            mc_group = _BCM_MULTICAST_ID_GET(info->broadcast_group);

            if ((mc_group_type != _BCM_MULTICAST_TYPE_VPLS) ||
                (mc_group >= soc_mem_index_count(unit, L3_IPMCm)) ||
                (info->broadcast_group != info->unknown_unicast_group) ||
                (info->broadcast_group != info->unknown_multicast_group)) {
                return BCM_E_PARAM;
            }
        }

        /* validate parameter protocol_pkt */
        if (SOC_MEM_FIELD_VALID(unit, VFIm, PROTOCOL_PKT_INDEXf)) {
            rv = _bcm_xgs3_protocol_packet_actions_validate(unit,
                                   &info->protocol_pkt);
            if (BCM_FAILURE(rv) && (rv != BCM_E_UNAVAIL)) {
                return rv;
            }
        }

        /* Allocate a VFI */
        /* Lock the VFI table while allocating and writing to allow
         * proper operation with the service counters */
        soc_mem_lock(unit, VFIm);
        if (info->flags & BCM_MPLS_VPN_WITH_ID) {
            _BCM_MPLS_VPN_SET(vpls_vpn_min, _BCM_MPLS_VPN_TYPE_VPLS, 0);
            if ( info->vpn < vpls_vpn_min || info->vpn > (vpls_vpn_min+num_vfi-1) ) {
                soc_mem_unlock(unit, VFIm);
                return BCM_E_PARAM;
            }

            _BCM_MPLS_VPN_GET(vfi_index, _BCM_MPLS_VPN_TYPE_VPLS, info->vpn);
            if (_bcm_vfi_used_get(unit, vfi_index, _bcmVfiTypeMpls)) {
                if (!(info->flags & BCM_MPLS_VPN_REPLACE)) {
                    soc_mem_unlock(unit, VFIm);
                    return BCM_E_EXISTS;
                }
            } else {
                rv = _bcm_vfi_alloc_with_id(unit, VFIm, _bcmVfiTypeMpls, vfi_index);
                if (rv < 0) {
                    soc_mem_unlock(unit, VFIm);
                    return rv;
                }
            }
        } else {
            rv = _bcm_vfi_alloc(unit, VFIm, _bcmVfiTypeMpls, &vfi_index);
            if (rv < 0) {
                soc_mem_unlock(unit, VFIm);
                return rv;
            }
        }

        /* Get the default flood mode */
        BCM_IF_ERROR_RETURN(_bcm_esw_vlan_flood_default_get(unit, &mode));
        switch (mode) {
        case BCM_VLAN_MCAST_FLOOD_ALL:
            mode = 0;
            break;
        case BCM_VLAN_MCAST_FLOOD_NONE:
            mode = 2;
            break;
        default:
            mode = 1;
            break;
        }

        /* Commit the entry to HW */
        BCM_IF_ERROR_RETURN(READ_VFIm(unit, MEM_BLOCK_ALL, vfi_index, &vfi_entry));
        if (info->flags & BCM_MPLS_VPN_REPLACE) {
            rv = READ_VFIm(unit, MEM_BLOCK_ALL, vfi_index, &vfi_entry);
            if (BCM_FAILURE(rv)) {
                soc_mem_unlock(unit, VFIm);
                return rv;
            }
        }
#ifdef BCM_TRIUMPH2_SUPPORT
        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
            soc_VFIm_field32_set(unit, &vfi_entry, UMC_INDEXf, umc_group);
            soc_VFIm_field32_set(unit, &vfi_entry, UUC_INDEXf, uuc_group);
            soc_VFIm_field32_set(unit, &vfi_entry, BC_INDEXf, bc_group);
        } else
#endif
        {
            soc_VFIm_field32_set(unit, &vfi_entry, L3MC_INDEXf, mc_group);
            soc_VFIm_field32_set(unit, &vfi_entry, PFMf, mode);
        }

#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) || \
                                                defined(BCM_APACHE_SUPPORT)
        if (SOC_IS_KATANAX(unit) || SOC_IS_TRIUMPH3(unit) || SOC_IS_APACHE(unit)) {
            rv = _bcm_esw_add_policer_to_table(unit, info->policer_id, VFIm,
                                                    0, &vfi_entry);
            if (rv < 0) {
                soc_mem_unlock(unit, VFIm);
                return rv;
            }
        }
#endif /* BCM_KATANA_SUPPORT or BCM_TRIUMPH3_SUPPORT or BCM_APACHE_SUPPORT */

        if (SOC_MEM_FIELD_VALID (unit,VFIm, PROTOCOL_PKT_INDEXf)) {
            if (info->flags & BCM_MPLS_VPN_REPLACE) {
                proto_pkt_inx = soc_VFIm_field32_get(unit, &vfi_entry, 
                                    PROTOCOL_PKT_INDEXf);
            } else {
                proto_pkt_inx = -1;
            }
            rv = _bcm_xgs3_protocol_pkt_ctrl_set(unit,proto_pkt_inx,
                                    &info->protocol_pkt,
                                    &proto_pkt_inx);
            if (BCM_SUCCESS(rv)) { 
                soc_VFIm_field32_set(unit, &vfi_entry, PROTOCOL_PKT_INDEXf, 
                                 proto_pkt_inx);
            } else if (rv != BCM_E_UNAVAIL) {
                soc_mem_unlock(unit, VFIm);
                return rv;
            }
        }
        rv = WRITE_VFIm(unit, MEM_BLOCK_ALL, vfi_index, &vfi_entry);
        if (rv < 0) {
            (void) _bcm_vfi_free(unit, _bcmVfiTypeMpls, vfi_index);
            soc_mem_unlock(unit, VFIm);
            return rv;
        }
        soc_mem_unlock(unit, VFIm);

        /* Set the returned VPN id */
        _BCM_MPLS_VPN_SET(info->vpn, _BCM_MPLS_VPN_TYPE_VPLS, vfi_index);

    } else if (info->flags & BCM_MPLS_VPN_L3) {
        int vrf;
        uint16 vrf_vpn_min;

        /* 
         * The caller manages VRF space. An MPLS L3 VPN identifier
         * is simply the VRF value passed in by the caller (info->lookup_id)
         * added to a base offset. The VPN ID returned here can 
         * be used in the bcm_mpls_tunnel_switch_* APIs. 
         */
        if (info->flags & BCM_MPLS_VPN_WITH_ID) {
            _BCM_MPLS_VPN_SET(vrf_vpn_min, _BCM_MPLS_VPN_TYPE_L3, 0);

            if ( info->vpn < vrf_vpn_min || info->vpn > (vrf_vpn_min +SOC_VRF_MAX(unit)) ) {
                return BCM_E_PARAM;
            }

            _BCM_MPLS_VPN_GET(vrf, _BCM_MPLS_VPN_TYPE_L3, info->vpn);
        } else {
            vrf = info->lookup_id;
        }

        if ((vrf < 0) || (vrf > SOC_VRF_MAX(unit))) {
            return BCM_E_PARAM;
        }
        if (_BCM_MPLS_VRF_USED_GET(unit, vrf)) {
            if (info->flags & BCM_MPLS_VPN_REPLACE) {
                return BCM_E_CONFIG;
            }
            return BCM_E_EXISTS;
        }
        _BCM_MPLS_VRF_USED_SET(unit, vrf);
        _BCM_MPLS_VPN_SET(info->vpn, _BCM_MPLS_VPN_TYPE_L3, vrf);
        rv = BCM_E_NONE;
    }

#ifdef BCM_WARM_BOOT_SUPPORT
    SOC_CONTROL_LOCK(unit);
    SOC_CONTROL(unit)->scache_dirty = 1;
    SOC_CONTROL_UNLOCK(unit);
#endif

    return rv;
}

/*
 * Function:
 *      bcm_tr_mpls_vpn_id_get
 * Purpose:
 *      Get VPN instance
 * Parameters:
 *      unit  - (IN)  Device Number
 *      vpn -  (IN)  VPN instance ID
 *      info  - (OUT) VPN configuration info
 * Returns:
 *      BCM_E_XXX
 * Note:
 *      No hardware action
 */

int
bcm_tr_mpls_vpn_id_get(int unit,
                       bcm_vpn_t vpn,
                       bcm_mpls_vpn_config_t *info)
{
    int proto_pkt_inx;
    int rv;

    if (_BCM_MPLS_VPN_IS_VPWS(vpn)) {
        bcm_vpn_t vpws_vpn_id;
        int num_vpws;

        num_vpws = soc_mem_index_count(unit, SOURCE_VPm) / 2;

        /*
         * validate the vpn id, must be in the range 0 to
         * (number of SOURCE_VP entries / 2)
         */
        _BCM_MPLS_VPN_GET(vpws_vpn_id, _BCM_MPLS_VPN_TYPE_VPWS, vpn);

        if (vpws_vpn_id >= num_vpws) {
            return BCM_E_PARAM;
        }

        if (!_BCM_MPLS_VPWS_USED_GET(unit, vpws_vpn_id)) {
            return BCM_E_NOT_FOUND;
        }

        /* Set the VPN id value returned to caller */
        _BCM_MPLS_VPN_SET(info->vpn, _BCM_MPLS_VPN_TYPE_VPWS, vpws_vpn_id);
        info->flags |=  BCM_MPLS_VPN_VPWS;

    } else if (_BCM_MPLS_VPN_IS_VPLS(vpn)) {
        int vfi_index, num_vfi;
        bcm_vpn_t vpls_vpn_min=0;
        vfi_entry_t vfi_entry;

        num_vfi = soc_mem_index_count(unit, VFIm);

        _BCM_MPLS_VPN_SET(vpls_vpn_min, _BCM_MPLS_VPN_TYPE_VPLS, 0);
        if ( vpn < vpls_vpn_min || vpn > (vpls_vpn_min+num_vfi-1) ) {
            return BCM_E_PARAM;
        }

        _BCM_MPLS_VPN_GET(vfi_index, _BCM_MPLS_VPN_TYPE_VPLS, vpn);

        if (!_bcm_vfi_used_get(unit, vfi_index, _bcmVfiTypeMpls)) {
            return BCM_E_NOT_FOUND;
        }

        BCM_IF_ERROR_RETURN
            (READ_VFIm(unit, MEM_BLOCK_ALL, vfi_index, &vfi_entry));
#ifdef BCM_TRIUMPH2_SUPPORT
        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
            _BCM_MULTICAST_GROUP_SET(info->broadcast_group,
                                     _BCM_MULTICAST_TYPE_VPLS,	
                      soc_VFIm_field32_get(unit, &vfi_entry, BC_INDEXf));
            _BCM_MULTICAST_GROUP_SET(info->unknown_unicast_group, 
                                     _BCM_MULTICAST_TYPE_VPLS,  
                      soc_VFIm_field32_get(unit, &vfi_entry, UUC_INDEXf));
            _BCM_MULTICAST_GROUP_SET(info->unknown_multicast_group, 
                                     _BCM_MULTICAST_TYPE_VPLS,  
                      soc_VFIm_field32_get(unit, &vfi_entry, UMC_INDEXf));
        } else
#endif
        {
            _BCM_MULTICAST_GROUP_SET(info->broadcast_group,
                                     _BCM_MULTICAST_TYPE_VPLS, 
                      soc_VFIm_field32_get(unit, &vfi_entry, L3MC_INDEXf));
            info->unknown_multicast_group = info->broadcast_group;
            info->unknown_unicast_group = info->broadcast_group;
        }

        if (SOC_MEM_FIELD_VALID (unit,VFIm, PROTOCOL_PKT_INDEXf)) {
            proto_pkt_inx = soc_VFIm_field32_get(unit, &vfi_entry,
                                    PROTOCOL_PKT_INDEXf);
            rv = _bcm_xgs3_protocol_pkt_ctrl_get(unit,proto_pkt_inx,
                                    &info->protocol_pkt);
            if (BCM_FAILURE(rv) && (rv != BCM_E_UNAVAIL)) {
                return (rv);
            }
        }

        /* Set the returned VPN id */
        _BCM_MPLS_VPN_SET(info->vpn, _BCM_MPLS_VPN_TYPE_VPLS, vfi_index);
        info->flags |=  BCM_MPLS_VPN_VPLS;
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit, soc_feature_global_meter)) {
            _bcm_esw_get_policer_from_table(unit, VFIm, vfi_index, &vfi_entry, 
                                                      &info->policer_id, 1); 
        }
#endif /* BCM_KATANA_SUPPORT or BCM_TRIUMPH3_SUPPORT */
    } else if (_BCM_MPLS_VPN_IS_L3(vpn)) {
        int vrf_id;

       _BCM_MPLS_VPN_GET(vrf_id, _BCM_MPLS_VPN_TYPE_L3, vpn);
        if ((vrf_id < 0) || (vrf_id > SOC_VRF_MAX(unit))) {
            return BCM_E_PARAM;
        }
        if (!_BCM_MPLS_VRF_USED_GET(unit, vrf_id)) {
            return BCM_E_NOT_FOUND;
        }
        _BCM_MPLS_VPN_SET(info->vpn, _BCM_MPLS_VPN_TYPE_L3, vrf_id);
        info->flags |=  BCM_MPLS_VPN_L3;

    } else {
        return BCM_E_PARAM;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_mpls_vpn_id_destroy
 * Purpose:
 *      Delete a VPN instance
 * Parameters:
 *      unit - Device Number
 *      vpn - VPN instance ID
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_vpn_id_destroy (int unit, bcm_vpn_t vpn)
{
    int rv;
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
    int policer = 0;
#endif 

    if (_BCM_MPLS_VPN_IS_VPWS(vpn)) {
        bcm_vpn_t   vpn_id;

        _BCM_MPLS_VPN_GET(vpn_id, _BCM_MPLS_VPN_TYPE_VPWS, vpn);

        if (!_BCM_MPLS_VPWS_USED_GET(unit, vpn_id)) {
            return BCM_E_NOT_FOUND;
        }

        /* Delete all the mpls ports on this VPN */
        rv = bcm_tr_mpls_port_delete_all(unit, vpn);
        BCM_IF_ERROR_RETURN(rv);

        (void)    _bcm_tr_mpls_vpws_vp_map_clear (unit, vpn_id);

        /* Clear the  VPWS pair in-use status */
        _BCM_MPLS_VPWS_USED_CLR(unit, vpn_id);
    } else if (_BCM_MPLS_VPN_IS_VPLS(vpn)) {
        int vfi, num_vfi;
        vfi_entry_t vfi_entry;
        bcm_vpn_t vpls_vpn_min=0;

        num_vfi = soc_mem_index_count(unit, VFIm);

        _BCM_MPLS_VPN_SET(vpls_vpn_min, _BCM_MPLS_VPN_TYPE_VPLS, 0);
        if ( vpn < vpls_vpn_min || vpn > (vpls_vpn_min+num_vfi-1) ) {
            return BCM_E_PARAM;
        }

       _BCM_MPLS_VPN_GET(vfi, _BCM_MPLS_VPN_TYPE_VPLS, vpn);

        if (!_bcm_vfi_used_get(unit, vfi, _bcmVfiTypeMpls)) {
            return BCM_E_NOT_FOUND;
        }

        /* Delete all the mpls ports on this VPN */
        rv = bcm_tr_mpls_port_delete_all(unit, vpn);
        BCM_IF_ERROR_RETURN(rv);

        /*
         * Clear the VFI table entry by setting L3MC_INDEX to 0 and
         * PFM to 3. The PFM==3 (invalid) indicates that the VFI entry is not used.
         * This can be used to recover SW state during warm reboot.
         */
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit, soc_feature_global_meter)) {
            rv = _bcm_esw_get_policer_from_table(unit,VFIm, vfi, &vfi_entry, 
                                                      &policer, 0); 
            if(BCM_SUCCESS(rv)) {
                 _bcm_esw_policer_decrement_ref_count(unit, policer);
            } else {
                return rv;
            }
        }
#endif /* BCM_KATANA_SUPPORT or BCM_TRIUMPH3_SUPPORT */
        if (SOC_MEM_FIELD_VALID(unit, VFIm, PROTOCOL_PKT_INDEXf)) {
            int ref_count;
            int proto_pkt_inx;

            sal_memset(&vfi_entry, 0, sizeof(vfi_entry_t));
            rv = READ_VFIm(unit, MEM_BLOCK_ALL, vfi, &vfi_entry);
            BCM_IF_ERROR_RETURN(rv);
            proto_pkt_inx = soc_VFIm_field32_get(unit, &vfi_entry,
                                    PROTOCOL_PKT_INDEXf);
            BCM_IF_ERROR_RETURN(_bcm_prot_pkt_ctrl_ref_count_get (unit,
                               proto_pkt_inx, &ref_count));
            if (ref_count > 0) { 
                BCM_IF_ERROR_RETURN(_bcm_prot_pkt_ctrl_delete(unit,proto_pkt_inx));
            }
        }

#ifdef BCM_TRIUMPH2_SUPPORT
        if (soc_feature(unit, soc_feature_gport_service_counters)) {
            /* Release Service counter, if any */
            _bcm_esw_flex_stat_handle_free(unit, _bcmFlexStatTypeService, vpn);
        }
#endif /* BCM_TRIUMPH2_SUPPORT */

        (void) _bcm_vfi_free(unit, _bcmVfiTypeMpls, vfi);

        if (!soc_feature(unit, soc_feature_mpls_enhanced)) {
            if (SOC_MEM_FIELD_VALID(unit, VFIm, PFMf)) {
                BCM_IF_ERROR_RETURN(soc_mem_field32_modify(unit, VFIm,
                                    vfi, PFMf, 3));
            }
        }
    } else if (_BCM_MPLS_VPN_IS_L3(vpn)) {
        int vrf;

       _BCM_MPLS_VPN_GET(vrf, _BCM_MPLS_VPN_TYPE_L3, vpn);
        if ((vrf < 0) || (vrf > SOC_VRF_MAX(unit))) {
            return BCM_E_PARAM;
        }

        if (!_BCM_MPLS_VRF_USED_GET(unit, vrf)) {
            return BCM_E_NOT_FOUND;
        }
        _BCM_MPLS_VRF_USED_CLR(unit, vrf);
    } else {
        return BCM_E_PARAM;
    }

#ifdef BCM_WARM_BOOT_SUPPORT
    SOC_CONTROL_LOCK(unit);
    SOC_CONTROL(unit)->scache_dirty = 1;
    SOC_CONTROL_UNLOCK(unit);
#endif

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_mpls_vpn_id_destroy_all
 * Purpose:
 *      Delete all  VPN instances
 * Parameters:
 *      unit - Device Number
 * Returns:
 *      BCM_E_XXX
 */

int
bcm_tr_mpls_vpn_id_destroy_all (int unit)
{
    int num_vpws, num_vfi, num_vrf, idx, rv, rv_error = BCM_E_NONE;
    bcm_vpn_t vpn;


    /* Destroy all VPWS VPNs */
    num_vpws = soc_mem_index_count(unit, SOURCE_VPm) / 2;
    for (idx = 0; idx < num_vpws; idx++) {
        if (_BCM_MPLS_VPWS_USED_GET(unit, idx)) {
            _BCM_MPLS_VPN_SET(vpn, _BCM_MPLS_VPN_TYPE_VPWS, (idx));
            rv = bcm_tr_mpls_vpn_id_destroy(unit, vpn);
            if (rv < 0) {
                rv_error = rv;
            }
        }
    }

    /* Destroy all VPLS VPNs */
    num_vfi = soc_mem_index_count(unit, VFIm);
    for (idx = 0; idx < num_vfi; idx++) {
        if (_bcm_vfi_used_get(unit, idx, _bcmVfiTypeMpls)) {
            _BCM_MPLS_VPN_SET(vpn, _BCM_MPLS_VPN_TYPE_VPLS, idx);
            rv = bcm_tr_mpls_vpn_id_destroy(unit, vpn);
            if (rv < 0) {
                rv_error = rv;
            }
        }
    }

    /* Destroy all L3 VPNs */
    num_vrf = SOC_VRF_MAX(unit) + 1;
    for (idx = 0; idx < num_vrf; idx++) {
        if (_BCM_MPLS_VRF_USED_GET(unit, idx)) {
            _BCM_MPLS_VPN_SET(vpn, _BCM_MPLS_VPN_TYPE_L3, idx);
            rv = bcm_tr_mpls_vpn_id_destroy(unit, vpn);
            if (rv < 0) {
                rv_error = rv;
            }
        }
    }

    /* Destroy default  VP */
    if (_bcm_vp_used_get(unit, 0, _bcmVpTypeMpls)) {
        _bcm_vp_free(unit, _bcmVpTypeMpls, 1, 0);
    }

    return rv_error;
}

/*
 * Function:
 *      bcm_mpls_vpn_traverse
 * Purpose:
 *      Get information about a VPN instance
 * Parameters:
 *      unit  - (IN)  Device Number
 *      cb    - (IN)  User-provided callback
 *      info  - (IN/OUT) Cookie
 * Returns:
 *      BCM_E_XXX
 */

int bcm_tr_mpls_vpn_traverse(int unit, bcm_mpls_vpn_traverse_cb cb, 
                             void *user_data)
{
    int idx, num_vpws, num_vfi, num_vrf, rv = BCM_E_NONE;
    int vpn;
    bcm_mpls_vpn_config_t info;

    /* VPWS VPNs */
    num_vpws = soc_mem_index_count(unit, SOURCE_VPm) / 2;
    for (idx = 0; idx < num_vpws; idx++) {
         if (_BCM_MPLS_VPWS_USED_GET(unit, idx)) {
              _BCM_MPLS_VPN_SET(vpn, _BCM_MPLS_VPN_TYPE_VPWS, (idx));
              bcm_mpls_vpn_config_t_init(&info);
              rv = bcm_tr_mpls_vpn_id_get(unit, vpn, &info);
              if (BCM_FAILURE(rv)) {
                   return rv;
              }
              rv = cb(unit, &info, user_data);
              if (BCM_FAILURE(rv)) {
                   return rv;
              }
        }
    }

    /* VPLS VPNs */
    num_vfi = soc_mem_index_count(unit, VFIm);
    for (idx = 0; idx < num_vfi; idx++) {
         if (_bcm_vfi_used_get(unit, idx, _bcmVfiTypeMpls)) {
              _BCM_MPLS_VPN_SET(vpn, _BCM_MPLS_VPN_TYPE_VPLS, idx);
              bcm_mpls_vpn_config_t_init(&info);
              rv = bcm_tr_mpls_vpn_id_get(unit, vpn, &info);
              if (BCM_FAILURE(rv)) {
                   return rv;
              }
              rv = cb(unit, &info, user_data);
              if (BCM_FAILURE(rv)) {
                   return rv;
              }
         }
    }

    /* L3 VPNs */
    num_vrf = SOC_VRF_MAX(unit) + 1;
    for (idx = 0; idx < num_vrf; idx++) {
         if (_BCM_MPLS_VRF_USED_GET(unit, idx)) {
              _BCM_MPLS_VPN_SET(vpn, _BCM_MPLS_VPN_TYPE_L3, idx);
              bcm_mpls_vpn_config_t_init(&info);
              rv = bcm_tr_mpls_vpn_id_get(unit, vpn, &info);
              if (BCM_FAILURE(rv)) {
                   return rv;
              }
              rv = cb(unit, &info, user_data);
              if (BCM_FAILURE(rv)) {
                   return rv;
              }
        }
    }
    return rv;
}

typedef struct _bcm_tr_ing_nh_info_s {
    bcm_port_t      port;
    int      module;
    bcm_trunk_t      trunk;
    uint32   mtu;
    int qtag;
    int tag_type;
} _bcm_tr_ing_nh_info_t;

typedef struct _bcm_tr_egr_nh_info_s {
    int    entry_type;
    int    dvp_is_network;
    int    dvp_network_group_id;
    int    sd_tag_action_present;
    int    sd_tag_action_not_present;
    int      dvp;
    int      intf_num;
    int      sd_tag_vlan;
    int      sd_tag_pri;
    int      sd_tag_cfi;
    int      lltag_vlan;
    int      lltag_pri;
    int      lltag_cfi;
    int      macda_index;
    int      vc_swap_index;
    int      tpid_index;
    int      pw_init_cnt;
    int      vintf_ctr_index;
    int      flex_ctr_base_cnt_idx;
    int      flex_ctr_pool_num;
    int      flex_ctr_offset_mode;
    int      class_id;
    uint16   oam_global_context_id;
} _bcm_tr_egr_nh_info_t;


STATIC int
_bcm_tr_mpls_INITIAL_ING_L3_NEXT_HOP_add(int unit,  _bcm_tr_ing_nh_info_t  *ing_nh_info,
                                         int nh_index, bcm_port_t *local_port, int *is_local)
{
    initial_ing_l3_next_hop_entry_t   initial_ing_nh;
    int rv;
    int modid_local=0;
#if defined(BCM_KATANA2_SUPPORT)
    bcm_port_t pp_port;
#endif

    sal_memset(&initial_ing_nh, 0, sizeof(initial_ing_l3_next_hop_entry_t));
    if (ing_nh_info->trunk == -1) {
        soc_mem_field32_set(unit, INITIAL_ING_L3_NEXT_HOPm,
                            &initial_ing_nh, PORT_NUMf, ing_nh_info->port);
        soc_mem_field32_set(unit, INITIAL_ING_L3_NEXT_HOPm,
                            &initial_ing_nh, MODULE_IDf, ing_nh_info->module);
        BCM_IF_ERROR_RETURN( _bcm_esw_modid_is_local(unit, ing_nh_info->module, &modid_local));

#if defined(BCM_HGPROXY_COE_SUPPORT)
        if(soc_feature(unit, soc_feature_hgproxy_subtag_coe) &&
           _bcm_xgs5_subport_coe_mod_local(unit, ing_nh_info->module)) {

            *is_local = 1;
            BCM_IF_ERROR_RETURN(_bcmi_coe_subport_mod_port_physical_port_get(unit,
                                                                    ing_nh_info->module,
                                                                    ing_nh_info->port,
                                                                    local_port));
        } else
#endif

#if defined(BCM_KATANA2_SUPPORT)
        if (soc_feature(unit, soc_feature_linkphy_coe) ||
            soc_feature(unit, soc_feature_subtag_coe)) {
            if (modid_local == 0) {
                /* Check if this is local LinkPHYSubtag modid */
                if (_bcm_kt2_mod_is_coe_mod_check(unit, ing_nh_info->module) == BCM_E_NONE) {
                    BCM_IF_ERROR_RETURN(_bcm_kt2_modport_to_pp_port_get(
                         unit, ing_nh_info->module,
                         ing_nh_info->port, &pp_port));
                    if (_bcm_kt2_is_pp_port_linkphy_subtag(unit, pp_port)){
                        *is_local = 1;
                        *local_port = pp_port;
                    }
                }
            }
        }
#endif
        if (modid_local) {
            /* Indicated to calling function that this is a local port */
            *is_local = 1;
            *local_port = ing_nh_info->port;
        }
    } else {
        soc_mem_field32_set(unit, INITIAL_ING_L3_NEXT_HOPm,
                            &initial_ing_nh, Tf, 1);
        soc_mem_field32_set(unit, INITIAL_ING_L3_NEXT_HOPm,
                            &initial_ing_nh, TGIDf, ing_nh_info->trunk);
        *is_local = 1;
        BCM_GPORT_TRUNK_SET(*local_port, ing_nh_info->trunk);
    }

    rv = soc_mem_write(unit, INITIAL_ING_L3_NEXT_HOPm,
                       MEM_BLOCK_ALL, nh_index, &initial_ing_nh);

    return rv;
}


STATIC int
_bcm_tr_mpls_ING_L3_NEXT_HOP_add(int unit,  _bcm_tr_ing_nh_info_t  *ing_nh_info, 
                                 int nh_index, int  drop)
{
    ing_l3_next_hop_entry_t ing_nh;
    int rv;
#ifdef BCM_TRIUMPH3_SUPPORT
    uint32 mtu_profile_index = 0;
#endif /* BCM_TRIUMPH3_SUPPORT */

    sal_memset(&ing_nh, 0, sizeof(ing_l3_next_hop_entry_t));

    soc_mem_field32_set(unit, ING_L3_NEXT_HOPm, &ing_nh, DROPf, drop);
    if (ing_nh_info->trunk == -1) {
        soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                            &ing_nh, PORT_NUMf, ing_nh_info->port);
        soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                            &ing_nh, MODULE_IDf, ing_nh_info->module);
    } else {	 
        soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                            &ing_nh, Tf, 1);
        soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                            &ing_nh, TGIDf, ing_nh_info->trunk);
    }
    soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                        &ing_nh, ENTRY_TYPEf, 0x2); /* L2 DVP */
    if (SOC_MEM_FIELD_VALID(unit, ING_L3_NEXT_HOPm, MTU_SIZEf)) {
        if (ing_nh_info->mtu >=
            (1 << soc_mem_field_length(unit, ING_L3_NEXT_HOPm, MTU_SIZEf))) {
            soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                                &ing_nh, MTU_SIZEf, 0x3fff);
        } else {
            soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                                &ing_nh, MTU_SIZEf, ing_nh_info->mtu);
        }
    }
#ifdef BCM_TRIUMPH3_SUPPORT
    else if (SOC_MEM_FIELD_VALID(unit, ING_L3_NEXT_HOPm, 
                 DVP_ATTRIBUTE_1_INDEXf)) {
        BCM_IF_ERROR_RETURN(
          _bcm_tr3_mtu_profile_index_get(unit, 0x3fff, &mtu_profile_index));
        soc_mem_field32_set(unit, ING_L3_NEXT_HOPm, &ing_nh, 
                      DVP_ATTRIBUTE_1_INDEXf, mtu_profile_index);
    }
#endif /*BCM_TRIUMPH3_SUPPORT */

    if ((ing_nh_info->qtag != -1) && 
        SOC_MEM_FIELD_VALID(unit, ING_L3_NEXT_HOPm,EH_QUEUE_TAGf)) {
        soc_mem_field32_set(unit, ING_L3_NEXT_HOPm, &ing_nh,
                      EH_QUEUE_TAGf, ing_nh_info->qtag);
    }
    if ((ing_nh_info->tag_type != -1) &&
        SOC_MEM_FIELD_VALID(unit, ING_L3_NEXT_HOPm,EH_TAG_TYPEf)) {
        soc_mem_field32_set(unit, ING_L3_NEXT_HOPm, &ing_nh,
                      EH_TAG_TYPEf, ing_nh_info->tag_type);
    }

    rv = soc_mem_write (unit, ING_L3_NEXT_HOPm,
                        MEM_BLOCK_ALL, nh_index, &ing_nh);
    return rv;
}


STATIC int
_bcm_tr_mpls_EGR_L3_NEXT_HOP_add(int unit,  _bcm_tr_egr_nh_info_t  *egr_nh_info, 
                                 uint32 flags, int nh_index, int *is_local, int is_vpws, int drop)
{
    egr_l3_next_hop_entry_t egr_nh;
    int rv;

    sal_memset(&egr_nh, 0, sizeof(egr_l3_next_hop_entry_t));

    if (egr_nh_info->entry_type == 0x1) {

        soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                           &egr_nh, INTF_NUMf, egr_nh_info->intf_num);
        if (egr_nh_info->macda_index != -1) {
              if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                   soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                            &egr_nh, MPLS__MAC_DA_PROFILE_INDEXf,
                            egr_nh_info->macda_index);
              } else {
                   soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                             &egr_nh, MAC_DA_PROFILE_INDEXf,
                             egr_nh_info->macda_index);
              }
        }
        if (egr_nh_info->vc_swap_index != -1) {
             if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                  &egr_nh, MPLS__VC_AND_SWAP_INDEXf,
                                  egr_nh_info->vc_swap_index);
             } else {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                  &egr_nh, VC_AND_SWAP_INDEXf,
                                  egr_nh_info->vc_swap_index);
             }
        }

        if (soc_feature(unit, soc_feature_mpls_enhanced)) {

            if (soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm, MPLS__CLASS_IDf)) {
                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                    MPLS__CLASS_IDf, egr_nh_info->class_id);
            }

#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
            if ((egr_nh_info->flex_ctr_base_cnt_idx != -1) &&
                soc_feature(unit,soc_feature_advanced_flex_counter)) {
                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                 MPLS__FLEX_CTR_BASE_COUNTER_IDXf, 
                                 egr_nh_info->flex_ctr_base_cnt_idx);
                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                 MPLS__FLEX_CTR_POOL_NUMBERf, 
                                 egr_nh_info->flex_ctr_pool_num);
                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                 MPLS__FLEX_CTR_OFFSET_MODEf, 
                                 egr_nh_info->flex_ctr_offset_mode);
            } else
#endif
            if ((egr_nh_info->vintf_ctr_index != -1) && 
                soc_feature(unit, soc_feature_gport_service_counters)) {
                if (soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm,
                                  MPLS__USE_VINTF_CTR_IDXf)) {
                     soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                                  MPLS__USE_VINTF_CTR_IDXf,
                                  egr_nh_info->vintf_ctr_index ? 1 : 0);
                }
                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                 MPLS__VINTF_CTR_IDXf, 
                                 egr_nh_info->vintf_ctr_index);
            }
        }

        if (egr_nh_info->pw_init_cnt != -1) {
             if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                                  MPLS__PW_INIT_NUMf, egr_nh_info->pw_init_cnt);
             } else {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                                  PW_INIT_NUMf, egr_nh_info->pw_init_cnt);
             }
        }

        if (soc_feature(unit, soc_feature_vlan_xlate)) {
             soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                  MPLS__USE_DVP_IN_EVXLT_KEYf, 0x1);
        }

        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                        MPLS__BC_DROPf, drop ? 1 : 0);
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                        MPLS__UUC_DROPf, drop ? 1 : 0);
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                        MPLS__UMC_DROPf, drop ? 1 : 0);
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                        MPLS__DVPf, egr_nh_info->dvp);
              if (soc_feature(unit, soc_feature_multiple_split_horizon_group)) {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &egr_nh, MPLS__DVP_NETWORK_GROUPf,
                        egr_nh_info->dvp_network_group_id);
              } else {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &egr_nh, MPLS__DVP_IS_NETWORK_PORTf,
                        egr_nh_info->dvp_is_network);
              }
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                        MPLS__HG_LEARN_OVERRIDEf, is_vpws ? 1 : 0);
              /*
               * Generally for L2 MPLS,
               * - VPWS application, MPLS tunnel header can be encapsulated
               * at ingress chip or egress chip, the suggestion is add MPLS
               * header at egress chip to save HG bandwidth.
               *
               * - VPLS application, L2 learning is needed on egress chip,
               * that means the MPLS tunnel header need to be encapsulated at
               * the egress chip. SVP and DVP field in PPD type 2 will provide
               * infomation for these requirements.
               */
              if (flags & BCM_MPLS_PORT2_PROXY_MODE) {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                      MPLS__HG_MODIFY_ENABLEf, 0x1);
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                      MPLS__HG_HDR_SELf, 0);
              } else {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                      MPLS__HG_MODIFY_ENABLEf, 0x0);
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                      MPLS__HG_HDR_SELf, 1);
              }
        } else {
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                        DVPf, egr_nh_info->dvp);
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &egr_nh, DVP_IS_NETWORK_PORTf,
                        egr_nh_info->dvp_is_network);
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                        HG_LEARN_OVERRIDEf, is_vpws ? 1 : 0);
              /* For L2 MPLS, HG_MODIFY_ENABLE must be 0x0 for Ingress and Egress Chip */
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                        HG_MODIFY_ENABLEf, 0x0);
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                        HG_HDR_SELf, 1);
        }
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit, soc_feature_mpls_nh_ttl_control)) {
           if (flags & BCM_MPLS_PORT2_KEEP_TTL) {
               soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                   MPLS__DISABLE_TTL_DECREMENTf, 1);
           }
        }
#endif
    } else if (egr_nh_info->entry_type == 0x2) {

        if (egr_nh_info->lltag_vlan != -1) {
            if (soc_feature(unit, soc_feature_lltag)) {
                if (egr_nh_info->lltag_pri != -1) {
                    /*  PCP, DE fields are copied from this table. */
                    soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                &egr_nh, SD_TAG__LLTAG_PCP_DE_SOURCEf, 0x02);
                    soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                &egr_nh, SD_TAG__LLTAG_PCPf, egr_nh_info->lltag_pri);
                    soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                &egr_nh, SD_TAG__LLTAG_DEf, egr_nh_info->lltag_cfi); 
                } else {
                    soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                &egr_nh, SD_TAG__LLTAG_PCP_DE_SOURCEf, 0x03);  
                }  
                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                &egr_nh, SD_TAG__LLTAG_ACTIONSf, 0x04);
                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                &egr_nh, SD_TAG__LLTAG_VIDf, egr_nh_info->lltag_vlan);
                
            }
        }

        if (egr_nh_info->sd_tag_vlan != -1) {
             if (soc_feature(unit, soc_feature_mpls_enhanced)) {
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
                if (soc_feature(unit, soc_feature_vp_sharing)) {
                    if (egr_nh_info->sd_tag_vlan == BCM_VLAN_INVALID) {
                        soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                       &egr_nh, SD_TAG__SD_TAG_NOT_VALIDf, 1);
                    } else {
                        soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                       &egr_nh, SD_TAG__SD_TAG_NOT_VALIDf, 0);
                        soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                    &egr_nh, SD_TAG__SD_TAG_VIDf, egr_nh_info->sd_tag_vlan);
                    }
                 } else
#endif
                {
                    soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                  &egr_nh, SD_TAG__SD_TAG_VIDf, egr_nh_info->sd_tag_vlan);
                }
             } else {
                 soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                &egr_nh, SD_TAG_VIDf, egr_nh_info->sd_tag_vlan);
             }
        }

        if (egr_nh_info->sd_tag_action_present != -1) {
             if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                   soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                             SD_TAG__SD_TAG_ACTION_IF_PRESENTf,
                             egr_nh_info->sd_tag_action_present);
             } else {
                   soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                             SD_TAG_ACTION_IF_PRESENTf,
                             egr_nh_info->sd_tag_action_present);
             }
        }

        if (egr_nh_info->sd_tag_action_not_present != -1) {
             if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                   soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                             SD_TAG__SD_TAG_ACTION_IF_NOT_PRESENTf,
                             egr_nh_info->sd_tag_action_not_present);
             } else {
                   soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                             SD_TAG_ACTION_IF_NOT_PRESENTf,
                             egr_nh_info->sd_tag_action_not_present);
             }
        }

        if (egr_nh_info->sd_tag_pri != -1) {
             if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                 if(soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm, SD_TAG__NEW_PRIf)) {
                       soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                             SD_TAG__NEW_PRIf,
                             egr_nh_info->sd_tag_pri);
                 }
             }
        }

        if (egr_nh_info->sd_tag_cfi != -1) {
             if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                   if(soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm, SD_TAG__NEW_CFIf)) {
                       soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                             SD_TAG__NEW_CFIf,
                             egr_nh_info->sd_tag_cfi);
                   }
             }
        }

        if (egr_nh_info->tpid_index != -1) {
             if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                &egr_nh, SD_TAG__SD_TAG_TPID_INDEXf,
                                egr_nh_info->tpid_index);
             } else {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                &egr_nh, SD_TAG_TPID_INDEXf,
                                egr_nh_info->tpid_index);
             }
        }

        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
            if ((egr_nh_info->flex_ctr_base_cnt_idx != -1) &&
                soc_feature(unit,soc_feature_advanced_flex_counter)) {
                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                 SD_TAG__FLEX_CTR_BASE_COUNTER_IDXf,
                                 egr_nh_info->flex_ctr_base_cnt_idx);
                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                 SD_TAG__FLEX_CTR_POOL_NUMBERf,
                                 egr_nh_info->flex_ctr_pool_num);
                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                 SD_TAG__FLEX_CTR_OFFSET_MODEf,
                                 egr_nh_info->flex_ctr_offset_mode);
            } else
#endif
            if ((egr_nh_info->vintf_ctr_index != -1) &&
                soc_feature(unit, soc_feature_gport_service_counters)) {
                if (soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm,
                                  SD_TAG__USE_VINTF_CTR_IDXf)) {
                     soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                  SD_TAG__USE_VINTF_CTR_IDXf,
                                  egr_nh_info->vintf_ctr_index ? 1 : 0);
                }
                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                 SD_TAG__VINTF_CTR_IDXf,
                                 egr_nh_info->vintf_ctr_index);
            }
        }

        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                        SD_TAG__BC_DROPf, drop ? 1 : 0);
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                        SD_TAG__UUC_DROPf, drop ? 1 : 0);
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                        SD_TAG__UMC_DROPf, drop ? 1 : 0);
        }

        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                        SD_TAG__BC_DROPf, drop ? 1 : 0);
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                        SD_TAG__UUC_DROPf, drop ? 1 : 0);
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                        SD_TAG__UMC_DROPf, drop ? 1 : 0);
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                        SD_TAG__DVPf, egr_nh_info->dvp);
              if (soc_feature(unit, soc_feature_multiple_split_horizon_group)) {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &egr_nh, SD_TAG__DVP_NETWORK_GROUPf,
                        egr_nh_info->dvp_network_group_id);
              } else {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &egr_nh, SD_TAG__DVP_IS_NETWORK_PORTf,
                        egr_nh_info->dvp_is_network);
              }
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                        SD_TAG__HG_LEARN_OVERRIDEf, is_vpws ? 1 : 0);
              /*
               * Generally for L2 MPLS,
               * - VPWS application, MPLS tunnel header can be encapsulated
               * at ingress chip or egress chip, the suggestion is add MPLS
               * header at egress chip to save HG bandwidth.
               *
               * - VPLS application, L2 learning is needed on egress chip,
               * that means the MPLS tunnel header need to be encapsulated at
               * the egress chip. SVP and DVP field in PPD type 2 will provide
               * infomation for these requirements.
               */
              if (flags & BCM_MPLS_PORT2_PROXY_MODE) {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                      SD_TAG__HG_MODIFY_ENABLEf, 0x1);
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                      SD_TAG__HG_HDR_SELf, 0);
              } else {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                      SD_TAG__HG_MODIFY_ENABLEf, 0x0);
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                      SD_TAG__HG_HDR_SELf, 1);
              }

              if (egr_nh_info->class_id != BCM_CLASS_ID_INVALID) {
                  if (soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm, SD_TAG__CLASS_IDf)) {
                      soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                              SD_TAG__CLASS_IDf, egr_nh_info->class_id);
                  }
              }
        } else {
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                        DVPf, egr_nh_info->dvp);
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &egr_nh, DVP_IS_NETWORK_PORTf,
                        egr_nh_info->dvp_is_network);
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                        HG_LEARN_OVERRIDEf, is_vpws ? 1 : 0);
              /* For L2 MPLS, HG_MODIFY_ENABLE must be 0x0 for Ingress and Egress Chip */
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                        HG_MODIFY_ENABLEf, 0x0);
              soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                        HG_HDR_SELf, 1);
        }
    } else {
         return BCM_E_PARAM;
    }

    if ((_BCM_MPLS_MEPID_IN_VALID_RANGE(egr_nh_info->oam_global_context_id)) &&
        (soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm, MPLS__MEPIDf))) {
        /* Writing MPLS LM/DM value. it doesnt matter since
           data packets are similar for BHH and MPLS LM/DM */
        soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                MPLS__MPLS_OAM_DOWNMEP_ENABLEf, 2);
        soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                MPLS__MEPIDf, egr_nh_info->oam_global_context_id);
    }

    soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                        ENTRY_TYPEf, egr_nh_info->entry_type);

    rv = soc_mem_write(unit, EGR_L3_NEXT_HOPm,
                       MEM_BLOCK_ALL, nh_index, &egr_nh);

    return rv;
}

/*
 * Function:
 *       _bcm_tr_mpls_compute_nhop_hash
 * Purpose:
 *       Compute the hash Index from Egress Tunnel If
 * Parameters:
 *       unit - (IN) bcm device.
 *       egress_if  - (IN) Egress Tunnel If
 * Returns:
 *       BCM_E_XXX
 */

STATIC void
_bcm_tr_mpls_compute_nhop_hash (bcm_if_t egress_if, int *hash_idx)
{
    *hash_idx = egress_if % 1024;
}

/*
 * Function:
 *		_bcm_tr_mpls_add_vp_nh
 * Purpose:
 *		Add MPLS VP NextHop index into a list
 * Parameters:
 *		unit	- (IN) bcm device.
 *		mpls_port	- (IN) MPLS Port
 *		nh_index	 - (IN) VP NextHop Index
 * Returns:
 *		BCM_E_XXX
 */

STATIC int
_bcm_tr_mpls_add_vp_nh ( int unit, bcm_if_t  egr_if, int nh_index )
{
    _bcm_mpls_egr_nhopList_t  *nhop_cursorPtr;
    _bcm_mpls_vp_nh_list_t  *vp_cursorPtr;
    _bcm_mpls_vp_nh_list_t *newVpNode;
    int   hash_idx;

    (void) _bcm_tr_mpls_compute_nhop_hash (egr_if, &hash_idx);


    /* If Node is present, then insert the VP_index */
    for (nhop_cursorPtr = _bcm_tr_mpls_nhop_headPtr[unit][hash_idx]; 
           nhop_cursorPtr != NULL; nhop_cursorPtr = nhop_cursorPtr->link) {
        if (nhop_cursorPtr->egr_if == egr_if ) {
            for (vp_cursorPtr = nhop_cursorPtr->vp_head_ptr; vp_cursorPtr != NULL; vp_cursorPtr = vp_cursorPtr->link) {
                if (vp_cursorPtr->vp_nh_idx == nh_index) {
                    return BCM_E_NONE;
                }
            }
            newVpNode = (_bcm_mpls_vp_nh_list_t *) sal_alloc (sizeof (_bcm_mpls_vp_nh_list_t), "VP NH Index");
            if (NULL == newVpNode) {
                return (BCM_E_MEMORY); 
            }	 
            newVpNode->vp_nh_idx = nh_index;
            newVpNode->link = nhop_cursorPtr->vp_head_ptr;
            nhop_cursorPtr->vp_head_ptr = newVpNode;
            return BCM_E_NONE;
        }
    }
    return BCM_E_NOT_FOUND;
}


/*
 * Function:
 *       bcm_tr_mpls_update_vp_nh
 * Purpose:
 *       Update NextHop Table indexed by VP NextHop index
 * Parameters:
 *unit	- (IN) bcm device.
 *       egr_index - (IN) NextHop Index of Parent Egr Object
 * Returns:
 *       BCM_E_XXX
 */

int
bcm_tr_mpls_update_vp_nh ( int unit, bcm_if_t  egr_if )
{
    _bcm_mpls_egr_nhopList_t  *nhop_cursorPtr;
    _bcm_mpls_vp_nh_list_t  *vp_cursorPtr;
    ing_l3_next_hop_entry_t ing_nh;
    egr_l3_next_hop_entry_t egr_nh;
    initial_ing_l3_next_hop_entry_t   initial_ing_nh;
    egr_mac_da_profile_entry_t macda;
    _bcm_tr_ing_nh_info_t ing_nh_info;
    _bcm_tr_egr_nh_info_t egr_nh_info;
    bcm_l3_egress_t nh_info;
    void *entries[1];
    int  vp_index=-1;
    int  egr_index=-1;
    int  hash_idx=-1;
    int  old_macda_idx=-1;
    uint32 flags=0;

    (void) _bcm_tr_mpls_compute_nhop_hash (egr_if, &hash_idx);


    for (nhop_cursorPtr = _bcm_tr_mpls_nhop_headPtr[unit][hash_idx]; 
              nhop_cursorPtr != NULL; nhop_cursorPtr = nhop_cursorPtr->link) {
        if (nhop_cursorPtr->egr_if == egr_if) {
            for (vp_cursorPtr = nhop_cursorPtr->vp_head_ptr;  vp_cursorPtr != NULL; vp_cursorPtr = vp_cursorPtr->link) {
                vp_index = vp_cursorPtr->vp_nh_idx;

                /* Get the Egress Hext Hop Index from the Egress Object Id without incrementing ref_count*/
                BCM_IF_ERROR_RETURN(bcm_xgs3_get_nh_from_egress_object(unit, egr_if, &flags, 0, &egr_index));
 
                /* Read the egress next_hop entry pointed by EGRess-Object */	  
                BCM_IF_ERROR_RETURN(bcm_xgs3_nh_get(unit, egr_index, &nh_info));  

                /* Obtain  the MPLS tunnel destination port information. */
                if (nh_info.flags & BCM_L3_TGID) {
                    ing_nh_info.port = -1;
                    ing_nh_info.module = -1;
                    ing_nh_info.trunk = nh_info.trunk; 
                } else {
                    BCM_IF_ERROR_RETURN(_bcm_esw_stk_modmap_map(unit, 
                                                                BCM_STK_MODMAP_SET,
                                                                nh_info.module, nh_info.port,
                                                                &nh_info.module, &nh_info.port));
                    ing_nh_info.port = nh_info.port;
                    ing_nh_info.module = nh_info.module;
                    ing_nh_info.trunk = -1; 
                }
                egr_nh_info.intf_num = nh_info.intf;
                egr_nh_info.entry_type = 0x1; /* MPLS_MACDA_PROFILE */

                /* Read the existing egress next_hop entry pointed by VP NextHop Index */
                BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ALL, 
                                                  vp_index, &egr_nh));

                /* Delete existing MACDA_PROFILE */
                if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                    old_macda_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                         MPLS__MAC_DA_PROFILE_INDEXf);
                } else {
                    old_macda_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                         MAC_DA_PROFILE_INDEXf);
                }

                if (old_macda_idx != -1) {
                   BCM_IF_ERROR_RETURN
                        (_bcm_mac_da_profile_entry_delete(unit, old_macda_idx));
                }

                /* Obtain new MACDA_PROFILE */
                sal_memset(&macda, 0, sizeof(egr_mac_da_profile_entry_t));
                soc_mem_mac_addr_set(unit, EGR_MAC_DA_PROFILEm, 
                                     &macda, MAC_ADDRESSf, nh_info.mac_addr);
                entries[0] = &macda;
                BCM_IF_ERROR_RETURN(_bcm_mac_da_profile_entry_add(unit, entries, 1,
                                                                  (uint32 *) &egr_nh_info.macda_index));

                /* Modify  the Egress NextHop entry pointed by VP NextHop Index */
                if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                     soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                    MPLS__MAC_DA_PROFILE_INDEXf, egr_nh_info.macda_index);
                     soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                    MPLS__INTF_NUMf, egr_nh_info.intf_num);
                } else {
                     soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                    MAC_DA_PROFILE_INDEXf, egr_nh_info.macda_index);
                     soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                    INTF_NUMf, egr_nh_info.intf_num);
                }
                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                    ENTRY_TYPEf, egr_nh_info.entry_type);

                if ((_BCM_MPLS_MEPID_IN_VALID_RANGE(nh_info.oam_global_context_id)) &&
                        (soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm, MPLS__MEPIDf))) {
                    /* Writing MPLS LM/DM value. it doesnt matter since
                       data packets are similar for BHH and MPLS LM/DM */
                    soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                            MPLS__MPLS_OAM_DOWNMEP_ENABLEf, 2);
                    soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                            MPLS__MEPIDf, nh_info.oam_global_context_id);
                }


                /* Update HW Table Memory */
                BCM_IF_ERROR_RETURN (soc_mem_write(unit, EGR_L3_NEXT_HOPm,
                                                   MEM_BLOCK_ALL, vp_index, &egr_nh));


                /* Read the existing Ingress next_hop entry pointed by VP NextHop Index */
                BCM_IF_ERROR_RETURN (soc_mem_read(unit, ING_L3_NEXT_HOPm, MEM_BLOCK_ALL, 
                                                  vp_index, &ing_nh));

                /* Modify  the Ingress NextHop entry pointed by VP NextHop Index */
                if (ing_nh_info.trunk == -1) {
                    soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                                        &ing_nh, Tf, 0);
                    soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                                        &ing_nh, TGIDf, 0);
                    soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                                        &ing_nh, PORT_NUMf, ing_nh_info.port);
                    soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                                        &ing_nh, MODULE_IDf, ing_nh_info.module);

                } else {
                    soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                                        &ing_nh, PORT_NUMf, 0);
                    soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                                        &ing_nh, MODULE_IDf, 0);
                    soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                                        &ing_nh, Tf, 1);
                    soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                                        &ing_nh, TGIDf, ing_nh_info.trunk);
                }

                /* Update HW Table Memory */
                BCM_IF_ERROR_RETURN (soc_mem_write (unit, ING_L3_NEXT_HOPm,
                                                    MEM_BLOCK_ALL, vp_index, &ing_nh));

                /* Read the existing Initial-Ingress next_hop entry pointed by VP NextHop Index */
                BCM_IF_ERROR_RETURN (soc_mem_read(unit, INITIAL_ING_L3_NEXT_HOPm, MEM_BLOCK_ALL, 
                                                  vp_index, &initial_ing_nh));

                /* Modify the Initial-Ingress NextHop entry pointed by VP NextHop Index */
                if (ing_nh_info.trunk == -1) {
                    soc_mem_field32_set(unit, INITIAL_ING_L3_NEXT_HOPm,
                                        &initial_ing_nh, PORT_NUMf, ing_nh_info.port);
                    soc_mem_field32_set(unit, INITIAL_ING_L3_NEXT_HOPm,
                                        &initial_ing_nh, MODULE_IDf, ing_nh_info.module);
                    soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                                        &initial_ing_nh, Tf, 0);
                } else {
                   soc_mem_field32_set(unit, INITIAL_ING_L3_NEXT_HOPm,
                                        &initial_ing_nh, Tf, 1);
                    soc_mem_field32_set(unit, INITIAL_ING_L3_NEXT_HOPm,
                                        &initial_ing_nh, TGIDf, ing_nh_info.trunk);
                }

                /* Update HW Table Memory */
                BCM_IF_ERROR_RETURN (soc_mem_write(unit, INITIAL_ING_L3_NEXT_HOPm,
                                                   MEM_BLOCK_ALL, vp_index, &initial_ing_nh));

            }
        }
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *       bcm_dump_mpls_port
 * Purpose:
 *       Displays MPLS Port Structure Information
 * Parameters:
 *       mpls_port - (IN) MPLS Port Structure
 * Returns:
 *       None
 */

	
void
bcm_dump_mpls_port (bcm_mpls_port_t *mpls_port) 
{
    LOG_CLI((BSL_META("\n MPLS Port ID: %d"), (bcm_gport_t) mpls_port->mpls_port_id));
    LOG_CLI((BSL_META("\n Egress Tunnel If: %d"),
             (bcm_if_t) mpls_port->egress_tunnel_if));
    LOG_CLI((BSL_META("\n flags: %X"), mpls_port->flags));
    LOG_CLI((BSL_META("\n service_tpid: %d"), mpls_port->service_tpid));
    LOG_CLI((BSL_META("\n match_vlan: %d"), mpls_port->match_vlan));
    LOG_CLI((BSL_META("\n mpls_label: %d"), (bcm_mpls_label_t) mpls_port->match_label));
    LOG_CLI((BSL_META("\n encap_id: %d"), (bcm_if_t) mpls_port->encap_id));
}

/* 
 * Function:   * Function: 
 *              _bcm_tr_mpls_dump_egrNhop 
 * Purpose: 
 *              Dump Egr NextHop List
 * Parameters: 
 *              unit    - (IN) bcm device. 
 *              vp_idx   - (IN) VP NextHop Index
 * Returns: 
 *              BCM_E_XXX 
 */ 
     
void
_bcm_tr_mpls_dump_egrNhop (int unit)
{ 
    _bcm_mpls_egr_nhopList_t  *nhop_cursorPtr; 
    _bcm_mpls_vp_nh_list_t *vp_cursorPtr; 
    int   index; 

    for(index=0; index < _BCM_TR_MPLS_HASH_ELEMENTS; ++index) {
        if (_bcm_tr_mpls_nhop_headPtr[unit][index] != NULL) {
            for (nhop_cursorPtr = _bcm_tr_mpls_nhop_headPtr[unit][index]; 
                 nhop_cursorPtr != NULL; nhop_cursorPtr = nhop_cursorPtr->link) {
                LOG_CLI((BSL_META_U(unit,
                                    "\n Egress Tunnel If: %d"),
                         (bcm_if_t) nhop_cursorPtr->egr_if));
                for (vp_cursorPtr = nhop_cursorPtr->vp_head_ptr; 
                     vp_cursorPtr != NULL; vp_cursorPtr = vp_cursorPtr->link) { 
                    LOG_CLI((BSL_META_U(unit,
                                        "\n VP Nhop Idx= %d"),
                             vp_cursorPtr->vp_nh_idx));
                } 
            }
        }
    }
}


/*
 * Function:
 *           _bcm_tr_mpls_add_egrNhop
 * Purpose:
 *            Add Nhop information into a Hash-Table
 * Parameters:
 *            unit - (IN) bcm device.
 *            egress_if - (IN) Egress Tunnel If
 * Returns:
 *            BCM_E_XXX
 */

STATIC int
_bcm_tr_mpls_add_egrNhop ( int unit, bcm_if_t egress_if)
{
    _bcm_mpls_egr_nhopList_t  *newNhopNode;
    _bcm_mpls_egr_nhopList_t  *nhop_cursorPtr;
    int   hash_idx;

    (void) _bcm_tr_mpls_compute_nhop_hash (egress_if, &hash_idx);
    /* Does the Node exist? */
    for (nhop_cursorPtr = _bcm_tr_mpls_nhop_headPtr[unit][hash_idx]; 
         nhop_cursorPtr != NULL; nhop_cursorPtr = nhop_cursorPtr->link) {
        if (nhop_cursorPtr->egr_if == egress_if) {
            return (BCM_E_NONE);
        } 
    }

    newNhopNode = (_bcm_mpls_egr_nhopList_t *) 
        sal_alloc (sizeof (_bcm_mpls_egr_nhopList_t), "MPLS Egr Nhop");
    if (NULL == newNhopNode) {
        return (BCM_E_MEMORY); 
    }
    newNhopNode->egr_if = (bcm_if_t) egress_if;
    newNhopNode->vp_head_ptr = ( _bcm_mpls_vp_nh_list_t *) NULL;
    newNhopNode->link = _bcm_tr_mpls_nhop_headPtr[unit][hash_idx];
    _bcm_tr_mpls_nhop_headPtr[unit][hash_idx] = newNhopNode;
    return BCM_E_NONE;
}


/*
 * Function:
 *            _bcm_tr_mpls_delete_egrNhop
 * Purpose:
 *            Delete MPLS Egress NHOP from Hash-table
 * Parameters:
 *           unit	- (IN) bcm device.
 *           egress_if - (IN) Egress Tunnel If
 * Returns:
 *            BCM_E_XXX
 */

STATIC int
_bcm_tr_mpls_delete_egrNhop (int unit,  bcm_if_t egress_if )
{
    _bcm_mpls_egr_nhopList_t  *nhop_cursorPtr;
    _bcm_mpls_egr_nhopList_t  *removeNode;
    int   hash_idx;

    (void) _bcm_tr_mpls_compute_nhop_hash (egress_if, &hash_idx);

    removeNode = _bcm_tr_mpls_nhop_headPtr[unit][hash_idx];
    if (removeNode != NULL)  {
        if (removeNode->egr_if ==  egress_if) {
            if (removeNode->vp_head_ptr == NULL) {
                _bcm_tr_mpls_nhop_headPtr[unit][hash_idx] = removeNode->link;
                sal_free (removeNode);
                return BCM_E_NONE;
            } else {
                return BCM_E_BUSY;
            }
        } else {
            for (nhop_cursorPtr = _bcm_tr_mpls_nhop_headPtr[unit][hash_idx]; 
                 nhop_cursorPtr->link != NULL; nhop_cursorPtr = nhop_cursorPtr->link) {
                if (nhop_cursorPtr->link->egr_if ==  egress_if) {
                    if (nhop_cursorPtr->link->vp_head_ptr == NULL) {
                        removeNode = nhop_cursorPtr->link;
                        nhop_cursorPtr->link = removeNode->link;
                        sal_free (removeNode);
                        return BCM_E_NONE;
                    } else {
                        return BCM_E_BUSY;
                    }
                }
            }
        }
    }
    return BCM_E_NOT_FOUND;
} 

/*
 * Function:
 *            _bcm_tr_mpls_delete_vp_nh
 * Purpose:
 *            Delete VP NextHop information from List
 * Parameters:
 *           unit - (IN) bcm device.
 *           vp_idx - (IN) VP NextHop Index
 *           egress_if - (IN) Egress Tunnel If
 * Returns:
 *            BCM_E_XXX
 */

STATIC int
_bcm_tr_mpls_delete_vp_nh ( int unit,  int vp_idx, bcm_if_t egress_if)
{
    _bcm_mpls_egr_nhopList_t  *nhop_cursorPtr;
    _bcm_mpls_vp_nh_list_t  *vp_cursorPtr;
    _bcm_mpls_vp_nh_list_t  *vpRemoveNode;	
    int   hash_idx;

    (void) _bcm_tr_mpls_compute_nhop_hash (egress_if, &hash_idx);

    for (nhop_cursorPtr = _bcm_tr_mpls_nhop_headPtr[unit][hash_idx]; 
         nhop_cursorPtr != NULL; nhop_cursorPtr = nhop_cursorPtr->link) {
        vp_cursorPtr = nhop_cursorPtr->vp_head_ptr;
        if (vp_cursorPtr  != NULL) {
            if (vp_cursorPtr->vp_nh_idx == vp_idx)  {
                nhop_cursorPtr->vp_head_ptr = vp_cursorPtr->link;
                sal_free (vp_cursorPtr);
                if (nhop_cursorPtr->vp_head_ptr == NULL) {
                    BCM_IF_ERROR_RETURN(
                        _bcm_tr_mpls_delete_egrNhop (unit, nhop_cursorPtr->egr_if));
                }
                return BCM_E_NONE;
            } else {
                for ( ; vp_cursorPtr->link != NULL; vp_cursorPtr = vp_cursorPtr->link){
                    if (vp_cursorPtr->link->vp_nh_idx == vp_idx) {
                        vpRemoveNode = vp_cursorPtr->link;
                        vp_cursorPtr->link = vpRemoveNode->link;
                        sal_free (vpRemoveNode);
                        return BCM_E_NONE;
                    }
                }
            } 
        }
    }
    return BCM_E_NOT_FOUND;
}

/*
 * Function:
 *       _bcm_tr_mpls_cleanup_egrNhop
 * Purpose:
 *      Cleanup MPLS Egress NHOP from Hash-table
 * Parameters:
 *      unit - (IN) bcm device.
 * Returns:
 *      BCM_E_XXX
 */

STATIC void
_bcm_tr_mpls_cleanup_egrNhop (int unit)
{
    _bcm_mpls_egr_nhopList_t  *nhop_cursorPtr;
    _bcm_mpls_vp_nh_list_t	*vp_cursorPtr;
    int   index;

    for(index=0; index < _BCM_TR_MPLS_HASH_ELEMENTS; ++index) {
        if (_bcm_tr_mpls_nhop_headPtr[unit][index] != NULL) {
            nhop_cursorPtr = _bcm_tr_mpls_nhop_headPtr[unit][index];
            while ( nhop_cursorPtr != NULL ) {
                vp_cursorPtr = nhop_cursorPtr->vp_head_ptr;
                while (vp_cursorPtr != NULL) {
                    (void)_bcm_tr_mpls_delete_vp_nh (unit, vp_cursorPtr->vp_nh_idx,
                                                     nhop_cursorPtr->egr_if);
                    nhop_cursorPtr = _bcm_tr_mpls_nhop_headPtr[unit][index];
                    if (nhop_cursorPtr != NULL) {
                        vp_cursorPtr = nhop_cursorPtr->vp_head_ptr;
                    } else {
                        vp_cursorPtr = NULL;
                    }
                }
            }
        }
    }
} 

/* 
 * Function:   * Function: 
 *              bcm_tr_mpls_get_vp_nh 
 * Purpose: 
 *              Find whether  VP NextHop is present within list 
 * Parameters: 
 *              unit    - (IN) bcm device. 
 *              vp_idx   - (IN) VP NextHop Index
 *              egress_if - (OUT) Egr_If
 * Returns: 
 *              BCM_E_XXX 
 */ 
     
int 
bcm_tr_mpls_get_vp_nh (int unit, int vp_index, bcm_if_t *egress_if) 
{ 
    _bcm_mpls_egr_nhopList_t  *nhop_cursorPtr; 
    _bcm_mpls_vp_nh_list_t *vp_cursorPtr; 
    int   index; 

    for(index=0; index < _BCM_TR_MPLS_HASH_ELEMENTS; ++index) {
        if (_bcm_tr_mpls_nhop_headPtr[unit][index] != NULL) {
            for (nhop_cursorPtr = _bcm_tr_mpls_nhop_headPtr[unit][index]; 
                 nhop_cursorPtr != NULL; nhop_cursorPtr = nhop_cursorPtr->link) { 
                for (vp_cursorPtr = nhop_cursorPtr->vp_head_ptr; 
                     vp_cursorPtr != NULL; vp_cursorPtr = vp_cursorPtr->link) { 
                    if (vp_index == vp_cursorPtr->vp_nh_idx) {
                        *egress_if = nhop_cursorPtr->egr_if;
                        return BCM_E_NONE; 
                    }
                } 
            }
        }
    }
    return BCM_E_NOT_FOUND; 
}

/* 
 * Function:   * Function: 
 *              _bcm_tr_mpls_create_vp_nh 
 * Purpose: 
 *              Allocate MPLS VP NextHop index entry
 * Parameters: 
 *              unit    - (IN) bcm device. 
 *              egress_if - (IN) Egr_If
 *              vp_nh_index   - (OUT) NextHop Index
 * Returns: 
 *              BCM_E_XXX 
 */ 
     
STATIC int 
_bcm_tr_mpls_create_vp_nh ( int unit, bcm_if_t  egress_tunnel_if,
                             int  *vp_nh_index, bcm_mpls_port_t *mpls_port)
{
    int rv;
    bcm_l3_egress_t nh_info;
    uint32 nh_flags;
    int flags = mpls_port->flags;
    int failover_nh_index = -1;
    int failover_vp;
    int i = 0;
    int num_elements = 1;
    ing_dvp_table_entry_t failover_dvp;
    /*
     * Allocate MPLS VP next-hop entry. By calling bcm_xgs3_nh_add()
     * with _BCM_L3_SHR_WRITE_DISABLE flag, a next-hop index is
     * allocated but nothing is written to hardware. The "nh_info"
     * in this case is not used, so just set to all zeros.
     */

    bcm_l3_egress_t_init(&nh_info);

    nh_info.failover_id       = mpls_port->failover_id;
    nh_info.failover_mc_group = mpls_port->failover_mc_group;

    failover_vp = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->failover_port_id);
    if (failover_vp !=-1) {
        BCM_IF_ERROR_RETURN(READ_ING_DVP_TABLEm(
            unit, MEM_BLOCK_ANY, failover_vp, &failover_dvp));

        failover_nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &failover_dvp,
                                             NEXT_HOP_INDEXf);
        failover_nh_index += BCM_XGS3_DVP_EGRESS_IDX_MIN;
    }

    nh_info.failover_if_id    = failover_nh_index;

    nh_flags = _BCM_L3_SHR_MATCH_DISABLE | _BCM_L3_SHR_WRITE_DISABLE;
    if (flags & BCM_MPLS_PORT_ENCAP_WITH_ID) {
        nh_flags |= _BCM_L3_SHR_WITH_ID | _BCM_L3_SHR_UPDATE;
    }
    BCM_IF_ERROR_RETURN(bcm_xgs3_nh_add(unit, nh_flags, &nh_info, vp_nh_index));

    rv = _bcm_tr_mpls_add_egrNhop (unit, egress_tunnel_if);
    if (rv < 0) {
       num_elements = bcmi_l3_nh_multi_count_get(unit, *vp_nh_index);
       for (; i<num_elements; i++) {
           (void) bcm_xgs3_nh_del(unit, _BCM_L3_SHR_WRITE_DISABLE, *vp_nh_index+i);
        }
        return rv;
    }

    rv = _bcm_tr_mpls_add_vp_nh (unit, egress_tunnel_if, *vp_nh_index);
    if (rv < 0) {
       num_elements = bcmi_l3_nh_multi_count_get(unit, *vp_nh_index);
       for (; i<num_elements; i++) {
           (void) bcm_xgs3_nh_del(unit, _BCM_L3_SHR_WRITE_DISABLE, *vp_nh_index+i);
        }
       (void) _bcm_tr_mpls_delete_egrNhop (unit, egress_tunnel_if);
    }

   return rv;
}

/*
 * Function:
 *      _bcm_tr_mpls_ing_queue_map_ref_count_adjust
 * Purpose:
 *      Increment / Decrement Reference Count for Ingress Queue Map
 * Parameters:
 *      unit              Unit number
 *      index            (IN) Entry Index
 *      step              (IN) usage reference
 */

STATIC void
_bcm_tr_mpls_ing_queue_map_ref_count_adjust (int unit, int index, int step)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);

    if (( mpls_info->ing_qmap_ref_count[index] == 0 ) && (step < 0)) {
         mpls_info->ing_qmap_ref_count[index] = 0;
    } else {
         mpls_info->ing_qmap_ref_count[index] += step;
    }
}


/*
 * Function:
 *       _bcm_tr_mpls_ing_queue_map_index_reset
 * Purpose:
 *       Reset  ING_QUEUE_MAP_TABLE entry
 * Parameters:
 *           IN :  Unit
 *           IN : ing_queue_map_index
 * Returns:
 *            BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_ing_queue_map_index_reset (int unit, int ing_queue_map_index)
{
    ing_queue_map_entry_t  ing_queue_entry;
    int rv = BCM_E_NONE;
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);

    if ( mpls_info->ing_qmap_ref_count[ing_queue_map_index] != 0 ) {
         return BCM_E_NONE;
    }

    sal_memset(&ing_queue_entry, 0,
              sizeof(ing_queue_map_entry_t));

    rv = BCM_XGS3_MEM_WRITE(unit, ING_QUEUE_MAPm,
                            ing_queue_map_index, &ing_queue_entry);

    return rv;
}
#if defined(BCM_TRIUMPH3_SUPPORT)
/*
 * Function:
 *       _bcm_tr_mpls_vc_swap_table_hash_nhop_alloc
 * Purpose:
 *       Allocate an nhop entry for vc and swap hash
 * Parameters:
 *       unit           - (IN) Device Number
 *       nh_index    -(IN) Next hop index
 * Returns:
 *       The pointer of nhop entry
 */

bcm_tr_mpls_vc_swap_table_nhop_list_t *
_bcm_tr_mpls_vc_swap_table_hash_nhop_alloc(int unit, int nh_index)
{
    bcm_tr_mpls_vc_swap_table_nhop_list_t *ptr;
    ptr =  sal_alloc(sizeof(bcm_tr_mpls_vc_swap_table_nhop_list_t),
                     "vc and swap table hash nhop list");
    sal_memset(ptr, 0, sizeof(bcm_tr_mpls_vc_swap_table_nhop_list_t));

    ptr->nh_index = nh_index;
    ptr->next = NULL;

    return ptr;
}
/*
 * Function:
 *       _bcm_tr_mpls_vc_and_swap_hash_nhop_delete
 * Purpose:
 *       Delete an nhop entry from vc and swap hash
 * Parameters:
 *       unit                  - (IN) Device Number
 *       vc_swap_index   -(IN) Vc and swap table index
 *       nh_index           -(IN) Next hop index
 * Returns:
 *       BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_vc_and_swap_hash_nhop_delete (int unit, int vc_swap_index,
                                                           int nh_index)
{
    int hash_idx;
    bcm_tr_mpls_vc_swap_table_hash_t *hash_input;
    bcm_tr_mpls_vc_swap_table_hash_entry_t *temp_hash;
    bcm_tr_mpls_vc_swap_table_nhop_list_t *temp_nhop, *prev_nhop;
    egr_mpls_vc_and_swap_label_table_entry_t vc_entry;

    /* Entry 0 is reserved with _bcm_tr_mpls_default_entry_setup during init */
    if (vc_swap_index == VC_AND_SWAP_TABLE_RESV_INDEX) {
        LOG_INFO(BSL_LS_BCM_L3,
                (BSL_META_U(unit,"Not resetting reserved index: %d in "
                            "VC AND SWAP TABLE\n"), VC_AND_SWAP_TABLE_RESV_INDEX));
        return BCM_E_NONE;
    }

    BCM_IF_ERROR_RETURN(
        soc_mem_read(unit,
                     EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                     MEM_BLOCK_ALL, vc_swap_index, &vc_entry));

    BCM_IF_ERROR_RETURN(
        bcmi_tr_mpls_vc_swap_hash_index_get(unit, NULL, 0, NULL, NULL,
                                            &vc_entry, &hash_idx));
    hash_input = &(bcmi_vc_swap_label_hash[unit][hash_idx]);
    temp_hash = hash_input->head_link;
    while (temp_hash) {
        if (temp_hash->index == vc_swap_index) {
            temp_nhop = prev_nhop = temp_hash->nhop_list;
            while (temp_nhop) {
                if (temp_nhop->nh_index == nh_index) {
                    if (temp_nhop == temp_hash->nhop_list) {
                        temp_hash->nhop_list = temp_nhop->next;
                    } else {
                        prev_nhop->next = temp_nhop->next;
                    }
                    sal_free(temp_nhop);
                    return BCM_E_NONE;
                }
                prev_nhop = temp_nhop;
                temp_nhop = temp_nhop->next;
            }
            return BCM_E_NOT_FOUND;
        }
        temp_hash = temp_hash->link;
    }
    return BCM_E_NOT_FOUND;
}
/*
 * Function:
 *       _bcm_tr_mpls_vc_and_swap_hash_nhop_add
 * Purpose:
 *       Add an nhop entry to vc and swap hash
 * Parameters:
 *       unit                  - (IN) Device Number
 *       vc_swap_index   -(IN) Vc and swap table index
 *       nh_index           -(IN) Next hop index
 * Returns:
 *       BCM_E_XXX
 */

STATIC int
_bcm_tr_mpls_vc_and_swap_hash_nhop_add (int unit, int vc_swap_index,
                                                        int nh_index)
{
    int hash_idx;
    bcm_tr_mpls_vc_swap_table_hash_t *hash_input;
    bcm_tr_mpls_vc_swap_table_hash_entry_t *temp_hash;
    bcm_tr_mpls_vc_swap_table_nhop_list_t *new_entry;
    bcm_tr_mpls_vc_swap_table_nhop_list_t *temp_nhop, *prev_nhop;
    egr_mpls_vc_and_swap_label_table_entry_t vc_entry;

    BCM_IF_ERROR_RETURN(
        soc_mem_read(unit,
                     EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                     MEM_BLOCK_ALL, vc_swap_index, &vc_entry));

    BCM_IF_ERROR_RETURN(
        bcmi_tr_mpls_vc_swap_hash_index_get(unit, NULL, 0, NULL, NULL,
                                            &vc_entry, &hash_idx));

    new_entry = _bcm_tr_mpls_vc_swap_table_hash_nhop_alloc(unit, nh_index);
    if (new_entry == NULL) {
        return BCM_E_MEMORY;
    }
    hash_input = &(bcmi_vc_swap_label_hash[unit][hash_idx]);
    temp_hash = hash_input->head_link;
    while (temp_hash) {
        if (temp_hash->index == vc_swap_index) {
            temp_nhop = prev_nhop = temp_hash->nhop_list;
            while (temp_nhop) {
                prev_nhop = temp_nhop;
                temp_nhop = temp_nhop->next;
            }
            if (prev_nhop == temp_nhop) {
                temp_hash->nhop_list = new_entry;
            } else {
                prev_nhop->next = new_entry;
            }
            return BCM_E_NONE;
        }
        temp_hash = temp_hash->link;
    }
    /* No entry was found, return BCM_E_NOT_FOUND */
    sal_free(new_entry);
    return BCM_E_NOT_FOUND;
}
#endif  /* BCM_TRIUMPH3_SUPPORT*/
/*
 * Function:
 *      _bcm_tr_mpls_vc_and_swap_ref_count_adjust
 * Purpose:
 *      Increment / Decrement Reference Count for VC usage
 * Parameters:
 *      unit              Unit number
 *      index            (IN) Entry Index
 *      step              (IN) usage reference
 */

STATIC void 
_bcm_tr_mpls_vc_and_swap_ref_count_adjust (int unit, int index, int step)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);

    if (( mpls_info->vc_swap_ref_count[index] == 0 ) && (step < 0)) {
         mpls_info->vc_swap_ref_count[index] = 0;
    } else {
         mpls_info->vc_swap_ref_count[index] += step;
    }

}

STATIC void
_bcm_tr_mpls_vc_and_swap_ref_count_get(int unit, int index, int *cnt_value)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    *cnt_value = mpls_info->vc_swap_ref_count[index];
}

STATIC void
_bcm_tr_mpls_vc_and_swap_ref_count_reset(int unit, int index)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    mpls_info->vc_swap_ref_count[index] = 0;
}
/*
 * Function:
 *     bcmi_tr_mpls_vc_swap_hash_key_get 
 * Purpose:
 *     Generate hash key from incoming entry
 * Parameters:
 *     IN  :  Unit
 *     IN  :  mpls port entry
 *     IN  :  next hop entry
 *     IN  :  action
 *     IN  :  VC table entry
 *     IN  :  key buffer to be filled in 
 * Returns:
 *     BCM_E_XXX
 */
int
bcmi_tr_mpls_vc_swap_hash_key_get(int unit, bcm_mpls_port_t *mpls_port,
                             bcm_mpls_tunnel_switch_t *info, 
                             bcm_l3_egress_t *egr_obj, int action,
                             egr_mpls_vc_and_swap_label_table_entry_t *vc_entry,
                             int *key)
{   

    if (key == NULL) {
        return BCM_E_INTERNAL;
    }
    /* make sure that key is zeroed out from wherever it is coming. */
    sal_memset(key, 0, sizeof(int) * BCM_TR_MPLS_VC_SWAP_KEY_LEN_IN_WORDS);

    /* Get hash key from various mpls_port, label info, egress_obj, hw_entry.
        below is the key.
        key[0] = label
        key[1] = action

        All the fields will default to 0 if not present. 
     */
    if (mpls_port != NULL) {
        key[0] = mpls_port->egress_label.label;
        key[1] = action;

    } else if (info !=NULL) {

        key[0] = info->egress_label.label;
        key[1] = action;

    } else if (egr_obj != NULL) {
        key[0] = egr_obj->mpls_label;
        key[1] = action;
    } else if (vc_entry != NULL) {
        key[0] = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_LABELf);

        key[1] = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_LABEL_ACTIONf);

    }
#if defined(BCM_SWAP_TO_SELF_SUPPORT)
    if (soc_feature(unit, soc_feature_mpls_swap_label_preserve)) {
        if (key[1] == _BCM_MPLS_ACTION_PRESERVED) {
            key[0] = 0;
            return BCM_E_NONE;
        }
    }
#endif    

/* if the label is invalid then set the key as 0 */
    if (key[0] == BCM_MPLS_LABEL_INVALID) {
        key[0] = 0;
        key[1] = _BCM_MPLS_ACTION_NOOP;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *       _bcm_tr_mpls_vc_swap_compare 
 * Purpose:
 *       Compare two incoming entries
 * Parameters:
 *       IN  :  Unit
 *       IN  :  mpls port entry
 *       IN  :  next hop entry
 *       IN  :  action
 *       IN  :  VC table entry

 * Returns:
 *       BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_vc_swap_compare(int unit, bcm_mpls_port_t *mpls_port,
               bcm_mpls_tunnel_switch_t *info, 
               bcm_l3_egress_t *egr_obj, int label_action, 
               egr_mpls_vc_and_swap_label_table_entry_t *vc_entry)
{
    bcm_mpls_label_t entry_label;
    uint32 ttl, entry_label_action, cw, exp;
    uint32 cfi, pri;
    uint32 sd_tag, sd_tag_action_present, sd_tag_action_not_present;
    uint32 exp_select, pri_mapping_ptr;
    int map_inx;

    if (mpls_port != NULL) {
         entry_label = SOC_MEM_VC_SWAP_F32_GET(unit, 
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_LABELf);
         if (entry_label != mpls_port->egress_label.label) {
             return BCM_E_NOT_FOUND;
         }
         entry_label_action = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,vc_entry, 
                        MPLS_LABEL_ACTIONf);
         if (label_action != entry_label_action) {
             return BCM_E_NOT_FOUND;
         }
         ttl = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_TTLf);
         if (mpls_port->egress_label.flags & BCM_MPLS_EGRESS_LABEL_TTL_SET &&
             ttl != mpls_port->egress_label.ttl) {
             return BCM_E_NOT_FOUND;
         }
         cw = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, CW_INSERT_FLAGf);
         if (mpls_port->flags & BCM_MPLS_PORT_SEQUENCED && cw != 0x2) {
             return BCM_E_NOT_FOUND;
         } else if (mpls_port->flags & BCM_MPLS_PORT_CONTROL_WORD && cw != 0x1) {
             return BCM_E_NOT_FOUND;
         } else if ((cw != 0) &&
             (!(mpls_port->flags & BCM_MPLS_PORT_CONTROL_WORD) &&
             !(mpls_port->flags & BCM_MPLS_PORT_SEQUENCED))) {
             return BCM_E_NOT_FOUND;
         }
         exp = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_EXPf);
         pri = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, NEW_PRIf);
         cfi = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, NEW_CFIf);
         if ((mpls_port->egress_label.flags & BCM_MPLS_EGRESS_LABEL_EXP_SET) ||
             (mpls_port->egress_label.flags & BCM_MPLS_EGRESS_LABEL_PRI_SET)) {
             if (exp != mpls_port->egress_label.exp) {
                 return BCM_E_NOT_FOUND;
             }
             if (pri != mpls_port->egress_label.pkt_pri) {
                 return BCM_E_NOT_FOUND;
             }
             if (cfi != mpls_port->egress_label.pkt_cfi) {
                 return BCM_E_NOT_FOUND;
             }
         } else if (exp || pri || cfi) {
             return BCM_E_NOT_FOUND;
         }
         sd_tag = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, SD_TAG_VIDf);
         sd_tag_action_present = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, SD_TAG_ACTION_IF_PRESENTf);
         sd_tag_action_not_present = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, SD_TAG_ACTION_IF_NOT_PRESENTf);
         if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_ADD) {
             if (sd_tag != mpls_port->egress_service_vlan ||
                 sd_tag_action_not_present != 0x1) {
                 return BCM_E_NOT_FOUND;
             }
         } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_TPID_REPLACE) {
             if (sd_tag != mpls_port->egress_service_vlan ||
                 sd_tag_action_present != 0x1) {
                 return BCM_E_NOT_FOUND;
             }
         } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_REPLACE) {
              if (sd_tag != mpls_port->egress_service_vlan ||
                 sd_tag_action_present != 0x2) {
                 return BCM_E_NOT_FOUND;
              }
         } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_DELETE) {
             if (sd_tag_action_present != 0x3) {
                 return BCM_E_NOT_FOUND;
             }
         } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_PRI_TPID_REPLACE) {
             if (sd_tag_action_present != 0x4) {
                 return BCM_E_NOT_FOUND;
             }
         } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_PRI_REPLACE) {
             if (sd_tag_action_present != 0x5) {
                 return BCM_E_NOT_FOUND;
             }
         } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_PRI_REPLACE) {
             if (sd_tag_action_present != 0x6) {
                 return BCM_E_NOT_FOUND;
             }
         } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_TPID_REPLACE) {
             if (sd_tag_action_present != 0x7) {
                 return BCM_E_NOT_FOUND;
             }
         } else if (sd_tag_action_present) {
             return BCM_E_NOT_FOUND;
         }
    } else if (info != NULL) {
        if (!(_BCM_MPLS_EGRESS_LABEL_PRESERVE(unit, info->egress_label.flags))) {
            entry_label = SOC_MEM_VC_SWAP_F32_GET(unit,
                              EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                              vc_entry, MPLS_LABELf);
            if (entry_label != info->egress_label.label) {
                return BCM_E_NOT_FOUND;
            }
        }
        entry_label_action = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_LABEL_ACTIONf);
        if (label_action != entry_label_action) {
            return BCM_E_NOT_FOUND;
        }
        ttl = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_TTLf);
        if ( ttl != info->egress_label.ttl) {
            return BCM_E_NOT_FOUND;
        }
        exp = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_EXPf);

        if (exp != info->egress_label.exp) {
            return BCM_E_NOT_FOUND;
        }

        pri = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, NEW_PRIf);

        if (pri != info->egress_label.pkt_pri) {
            return BCM_E_NOT_FOUND;
        }

        cfi = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, NEW_CFIf);

        if (cfi != info->egress_label.pkt_cfi) {
            return BCM_E_NOT_FOUND;
        }

     } else if (egr_obj != NULL) {
        if (!(_BCM_MPLS_EGRESS_LABEL_PRESERVE(unit, egr_obj->mpls_flags))) {
            entry_label = SOC_MEM_VC_SWAP_F32_GET(unit,
                               EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                               vc_entry, MPLS_LABELf);
             if (entry_label != egr_obj->mpls_label) {
                 return BCM_E_NOT_FOUND;
             }
         }
         entry_label_action = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_LABEL_ACTIONf);
         if (label_action != entry_label_action) {
             return BCM_E_NOT_FOUND;
         }
         ttl = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_TTLf);
         if ( ttl != egr_obj->mpls_ttl) {
             return BCM_E_NOT_FOUND;
         }

         if ((egr_obj->mpls_flags & BCM_MPLS_EGRESS_LABEL_EXP_SET) ||
            (egr_obj->mpls_flags & BCM_MPLS_EGRESS_LABEL_PRI_SET)) {
              exp = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_EXPf);
              pri = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, NEW_PRIf);
              cfi = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, NEW_CFIf);
              exp_select = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_EXP_SELECTf);
              if (exp_select != 0) {
                  return BCM_E_NOT_FOUND;
              }
              if (exp != egr_obj->mpls_exp) {
                  return BCM_E_NOT_FOUND;
              }
              if (pri != egr_obj->mpls_pkt_pri) {
                  return BCM_E_NOT_FOUND;
              }
              if (cfi != egr_obj->mpls_pkt_cfi) {
                  return BCM_E_NOT_FOUND;
              }
         } else if (egr_obj->mpls_flags & BCM_MPLS_EGRESS_LABEL_EXP_REMARK) {
              exp_select = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_EXP_SELECTf);
              pri_mapping_ptr = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_EXP_MAPPING_PTRf);
              if (exp_select != 1) {
                  return BCM_E_NOT_FOUND;
              }
              BCM_IF_ERROR_RETURN(
                  _egr_qos_id2hw_idx(unit,egr_obj->mpls_qos_map_id,&map_inx));
              if ( pri_mapping_ptr != map_inx) {
                  return BCM_E_NOT_FOUND;
              }
         } else if ((egr_obj->mpls_flags & BCM_MPLS_EGRESS_LABEL_EXP_COPY) &&
                    (label_action != _BCM_MPLS_ACTION_SWAP)) {
              exp_select = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_EXP_SELECTf);
              if (exp_select != 2) {
                  return BCM_E_NOT_FOUND;
              }
              exp = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_EXPf);
              if (exp != egr_obj->mpls_exp) {
                  return BCM_E_NOT_FOUND;
              }
              pri_mapping_ptr = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_EXP_MAPPING_PTRf);

              if (egr_obj->mpls_qos_map_id == 0) { /* use default */
                 map_inx = 0;
              } else {
                  BCM_IF_ERROR_RETURN(
                  _egr_qos_id2hw_idx(unit,egr_obj->mpls_qos_map_id,&map_inx));
              }
              if (pri_mapping_ptr != map_inx) {
                  return BCM_E_NOT_FOUND;              
              }
         } else if ((egr_obj->mpls_flags & BCM_MPLS_EGRESS_LABEL_EXP_COPY) &&
                    (label_action == _BCM_MPLS_ACTION_SWAP)) {
              exp_select = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_EXP_SELECTf);
              if (exp_select != 3) {
                  return BCM_E_NOT_FOUND;
              }
              exp = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_EXPf);
              if (exp != egr_obj->mpls_exp) {
                  return BCM_E_NOT_FOUND;
              }
              pri_mapping_ptr = SOC_MEM_VC_SWAP_F32_GET(unit,
                        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                        vc_entry, MPLS_EXP_MAPPING_PTRf);

              if (egr_obj->mpls_qos_map_id == 0) { /* use default */
                 map_inx = 0;
              } else {
                  BCM_IF_ERROR_RETURN(
                  _egr_qos_id2hw_idx(unit,egr_obj->mpls_qos_map_id,&map_inx));
              }
              if (pri_mapping_ptr != map_inx) {
                  return BCM_E_NOT_FOUND;              
              }
         }
    }

    return BCM_E_NONE;
}


/*
 * Function:
 *     bcmi_tr_mpls_vc_swap_hash_index_get
 * Purpose:
 *     Get hash entry for an entry
 * Parameters:
 *     IN  :  Unit
 *     IN  :  mpls port entry
 *     IN  :  action
 *     IN  :  next hop entry
 *     IN  :  VC table entry
 *     OUT :  index

 * Returns:
 *     BCM_E_XXX
 */
int 
bcmi_tr_mpls_vc_swap_hash_index_get(int unit, bcm_mpls_port_t *mpls_port, 
                       int action, bcm_mpls_tunnel_switch_t *info, 
                       bcm_l3_egress_t *egr_obj, 
                       egr_mpls_vc_and_swap_label_table_entry_t *vc_entry, 
                       int *idx)
{
    int key[BCM_TR_MPLS_VC_SWAP_KEY_LEN_IN_WORDS];

    if ((mpls_port == NULL) && (egr_obj == NULL) && (vc_entry == NULL)
        && (info == NULL)) {
        return BCM_E_PARAM;
    }

    sal_memset(key, 0, sizeof(int) * BCM_TR_MPLS_VC_SWAP_KEY_LEN_IN_WORDS);
    /* Generate key for hash table lookup */
    BCM_IF_ERROR_RETURN(bcmi_tr_mpls_vc_swap_hash_key_get
        (unit, mpls_port, info, egr_obj, action, vc_entry, key));
    LOG_INFO(BSL_LS_SOC_MPLS, (BSL_META_U(unit,
        "Printing Key : Label = %d , label_action = %d\n "), key[0], key[1]));

    *idx = _shr_crc16b(0, (uint8*)key, 
        sizeof(int)* BYTES2BITS(BCM_TR_MPLS_VC_SWAP_KEY_LEN_IN_WORDS)) % 
        BCM_TR_MPLS_VC_SWAP_HASH_BUCKETS;

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcmi_tr_mpls_vc_swap_hash_delete 
 * Purpose:
 *     Delete hash table entry
 * Parameters:
 *     IN  :  Unit
 *     IN  :  hash table head
 *     IN  :  table index
 * Returns:
 *     BCM_E_XXX
 */
int 
bcmi_tr_mpls_vc_swap_hash_delete(int unit, bcm_tr_mpls_vc_swap_table_hash_t *hash,
                                 int vc_swap_idx)
{

    bcm_tr_mpls_vc_swap_table_hash_entry_t *temp, *prev;

    if (hash == NULL) {
        return BCM_E_INTERNAL;
    }
    prev = temp = hash->head_link;

    while (temp != NULL) {
        if (temp->index == vc_swap_idx) {
            if (temp == hash->head_link) {
                hash->head_link = temp->link;
            } else {
                prev->link = temp->link;
            } 
            sal_free(temp);
            return BCM_E_NONE;
        } else {
            prev = temp;
            temp = temp->link;
        }
    } /* while */

    return BCM_E_NOT_FOUND;
}

/*
 * Function:
 *     bcmi_tr_mpls_vc_swap_hash_cleanup 
 * Purpose:
 *     Clean/Free hash table
 * Parameters:
 *     IN :  Unit
 * Returns:
 *     void
 */
STATIC void
bcmi_tr_mpls_vc_swap_hash_cleanup(int unit)
{

    bcm_tr_mpls_vc_swap_table_hash_entry_t *temp;
    bcm_tr_mpls_vc_swap_table_hash_t *hash;
    int i = 0;
#if defined(BCM_TRIUMPH3_SUPPORT)
    bcm_tr_mpls_vc_swap_table_nhop_list_t *temp_nhop;
#endif

    for (; i < BCM_TR_MPLS_VC_SWAP_HASH_BUCKETS; i++) {
        hash =  &(bcmi_vc_swap_label_hash[unit][i]);
        temp = hash->head_link;

        while (temp != NULL) {
            hash->head_link = temp->link;
#if defined(BCM_TRIUMPH3_SUPPORT)
            if (soc_feature(unit,
                            soc_feature_vc_and_swap_table_overlaid)) {
                temp_nhop = temp->nhop_list;
                while (temp_nhop) {
                    temp->nhop_list = temp_nhop->next;
                    sal_free(temp_nhop);
                    temp_nhop = temp->nhop_list;
                }
            }
#endif
            sal_free(temp);
            temp = hash->head_link;
        }
    }
    if (bcmi_vc_swap_label_hash[unit]) {
        sal_free(bcmi_vc_swap_label_hash[unit]);
        bcmi_vc_swap_label_hash[unit] = NULL;
    }
}


/*
 * Function:
 *     bcmi_tr_mpls_vc_swap_hash_entry_alloc 
 * Purpose:
 *     Allocates a hash table entry
 * Parameters:
 *     IN  :  Unit
 *     OUT :  entry pointer.
 * Returns:
 *     BCM_E_XXX
 */

STATIC int 
bcmi_tr_mpls_vc_swap_hash_entry_alloc(int unit, 
                bcm_tr_mpls_vc_swap_table_hash_entry_t **entry)
{

    bcm_tr_mpls_vc_swap_table_hash_entry_t *new_entry;

    new_entry = sal_alloc( sizeof(bcm_tr_mpls_vc_swap_table_hash_entry_t),
                    "hash_table_entry");

    if (new_entry == NULL) {
        return BCM_E_MEMORY;
    }

    /* Set the link to NULL. adding entry at tail. */
    new_entry->link = NULL;
    new_entry->nhop_list = NULL;
    *entry = new_entry;

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcmi_tr_mpls_vc_swap_hash_insert 
 * Purpose:
 *     Insert vc entry into hash table
 * Parameters:
 *     IN  :  Unit
 *     IN  :  hash table head
 *     IN  :  table index
 *     IN  :  hash index
 *     IN  :  mpls port entry
 *     IN  :  next hop entry
 *     IN  :  VC table entry
 *     OUT :  vc_swap_index
 *     IN  :  Compare Option
 * Returns:
 *      BCM_E_XXX
 */
int 
bcmi_tr_mpls_vc_swap_hash_insert(int unit, 
            bcm_tr_mpls_vc_swap_table_hash_t *hash,
            int table_idx, bcm_mpls_port_t *mpls_port, 
            bcm_mpls_tunnel_switch_t *info, bcm_l3_egress_t *egr_obj, 
            int action, int *vc_swap_index, int cmp)
{

    bcm_tr_mpls_vc_swap_table_hash_entry_t *new_entry = NULL;
    bcm_tr_mpls_vc_swap_table_hash_entry_t *temp, *prev;
    egr_mpls_vc_and_swap_label_table_entry_t vc_entry;
    
    if (hash == NULL) {
        return BCM_E_INTERNAL;
    }
    prev = temp = hash->head_link;

    while (temp != NULL) {
        if (cmp) {
            BCM_IF_ERROR_RETURN(soc_mem_read(unit, 
                EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                MEM_BLOCK_ALL, temp->index, &vc_entry));
             
            if (BCM_SUCCESS(_bcm_tr_mpls_vc_swap_compare
                (unit, mpls_port, info, egr_obj, action, &vc_entry))) {
                /* 
                 * Entry already exists. 
                 * return BCM_E_EXISTS to increase refcount. 
                 */
               *vc_swap_index = temp->index;
               return BCM_E_EXISTS;
            }
        }
        prev = temp;
        temp = temp->link;
    }
    /* Entry does not exist, add a new one. */
    BCM_IF_ERROR_RETURN(bcmi_tr_mpls_vc_swap_hash_entry_alloc
        (unit, &new_entry));

    new_entry->index = table_idx;
    
    /* update the link table */
    if (hash->head_link == NULL) {
        hash->head_link = new_entry;
    } else {
        prev->link = new_entry;
    } 

    *vc_swap_index = table_idx;
    return BCM_E_NONE;
}
#if defined(BCM_TRIUMPH3_SUPPORT)
/*
 * Function:
 *       _bcm_tr_mpls_vc_swap_hash_index_update
 * Purpose:
 *       Update the vc and swap table index in hash entry and
 *       EGR_L3_NEXT_HOP table.
 * Parameters:
 *       unit                         - (IN) Device Number
 *       hash                        -(IN) Hash input
 *       new_vc_swap_index   - (IN) New vc and swap table index
 *       old_vc_swap_index    - (IN) Old vc and swap table index
 * Returns:
 *       BCM_E_XXX
 */

STATIC int
_bcm_tr_mpls_vc_swap_hash_index_update (int unit,
                     bcm_tr_mpls_vc_swap_table_hash_t *hash,
                     int new_vc_swap_index,
                     int old_vc_swap_index)
{
    bcm_tr_mpls_vc_swap_table_hash_entry_t *temp_hash;
    bcm_tr_mpls_vc_swap_table_nhop_list_t *temp_nhop;
    soc_field_t vc_swap_idxf;

    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
        vc_swap_idxf = MPLS__VC_AND_SWAP_INDEXf ;
    } else {
        vc_swap_idxf = VC_AND_SWAP_INDEXf;
    }

    if (hash == NULL) {
        return BCM_E_INTERNAL;
    }
    temp_hash = hash->head_link;
    while (temp_hash) {
        if (temp_hash->index == old_vc_swap_index) {
            temp_hash->index = new_vc_swap_index;
            temp_nhop = temp_hash->nhop_list;
            while (temp_nhop) {
                BCM_IF_ERROR_RETURN
                    (soc_mem_field32_modify(unit, EGR_L3_NEXT_HOPm,
                                            temp_nhop->nh_index,
                                            vc_swap_idxf,
                                            new_vc_swap_index));
                temp_nhop = temp_nhop->next;
            }
            return BCM_E_NONE;
        }
        temp_hash = temp_hash->link;
    }
    return BCM_E_NOT_FOUND;
}
#endif  /* BCM_TRIUMPH3_SUPPORT */
/*
 * Function:
 *      _bcm_tr_mpls_get_vc_and_swap_table_index
 * Purpose:
 *     Obtain Index into  VC_AND_SWAP_TABLE
 * Parameters:
 *     IN  :  Unit
 *     IN  :  num_vc
 *     IN  :  mpls port entry
 *     IN  :  next hop entry
 *     IN  :  action
 *     OUT :  vc_swap_index
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_get_vc_and_swap_table_index (int unit, int flags,
                        bcm_mpls_port_t *mpls_port, 
                        bcm_mpls_tunnel_switch_t *info, 
                        bcm_l3_egress_t *egr_obj, 
                        int action, int *vc_swap_index)
{
    int hash_idx, num_vc;
    int imin; 
    int rv;
    int action_flags = _BCM_MPLS_ACTION_GET(flags);
    int free_idx = 0, offset = 0;
    bcm_tr_mpls_vc_swap_table_hash_t *hash_input;

    if (bcmi_vc_swap_label_hash[unit] == NULL) {
        return BCM_E_INTERNAL;
    }

    imin = soc_mem_index_min(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);
    num_vc = soc_mem_index_count(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);

    /* we are dividng the table in half to manage counted and 
     * non-counted entries. 
     */
    num_vc /= 2;

    BCM_IF_ERROR_RETURN(bcmi_tr_mpls_vc_swap_hash_index_get(
        unit, mpls_port, action, info, egr_obj, NULL, &hash_idx));

    /* Try getting index from 2nd half of MPLS_VC_AND_SWAP table */
    if (!(action_flags & _BCM_MPLS_ACTION_SEQUENCE)) {
        for (free_idx = imin; free_idx < num_vc + imin; free_idx++) {
            if (!(_BCM_MPLS_VC_NON_COUNT_USED_GET(unit, free_idx) ||
                  (soc_feature(unit, soc_feature_vc_and_swap_table_overlaid) &&
                  _bcm_vp_used_get(unit, free_idx, _bcmVpTypeL2Gre)))) {
                break;
            }
        }
    }

    if ((free_idx == (num_vc + imin)) || 
        (action_flags &  _BCM_MPLS_ACTION_SEQUENCE)) {

        /* Get index from 1st half of MPLS_VC_AND_SWAP table */
        for (free_idx = imin; free_idx < num_vc + imin; free_idx++) {
            if (!_BCM_MPLS_VC_COUNT_USED_GET(unit, free_idx - imin)) {
                break;
            }
        }
        if (free_idx == num_vc + imin) {
            return  BCM_E_RESOURCE;
        }
        _BCM_MPLS_VC_COUNT_USED_SET(unit, free_idx - imin);
        offset = imin;
    } else {
        _BCM_MPLS_VC_NON_COUNT_USED_SET(unit, free_idx - imin);
        offset = num_vc + imin;
    }

    hash_input = &(bcmi_vc_swap_label_hash[unit][hash_idx]);

    if (flags & _BCM_MPLS_ACTION_LABEL_ALLOC) {
        /* insert the entry without comparison */
        rv = bcmi_tr_mpls_vc_swap_hash_insert(unit, hash_input, 
            free_idx + offset, mpls_port, info, 
            egr_obj, action, vc_swap_index, FALSE);

    } else {
        /* insert the entry with comparison */
        rv = bcmi_tr_mpls_vc_swap_hash_insert(unit, hash_input, 
            free_idx + offset, mpls_port, info, 
            egr_obj, action, vc_swap_index, TRUE);
    }

    if (BCM_FAILURE(rv)) { 

        if (offset == imin) {
            _BCM_MPLS_VC_COUNT_USED_CLR(unit, free_idx - imin);
        } else {
            _BCM_MPLS_VC_NON_COUNT_USED_CLR(unit, free_idx - imin);
        }

        if (rv != BCM_E_EXISTS) {
            LOG_ERROR(BSL_LS_SOC_COMMON, (BSL_META_U(unit,
                "Hash Entry insertion failed  = %d\n"), rv));
            return rv;
        }
    }
    /* Increase refcount on table idx */ 
    _bcm_tr_mpls_vc_and_swap_ref_count_adjust(unit, *vc_swap_index , 1);

    return BCM_E_NONE;
}

/*
 * Function:
 *       _bcm_tr_mpls_vc_and_swap_table_reset
 * Purpose:
 *       Reset  VC_AND_SWAP_TABLE entry
 * Parameters:
 *           IN :  Unit
 *           IN : vc_swap_index
 * Returns:
 *            BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_vc_and_swap_table_index_reset (int unit, int vc_swap_index)
{
    egr_mpls_vc_and_swap_label_table_entry_t  vc_entry;
    int rv = BCM_E_NONE;
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int num_vc, idx;
    bcm_tr_mpls_vc_swap_table_hash_t *hash_input;
    
    /* Entry 0 is reserved with _bcm_tr_mpls_default_entry_setup during init */
    if (vc_swap_index == VC_AND_SWAP_TABLE_RESV_INDEX) {
        LOG_INFO(BSL_LS_BCM_L3,
                 (BSL_META_U(unit,"Not resetting reserved index: %d in "
                         "VC AND SWAP TABLE\n"), VC_AND_SWAP_TABLE_RESV_INDEX));
        return BCM_E_NONE;
    }
    if ( mpls_info->vc_swap_ref_count[vc_swap_index] != 0 ) {
         return BCM_E_NONE;
    }
    if (bcmi_vc_swap_label_hash[unit] == NULL) {
        return BCM_E_INTERNAL;
    }
    num_vc = soc_mem_index_count(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);
    num_vc /= 2;

    BCM_IF_ERROR_RETURN(soc_mem_read(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
        MEM_BLOCK_ALL, vc_swap_index, &vc_entry));

    BCM_IF_ERROR_RETURN(bcmi_tr_mpls_vc_swap_hash_index_get(
        unit, NULL, 0, NULL, NULL, &vc_entry, &idx));

    /* remove the entry from sw hash table */
    hash_input = &(bcmi_vc_swap_label_hash[unit][idx]);
    BCM_IF_ERROR_RETURN(bcmi_tr_mpls_vc_swap_hash_delete(
        unit, hash_input, vc_swap_index));

    if (vc_swap_index >= num_vc) {
        _BCM_MPLS_VC_NON_COUNT_USED_CLR(unit, vc_swap_index - num_vc);
    } else {
        _BCM_MPLS_VC_COUNT_USED_CLR(unit, vc_swap_index);
    }

    sal_memset(&vc_entry, 0,
              sizeof(egr_mpls_vc_and_swap_label_table_entry_t));

    rv = soc_mem_write(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
              MEM_BLOCK_ALL, vc_swap_index,
              &vc_entry);

    return rv;
}

/*
 * Function:
 *       _bcm_tr_mpls_vc_and_swap_table_entry_set
 * Purpose:
 *       Set  VC_AND_SWAP_TABLE entry
 * Parameters:
 *           IN :  Unit
 *           IN : vc_swap_index
 * Returns:
 *            BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_vc_and_swap_table_entry_set (int unit, 
                                       bcm_mpls_port_t *mpls_port,
                                       bcm_mpls_tunnel_switch_t *info,
                                       bcm_l3_egress_t *egr_obj,
                                       int label_action,
                                       egr_mpls_vc_and_swap_label_table_entry_t *vc_entry,
                                       int hw_map_idx)
{
    bcm_mpls_label_t mpls_label = BCM_MPLS_LABEL_INVALID;
    uint8 mpls_ttl = 0;
    uint32 mpls_flags = 0;
    uint8 mpls_exp = 0; 
    uint8 mpls_pkt_pri = 0; 
    uint8 mpls_pkt_cfi = 0; 
#ifdef BCM_TOMAHAWK2_SUPPORT    
    int   mpls_ecn_map_id = ECN_MAP_ID_INVALID;
    int   mpls_int_cn_map_id = ECN_MAP_ID_INVALID;
#endif
    if (mpls_port != NULL) {
         mpls_label = mpls_port->egress_label.label;
         mpls_ttl = mpls_port->egress_label.ttl;
         mpls_flags = mpls_port->egress_label.flags;
         mpls_pkt_pri = mpls_port->egress_label.pkt_pri;
         mpls_pkt_cfi = mpls_port->egress_label.pkt_cfi;
         mpls_exp = mpls_port->egress_label.exp;
#ifdef BCM_TOMAHAWK2_SUPPORT        
         if (soc_feature(unit, soc_feature_mpls_ecn)) {
            if (mpls_flags & BCM_MPLS_EGRESS_LABEL_ECN_TO_EXP_MAP) {
                mpls_ecn_map_id = mpls_port->egress_label.ecn_map_id;
            }
            if (mpls_flags & BCM_MPLS_EGRESS_LABEL_INT_CN_TO_EXP_MAP) {
                mpls_int_cn_map_id = mpls_port->egress_label.int_cn_map_id;
            }
         }
#endif         
    } else if (info != NULL) {
         mpls_label = info->egress_label.label;
         mpls_ttl = info->egress_label.ttl;
         mpls_flags = info->egress_label.flags;
         mpls_pkt_pri = info->egress_label.pkt_pri;
         mpls_pkt_cfi = info->egress_label.pkt_cfi;
         mpls_exp = info->egress_label.exp;
#ifdef BCM_TOMAHAWK2_SUPPORT        
         if (soc_feature(unit, soc_feature_mpls_ecn) && 
             (mpls_flags & BCM_MPLS_EGRESS_LABEL_INT_CN_TO_EXP_MAP)) {
             mpls_int_cn_map_id = info->egress_label.int_cn_map_id;
         }
#endif           
    } else if (egr_obj != NULL) {
         mpls_label = egr_obj->mpls_label;
         mpls_ttl = egr_obj->mpls_ttl;
         mpls_flags = egr_obj->mpls_flags;
         mpls_pkt_pri = egr_obj->mpls_pkt_pri;
         mpls_pkt_cfi = egr_obj->mpls_pkt_cfi;
         mpls_exp = egr_obj->mpls_exp;
#ifdef BCM_TOMAHAWK2_SUPPORT  
         if (soc_feature(unit, soc_feature_mpls_ecn)) {
            if (mpls_flags & BCM_MPLS_EGRESS_LABEL_INT_CN_TO_EXP_MAP) {
                mpls_int_cn_map_id = egr_obj->mpls_ecn_map_id;
            }
            if (mpls_flags & BCM_MPLS_EGRESS_LABEL_ECN_TO_EXP_MAP) {
                mpls_ecn_map_id = egr_obj->mpls_ecn_map_id;
            }
         }
#endif 
    }

        if ((mpls_exp > 7) || (mpls_pkt_pri > 7) ||
            (mpls_pkt_cfi > 1)) {
            return BCM_E_PARAM;
        }

        sal_memset(vc_entry, 0, 
                   sizeof(egr_mpls_vc_and_swap_label_table_entry_t));
        if (_BCM_MPLS_EGRESS_LABEL_PRESERVE(unit, mpls_flags)) {
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, MPLS_LABEL_ACTIONf, 
                                _BCM_MPLS_ACTION_PRESERVED);
        } else {
            if (BCM_XGS3_L3_MPLS_LBL_VALID(mpls_label)) {
                soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    vc_entry, MPLS_LABELf,
                                    mpls_label);
                soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    vc_entry, MPLS_LABEL_ACTIONf,
                                    label_action);
            } else {
                soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    vc_entry, MPLS_LABEL_ACTIONf,
                                    _BCM_MPLS_ACTION_NOOP);
            }
        }
        if (mpls_flags & BCM_MPLS_EGRESS_LABEL_TTL_SET) {
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                vc_entry, MPLS_TTLf,
                                mpls_ttl);
        } else {
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                vc_entry, MPLS_TTLf, 0x0);
        }
#if (defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT))
        if (soc_feature(unit, soc_feature_mpls_entropy) && (mpls_port != NULL)) {
            if ((mpls_port->flags & BCM_MPLS_PORT_ENTROPY_ENABLE) ||
                (mpls_flags & BCM_MPLS_EGRESS_LABEL_ENTROPY_ENABLE)) {
                if (soc_mem_field_valid(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    MPLS_INSERT_ENTROPY_LABELf)) {
                     soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                            vc_entry, MPLS_INSERT_ENTROPY_LABELf, 0x1);
                }
            }
        }
#endif /* defined(BCM_KATANA_SUPPORT) || defined(BCM_APACHE_SUPPORT) */
        if (mpls_port != NULL) {
            if (mpls_port->flags & BCM_MPLS_PORT_SEQUENCED) {
                /* Append control word with sequence number */
                soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                vc_entry, CW_INSERT_FLAGf, 0x2);
            } else if (mpls_port->flags & BCM_MPLS_PORT_CONTROL_WORD) {
                /* Append control word with NO sequence number */
                soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                vc_entry, CW_INSERT_FLAGf, 0x1);
            }
            if ((mpls_port->flags & BCM_MPLS_PORT_COUNTED) ||
                (mpls_port->flags & BCM_MPLS_PORT_SEQUENCED)) {
                 soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, UPDATE_PW_INIT_COUNTERSf, 0x1);
            }
        }
        if ((mpls_flags & BCM_MPLS_EGRESS_LABEL_EXP_SET) ||
            (mpls_flags & BCM_MPLS_EGRESS_LABEL_PRI_SET)) {

            if ((mpls_flags & BCM_MPLS_EGRESS_LABEL_EXP_REMARK) ||
                (mpls_flags & BCM_MPLS_EGRESS_LABEL_EXP_COPY) ||
                (mpls_flags & BCM_MPLS_EGRESS_LABEL_PRI_REMARK)) {
                return  BCM_E_PARAM;
            }
            /* Use the specified EXP, PRI and CFI */
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, MPLS_EXP_SELECTf, 
                                0x0); /* USE_FIXED */
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, MPLS_EXPf, 
                                mpls_exp);
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, NEW_PRIf, 
                                mpls_pkt_pri);
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, NEW_CFIf, 
                                mpls_pkt_cfi);
        } else if (mpls_flags & BCM_MPLS_EGRESS_LABEL_EXP_REMARK) {
            if (mpls_flags & BCM_MPLS_EGRESS_LABEL_PRI_SET) {
                return BCM_E_PARAM;
            }
            /* Use EXP-map for EXP, PRI and CFI */
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, MPLS_EXP_SELECTf, 
                                0x1); /* USE_MAPPING */
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, MPLS_EXP_MAPPING_PTRf,
                                hw_map_idx);
        } else if ((label_action != _BCM_MPLS_ACTION_SWAP) && 
                        (mpls_flags & BCM_MPLS_EGRESS_LABEL_EXP_COPY) ) { 
            /* Use EXP from incoming label. If there is no incoming label,
             * use the specified EXP value. Use EXP-map for PRI/CFI.
             */
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, MPLS_EXP_SELECTf,
                                0x2); /* USE_INNER */
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                vc_entry, MPLS_EXPf, 
                                mpls_exp); 

            /* Use EXP-PRI-map for PRI/CFI */
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, MPLS_EXP_MAPPING_PTRf,
                                hw_map_idx);
        } else if ((label_action == _BCM_MPLS_ACTION_SWAP) && 
                        (mpls_flags & BCM_MPLS_EGRESS_LABEL_EXP_COPY) ) {
              soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, MPLS_EXP_SELECTf,
                                0x3); /* USE_SWAP */
              soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                vc_entry, MPLS_EXPf, mpls_exp); 
              /* Use EXP-PRI-map for PRI/CFI */
              soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, MPLS_EXP_MAPPING_PTRf,
                                hw_map_idx);
        }
#ifdef BCM_TOMAHAWK2_SUPPORT
        if (soc_feature(unit, soc_feature_mpls_ecn)) {
            if ((mpls_flags & BCM_MPLS_EGRESS_LABEL_INT_CN_TO_EXP_MAP) && 
                (mpls_int_cn_map_id != ECN_MAP_ID_INVALID)) {
                int ecn_map_index;
                int ecn_map_type;
                int ecn_map_num;
                int ecn_map_hw_idx;
                ecn_map_type = mpls_int_cn_map_id & 
                                _BCM_XGS5_MPLS_ECN_MAP_TYPE_MASK;
                ecn_map_index = mpls_int_cn_map_id & 
                                _BCM_XGS5_MPLS_ECN_MAP_NUM_MASK;
                ecn_map_num = 
                    soc_mem_index_count(unit, EGR_INT_CN_TO_EXP_MAPPING_TABLEm) / 
                    _BCM_ECN_MAX_ENTRIES_PER_MAP;
                if (ecn_map_type != _BCM_XGS5_MPLS_ECN_MAP_TYPE_INTCN2EXP) {
                    return BCM_E_PARAM;
                }
                if (ecn_map_index >= ecn_map_num) {
                    return BCM_E_PARAM;
                }
                BCM_IF_ERROR_RETURN(
                    bcmi_ecn_map_id2hw_idx(unit, mpls_int_cn_map_id, &ecn_map_hw_idx));
                soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    vc_entry, INT_CN_TO_EXP_MAPPING_PTRf,
                                    ecn_map_hw_idx);
                if (mpls_flags & BCM_MPLS_EGRESS_LABEL_ECN_EXP_MAP_TRUST) {
                    soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                        vc_entry, INT_CN_TO_EXP_PRIORITYf, 1);

                } else {
                    soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                        vc_entry, INT_CN_TO_EXP_PRIORITYf, 0);

                }
            }
            if ((mpls_flags & BCM_MPLS_EGRESS_LABEL_ECN_TO_EXP_MAP) && 
                (mpls_ecn_map_id != ECN_MAP_ID_INVALID)) {
                int ecn_map_index;
                int ecn_map_type;
                int ecn_map_num;
                int ecn_map_hw_idx;
                ecn_map_type = mpls_ecn_map_id & 
                                _BCM_XGS5_MPLS_ECN_MAP_TYPE_MASK;
                ecn_map_index = mpls_ecn_map_id & 
                                _BCM_XGS5_MPLS_ECN_MAP_NUM_MASK;
                ecn_map_num = 
                    soc_mem_index_count(unit, EGR_IP_ECN_TO_EXP_MAPPING_TABLEm) / 
                    _BCM_ECN_MAX_ENTRIES_PER_MAP;
                if (ecn_map_type != _BCM_XGS5_MPLS_ECN_MAP_TYPE_ECN2EXP) {
                    return BCM_E_PARAM;
                }
                if (ecn_map_index >= ecn_map_num) {
                    return BCM_E_PARAM;
                }
                BCM_IF_ERROR_RETURN(
                    bcmi_ecn_map_id2hw_idx(unit, mpls_ecn_map_id, &ecn_map_hw_idx));
                
                soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    vc_entry, IP_ECN_TO_EXP_MAPPING_PTRf,
                                    ecn_map_hw_idx);
                if (mpls_flags & BCM_MPLS_EGRESS_LABEL_ECN_EXP_MAP_TRUST) {
                    soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                        vc_entry, IP_ECN_TO_EXP_PRIORITYf, 1);

                } else {
                    soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                        vc_entry, IP_ECN_TO_EXP_PRIORITYf, 0);

                }

            }
        }
#endif
        return BCM_E_NONE;
}
#if defined(BCM_TRIUMPH3_SUPPORT)
/*
 * Function:
 *       _bcm_tr_mpls_vc_and_swap_table_entry_adjust
 * Purpose:
 *       Move the vc and swap table entry to another index.
 *       The original index should be occupied by
 *       EGR_DVP_ATTRIBUTE_1
 * Parameters:
 *       unit     - (IN) Device Number
 *       VP      -(IN) virtual port number
 * Returns:
 *       BCM_E_XXX
 */

int
_bcm_tr_mpls_vc_and_swap_table_entry_adjust (int unit, int vp)
{
    int hash_idx, num_vc;
    int imin = 0;
    int rv, ref_count;
    int free_idx = 0, offset = 0;
    int old_vc_swap_index, vc_swap_index;
    bcm_tr_mpls_vc_swap_table_hash_t *hash_input;
    egr_mpls_vc_and_swap_label_table_entry_t vc_entry;

    if (bcmi_vc_swap_label_hash[unit] == NULL) {
        return BCM_E_INTERNAL;
    }
    rv = bcm_tr_mpls_lock(unit);
    _BCM_MPLS_CLEANUP(rv);

    /* The entry is not occupied. */
    if (!_BCM_MPLS_VC_NON_COUNT_USED_GET(unit, vp)) {
        bcm_tr_mpls_unlock(unit);
        return BCM_E_NONE;
    }

    imin = soc_mem_index_min(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);
    num_vc = soc_mem_index_count(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);

    /* we are dividng the table in half to manage counted and
     * non-counted entries.
     */
    num_vc /= 2;

    old_vc_swap_index = vp + num_vc;
    rv = soc_mem_read(unit,
                      EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                      MEM_BLOCK_ALL, old_vc_swap_index, &vc_entry);
    _BCM_MPLS_CLEANUP(rv);
    /* Try getting index from 2nd half of MPLS_VC_AND_SWAP table */
    for (free_idx = imin; free_idx < num_vc + imin; free_idx++) {
        if (!(_BCM_MPLS_VC_NON_COUNT_USED_GET(unit, free_idx) ||
              _bcm_vp_used_get(unit, free_idx, _bcmVpTypeL2Gre))) {
            break;
        }
    }
    if (free_idx == (num_vc + imin)) {
        /* Get index from 1st half of MPLS_VC_AND_SWAP table */
        for (free_idx = imin; free_idx < num_vc + imin; free_idx++) {
            if (!_BCM_MPLS_VC_COUNT_USED_GET(unit, free_idx - imin)) {
                break;
            }
        }
        if (free_idx == num_vc + imin) {
            bcm_tr_mpls_unlock(unit);
            return  BCM_E_RESOURCE;
        }
        _BCM_MPLS_VC_COUNT_USED_SET(unit, free_idx - imin);
        offset = imin;
    } else {
        _BCM_MPLS_VC_NON_COUNT_USED_SET(unit, free_idx - imin);
        offset = num_vc + imin;
    }

    vc_swap_index = free_idx + offset;
    rv = soc_mem_write(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                       MEM_BLOCK_ALL, vc_swap_index,
                       &vc_entry);
    _BCM_MPLS_CLEANUP(rv);
    /* Update nhop entry and EGR_L3_NEXT_HOP table */
    rv = bcmi_tr_mpls_vc_swap_hash_index_get(unit, NULL, 0, NULL, NULL,
                                             &vc_entry, &hash_idx);
    _BCM_MPLS_CLEANUP(rv);
    hash_input = &(bcmi_vc_swap_label_hash[unit][hash_idx]);
    rv = _bcm_tr_mpls_vc_swap_hash_index_update(unit,
                                                hash_input, vc_swap_index,
                                                old_vc_swap_index);
    _BCM_MPLS_CLEANUP(rv);
    /* Clear the original entry */
    rv = soc_mem_write(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                       MEM_BLOCK_ALL, old_vc_swap_index,
                       soc_mem_entry_null(unit,
                                          EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm));
    _BCM_MPLS_CLEANUP(rv);
    /* Adjust the ref count */
    _bcm_tr_mpls_vc_and_swap_ref_count_get(unit, old_vc_swap_index, &ref_count);
    _bcm_tr_mpls_vc_and_swap_ref_count_reset(unit, old_vc_swap_index);
    _bcm_tr_mpls_vc_and_swap_ref_count_adjust(unit, vc_swap_index, ref_count);
    bcm_tr_mpls_unlock(unit);
    return rv;
cleanup:
    if (offset == imin) {
        _BCM_MPLS_VC_COUNT_USED_CLR(unit, free_idx - imin);
    } else {
        _BCM_MPLS_VC_NON_COUNT_USED_CLR(unit, free_idx - imin);
    }
    bcm_tr_mpls_unlock(unit);
    return rv;
}
#endif
/*
 * Function:
 *      _bcm_tr_mpls_pw_init_counter_set
 * Purpose:
 *      Obtain the pw_init counter index, Set the EGR L3 NH entry parameter
 * Parameters:
 *      unit   - (IN) SOC unit #
 *      egr_nh_info  - (IN/OUT) Egress NHOP Internal Structurel
 * Returns:
 *      BCM_E_XXX
 */     

STATIC int
_bcm_tr_mpls_pw_init_counter_set (int unit, 
                   uint32 pw_seq_number, _bcm_tr_egr_nh_info_t  *egr_nh_info)
{
    int  pw_init_cnt=-1;
    egr_pw_init_counters_entry_t pw_init_entry;
    egr_pw_init_counters_x_entry_t pw_init_entry_x;
    egr_pw_init_counters_y_entry_t pw_init_entry_y;    
    int  vc_swap_index;
    uint32 num_pw_mask;

    if (SOC_MEM_IS_VALID(unit, EGR_PW_INIT_COUNTERSm)) {
        vc_swap_index =  egr_nh_info->vc_swap_index;

        /* pw_init_counter index is from the LSB bits of vc_and_swap_index */
        num_pw_mask = soc_mem_index_count(0, EGR_PW_INIT_COUNTERSm) - 1;
        pw_init_cnt = vc_swap_index & num_pw_mask;

        _BCM_MPLS_PW_INIT_USED_SET(unit, pw_init_cnt);

        egr_nh_info->pw_init_cnt = pw_init_cnt;

        sal_memset(&pw_init_entry, 0, sizeof(egr_pw_init_counters_entry_t));
        soc_EGR_PW_INIT_COUNTERSm_field32_set(unit, &pw_init_entry, SEQ_NUMf, 
                                              pw_seq_number);

        WRITE_EGR_PW_INIT_COUNTERSm(unit, MEM_BLOCK_ALL, pw_init_cnt,
                                    &pw_init_entry);
        if (SOC_MEM_IS_VALID(unit, EGR_PW_INIT_COUNTERS_Xm)) {
            sal_memset(&pw_init_entry_x, 0, sizeof(egr_pw_init_counters_x_entry_t));
            soc_EGR_PW_INIT_COUNTERS_Xm_field32_set(unit, &pw_init_entry_x, 
                                                    SEQ_NUMf, pw_seq_number);

            WRITE_EGR_PW_INIT_COUNTERS_Xm(unit, MEM_BLOCK_ALL, pw_init_cnt,
                                          &pw_init_entry_x);
        }
        if (SOC_MEM_IS_VALID(unit, EGR_PW_INIT_COUNTERS_Ym)) {
            sal_memset(&pw_init_entry_y, 0, sizeof(egr_pw_init_counters_y_entry_t));
            soc_EGR_PW_INIT_COUNTERS_Ym_field32_set(unit, &pw_init_entry_y, 
                                                    SEQ_NUMf, pw_seq_number);

            WRITE_EGR_PW_INIT_COUNTERS_Ym(unit, MEM_BLOCK_ALL, pw_init_cnt,
                                          &pw_init_entry_y);
        }

    }
    return BCM_E_NONE;

}

/*
 * Function:
 *      _bcm_tr_mpls_pw_init_counter_delete
 * Purpose:
 *      Get L2 MPLS PW Stats
 * Parameters:
 *      unit   - (IN) SOC unit #
 * Returns:
 *      BCM_E_XXX
 */     

STATIC int
_bcm_tr_mpls_pw_init_counter_delete (int unit, int nh_index)
{
    int  pw_init_cnt=-1,rv;
    egr_l3_next_hop_entry_t egr_nh;
    egr_pw_init_counters_entry_t pw_init_entry;
    egr_pw_init_counters_x_entry_t pw_init_entry_x;
    egr_pw_init_counters_y_entry_t pw_init_entry_y;

    if (SOC_MEM_IS_VALID(unit, EGR_PW_INIT_COUNTERSm)) {
         rv = soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY,
                      nh_index, &egr_nh);
         if (rv < 0) {
              return rv;
         }

         if (soc_feature(unit, soc_feature_mpls_enhanced)) {
              pw_init_cnt =
                   soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                     MPLS__PW_INIT_NUMf);
         } else {
              pw_init_cnt =
                   soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                     PW_INIT_NUMf);
         }

         sal_memset(&pw_init_entry, 0, sizeof(egr_pw_init_counters_entry_t));
         WRITE_EGR_PW_INIT_COUNTERSm(unit, MEM_BLOCK_ALL, pw_init_cnt,
                                            &pw_init_entry);
         if (SOC_MEM_IS_VALID(unit, EGR_PW_INIT_COUNTERS_Xm)) {
             sal_memset(&pw_init_entry_x, 0, sizeof(egr_pw_init_counters_x_entry_t));        
             WRITE_EGR_PW_INIT_COUNTERS_Xm(unit, MEM_BLOCK_ALL, pw_init_cnt,
                                           &pw_init_entry_x);
         }
         if (SOC_MEM_IS_VALID(unit, EGR_PW_INIT_COUNTERS_Ym)) {
             sal_memset(&pw_init_entry_y, 0, sizeof(egr_pw_init_counters_y_entry_t));
             WRITE_EGR_PW_INIT_COUNTERS_Ym(unit, MEM_BLOCK_ALL, pw_init_cnt,
                                           &pw_init_entry_y);
         }

         _BCM_MPLS_PW_INIT_USED_CLR (unit, pw_init_cnt);
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_tr_mpls_pw_term_counter_set
 * Purpose:
 *      Obtain the pw_term counter index
 * Parameters:
 *      unit   - (IN) SOC unit #
 *      ment  - (IN/OUT) MPLS Entry Structure
 * Returns:
 *      BCM_E_XXX
 */     

int
_bcm_tr_mpls_pw_term_counter_set (int unit, int  *pw_term_cnt)
{
    int num_pw_term=-1;
    int pw_idx=0;
    ing_pw_term_counters_entry_t pw_term_entry;
    ing_pw_term_seq_num_entry_t pw_term_seq_num_entry;

    if (SOC_MEM_IS_VALID(unit, ING_PW_TERM_COUNTERSm)) {
         num_pw_term = soc_mem_index_count(unit, ING_PW_TERM_COUNTERSm);
         for (pw_idx = 0; pw_idx < num_pw_term; pw_idx++) { 
              if (!_BCM_MPLS_PW_TERM_USED_GET(unit, pw_idx)) {
                    break;
              }
         }
         if (pw_idx == num_pw_term) {
              return BCM_E_RESOURCE;
         }
          _BCM_MPLS_PW_TERM_USED_SET(unit, pw_idx);

        sal_memset(&pw_term_entry, 0, sizeof(ing_pw_term_counters_entry_t));
        (void) WRITE_ING_PW_TERM_COUNTERSm(unit, MEM_BLOCK_ALL, pw_idx,
                                            &pw_term_entry);

        *pw_term_cnt  = pw_idx;
    } else if (SOC_MEM_IS_VALID(unit, ING_PW_TERM_SEQ_NUMm)) {
        num_pw_term = soc_mem_index_count(unit, ING_PW_TERM_SEQ_NUMm);
        for (pw_idx = 0; pw_idx < num_pw_term; pw_idx++) {
            if (!_BCM_MPLS_PW_TERM_USED_GET(unit, pw_idx)) {
                break;
            }
        }
        if (pw_idx == num_pw_term) {
            return BCM_E_RESOURCE;
        }
        _BCM_MPLS_PW_TERM_USED_SET(unit, pw_idx);
        
        sal_memset(&pw_term_seq_num_entry, 0, sizeof(ing_pw_term_seq_num_entry_t));
        WRITE_ING_PW_TERM_SEQ_NUMm(unit, MEM_BLOCK_ALL, pw_idx,
                                   &pw_term_seq_num_entry);

        *pw_term_cnt = pw_idx;
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_tr_mpls_pw_term_counter_delete
 * Purpose:
 *      Delete pw_term counter
 * Parameters:
 *      unit   - (IN) SOC unit #
 * Returns:
 *      BCM_E_XXX
 */     

STATIC int
_bcm_tr_mpls_pw_term_counter_delete (int unit,  int  pw_term_cnt)
{
    ing_pw_term_counters_entry_t pw_term_entry;
    ing_pw_term_seq_num_entry_t pw_term_seq_num_entry;

    if (SOC_MEM_IS_VALID(unit, ING_PW_TERM_COUNTERSm)) {
         _BCM_MPLS_PW_TERM_USED_CLR(unit, pw_term_cnt);

         sal_memset(&pw_term_entry, 0, sizeof(ing_pw_term_counters_entry_t));
         (void) WRITE_ING_PW_TERM_COUNTERSm(unit, MEM_BLOCK_ALL, pw_term_cnt,
                                            &pw_term_entry);
    } else if (SOC_MEM_IS_VALID(unit, ING_PW_TERM_SEQ_NUMm)) {
        _BCM_MPLS_PW_TERM_USED_CLR(unit, pw_term_cnt);

        sal_memset(&pw_term_seq_num_entry, 0, sizeof(ing_pw_term_seq_num_entry_t));
        WRITE_ING_PW_TERM_SEQ_NUMm(unit, MEM_BLOCK_ALL, pw_term_cnt,
                                   &pw_term_seq_num_entry);
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_tr_mpls_port_untagged_profile_set
 * Purpose:
 *      Set  VLan profile per Physical port entry
 * Parameters:
 *      unit   - (IN) SOC unit #
 *      port  - (IN) Physical port
 * Returns:
 *      BCM_E_XXX
 */     

int
bcm_tr_mpls_port_untagged_profile_set (int unit, bcm_port_t port)
{
    int rv;
    bcm_vlan_action_set_t action;
    uint32 ing_profile_idx = 0xffffffff;
    int old_ing_profile_idx = -1;

    bcm_vlan_action_set_t_init(&action);
    action.ut_outer = 0x0; /* No Op */
    action.ut_inner = 0x0; /* No Op */

    SOC_IF_ERROR_RETURN(
        _bcm_esw_port_tab_get(unit, port, TAG_ACTION_PROFILE_PTRf,
                              &old_ing_profile_idx));

    rv = _bcm_trx_vlan_action_profile_entry_add(unit, &action, &ing_profile_idx);
    if (rv < 0) {
        return rv;
    }

    rv = _bcm_esw_port_tab_set_without_portlock(unit, port, _BCM_CPU_TABS_NONE,
                                                TAG_ACTION_PROFILE_PTRf,
                                                ing_profile_idx);
    if (rv < 0) {
        return rv;
    }

    if(old_ing_profile_idx != ING_ACTION_PROFILE_DEFAULT) {
        rv = _bcm_trx_vlan_action_profile_entry_delete(unit, old_ing_profile_idx);
    }

    return rv;
}

/*
 * Function:
 *      bcm_tr_mpls_port_untagged_profile_reset
 * Purpose:
 *      Reset  VLan profile per Physical port entry
 * Parameters:
 *      unit   - (IN) SOC unit #
 *      port  - (IN) Physical port
 * Returns:
 *      BCM_E_XXX
 */     

int
bcm_tr_mpls_port_untagged_profile_reset(int unit, bcm_port_t port)
{
    int rv;
    int ing_profile_idx = -1;

    SOC_IF_ERROR_RETURN(
        _bcm_esw_port_tab_get(unit, port, TAG_ACTION_PROFILE_PTRf,
                              &ing_profile_idx));

    rv = _bcm_trx_vlan_action_profile_entry_delete(unit, ing_profile_idx);
    if (rv < 0) {
        return rv;
    }

    rv = _bcm_esw_port_tab_set_without_portlock(unit, port, _BCM_CPU_TABS_NONE,
                                                TAG_ACTION_PROFILE_PTRf, 0);

    return rv;
}

/*
 * Function:
 *      _bcm_tr_mpls_port_mtu_set
 * Purpose:
 *      Set  Vitual port MTU
 * Returns:
 *      BCM_E_XXX
 */     

int
_bcm_tr_mpls_port_mtu_set(int unit, int vp, int mtu)
{
    int rv=BCM_E_UNAVAIL;
    egr_dvp_attribute_entry_t  egr_dvp_attribute;
    
    sal_memset(&egr_dvp_attribute, 0, sizeof(egr_dvp_attribute_entry_t));

    if (mtu == 0) {
         mtu = 0x3FFF;
    }

    soc_mem_field32_set(unit, EGR_DVP_ATTRIBUTEm, &egr_dvp_attribute,
                                            VP_TYPEf, 0x0);
    soc_mem_field32_set(unit, EGR_DVP_ATTRIBUTEm, &egr_dvp_attribute,
                                            MTU_VALUEf, mtu);
    soc_mem_field32_set(unit, EGR_DVP_ATTRIBUTEm, &egr_dvp_attribute,
                                            MTU_ENABLEf, 0x1);
    rv = soc_mem_write(unit, EGR_DVP_ATTRIBUTEm,
                                            MEM_BLOCK_ALL, vp, &egr_dvp_attribute);
    return rv;

}

/*
 * Function:
 *      _bcm_tr_mpls_port_mtu_clear
 * Purpose:
 *      Clear  Vitual port MTU
 * Returns:
 *      BCM_E_XXX
 */     

int
_bcm_tr_mpls_port_mtu_clear (int unit, int vp)
{
    int rv=BCM_E_UNAVAIL;
    egr_dvp_attribute_entry_t  egr_dvp_attribute;
    
     rv =   soc_mem_read(unit, EGR_DVP_ATTRIBUTEm,
                       MEM_BLOCK_ALL, vp, &egr_dvp_attribute);
     if (BCM_SUCCESS(rv)) {
         if (soc_mem_field32_get(unit, EGR_DVP_ATTRIBUTEm,
                                    &egr_dvp_attribute, MTU_ENABLEf)) {
              soc_mem_field32_set(unit, EGR_DVP_ATTRIBUTEm, &egr_dvp_attribute,
                                            MTU_VALUEf, 0x0);
              soc_mem_field32_set(unit, EGR_DVP_ATTRIBUTEm, &egr_dvp_attribute,
                                            MTU_ENABLEf, 0x0);
              rv = soc_mem_write(unit, EGR_DVP_ATTRIBUTEm,
                                            MEM_BLOCK_ALL, vp, &egr_dvp_attribute);
         }
     }
     return rv;
}

/*
 * Function:
 *      _bcm_tr_mpls_port_mtu_get
 * Purpose:
 *      Get  Vitual port MTU
 * Returns:
 *      BCM_E_XXX
 */     

int
_bcm_tr_mpls_port_mtu_get(int unit, int vp, int *mtu)
{
    int rv=BCM_E_UNAVAIL;
    egr_dvp_attribute_entry_t  egr_dvp_attribute;

     rv =   soc_mem_read(unit, EGR_DVP_ATTRIBUTEm,
                       MEM_BLOCK_ALL, vp, &egr_dvp_attribute);
     if (BCM_SUCCESS(rv)) {
         if (soc_mem_field32_get(unit, EGR_DVP_ATTRIBUTEm,
                                    &egr_dvp_attribute, MTU_ENABLEf)) {
              *mtu =  soc_mem_field32_get(unit, EGR_DVP_ATTRIBUTEm,
                                    &egr_dvp_attribute, MTU_VALUEf);
         }
     }
     return rv;
}

/*
 * Function:
 *      _bcm_tr_mpls_sd_tag_set
 * Purpose:
 *      Program  SD_TAG settings
 * Parameters:
 *      unit           - (IN)  SOC unit #
 *      mpls_port  - (IN)  MPLS VP
 *      egr_nh_info  (IN/OUT) Egress NHop Info
 * Returns:
 *      BCM_E_XXX
 */     

STATIC int
_bcm_tr_mpls_sd_tag_set( int unit, bcm_mpls_port_t *mpls_port, 
                         _bcm_tr_egr_nh_info_t  *egr_nh_info, 
                         egr_mpls_vc_and_swap_label_table_entry_t *vc_entry,
                         int *tpid_index )
{
  int rv = BCM_E_NONE;

    if (mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) {
        if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_ADD) {
            if (mpls_port->egress_service_vlan >= BCM_VLAN_INVALID) {
                return  BCM_E_PARAM;
            }
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_VIDf,
                                mpls_port->egress_service_vlan);
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_ACTION_IF_NOT_PRESENTf,
                                0x1); /* ADD */
        }

        if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_TPID_REPLACE) {
            if (mpls_port->egress_service_vlan >= BCM_VLAN_INVALID) {
                return  BCM_E_PARAM;
            }
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_VIDf,
                                mpls_port->egress_service_vlan);
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_ACTION_IF_PRESENTf,
                                0x1); /* REPLACE_VID_TPID */
        } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_REPLACE) {
            if (mpls_port->egress_service_vlan >= BCM_VLAN_INVALID) {
                return  BCM_E_PARAM;
            }
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_VIDf,
                                mpls_port->egress_service_vlan);
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_ACTION_IF_PRESENTf,
                                0x2); /* REPLACE_VID_ONLY */
        } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_DELETE) {
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_ACTION_IF_PRESENTf,
                                0x3); /* DELETE */
        } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_PRI_TPID_REPLACE) {
            if (soc_feature(unit, soc_feature_mpls_enhanced)) {
               if (mpls_port->egress_service_vlan >= BCM_VLAN_INVALID) {
                   return  BCM_E_PARAM;
               }
               soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_VIDf,
                                mpls_port->egress_service_vlan);
               if (soc_mem_field_valid(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                       SD_TAG_NEW_PRIf)) {
                   soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_NEW_PRIf,
                                mpls_port->pkt_pri);
               }
               if (soc_mem_field_valid(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                       SD_TAG_NEW_CFIf)) {
                   soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_NEW_CFIf,
                                mpls_port->pkt_cfi);
               }

               soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_ACTION_IF_PRESENTf,
                                0x4); /* REPLACE_VID_PRI_TPID */
            }else {
                  return BCM_E_PARAM;
            }
        } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_PRI_REPLACE ) {
            if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                if (mpls_port->egress_service_vlan >= BCM_VLAN_INVALID) {
                    return  BCM_E_PARAM;
                }
                soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_VIDf,
                                mpls_port->egress_service_vlan);

                if (soc_mem_field_valid(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                       SD_TAG_NEW_PRIf)) {
                   soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_NEW_PRIf,
                                mpls_port->pkt_pri);
                }
                if (soc_mem_field_valid(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                       SD_TAG_NEW_CFIf)) {
                   soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_NEW_CFIf,
                                mpls_port->pkt_cfi);
                }

                soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_ACTION_IF_PRESENTf,
                                0x5); /* REPLACE_VID_PRI */
            } else {
                 return BCM_E_PARAM;
            }      
        }else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_PRI_REPLACE ) {
            if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                if (soc_mem_field_valid(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                       SD_TAG_NEW_PRIf)) {
                   soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_NEW_PRIf,
                                mpls_port->pkt_pri);
                }
                if (soc_mem_field_valid(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                       SD_TAG_NEW_CFIf)) {
                   soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_NEW_CFIf,
                                mpls_port->pkt_cfi);
                }

                soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_ACTION_IF_PRESENTf,
                                0x6); /* REPLACE_PRI_ONLY */
            } else {
                 return BCM_E_PARAM;
            }
      
        } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_TPID_REPLACE ) {
            if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                 soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_ACTION_IF_PRESENTf,
                                0x7); /* REPLACE_TPID_ONLY */
            } else {
                 return BCM_E_PARAM;
            }
        }

        if ((mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_ADD) ||
            (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_TPID_REPLACE) ||
            (mpls_port->flags & BCM_MPLS_PORT_SERVICE_TPID_REPLACE) ||
            (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_PRI_TPID_REPLACE)) {
            _bcm_fb2_outer_tpid_tab_lock(unit);
            /* TPID value is used */
            rv = _bcm_fb2_outer_tpid_entry_add(unit, mpls_port->service_tpid, tpid_index);
            _bcm_fb2_outer_tpid_tab_unlock(unit);
            if (rv < 0) {
               return rv;
            }
            soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                vc_entry, SD_TAG_TPID_INDEXf, *tpid_index);
        }

        BCM_IF_ERROR_RETURN(soc_mem_write(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                          MEM_BLOCK_ALL, egr_nh_info->vc_swap_index,
                                          vc_entry));
    } else {
        
        if ((mpls_port->criteria == BCM_MPLS_PORT_MATCH_PORT_SUBPORT_PKT_VID) ||
            (mpls_port->criteria == BCM_MPLS_PORT_MATCH_PORT_SUBPORT_PKT_VID_INNER_VLAN) ||
            (mpls_port->criteria == BCM_MPLS_PORT_MATCH_PORT_SUBPORT_PKT_VID_OUTER_VLAN)) {
            if (soc_feature(unit, soc_feature_lltag)) {
                if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_PRI_REPLACE) {
                    egr_nh_info->lltag_cfi = mpls_port->pkt_cfi;
                    egr_nh_info->lltag_pri = mpls_port->pkt_pri;
                }
                egr_nh_info->lltag_vlan = mpls_port->match_subport_pkt_vid;
            } else {
                return BCM_E_PARAM;
            }
            return BCM_E_NONE;
        }

        if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_ADD) {
            if (mpls_port->egress_service_vlan >= BCM_VLAN_INVALID) {
#if defined(BCM_TRIDENT2PLUS_SUPPORT) 
                if (soc_feature(unit, soc_feature_vp_sharing)) {
                    egr_nh_info->sd_tag_vlan = BCM_VLAN_INVALID;
                } else
#endif
                {
                    return  BCM_E_PARAM;
                }
            }
            if (egr_nh_info->sd_tag_vlan != BCM_VLAN_INVALID) {
                egr_nh_info->sd_tag_vlan = mpls_port->egress_service_vlan;
            }
            egr_nh_info->sd_tag_action_not_present = 0x1; /* ADD */
        }

        if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_TPID_REPLACE) {
            if (mpls_port->egress_service_vlan >= BCM_VLAN_INVALID) {
#if defined(BCM_TRIDENT2PLUS_SUPPORT) 
                if (soc_feature(unit, soc_feature_vp_sharing)) {
                    egr_nh_info->sd_tag_vlan = BCM_VLAN_INVALID;
                } else
#endif
                {
                    return  BCM_E_PARAM;
                }
            }
            /* REPLACE_VID_TPID */
            if (egr_nh_info->sd_tag_vlan != BCM_VLAN_INVALID) {
                egr_nh_info->sd_tag_vlan = mpls_port->egress_service_vlan;
            }
            egr_nh_info->sd_tag_action_present = 0x1;
        } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_REPLACE) {

            if (mpls_port->egress_service_vlan >= BCM_VLAN_INVALID) {
#if defined(BCM_TRIDENT2PLUS_SUPPORT) 
                if (soc_feature(unit, soc_feature_vp_sharing)) {
                    egr_nh_info->sd_tag_vlan = BCM_VLAN_INVALID;
                } else
#endif
                {
                    return  BCM_E_PARAM;
                }
            }
            /* REPLACE_VID_ONLY */
            if (egr_nh_info->sd_tag_vlan != BCM_VLAN_INVALID) {
                egr_nh_info->sd_tag_vlan = mpls_port->egress_service_vlan;
            }
            egr_nh_info->sd_tag_action_present = 0x2;
        } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_DELETE) {
            egr_nh_info->sd_tag_action_present = 0x3; /* DELETE */
        } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_PRI_TPID_REPLACE) {
            if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                if (mpls_port->egress_service_vlan >= BCM_VLAN_INVALID) {
#if defined(BCM_TRIDENT2PLUS_SUPPORT) 
                    if (soc_feature(unit, soc_feature_vp_sharing)) {
                        egr_nh_info->sd_tag_vlan = BCM_VLAN_INVALID;
                    } else
#endif
                    {
                        return  BCM_E_PARAM;
                    }
                }

                /* REPLACE_VID_PRI_TPID */
                if (egr_nh_info->sd_tag_vlan != BCM_VLAN_INVALID) {
                    egr_nh_info->sd_tag_vlan = mpls_port->egress_service_vlan;
                }
                egr_nh_info->sd_tag_pri = mpls_port->pkt_pri;
                egr_nh_info->sd_tag_cfi = mpls_port->pkt_cfi;
                egr_nh_info->sd_tag_action_present = 0x4;
            } else {
                 return BCM_E_PARAM;
            }
        } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_PRI_REPLACE ) {
            if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                if (mpls_port->egress_service_vlan >= BCM_VLAN_INVALID) {
#if defined(BCM_TRIDENT2PLUS_SUPPORT) 
                    if (soc_feature(unit, soc_feature_vp_sharing)) {
                        egr_nh_info->sd_tag_vlan = BCM_VLAN_INVALID;
                    } else
#endif
                    {
                        return  BCM_E_PARAM;
                    }
                }

                /* REPLACE_VID_PRI_ONLY */
                if (egr_nh_info->sd_tag_vlan != BCM_VLAN_INVALID) {
                    egr_nh_info->sd_tag_vlan = mpls_port->egress_service_vlan;
                }
                egr_nh_info->sd_tag_pri = mpls_port->pkt_pri;
                egr_nh_info->sd_tag_cfi = mpls_port->pkt_cfi;
                egr_nh_info->sd_tag_action_present = 0x5;
            } else {
                 return BCM_E_PARAM;
            }
        } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_PRI_REPLACE ) {
            if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                 /* REPLACE_PRI_ONLY */
                egr_nh_info->sd_tag_pri = mpls_port->pkt_pri;
                egr_nh_info->sd_tag_cfi = mpls_port->pkt_cfi;
                egr_nh_info->sd_tag_action_present = 0x6;
            } else {
                 return BCM_E_PARAM;
            }
        } else if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_TPID_REPLACE ) {
            if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                 /* REPLACE_TPID_ONLY */
                egr_nh_info->sd_tag_action_present = 0x7;
            } else {
                return BCM_E_PARAM;
            }

        }

        if ((mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_ADD) ||
            (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_TPID_REPLACE) ||
            (mpls_port->flags & BCM_MPLS_PORT_SERVICE_TPID_REPLACE) ||
            (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_PRI_TPID_REPLACE)) {
            _bcm_fb2_outer_tpid_tab_lock(unit);
            /* TPID value is used */
            rv = _bcm_fb2_outer_tpid_entry_add(unit, mpls_port->service_tpid, tpid_index);
            _bcm_fb2_outer_tpid_tab_unlock(unit);
            if (rv < 0) {
               return rv;
            }
            egr_nh_info->tpid_index = *tpid_index;
        }
    } 
    return  BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_tr_mpls_l2_old_nh_info_delete
 * Purpose:
 *      Delete old L2 MPLS Next Hop entry being replaced
 * Parameters:
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_l2_old_nh_info_delete(int unit, bcm_mpls_port_t *mpls_port,
                                   int nh_index, _bcm_mpls_old_nh_info_t *old_nh_info)
{
    int old_vp_nh_index = -1;
    int egress_nh_index = -1;
    uint32 mpath_flag = 0;

    if (mpls_port->flags & BCM_MPLS_PORT_REPLACE &&
        mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) {
        old_vp_nh_index = old_nh_info->old_nh_index;
        if (old_vp_nh_index == nh_index) {
            old_vp_nh_index = -1;
        }
    }

    /* Handle old VC_SWAP index. */
    if ((old_nh_info->old_vc_swap_idx != -1 && old_vp_nh_index == -1) ||
        (old_nh_info->old_vc_swap_idx != -1 && (mpls_port->flags & BCM_MPLS_PORT_ENCAP_WITH_ID))) {
#if defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit, soc_feature_vc_and_swap_table_overlaid)) {
            (void)_bcm_tr_mpls_vc_and_swap_hash_nhop_delete(unit,
                    old_nh_info->old_vc_swap_idx,
                    nh_index);
        }
#endif
        (void)_bcm_tr_mpls_vc_and_swap_ref_count_adjust(unit,
                old_nh_info->old_vc_swap_idx, -1);
        (void)_bcm_tr_mpls_vc_and_swap_table_index_reset(unit,
                old_nh_info->old_vc_swap_idx);
    }

    /* Delete old TPID, MAC indexes */
    if (old_nh_info->old_tpid_idx != -1) {
        (void)_bcm_fb2_outer_tpid_entry_delete(unit, old_nh_info->old_tpid_idx);
    }

    if (mpls_port->flags & BCM_MPLS_PORT_ENCAP_WITH_ID) {
        if (old_vp_nh_index == nh_index) {
            old_vp_nh_index = -1;    
        }
 
        if (mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) {
            (void)bcm_xgs3_get_nh_from_egress_object(unit, mpls_port->egress_tunnel_if,
                                                    &mpath_flag, 0, &egress_nh_index);
        }

        if (old_nh_info->old_tunnel_if == mpls_port->egress_tunnel_if) {
            if (! (mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
                if (egress_nh_index != -1) {
                    BCM_XGS3_L3_ENT_REF_CNT_DEC
                        (BCM_XGS3_L3_TBL_PTR(unit, next_hop), egress_nh_index, _BCM_SINGLE_WIDE);
                }
            }
        } else {
            if (old_nh_info->old_tunnel_if != 0) {
               BCM_IF_ERROR_RETURN(bcm_xgs3_nh_del(unit, 0,
                             (old_nh_info->old_tunnel_if - BCM_XGS3_EGRESS_IDX_MIN)));
               BCM_IF_ERROR_RETURN(_bcm_tr_mpls_delete_vp_nh(unit,
                              nh_index, old_nh_info->old_tunnel_if));
           }
        }
    }

    if (old_vp_nh_index != -1) {
        /* Free the old_vp_next-hop entry. */
        (void)_bcm_tr_mpls_l2_nh_info_delete(unit, old_vp_nh_index);
    } else if (old_nh_info->old_macda_idx != -1) {
        (void)_bcm_mac_da_profile_entry_delete(unit, old_nh_info->old_macda_idx);
    }
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_tr_mpls_l2_nh_info_add
 * Purpose:
 *      Create the L2 MPLS Next Hop entry
 * Parameters:
 * Returns:
 *      BCM_E_XXX
 */     

STATIC int
_bcm_tr_mpls_l2_nh_info_add(int unit, bcm_mpls_port_t *mpls_port, int vp,
                            int drop, int is_vpws, int *vp_nh_index, 
                            bcm_port_t *local_port, int *is_local,
                            _bcm_mpls_old_nh_info_t *old_nh_info)
{
    egr_l3_next_hop_entry_t egr_nh;
    egr_mac_da_profile_entry_t macda;
    egr_mpls_vc_and_swap_label_table_entry_t vc_entry;
    _bcm_tr_ing_nh_info_t ing_nh_info;
    _bcm_tr_egr_nh_info_t egr_nh_info;
    bcm_l3_egress_t nh_info;
    int gport_id, rv=0, num_vc=0;
    int egress_nh_index = -1, old_vp_nh_index= -1;
    bcm_module_t mod_out = 0;
    bcm_port_t port_out = 0;
    bcm_trunk_t trunk_id=0;
    int action_present, action_not_present, tpid_index = -1;
    int num_mpls_map, hw_map_idx;
    void *entries[1];
    _bcm_port_info_t *info;
    uint32 mpath_flag=0;
    bcm_port_t phys_port;
    bcm_gport_t  gport;
    uint32 port_flags;
#if defined(BCM_TRIUMPH2_SUPPORT)
    int failover_vp, failover_nh_index = -1;
    ing_dvp_table_entry_t failover_dvp;
    uint32 protection_flags=0;
#endif /* BCM_TRIUMPH2_SUPPORT  */
#if defined(BCM_KATANA2_SUPPORT)
    int       my_modid = 0;
#endif
#if defined(BCM_KATANA2_SUPPORT) || defined(BCM_HGPROXY_COE_SUPPORT)
    int is_local_subport = 0;
#endif
#if defined(BCM_KATANA_SUPPORT)
    bcm_module_t       module_id;            /* Module ID           */
    bcm_port_t         port_id;              /* Port ID.            */
    int                local_id;             /* Hardware ID.        */
    int                extended_queue_id;    /* Subscriber queue    */
    ing_queue_map_entry_t ing_queue_entry;
#endif /* BCM_KATANA_SUPPORT */
    int network_group=0;

    /* Initialize values */
    *local_port = 0;
    *is_local = 0;
    if (mpls_port->mtu != 0) {
        ing_nh_info.mtu = mpls_port->mtu;
    }  else {
        ing_nh_info.mtu =  
            (1 << soc_mem_field_length(unit, ING_L3_NEXT_HOPm, MTU_SIZEf)) - 1;
    }
    ing_nh_info.port = -1;
    ing_nh_info.module = -1;
    ing_nh_info.trunk = -1;
    ing_nh_info.qtag = -1;
    ing_nh_info.tag_type = -1;

    egr_nh_info.dvp = vp;
    egr_nh_info.entry_type = -1;
    egr_nh_info.sd_tag_action_present = -1;
    egr_nh_info.sd_tag_action_not_present = -1;
    egr_nh_info.intf_num = -1;
    egr_nh_info.sd_tag_vlan = -1;
    egr_nh_info.sd_tag_pri = -1;
    egr_nh_info.sd_tag_cfi = -1;
    egr_nh_info.lltag_vlan = -1;
    egr_nh_info.lltag_pri = -1;
    egr_nh_info.lltag_cfi = -1;
    egr_nh_info.macda_index = -1;
    egr_nh_info.vc_swap_index = -1;
    egr_nh_info.tpid_index = -1;
    egr_nh_info.pw_init_cnt = -1;
    egr_nh_info.vintf_ctr_index = -1;
    egr_nh_info.flex_ctr_base_cnt_idx = -1;
    egr_nh_info.dvp_network_group_id = 0;
    egr_nh_info.class_id = -1;
    egr_nh_info.oam_global_context_id = 0;

    old_nh_info->old_nh_index = *vp_nh_index;
    old_nh_info->old_tunnel_if = 0;
    old_nh_info->old_tpid_idx = -1;
    old_nh_info->old_macda_idx = -1;
    old_nh_info->old_vc_swap_idx = -1;

    if (soc_feature(unit, soc_feature_multiple_split_horizon_group)) {
        network_group = mpls_port->network_group_id;
        rv = _bcm_validate_splithorizon_network_group(unit,
                mpls_port->flags & BCM_MPLS_PORT_NETWORK, &network_group);
        BCM_IF_ERROR_RETURN(rv);
        egr_nh_info.dvp_network_group_id = network_group;
    }
    if ((mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) &&
        (mpls_port->flags & BCM_MPLS_PORT_NETWORK) ) {
            egr_nh_info.dvp_is_network = 1;
    } else {
            egr_nh_info.dvp_is_network = 0;
    }
    if ((mpls_port->flags & BCM_MPLS_PORT_ENCAP_WITH_ID) ||
            (mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
        rv = bcm_tr_mpls_get_vp_nh (unit, *vp_nh_index, &old_nh_info->old_tunnel_if);
        if (rv == BCM_E_NOT_FOUND) {
            old_nh_info->old_tunnel_if = 0;
        } else if (BCM_FAILURE(rv)) {
            return rv;
        }
    }

    if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
        if ((*vp_nh_index > soc_mem_index_max(unit, EGR_L3_NEXT_HOPm)) ||
            (*vp_nh_index < soc_mem_index_min(unit, EGR_L3_NEXT_HOPm)))  {
            return BCM_E_PARAM;
        }
        /* Read the existing egress next_hop entry */
        rv = soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                          *vp_nh_index, &egr_nh);
        BCM_IF_ERROR_RETURN(rv);
    } else {
       /* Allocate MPLS VP Next Hop */
        BCM_IF_ERROR_RETURN(_bcm_tr_mpls_create_vp_nh 
                            (unit, mpls_port->egress_tunnel_if, vp_nh_index, mpls_port));
        if ((mpls_port->flags & BCM_MPLS_PORT_ENCAP_WITH_ID) && (!old_nh_info->old_tunnel_if)) {
            /* increase the reference count to indicates the given next hop is used by mpls port*/
            BCM_XGS3_L3_ENT_REF_CNT_INC
                (BCM_XGS3_L3_TBL_PTR(unit, next_hop), *vp_nh_index, _BCM_SINGLE_WIDE);

       }
    }

    /*
     * Indexes in the first half of EGR_MPLS_VC_AND_SWAP_LABEL_TABLE
     * also get used as the index into the EGR_PW_INIT_COUNTERS table.
     * Set num_vc to half the EGR_MPLS_VC_AND_SWAP_LABEL_TABLE size.
     */
    num_vc = soc_mem_index_count(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);
    num_vc /= 2;

    if (mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) {

        num_mpls_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 64;
        if (mpls_port->egress_label.qos_map_id == 0) {
            hw_map_idx = 0; /* use default */
        } else {
            BCM_IF_ERROR_RETURN(_egr_qos_id2hw_idx(unit,
                            mpls_port->egress_label.qos_map_id,&hw_map_idx));
        }

        if (hw_map_idx < 0 || hw_map_idx >= num_mpls_map) {
            rv = BCM_E_PARAM;
            goto cleanup;
        }

        /* Egressing into an MPLS tunnel */
        if (!BCM_XGS3_L3_EGRESS_IDX_VALID(unit, mpls_port->egress_tunnel_if)) {
            /* invalid egress object */
            rv = BCM_E_PARAM;
            goto cleanup;
        } else if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
            /* Be sure that the existing entry is already setup to
             * egress into an MPLS tunnel. If not, return BCM_E_PARAM.
             */
            egr_nh_info.entry_type = 
                soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, ENTRY_TYPEf);
            if (egr_nh_info.entry_type != 0x1) { /* != MPLS_MACDA_PROFILE */
                rv = BCM_E_PARAM;
                goto cleanup;
            }

            if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                /* TD2 should defines BCM_TRIUMPH3_SUPPORT */
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
                if (soc_feature(unit,soc_feature_advanced_flex_counter)) {
                    egr_nh_info.flex_ctr_base_cnt_idx = 
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                             MPLS__FLEX_CTR_BASE_COUNTER_IDXf);
                    egr_nh_info.flex_ctr_pool_num = 
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                             MPLS__FLEX_CTR_POOL_NUMBERf);
                    egr_nh_info.flex_ctr_offset_mode = 
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                             MPLS__FLEX_CTR_OFFSET_MODEf);
                } else
#endif
                if (soc_feature(unit, soc_feature_gport_service_counters)) {
                    egr_nh_info.vintf_ctr_index = 
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                             MPLS__VINTF_CTR_IDXf);
                }
            }

            /* Remember old MAC_DA_PROFILE_INDEX,  VC_AND_SWAP_INDEX */
            if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                old_nh_info->old_macda_idx =
                    soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                MPLS__MAC_DA_PROFILE_INDEXf);
                old_nh_info->old_vc_swap_idx =
                    soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                MPLS__VC_AND_SWAP_INDEXf);
            } else {
                old_nh_info->old_macda_idx =
                    soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                MAC_DA_PROFILE_INDEXf);
                old_nh_info->old_vc_swap_idx =
                    soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                VC_AND_SWAP_INDEXf);
            }

            /* Remember old TPID index if it's used */
            rv = soc_mem_read(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                              MEM_BLOCK_ALL, old_nh_info->old_vc_swap_idx, &vc_entry);
            _BCM_MPLS_CLEANUP(rv);
            action_present =
                soc_EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm_field32_get(unit,
                                                 &vc_entry,
                                                 SD_TAG_ACTION_IF_PRESENTf);
            action_not_present =
                soc_EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm_field32_get(unit,
                                                 &vc_entry,
                                                 SD_TAG_ACTION_IF_NOT_PRESENTf);
            if ((action_not_present == 0x1) || (action_present == 0x1)) {
                /* If SD tag action is ADD or REPLACE_VID_TPID, the tpid
                 * index of the entry getting replaced is valid. Save
                 * the old tpid index to be deleted later.
                 */
                old_nh_info->old_tpid_idx =
                    soc_EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm_field32_get(unit,
                                                 &vc_entry, SD_TAG_TPID_INDEXf);
            }

            /* Remember old vc_nh_index */
            old_vp_nh_index = *vp_nh_index;

            /* Allocate MPLS VP Next Hop */
            if (old_nh_info->old_tunnel_if != mpls_port->egress_tunnel_if) {
                BCM_IF_ERROR_RETURN(_bcm_tr_mpls_create_vp_nh
                                    (unit, mpls_port->egress_tunnel_if, vp_nh_index,
                                     mpls_port));
            }
        }

        /* 
         * Get egress next-hop index from egress object and
         * increment egress object reference count. 
         */
        if (old_vp_nh_index != *vp_nh_index) {
            rv = bcm_xgs3_get_nh_from_egress_object(unit, mpls_port->egress_tunnel_if,
                    &mpath_flag, 1, &egress_nh_index);
        } else {
            rv = bcm_xgs3_get_nh_from_egress_object(unit, mpls_port->egress_tunnel_if,
                    &mpath_flag, 0, &egress_nh_index);
        }
        _BCM_MPLS_CLEANUP(rv);

        rv = bcm_xgs3_nh_get(unit, egress_nh_index, &nh_info);
        _BCM_MPLS_CLEANUP(rv);

#if defined(BCM_KATANA_SUPPORT)
        if (SOC_IS_KATANAX(unit)) {
            ing_l3_next_hop_entry_t in_nh;

            rv = soc_mem_read (unit, ING_L3_NEXT_HOPm,
                        MEM_BLOCK_ALL, egress_nh_index, &in_nh);
            _BCM_MPLS_CLEANUP(rv);
            ing_nh_info.qtag = soc_ING_L3_NEXT_HOPm_field32_get(unit, &in_nh,
                                          EH_QUEUE_TAGf);
            ing_nh_info.tag_type = soc_ING_L3_NEXT_HOPm_field32_get(unit,
                                          &in_nh,
                                          EH_TAG_TYPEf);
        }
#endif

        /* make sure (mod/port) is local(single modid) view */
        rv = _bcm_esw_stk_modmap_map(unit, BCM_STK_MODMAP_SET,
                       nh_info.module, nh_info.port, 
                       &nh_info.module, &nh_info.port);
        BCM_IF_ERROR_RETURN(rv);

        if (nh_info.flags & BCM_L3_TGID) {
            ing_nh_info.port = -1;
            ing_nh_info.module = -1;
            ing_nh_info.trunk = nh_info.trunk;
            trunk_id =  nh_info.trunk;
        } else {
            ing_nh_info.port = nh_info.port;
            port_out = nh_info.port;
            ing_nh_info.module = nh_info.module;
            mod_out = nh_info.module;
            ing_nh_info.trunk = -1; 
        }

        egr_nh_info.intf_num = nh_info.intf;
        egr_nh_info.entry_type = 0x1; /* MPLS_MACDA_PROFILE */
        egr_nh_info.oam_global_context_id = nh_info.oam_global_context_id;

        sal_memset(&macda, 0, sizeof(egr_mac_da_profile_entry_t));
        soc_mem_mac_addr_set(unit, EGR_MAC_DA_PROFILEm, 
                             &macda, MAC_ADDRESSf, nh_info.mac_addr);
        entries[0] = &macda;
        rv = _bcm_mac_da_profile_entry_add(unit, entries, 1,
                                           (uint32 *) &egr_nh_info.macda_index);
        _BCM_MPLS_CLEANUP(rv);

        if ((mpls_port->flags & BCM_MPLS_PORT_COUNTED) ||
            (mpls_port->flags & BCM_MPLS_PORT_SEQUENCED)) {

            rv = _bcm_tr_mpls_get_vc_and_swap_table_index (unit, 
                                   _BCM_MPLS_ACTION_SEQUENCE, 
                                   mpls_port, NULL, NULL,
                                   _BCM_MPLS_ACTION_PUSH,
                                   &egr_nh_info.vc_swap_index);
            _BCM_MPLS_CLEANUP(rv);

            rv = _bcm_tr_mpls_pw_init_counter_set (unit, mpls_port->pw_seq_number, &egr_nh_info);
            _BCM_MPLS_CLEANUP(rv);

        } else {

            rv = _bcm_tr_mpls_get_vc_and_swap_table_index (unit, 0,
                                   mpls_port, NULL, NULL,
                                   _BCM_MPLS_ACTION_PUSH,
                                   &egr_nh_info.vc_swap_index);
            _BCM_MPLS_CLEANUP(rv);
        }
        /* Program the MPLS_VC_AND_SWAP table entry */
        rv = _bcm_tr_mpls_vc_and_swap_table_entry_set(unit, mpls_port, NULL, NULL,
                                                 _BCM_MPLS_ACTION_PUSH, &vc_entry, hw_map_idx);
        _BCM_MPLS_CLEANUP(rv);
        /* SD_TAG setting */
        rv = _bcm_tr_mpls_sd_tag_set(unit, mpls_port, &egr_nh_info, &vc_entry, &tpid_index);
        _BCM_MPLS_CLEANUP(rv);

        rv = soc_mem_write(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                           MEM_BLOCK_ALL, egr_nh_info.vc_swap_index,
                           &vc_entry);
        _BCM_MPLS_CLEANUP(rv);
    } else {
        if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
            /* Be sure that the existing entry is not egressing into
             * an MPLS tunnel. If it is, return BCM_E_PARAM.
             */
            egr_nh_info.entry_type = 
                soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, ENTRY_TYPEf);
            if (egr_nh_info.entry_type != 0x2) { /* != MPLS_SD_TAG_ACTIONS */
                rv = BCM_E_PARAM;
                goto cleanup;
            }

            if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                action_present =
                     soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                 SD_TAG__SD_TAG_ACTION_IF_PRESENTf);
                action_not_present =
                     soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                 SD_TAG__SD_TAG_ACTION_IF_NOT_PRESENTf);
            } else {
                action_present =
                     soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                 SD_TAG_ACTION_IF_PRESENTf);
                action_not_present =
                     soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                 SD_TAG_ACTION_IF_NOT_PRESENTf);
            }
            if ((action_not_present == 0x1) || (action_present == 0x1)) {
                /* If SD tag action is ADD or REPLACE_VID_TPID, the tpid
                 * index of the entry getting replaced is valid. Save
                 * the old tpid index to be deleted later.
                 */
                if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                    old_nh_info->old_tpid_idx =
                          soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                 SD_TAG__SD_TAG_TPID_INDEXf);
                } else {
                    old_nh_info->old_tpid_idx =
                          soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                 SD_TAG_TPID_INDEXf);
                }
            }

            if (soc_feature(unit, soc_feature_mpls_enhanced) && 
                    soc_feature(unit, soc_feature_gport_service_counters)) {
                egr_nh_info.vintf_ctr_index = 
                   soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                        SD_TAG__VINTF_CTR_IDXf);
            }
            if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                /* TD2 should defines BCM_TRIUMPH3_SUPPORT */
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
                if (soc_feature(unit,soc_feature_advanced_flex_counter)) {
                    egr_nh_info.flex_ctr_base_cnt_idx =
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                             SD_TAG__FLEX_CTR_BASE_COUNTER_IDXf);
                    egr_nh_info.flex_ctr_pool_num =
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                             SD_TAG__FLEX_CTR_POOL_NUMBERf);
                    egr_nh_info.flex_ctr_offset_mode =
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                             SD_TAG__FLEX_CTR_OFFSET_MODEf);
                } else
#endif
                if (soc_feature(unit, soc_feature_gport_service_counters)) {
                    egr_nh_info.vintf_ctr_index =
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                             SD_TAG__VINTF_CTR_IDXf);
                }
            }
        }

        egr_nh_info.entry_type = 0x2; /* MPLS_SD_TAG_ACTIONS */

        rv = _bcm_esw_gport_resolve(unit, mpls_port->port, &mod_out, 
                                    &port_out, &trunk_id, &gport_id);
        _BCM_MPLS_CLEANUP(rv);

        if (BCM_GPORT_IS_TRUNK(mpls_port->port)) {
            ing_nh_info.module = -1;
            ing_nh_info.port = -1;
            ing_nh_info.trunk = trunk_id;
        } else {
            ing_nh_info.module = mod_out;
            ing_nh_info.port = port_out;
            ing_nh_info.trunk = -1;
        }

        /* SD_TAG setting */
        rv = _bcm_tr_mpls_sd_tag_set(unit, mpls_port, &egr_nh_info, &vc_entry, &tpid_index);
        _BCM_MPLS_CLEANUP(rv);

    }

    /* CLASS ID */
    if (mpls_port->egress_class_id != BCM_CLASS_ID_INVALID) {
        int len = 0;

        if ((mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) &&
                (mpls_port->egress_class_id == nh_info.intf_class)) {
            /*
             * if you want to have same interface class, then
             * add it through next hop.
             */
            return BCM_E_PARAM;
        }

        len = soc_mem_field_length(unit, EGR_L3_NEXT_HOPm, MPLS__CLASS_IDf);
        if (mpls_port->egress_class_id >= (1 << len)) {
            return BCM_E_PARAM;
        }

        egr_nh_info.class_id = mpls_port->egress_class_id;
    } else if (mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) {
        egr_nh_info.class_id = nh_info.intf_class;
    }

    /* Update the physical port's SW state */
    rv = _bcm_esw_modid_is_local(unit, ing_nh_info.module, is_local);
    _BCM_MPLS_CLEANUP(rv);

#if defined(BCM_HGPROXY_COE_SUPPORT)
    if (soc_feature(unit, soc_feature_hgproxy_subtag_coe) &&
        BCM_GPORT_IS_SET(mpls_port->port) &&
        _BCM_COE_GPORT_IS_SUBTAG_SUBPORT_PORT(unit,
            mpls_port->port)) {

        if(_bcmi_coe_subport_mod_port_physical_port_get(unit, mod_out,
				port_out, &phys_port) == BCM_E_NONE) {
            is_local_subport = 1;
        }
    }

    if (is_local_subport) {
        _bcm_port_info_access(unit, phys_port, &info);
        info->vp_count++;
        /* Update the physical port's attributes */
        rv = bcm_esw_port_vlan_member_get(unit, phys_port, &port_flags);
        _BCM_MPLS_CLEANUP(rv);

        rv = bcm_esw_port_vlan_member_set(unit, phys_port, port_flags);
        _BCM_MPLS_CLEANUP(rv);
    } else
#endif

#if defined(BCM_KATANA2_SUPPORT)
    if (BCM_GPORT_IS_SET(mpls_port->port) &&
        _BCM_KT2_GPORT_IS_LINKPHY_OR_SUBTAG_SUBPORT_GPORT(unit,
            mpls_port->port)) {
        rv = bcm_esw_stk_my_modid_get(unit, &my_modid);
        _BCM_MPLS_CLEANUP(rv);
        if (_bcm_kt2_mod_is_coe_mod_check(unit, mod_out) == BCM_E_NONE) {
            rv = _bcm_kt2_modport_to_pp_port_get(
                     unit, mod_out, port_out, &phys_port);
            _BCM_MPLS_CLEANUP(rv);
            if (_bcm_kt2_is_pp_port_linkphy_subtag(unit, phys_port)){
                is_local_subport = 1;
            }
        }
    }

    if (is_local_subport) {
        _bcm_port_info_access(unit, phys_port, &info);
        info->vp_count++;
        /* Update the physical port's attributes */
        rv = bcm_esw_port_vlan_member_get(unit, phys_port, &port_flags);
        _BCM_MPLS_CLEANUP(rv);

        rv = bcm_esw_port_vlan_member_set(unit, phys_port, port_flags);
        _BCM_MPLS_CLEANUP(rv);
    } else
#endif

    if (*is_local) {

        BCM_GPORT_MODPORT_SET(gport, ing_nh_info.module, ing_nh_info.port);
        BCM_IF_ERROR_RETURN(bcm_esw_port_local_get(unit, gport, &phys_port));

        _bcm_port_info_access(unit, phys_port, &info);
        info->vp_count++;
        /* Update the physical port's attributes */
        rv = bcm_esw_port_vlan_member_get(unit, phys_port, &port_flags);
        _BCM_MPLS_CLEANUP(rv);

        rv = bcm_esw_port_vlan_member_set(unit, phys_port, port_flags);
        _BCM_MPLS_CLEANUP(rv);

    }

    /* If associated with a trunk, update each local physical port's SW state */
    if (ing_nh_info.trunk != -1) {
#if defined(BCM_ESW_SUPPORT) && (SOC_MAX_NUM_PP_PORTS > SOC_MAX_NUM_PORTS)
        int        max_num_port = SOC_MAX_NUM_PP_PORTS;
        bcm_port_t local_member_array[SOC_MAX_NUM_PP_PORTS];
#else
        int        max_num_port = SOC_MAX_NUM_PORTS;
        bcm_port_t local_member_array[SOC_MAX_NUM_PORTS];
#endif
        int local_member_count;
        int idx;
        uint32 port_flags;

        rv = _bcm_esw_trunk_local_members_get(unit, trunk_id,
                max_num_port, local_member_array, &local_member_count);
        _BCM_MPLS_CLEANUP(rv);

        for (idx = 0; idx < local_member_count; idx++) {
            _bcm_port_info_access(unit, local_member_array[idx], &info);
            info->vp_count++;

            /* Update the physical port's attributes */
            rv = bcm_esw_port_vlan_member_get(unit, local_member_array[idx], &port_flags);
            _BCM_MPLS_CLEANUP(rv);
            rv = bcm_esw_port_vlan_member_set(unit, local_member_array[idx], port_flags);
            _BCM_MPLS_CLEANUP(rv);
        }
    }

    /* Write INITIAL_ING_L3_NEXT_HOP entry */
    rv = _bcm_tr_mpls_INITIAL_ING_L3_NEXT_HOP_add(unit, 
                                             &ing_nh_info, *vp_nh_index, 
                                             local_port, is_local);
    _BCM_MPLS_CLEANUP(rv);

    /* Write EGR_L3_NEXT_HOP entry */
    rv = _bcm_tr_mpls_EGR_L3_NEXT_HOP_add(unit, &egr_nh_info, 
                                          mpls_port->flags2, *vp_nh_index,
                                          is_local, is_vpws, drop);
    _BCM_MPLS_CLEANUP(rv);

#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
       egr_l3_next_hop_entry_t egr_nh;
       rv = soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, *vp_nh_index, &egr_nh);
       _BCM_MPLS_CLEANUP(rv);

       if (SOC_IS_TRIUMPH3(unit) && soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm,
           MPLS__DO_EVXLT_AFTER_TUNNEL_ENCAPf)) {
           soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
               MPLS__DO_EVXLT_AFTER_TUNNEL_ENCAPf,
               (mpls_port->flags2 &
               BCM_MPLS_PORT2_TUNNEL_HEADER_EGRESS_VLAN_TRANSLATION) != 0);
       }
       rv = soc_mem_write (unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ALL, *vp_nh_index,
            &egr_nh);

       _BCM_MPLS_CLEANUP(rv);
    }
#endif

#ifdef BCM_TRIDENT2PLUS_SUPPORT
    if (soc_feature(unit, soc_feature_egress_failover)) {
        egr_l3_next_hop_entry_t egr_nh;
        int local_failover_id=0,prot_type=0;
        if (mpls_port->egress_failover_id!=0) {
            local_failover_id=mpls_port->egress_failover_id;
            _BCM_GET_FAILOVER_TYPE(local_failover_id, prot_type);
            if (prot_type & _BCM_FAILOVER_1_1_MC_PROTECTION_MODE) {
                _BCM_GET_FAILOVER_ID(local_failover_id);

                BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm,
                            MEM_BLOCK_ANY, *vp_nh_index, &egr_nh));

                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                        MPLS__PROT_GROUPf, local_failover_id);

                rv = soc_mem_write(unit, EGR_L3_NEXT_HOPm,
                        MEM_BLOCK_ALL, *vp_nh_index, &egr_nh);
            }
        }
    }
#endif
#ifdef BCM_KATANA_SUPPORT
    if (soc_feature(unit, soc_feature_extended_queueing)) {
        if ((BCM_GPORT_IS_UCAST_QUEUE_GROUP(mpls_port->per_flow_queue_base)) ||
            (BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP
                                       (mpls_port->per_flow_queue_base)) ||
            (BCM_GPORT_IS_MCAST_SUBSCRIBER_QUEUE_GROUP
                                       (mpls_port->per_flow_queue_base))) {
            _BCM_MPLS_CLEANUP(_bcm_esw_gport_resolve(unit,
                                            mpls_port->per_flow_queue_base,
                                            &port_id,
                                            &module_id,
                                            &extended_queue_id,
                                            &local_id));
            if (((mpls_port->qos_map_id >> _BCM_QOS_MAP_SHIFT) ==
                              _BCM_QOS_MAP_TYPE_ING_QUEUE_OFFSET_MAP_TABLE) &&
                ((mpls_port->qos_map_id & _BCM_QOS_MAP_TYPE_MASK) <=
                   _BCM_QOS_MAP_ING_QUEUE_OFFSET_MAX)) {
            /*  ING_QUEUE_MAP needs to be configured */
                sal_memset(&ing_queue_entry, 0, sizeof(ing_queue_map_entry_t));
                soc_mem_field32_set(unit, ING_QUEUE_MAPm, &ing_queue_entry,
                                    QUEUE_SET_BASEf, extended_queue_id);
                soc_mem_field32_set(unit, ING_QUEUE_MAPm, &ing_queue_entry,
                             QUEUE_OFFSET_PROFILE_INDEXf,
                             (mpls_port->qos_map_id & _BCM_QOS_MAP_TYPE_MASK));
                _BCM_MPLS_CLEANUP(BCM_XGS3_MEM_WRITE(unit, ING_QUEUE_MAPm,
                                                       extended_queue_id,
                                                       &ing_queue_entry));
                /* Derive actual queue from qos map */
                ing_nh_info.tag_type = EH_TAG_TYPE_ING_QUEUE_MAP;
                ing_nh_info.qtag = extended_queue_id;

                /* Update ING_QUEUE_MAP reference count */
                _bcm_tr_mpls_ing_queue_map_ref_count_adjust (unit,
                                                             extended_queue_id,
                                                             +1);

            } else {
                /* use extended_queue_id as Queue */
                ing_nh_info.tag_type = EH_TAG_TYPE_EXPLICT_QUEUE;
                ing_nh_info.qtag = extended_queue_id;
            }
        }
        if ((mpls_port->flags & BCM_MPLS_PORT_REPLACE) &&
                (0 == mpls_port->per_flow_queue_base)) {
            if (ing_nh_info.qtag > 0) {
                if (ing_nh_info.tag_type == EH_TAG_TYPE_ING_QUEUE_MAP)  {
                    /* Update ING_QUEUE_MAP reference count */
                    _bcm_tr_mpls_ing_queue_map_ref_count_adjust (unit,
                                                               ing_nh_info.qtag,
                                                               -1);
                    /*  ING_QUEUE_MAP needs to be reset */
                    _BCM_MPLS_CLEANUP(
                     _bcm_tr_mpls_ing_queue_map_index_reset (unit,
                                                             ing_nh_info.qtag));
                }
                ing_nh_info.qtag = 0;
                ing_nh_info.tag_type = EH_TAG_TYPE_NONE;  /* No HQOS */
            }
        }
    }
#endif /* BCM_KATANA_SUPPORT */
    /* Write ING_L3_NEXT_HOP entry */
    rv = _bcm_tr_mpls_ING_L3_NEXT_HOP_add(unit, &ing_nh_info,
                                            *vp_nh_index, drop);
    _BCM_MPLS_CLEANUP(rv);

#if defined(BCM_TRIUMPH2_SUPPORT)
    if (soc_feature(unit, soc_feature_mpls_failover)) {
        if (BCM_SUCCESS(_bcm_esw_failover_mpls_check(unit, mpls_port))) {
            failover_vp = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->failover_port_id);
            if (failover_vp != -1) {
                rv = READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, failover_vp, &failover_dvp);
                _BCM_MPLS_CLEANUP(rv);

                failover_nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &failover_dvp,
                                                   NEXT_HOP_INDEXf);
            }

            if (_BCM_MULTICAST_IS_SET(mpls_port->failover_mc_group)) {
                 protection_flags |= _BCM_FAILOVER_1_PLUS_PROTECTION;
            }
            if (mpls_port->flags & BCM_MPLS_PORT_FAILOVER) {
                  bcm_failover_t failover_id;
                  int multicast_group;
                  rv = _bcm_esw_failover_prot_nhi_get(unit, failover_nh_index,
                                &failover_id, &old_nh_info->old_nh_index, &multicast_group);
                  _BCM_MPLS_CLEANUP(rv);
                  /* coverity[negative_returns : FALSE] */
                  rv = _bcm_esw_failover_prot_nhi_create(unit, protection_flags, failover_nh_index,
                                (uint32) *vp_nh_index, mpls_port->failover_mc_group, mpls_port->failover_id);
                  _BCM_MPLS_CLEANUP(rv);

                  if (old_nh_info->old_nh_index == 0) {
                      old_nh_info->old_nh_index = -1;
                  }
            } else {
                 rv = _bcm_esw_failover_prot_nhi_create(unit, protection_flags, (uint32) *vp_nh_index,
                     failover_nh_index, mpls_port->failover_mc_group, mpls_port->failover_id);

                 _BCM_MPLS_CLEANUP(rv);
            }
       } else if ((mpls_port->flags & BCM_MPLS_PORT_REPLACE)  &&
                    (mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL)&&
                    (old_vp_nh_index != *vp_nh_index) &&
                    (mpls_port->flags & BCM_MPLS_PORT_FAILOVER)) {

             rv = _bcm_esw_failover_prot_nhi_update(unit, old_vp_nh_index, (uint32) *vp_nh_index );
            _BCM_MPLS_CLEANUP(rv);
      }
    }
#endif /* BCM_TRIUMPH2_SUPPORT  */
#if defined(BCM_TRIUMPH3_SUPPORT)
    if ((mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) &&
        soc_feature(unit, soc_feature_vc_and_swap_table_overlaid)) {
        rv = _bcm_tr_mpls_vc_and_swap_hash_nhop_add(unit,
                                                    egr_nh_info.vc_swap_index,
                                                    *vp_nh_index);
        _BCM_MPLS_CLEANUP(rv);
    }
#endif
    return rv;

cleanup:
    if (!(mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
        (void) bcm_xgs3_nh_del(unit, _BCM_L3_SHR_WRITE_DISABLE, *vp_nh_index);
    }
    if (egress_nh_index != -1) {
        (void) bcm_xgs3_nh_del(unit, 0, egress_nh_index);
    }
    if (tpid_index != -1) {
        (void) _bcm_fb2_outer_tpid_entry_delete(unit, tpid_index);
    }
    if (egr_nh_info.macda_index != -1) {
        (void) _bcm_mac_da_profile_entry_delete(unit, egr_nh_info.macda_index);
    }
    if (egr_nh_info.vc_swap_index != -1) {
         _bcm_tr_mpls_vc_and_swap_ref_count_adjust (unit,
                             egr_nh_info.vc_swap_index, -1);
         (void) _bcm_tr_mpls_vc_and_swap_table_index_reset (unit, 
                             egr_nh_info.vc_swap_index);
    }
    return rv;
}

STATIC int
_bcm_tr_mpls_l2_nh_info_delete(int unit, int nh_index)
{
    int rv=BCM_E_NONE, is_local, old_macda_idx = -1, old_vc_swap_idx = -1;
    int action_present, action_not_present, old_tpid_idx = -1;
    bcm_if_t tunnel_if = 0;
    egr_mpls_vc_and_swap_label_table_entry_t vc_entry;
    initial_ing_l3_next_hop_entry_t initial_ing_nh;
    ing_l3_next_hop_entry_t ing_nh;
    egr_l3_next_hop_entry_t egr_nh;
    bcm_module_t modid;
    bcm_trunk_t trunk_id;
    _bcm_port_info_t *info;
    bcm_port_t phys_port;
    uint32 port_flags;
#if defined(BCM_KATANA2_SUPPORT)
    bcm_port_t pp_port = -1;
#endif
#if defined(BCM_KATANA_SUPPORT)
    int qtag, qtag_type;
#endif
#if defined(BCM_HGPROXY_COE_SUPPORT)
    int local_port;
#endif

    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, 
                                  MEM_BLOCK_ANY, nh_index, &egr_nh));
    BCM_IF_ERROR_RETURN (soc_mem_read(unit, ING_L3_NEXT_HOPm, 
                                  MEM_BLOCK_ANY, nh_index, &ing_nh));
    BCM_IF_ERROR_RETURN (soc_mem_read(unit, INITIAL_ING_L3_NEXT_HOPm, 
                                  MEM_BLOCK_ANY, nh_index, &initial_ing_nh));

    if (soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, ENTRY_TYPEf) == 0x1) {
        /* Egressing into an MPLS tunnel */
        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
              old_macda_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                         MPLS__MAC_DA_PROFILE_INDEXf);
              old_vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                         MPLS__VC_AND_SWAP_INDEXf);
        } else {
              old_macda_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                           MAC_DA_PROFILE_INDEXf);
              old_vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                           VC_AND_SWAP_INDEXf);
        }
        /* Get old TPID index if it's used */
        rv = soc_mem_read(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                          MEM_BLOCK_ALL, old_vc_swap_idx, &vc_entry);
        BCM_IF_ERROR_RETURN(rv);
        action_present =
            soc_EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm_field32_get(unit,
                                             &vc_entry,
                                             SD_TAG_ACTION_IF_PRESENTf);
        action_not_present =
            soc_EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm_field32_get(unit,
                                             &vc_entry,
                                             SD_TAG_ACTION_IF_NOT_PRESENTf);
        if ((action_not_present == 0x1) || (action_present == 0x1)) {
            /* If SD tag action is ADD or REPLACE_VID_TPID, the tpid
             * index of the entry is valid. Save
             * the old tpid index to be deleted later.
             */
              old_tpid_idx =
                   soc_EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm_field32_get(unit,
                                             &vc_entry, SD_TAG_TPID_INDEXf);
        }

        /* Find the tunnel_if */
        rv = bcm_tr_mpls_get_vp_nh (unit, nh_index, &tunnel_if);
        if ( rv < 0) {
            return rv;
        }

        rv = _bcm_tr_mpls_pw_init_counter_delete (unit, nh_index);
        if ( rv < 0) {
            return rv;
        }
    } else if (soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, 
                                                ENTRY_TYPEf) == 0x2) {
        /* egressing into a regular port */
        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
              action_present =
                      soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                            SD_TAG__SD_TAG_ACTION_IF_PRESENTf);
              action_not_present =
                      soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                            SD_TAG__SD_TAG_ACTION_IF_NOT_PRESENTf);
        } else {
              action_present =
                      soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                            SD_TAG_ACTION_IF_PRESENTf);
              action_not_present =
                      soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                            SD_TAG_ACTION_IF_NOT_PRESENTf);
        }
        if ((action_not_present == 0x1) || (action_present == 0x1)) {
            /* If SD tag action is ADD or REPLACE_VID_TPID, the tpid
             * index of the entry is valid. Save
             * the old tpid index to be deleted later.
             */
                if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                    old_tpid_idx =
                          soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                 SD_TAG__SD_TAG_TPID_INDEXf);
                } else {
                    old_tpid_idx =
                          soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                 SD_TAG_TPID_INDEXf);
                }

        }
    } else {
        return BCM_E_NOT_FOUND;
    }

#if defined(BCM_TRIUMPH2_SUPPORT)
    if (soc_feature(unit, soc_feature_mpls_failover)) {
        rv = _bcm_esw_failover_prot_nhi_cleanup(unit, nh_index);
        if ( (rv != BCM_E_NOT_FOUND) && (rv != BCM_E_NONE) ) {
            return rv;
        }
    }
#endif /* BCM_TRIUMPH2_SUPPORT  */

    /* Update the physical port's SW state */
    modid = soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, MODULE_IDf);
#if defined(BCM_HGPROXY_COE_SUPPORT)
    if (soc_feature(unit, soc_feature_hgproxy_subtag_coe) &&
        _bcm_xgs5_subport_coe_mod_local(unit, modid)) {
            phys_port = 
                soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, PORT_NUMf);
            BCM_IF_ERROR_RETURN(
                _bcmi_coe_subport_mod_port_physical_port_get(unit, modid,
                                                             phys_port,
                                                             &local_port));
            _bcm_port_info_access(unit, local_port, &info);
            info->vp_count--;
            /* Update the physical port's attributes */
            BCM_IF_ERROR_RETURN(bcm_esw_port_vlan_member_get(unit,
                local_port, &port_flags));
            BCM_IF_ERROR_RETURN(bcm_esw_port_vlan_member_set(unit,
                local_port, port_flags));
    } else 
#endif
    {
        BCM_IF_ERROR_RETURN(_bcm_esw_modid_is_local(unit, modid, &is_local));
        if (is_local && !soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, Tf)) {
            phys_port = soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh,
                                                         PORT_NUMf); 
            if (soc_feature(unit, soc_feature_sysport_remap)) { 
                BCM_XLATE_SYSPORT_S2P(unit, &phys_port); 
            }
            _bcm_port_info_access(unit, phys_port, &info);
            info->vp_count--;
            /* Update the physical port's attributes */
            BCM_IF_ERROR_RETURN
                (bcm_esw_port_vlan_member_get(unit, phys_port, &port_flags));
            BCM_IF_ERROR_RETURN
                /* coverity[stack_use_callee] */
                /* coverity[stack_use_overflow] */
                (bcm_esw_port_vlan_member_set(unit, phys_port, port_flags));
        }
    }

#if defined(BCM_KATANA2_SUPPORT)
    if (soc_feature(unit, soc_feature_linkphy_coe) ||
        soc_feature(unit, soc_feature_subtag_coe)) {
        /* For LinkPHY/SubportPktTag pp_ports, check if the mod is a valid one */
        if (_bcm_kt2_mod_is_coe_mod_check(unit, modid) == BCM_E_NONE) {
            phys_port =
                soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, PORT_NUMf);
            BCM_IF_ERROR_RETURN(
                _bcm_kt2_modport_to_pp_port_get(unit, modid,
                    phys_port, &pp_port));
            /*For KT2, pp_port >=42 are used for LinkPHY/SubportPktTag subport.
            * Get the subport info associated with the pp_port and form the
            * subport_gport */
            if (_bcm_kt2_is_pp_port_linkphy_subtag(unit, pp_port)){
                _bcm_port_info_access(unit, pp_port, &info);
                info->vp_count--;
                /* Update the physical port's attributes */
                BCM_IF_ERROR_RETURN(bcm_esw_port_vlan_member_get(unit,
                            pp_port, &port_flags));
                BCM_IF_ERROR_RETURN(bcm_esw_port_vlan_member_set(unit,
                            pp_port, port_flags));
            }
        }
    }
#endif

    /* If associated with a trunk, update each local physical port's SW state */
    if (soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, Tf)) {
        int idx;
        uint32 port_flags;
#if defined(BCM_ESW_SUPPORT) && (SOC_MAX_NUM_PP_PORTS > SOC_MAX_NUM_PORTS)
        int        max_num_port = SOC_MAX_NUM_PP_PORTS;
        bcm_port_t local_member_array[SOC_MAX_NUM_PP_PORTS];
#else
        int        max_num_port = SOC_MAX_NUM_PORTS;
        bcm_port_t local_member_array[SOC_MAX_NUM_PORTS];
#endif
        int local_member_count;

        trunk_id = soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, TGIDf);
        rv = _bcm_esw_trunk_local_members_get(unit, trunk_id,
                                              max_num_port, local_member_array,
                                              &local_member_count);
        /* The resource(next_hop/vx_and_swap entry etc.) should be cleared even
         * if local members can't be got.
         */
        if (BCM_E_NONE == rv) {
            for (idx = 0; idx < local_member_count; idx++) {
                _bcm_port_info_access(unit, local_member_array[idx], &info);
                info->vp_count--;

                /* Update the physical port's attributes */
                BCM_IF_ERROR_RETURN(
                    bcm_esw_port_vlan_member_get(unit, local_member_array[idx],
                                                 &port_flags));
                BCM_IF_ERROR_RETURN(
                    bcm_esw_port_vlan_member_set(unit, local_member_array[idx],
                                                 port_flags));
            }
        }
    }
#if defined(BCM_KATANA_SUPPORT)     
    if (soc_feature(unit, soc_feature_extended_queueing)) {
        qtag = soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, EH_QUEUE_TAGf);
        qtag_type = soc_ING_L3_NEXT_HOPm_field32_get(unit,
                                                     &ing_nh, EH_TAG_TYPEf);
        if ((qtag > 0) && (qtag_type == EH_TAG_TYPE_ING_QUEUE_MAP)) {
           /* Update ING_QUEUE_MAP reference count */
           _bcm_tr_mpls_ing_queue_map_ref_count_adjust (unit, qtag, -1);

           /*  ING_QUEUE_MAP needs to be reset */
           BCM_IF_ERROR_RETURN(
               _bcm_tr_mpls_ing_queue_map_index_reset (unit, qtag));
        }
    }
#endif /* BCM_KATANA_SUPPORT */

    /* Clear ING_L3_NEXT_HOP entry */
    sal_memset(&ing_nh, 0, sizeof(ing_l3_next_hop_entry_t));
    BCM_IF_ERROR_RETURN (soc_mem_write (unit, ING_L3_NEXT_HOPm,
                                   MEM_BLOCK_ALL, nh_index, &ing_nh));

    /* Clear INITIAL_ING_L3_NEXT_HOP entry */
    sal_memset(&initial_ing_nh, 0, sizeof(initial_ing_l3_next_hop_entry_t));
    BCM_IF_ERROR_RETURN (soc_mem_write(unit, INITIAL_ING_L3_NEXT_HOPm,
                                   MEM_BLOCK_ALL, nh_index, &initial_ing_nh));

    /* Delete old TPID, MAC, tunnel next-hop and VC_SWAP indexes */
    if (old_vc_swap_idx != -1) {
#if defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit, soc_feature_vc_and_swap_table_overlaid)) {
            BCM_IF_ERROR_RETURN(
                _bcm_tr_mpls_vc_and_swap_hash_nhop_delete(unit,
                                                          old_vc_swap_idx,
                                                          nh_index));
        }
#endif
         _bcm_tr_mpls_vc_and_swap_ref_count_adjust (unit, old_vc_swap_idx, -1);
         BCM_IF_ERROR_RETURN
             (_bcm_tr_mpls_vc_and_swap_table_index_reset (unit, old_vc_swap_idx));
    }
    if (old_macda_idx != -1) {
        rv = _bcm_mac_da_profile_entry_delete(unit, old_macda_idx);
        BCM_IF_ERROR_RETURN(rv);
    }
    if (old_tpid_idx != -1) {
        (void) _bcm_fb2_outer_tpid_entry_delete(unit, old_tpid_idx);
    }
    if (tunnel_if != 0) {
        rv = bcm_xgs3_nh_del(unit, 0, 
                             (tunnel_if - BCM_XGS3_EGRESS_IDX_MIN));
        BCM_IF_ERROR_RETURN(rv);
    }

    BCM_IF_ERROR_RETURN(_bcm_tr_mpls_delete_vp_nh ( unit, nh_index, tunnel_if ));

    /* Clear EGR_L3_NEXT_HOP entry */
    sal_memset(&egr_nh, 0, sizeof(egr_l3_next_hop_entry_t));
    BCM_IF_ERROR_RETURN (soc_mem_write(unit, EGR_L3_NEXT_HOPm,
                                   MEM_BLOCK_ALL, nh_index, &egr_nh));

    /* Free the next-hop entry. */
    rv = bcm_xgs3_nh_del(unit, _BCM_L3_SHR_WRITE_DISABLE, nh_index);
    return rv;
}

/* Translate hardware table index into map ID used by API */
int
_egr_idx2id(int unit, int hw_map_idx)
{
    int num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 64;
    int id;

    for (id = 0; id < num_exp_map; id++) {
        if (_BCM_EGR_MPLS_MAP_USED_GET(unit, id)) {
            if (MPLS_INFO(unit)->egr_mpls_hw_idx[id] == (uint32)hw_map_idx) {
                return id;
            }
        }
    }
    return -1;
}

STATIC int
_bcm_tr_mpls_l2_nh_info_get(int unit, int vp, bcm_mpls_port_t *mpls_port, 
                            int nh_index)
{
    ing_l3_next_hop_entry_t ing_nh;
    egr_l3_next_hop_entry_t egr_nh;
    egr_l3_next_hop_entry_t egr_l3_nh;
    egr_mpls_vc_and_swap_label_table_entry_t vc_entry;
    int vc_swap_idx, action_present, action_not_present;
    bcm_module_t mod_out, mod_in;
    bcm_port_t port_out, port_in;
    bcm_trunk_t trunk_id;
    int ttl = 0, entry_type, ix;
    int network_port_flag = 0, tpid_index = 0;
    bcm_if_t tunnel_if = 0;
    _bcm_vp_info_t vp_info;
#if defined(BCM_KATANA2_SUPPORT)
    bcm_port_t pp_port = 0;
#endif
#if defined(BCM_KATANA2_SUPPORT) | defined(BCM_HGPROXY_COE_SUPPORT)
    int is_local_subport = 0;
#endif
#if defined(BCM_KATANA_SUPPORT)
    ing_queue_map_entry_t ing_queue_entry;
    int qtag, qtag_type;
#endif
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
    int egress_failover_id;
#endif

#if defined(BCM_TOMAHAWK2_SUPPORT)
    int ecn_map_id;
    int ecn_map_hw_index;
    int ecn_map_pri;
#endif

    uint32 base_class_id = 0;
    int base_nh_entry_type = 0;
    int nh_idx = 0;

    /* Read the HW entries */
    BCM_IF_ERROR_RETURN (soc_mem_read(unit, ING_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                                      nh_index, &ing_nh));
    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                                      nh_index, &egr_nh));
    entry_type =  soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, ENTRY_TYPEf);
  
        /* Egressing into an MPLS tunnel, find the tunnel_if */
    if (entry_type == 0x1) {
        mpls_port->flags |= BCM_MPLS_PORT_EGRESS_TUNNEL;
        BCM_IF_ERROR_RETURN(
            bcm_tr_mpls_get_vp_nh(unit, nh_index, &tunnel_if));
        mpls_port->egress_tunnel_if = tunnel_if;
        mpls_port->flags |= BCM_MPLS_PORT_NETWORK;

        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
            if (soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                    MPLS__HG_MODIFY_ENABLEf)) {
                mpls_port->flags2 |= BCM_MPLS_PORT2_PROXY_MODE;
            }
        }
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit, soc_feature_mpls_nh_ttl_control)) {
            if (soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                    MPLS__DISABLE_TTL_DECREMENTf)) {
                mpls_port->flags2 |= BCM_MPLS_PORT2_KEEP_TTL;
            }
        }
#endif
        /* Calculate next hop index. */
        if (BCM_XGS3_L3_EGRESS_IDX_VALID(unit, tunnel_if)) {
            nh_idx = tunnel_if - BCM_XGS3_EGRESS_IDX_MIN;
        } else {
            nh_idx = tunnel_if - BCM_XGS3_DVP_EGRESS_IDX_MIN;
        }

        if ((nh_idx > 0) && (nh_idx < BCM_XGS3_L3_NH_TBL_SIZE(unit))) {

           if (soc_feature(unit, soc_feature_egr_nh_class_id_valid)) {

                BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm,
                                MEM_BLOCK_ANY,
                                nh_idx, &egr_l3_nh));

                base_nh_entry_type = soc_EGR_L3_NEXT_HOPm_field32_get(unit,
                                                             &egr_l3_nh,
                                                             ENTRY_TYPEf);
                if (base_nh_entry_type == 0) {
                    /* Get class id for l3 unicast Nhs.
                     */
                    base_class_id = soc_EGR_L3_NEXT_HOPm_field32_get(unit,
                                                             &egr_l3_nh,
                                                             L3__CLASS_IDf);
                } else if (base_nh_entry_type == 1) {
                   /* Get class id for mpls unicast Nhs.
                     */
                    base_class_id = soc_EGR_L3_NEXT_HOPm_field32_get(unit,
                                                             &egr_l3_nh,
                                                             MPLS__CLASS_IDf);
                }

                mpls_port->egress_class_id = soc_EGR_L3_NEXT_HOPm_field32_get(
                                                             unit, &egr_nh,
                                                             MPLS__CLASS_IDf);


                if (base_class_id == mpls_port->egress_class_id) {
                    /* if l3 nh class id and mpls class id is same
                     *  then the value has come from l3 nh.
                     *  User must had sent invalid class if mpls port.
                     *  We dont allow same class ids in mpls port and base l3 nh.
                     */
                    mpls_port->egress_class_id = BCM_CLASS_ID_INVALID;
                }
            } else {
                mpls_port->egress_class_id = BCM_CLASS_ID_INVALID;
            }
        } else {
            return BCM_E_PARAM;
        }
    } else if (entry_type == 0x2) {
        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
            if (soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                    SD_TAG__HG_MODIFY_ENABLEf)) {
                mpls_port->flags2 |= BCM_MPLS_PORT2_PROXY_MODE;
            }

            if (soc_feature(unit, soc_feature_egr_nh_class_id_valid)) {
                mpls_port->egress_class_id = soc_EGR_L3_NEXT_HOPm_field32_get(
                                                             unit, &egr_nh,
                                                             SD_TAG__CLASS_IDf);
                if (mpls_port->egress_class_id == 0) {
                    mpls_port->egress_class_id = BCM_CLASS_ID_INVALID;
                }

            }
        }
    }

    entry_type = soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, ENTRY_TYPEf);

    if (entry_type == 0x2) {

        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
            if (soc_feature(unit, soc_feature_multiple_split_horizon_group)) {
                mpls_port->network_group_id = soc_EGR_L3_NEXT_HOPm_field32_get(
                                    unit, &egr_nh, MPLS__DVP_NETWORK_GROUPf);
            } else {
                mpls_port->network_group_id = soc_EGR_L3_NEXT_HOPm_field32_get(
                                    unit, &egr_nh, MPLS__DVP_IS_NETWORK_PORTf);
            }
        } else {
            mpls_port->network_group_id = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                             DVP_IS_NETWORK_PORTf);
        }

        BCM_IF_ERROR_RETURN(_bcm_vp_info_get(unit, vp, &vp_info));
        if (vp_info.flags & _BCM_VP_INFO_NETWORK_PORT) {
            network_port_flag = TRUE;
        }

        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
            vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       MPLS__VC_AND_SWAP_INDEXf);
        } else {
            vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       VC_AND_SWAP_INDEXf);
        }

        BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                      MEM_BLOCK_ANY, vc_swap_idx, &vc_entry));

        if (soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                &vc_entry, MPLS_LABEL_ACTIONf)) {
            mpls_port->egress_label.label = 
                soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    &vc_entry, MPLS_LABELf);
            ttl =  soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    &vc_entry, MPLS_TTLf);
            if (ttl) {
                mpls_port->egress_label.ttl = ttl;
                mpls_port->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_TTL_SET;
            } else {
                mpls_port->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_TTL_DECREMENT;
            }
            if (soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    &vc_entry, MPLS_EXP_SELECTf) == 0x0) {
                /* Use the specified EXP, PRI and CFI */
                mpls_port->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_EXP_SET;
                mpls_port->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_PRI_SET;
                mpls_port->egress_label.exp =
                    soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                        &vc_entry, MPLS_EXPf);
                mpls_port->egress_label.pkt_pri =
                    soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                        &vc_entry, NEW_PRIf);
                mpls_port->egress_label.pkt_cfi =
                    soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                        &vc_entry, NEW_CFIf);
            } else if (soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                           &vc_entry, MPLS_EXP_SELECTf) == 0x1) {
                /* Use EXP-map for EXP, PRI and CFI */
                mpls_port->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_EXP_REMARK;
                mpls_port->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_PRI_REMARK;
                ix = soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                        &vc_entry, MPLS_EXP_MAPPING_PTRf);
                BCM_IF_ERROR_RETURN(
                    _egr_qos_hw_idx2id(unit, ix, &mpls_port->egress_label.qos_map_id));

            } else if (soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                           &vc_entry, MPLS_EXP_SELECTf) == 0x2) {
                /* Use EXP from incoming MPLS label. If no incoming label, use the specified
                 * EXP value. Use EXP-map for PRI/CFI.
                 */
                mpls_port->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_EXP_COPY;
                mpls_port->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_PRI_REMARK;
                mpls_port->egress_label.exp =
                    soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                        &vc_entry, MPLS_EXPf);
                ix = soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                         &vc_entry, MPLS_EXP_MAPPING_PTRf);

                BCM_IF_ERROR_RETURN(
                    _egr_qos_hw_idx2id(unit, ix, &mpls_port->egress_label.qos_map_id));
            }
#ifdef BCM_TOMAHAWK2_SUPPORT
            if (soc_feature(unit, soc_feature_mpls_ecn)) {
                int rv;
                ecn_map_hw_index = soc_mem_field32_get(unit, 
                                                       EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                                       &vc_entry, 
                                                       INT_CN_TO_EXP_MAPPING_PTRf);
                rv = bcmi_ecn_map_hw_idx2id(unit, ecn_map_hw_index, 
                                            _BCM_XGS5_MPLS_ECN_MAP_TYPE_INTCN2EXP, 
                                            &ecn_map_id);  
                if (BCM_SUCCESS(rv)) {
                    mpls_port->egress_label.int_cn_map_id = ecn_map_id;
                    ecn_map_pri = soc_mem_field32_get(unit, 
                                                      EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                                      &vc_entry, 
                                                      INT_CN_TO_EXP_PRIORITYf);
                    if (ecn_map_pri) {
                        mpls_port->egress_label.flags |= 
                            BCM_MPLS_EGRESS_LABEL_ECN_EXP_MAP_TRUST;
                    }
                    mpls_port->egress_label.flags |= 
                        BCM_MPLS_EGRESS_LABEL_INT_CN_TO_EXP_MAP;
                }
                ecn_map_hw_index = soc_mem_field32_get(unit, 
                                                       EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                                       &vc_entry, 
                                                       IP_ECN_TO_EXP_MAPPING_PTRf);
                rv = bcmi_ecn_map_hw_idx2id(unit, ecn_map_hw_index, 
                                            _BCM_XGS5_MPLS_ECN_MAP_TYPE_ECN2EXP, 
                                            &ecn_map_id);  
                if (BCM_SUCCESS(rv)) {
                    mpls_port->egress_label.ecn_map_id = ecn_map_id;
                    ecn_map_pri = soc_mem_field32_get(unit, 
                                                      EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                                      &vc_entry, 
                                                      IP_ECN_TO_EXP_PRIORITYf);
                    if (ecn_map_pri) {
                        mpls_port->egress_label.flags |= 
                            BCM_MPLS_EGRESS_LABEL_ECN_EXP_MAP_TRUST;
                    }
                    mpls_port->egress_label.flags |= 
                        BCM_MPLS_EGRESS_LABEL_ECN_TO_EXP_MAP;                    
                }                
            }
#endif /* BCM_TOMAHAWK2_SUPPORT */            
        } else {
            /*Return egress label as zero for default VCSWAP label entry*/
            if (vc_swap_idx == 0) {
                mpls_port->egress_label.label = 0;
            }
        }
        if (soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                &vc_entry, CW_INSERT_FLAGf) == 0x2) {
            mpls_port->flags |= BCM_MPLS_PORT_SEQUENCED;
        } else if (soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                       &vc_entry, CW_INSERT_FLAGf) == 0x1) {
            mpls_port->flags |= BCM_MPLS_PORT_CONTROL_WORD;
        }
        if (soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                &vc_entry, UPDATE_PW_INIT_COUNTERSf)) {
            mpls_port->flags |= BCM_MPLS_PORT_COUNTED;
        }

#if (defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT))
        if (soc_feature(unit, soc_feature_mpls_entropy)) {
             if (soc_mem_field_valid(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    MPLS_INSERT_ENTROPY_LABELf)) {
                  if (soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                &vc_entry, MPLS_INSERT_ENTROPY_LABELf)) {
                        mpls_port->egress_label.flags |= 
                            BCM_MPLS_EGRESS_LABEL_ENTROPY_ENABLE;
                  }
             }
        }
#endif /* defined(BCM_KATANA_SUPPORT) || defined(BCM_APACHE_SUPPORT) */

        if (soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, Tf)) {
            trunk_id = soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, TGIDf);
            BCM_GPORT_TRUNK_SET(mpls_port->port, trunk_id);
        } else {
            mod_in = soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, MODULE_IDf);
            port_in = soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, PORT_NUMf);

#if defined(BCM_HGPROXY_COE_SUPPORT)
            if (soc_feature(unit, soc_feature_hgproxy_subtag_coe)) {

                if(_bcm_xgs5_subport_coe_mod_port_local(unit, mod_in, port_in)) {
                    is_local_subport = 1;
                }

                if (is_local_subport) {
                    /* Encode type and fill in the associated (mod,port) */
                    _BCM_SUBPORT_COE_PORT_ID_SET(mpls_port->port, mod_in, port_in);
                    /* Set the bit on the outgoing param indicating this is subtag type */
                    _BCM_SUBPORT_COE_PORT_TYPE_SET(mpls_port->port, _BCM_SUBPORT_COE_TYPE_SUBTAG);
                }
            }
#endif

#if defined(BCM_KATANA2_SUPPORT)
            if (soc_feature(unit, soc_feature_linkphy_coe) ||
                soc_feature(unit, soc_feature_subtag_coe)) {
                if (_bcm_kt2_mod_is_coe_mod_check(unit, mod_in) == BCM_E_NONE) {
                    BCM_IF_ERROR_RETURN(_bcm_kt2_modport_to_pp_port_get(
                        unit, mod_in, port_in, &pp_port));
                    if (_bcm_kt2_is_pp_port_linkphy_subtag(unit, pp_port)){
                        is_local_subport = 1;
                    }
                }

                if (is_local_subport) {
                    _BCM_KT2_SUBPORT_PORT_ID_SET(mpls_port->port, pp_port);
                    if (BCM_PBMP_MEMBER(SOC_INFO(unit).linkphy_pp_port_pbm,
                        pp_port)) {
                        _BCM_KT2_SUBPORT_PORT_TYPE_SET(mpls_port->port,
                            _BCM_KT2_SUBPORT_TYPE_LINKPHY);
                    } else if (BCM_PBMP_MEMBER(SOC_INFO(unit).subtag_pp_port_pbm,
                        pp_port)) {
                        _BCM_KT2_SUBPORT_PORT_TYPE_SET(mpls_port->port,
                            _BCM_KT2_SUBPORT_TYPE_SUBTAG);
                    } else {
                        return BCM_E_PORT;
                    }
                }
            }
#endif

#if defined(BCM_KATANA2_SUPPORT) || defined(BCM_HGPROXY_COE_SUPPORT)
            if (is_local_subport) {
                /* gport assigned above */
            } else
#endif
            {
                BCM_IF_ERROR_RETURN(_bcm_esw_stk_modmap_map(unit,
                         BCM_STK_MODMAP_GET, mod_in, port_in, &mod_out, &port_out));
                         BCM_GPORT_MODPORT_SET(mpls_port->port, mod_out, port_out);
            }
        }

        if (network_port_flag) {

            action_present =
                soc_EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm_field32_get(unit,
                                             &vc_entry, SD_TAG_ACTION_IF_PRESENTf);
            action_not_present =
                soc_EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm_field32_get(unit,
                                             &vc_entry, SD_TAG_ACTION_IF_NOT_PRESENTf);

            switch (action_present) {

            case 0:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                mpls_port->egress_service_vlan =
                    soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, &vc_entry, SD_TAG_VIDf);
                break;

            case 1:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_TPID_REPLACE;
                mpls_port->egress_service_vlan =
                    soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, &vc_entry, SD_TAG_VIDf);
                break;

            case 2:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_REPLACE;
                mpls_port->egress_service_vlan =
                    soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, &vc_entry, SD_TAG_VIDf);
                break;

            case 3:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_DELETE;
                mpls_port->egress_service_vlan = BCM_VLAN_INVALID;
                break;

            case 4:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_PRI_TPID_REPLACE;
                mpls_port->egress_service_vlan = 
                    soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                        &vc_entry, SD_TAG_VIDf);
                mpls_port->pkt_pri = 
                    soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                        &vc_entry, SD_TAG_NEW_PRIf);
                break;    

            case 5:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_PRI_REPLACE;
                mpls_port->egress_service_vlan = 
                    soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                        &vc_entry, SD_TAG_VIDf);
                mpls_port->pkt_pri = 
                    soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                        &vc_entry, SD_TAG_NEW_PRIf);
                break; 
                
            case 6:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                mpls_port->flags |= BCM_MPLS_PORT_SERVICE_PRI_REPLACE;
                mpls_port->pkt_pri = 
                    soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                        &vc_entry, SD_TAG_NEW_PRIf);
                break; 
                
            case 7:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                mpls_port->flags |= BCM_MPLS_PORT_SERVICE_TPID_REPLACE;
                break; 

            default:
                break;
            }
            if ((mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_ADD) ||
                (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_TPID_REPLACE) ||
                (mpls_port->flags & BCM_MPLS_PORT_SERVICE_TPID_REPLACE) ||
                (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_PRI_TPID_REPLACE)) {
                tpid_index = soc_mem_field32_get(unit,
                                 EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                 &vc_entry, SD_TAG_TPID_INDEXf);
                BCM_IF_ERROR_RETURN(_bcm_fb2_outer_tpid_entry_get(unit,
                                        &mpls_port->service_tpid, tpid_index));
            }
        } else {

             if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                     action_present =
                               soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                                           SD_TAG__SD_TAG_ACTION_IF_PRESENTf);
                     action_not_present = 
                               soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                                           SD_TAG__SD_TAG_ACTION_IF_NOT_PRESENTf);
             } else {
                     action_present =
                               soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                                           SD_TAG_ACTION_IF_PRESENTf);
                     action_not_present =
                               soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                                           SD_TAG_ACTION_IF_NOT_PRESENTf);
              }

            switch (action_present) {

            case 0:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                    mpls_port->egress_service_vlan =
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                                                                SD_TAG__SD_TAG_VIDf);
                } else {
                    mpls_port->egress_service_vlan =
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                                                                SD_TAG_VIDf);
                }
                break;

            case 1:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_TPID_REPLACE;
                if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                    mpls_port->egress_service_vlan =
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                                                                SD_TAG__SD_TAG_VIDf);
                } else {
                    mpls_port->egress_service_vlan =
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                                                                SD_TAG_VIDf);
                }
                break;

            case 2:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_REPLACE;
                if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                    mpls_port->egress_service_vlan =
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                                                                SD_TAG__SD_TAG_VIDf);
                } else {
                    mpls_port->egress_service_vlan =
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh, 
                                                                SD_TAG_VIDf);
                }
                break;

            case 3:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_DELETE;
                mpls_port->egress_service_vlan = BCM_VLAN_INVALID;
                break;

            case 4:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_PRI_TPID_REPLACE;
                if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                    mpls_port->egress_service_vlan =
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                            SD_TAG__SD_TAG_VIDf);
                    if(soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm,
                       SD_TAG__NEW_PRIf)) {
                        mpls_port->pkt_pri =
                            soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                                SD_TAG__NEW_PRIf);
                    }
                    if(soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm,
                       SD_TAG__NEW_CFIf)) {
                        mpls_port->pkt_cfi =
                            soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                                SD_TAG__NEW_CFIf);
                    }
                }
                break;    

            case 5:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_PRI_REPLACE;
                if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                    mpls_port->egress_service_vlan =
                        soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                            SD_TAG__SD_TAG_VIDf);
                    if(soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm,
                       SD_TAG__NEW_PRIf)) {
                        mpls_port->pkt_pri =
                            soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                                SD_TAG__NEW_PRIf);
                    }
                    if(soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm,
                       SD_TAG__NEW_CFIf)) {
                        mpls_port->pkt_cfi =
                            soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                                SD_TAG__NEW_CFIf);
                    }
                }
                break; 
                
            case 6:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                mpls_port->flags |= BCM_MPLS_PORT_SERVICE_PRI_REPLACE;
                if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                    if(soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm,
                        SD_TAG__NEW_PRIf)) {
                        mpls_port->pkt_pri =
                            soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                                SD_TAG__NEW_PRIf);
                    }
                    if(soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm,
                       SD_TAG__NEW_CFIf)) {
                        mpls_port->pkt_cfi =
                            soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                                SD_TAG__NEW_CFIf);
                    }
                }
                break; 
                
            case 7:
                if (action_not_present == 1) {
                    mpls_port->flags |= BCM_MPLS_PORT_SERVICE_VLAN_ADD;
                }
                mpls_port->flags |= BCM_MPLS_PORT_SERVICE_TPID_REPLACE;
                break; 

            default:
                break;
            }
            if ((mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_ADD) ||
                (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_TPID_REPLACE) ||
                (mpls_port->flags & BCM_MPLS_PORT_SERVICE_TPID_REPLACE) ||
                (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_PRI_TPID_REPLACE)) {
                 if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                     tpid_index = soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm,
                                      &egr_nh, SD_TAG__SD_TAG_TPID_INDEXf);
                 } else {
                     tpid_index = soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm,
                                      &egr_nh, SD_TAG_TPID_INDEXf);
                 }
                 BCM_IF_ERROR_RETURN(_bcm_fb2_outer_tpid_entry_get(unit,
                                         &mpls_port->service_tpid, tpid_index));
            }
        }

        if (soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, DROPf)) {
            mpls_port->flags |= BCM_MPLS_PORT_DROP;
        }
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
       if (SOC_IS_TRIUMPH3(unit) && soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm,
           MPLS__DO_EVXLT_AFTER_TUNNEL_ENCAPf)) {
           if (soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
               MPLS__DO_EVXLT_AFTER_TUNNEL_ENCAPf)) {
               mpls_port->flags2 |= BCM_MPLS_PORT2_TUNNEL_HEADER_EGRESS_VLAN_TRANSLATION;
           }
       }
    }
#endif
#if defined(BCM_KATANA_SUPPORT)
        if (soc_feature(unit, soc_feature_extended_queueing)) {
            qtag = soc_ING_L3_NEXT_HOPm_field32_get(unit,
                                                 &ing_nh, EH_QUEUE_TAGf);
            qtag_type = soc_ING_L3_NEXT_HOPm_field32_get(unit,
                                                 &ing_nh, EH_TAG_TYPEf);
            if (qtag_type == EH_TAG_TYPE_ING_QUEUE_MAP) {
                sal_memset(&ing_queue_entry, 0, sizeof(ing_queue_map_entry_t));
                BCM_IF_ERROR_RETURN (soc_mem_read(unit, ING_QUEUE_MAPm,
                                                 MEM_BLOCK_ANY,
                                                 qtag, &ing_queue_entry));
                mpls_port->qos_map_id = soc_ING_QUEUE_MAPm_field32_get(unit,
                               &ing_queue_entry, QUEUE_OFFSET_PROFILE_INDEXf);
                mpls_port->qos_map_id |=
                    (_BCM_QOS_MAP_TYPE_ING_QUEUE_OFFSET_MAP_TABLE
                                                    << _BCM_QOS_MAP_SHIFT);
            }
            if (qtag_type > EH_TAG_TYPE_NONE) {
                /* We need to have a cosq function which returns the gport for 
                   the queue. Currently, creating the gport of type 
                   subscriber queue locally */
#ifdef BCM_KATANA2_SUPPORT
                if (SOC_IS_KATANA2(unit)) {
                    bcm_kt2_cosq_subscriber_qid_set(unit, &qtag, qtag);
                } else
#endif
                {
                    BCM_GPORT_UCAST_SUBSCRIBER_QUEUE_GROUP_QID_SET(qtag, qtag);
                }
                qtag |= soc_ING_L3_NEXT_HOPm_field32_get(unit,
                                                    &ing_nh, PORT_NUMf) << 16;
                mpls_port->per_flow_queue_base = qtag;
            }
        }
#endif /* BCM_KATANA_SUPPORT */        
#ifdef BCM_TRIDENT2PLUS_SUPPORT
        if (soc_feature(unit, soc_feature_egress_failover)) {
            egress_failover_id = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, MPLS__PROT_GROUPf);
            if(egress_failover_id > 0)
                mpls_port->egress_failover_id = egress_failover_id;
        }
#endif
    } else {
        return BCM_E_NOT_FOUND;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_tr_mpls_match_trunk_idx_get
 * Purpose:
 *      Obtain Trunk_Idx
 * Parameters:
 *      unit    - (IN) Device Number
 *      mod_id - (IN) Module Id
 *      port_id  - (IN) Port Id
 *      src_trk_idx - (OUT) trunk_idx
 * Returns:
 *      BCM_E_XXX
 */

STATIC int
_bcm_tr_mpls_match_trunk_idx_get(int unit,
                                  bcm_module_t mod_id,
                                  bcm_port_t port_id,
                                  int *src_trk_idx)
{
    int num_bits_for_port = 0;

#if defined(BCM_KATANA2_SUPPORT)
    if (soc_feature(unit, soc_feature_linkphy_coe) ||
        soc_feature(unit, soc_feature_subtag_coe)) {
            BCM_IF_ERROR_RETURN(_bcm_kt2_pp_port_to_modport_get(
                unit, port_id, &mod_id, &port_id));
    }
#endif

#if defined(BCM_TRIDENT_SUPPORT)
    if (soc_feature(unit, soc_feature_src_modid_base_index)) {
        source_trunk_map_modbase_entry_t modbase_entry;

         BCM_IF_ERROR_RETURN
              (soc_mem_read(unit, SOURCE_TRUNK_MAP_MODBASEm, 
                             MEM_BLOCK_ANY, mod_id, &modbase_entry));
         *src_trk_idx =	soc_mem_field32_get(unit, 
                                       SOURCE_TRUNK_MAP_MODBASEm,
                                       &modbase_entry, BASEf) + port_id;
    } else 
#endif
    { 
         num_bits_for_port = 
                   _shr_popcount((unsigned int)SOC_PORT_ADDR_MAX(unit));
         *src_trk_idx = (mod_id << num_bits_for_port) | port_id;
    }
    return BCM_E_NONE;
}

int
bcm_tr_mpls_trunk_member_add(int unit, bcm_trunk_t trunk_id, 
        int trunk_member_count, bcm_port_t *trunk_member_array)
{
    int rv = BCM_E_NONE;
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int i, my_modid, stm_idx, vp;

    rv = _bcm_trunk_id_validate(unit, trunk_id);
    if (BCM_FAILURE(rv)) {
        return BCM_E_PORT;
    }
    if (!mpls_info->initialized) {
        LOG_ERROR(BSL_LS_BCM_MPLS,
                  (BSL_META_U(unit,
                              "MPLS module not initialized. \n")));
        return BCM_E_INIT;
    }

    vp = mpls_info->trunk_vp_map[trunk_id];
    if (vp != -1) {
        BCM_IF_ERROR_RETURN(
            bcm_esw_stk_my_modid_get(unit, &my_modid));
        /* Get index to source trunk map table */
        
        for (i = 0; i < trunk_member_count; i++) {
            BCM_IF_ERROR_RETURN
                (_bcm_esw_src_mod_port_table_index_get(unit, my_modid, 
                                          trunk_member_array[i], &stm_idx));
            BCM_IF_ERROR_RETURN
                (soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm,
                                        stm_idx, SOURCE_VPf, vp));
            if (SOC_MEM_FIELD_VALID(unit,SOURCE_TRUNK_MAP_TABLEm,SVP_VALIDf)) {
                BCM_IF_ERROR_RETURN
                    (soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm,
                                            stm_idx, SVP_VALIDf, 1));
            }
            BCM_IF_ERROR_RETURN
                (_bcm_esw_port_tab_set(unit, trunk_member_array[i], 
                                       _BCM_CPU_TABS_NONE,
                                       PORT_OPERATIONf, 0x1)); /* L2_SVP */
        }
    }

    return BCM_E_NONE;
}

int
bcm_tr_mpls_trunk_member_delete(int unit, bcm_trunk_t trunk_id, 
        int trunk_member_count, bcm_port_t *trunk_member_array)
{
    int rv = BCM_E_NONE;
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int i, my_modid, stm_idx, vp;

    rv = _bcm_trunk_id_validate(unit, trunk_id);
    if (BCM_FAILURE(rv)) {
        return BCM_E_PORT;
    }

    vp = mpls_info->trunk_vp_map[trunk_id];
    if (vp != -1) {
        BCM_IF_ERROR_RETURN(
            bcm_esw_stk_my_modid_get(unit, &my_modid));
        /* Get index to source trunk map table */
        
        for (i = 0; i < trunk_member_count; i++) {
            BCM_IF_ERROR_RETURN
                (_bcm_esw_src_mod_port_table_index_get(unit, my_modid, 
                                          trunk_member_array[i], &stm_idx));
            BCM_IF_ERROR_RETURN
                (soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm,
                                        stm_idx, SOURCE_VPf, 0));
            if (SOC_MEM_FIELD_VALID(unit,SOURCE_TRUNK_MAP_TABLEm,SVP_VALIDf)) {
                BCM_IF_ERROR_RETURN
                    (soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm,
                                            stm_idx, SVP_VALIDf, 0));
            }
            BCM_IF_ERROR_RETURN
                (_bcm_esw_port_tab_set(unit, trunk_member_array[i], 
                                       _BCM_CPU_TABS_NONE,
                                       PORT_OPERATIONf, 0)); /* L2_SVP */
        }
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_tr_mpls_match_trunk_add
 * Purpose:
 *      Assign SVP of an MPLS Trunk port
 * Parameters:
 *      unit    - (IN) Device Number
 *      tgid - (IN) Trunk group Id
 *      vp  - (IN) Source Virtual Port
 * Returns:
 *      BCM_E_XXX
 */

int
 bcm_tr_mpls_match_trunk_add(int unit, bcm_trunk_t tgid, int vp)
{
    int port_idx;
    bcm_module_t my_modid;
#if defined(BCM_ESW_SUPPORT) && (SOC_MAX_NUM_PP_PORTS > SOC_MAX_NUM_PORTS)
    int max_num_port = SOC_MAX_NUM_PP_PORTS;
#else
    int max_num_port = SOC_MAX_NUM_PORTS;
#endif
    int * local_src_trk_idx = NULL;
    int * local_port_out = NULL;
    int local_member_count;
    int rv = BCM_E_NONE;
    int src_trk_idx = -1;
    bcm_port_t port_out;

    local_src_trk_idx = sal_alloc(sizeof(int) * max_num_port, "local source Trunk index");
    if (NULL == local_src_trk_idx) {
        return BCM_E_MEMORY;
    }
    sal_memset(local_src_trk_idx, 0, sizeof(int) * max_num_port);
    local_port_out = sal_alloc(sizeof(int) * max_num_port, "Local port out");
    if (NULL == local_port_out) {
        sal_free(local_src_trk_idx);
        return BCM_E_MEMORY;
    }
    sal_memset(local_port_out, 0, sizeof(int) * max_num_port);
    rv = bcm_esw_stk_my_modid_get(unit, &my_modid);
    if (BCM_FAILURE(rv)) {
        sal_free(local_src_trk_idx);
        sal_free(local_port_out);
        return rv;
    }
    rv = _bcm_esw_trunk_local_members_get(unit, tgid,
                max_num_port, local_port_out, &local_member_count);
    if (BCM_FAILURE(rv)) {
        sal_free(local_src_trk_idx);
        sal_free(local_port_out);
        return rv;
    }
    for (port_idx = 0; port_idx < local_member_count; port_idx++) {
        rv = _bcm_tr_mpls_match_trunk_idx_get(unit,
                    my_modid, local_port_out[port_idx], &src_trk_idx);
        if (BCM_FAILURE(rv)) {
            sal_free(local_src_trk_idx);
            sal_free(local_port_out);
            return rv;
        }
        local_src_trk_idx[port_idx] = src_trk_idx;

        rv = soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm,
                                 src_trk_idx, SOURCE_VPf, vp);
        if (BCM_FAILURE(rv)) {
             goto trunk_cleanup;
        }
        if (SOC_MEM_FIELD_VALID(unit,SOURCE_TRUNK_MAP_TABLEm,SVP_VALIDf)) {
            rv = soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm,
                                 src_trk_idx, SVP_VALIDf, 1);
            if (BCM_FAILURE(rv)) {
                goto trunk_cleanup;
            }
        }
        rv = _bcm_esw_port_tab_set_without_portlock(
                 unit, local_port_out[port_idx], _BCM_CPU_TABS_NONE,
                 PORT_OPERATIONf, 0x1); /* L2_SVP */
        if (BCM_FAILURE(rv)) {
            goto trunk_cleanup;
        }
 
        /* Set TAG_ACTION_PROFILE_PTR */ 
        rv = bcm_tr_mpls_port_untagged_profile_set(unit, local_port_out[port_idx]); 
        if (BCM_FAILURE(rv)) { 
            goto trunk_cleanup; 
        } 
    }
    sal_free(local_src_trk_idx);
    sal_free(local_port_out);
    return BCM_E_NONE;

 trunk_cleanup:
    for (;port_idx >= 0; port_idx--) {
        src_trk_idx = local_src_trk_idx[port_idx];
        port_out = local_port_out[port_idx];
        rv = soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm,
                                 src_trk_idx, SOURCE_VPf, 0);
        if (rv < 0) {
            sal_free(local_src_trk_idx);
            sal_free(local_port_out);
            return rv;
        }
        if (SOC_MEM_FIELD_VALID(unit,SOURCE_TRUNK_MAP_TABLEm,SVP_VALIDf)) {
            rv = soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm,
                                 src_trk_idx, SVP_VALIDf, 0);
        }
        if (BCM_SUCCESS(rv)) {
            rv = _bcm_esw_port_tab_set_without_portlock(
                     unit, port_out, _BCM_CPU_TABS_NONE, PORT_OPERATIONf, 0);
        }
    }
    sal_free(local_src_trk_idx);
    sal_free(local_port_out);
    return rv;
}

/*
 * Function:
 *      bcm_tr_mpls_match_trunk_delete
 * Purpose:
 *      Remove SVP of an MPLS Trunk port
 * Parameters:
 *      unit    - (IN) Device Number
 *      tgid - (IN) Trunk group Id
 *      vp  - (IN) Source Virtual Port
 * Returns:
 *      BCM_E_XXX
 */

int
bcm_tr_mpls_match_trunk_delete(int unit, bcm_trunk_t tgid, int vp)
{
    int port_idx;
    bcm_module_t my_modid;
#if defined(BCM_ESW_SUPPORT) && (SOC_MAX_NUM_PP_PORTS > SOC_MAX_NUM_PORTS)
    int max_num_port = SOC_MAX_NUM_PP_PORTS;
    int local_src_trk_idx[SOC_MAX_NUM_PP_PORTS];
    int local_port_out[SOC_MAX_NUM_PP_PORTS];
#else
    int max_num_port = SOC_MAX_NUM_PORTS;
    int local_src_trk_idx[SOC_MAX_NUM_PORTS];
    int local_port_out[SOC_MAX_NUM_PORTS];
#endif
    int local_member_count;
    int rv = BCM_E_NONE;
    int src_trk_idx = -1;
    bcm_port_t port_out;

    BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &my_modid));

    BCM_IF_ERROR_RETURN(_bcm_esw_trunk_local_members_get(unit, tgid,
                max_num_port, local_port_out, &local_member_count));

    for (port_idx = 0; port_idx < local_member_count; port_idx++) {
        BCM_IF_ERROR_RETURN(_bcm_tr_mpls_match_trunk_idx_get(unit,
                    my_modid, local_port_out[port_idx], &src_trk_idx));
        local_src_trk_idx[port_idx] = src_trk_idx;

        rv = soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm,
                                 src_trk_idx, SOURCE_VPf, 0);
        if (BCM_FAILURE(rv)) {
             goto trunk_cleanup;
        }
        if (SOC_MEM_FIELD_VALID(unit,SOURCE_TRUNK_MAP_TABLEm,SVP_VALIDf)) {
            rv = soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm,
                                 src_trk_idx, SVP_VALIDf, 0);
            if (BCM_FAILURE(rv)) {
                goto trunk_cleanup;
            }
        }
        rv = _bcm_esw_port_tab_set_without_portlock(
                 unit, local_port_out[port_idx], _BCM_CPU_TABS_NONE,
                 PORT_OPERATIONf, 0x0); /* L2_SVP */
        if (BCM_FAILURE(rv)) {
             goto trunk_cleanup;
        }

         /* Reset TAG_ACTION_PROFILE_PTR */
         /* coverity[stack_use_callee] */
         /* coverity[stack_use_overflow] */
         rv = bcm_tr_mpls_port_untagged_profile_reset(unit, local_port_out[port_idx]);
         BCM_IF_ERROR_RETURN(rv);
    }

    return BCM_E_NONE;

 trunk_cleanup:
    for (;port_idx >= 0; port_idx--) {
        src_trk_idx = local_src_trk_idx[port_idx];
        port_out = local_port_out[port_idx];
        (void)soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm,
                                 src_trk_idx, SOURCE_VPf, vp);
        if (SOC_MEM_FIELD_VALID(unit,SOURCE_TRUNK_MAP_TABLEm,SVP_VALIDf)) {
            (void)soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm,
                                 src_trk_idx, SVP_VALIDf, 1);
        }
        (void)_bcm_esw_port_tab_set_without_portlock(unit, port_out,
                                                     _BCM_CPU_TABS_NONE,
                                                     PORT_OPERATIONf, 0x1);
    }
    return rv;
}

/*
 * Function:
 *		_bcm_tr_mpls_match_vlan_entry_update
 * Purpose:
 *		Update MPLS Match Vlan_xlate Entry 
 * Parameters:
 *  IN :  Unit
 *  IN :  vlan_xlate_entry
 *  OUT :  vlan_xlate_entry

 */

STATIC int
_bcm_tr_mpls_match_vlan_entry_update(int unit, bcm_mpls_port_t *mpls_port,
        vlan_xlate_entry_t *vent, vlan_xlate_entry_t *return_ent)
{
    uint32  vp, key_type, value;


    /* Check if Key_Type identical */
    key_type = soc_VLAN_XLATEm_field32_get (unit, vent, KEY_TYPEf);
    value = soc_VLAN_XLATEm_field32_get (unit, return_ent, KEY_TYPEf);
    if (key_type != value) {
         return BCM_E_PARAM;
    }

    value = soc_VLAN_XLATEm_field32_get(unit, return_ent, MPLS_ACTIONf);
    /* SVP_VALID and L3_IIF_VALID overlay high and low bits of MPLS_ACTION. */
    if (SOC_MEM_FIELD_VALID(unit, VLAN_XLATEm, L3_IIF_VALIDf)) {
        value |= 0x1;
    } else {
        value = 0x1;
    }
    /* Retain original Keys -- Update data only */
    soc_VLAN_XLATEm_field32_set(unit, return_ent, MPLS_ACTIONf, value);
    if (!(mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
        soc_VLAN_XLATEm_field32_set(unit, return_ent, DISABLE_VLAN_CHECKSf, 1);
    }
    vp = soc_VLAN_XLATEm_field32_get (unit, vent, SOURCE_VPf);
    soc_VLAN_XLATEm_field32_set(unit, return_ent, SOURCE_VPf, vp);

   return BCM_E_NONE;
}


/*
 * Function:
 *		_bcm_tr_mpls_match_label_entry_update
 * Purpose:
 *		Update Match MPLS Entry 
 * Parameters:
 *  IN :  Unit
 *  IN :  mpls_entry
 *  OUT :  mpls_entry

 */

STATIC void
_bcm_tr_mpls_match_label_entry_update(int unit, bcm_mpls_port_t *mpls_port, 
                                                                     mpls_entry_entry_t *ment, 
                                                                     mpls_entry_entry_t *return_ment)
{
    uint32 value;
    bcm_module_t mod_id;
    bcm_port_t port_id;
    bcm_trunk_t trunk_id;
    bcm_mpls_label_t match_label;

    value = soc_MPLS_ENTRYm_field32_get (unit, ment, Tf);
    if (value) {
         soc_MPLS_ENTRYm_field32_set(unit, return_ment, Tf, value);
         trunk_id = soc_MPLS_ENTRYm_field32_get (unit, ment, TGIDf);
         soc_MPLS_ENTRYm_field32_set(unit, return_ment, TGIDf, trunk_id);
    } else {
         mod_id = soc_MPLS_ENTRYm_field32_get (unit, ment, MODULE_IDf);
         soc_MPLS_ENTRYm_field32_set(unit, return_ment, MODULE_IDf, mod_id);
         port_id = soc_MPLS_ENTRYm_field32_get (unit, ment, PORT_NUMf);
         soc_MPLS_ENTRYm_field32_set(unit, return_ment, PORT_NUMf, port_id);
    }

    match_label = soc_MPLS_ENTRYm_field32_get(unit, ment, MPLS_LABELf);
    soc_MPLS_ENTRYm_field32_set(unit, return_ment, MPLS_LABELf, match_label);
    soc_MPLS_ENTRYm_field32_set(unit, return_ment, VALIDf, 1);
    value = soc_MPLS_ENTRYm_field32_get(unit, ment, SOURCE_VPf);
    soc_MPLS_ENTRYm_field32_set(unit, return_ment, SOURCE_VPf, value);
    soc_MPLS_ENTRYm_field32_set(unit, return_ment, MPLS_ACTION_IF_BOSf, 0x1); /* L2 SVP */
    soc_MPLS_ENTRYm_field32_set(unit, return_ment, MPLS_ACTION_IF_NOT_BOSf, 0x0);
    value = soc_MPLS_ENTRYm_field32_get(unit, ment, CW_CHECK_CTRLf);
    soc_MPLS_ENTRYm_field32_set(unit, return_ment, CW_CHECK_CTRLf, value);
    value = soc_MPLS_ENTRYm_field32_get(unit, ment, PW_TERM_NUM_VALIDf);
    soc_MPLS_ENTRYm_field32_set(unit, return_ment, PW_TERM_NUM_VALIDf, value);
    if(value) {
        value = soc_MPLS_ENTRYm_field32_get(unit, ment, PW_TERM_NUMf);
        soc_MPLS_ENTRYm_field32_set(unit, return_ment, PW_TERM_NUMf, value);
    } 
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
    else {
        if ((soc_feature(unit, soc_feature_mpls_enhanced)) &&
             (soc_feature(unit,soc_feature_advanced_flex_counter))) {
             value = soc_MPLS_ENTRYm_field32_get(unit, ment, MPLS__FLEX_CTR_POOL_NUMBERf);
             soc_MPLS_ENTRYm_field32_set(unit, return_ment, MPLS__FLEX_CTR_POOL_NUMBERf, value);
             value = soc_MPLS_ENTRYm_field32_get(unit, ment, MPLS__FLEX_CTR_BASE_COUNTER_IDXf);
             soc_MPLS_ENTRYm_field32_set(unit, return_ment, MPLS__FLEX_CTR_BASE_COUNTER_IDXf, value);
             value = soc_MPLS_ENTRYm_field32_get(unit, ment, MPLS__FLEX_CTR_OFFSET_MODEf);
             soc_MPLS_ENTRYm_field32_set(unit, return_ment, MPLS__FLEX_CTR_OFFSET_MODEf, value);
        }
    }
#endif

    value = soc_MPLS_ENTRYm_field32_get(unit, ment, DECAP_USE_EXP_FOR_PRIf);
    soc_MPLS_ENTRYm_field32_set(unit, return_ment, DECAP_USE_EXP_FOR_PRIf, 
                                                           value);
    value = soc_MPLS_ENTRYm_field32_get(unit, ment, EXP_MAPPING_PTRf);
    soc_MPLS_ENTRYm_field32_set(unit, return_ment, EXP_MAPPING_PTRf, value);
    value = soc_MPLS_ENTRYm_field32_get(unit, ment, NEW_PRIf);
    soc_MPLS_ENTRYm_field32_set(unit, return_ment, NEW_PRIf, value);
    if (soc_mem_field_valid(unit, MPLS_ENTRYm, TRUST_OUTER_DOT1P_PTRf)) {
         value = soc_MPLS_ENTRYm_field32_get(unit, ment, TRUST_OUTER_DOT1P_PTRf);
         soc_MPLS_ENTRYm_field32_set(unit, return_ment, TRUST_OUTER_DOT1P_PTRf, 
                                                           value);
    }
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
        if (soc_feature(unit, soc_feature_mpls_entropy)) {
            if ((mpls_port->flags & BCM_MPLS_PORT_ENTROPY_ENABLE) ||
                (mpls_port->flags2 & BCM_MPLS_PORT2_INGRESS_ENTROPY_ENABLE)) {
                if (soc_mem_field_valid(unit, MPLS_ENTRYm,
                                    ENTROPY_LABEL_PRESENTf)) {
                     soc_MPLS_ENTRYm_field32_set(unit, return_ment,
                                            ENTROPY_LABEL_PRESENTf, 0x1);
                     soc_MPLS_ENTRYm_field32_set(unit, return_ment, MPLS_ACTION_IF_BOSf,
                                    0x0); /* INVALID */
                     if (SOC_IS_APACHE(unit)) {
                         soc_MPLS_ENTRYm_field32_set(unit, return_ment, MPLS_ACTION_IF_NOT_BOSf,
                                 0x6); /* POP_SVP_ENTROPY */
                     } else {
                         soc_MPLS_ENTRYm_field32_set(unit, return_ment, MPLS_ACTION_IF_NOT_BOSf,
                                 0x4); /* POP_SVP_ENTROPY */
                     }
                }
            } else if (soc_mem_field_valid(unit, MPLS_ENTRYm,
                                    ENTROPY_LABEL_PRESENTf)) {
                soc_MPLS_ENTRYm_field32_set(unit, return_ment,
                                    ENTROPY_LABEL_PRESENTf, 0x0);
            }
        }
#endif
#if defined(BCM_TOMAHAWK2_SUPPORT)
    if (soc_feature(unit, soc_feature_mpls_ecn)) {
         value = soc_MPLS_ENTRYm_field32_get(unit, ment, MPLS__EXP_TO_IP_ECN_MAPPING_PTRf);
         soc_MPLS_ENTRYm_field32_set(unit, return_ment, MPLS__EXP_TO_IP_ECN_MAPPING_PTRf, value);
    }
#endif
   return;
}

/*
 * Function:
 *           _bcm_tr_mpls_vpless_failover_nh_index_find
 * Purpose:
 *           Traverse the EGR_L3_NEXT_HOP table to find next hop 
 *           index for the failover mpls port of the same VP(vpless mpls port). 
 * Parameters:
 *           IN :  Unit
 *           IN :  vp - the same vp for primary and vpless failover mpls ports 
 *           IN :  primary_nh_index - primary mpls port next hop index 
 *          OUT :  fo_nh_index - the failover mpls port next hop index
 * Returns:
 *      BCM_E_XXX
 */
int
_bcm_tr_mpls_vpless_failover_nh_index_find (int unit, int vp, 
                            int primary_nh_index, int *fo_nh_index)
{
    /* Indexes to iterate over memories, chunks and entries */
    int             chnk_idx, ent_idx, chnk_idx_max, mem_idx_max;
    int             buf_size, chunksize, chnk_end;
    uint32          *tbl_chnk;
    egr_l3_next_hop_entry_t *egr_nh;
    int             rv = BCM_E_NONE;
    soc_mem_t mem;
    int dvp;
    int entry_type;

    *fo_nh_index = 0;
    mem = EGR_L3_NEXT_HOPm;
    chunksize = 0x400; /* 1k */
    buf_size = sizeof(egr_l3_next_hop_entry_t) * chunksize;
    tbl_chnk = soc_cm_salloc(unit, buf_size, "egr_l3_next_hop traverse");
    if (NULL == tbl_chnk) {
        return BCM_E_MEMORY;
    }

    mem_idx_max = soc_mem_index_max(unit, mem);
    for (chnk_idx = soc_mem_index_min(unit, mem);
         chnk_idx <= mem_idx_max;
         chnk_idx += chunksize) {
        sal_memset((void *)tbl_chnk, 0, buf_size);

        chnk_idx_max =
            ((chnk_idx + chunksize) <= mem_idx_max) ?
            chnk_idx + chunksize - 1: mem_idx_max;

        soc_mem_lock(unit, mem);
        rv = soc_mem_read_range(unit, mem, MEM_BLOCK_ANY,
                                chnk_idx, chnk_idx_max, tbl_chnk);
        if (SOC_FAILURE(rv)) {
            soc_mem_unlock(unit, mem);
            soc_cm_sfree(unit, tbl_chnk);
            return rv;
        }
        chnk_end = (chnk_idx_max - chnk_idx);
        for (ent_idx = 0 ; ent_idx <= chnk_end; ent_idx ++) {
            egr_nh = soc_mem_table_idx_to_pointer(unit, mem, 
                                             egr_l3_next_hop_entry_t *,
                                             tbl_chnk, ent_idx);

            entry_type = soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, 
                                            egr_nh, ENTRY_TYPEf);
            if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                dvp = soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm,
                        egr_nh, MPLS__DVPf);
            } else {
                dvp = soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm,
                        egr_nh, DVPf);
            }

            if ((entry_type == 1) && 
                (dvp == vp) && ((ent_idx+chnk_idx) != primary_nh_index)) {
                *fo_nh_index = ent_idx+chnk_idx;
                soc_mem_unlock(unit, mem);
                soc_cm_sfree(unit, tbl_chnk);
                return BCM_E_NONE;
            }  
        }
        soc_mem_unlock(unit, mem);
    }
    soc_cm_sfree(unit, tbl_chnk);
    return BCM_E_NOT_FOUND;
}

/*
 * Function:
 *        _bcm_tr_mpls_match_vlan_entry_set
 * Purpose:
 *       Set MPLS Match Vlan Entry 
 * Parameters:
 *  IN :  Unit
 *  IN :  mpls_port
 *  IN :  vlan_xlate_entry
 * Returns:
 *      BCM_E_XXX
 */

STATIC int
_bcm_tr_mpls_match_vlan_entry_set(int unit, bcm_mpls_port_t *mpls_port, vlan_xlate_entry_t *vent)
{
    vlan_xlate_entry_t return_vent;
    int rv = BCM_E_UNAVAIL;
    int index;

    rv = soc_mem_search(unit, VLAN_XLATEm, MEM_BLOCK_ANY, &index,
                        vent, &return_vent, 0);

    if (rv == SOC_E_NONE) {
         BCM_IF_ERROR_RETURN(
              _bcm_tr_mpls_match_vlan_entry_update (unit, mpls_port, vent, &return_vent));
         rv = soc_mem_write(unit, VLAN_XLATEm,
                                           MEM_BLOCK_ALL, index,
                                           &return_vent);
    } else if (rv != SOC_E_NOT_FOUND) {
        return rv;
    } else {
         if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
             return BCM_E_NOT_FOUND;
         } else {
              rv = soc_mem_insert(unit, VLAN_XLATEm, MEM_BLOCK_ALL, vent);
         }
    }

    return rv;
}

/*
 * Function:
 *		_bcm_tr_mpls_match_label_entry_set
 * Purpose:
 *		Set MPLS Match Label Entry 
 * Parameters:
 *  IN :  Unit
 *  IN :  mpls_port
 *  IN :  mpls_entry
 * Returns:
 *		BCM_E_XXX
 */

STATIC int
_bcm_tr_mpls_match_label_entry_set(int unit, bcm_mpls_port_t *mpls_port, mpls_entry_entry_t *ment)
{
    mpls_entry_entry_t return_ment;
    int rv = BCM_E_UNAVAIL;
    int index;

    rv = soc_mem_search(unit, MPLS_ENTRYm, MEM_BLOCK_ANY, &index,
                        ment, &return_ment, 0);

    if (rv == SOC_E_NONE) {
         (void) _bcm_tr_mpls_match_label_entry_update (unit, mpls_port, ment, &return_ment);
         rv = soc_mem_write(unit, MPLS_ENTRYm,
                                           MEM_BLOCK_ALL, index,
                                           &return_ment);
    } else if (rv != SOC_E_NOT_FOUND) {
        return rv;
    } else {
             rv = soc_mem_insert(unit, MPLS_ENTRYm, MEM_BLOCK_ALL, ment);
    }

    return rv;
}

/*
 * Function:
 *      _bcm_tr_mpls_match_clear
 * Purpose:
 *      Clear MPLS Match Software State
 * Parameters:
 *      unit    - (IN) Device Number
 *      vp  - (IN) Source Virtual Port
 * Returns:
 *      void
 */

void
bcm_tr_mpls_match_clear (int unit, int vp)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);

    mpls_info->match_key[vp].flags = 0;
    mpls_info->match_key[vp].match_vlan = 0;
    mpls_info->match_key[vp].match_inner_vlan = 0;
    mpls_info->match_key[vp].match_label = 0;
    mpls_info->match_key[vp].trunk_id = -1;
    mpls_info->match_key[vp].port = 0;
    mpls_info->match_key[vp].modid = -1;

    return;
}

/*
 * Function:
 *		bcm_tr_mpls_entry_internal_qos_set
 * Purpose:
 *		Set MPLS Entry QoS parameters
 * Parameters:
 *  IN :  Unit
 *  IN :  mpls_port
 *  IN :  mpls_entry
 * Returns:
 *		BCM_E_XXX
 */

void
bcm_tr_mpls_entry_internal_qos_set(int unit, bcm_mpls_port_t *mpls_port, 
                                                                          bcm_mpls_tunnel_switch_t  *info, mpls_entry_entry_t *ment)
{
    if (mpls_port != NULL ) {
        if (mpls_port->flags & BCM_MPLS_PORT_INT_PRI_MAP) {
            soc_MPLS_ENTRYm_field32_set(unit, ment, DECAP_USE_EXP_FOR_PRIf,
                                        0x1); /* MAP_PRI_AND_CNG */
            /* Use specified EXP-map to determine internal prio/color */
            soc_MPLS_ENTRYm_field32_set(unit, ment, EXP_MAPPING_PTRf,
                mpls_port->exp_map & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK);
        } else if ((mpls_port->flags & BCM_MPLS_PORT_INT_PRI_SET) &&
                   (mpls_port->flags & BCM_MPLS_PORT_COLOR_MAP)) {
            soc_MPLS_ENTRYm_field32_set(unit, ment, DECAP_USE_EXP_FOR_PRIf,
                                        0x2); /* NEW_PRI_MAP_CNG */
            /* Use the specified internal priority value */
            soc_MPLS_ENTRYm_field32_set(unit, ment, NEW_PRIf,
                                        mpls_port->int_pri);
            /* Use specified EXP-map to determine internal color */
            soc_MPLS_ENTRYm_field32_set(unit, ment, EXP_MAPPING_PTRf,
                mpls_port->exp_map & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK);
        } else if (mpls_port->flags & BCM_MPLS_PORT_INT_PRI_SET) {
            soc_MPLS_ENTRYm_field32_set(unit, ment, DECAP_USE_EXP_FOR_PRIf, 0x2);
            if (soc_mem_field_valid(unit, MPLS_ENTRYm,
                                    TRUST_OUTER_DOT1P_PTRf)) {
                soc_MPLS_ENTRYm_field32_set(unit, ment,
                                            TRUST_OUTER_DOT1P_PTRf,
                                            _BCM_TR_MPLS_PRI_CNG_MAP_NONE);
            }
            soc_MPLS_ENTRYm_field32_set(unit, ment, NEW_PRIf,
                                        mpls_port->int_pri);
        } else {
            /* Trust the encapsulated packet's 802.1p pri/cfi, 
             * and use the incoming ports mapping.
             */
            soc_MPLS_ENTRYm_field32_set(unit, ment, DECAP_USE_EXP_FOR_PRIf,
                                        0x0); /* NONE */
            if (soc_mem_field_valid(unit, MPLS_ENTRYm,
                                    TRUST_OUTER_DOT1P_PTRf)) {
                soc_MPLS_ENTRYm_field32_set(unit, ment,
                                            TRUST_OUTER_DOT1P_PTRf,
                                            _BCM_TR_MPLS_PRI_CNG_MAP_IDENTITY);
            }
        }
    }else if ( info != NULL ) {
        if (info->flags & BCM_MPLS_SWITCH_INT_PRI_MAP) {
            soc_MPLS_ENTRYm_field32_set(unit, ment, DECAP_USE_EXP_FOR_PRIf,
                                        0x1); /* MAP_PRI_AND_CNG */
            /* Use specified EXP-map to determine internal prio/color */
            soc_MPLS_ENTRYm_field32_set(unit, ment, EXP_MAPPING_PTRf,
                info->exp_map & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK);
        } else if ((info->flags & BCM_MPLS_SWITCH_INT_PRI_SET) &&
                   (info->flags & BCM_MPLS_SWITCH_COLOR_MAP)) {
            soc_MPLS_ENTRYm_field32_set(unit, ment, DECAP_USE_EXP_FOR_PRIf,
                                        0x2); /* NEW_PRI_MAP_CNG */
            /* Use the specified internal priority value */
            soc_MPLS_ENTRYm_field32_set(unit, ment, NEW_PRIf,
                                        info->int_pri);
            /* Use specified EXP-map to determine internal color */
            soc_MPLS_ENTRYm_field32_set(unit, ment, EXP_MAPPING_PTRf,
                info->exp_map & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK);
        } else if (info->flags & BCM_MPLS_SWITCH_INT_PRI_SET) {
            soc_MPLS_ENTRYm_field32_set(unit, ment, DECAP_USE_EXP_FOR_PRIf, 0x2);
            if (soc_mem_field_valid(unit, MPLS_ENTRYm,
                                    TRUST_OUTER_DOT1P_PTRf)) {
                soc_MPLS_ENTRYm_field32_set(unit, ment,
                                            TRUST_OUTER_DOT1P_PTRf,
                                            _BCM_TR_MPLS_PRI_CNG_MAP_NONE);
            }
            soc_MPLS_ENTRYm_field32_set(unit, ment, NEW_PRIf,
                                        info->int_pri);
        } else {
            /* Trust the encapsulated packet's 802.1p pri/cfi, 
             * and use the incoming ports mapping.
             */
            soc_MPLS_ENTRYm_field32_set(unit, ment, DECAP_USE_EXP_FOR_PRIf,
                                        0x0); /* NONE */
            if (soc_mem_field_valid(unit, MPLS_ENTRYm,
                                    TRUST_OUTER_DOT1P_PTRf)) {
                soc_MPLS_ENTRYm_field32_set(unit, ment,
                                            TRUST_OUTER_DOT1P_PTRf,
                                            _BCM_TR_MPLS_PRI_CNG_MAP_IDENTITY);
            }
        }
    }
}

/*
 * Function:
 *      _bcm_tr_mpls_match_add
 * Purpose:
 *      Assign match criteria of an MPLS port
 * Parameters:
 *      unit    - (IN) Device Number
 *      mpls_port - (IN) mpls port information
 *      vp  - (IN) Source Virtual Port
 *      vpn - (IN) VPN
 * Returns:
 *      BCM_E_XXX
 */

int
_bcm_tr_mpls_match_add(int unit, bcm_mpls_port_t *mpls_port, int vp, int vpn,
                       int hw_update)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int rv = BCM_E_NONE, gport_id;
    bcm_module_t mod_out;
    bcm_port_t port_out;
    bcm_trunk_t trunk_id;
    vlan_xlate_entry_t vent;
    int    src_trk_idx=0;  /*Source Trunk table index.*/
    int vpless_failover_port = FALSE; /* the failover port without a dedicated vp */
    bcm_mpls_label_t old_match_label;
#if defined(BCM_KATANA2_SUPPORT)
    int my_modid = 0;
#endif
#if defined(BCM_KATANA2_SUPPORT) || defined(BCM_HGPROXY_COE_SUPPORT)
    int is_local_subport = 0, local_port = 0;
#endif
    int    vfi = 0;
    _bcm_vp_info_t vp_info;

#if defined(BCM_TRIDENT2PLUS_SUPPORT)
    if (mpls_port->criteria == BCM_MPLS_PORT_MATCH_SHARE) {
        mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_SHARE;
        return BCM_E_NONE;
    }

    if (SOC_MEM_FIELD_VALID(unit, MPLS_ENTRYm, CLASS_IDf)) {
        /* Ensure that class_id doesnt overcross CLASS_IDf */
        int bit_num = 0;
        if ((mpls_port->flags & (BCM_MPLS_PORT_COUNTED |
            BCM_MPLS_PORT_SEQUENCED)) && (mpls_port->class_id)) {
            return BCM_E_PARAM;
        }
        if (mpls_port->class_id) {
            bit_num = soc_mem_field_length(unit, MPLS_ENTRYm, CLASS_IDf);
            if (mpls_port->class_id > (( 1 << bit_num ) - 1)) {
                return BCM_E_PARAM;
            }
        }
    }
#endif

    if (_BCM_MPLS_GPORT_FAILOVER_VPLESS_GET(vp)) {
        if (mpls_port->criteria & BCM_MPLS_PORT_MATCH_LABEL) {
            /* should use same entry as primary */
            return BCM_E_NONE;
        }
        vpless_failover_port = TRUE;
        vp = _BCM_MPLS_GPORT_FAILOVER_VPLESS_CLEAR(vp);
    }

    sal_memset(&vent, 0, sizeof(vlan_xlate_entry_t));

    if (mpls_port->criteria != BCM_MPLS_PORT_MATCH_LABEL) {
         rv = _bcm_esw_gport_resolve(unit, mpls_port->port, &mod_out, 
                                &port_out, &trunk_id, &gport_id);
         BCM_IF_ERROR_RETURN(rv);
    }

    /* set default vlan_xlate key source field type as SGLP */
    if (SOC_MEM_FIELD_VALID(unit,VLAN_XLATEm,SOURCE_TYPEf)) {
        soc_VLAN_XLATEm_field32_set(unit, &vent, SOURCE_TYPEf, 1); 
    }
    /* VLAN Translation Action is valid for this lookup  */
    if (SOC_MEM_FIELD_VALID(unit,VLAN_XLATEm,XLATE__VLAN_ACTION_VALIDf)) {
        soc_VLAN_XLATEm_field32_set(unit, &vent, XLATE__VLAN_ACTION_VALIDf, 1); 
    }
    if (mpls_port->criteria == BCM_MPLS_PORT_MATCH_PORT_VLAN ) {

        soc_VLAN_XLATEm_field32_set(unit, &vent, VALIDf, 1);
        soc_VLAN_XLATEm_field32_set(unit, &vent, MPLS_ACTIONf, 0x1); /* SVP */
        soc_VLAN_XLATEm_field32_set(unit, &vent, DISABLE_VLAN_CHECKSf, 1);
        soc_VLAN_XLATEm_field32_set(unit, &vent, SOURCE_VPf, vp);
        if (!BCM_VLAN_VALID(mpls_port->match_vlan)) {
             return BCM_E_PARAM;
        }
        soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf, 
                                        TR_VLXLT_HASH_KEY_TYPE_OVID);
        soc_VLAN_XLATEm_field32_set(unit, &vent, OVIDf, 
                                        mpls_port->match_vlan);

        if (BCM_GPORT_IS_TRUNK(mpls_port->port)) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, Tf, 1);
            soc_VLAN_XLATEm_field32_set(unit, &vent, TGIDf, trunk_id);
        } else {
            soc_VLAN_XLATEm_field32_set(unit, &vent, MODULE_IDf, mod_out);
            soc_VLAN_XLATEm_field32_set(unit, &vent, PORT_NUMf, port_out);
        }
        if (soc_feature(unit, soc_feature_vp_sharing)) {
            BCM_IF_ERROR_RETURN(_bcm_vp_info_get(unit, vp, &vp_info));
            if (vp_info.flags & _BCM_VP_INFO_SHARED_PORT) {
                _BCM_MPLS_VPN_GET(vfi, _BCM_VPN_TYPE_VFI, vpn);
                soc_VLAN_XLATEm_field32_set(unit, &vent, VFIf, vfi);
            }
        }
        rv = _bcm_tr_mpls_match_vlan_entry_set(unit, mpls_port, &vent);
        BCM_IF_ERROR_RETURN(rv);

        if (!(mpls_info->match_key[vp].flags & _BCM_MPLS_PORT_MATCH_TYPE_SHARE)) {
            mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_VLAN;
            mpls_info->match_key[vp].match_vlan = mpls_port->match_vlan;

            if (BCM_GPORT_IS_TRUNK(mpls_port->port)) {
                mpls_info->match_key[vp].trunk_id = trunk_id;
            } else {
                mpls_info->match_key[vp].port = port_out;
                mpls_info->match_key[vp].modid = mod_out;
            }
        }

        if (!(mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
              bcm_tr_mpls_port_match_count_adjust(unit, vp, 1);
        }
    } else if (mpls_port->criteria ==
                            BCM_MPLS_PORT_MATCH_PORT_INNER_VLAN) {

        soc_VLAN_XLATEm_field32_set(unit, &vent, VALIDf, 1);
        soc_VLAN_XLATEm_field32_set(unit, &vent, MPLS_ACTIONf, 0x1); /* SVP */
        soc_VLAN_XLATEm_field32_set(unit, &vent, DISABLE_VLAN_CHECKSf, 1);
        soc_VLAN_XLATEm_field32_set(unit, &vent, SOURCE_VPf, vp);
        if (!BCM_VLAN_VALID(mpls_port->match_inner_vlan)) {
            return BCM_E_PARAM;
        }
        soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf,
                                    TR_VLXLT_HASH_KEY_TYPE_IVID);
        soc_VLAN_XLATEm_field32_set(unit, &vent, IVIDf,
                                    mpls_port->match_inner_vlan);

        if (BCM_GPORT_IS_TRUNK(mpls_port->port)) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, Tf, 1);
            soc_VLAN_XLATEm_field32_set(unit, &vent, TGIDf, trunk_id);
        } else {
            soc_VLAN_XLATEm_field32_set(unit, &vent, MODULE_IDf, mod_out);
            soc_VLAN_XLATEm_field32_set(unit, &vent, PORT_NUMf, port_out);
        }
        if (soc_feature(unit, soc_feature_vp_sharing)) {
            BCM_IF_ERROR_RETURN(_bcm_vp_info_get(unit, vp, &vp_info));
            if (vp_info.flags & _BCM_VP_INFO_SHARED_PORT) {
                _BCM_MPLS_VPN_GET(vfi, _BCM_VPN_TYPE_VFI, vpn);
                soc_VLAN_XLATEm_field32_set(unit, &vent, VFIf, vfi);
            }
        }
        rv = _bcm_tr_mpls_match_vlan_entry_set(unit, mpls_port, &vent);
        BCM_IF_ERROR_RETURN(rv);

        if (!(mpls_info->match_key[vp].flags & _BCM_MPLS_PORT_MATCH_TYPE_SHARE)) {
            mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_INNER_VLAN;
            mpls_info->match_key[vp].match_inner_vlan = mpls_port->match_inner_vlan;

            if (BCM_GPORT_IS_TRUNK(mpls_port->port)) {
                mpls_info->match_key[vp].trunk_id = trunk_id;
            } else {
                mpls_info->match_key[vp].port = port_out;
                mpls_info->match_key[vp].modid = mod_out;
            }
        }

        if (!(mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
             bcm_tr_mpls_port_match_count_adjust(unit, vp, 1);
        }
    } else if (mpls_port->criteria == 
                            BCM_MPLS_PORT_MATCH_PORT_VLAN_STACKED) {

         soc_VLAN_XLATEm_field32_set(unit, &vent, VALIDf, 1);
         soc_VLAN_XLATEm_field32_set(unit, &vent, MPLS_ACTIONf, 0x1); /* SVP */
         soc_VLAN_XLATEm_field32_set(unit, &vent, DISABLE_VLAN_CHECKSf, 1);
         soc_VLAN_XLATEm_field32_set(unit, &vent, SOURCE_VPf, vp);
         if (!BCM_VLAN_VALID(mpls_port->match_vlan) || 
                !BCM_VLAN_VALID(mpls_port->match_inner_vlan)) {
              return BCM_E_PARAM;
         }
         soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf, 
                   TR_VLXLT_HASH_KEY_TYPE_IVID_OVID);
         soc_VLAN_XLATEm_field32_set(unit, &vent, OVIDf, 
                   mpls_port->match_vlan);
         soc_VLAN_XLATEm_field32_set(unit, &vent, IVIDf, 
                   mpls_port->match_inner_vlan);
         if (BCM_GPORT_IS_TRUNK(mpls_port->port)) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, Tf, 1);
            soc_VLAN_XLATEm_field32_set(unit, &vent, TGIDf, trunk_id);
         } else {
            soc_VLAN_XLATEm_field32_set(unit, &vent, MODULE_IDf, mod_out);
            soc_VLAN_XLATEm_field32_set(unit, &vent, PORT_NUMf, port_out);
         }
         if (soc_feature(unit, soc_feature_vp_sharing)) {
             BCM_IF_ERROR_RETURN(_bcm_vp_info_get(unit, vp, &vp_info));
             if (vp_info.flags & _BCM_VP_INFO_SHARED_PORT) {
                 _BCM_MPLS_VPN_GET(vfi, _BCM_VPN_TYPE_VFI, vpn);
                 soc_VLAN_XLATEm_field32_set(unit, &vent, VFIf, vfi);
             }
         }
         rv = _bcm_tr_mpls_match_vlan_entry_set(unit, mpls_port, &vent);
         BCM_IF_ERROR_RETURN(rv);

         if (!(mpls_info->match_key[vp].flags & _BCM_MPLS_PORT_MATCH_TYPE_SHARE)) {
             mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_VLAN_STACKED;
             mpls_info->match_key[vp].match_vlan = mpls_port->match_vlan;
             mpls_info->match_key[vp].match_inner_vlan = mpls_port->match_inner_vlan;
             if (BCM_GPORT_IS_TRUNK(mpls_port->port)) {
                mpls_info->match_key[vp].trunk_id = trunk_id;
             } else {
                mpls_info->match_key[vp].port = port_out;
                mpls_info->match_key[vp].modid = mod_out;
             }
         }

         if (!(mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
             bcm_tr_mpls_port_match_count_adjust(unit, vp, 1);
         }
    } else if (mpls_port->criteria == BCM_MPLS_PORT_MATCH_VLAN_PRI) {
         soc_VLAN_XLATEm_field32_set(unit, &vent, VALIDf, 1);
         soc_VLAN_XLATEm_field32_set(unit, &vent, MPLS_ACTIONf, 0x1); /* SVP */
         soc_VLAN_XLATEm_field32_set(unit, &vent, DISABLE_VLAN_CHECKSf, 1);
         soc_VLAN_XLATEm_field32_set(unit, &vent, SOURCE_VPf, vp);
         soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf, 
                                           TR_VLXLT_HASH_KEY_TYPE_PRI_CFI);
          /* match_vlan : Bits 12-15 contains VLAN_PRI + CFI, vlan=BCM_E_NONE */
          soc_VLAN_XLATEm_field32_set(unit, &vent, OTAGf, 
                                             mpls_port->match_vlan);
          if (BCM_GPORT_IS_TRUNK(mpls_port->port)) {
               soc_VLAN_XLATEm_field32_set(unit, &vent, Tf, 1);
               soc_VLAN_XLATEm_field32_set(unit, &vent, TGIDf, trunk_id);
          } else {
               soc_VLAN_XLATEm_field32_set(unit, &vent, MODULE_IDf, mod_out);
               soc_VLAN_XLATEm_field32_set(unit, &vent, PORT_NUMf, port_out);
          }
          if (soc_feature(unit, soc_feature_vp_sharing)) {
              BCM_IF_ERROR_RETURN(_bcm_vp_info_get(unit, vp, &vp_info));
              if (vp_info.flags & _BCM_VP_INFO_SHARED_PORT) {
                  _BCM_MPLS_VPN_GET(vfi, _BCM_VPN_TYPE_VFI, vpn);
                  soc_VLAN_XLATEm_field32_set(unit, &vent, VFIf, vfi);
              }
          }
#if defined(BCM_TRIUMPH2_SUPPORT)
          if (SOC_MEM_FIELD_VALID(unit, SOURCE_VPm, TRUST_OUTER_DOT1Pf)) {
              source_vp_entry_t svp;
              BCM_IF_ERROR_RETURN(READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp));
              soc_SOURCE_VPm_field32_set(unit, &svp, TRUST_OUTER_DOT1Pf, 1);
              BCM_IF_ERROR_RETURN(WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp, &svp));
          }
#endif /* BCM_TRIUMPH2_SUPPORT */

          rv = _bcm_tr_mpls_match_vlan_entry_set(unit, mpls_port, &vent);
          BCM_IF_ERROR_RETURN(rv);

          if (!(mpls_info->match_key[vp].flags & _BCM_MPLS_PORT_MATCH_TYPE_SHARE)) {
              mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_VLAN_PRI;
              mpls_info->match_key[vp].match_vlan = mpls_port->match_vlan;

              if (BCM_GPORT_IS_TRUNK(mpls_port->port)) {
                   mpls_info->match_key[vp].trunk_id = trunk_id;
              } else {
                   mpls_info->match_key[vp].port = port_out;
                   mpls_info->match_key[vp].modid = mod_out;
              }
          }
          if (!(mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
               bcm_tr_mpls_port_match_count_adjust(unit, vp, 1);
          }
    } else if (mpls_port->criteria == BCM_MPLS_PORT_MATCH_PORT) {

        if (BCM_GPORT_IS_TRUNK(mpls_port->port)) {
             rv = bcm_tr_mpls_match_trunk_add(unit, trunk_id, vp);
             if (rv >= 0) {
                 if (!(mpls_info->match_key[vp].flags &
                              _BCM_MPLS_PORT_MATCH_TYPE_SHARE)) {
                    /* Delete references associated with old port in mpls_port */
                    if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
                        if ((mpls_info->match_key[vp].port != port_out) ||
                            (mpls_info->match_key[vp].modid != mod_out) ||
                            ((mpls_info->match_key[vp].flags &
                              _BCM_MPLS_PORT_MATCH_TYPE_TRUNK) &&
                             (mpls_info->match_key[vp].trunk_id != trunk_id))) {
                            BCM_IF_ERROR_RETURN(_bcm_tr_mpls_match_delete(unit, vp, NULL, TRUE));
                        }
                    }
                     mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_TRUNK;
                     mpls_info->match_key[vp].trunk_id = trunk_id;
                     mpls_info->trunk_vp_map[trunk_id] = vp;
                     mpls_info->match_key[vp].port  = port_out;
                     mpls_info->match_key[vp].modid = mod_out;
                 }
             }
             BCM_IF_ERROR_RETURN(rv);       
        } else {
            int is_local;

            BCM_IF_ERROR_RETURN
                ( _bcm_esw_modid_is_local(unit, mod_out, &is_local));

            /* Get index to source trunk map table */
            BCM_IF_ERROR_RETURN(
                   _bcm_esw_src_mod_port_table_index_get(unit, mod_out,
                     port_out, &src_trk_idx));
        
            rv = soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm, 
                                        src_trk_idx, SOURCE_VPf, vp);
            BCM_IF_ERROR_RETURN(rv);
            if (SOC_MEM_FIELD_VALID(unit,SOURCE_TRUNK_MAP_TABLEm,SVP_VALIDf)) {
                rv = soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm,
                                 src_trk_idx, SVP_VALIDf, 1);
                BCM_IF_ERROR_RETURN(rv);
            }

#if defined(BCM_HGPROXY_COE_SUPPORT)
            if (soc_feature(unit, soc_feature_hgproxy_subtag_coe) &&
                BCM_GPORT_IS_SET(mpls_port->port) &&
                _BCM_COE_GPORT_IS_SUBTAG_SUBPORT_PORT(unit,
                    mpls_port->port)) {
                local_port = mpls_port->port;
                is_local_subport = 1;
            } else
#endif

#if defined(BCM_KATANA2_SUPPORT)
            if (BCM_GPORT_IS_SET(mpls_port->port) &&
                _BCM_KT2_GPORT_IS_LINKPHY_OR_SUBTAG_SUBPORT_GPORT(unit,
                    mpls_port->port)) {
                BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &my_modid));
                if (_bcm_kt2_mod_is_coe_mod_check(unit, mod_out) == BCM_E_NONE) {
                    BCM_IF_ERROR_RETURN(_bcm_kt2_modport_to_pp_port_get(
                             unit, mod_out, port_out, &local_port));
                    if (_bcm_kt2_is_pp_port_linkphy_subtag(unit, local_port)){
                        is_local_subport = 1;
                    }
                }
            }
#endif

#if defined(BCM_HGPROXY_COE_SUPPORT) || defined(BCM_KATANA2_SUPPORT)
            if (is_local_subport) {
                rv = _bcm_esw_port_tab_set_without_portlock(
                         unit, local_port, _BCM_CPU_TABS_NONE, PORT_OPERATIONf,
                         0x1); /* L2_SVP */
                BCM_IF_ERROR_RETURN(rv);

                /* Set TAG_ACTION_PROFILE_PTR */
                rv = bcm_tr_mpls_port_untagged_profile_set(unit, local_port);
                BCM_IF_ERROR_RETURN(rv);
            } else
#endif
            if (is_local) {
                /* Convert system ports to physical ports */ 
                if (soc_feature(unit, soc_feature_sysport_remap)) { 
                    BCM_XLATE_SYSPORT_S2P(unit, &port_out); 
                }

                rv = soc_mem_field32_modify(unit, PORT_TABm, port_out,
                                        PORT_OPERATIONf, 0x1); /* L2_SVP */
                BCM_IF_ERROR_RETURN(rv);

                /* Set TAG_ACTION_PROFILE_PTR */
                rv = bcm_tr_mpls_port_untagged_profile_set(unit, port_out);
                BCM_IF_ERROR_RETURN(rv);
            }
            /* Delete references associated with old port in mpls_port */
            if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
                if ((mpls_info->match_key[vp].port != port_out) ||
                    (mpls_info->match_key[vp].modid != mod_out) ||
                    ((mpls_info->match_key[vp].flags &
                      _BCM_MPLS_PORT_MATCH_TYPE_TRUNK) &&
                     (mpls_info->match_key[vp].trunk_id != trunk_id))) {
                    BCM_IF_ERROR_RETURN(_bcm_tr_mpls_match_delete(unit, vp, NULL, TRUE));
                }
            }
            /*
             * The vp has been created based on trunk.
             * When a new port is added into the trunk, a match will be
             * added for the vp through bcm_port_match_add.
             */
            if ((mpls_info->match_key[vp].flags != _BCM_MPLS_PORT_MATCH_TYPE_TRUNK) &&
               (!(mpls_info->match_key[vp].flags & _BCM_MPLS_PORT_MATCH_TYPE_SHARE))) {
                mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_PORT;
                mpls_info->match_key[vp].index = src_trk_idx;
                mpls_info->match_key[vp].port  = port_out;
                mpls_info->match_key[vp].modid = mod_out;
            }
        }
    } else if ((mpls_port->criteria == BCM_MPLS_PORT_MATCH_LABEL) ||
               (mpls_port->criteria == BCM_MPLS_PORT_MATCH_LABEL_PORT)) {

        mpls_entry_entry_t ment, return_ment;
        int old_pw_term_cntr_idx=-1, new_pw_term_cntr_idx=-1;
        int entry_index=-1;
        int old_mpls_entry_rv = BCM_E_NONE;
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
        uint32 flex_ctr_poolnum=0;
        uint32 flex_ctr_basectr_idx=0;
        uint32 flex_ctr_offset_mode=0;
#endif 
#ifdef BCM_TRIUMPH2_SUPPORT
        int old_vintf_cntr_idx = -1;
#endif


        sal_memset(&ment, 0, sizeof(mpls_entry_entry_t));
        if (mpls_port->criteria == BCM_MPLS_PORT_MATCH_LABEL_PORT) {
            /* Check for Port_independent Label mapping */
            rv = bcm_tr_mpls_port_independent_range (unit, mpls_port->match_label, mpls_port->port);
            if (rv < 0) {
                return rv;
            }
            if (BCM_GPORT_IS_TRUNK(mpls_port->port)) {
                soc_MPLS_ENTRYm_field32_set(unit, &ment, Tf, 1);
                soc_MPLS_ENTRYm_field32_set(unit, &ment, TGIDf, trunk_id);
                if (vpless_failover_port == FALSE) {
                    mpls_info->match_key[vp].trunk_id = trunk_id;
                } else {
                    mpls_info->match_key[vp].fo_trunk_id = trunk_id;
                }
            } else {
                soc_MPLS_ENTRYm_field32_set(unit, &ment, MODULE_IDf, mod_out);
                soc_MPLS_ENTRYm_field32_set(unit, &ment, PORT_NUMf, port_out);
                if (vpless_failover_port == FALSE) {
                    mpls_info->match_key[vp].port = port_out;
                    mpls_info->match_key[vp].modid = mod_out;
                } else {
                    mpls_info->match_key[vp].fo_port = port_out;
                    mpls_info->match_key[vp].fo_modid = mod_out;
                }
            }
            mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_LABEL_PORT;
        } else if (mpls_port->criteria == BCM_MPLS_PORT_MATCH_LABEL) {
            soc_MPLS_ENTRYm_field32_set(unit, &ment, MODULE_IDf, 0);
            soc_MPLS_ENTRYm_field32_set(unit, &ment, PORT_NUMf, 0);
            mpls_info->match_key[vp].port = 0;
            mpls_info->match_key[vp].modid = 0;
            mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_LABEL;
        }
        /* Use old label to get old mpls entry for replace operation */
        old_match_label = mpls_info->match_key[vp].match_label;
        soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_LABELf, 
                                    old_match_label);
        soc_MPLS_ENTRYm_field32_set(unit, &ment, VALIDf, 1);
        soc_MPLS_ENTRYm_field32_set(unit, &ment, SOURCE_VPf, vp);
        soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_ACTION_IF_BOSf, 
                                    0x1); /* L2 SVP */
        soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_ACTION_IF_NOT_BOSf, 
                                    0x0); /* INVALID */

        if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
            /* Find Entry within Hardware */
            old_mpls_entry_rv = soc_mem_search(unit, MPLS_ENTRYm, 
                                               MEM_BLOCK_ANY, &entry_index,
                                               &ment, &return_ment, 0);

            /* Obtain old pw_term counter-index and old label flex stat index */
            if (BCM_SUCCESS(old_mpls_entry_rv)) {
                if (soc_MPLS_ENTRYm_field32_get(unit, &return_ment,
                                            PW_TERM_NUM_VALIDf)) {
                    old_pw_term_cntr_idx = soc_MPLS_ENTRYm_field32_get(unit,
                                               &return_ment,PW_TERM_NUMf);
                }
#ifdef BCM_TRIUMPH2_SUPPORT
                if (soc_feature(unit, soc_feature_gport_service_counters)) {
                    if (soc_MPLS_ENTRYm_field32_get(unit, &return_ment,
                                                    USE_VINTF_CTR_IDXf)) {
                        old_vintf_cntr_idx = soc_MPLS_ENTRYm_field32_get(unit,
                                                 &return_ment, VINTF_CTR_IDXf);
                    }
                }
#endif /* BCM_TRIUMPH2_SUPPORT  */

#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
                if ((soc_feature(unit, soc_feature_mpls_enhanced)) &&
                     (soc_feature(unit,soc_feature_advanced_flex_counter))) {
                    /* Store Flex counter values */
                    flex_ctr_poolnum = soc_MPLS_ENTRYm_field32_get(unit,
                                                                   &return_ment,
                                                    MPLS__FLEX_CTR_POOL_NUMBERf);
                    if(flex_ctr_poolnum) {
                        soc_MPLS_ENTRYm_field32_set(unit, &ment,
                                                    MPLS__FLEX_CTR_POOL_NUMBERf,
                                                    flex_ctr_poolnum);
                    }
                    flex_ctr_basectr_idx = soc_MPLS_ENTRYm_field32_get(unit,
                                                               &return_ment,
                                              MPLS__FLEX_CTR_BASE_COUNTER_IDXf);
                    if(flex_ctr_basectr_idx) {
                        soc_MPLS_ENTRYm_field32_set(unit, &ment,
                                              MPLS__FLEX_CTR_BASE_COUNTER_IDXf,
                                              flex_ctr_basectr_idx);
                    }
                    flex_ctr_offset_mode = soc_MPLS_ENTRYm_field32_get(unit,
                                                                  &return_ment,
                                                  MPLS__FLEX_CTR_OFFSET_MODEf);
                    soc_MPLS_ENTRYm_field32_set(unit, &ment,
                                                MPLS__FLEX_CTR_OFFSET_MODEf,
                                                flex_ctr_offset_mode);
                }
#endif /* (BCM_KATANA_SUPPORT) || (BCM_TRIUMPH3_SUPPORT) */
            }
        }

        /* Add the new MPLS label for the new entry*/
        soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_LABELf,
                                    mpls_port->match_label);

        if (mpls_port->flags & BCM_MPLS_PORT_SEQUENCED) {
            /* Control word is present, check sequence number */
            soc_MPLS_ENTRYm_field32_set(unit, &ment, CW_CHECK_CTRLf, 
                                        0x3); /* CW_CHECK_LOOSE */
            

        } else if (mpls_port->flags & BCM_MPLS_PORT_CONTROL_WORD) {
            /* Control word is present, but no sequence number check */
            soc_MPLS_ENTRYm_field32_set(unit, &ment, CW_CHECK_CTRLf, 
                                        0x1); /* CW_NO_CHECK */
        }
        if (((mpls_port->flags & BCM_MPLS_PORT_COUNTED) ||
            (mpls_port->flags & BCM_MPLS_PORT_SEQUENCED))) {
               /*  Obtain pw_term_counter */
               BCM_IF_ERROR_RETURN(
                   _bcm_tr_mpls_pw_term_counter_set (unit, &new_pw_term_cntr_idx));
               if ( new_pw_term_cntr_idx != -1) {
                   soc_MPLS_ENTRYm_field32_set(unit, &ment, PW_TERM_NUMf, new_pw_term_cntr_idx);
                   soc_MPLS_ENTRYm_field32_set(unit, &ment, PW_TERM_NUM_VALIDf, 1);
               }
        }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
        else if (SOC_MEM_FIELD_VALID(unit, MPLS_ENTRYm, CLASS_IDf)) {
            soc_MPLS_ENTRYm_field32_set(unit, &ment, CLASS_IDf, mpls_port->class_id);
        }
#endif
#if defined(BCM_TRIDENT2PLUS_SUPPORT) || defined(BCM_SABER2_SUPPORT)
        if (soc_mem_field_valid(unit, MPLS_ENTRYm, PW_CC_TYPEf)) {
            if (mpls_port->vccv_type == bcmMplsPortControlChannelGalUnderPw) {
                soc_MPLS_ENTRYm_field32_set(unit, &ment,
                    MPLS_ACTION_IF_NOT_BOSf, 0x1);
            }
        }
#endif

        (void) bcm_tr_mpls_entry_internal_qos_set(unit, mpls_port, NULL, &ment);

#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
        if (soc_feature(unit, soc_feature_mpls_entropy)) {
            if ((mpls_port->flags & BCM_MPLS_PORT_ENTROPY_ENABLE) ||
                (mpls_port->flags2 & BCM_MPLS_PORT2_INGRESS_ENTROPY_ENABLE)) {
                if (soc_mem_field_valid(unit, MPLS_ENTRYm,
                                    ENTROPY_LABEL_PRESENTf)) {
                     soc_MPLS_ENTRYm_field32_set(unit, &ment,
                                            ENTROPY_LABEL_PRESENTf, 0x1);
                     soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_ACTION_IF_BOSf, 
                                    0x0); /* INVALID */
                     
                     if (SOC_IS_APACHE(unit)) {
                         soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_ACTION_IF_NOT_BOSf, 
                                 0x6); /* POP_SVP_ENTROPY */
                     } else {
                         soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_ACTION_IF_NOT_BOSf, 
                                 0x4); /* POP_SVP_ENTROPY */
                     }
                }
            }
        }

        /* VCCV defines 3 control-channels */
        if (soc_mem_field_valid(unit, MPLS_ENTRYm, PW_CC_TYPEf)) {
            soc_MPLS_ENTRYm_field32_set(unit, &ment, PW_CC_TYPEf, 
                                       mpls_port->vccv_type);
        }
#endif /* defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT) */

#ifdef BCM_TRIUMPH2_SUPPORT
        if (soc_feature(unit, soc_feature_gport_service_counters)) {
            if ((mpls_port->flags & BCM_MPLS_PORT_REPLACE) &&
                (old_vintf_cntr_idx != -1)) {
                if (old_match_label != mpls_port->match_label) {
                    /* If MPLS label stat is used for this MPLS_ENTRY then
                    * it must be released first using stat_detach*/
                    LOG_ERROR(BSL_LS_BCM_MPLS,
                              (BSL_META_U(unit,
                                          "The flex stat counter %d for the "
                                          "old label %d must be detached first\n before performing "
                                          "the MPLS label replace action.\n"),
                               old_vintf_cntr_idx, old_match_label));
                    return BCM_E_INTERNAL;
                } else {
                     /* copy the old label flex sat counter index if old_label == new_label*/
                     soc_MPLS_ENTRYm_field32_set(unit, &ment,
                                                 USE_VINTF_CTR_IDXf, 1);
                     soc_MPLS_ENTRYm_field32_set(unit, &ment, VINTF_CTR_IDXf,
                                                 old_vintf_cntr_idx);
                }
            }
        }
#endif /* BCM_TRIUMPH2_SUPPORT */
#ifdef BCM_TOMAHAWK2_SUPPORT
        if (soc_feature(unit, soc_feature_mpls_ecn)) {
            if (mpls_port->flags2 & BCM_MPLS_PORT2_INGRESS_ECN_MAP) {
                int ecn_map_index;
                int ecn_map_hw_idx;
                int ecn_map_type;
                int ecn_map_num;
                ecn_map_type = mpls_port->ecn_map_id & 
                               _BCM_XGS5_MPLS_ECN_MAP_TYPE_MASK;
                ecn_map_index = mpls_port->ecn_map_id & 
                                _BCM_XGS5_MPLS_ECN_MAP_NUM_MASK;
                ecn_map_num = 
                    soc_mem_index_count(unit, ING_EXP_TO_IP_ECN_MAPPINGm) / 
                    _BCM_ECN_MAX_ENTRIES_PER_MAP;
                if (ecn_map_type != _BCM_XGS5_MPLS_ECN_MAP_TYPE_EXP2ECN) {
                    return BCM_E_PARAM;
                }
                if (ecn_map_index >= ecn_map_num) {
                    return BCM_E_PARAM;
                }
                if (!bcmi_xgs5_ecn_map_used_get(unit, ecn_map_index, 
                                                _bcmEcnmapTypeExp2Ecn)) {
                    return BCM_E_PARAM; 
                }
                BCM_IF_ERROR_RETURN(
                    bcmi_ecn_map_id2hw_idx(unit, mpls_port->ecn_map_id, &ecn_map_hw_idx));
                soc_MPLS_ENTRYm_field32_set(unit, &ment, 
                                            EXP_TO_IP_ECN_MAPPING_PTRf, 
                                            ecn_map_hw_idx);
            }
        }
#endif /* BCM_TOMAHAWK2_SUPPORT */


        if (soc_mem_field_valid(unit, MPLS_ENTRYm, DO_NOT_CHANGE_PAYLOAD_DSCPf)) {
            if (_BCM_MPLS_VPN_IS_VPLS(vpn) || _BCM_MPLS_VPN_IS_VPWS(vpn)) {            
                soc_MPLS_ENTRYm_field32_set(unit, &ment, 
                                            DO_NOT_CHANGE_PAYLOAD_DSCPf, 1); 
            }
        }

        /* Add the new MPLS entry */
        if (hw_update) {
            rv = _bcm_tr_mpls_match_label_entry_set(unit, mpls_port, &ment);
        }

        /* update the new label in match key of the VP*/
        if (BCM_SUCCESS(rv)) {
            mpls_info->match_key[vp].match_label = mpls_port->match_label;
        }

        /* Delete the old MPLS entry */
        if (BCM_SUCCESS(rv) && (mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
            int failover_vp = -1;
            bcm_mpls_label_t failover_match_label = -1;

            failover_vp = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->failover_port_id);
            if ((failover_vp >= 0) &&
                _bcm_vp_used_get(unit, failover_vp, _bcmVpTypeMpls)) {
                failover_match_label = mpls_info->match_key[failover_vp].match_label;
            }

            if ((old_pw_term_cntr_idx != -1) &&
                (new_pw_term_cntr_idx != old_pw_term_cntr_idx)) {
                /* Delete old pw_term counter associated with old mpls label */
                (void) _bcm_tr_mpls_pw_term_counter_delete (unit,
                     old_pw_term_cntr_idx);
            }
            /*
             * When replace match criteria of primary port,  old match entry keeps
             * unchanged if 1: new match info is same as old one; 2: backup port
             * has the same match info.
             */
            if ((old_mpls_entry_rv == BCM_E_NONE) 
                && (old_match_label != mpls_port->match_label)){
                if ((failover_match_label != -1) && (old_match_label == failover_match_label)) {
                    soc_MPLS_ENTRYm_field32_set(unit, &return_ment, SOURCE_VPf, failover_vp);
                    BCM_IF_ERROR_RETURN(soc_mem_write(unit, MPLS_ENTRYm,
                                           MEM_BLOCK_ALL, entry_index,
                                           &return_ment));
                } else {
                    BCM_IF_ERROR_RETURN(soc_mem_delete(unit, MPLS_ENTRYm,
                        MEM_BLOCK_ANY, &return_ment));
                }
            }
        }
    } else if (mpls_port->criteria == BCM_MPLS_PORT_MATCH_NONE) {
        (void) bcm_tr_mpls_match_clear (unit, vp);
        mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_NONE;
        bcm_tr_mpls_port_match_count_adjust(unit, vp, 1);
    }
#if defined(BCM_KATANA2_SUPPORT)
    else if ((mpls_port->criteria == BCM_MPLS_PORT_MATCH_PORT_SUBPORT_PKT_VID) ||
            (mpls_port->criteria == BCM_MPLS_PORT_MATCH_PORT_SUBPORT_PKT_VID_INNER_VLAN) ||
            (mpls_port->criteria == BCM_MPLS_PORT_MATCH_PORT_SUBPORT_PKT_VID_OUTER_VLAN)) {

        int is_local = 0;    

        BCM_IF_ERROR_RETURN
            (_bcm_esw_modid_is_local(unit, mod_out, &is_local));

        if (is_local) {
            /* Convert system ports to physical ports */ 
            if (soc_feature(unit, soc_feature_sysport_remap)) { 
                BCM_XLATE_SYSPORT_S2P(unit, &port_out); 
            }
            rv = soc_mem_field32_modify(unit, PORT_TABm, port_out,
                            VT_PORT_TYPE_SELECTf, 1);/*default*/ 
            BCM_IF_ERROR_RETURN(rv);  
        }
        
        if (!soc_feature(unit, soc_feature_lltag)) {
            return BCM_E_UNAVAIL;
        }

        soc_VLAN_XLATEm_field32_set(unit, &vent, VALIDf, 0x1);                       
        soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__MPLS_ACTIONf, 0x1); /* SVP */
        soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__DISABLE_VLAN_CHECKSf, 1);
        soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__SOURCE_VPf, vp);
        if (!BCM_VLAN_VALID(mpls_port->match_subport_pkt_vid)) {
            return BCM_E_PARAM;
        }

        if (mpls_port->criteria == BCM_MPLS_PORT_MATCH_PORT_SUBPORT_PKT_VID) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf,
                                        KT2_PT_XLT_KEY_TYPE_LLVID);
        } else if (mpls_port->criteria ==
                BCM_MPLS_PORT_MATCH_PORT_SUBPORT_PKT_VID_INNER_VLAN) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf,
                                        KT2_PT_XLT_KEY_TYPE_LLVID_IVID);
            soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__IVIDf,
                                        mpls_port->match_inner_vlan);
        } else {
            soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf,
                                        KT2_PT_XLT_KEY_TYPE_LLVID_OVID);
            soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__OVIDf,
                                        mpls_port->match_vlan);
        }

        soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__LLVIDf,
                                    mpls_port->match_subport_pkt_vid);

        if (BCM_GPORT_IS_TRUNK(mpls_port->port)) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__Tf, 1);
            soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__TGIDf, trunk_id);
        } else {
            soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__MODULE_IDf, mod_out);
            soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__PORT_NUMf, port_out);
        }
        rv = _bcm_tr_mpls_match_vlan_entry_set(unit, mpls_port, &vent);
        BCM_IF_ERROR_RETURN(rv);
            /*Update S/W information only after match entry set successfully*/
        if (mpls_port->criteria == BCM_MPLS_PORT_MATCH_PORT_SUBPORT_PKT_VID) {
            if (is_local) {
                rv = soc_mem_field32_modify(unit, PORT_TABm, port_out,
                    VT_KEY_TYPEf, KT2_PT_XLT_KEY_TYPE_LLVID);  
                BCM_IF_ERROR_RETURN(rv);
            }
            mpls_info->match_key[vp].flags = _BCM_MPLS_PORT_MATCH_TYPE_PORT_SUBPORT_PKT_VID;
        } else if (mpls_port->criteria == 
                BCM_MPLS_PORT_MATCH_PORT_SUBPORT_PKT_VID_INNER_VLAN) {
            if (is_local) {
                rv = soc_mem_field32_modify(unit, PORT_TABm, port_out,
                    VT_KEY_TYPEf, KT2_PT_XLT_KEY_TYPE_LLVID_IVID);  
                BCM_IF_ERROR_RETURN(rv);
            }
            mpls_info->match_key[vp].flags = 
                _BCM_MPLS_PORT_MATCH_TYPE_PORT_SUBPORT_PKT_VID_INNER_VLAN;
            mpls_info->match_key[vp].match_inner_vlan = mpls_port->match_inner_vlan;
        } else {
            if (is_local) {
                rv = soc_mem_field32_modify(unit, PORT_TABm, port_out,
                    VT_KEY_TYPEf, KT2_PT_XLT_KEY_TYPE_LLVID_OVID);  
                BCM_IF_ERROR_RETURN(rv);
            }
            mpls_info->match_key[vp].flags = 
                _BCM_MPLS_PORT_MATCH_TYPE_PORT_SUBPORT_PKT_VID_OUTER_VLAN;
            mpls_info->match_key[vp].match_vlan = mpls_port->match_vlan;
        }

        mpls_info->match_key[vp].match_subport_pkt_vid = mpls_port->match_subport_pkt_vid;

        if (BCM_GPORT_IS_TRUNK(mpls_port->port)) {
            mpls_info->match_key[vp].trunk_id = trunk_id;
        } else {
            mpls_info->match_key[vp].port = port_out;
            mpls_info->match_key[vp].modid = mod_out;
        }

        if (!(mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
             bcm_tr_mpls_port_match_count_adjust(unit, vp, 1);
        }
    }
 #endif /* end BCM_KATANA2-SUPPORT */
    return rv;
}

/*
 * Function:
 *      _bcm_tr_mpls_match_delete
 * Purpose:
 *      Delete match criteria of an MPLS port
 * Parameters:
 *      unit    - (IN) Device Number
 *      mpls_port - (IN) mpls port information
 *      vp  - (IN) Source Virtual Port
 * Returns:
 *      BCM_E_XXX
 */

int
_bcm_tr_mpls_match_delete(int unit, int vp, bcm_mpls_port_t *mpls_port,
                          int sw_clear)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int rv=BCM_E_NONE;
    bcm_trunk_t trunk_id;
    int    src_trk_idx=0;   /*Source Trunk table index.*/
    int    mod_id_idx=0;   /* Module_Id */
    int port;
    int vpless_failover_port = FALSE;
#if defined(BCM_KATANA2_SUPPORT)
    bcm_port_t pp_port = 0;
#endif
#if defined(BCM_KATANA2_SUPPORT) || defined(BCM_HGPROXY_COE_SUPPORT)
    int is_local_subport = 0;
    int local_port = 0;
#endif

    if (_BCM_MPLS_GPORT_FAILOVER_VPLESS_GET(vp)) {
        vp = _BCM_MPLS_GPORT_FAILOVER_VPLESS_CLEAR(vp);
        if (mpls_info->match_key[vp].flags & _BCM_MPLS_PORT_MATCH_TYPE_LABEL) {
            /* label entry belongs to primary port */
            return BCM_E_NONE;
        }
        vpless_failover_port = TRUE;
    }

    /* Get module id for unit. */
    BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &mod_id_idx));

    if (mpls_info->match_key[vp].flags == _BCM_MPLS_PORT_MATCH_TYPE_NONE) {
        if (sw_clear) {
            (void) bcm_tr_mpls_match_clear (unit, vp);
            bcm_tr_mpls_port_match_count_adjust(unit, vp, -1);
        }
    } else if (mpls_info->match_key[vp].flags ==
                  _BCM_MPLS_PORT_MATCH_TYPE_VLAN) {

        vlan_xlate_entry_t vent;

        sal_memset(&vent, 0, sizeof(vlan_xlate_entry_t));

        /* default to SGLP type. TD2 */
        if (SOC_MEM_FIELD_VALID(unit,VLAN_XLATEm,SOURCE_TYPEf)) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, SOURCE_TYPEf, 1);
        }

        soc_VLAN_XLATEm_field32_set(unit, &vent, VALIDf, 1);
        soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf, 
                                    TR_VLXLT_HASH_KEY_TYPE_OVID);
        soc_VLAN_XLATEm_field32_set(unit, &vent, OVIDf,
                                    mpls_info->match_key[vp].match_vlan);
        if (mpls_info->match_key[vp].modid != -1) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, MODULE_IDf, 
                                        mpls_info->match_key[vp].modid);
            soc_VLAN_XLATEm_field32_set(unit, &vent, PORT_NUMf, 
                                        mpls_info->match_key[vp].port);
        } else {
            soc_VLAN_XLATEm_field32_set(unit, &vent, Tf, 1);
            soc_VLAN_XLATEm_field32_set(unit, &vent, TGIDf, 
                                        mpls_info->match_key[vp].trunk_id);
        }
        rv = soc_mem_delete(unit, VLAN_XLATEm, MEM_BLOCK_ANY, &vent);
        if ((rv == BCM_E_NOT_FOUND) || (rv == BCM_E_NONE)) {
            /*  if we are not able to search for matching entry then we
             *  return BCM_E_NONE. so it is good to clean the local
             *  state also.
             */
            if (sw_clear) {
                (void) bcm_tr_mpls_match_clear (unit, vp);
                bcm_tr_mpls_port_match_count_adjust(unit, vp, -1);
            }
        }

        BCM_IF_ERROR_RETURN(rv);
    } else if  (mpls_info->match_key[vp].flags == 
                     _BCM_MPLS_PORT_MATCH_TYPE_INNER_VLAN) {

        vlan_xlate_entry_t vent;

        sal_memset(&vent, 0, sizeof(vlan_xlate_entry_t));

        /* default to SGLP type. TD2 */
        if (SOC_MEM_FIELD_VALID(unit,VLAN_XLATEm,SOURCE_TYPEf)) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, SOURCE_TYPEf, 1);
        }

        soc_VLAN_XLATEm_field32_set(unit, &vent, VALIDf, 1);
        soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf, 
                                    TR_VLXLT_HASH_KEY_TYPE_IVID);
        soc_VLAN_XLATEm_field32_set(unit, &vent, IVIDf,
                                    mpls_info->match_key[vp].match_inner_vlan);
        if (mpls_info->match_key[vp].modid != -1) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, MODULE_IDf, 
                                        mpls_info->match_key[vp].modid);
            soc_VLAN_XLATEm_field32_set(unit, &vent, PORT_NUMf, 
                                        mpls_info->match_key[vp].port);
        } else {
            soc_VLAN_XLATEm_field32_set(unit, &vent, Tf, 1);
            soc_VLAN_XLATEm_field32_set(unit, &vent, TGIDf, 
                                        mpls_info->match_key[vp].trunk_id);
        }
        rv = soc_mem_delete(unit, VLAN_XLATEm, MEM_BLOCK_ANY, &vent);
        if ((rv == BCM_E_NOT_FOUND) || (rv == BCM_E_NONE)) {
            /*  if we are not able to search for matching entry then we
             *  return BCM_E_NONE. so it is good to clean the local
             *  state also.
             */

            if (sw_clear) {
                (void) bcm_tr_mpls_match_clear (unit, vp);
                bcm_tr_mpls_port_match_count_adjust(unit, vp, -1);
            }
        }

        BCM_IF_ERROR_RETURN(rv);
        
    }else if (mpls_info->match_key[vp].flags == 
                    _BCM_MPLS_PORT_MATCH_TYPE_VLAN_STACKED) {
        vlan_xlate_entry_t vent;
        sal_memset(&vent, 0, sizeof(vlan_xlate_entry_t));

        /* default to SGLP type. TD2 */
        if (SOC_MEM_FIELD_VALID(unit,VLAN_XLATEm,SOURCE_TYPEf)) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, SOURCE_TYPEf, 1);
        }

        soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf, 
                                    TR_VLXLT_HASH_KEY_TYPE_IVID_OVID);
        soc_VLAN_XLATEm_field32_set(unit, &vent, OVIDf,
                                    mpls_info->match_key[vp].match_vlan);
        soc_VLAN_XLATEm_field32_set(unit, &vent, IVIDf,
                              mpls_info->match_key[vp].match_inner_vlan);
        if (mpls_info->match_key[vp].modid != -1) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, MODULE_IDf, 
                                        mpls_info->match_key[vp].modid);
            soc_VLAN_XLATEm_field32_set(unit, &vent, PORT_NUMf, 
                                        mpls_info->match_key[vp].port);
        } else {
            soc_VLAN_XLATEm_field32_set(unit, &vent, Tf, 1);
            soc_VLAN_XLATEm_field32_set(unit, &vent, TGIDf, 
                                        mpls_info->match_key[vp].trunk_id);
        }
        rv = soc_mem_delete(unit, VLAN_XLATEm, MEM_BLOCK_ANY, &vent);
        if ((rv == BCM_E_NOT_FOUND) || (rv == BCM_E_NONE)) {
            /*  if we are not able to search for matching entry then we
             *  return BCM_E_NONE. so it is good to clean the local
             *  state also.
             */

            if (sw_clear) {
                (void) bcm_tr_mpls_match_clear (unit, vp);
                bcm_tr_mpls_port_match_count_adjust(unit, vp, -1);
            }
        }

        BCM_IF_ERROR_RETURN(rv);

    } else if	(mpls_info->match_key[vp].flags ==
                                               _BCM_MPLS_PORT_MATCH_TYPE_VLAN_PRI) {

        vlan_xlate_entry_t vent;

        sal_memset(&vent, 0, sizeof(vlan_xlate_entry_t));

        /* default to SGLP type. TD2 */
        if (SOC_MEM_FIELD_VALID(unit,VLAN_XLATEm,SOURCE_TYPEf)) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, SOURCE_TYPEf, 1);
        }

        soc_VLAN_XLATEm_field32_set(unit, &vent, VALIDf, 1);
        soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf, 
                                                      TR_VLXLT_HASH_KEY_TYPE_PRI_CFI);
        soc_VLAN_XLATEm_field32_set(unit, &vent, OTAGf,
                                                       mpls_info->match_key[vp].match_vlan);
        if (mpls_info->match_key[vp].modid != -1) {
              soc_VLAN_XLATEm_field32_set(unit, &vent, MODULE_IDf, 
                                                      mpls_info->match_key[vp].modid);
              soc_VLAN_XLATEm_field32_set(unit, &vent, PORT_NUMf, 
                                                      mpls_info->match_key[vp].port);
        } else {
              soc_VLAN_XLATEm_field32_set(unit, &vent, Tf, 1);
              soc_VLAN_XLATEm_field32_set(unit, &vent, TGIDf, 
                                                      mpls_info->match_key[vp].trunk_id);
        }
        rv = soc_mem_delete(unit, VLAN_XLATEm, MEM_BLOCK_ANY, &vent);
        if ((rv == BCM_E_NOT_FOUND) || (rv == BCM_E_NONE)) {
            /*  if we are not able to search for matching entry then we
             *  return BCM_E_NONE. so it is good to clean the local
             *  state also.
             */

            if (sw_clear) {
                (void) bcm_tr_mpls_match_clear (unit, vp);
                bcm_tr_mpls_port_match_count_adjust(unit, vp, -1);
            }
        }

        BCM_IF_ERROR_RETURN(rv);

#if defined(BCM_TRIUMPH2_SUPPORT)
        if (SOC_MEM_FIELD_VALID(unit, SOURCE_VPm, TRUST_OUTER_DOT1Pf)) {
            source_vp_entry_t svp;
            BCM_IF_ERROR_RETURN(READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp));
            soc_SOURCE_VPm_field32_set(unit, &svp, TRUST_OUTER_DOT1Pf, 0);
            BCM_IF_ERROR_RETURN(WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp, &svp));
        }
#endif /* BCM_TRIUMPH2_SUPPORT */

    } else if (mpls_info->match_key[vp].flags ==
                                             _BCM_MPLS_PORT_MATCH_TYPE_SHARE) {
        vlan_xlate_entry_t vent;
        bcm_module_t mod_out = -1;
        bcm_port_t port_out = -1;
        int    gport_id =-1;
        bcm_trunk_t trunk_id;

        sal_memset(&vent, 0, sizeof(vlan_xlate_entry_t));

        if (SOC_MEM_FIELD_VALID(unit,VLAN_XLATEm,SOURCE_TYPEf)) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, SOURCE_TYPEf, 1);
        }
        soc_VLAN_XLATEm_field32_set(unit, &vent, VALIDf, 1);

        if (BCM_MPLS_PORT_MATCH_PORT_VLAN == mpls_port->criteria) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf,
                                        TR_VLXLT_HASH_KEY_TYPE_OVID);
            soc_VLAN_XLATEm_field32_set(unit, &vent, OVIDf,
                                        mpls_port->match_vlan);
        } else if (BCM_MPLS_PORT_MATCH_PORT_VLAN_STACKED == mpls_port->criteria) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf,
                                        TR_VLXLT_HASH_KEY_TYPE_IVID_OVID);
            soc_VLAN_XLATEm_field32_set(unit, &vent, OVIDf,
                                        mpls_port->match_vlan);
            soc_VLAN_XLATEm_field32_set(unit, &vent, IVIDf,
                                        mpls_port->match_inner_vlan);
        } else if (BCM_MPLS_PORT_MATCH_PORT_INNER_VLAN == mpls_port->criteria) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf,
                                        TR_VLXLT_HASH_KEY_TYPE_IVID);
            soc_VLAN_XLATEm_field32_set(unit, &vent, IVIDf,
                                        mpls_port->match_inner_vlan);
        } else if (BCM_MPLS_PORT_MATCH_VLAN_PRI == mpls_port->criteria) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf,
                                        TR_VLXLT_HASH_KEY_TYPE_PRI_CFI);
            soc_VLAN_XLATEm_field32_set(unit, &vent, OTAGf,
                                        mpls_port->match_vlan);
        } else {
            return BCM_E_UNAVAIL;
        }
        rv = _bcm_esw_gport_resolve(unit, mpls_port->port, &mod_out,
                                    &port_out, &trunk_id, &gport_id);
        BCM_IF_ERROR_RETURN(rv);

        if (BCM_GPORT_IS_TRUNK(mpls_port->port)) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, Tf, 1);
            soc_VLAN_XLATEm_field32_set(unit, &vent, TGIDf, trunk_id);
        } else {
            soc_VLAN_XLATEm_field32_set(unit, &vent, MODULE_IDf, mod_out);
            soc_VLAN_XLATEm_field32_set(unit, &vent, PORT_NUMf, port_out);
        }
        rv = soc_mem_delete(unit, VLAN_XLATEm, MEM_BLOCK_ANY, &vent);
        BCM_IF_ERROR_RETURN(rv);

        bcm_tr_mpls_port_match_count_adjust(unit, vp, -1);
    } else if ((mpls_info->match_key[vp].flags ==  _BCM_MPLS_PORT_MATCH_TYPE_PORT)
               || ((mpls_info->match_key[vp].flags == _BCM_MPLS_PORT_MATCH_TYPE_TRUNK)
                   && (mpls_port != NULL) && (!BCM_GPORT_IS_TRUNK(mpls_port->port)))) {

        int is_local;
        bcm_module_t mod_out;
        bcm_port_t port_out;
        int gport_id;

        if (mpls_info->match_key[vp].flags == _BCM_MPLS_PORT_MATCH_TYPE_PORT) {
            src_trk_idx = mpls_info->match_key[vp].index;
        } else {
            /*
             * The vp has been created based on trunk.
             * When a new port is deleted from the trunk, a match will be deleted
             * for the vp through bcm_port_match_delete.
             */
            if (mpls_port == NULL) {
                return BCM_E_PARAM;
            }
            BCM_IF_ERROR_RETURN(
                _bcm_esw_gport_resolve(unit, mpls_port->port, &mod_out,
                                       &port_out, &trunk_id, &gport_id));
            /* Get index to source trunk map table */
            BCM_IF_ERROR_RETURN(
                _bcm_esw_src_mod_port_table_index_get(
                    unit, mod_out, port_out, &src_trk_idx));
       }

         rv = soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm, 
                                  src_trk_idx, SOURCE_VPf, 0);
         BCM_IF_ERROR_RETURN(rv);
         if (SOC_MEM_FIELD_VALID(unit,SOURCE_TRUNK_MAP_TABLEm,SVP_VALIDf)) {
            rv = soc_mem_field32_modify(unit, SOURCE_TRUNK_MAP_TABLEm,
                                 src_trk_idx, SVP_VALIDf, 0);
            BCM_IF_ERROR_RETURN(rv);
         }

         BCM_IF_ERROR_RETURN(_bcm_esw_src_modid_port_get( unit, src_trk_idx,
                                                          &mod_id_idx, &port));
         BCM_IF_ERROR_RETURN
             ( _bcm_esw_modid_is_local(unit, mod_id_idx, &is_local));

#if defined(BCM_HGPROXY_COE_SUPPORT)
    if (soc_feature(unit, soc_feature_hgproxy_subtag_coe)) {

        if (is_local == 0) {
            if(_bcm_xgs5_subport_coe_mod_port_local(unit, mod_id_idx, port)) {
                is_local_subport = 1;
                local_port = mpls_port->port;
            }
        }
    } else
#endif

#if defined(BCM_KATANA2_SUPPORT)
         if (soc_feature(unit, soc_feature_linkphy_coe) ||
            soc_feature(unit, soc_feature_subtag_coe)) {
             if (is_local == 0) {
                 is_local_subport = 0;
                 if (_bcm_kt2_mod_is_coe_mod_check(unit, mod_id_idx) == BCM_E_NONE) {
                     rv = _bcm_kt2_modport_to_pp_port_get(
                              unit, mod_id_idx, port, &pp_port);
                     if (_bcm_kt2_is_pp_port_linkphy_subtag(unit, pp_port)){
                            is_local_subport = 1;
                            local_port = pp_port;
                     }
                 }
             }
         }
#endif

#if defined(BCM_KATANA2_SUPPORT) || defined(BCM_HGPROXY_COE_SUPPORT)
        if (is_local_subport) {
            rv = _bcm_esw_port_tab_set_without_portlock(
                     unit, local_port, _BCM_CPU_TABS_NONE, PORT_OPERATIONf,
                     0x0); /* Normal */
            BCM_IF_ERROR_RETURN(rv);

            /* Set TAG_ACTION_PROFILE_PTR */
            rv = bcm_tr_mpls_port_untagged_profile_reset(unit, local_port);
            BCM_IF_ERROR_RETURN(rv);
        } else
#endif
         if (is_local) {
             /* Convert system ports to physical ports */ 
             if (soc_feature(unit, soc_feature_sysport_remap)) { 
                 BCM_XLATE_SYSPORT_S2P(unit, &port); 
             }

             rv = soc_mem_field32_modify(unit, PORT_TABm, port,
                                           PORT_OPERATIONf, 0x0); /* NORMAL */
             BCM_IF_ERROR_RETURN(rv);
    
             /* Reset TAG_ACTION_PROFILE_PTR */
             rv = bcm_tr_mpls_port_untagged_profile_reset(unit, port);
             BCM_IF_ERROR_RETURN(rv);
         }

        if (sw_clear) {
            (void) bcm_tr_mpls_match_clear (unit, vp);
        }
    } else if  (mpls_info->match_key[vp].flags == 
                  _BCM_MPLS_PORT_MATCH_TYPE_TRUNK) {
         trunk_id = mpls_info->match_key[vp].trunk_id;
         rv = bcm_tr_mpls_match_trunk_delete(unit, trunk_id, vp);
         BCM_IF_ERROR_RETURN(rv);
         mpls_info->trunk_vp_map[trunk_id] = -1;

        if (sw_clear) {
            (void) bcm_tr_mpls_match_clear (unit, vp);
        }
    } else if ((mpls_info->match_key[vp].flags & 
                     _BCM_MPLS_PORT_MATCH_TYPE_LABEL) ||
                   (mpls_info->match_key[vp].flags == 
                     _BCM_MPLS_PORT_MATCH_TYPE_LABEL_PORT)) {
        mpls_entry_entry_t ment, ret_ment;
        int pw_cnt;

        sal_memset(&ment, 0, sizeof(mpls_entry_entry_t));
        sal_memset(&ret_ment, 0, sizeof(mpls_entry_entry_t));
        if (vpless_failover_port) {
            if (mpls_info->match_key[vp].fo_modid != -1) {
                soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS__MODULE_IDf,
                                        mpls_info->match_key[vp].fo_modid);
                soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS__PORT_NUMf,
                                        mpls_info->match_key[vp].fo_port);
            } else {
                soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS__Tf, 1);
                soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS__TGIDf,
                                       mpls_info->match_key[vp].fo_trunk_id);
            }
        } else {
            if (mpls_info->match_key[vp].modid != -1) {
                soc_MPLS_ENTRYm_field32_set(unit, &ment, MODULE_IDf, 
                                        mpls_info->match_key[vp].modid);
                soc_MPLS_ENTRYm_field32_set(unit, &ment, PORT_NUMf, 
                                        mpls_info->match_key[vp].port);
            } else {
                soc_MPLS_ENTRYm_field32_set(unit, &ment, Tf, 1);
                soc_MPLS_ENTRYm_field32_set(unit, &ment, TGIDf, 
                                        mpls_info->match_key[vp].trunk_id);
            }
        }
        soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_LABELf,
                                    mpls_info->match_key[vp].match_label);

        rv = soc_mem_search(unit, MPLS_ENTRYm, MEM_BLOCK_ANY, &src_trk_idx,
                                    &ment, &ret_ment, 0);

        if (rv == SOC_E_NONE) {
            if  (soc_MPLS_ENTRYm_field32_get(unit, &ret_ment, PW_TERM_NUM_VALIDf)) {
                pw_cnt = soc_MPLS_ENTRYm_field32_get(unit, &ret_ment, PW_TERM_NUMf);
                (void) _bcm_tr_mpls_pw_term_counter_delete (unit, pw_cnt);
                 soc_MPLS_ENTRYm_field32_set(unit, &ret_ment, PW_TERM_NUM_VALIDf, 0);
                 soc_MPLS_ENTRYm_field32_set(unit, &ret_ment, PW_TERM_NUMf, 0);
            }

#ifdef BCM_TRIUMPH2_SUPPORT
            if (soc_feature(unit, soc_feature_gport_service_counters)) {
                if ((0 != soc_MPLS_ENTRYm_field32_get(unit, &ret_ment,
                                                  USE_VINTF_CTR_IDXf)) &&
                      (0 != soc_MPLS_ENTRYm_field32_get(unit, &ret_ment,
                                                  VINTF_CTR_IDXf))) {
                       /* Release MPLS label stat use, if any */
                       _bcm_flex_stat_handle_t mpls_fsh;
                       _BCM_FLEX_STAT_HANDLE_CLEAR(mpls_fsh);
                       _BCM_FLEX_STAT_HANDLE_COPY(mpls_fsh, ment);
                       _bcm_esw_flex_stat_ext_handle_free(unit,
                                                   _bcmFlexStatTypeMplsLabel,
                                                   mpls_fsh);
                }
            }
#endif /* BCM_TRIUMPH2_SUPPORT  */

            rv =  soc_mem_delete(unit, MPLS_ENTRYm, MEM_BLOCK_ANY, &ret_ment);

            if ((rv == BCM_E_NOT_FOUND) || (rv == BCM_E_NONE)) {
                if (vpless_failover_port) {
                    mpls_info->match_key[vp].fo_trunk_id = -1;
                    mpls_info->match_key[vp].fo_port = 0;
                    mpls_info->match_key[vp].fo_modid = -1;
                } else {
                    if (sw_clear) {
                        if (mpls_info->match_key[vp].flags & 
                            _BCM_MPLS_PORT_MATCH_TYPE_NONE) {
                            bcm_tr_mpls_port_match_count_adjust(unit, vp, -1);
                        }
                        (void) bcm_tr_mpls_match_clear(unit, vp);
                    }
                }
            }
            BCM_IF_ERROR_RETURN(rv);
        }
    }   
#if defined(BCM_KATANA2_SUPPORT)
    else if ((mpls_info->match_key[vp].flags == 
                BCM_MPLS_PORT_MATCH_PORT_SUBPORT_PKT_VID) ||
            (mpls_info->match_key[vp].flags == 
                BCM_MPLS_PORT_MATCH_PORT_SUBPORT_PKT_VID_INNER_VLAN) ||
            (mpls_info->match_key[vp].flags == 
                BCM_MPLS_PORT_MATCH_PORT_SUBPORT_PKT_VID_OUTER_VLAN)) {

        vlan_xlate_entry_t vent;
        int is_local = 0;
        
        BCM_IF_ERROR_RETURN
             ( _bcm_esw_modid_is_local(unit, mod_id_idx, &is_local));
        sal_memset(&vent, 0, sizeof(vlan_xlate_entry_t));

        /* default to SGLP type. TD2 */
        if (SOC_MEM_FIELD_VALID(unit,VLAN_XLATEm,SOURCE_TYPEf)) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, SOURCE_TYPEf, 1);
        }

        soc_VLAN_XLATEm_field32_set(unit, &vent, VALIDf, 1);
        if (mpls_info->match_key[vp].flags == 
                BCM_MPLS_PORT_MATCH_PORT_SUBPORT_PKT_VID) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf, 
                                        KT2_PT_XLT_KEY_TYPE_LLVID);        
        } else if (mpls_info->match_key[vp].flags == 
                BCM_MPLS_PORT_MATCH_PORT_SUBPORT_PKT_VID_OUTER_VLAN) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf, 
                                        KT2_PT_XLT_KEY_TYPE_LLVID_OVID);   
            soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__OVIDf,
                                    mpls_info->match_key[vp].match_vlan);
        } else {
            soc_VLAN_XLATEm_field32_set(unit, &vent, KEY_TYPEf, 
                                        KT2_PT_XLT_KEY_TYPE_LLVID_IVID); 
            soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__IVIDf,
                                    mpls_info->match_key[vp].match_inner_vlan);
        }
        
        soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__LLVIDf,
                                    mpls_info->match_key[vp].match_subport_pkt_vid);
        if (mpls_info->match_key[vp].modid != -1) {
            soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__MODULE_IDf, 
                                        mpls_info->match_key[vp].modid);
            soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__PORT_NUMf, 
                                        mpls_info->match_key[vp].port);
        } else {
            soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__Tf, 1);
            soc_VLAN_XLATEm_field32_set(unit, &vent, LLTAG__TGIDf, 
                                        mpls_info->match_key[vp].trunk_id);
        }
        rv = soc_mem_delete(unit, VLAN_XLATEm, MEM_BLOCK_ANY, &vent);
        BCM_IF_ERROR_RETURN(rv);
        if (is_local) {
            pp_port = mpls_info->match_key[vp].port;
            if (soc_feature(unit, soc_feature_sysport_remap)) { 
                BCM_XLATE_SYSPORT_S2P(unit, &pp_port); 
            }

            rv = soc_mem_field32_modify(unit, PORT_TABm, pp_port,
                            VT_KEY_TYPEf, KT2_PT_XLT_KEY_TYPE_OVID);/*default*/ 
            BCM_IF_ERROR_RETURN(rv);  
            rv = soc_mem_field32_modify(unit, PORT_TABm, pp_port,
                            VT_PORT_TYPE_SELECTf, 1);/*default*/ 
            BCM_IF_ERROR_RETURN(rv);  
        }
        
        if (sw_clear) {
            (void) bcm_tr_mpls_match_clear (unit, vp);
            bcm_tr_mpls_port_match_count_adjust(unit, vp, -1);
        }
    }
#endif /* end BCM_KATANA2_SUPPORT */
    return rv;
}

/*
 * Function:
 *      _bcm_tr_mpls_match_get
 * Purpose:
 *      Obtain match information of an MPLS port
 * Parameters:
 *      unit    - (IN) Device Number
 *      mpls_port - (OUT) mpls port information
 *      vp  - (IN) Source Virtual Port
 * Returns:
 *      BCM_E_XXX
 */

int
_bcm_tr_mpls_match_get(int unit, bcm_mpls_port_t *mpls_port, int vp)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int num_bits_for_port, rv = BCM_E_NONE;
    uint32 port_mask;
    bcm_module_t mod_in=0, mod_out;
    bcm_port_t port_in=0, port_out;
    bcm_trunk_t trunk_id=0;
    int    src_trk_idx=0;    /*Source Trunk table index.*/
    int    mode_in=0;   /* Module_Id */
    int vpless_failover_port = FALSE; /* the failover port without a dedicated vp */
    int match_trunk_id;
    int match_modid;
    int match_port;

    if (_BCM_MPLS_GPORT_FAILOVER_VPLESS_GET(vp)) {    
        vp = _BCM_MPLS_GPORT_FAILOVER_VPLESS_CLEAR(vp);

        if (!(mpls_info->match_key[vp].flags & 
             _BCM_MPLS_PORT_MATCH_TYPE_LABEL)) {
              vpless_failover_port = TRUE;
         } 
    }

    if (vpless_failover_port == FALSE) {
        match_trunk_id = mpls_info->match_key[vp].trunk_id;
        match_modid    = mpls_info->match_key[vp].modid;
        match_port     = mpls_info->match_key[vp].port;
    } else {
        match_trunk_id = mpls_info->match_key[vp].fo_trunk_id;
        match_modid    = mpls_info->match_key[vp].fo_modid;
        match_port     = mpls_info->match_key[vp].fo_port;
    }
    
    /* Get module id for unit. */
    BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &mode_in));

    if  (mpls_info->match_key[vp].flags &  _BCM_MPLS_PORT_MATCH_TYPE_VLAN) {

         mpls_port->criteria = BCM_MPLS_PORT_MATCH_PORT_VLAN;
         mpls_port->match_vlan = mpls_info->match_key[vp].match_vlan;

        if (mpls_info->match_key[vp].trunk_id != -1) {
             trunk_id = mpls_info->match_key[vp].trunk_id;
             BCM_GPORT_TRUNK_SET(mpls_port->port, trunk_id);
        } else {
             port_in = mpls_info->match_key[vp].port;
             mod_in = mpls_info->match_key[vp].modid;
             rv = _bcm_esw_stk_modmap_map(unit, BCM_STK_MODMAP_GET,
                                  mod_in, port_in, &mod_out, &port_out);
             BCM_GPORT_MODPORT_SET(mpls_port->port, mod_out, port_out);
        }
    } else if (mpls_info->match_key[vp].flags &  
                 _BCM_MPLS_PORT_MATCH_TYPE_INNER_VLAN) {
         mpls_port->criteria = BCM_MPLS_PORT_MATCH_PORT_INNER_VLAN;
         mpls_port->match_inner_vlan = mpls_info->match_key[vp].match_inner_vlan;

        if (mpls_info->match_key[vp].trunk_id != -1) {
             trunk_id = mpls_info->match_key[vp].trunk_id;
             BCM_GPORT_TRUNK_SET(mpls_port->port, trunk_id);
        } else {
             port_in = mpls_info->match_key[vp].port;
             mod_in = mpls_info->match_key[vp].modid;
             rv = _bcm_esw_stk_modmap_map(unit, BCM_STK_MODMAP_GET,
                                  mod_in, port_in, &mod_out, &port_out);
             BCM_GPORT_MODPORT_SET(mpls_port->port, mod_out, port_out);
        }
    }else if (mpls_info->match_key[vp].flags &
                    _BCM_MPLS_PORT_MATCH_TYPE_VLAN_STACKED) {

         mpls_port->criteria = BCM_MPLS_PORT_MATCH_PORT_VLAN_STACKED;
         mpls_port->match_vlan = mpls_info->match_key[vp].match_vlan;
         mpls_port->match_inner_vlan = mpls_info->match_key[vp].match_inner_vlan;

         if (mpls_info->match_key[vp].trunk_id != -1) {
              trunk_id = mpls_info->match_key[vp].trunk_id;
              BCM_GPORT_TRUNK_SET(mpls_port->port, trunk_id);
         } else {
              port_in = mpls_info->match_key[vp].port;
              mod_in = mpls_info->match_key[vp].modid;
              rv = _bcm_esw_stk_modmap_map(unit, BCM_STK_MODMAP_GET,
                             mod_in, port_in, &mod_out, &port_out);
              BCM_GPORT_MODPORT_SET(mpls_port->port, mod_out, port_out);
         }
    } else if  (mpls_info->match_key[vp].flags &  _BCM_MPLS_PORT_MATCH_TYPE_VLAN_PRI) {

         mpls_port->criteria = BCM_MPLS_PORT_MATCH_VLAN_PRI;
         mpls_port->match_vlan = mpls_info->match_key[vp].match_vlan;

        if (mpls_info->match_key[vp].trunk_id != -1) {
             trunk_id = mpls_info->match_key[vp].trunk_id;
             BCM_GPORT_TRUNK_SET(mpls_port->port, trunk_id);
        } else {
             port_in = mpls_info->match_key[vp].port;
             mod_in = mpls_info->match_key[vp].modid;
             rv = _bcm_esw_stk_modmap_map(unit, BCM_STK_MODMAP_GET,
                                  mod_in, port_in, &mod_out, &port_out);
             BCM_GPORT_MODPORT_SET(mpls_port->port, mod_out, port_out);
        }
    } else if (mpls_info->match_key[vp].flags &
                   _BCM_MPLS_PORT_MATCH_TYPE_PORT) {

         src_trk_idx = mpls_info->match_key[vp].index;

         if (soc_feature(unit, soc_feature_src_modid_base_index)) {
              source_trunk_map_modbase_entry_t modbase_entry;

              BCM_IF_ERROR_RETURN
                   (soc_mem_read(unit, SOURCE_TRUNK_MAP_MODBASEm, 
                                       MEM_BLOCK_ANY,  mode_in, &modbase_entry));
              port_in = src_trk_idx - soc_mem_field32_get(unit, 
                                       SOURCE_TRUNK_MAP_MODBASEm,
                                       &modbase_entry, BASEf);
         } else {
              num_bits_for_port =
                   _shr_popcount((unsigned int)SOC_PORT_ADDR_MAX(unit));
                                       port_mask = (1 << num_bits_for_port) - 1;

              port_in = src_trk_idx & port_mask;
              mod_in = src_trk_idx >> num_bits_for_port;
         }

        mpls_port->criteria = BCM_MPLS_PORT_MATCH_PORT;
        rv = _bcm_esw_stk_modmap_map(unit, BCM_STK_MODMAP_GET,
                               mod_in, port_in, &mod_out, &port_out);
         BCM_GPORT_MODPORT_SET(mpls_port->port, mod_out, port_out);
    }else if (mpls_info->match_key[vp].flags &
                  _BCM_MPLS_PORT_MATCH_TYPE_TRUNK) {

         trunk_id = mpls_info->match_key[vp].trunk_id;
         mpls_port->criteria = BCM_MPLS_PORT_MATCH_PORT;
         BCM_GPORT_TRUNK_SET(mpls_port->port, trunk_id);
    } else if ((mpls_info->match_key[vp].flags & 
                _BCM_MPLS_PORT_MATCH_TYPE_LABEL) ||
                (mpls_info->match_key[vp].flags == 
                 _BCM_MPLS_PORT_MATCH_TYPE_LABEL_PORT)) {
        mpls_entry_entry_t ment, ret_ment;

        sal_memset(&ment, 0, sizeof(mpls_entry_entry_t));
        sal_memset(&ret_ment, 0, sizeof(mpls_entry_entry_t));

         if (mpls_info->match_key[vp].flags & 
             _BCM_MPLS_PORT_MATCH_TYPE_LABEL) {
              mpls_port->criteria = BCM_MPLS_PORT_MATCH_LABEL;
         } else if (mpls_info->match_key[vp].flags & 
                    _BCM_MPLS_PORT_MATCH_TYPE_LABEL_PORT) {
              mpls_port->criteria = BCM_MPLS_PORT_MATCH_LABEL_PORT; 
         }
         mpls_port->match_label = mpls_info->match_key[vp].match_label;

         if (match_trunk_id != -1) {
              trunk_id = match_trunk_id;
              soc_MPLS_ENTRYm_field32_set(unit, &ment, Tf, 1);
              soc_MPLS_ENTRYm_field32_set(unit, &ment, TGIDf, trunk_id);
              BCM_GPORT_TRUNK_SET(mpls_port->port, trunk_id);
         } else {
              mod_in = match_modid;
              port_in = match_port;
              soc_MPLS_ENTRYm_field32_set(unit, &ment, MODULE_IDf, mod_in);
              soc_MPLS_ENTRYm_field32_set(unit, &ment, PORT_NUMf, port_in);

              BCM_IF_ERROR_RETURN(_bcm_esw_stk_modmap_map(unit, 
                          BCM_STK_MODMAP_GET, mod_in, port_in, 
                          &mod_out, &port_out));
              BCM_GPORT_MODPORT_SET(mpls_port->port, mod_out, port_out);
         }
         soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_LABELf, 
                                     mpls_info->match_key[vp].match_label);
 
         rv = soc_mem_search(unit, MPLS_ENTRYm, MEM_BLOCK_ANY, &src_trk_idx,
                             &ment, &ret_ment, 0);
 
         if (rv == SOC_E_NONE) {
              if (soc_MPLS_ENTRYm_field32_get(unit, &ret_ment, 
                                            CW_CHECK_CTRLf) == 0x3) {
                   
                   /* Control word is present, and check sequence number */
                   mpls_port->flags |= BCM_MPLS_PORT_SEQUENCED;
              } else if (soc_MPLS_ENTRYm_field32_get(unit, &ret_ment, 
                                            CW_CHECK_CTRLf) == 0x1) {
                   mpls_port->flags |= BCM_MPLS_PORT_CONTROL_WORD;
              }

              if (soc_MPLS_ENTRYm_field32_get(unit, &ret_ment, PW_TERM_NUM_VALIDf)) {
                   mpls_port->flags |= BCM_MPLS_PORT_COUNTED;
              }

#if defined(BCM_TRIDENT2PLUS_SUPPORT)
              else if (SOC_MEM_FIELD_VALID(unit, MPLS_ENTRYm, CLASS_IDf)) {
                  mpls_port->class_id = soc_MPLS_ENTRYm_field32_get(unit, &ret_ment,
                                                                    CLASS_IDf);
              }
#endif

#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
              if (soc_feature(unit, soc_feature_mpls_entropy)) {
                   if (soc_MPLS_ENTRYm_field32_get(unit, &ret_ment, ENTROPY_LABEL_PRESENTf)) {
                        mpls_port->flags2 |= BCM_MPLS_PORT2_INGRESS_ENTROPY_ENABLE;
                   }
              }
              /* VCCV defines 3 control-channels */
              if (soc_mem_field_valid(unit, MPLS_ENTRYm, PW_CC_TYPEf)) {
                  mpls_port->vccv_type = soc_MPLS_ENTRYm_field32_get(unit, 
                                       &ret_ment, PW_CC_TYPEf);
              }
#endif /* defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT_SUPPORT) */
              if (soc_MPLS_ENTRYm_field32_get(unit, &ret_ment, 
                                        DECAP_USE_EXP_FOR_PRIf) == 0x1) {
                   /* MAP_PRI_AND_CNG */
                   /* Use specified EXP-map to determine internal prio/color */
                   mpls_port->exp_map = 
                        soc_MPLS_ENTRYm_field32_get(unit, &ret_ment, EXP_MAPPING_PTRf);
                   mpls_port->exp_map |= _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_INGRESS;
                   mpls_port->flags |= BCM_MPLS_PORT_INT_PRI_MAP;
              } else if (soc_MPLS_ENTRYm_field32_get(unit, &ret_ment, 
                                               DECAP_USE_EXP_FOR_PRIf) == 0x2) {
                   /* NEW_PRI_MAP_CNG */
                   /* Use the specified internal priority value */
                   mpls_port->int_pri = 
                        soc_MPLS_ENTRYm_field32_get(unit, &ret_ment, NEW_PRIf);
                   mpls_port->flags |= BCM_MPLS_PORT_INT_PRI_SET;

                   /* Use specified EXP-map to determine internal color */
                   mpls_port->exp_map = 
                        soc_MPLS_ENTRYm_field32_get(unit, &ret_ment, EXP_MAPPING_PTRf);
                   mpls_port->exp_map |= _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_INGRESS;
                   mpls_port->flags |= BCM_MPLS_PORT_COLOR_MAP;
              } else {
                   if (soc_mem_field_valid(unit, MPLS_ENTRYm,
                                        TRUST_OUTER_DOT1P_PTRf)) {
                        if (soc_MPLS_ENTRYm_field32_get(unit, &ret_ment, 
                                               TRUST_OUTER_DOT1P_PTRf) == 
                                               _BCM_TR_MPLS_PRI_CNG_MAP_NONE) {
                          /*
                           * Don't trust the encapsulated packet's 802.1p pri/cfi.
                           * Use a fixed value for internal priority.
                          */
                           mpls_port->int_pri = 
                                soc_MPLS_ENTRYm_field32_get(unit, &ret_ment, NEW_PRIf);
                           mpls_port->flags |= BCM_MPLS_PORT_INT_PRI_SET;
                        }
                   }
              }
         }
#ifdef BCM_TOMAHAWK2_SUPPORT
        if (soc_feature(unit, soc_feature_mpls_ecn)) {
            int ecn_map_id;
            int ecn_map_hw_idx;
            int ret;
            ecn_map_hw_idx = soc_MPLS_ENTRYm_field32_get(unit, &ret_ment, 
                                                         EXP_TO_IP_ECN_MAPPING_PTRf);
            ret = bcmi_ecn_map_hw_idx2id(unit, ecn_map_hw_idx, 
                                         _BCM_XGS5_MPLS_ECN_MAP_TYPE_EXP2ECN, 
                                         &ecn_map_id);
            if (BCM_SUCCESS(ret)) {
                mpls_port->flags2 |= BCM_MPLS_PORT2_INGRESS_ECN_MAP;
                mpls_port->ecn_map_id = ecn_map_id;
            }
        }
#endif 
    } else if (mpls_info->match_key[vp].flags & _BCM_MPLS_PORT_MATCH_TYPE_SHARE) {
        mpls_port->criteria = BCM_MPLS_PORT_MATCH_SHARE;
    } else {
        mpls_port->criteria = BCM_MPLS_PORT_MATCH_NONE;
    }
    return rv;
}

/*
 * Function:
 *      _bcm_tr_mpls_nh_drop
 * Purpose:
 *      Set MPLS Next-Hop based Drop setting
 * Parameters:
 *      unit    - (IN) Device Number
 *      drop   - (IN) Drop setting
 * Returns:
 *      BCM_E_XXX
 */
int
_bcm_tr_mpls_nh_drop(int unit, int vp, int drop) 
{
    int nh_index;
    ing_dvp_table_entry_t dvp;

    BCM_IF_ERROR_RETURN (READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp, &dvp));
    nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp, NEXT_HOP_INDEXf);
    return soc_mem_field32_modify(unit, ING_L3_NEXT_HOPm, nh_index, DROPf, drop);
}

/*
 * Function:
 *      _bcm_tr_mpls_l2vpn_source_port_set
 * Purpose:
 *      Set L2 VPN source Modid/port 
 * Parameters:
 *      unit    - (IN) Device Number
 *      mpls_port     - (IN) MPLS port
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
_bcm_tr_mpls_l2vpn_source_port_set(int unit, bcm_mpls_port_t *mpls_port, source_vp_entry_t *svp)
{
    int rv = BCM_E_NONE, gport_id;
    bcm_module_t mod_in;
    bcm_port_t port_in;
    bcm_trunk_t trunk_id;

         rv = _bcm_esw_gport_resolve(unit, mpls_port->mpls_port_id, &mod_in, 
                                &port_in, &trunk_id, &gport_id);
         BCM_IF_ERROR_RETURN(rv);
         if (mod_in != -1) {
             soc_SOURCE_VPm_field32_set(unit, svp, SRC_MODIDf, mod_in);
         }
         if (port_in != -1) {
             soc_SOURCE_VPm_field32_set(unit, svp, SRC_PORTf, port_in);
         }
         return rv;
}

/*
 * Function:
 *      bcmi_mpls_clear_mpls_entry_for_vp
 * Purpose:
 *      This function checks whether to clear MPLS entry for 
 *      this vpws vp. THis could be a backup port so
 *      we may not need to clean the entry as entry should be cleaned
 *      only when primary port is removed.
 * Parameters:
 *      unit    - (IN) Device Number
 *      vpn     - (IN) vpn
 *      vp      - (IN) virtual port
 * Returns:
 *      TRUE/FALSE
 */
int
bcmi_mpls_clear_mpls_entry_for_vp(int unit, bcm_vpn_t vpn, int vp)
{

    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int max_vpws_vpn, num_vp;

    num_vp = soc_mem_index_count(unit, SOURCE_VPm);
    max_vpws_vpn = num_vp / 2;

    if (vpn <= max_vpws_vpn) {
        /* if vp3 port is not valid then vp2 could be a primary port */
        if (mpls_info->vpws_vp_map[vpn].vp3 == -1) {
            /* if vp3 in invalid and vp2 is valid then vp2 is
               first backup port to be created so we should
               remove the mpls entry.
               For all others we should not remove the mpls
               label entry.
             */
            if (mpls_info->vpws_vp_map[vpn].vp2 == vp) {
                return 0;
            } else {
                return 1;
            }
         } else {
             if ((mpls_info->vpws_vp_map[vpn].vp2 == vp) &&
                 (sal_memcmp(&mpls_info->match_key[vp],
                          &mpls_info->match_key[mpls_info->vpws_vp_map[vpn].vp3],
                          sizeof(_bcm_tr_mpls_match_port_info_t)))) {
                 return 0;
             } else {
                 return 1;
             }
         }
    }
    return 0;
}

/*
 * Function:
 *      _bcm_tr_mpls_vpws_vp_map_backup_port_clear
 * Purpose:
 *      Clear VPWS VPN member Virtual backup ports
 * Parameters:
 *      unit    - (IN) Device Number
 *      vpn     - (IN) VPN instance ID
 *      vp     - (IN) Network Virtual Port
 * Returns:
 *      void
 */
STATIC void
_bcm_tr_mpls_vpws_vp_map_backup_port_clear(int unit, bcm_vpn_t vpn, int vp)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int max_vpws_vpn, num_vp;

    num_vp = soc_mem_index_count(unit, SOURCE_VPm);
    max_vpws_vpn = num_vp / 2;

    if (vpn <= max_vpws_vpn) {

        if (mpls_info->vpws_vp_map[vpn].vp_b_1 == vp) {
            (void) _bcm_vp_free(unit, _bcmVpTypeMpls, 1,
                        mpls_info->vpws_vp_map[vpn].vp_b_1 );
            mpls_info->vpws_vp_map[vpn].vp_b_1 = -1;
        } else if ( mpls_info->vpws_vp_map[vpn].vp_b_2 == vp) {
            (void) _bcm_vp_free(unit, _bcmVpTypeMpls, 1,
                        mpls_info->vpws_vp_map[vpn].vp_b_2 );
            mpls_info->vpws_vp_map[vpn].vp_b_2 = -1;
        } else if ( mpls_info->vpws_vp_map[vpn].vp_b_3 == vp) {
            (void) _bcm_vp_free(unit, _bcmVpTypeMpls, 1,
                        mpls_info->vpws_vp_map[vpn].vp_b_3 );
            mpls_info->vpws_vp_map[vpn].vp_b_3 = -1;
        } else if(mpls_info->vpws_vp_map[vpn].vp2 == vp) {
            (void) _bcm_vp_free(unit, _bcmVpTypeMpls, 1,
                        mpls_info->vpws_vp_map[vpn].vp2);
            mpls_info->vpws_vp_map[vpn].vp2 = -1;
        }
    }
}

/*
 * Function:
 *      bcmi_mpls_port_failover_update_is_required
 * Purpose:
 *      Check if update to failover entry is required at this nh index.
 * Parameters:
 *      unit              - (IN) Device Number
 *      mpls_port         - (IN) Mpls Port Id
 *      primary_nh_index  - (IN) primary next hop index
 *      nh_index          - (IN) next hop index
 * Returns:
 *      TRUE/FALSE
 */
int
bcmi_mpls_port_failover_update_is_required(int unit, bcm_mpls_port_t *mpls_port,
                                 bcm_if_t primary_nh_index, bcm_if_t nh_index)
{

    if (_BCM_FAILOVER_IS_MULTI_LEVEL(mpls_port->failover_id)) {
        /* in case of multicast group, we need to create entry */
        if (mpls_port->failover_mc_group > 1) {
            return 1;
        }
        /*
         * if backup nh index is next to primary then create entry.
         * Also make sure that primary nh is start of nh chain.
         */
        if (((primary_nh_index+1) == nh_index) &&
            (bcmi_l3_nh_multi_count_get(unit, primary_nh_index) == 4)) {
            return 1;
        }
        /*
         *  if we are adding a backup port where primary is not in same chain
         *  then backup will have chain of 4. this is case where we have
         *  one separate primary and 4 other backup ports.
         *  In this case, primary nh will point to double wide prot entry
         *  and should have a chain of 2 nhs.
         */
        if ((bcmi_l3_nh_multi_count_get(unit, nh_index) == 4) &&
           ((bcmi_l3_nh_multi_count_get(unit, primary_nh_index) == 2))) {
            return 1;
        } else {
            return 0;
        }
    }
    return 1;
}

/*
 * Function:
 *      _bcm_tr_mpls_vpws_vp_map_set
 * Purpose:
 *      Set VPWS VPN member Virtual ports
 * Parameters:
 *      unit    - (IN) Device Number
 *      vpn     - (IN) VPN instance ID
 *      vp1     - (IN) Access Virtual Port
 *      vp2     - (IN) Network Virtual Port
 *      vp3     - (IN) Primary Network VPort
 * Returns:
 *      BCM_E_XXX
 */
STATIC void
_bcm_tr_mpls_vpws_vp_map_set (int unit, bcm_vpn_t vpn, int vp1, int vp2, 
                              int vp3) 
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int max_vpws_vpn, num_vp;

    num_vp = soc_mem_index_count(unit, SOURCE_VPm);
    max_vpws_vpn = num_vp / 2;

    if ( vpn <= max_vpws_vpn ) {
            mpls_info->vpws_vp_map[vpn].vp1 = vp1;
            mpls_info->vpws_vp_map[vpn].vp2 = vp2;
            mpls_info->vpws_vp_map[vpn].vp3 = vp3;
        }
}

/*
 * Function:
 *      bcmi_mpls_vpws_vp_map_backup_port_set
 * Purpose:
 *      Set VPWS VPN member Virtual backup ports
 * Parameters:
 *      unit    - (IN) Device Number
 *      vpn     - (IN) VPN instance ID
 *      vp     - (IN) Network Virtual Port
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
bcmi_mpls_vpws_vp_map_backup_port_set(int unit, bcm_vpn_t vpn, int vp)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int max_vpws_vpn, num_vp;
    if (!(soc_feature(unit, soc_feature_hierarchical_protection))) {
        return BCM_E_NONE;
    }

    num_vp = soc_mem_index_count(unit, SOURCE_VPm);
    max_vpws_vpn = num_vp / 2;

    if (vpn <= max_vpws_vpn) {
        if (mpls_info->vpws_vp_map[vpn].vp_b_1 == -1) {
            mpls_info->vpws_vp_map[vpn].vp_b_1 = vp;
        } else if ( mpls_info->vpws_vp_map[vpn].vp_b_2 == -1) {
            mpls_info->vpws_vp_map[vpn].vp_b_2 = vp;
        } else if ( mpls_info->vpws_vp_map[vpn].vp_b_3 == -1) {
            mpls_info->vpws_vp_map[vpn].vp_b_3 = vp;
        } else {
            return BCM_E_CONFIG;
        }
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      bcmi_mpls_match_backup_ports
 * Purpose:
 *      Match a virtual port in VPWS VPN member Virtual backup ports
 * Parameters:
 *      unit    - (IN) Device Number
 *      vpn     - (IN) VPN instance ID
 *      vp     - (IN) Network Virtual Port
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
bcmi_mpls_match_backup_ports(int unit, bcm_vpn_t vpn, int vp)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int max_vpws_vpn, num_vp;

    if (!(soc_feature(unit, soc_feature_hierarchical_protection))) {
        return 0;
    }

    num_vp = soc_mem_index_count(unit, SOURCE_VPm);
    max_vpws_vpn = num_vp / 2;

    if (vpn <= max_vpws_vpn) {
        if ((mpls_info->vpws_vp_map[vpn].vp_b_1 == vp) ||
            (mpls_info->vpws_vp_map[vpn].vp_b_2 == vp) ||
                ( mpls_info->vpws_vp_map[vpn].vp_b_3 == vp)) {
            return 1;
        }
    }
    return 0;
}

/*
 * Function:
 *      bcmi_mpls_update_backup_svp
 * Purpose:
 *      Update source vp for a backup port.
 * Parameters:
 *      unit    - (IN) Device Number
 *      vp      - (IN) source virtual port
 *      dvp     - (IN) Destination Virtual Port
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
bcmi_mpls_update_backup_svp(int unit, int vp, int dvp)
{
    source_vp_entry_t svp;

    BCM_IF_ERROR_RETURN(READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp));
    soc_SOURCE_VPm_field32_set(unit, &svp, DVPf, dvp);
    BCM_IF_ERROR_RETURN(WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp, &svp));

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcmi_mpls_update_dvp_in_backup_ports
 * Purpose:
 *      Update destination vp for a matching backup port.
 * Parameters:
 *      unit     - (IN) Device Number
 *      vpn      - (IN) VPN instance ID
 *      dvp      - (IN) Destination Virtual Port
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
bcmi_mpls_update_dvp_in_backup_ports(int unit, bcm_vpn_t vpn, int uni_vp)
{
   _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int max_vpws_vpn, num_vp;

    if (!(soc_feature(unit, soc_feature_hierarchical_protection))) {
        return BCM_E_NONE;
    }

    num_vp = soc_mem_index_count(unit, SOURCE_VPm);
    max_vpws_vpn = num_vp / 2;

    if (uni_vp == -1) {
        /* clearing the dvp from entry */
        uni_vp = 0;
    }

    if (vpn <= max_vpws_vpn) {
        if (mpls_info->vpws_vp_map[vpn].vp_b_1 != -1) {
            BCM_IF_ERROR_RETURN(bcmi_mpls_update_backup_svp(unit,
                mpls_info->vpws_vp_map[vpn].vp_b_1, uni_vp));
        }

        if (mpls_info->vpws_vp_map[vpn].vp_b_2 != -1) {
            BCM_IF_ERROR_RETURN(bcmi_mpls_update_backup_svp(unit,
                mpls_info->vpws_vp_map[vpn].vp_b_2, uni_vp));
        }

        if (mpls_info->vpws_vp_map[vpn].vp_b_3 != -1) {
            BCM_IF_ERROR_RETURN(bcmi_mpls_update_backup_svp(unit,
                mpls_info->vpws_vp_map[vpn].vp_b_3, uni_vp));
        }

    }
    return BCM_E_NONE;
}

STATIC int
bcmi_mpls_failover_nhi_cleanup_check(int unit, bcm_vpn_t vpn, int incoming_vp,
                              bcm_failover_t failover_id,
                              bcm_if_t failover_nh_index,
                              bcm_if_t primary_nh_index,
                              bcm_if_t nh_index, int *cleanup)
{

    uint32 vfi, vp;
    source_vp_entry_t svp;
    egr_l3_next_hop_entry_t egr_nh;
    int i, base_nh_index;

    if (_BCM_FAILOVER_IS_MULTI_LEVEL(failover_id)) {
        /* if the primary index is the base index of multi level group,
         * then its failover port should should have next nh index.
         */
        if ((primary_nh_index + 1) == failover_nh_index) {
            base_nh_index = primary_nh_index;
        } else {
            base_nh_index = failover_nh_index;
        }

        /*
         * in case of multi level failover, all the backup links
         * have to be in consecutive order.
         * therefore we can check if this incoming NH is part of
         * that group and vp on it is valid.
         */
        for (i=0 ; i<4; i++) {
             BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm,
                         MEM_BLOCK_ANY, base_nh_index + i, &egr_nh));
             /*
              * Reading dvp here and reading svp. we keep
              * svp and dvp same for a particular flow.
              */
             if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                 vp = soc_EGR_L3_NEXT_HOPm_field32_get(unit,
                         &egr_nh, MPLS__DVPf);
             } else {
                 vp = soc_EGR_L3_NEXT_HOPm_field32_get(unit,
                         &egr_nh, DVPf);
             }

            /* Check for the validity of the VP */
            if (!_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
                 continue;
            }

            /*
             * we should have some other port than incoming to see
             * if we want to delete the failover entry.
             */
            if (vp == incoming_vp) {
                continue;
            }
            _BCM_MPLS_VPN_GET(vfi, _BCM_MPLS_VPN_TYPE_VPLS, vpn);
            BCM_IF_ERROR_RETURN(READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp));
            if ((soc_SOURCE_VPm_field32_get
                (unit, &svp, ENTRY_TYPEf) != 0x1) ||
                (vfi != soc_SOURCE_VPm_field32_get(unit, &svp, VFIf))) {

                continue;
            }
            /* if the incoming NH is in the range of multi
             * level failover NH group then cleanup is not required.
             */
            if ((nh_index > base_nh_index) &&
                (nh_index < (base_nh_index + 4))) {
                *cleanup = 0;
            }
        }
    } else {
        if (failover_nh_index == nh_index) {
            *cleanup = 1;
        } else {
            *cleanup = 0;
        }
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_tr_mpls_vpws_vp_map_get
 * Purpose:
 *      Get VPWS VPN member Virtual ports
 * Parameters:
 *      unit    - (IN) Device Number
 *      vpn     - (IN) VPN instance ID
 *      vp1     - (OUT) Access Virtual Port
 *      vp2     - (OUT) Network Virtual Port
 *      vp3     - (OUT) Primary Network VPort.
 * Returns:
 *      BCM_E_XXX
 */

STATIC void
_bcm_tr_mpls_vpws_vp_map_get (int unit, bcm_vpn_t vpn, int *vp1, int *vp2,
                              int *vp3) 
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int max_vpws_vpn, num_vp;

    num_vp = soc_mem_index_count(unit, SOURCE_VPm);
    max_vpws_vpn = num_vp / 2;

    if ( vpn <= max_vpws_vpn ) {
         *vp1 = mpls_info->vpws_vp_map[vpn].vp1;
         *vp2 = mpls_info->vpws_vp_map[vpn].vp2;
         *vp3 = mpls_info->vpws_vp_map[vpn].vp3;
      }
}

/*
 * Function:
 *      _bcm_tr_mpls_vpws_vp_map_get
 * Purpose:
 *      Clear VPWS VPN member Virtual ports
 * Parameters:
 *      unit    - (IN) Device Number
 *      vpn     - (IN) VPN instance ID
 * Returns:
 *      BCM_E_XXX
 */

STATIC void
_bcm_tr_mpls_vpws_vp_map_clear (int unit, bcm_vpn_t vpn) 
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int max_vpws_vpn, num_vp;

    num_vp = soc_mem_index_count(unit, SOURCE_VPm);
    max_vpws_vpn = num_vp / 2;

    if ( vpn <= max_vpws_vpn ) {
        if (mpls_info->vpws_vp_map[vpn].vp1 != -1) {
            (void) _bcm_vp_free(unit, _bcmVpTypeMpls, 1, mpls_info->vpws_vp_map[vpn].vp1 );
            mpls_info->vpws_vp_map[vpn].vp1 = -1;
        }
        if (mpls_info->vpws_vp_map[vpn].vp2 != -1) {
            (void) _bcm_vp_free(unit, _bcmVpTypeMpls, 1, mpls_info->vpws_vp_map[vpn].vp2 );
             mpls_info->vpws_vp_map[vpn].vp2 = -1;
        }
        if (mpls_info->vpws_vp_map[vpn].vp3 != -1) {
             mpls_info->vpws_vp_map[vpn].vp3 = -1;
        }
    }
}

#if defined(BCM_TRIUMPH2_SUPPORT)
STATIC int
_bcm_mpls_source_vp_set(int unit, bcm_mpls_port_t* mpls_port, int active_vp)
{
    source_vp_entry_t svp;
    bcm_port_t local_port;
    int value;
    int rv = BCM_E_NONE;
    int dscp_map_chunk = 64, ing_pri_cng_chunk = 16;

    /* Sanity Checking */
    if (mpls_port == NULL || active_vp < 0) {
        return BCM_E_PARAM;
    }
    /* For Katana for each SVP set TRUST_DSCP_PTR in SOURCE_VP with
     * the TRUST_DSCP_PTR of the physical port
     */
    if (!(mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL)) {
        /* Deal with physical ports */
        if (BCM_GPORT_IS_SET(mpls_port->port) && !BCM_GPORT_IS_TRUNK(mpls_port->port)) {
            if (bcm_esw_port_local_get(unit, mpls_port->port, &local_port) < 0) {
                return BCM_E_PARAM;
            }
        } else {
            return BCM_E_PARAM;
        }

        if (!SOC_PORT_VALID(unit, local_port)) {
            return BCM_E_PARAM;
        }

        switch(mpls_port->criteria) {
        case BCM_MPLS_PORT_MATCH_PORT:
        case BCM_MPLS_PORT_MATCH_PORT_VLAN:
        case BCM_MPLS_PORT_MATCH_PORT_INNER_VLAN:
        case BCM_MPLS_PORT_MATCH_PORT_VLAN_STACKED:
            /*  Read PORT Table */
            if (!soc_mem_field_valid(unit, SOURCE_VPm, TRUST_DSCP_PTRf) ||
                !soc_mem_field_valid(unit, PORT_TABm, TRUST_DOT1P_PTRf) ||
                !soc_mem_field_valid(unit, SOURCE_VPm, TRUST_DOT1P_PTRf))  {
                return BCM_E_NONE;
            }

            rv = READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, active_vp, &svp);
            if (rv < 0) {
                return rv;
            }

            if (soc_feature(unit, soc_feature_dscp_map_per_port)) {
                int index_min = 0;
                /* Delete the old profile first */
                if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
                    value = soc_SOURCE_VPm_field32_get(unit, 
                                                       &svp, 
                                                       TRUST_DSCP_PTRf);
                    BCM_IF_ERROR_RETURN(
                        _bcm_dscp_table_entry_delete(unit, 
                                                     value * dscp_map_chunk));
                    
                    value = soc_SOURCE_VPm_field32_get(unit, 
                                                       &svp, 
                                                       TRUST_DOT1P_PTRf);
                    BCM_IF_ERROR_RETURN(
                        _bcm_ing_pri_cng_map_entry_delete(
                            unit, value * ing_pri_cng_chunk));
                }

                /* Get dscp table profile min index */
                BCM_IF_ERROR_RETURN(
                    _bcm_dscp_table_entry_range(unit, &index_min, NULL));

                /*
                 * Increase the profile ref_count 
                 * If index_min is zero, means there is no reserved dscp profile
                 * Otherwise need to handle specially
                 */
                if (index_min == 0) {
                    rv = _bcm_tr2_port_tab_get(unit, local_port, 
                                               TRUST_DSCP_PTRf, 
                                               &value);
                    if (rv == BCM_E_NONE) {
                        BCM_IF_ERROR_RETURN(
                            _bcm_dscp_table_entry_reference(
                                unit, (value * dscp_map_chunk), 
                                dscp_map_chunk));
                        soc_SOURCE_VPm_field32_set(unit, &svp, 
                                                   TRUST_DSCP_PTRf, value);
                    }
                } else {
                    /* Create a dscp profile, copy content from phy_port */
                    void    *dscp_table;
                    int     alloc_size;
                    void    *entries[2]; 

                    alloc_size = dscp_map_chunk *
                                 sizeof(dscp_table_entry_t);
                    dscp_table = soc_cm_salloc(unit, alloc_size,
                                               "TR2 dscp table");
                    if (NULL == dscp_table) {
                        return BCM_E_MEMORY;
                    }
                    entries[0] = dscp_table;
                    /* Read the port chunk */
                    rv = soc_mem_read_range(unit, DSCP_TABLEm, MEM_BLOCK_ANY, 
                                            local_port * dscp_map_chunk,
                                            (local_port + 1) * dscp_map_chunk - 1, 
                                            dscp_table); 
                    if (BCM_FAILURE(rv)) {
                        soc_cm_sfree(unit, dscp_table);
                        return rv;
                    }
                    /* Write the profile chunk */
                    rv = _bcm_dscp_table_entry_add(unit, entries, 
                                                   dscp_map_chunk, 
                                                   (uint32 *)&value);
                    if (BCM_FAILURE(rv)) {
                        soc_cm_sfree(unit, dscp_table);
                        return rv;
                    }

                    soc_cm_sfree(unit, dscp_table);
                    soc_SOURCE_VPm_field32_set(unit, &svp, TRUST_DSCP_PTRf, 
                                               (value / dscp_map_chunk));
                }
            }
			
            if (soc_feature(unit, soc_feature_color_prio_map_profile)) {
                /* Increase profile ref_count */
                rv = _bcm_tr2_port_tab_get(unit, local_port, TRUST_DOT1P_PTRf, 
                                           &value);
                if (rv == BCM_E_NONE) {
                    BCM_IF_ERROR_RETURN(
                        _bcm_ing_pri_cng_map_entry_reference(
                            unit, (value * ing_pri_cng_chunk), 
                            ing_pri_cng_chunk));
                    soc_SOURCE_VPm_field32_set(unit, &svp, TRUST_DOT1P_PTRf, 
                                               value);
                }
            }

            rv = _bcm_tr2_port_tab_get(unit, local_port, TRUST_DSCP_V4f, 
                                       &value);
            if (rv == BCM_E_NONE) {
                soc_SOURCE_VPm_field32_set(unit, &svp, TRUST_DSCP_V4f, value);
            }
            
            rv = _bcm_tr2_port_tab_get(unit, local_port, TRUST_DSCP_V6f, 
                                       &value);
            if (rv == BCM_E_NONE) {
                soc_SOURCE_VPm_field32_set(unit, &svp, TRUST_DSCP_V6f, value);
            }
            
            rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, active_vp, &svp);
            if (rv == BCM_E_NONE) {
                _BCM_MPLS_SVP_PROFILE_SET_USED_SET(unit, active_vp);
            }
            break;
        default:
            break;
        }
    }
    return rv;
}

STATIC int
_bcm_mpls_source_vp_clear(int unit, int criteria, int vp, bcm_vpn_t vpn)
{
    source_vp_entry_t svp;
    bcm_mpls_port_t mpls_port;
    int rv = BCM_E_NONE;
    int value = 0;
    int dscp_map_chunk = 64, ing_pri_cng_chunk = 16;

    if (!_BCM_MPLS_SVP_PROFILE_SET_USED_GET(unit, vp)) {
        return rv;
    }

    /* Decrease svp DSCP&DOT1P profiles ref_count */
    if ((criteria == _BCM_MPLS_PORT_MATCH_TYPE_PORT) ||
        (criteria == _BCM_MPLS_PORT_MATCH_TYPE_VLAN) ||
        (criteria == _BCM_MPLS_PORT_MATCH_TYPE_INNER_VLAN) ||
        (criteria == _BCM_MPLS_PORT_MATCH_TYPE_VLAN_STACKED) ||
        (criteria == _BCM_MPLS_PORT_MATCH_TYPE_TRUNK)) {

        /*  Read PORT Table */
        if (!soc_mem_field_valid(unit, SOURCE_VPm, TRUST_DSCP_PTRf) ||
            !soc_mem_field_valid(unit, PORT_TABm, TRUST_DOT1P_PTRf) ||
            !soc_mem_field_valid(unit, SOURCE_VPm, TRUST_DOT1P_PTRf))  {
            return BCM_E_NONE;
        }

        rv = READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp); 
        if (rv < 0) {
            return rv;
        }
        /* avoid using wrong index to delete DSCP entry
         * and get error return code,  0 is not always min index for DSCP
         * in some chips.
         */ 
        rv = _bcm_tr_mpls_port_get(unit, vpn, vp, &mpls_port);
        if (rv < 0) {
            return rv;
        }
        if (mpls_port.flags & BCM_MPLS_PORT_EGRESS_TUNNEL) {
            return BCM_E_NONE;
        }
        if (soc_feature(unit, soc_feature_dscp_map_per_port)) {
            if (soc_mem_field_valid(unit, SOURCE_VPm, TRUST_DSCP_PTRf)) {
                value = soc_SOURCE_VPm_field32_get(unit, 
                                                   &svp, 
                                                   TRUST_DSCP_PTRf);
                rv = _bcm_dscp_table_entry_delete(unit, value * dscp_map_chunk);
                if (rv != BCM_E_NONE) {
                    return rv;
                } else {
                    rv = BCM_E_NONE;
                }
            }
        }
        if (soc_feature(unit, soc_feature_color_prio_map_profile)) {
            if (soc_mem_field_valid(unit, SOURCE_VPm, TRUST_DOT1P_PTRf)) {
                value = soc_SOURCE_VPm_field32_get(unit, 
                                                   &svp, 
                                                   TRUST_DOT1P_PTRf);
                rv = _bcm_ing_pri_cng_map_entry_delete(unit, 
                                                       value * ing_pri_cng_chunk);
                if (rv != BCM_E_NONE) {
                    return rv;
                } else {
                    rv = BCM_E_NONE;
                }
            } 
        }
        _BCM_MPLS_SVP_PROFILE_SET_USED_CLR(unit, vp);
    }
    return rv;
}

/*
 * Function:
 *      bcm_tr_mpls_share_vp_bitmap_get
 * Purpose:
 *      Get shared VPs info per VPN
 * Parameters:
 *      unit       - (IN) Device Number
 *      vpn        - (IN) VPN that shared vp belong to
 *      vp_bitmap  - (OUT) shared vp bitmap
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_share_vp_bitmap_get(int unit, bcm_vpn_t vpn, SHR_BITDCL *vp_bitmap)
{
    vlan_xlate_entry_t *vent;
    soc_mem_t mem;
    int index_min = 0;
    int index_max = 0;
    uint8 *xlate_buf = NULL;
    int buf_size = 0;
    int i = 0;
    int rv = BCM_E_NONE;
    int vfi = -1;
    int vp = -1;

    if (vpn != BCM_MPLS_VPLS_VPN_INVALID) {
         _BCM_MPLS_VPN_GET(vfi, _BCM_MPLS_VPN_TYPE_VPLS, vpn);
         if (!_bcm_vfi_used_get(unit, vfi, _bcmVfiTypeMpls)) {
             return BCM_E_NOT_FOUND;
         }
    }

    mem = VLAN_XLATEm;
    index_min = soc_mem_index_min(unit, mem);
    index_max = soc_mem_index_max(unit, mem);

    buf_size = SOC_MEM_TABLE_BYTES(unit, mem);
    xlate_buf = soc_cm_salloc(unit, buf_size, "VLAN_XLATE buffer");
    if (NULL == xlate_buf) {
        return BCM_E_MEMORY;
    }
    rv = soc_mem_read_range(unit, mem, MEM_BLOCK_ANY, index_min, index_max, xlate_buf);
    if (rv < 0) {
        soc_cm_sfree(unit, xlate_buf);
        return rv;
    }

    for (i = index_min; i <= index_max; i++) {
        vent = soc_mem_table_idx_to_pointer(unit,
            mem, vlan_xlate_entry_t *, xlate_buf, i);
        if (soc_mem_field32_get(unit, mem, vent, VALIDf) == 0) {
            continue;
        }

        if (soc_mem_field32_get(unit, mem, vent, XLATE__MPLS_ACTIONf) != 0x1) {
            continue;
        }

        if (vfi != -1 && soc_mem_field32_get(unit, mem, vent, XLATE__VFIf) != vfi) {
            continue;
        }

        vp = soc_mem_field32_get(unit, mem, vent, XLATE__SOURCE_VPf);
        SHR_BITSET(vp_bitmap, vp);
    }

    (void)soc_cm_sfree(unit, xlate_buf);

    return BCM_E_NONE;
}
#endif

/*
 * Function:
 *      _bcm_tr_mpls_vpws_port_add
 * Purpose:
 *      Add VPWS port to a VPN
 * Parameters:
 *      unit    - (IN) Device Number
 *      vpn     - (IN) VPN instance ID
 *      mpls_port - (IN/OUT) mpls port information (OUT : mpls_port_id)
 * Returns:
 *      BCM_E_XXX
 */

STATIC int
_bcm_tr_mpls_vpws_port_add(int unit, bcm_vpn_t vpn, bcm_mpls_port_t *mpls_port)
{
    int active_vp = -1;
    source_vp_entry_t svp1, svp2;
    ing_dvp_table_entry_t dvp1, dvp2;
    egr_l3_next_hop_entry_t egr_nh;
    uint8 vp_valid_flag = 0;
    bcm_port_t local_port;
    int tpid_enable = 0, tpid_index;
    int customer_drop=0, is_local = 0, rv = BCM_E_PARAM, nh_index = 0;
    int     vp3= -1, num_vp, match_vp=0, vp3_read = -1;
    source_vp_entry_t  svp3;
    ing_dvp_table_entry_t     dvp3;
    bcm_vpn_t  vpn_id;
    int vp1=-1, vp2=-1, backup_vp2 = -1;
    _bcm_mpls_old_nh_info_t old_nh_info;
    int  prot_index = -1;
#ifdef BCM_ENDURO_SUPPORT
    svp_disable_vlan_checks_tab_entry_t svp_disable_check;
#endif /* BCM_ENDURO_SUPPORT */
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int match_add = TRUE;
    bcm_mpls_label_t vp3_match_label;
    _bcm_vp_info_t vp_info;
    bcm_switch_network_group_t network_group;


        network_group = mpls_port->network_group_id;
        rv = _bcm_validate_splithorizon_network_group(unit,
            mpls_port->flags & BCM_MPLS_PORT_NETWORK, &network_group);
        BCM_IF_ERROR_RETURN(rv);
        num_vp = soc_mem_index_count(unit, SOURCE_VPm);
        _BCM_MPLS_VPN_GET(vpn_id, _BCM_MPLS_VPN_TYPE_VPWS, vpn);
        _bcm_vp_info_init(&vp_info);
        vp_info.vp_type = _bcmVpTypeMpls;

        sal_memset(&svp2, 0, sizeof(source_vp_entry_t));
        sal_memset(&svp1, 0, sizeof(source_vp_entry_t));
        if ( vpn != BCM_MPLS_VPWS_VPN_INVALID) {

             (void) _bcm_tr_mpls_vpws_vp_map_get (unit, vpn_id,  &vp1,  &vp2, &vp3_read);
             backup_vp2 = vp2;
            /* ---- Read in current table values for VP1 and VP2 ----- */
            if (vp1 != -1) {
                BCM_IF_ERROR_RETURN (
                   READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp1, &svp1));
                if (soc_SOURCE_VPm_field32_get(unit, &svp1, ENTRY_TYPEf) != 0) {
                    vp_valid_flag |= 0x1;  /* -- VP1 Valid ----- */
                }
                BCM_IF_ERROR_RETURN (
                   READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp1, &dvp1));
            }

            if (vp2 != -1) {
                BCM_IF_ERROR_RETURN (
                   READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp2, &svp2));
                if (soc_SOURCE_VPm_field32_get(unit, &svp2, ENTRY_TYPEf) != 0) {
                    vp_valid_flag |= 0x2;        /* -- VP2 Valid ----- */
                }
                BCM_IF_ERROR_RETURN (
                   READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp2, &dvp2));
            }

            /* ---- Read in current table values for VP3 ----- */
            if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {

                active_vp = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->mpls_port_id);
                if (_BCM_MPLS_GPORT_FAILOVER_VPLESS_GET(active_vp)) {
                    active_vp =
                        _BCM_MPLS_GPORT_FAILOVER_VPLESS_CLEAR(active_vp);
                }
                if (active_vp == -1) {
                   return BCM_E_PARAM;
                }

                if (!_bcm_vp_used_get(unit, active_vp, _bcmVpTypeMpls)) {
                     return BCM_E_NOT_FOUND;
                }
                /* IN case of multi level failover, we can have backup
                 * ports which are not vp2, but to resue the same code,
                 * we will try to use them through vp2 variables.
                 */
                if ((active_vp != vp1) && (active_vp != vp2)) {
                    if (bcmi_mpls_match_backup_ports(unit, vpn_id, active_vp)) {

                    vp2 = active_vp;
                    BCM_IF_ERROR_RETURN (
                        READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp2, &svp2));

                    if (soc_SOURCE_VPm_field32_get(unit, &svp2, ENTRY_TYPEf) != 0) {
                        vp_valid_flag |= 0x2;        /* -- VP2 Valid ----- */
                    }
                    BCM_IF_ERROR_RETURN (
                       READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp2, &dvp2));
                    } else {
                        /* Read in current table values for VP3 */
                        vp3 = active_vp;
                        BCM_IF_ERROR_RETURN (
                            READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp3, &dvp3));
                        BCM_IF_ERROR_RETURN (
                            READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp3, &svp3));
                    }
                }

                if (soc_SOURCE_VPm_field32_get(unit, &svp1, ENTRY_TYPEf) == 0) {
                   vp_valid_flag |= 0x10;
                } 
                if (soc_SOURCE_VPm_field32_get(unit, &svp2, ENTRY_TYPEf) == 0) {
                   vp_valid_flag |= 0x20;
                }
            }
        } else if (vpn == BCM_MPLS_VPWS_VPN_INVALID) {

              if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
                   active_vp = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->mpls_port_id);
                   if (active_vp == -1) {
                       return BCM_E_PARAM;
                   }

                   if (!_bcm_vp_used_get(unit, active_vp, _bcmVpTypeMpls)) {
                        return BCM_E_NOT_FOUND;
                   }

                   BCM_IF_ERROR_RETURN (
                             READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, active_vp, &svp2));
                   match_vp = soc_SOURCE_VPm_field32_get(unit, &svp2, DVPf);

                   soc_SOURCE_VPm_field32_set(unit, &svp2, 
                                  ENTRY_TYPEf, 0x0);
                   rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, active_vp, &svp2);
                   if (BCM_SUCCESS(rv)) {
                        BCM_IF_ERROR_RETURN (
                                  READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, match_vp, &svp1));
                        soc_SOURCE_VPm_field32_set(unit, &svp1, 
                                       ENTRY_TYPEf, 0x0);
                        rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, match_vp, &svp1);
                   }
                   return rv;
              } else {
                   /* VP3 Add */
                   vp_valid_flag = 0x4;
              }
         }


        switch (vp_valid_flag) {

              case 0x0: /* No VP is valid */
                             if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
                                  return BCM_E_NOT_FOUND;
                             }

                             if (mpls_port->flags & BCM_MPLS_PORT_WITH_ID) {
                                 if (mpls_port->flags & BCM_MPLS_PORT_NETWORK) {
                                     vp2 = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->mpls_port_id);
                                     if (vp2 == -1) {
                                         return BCM_E_PARAM;
                                     }

                                     if (_bcm_vp_used_get(unit, vp2, _bcmVpTypeMpls)) {
                                         return BCM_E_EXISTS;
                                     }
                                     if (vp2 >= num_vp) {
                                         return (BCM_E_BADID);
                                     }
                                     vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                     BCM_IF_ERROR_RETURN(_bcm_vp_used_set(unit, vp2, vp_info));
                                 }
                                 else {
                                     vp1 = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->mpls_port_id);
                                     if (vp1 == -1) {
                                         return BCM_E_PARAM;
                                     }

                                     if (_bcm_vp_used_get(unit, vp1, _bcmVpTypeMpls)) {
                                         return BCM_E_EXISTS;
                                     }
                                     if (vp1 >= num_vp) {
                                         return (BCM_E_BADID);
                                     }
                                     vp_info.flags &= ~(_BCM_VP_INFO_NETWORK_PORT);
                                     BCM_IF_ERROR_RETURN(_bcm_vp_used_set(unit, vp1, vp_info));
                                 }

                             } else {
                                 /* No entries are used, allocate a new VP index for VP1 */
                                 if (mpls_port->flags & BCM_MPLS_PORT_NETWORK) {
                                     /* Allocate a new VP index for VP2 */
                                     vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                     rv = _bcm_vp_alloc(unit, 0, (num_vp - 1), 1, SOURCE_VPm, vp_info, &vp2);
                                     if (rv < 0) {
                                         (void) _bcm_vp_free(unit, _bcmVpTypeMpls, 1, vp1);
                                         return rv;
                                     }
                                     _BCM_MPLS_NETWORK_VP_USED_SET(unit, vp2);
                                 } else {
                                     vp_info.flags &= ~(_BCM_VP_INFO_NETWORK_PORT);
                                     rv = _bcm_vp_alloc(unit, 0, (num_vp - 1), 1, SOURCE_VPm, vp_info, &vp1);
                                     if (rv < 0) {
                                         return rv;
                                     }
                                 }
                             }

                             /*
                              * This is the first port being added. If it's a network side port, it should go
                              * into VP2. If it's a customer side port, it should go into VP1.
                              */
                             if (mpls_port->flags & BCM_MPLS_PORT_NETWORK) {
                                 active_vp = vp2;
                                 vp_valid_flag = 0x2;
                             } else {
                                 active_vp = vp1;
                                 vp_valid_flag = 1;
                             }

                             sal_memset(&svp1, 0, sizeof(source_vp_entry_t));
                             sal_memset(&dvp1, 0, sizeof(ing_dvp_table_entry_t));
                             sal_memset(&svp2, 0, sizeof(source_vp_entry_t));
                             sal_memset(&dvp2, 0, sizeof(ing_dvp_table_entry_t));

                             (void) _bcm_tr_mpls_vpws_vp_map_set(unit, vpn_id, vp1, vp2, vp3_read);
                             break;


         case 0x1:    /* Only VP1 is valid */	
                             if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
                                  if (active_vp != vp1) {
                                       return BCM_E_NOT_FOUND;
                                  }
                                  nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp1, 
                                                                         NEXT_HOP_INDEXf);
                             } else if (mpls_port->flags & BCM_MPLS_PORT_WITH_ID) {
                                vp2 = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->mpls_port_id);
                                if (vp2 == -1) {
                                     return BCM_E_PARAM;
                                }

                                if (_bcm_vp_used_get(unit, vp2, _bcmVpTypeMpls)) {
                                     return BCM_E_EXISTS;
                                }
                                if (vp2 >= num_vp) {
                                     return (BCM_E_BADID);
                                }
                                vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                BCM_IF_ERROR_RETURN(_bcm_vp_used_set(unit, vp2, vp_info));
                               (void) _bcm_tr_mpls_vpws_vp_map_set(unit, vpn_id, vp1,  vp2, vp3_read);

                                 active_vp = vp2;
                                 vp_valid_flag = 3;
                                 sal_memset(&svp2, 0, sizeof(source_vp_entry_t));
                                 sal_memset(&dvp2, 0, sizeof(ing_dvp_table_entry_t));

                             } else {
                                 if(vp2 == -1) {

                                    vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                     /* Allocate a new VP index for VP2 */
                                     rv = _bcm_vp_alloc(unit, 0, (num_vp - 1), 1, SOURCE_VPm, vp_info, &vp2);
                                     if (rv < 0) {
                                         return rv;
                                     }
                                     _BCM_MPLS_NETWORK_VP_USED_SET(unit, vp2);
                                 }

                                 active_vp = vp2;
                                 vp_valid_flag = 3;
                                 sal_memset(&svp2, 0, sizeof(source_vp_entry_t));
                                 sal_memset(&dvp2, 0, sizeof(ing_dvp_table_entry_t));
                                 (void) _bcm_tr_mpls_vpws_vp_map_set(unit, vpn_id, vp1, vp2, vp3_read);
                             }
                             break;



         case 0x2: /* Only VP2 is valid */
                             if (!(mpls_port->flags & BCM_MPLS_PORT_NETWORK)) {
                                 /* 
                                  * Since only VP2 is valid and the port being added is not a network port,
                                  * it's customer side port i.e. VP1
                                  */
                                 if (mpls_port->flags & BCM_MPLS_PORT_WITH_ID) {
                                     vp1 = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->mpls_port_id);
                                     if (vp1 == -1) {
                                         return BCM_E_PARAM;
                                     }

                                     if (_bcm_vp_used_get(unit, vp1, _bcmVpTypeMpls)) {
                                         return BCM_E_EXISTS;
                                     }
                                     if (vp1 >= num_vp) {
                                         return (BCM_E_BADID);
                                     }
                                     vp_info.flags &= ~(_BCM_VP_INFO_NETWORK_PORT);
                                     BCM_IF_ERROR_RETURN(_bcm_vp_used_set(unit, vp1, vp_info));
                                     (void) _bcm_tr_mpls_vpws_vp_map_set(unit, vpn_id, vp1, vp2, vp3_read);

                                     active_vp = vp1;
                                     vp_valid_flag = 3;
                                     sal_memset(&svp1, 0, sizeof(source_vp_entry_t));
                                     sal_memset(&dvp1, 0, sizeof(ing_dvp_table_entry_t));
                                 } else {
                                     if (vp1 == -1) {
                                         /* Allocate a new VP index for VP1 */
                                         vp_info.flags &= ~(_BCM_VP_INFO_NETWORK_PORT);
                                         rv = _bcm_vp_alloc(unit, 0, (num_vp - 1), 1, SOURCE_VPm, vp_info, &vp1);
                                         if (rv < 0) {
                                             return rv;
                                         }
                                     }

                                     active_vp = vp1;
                                     vp_valid_flag = 3;
                                     sal_memset(&svp1, 0, sizeof(source_vp_entry_t));
                                     sal_memset(&dvp1, 0, sizeof(ing_dvp_table_entry_t));
                                     (void) _bcm_tr_mpls_vpws_vp_map_set(unit, vpn_id, vp1, vp2, vp3_read);
                                 }
                             } else {
                                 /* 
                                  * Since only VP2 is valid and the port being
                                  * added is a network port, its either existing 
                                  * network port (VP2) being replaced or a 
                                  * primary network port (VP3) being added.
                                  */
                                 if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
                                     /* in case of multi level protection, 
                                      * we have already changed the vp2 above for 
                                      * matching backup port. 
                                      */
                                     if (active_vp != vp2) {
                                         return BCM_E_NOT_FOUND;
                                     }
                                     nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp2,
                                             NEXT_HOP_INDEXf);
                                     break;
                                 }
#if defined(BCM_TRIUMPH2_SUPPORT)
                                 if (soc_feature(unit, soc_feature_mpls_failover)) {
                                     if (BCM_SUCCESS(_bcm_esw_failover_mpls_check(unit, mpls_port))) {

                                         if (mpls_port->flags & BCM_MPLS_PORT_WITH_ID) {
                                             vp3 = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->mpls_port_id);
                                             if (vp3 == -1) {
                                                 return BCM_E_PARAM;
                                             }

                                             if (_bcm_vp_used_get(unit, vp3, _bcmVpTypeMpls)) {
                                                 return BCM_E_EXISTS;
                                             }
                                             if (vp3 >= num_vp) {
                                                 return (BCM_E_BADID);
                                             }

                                             sal_memset(&svp3, 0, sizeof(source_vp_entry_t));
                                             sal_memset(&dvp3, 0, sizeof(ing_dvp_table_entry_t));
                                             active_vp = vp3;

                                             /*
                                              * Book keep the Primary Network VP (vp3). It's required to link to
                                              * the SVP of customer side when it's created
                                              */
                                             (void) _bcm_tr_mpls_vpws_vp_map_set(unit, vpn_id, vp1, vp2, vp3);
                                             vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                             BCM_IF_ERROR_RETURN(
                                                     _bcm_vp_used_set(unit, active_vp, vp_info));
                                         } else {
                                             if (mpls_port->flags & BCM_MPLS_PORT_FAILOVER) {
                                                 /* VP conservation */
                                                 vp3 = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->failover_port_id);
                                                 if (vp3 == -1) {
                                                     return BCM_E_PARAM;
                                                 }
                                             } else {
                                                 /* allocate a new VP index */
                                                 vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                                 rv = _bcm_vp_alloc(unit, 0, (num_vp - 1), 1, SOURCE_VPm, vp_info, &vp3);
                                                 if (vp3 == -1) {
                                                     return BCM_E_PARAM;
                                                 }
                                                 if (rv < 0) {
                                                     return rv;
                                                 }
                                                 _BCM_MPLS_NETWORK_VP_USED_SET(unit, vp3);
                                             }
                                             sal_memset(&svp3, 0, sizeof(source_vp_entry_t));
                                             sal_memset(&dvp3, 0, sizeof(ing_dvp_table_entry_t));
                                             active_vp = vp3;
                                             /*
                                              * Book keep the Primary Network VP (vp3). It's required to link to
                                              * the SVP of customer side when it's created
                                              */
                                             (void) _bcm_tr_mpls_vpws_vp_map_set(unit, vpn_id, vp1, vp2, vp3);
                                             vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                             BCM_IF_ERROR_RETURN(
                                                     _bcm_vp_used_set(unit, active_vp, vp_info));
                                         }
                                     } else if (_BCM_FAILOVER_IS_MULTI_LEVEL(mpls_port->failover_id)) {
                                         /* We can get another backup port here in case this is multi level failover */
                                         if (mpls_port->flags & BCM_MPLS_PORT_WITH_ID) {
                                             vp2 = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->mpls_port_id);
                                             if (vp2 == -1) {
                                                 return BCM_E_PARAM;
                                             }
                                             if (_bcm_vp_used_get(unit, vp2, _bcmVpTypeMpls)) {
                                                 return BCM_E_EXISTS;
                                             }
                                             if (vp2 >= num_vp) {
                                                 return (BCM_E_BADID);
                                             }
                                             sal_memset(&svp2, 0, sizeof(source_vp_entry_t));
                                             sal_memset(&dvp2, 0, sizeof(ing_dvp_table_entry_t));
                                             active_vp = vp2;
                                             /* We need to update backup ports */
                                             BCM_IF_ERROR_RETURN(
                                                 bcmi_mpls_vpws_vp_map_backup_port_set(
                                                 unit, vpn_id, vp2));
                                         } else {
                                             /* allocate a new VP index */
                                             vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                             rv = _bcm_vp_alloc(unit, 0, (num_vp - 1), 1, SOURCE_VPm, vp_info, &vp2);
                                             if (vp2 == -1) {
                                                 return BCM_E_PARAM;
                                             }
                                             if (rv < 0) {
                                                 return rv;
                                             }
                                             _BCM_MPLS_NETWORK_VP_USED_SET(unit, vp2);
                                             sal_memset(&svp2, 0, sizeof(source_vp_entry_t));
                                             sal_memset(&dvp2, 0, sizeof(ing_dvp_table_entry_t));
                                             active_vp = vp2;
                                             vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                             BCM_IF_ERROR_RETURN(
                                                     _bcm_vp_used_set(unit, active_vp, vp_info));
                                             BCM_IF_ERROR_RETURN(
                                                 bcmi_mpls_vpws_vp_map_backup_port_set(
                                                 unit, vpn_id, vp2));
                                         }
                                     }
                                 } else 
#endif /* BCM_TRIUMPH2_SUPPORT  */
                                 {
                                     if (soc_feature(unit, soc_feature_mpls_software_failover) &&
                                             (mpls_port->flags & BCM_MPLS_PORT_FAILOVER)) {
                                         active_vp = BCM_GPORT_MPLS_PORT_ID_GET(
                                                 mpls_port->failover_port_id);
                                     } else {
                                         if (!(mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
                                             return BCM_E_FULL;
                                         }
                                     }
                                 }

                                 if (active_vp == vp1) {
                                     /* Replacing VP1 */
                                     nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp1,
                                             NEXT_HOP_INDEXf);
                                 } else if (active_vp == vp2) {
                                     /* Replacing VP2 */
                                     nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp2,
                                             NEXT_HOP_INDEXf);
                                 } else if (active_vp == vp3) {
                                     nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp3,
                                             NEXT_HOP_INDEXf);
                                 }
                             }
                             break;


              
         case 0x3: /* VP1 and VP2 are valid */
#if defined(BCM_TRIUMPH2_SUPPORT)
                             if (soc_feature(unit, soc_feature_mpls_failover)) {
                                if ((mpls_port->flags & BCM_MPLS_PORT_FAILOVER) &&
                                    (mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
                                    int failover_nh_index = -1;
                                    int failover_id = -1;
                                    int failover_mc_group = -1;
                                    if (active_vp != vp2) {
                                        return BCM_E_NOT_FOUND;
                                    }
                                    nh_index = 
                                        soc_ING_DVP_TABLEm_field32_get(
                                            unit, &dvp2, NEXT_HOP_INDEXf);
                                    rv = _bcm_esw_failover_prot_nhi_get(
                                              unit, nh_index, &failover_id,
                                              &failover_nh_index, &failover_mc_group);
                                    if (BCM_SUCCESS(rv) && (failover_mc_group > 0)) {
                                        rv = _bcm_tr_mpls_vpless_failover_nh_index_find(unit, vp2,
                                                           nh_index, &failover_nh_index);
                                        if (BCM_SUCCESS(rv) && (failover_nh_index > 0)) {
                                            nh_index =  failover_nh_index;
                                        }
                                    } else if (BCM_SUCCESS(rv) && (failover_nh_index > 0)) {
                                        nh_index = failover_nh_index;
                                    }
                                    break;
                                } else if (BCM_SUCCESS(_bcm_esw_failover_mpls_check(unit, mpls_port))) {
                                  if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
                                       if (active_vp != vp3) {
                                           return BCM_E_NOT_FOUND;
                                       }
                                       nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp3,
                                                                          NEXT_HOP_INDEXf);
                                  }  else if (mpls_port->flags & BCM_MPLS_PORT_WITH_ID) {
                                            vp3 = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->mpls_port_id);
                                            if (vp3 == -1) {
                                                 return BCM_E_PARAM;
                                            }

                                            if (_bcm_vp_used_get(unit, vp3, _bcmVpTypeMpls)) {
                                                 return BCM_E_EXISTS;
                                            }
                                            if (vp3 >= num_vp) {
                                                 return (BCM_E_BADID);
                                            }
                                            vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                            BCM_IF_ERROR_RETURN(_bcm_vp_used_set(unit, vp3, vp_info));
                                            sal_memset(&svp3, 0, sizeof(source_vp_entry_t));
                                            sal_memset(&dvp3, 0, sizeof(ing_dvp_table_entry_t));
                                            active_vp = vp3;
                                            (void) _bcm_tr_mpls_vpws_vp_map_set(unit, vpn_id, vp1, vp2, vp3);
                                            BCM_IF_ERROR_RETURN(
                                                 _bcm_vp_used_set(unit, active_vp, vp_info));
                                    } else {
                                        if (mpls_port->flags & BCM_MPLS_PORT_FAILOVER) {
                                            /* VP conservation */
                                            vp3 = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->failover_port_id);
                                            if (vp3 == -1) {
                                                return BCM_E_PARAM;
                                            }
                                        } else {
                                            /* allocate a new VP index */
                                            vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                            rv = _bcm_vp_alloc(unit, 0, (num_vp - 1), 1, SOURCE_VPm, vp_info, &vp3);
                                            if (vp3 == -1) {
                                                return BCM_E_PARAM;
                                            }
                                            if (rv < 0) {
                                                return rv;
                                            }
                                            _BCM_MPLS_NETWORK_VP_USED_SET(unit, vp3);
                                        }
                                        sal_memset(&svp3, 0, sizeof(source_vp_entry_t));
                                        sal_memset(&dvp3, 0, sizeof(ing_dvp_table_entry_t));
                                        active_vp = vp3;
                                        if (!(mpls_port->flags & BCM_MPLS_PORT_FAILOVER)) {
                                            (void) _bcm_tr_mpls_vpws_vp_map_set(unit, vpn_id, vp1, vp2, vp3);
                                            vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                            BCM_IF_ERROR_RETURN(
                                                 _bcm_vp_used_set(unit, active_vp, vp_info));
                                        }
                                  }
                              } else if (_BCM_FAILOVER_IS_MULTI_LEVEL(mpls_port->failover_id)) {
                                  if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
                                       if (active_vp != vp2) {
                                           return BCM_E_NOT_FOUND;
                                       }
                                   nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp2,
                                                                      NEXT_HOP_INDEXf);
                                  }  else if (mpls_port->flags & BCM_MPLS_PORT_WITH_ID) {
                                        vp2 = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->mpls_port_id);
                                        if (vp2 == -1) {
                                             return BCM_E_PARAM;
                                        }

                                        if (_bcm_vp_used_get(unit, vp2, _bcmVpTypeMpls)) {
                                             return BCM_E_EXISTS;
                                        }
                                        if (vp2 >= num_vp) {
                                             return (BCM_E_BADID);
                                        }
                                        vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                        BCM_IF_ERROR_RETURN(_bcm_vp_used_set(unit, vp2, vp_info));
                                        sal_memset(&svp2, 0, sizeof(source_vp_entry_t));
                                        sal_memset(&dvp2, 0, sizeof(ing_dvp_table_entry_t));
                                        active_vp = vp2;
                                        BCM_IF_ERROR_RETURN(
                                             _bcm_vp_used_set(unit, active_vp, vp_info));
                                        BCM_IF_ERROR_RETURN(
                                            bcmi_mpls_vpws_vp_map_backup_port_set(
                                            unit, vpn_id, vp2));
                                  } else {
                                       if (mpls_port->flags & BCM_MPLS_PORT_FAILOVER) {
                                           /* VP conservation */
                                           vp2 = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->failover_port_id);
                                           if (vp2 == -1) {
                                               return BCM_E_PARAM;
                                           }
                                       } else {
                                           /* allocate a new VP index */
                                           vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                           rv = _bcm_vp_alloc(unit, 0, (num_vp - 1), 1, SOURCE_VPm, vp_info, &vp2);
                                           if (vp2 == -1) {
                                                return BCM_E_PARAM;
                                           }
                                           if (rv < 0) {
                                                return rv;
                                           }
                                           _BCM_MPLS_NETWORK_VP_USED_SET(unit, vp2);
                                       }
                                       sal_memset(&svp2, 0, sizeof(source_vp_entry_t));
                                       sal_memset(&dvp2, 0, sizeof(ing_dvp_table_entry_t));
                                       active_vp = vp2;

                                       if (!(mpls_port->flags & BCM_MPLS_PORT_FAILOVER)) {
                                           vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                           BCM_IF_ERROR_RETURN(
                                                _bcm_vp_used_set(unit, active_vp, vp_info));
                                           BCM_IF_ERROR_RETURN(
                                                bcmi_mpls_vpws_vp_map_backup_port_set(
                                                unit, vpn_id, vp2));
                                        }
                                     }
                                 }
                             } else 
#endif /* BCM_TRIUMPH2_SUPPORT  */
                             {
                                 if (soc_feature(unit, soc_feature_mpls_software_failover) &&
                                     (mpls_port->flags & BCM_MPLS_PORT_FAILOVER)) {
                                      active_vp = BCM_GPORT_MPLS_PORT_ID_GET(
                                                          mpls_port->failover_port_id);
                                 } else {
                                     if (!(mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
                                         return BCM_E_FULL;
                                     }
                                 }
                             }

                             if (active_vp == vp1) {
                                  /* Replacing VP1 */
                                       nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp1,
                                                                                         NEXT_HOP_INDEXf);
                             }

                             else if (active_vp == vp2) {
                                  /* Replacing VP2 */
                                  nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp2,
                                                                                         NEXT_HOP_INDEXf);
                             }

                             else if (active_vp == vp3) {
                                  nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp3,
                                                                                         NEXT_HOP_INDEXf);
                             }
                             break;


              
              case 0x4: /* Only VP3 is valid - For Software based Failover purposes */
                            if (soc_feature(unit, soc_feature_mpls_software_failover)) {
                                 if (mpls_port->flags & BCM_MPLS_PORT_WITH_ID) {
                                      vp3 = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->mpls_port_id);
                                       if (vp3 == -1) {
                                           return BCM_E_PARAM;
                                       }

                                      if (_bcm_vp_used_get(unit, vp3, _bcmVpTypeMpls)) {
                                           return BCM_E_EXISTS;
                                      }
                                      if (vp3 >= num_vp) {
                                           return (BCM_E_BADID);
                                      }
                                      vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                      BCM_IF_ERROR_RETURN(
                                           _bcm_vp_used_set(unit, vp3, vp_info));
                                 } else {
                                           /* allocate a new VP index */
                                           vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                           rv = _bcm_vp_alloc(unit, 0, (num_vp - 1),
                                                         1, SOURCE_VPm, vp_info, &vp3);
                                           if (rv < 0) {
                                                return rv;
                                           }
                                           _BCM_MPLS_NETWORK_VP_USED_SET(unit, vp3);
                                 }
                                 sal_memset(&svp3, 0, sizeof(source_vp_entry_t));
                                 sal_memset(&dvp3, 0, sizeof(ing_dvp_table_entry_t));
                                 active_vp = vp3;
                                 vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
                                 BCM_IF_ERROR_RETURN(
                                       _bcm_vp_used_set(unit, active_vp, vp_info));
                             }
                             break;

              case 0x12:
                             if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
                                 if (active_vp == vp2) {
                                     nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp2, 
                                             NEXT_HOP_INDEXf);
                                 } else if (active_vp == vp3_read) {
                                     nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp3, 
                                             NEXT_HOP_INDEXf);
                                 } else {
                                     return BCM_E_NOT_FOUND;
                                 }
                             }
                             break;

              case 0x21:
                             if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
                                 if (active_vp == vp1) {
                                     nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp1, 
                                             NEXT_HOP_INDEXf);
                                 } else if (active_vp == vp3_read) {
                                     nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp3, 
                                             NEXT_HOP_INDEXf);
                                 } else {
                                     return BCM_E_NOT_FOUND;
                                 }
                             }
                             break;

              case 0x30:
                             if (soc_feature(unit, soc_feature_mpls_software_failover)) {
                                  if ( vpn != BCM_MPLS_VPWS_VPN_INVALID) {
                                       if (active_vp == vp2) {
                                            soc_SOURCE_VPm_field32_set(unit, &svp2, 
                                                           ENTRY_TYPEf, 0x2);
                                            rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp2, &svp2);
                                            if (BCM_SUCCESS(rv)) {
                                                 soc_SOURCE_VPm_field32_set(unit, &svp1, 
                                                                ENTRY_TYPEf, 0x2);
                                                 soc_SOURCE_VPm_field32_set(unit, &svp1, 
                                                                DVPf, vp2);
                                                 rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp1, &svp1);
                                            }
                                       } else if (active_vp == vp3) {
                                            soc_SOURCE_VPm_field32_set(unit, &svp3, 
                                                      ENTRY_TYPEf, 0x2);
                                            soc_SOURCE_VPm_field32_set(unit, &svp3, 
                                                      DVPf, vp1);
                                            rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp3, &svp3);
                                            if (BCM_SUCCESS(rv)) {
                                                 soc_SOURCE_VPm_field32_set(unit, &svp1, 
                                                       ENTRY_TYPEf, 0x2);
                                                 soc_SOURCE_VPm_field32_set(unit, &svp1, 
                                                       DVPf, vp3);
                                                 rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp1, &svp1);
                                            }
                                            if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
                                                (void) _bcm_tr_mpls_vpws_vp_map_set(unit, vpn_id, vp1, active_vp, vp3_read);
                                            }
                                       }
                                  }
                                  return rv;
                             }
                             break;

              default:
                             return BCM_E_CONFIG;
       }

        if (active_vp == -1) {
           return BCM_E_CONFIG;
        }

        if (!(mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
            /* Set the MPLS port ID */
            BCM_GPORT_MPLS_PORT_ID_SET(mpls_port->mpls_port_id, active_vp);
        } 
        if (mpls_port->flags & BCM_MPLS_PORT_ENCAP_WITH_ID) {
            /* Input parameters check. */
            if (!BCM_XGS3_L3_EGRESS_IDX_VALID(unit, mpls_port->encap_id) &&
                !BCM_XGS3_DVP_EGRESS_IDX_VALID(unit, mpls_port->encap_id)) {
                return (BCM_E_PARAM);
            }

            /* Calculate next hop index. */
            if (BCM_XGS3_L3_EGRESS_IDX_VALID(unit, mpls_port->encap_id)) {
                nh_index = mpls_port->encap_id - BCM_XGS3_EGRESS_IDX_MIN;
            } else {
                nh_index = mpls_port->encap_id - BCM_XGS3_DVP_EGRESS_IDX_MIN;
            }
        } else {
            mpls_port->encap_id = 0;
        }

        switch (vp_valid_flag) {

              case 0x30:
                            if (soc_feature(unit, soc_feature_vlan_xlate)) {
                                rv = soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, nh_index, &egr_nh);
                                if (rv < 0) {
                                    goto vpws_cleanup;
                                }

                                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, MPLS__USE_DVP_IN_EVXLT_KEYf, 0x1);
                                rv = soc_mem_write (unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ALL, nh_index, &egr_nh);

                                if (rv < 0) {
                                    goto vpws_cleanup;
                                }
                            }
                            break;

              default:
                            customer_drop = (mpls_port->flags & BCM_MPLS_PORT_DROP) ? 1:0;
                            rv = _bcm_tr_mpls_l2_nh_info_add(unit, mpls_port, active_vp, customer_drop, 1,
                                         &nh_index, &local_port, &is_local, &old_nh_info);
                            if (rv < 0) {
                                return rv;
                            }

                            if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_TAGGED) {
                                    rv = _bcm_fb2_outer_tpid_lkup(unit, mpls_port->service_tpid, 
                                                                   &tpid_index);
                                    if (rv < 0) {
                                        goto vpws_cleanup;
                                    }
                                    tpid_enable = (1 << tpid_index);
                            }
                            break;
        }

       if (mpls_port->flags & BCM_MPLS_PORT_FAILOVER) {
           rv = _bcm_esw_mpls_match_add(unit, mpls_port, 
                      _BCM_MPLS_GPORT_FAILOVER_VPLESS_SET(active_vp), vpn, TRUE);
           mpls_port->mpls_port_id =
                _BCM_MPLS_GPORT_FAILOVER_VPLESS_SET(mpls_port->mpls_port_id); 
           mpls_port->encap_id = BCM_XGS3_DVP_EGRESS_IDX_MIN + nh_index;
           if (soc_feature(unit, soc_feature_mpls_software_failover)) {
               if (mpls_port->flags & BCM_MPLS_PORT_ENCAP_WITH_ID) {
                   rv = soc_mem_field32_modify(unit, ING_DVP_TABLEm,
                                                active_vp, NEXT_HOP_INDEXf, nh_index);
                   if (rv < 0) {
                       goto vpws_cleanup;
                   }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
                   if (SOC_IS_TRIDENT2PLUS(unit)) {
                       rv = soc_mem_field32_modify(unit, ING_DVP_2_TABLEm,
                               active_vp, NEXT_HOP_INDEXf, nh_index);
                   }
                   if (rv < 0) {
                       goto vpws_cleanup;
                   }
#endif
               }
           }
           if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
               (void)_bcm_tr_mpls_l2_old_nh_info_delete(
                    unit, mpls_port, nh_index, &old_nh_info);
           }
           goto vpws_cleanup;
        }

              if (active_vp == vp1) {
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) || \
                                                defined(BCM_APACHE_SUPPORT)
                  if (SOC_IS_KATANAX(unit) || SOC_IS_TRIUMPH3(unit) ||
                                                        SOC_IS_APACHE(unit)) {
                      rv = _bcm_esw_add_policer_to_table(unit, 
                                               mpls_port->policer_id,
                                               SOURCE_VPm, 0, &svp1);
                      BCM_IF_ERROR_RETURN(rv);
                  }
#endif /* BCM_KATANA_SUPPORT or BCM_TRIUMPH3_SUPPORT or BCM_APACHE_SUPPORT */
                        /* Set SOURCE_VP */
                        soc_SOURCE_VPm_field32_set(unit, &svp1, CLASS_IDf,
                            mpls_port->if_class);
                        if (soc_feature(unit,
                            soc_feature_multiple_split_horizon_group)) {
                            soc_SOURCE_VPm_field32_set(
                                unit, &svp1, NETWORK_GROUPf, network_group);
                        } else {
                            soc_SOURCE_VPm_field32_set(
                                unit, &svp1, NETWORK_PORTf,
                                (mpls_port->flags & BCM_MPLS_PORT_NETWORK) ?
                                1 : 0);
                        }
                        if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_TAGGED) {
                             soc_SOURCE_VPm_field32_set(unit, &svp1, SD_TAG_MODEf, 1);
                             soc_SOURCE_VPm_field32_set(unit, &svp1, TPID_ENABLEf, tpid_enable);
                             if (SOC_MEM_FIELD_VALID(unit, SOURCE_VPm, TRUST_OUTER_DOT1Pf)) {
                                  soc_SOURCE_VPm_field32_set(unit, &svp1, TRUST_OUTER_DOT1Pf, 1);
                             }
                        } else {
                             soc_SOURCE_VPm_field32_set(unit, &svp1, SD_TAG_MODEf, 0);
                        }

                        if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
                             rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp1, &svp1);
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }
                             /* Link in the newly allocated next-hop entry */
                            soc_ING_DVP_TABLEm_field32_set(unit, &dvp1, NEXT_HOP_INDEXf,
                                                                                         nh_index);
                            /* Enable DVP as Network_port */
                            if (soc_mem_field_valid(unit, ING_DVP_TABLEm, NETWORK_PORTf)) {
                                  soc_ING_DVP_TABLEm_field32_set(unit, &dvp1, NETWORK_PORTf,
                                       ((mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) &&
                                        (mpls_port->flags & BCM_MPLS_PORT_NETWORK) ) ? 1 : 0);
                            }
                            if (soc_feature(unit,
                                soc_feature_multiple_split_horizon_group)) {
                                if (soc_mem_field_valid(
                                    unit, ING_DVP_TABLEm, NETWORK_GROUPf)) {
                                    soc_ING_DVP_TABLEm_field32_set(
                                            unit, &dvp1, NETWORK_GROUPf,
                                            network_group);
                                }
                            }

                            rv = WRITE_ING_DVP_TABLEm(unit, MEM_BLOCK_ALL, active_vp, &dvp1);
                            if (rv < 0) {
                               goto vpws_cleanup;
                            }
                            rv = _bcm_vp_ing_dvp_to_ing_dvp2(unit, &dvp1, active_vp);
                            if (rv < 0) {
                               goto vpws_cleanup;
                            }
                         } else {

                             /* Link in the newly allocated next-hop entry to DVP */
                             soc_ING_DVP_TABLEm_field32_set(unit, &dvp1, NEXT_HOP_INDEXf,
                                                                                         nh_index);
                            /* Enable DVP as Network_port */
                            if (soc_mem_field_valid(unit, ING_DVP_TABLEm, NETWORK_PORTf)) {
                                  soc_ING_DVP_TABLEm_field32_set(unit, &dvp1, NETWORK_PORTf,
                                       ((mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) &&
                                        (mpls_port->flags & BCM_MPLS_PORT_NETWORK) ) ? 1 : 0);
                            }

                            if (soc_feature(unit,
                                soc_feature_multiple_split_horizon_group)) {
                                if (soc_mem_field_valid(
                                    unit, ING_DVP_TABLEm, NETWORK_GROUPf)) {
                                    soc_ING_DVP_TABLEm_field32_set(
                                            unit, &dvp1, NETWORK_GROUPf,
                                            network_group);

                                }
                            }

                             rv = WRITE_ING_DVP_TABLEm(unit, MEM_BLOCK_ALL, active_vp, &dvp1);
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }
                             rv = _bcm_vp_ing_dvp_to_ing_dvp2(unit, &dvp1, active_vp);
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }

                             /* Tie the SVP to DVP. */
                             soc_SOURCE_VPm_field32_set(unit, &svp1, 
                                                                               ENTRY_TYPEf, 0x2); /* VPWS */
                             switch (vp_valid_flag) {
                                  case 1:
                                       soc_SOURCE_VPm_field32_set(unit, &svp1, DVPf, 0);
                                       break;
                                  case 3:
                                       if (vp3_read != -1) {
                                           soc_SOURCE_VPm_field32_set(unit, &svp1, DVPf, vp3_read);
                                       } else if (vp2 != -1) {
                                           soc_SOURCE_VPm_field32_set(unit, &svp1, DVPf, vp2);
                                       }
                                       break;
                                  default:
                                       soc_SOURCE_VPm_field32_set(unit, &svp1, DVPf, vp1);
                                       break;
                             }
                             if (soc_mem_field_valid(unit, SOURCE_VPm, DISABLE_VLAN_CHECKSf)) {
                                  soc_SOURCE_VPm_field32_set(unit, &svp1, DISABLE_VLAN_CHECKSf, 1);
                             }
                             if (soc_mem_field_valid(unit, SOURCE_VPm, SRC_PORTf) && 
                                 soc_mem_field_valid(unit, SOURCE_VPm, SRC_MODIDf)) {
                                 rv = _bcm_tr_mpls_l2vpn_source_port_set(unit, mpls_port, &svp1);
                             }
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }
                             rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp1, &svp1);
                             if ((rv >= 0) && (vp_valid_flag == 3)) {
                                  soc_SOURCE_VPm_field32_set(unit, &svp2, DVPf, vp1);
                                  rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp2, &svp2);
                             } else if ((rv >= 0) && (vp_valid_flag == 1)) {
                                  if (vp2 != -1) {
                                      rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp2, &svp2);
                                  }
                             }
                             if ((rv >= 0) && (vp3_read != -1)) {
                                 source_vp_entry_t  svp3_read;
                                 sal_memset(&svp3_read, 0, sizeof(source_vp_entry_t));
                                 BCM_IF_ERROR_RETURN (READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp3_read, &svp3_read));
                                 if (vp1 != soc_SOURCE_VPm_field32_get(unit, &svp3_read, DVPf)) {
                                     rv = soc_mem_field32_modify(unit, SOURCE_VPm, vp3_read, DVPf, vp1);
                                  }
                             }
                             /* update other backup ports also here */
                             bcmi_mpls_update_dvp_in_backup_ports(unit, vpn_id, vp1);
#ifdef BCM_ENDURO_SUPPORT
                             if (SOC_IS_ENDURO(unit)) {
                                 BCM_IF_ERROR_RETURN (
                                    READ_SVP_DISABLE_VLAN_CHECKS_TABm(unit, MEM_BLOCK_ANY, vp1, &svp_disable_check));
                                 soc_SVP_DISABLE_VLAN_CHECKS_TABm_field32_set(unit, &svp_disable_check, DISABLE_VLAN_CHECKSf, 1); 
                                 BCM_IF_ERROR_RETURN ( 
                                    WRITE_SVP_DISABLE_VLAN_CHECKS_TABm(unit, MEM_BLOCK_ALL, vp1, &svp_disable_check)); 
                             }
#endif /* BCM_ENDURO_SUPPORT */
                        }
                         if (rv < 0) {
                              goto vpws_cleanup;
                         }
              }



              else if (active_vp == vp2) {
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) || defined(BCM_APACHE_SUPPORT)
                  if (SOC_IS_KATANAX(unit) || SOC_IS_TRIUMPH3(unit) || SOC_IS_APACHE(unit)) {
                      rv = _bcm_esw_add_policer_to_table(unit,
                                               mpls_port->policer_id,
                                               SOURCE_VPm, 0, &svp2);
                      BCM_IF_ERROR_RETURN(rv);
                  }
#endif /* BCM_KATANA_SUPPORT or BCM_TRIUMPH3_SUPPORT or BCM_APACHE_SUPPORT */
                        /* Set SOURCE_VP */
                        soc_SOURCE_VPm_field32_set(unit, &svp2, CLASS_IDf,
                             mpls_port->if_class);
                        if (soc_feature(unit,
                            soc_feature_multiple_split_horizon_group)) {
                            soc_SOURCE_VPm_field32_set(
                                    unit, &svp2, NETWORK_GROUPf, network_group);
                        } else {
                            soc_SOURCE_VPm_field32_set(
                                unit, &svp2, NETWORK_PORTf,
                                (mpls_port->flags & BCM_MPLS_PORT_NETWORK) ?
                                1 : 0);
                        }
                        if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_TAGGED) {
                             soc_SOURCE_VPm_field32_set(unit, &svp2, SD_TAG_MODEf, 1);
                              soc_SOURCE_VPm_field32_set(unit, &svp2, TPID_ENABLEf, tpid_enable);
                             if (SOC_MEM_FIELD_VALID(unit, SOURCE_VPm, TRUST_OUTER_DOT1Pf)) {
                                  soc_SOURCE_VPm_field32_set(unit, &svp2, TRUST_OUTER_DOT1Pf, 1);
                             }
                        } else {
                             soc_SOURCE_VPm_field32_set(unit, &svp2, SD_TAG_MODEf, 0);
                        }

                        if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
                             rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp2, &svp2);
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }
                             /* Link in the newly allocated next-hop entry */
                             soc_ING_DVP_TABLEm_field32_set(unit, &dvp2, NEXT_HOP_INDEXf,
                                                                                        nh_index);
                            /* Enable DVP as Network_port */
                            if (soc_mem_field_valid(unit, ING_DVP_TABLEm, NETWORK_PORTf)) {
                                  soc_ING_DVP_TABLEm_field32_set(unit, &dvp2, NETWORK_PORTf,
                                       ((mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) &&
                                        (mpls_port->flags & BCM_MPLS_PORT_NETWORK) ) ? 1 : 0);
                            }
                            if (soc_feature(unit,
                                soc_feature_multiple_split_horizon_group)) {
                                if (soc_mem_field_valid(
                                    unit, ING_DVP_TABLEm, NETWORK_GROUPf)) {
                                    soc_ING_DVP_TABLEm_field32_set(
                                            unit, &dvp2, NETWORK_GROUPf,
                                            network_group);

                                }
                            }

                             rv = WRITE_ING_DVP_TABLEm(unit, MEM_BLOCK_ALL, active_vp, &dvp2);
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }
                             rv = _bcm_vp_ing_dvp_to_ing_dvp2(unit, &dvp2, active_vp);
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }
                          
                             if (vp3_read != -1) {
                                 /*
                                  * A primary network port already exists.
                                  * vp2 is backup network port.
                                  */
                                 vp3_match_label = mpls_info->match_key[vp3_read].match_label;
                                 if (mpls_port->criteria == BCM_MPLS_PORT_MATCH_LABEL) {
                                     if (vp3_match_label == mpls_port->match_label) {
                                       /*
                                        * If same label is used for match on primary and backup,
                                        * no need to add another MPLS_ENTRY for backup
                                        */
                                        match_add = FALSE;
                                        if(!((mpls_info->match_key[vp2].match_label == -1) ||
                                            (vp3_match_label == mpls_info->match_key[vp2].match_label))){
                                            /* Delete old MPLS_ENTRY, update software record*/
                                            (void)_bcm_esw_mpls_match_delete(unit, vp2, TRUE);
                                            mpls_info->match_key[vp2].flags =
                                                _BCM_MPLS_PORT_MATCH_TYPE_LABEL;
                                            mpls_info->match_key[vp2].match_label =
                                                mpls_port->match_label;
                                            mpls_info->match_key[vp2].modid = 0;
                                            mpls_info->match_key[vp2].port = 0;
                                        }
                                    }
                                 }
                             }
                        } else {
                            /* we may have other backup ports coming up with 
                             * the same label as vp2 label, So we might have 
                             * to make sure that we dont update the match entry.
                             */
                            if ((backup_vp2 != -1) && (backup_vp2 != vp2)) {
                                if ((mpls_port->criteria == 
                                    BCM_MPLS_PORT_MATCH_LABEL) &&
                                     (mpls_info->match_key[backup_vp2].match_label == 
                                         mpls_port->match_label)) {
                                         /*
                                          * If same label is used for match on other backup,
                                          * no need to add another MPLS_ENTRY for backup
                                          */
                                         match_add = FALSE;
                                     }
                                 }

                             /* Link in the newly allocated next-hop entry to DVP */
                             soc_ING_DVP_TABLEm_field32_set(unit, &dvp2, NEXT_HOP_INDEXf,
                                                                                           nh_index);
                            /* Enable DVP as Network_port */
                            if (soc_mem_field_valid(unit, ING_DVP_TABLEm, NETWORK_PORTf)) {
                                  soc_ING_DVP_TABLEm_field32_set(unit, &dvp2, NETWORK_PORTf,
                                       ((mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) &&
                                        (mpls_port->flags & BCM_MPLS_PORT_NETWORK) ) ? 1 : 0);
                            }

                            if (soc_feature(unit,
                                soc_feature_multiple_split_horizon_group)) {
                                if (soc_mem_field_valid(
                                    unit, ING_DVP_TABLEm, NETWORK_GROUPf)) {
                                    soc_ING_DVP_TABLEm_field32_set(
                                            unit, &dvp2, NETWORK_GROUPf,
                                            network_group);

                                }
                            }

                             rv = WRITE_ING_DVP_TABLEm(unit, MEM_BLOCK_ALL, active_vp, &dvp2);
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }
                             rv = _bcm_vp_ing_dvp_to_ing_dvp2(unit, &dvp2, active_vp);
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }

                             /* Tie the SVP to DVP. If only one port is valid, point to itself */
                             soc_SOURCE_VPm_field32_set(unit, &svp2, 
                                            ENTRY_TYPEf, 0x2); /* VPWS */
                             soc_SOURCE_VPm_field32_set(unit, &svp2, DVPf,
                                            (vp_valid_flag == 0x3) ? vp1 : vp2);
                             if (soc_mem_field_valid(unit, SOURCE_VPm, DISABLE_VLAN_CHECKSf)) {
                                  soc_SOURCE_VPm_field32_set(unit, &svp2, DISABLE_VLAN_CHECKSf, 1);
                             }
                             if (soc_mem_field_valid(unit, SOURCE_VPm, SRC_PORTf) && 
                                 soc_mem_field_valid(unit, SOURCE_VPm, SRC_MODIDf)) {
                                 rv = _bcm_tr_mpls_l2vpn_source_port_set(unit, mpls_port, &svp2);
                             }
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }
                             rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, active_vp, &svp2);
                             /* we write source_vp entry only when we create the first port or it is primary port.
                                usually primary port will be vp3, as failover needs to be created first */
                             if (rv >= 0) {
                                 int vp3_valid = 0;
                                 if (vp_valid_flag == 0x3) {
                                 vp3 = soc_SOURCE_VPm_field32_get(unit, &svp1, DVPf);
                                 }
                                 else if (vp3_read != -1) {
                                     vp3 = vp3_read;
                                 }
                                 if ((vp3 != -1) && (_bcm_vp_used_get(unit, vp3, _bcmVpTypeMpls))) {
                                     vp3_valid = 1;
                                 }
                                 if (!vp3_valid || (vp3_valid &&
                                     BCM_SUCCESS(_bcm_esw_failover_mpls_check(unit, mpls_port)))) {
                                     if (vp_valid_flag == 0x3) {
                                    soc_SOURCE_VPm_field32_set(unit, &svp1, DVPf, vp2);
                                    if (vp1 != -1) {
                                        rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp1, &svp1);
                                        }
                                     }
                                 } else {
                                     rv = READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp3, &dvp3);
                                     prot_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp3,
                                                                                     NEXT_HOP_INDEXf);

                                     if (bcmi_mpls_port_failover_update_is_required
                                         (unit, mpls_port, prot_index, nh_index)) {
                                         /* if user does not send any if id then he might
                                          * want to use the same NH as prot NH.
                                          * that way next 3 NHs are used as backup NHs.
                                          * other option is that he might be sending mc group,
                                          * but in that case we will not even use failover_if_id.
                                          */

                                     rv = _bcm_esw_failover_prot_nhi_create
                                         (unit, 0, (uint32)prot_index,
                                         nh_index, mpls_port->failover_mc_group,
                                          mpls_port->failover_id);

                                    }
                                 }

                             }
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }
#ifdef BCM_ENDURO_SUPPORT
                             if (SOC_IS_ENDURO(unit)) {
                                 BCM_IF_ERROR_RETURN ( 
                                    READ_SVP_DISABLE_VLAN_CHECKS_TABm(unit, MEM_BLOCK_ANY, active_vp, &svp_disable_check)); 
                                 soc_SVP_DISABLE_VLAN_CHECKS_TABm_field32_set(unit, &svp_disable_check, DISABLE_VLAN_CHECKSf, 1); 
                                 BCM_IF_ERROR_RETURN ( 
                                     WRITE_SVP_DISABLE_VLAN_CHECKS_TABm(unit, MEM_BLOCK_ALL, active_vp, &svp_disable_check)); 
                             }
#endif /* BCM_ENDURO_SUPPORT */
                        }
              }



              else if (active_vp == vp3) {
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) || defined(BCM_APACHE_SUPPORT)
                  if (SOC_IS_KATANAX(unit) || SOC_IS_TRIUMPH3(unit) || SOC_IS_APACHE(unit)) {
                      rv = _bcm_esw_add_policer_to_table(unit,
                                               mpls_port->policer_id,
                                               SOURCE_VPm, 0, &svp3);
                      BCM_IF_ERROR_RETURN(rv);
                  }
#endif /* BCM_KATANA_SUPPORT or BCM_TRIUMPH3_SUPPORT or BCM_APACHE_SUPPORT */
                        /* Set SOURCE_VP */
                        soc_SOURCE_VPm_field32_set(unit, &svp3, CLASS_IDf,
                                            mpls_port->if_class);
                        if (soc_feature(unit,
                            soc_feature_multiple_split_horizon_group)) {
                            soc_SOURCE_VPm_field32_set(
                                    unit, &svp3, NETWORK_GROUPf, network_group);
                        } else {
                            soc_SOURCE_VPm_field32_set(
                                unit, &svp3, NETWORK_PORTf,
                                (mpls_port->flags & BCM_MPLS_PORT_NETWORK) ?
                                1 : 0);
                        }
                        if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_TAGGED) {
                             soc_SOURCE_VPm_field32_set(unit, &svp3, 
                                            SD_TAG_MODEf, 1);
                             soc_SOURCE_VPm_field32_set(unit, &svp3, 
                                            TPID_ENABLEf, tpid_enable);
                             if (SOC_MEM_FIELD_VALID(unit, SOURCE_VPm, TRUST_OUTER_DOT1Pf)) {
                                  soc_SOURCE_VPm_field32_set(unit, &svp3, TRUST_OUTER_DOT1Pf, 1);
                             }
                        } else {
                             soc_SOURCE_VPm_field32_set(unit, &svp3, 
                                            SD_TAG_MODEf, 0);
                        }
                        if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
                             rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp3, &svp3);
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }
                             /* Link in the newly allocated next-hop entry */
                             soc_ING_DVP_TABLEm_field32_set(unit, &dvp3, 
                                       NEXT_HOP_INDEXf, nh_index);
                            /* Enable DVP as Network_port */
                            if (soc_mem_field_valid(unit, ING_DVP_TABLEm, NETWORK_PORTf)) {
                                  soc_ING_DVP_TABLEm_field32_set(unit, &dvp3, NETWORK_PORTf,
                                       ((mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) &&
                                        (mpls_port->flags & BCM_MPLS_PORT_NETWORK) ) ? 1 : 0);
                            }

                            if (soc_feature(unit,
                                soc_feature_multiple_split_horizon_group)) {
                                if (soc_mem_field_valid(
                                    unit, ING_DVP_TABLEm, NETWORK_GROUPf)) {
                                    soc_ING_DVP_TABLEm_field32_set(
                                        unit, &dvp3, NETWORK_GROUPf, network_group);
                               }
                            }

                             rv = WRITE_ING_DVP_TABLEm(unit, MEM_BLOCK_ALL, 
                                       active_vp, &dvp3);
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }
                             rv = _bcm_vp_ing_dvp_to_ing_dvp2(unit, &dvp3, active_vp);
                             if (rv < 0) {
                                 goto vpws_cleanup;
                             }

                        } else {
                             /* Link in the newly allocated next-hop entry to DVP */
                             soc_ING_DVP_TABLEm_field32_set(unit, &dvp3, 
                                            NEXT_HOP_INDEXf, nh_index);
                            /* Enable DVP as Network_port */
                            if (soc_mem_field_valid(unit, ING_DVP_TABLEm, NETWORK_PORTf)) {
                                  soc_ING_DVP_TABLEm_field32_set(unit, &dvp3, NETWORK_PORTf,
                                       ((mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) &&
                                        (mpls_port->flags & BCM_MPLS_PORT_NETWORK) ) ? 1 : 0);
                            }

                            if (soc_feature(unit,
                                soc_feature_multiple_split_horizon_group)) {
                                if (soc_mem_field_valid(
                                    unit, ING_DVP_TABLEm, NETWORK_GROUPf)) {
                                    soc_ING_DVP_TABLEm_field32_set(
                                        unit, &dvp3, NETWORK_GROUPf, network_group);
                                }
                            }

                             rv = WRITE_ING_DVP_TABLEm(unit, MEM_BLOCK_ALL, 
                                            active_vp, &dvp3);
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }
                             rv = _bcm_vp_ing_dvp_to_ing_dvp2(unit, &dvp3, active_vp);
                             if (rv < 0) {
                                 goto vpws_cleanup;
                             }

                             if (vpn == BCM_MPLS_VPWS_VPN_INVALID) {
                                  /* Set SVP Invalid */
                                  soc_SOURCE_VPm_field32_set(unit, &svp3, 
                                                      ENTRY_TYPEf, 0x0);
                             } else {
                                  /* Set SVP Valid */
                                  soc_SOURCE_VPm_field32_set(unit, &svp3, 
                                                      ENTRY_TYPEf, 0x2);
                                  /* Tie SVP to DVP. */
                                  if (vp1 != -1) {
                                      soc_SOURCE_VPm_field32_set(unit, &svp3, DVPf, vp1);
                                  }
                             }
                             if (soc_mem_field_valid(unit, SOURCE_VPm, DISABLE_VLAN_CHECKSf)) {
                                  soc_SOURCE_VPm_field32_set(unit, &svp3, DISABLE_VLAN_CHECKSf, 1);
                             }
                             if (soc_mem_field_valid(unit, SOURCE_VPm, SRC_PORTf) && 
                                 soc_mem_field_valid(unit, SOURCE_VPm, SRC_MODIDf)) {
                                 rv = _bcm_tr_mpls_l2vpn_source_port_set(unit, mpls_port, &svp3);
                             }
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }
                             rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, 
                                            active_vp, &svp3);
                             if ((rv >= 0) && (vp_valid_flag == 0x3)) {
                                  soc_SOURCE_VPm_field32_set(unit, &svp1, DVPf, vp3);
                                  rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp1, &svp1);
                             }
                             if (rv < 0) {
                                  goto vpws_cleanup;
                             }
#ifdef BCM_ENDURO_SUPPORT
                             if (SOC_IS_ENDURO(unit)) {
                                 BCM_IF_ERROR_RETURN ( 
                                    READ_SVP_DISABLE_VLAN_CHECKS_TABm(unit, MEM_BLOCK_ANY, active_vp, &svp_disable_check)); 
                                 soc_SVP_DISABLE_VLAN_CHECKS_TABm_field32_set(unit, &svp_disable_check, DISABLE_VLAN_CHECKSf, 1); 
                                 BCM_IF_ERROR_RETURN ( 
                                    WRITE_SVP_DISABLE_VLAN_CHECKS_TABm(unit, MEM_BLOCK_ALL, active_vp, &svp_disable_check)); 
                            }
#endif /* BCM_ENDURO_SUPPORT */
                        }

              }

#if defined(BCM_TRIUMPH2_SUPPORT)
            if (soc_feature(unit, soc_feature_mpls_failover)) {
                int failover_vp = 0;
                bcm_mpls_port_t   failover_mpls_port;

                if (BCM_SUCCESS(_bcm_esw_failover_mpls_check(unit, mpls_port))) {

                    failover_vp = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->failover_port_id);

                    if (failover_vp >= 0) {
                        if (!_bcm_vp_used_get(unit, failover_vp, _bcmVpTypeMpls)) {
                            rv = BCM_E_NOT_FOUND;
                            goto vpws_cleanup;
                        }
                        rv = _bcm_esw_mpls_match_get(unit,  &failover_mpls_port, failover_vp);
                        if ( rv < 0 ) {
                            if (rv != BCM_E_NOT_FOUND) {
                                goto vpws_cleanup;
                            }
                        }
                        if (mpls_port->criteria == BCM_MPLS_PORT_MATCH_LABEL_PORT) {
                            if ((mpls_port->match_label == failover_mpls_port.match_label) &&
                                (mpls_port->port == failover_mpls_port.port)) {

                                /*
                                 * Replace failover_vp with Primary vp, keep software record
                                 * for get operation.
                                 */
                                rv = _bcm_esw_mpls_match_delete(unit, failover_vp, FALSE);
                            }
                        } else if (mpls_port->criteria == BCM_MPLS_PORT_MATCH_LABEL) {
                            if (mpls_port->match_label == failover_mpls_port.match_label) {
                                /*
                                 * Replace failover_vp with Primary vp, keep software record
                                 * for get operation.
                                 */
                                rv = _bcm_esw_mpls_match_delete(unit, failover_vp, FALSE);
                            }
                        }
                    }
                }
            }
#endif /* BCM_TRIUMPH2_SUPPORT  */

         if (soc_mem_field_valid(unit, EGR_DVP_ATTRIBUTEm, MTU_VALUEf)) {
            rv = _bcm_tr_mpls_port_mtu_set(unit, active_vp, mpls_port->mtu);
             if (rv < 0) {
                  goto vpws_cleanup;
             }
         }

         if (TRUE == match_add) {
            rv = _bcm_esw_mpls_match_add(unit, mpls_port, active_vp, vpn, TRUE);
         }

            /* Set the MPLS port ID */
            if (!(mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
                BCM_GPORT_MPLS_PORT_ID_SET(mpls_port->mpls_port_id, active_vp);
                mpls_port->encap_id = BCM_XGS3_DVP_EGRESS_IDX_MIN + nh_index;
            }
#if defined(BCM_TRIUMPH2_SUPPORT)
            if (!BCM_GPORT_IS_TRUNK(mpls_port->port)) {
                if (!(mpls_port->flags2 & BCM_MPLS_PORT2_QOS_NONE)) {
                    _bcm_mpls_source_vp_set(unit, mpls_port, active_vp);
                }
            }
#endif

        /* Free the old next-hop entry. */
        if (vp_valid_flag != 0x30) {
            (void)_bcm_tr_mpls_l2_old_nh_info_delete(unit, mpls_port, nh_index,
                                                     &old_nh_info);
        }

vpws_cleanup:
        if (rv < 0) {
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) || \
                                                    defined(BCM_APACHE_SUPPORT)
            if (SOC_IS_KATANAX(unit) || SOC_IS_TRIUMPH3(unit) ||
                                            SOC_IS_APACHE(unit)) {
                _bcm_esw_reset_policer_from_table(unit, 
                                              mpls_port->policer_id, 
                                              SOURCE_VPm, active_vp, &svp1); 
            }
#endif /* BCM_KATANA_SUPPORT or BCM_TRIUMPH3_SUPPORT or BCM_APACHE_SUPPORT */
            if (tpid_enable) {
                (void) _bcm_fb2_outer_tpid_entry_delete(unit, tpid_index);
            }
            if (!(mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
                (void) _bcm_tr_mpls_l2_nh_info_delete(unit, nh_index);
            
                /* Free the VP's */
                if (vp_valid_flag) {
                    if (vp1 != -1) {
                        (void) _bcm_vp_free(unit, _bcmVpTypeMpls, 1, vp1);
                    }
                    if (vp2 != -1) {
                        (void) _bcm_vp_free(unit, _bcmVpTypeMpls, 1, vp2);
                        _BCM_MPLS_NETWORK_VP_USED_CLR(unit, vp2);
                    }                 
                }
            }
            return rv;
        }
        return rv;
}

/*
 * Function:
 *     _bcm_tr_mpls_vpls_port_add
 * Purpose:
 *      Add VPLS port to a VPN
 * Parameters:
 *   unit - (IN) Device Number
 *   vpn - (IN) VPN instance ID
 *   mpls_port - (IN/OUT) mpls port information (OUT : mpls_port_id)
 * Returns:
 *     BCM_E_XXX
 */

STATIC int
_bcm_tr_mpls_vpls_port_add(int unit, bcm_vpn_t vpn, bcm_mpls_port_t *mpls_port)
{
    int vp, num_vp, vfi = 0;
    source_vp_entry_t svp;
    ing_dvp_table_entry_t dvp;
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) \
    || defined(BCM_TRIDENT_SUPPORT)
    ing_dvp_2_table_entry_t dvp2;
#endif /* BCM_TRIDENT2_SUPPORT or BCM_TRIUMPH3_SUPPORT or BCM_TRIDENT_SUPPORT */
    ing_l3_next_hop_entry_t ing_nh;
    int modid_local=0, module_id, is_trunk=0;
    bcm_port_t local_port;
    bcm_trunk_t trunk_id;
    int tpid_enable = 0, tpid_index;
    int drop, is_local = 0, rv = BCM_E_PARAM, nh_index = 0;
    int cml_default_enable=0, cml_default_new=0, cml_default_move=0;
    _bcm_mpls_old_nh_info_t old_nh_info;
#ifdef BCM_ENDURO_SUPPORT
    svp_disable_vlan_checks_tab_entry_t svp_disable_check;
#endif /* BCM_ENDURO_SUPPORT */
    int entry_update = TRUE;
    _bcm_vp_info_t vp_info;
    bcm_switch_network_group_t network_group;
#ifdef BCM_TRIUMPH2_SUPPORT
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    uint32 old_failover_vp = 0;
#endif
    _bcm_vp_info_init(&vp_info);
    vp_info.vp_type = _bcmVpTypeMpls;

    if (mpls_port->criteria != BCM_MPLS_PORT_MATCH_SHARE) {
        if ( vpn != BCM_MPLS_VPLS_VPN_INVALID) {
            _BCM_MPLS_VPN_GET(vfi, _BCM_MPLS_VPN_TYPE_VPLS, vpn);
            if (!_bcm_vfi_used_get(unit, vfi, _bcmVfiTypeMpls)) {
                return BCM_E_NOT_FOUND;
            }
        } else {
            vfi = 0x3FF;
        }
    }

    if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
        vp = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->mpls_port_id);

        if (vp == -1) {
            return BCM_E_PARAM;
        }

        if (_BCM_MPLS_GPORT_FAILOVER_VPLESS_GET(vp)) {
            vp = _BCM_MPLS_GPORT_FAILOVER_VPLESS_CLEAR(vp);
        }

        if (!_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
            return BCM_E_NOT_FOUND;
        }
        rv = READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp);
        if (rv < 0) {
            return rv;
        }
        rv = READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp, &dvp);
        if (rv < 0) {
            return rv;
        }
        nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp, 
                                                  NEXT_HOP_INDEXf);
        if (soc_feature(unit, soc_feature_mpls_software_failover)) {        
           /* VP Detach */
           if ((soc_SOURCE_VPm_field32_get(unit, &svp, ENTRY_TYPEf) == 0x1) && 
                                        (vpn == BCM_MPLS_VPLS_VPN_INVALID)){
             soc_SOURCE_VPm_field32_set(unit, &svp, ENTRY_TYPEf, 0x0);
             BCM_IF_ERROR_RETURN (soc_mem_read(unit, ING_L3_NEXT_HOPm, 
                                               MEM_BLOCK_ANY, nh_index, &ing_nh));             
             /* Set Drop */
             soc_mem_field32_set(unit, ING_L3_NEXT_HOPm, &ing_nh, DROPf, 0x1); 
             BCM_IF_ERROR_RETURN(soc_mem_write (unit, ING_L3_NEXT_HOPm, 
                                        MEM_BLOCK_ALL, nh_index, &ing_nh));
             rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp, &svp);
             return rv;
           }
     
           /* VP Attach */
           if ((soc_SOURCE_VPm_field32_get(unit, &svp, ENTRY_TYPEf) == 0x0) && 
                                        (vpn != BCM_MPLS_VPLS_VPN_INVALID)){
             soc_SOURCE_VPm_field32_set(unit, &svp, ENTRY_TYPEf, 0x1);
             BCM_IF_ERROR_RETURN (soc_mem_read(unit, ING_L3_NEXT_HOPm, 
                                               MEM_BLOCK_ANY, nh_index, &ing_nh));             
             /* Reset Drop */
             soc_mem_field32_set(unit, ING_L3_NEXT_HOPm, &ing_nh, DROPf, 0x0); 
             BCM_IF_ERROR_RETURN(soc_mem_write (unit, ING_L3_NEXT_HOPm, 
                                        MEM_BLOCK_ALL, nh_index, &ing_nh));
             if (mpls_port->criteria != BCM_MPLS_PORT_MATCH_SHARE) {
                soc_SOURCE_VPm_field32_set(unit, &svp, VFIf, vfi);
             } 
             rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp, &svp);
             return rv;
           }
        }

    } else if (mpls_port->flags & BCM_MPLS_PORT_WITH_ID ) {
        int vpless_port = FALSE;
        if (!BCM_GPORT_IS_MPLS_PORT(mpls_port->mpls_port_id)) {
            return (BCM_E_BADID);
        }

        num_vp = soc_mem_index_count(unit, SOURCE_VPm);
        vp = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->mpls_port_id);
        if (_BCM_MPLS_GPORT_FAILOVER_VPLESS_GET(vp)) {
            vp = _BCM_MPLS_GPORT_FAILOVER_VPLESS_CLEAR(vp);
            vpless_port = TRUE;
        }

        if (vp == -1) {
            return BCM_E_PARAM;
        }

        if (vp >= num_vp) {
            return (BCM_E_BADID);
        }

        if ((vpn != BCM_MPLS_VPLS_VPN_INVALID) && (!vpless_port)) {
            if (_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
                return BCM_E_EXISTS;
            }
        }

        if (mpls_port->flags & BCM_MPLS_PORT_NETWORK) {
            vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
        } else {
            vp_info.flags &= ~(_BCM_VP_INFO_NETWORK_PORT);
        }

        BCM_IF_ERROR_RETURN(_bcm_vp_used_set(unit, vp, vp_info));

        /* ------------  SW Failover - Applicable for TRIUMPH/VL only ---- */
        if (soc_feature(unit, soc_feature_mpls_software_failover)) {

            if (!_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
                return BCM_E_NOT_FOUND;
            }
            rv = READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp);
            if (rv < 0) {
                return rv;
            }
            rv = READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp, &dvp);
            if (rv < 0) {
                return rv;
            }
            nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp, NEXT_HOP_INDEXf);
            if ((nh_index != 0)  &&  (nh_index < soc_mem_index_count(unit, ING_L3_NEXT_HOPm))) {
               BCM_IF_ERROR_RETURN (soc_mem_read(unit, ING_L3_NEXT_HOPm, 
                                                           MEM_BLOCK_ANY, nh_index, &ing_nh));
               module_id = soc_mem_field32_get(unit, ING_L3_NEXT_HOPm, 
                                                           &ing_nh, MODULE_IDf);
               is_trunk = soc_mem_field32_get(unit, ING_L3_NEXT_HOPm, 
                                                           &ing_nh, Tf);
               if (!is_trunk) {
                   BCM_IF_ERROR_RETURN
                   ( _bcm_esw_modid_is_local(unit, module_id, &modid_local));
               }
               if (modid_local) {
                  local_port = 
                     soc_mem_field32_get(unit, ING_L3_NEXT_HOPm, &ing_nh, PORT_NUMf);
               } else if (is_trunk) {
                     trunk_id = 
                       soc_mem_field32_get(unit, ING_L3_NEXT_HOPm, &ing_nh, TGIDf); 
                     BCM_GPORT_TRUNK_SET(local_port, trunk_id);
               }

               /* VP Detach */
               if ((soc_SOURCE_VPm_field32_get(unit, &svp, ENTRY_TYPEf) == 0x1) && 
                                            (vpn == BCM_MPLS_VPLS_VPN_INVALID)){
                 soc_SOURCE_VPm_field32_set(unit, &svp, ENTRY_TYPEf, 0x0);
                 /* Set Drop */
                 soc_mem_field32_set(unit, ING_L3_NEXT_HOPm, &ing_nh, DROPf, 0x1); 
                 BCM_IF_ERROR_RETURN(soc_mem_write (unit, ING_L3_NEXT_HOPm, 
                                            MEM_BLOCK_ALL, nh_index, &ing_nh));
                 rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp, &svp);
                 return rv;
               }
         
               /* VP Attach */
               if ((soc_SOURCE_VPm_field32_get(unit, &svp, ENTRY_TYPEf) == 0x0) && 
                                            (vpn != BCM_MPLS_VPLS_VPN_INVALID)){
                 soc_SOURCE_VPm_field32_set(unit, &svp, ENTRY_TYPEf, 0x1);
                 /* Reset Drop */
                 soc_mem_field32_set(unit, ING_L3_NEXT_HOPm, &ing_nh, DROPf, 0x0); 
                 BCM_IF_ERROR_RETURN(soc_mem_write (unit, ING_L3_NEXT_HOPm, 
                                            MEM_BLOCK_ALL, nh_index, &ing_nh));
                 rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp, &svp);
                 return rv;
               }
            }
        }

        sal_memset(&svp, 0, sizeof(source_vp_entry_t));
        sal_memset(&dvp, 0, sizeof(ing_dvp_table_entry_t));
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) \
    || defined(BCM_TRIDENT_SUPPORT)
        if (SOC_IS_TRIUMPH3(unit) || SOC_IS_TRIDENT2X(unit) 
            || SOC_IS_TRIDENT(unit)) {
            sal_memset(&dvp2, 0, sizeof(ing_dvp_2_table_entry_t));
        }
#endif /* BCM_TRIDENT2_SUPPORT or BCM_TRIUMPH3_SUPPORT or BCM_TRIDENT_SUPPORT */
    } else {
        if (mpls_port->flags & BCM_MPLS_PORT_NETWORK) {
            vp_info.flags |= _BCM_VP_INFO_NETWORK_PORT;
        } else {
            vp_info.flags &= ~(_BCM_VP_INFO_NETWORK_PORT);
        }

        if (mpls_port->criteria == BCM_MPLS_PORT_MATCH_SHARE) {
            vp_info.flags |= _BCM_VP_INFO_SHARED_PORT;
        }

        if (mpls_port->flags & BCM_MPLS_PORT_FAILOVER) {
             /* VP conservation */
             vp = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->failover_port_id);
             if (vp == -1) {
                 return BCM_E_PARAM;
             }
        } else {
           /* allocate a new VP index */
           num_vp = soc_mem_index_count(unit, SOURCE_VPm);
           rv = _bcm_vp_alloc(unit, 0, (num_vp - 1), 1, SOURCE_VPm, vp_info, &vp);
           if (rv < 0) {
               return rv;
           }
        }
        sal_memset(&svp, 0, sizeof(source_vp_entry_t));
        sal_memset(&dvp, 0, sizeof(ing_dvp_table_entry_t));
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) \
    || defined(BCM_TRIDENT_SUPPORT)
        if (SOC_IS_TRIUMPH3(unit) || SOC_IS_TRIDENT2X(unit) 
            || SOC_IS_TRIDENT(unit)) {
            sal_memset(&dvp2, 0, sizeof(ing_dvp_2_table_entry_t));
        }
#endif /* BCM_TRIDENT2_SUPPORT or BCM_TRIUMPH3_SUPPORT or BCM_TRIDENT_SUPPORT */
        BCM_IF_ERROR_RETURN(_bcm_vp_used_set(unit, vp, vp_info));
    }
#ifdef BCM_TRIUMPH2_SUPPORT 
    if (soc_feature(unit, soc_feature_mpls_failover)) {
        old_failover_vp = mpls_info->failover_vp[vp];
    }
#endif
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) || \
                                        defined(BCM_APACHE_SUPPORT)
    if (SOC_IS_KATANAX(unit) || SOC_IS_TRIUMPH3(unit) || SOC_IS_APACHE(unit)) {
        rv = _bcm_esw_add_policer_to_table(unit, mpls_port->policer_id, 
                                           SOURCE_VPm, 0, &svp);
        BCM_IF_ERROR_RETURN(rv);
    }
#endif /* BCM_KATANA_SUPPORT or BCM_TRIUMPH3_SUPPORT BCM_APACHE_SUPPORT */

    if (!(mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
        /* Set the MPLS port ID */
        BCM_GPORT_MPLS_PORT_ID_SET(mpls_port->mpls_port_id, vp);
    }

    if (mpls_port->flags & BCM_MPLS_PORT_ENCAP_WITH_ID) {
        /* Input parameters check. */
        if (!BCM_XGS3_L3_EGRESS_IDX_VALID(unit, mpls_port->encap_id) &&
            !BCM_XGS3_DVP_EGRESS_IDX_VALID(unit, mpls_port->encap_id)) {
            return (BCM_E_PARAM);
        }

        /* Calculate next hop index. */
        if (BCM_XGS3_L3_EGRESS_IDX_VALID(unit, mpls_port->encap_id)) {
            nh_index = mpls_port->encap_id - BCM_XGS3_EGRESS_IDX_MIN;
        } else {
            nh_index = mpls_port->encap_id - BCM_XGS3_DVP_EGRESS_IDX_MIN;
        }
    }

    if (soc_feature(unit, soc_feature_multiple_split_horizon_group)) {
        network_group = mpls_port->network_group_id;
        rv = _bcm_validate_splithorizon_network_group(unit,
                mpls_port->flags & BCM_MPLS_PORT_NETWORK, &network_group);
         BCM_IF_ERROR_RETURN(rv);
    }

    drop = (mpls_port->flags & BCM_MPLS_PORT_DROP) ? 1 : 0;
    rv = _bcm_tr_mpls_l2_nh_info_add(unit, mpls_port, vp, drop, 0,
                                     &nh_index, &local_port, &is_local,
                                     &old_nh_info);
    if (rv < 0) {
        if (!(mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
            (void) _bcm_vp_free(unit, _bcmVpTypeMpls, 1, vp);
            _BCM_MPLS_NETWORK_VP_USED_CLR(unit, vp);
        }
        return rv;
    }

    if (mpls_port->flags & BCM_MPLS_PORT_FAILOVER) {
         rv = _bcm_esw_mpls_match_add(unit, mpls_port, 
                 _BCM_MPLS_GPORT_FAILOVER_VPLESS_SET(vp), vpn, TRUE);
         mpls_port->mpls_port_id = 
              _BCM_MPLS_GPORT_FAILOVER_VPLESS_SET(mpls_port->mpls_port_id);
         mpls_port->encap_id = BCM_XGS3_DVP_EGRESS_IDX_MIN + nh_index;
         if (soc_feature(unit, soc_feature_mpls_software_failover)) {
             if (mpls_port->flags & BCM_MPLS_PORT_ENCAP_WITH_ID) {
                 rv = soc_mem_field32_modify(unit, ING_DVP_TABLEm,
                                                vp, NEXT_HOP_INDEXf, nh_index);
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) \
    || defined(BCM_TRIDENT_SUPPORT)
                 if (SOC_IS_TRIUMPH3(unit) || SOC_IS_TRIDENT2X(unit) 
                     || SOC_IS_TRIDENT(unit)) {
                 rv = soc_mem_field32_modify(unit, ING_DVP_2_TABLEm,
                                                vp, NEXT_HOP_INDEXf, nh_index);
                 }
#endif /* BCM_TRIDENT2_SUPPORT or BCM_TRIUMPH3_SUPPORT or BCM_TRIDENT_SUPPORT */ 
             }
         }
         goto vpls_cleanup;
    }

    if (mpls_port->flags & BCM_MPLS_PORT_SERVICE_TAGGED) {
        rv = _bcm_fb2_outer_tpid_lkup(unit, mpls_port->service_tpid,
                                           &tpid_index);
        if (rv < 0) {
            goto vpls_cleanup;
        }
        tpid_enable = (1 << tpid_index);
        soc_SOURCE_VPm_field32_set(unit, &svp, SD_TAG_MODEf, 1);
        soc_SOURCE_VPm_field32_set(unit, &svp, TPID_ENABLEf, tpid_enable);
        if (SOC_MEM_FIELD_VALID(unit, SOURCE_VPm, TRUST_OUTER_DOT1Pf)) {
              soc_SOURCE_VPm_field32_set(unit, &svp, TRUST_OUTER_DOT1Pf, 1);
        }
    } else {
        soc_SOURCE_VPm_field32_set(unit, &svp, SD_TAG_MODEf, 0);
    }
    soc_SOURCE_VPm_field32_set(unit, &svp, CLASS_IDf, 
                               mpls_port->if_class);
    if (soc_feature(unit, soc_feature_multiple_split_horizon_group)) {
        soc_SOURCE_VPm_field32_set(unit, &svp, NETWORK_GROUPf, network_group);
    } else {
        soc_SOURCE_VPm_field32_set(unit, &svp, NETWORK_PORTf,
                            (mpls_port->flags & BCM_MPLS_PORT_NETWORK) ? 1 : 0);
    }
    if (mpls_port->flags & BCM_MPLS_PORT_NETWORK) {
        _BCM_MPLS_NETWORK_VP_USED_SET(unit, vp);
    }

    if (mpls_port->flags & BCM_MPLS_PORT_REPLACE) {
        rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp, &svp);

        /* Link in the newly allocated next-hop entry in ING_DVP_TABLE */
        soc_ING_DVP_TABLEm_field32_set(unit, &dvp, NEXT_HOP_INDEXf,
                                       nh_index);

        if (soc_mem_field_valid(unit, ING_DVP_TABLEm, NETWORK_PORTf)) {
            if ((mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) &&
                (mpls_port->flags & BCM_MPLS_PORT_NETWORK)) { /* Network */
#ifdef BCM_RIOT_SUPPORT
                if (BCMI_RIOT_IS_ENABLED(unit)) {
                    soc_ING_DVP_TABLEm_field32_set(unit, &dvp, NETWORK_PORTf, 0);
                } else
#endif
                {
                    soc_ING_DVP_TABLEm_field32_set(unit, &dvp, NETWORK_PORTf, 1);
                }
            } else { /* Access */
                soc_ING_DVP_TABLEm_field32_set(unit, &dvp, NETWORK_PORTf, 0);
            }
        }

        if (soc_feature(unit, soc_feature_multiple_split_horizon_group)) {
            if (soc_mem_field_valid(unit, ING_DVP_TABLEm, NETWORK_GROUPf)) {
#ifdef BCM_RIOT_SUPPORT
                if (BCMI_RIOT_IS_ENABLED(unit)) {
                    soc_ING_DVP_TABLEm_field32_set(unit, &dvp, NETWORK_GROUPf, 0);
                } else
#endif
                {
                    soc_ING_DVP_TABLEm_field32_set(unit, &dvp, NETWORK_GROUPf, network_group);
                }
            }
        }

        rv = WRITE_ING_DVP_TABLEm(unit, MEM_BLOCK_ALL, vp, &dvp);
        if (rv < 0) {
            goto vpls_cleanup;
        }
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) \
    || defined(BCM_TRIDENT_SUPPORT)
        if (SOC_IS_TRIUMPH3(unit) || SOC_IS_TRIDENT2X(unit) 
            || SOC_IS_TRIDENT(unit)) { 
           /* Link in the newly allocated next-hop entry in ING_DVP_2_TABLE
            * Since Field module uses DVP_2_TABLE for REDIRECT_TO_DVP Action
            * for devices TD/TD2/TR3
            */
           sal_memset(&dvp2, 0, sizeof(ing_dvp_2_table_entry_t));
           BCM_IF_ERROR_RETURN(READ_ING_DVP_2_TABLEm(unit, MEM_BLOCK_ANY,
                                                     vp, &dvp2));
           soc_ING_DVP_2_TABLEm_field32_set(unit, &dvp2, NEXT_HOP_INDEXf,
                                          nh_index);

           if (soc_mem_field_valid(unit, ING_DVP_2_TABLEm, NETWORK_PORTf)) {
               if ((mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) &&
                   (mpls_port->flags & BCM_MPLS_PORT_NETWORK)) { /* Network */
#ifdef BCM_RIOT_SUPPORT
                   if (BCMI_RIOT_IS_ENABLED(unit)) {
                       soc_ING_DVP_2_TABLEm_field32_set(unit, &dvp2, NETWORK_PORTf, 0);
                   } else
#endif
                   {
                       soc_ING_DVP_2_TABLEm_field32_set(unit, &dvp2, NETWORK_PORTf, 1);
                   }
               } else { /* Access */
                   soc_ING_DVP_2_TABLEm_field32_set(unit, &dvp2, NETWORK_PORTf, 0);
               }
           }
           
           if ((soc_feature(unit, soc_feature_multiple_split_horizon_group)) &&
              (soc_mem_field_valid(unit, ING_DVP_2_TABLEm, NETWORK_GROUPf))) {
#ifdef BCM_RIOT_SUPPORT
               if (BCMI_RIOT_IS_ENABLED(unit)) {
                   soc_ING_DVP_2_TABLEm_field32_set(unit, &dvp2, NETWORK_GROUPf, 0);
               } else
#endif
               {
                   soc_ING_DVP_2_TABLEm_field32_set(unit, &dvp2, NETWORK_GROUPf, network_group);
               }
           }

           rv = WRITE_ING_DVP_2_TABLEm(unit, MEM_BLOCK_ALL, vp, &dvp2);
           if (rv < 0) {
               goto vpls_cleanup;
           }
        }
#endif /* BCM_TRIDENT2_SUPPORT or BCM_TRIUMPH3_SUPPORT or BCM_TRIDENT_SUPPORT */
    } else {
        /* Link in the newly allocated next-hop entry in ING_DVP_TABLE */
        soc_ING_DVP_TABLEm_field32_set(unit, &dvp, NEXT_HOP_INDEXf,
                                       nh_index);

        if (soc_mem_field_valid(unit, ING_DVP_TABLEm, NETWORK_PORTf)) {
            if ((mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) &&
                (mpls_port->flags & BCM_MPLS_PORT_NETWORK)) { /* Network */
#ifdef BCM_RIOT_SUPPORT
                if (BCMI_RIOT_IS_ENABLED(unit)) {
                    soc_ING_DVP_TABLEm_field32_set(unit, &dvp, NETWORK_PORTf, 0);
                } else
#endif
                {
                    soc_ING_DVP_TABLEm_field32_set(unit, &dvp, NETWORK_PORTf, 1);
                }
            } else { /* Access */
                soc_ING_DVP_TABLEm_field32_set(unit, &dvp, NETWORK_PORTf, 0);
            }
        }

        if (soc_feature(unit, soc_feature_multiple_split_horizon_group)) {
            if (soc_mem_field_valid(unit, ING_DVP_TABLEm, NETWORK_GROUPf)) {
#ifdef BCM_RIOT_SUPPORT
                if (BCMI_RIOT_IS_ENABLED(unit)) {
                    soc_ING_DVP_TABLEm_field32_set(unit, &dvp, NETWORK_GROUPf, 0);
                } else
#endif
                {
                    soc_ING_DVP_TABLEm_field32_set(unit, &dvp, NETWORK_GROUPf, network_group);
                }
            }
        }

        rv = WRITE_ING_DVP_TABLEm(unit, MEM_BLOCK_ALL, vp, &dvp);
        if (rv < 0) {
            goto vpls_cleanup;
        }
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) \
    || defined(BCM_TRIDENT_SUPPORT)
        if (SOC_IS_TRIUMPH3(unit) || SOC_IS_TRIDENT2X(unit)
             || SOC_IS_TRIDENT(unit)) {
           /* Link in the newly allocated next-hop entry in ING_DVP_2_TABLE
            * Since Field module uses DVP_2_TABLE for REDIRECT_TO_DVP Action
            * for devices TD/TD2/TR3
            */
           soc_ING_DVP_2_TABLEm_field32_set(unit, &dvp2, NEXT_HOP_INDEXf,
                                          nh_index);

           if (soc_mem_field_valid(unit, ING_DVP_2_TABLEm, NETWORK_PORTf)) {
               if ((mpls_port->flags & BCM_MPLS_PORT_EGRESS_TUNNEL) &&
                   (mpls_port->flags & BCM_MPLS_PORT_NETWORK)) { /* Network */
#ifdef BCM_RIOT_SUPPORT
                   if (BCMI_RIOT_IS_ENABLED(unit)) {
                       soc_ING_DVP_2_TABLEm_field32_set(unit, &dvp2, NETWORK_PORTf, 0);
                   } else
#endif
                   {
                       soc_ING_DVP_2_TABLEm_field32_set(unit, &dvp2, NETWORK_PORTf, 1);
                   }
               } else { /* Access */
                   soc_ING_DVP_2_TABLEm_field32_set(unit, &dvp2, NETWORK_PORTf, 0);
               }
           }

           if (soc_feature(unit, soc_feature_multiple_split_horizon_group)) {
               if (soc_mem_field_valid(unit, ING_DVP_2_TABLEm, NETWORK_GROUPf)) {
#ifdef BCM_RIOT_SUPPORT
                   if (BCMI_RIOT_IS_ENABLED(unit)) {
                       soc_ING_DVP_2_TABLEm_field32_set(unit, &dvp2, NETWORK_GROUPf, 0);
                   } else
#endif
                   {
                       soc_ING_DVP_2_TABLEm_field32_set(unit, &dvp2, NETWORK_GROUPf, network_group);
                   }
               }
           }

           rv = WRITE_ING_DVP_2_TABLEm(unit, MEM_BLOCK_ALL, vp, &dvp2);
           if (rv < 0) {
               goto vpls_cleanup;
           }
        }
#endif /* BCM_TRIDENT2_SUPPORT or BCM_TRIUMPH3_SUPPORT or BCM_TRIDENT_SUPPORT */

        if (vpn == BCM_MPLS_VPLS_VPN_INVALID) {
            soc_SOURCE_VPm_field32_set(unit, &svp, 
                                       ENTRY_TYPEf, 0x0); /* INVALID */
            rv = soc_mem_read(unit, ING_L3_NEXT_HOPm, MEM_BLOCK_ANY, nh_index, &ing_nh);
            if (rv < 0) {
                goto vpls_cleanup;
            }
            soc_mem_field32_set(unit, ING_L3_NEXT_HOPm, &ing_nh, DROPf, 0x1); /* Set Drop */
            rv = soc_mem_write (unit, ING_L3_NEXT_HOPm, MEM_BLOCK_ALL, nh_index, &ing_nh);	   
            if (rv < 0) {
                goto vpls_cleanup;
            }
        } else {
            /* Initialize the SVP parameters */
            soc_SOURCE_VPm_field32_set(unit, &svp, 
                                       ENTRY_TYPEf, 0x1); /* VPLS */
        }

        if (mpls_port->criteria != BCM_MPLS_PORT_MATCH_SHARE) {
            soc_SOURCE_VPm_field32_set(unit, &svp, VFIf, vfi);
        }

        rv = _bcm_vp_default_cml_mode_get (unit, 
                           &cml_default_enable, &cml_default_new, 
                           &cml_default_move);
         if (rv < 0) {
             goto vpls_cleanup;
         }

        if (cml_default_enable) {
            /* Set the CML to default values */
            soc_SOURCE_VPm_field32_set(unit, &svp, CML_FLAGS_NEWf, cml_default_new);
            soc_SOURCE_VPm_field32_set(unit, &svp, CML_FLAGS_MOVEf, cml_default_move);
        } else {
            /* Set the CML to PVP_CML_SWITCH by default (hw learn and forward) */
            soc_SOURCE_VPm_field32_set(unit, &svp, CML_FLAGS_NEWf, 0x8);
            soc_SOURCE_VPm_field32_set(unit, &svp, CML_FLAGS_MOVEf, 0x8);
        }
        if (soc_mem_field_valid(unit, SOURCE_VPm, DISABLE_VLAN_CHECKSf)) {
            if (!soc_feature(unit, soc_feature_vlan_vfi_membership)) {
                soc_SOURCE_VPm_field32_set(unit, &svp, DISABLE_VLAN_CHECKSf, 1);
            }
        }
        if (soc_mem_field_valid(unit, SOURCE_VPm, SRC_PORTf) && 
             soc_mem_field_valid(unit, SOURCE_VPm, SRC_MODIDf)) {
             rv = _bcm_tr_mpls_l2vpn_source_port_set(unit, mpls_port, &svp);
        }
        if (rv < 0) {
            goto vpls_cleanup;
        }
        rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp, &svp);
        if (rv < 0) {
            goto vpls_cleanup;
        }
#ifdef BCM_ENDURO_SUPPORT
        if (SOC_IS_ENDURO(unit)) {
            BCM_IF_ERROR_RETURN ( 
                READ_SVP_DISABLE_VLAN_CHECKS_TABm(unit, MEM_BLOCK_ANY, vp, &svp_disable_check)); 
            soc_SVP_DISABLE_VLAN_CHECKS_TABm_field32_set(unit, &svp_disable_check, DISABLE_VLAN_CHECKSf, 1); 
            BCM_IF_ERROR_RETURN ( 
                WRITE_SVP_DISABLE_VLAN_CHECKS_TABm(unit, MEM_BLOCK_ALL, vp, &svp_disable_check)); 
        }
#endif /* BCM_ENDURO_SUPPORT */
    }

#if defined(BCM_TRIUMPH2_SUPPORT)
        if (soc_feature(unit, soc_feature_mpls_failover)) {
            int failover_vp = 0;
            bcm_mpls_port_t  failover_mpls_port;

            if (BCM_SUCCESS(_bcm_esw_failover_mpls_check(unit, mpls_port))) {
                failover_vp = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->failover_port_id);
                if (failover_vp >=0 ) {
                    if (!_bcm_vp_used_get(unit, failover_vp, _bcmVpTypeMpls)) {
                        rv = BCM_E_NOT_FOUND;
                        goto vpls_cleanup;
                    }
                    rv = _bcm_esw_mpls_match_get(unit,  &failover_mpls_port, failover_vp);
                    if ( rv < 0 ) {
                        if (rv != BCM_E_NOT_FOUND) {
                             goto vpls_cleanup;
                        }
                    }
                    if (mpls_port->match_label == failover_mpls_port.match_label) {
                        /* Replace failover_vp with Primary vp
                         * As part of doing this, we should only remove the
                         * h/w entry for backup port and not the software state.
                         */
                        rv = _bcm_esw_mpls_match_delete(unit, failover_vp, FALSE);
                        if (BCM_FAILURE(rv)) {
                            if (rv != BCM_E_NOT_FOUND) {
                                return rv;
                            }
                        }
                    }
                    mpls_info->failover_vp[vp] = failover_vp;
                }
            } else if (mpls_port->failover_id) {
                /*
                 * If this is not a primary port but user
                 * is providing failover id then this is a failover
                 * port and we need to make sure we dont overwrite
                 * primary matching entry,if present.
                 */
                mpls_entry_entry_t return_ment;
                int primary_port = 0, primary_nh_index = 0;

                rv = _bcm_esw_mpls_failover_nw_port_match_get(unit, mpls_port, -1,
                        &return_ment);

                if (rv == BCM_E_NONE) {
                    /* entry exists. dont update h/w entry as this is failover port */
                    entry_update = FALSE;
                    primary_port = soc_MPLS_ENTRYm_field32_get(unit,
                        &return_ment, MPLS__SOURCE_VPf);

                    if (primary_port > 0) {
                        ing_dvp_table_entry_t p_dvp;

                        BCM_IF_ERROR_RETURN (READ_ING_DVP_TABLEm(unit,
                            MEM_BLOCK_ANY, primary_port, &p_dvp));
                        primary_nh_index = soc_ING_DVP_TABLEm_field32_get(unit,
                             &p_dvp, NEXT_HOP_INDEXf);
                    }
                    /*
                     * if we are adding a failover port to existing primary
                     * port then create protection table entry also.
                     */
                    if (bcmi_mpls_port_failover_update_is_required(
                        unit, mpls_port, primary_nh_index, nh_index)) {
                        rv = _bcm_esw_failover_prot_nhi_create(unit, 0,
                             (uint32) primary_nh_index, nh_index,
                             mpls_port->failover_mc_group,
                             mpls_port->failover_id);

                        if (BCM_FAILURE(rv)) {
                            goto vpls_cleanup;
                        }
                    }
                } else if (rv != BCM_E_NOT_FOUND) {
                    return rv;
            }
        }
    }
#endif /* BCM_TRIUMPH2_SUPPORT  */

         if (soc_mem_field_valid(unit, EGR_DVP_ATTRIBUTEm, MTU_VALUEf)) {
            rv = _bcm_tr_mpls_port_mtu_set(unit, vp, mpls_port->mtu); 
             if (rv < 0) {
                  goto vpls_cleanup;
             }
         }
        /*
         * Add the matching entry in s/w and h/w tables.
         * If this is a failover port, you may not want to write this
         * entry in h/w tables as that would overwrite primary entry.
         * entry_update is made false above to achieve that behavior.
         */
        rv = _bcm_esw_mpls_match_add(unit, mpls_port, vp, vpn, entry_update);
        if (rv == BCM_E_NONE) {
            /* Set the MPLS port ID */
            BCM_GPORT_MPLS_PORT_ID_SET(mpls_port->mpls_port_id, vp);
            mpls_port->encap_id = BCM_XGS3_DVP_EGRESS_IDX_MIN + nh_index;
        }

#if defined(BCM_TRIUMPH2_SUPPORT)
        if (!BCM_GPORT_IS_TRUNK(mpls_port->port)) {
            if (!(mpls_port->flags2 & BCM_MPLS_PORT2_QOS_NONE)) {
                _bcm_mpls_source_vp_set(unit, mpls_port, vp);
            }
        }
#endif
        /* Free the old next-hop entry. */

        (void)_bcm_tr_mpls_l2_old_nh_info_delete(unit, mpls_port, nh_index,
                                                 &old_nh_info);

  vpls_cleanup:
    if (rv < 0) {
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) || \
                                                    defined(BCM_APACHE_SUPPORT)
        if (SOC_IS_KATANAX(unit) || SOC_IS_TRIUMPH3(unit) || SOC_IS_APACHE(unit)) {
            _bcm_esw_reset_policer_from_table(unit, mpls_port->policer_id, 
                                                    SOURCE_VPm, vp, &svp); 
        }
#endif /* BCM_KATANA_SUPPORT or BCM_TRIUMPH3_SUPPORT or BCM_APACHE_SUPPORT */
        if (tpid_enable) {
            (void) _bcm_fb2_outer_tpid_entry_delete(unit, tpid_index);
        }
        if (!(mpls_port->flags & BCM_MPLS_PORT_REPLACE)) {
            (void) _bcm_vp_free(unit, _bcmVpTypeMpls, 1, vp);
            _BCM_MPLS_NETWORK_VP_USED_CLR(unit, vp);
            (void) _bcm_tr_mpls_l2_nh_info_delete(unit, nh_index);
        }
#ifdef BCM_TRIUMPH2_SUPPORT 
        if (soc_feature(unit, soc_feature_mpls_failover)) {
            mpls_info->failover_vp[vp] = old_failover_vp;
        }
#endif

        return rv;
    }

    return rv;
}

/*
 * Function:
 *      bcm_mpls_port_add
 * Purpose:
 *      Add an mpls port to a VPN
 * Parameters:
 *      unit    - (IN) Device Number
 *      vpn     - (IN) VPN instance ID
 *      mpls_port - (IN/OUT) mpls port information (OUT : mpls_port_id)
 * Returns:
 *      BCM_E_XXX
 */
int 
bcm_tr_mpls_port_add(int unit, bcm_vpn_t vpn, bcm_mpls_port_t *mpls_port)
{
    int mode, rv = BCM_E_PARAM, bit_num = 0; 

    BCM_IF_ERROR_RETURN(bcm_xgs3_l3_egress_mode_get(unit, &mode));
    if (!mode) {
        LOG_INFO(BSL_LS_BCM_L3,
                 (BSL_META_U(unit,
                             "L3 egress mode must be set first\n")));
        return BCM_E_DISABLED;
    }

    if (mpls_port->criteria == BCM_MPLS_PORT_MATCH_SHARE) {
        if (!soc_feature(unit, soc_feature_vp_sharing)) {
            return BCM_E_UNAVAIL;
        }
    }

    if (!_BCM_MPLS_VPN_IS_VPLS(vpn) && 
         !_BCM_MPLS_VPN_IS_VPWS(vpn)) {

        if ((vpn != BCM_MPLS_VPWS_VPN_INVALID) && (vpn != BCM_MPLS_VPLS_VPN_INVALID)) {
            return BCM_E_PARAM;
        }
    }

    if (mpls_port->if_class) {
        bit_num = soc_mem_field_length(unit, SOURCE_VPm, CLASS_IDf);
        if (mpls_port->if_class > (( 1 << bit_num ) - 1)) {
            return BCM_E_PARAM;
        }
    }

    /* BCM_MPLS_PORT_INT_PRI_MAP or BCM_MPLS_PORT_INT_PRI_SET indicates 
     * how to generate the internal Priority and Color values, and they don't coexist.
     */
    if ((mpls_port->flags & BCM_MPLS_PORT_INT_PRI_MAP) && 
        (mpls_port->flags & BCM_MPLS_PORT_INT_PRI_SET)) {
        return BCM_E_PARAM;
    }

    if ((mpls_port->criteria == BCM_MPLS_PORT_MATCH_LABEL_VLAN) ||
        (mpls_port->criteria == BCM_MPLS_PORT_MATCH_INVALID)) {
        return BCM_E_PARAM;
    }

    if ((mpls_port->flags & BCM_MPLS_PORT_CONTROL_WORD) &&
        (mpls_port->flags & BCM_MPLS_PORT_SEQUENCED)) {
        return BCM_E_PARAM;
    }

    if (_BCM_MPLS_VPN_IS_VPWS(vpn) || (vpn == BCM_MPLS_VPWS_VPN_INVALID) ) {
        rv = _bcm_tr_mpls_vpws_port_add(unit, vpn, mpls_port);
    } else if (_BCM_MPLS_VPN_IS_VPLS(vpn) || (vpn == BCM_MPLS_VPLS_VPN_INVALID)) {
        rv = _bcm_tr_mpls_vpls_port_add(unit, vpn, mpls_port);
    }

#ifdef BCM_WARM_BOOT_SUPPORT
    SOC_CONTROL_LOCK(unit);
    SOC_CONTROL(unit)->scache_dirty = 1;
    SOC_CONTROL_UNLOCK(unit);
#endif

    return rv;
}

/*
 * Function:
 *      _bcm_share_mpls_vp_delete
 * Purpose:
 *      Delete match criteria of shared VPs
 * Parameters:
 *      unit    - (IN) Device Number
 *      vpn     - (IN) VPN that shared vp belong to
 *      vp      - (IN) Source Virtual Port
 * Returns:
 *      BCM_E_XXX
 */
int
_bcm_share_mpls_vp_delete(int unit, bcm_vpn_t vpn, int vp)
{
    vlan_xlate_entry_t *vent;
    soc_mem_t mem;
    int index_min = 0;
    int index_max = 0;
    uint8 *xlate_buf = NULL;
    int buf_size = 0;
    int i = 0;
    int rv = BCM_E_NONE;
    int vfi = -1;
    int source_vp = -1;

    _BCM_MPLS_VPN_GET(vfi, _BCM_VPN_TYPE_VFI, vpn);

    mem = VLAN_XLATEm;
    index_min = soc_mem_index_min(unit, mem);
    index_max = soc_mem_index_max(unit, mem);

    buf_size = SOC_MEM_TABLE_BYTES(unit, mem);
    xlate_buf = soc_cm_salloc(unit, buf_size, "VLAN_XLATE buffer");
    if (NULL == xlate_buf) {
        return BCM_E_MEMORY;
    }
    rv = soc_mem_read_range(unit, mem, MEM_BLOCK_ANY, index_min, index_max, xlate_buf);
    if (rv < 0) {
        soc_cm_sfree(unit, xlate_buf);
        return rv;
    }

    for (i = index_min; i <= index_max; i++) {
        vent = soc_mem_table_idx_to_pointer(unit,
            mem, vlan_xlate_entry_t *, xlate_buf, i);
        if (soc_mem_field32_get(unit, mem, vent, VALIDf) == 0) {
            continue;
        }

        if (soc_mem_field32_get(unit, mem, vent, XLATE__MPLS_ACTIONf) != 0x1) {
            continue;
        }

        if (vfi != -1 && soc_mem_field32_get(unit, mem, vent, XLATE__VFIf) != vfi) {
            continue;
        }

        source_vp = soc_mem_field32_get(unit, mem, vent, XLATE__SOURCE_VPf);
        if (vp != source_vp) {
            continue;
        }
        rv = soc_mem_delete_index(unit, VLAN_XLATEm, MEM_BLOCK_ALL, i);
        if (rv < BCM_E_NONE) {
            (void)soc_cm_sfree(unit, xlate_buf);
            return rv;
        }
        bcm_tr_mpls_port_match_count_adjust(unit, source_vp, -1);
    }

    (void)soc_cm_sfree(unit, xlate_buf);

    return BCM_E_NONE;
}


STATIC int
_bcm_tr_mpls_port_delete(int unit, bcm_vpn_t vpn, int vp)
{
    int rv = BCM_E_NONE;
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int failover_nh_index=-1;
    int failover_id =-1;
    int failover_mc_group=-1;
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
    bcm_policer_t policer = 0;
#endif
    int vpless_failover_port = FALSE; /* the failover port without a dedicated vp */
    int failover_port = FALSE;
    int prot_index;
    int dvp = 0;
    _bcm_vp_info_t vp_info;
#if defined(BCM_TRIUMPH2_SUPPORT)
    int criteria, is_trunk;
    int failover_dvp;
    egr_l3_next_hop_entry_t egr_nh;
    int nh_index = 0;
    ing_l3_next_hop_entry_t ing_nh;
#endif

    if (_BCM_MPLS_GPORT_FAILOVER_VPLESS_GET(vp)) {
        vpless_failover_port = TRUE;
        vp = _BCM_MPLS_GPORT_FAILOVER_VPLESS_CLEAR(vp);
    }

    BCM_IF_ERROR_RETURN(_bcm_vp_info_get(unit, vp, &vp_info));
    if ((vp_info.flags & _BCM_VP_INFO_SHARED_PORT)) {
        if (mpls_info->match_key[vp].match_count >= 1) {
            BCM_IF_ERROR_RETURN(_bcm_share_mpls_vp_delete(unit, vpn, vp));
        }
        if (mpls_info->match_key[vp].match_count > 0) {
            return BCM_E_NONE;
        } else if (mpls_info->match_key[vp].match_count == 0) {
            bcm_tr_mpls_match_clear (unit, vp);
        }
    } else if (mpls_info->match_key[vp].match_count > 1) {
         return BCM_E_BUSY;
    }

#if defined(BCM_TRIUMPH2_SUPPORT)
    criteria = mpls_info->match_key[vp].flags;
    if ((vp_info.flags & _BCM_VP_INFO_SHARED_PORT)) {
        rv = READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp, &dvp);
        if (rv < 0) {
            return rv;
        }
        nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp,
                                                  NEXT_HOP_INDEXf);
        BCM_IF_ERROR_RETURN (soc_mem_read(unit, ING_L3_NEXT_HOPm,
                                MEM_BLOCK_ANY, nh_index, &ing_nh));
        is_trunk = soc_mem_field32_get(unit, ING_L3_NEXT_HOPm, &ing_nh, Tf);
    } else {
        is_trunk = (mpls_info->match_key[vp].trunk_id != -1) ? TRUE : FALSE;
    }
#endif
    if ((vpn == BCM_MPLS_VPWS_VPN_INVALID) || (vpn == BCM_MPLS_VPLS_VPN_INVALID)) {
        source_vp_entry_t svp;
        if (!soc_feature(unit, soc_feature_mpls_software_failover)) {
            return BCM_E_UNAVAIL;
        }        
        BCM_IF_ERROR_RETURN (
            READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp));
        if (soc_SOURCE_VPm_field32_get(unit, &svp, ENTRY_TYPEf) == 0) {
            ing_dvp_table_entry_t dvp;
            int nh_index = 0;
            BCM_IF_ERROR_RETURN (
                READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp, &dvp));
            nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp, 
                                                      NEXT_HOP_INDEXf);
            BCM_IF_ERROR_RETURN(_bcm_esw_mpls_match_delete(unit, vp, TRUE));
            sal_memset(&svp, 0, sizeof(source_vp_entry_t));
            rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp, &svp);
            if (BCM_FAILURE(rv)) {
                return rv;
            }
            sal_memset(&dvp, 0, sizeof(ing_dvp_table_entry_t));
            rv = WRITE_ING_DVP_TABLEm(unit, MEM_BLOCK_ALL, vp, &dvp);
            if (rv < 0) {
                return rv;
            }
            rv = _bcm_vp_ing_dvp_to_ing_dvp2(unit, &dvp, vp);
            if (rv < 0) {
                return rv;
            }
            rv = _bcm_tr_mpls_l2_nh_info_delete(unit, nh_index);
            if (BCM_FAILURE(rv)) {
                return rv;;
            }
            (void) _bcm_vp_free(unit, _bcmVpTypeMpls, 1, vp);
            _BCM_MPLS_NETWORK_VP_USED_CLR(unit, vp);
        } else {
            return BCM_E_PARAM;
        }
    } else if (_BCM_MPLS_VPN_IS_VPWS(vpn)) {
        int nh_index = 0, vp3=0;
        source_vp_entry_t svp1, svp2, svp3;
        ing_dvp_table_entry_t dvp1, dvp2, dvp3;
        uint8 valid = 0;
        bcm_vpn_t   vpn_id;
        int vp1=-1, vp2=-1, vp3_read = -1;

        sal_memset(&svp1, 0, sizeof(source_vp_entry_t));
        sal_memset(&svp2, 0, sizeof(source_vp_entry_t));
        sal_memset(&svp3, 0, sizeof(source_vp_entry_t));

        sal_memset(&dvp1, 0, sizeof(ing_dvp_table_entry_t));
        sal_memset(&dvp2, 0, sizeof(ing_dvp_table_entry_t));
        sal_memset(&dvp3, 0, sizeof(ing_dvp_table_entry_t));


       _BCM_MPLS_VPN_GET(vpn_id, _BCM_MPLS_VPN_TYPE_VPWS, vpn);

        (void) _bcm_tr_mpls_vpws_vp_map_get (unit, vpn_id,  &vp1,  &vp2, &vp3_read);

        if (!_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
            return BCM_E_NOT_FOUND;
        }

        if (vp1 != -1) {
              BCM_IF_ERROR_RETURN (
                  READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp1, &svp1));
              if (soc_SOURCE_VPm_field32_get(unit, &svp1, ENTRY_TYPEf) != 0) {
                   valid |= 1;
                   BCM_IF_ERROR_RETURN (
                        READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp1, &dvp1));
              }
         }

         if ((vp2 != -1) || (bcmi_mpls_match_backup_ports(unit, vpn_id, vp))) {
              if ((vp != vp2) && (bcmi_mpls_match_backup_ports(unit, vpn_id, vp))) {
                  vp2 = vp;
              }
              if (vp2 != -1) {
                  BCM_IF_ERROR_RETURN (
                          READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp2, &svp2));
                  if (soc_SOURCE_VPm_field32_get(unit, &svp2, ENTRY_TYPEf) != 0) {
                      valid |= 2;
                      BCM_IF_ERROR_RETURN (
                              READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp2, &dvp2));
                  }
              }
         }

         if (vp == vp1) {
            if (!(valid & 1)) {
                 return BCM_E_NOT_FOUND;
            }
            dvp = soc_SOURCE_VPm_field32_get(unit, &svp1, DVPf);
            nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp1, 
                                                      NEXT_HOP_INDEXf);
         } else if (vp == vp2)  {
            if (!(valid & 2)) {
                 return BCM_E_NOT_FOUND;
            }
            dvp = soc_SOURCE_VPm_field32_get(unit, &svp2, DVPf);
            nh_index = soc_ING_DVP_TABLEm_field32_get(unit, 
                                       &dvp2, NEXT_HOP_INDEXf);
         } else {
            /* Read in current table values for VP3 */
            vp3 = vp;
            BCM_IF_ERROR_RETURN (
                 READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp3, &dvp3));
            BCM_IF_ERROR_RETURN (
                 READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp3, &svp3));
            nh_index = soc_ING_DVP_TABLEm_field32_get(unit, 
                                       &dvp3, NEXT_HOP_INDEXf);
         }
         if (soc_mem_field_valid(unit, EGR_DVP_ATTRIBUTEm, MTU_VALUEf)) {
            rv = _bcm_tr_mpls_port_mtu_clear(unit, vp);
             if (rv < 0) {
                  return rv;
             }
         }

        if (vpless_failover_port) {
            failover_port = TRUE;
        } else {
            source_vp_entry_t svp_uni;
            ing_dvp_table_entry_t dvp_nni;
            int dvp_num_nni; /* network side dvp number*/

            /* check if vp is a failover port. Both failover port and
             * primary port should point to the same DVP customer(uni) side.
             */
            if (vp3_read != -1) {
                int del_fo_nh = 0;
                sal_memset(&svp_uni, 0, sizeof(source_vp_entry_t));
                sal_memset(&dvp_nni, 0, sizeof(ing_dvp_table_entry_t));
                if (dvp == 0) {
                    del_fo_nh = 1;
                } else {
                BCM_IF_ERROR_RETURN (READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, 
                                     dvp, &svp_uni));
                dvp_num_nni = soc_SOURCE_VPm_field32_get(unit, &svp_uni, DVPf);
                if (dvp_num_nni && (dvp_num_nni != vp)) {
                        /* we need to check if this is the prime multi level backup port
                         get the nh index */
                        del_fo_nh = 1; 
                    }                                      
                }
                if (del_fo_nh) {
                    /* possbile failover port case */
                    BCM_IF_ERROR_RETURN (READ_ING_DVP_TABLEm(unit, 
                             MEM_BLOCK_ANY, vp3_read, &dvp_nni));
                    prot_index = soc_ING_DVP_TABLEm_field32_get(unit, 
                                       &dvp_nni, NEXT_HOP_INDEXf);
                    rv = _bcm_esw_failover_prot_nhi_get (unit, prot_index,
                          &failover_id, &failover_nh_index, &failover_mc_group);
                    /* For 1+1 protection NHI use failover multicast group index */
                    /* Do this only if this is not a multi level level protection.
                     * For multi level protection, we wil have mc group pointing to
                     * backup links
                     */
                    if (BCM_SUCCESS(rv) && (failover_mc_group > 0) && 
                        (!(_BCM_FAILOVER_IS_MULTI_LEVEL(failover_id)))) {

                        rv = _bcm_tr_mpls_vpless_failover_nh_index_find(unit, vp,
                            prot_index, &failover_nh_index);
                    }
                    if (BCM_SUCCESS(rv)) {
                        if (failover_nh_index == nh_index) {
                            /* confirm the failover port */
                            failover_port = TRUE; 
                            BCM_IF_ERROR_RETURN(_bcm_esw_failover_prot_nhi_cleanup(unit,
                                   prot_index));
                        }
                    }
                }
            }
            if (bcmi_mpls_match_backup_ports(unit, vpn_id, vp)) {
                failover_port = TRUE;
            } 
        }

        /* If the other port is valid, point it to itself */
        if (vp == vp1) {
            rv = _bcm_esw_mpls_match_delete(unit, 
                     vpless_failover_port == FALSE?
                             vp:
                            _BCM_MPLS_GPORT_FAILOVER_VPLESS_SET(vp), TRUE);
            if ( rv < 0 ) {
                if (rv != BCM_E_NOT_FOUND) {
                     return rv;
                } else {
                    rv = BCM_E_NONE;
                }
            }

#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
            if (soc_feature(unit, soc_feature_global_meter)) {
                rv = _bcm_esw_get_policer_from_table(unit, SOURCE_VPm, vp1, &svp1, 
                                                          &policer, 1); 
                if(BCM_SUCCESS(rv)) {
                     _bcm_esw_policer_decrement_ref_count(unit, policer);
                } else {
                    return rv;
                }
            }
#endif /* BCM_KATANA_SUPPORT or BCM_TRIUMPH3_SUPPORT */
                if ((rv >= 0) && (failover_port == FALSE)) {
               if ((vp2 != -1) && 
                   (vp1 == soc_SOURCE_VPm_field32_get(unit, &svp2, DVPf))) {
                    rv = soc_mem_field32_modify(unit, SOURCE_VPm, 
                                                vp2, DVPf, 0);
                   /* Please update other backup ports here */
                   bcmi_mpls_update_dvp_in_backup_ports(unit, vpn_id, -1);
               }
               if ((rv >= 0) && (vp3_read != -1)) {
                   BCM_IF_ERROR_RETURN (
                   READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp3_read, &svp3));
                   if (vp1 == soc_SOURCE_VPm_field32_get(unit, &svp3, DVPf)) {
                       rv = soc_mem_field32_modify(unit, SOURCE_VPm,
                                                   vp3_read, DVPf, 0);                      
                   }
                }
                }
        } else if (vp == vp2) {
            if (bcmi_mpls_clear_mpls_entry_for_vp(unit, vpn_id, vp)) {
                (void) bcm_tr_mpls_match_clear (unit, vp);
            } else {
                rv = _bcm_esw_mpls_match_delete(unit,
                        vpless_failover_port == FALSE?
                        vp:
                        _BCM_MPLS_GPORT_FAILOVER_VPLESS_SET(vp), TRUE);

                if ( rv < 0 ) {
                    if (rv != BCM_E_NOT_FOUND) {
                        return rv;
                    } else {
                        rv = BCM_E_NONE;
                    }
                }
            }
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
            if (soc_feature(unit, soc_feature_global_meter)) {
                rv = _bcm_esw_get_policer_from_table(unit, SOURCE_VPm, vp2, &svp2, 
                                                          &policer, 1); 
                if(BCM_SUCCESS(rv)) {
                     _bcm_esw_policer_decrement_ref_count(unit, policer);
                } else {
                    return rv;
                }
            }
#endif /* BCM_KATANA_SUPPORT or BCM_TRIUMPH3_SUPPORT */

                if (rv >= 0 && (failover_port == FALSE)) {
                    if (vp1 != -1) {
                    rv = soc_mem_field32_modify(unit, SOURCE_VPm, 
                                                vp1, DVPf, 0);
                }                    
                }                                          
        } else if (vp == vp3) {
            /* vp1: customer side mpls port
             * vp2: failover port
             * vp3: service side primary mpls port
             */ 
            int match_label_delete = TRUE;
            dvp = soc_SOURCE_VPm_field32_get(unit, &svp3, DVPf);
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
            if (soc_feature(unit, soc_feature_global_meter)) {
                rv = _bcm_esw_get_policer_from_table(unit, SOURCE_VPm, vp3, &svp3, 
                                                          &policer, 1); 
                if(BCM_SUCCESS(rv)) {
                     _bcm_esw_policer_decrement_ref_count(unit, policer);
                } else {
                    return rv;
                }
            }
#endif /* BCM_KATANA_SUPPORT or BCM_TRIUMPH3_SUPPORT */
            if (rv >= 0) {
                rv = soc_mem_field32_modify(unit, SOURCE_VPm, 
                                                vp3, DVPf, 0);
            }
            failover_nh_index = -1;
            failover_mc_group = -1;
            if (BCM_SUCCESS(rv)) {
                rv = _bcm_esw_failover_prot_nhi_get (unit, nh_index,
                        &failover_id, &failover_nh_index, &failover_mc_group);
            }
            if (BCM_SUCCESS(rv) &&
                ((failover_nh_index > 0) || (failover_mc_group > 0))) {

                if (failover_nh_index > 0) {
                    int fo_vp;
                    /* a failover port exists, vc_term label is shared with failover
                     * port. let the failover port delete it.
                     */
                    fo_vp = dvp==vp1?vp2:vp1;
                    if ((dvp > 0) && (fo_vp != -1)) {
                    rv = soc_mem_field32_modify(unit, SOURCE_VPm, 
                                                dvp, DVPf, fo_vp);
                    }
                    if ((fo_vp != -1) && 
                        (mpls_info->match_key[vp3].flags ==
                                _BCM_MPLS_PORT_MATCH_TYPE_LABEL)) {
                        mpls_info->match_key[fo_vp].match_label = 
                                   mpls_info->match_key[vp3].match_label;
                        mpls_info->match_key[fo_vp].flags |= 
                                         _BCM_MPLS_PORT_MATCH_TYPE_LABEL;
                        mpls_info->match_key[fo_vp].modid = 0;
                        mpls_info->match_key[fo_vp].port  = 0;
                        match_label_delete = FALSE;
                    }
                }
                if (BCM_SUCCESS(rv)) {
                   rv = _bcm_esw_failover_prot_nhi_cleanup(unit,nh_index);
                }
            }
            if (rv < 0 ) {
                if ((rv == BCM_E_NOT_FOUND) || (rv == BCM_E_UNAVAIL)) {
                    rv = 0;
                } else {
                   return rv;
                }
            }
            if (match_label_delete) {
                rv = _bcm_esw_mpls_match_delete(unit, vp, TRUE);
                if ( rv < 0 ) {
                    if (rv != BCM_E_NOT_FOUND) {
                         return rv;
                    } else {
                        rv = BCM_E_NONE;
                    }
                }
            } else {
                (void) bcm_tr_mpls_match_clear (unit, vp);
            }
        }

        if (rv >= 0) {
            /* Invalidate the VP being deleted */
            if ((vp == vp1) && (vpless_failover_port == FALSE)) {
#if defined(BCM_TRIUMPH2_SUPPORT)
                if (!is_trunk) {
                    rv = _bcm_mpls_source_vp_clear(unit, criteria, vp, vpn);
                    if (BCM_FAILURE(rv)) {
                        return rv;
                    }
                }
#endif
                sal_memset(&svp1, 0, sizeof(source_vp_entry_t));
                rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp1, &svp1);
                if (rv >= 0) {
                    rv = soc_mem_field32_modify(unit, ING_DVP_TABLEm, 
                                                vp1, NEXT_HOP_INDEXf, 0);
                }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
                if (rv >= 0 &&
                        soc_feature(unit, soc_feature_ifp_redirect_to_dvp)) {
                    rv = soc_mem_field32_modify(unit, ING_DVP_2_TABLEm, 
                                                vp1, NEXT_HOP_INDEXf, 0);
                }
#endif
            } else if ((vp == vp2) && (vpless_failover_port == FALSE)) {
#if defined(BCM_TRIUMPH2_SUPPORT)
                if (!is_trunk) {
                    rv = _bcm_mpls_source_vp_clear(unit, criteria, vp, vpn);
                    if (BCM_FAILURE(rv)) {
                        return rv;
                    }
                }
#endif
                sal_memset(&svp2, 0, sizeof(source_vp_entry_t));
                rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp2, &svp2);
                if (rv >= 0) {
                    rv = soc_mem_field32_modify(unit, ING_DVP_TABLEm, 
                                                vp2, NEXT_HOP_INDEXf, 0);
                }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
                if (rv >= 0 &&
                        soc_feature(unit, soc_feature_ifp_redirect_to_dvp)) {
                    rv = soc_mem_field32_modify(unit, ING_DVP_2_TABLEm, 
                                                vp2, NEXT_HOP_INDEXf, 0);
                }
#endif
            } else if (vp == vp3) {
                sal_memset(&svp3, 0, sizeof(source_vp_entry_t));
                rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp3, &svp3);
                if (rv >= 0) {
                    rv = soc_mem_field32_modify(unit, ING_DVP_TABLEm, 
                                                vp3, NEXT_HOP_INDEXf, 0);
                }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
                if (rv >= 0 &&
                        soc_feature(unit, soc_feature_ifp_redirect_to_dvp)) {
                    rv = soc_mem_field32_modify(unit, ING_DVP_2_TABLEm, 
                                                vp3, NEXT_HOP_INDEXf, 0);
                }
#endif
                if (vp1 != -1) {
                    if (vp2 != -1) {
                        rv = soc_mem_field32_modify(unit, SOURCE_VPm, vp1, DVPf, vp2);
                    } else {
                        rv = soc_mem_field32_modify(unit, SOURCE_VPm, vp1, DVPf, 0);
                    }
                }
            }

            if (vp != vp3) {  
                /* Free vp-less failover port resource when deleting
                 * either vp-less failover port or its primary port.
                 */
                failover_nh_index = -1;
                failover_mc_group = -1;
                rv = _bcm_esw_failover_prot_nhi_get (unit, nh_index,
                        &failover_id, &failover_nh_index, &failover_mc_group);
                if (BCM_SUCCESS(rv)) {
                    if (failover_mc_group > 0) {
                        rv = _bcm_tr_mpls_vpless_failover_nh_index_find(unit,vp,nh_index,
                                                   &failover_nh_index);
                    }
                    if (failover_nh_index > 0) {
                        rv = _bcm_tr_mpls_l2_nh_info_delete(unit, 
                                                       failover_nh_index);
                        if (BCM_SUCCESS(rv)) {
                            rv = _bcm_esw_failover_prot_nhi_cleanup(unit,nh_index);
                        }
                    }
                } else if (rv == BCM_E_UNAVAIL) {
                    if (soc_feature(unit, soc_feature_mpls_software_failover) &&
                        (vpless_failover_port == TRUE) ) {
                        rv = _bcm_tr_mpls_vpless_failover_nh_index_find(unit,vp,nh_index,
                                                   &failover_nh_index);
                        if (BCM_SUCCESS(rv)) {
                            rv = _bcm_tr_mpls_l2_nh_info_delete(unit, failover_nh_index);
                        }
                    }
                }

                if (rv < 0 ) {
                    if ((rv == BCM_E_NOT_FOUND) || (rv == BCM_E_UNAVAIL)) {
                        rv = 0;
                    } else {
                        return rv;
                    }
                }
            }

            /* Delete the next-hop info */
            if ((rv >= 0) && (vpless_failover_port == FALSE)) {
                rv = _bcm_tr_mpls_l2_nh_info_delete(unit, nh_index);
            }
            if ( rv < 0 ) {
                if (rv != BCM_E_NOT_FOUND) {
                    return rv;
                } else {
                        rv = 0;
                }
            }
            /* Free the VP */
            if (vpless_failover_port == FALSE) {
                (void) _bcm_vp_free(unit, _bcmVpTypeMpls, 1, vp);
                _BCM_MPLS_NETWORK_VP_USED_CLR(unit, vp);
                if (vp == vp1) {
                    _bcm_tr_mpls_vpws_vp_map_set(unit, vpn_id, -1, vp2, vp3_read);
                } else if (vp == vp2) {
                    _bcm_tr_mpls_vpws_vp_map_backup_port_clear(unit, vpn_id, vp2);
                } else if (vp == vp3) {
                    _bcm_tr_mpls_vpws_vp_map_set(unit, vpn_id, vp1, vp2, -1);
                }
            }
        }
    } else if (_BCM_MPLS_VPN_IS_VPLS(vpn)) {
        int nh_index = 0, vfi;
        source_vp_entry_t svp;
        ing_dvp_table_entry_t dvp;
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) \
    || defined(BCM_TRIDENT_SUPPORT)
        ing_dvp_2_table_entry_t dvp2;
#endif /* BCM_TRIDENT2_SUPPORT or BCM_TRIUMPH3_SUPPORT or BCM_TRIDENT_SUPPORT */

        sal_memset(&svp, 0, sizeof(source_vp_entry_t));
        sal_memset(&dvp, 0, sizeof(ing_dvp_table_entry_t));
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) \
    || defined(BCM_TRIDENT_SUPPORT)
        if (SOC_IS_TRIUMPH3(unit) || SOC_IS_TRIDENT2X(unit)
            || SOC_IS_TRIDENT(unit)) {
            sal_memset(&dvp2, 0, sizeof(ing_dvp_2_table_entry_t));
        }
#endif /* BCM_TRIDENT2_SUPPORT or BCM_TRIUMPH3_SUPPORT or BCM_TRIDENT_SUPPORT */

        _BCM_MPLS_VPN_GET(vfi, _BCM_MPLS_VPN_TYPE_VPLS, vpn);
        if (!_bcm_vfi_used_get(unit, vfi, _bcmVfiTypeMpls)) {
            return BCM_E_NOT_FOUND;
        }
        if (!_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
            return BCM_E_NOT_FOUND;
        }

        BCM_IF_ERROR_RETURN (
            READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp));
        if (soc_SOURCE_VPm_field32_get(unit, &svp, ENTRY_TYPEf) == 0) {
            return BCM_E_NOT_FOUND;
        }

        rv = READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp, &dvp);
        if (rv < 0) {
            return rv;
        }
        nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp, NEXT_HOP_INDEXf);
        if (soc_mem_field_valid(unit, EGR_DVP_ATTRIBUTEm, MTU_VALUEf)) {
            rv = _bcm_tr_mpls_port_mtu_clear(unit, vp);
             if (rv < 0) {
                  return rv;
             }
        }

        /*
         * In case we are removing the failover port,
         * we should clean protection tables also.
         * check for primary matching entry and then
         * make sure that this is failover port for that entry.
         */
        if (soc_feature(unit, soc_feature_mpls_failover)) {
        mpls_entry_entry_t ret_ment;
        int primary_port=0, primary_nh_index=0;
        int cleanup = 1;

        rv = _bcm_esw_mpls_failover_nw_port_match_get(unit, NULL, vp, &ret_ment);

            if (rv == BCM_E_NONE) {
                ing_dvp_table_entry_t p_dvp;

                primary_port =
                   soc_MPLS_ENTRYm_field32_get(unit, &ret_ment, MPLS__SOURCE_VPf);

                if (primary_port > 0) {
                    BCM_IF_ERROR_RETURN (READ_ING_DVP_TABLEm(unit,
                        MEM_BLOCK_ANY, primary_port, &p_dvp));
                    primary_nh_index = soc_ING_DVP_TABLEm_field32_get(unit,
                        &p_dvp, NEXT_HOP_INDEXf);


                    rv = _bcm_esw_failover_prot_nhi_get (unit,
                        primary_nh_index, &failover_id,
                        &failover_nh_index, &failover_mc_group);

                     if (failover_nh_index > 0) {
                         /* check if we can cleanup the failover entry if failover port
                          * is getting deleted
                          */
                         rv = bcmi_mpls_failover_nhi_cleanup_check(
                              unit, vpn, vp, failover_id,
                              failover_nh_index, primary_nh_index, nh_index, &cleanup);
                         if (rv) {
                             return(rv);
                         }

                         if (cleanup) {
                             /* Clean failover protection entry.*/
                             rv = _bcm_esw_failover_prot_nhi_cleanup(unit, primary_nh_index);
                             if (rv) {
                                 return(rv);
                             }
                         }
                         if (primary_port != vp) {
                             /* clean s/w state so that match delete does not
                              * find this port to delete matching entry.
                              * else, match_delete routing can remove
                              * primary port entry as that is with same label */
                             (void) bcm_tr_mpls_match_clear (unit, vp);
                          }
                     }
                }
            } else if (rv != BCM_E_NOT_FOUND) {
                return rv;
            }
        }

        BCM_IF_ERROR_RETURN(_bcm_vp_info_get(unit, vp, &vp_info));
        if (!(vp_info.flags & _BCM_VP_INFO_SHARED_PORT)) {
            rv = _bcm_esw_mpls_match_delete(unit,
                   vpless_failover_port == FALSE? vp:
                   _BCM_MPLS_GPORT_FAILOVER_VPLESS_SET(vp), TRUE);
            if ( rv < 0 ) {
                if (rv != BCM_E_NOT_FOUND) {
                    return rv;
                }
            }
        } else {
            if (mpls_info->match_key[vp].match_count == 0) {
                bcm_tr_mpls_match_clear (unit, vp);
            }
        }

#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit, soc_feature_global_meter)) {
            rv = _bcm_esw_get_policer_from_table(unit, SOURCE_VPm, vp, &svp, 
                                                      &policer, 0); 
            if(BCM_SUCCESS(rv)) {
                 _bcm_esw_policer_decrement_ref_count(unit, policer);
            } else {
                return rv;
            }
        }
#endif /* BCM_KATANA_SUPPORT  or BCM_TRIUMPH3_SUPPORT*/

        if (vpless_failover_port == FALSE) {
#if defined(BCM_TRIUMPH2_SUPPORT)
            if (!is_trunk) {
               rv = _bcm_mpls_source_vp_clear(unit, criteria, vp, vpn);
               if (BCM_FAILURE(rv)) {
                   return rv;
               }
            }
#endif
            /* Clear the SVP and DVP table entries */
            sal_memset(&svp, 0, sizeof(source_vp_entry_t));
            rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp, &svp);
            if (rv < 0) {
                return rv;
            }

            sal_memset(&dvp, 0, sizeof(ing_dvp_table_entry_t));
            rv = WRITE_ING_DVP_TABLEm(unit, MEM_BLOCK_ALL, vp, &dvp);
            if (rv < 0) {
                return rv;
            }
            rv = _bcm_vp_ing_dvp_to_ing_dvp2(unit, &dvp, vp);
            if (rv < 0) {
                return rv;
            }
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT) \
    || defined(BCM_TRIDENT_SUPPORT)
            if (SOC_IS_TRIUMPH3(unit) || SOC_IS_TRIDENT2X(unit)
                || SOC_IS_TRIDENT(unit)) {
                sal_memset(&dvp2, 0, sizeof(ing_dvp_2_table_entry_t));
                rv = WRITE_ING_DVP_2_TABLEm(unit, MEM_BLOCK_ALL, vp, &dvp2);
                if (rv < 0) {
                    return rv;
                }
            }
#endif /* BCM_TRIDENT2_SUPPORT or BCM_TRIUMPH3_SUPPORT or BCM_TRIDENT_SUPPORT */

        }

#if defined(BCM_TRIUMPH2_SUPPORT)
        if (soc_feature(unit, soc_feature_mpls_failover)) {
             rv = _bcm_esw_failover_prot_nhi_get (unit, nh_index,
                           &failover_id, &failover_nh_index, &failover_mc_group);
             /*For 1+1 vpless failover get failover nhi using multicast group index*/
             if(failover_mc_group > 0){
                 rv = _bcm_tr_mpls_vpless_failover_nh_index_find(unit,vp,nh_index,
                         &failover_nh_index);
             }
             if ((failover_nh_index > 0) && (!(_BCM_FAILOVER_IS_MULTI_LEVEL(failover_id)))) {
                 BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm,
                             MEM_BLOCK_ANY, failover_nh_index, &egr_nh));
                 if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                     failover_dvp = soc_EGR_L3_NEXT_HOPm_field32_get(unit,
                             &egr_nh, MPLS__DVPf);
                 } else {
                     failover_dvp = soc_EGR_L3_NEXT_HOPm_field32_get(unit,
                             &egr_nh, DVPf);
                 }
                 /*
                  * If vpless failover, delete failover nh index also.
                  * If non-vpless failover, do not delete failover nh index.
                  * .If this is the VPLESS primary port being deleted, then the
                  *  VPLESS backup port's nh index is also removed.
                  * .If this is the VPLESS backup port being deleted,
                  *  failover_nh_index is its own nh index, so backup port's
                  *  nh index will be removed.
                  */
                 if (failover_dvp == vp) {
                     rv = _bcm_tr_mpls_l2_nh_info_delete(unit, failover_nh_index);
                 } else {
                     int rv1;
                     bcm_mpls_port_t failover_mpls_port;
                     bcm_mpls_port_t_init(&failover_mpls_port);
                     rv1 = _bcm_esw_mpls_match_get(unit, &failover_mpls_port, failover_dvp);
                     if (rv1 == BCM_E_NOT_FOUND) {
                         BCM_IF_ERROR_RETURN(_bcm_esw_mpls_match_add(unit, &failover_mpls_port, 
                                                                    failover_dvp, vpn, TRUE));
                     }
                 }
             }
             if (BCM_SUCCESS(rv)) {
                 rv = _bcm_esw_failover_prot_nhi_cleanup(unit,nh_index);
             }

             if ( rv < 0 ) {
               if (rv != BCM_E_NOT_FOUND) {
                  return rv;
               } else {
                   rv = 0;
               }
             }
        } else 
#endif /* BCM_TRIUMPH2_SUPPORT */
        if (soc_feature(unit, soc_feature_mpls_software_failover) &&
            (vpless_failover_port == TRUE) ) {
            rv = _bcm_tr_mpls_vpless_failover_nh_index_find(unit,vp,nh_index,
                                                   &failover_nh_index);
            if (BCM_SUCCESS(rv)) {
                rv = _bcm_tr_mpls_l2_nh_info_delete(unit, failover_nh_index);
            }
            if (rv < 0 ) {
                if (rv != BCM_E_NOT_FOUND) {
                    return rv;
                } else {
                    rv = 0;
                }
            }
        }

        if (vpless_failover_port == FALSE) {
            /* Delete the next-hop info */
            if (rv >= 0) {
                rv = _bcm_tr_mpls_l2_nh_info_delete(unit, nh_index);
            }
            if ( rv < 0 ) {
                if (rv != BCM_E_NOT_FOUND) {
                    return rv;
                } else {
                        rv = 0;
                }
            }

            /* Free the VP */
            (void) _bcm_vp_free(unit, _bcmVpTypeMpls, 1, vp);
            _BCM_MPLS_NETWORK_VP_USED_CLR(unit, vp);
#ifdef BCM_TRIUMPH2_SUPPORT 
            if (soc_feature(unit, soc_feature_mpls_failover)) {           
                int i;
                int num_vp = soc_mem_index_count(unit, SOURCE_VPm);
                for (i = 0; i < num_vp; i++) {
                    if (mpls_info->failover_vp[i] == vp) {
                        mpls_info->failover_vp[i] = 0;
                    }
                }    
                mpls_info->failover_vp[vp] = 0;
                
            }
#endif  
        }
    }

#ifdef BCM_TRIUMPH2_SUPPORT
    if (soc_feature(unit, soc_feature_gport_service_counters)) {
        bcm_gport_t gport;

        /* Release Service counter, if any */
        BCM_GPORT_MPLS_PORT_ID_SET(gport, vp);
        _bcm_esw_flex_stat_handle_free(unit, _bcmFlexStatTypeGport, gport);
    }
#endif

#ifdef BCM_WARM_BOOT_SUPPORT
    SOC_CONTROL_LOCK(unit);
    SOC_CONTROL(unit)->scache_dirty = 1;
    SOC_CONTROL_UNLOCK(unit);
#endif

    return rv;
}

/*
 * Function:
 *      bcm_mpls_port_delete
 * Purpose:
 *      Delete an mpls port from a VPN
 * Parameters:
 *      unit       - (IN) Device Number
 *      vpn        - (IN) VPN instance ID
 *      mpls_port_id - (IN) mpls port information
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_port_delete(int unit, bcm_vpn_t vpn, bcm_gport_t mpls_port_id)
{
    int vp;

    if (((vpn == BCM_MPLS_VPWS_VPN_INVALID) ||
         (vpn == BCM_MPLS_VPLS_VPN_INVALID)) && 
         !soc_feature(unit, soc_feature_mpls_software_failover)) {
        return BCM_E_PARAM; 
    }
    if (!_BCM_MPLS_VPN_IS_VPLS(vpn) && 
         !_BCM_MPLS_VPN_IS_VPWS(vpn)) {
        return BCM_E_PARAM;
    }

    vp = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port_id);
    if (vp == -1) {
        return BCM_E_PARAM;
    }

    if (!_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
        return BCM_E_NOT_FOUND;
    }
    if (_BCM_MPLS_GPORT_FAILOVER_VPLESS_GET(mpls_port_id)) {
        vp = _BCM_MPLS_GPORT_FAILOVER_VPLESS_SET(vp);
    }
    return (_bcm_tr_mpls_port_delete(unit, vpn, vp));
}
/*
 * Function:
 *      bcmi_mpls_delete_backup_ports
 * Purpose:
 *      Delete an mpls backup port
 * Parameters:
 *      unit       - (IN) Device Number
 *      vpn        - (IN) VPN instance ID
 *      vp         - (IN) mpls backup port 
 * Returns:
 *      BCM_E_XXX
 */
int
bcmi_mpls_delete_backup_ports(int unit, int vpn, int vp)
{

    int rv;
    source_vp_entry_t svp;
    ing_dvp_table_entry_t dvp;

    rv = READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp);
    if (rv < 0) {
        return rv;
    }

    rv = READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp, &dvp);
    if (rv < 0) {
        return rv;
    }
    if (soc_SOURCE_VPm_field32_get(unit, &svp, ENTRY_TYPEf) != 0) {
        rv = _bcm_tr_mpls_port_delete(unit, vpn, vp);
        if (rv < 0) {
            return rv;
        }
    }
    return BCM_E_NONE;
}
/*
 * Function:
 *      bcmi_mpls_delete_all_backup_ports
 * Purpose:
 *      Delete all mpls backup ports in that vpn
 * Parameters:
 *      unit       - (IN) Device Number
 *      vpn        - (IN) VPN instance ID
 * Returns:
 *      BCM_E_XXX
 */
int
bcmi_mpls_delete_all_backup_ports(int unit, bcm_vpn_t vpn)
{

    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int max_vpws_vpn, num_vp;
    bcm_vpn_t vpn_id;

    if (!_BCM_MPLS_VPN_IS_VPWS(vpn)) {
        return BCM_E_PARAM;
    }

    _BCM_MPLS_VPN_GET(vpn_id, _BCM_MPLS_VPN_TYPE_VPWS, vpn);

    num_vp = soc_mem_index_count(unit, SOURCE_VPm);
    max_vpws_vpn = num_vp / 2;

    if (vpn_id <= max_vpws_vpn) {
        if (mpls_info->vpws_vp_map[vpn_id].vp_b_1 != -1) {
            BCM_IF_ERROR_RETURN(bcmi_mpls_delete_backup_ports(unit, vpn,
                mpls_info->vpws_vp_map[vpn_id].vp_b_1));
        }

        if (mpls_info->vpws_vp_map[vpn_id].vp_b_2 != -1) {
            BCM_IF_ERROR_RETURN(bcmi_mpls_delete_backup_ports(unit, vpn,
                mpls_info->vpws_vp_map[vpn_id].vp_b_2));
        }

        if (mpls_info->vpws_vp_map[vpn_id].vp_b_3 != -1) {
             BCM_IF_ERROR_RETURN(bcmi_mpls_delete_backup_ports(unit, vpn,
                mpls_info->vpws_vp_map[vpn_id].vp_b_3));
        }
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_mpls_port_delete_all
 * Purpose:
 *      Delete all mpls ports from a VPN
 * Parameters:
 *      unit - Device Number
 *      vpn - VPN instance ID
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_port_delete_all(int unit, bcm_vpn_t vpn)
{
    int rv = BCM_E_NONE;

    if (!_BCM_MPLS_VPN_IS_VPLS(vpn) && 
         !_BCM_MPLS_VPN_IS_VPWS(vpn)) {
        return BCM_E_PARAM;
    }

    if (_BCM_MPLS_VPN_IS_VPWS(vpn)) {
        int vp1=-1, vp2=-1, vp3_read = -1;
        source_vp_entry_t svp1, svp2, svp3;
        ing_dvp_table_entry_t dvp1, dvp2, dvp3;
        bcm_vpn_t   vpn_id;

        sal_memset(&svp1, 0, sizeof(source_vp_entry_t));
        sal_memset(&svp2, 0, sizeof(source_vp_entry_t));
        sal_memset(&svp3, 0, sizeof(source_vp_entry_t));
        sal_memset(&dvp1, 0, sizeof(ing_dvp_table_entry_t));
        sal_memset(&dvp2, 0, sizeof(ing_dvp_table_entry_t));
        sal_memset(&dvp3, 0, sizeof(ing_dvp_table_entry_t));

       _BCM_MPLS_VPN_GET(vpn_id, _BCM_MPLS_VPN_TYPE_VPWS, vpn);

        (void) _bcm_tr_mpls_vpws_vp_map_get (unit, vpn_id, &vp1, &vp2, &vp3_read);

        if (vp1 != -1) {
            rv = READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp1, &svp1);
            if (rv < 0) {
                goto done;
            }
            rv = READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp1, &dvp1);
            if (rv < 0) {
                goto done;
            }
            /* See if the entry already exists */
            if (soc_SOURCE_VPm_field32_get(unit, &svp1, ENTRY_TYPEf) != 0) {
                rv = _bcm_tr_mpls_port_delete(unit, vpn, vp1);
                if (rv < 0) {
                    goto done;
                }
            }
        }

        if (vp2 != -1) {
            rv = READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp2, &svp2);
            if (rv < 0) {
                goto done;
            }
            rv = READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp2, &dvp2);
            if (rv < 0) {
                goto done;
            }
            if (soc_SOURCE_VPm_field32_get(unit, &svp2, ENTRY_TYPEf) != 0) {
                rv = _bcm_tr_mpls_port_delete(unit, vpn, vp2);
                if (rv < 0) {
                    goto done;
                }
            }
        }
#ifdef BCM_MULTI_LEVEL_FAILOVER_SUPPORT
        if (soc_feature(unit, soc_feature_hierarchical_protection)) {
             rv = bcmi_mpls_delete_all_backup_ports(unit, vpn);
             if (rv < 0) {
                 goto done;
             }
        }
#endif 
        /* need to delete vp3 for there may be a dedicated vp for failover port. */
        if (vp3_read != -1) {
            rv = READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp3_read, &svp3);
            if (rv < 0) {
                goto done;
            }
            rv = READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp3_read, &dvp3);
            if (rv < 0) {
                goto done;
            }
            if (soc_SOURCE_VPm_field32_get(unit, &svp3, ENTRY_TYPEf) != 0) {
                rv = _bcm_tr_mpls_port_delete(unit, vpn, vp3_read);
                if (rv < 0) {
                    goto done;
                }
            }
        }
    } else if (_BCM_MPLS_VPN_IS_VPLS(vpn)) {
        uint32 vfi, vp, num_vp;
        source_vp_entry_t svp;

        sal_memset(&svp, 0, sizeof(source_vp_entry_t));

        _BCM_MPLS_VPN_GET(vfi, _BCM_MPLS_VPN_TYPE_VPLS, vpn);

        if (!_bcm_vfi_used_get(unit, vfi, _bcmVfiTypeMpls)) {
            rv =  BCM_E_NOT_FOUND;
            goto done;
        }
        num_vp = soc_mem_index_count(unit, SOURCE_VPm);
        for (vp = 0; vp < num_vp; vp++) {
            /* Check for the validity of the VP */
            if (!_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {	
                continue;
            }
            rv = READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp);
            if (rv < 0) {
                goto done;
            }
            if ((soc_SOURCE_VPm_field32_get(unit, &svp, ENTRY_TYPEf) == 0x1) &&
                    (vfi == soc_SOURCE_VPm_field32_get(unit, &svp, VFIf))) {
                rv = _bcm_tr_mpls_port_delete(unit, vpn, vp);
                if (rv < 0) {
                    goto done;
                }
            } else if ((soc_feature(unit, soc_feature_vp_sharing) ||
                    soc_feature(unit, soc_feature_nexthop_share_dvp)) && 
                    (_bcm_virtual_bk_info[unit].vp_shared_vp_bitmap)) {
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
                if (SHR_BITGET((&_bcm_virtual_bk_info[unit])->vp_shared_vp_bitmap, vp) &&
                        MPLS_INFO(unit)->match_key[vp].match_count > 0) {
                    rv = _bcm_tr_mpls_port_delete(unit, vpn, vp);
                    if (rv < 0) {
                        goto done;
                    }
                }
#endif
            }
        }
    }

  done:
    return rv;
}

STATIC int
_bcm_tr_mpls_port_get(int unit, bcm_vpn_t vpn, int vp,
                      bcm_mpls_port_t *mpls_port)
{
    int i, nh_index, tpid_enable = 0, rv = BCM_E_NONE;
    ing_dvp_table_entry_t dvp;
    source_vp_entry_t svp;
    int split_horizon_port_flag=0;
    int vpless_failover_port = FALSE; /* the failover port without a dedicated vp */
    _bcm_vp_info_t vp_info;
#if defined(BCM_TRIUMPH2_SUPPORT)
    int vp1, vp2, vp3;
    vp1 = vp2 = vp3 = -1;
#endif

    /* Initialize the structure */
    bcm_mpls_port_t_init(mpls_port);
    BCM_GPORT_MPLS_PORT_ID_SET(mpls_port->mpls_port_id, vp);

    if (_BCM_MPLS_GPORT_FAILOVER_VPLESS_GET(vp)) {
        vpless_failover_port = TRUE;
        mpls_port->mpls_port_id =
              _BCM_MPLS_GPORT_FAILOVER_VPLESS_SET(mpls_port->mpls_port_id);
        vp = _BCM_MPLS_GPORT_FAILOVER_VPLESS_CLEAR(vp);

        mpls_port->flags |= BCM_MPLS_PORT_FAILOVER;
    }

    BCM_IF_ERROR_RETURN (READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp, &dvp));
    nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp, NEXT_HOP_INDEXf);

#if defined(BCM_TRIUMPH2_SUPPORT)
    if (soc_feature(unit, soc_feature_mpls_failover)) {  
        int prot_nh_index =-1;
        int prot_mc_group=-1;
        int failover_id;
        int dvp = -1;
        egr_l3_next_hop_entry_t egr_nh;

        BCM_IF_ERROR_RETURN(_bcm_esw_failover_prot_nhi_get(unit, nh_index,
                            &failover_id, &prot_nh_index, &prot_mc_group));
        if (vpless_failover_port == TRUE) {
            if (prot_mc_group > 0) {
                rv = _bcm_tr_mpls_vpless_failover_nh_index_find(unit, vp,
                            nh_index, &prot_nh_index);
                if (BCM_SUCCESS(rv) && (prot_nh_index > 0)) {
                    nh_index = prot_nh_index;
                }
            } else if (prot_nh_index > 0) {
                nh_index = prot_nh_index;
            }
        }
        
        if (failover_id != 0) {
            if (prot_mc_group > 0) {
                if ((_BCM_MPLS_VPN_IS_VPWS(vpn)) && 
                    (!_BCM_FAILOVER_IS_MULTI_LEVEL(failover_id))){
                    bcm_vpn_t   vpn_id;

                    _BCM_MPLS_VPN_GET(vpn_id, _BCM_MPLS_VPN_TYPE_VPWS, vpn);
                    (void) _bcm_tr_mpls_vpws_vp_map_get (unit, vpn_id, &vp1,
                                                         &vp2, &vp3);
                    if (((FALSE == vpless_failover_port) && (vp3 == vp)) ||
                        (TRUE == vpless_failover_port)) {                        
                        BCM_GPORT_MPLS_PORT_ID_SET(mpls_port->failover_port_id,
                                                   vp2);
                        mpls_port->failover_id = failover_id;
                        mpls_port->failover_mc_group = prot_mc_group;
                    }
                } else {
                    mpls_port->failover_id = failover_id;
                    mpls_port->failover_mc_group = prot_mc_group;
#ifdef BCM_TRIUMPH2_SUPPORT 
                    if (soc_feature(unit, soc_feature_mpls_failover)) {
                        if ((_BCM_MPLS_VPN_IS_VPLS(vpn)) && 
                        (!_BCM_FAILOVER_IS_MULTI_LEVEL(failover_id))) {
                            if (vpless_failover_port == FALSE) {
                                _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
                                BCM_GPORT_MPLS_PORT_ID_SET(mpls_port->failover_port_id,
                                                           mpls_info->failover_vp[vp]);
                            } else {
                                BCM_GPORT_MPLS_PORT_ID_SET(mpls_port->failover_port_id,
                                                           vp);
                            }
                        }
                    }
#endif                                        
                }
            } else {
                /* Retrieve EGR L3 NHOP Entry */
                BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm,
                            MEM_BLOCK_ANY, prot_nh_index, &egr_nh));
                if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                    dvp = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, MPLS__DVPf);
                } else {
                    dvp = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, DVPf);
                }
                if (vpless_failover_port == TRUE) {
                    mpls_port->failover_id = failover_id;
                    BCM_GPORT_MPLS_PORT_ID_SET(mpls_port->failover_port_id, dvp);
                } else {
                    int primary_dvp = -1;
                    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm,
                                                      MEM_BLOCK_ANY, nh_index, &egr_nh));
                    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                        primary_dvp = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, MPLS__DVPf);
                    } else {
                        primary_dvp = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, DVPf);
                    }
                    if (primary_dvp != dvp) {
                        mpls_port->failover_id = failover_id;
                        BCM_GPORT_MPLS_PORT_ID_SET(mpls_port->failover_port_id, dvp);
                    }
                }
            }
        }
    }
#endif /* BCM_TRIUMPH2_SUPPORT  */

    if (soc_mem_field_valid(unit, EGR_DVP_ATTRIBUTEm, MTU_VALUEf)) {
        rv = _bcm_tr_mpls_port_mtu_get(unit, vp, &mpls_port->mtu);
        BCM_IF_ERROR_RETURN(rv);
    }

    /* Get the match parameters */
    rv = _bcm_esw_mpls_match_get(unit, mpls_port,
          vpless_failover_port == FALSE? vp: 
          _BCM_MPLS_GPORT_FAILOVER_VPLESS_SET(vp));
    BCM_IF_ERROR_RETURN(rv);

    /* Check for Network-Port */
    BCM_IF_ERROR_RETURN(_bcm_vp_info_get(unit, vp, &vp_info));
    if (vp_info.flags & _BCM_VP_INFO_NETWORK_PORT) {
        split_horizon_port_flag = TRUE;
    }

    /* Get the next-hop parameters */
    rv = _bcm_tr_mpls_l2_nh_info_get(unit, vp, mpls_port, nh_index);
    BCM_IF_ERROR_RETURN(rv);

    if ((mpls_port->egress_label.flags & BCM_MPLS_EGRESS_LABEL_ENTROPY_ENABLE) &&
        (mpls_port->flags2 & BCM_MPLS_PORT2_INGRESS_ENTROPY_ENABLE)) {
        mpls_port->flags |= BCM_MPLS_PORT_ENTROPY_ENABLE;
    }

    if(nh_index>0)
        mpls_port->encap_id = nh_index + BCM_XGS3_DVP_EGRESS_IDX_MIN;
    /* Fill in SVP parameters */
    BCM_IF_ERROR_RETURN (READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp));
    mpls_port->if_class = soc_SOURCE_VPm_field32_get(unit, &svp, CLASS_IDf);
    if (split_horizon_port_flag) {
        mpls_port->flags |= BCM_MPLS_PORT_NETWORK;
    }
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_global_meter)) {
        _bcm_esw_get_policer_from_table(unit, SOURCE_VPm, vp, &svp, 
                                                    &mpls_port->policer_id, 1); 
    }
#endif
    if (soc_SOURCE_VPm_field32_get(unit, &svp, SD_TAG_MODEf)) {
        tpid_enable = soc_SOURCE_VPm_field32_get(unit, &svp, TPID_ENABLEf);
        if (tpid_enable) {
            mpls_port->flags |= BCM_MPLS_PORT_SERVICE_TAGGED;
            if (!((mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_ADD) ||
                  (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_TPID_REPLACE) ||
                  (mpls_port->flags & BCM_MPLS_PORT_SERVICE_TPID_REPLACE) ||
                  (mpls_port->flags & BCM_MPLS_PORT_SERVICE_VLAN_PRI_TPID_REPLACE))) {
                for (i = 0; i < 4; i++) {
                    if (tpid_enable & (1 << i)) {
                        _bcm_fb2_outer_tpid_entry_get(unit, &mpls_port->service_tpid, i);
                    }
                }
            }
        }
    }
    return rv;
}

/*
 * Function:
 *      bcm_mpls_port_get
 * Purpose:
 *      Get an mpls port from a VPN
 * Parameters:
 *      unit       - (IN) Device Number
 *      vpn        - (IN) VPN instance ID
 *      mpls_port  - (IN/OUT) mpls port information
 */
int
bcm_tr_mpls_port_get(int unit, bcm_vpn_t vpn, bcm_mpls_port_t *mpls_port)
{
    int vp;

    if (!_BCM_MPLS_VPN_IS_VPLS(vpn) && 
         !_BCM_MPLS_VPN_IS_VPWS(vpn)) {
        if ((vpn != BCM_MPLS_VPWS_VPN_INVALID) && 
            (vpn != BCM_MPLS_VPLS_VPN_INVALID)) {
            return BCM_E_PARAM;
        }
    }

    vp = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port->mpls_port_id);
    if (vp == -1) {
        return BCM_E_PARAM;
    }

    if (!_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
        return BCM_E_NOT_FOUND;
    }
    if (_BCM_MPLS_GPORT_FAILOVER_VPLESS_GET(mpls_port->mpls_port_id)) {
        vp = _BCM_MPLS_GPORT_FAILOVER_VPLESS_SET(vp);
    }
    return _bcm_tr_mpls_port_get(unit, vpn, vp, mpls_port);
}

/*
 * Function:
 *      bcm_mpls_port_get_all
 * Purpose:
 *      Get all mpls ports from a VPN
 * Parameters:
 *      unit     - (IN) Device Number
 *      vpn      - (IN) VPN instance ID
 *      port_max   - (IN) Maximum number of interfaces in array
 *      port_array - (OUT) Array of mpls ports
 *      port_count - (OUT) Number of interfaces returned in array
 *
 */
int
bcm_tr_mpls_port_get_all(int unit, bcm_vpn_t vpn, int port_max,
                         bcm_mpls_port_t *port_array, int *port_count)
{
    int vp, rv = BCM_E_NONE;
    int vp1=-1, vp2=-1, vp3_read = -1;
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
    SHR_BITDCL *share_vp_bitmap = NULL;
#endif

    if (!_BCM_MPLS_VPN_IS_VPLS(vpn) && 
         !_BCM_MPLS_VPN_IS_VPWS(vpn)) {
        return BCM_E_PARAM;
    }

    *port_count = 0;

    if (_BCM_MPLS_VPN_IS_VPWS(vpn)) {
        bcm_vpn_t   vpn_id;

        _BCM_MPLS_VPN_GET(vpn_id, _BCM_MPLS_VPN_TYPE_VPWS, vpn);

        (void) _bcm_tr_mpls_vpws_vp_map_get (unit, vpn_id, &vp1, &vp2, &vp3_read);

        vp = vp1;
        if (_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
            if (*port_count < port_max) {
                rv = _bcm_tr_mpls_port_get(unit, vpn, vp, 
                                           &port_array[*port_count]);
                if (rv < 0) {
                    goto done;
                }
                (*port_count)++;
            }
        }

        vp = vp2;
        if (_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
            if (*port_count < port_max) {
                rv = _bcm_tr_mpls_port_get(unit, vpn, vp, 
                                           &port_array[*port_count]);
                if (rv < 0) {
                    goto done;
                }
                (*port_count)++;
            }
        }

        vp = vp3_read;
        if (_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
            if (*port_count < port_max) {
                rv = _bcm_tr_mpls_port_get(unit, vpn, vp, 
                                           &port_array[*port_count]);
                if (rv < 0) {
                    goto done;
                }
                (*port_count)++;
            }
        }
    } else if (_BCM_MPLS_VPN_IS_VPLS(vpn)) {
        uint32 vfi, entry_type;
        int num_vp;
        source_vp_entry_t svp;
        int vpls_vpn_min = 0;
        int vpls_vpn_max = 0;

        _BCM_MPLS_VPN_SET(vpls_vpn_min, _BCM_MPLS_VPN_TYPE_VPLS, 0);
        vpls_vpn_max = soc_mem_index_count(unit, VFIm) + vpls_vpn_min - 1;
        if (vpn < vpls_vpn_min || vpn > vpls_vpn_max) {
            return BCM_E_PARAM;
        }

        _BCM_MPLS_VPN_GET(vfi, _BCM_MPLS_VPN_TYPE_VPLS, vpn);

        if (!_bcm_vfi_used_get(unit, vfi, _bcmVfiTypeMpls)) {
            rv = BCM_E_NOT_FOUND;
            goto done;
        }
        num_vp = soc_mem_index_count(unit, SOURCE_VPm);
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
        /*get sharing VP bitmap*/
        share_vp_bitmap = sal_alloc(SHR_BITALLOCSIZE(num_vp), "sharr_vp_bitmap");
        if (share_vp_bitmap == NULL) {
            return BCM_E_MEMORY;
        }
        sal_memset(share_vp_bitmap, 0, SHR_BITALLOCSIZE(num_vp));

        if (soc_feature(unit, soc_feature_vp_sharing)) {
            rv = bcm_tr_mpls_share_vp_bitmap_get(unit, vpn, share_vp_bitmap);
            if (rv < 0) {
                goto cleanup;
            }
        }
#endif
        for (vp = 0; vp < num_vp; vp++) {
            if (*port_count == port_max) {
                break;
            }

            if (!_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
                continue;
            }

            rv = READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp);
            if (rv < 0) {
                goto cleanup;
            }
            entry_type = soc_SOURCE_VPm_field32_get(unit, &svp, ENTRY_TYPEf);

            if ((vfi == soc_SOURCE_VPm_field32_get(unit, &svp, VFIf) && entry_type == 0x1)
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
                     || SHR_BITGET(share_vp_bitmap, vp)
#endif
            ) {
                rv = _bcm_tr_mpls_port_get(unit, vpn, vp, 
                                           &port_array[*port_count]);
                if (rv < 0) {
                    goto cleanup;
                }
                (*port_count)++;
            }
        }
        cleanup:;
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
          sal_free(share_vp_bitmap);
#endif
    }
  done:
    return rv;
}

/*
 * Function:
 *      _bcm_tr_mpls_port_resolve
 * Purpose:
 *      Get the modid, port, trunk values for a MPLS port
 * Returns:
 *      BCM_E_XXX
 */
int
_bcm_tr_mpls_port_resolve(int unit, bcm_gport_t mpls_port_id, 
                          bcm_module_t *modid, bcm_port_t *port,
                          bcm_trunk_t *trunk_id, int *id)

{
    int rv = BCM_E_NONE, nh_index, vp;
    ing_l3_next_hop_entry_t ing_nh;
    ing_dvp_table_entry_t dvp;

    if(!SOC_WARM_BOOT(unit)) {
        /* Warmboot re-init is in progress. So skip init check */
        rv = _bcm_tr_mpls_check_init (unit);
        if (rv < 0) {
            return rv;
        }
    }

    if (!BCM_GPORT_IS_MPLS_PORT(mpls_port_id)) {
        return (BCM_E_BADID);
    }

    vp = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port_id);
    if (vp == -1) {
       return BCM_E_PARAM;
    }

    if (!_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
        return BCM_E_NOT_FOUND;
    }
    BCM_IF_ERROR_RETURN (READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp, &dvp));
    nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp, 
                                              NEXT_HOP_INDEXf);
    BCM_IF_ERROR_RETURN (soc_mem_read(unit, ING_L3_NEXT_HOPm, MEM_BLOCK_ANY,
                                      nh_index, &ing_nh));

    if (soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, ENTRY_TYPEf) != 0x2) {
        /* Entry type is not L2 DVP */
        return BCM_E_NOT_FOUND;
    }
    if (soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, Tf)) {
        *trunk_id = soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, TGIDf);
    } else {
        /* Only add this to replication set if destination is local */
        *modid = soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, MODULE_IDf);
        *port = soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, PORT_NUMf);
    }
    *id = vp;
    return rv;
}

void 
bcm_tr_mpls_port_match_count_adjust(int unit, int vp, int step)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
 
    mpls_info->match_key[vp].match_count += step;
}

static soc_field_t _tnl_label_f[] =
{ MPLS_LABEL_0f, MPLS_LABEL_1f, MPLS_LABEL_2f, MPLS_LABEL_3f,
  MPLS_LABEL_4f, MPLS_LABEL_5f, MPLS_LABEL_6f, MPLS_LABEL_7f };
static soc_field_t _tnl_push_action_f[] =
{ MPLS_PUSH_ACTION_0f, MPLS_PUSH_ACTION_1f,
  MPLS_PUSH_ACTION_2f, MPLS_PUSH_ACTION_3f,
  MPLS_PUSH_ACTION_4f, MPLS_PUSH_ACTION_5f,
  MPLS_PUSH_ACTION_6f, MPLS_PUSH_ACTION_7f };
static soc_field_t _tnl_exp_select_f[] =
{ MPLS_EXP_SELECT_0f, MPLS_EXP_SELECT_1f, 
  MPLS_EXP_SELECT_2f, MPLS_EXP_SELECT_3f,
  MPLS_EXP_SELECT_4f, MPLS_EXP_SELECT_5f,
  MPLS_EXP_SELECT_6f, MPLS_EXP_SELECT_7f };
static soc_field_t _tnl_exp_ptr_f[] =   
{ MPLS_EXP_MAPPING_PTR_0f, MPLS_EXP_MAPPING_PTR_1f, 
  MPLS_EXP_MAPPING_PTR_2f, MPLS_EXP_MAPPING_PTR_3f,
  MPLS_EXP_MAPPING_PTR_4f, MPLS_EXP_MAPPING_PTR_5f,
  MPLS_EXP_MAPPING_PTR_6f, MPLS_EXP_MAPPING_PTR_7f };
static soc_field_t _tnl_exp_f[] =    
{ MPLS_EXP_0f, MPLS_EXP_1f, MPLS_EXP_2f, MPLS_EXP_3f,
  MPLS_EXP_4f, MPLS_EXP_5f, MPLS_EXP_6f, MPLS_EXP_7f};
static soc_field_t _tnl_ttl_f[] =
{ MPLS_TTL_0f, MPLS_TTL_1f, MPLS_TTL_2f, MPLS_TTL_3f,
  MPLS_TTL_4f, MPLS_TTL_5f, MPLS_TTL_6f, MPLS_TTL_7f};
static soc_field_t _tnl_pri_f[] = 
{ NEW_PRI_0f, NEW_PRI_1f, NEW_PRI_2f, NEW_PRI_3f,
  NEW_PRI_4f, NEW_PRI_5f, NEW_PRI_6f, NEW_PRI_7f};
static soc_field_t _tnl_cfi_f[] = 
{ NEW_CFI_0f, NEW_CFI_1f, NEW_CFI_2f, NEW_CFI_3f,
  NEW_CFI_4f, NEW_CFI_5f, NEW_CFI_6f, NEW_CFI_7f};

/*  
 * Function:
 *      bcm_tr_mpls_tunnel_intf_valid
 * Purpose:
 *      Set MPLS Tunnel
 * Parameters:
 *      unit - Device Number
 *      nh_index - Next Hop Index
 * Returns:
 *      BCM_E_XXX
 */

int
bcm_tr_mpls_tunnel_intf_valid (int unit, int nh_index)
{
    egr_ip_tunnel_mpls_entry_t tnl_entry;
    egr_l3_intf_entry_t if_entry;
    egr_l3_next_hop_entry_t egr_nh;
    bcm_if_t   l3_intf;
    int  tnl_index = 0, mpls_index = 0;

    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                                      nh_index, &egr_nh));

    l3_intf = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, 
                                             INTF_NUMf);

    if (!BCM_L3_INTF_USED_GET(unit, l3_intf)) {
        return BCM_E_NOT_FOUND;
    }

    BCM_IF_ERROR_RETURN ( READ_EGR_L3_INTFm(unit, 
                                 MEM_BLOCK_ANY, l3_intf, &if_entry));
    mpls_index = soc_EGR_L3_INTFm_field32_get(unit, &if_entry, 
                                              MPLS_TUNNEL_INDEXf);
    tnl_index = mpls_index / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);

    if ((mpls_index != 0) && (_BCM_MPLS_TNL_USED_GET(unit, mpls_index))) {
         BCM_IF_ERROR_RETURN (READ_EGR_IP_TUNNEL_MPLSm(unit, MEM_BLOCK_ANY, 
                                      tnl_index, &tnl_entry));
         if (soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                                ENTRY_TYPEf) == 3) {
              return BCM_E_NONE;
         }
    }
    return BCM_E_NOT_FOUND;
}

/*
 * Function:
 *      _bcm_tr_mpls_egr_tunnel_ref_count_adjust
 * Purpose:
 *      Increment / Decrement Reference Count for Egress Tunnel
 * Parameters:
 *      unit              Unit number
 *      index            (IN) Entry Index
 *      step              (IN) usage reference
 */

STATIC void 
_bcm_tr_mpls_egr_tunnel_ref_count_adjust (int unit, int index, int step)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
 
    if (( mpls_info->egr_tunnel_ref_count[index] == 0 ) && (step < 0)) {
         mpls_info->egr_tunnel_ref_count[index] = 0;
    } else {
         mpls_info->egr_tunnel_ref_count[index] += step;
    }

}

/*
 * Function:
 *      _bcm_tr_mpls_egr_tunnel_ref_count_reset
 * Purpose:
 *      reset the reference Count 
 * Parameters:
 *      unit              Unit number
 *      index            (IN) Entry Index
 */

STATIC void
_bcm_tr_mpls_egr_tunnel_ref_count_reset (int unit, int index)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);

    mpls_info->egr_tunnel_ref_count[index] = 0;
}

/*
 * Function:
 *      _bcm_tr_mpls_egr_tunnel_ref_count_get
 * Purpose:
 *      get the current reference Count
 * Parameters:
 *      unit              Unit number
 *      index            (IN) Entry Index
 */

STATIC void
_bcm_tr_mpls_egr_tunnel_ref_count_get (int unit, int index, int *cnt_value)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);

    *cnt_value = mpls_info->egr_tunnel_ref_count[index];
}

/*
 * Function:
 *           _bcm_tr_mpls_get_vc_and_swap_table_index
 * Purpose:
 *           Obtain Index into  EGR_IP_TUNNEL_MPLS
 * Parameters:
 *           IN :  Unit
 *           IN :  push_action
 *           IN :  label_array
 *           OUT : match_mpls_tunnel_index
 * Returns:
 *      BCM_E_XXX
 */

STATIC int
_bcm_tr_mpls_egr_tunnel_lookup (int unit, int push_action, 
                                          bcm_mpls_egress_label_t *label_array,
                                          int *match_mpls_tunnel_index)
{
    uint32 label_value[8];
    uint32 entry_ttl, entry_exp, entry_push_action;
    int num_ip_tnl;
    int i = 0, tnl_index = -1, mpls_index = -1;
    egr_ip_tunnel_mpls_entry_t tnl_entry;

    num_ip_tnl = soc_mem_index_count(unit, EGR_IP_TUNNEL_MPLSm);

    /* parameter checking */
    if (push_action != 0 && label_array == NULL) {
        return BCM_E_PARAM;
    }

    for (tnl_index = 0; tnl_index < num_ip_tnl; tnl_index++) {
         BCM_IF_ERROR_RETURN(
              READ_EGR_IP_TUNNEL_MPLSm(unit, MEM_BLOCK_ANY, 
                             tnl_index, &tnl_entry));
              if (!_BCM_MPLS_IP_TNL_USED_GET(unit, tnl_index)) {
                continue;
              }
              if (soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                                ENTRY_TYPEf) != 3) {
                    return BCM_E_INTERNAL;
              }
         mpls_index = tnl_index * _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);
         for (i = 0; i <  _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit); i++) {
              if (!(_BCM_MPLS_TNL_USED_GET(unit, mpls_index))) {
                    mpls_index++;
                    continue;
              }
              label_value[i] = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, 
                            &tnl_entry, _tnl_label_f[i]);
              entry_ttl = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, 
                            &tnl_entry, _tnl_ttl_f[i]);
              entry_exp = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, 
                            &tnl_entry, _tnl_exp_f[i]);
              entry_push_action = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm,
                            &tnl_entry, _tnl_push_action_f[i]);

              if ((push_action == 0x2) &&
                      (i < (_BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit) - 1))){
                   /* Match 2 consecutive label entries. */
                  label_value[i+1] = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, 
                            &tnl_entry, _tnl_label_f[i+1]);
                  if ( (label_array[0].label == label_value[i]) && 
                        (label_array[1].label == label_value[i+1]) ) {
                       *match_mpls_tunnel_index = (tnl_index * 
                               _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit)) + i;
                       return BCM_E_NONE;
                   }
              } else if (push_action == 0x1) {
                   if ((label_array[0].label == label_value[i]) &&
                       (label_array[0].ttl == entry_ttl) &&
                       (label_array[0].exp == entry_exp)) {
                       *match_mpls_tunnel_index = (tnl_index * 
                               _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit)) + i;
                       return BCM_E_NONE;
                   }
              } else if (push_action == 0x0) {
                   /* Case of Dummy entry */
                   if ((0 == label_value[i]) &&
                       (0 == entry_ttl) &&
                       (0 == entry_exp) &&
                       (0 == entry_push_action)) {
                       *match_mpls_tunnel_index = (tnl_index * 
                               _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit)) + i;
                       return BCM_E_NONE;
                   }
              }
              mpls_index++;
         }
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *           _tr_mpls_tunnel_entry_copy
 * Purpose:
 *           Copy a source EGR_IP_TUNNEL_MPLS table entry to the
             destination. 
 * Parameters:
 *           IN :  Unit
 *           IN :  dst_mpls_inx - destination tunnel mpls index
 *           OUT : dst_entry    - destination tunnel entry
 *           IN :  src_mpls_inx - source tunnel mpls index
 *           IN :  src_entry    - source tunnel entry
 * Returns:
 *      BCM_E_XXX
 */

STATIC int
_tr_mpls_tunnel_entry_copy (int unit, int dst_mpls_inx,
                         egr_ip_tunnel_mpls_entry_t *dst_entry,
                         int src_mpls_inx,
                         egr_ip_tunnel_mpls_entry_t *src_entry)
{
    int dst_offset;
    int src_offset;
    int src_value;

    dst_offset = dst_mpls_inx &
        _BCM_MPLS_EGR_L3_INTF_MPLS_INDEX_OFFSET_MASK(unit);
    src_offset = src_mpls_inx &
        _BCM_MPLS_EGR_L3_INTF_MPLS_INDEX_OFFSET_MASK(unit);

    src_value = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, src_entry,
                  _tnl_push_action_f[src_offset]);
    soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, dst_entry,
                  _tnl_push_action_f[dst_offset], src_value);
    src_value = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, src_entry,
                  _tnl_label_f[src_offset]);
    soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, dst_entry, 
                  _tnl_label_f[dst_offset], src_value);
    src_value = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, src_entry,
                  _tnl_exp_select_f[src_offset]);
    soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, dst_entry, 
                  _tnl_exp_select_f[dst_offset], src_value);
    src_value = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, src_entry,
                  _tnl_exp_f[src_offset]);
    soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, dst_entry, 
                  _tnl_exp_f[dst_offset], src_value);
    src_value = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, src_entry,
                  _tnl_pri_f[src_offset]);
    soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, dst_entry, 
                  _tnl_pri_f[dst_offset], src_value);
    src_value = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, src_entry,
                  _tnl_cfi_f[src_offset]);
    soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, dst_entry, 
                  _tnl_cfi_f[dst_offset], src_value);
    src_value = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, src_entry,
                  _tnl_exp_ptr_f[src_offset]);
    soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, dst_entry, 
                  _tnl_exp_ptr_f[dst_offset], src_value);
    src_value = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, src_entry,
                  _tnl_ttl_f[src_offset]);
    soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, dst_entry, 
                  _tnl_ttl_f[dst_offset], src_value);
    return BCM_E_NONE;
}

/*
 * Function:
 *           _tr_egr_l3_intf_tunnel_index_replace
 * Purpose:
 *           Traverse the EGR_L3_INTF table to replace the old tunnel 
 *           index with the new index. 
 * Parameters:
 *           IN :  Unit
 *           IN :  old_mpls_inx - old tunnel mpls index
 *           IN :  new_mpls_inx - new tunnel mpls index
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
_tr_egr_l3_intf_tunnel_index_replace (int unit, int old_mpls_inx, 
                            int new_mpls_inx)
{
    /* Indexes to iterate over memories, chunks and entries */
    int             chnk_idx, ent_idx, chnk_idx_max, mem_idx_max;
    int             buf_size, chunksize, chnk_end;
    uint32          *tbl_chnk;
    uint32          *el3inf_entry;
    int             rv = BCM_E_NONE;
    soc_mem_t mem;
    int mpls_inx;

    mem = EGR_L3_INTFm;

    chunksize = 0x400; /* 1k */
    buf_size = sizeof(egr_l3_intf_entry_t) * chunksize;
    tbl_chnk = soc_cm_salloc(unit, buf_size, "egr_l3_intf traverse");
    if (NULL == tbl_chnk) {
        return BCM_E_MEMORY;
    }

    mem_idx_max = soc_mem_index_max(unit, mem);
    for (chnk_idx = soc_mem_index_min(unit, mem);
         chnk_idx <= mem_idx_max;
         chnk_idx += chunksize) {
        sal_memset((void *)tbl_chnk, 0, buf_size);

        chnk_idx_max =
            ((chnk_idx + chunksize) <= mem_idx_max) ?
            chnk_idx + chunksize - 1: mem_idx_max;

        soc_mem_lock(unit, mem);
        rv = soc_mem_read_range(unit, mem, MEM_BLOCK_ANY,
                                chnk_idx, chnk_idx_max, tbl_chnk);
        if (SOC_FAILURE(rv)) {
            soc_mem_unlock(unit, mem);
            break;
        }
        chnk_end = (chnk_idx_max - chnk_idx);
        for (ent_idx = 0 ; ent_idx <= chnk_end; ent_idx ++) {
            el3inf_entry =
                soc_mem_table_idx_to_pointer(unit, mem, uint32 *,
                                             tbl_chnk, ent_idx);

            mpls_inx = soc_mem_field32_get(unit, mem, el3inf_entry, 
                                            MPLS_TUNNEL_INDEXf);

            if (mpls_inx != old_mpls_inx) {
                continue;
            }
            /* replace with new tunnel index */
            soc_mem_field32_set(unit, mem,el3inf_entry, 
                                MPLS_TUNNEL_INDEXf, new_mpls_inx);
            rv = soc_mem_write(unit, mem, MEM_BLOCK_ANY, chnk_idx+ent_idx, 
                                el3inf_entry);
            if (BCM_FAILURE(rv)) {
                break;
            }
        }
        soc_mem_unlock(unit, mem);
        if (BCM_FAILURE(rv)) {
            break;
        }
    }
    soc_cm_sfree(unit, tbl_chnk);
    return rv;
}

/*
 * Function:
 *           _tr_egr_tunnel_mpls_free_slots_reorder
 * Purpose:
 *           Re-arrange two non-consecutive free slots in EGR_TUNNEL_MPLS table
 *           to make it consective for the purpose of pushing two labels. 
 *           This involves to move a occupied slot ajacent to the first free slot
 *           to the second free slot and traverse the EGR_L3_INTF table to 
 *           replace the tunnel index accordingly.
 * Parameters:
 *           IN :  Unit
 *           IN :  free_slot1 - first free slot 
 *           IN :  free_slot2 - second free slot
 *           IN :  src_tnl_entry - tunnel entry with first free MPLS slot
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
_tr_egr_tunnel_mpls_free_slots_reorder (int unit, int free_slot1, int free_slot2,
                    egr_ip_tunnel_mpls_entry_t *src_tnl_entry)
{
    int src_tnl_mpls_inx;
    int dst_tnl_mpls_inx;
    int ref_count;
    int rv;
    egr_ip_tunnel_mpls_entry_t dst_tnl_entry;

    if (free_slot1 & 0x1) { /* need to free even entry above */
        src_tnl_mpls_inx = free_slot1 - 1;
    } else { /* free odd entry below */
        src_tnl_mpls_inx = free_slot1 + 1;
    }
    dst_tnl_mpls_inx = free_slot2;

    /* read destination  tunnel entry */
    rv = READ_EGR_IP_TUNNEL_MPLSm(unit, MEM_BLOCK_ANY,
            (dst_tnl_mpls_inx / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit)),
            &dst_tnl_entry);
    BCM_IF_ERROR_RETURN(rv);

    /* copy the source mpls entry to the destination */
    rv = _tr_mpls_tunnel_entry_copy(unit,dst_tnl_mpls_inx,&dst_tnl_entry,
                                 src_tnl_mpls_inx,src_tnl_entry);
    BCM_IF_ERROR_RETURN(rv);

    /* write the destination entry to HW */
    rv = WRITE_EGR_IP_TUNNEL_MPLSm(unit, MEM_BLOCK_ANY,
            (dst_tnl_mpls_inx / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit)),
             &dst_tnl_entry);
    BCM_IF_ERROR_RETURN(rv);

    /* now search the egr_l3_intf table to update the new tunnel index */
    rv = _tr_egr_l3_intf_tunnel_index_replace(unit,
                          src_tnl_mpls_inx,dst_tnl_mpls_inx);
    BCM_IF_ERROR_RETURN(rv);

    /* get the moved entry's original ref counter number */
    _bcm_tr_mpls_egr_tunnel_ref_count_get (unit, 
                            src_tnl_mpls_inx,&ref_count);
    _bcm_tr_mpls_egr_tunnel_ref_count_reset (unit, src_tnl_mpls_inx);
    _bcm_tr_mpls_egr_tunnel_ref_count_adjust (unit, 
                            dst_tnl_mpls_inx, ref_count);
    _BCM_MPLS_TNL_USED_SET(unit, dst_tnl_mpls_inx);
    _BCM_MPLS_TNL_USED_CLR(unit, src_tnl_mpls_inx);
    return rv;
}

/*  
 * Function:
 *      bcm_mpls_tunnel_initiator_set
 * Purpose:
 *      Set MPLS Tunnel initiator
 * Parameters:
 *      unit - Device Number
 *      intf - The egress L3 interface
 *      num_labels  - Number of labels in the array
 *      label_array - Array of MPLS label and header information
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_tunnel_initiator_set(int unit, bcm_if_t intf, int num_labels,
                                 bcm_mpls_egress_label_t *label_array)
{
    egr_ip_tunnel_mpls_entry_t tnl_entry;
    egr_l3_intf_entry_t if_entry;
    int rv, i, num_mpls_map, num_ip_tnl, push_action=0, offset;
    int found = 0, tnl_index = 0, mpls_index = 0;
    int hw_map_idx;
    bcm_tunnel_initiator_t tnl_init;
    uint32 tnl_flags;
    int old_push_act_value;
    int match_mpls_tunnel_index=-1;
    int free_slot1;
    int free_slot2;

    if ((num_labels < 0) || (num_labels > 2)  ||
        (intf < 0) || (intf >= L3_INFO(unit)->l3_intf_table_size)) {
        return BCM_E_PARAM;
    }

    if (num_labels == 2) {
        push_action = 0x2; /* push 2 labels */
    } else if (num_labels == 1) {
        push_action = 0x1; /* push 1 label */
    } 

    /* Param checking */
    num_mpls_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 64;
    for (i = 0; i < num_labels; i++) {
        if (label_array[i].qos_map_id == 0) { /* treat it as using default */
            hw_map_idx = 0;
        } else {
            BCM_IF_ERROR_RETURN(_egr_qos_id2hw_idx(unit,
                            label_array[i].qos_map_id,&hw_map_idx)); 
        }
        if ((label_array[i].label > 0xfffff) ||
            (hw_map_idx < 0) ||
            (hw_map_idx >= num_mpls_map) ||
            (label_array[i].exp > 7) || (label_array[i].pkt_pri > 7) ||
            (label_array[i].pkt_cfi > 1)) {
            return BCM_E_PARAM;
        }
    }

    if (!BCM_L3_INTF_USED_GET(unit, intf)) {
        LOG_INFO(BSL_LS_BCM_L3,
                 (BSL_META_U(unit,
                             "L3 interface not created\n")));
        return BCM_E_NOT_FOUND;
    }

    /* L3 interface info */
    rv = READ_EGR_L3_INTFm(unit, MEM_BLOCK_ANY, intf, &if_entry);
    if (rv < 0) {
        return rv;
    }

   /* init the allocated entry */
   sal_memset(&tnl_entry, 0, sizeof(egr_ip_tunnel_mpls_entry_t));

   /* Case of Dummy Tunnel entry sharing by multiple L3_INTF */
   if (num_labels == 0) {
           /* Lookup if Tunnel Entry with Tunnel Label exists? */
           rv = _bcm_tr_mpls_egr_tunnel_lookup (unit, push_action,
                                   NULL, &match_mpls_tunnel_index);
           if (rv < 0) {
              return rv;
           }

           if (match_mpls_tunnel_index != -1) {
               /* L3_Interface to point to mpls_tunnel */
               soc_EGR_L3_INTFm_field32_set(unit, &if_entry,
                                     MPLS_TUNNEL_INDEXf, match_mpls_tunnel_index);
               /* Increment Ref count */
               _bcm_tr_mpls_egr_tunnel_ref_count_adjust (unit, 
                                                   match_mpls_tunnel_index, 1);
               rv = WRITE_EGR_L3_INTFm(unit, MEM_BLOCK_ANY, intf, &if_entry);
               return rv;
           }
   }

   for (i = 0; i < num_labels; i++) {
       /* Check for Port_independent Label mapping */
       rv = bcm_tr_mpls_port_independent_range (unit, label_array[i].label, 
                                                      BCM_GPORT_INVALID);
       /* Either Port-scope-label or Explicit-Null-label */
       if ((rv == BCM_E_CONFIG) || (label_array[i].label == 0)) {
           /* Tunnel-label is Port-based */
           /* Lookup if Tunnel Entry with Tunnel Label exists? */
           rv = _bcm_tr_mpls_egr_tunnel_lookup (unit, push_action,
                                   label_array, &match_mpls_tunnel_index);
           if (rv < 0) {
              return rv;
           }

           if (match_mpls_tunnel_index != -1) {
               /* L3_Interface to point to mpls_tunnel */
               soc_EGR_L3_INTFm_field32_set(unit, &if_entry,
                                     MPLS_TUNNEL_INDEXf, match_mpls_tunnel_index);
               /* Increment Ref count */
               _bcm_tr_mpls_egr_tunnel_ref_count_adjust (unit, 
                                                   match_mpls_tunnel_index, 1);
               rv = WRITE_EGR_L3_INTFm(unit, MEM_BLOCK_ANY, intf, &if_entry);
               return rv;
           }
       }
    }

    mpls_index = soc_EGR_L3_INTFm_field32_get(unit, &if_entry, 
                                              MPLS_TUNNEL_INDEXf);
    tnl_index = mpls_index / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);

    if ((mpls_index != 0) && (_BCM_MPLS_TNL_USED_GET(unit, mpls_index))) {
       /* Obtain referenced Tunnel entry */ 
        rv = READ_EGR_IP_TUNNEL_MPLSm(unit, MEM_BLOCK_ANY, 
                                      tnl_index, &tnl_entry);
        if (rv < 0) {
            return rv;
        }
        if (soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                                ENTRY_TYPEf) != 3) {
            return BCM_E_INTERNAL;
        }
        offset = mpls_index & _BCM_MPLS_EGR_L3_INTF_MPLS_INDEX_OFFSET_MASK(unit);
        old_push_act_value = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm,
                                                 &tnl_entry, _tnl_push_action_f[offset]);
        if (old_push_act_value != push_action) {
            /* Changing push action is not permitted, need to clear/set */
            return BCM_E_PARAM;
        }
    } else {
        /* allocate an unused EGR_IP_TUNNEL_MPLS_ENTRY */
       num_ip_tnl = soc_mem_index_count(unit, EGR_IP_TUNNEL_MPLSm);

        /*
         * First try finding an allocated tunnel entry with 
         * unused MPLS entries.
         */
        free_slot1 = -1;
        free_slot2 = -1;
        for (tnl_index = 0; tnl_index < num_ip_tnl; tnl_index++) {
            if (!_BCM_MPLS_IP_TNL_USED_GET(unit, tnl_index)) {
                continue;
            }
            mpls_index = tnl_index * _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);
            for (i = 0; i < _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit); i++) {
                if (!(_BCM_MPLS_TNL_USED_GET(unit, mpls_index))) {
                    if (free_slot1 == -1) {
                        free_slot1 = mpls_index;
                    } else if (free_slot2 == -1) {
                        free_slot2 = mpls_index;
                    }
                }
                if ((push_action == 0x2) &&
                        (i == (_BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit) - 1))) {
                    /* Pushing 2 labels, need 2 consecutive entries. */
                    break;
                }
                if (!(_BCM_MPLS_TNL_USED_GET(unit, mpls_index))) {
                    if (push_action == 0x2) {
                        /* Pushing 2 labels, need 2 consecutive entries with even base */
                        if (!(_BCM_MPLS_TNL_USED_GET(unit, mpls_index + 1))) {
                            if(mpls_index % 2 == 0) {
                                found = 1;
                                break;
                            }
                        }
                    } else {
                        found = 1;
                        break;
                    }
                }
                mpls_index++;
            }
            if (found) {
                break;
            }
        }
        if (found) {
            /* Mark entry as used */
            _BCM_MPLS_TNL_USED_SET(unit, mpls_index);
            _bcm_tr_mpls_egr_tunnel_ref_count_adjust (unit, 
                                                   mpls_index, 1);
            if (push_action == 0x2) {
                _BCM_MPLS_TNL_USED_SET(unit, mpls_index+1);
                _bcm_tr_mpls_egr_tunnel_ref_count_adjust (unit, 
                                                   mpls_index+1, 1);
            }

            /* Read existing entry */
            rv = READ_EGR_IP_TUNNEL_MPLSm(unit, MEM_BLOCK_ANY, 
                                          tnl_index, &tnl_entry);
            if (rv < 0) {
                goto cleanup;
            }
        } else {
            /*
             * Alloc an egr_ip_tunnel entry. By calling bcm_xgs3_tnl_init_add
             * with _BCM_L3_SHR_WRITE_DISABLE flag, a tunnel index is
             * allocated but nothing is written to hardware. The "tnl_init"
             * information is not used, set to all zero.
             */
            bcm_tunnel_initiator_t_init(&tnl_init);
            tnl_flags = _BCM_L3_SHR_MATCH_DISABLE | _BCM_L3_SHR_WRITE_DISABLE |
                _BCM_L3_SHR_SKIP_INDEX_ZERO;
            rv = bcm_xgs3_tnl_init_add(unit, tnl_flags, &tnl_init, &tnl_index);
            if (rv == BCM_E_FULL) {
                if (free_slot2 == -1 || push_action != 2) {
                    return rv; /* table truely full */
                }
                /* process the case of table fragmentation fo two labels.
                 * push_action==2 
                 */

                tnl_index = (free_slot1 /
                        _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit));
                rv = READ_EGR_IP_TUNNEL_MPLSm(unit, MEM_BLOCK_ANY, 
                                          tnl_index, &tnl_entry);
                BCM_IF_ERROR_RETURN(rv);
                rv = _tr_egr_tunnel_mpls_free_slots_reorder(unit, free_slot1,
                     free_slot2, &tnl_entry);
                BCM_IF_ERROR_RETURN(rv);

                /*sync up after update if both free slots in same entry*/
                if ((free_slot1 / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit)) ==
                        (free_slot2 / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit))) {
                    rv = READ_EGR_IP_TUNNEL_MPLSm(unit, MEM_BLOCK_ANY, 
                                          tnl_index, &tnl_entry);
                    BCM_IF_ERROR_RETURN(rv);  
                }

                mpls_index = free_slot1 & (~1);
                /* Mark entries as used */
                _BCM_MPLS_TNL_USED_SET(unit, mpls_index);
                _bcm_tr_mpls_egr_tunnel_ref_count_adjust (unit,
                                                   mpls_index, 1);
                _BCM_MPLS_TNL_USED_SET(unit, mpls_index + 1);
                _bcm_tr_mpls_egr_tunnel_ref_count_adjust (unit,
                                                   mpls_index+1, 1);
                found = 1;  /* for cleanup */
            } else if (rv < 0) {
                return rv;
            } else {
                mpls_index = tnl_index * _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);

                /* Mark entries as used */
                _BCM_MPLS_IP_TNL_USED_SET(unit, tnl_index);
                _BCM_MPLS_TNL_USED_SET(unit, mpls_index);
                _bcm_tr_mpls_egr_tunnel_ref_count_adjust (unit, 
                                                   mpls_index, 1);
                if (push_action == 0x2) {
                    _BCM_MPLS_TNL_USED_SET(unit, mpls_index + 1);
                    _bcm_tr_mpls_egr_tunnel_ref_count_adjust (unit, 
                                                   mpls_index+1, 1);
                }
                /* Clear and set ENTRY_TYPE */
                sal_memset(&tnl_entry, 0, sizeof(egr_ip_tunnel_mpls_entry_t));
                soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry, 
                                ENTRY_TYPEf, 0x3);
            }
        } 
    }

    offset = mpls_index & _BCM_MPLS_EGR_L3_INTF_MPLS_INDEX_OFFSET_MASK(unit);
    for (i = 0; i < num_labels; i++) {
        if (label_array[i].qos_map_id == 0) { /* treat it as using default */
            hw_map_idx = 0;
        } else {
            rv = _egr_qos_id2hw_idx(unit,
                            label_array[i].qos_map_id,&hw_map_idx);
            if (rv < 0) {
                goto cleanup;
            }
        }

        soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                            _tnl_push_action_f[offset], push_action);
        if (push_action > 0) {
            soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry, 
                                _tnl_label_f[offset], label_array[i].label);
        }
        /*For Two Label push set Second label push action as 1*/
        if ((push_action == 0x2) && (i == 1)) {
            soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                    _tnl_push_action_f[offset], 1);

        }

        if ((label_array[i].flags & BCM_MPLS_EGRESS_LABEL_EXP_SET) ||
            (label_array[i].flags & BCM_MPLS_EGRESS_LABEL_PRI_SET)) {

            /* Use the specified EXP, PRI and CFI */
            if ((label_array[i].flags & BCM_MPLS_EGRESS_LABEL_EXP_REMARK) ||
                (label_array[i].flags & BCM_MPLS_EGRESS_LABEL_EXP_COPY) ||
                (label_array[i].flags & BCM_MPLS_EGRESS_LABEL_PRI_REMARK)) {
                rv = BCM_E_PARAM;
                goto cleanup;
            }
            soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry, 
                                _tnl_exp_select_f[offset], 0x0); /* USE_FIXED */
            soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry, 
                                _tnl_exp_f[offset], label_array[i].exp);
            soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry, 
                                _tnl_pri_f[offset], label_array[i].pkt_pri);
            soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry, 
                                _tnl_cfi_f[offset], label_array[i].pkt_cfi);
        } else if (label_array[i].flags & BCM_MPLS_EGRESS_LABEL_EXP_REMARK) {
            if (label_array[i].flags & BCM_MPLS_EGRESS_LABEL_PRI_SET) {
                rv = BCM_E_PARAM;
                goto cleanup;
            }
            /* Use EXP-map for EXP, PRI and CFI */
            soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                                _tnl_exp_select_f[offset], 0x1); /* USE_MAPPING */
            soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry, 
                                _tnl_exp_ptr_f[offset], hw_map_idx);
        } else { /* BCM_MPLS_EGRESS_LABEL_EXP_COPY */
            /* Use EXP from inner label. If there is no inner label,
             * use the specified EXP value. Use EXP-map for PRI/CFI.
             */
            soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                                _tnl_exp_select_f[offset], 0x2); /* USE_INNER */
    
            soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry, 
                                _tnl_exp_f[offset], label_array[i].exp);
    
            /* Use EXP-map for PRI/CFI */
            soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry, 
                                _tnl_exp_ptr_f[offset], hw_map_idx);
        }
    
        if (label_array[i].flags & BCM_MPLS_EGRESS_LABEL_TTL_SET) {
            /* Use specified TTL */
            soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry, 
                                _tnl_ttl_f[offset], label_array[i].ttl);
        } else {
            soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry, 
                                _tnl_ttl_f[offset], 0);
        }
        offset++;
    }

    /* Commit the values to HW */
    rv = WRITE_EGR_IP_TUNNEL_MPLSm(unit, MEM_BLOCK_ANY, 
                                   tnl_index, &tnl_entry);
    if (rv < 0) {
        goto cleanup;
    }

    /* Update the EGR_L3_INTF to point to the MPLS tunnel entry */
    rv = soc_mem_field32_modify(unit, EGR_L3_INTFm, intf, 
                                MPLS_TUNNEL_INDEXf, mpls_index);

#ifdef BCM_WARM_BOOT_SUPPORT
    SOC_CONTROL_LOCK(unit);
    SOC_CONTROL(unit)->scache_dirty = 1;
    SOC_CONTROL_UNLOCK(unit);
#endif

    return rv;

  cleanup:
    if (!found) {
        /* Free the tunnel entry */
        tnl_flags = _BCM_L3_SHR_WRITE_DISABLE;
        (void) bcm_xgs3_tnl_init_del(unit, tnl_flags, tnl_index);

        /* Clear "in-use" for the IP tunnel entry */
        _BCM_MPLS_IP_TNL_USED_CLR(unit, tnl_index);
    }
    /* Clear "in-use" for the MPLS tunnel entry(s) */
    _BCM_MPLS_TNL_USED_CLR(unit, mpls_index);
    if (push_action == 0x2) {
        _BCM_MPLS_TNL_USED_CLR(unit, mpls_index + 1);
    }
    return rv;
}

/*  
 * Function:
 *      _bcm_tr_mpls_tunnel_initiator_clear
 * Purpose:
 *      Clear MPLS Tunnel initiator
 * Parameters:
 *      unit - Device Number
 *      intf - The egress L3 interface
 *      num_labels  - Number of labels in the array
 *      label_array - Array of MPLS label and header information
 * Returns:
 *      BCM_E_XXX
 */

STATIC int
_bcm_tr_mpls_tunnel_initiator_clear(int unit, int intf_id)
{
    egr_ip_tunnel_mpls_entry_t tnl_entry;
    egr_l3_intf_entry_t if_entry;
    int rv, offset, push_action, mpls_entry_used;
    int tnl_index = 0, mpls_index = 0;
    uint32 tnl_flags;
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);


    /* L3 interface info */
    rv = READ_EGR_L3_INTFm(unit, MEM_BLOCK_ANY, intf_id, &if_entry);
    if (rv < 0) {
        return rv;
    }
    mpls_index = soc_EGR_L3_INTFm_field32_get(unit, &if_entry,
                                              MPLS_TUNNEL_INDEXf);
    tnl_index = mpls_index / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);

    if (!_BCM_MPLS_TNL_USED_GET(unit, mpls_index)) {
        return BCM_E_NOT_FOUND;
    }
    rv = READ_EGR_IP_TUNNEL_MPLSm(unit, MEM_BLOCK_ANY,
                                  tnl_index, &tnl_entry);
    if (rv < 0) {
        return rv;
    }
    if (soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                            ENTRY_TYPEf) != 3) {
        return BCM_E_NOT_FOUND;
    }
    offset = mpls_index & _BCM_MPLS_EGR_L3_INTF_MPLS_INDEX_OFFSET_MASK(unit);
    push_action = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                                      _tnl_push_action_f[offset]);

    /* Decrement Reference Count */
    _bcm_tr_mpls_egr_tunnel_ref_count_adjust (unit, 
                                                  mpls_index, -1);
    if (push_action == 0x2) {
        _bcm_tr_mpls_egr_tunnel_ref_count_adjust (unit, 
                                                   mpls_index+1, -1);
    }

    /* Update the EGR_L3_INTF to no longer point to the MPLS tunnel entry */
    soc_mem_field32_set(unit, EGR_L3_INTFm, &if_entry,
                        MPLS_TUNNEL_INDEXf, 0);
    rv = WRITE_EGR_L3_INTFm(unit, MEM_BLOCK_ANY, intf_id, &if_entry);
    if (rv < 0) {
        return rv;
    }

    if ( mpls_info->egr_tunnel_ref_count[mpls_index] != 0x0 ) {
         return BCM_E_NONE;
    }

    /* Clear the MPLS tunnel entry(s) */
    soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                        _tnl_label_f[offset], 0);
    soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                        _tnl_push_action_f[offset], 0);
    soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                        _tnl_exp_select_f[offset], 0);
    soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                        _tnl_exp_ptr_f[offset], 0);
    soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                        _tnl_exp_f[offset], 0);
    soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                        _tnl_ttl_f[offset], 0);
    if (push_action == 0x2) {
        offset++;
        soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                            _tnl_label_f[offset], 0);
        soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                            _tnl_push_action_f[offset], 0);
        soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                            _tnl_exp_select_f[offset], 0);
        soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                            _tnl_exp_ptr_f[offset], 0);
        soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                            _tnl_exp_f[offset], 0);
        soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                            _tnl_ttl_f[offset], 0);
    }

    /* Clear "in-use" for the MPLS tunnel entry(s) */
    _BCM_MPLS_TNL_USED_CLR(unit, mpls_index);
    if (push_action == 0x2) {
        _BCM_MPLS_TNL_USED_CLR(unit, mpls_index + 1);
    }

    mpls_index = tnl_index * _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);

    /* See if we can free the IP tunnel base entry */
    _BCM_MPLS_TNL_USED_RANGE_GET(unit, mpls_index,
            _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit), mpls_entry_used);

    if (!mpls_entry_used) {
        /* None of the 4 or 8 entries are used, free base entry */
        tnl_flags = _BCM_L3_SHR_WRITE_DISABLE;
        (void) bcm_xgs3_tnl_init_del(unit, tnl_flags, tnl_index);

        /* Clear "in-use" for the IP tunnel entry */
        _BCM_MPLS_IP_TNL_USED_CLR(unit, tnl_index);

        /* Clear HW Entry-Type */
        soc_mem_field32_set(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                ENTRY_TYPEf, 0);
    }

    rv = WRITE_EGR_IP_TUNNEL_MPLSm(unit, MEM_BLOCK_ANY, 
                                   tnl_index, &tnl_entry);
    if (rv < 0) {
        return rv;
    }

#ifdef BCM_WARM_BOOT_SUPPORT
    SOC_CONTROL_LOCK(unit);
    SOC_CONTROL(unit)->scache_dirty = 1;
    SOC_CONTROL_UNLOCK(unit);
#endif

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_mpls_tunnel_initiator_clear
 * Purpose:
 *      Clear MPLS Tunnel Initiator
 * Parameters:
 *      unit - Device Number
 *      intf - The egress L3 interface
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_tunnel_initiator_clear(int unit, bcm_if_t intf)
{
    if ((intf < 0) || (intf >= L3_INFO(unit)->l3_intf_table_size)) {
        return BCM_E_PARAM;
    }

    if (!BCM_L3_INTF_USED_GET(unit, intf)) {
        LOG_INFO(BSL_LS_BCM_L3,
                 (BSL_META_U(unit,
                             "L3 interface not created\n")));
        return BCM_E_NOT_FOUND;
    }
    return _bcm_tr_mpls_tunnel_initiator_clear(unit, intf);
}

/*
 * Function:
 *      bcm_mpls_tunnel_initiator_get
 * Purpose:
 *      Get MPLS Tunnel Initiator info
 * Parameters:
 *      unit        - (IN) Device Number
 *      intf        - (IN) The egress L3 interface
 *      label_max   - (IN) Number of entries in label_array
 *      label_array - (OUT) MPLS header information
 *      label_count - (OUT) Actual number of labels returned
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_tunnel_initiator_get(int unit, bcm_if_t intf, int label_max,
                                 bcm_mpls_egress_label_t *label_array,
                                 int *label_count)
{
    egr_ip_tunnel_mpls_entry_t tnl_entry;
    egr_l3_intf_entry_t if_entry;
    int rv, i, ix, push_action, offset;
    int tnl_index = 0, mpls_index = 0;

    if ((label_array == NULL) ||
        (intf < 0) || (intf >= L3_INFO(unit)->l3_intf_table_size)) {
        return BCM_E_PARAM;
    }

    if (!BCM_L3_INTF_USED_GET(unit, intf)) {
        LOG_INFO(BSL_LS_BCM_L3,
                 (BSL_META_U(unit,
                             "L3 interface not created\n")));
        return BCM_E_NOT_FOUND;
    }

    /* L3 interface info */
    rv = READ_EGR_L3_INTFm(unit, MEM_BLOCK_ANY, intf, &if_entry);
    if (rv < 0) {
        return rv;
    }
    mpls_index = soc_EGR_L3_INTFm_field32_get(unit, &if_entry, 
                                              MPLS_TUNNEL_INDEXf);
    tnl_index = mpls_index / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);

    if (!_BCM_MPLS_TNL_USED_GET(unit, mpls_index)) {
        return BCM_E_NOT_FOUND;
    }
    rv = READ_EGR_IP_TUNNEL_MPLSm(unit, MEM_BLOCK_ANY, 
                                  tnl_index, &tnl_entry);
    if (rv < 0) {
        return rv;
    }
    if (soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                            ENTRY_TYPEf) != 3) {
        return BCM_E_NOT_FOUND;
    }

    *label_count = 0;
    sal_memset(label_array, 0, sizeof(bcm_mpls_egress_label_t) * label_max);
    offset = mpls_index & _BCM_MPLS_EGR_L3_INTF_MPLS_INDEX_OFFSET_MASK(unit);
    push_action = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                                      _tnl_push_action_f[offset]);
    if (push_action == 0x1) {
        *label_count = 1;
    } else if (push_action == 0x2) {
        *label_count = 2;
    }
    if (label_max < *label_count) {
        *label_count = label_max;
    }
    for (i = 0; i < *label_count; i++) {
        label_array[i].label =
            soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm,
                                &tnl_entry, _tnl_label_f[offset]);
        if (soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                                _tnl_exp_select_f[offset]) == 0x0) {
            /* Use the specified EXP, PRI and CFI */
            label_array[i].flags |= BCM_MPLS_EGRESS_LABEL_EXP_SET;
            label_array[i].flags |= BCM_MPLS_EGRESS_LABEL_PRI_SET;
            label_array[i].exp = 
                soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm,
                                    &tnl_entry, _tnl_exp_f[offset]);
            label_array[i].pkt_pri = 
                soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm,
                                    &tnl_entry, _tnl_pri_f[offset]);
            label_array[i].pkt_cfi = 
                soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm,
                                    &tnl_entry, _tnl_cfi_f[offset]);
        } else if (soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                                       _tnl_exp_select_f[offset]) == 0x1) {
            /* Use EXP-map for EXP, PRI and CFI */
            label_array[i].flags |= BCM_MPLS_EGRESS_LABEL_EXP_REMARK;
            label_array[i].flags |= BCM_MPLS_EGRESS_LABEL_PRI_REMARK;

            ix = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm,
                                     &tnl_entry, _tnl_exp_ptr_f[offset]);

            BCM_IF_ERROR_RETURN(
                    _egr_qos_hw_idx2id(unit, ix, &label_array[i].qos_map_id));
        } else {
            /* Use EXP from incoming label. If there is no incoming label,
             * use the specified EXP value.
             */
            label_array[i].flags |= BCM_MPLS_EGRESS_LABEL_EXP_COPY;
            label_array[i].flags |= BCM_MPLS_EGRESS_LABEL_PRI_REMARK;
            label_array[i].exp = 
                soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm,
                                    &tnl_entry, _tnl_exp_f[offset]);

            /* Use EXP-map for PRI/CFI */
            ix = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm,
                                     &tnl_entry, _tnl_exp_ptr_f[offset]);
            BCM_IF_ERROR_RETURN(
                    _egr_qos_hw_idx2id(unit, ix, &label_array[i].qos_map_id));
        }
        if (soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                                _tnl_ttl_f[offset])) {
            /* Use specified TTL value */
            label_array[i].flags |= BCM_MPLS_EGRESS_LABEL_TTL_SET;
            label_array[i].ttl = 
                soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm,
                                    &tnl_entry, _tnl_ttl_f[offset]);
        } else {
            label_array[i].flags |= BCM_MPLS_EGRESS_LABEL_TTL_DECREMENT;
        }
        offset++;
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_tr_mpls_tunnel_chain_label_count_get
 * Purpose:
 *      Get number of labels in a single chain
 * Parameters:
 *      unit        - (IN) Device Number
 *      index       - (IN) Mpls index
 *      label_count - (OUT) Actual number of labels returned
 * Returns:
 *      BCM_E_XXX
 */
int
_bcm_tr_mpls_tunnel_chain_label_count_get(int unit, uint32 index,
                                          int *label_count)
{
    egr_ip_tunnel_mpls_entry_t tnl_entry;
    int rv, i, push_action, offset;
    int tnl_index = 0;
    rv = BCM_E_NONE;

    tnl_index = index / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);
    if (!_BCM_MPLS_TNL_USED_GET(unit, index)) {
        return BCM_E_NOT_FOUND;
    }

    rv = READ_EGR_IP_TUNNEL_MPLSm(unit, MEM_BLOCK_ANY,
            tnl_index, &tnl_entry);

    if (rv < 0) {
        return rv;
    }
    if (soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                ENTRY_TYPEf) != 3) {
        return BCM_E_NOT_FOUND;
    }
    *label_count = 0;
    offset = index & _BCM_MPLS_EGR_L3_INTF_MPLS_INDEX_OFFSET_MASK(unit);

    for(i = 0; i < _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit) ; i++) {
        push_action = soc_mem_field32_get(unit, EGR_IP_TUNNEL_MPLSm, &tnl_entry,
                _tnl_push_action_f[offset+i]);
        if (push_action == 0x1) {
            break;
        }
    }
    *label_count = i+1;
    return rv;
}

/*
 * Function:
 *      bcm_mpls_tunnel_initiator_clear_all
 * Purpose:
 *      Clear all MPLS Tunnel Initiators
 * Parameters:
 *      unit - Device Number
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_tunnel_initiator_clear_all(int unit)
{
    egr_l3_intf_entry_t if_entry;
    int mpls_index, i;
    int rv=BCM_E_NONE;

    for (i = 0; i < L3_INFO(unit)->l3_intf_table_size; i++) {
        if (!BCM_L3_INTF_USED_GET(unit, i)) {
            continue;
        }
        rv = READ_EGR_L3_INTFm(unit, MEM_BLOCK_ANY, i, &if_entry);
        if (rv < 0) {
            return rv;
        }
        mpls_index = soc_EGR_L3_INTFm_field32_get(unit, &if_entry,
                                                  MPLS_TUNNEL_INDEXf);
        if (!_BCM_MPLS_TNL_USED_GET(unit, mpls_index)) {
            continue;
        }
        rv = _bcm_tr_mpls_tunnel_initiator_clear(unit, i);
        if (rv < 0) {
            return rv;
        }
    }
    return rv;
}

/*
 * Function:
 *       bcm_tr_mpls_get_entry_type
 * Purpose:
 *       To get Entry_type of  EGR_L3_NEXT_HOP Entry  for a specific EGR_NH Index
 * Parameters:
 *       IN :  Unit
 *       IN :  nh_index
 *      OUT: entry_type
 * Returns:
 *       BCM_E_XXX
 */

int
bcm_tr_mpls_get_entry_type (int unit, int nh_index, uint32 *entry_type )
{
    egr_l3_next_hop_entry_t egr_nh;
    int rv=BCM_E_NONE;

    /* Retrieve EGR L3 NHOP Entry */
    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                                      nh_index, &egr_nh));

    *entry_type = 
        soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, ENTRY_TYPEf);

    return rv;
}

/*
 * Function:
 *       bcm_tr_mpls_get_label_action
 * Purpose:
 *       To get MPLS Label action for a specific next_hop_index
 * Parameters:
 *       IN :  Unit
 *       IN :  nh_index
 *     OUT : label_action
 * Returns:
 *       BCM_E_XXX
 */

int
bcm_tr_mpls_get_label_action (int unit, int nh_index, uint32 *label_action )
{

    egr_l3_next_hop_entry_t egr_nh;
    egr_mpls_vc_and_swap_label_table_entry_t vc_entry;
    int vc_swap_idx;

    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                                      nh_index, &egr_nh));

    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
         vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       MPLS__VC_AND_SWAP_INDEXf);
    } else {
         vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       VC_AND_SWAP_INDEXf);
    }

    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                      MEM_BLOCK_ANY, vc_swap_idx, &vc_entry));

    *label_action =  soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                        &vc_entry, MPLS_LABEL_ACTIONf);
    return BCM_E_NONE;

}

/*
 * Function:
 *       bcm_tr_mpls_egress_entry_mac_replace
 * Purpose:
 *       Replace  MAC_DA within MAC_DA_PROFILE
 * Parameters:
 *       IN :  Unit
 *       IN :  nh_index
 *       IN :  Egress forwarding destination.
 * Returns:
 *       BCM_E_XXX
 */

int
bcm_tr_mpls_egress_entry_mac_replace(int unit, int nh_index, bcm_l3_egress_t *egr)
{
    egr_l3_next_hop_entry_t egr_nh;
    egr_mac_da_profile_entry_t macda;
    uint32 new_macda_index=-1, old_macda_index=-1;
    void *entries[1];
    int rv = BCM_E_NONE;
    int entry_type;

    BCM_IF_ERROR_RETURN
        (soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ALL, nh_index, &egr_nh));

    entry_type = soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh, ENTRY_TYPEf);

     /*
     * If entry_type is 0, then the MAC address is in EGR_L3_NEXT_HOPm
     */
     if (entry_type == 1) {
         sal_memset(&macda, 0, sizeof(egr_mac_da_profile_entry_t));
         soc_mem_mac_addr_set(unit, EGR_MAC_DA_PROFILEm, &macda, MAC_ADDRESSf, egr->mac_addr);
         entries[0] = &macda;
         BCM_IF_ERROR_RETURN
             (_bcm_mac_da_profile_entry_add(unit, entries, 1, (uint32 *) &new_macda_index));
 
         if (soc_feature(unit, soc_feature_mpls_enhanced)) {
             old_macda_index = soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm,
                         &egr_nh, MPLS__MAC_DA_PROFILE_INDEXf);
             soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                         &egr_nh, MPLS__MAC_DA_PROFILE_INDEXf, new_macda_index);
         } else {
             old_macda_index = soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm,
                          &egr_nh, MAC_DA_PROFILE_INDEXf);
             soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                          &egr_nh, MAC_DA_PROFILE_INDEXf, new_macda_index);
         }
 
         rv = soc_mem_write(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ALL, nh_index, &egr_nh);
         if (BCM_SUCCESS(rv)) { 
             if (old_macda_index != -1) {
                 _bcm_mac_da_profile_entry_delete(unit, old_macda_index);
             }
         } else {
                 _bcm_mac_da_profile_entry_delete(unit, new_macda_index);
         }
     } else if (soc_feature(unit, soc_feature_egress_object_mac_da_replace) && (entry_type == 0)) {
         if (soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm, MAC_ADDRESSf)) {
             /* Set next hop mac address. */
             soc_mem_mac_addr_set(unit, EGR_L3_NEXT_HOPm, &egr_nh, MAC_ADDRESSf, egr->mac_addr);
             rv = soc_mem_write(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ALL, nh_index, &egr_nh);
         }
     }
     return rv;
}

/*
 * Function:
 *       bcm_tr_mpls_l3_label_add
 * Purpose:
 *       Updates  Inner_LABEL within VC_AND_SWAP_TABLE for a specific EGR_NH Index
 * Parameters:
 *       IN :  Unit
 *       IN :  mpls_label
 *       IN :  nh_index
 * Returns:
 *       BCM_E_XXX
 */

int
bcm_tr_mpls_l3_label_add (int unit, bcm_l3_egress_t *egr, int nh_index, uint32 flags)
{
    int  rv=BCM_E_NONE;
    egr_mpls_vc_and_swap_label_table_entry_t vc_entry;
    int      vc_swap_index=-1;
    egr_l3_next_hop_entry_t egr_nh;
    uint32    entry_type;
    int       hw_map_idx, num_mpls_map;
    int bkt_idx;
    bcm_tr_mpls_vc_swap_table_hash_t *hash_input;
    int ref_count = 0;

    /* Retrieve EGR L3 NHOP Entry */
    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                                      nh_index, &egr_nh));

    sal_memset(&vc_entry, 0,
            sizeof(egr_mpls_vc_and_swap_label_table_entry_t));

    BCM_IF_ERROR_RETURN(bcm_tr_mpls_get_entry_type(unit, nh_index, &entry_type));
    if ((entry_type == 1) && (egr->mpls_label != BCM_MPLS_LABEL_INVALID) && (flags & BCM_L3_REPLACE)) {
        /* Be sure that the existing entry is already setup to
         * egress into an MPLS tunnel. If not, return BCM_E_PARAM.
         */
        entry_type = 
            soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, ENTRY_TYPEf);

        if (entry_type != 0x1) { /* != MPLS_MACDA_PROFILE */
            return BCM_E_PARAM;
        }

        /* Retrieve VC_AND_SWAP_INDEX */
        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
             vc_swap_index = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       MPLS__VC_AND_SWAP_INDEXf);
        } else {
             vc_swap_index = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       VC_AND_SWAP_INDEXf);
        }

        if (vc_swap_index == 0) {
 
            BCM_IF_ERROR_RETURN(
                _bcm_tr_mpls_get_vc_and_swap_table_index(unit, 0, 
                    NULL, NULL, egr, _BCM_MPLS_ACTION_PUSH, 
                    &vc_swap_index));
        } else {
            /* Retrieve VC_AND_SWAP_TABLE entry */
            BCM_IF_ERROR_RETURN(
                soc_mem_read(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                     MEM_BLOCK_ALL, vc_swap_index, &vc_entry));
#if defined(BCM_TRIUMPH3_SUPPORT)
            if (soc_feature(unit, soc_feature_vc_and_swap_table_overlaid)) {
                BCM_IF_ERROR_RETURN(
                    _bcm_tr_mpls_vc_and_swap_hash_nhop_delete(unit,
                                                              vc_swap_index,
                                                              nh_index));
            }
#endif
            /* Check if any VC_AND_SWAP_LABEL related info has changed */
            if (BCM_FAILURE(_bcm_tr_mpls_vc_swap_compare
                (unit, NULL, NULL, egr, _BCM_MPLS_ACTION_PUSH, &vc_entry))) {

                _bcm_tr_mpls_vc_and_swap_ref_count_get(unit, vc_swap_index, &ref_count);

                /* Check if any other Egress objects shares the same vc_swap_index */
                if (ref_count > 1) {
                    _bcm_tr_mpls_vc_and_swap_ref_count_adjust (unit, vc_swap_index, -1);

                    /* Entry sharing can't be possible, create a new index */
                    BCM_IF_ERROR_RETURN(
                        _bcm_tr_mpls_get_vc_and_swap_table_index(unit, 0,
                            NULL, NULL, egr, _BCM_MPLS_ACTION_PUSH,
                            &vc_swap_index));
                } else {
                    /* delete the sw state for old entry */
                    BCM_IF_ERROR_RETURN(bcmi_tr_mpls_vc_swap_hash_index_get(
                        unit, NULL, 0, NULL, NULL, &vc_entry, &bkt_idx));

                    hash_input = &(bcmi_vc_swap_label_hash[unit][bkt_idx]);
                    BCM_IF_ERROR_RETURN(bcmi_tr_mpls_vc_swap_hash_delete(
                        unit, hash_input, vc_swap_index));

                    /* Create the sw state for new entry */
                    BCM_IF_ERROR_RETURN(bcmi_tr_mpls_vc_swap_hash_index_get(
                        unit, NULL, _BCM_MPLS_ACTION_PUSH, NULL, egr, NULL, &bkt_idx));

                    hash_input = &(bcmi_vc_swap_label_hash[unit][bkt_idx]);
                    BCM_IF_ERROR_RETURN(bcmi_tr_mpls_vc_swap_hash_insert(unit, hash_input,
                        vc_swap_index, NULL, NULL, egr, _BCM_MPLS_ACTION_PUSH,
                            &vc_swap_index, FALSE));
                }
            }
        }
    }else if ((entry_type == 1) && (egr->mpls_label == BCM_MPLS_LABEL_INVALID) && (flags & BCM_L3_REPLACE)) {
        rv = bcm_tr_mpls_l3_label_delete (unit, nh_index);
        return rv;
    } else {

        BCM_IF_ERROR_RETURN(
            _bcm_tr_mpls_get_vc_and_swap_table_index(unit, 0,
                NULL, NULL, egr, _BCM_MPLS_ACTION_PUSH, &vc_swap_index));
    }
    /* Retrieve hardware index for mapping pointer */
    if ((egr->mpls_flags & BCM_MPLS_EGRESS_LABEL_EXP_SET) ||
        (egr->mpls_flags & BCM_MPLS_EGRESS_LABEL_PRI_SET)) {
        /* Mapping pointer not used */
        hw_map_idx = -1;
    } else {
        num_mpls_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 64;
        rv = _egr_qos_id2hw_idx(unit,egr->mpls_qos_map_id,&hw_map_idx);
        if ((rv != BCM_E_NONE) || hw_map_idx < 0 || hw_map_idx >= num_mpls_map) {
            if (egr->mpls_flags & BCM_MPLS_EGRESS_LABEL_EXP_REMARK) {
                rv = BCM_E_PARAM;
                goto cleanup;
            } else {
                hw_map_idx = 0; /* use default */
            }
        }
    }

    /* Program the MPLS_VC_AND_SWAP table entry */
    rv = _bcm_tr_mpls_vc_and_swap_table_entry_set(unit, NULL, NULL, egr,
                                             _BCM_MPLS_ACTION_PUSH, &vc_entry, hw_map_idx);
    if (rv < 0){
       goto cleanup;
    }
#if defined(BCM_TRIUMPH2_SUPPORT) \
    ||  defined(BCM_ENDURO_SUPPORT) || defined(BCM_TRIDENT_SUPPORT)
    if (SOC_IS_TRIUMPH2(unit)
        || SOC_IS_ENDURO(unit) || SOC_IS_TRIDENT(unit)) {
        /* workaround a hardware issue which is corrected on newer chips
         * that the EGR_SD_TAG_CONTROL register influnces L3 MPLS tunnel's
         * TPID selection. This shouldn't happen since L3 MPLS 
         * isn't using SD_TAG.
         */
        soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                &vc_entry, SD_TAG_ACTION_IF_NOT_PRESENTf,
                                3);
    }
#endif
    rv = soc_mem_write(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                   MEM_BLOCK_ALL, vc_swap_index,
                   &vc_entry);
    if (rv < 0){
       goto cleanup;
    }
    soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, 
                        &egr_nh, ENTRY_TYPEf, 
                        entry_type);

    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
          soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                          &egr_nh, MPLS__VC_AND_SWAP_INDEXf,
                          vc_swap_index);
          /* Enable HG.L3_bit */
          soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                    &egr_nh, MPLS__HG_L3_OVERRIDEf, 0x1);
           soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                     &egr_nh, MPLS__HG_MODIFY_ENABLEf, 0x1);
    } else {
          soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                          &egr_nh, VC_AND_SWAP_INDEXf,
                          vc_swap_index);
          /* Enable HG.L3_bit */
          soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                    &egr_nh, HG_L3_OVERRIDEf, 0x1);
           soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                     &egr_nh, HG_MODIFY_ENABLEf, 0x1);
    }
 #if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_mpls_nh_ttl_control)) {
        if (egr->flags & BCM_L3_KEEP_TTL) {
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                MPLS__DISABLE_TTL_DECREMENTf, 1);
        } else {
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                MPLS__DISABLE_TTL_DECREMENTf, 0);
        }
    }
#endif
    rv = soc_mem_write(unit, EGR_L3_NEXT_HOPm,
                       MEM_BLOCK_ALL, nh_index, &egr_nh);

    if (rv < 0) {
       goto cleanup;
    }
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_vc_and_swap_table_overlaid)) {
        rv = _bcm_tr_mpls_vc_and_swap_hash_nhop_add(unit,
                                                    vc_swap_index,
                                                    nh_index);
        if (rv < 0) {
            goto cleanup;
        }
    }
#endif
#ifdef BCM_WARM_BOOT_SUPPORT
    SOC_CONTROL_LOCK(unit);
    SOC_CONTROL(unit)->scache_dirty = 1;
    SOC_CONTROL_UNLOCK(unit);
#endif

    return rv;

cleanup:
    if (vc_swap_index != -1) {
        if (!(flags & BCM_L3_REPLACE)) {
            _bcm_tr_mpls_vc_and_swap_ref_count_adjust (unit, vc_swap_index, -1);
            /* Free VC_AND_SWAP_LABEL entry */
            (void)_bcm_tr_mpls_vc_and_swap_table_index_reset (unit, vc_swap_index);
        }
    }
    return rv; 
}

/*
 * Function:
 *		bcm_tr_mpls_l3_label_get
 * Purpose:
 *		Retrieves the  Inner_LABEL within VC_AND_SWAP_TABLE for a specific EGR_NH Index
 * Parameters:
 *		IN :  Unit
 *           IN :  mpls_label
 *           IN :  nh_index
 * Returns:
 *		BCM_E_XXX
 */

int
bcm_tr_mpls_l3_label_get (int unit, int index, bcm_l3_egress_t *egr)
{
    int rv=BCM_E_NONE;
    int  vc_swap_index=-1;
    egr_l3_next_hop_entry_t egr_nh;
    egr_mpls_vc_and_swap_label_table_entry_t vc_entry;
    uint32	entry_type;
    uint32  label_action;

    /* Retrieve EGR L3 NHOP Entry */
    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                                      index, &egr_nh));

    entry_type = 
        soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, ENTRY_TYPEf);

    if (entry_type != 0x1) { /* != MPLS_MACDA_PROFILE */
        egr->mpls_label = BCM_MPLS_LABEL_INVALID;
        return rv;
    }
  
    /* Retrieve VC_AND_SWAP_INDEX */
    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
         vc_swap_index = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       MPLS__VC_AND_SWAP_INDEXf);
    } else {
         vc_swap_index = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       VC_AND_SWAP_INDEXf);
    }
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_mpls_nh_ttl_control)) {
        if (soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                MPLS__DISABLE_TTL_DECREMENTf)) {
            egr->flags |= BCM_L3_KEEP_TTL;
        }
    }
#endif
    if (vc_swap_index != -1) {
        /* Retrieve VC_AND_SWAP_TABLE entry */
        BCM_IF_ERROR_RETURN( 
            soc_mem_read(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                         MEM_BLOCK_ALL, vc_swap_index, &vc_entry));
        label_action = 
            soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                &vc_entry, MPLS_LABEL_ACTIONf);
#if defined(BCM_SWAP_TO_SELF_SUPPORT)
        if (soc_feature(unit, soc_feature_mpls_swap_label_preserve) && 
            (label_action == _BCM_MPLS_ACTION_NOOP)) {
            return rv;
        }
#endif
        if (label_action == _BCM_MPLS_ACTION_PUSH) {
            egr->flags |= BCM_L3_ROUTE_LABEL;
            egr->mpls_label = 
                soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    &vc_entry, MPLS_LABELf);
            egr->mpls_ttl = 
                soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    &vc_entry, MPLS_TTLf);
            if (egr->mpls_ttl) {
                egr->mpls_flags |= BCM_MPLS_EGRESS_LABEL_TTL_SET;
            }
#if defined(BCM_TOMAHAWK2_SUPPORT)
        if (soc_feature(unit, soc_feature_mpls_ecn)) {
            int ecn_map_id;
            int ecn_map_pri;
            int ecn_map_hw_idx;
            int ret;
            ecn_map_hw_idx = soc_mem_field32_get(unit, 
                                                 EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                                 &vc_entry, 
                                                 IP_ECN_TO_EXP_MAPPING_PTRf);
            ecn_map_pri = soc_mem_field32_get(unit, 
                                              EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                              &vc_entry, 
                                              IP_ECN_TO_EXP_PRIORITYf); 
            ret =  bcmi_ecn_map_hw_idx2id(unit, ecn_map_hw_idx, 
                                          _BCM_XGS5_MPLS_ECN_MAP_TYPE_ECN2EXP, 
                                          &ecn_map_id); 
            if (BCM_SUCCESS(ret)) {
                egr->mpls_flags |= BCM_MPLS_EGRESS_LABEL_ECN_TO_EXP_MAP;
                egr->mpls_ecn_map_id = ecn_map_id;
                if (ecn_map_pri) {
                    egr->mpls_flags |= BCM_MPLS_EGRESS_LABEL_ECN_EXP_MAP_TRUST;
                }
            }
        }
#endif 
        } else {
            egr->mpls_label = BCM_MPLS_LABEL_INVALID;
        }
    } else {
        egr->mpls_label = BCM_MPLS_LABEL_INVALID;
    }
   
    return rv;
}

/*
 * Function:
 *		bcm_tr_mpls_l3_label_delete
 * Purpose:
 *		Deletes  Inner_LABEL within VC_AND_SWAP_TABLE for a specific EGR_NH Index
 * Parameters:
 *		IN :  Unit
 *           IN :  nh_index
 * Returns:
 *		BCM_E_XXX
 */


int 
bcm_tr_mpls_l3_label_delete (int unit, int index)
{
    int rv=BCM_E_NONE;
    int  vc_swap_index=-1;
    egr_l3_next_hop_entry_t egr_nh;
    uint8  entry_type;
    int  macda_index=-1;

    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                                      index, &egr_nh));

    /* Retrieve  and Verify  ENTRY_TYPE */
    entry_type = 
        soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, ENTRY_TYPEf);

    if (entry_type != 0x1) { /* != MPLS_MACDA_PROFILE */
        return BCM_E_PARAM;
    }

    /* Set Entry_Type to Normal */
    entry_type = 0; /* Normal */
    soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                        ENTRY_TYPEf, entry_type);

    /* Delete MAC_DA_PROFILE_INDEX */
    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
        macda_index = 
              soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                         MPLS__MAC_DA_PROFILE_INDEXf);
    } else {
        macda_index = 
              soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                         MAC_DA_PROFILE_INDEXf);
    }

    if (macda_index != -1) {
        _bcm_mac_da_profile_entry_delete(unit, macda_index);
    }

    /* Retrieve VC_AND_SWAP_INDEX */
    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
         vc_swap_index = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       MPLS__VC_AND_SWAP_INDEXf);
    } else {
         vc_swap_index = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       VC_AND_SWAP_INDEXf);
    }

#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_vc_and_swap_table_overlaid)) {
        BCM_IF_ERROR_RETURN(
            _bcm_tr_mpls_vc_and_swap_hash_nhop_delete(unit,
                                                      vc_swap_index,
                                                      index));
    }
#endif

    _bcm_tr_mpls_vc_and_swap_ref_count_adjust (unit, vc_swap_index, -1);

    BCM_IF_ERROR_RETURN
         (_bcm_tr_mpls_vc_and_swap_table_index_reset (unit, vc_swap_index));

    vc_swap_index = 0;
    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
         soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                              &egr_nh, MPLS__VC_AND_SWAP_INDEXf,
                              vc_swap_index);
    } else {
         soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                              &egr_nh, VC_AND_SWAP_INDEXf,
                              vc_swap_index);
    }

    rv = soc_mem_write(unit, EGR_L3_NEXT_HOPm,
                       MEM_BLOCK_ALL, index, &egr_nh);

#ifdef BCM_WARM_BOOT_SUPPORT
    SOC_CONTROL_LOCK(unit);
    SOC_CONTROL(unit)->scache_dirty = 1;
    SOC_CONTROL_UNLOCK(unit);
#endif

    return rv;
}

/*
 * Function:
 *      bcm_tr_mpls_swap_nh_info_add
 * Purpose:
 *       Add  SWAP_LABEL within VC_AND_SWAP_TABLE for a specific EGR_NH Index
 * Parameters:
 *           IN :  Unit
 *           IN : Egress object Pointer
 *           IN :  nh_index
 * Returns:
 *           BCM_E_XXX
 */

int
bcm_tr_mpls_swap_nh_info_add (int unit, bcm_l3_egress_t *egr, int nh_index, uint32 flags)
{
    ing_l3_next_hop_entry_t ing_nh;
    egr_l3_next_hop_entry_t egr_nh;
    egr_mpls_vc_and_swap_label_table_entry_t vc_entry, old_vc_entry;
    int vc_swap_index = -1;
    uint32 old_vc_swap_idx = -1;
    int rv, num_vc;
    int hw_map_idx, num_mpls_map;

    if (!(BCM_XGS3_L3_MPLS_LBL_VALID(egr->mpls_label) ||
        _BCM_MPLS_EGRESS_LABEL_PRESERVE(unit, egr->mpls_flags))) {
        return BCM_E_PARAM;
    }
    sal_memset(&egr_nh, 0, sizeof(egr_l3_next_hop_entry_t));

    BCM_IF_ERROR_RETURN (soc_mem_read(unit, ING_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                                      nh_index, &ing_nh));
    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                                      nh_index, &egr_nh));

    /*
     * Indexes in the first half of EGR_MPLS_VC_AND_SWAP_LABEL_TABLE
     * also get used as the index into the EGR_PW_INIT_COUNTERS table.
     * Set num_vc to half the EGR_MPLS_VC_AND_SWAP_LABEL_TABLE size.
     * Choose from the non-counted section (second half) so the counted
     * section is available for VPWS.
     */
    num_vc = soc_mem_index_count(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);
    num_vc /= 2;

    /* Check for egr->flags & BCM_L3_REPLACE */
    if (flags & BCM_L3_REPLACE) {
       /* Remember old  VC_AND_SWAP_INDEX */
        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
          old_vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                     MPLS__VC_AND_SWAP_INDEXf);
        } else {
            old_vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                    VC_AND_SWAP_INDEXf);
        }
        BCM_IF_ERROR_RETURN (soc_mem_read(unit,
                    EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                    MEM_BLOCK_ANY, old_vc_swap_idx, &old_vc_entry));
        if (!(egr->mpls_flags & BCM_MPLS_EGRESS_LABEL_TTL_SET)) {
            egr->mpls_ttl = soc_mem_field32_get(unit,
                    EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                    &old_vc_entry, MPLS_TTLf);
            if (egr->mpls_ttl) {
                egr->mpls_flags |= BCM_MPLS_EGRESS_LABEL_TTL_SET;
            }
        }
    }
    rv =  _bcm_tr_mpls_get_vc_and_swap_table_index (unit, 0, NULL, NULL,
              egr, _BCM_MPLS_EGRESS_LABEL_PRESERVE(unit, egr->mpls_flags) ?
              _BCM_MPLS_ACTION_PRESERVED : _BCM_MPLS_ACTION_SWAP,
              &vc_swap_index);
    _BCM_MPLS_CLEANUP(rv);
    /* Retrieve hardware index for mapping pointer */
    if ((egr->mpls_flags & BCM_MPLS_EGRESS_LABEL_EXP_SET) ||
        (egr->mpls_flags & BCM_MPLS_EGRESS_LABEL_PRI_SET)) {
        /* Mapping pointer not used */
        hw_map_idx = -1;
    } else {
        rv = _egr_qos_id2hw_idx(unit,egr->mpls_qos_map_id,&hw_map_idx);
        num_mpls_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 64;
        if ((rv != BCM_E_NONE) || hw_map_idx < 0 || hw_map_idx >= num_mpls_map) {
            if (egr->mpls_flags & BCM_MPLS_EGRESS_LABEL_EXP_REMARK) {
                rv = BCM_E_PARAM;
                goto cleanup;
            } else {
                hw_map_idx = 0; /* use default */
            }
        }
    }
    rv = _bcm_tr_mpls_vc_and_swap_table_entry_set(unit, NULL, NULL, egr,
             _BCM_MPLS_EGRESS_LABEL_PRESERVE(unit, egr->mpls_flags) ?
             _BCM_MPLS_ACTION_PRESERVED : _BCM_MPLS_ACTION_SWAP,
             &vc_entry, hw_map_idx);
    _BCM_MPLS_CLEANUP(rv);
#if defined(BCM_TRIUMPH2_SUPPORT) \
    ||  defined(BCM_ENDURO_SUPPORT) || defined(BCM_TRIDENT_SUPPORT)
    if (SOC_IS_TRIUMPH2(unit)
        || SOC_IS_ENDURO(unit) || SOC_IS_TRIDENT(unit)) {
        /* workaround a hardware issue which is corrected on newer chips
         * that the EGR_SD_TAG_CONTROL register influnces L3 MPLS tunnel's
         * TPID selection. This shouldn't happen since L3 MPLS 
         * isn't using SD_TAG.
         */
        soc_mem_field32_set(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                &vc_entry, SD_TAG_ACTION_IF_NOT_PRESENTf,
                                3);
    }
#endif
    rv = soc_mem_write(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                       MEM_BLOCK_ALL, vc_swap_index,
                       &vc_entry);
    _BCM_MPLS_CLEANUP(rv);
    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
         soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                              &egr_nh, MPLS__VC_AND_SWAP_INDEXf,
                              vc_swap_index);
         /* Enable HG.L3_bit */
         soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                &egr_nh, MPLS__HG_L3_OVERRIDEf, 0x1);
         soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                &egr_nh, MPLS__HG_MODIFY_ENABLEf, 0x1);
    } else {
         soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                              &egr_nh, VC_AND_SWAP_INDEXf,
                              vc_swap_index);
         /* Enable HG.L3_bit */
         soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                &egr_nh, HG_L3_OVERRIDEf, 0x1);
         soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                &egr_nh, HG_MODIFY_ENABLEf, 0x1);
    }
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_mpls_nh_ttl_control)) {
        if (egr->flags & BCM_L3_KEEP_TTL) {
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                MPLS__DISABLE_TTL_DECREMENTf, 1);
        } else {
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                MPLS__DISABLE_TTL_DECREMENTf, 0);
        }
    }
#endif

    /* Write EGR_L3_NEXT_HOP entry */
    rv = soc_mem_write(unit, EGR_L3_NEXT_HOPm,
                       MEM_BLOCK_ALL, nh_index, &egr_nh);
    _BCM_MPLS_CLEANUP(rv);

    /* Write ING_L3_NEXT_HOP entry */
    
    soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                        &ing_nh, ENTRY_TYPEf, 0x1); /* MPLS L3_OIF */
    rv = soc_mem_write (unit, ING_L3_NEXT_HOPm,
                        MEM_BLOCK_ALL, nh_index, &ing_nh);
    _BCM_MPLS_CLEANUP(rv);

    /* Handle old VC_SWAP indexes */
    if (old_vc_swap_idx != -1) {
#if defined(BCM_TRIUMPH3_SUPPORT)
         if (soc_feature(unit, soc_feature_vc_and_swap_table_overlaid)) {
             BCM_IF_ERROR_RETURN(
                 _bcm_tr_mpls_vc_and_swap_hash_nhop_delete(unit,
                                                           old_vc_swap_idx,
                                                           nh_index));
         }
#endif
         _bcm_tr_mpls_vc_and_swap_ref_count_adjust (unit, old_vc_swap_idx, -1);
         BCM_IF_ERROR_RETURN
             (_bcm_tr_mpls_vc_and_swap_table_index_reset (unit, old_vc_swap_idx));
    }
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_vc_and_swap_table_overlaid)) {
        rv = _bcm_tr_mpls_vc_and_swap_hash_nhop_add(unit,
                                                    vc_swap_index,
                                                    nh_index);
        _BCM_MPLS_CLEANUP(rv);
    }
#endif
#ifdef BCM_WARM_BOOT_SUPPORT
    SOC_CONTROL_LOCK(unit);
    SOC_CONTROL(unit)->scache_dirty = 1;
    SOC_CONTROL_UNLOCK(unit);
#endif

    return rv;

  cleanup:

    if (vc_swap_index != -1) {
         _bcm_tr_mpls_vc_and_swap_ref_count_adjust (unit, vc_swap_index, -1);
         (void)_bcm_tr_mpls_vc_and_swap_table_index_reset (unit, vc_swap_index);
    }
    return rv;
}

/*
 * Function:
 *		bcm_tr_mpls_swap_nh_info_delete
 * Purpose:
 *		Delete  SWAP_LABEL within VC_AND_SWAP_TABLE for a specific EGR_NH Index
 * Parameters:
 *		IN   :  Unit
 *           IN    :  nh_index
 * Returns:
 *		BCM_E_XXX
 */

int
bcm_tr_mpls_swap_nh_info_delete(int unit, int nh_index)
{
    int rv = BCM_E_NONE, old_vc_swap_idx = -1;
    ing_l3_next_hop_entry_t ing_nh;
    egr_l3_next_hop_entry_t egr_nh;
    int intf;
    int tunnel_id;

    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, 
                                      MEM_BLOCK_ANY, nh_index, &egr_nh));
    BCM_IF_ERROR_RETURN (soc_mem_read(unit, ING_L3_NEXT_HOPm, 
                                      MEM_BLOCK_ANY, nh_index, &ing_nh));

    if (soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, ENTRY_TYPEf) != 0x1) {
        return BCM_E_NONE;
    }

    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
         old_vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       MPLS__VC_AND_SWAP_INDEXf);
         intf = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                 MPLS__INTF_NUMf);
    } else {
         old_vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       VC_AND_SWAP_INDEXf);
         intf = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                 INTF_NUMf);
    }

    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
         soc_EGR_L3_NEXT_HOPm_field32_set(unit, &egr_nh,
                                          MPLS__VC_AND_SWAP_INDEXf,
                                          0);
    } else {
         soc_EGR_L3_NEXT_HOPm_field32_set(unit, &egr_nh,
                                          VC_AND_SWAP_INDEXf,
                                          0);
    }
    BCM_IF_ERROR_RETURN (soc_mem_write(unit, EGR_L3_NEXT_HOPm,
                                       MEM_BLOCK_ALL, nh_index, &egr_nh));
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_vc_and_swap_table_overlaid)) {
        BCM_IF_ERROR_RETURN (
            _bcm_tr_mpls_vc_and_swap_hash_nhop_delete(unit,
                                                      old_vc_swap_idx,
                                                      nh_index));
    }
#endif

    /* clear VC_AND_SWAP_LABEL entry */
    _bcm_tr_mpls_vc_and_swap_ref_count_adjust (unit, old_vc_swap_idx, -1);

    /* Free VC_AND_SWAP_LABEL entry */
    BCM_IF_ERROR_RETURN
         (_bcm_tr_mpls_vc_and_swap_table_index_reset (unit, old_vc_swap_idx));

    BCM_IF_ERROR_RETURN
        (_bcm_xgs3_l3_get_tunnel_id(unit, intf, &tunnel_id));
    if (!tunnel_id) {

        soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                            &ing_nh, ENTRY_TYPEf, 0x0); 
        rv = soc_mem_write (unit, ING_L3_NEXT_HOPm,
                            MEM_BLOCK_ALL, nh_index, &ing_nh);
    } 

#ifdef BCM_WARM_BOOT_SUPPORT
    SOC_CONTROL_LOCK(unit);
    SOC_CONTROL(unit)->scache_dirty = 1;
    SOC_CONTROL_UNLOCK(unit);
#endif

    return rv;
}

/*
 * Function:
 *        bcm_tr_mpls_swap_nh_info_get
 * Purpose:
 *        Get  Label_Swap info within Egress Object for a specific EGR_NH Index
 * Parameters:
 *           IN :  Unit
 *           OUT : Egress object Pointer
 *           IN :  nh_index
 * Returns:
 *           BCM_E_XXX
 */

int
bcm_tr_mpls_swap_nh_info_get(int unit, bcm_l3_egress_t *egr, int nh_index)
{
    egr_l3_next_hop_entry_t egr_nh;
    egr_mpls_vc_and_swap_label_table_entry_t vc_entry;
    int vc_swap_idx, ix;
    uint32	 label_action;

    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                                      nh_index, &egr_nh));

    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
         vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       MPLS__VC_AND_SWAP_INDEXf);
    } else {
         vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       VC_AND_SWAP_INDEXf);
    }

    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                      MEM_BLOCK_ANY, vc_swap_idx, &vc_entry));

    label_action =  soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                        &vc_entry, MPLS_LABEL_ACTIONf);

    if (label_action == _BCM_MPLS_ACTION_SWAP ||
        (soc_feature(unit, soc_feature_mpls_swap_label_preserve) &&
        label_action == _BCM_MPLS_ACTION_PRESERVED)) { /* SWAP */

        if (soc_feature(unit, soc_feature_mpls_swap_label_preserve) &&
            label_action == _BCM_MPLS_ACTION_PRESERVED) {
            egr->mpls_flags |= BCM_MPLS_EGRESS_LABEL_PRESERVE;
            egr->mpls_label = BCM_MPLS_LABEL_INVALID;
        } else {
            egr->mpls_label =
            soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                &vc_entry, MPLS_LABELf);
        }
        egr->mpls_ttl =
            soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                &vc_entry, MPLS_TTLf);
        if (egr->mpls_ttl) {
            egr->mpls_flags |= BCM_MPLS_EGRESS_LABEL_TTL_SET;
        } else {
            egr->mpls_flags |= BCM_MPLS_EGRESS_LABEL_TTL_DECREMENT;
        }
        if (soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                &vc_entry, MPLS_EXP_SELECTf) == 0x0) {
            /* Use the specified EXP, PRI and CFI */
            egr->mpls_flags |= BCM_MPLS_EGRESS_LABEL_EXP_SET;
            egr->mpls_flags |= BCM_MPLS_EGRESS_LABEL_PRI_SET;
            egr->mpls_exp = 
                soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    &vc_entry, MPLS_EXPf);
            egr->mpls_pkt_pri = 
                soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    &vc_entry, NEW_PRIf);
            egr->mpls_pkt_cfi = 
                soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    &vc_entry, NEW_CFIf);
        } else if (soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                       &vc_entry, MPLS_EXP_SELECTf) == 0x1) {
            /* Use EXP-map for EXP, PRI and CFI */
            egr->mpls_flags |= BCM_MPLS_EGRESS_LABEL_EXP_REMARK;
            egr->mpls_flags |= BCM_MPLS_EGRESS_LABEL_PRI_REMARK;
            ix = soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                     &vc_entry, MPLS_EXP_MAPPING_PTRf);
                BCM_IF_ERROR_RETURN(
                    _egr_qos_hw_idx2id(unit, ix, &egr->mpls_qos_map_id));
        } else if (soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                       &vc_entry, MPLS_EXP_SELECTf) == 0x3) {
            /* Use EXP from incoming MPLS label. If no incoming label, use the specified
             * EXP value. Use EXP-map for PRI/CFI.
             */
            egr->mpls_flags |= BCM_MPLS_EGRESS_LABEL_EXP_COPY;
            egr->mpls_flags |= BCM_MPLS_EGRESS_LABEL_PRI_REMARK;
            egr->mpls_exp = 
                soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                    &vc_entry, MPLS_EXPf);
            ix = soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                     &vc_entry, MPLS_EXP_MAPPING_PTRf);
                BCM_IF_ERROR_RETURN(
                    _egr_qos_hw_idx2id(unit, ix, &egr->mpls_qos_map_id));
        }
#if defined(BCM_TOMAHAWK2_SUPPORT)
        if (soc_feature(unit, soc_feature_mpls_ecn)) {
            int ecn_map_id;
            int ecn_map_pri;
            int ecn_map_hw_idx;
            int rv;
            ecn_map_hw_idx = soc_mem_field32_get(unit, 
                                EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, &vc_entry, 
                                INT_CN_TO_EXP_MAPPING_PTRf);
            ecn_map_pri = soc_mem_field32_get(unit, 
                              EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, &vc_entry, 
                              INT_CN_TO_EXP_PRIORITYf);
            rv = bcmi_ecn_map_hw_idx2id(unit, ecn_map_hw_idx, 
                                        _BCM_XGS5_MPLS_ECN_MAP_TYPE_INTCN2EXP, 
                                        &ecn_map_id); 
            if (BCM_SUCCESS(rv)) {
                egr->mpls_flags |= BCM_MPLS_EGRESS_LABEL_INT_CN_TO_EXP_MAP;
                egr->mpls_ecn_map_id = ecn_map_id;
                if (ecn_map_pri) {
                    egr->mpls_flags |= BCM_MPLS_EGRESS_LABEL_ECN_EXP_MAP_TRUST;
                }
            }
        }
#endif
    } else {
        egr->mpls_label = BCM_MPLS_LABEL_INVALID;
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *        bcm_tr_mpls_l3_nh_info_add
 * Purpose:
 *        Add  SWAP_LABEL within VC_AND_SWAP_TABLE for a specific EGR_NH Index
 * Parameters:
 *           IN :  Unit
 *           IN/OUT : Tunnel Switch Pointer
 *           IN :  nh_index
 * Returns:
 *           BCM_E_XXX
 */

int
bcm_tr_mpls_l3_nh_info_add(int unit, bcm_mpls_tunnel_switch_t *info, int *nh_index)
{
    initial_ing_l3_next_hop_entry_t initial_ing_nh;
    ing_l3_next_hop_entry_t ing_nh;
    egr_l3_next_hop_entry_t egr_nh;
    egr_mac_da_profile_entry_t macda;
    egr_mpls_vc_and_swap_label_table_entry_t vc_entry;
    _bcm_tr_ing_nh_info_t ing_nh_info;
    _bcm_tr_egr_nh_info_t egr_nh_info;
    bcm_l3_egress_t nh_info, nh_obj;
    bcm_module_t mod_out;
    bcm_port_t port_out;
    uint32 nh_flags, macda_index;
    int rv, temp_nh_index = -1, num_vc=0;
    void *entries[1];
    int num_mpls_map, hw_map_idx;
    uint32 mpath_flag=0;

    /* Initialize values */
    if (info->mtu != 0) {
        ing_nh_info.mtu = info->mtu;
    }  else {
        ing_nh_info.mtu = 0x3FFF;
    }

    ing_nh_info.port = -1;
    ing_nh_info.module = -1;
    ing_nh_info.trunk = -1;
    ing_nh_info.qtag = -1;
    ing_nh_info.tag_type = -1;

    egr_nh_info.dvp = -1; /* Not applicable */
    egr_nh_info.dvp_is_network = -1; /* Not applicable */
    egr_nh_info.entry_type = -1;
    egr_nh_info.sd_tag_action_present = -1;
    egr_nh_info.sd_tag_action_not_present = -1;
    egr_nh_info.intf_num = -1;
    egr_nh_info.sd_tag_vlan = -1;
    egr_nh_info.macda_index = -1;
    egr_nh_info.vc_swap_index = -1;
    egr_nh_info.pw_init_cnt = -1;


    /* 
     * Get egress next-hop index from egress object and
     * increment egress object reference count. 
     */
    rv = bcm_xgs3_get_nh_from_egress_object(unit, info->egress_if,
                                            &mpath_flag, 1, &temp_nh_index);
    _BCM_MPLS_CLEANUP(rv);
    bcm_l3_egress_t_init(&nh_info);


    /* Get egress next-hop info from the egress object */
    rv = bcm_xgs3_nh_get(unit, temp_nh_index, &nh_info);
    _BCM_MPLS_CLEANUP(rv);

#if defined(BCM_KATANA_SUPPORT)
    if (SOC_IS_KATANAX(unit)) {
        ing_l3_next_hop_entry_t in_nh;

        rv = soc_mem_read (unit, ING_L3_NEXT_HOPm,
                    MEM_BLOCK_ALL, temp_nh_index, &in_nh);
        _BCM_MPLS_CLEANUP(rv);
        ing_nh_info.qtag = soc_ING_L3_NEXT_HOPm_field32_get(unit, &in_nh,
                                      EH_QUEUE_TAGf);
        ing_nh_info.tag_type = soc_ING_L3_NEXT_HOPm_field32_get(unit,
                                      &in_nh,
                                      EH_TAG_TYPEf);
    }
#endif

    /* Check if Egress Object contains a valid SWAP Label */
    if (BCM_XGS3_L3_MPLS_LBL_VALID(nh_info.mpls_label)) {
        return BCM_E_CONFIG;
    }

    /*
     * Indexes in the first half of EGR_MPLS_VC_AND_SWAP_LABEL_TABLE
     * also get used as the index into the EGR_PW_INIT_COUNTERS table.
     * Set num_vc to half the EGR_MPLS_VC_AND_SWAP_LABEL_TABLE size.
     * Choose from the non-counted section (second half) so the counted
     * section is available for VPWS.
     */
    num_vc = soc_mem_index_count(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);
    num_vc /= 2;

    /*
     * For SWAP operation, we need to allocate a unique next-hop
     * index and populate the swap label info. Copy the destination
     * port information from the egress object.
     * 
     * Allocate a next-hop entry. By calling bcm_xgs3_nh_add()
     * with _BCM_L3_SHR_WRITE_DISABLE flag, a next-hop index is
     * allocated but nothing is written to hardware. The "nh_info"
     * in this case is not used, so just set to all zeros.
     */
    bcm_l3_egress_t_init(&nh_obj);

    nh_flags = _BCM_L3_SHR_MATCH_DISABLE | _BCM_L3_SHR_WRITE_DISABLE;
    BCM_IF_ERROR_RETURN(bcm_xgs3_nh_add(unit, nh_flags, &nh_obj, nh_index));

    rv = _bcm_tr_mpls_add_egrNhop (unit, info->egress_if);
    _BCM_MPLS_CLEANUP(rv);

    rv = _bcm_tr_mpls_add_vp_nh (unit, info->egress_if, *nh_index);
    _BCM_MPLS_CLEANUP(rv);

    /* Copy the destination port information. */
    if (nh_info.flags & BCM_L3_TGID) {
        ing_nh_info.port = -1;
        ing_nh_info.module = -1;
        ing_nh_info.trunk = nh_info.trunk; 
    } else {
        BCM_IF_ERROR_RETURN
            (_bcm_esw_stk_modmap_map(unit, BCM_STK_MODMAP_SET, nh_info.module, nh_info.port, 
                                    &mod_out, &port_out));
        ing_nh_info.port = port_out;
        ing_nh_info.module = mod_out;
        ing_nh_info.trunk = -1; 
    }
    egr_nh_info.intf_num = nh_info.intf;
    egr_nh_info.entry_type = 0x1; /* MPLS_MACDA_PROFILE */

    sal_memset(&macda, 0, sizeof(egr_mac_da_profile_entry_t));
    soc_mem_mac_addr_set(unit, EGR_MAC_DA_PROFILEm, 
                         &macda, MAC_ADDRESSf, nh_info.mac_addr);
    entries[0] = &macda;
    rv = _bcm_mac_da_profile_entry_add(unit, entries, 1, &macda_index);
    egr_nh_info.macda_index = macda_index;
    _BCM_MPLS_CLEANUP(rv);
    
    rv = _bcm_tr_mpls_get_vc_and_swap_table_index(unit, 0, NULL, info, NULL, 
             _BCM_MPLS_EGRESS_LABEL_PRESERVE(unit,info->egress_label.flags) ? 
             _BCM_MPLS_ACTION_PRESERVED : _BCM_MPLS_ACTION_SWAP, 
             &egr_nh_info.vc_swap_index);
    


    _BCM_MPLS_CLEANUP(rv);
    /* Retrieve hardware index for mapping pointer */
    num_mpls_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 64;

    if (info->egress_label.qos_map_id == 0) {
        hw_map_idx = 0; /* use default */
    } else {
        BCM_IF_ERROR_RETURN(_egr_qos_id2hw_idx(unit,
                        info->egress_label.qos_map_id,&hw_map_idx));
    }
    if (hw_map_idx < 0 || hw_map_idx >= num_mpls_map) {
        rv = BCM_E_PARAM;
        goto cleanup;
    }

    

    /* Program the MPLS_VC_AND_SWAP table entry */
    rv = _bcm_tr_mpls_vc_and_swap_table_entry_set(unit, NULL, info, NULL,
         _BCM_MPLS_EGRESS_LABEL_PRESERVE(unit, info->egress_label.flags) ?
         _BCM_MPLS_ACTION_PRESERVED : _BCM_MPLS_ACTION_SWAP,
         &vc_entry, hw_map_idx);
     _BCM_MPLS_CLEANUP(rv);

    rv = soc_mem_write(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                       MEM_BLOCK_ALL, egr_nh_info.vc_swap_index,
                       &vc_entry);
    _BCM_MPLS_CLEANUP(rv);
    /* Write EGR_L3_NEXT_HOP entry */
    sal_memset(&egr_nh, 0, sizeof(egr_l3_next_hop_entry_t));
   if (egr_nh_info.entry_type == 0x1) {
         soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &egr_nh, INTF_NUMf, egr_nh_info.intf_num);

         if (egr_nh_info.vc_swap_index != -1) {
             if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                  &egr_nh, MPLS__VC_AND_SWAP_INDEXf,
                                  egr_nh_info.vc_swap_index);
             } else {
                  soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                                  &egr_nh, VC_AND_SWAP_INDEXf,
                                  egr_nh_info.vc_swap_index);
             }
         }

        if (egr_nh_info.macda_index != -1) {
              if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                   soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                            &egr_nh, MPLS__MAC_DA_PROFILE_INDEXf,
                            egr_nh_info.macda_index);
              } else {
                   soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                             &egr_nh, MAC_DA_PROFILE_INDEXf,
                             egr_nh_info.macda_index);
              }
        }

        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
           soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                    &egr_nh, MPLS__HG_MODIFY_ENABLEf, 0x1);
           /* Enable HG.L3_bit */
           soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                    &egr_nh, MPLS__HG_L3_OVERRIDEf, 0x1);

        } else {
           soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                     &egr_nh, HG_MODIFY_ENABLEf, 0x1);
           /* Enable HG.L3_bit */
           soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                    &egr_nh, HG_L3_OVERRIDEf, 0x1);
        }
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit, soc_feature_mpls_nh_ttl_control)) {
            if (info->flags & BCM_MPLS_SWITCH_KEEP_TTL) {
                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                    MPLS__DISABLE_TTL_DECREMENTf, 1);
            }
        }
#endif
         soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                        ENTRY_TYPEf, egr_nh_info.entry_type);

         rv = soc_mem_write(unit, EGR_L3_NEXT_HOPm,
                       MEM_BLOCK_ALL, *nh_index, &egr_nh);
         _BCM_MPLS_CLEANUP(rv);
    }

    /* Write ING_L3_NEXT_HOP entry */
    sal_memset(&ing_nh, 0, sizeof(ing_l3_next_hop_entry_t));
    if (ing_nh_info.trunk == -1) {
        soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                            &ing_nh, PORT_NUMf, ing_nh_info.port);
        soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                            &ing_nh, MODULE_IDf, ing_nh_info.module);
    } else {    
        soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                            &ing_nh, Tf, 1);
        soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                            &ing_nh, TGIDf, ing_nh_info.trunk);
    }

    if ((ing_nh_info.qtag != -1) &&
        SOC_MEM_FIELD_VALID(unit, ING_L3_NEXT_HOPm,EH_QUEUE_TAGf)) {
        soc_mem_field32_set(unit, ING_L3_NEXT_HOPm, &ing_nh,
                      EH_QUEUE_TAGf, ing_nh_info.qtag);
    }
    if ((ing_nh_info.tag_type != -1) &&
        SOC_MEM_FIELD_VALID(unit, ING_L3_NEXT_HOPm,EH_TAG_TYPEf)) {
        soc_mem_field32_set(unit, ING_L3_NEXT_HOPm, &ing_nh,
                      EH_TAG_TYPEf, ing_nh_info.tag_type);
    }

    
    soc_mem_field32_set(unit, ING_L3_NEXT_HOPm,
                        &ing_nh, ENTRY_TYPEf, 0x1); /* MPLS L3_OIF */
    rv = soc_mem_write (unit, ING_L3_NEXT_HOPm,
                        MEM_BLOCK_ALL, *nh_index, &ing_nh);
    _BCM_MPLS_CLEANUP(rv);

    /* Write INITIAL_ING_L3_NEXT_HOP entry */
    sal_memset(&initial_ing_nh, 0, sizeof(initial_ing_l3_next_hop_entry_t));
    if (ing_nh_info.trunk == -1) {
        soc_mem_field32_set(unit, INITIAL_ING_L3_NEXT_HOPm,
                            &initial_ing_nh, PORT_NUMf, ing_nh_info.port);
        soc_mem_field32_set(unit, INITIAL_ING_L3_NEXT_HOPm,
                            &initial_ing_nh, MODULE_IDf, ing_nh_info.module);
    } else {
        soc_mem_field32_set(unit, INITIAL_ING_L3_NEXT_HOPm,
                            &initial_ing_nh, Tf, 1);
        soc_mem_field32_set(unit, INITIAL_ING_L3_NEXT_HOPm,
                            &initial_ing_nh, TGIDf, ing_nh_info.trunk);
    }
    rv = soc_mem_write(unit, INITIAL_ING_L3_NEXT_HOPm,
                       MEM_BLOCK_ALL, *nh_index, &initial_ing_nh);
    _BCM_MPLS_CLEANUP(rv);
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_vc_and_swap_table_overlaid)) {
        rv = _bcm_tr_mpls_vc_and_swap_hash_nhop_add(unit,
                                                    egr_nh_info.vc_swap_index,
                                                    *nh_index);
        _BCM_MPLS_CLEANUP(rv);
    }
#endif
    return rv;

  cleanup:
    (void) bcm_xgs3_nh_del(unit, _BCM_L3_SHR_WRITE_DISABLE, *nh_index);

    if (temp_nh_index != -1) {
        (void) bcm_xgs3_nh_del(unit, 0, temp_nh_index);
    }
    if (egr_nh_info.macda_index != -1) {
        _bcm_mac_da_profile_entry_delete(unit, egr_nh_info.macda_index);
    }
    if (egr_nh_info.vc_swap_index != -1) {
        _bcm_tr_mpls_vc_and_swap_ref_count_adjust (unit, 
                             egr_nh_info.vc_swap_index, -1);
        /* Free VC_AND_SWAP_LABEL entry */
        (void)_bcm_tr_mpls_vc_and_swap_table_index_reset (unit, 
                             egr_nh_info.vc_swap_index);
    }
    return rv;
}

/*
 * Function:
 *        bcm_tr_mpls_l3_nh_info_delete
 * Purpose:
 *        Delete  SWAP_LABEL within VC_AND_SWAP_TABLE for a specific EGR_NH Index
 * Parameters:
 *           IN :  Unit
 *           IN :  nh_index
 * Returns:
 *           BCM_E_XXX
 */

int
bcm_tr_mpls_l3_nh_info_delete(int unit, int nh_index)
{
    int rv, old_macda_idx = -1, old_vc_swap_idx = -1;
    bcm_if_t old_egress_if = 0;
    initial_ing_l3_next_hop_entry_t initial_ing_nh;
    ing_l3_next_hop_entry_t ing_nh;
    egr_l3_next_hop_entry_t egr_nh;

    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, 
                                      MEM_BLOCK_ANY, nh_index, &egr_nh));
    BCM_IF_ERROR_RETURN (soc_mem_read(unit, ING_L3_NEXT_HOPm, 
                                      MEM_BLOCK_ANY, nh_index, &ing_nh));
    BCM_IF_ERROR_RETURN (soc_mem_read(unit, INITIAL_ING_L3_NEXT_HOPm, 
                                      MEM_BLOCK_ANY, nh_index, &initial_ing_nh));

    if (soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, ENTRY_TYPEf) != 0x1) {
        return BCM_E_NOT_FOUND;
    }

    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
         old_macda_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                     MPLS__MAC_DA_PROFILE_INDEXf);
    } else {
         old_macda_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                     MAC_DA_PROFILE_INDEXf);
    }

    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
         old_vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       MPLS__VC_AND_SWAP_INDEXf);
    } else {
         old_vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       VC_AND_SWAP_INDEXf);
    }

    /* Find the tunnel_if */
    rv = bcm_tr_mpls_get_vp_nh (unit, (bcm_if_t) nh_index, &old_egress_if);
    BCM_IF_ERROR_RETURN (rv);

    /* Clear EGR_L3_NEXT_HOP entry */
    sal_memset(&egr_nh, 0, sizeof(egr_l3_next_hop_entry_t));
    BCM_IF_ERROR_RETURN (soc_mem_write(unit, EGR_L3_NEXT_HOPm,
                                       MEM_BLOCK_ALL, nh_index, &egr_nh));

    /* Clear ING_L3_NEXT_HOP entry */
    sal_memset(&ing_nh, 0, sizeof(ing_l3_next_hop_entry_t));
    BCM_IF_ERROR_RETURN (soc_mem_write (unit, ING_L3_NEXT_HOPm,
                                        MEM_BLOCK_ALL, nh_index, &ing_nh));

    /* Clear INITIAL_ING_L3_NEXT_HOP entry */
    sal_memset(&initial_ing_nh, 0, sizeof(initial_ing_l3_next_hop_entry_t));
    BCM_IF_ERROR_RETURN (soc_mem_write(unit, INITIAL_ING_L3_NEXT_HOPm,
                                       MEM_BLOCK_ALL, nh_index, &initial_ing_nh));

#if defined(BCM_TRIUMPH3_SUPPORT)
    if (soc_feature(unit, soc_feature_vc_and_swap_table_overlaid)) {
        BCM_IF_ERROR_RETURN(
            _bcm_tr_mpls_vc_and_swap_hash_nhop_delete(unit,
                                                      old_vc_swap_idx,
                                                      nh_index));
    }
#endif

    /* clear VC_AND_SWAP_LABEL entry */
    _bcm_tr_mpls_vc_and_swap_ref_count_adjust (unit, old_vc_swap_idx, -1);

    /* Free VC_AND_SWAP_LABEL entry */
    BCM_IF_ERROR_RETURN
         (_bcm_tr_mpls_vc_and_swap_table_index_reset (unit, old_vc_swap_idx));

    /* Delete old MAC profile reference */
    rv = _bcm_mac_da_profile_entry_delete(unit, old_macda_idx);
    BCM_IF_ERROR_RETURN(rv);

    /* Delete egress_if next-hop reference */
    rv = bcm_xgs3_nh_del(unit, 0, (old_egress_if - BCM_XGS3_EGRESS_IDX_MIN));
    BCM_IF_ERROR_RETURN(rv);

    BCM_IF_ERROR_RETURN(_bcm_tr_mpls_delete_vp_nh ( unit, nh_index, old_egress_if ));

    /* Free the next-hop entry. */
    rv = bcm_xgs3_nh_del(unit, _BCM_L3_SHR_WRITE_DISABLE, nh_index);
    return rv;
}

/*
 * Function:
 *        bcm_tr_mpls_l3_nh_info_get
 * Purpose:
 *        Get Tunnel_switch info for a specific EGR_NH Index
 * Parameters:
 *           IN :  Unit
 *           OUT : Tunnel switch object pointer
 *           IN :  nh_index
 * Returns:
 *           BCM_E_XXX
 */

int
bcm_tr_mpls_l3_nh_info_get(int unit, bcm_mpls_tunnel_switch_t *info, int nh_index)
{
    egr_l3_next_hop_entry_t egr_nh;
    egr_mpls_vc_and_swap_label_table_entry_t vc_entry;
    int vc_swap_idx, ix;
#ifdef BCM_TOMAHAWK2_SUPPORT
    int ecn_map_index;
    int ecn_map_pri;
#endif
    info->egress_if = nh_index + BCM_XGS3_EGRESS_IDX_MIN;

    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                                      nh_index, &egr_nh));

    if (soc_feature(unit, soc_feature_mpls_enhanced)) {
         vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       MPLS__VC_AND_SWAP_INDEXf);
    } else {
         vc_swap_idx = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                                       VC_AND_SWAP_INDEXf);
    }
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit, soc_feature_mpls_nh_ttl_control)) {
            if (soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                    MPLS__DISABLE_TTL_DECREMENTf)) {
                info->flags |= BCM_MPLS_SWITCH_KEEP_TTL;
            }
        }
#endif

    BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, 
                                      MEM_BLOCK_ANY, vc_swap_idx, &vc_entry));
    if (_BCM_MPLS_ACTION_PRESERVED == soc_mem_field32_get(unit, 
        EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm, &vc_entry, MPLS_LABEL_ACTIONf)) {
        info->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_PRESERVE;
        info->egress_label.label = BCM_MPLS_LABEL_INVALID;
    } else {
        info->egress_label.label = 
            soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                &vc_entry, MPLS_LABELf);
    }

    info->egress_label.ttl = 
        soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                            &vc_entry, MPLS_TTLf);
    if (info->egress_label.ttl) {
        info->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_TTL_SET;
    } else {
        info->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_TTL_DECREMENT;
    }
    if (soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                            &vc_entry, MPLS_EXP_SELECTf) == 0x0) {
        /* Use the specified EXP, PRI and CFI */
        info->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_EXP_SET;
        info->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_PRI_SET;
        info->egress_label.exp = 
            soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                &vc_entry, MPLS_EXPf);
        info->egress_label.pkt_pri = 
            soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                &vc_entry, NEW_PRIf);
        info->egress_label.pkt_cfi = 
            soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                &vc_entry, NEW_CFIf);
    } else if (soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                   &vc_entry, MPLS_EXP_SELECTf) == 0x1) {
        /* Use EXP-map for EXP, PRI and CFI */
        info->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_EXP_REMARK;
        info->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_PRI_REMARK;
        ix = soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                 &vc_entry, MPLS_EXP_MAPPING_PTRf);
                BCM_IF_ERROR_RETURN(
                    _egr_qos_hw_idx2id(unit, ix, &info->egress_label.qos_map_id));
    } else if (soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                   &vc_entry, MPLS_EXP_SELECTf) == 0x3) {
        /* Use EXP from incoming MPLS label. If no incoming label, use the specified
         * EXP value. Use EXP-map for PRI/CFI.
         */
        info->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_EXP_COPY;
        info->egress_label.flags |= BCM_MPLS_EGRESS_LABEL_PRI_REMARK;
        info->egress_label.exp = 
            soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                &vc_entry, MPLS_EXPf);
        ix = soc_mem_field32_get(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                 &vc_entry, MPLS_EXP_MAPPING_PTRf);
        BCM_IF_ERROR_RETURN(
                    _egr_qos_hw_idx2id(unit, ix, &info->egress_label.qos_map_id));
    }
#ifdef BCM_TOMAHAWK2_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_ecn)) {
        ecn_map_index = soc_mem_field32_get(unit, 
                                            EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                            &vc_entry, 
                                            INT_CN_TO_EXP_MAPPING_PTRf);
        if (bcmi_xgs5_ecn_map_used_get(unit, ecn_map_index, 
                                       _bcmEcnmapTypeIntcn2Exp)) {
            info->egress_label.int_cn_map_id = 
                _BCM_XGS5_MPLS_ECN_MAP_TYPE_INTCN2EXP | ecn_map_index;
            ecn_map_pri = soc_mem_field32_get(unit, 
                                              EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm,
                                              &vc_entry, 
                                              INT_CN_TO_EXP_PRIORITYf);
            if (ecn_map_pri) {
                info->egress_label.flags |= 
                    BCM_MPLS_EGRESS_LABEL_ECN_EXP_MAP_TRUST;
            }
            info->egress_label.flags |= 
                BCM_MPLS_EGRESS_LABEL_INT_CN_TO_EXP_MAP;
        }
    }
#endif /* BCM_TOMAHAWK2_SUPPORT */ 
    return BCM_E_NONE;
}

/* Convert key part of application format to HW entry. */
STATIC int
_bcm_tr_mpls_entry_set_key(int unit, bcm_mpls_tunnel_switch_t *info,
                           mpls_entry_entry_t *ment)
{
    bcm_module_t mod_out;
    bcm_port_t port_out;
    bcm_trunk_t trunk_id;
    int rv, gport_id;

    sal_memset(ment, 0, sizeof(mpls_entry_entry_t));

    if (info->port == BCM_GPORT_INVALID) {
        /* Global label, mod/port not part of lookup key */
        soc_MPLS_ENTRYm_field32_set(unit, ment, MODULE_IDf, 0);
        soc_MPLS_ENTRYm_field32_set(unit, ment, PORT_NUMf, 0);
        if (BCM_XGS3_L3_MPLS_LBL_VALID(info->label)) {
            soc_MPLS_ENTRYm_field32_set(unit, ment, MPLS_LABELf, info->label);
        } else {
            return BCM_E_PARAM;
        }
        soc_MPLS_ENTRYm_field32_set(unit, ment, VALIDf, 1);
        return BCM_E_NONE;
    }

    rv = _bcm_esw_gport_resolve(unit, info->port, &mod_out, 
                                &port_out, &trunk_id, &gport_id);
    BCM_IF_ERROR_RETURN(rv);

    if (BCM_GPORT_IS_TRUNK(info->port)) {
        soc_MPLS_ENTRYm_field32_set(unit, ment, Tf, 1);
        soc_MPLS_ENTRYm_field32_set(unit, ment, TGIDf, trunk_id);
    } else {
        soc_MPLS_ENTRYm_field32_set(unit, ment, MODULE_IDf, mod_out);
        soc_MPLS_ENTRYm_field32_set(unit, ment, PORT_NUMf, port_out);
    }
    if (BCM_XGS3_L3_MPLS_LBL_VALID(info->label)) {
        soc_MPLS_ENTRYm_field32_set(unit, ment, MPLS_LABELf, info->label);
    } else {
        return BCM_E_PARAM;
    }
    soc_MPLS_ENTRYm_field32_set(unit, ment, VALIDf, 1);
    
    return BCM_E_NONE;
}

/* Convert key part of HW entry to application format. */
STATIC int
_bcm_tr_mpls_entry_get_key(int unit, mpls_entry_entry_t *ment,
                           bcm_mpls_tunnel_switch_t *info)
{
    bcm_port_t port_in, port_out;
    bcm_module_t mod_in, mod_out;
    bcm_trunk_t trunk_id;

    port_in = soc_MPLS_ENTRYm_field32_get(unit, ment, PORT_NUMf);
    mod_in = soc_MPLS_ENTRYm_field32_get(unit, ment, MODULE_IDf);
    if (soc_MPLS_ENTRYm_field32_get(unit, ment, Tf)) {
        trunk_id = soc_MPLS_ENTRYm_field32_get(unit, ment, TGIDf);
        BCM_GPORT_TRUNK_SET(info->port, trunk_id);
    } else if ((port_in == 0) && (mod_in == 0)) {
        /* Global label, mod/port not part of lookup key */
        info->port = BCM_GPORT_INVALID;
    } else {
        BCM_IF_ERROR_RETURN
            (_bcm_esw_stk_modmap_map(unit, BCM_STK_MODMAP_GET,
                                    mod_in, port_in, &mod_out, &port_out));
        BCM_GPORT_MODPORT_SET(info->port, mod_out, port_out);
    } 
    info->label = soc_MPLS_ENTRYm_field32_get(unit, ment, MPLS_LABELf);

    return BCM_E_NONE;
}

/* Convert data part of HW entry to application format. */
STATIC int
_bcm_tr_mpls_entry_get_data(int unit, mpls_entry_entry_t *ment,
                            bcm_mpls_tunnel_switch_t *info)
{
    int rv, action, nh_index, vrf;
    bcm_if_t egress_if=0;
    int mode = 0;

    BCM_IF_ERROR_RETURN(bcm_xgs3_l3_ingress_mode_get(unit, &mode));

    action = soc_MPLS_ENTRYm_field32_get(unit, ment, MPLS_ACTION_IF_BOSf);
    switch(action) {
    case 0x2:
        info->action = BCM_MPLS_SWITCH_ACTION_POP;
        if (!mode) {
            vrf = soc_MPLS_ENTRYm_field32_get(unit, ment, L3_IIFf);
            vrf -= _BCM_TR_MPLS_L3_IIF_BASE;
            _BCM_MPLS_VPN_SET(info->vpn, _BCM_MPLS_VPN_TYPE_L3, vrf);
        } else {
            info->ingress_if = soc_MPLS_ENTRYm_field32_get(unit, ment, L3_IIFf);
        }
        break;
    case 0x3:
        info->action = BCM_MPLS_SWITCH_ACTION_SWAP;
        nh_index = soc_MPLS_ENTRYm_field32_get(unit, ment,
                                               NEXT_HOP_INDEXf);
        rv = bcm_tr_mpls_get_vp_nh (unit, nh_index,&egress_if);
        if (rv == BCM_E_NONE) {
            rv = bcm_tr_mpls_l3_nh_info_get(unit, info, nh_index);
            info->egress_if = egress_if;
        } else {
            info->egress_if = nh_index + BCM_XGS3_EGRESS_IDX_MIN;
            info->egress_label.label = BCM_MPLS_LABEL_INVALID;
            rv = BCM_E_NONE;
        }
        BCM_IF_ERROR_RETURN(rv);
        break;
    case 0x4:
        info->action = BCM_MPLS_SWITCH_ACTION_PHP;
        nh_index = soc_MPLS_ENTRYm_field32_get(unit, ment,
                                               NEXT_HOP_INDEXf);
        info->egress_if = nh_index + BCM_XGS3_EGRESS_IDX_MIN;
        break;
    case 0x5:
        info->action = BCM_MPLS_SWITCH_ACTION_PHP;
        nh_index = soc_MPLS_ENTRYm_field32_get(unit, ment,
                                               ECMP_PTRf);
        info->egress_if = nh_index + BCM_XGS3_MPATH_EGRESS_IDX_MIN;
        break;
    default:
        return BCM_E_INTERNAL;
        break;
    }
    if (soc_MPLS_ENTRYm_field32_get(unit, ment, PW_TERM_NUM_VALIDf)) {
        info->flags |= BCM_MPLS_SWITCH_COUNTED;
    }

    if (!soc_MPLS_ENTRYm_field32_get(unit, ment, DECAP_USE_TTLf)) {
        info->flags |= BCM_MPLS_SWITCH_INNER_TTL;
    }
    if (soc_MPLS_ENTRYm_field32_get(unit, ment, DECAP_USE_EXP_FOR_INNERf)) {
        if (info->action != BCM_MPLS_SWITCH_ACTION_SWAP) {
            info->flags |= BCM_MPLS_SWITCH_INNER_EXP;
        }
    }
    if (soc_MPLS_ENTRYm_field32_get(unit, ment, 
                                    DECAP_USE_EXP_FOR_PRIf) == 0x1) {

        /* Use specified EXP-map to determine internal prio/color */
        info->flags |= BCM_MPLS_SWITCH_INT_PRI_MAP;
        info->exp_map = 
            soc_MPLS_ENTRYm_field32_get(unit, ment, EXP_MAPPING_PTRf);
        info->exp_map |= _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_INGRESS;
    } else if (soc_MPLS_ENTRYm_field32_get(unit, ment, 
                                           DECAP_USE_EXP_FOR_PRIf) == 0x2) {

        /* Use the specified internal priority value */
        info->flags |= BCM_MPLS_SWITCH_INT_PRI_SET;
        info->int_pri =
            soc_MPLS_ENTRYm_field32_get(unit, ment, NEW_PRIf);

        /* Use specified EXP-map to determine internal color */
        info->flags |= BCM_MPLS_SWITCH_COLOR_MAP;
        info->exp_map = 
            soc_MPLS_ENTRYm_field32_get(unit, ment, EXP_MAPPING_PTRf);
        info->exp_map |= _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_INGRESS;
    }
    if (SOC_MEM_FIELD_VALID(unit,MPLS_ENTRYm,
                            DO_NOT_CHANGE_PAYLOAD_DSCPf)) {
        if (soc_MPLS_ENTRYm_field32_get(unit, ment, 
                                        DO_NOT_CHANGE_PAYLOAD_DSCPf) == 0) {
            if ((info->action == BCM_MPLS_SWITCH_ACTION_PHP) ||
                (info->action == BCM_MPLS_SWITCH_ACTION_POP)) {
                     info->flags |=
                         (BCM_MPLS_SWITCH_OUTER_EXP | BCM_MPLS_SWITCH_OUTER_TTL);
            }
        }
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_tr_mpls_egress_object_modify
 * Purpose:
 *      Modify Egress NH entry
 * Parameters:
 *      unit - Device Number
 *      nh_index - Next Hop Index/ECMP index
 *      new_entry_type - New Entry type
 * Returns:
 *      BCM_E_XXX
 */

int
bcm_tr_mpls_egress_object_modify(int unit, int nh_index, int new_entry_type)
{
    bcm_l3_egress_t nh_info;
    egr_l3_next_hop_entry_t new_egr_nh;
    egr_mac_da_profile_entry_t macda;
    uint32 macda_index=-1;
    void *entries[1];
    int rv = BCM_E_NONE;
    int old_entry_type;

#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
    uint32 flex_ctr_offset_mode, flex_ctr_pool_number, flex_ctr_base_idx;
#endif
    BCM_IF_ERROR_RETURN(soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ALL,
                          nh_index, &new_egr_nh));

    old_entry_type = 
               soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &new_egr_nh,
                            ENTRY_TYPEf);

    if ((old_entry_type == 0) && (new_entry_type == 1)) {
        BCM_IF_ERROR_RETURN(bcm_xgs3_nh_get(unit, nh_index, &nh_info));
        sal_memset(&macda, 0, sizeof(egr_mac_da_profile_entry_t));
        soc_mem_mac_addr_set(unit, EGR_MAC_DA_PROFILEm,
                             &macda, MAC_ADDRESSf, nh_info.mac_addr);
        entries[0] = &macda;
        BCM_IF_ERROR_RETURN(_bcm_mac_da_profile_entry_add(unit, entries, 1,
                                                          (uint32 *) &macda_index));
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit,soc_feature_advanced_flex_counter)) {
            /* 1 Store the old flex counter */
            BCM_IF_ERROR_RETURN(
                _bcm_esw_get_flex_counter_fields_values(unit, nh_index,
                                                        EGR_L3_NEXT_HOPm, 
                                                        &new_egr_nh,
                                                        0,
                                                        &flex_ctr_offset_mode, 
                                                        &flex_ctr_pool_number, 
                                                        &flex_ctr_base_idx));
            /* 2 Clear flex counter in old entry view */
            BCM_IF_ERROR_RETURN(
                _bcm_esw_set_flex_counter_fields_values(unit, nh_index, 
                                                        EGR_L3_NEXT_HOPm, 
                                                        &new_egr_nh,
                                                        0,
                                                        0, 
                                                        0, 
                                                        0));
        }
#endif

        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                &new_egr_nh, MPLS__MAC_DA_PROFILE_INDEXf,
                macda_index);
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                &new_egr_nh, MPLS__DVPf, 0);
            if (soc_feature(unit,
                soc_feature_multiple_split_horizon_group)) {
                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                    &new_egr_nh, MPLS__DVP_NETWORK_GROUPf, 0);
            } else {
                soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                    &new_egr_nh, MPLS__DVP_IS_NETWORK_PORTf, 0);
            }
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                     &new_egr_nh, MPLS__HG_MODIFY_ENABLEf, 0x1);
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                     &new_egr_nh, MPLS__HG_HDR_SELf, 0);
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                     &new_egr_nh, MPLS__HG_MC_DST_MODIDf, 0);
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                     &new_egr_nh, MPLS__HG_MC_DST_PORT_NUMf, 0);
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                     &new_egr_nh, MPLS__HG_ADD_SYS_RSVD_VIDf, 0);
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                     &new_egr_nh, MPLS__HG_LEARN_OVERRIDEf, 0);
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                     &new_egr_nh, MPLS__HG_L3_OVERRIDEf, 0);
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                     &new_egr_nh, MPLS__PW_INIT_NUMf, 0);
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                     &new_egr_nh, MPLS__VC_AND_SWAP_INDEXf, 0);
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                     &new_egr_nh, MPLS__UMC_DROPf, 0);
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                     &new_egr_nh, MPLS__UUC_DROPf, 0);
            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                     &new_egr_nh, MPLS__BC_DROPf, 0);

        } else {
               soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                         &new_egr_nh, MAC_DA_PROFILE_INDEXf,
                         macda_index);
               soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &new_egr_nh, DVPf, 0);
               soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &new_egr_nh, DVP_IS_NETWORK_PORTf, 0);
               soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &new_egr_nh, HG_MODIFY_ENABLEf, 0x1);
               soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &new_egr_nh, HG_HDR_SELf, 0);
               soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &new_egr_nh, HG_MC_DST_MODIDf, 0);
               soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &new_egr_nh, HG_MC_DST_PORT_NUMf, 0);
               soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &new_egr_nh, HG_ADD_SYS_RSVD_VIDf, 0);
               soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &new_egr_nh, HG_LEARN_OVERRIDEf, 0);
               soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &new_egr_nh, HG_L3_OVERRIDEf, 0);
               soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &new_egr_nh, PW_INIT_NUMf, 0);
               soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &new_egr_nh, VC_AND_SWAP_INDEXf, 0);
        }

#ifdef BCM_KATANA_SUPPORT
           if (soc_feature(unit, soc_feature_vlan_xlate)) {
           soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                           &new_egr_nh, MPLS__USE_DVP_IN_EVXLT_KEYf, 0);
        }
#endif
          
        soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &new_egr_nh,
                            ENTRY_TYPEf, new_entry_type);

#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit,soc_feature_advanced_flex_counter)) {
            /* Restore flex counter in new entry view */
            BCM_IF_ERROR_RETURN(
                _bcm_esw_set_flex_counter_fields_values(unit, nh_index, 
                                                        EGR_L3_NEXT_HOPm, 
                                                        &new_egr_nh,
                                                        0,
                                                        flex_ctr_offset_mode, 
                                                        flex_ctr_pool_number, 
                                                        flex_ctr_base_idx));
        }
#endif

        rv = soc_mem_write(unit, EGR_L3_NEXT_HOPm,
                           MEM_BLOCK_ALL, nh_index, &new_egr_nh);
        if (rv < 0) {
            if (macda_index != (uint32)-1) {
                (void) _bcm_mac_da_profile_entry_delete(unit, macda_index);
            }
        }
    }else if ((old_entry_type == 1) && (new_entry_type == 0)) {
        /* Delete MAC_DA_PROFILE_INDEX */
        bcm_mac_t mac_addr;
        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
            macda_index =
                  soc_EGR_L3_NEXT_HOPm_field32_get(unit, &new_egr_nh,
                                         MPLS__MAC_DA_PROFILE_INDEXf);
        } else {
           macda_index =
                  soc_EGR_L3_NEXT_HOPm_field32_get(unit, &new_egr_nh,
                                         MAC_DA_PROFILE_INDEXf);
        }

        /* Read entry from MAC-DA-PROFILEm */
        BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_MAC_DA_PROFILEm,
                             MEM_BLOCK_ANY, macda_index, &macda));
        /* Obtain Mac-Address */
        soc_mem_mac_addr_get(unit, EGR_MAC_DA_PROFILEm,
                             &macda, MAC_ADDRESSf, mac_addr);
        if (soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm, L3__MAC_ADDRESSf)) { 
            soc_mem_mac_addr_set(unit, EGR_L3_NEXT_HOPm, &new_egr_nh, 
                                 L3__MAC_ADDRESSf, mac_addr); 
        } else {
            soc_mem_mac_addr_set(unit, EGR_L3_NEXT_HOPm, &new_egr_nh,
                                 MAC_ADDRESSf, mac_addr);
        }
        if (macda_index != (uint32)-1) {
           (void) _bcm_mac_da_profile_entry_delete(unit, macda_index);
        } 

#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit,soc_feature_advanced_flex_counter)) {
            /* 1 Store the old flex counter */
            BCM_IF_ERROR_RETURN(
                _bcm_esw_get_flex_counter_fields_values(unit, nh_index, 
                                                        EGR_L3_NEXT_HOPm, 
                                                        &new_egr_nh,
                                                        0,
                                                        &flex_ctr_offset_mode, 
                                                        &flex_ctr_pool_number, 
                                                        &flex_ctr_base_idx));
            /* 2 Clear flex counter in old entry view */
            BCM_IF_ERROR_RETURN(
                _bcm_esw_set_flex_counter_fields_values(unit, nh_index, 
                                                        EGR_L3_NEXT_HOPm, 
                                                        &new_egr_nh, 
                                                        0,
                                                        0, 
                                                        0, 
                                                        0));
        }
#endif

        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
               soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                        &new_egr_nh, MPLS__MAC_DA_PROFILE_INDEXf, 0);
        } else {
               soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm,
                         &new_egr_nh, MAC_DA_PROFILE_INDEXf, 0);
        }

        soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &new_egr_nh,
                            ENTRY_TYPEf, new_entry_type);

#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
        if (soc_feature(unit,soc_feature_advanced_flex_counter)) {
            /* Restore flex counter in new entry view */
            BCM_IF_ERROR_RETURN(
                _bcm_esw_set_flex_counter_fields_values(unit, nh_index, 
                                                        EGR_L3_NEXT_HOPm, 
                                                        &new_egr_nh,
                                                        0,
                                                        flex_ctr_offset_mode, 
                                                        flex_ctr_pool_number, 
                                                        flex_ctr_base_idx));
        }
#endif

        rv = soc_mem_write(unit, EGR_L3_NEXT_HOPm,
                           MEM_BLOCK_ALL, nh_index, &new_egr_nh);

    }

    return rv;
}

/*
 * Function:
 *      bcm_tr_mpls_egress_entry_modify
 * Purpose:
 *      Modify Egress entry
 * Parameters:
 *      unit - Device Number
 *      nh_index - Next Hop Index/ECMP index
 *      mpath_flag - Flag to to indicate NH/ECMP
 *      new_entry_type - New Entry type
 * Returns:
 *      BCM_E_XXX
 */

int
bcm_tr_mpls_egress_entry_modify(int unit, int nh_index,
        int mpath_flag, int new_entry_type)
{
    int i = 0, ecmp_member_count = 0;
    int alloc_size, index;
    bcm_l3_egress_ecmp_t ecmp_info;
    bcm_l3_ecmp_member_t *ecmp_member_array = NULL;
    int rv = BCM_E_NONE;
   
    if (mpath_flag & BCM_L3_MULTIPATH) {
    
        alloc_size = sizeof(bcm_l3_ecmp_member_t) * BCM_XGS3_L3_ECMP_MAX(unit);
        ecmp_member_array = sal_alloc(alloc_size, "ecmp member array");
        if (NULL == ecmp_member_array) {
            return BCM_E_MEMORY;
        }
        sal_memset(ecmp_member_array, 0, alloc_size);

        ecmp_info.ecmp_intf = nh_index + BCM_XGS3_MPATH_EGRESS_IDX_MIN;
        rv = bcm_xgs3_l3_ecmp_get(unit, &ecmp_info, BCM_XGS3_L3_ECMP_MAX(unit),
                                  ecmp_member_array, &ecmp_member_count);
        if (BCM_FAILURE(rv)) {
            goto clean_up;
        }

        while (i < ecmp_member_count) {
            if (BCM_XGS3_L3_EGRESS_IDX_VALID(unit,
                        ecmp_member_array[i].egress_if)) {
                index = (ecmp_member_array[i].egress_if -
                        BCM_XGS3_EGRESS_IDX_MIN);
            } else {
                index = (ecmp_member_array[i].egress_if -
                        BCM_XGS3_DVP_EGRESS_IDX_MIN);
            }

            rv = bcm_tr_mpls_egress_object_modify(unit, index, new_entry_type);
            if (BCM_FAILURE(rv)) {
                goto clean_up;
            }
            i++;
        }
    } else {
        rv = bcm_tr_mpls_egress_object_modify(unit, nh_index, new_entry_type);
    }

clean_up:
    if (ecmp_member_array) {
        sal_free(ecmp_member_array);
        ecmp_member_array = NULL;
    }

    return rv;
}

/*
 * Function:
 *      bcm_mpls_tunnel_switch_add
 * Purpose:
 *      Add an MPLS label entry.
 * Parameters:
 *      unit - Device Number
 *      info - Label (switch) information
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_tunnel_switch_add(int unit, bcm_mpls_tunnel_switch_t *info)
{
    mpls_entry_entry_t ment;
    int mode=0, nh_index = -1, vrf=0, rv, num_pw_term, old_pw_cnt = -1, pw_cnt = -1;
    int index, action = -1, old_nh_index = -1, old_ecmp_index = -1;
    int  tunnel_switch_update=0;
    uint32 mpath_flag=0;
    int  ref_count=0;
    bcm_if_t  egress_if=0;

    rv = bcm_xgs3_l3_egress_mode_get(unit, &mode);
    BCM_IF_ERROR_RETURN(rv);
    if (!mode) {
        LOG_INFO(BSL_LS_BCM_L3,
                 (BSL_META_U(unit,
                             "L3 egress mode must be set first\n")));
        return BCM_E_DISABLED;
    }

    /* Check for Port_independent Label mapping */
    if (!BCM_XGS3_L3_MPLS_LBL_VALID(info->label)) {
        return BCM_E_PARAM;
    }

    rv = bcm_tr_mpls_port_independent_range (unit, info->label, info->port);
    if (rv < 0) {
        return rv;
    }

    BCM_IF_ERROR_RETURN(_bcm_tr_mpls_entry_set_key(unit, info, &ment));

    /* See if entry already exists */
    rv = soc_mem_search(unit, MPLS_ENTRYm, MEM_BLOCK_ANY, &index,
                        &ment, &ment, 0);

    /* default not to use DSCP from ING_MPLS_EXP_MAPPING table */
    if (SOC_MEM_FIELD_VALID(unit,MPLS_ENTRYm,DO_NOT_CHANGE_PAYLOAD_DSCPf)) {
        soc_MPLS_ENTRYm_field32_set(unit, &ment,
                                          DO_NOT_CHANGE_PAYLOAD_DSCPf, 1);
    }

    if (rv == SOC_E_NONE) {
        /* Entry exists, save old info */
        tunnel_switch_update = 1;
        action = soc_MPLS_ENTRYm_field32_get(unit, &ment, MPLS_ACTION_IF_BOSf);
        if ((action == 0x3) || (action == 0x4)) {
            /* SWAP_NHI or L3_NHI */
            old_nh_index = soc_MPLS_ENTRYm_field32_get(unit, &ment, NEXT_HOP_INDEXf);
        } else if (action == 0x5) {
            /* L3_ECMP */
            old_ecmp_index = soc_MPLS_ENTRYm_field32_get(unit, &ment, ECMP_PTRf);
        }

        if (soc_MPLS_ENTRYm_field32_get(unit, &ment, PW_TERM_NUM_VALIDf)) {
            old_pw_cnt = soc_MPLS_ENTRYm_field32_get(unit, &ment, PW_TERM_NUMf);
        } 
    } else if (rv != SOC_E_NOT_FOUND) {
        return rv;
    }

    switch(info->action) {
    case BCM_MPLS_SWITCH_ACTION_SWAP:
        if (!BCM_XGS3_L3_EGRESS_IDX_VALID(unit, info->egress_if)) {
            return BCM_E_PARAM;
        }
        soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_ACTION_IF_NOT_BOSf,
                                    0x3); /* SWAP_NHI */
        soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_ACTION_IF_BOSf,
                                    0x3); /* SWAP NHI */
        break;
    case BCM_MPLS_SWITCH_ACTION_POP:
        soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_ACTION_IF_NOT_BOSf,
                                    0x1); /* POP */
        soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_ACTION_IF_BOSf,
                                    0x2); /* L3_IIF */ 
        /* uniform qos model if either of these two flags set.
         * Only apply to L3 MPLS and BOS
         */
        if (info->flags & (BCM_MPLS_SWITCH_OUTER_EXP |
                           BCM_MPLS_SWITCH_OUTER_TTL) ) {
            if (SOC_MEM_FIELD_VALID(unit,MPLS_ENTRYm,
                             DO_NOT_CHANGE_PAYLOAD_DSCPf)) {
                soc_MPLS_ENTRYm_field32_set(unit, &ment,
                                  DO_NOT_CHANGE_PAYLOAD_DSCPf, 0);
            }
        }

        break;
    case BCM_MPLS_SWITCH_ACTION_PHP:
        soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_ACTION_IF_NOT_BOSf,
                                    0x2); /* PHP_NHI */
        if (BCM_XGS3_L3_MPATH_EGRESS_IDX_VALID(unit, info->egress_if)) {
            soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_ACTION_IF_BOSf,
                                        0x5); /* L3_ECMP */ 
        } else if (BCM_XGS3_L3_EGRESS_IDX_VALID(unit, info->egress_if)) {
            soc_MPLS_ENTRYm_field32_set(unit, &ment, MPLS_ACTION_IF_BOSf,
                                        0x4); /* L3_NHI */ 
        } else {
            return BCM_E_PARAM;
        }
        /* uniform qos model if either of these two flags set.
         * Only apply to L3 MPLS and BOS
         */
        if (info->flags & (BCM_MPLS_SWITCH_OUTER_EXP |
                           BCM_MPLS_SWITCH_OUTER_TTL) ) {
            if (SOC_MEM_FIELD_VALID(unit,MPLS_ENTRYm,
                             DO_NOT_CHANGE_PAYLOAD_DSCPf)) {
                soc_MPLS_ENTRYm_field32_set(unit, &ment,
                                  DO_NOT_CHANGE_PAYLOAD_DSCPf, 0);
            }
        }
        break;
    default:
        return BCM_E_PARAM;
        break;
    }
    if (info->action == BCM_MPLS_SWITCH_ACTION_POP) {
        mode = -1;
        if (_BCM_MPLS_VPN_IS_L3(info->vpn)) {
            _BCM_MPLS_VPN_GET(vrf, _BCM_MPLS_VPN_TYPE_L3, info->vpn);
            if (!_BCM_MPLS_VRF_USED_GET(unit, vrf)) {
                return BCM_E_PARAM;
            }

            /* Check L3 Ingress Interface mode. */ 
            mode = 0;
            rv = bcm_xgs3_l3_ingress_mode_get(unit, &mode);
            BCM_IF_ERROR_RETURN(rv);
        }
        if (!mode) {
             _bcm_l3_ingress_intf_t iif;
             sal_memset(&iif, 0, sizeof(_bcm_l3_ingress_intf_t));
             iif.intf_id =  _BCM_TR_MPLS_L3_IIF_BASE + vrf;
             rv = _bcm_tr_l3_ingress_interface_get(unit, NULL, &iif);
             if (BCM_FAILURE(rv)) {
                 return rv;
             }
             iif.vrf = vrf;
             rv = _bcm_tr_l3_ingress_interface_set(unit, &iif, NULL, NULL);
             if (BCM_FAILURE(rv)) {
                 return rv;
             }
             soc_MPLS_ENTRYm_field32_set(unit, &ment, L3_IIFf, 
                                        _BCM_TR_MPLS_L3_IIF_BASE + vrf);
         } else {
             soc_MPLS_ENTRYm_field32_set(unit, &ment, L3_IIFf, info->ingress_if);
         }
    } else if (info->action == BCM_MPLS_SWITCH_ACTION_PHP) {

        /*
         * Get egress next-hop index from egress object and
         * increment egress object reference count.
         */

        BCM_IF_ERROR_RETURN(bcm_xgs3_get_nh_from_egress_object(unit, info->egress_if,
                                                               &mpath_flag, 1, &nh_index));
       
        if (mpath_flag != BCM_L3_MULTIPATH) { 
            /* Fix: Entry_Type = 1, for PHP Packets with more than 1 Label */
            /* Read the egress next_hop entry pointed by Egress-Object */   
            rv = bcm_tr_mpls_egress_entry_modify(unit, nh_index, mpath_flag, 1);
            if (rv < 0 ) {
               return rv;
            }
        }
        if (mpath_flag == BCM_L3_MULTIPATH) {
            soc_MPLS_ENTRYm_field32_set(unit, &ment, ECMP_PTRf, nh_index);
        } else {
            soc_MPLS_ENTRYm_field32_set(unit, &ment, NEXT_HOP_INDEXf, nh_index);
        }
    } else if (info->action == BCM_MPLS_SWITCH_ACTION_SWAP) {
        if (BCM_XGS3_L3_MPLS_LBL_VALID(info->egress_label.label)) {         
            rv = bcm_tr_mpls_l3_nh_info_add(unit, info, &nh_index);
            if (rv < 0) {
                goto cleanup;
            }
        } else {
            rv = bcm_xgs3_get_nh_from_egress_object(unit, info->egress_if,
                                                    &mpath_flag, 1, &nh_index);
            if (rv < 0) {
                return rv;
            }
        }
        soc_MPLS_ENTRYm_field32_set(unit, &ment, NEXT_HOP_INDEXf, nh_index);
    }/* BCM_MPLS_SWITCH_ACTION_SWAP */

    
    soc_MPLS_ENTRYm_field32_set(unit, &ment, V4_ENABLEf, 1);
    soc_MPLS_ENTRYm_field32_set(unit, &ment, V6_ENABLEf, 1);
    if (info->flags & BCM_MPLS_SWITCH_INNER_TTL) {
        if (info->action == BCM_MPLS_SWITCH_ACTION_SWAP) {
            rv = BCM_E_PARAM;
            goto cleanup;
        }
        soc_MPLS_ENTRYm_field32_set(unit, &ment, DECAP_USE_TTLf, 0);
    } else {
        soc_MPLS_ENTRYm_field32_set(unit, &ment, DECAP_USE_TTLf, 1);
    }
    if (info->flags & BCM_MPLS_SWITCH_INNER_EXP) {
        if (info->action == BCM_MPLS_SWITCH_ACTION_SWAP) {
            rv = BCM_E_PARAM;
            goto cleanup;
        }
        soc_MPLS_ENTRYm_field32_set(unit, &ment, DECAP_USE_EXP_FOR_INNERf, 0);
    } else {
        /* For SWAP, Do-not PUSH EXP */
        if (info->action == BCM_MPLS_SWITCH_ACTION_SWAP) {
            soc_MPLS_ENTRYm_field32_set(unit, &ment, DECAP_USE_EXP_FOR_INNERf, 0);
        } else {
            soc_MPLS_ENTRYm_field32_set(unit, &ment, DECAP_USE_EXP_FOR_INNERf, 1);
        }
    }

    (void) bcm_tr_mpls_entry_internal_qos_set(unit, NULL, info, &ment);

    if ((info->flags & BCM_MPLS_SWITCH_COUNTED)) {
       if (SOC_MEM_IS_VALID(unit, ING_PW_TERM_COUNTERSm)) {
        if (old_pw_cnt == -1) {
            num_pw_term = soc_mem_index_count(unit, ING_PW_TERM_COUNTERSm);
            for (pw_cnt = 0; pw_cnt < num_pw_term; pw_cnt++) {
                if (!_BCM_MPLS_PW_TERM_USED_GET(unit, pw_cnt)) {
                    break;
                }
            }
            if (pw_cnt == num_pw_term) {
                rv = BCM_E_RESOURCE;
                goto cleanup;
            }
            _BCM_MPLS_PW_TERM_USED_SET(unit, pw_cnt);
            soc_MPLS_ENTRYm_field32_set(unit, &ment, PW_TERM_NUMf, pw_cnt);
            soc_MPLS_ENTRYm_field32_set(unit, &ment, PW_TERM_NUM_VALIDf, 1);
        }
      }
    }
#ifdef BCM_TRIUMPH3_SUPPORT
    if (SOC_IS_TRIUMPH3(unit)) {
        soc_MPLS_ENTRYm_field32_set(unit, &ment, KEY_TYPEf, 0x10);
    }
#endif

    if (!tunnel_switch_update) {
        rv = soc_mem_insert(unit, MPLS_ENTRYm, MEM_BLOCK_ALL, &ment);
    } else {
        rv = soc_mem_write(unit, MPLS_ENTRYm,
                           MEM_BLOCK_ALL, index,
                           &ment);
    }

    if (rv < 0) {
        goto cleanup;
    }

    if (tunnel_switch_update) {
        /* Clean up old next-hop and counter info if entry was replaced */
        if ((old_pw_cnt != -1) && !(info->flags & BCM_MPLS_SWITCH_COUNTED)) {
            _BCM_MPLS_PW_TERM_USED_CLR(unit, old_pw_cnt);
        }
        if (action ==  0x3) { /* SWAP_NHI */
            /* Check if tunnel_switch.egress_label mode is being used */
            rv = bcm_tr_mpls_get_vp_nh (unit, (bcm_if_t) old_nh_index, &egress_if);
            if (rv == BCM_E_NONE) {
                rv = bcm_tr_mpls_l3_nh_info_delete(unit, old_nh_index);
            } else {
                /* Decrement next-hop Reference count */
                rv = bcm_xgs3_get_ref_count_from_nhi(unit, 0, &ref_count, old_nh_index);
            }
        } else if (action == 0x4) {
            /* L3_NHI */
            rv = bcm_xgs3_nh_del(unit, 0, old_nh_index);
        } else if (action == 0x5) {
            /* L3_ECMP */
            rv = bcm_xgs3_ecmp_group_del(unit, old_ecmp_index);
        }
    }
    if (rv < 0) {
        goto cleanup;
    }

#ifdef BCM_WARM_BOOT_SUPPORT
    SOC_CONTROL_LOCK(unit);
    SOC_CONTROL(unit)->scache_dirty = 1;
    SOC_CONTROL_UNLOCK(unit);
#endif

    return rv;

  cleanup:
    if (pw_cnt != -1) {
        _BCM_MPLS_PW_TERM_USED_CLR(unit, pw_cnt);
    }
    if (nh_index != -1) {
        if (info->action == BCM_MPLS_SWITCH_ACTION_SWAP) {
            if (BCM_XGS3_L3_MPLS_LBL_VALID(info->egress_label.label) ||
                (info->action == BCM_MPLS_SWITCH_ACTION_PHP)) {
                (void) bcm_tr_mpls_l3_nh_info_delete(unit, nh_index);
            } else {
                /* Decrement next-hop Reference count */
                rv = bcm_xgs3_get_ref_count_from_nhi(unit, 0, &ref_count, nh_index);
            }
        } else if (info->action == BCM_MPLS_SWITCH_ACTION_PHP) {
            (void) bcm_xgs3_nh_del(unit, 0, nh_index);
        }
    }
    return rv;
}

STATIC int
_bcm_tr_mpls_entry_delete(int unit, mpls_entry_entry_t *ment)
{   
    ing_pw_term_counters_entry_t pw_cnt_entry;
    int rv, action, ecmp_index = -1, nh_index = -1, pw_cnt = -1;
    bcm_if_t  egress_if=0;
    int  ref_count=0;

    if (soc_MPLS_ENTRYm_field32_get(unit, ment, PW_TERM_NUM_VALIDf)) {
        pw_cnt = soc_MPLS_ENTRYm_field32_get(unit, ment, PW_TERM_NUMf);
    }

    action = soc_MPLS_ENTRYm_field32_get(unit, ment, MPLS_ACTION_IF_BOSf);
    if ((action == 0x3) || (action == 0x4)) {
        /* SWAP_NHI or L3_NHI */
        nh_index = soc_MPLS_ENTRYm_field32_get(unit, ment, NEXT_HOP_INDEXf);
    } else if (action == 0x5) {
        /* L3_ECMP */
        ecmp_index = soc_MPLS_ENTRYm_field32_get(unit, ment, ECMP_PTRf);
    }
        
    /* Delete the entry from HW */
    rv = soc_mem_delete(unit, MPLS_ENTRYm, MEM_BLOCK_ALL, ment);
    if ( (rv != BCM_E_NOT_FOUND) && (rv != BCM_E_NONE) ) {
         return rv;
    }

#if defined(BCM_TRIUMPH2_SUPPORT)
    if (soc_feature(unit, soc_feature_gport_service_counters) &&
        (0 != soc_MPLS_ENTRYm_field32_get(unit, ment,
                                          USE_VINTF_CTR_IDXf)) &&
        (0 != soc_MPLS_ENTRYm_field32_get(unit, ment,
                                          VINTF_CTR_IDXf))) {
        /* Release MPLS label stat use, if any */
        _bcm_flex_stat_handle_t mpls_fsh;
        mpls_entry_entry_t fs_ment;
        uint32 key[2];

        sal_memset(&fs_ment, 0, sizeof(fs_ment));
        /* Construct key-only entry, copy to FS handle */
        soc_MPLS_ENTRYm_field32_set(unit, &fs_ment, VALIDf,
                 soc_MPLS_ENTRYm_field32_get(unit, ment, VALIDf));
        soc_mem_field_get(unit, MPLS_ENTRYm, (uint32 *) ment,
                          KEYf, (uint32 *) key);
        soc_mem_field_set(unit, MPLS_ENTRYm, (uint32 *) &fs_ment,
                          KEYf, (uint32 *) key);
        /* Technically, we should copy the KEY_TYPE field here,
         * but the rest of the MPLS code appears to abuse the fact that
         * the key type value is 0, so it was handled when we
         * cleared the entry above.
         */

        _BCM_FLEX_STAT_HANDLE_CLEAR(mpls_fsh);
        _BCM_FLEX_STAT_HANDLE_COPY(mpls_fsh, fs_ment);
        _bcm_esw_flex_stat_ext_handle_free(unit,
                                           _bcmFlexStatTypeMplsLabel,
                                           mpls_fsh);
    }
#endif /* BCM_TRIUMPH2_SUPPORT  */

    if (pw_cnt != -1) {
        sal_memset(&pw_cnt_entry, 0, sizeof(ing_pw_term_counters_entry_t));
        (void) WRITE_ING_PW_TERM_COUNTERSm(unit, MEM_BLOCK_ALL, pw_cnt,
                                           &pw_cnt_entry);
        _BCM_MPLS_PW_TERM_USED_CLR(unit, pw_cnt);
    }

    if (action == 0x3) {
        /* SWAP_NHI */
        /* Check if tunnel_switch.egress_label mode is being used */
        rv = bcm_tr_mpls_get_vp_nh (unit, (bcm_if_t) nh_index, &egress_if);
        if (rv == BCM_E_NONE) {
            rv = bcm_tr_mpls_l3_nh_info_delete(unit, nh_index);
        } else {
            /* Decrement next-hop Reference count */
            rv = bcm_xgs3_get_ref_count_from_nhi(unit, 0, &ref_count, nh_index);
        }
    } else if (action == 0x4) {
        /* L3_NHI */
        rv = bcm_xgs3_nh_del(unit, 0, nh_index);
    } else if (action == 0x5) {
        /* L3_ECMP */
        rv = bcm_xgs3_ecmp_group_del(unit, ecmp_index);
    }
    return rv;
}

/*
 * Function:
 *      bcm_mpls_tunnel_switch_delete
 * Purpose:
 *      Delete an MPLS label entry.
 * Parameters:
 *      unit - Device Number
 *      info - Label (switch) information
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_tunnel_switch_delete(int unit, bcm_mpls_tunnel_switch_t *info)
{
    int rv, index;
    mpls_entry_entry_t ment;


    rv = _bcm_tr_mpls_entry_set_key(unit, info, &ment);
    BCM_IF_ERROR_RETURN(rv);

    rv = soc_mem_search(unit, MPLS_ENTRYm, MEM_BLOCK_ANY, &index,
                        &ment, &ment, 0);
    if (rv < 0) {
        return rv;
    }
    rv = _bcm_tr_mpls_entry_delete(unit, &ment);

#ifdef BCM_WARM_BOOT_SUPPORT
    SOC_CONTROL_LOCK(unit);
    SOC_CONTROL(unit)->scache_dirty = 1;
    SOC_CONTROL_UNLOCK(unit);
#endif

    return rv;
}

/*
 * Function:
 *      bcm_mpls_tunnel_switch_delete_all
 * Purpose:
 *      Delete all MPLS label entries.
 * Parameters:
 *      unit   - Device Number
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_tunnel_switch_delete_all(int unit)
{
    int rv, i, num_entries;
    mpls_entry_entry_t *ment, *mentp;
    int imin, imax;

    imin = soc_mem_index_min(unit, MPLS_ENTRYm);
    imax = soc_mem_index_max(unit, MPLS_ENTRYm);
    num_entries = soc_mem_index_count(unit, MPLS_ENTRYm);

    ment = soc_cm_salloc(unit, num_entries * sizeof(*ment), "mpls_entry");
    if (ment == NULL) {
        return BCM_E_MEMORY;
    }
    sal_memset(ment, 0, num_entries * sizeof(*ment));

    rv = soc_mem_read_range(unit, MPLS_ENTRYm, MEM_BLOCK_ANY,
                            imin, imax, ment);
    if (BCM_FAILURE(rv)) {
        soc_cm_sfree(unit, ment);
        return rv; 
    }
    for (i = 0; i < num_entries; i++) {
        mentp = soc_mem_table_idx_to_pointer(unit, MPLS_ENTRYm,
                                             mpls_entry_entry_t *, ment, i);
        if (!soc_MPLS_ENTRYm_field32_get(unit, mentp, VALIDf)) {
            continue;
        }
        if (soc_MPLS_ENTRYm_field32_get(unit, mentp,
                                        MPLS_ACTION_IF_BOSf) == 0x1) {
            /* L2_SVP */
            continue;
        }
        rv = _bcm_tr_mpls_entry_delete(unit, mentp);
        if (rv < 0) {
            soc_cm_sfree(unit, ment);
            return rv;
        }
    }
    soc_cm_sfree(unit, ment);

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_mpls_tunnel_switch_get
 * Purpose:
 *      Get an MPLS label entry.
 * Parameters:
 *      unit - Device Number
 *      info - Label (switch) information
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_tunnel_switch_get(int unit, bcm_mpls_tunnel_switch_t *info)
{
    int rv, index;
    mpls_entry_entry_t ment;

    rv = _bcm_tr_mpls_entry_set_key(unit, info, &ment);


    BCM_IF_ERROR_RETURN(rv);

    rv = soc_mem_search(unit, MPLS_ENTRYm, MEM_BLOCK_ANY, &index,
                        &ment, &ment, 0);

    if (rv < 0) {
        return rv;
    }
    rv = _bcm_tr_mpls_entry_get_data(unit, &ment, info);

    return rv;
}

/*
 * Function:
 *      bcm_mpls_tunnel_switch_traverse
 * Purpose:
 *      Traverse all valid MPLS label entries an call the
 *      supplied callback routine.
 * Parameters:
 *      unit      - Device Number
 *      cb        - User callback function, called once per MPLS entry.
 *      user_data - cookie
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_tunnel_switch_traverse(int unit, 
                                   bcm_mpls_tunnel_switch_traverse_cb cb,
                                   void *user_data)
{
    int rv, i, num_entries;
    mpls_entry_entry_t ment;
    bcm_mpls_tunnel_switch_t info;


    

    num_entries = soc_mem_index_count(unit, MPLS_ENTRYm);
    for (i = 0; i < num_entries; i++) {
        BCM_IF_ERROR_RETURN (READ_MPLS_ENTRYm(unit, MEM_BLOCK_ANY, i, &ment));
        /* Check MPLS Key_type */
        if (soc_feature(unit, soc_feature_mpls_enhanced)) {
            if (0x0 != soc_MPLS_ENTRYm_field32_get(unit, &ment, KEY_TYPEf)) {
                continue;
            }
        }
        /* Check for valid entry */
        if (!soc_MPLS_ENTRYm_field32_get(unit, &ment, VALIDf)) {
            continue;
        }
        if (soc_MPLS_ENTRYm_field32_get(unit, &ment,
                                        MPLS_ACTION_IF_BOSf) == 0x1) {
            /* L2_SVP */
            continue;
        }
        sal_memset(&info, 0, sizeof(bcm_mpls_tunnel_switch_t));
        rv = _bcm_tr_mpls_entry_get_key(unit, &ment, &info);
        if (rv < 0) {
            return rv;
        }
        rv = _bcm_tr_mpls_entry_get_data(unit, &ment, &info);
        if (rv < 0) {
            return rv;
        }

#ifndef BCM_CB_ABORT_ON_ERR
        (void) cb(unit, &info, user_data);
#else
        rv = cb(unit, &info, user_data);
        if (BCM_FAILURE(rv) && SOC_CB_ABORT_ON_ERR(unit)) {
            return rv;
        }
#endif
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_mpls_exp_map_create
 * Purpose:
 *      Create an MPLS EXP map instance.
 * Parameters:
 *      unit        - (IN)  SOC unit #
 *      flags       - (IN)  MPLS flags
 *      exp_map_id  - (OUT) Allocated EXP map ID
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_exp_map_create(int unit, uint32 flags, int *exp_map_id)
{
    int i, table_num, num_exp_map;
    egr_mpls_pri_mapping_entry_t egr_mpls_pri_map[64];
    egr_mpls_exp_mapping_1_entry_t egr_mpls_exp_map[64];
    egr_mpls_exp_mapping_2_entry_t egr_mpls_exp_map2[64];
    int id, index = -1;
    void *entries[3];

    /* Check for unsupported Flag */
    if (flags & (~(BCM_MPLS_EXP_MAP_WITH_ID |
                   BCM_MPLS_EXP_MAP_INGRESS |
                   BCM_MPLS_EXP_MAP_EGRESS |
                   BCM_MPLS_EXP_MAP_EGRESS_L2 ))) {
        return BCM_E_PARAM;
    }

    /* Input parameters check. */
    if (NULL == exp_map_id) {
        return BCM_E_PARAM;
    }

    if (flags & BCM_MPLS_EXP_MAP_INGRESS) {
        /* ING_MPLS_EXP_MAPPING profiles indexed by EXP */
        num_exp_map = soc_mem_index_count(unit, ING_MPLS_EXP_MAPPINGm) / 8;

        if (flags & BCM_MPLS_EXP_MAP_WITH_ID) {
            table_num = (*exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK);
            if (table_num >= num_exp_map) {
                return BCM_E_PARAM;
            }
            if (_BCM_MPLS_ING_EXP_MAP_USED_GET(unit, table_num)) {
                return BCM_E_EXISTS;
            }
            _BCM_MPLS_ING_EXP_MAP_USED_SET(unit, table_num);
#ifdef BCM_WARM_BOOT_SUPPORT
            SOC_CONTROL_LOCK(unit);
            SOC_CONTROL(unit)->scache_dirty = 1;
            SOC_CONTROL_UNLOCK(unit);
#endif
            return BCM_E_NONE;
        }

        for (i = 0; i < num_exp_map; i++) {
            if (!_BCM_MPLS_ING_EXP_MAP_USED_GET(unit, i)) {
                _BCM_MPLS_ING_EXP_MAP_USED_SET(unit, i);
                *exp_map_id = (i | _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_INGRESS);
#ifdef BCM_WARM_BOOT_SUPPORT
                SOC_CONTROL_LOCK(unit);
                SOC_CONTROL(unit)->scache_dirty = 1;
                SOC_CONTROL_UNLOCK(unit);
#endif
                return BCM_E_NONE;
            }
        }

    } else if (flags & BCM_MPLS_EXP_MAP_EGRESS) {
        /* EGR_MPLS_EXP_MAPPING_1 profiles indexed by ((int_pri << 2) + cng) */
        num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 64;

        if (flags & BCM_MPLS_EXP_MAP_WITH_ID) {
            table_num = (*exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK);
            if (table_num >= num_exp_map) {
                return BCM_E_PARAM;
            }
            if (!_BCM_EGR_MPLS_MAP_USED_GET(unit, table_num)) {
                /* 
                 * Reserve a chunk in the EGR_MPLS_EXP_MAPPING_1 and
                 * EGR_MPLS_PRI_MAPPING tables. These two tables always
                 * done together.
                 */
                sal_memset(egr_mpls_pri_map, 0, sizeof(egr_mpls_pri_map));
                entries[0] = egr_mpls_pri_map;
                sal_memset(egr_mpls_exp_map, 0, sizeof(egr_mpls_exp_map));
                entries[1] = egr_mpls_exp_map;
                sal_memset(egr_mpls_exp_map2, 0, sizeof(egr_mpls_exp_map2));
                entries[2] = egr_mpls_exp_map2;
                BCM_IF_ERROR_RETURN
                    (_bcm_egr_mpls_combo_map_entry_add(unit, entries, 64,
                                                       (uint32 *)&index));
                MPLS_INFO(unit)->egr_mpls_hw_idx[table_num] = index / 64;
                _BCM_EGR_MPLS_MAP_USED_SET(unit, table_num);
            } else {
                return BCM_E_EXISTS;
            }
#ifdef BCM_WARM_BOOT_SUPPORT
            SOC_CONTROL_LOCK(unit);
            SOC_CONTROL(unit)->scache_dirty = 1;
            SOC_CONTROL_UNLOCK(unit);
#endif
            return BCM_E_NONE;
        }

        for (id = 0; id < num_exp_map; id++) {
            if (!_BCM_EGR_MPLS_MAP_USED_GET(unit, id)) {
                *exp_map_id = (id | _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS);
                /* 
                 * Reserve a chunk in the EGR_MPLS_EXP_MAPPING_1 and
                 * EGR_MPLS_PRI_MAPPING tables. These two tables always
                 * done together.
                 */
                sal_memset(egr_mpls_pri_map, 0, sizeof(egr_mpls_pri_map));
                entries[0] = egr_mpls_pri_map;
                sal_memset(egr_mpls_exp_map, 0, sizeof(egr_mpls_exp_map));
                entries[1] = egr_mpls_exp_map;
                sal_memset(egr_mpls_exp_map2, 0, sizeof(egr_mpls_exp_map2));
                entries[2] = egr_mpls_exp_map2;
                BCM_IF_ERROR_RETURN
                    (_bcm_egr_mpls_combo_map_entry_add(unit, entries, 64,
                                                       (uint32 *)&index));
                MPLS_INFO(unit)->egr_mpls_hw_idx[id] = index / 64;
                _BCM_EGR_MPLS_MAP_USED_SET(unit, id);
#ifdef BCM_WARM_BOOT_SUPPORT
                SOC_CONTROL_LOCK(unit);
                SOC_CONTROL(unit)->scache_dirty = 1;
                SOC_CONTROL_UNLOCK(unit);
#endif
                return BCM_E_NONE;
            }
        }
    } else if (flags & BCM_MPLS_EXP_MAP_EGRESS_L2) {
        /* EGR_MPLS_EXP_PRI_MAPPINGm profiles indexed by EXP */
        num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_PRI_MAPPINGm) / 8;

         if (flags & BCM_MPLS_EXP_MAP_WITH_ID) {
            table_num = (*exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK);
            if (table_num > num_exp_map) {
                return BCM_E_PARAM;
            }
            if (_BCM_MPLS_EGR_EXP_MAP_USED_GET(unit, table_num)) {
                return BCM_E_EXISTS;
            }
            _BCM_MPLS_EGR_EXP_MAP_USED_SET(unit, table_num);
#ifdef BCM_WARM_BOOT_SUPPORT
            SOC_CONTROL_LOCK(unit);
            SOC_CONTROL(unit)->scache_dirty = 1;
            SOC_CONTROL_UNLOCK(unit);
#endif
            return BCM_E_NONE;
         }

         for (i = 0; i < num_exp_map; i++) {
            if (!_BCM_MPLS_EGR_EXP_MAP_USED_GET(unit, i)) {
                _BCM_MPLS_EGR_EXP_MAP_USED_SET(unit, i);
#ifdef BCM_WARM_BOOT_SUPPORT
                SOC_CONTROL_LOCK(unit);
                SOC_CONTROL(unit)->scache_dirty = 1;
                SOC_CONTROL_UNLOCK(unit);
#endif
                *exp_map_id = (i | _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS_L2);
                return BCM_E_NONE;
            }
         }
    } else {
        return BCM_E_PARAM;
    }
    return BCM_E_FULL;
}

/*
 * Function:
 *      bcm_mpls_exp_map_destroy
 * Purpose:
 *      Destroy an existing MPLS EXP map instance.
 * Parameters:
 *      unit       - (IN) SOC unit #
 *      exp_map_id - (IN) EXP map ID
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_exp_map_destroy(int unit, int exp_map_id)
{
    int num_exp_map, table_num, base_idx;

    if (exp_map_id < 0) {
        return BCM_E_PARAM;
    }
    table_num = exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK;

    if ((exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_MASK) == 
        _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_INGRESS) {

        /* ING_MPLS_EXP_MAPPING profiles indexed by EXP */
        num_exp_map = soc_mem_index_count(unit, ING_MPLS_EXP_MAPPINGm) / 8;
        if (table_num >= num_exp_map) {
            return BCM_E_PARAM;
        }
        if (!_BCM_MPLS_ING_EXP_MAP_USED_GET(unit, table_num)) {
            return BCM_E_NOT_FOUND;
        }
        _BCM_MPLS_ING_EXP_MAP_USED_CLR(unit, table_num);
#ifdef BCM_WARM_BOOT_SUPPORT
        SOC_CONTROL_LOCK(unit);
        SOC_CONTROL(unit)->scache_dirty = 1;
        SOC_CONTROL_UNLOCK(unit);
#endif
    } else if ((exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_MASK) == 
               _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS) {
        /* EGR_MPLS_EXP_MAPPING_1 profiles indexed by ((int_pri << 2) + cng) */
        num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 64;

        if (table_num >= num_exp_map) {
            return BCM_E_PARAM;
        }
        if (!_BCM_EGR_MPLS_MAP_USED_GET(unit, table_num)) {
            return BCM_E_NOT_FOUND;
        }
        base_idx = MPLS_INFO(unit)->egr_mpls_hw_idx[table_num] * 64;
        _BCM_EGR_MPLS_MAP_USED_CLR(unit, table_num);
#ifdef BCM_WARM_BOOT_SUPPORT
        SOC_CONTROL_LOCK(unit);
        SOC_CONTROL(unit)->scache_dirty = 1;
        SOC_CONTROL_UNLOCK(unit);
#endif
        /* Clear from profile memories */
        BCM_IF_ERROR_RETURN
            (_bcm_egr_mpls_combo_map_entry_delete(unit, base_idx));
    } else if ((exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_MASK) == 
               _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS_L2) {

        /* EGR_MPLS_EXP_PRI_MAPPINGm profiles indexed by EXP */
        num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_PRI_MAPPINGm) / 8;
        if (table_num >= num_exp_map) {
            return BCM_E_PARAM;
        }
        if (!_BCM_MPLS_EGR_EXP_MAP_USED_GET(unit, table_num)) {
            return BCM_E_NOT_FOUND;
        }
        _BCM_MPLS_EGR_EXP_MAP_USED_CLR(unit, table_num);
#ifdef BCM_WARM_BOOT_SUPPORT
        SOC_CONTROL_LOCK(unit);
        SOC_CONTROL(unit)->scache_dirty = 1;
        SOC_CONTROL_UNLOCK(unit);
#endif        
    } else {
        return BCM_E_PARAM;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_mpls_exp_map_destroy_all
 * Purpose:
 *      Destroy all existing MPLS EXP map instances.
 * Parameters:
 *      unit       - (IN) SOC unit #
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_exp_map_destroy_all(int unit)
{
    int i, num_exp_map, base_idx;
    int rv = BCM_E_NONE;


    /* ING_MPLS_EXP_MAPPING profiles indexed by EXP */
    num_exp_map = soc_mem_index_count(unit, ING_MPLS_EXP_MAPPINGm) / 8;
    for (i = 0; i < num_exp_map; i++) {
        if (_BCM_MPLS_ING_EXP_MAP_USED_GET(unit, i)) {
            _BCM_MPLS_ING_EXP_MAP_USED_CLR(unit, i);
        }
    }

    /* EGR_MPLS_EXP_MAPPING_1 profiles indexed by ((int_pri << 2) + cng) */
    num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 64;

    for (i = 0; i < num_exp_map; i++) {
        if (_BCM_EGR_MPLS_MAP_USED_GET(unit, i)) {
            _BCM_EGR_MPLS_MAP_USED_CLR(unit, i);
            base_idx = MPLS_INFO(unit)->egr_mpls_hw_idx[i] * 64;
            /* Clear from profile memories */
            rv = _bcm_egr_mpls_combo_map_entry_delete(unit, base_idx);
            if ((rv != BCM_E_NONE) && (rv != BCM_E_NOT_FOUND)) {
                return rv;
            }
        }
    }

    /* EGR_MPLS_EXP_PRI_MAPPINGm profiles indexed by EXP */
    num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_PRI_MAPPINGm) / 8;
    for (i = 0; i < num_exp_map; i++) {
        if (_BCM_MPLS_EGR_EXP_MAP_USED_GET(unit, i)) {
            _BCM_MPLS_EGR_EXP_MAP_USED_CLR(unit, i);
        }
    }

    return BCM_E_NONE;
}


/*
 * Function:
 *      bcm_mpls_exp_map_set
 * Purpose:
 *      Set the mapping of { internal priority, color }
 *      to a EXP value for MPLS headers
 *      in the specified EXP map instance.
 * Parameters:
 *      unit        - (IN) SOC unit #
 *      exp_map_id  - (IN) EXP map ID
 *      map         - (IN) EXP map config structure 
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_exp_map_set(int unit, int exp_map_id,
                        bcm_mpls_exp_map_t *map)
{
    int table_num, num_exp_map, cng, rv, index;
    egr_mpls_exp_pri_mapping_entry_t exp_pri_entry;
    ing_mpls_exp_mapping_entry_t ing_pri_entry;
    int max_int_pri = 15;
    
    /* Check validity of exp_map_id */
    if (exp_map_id < 0) {
        return BCM_E_PARAM;
    }

    if (!map) {
        /* invalid pointer to map data */
        return BCM_E_PARAM;
    }

    table_num = exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK;

    if ((exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_MASK) == 
        _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_INGRESS) {

        /* ING_MPLS_EXP_MAPPING profiles indexed by EXP */
        num_exp_map = soc_mem_index_count(unit, ING_MPLS_EXP_MAPPINGm) / 8;

        if (!_BCM_MPLS_ING_EXP_MAP_USED_GET(unit, table_num)) {
            return BCM_E_NOT_FOUND;
        }

        if ((map->priority < BCM_PRIO_MIN) || (map->priority > max_int_pri) ||
            (map->exp > 7)) {
            return BCM_E_PARAM;
        }
 
        cng = _BCM_COLOR_ENCODING(unit, map->color);
 
        /* Get the base index for this EXP map */
        index = (table_num * 8) + map->exp; 
 
        /* Commit to HW */
        BCM_IF_ERROR_RETURN(READ_ING_MPLS_EXP_MAPPINGm(unit, MEM_BLOCK_ANY,
                                                       index, &ing_pri_entry));
        soc_ING_MPLS_EXP_MAPPINGm_field32_set(unit, &ing_pri_entry, 
                                              PRIf, map->priority);
        soc_ING_MPLS_EXP_MAPPINGm_field32_set(unit, &ing_pri_entry, 
                                              CNGf, cng);
        if (SOC_MEM_FIELD_VALID(unit,ING_MPLS_EXP_MAPPINGm,DSCPf)) {
            if (map->dscp >= (1 << soc_mem_field_length(
                  unit, ING_MPLS_EXP_MAPPINGm, DSCPf))) {
                return BCM_E_PARAM;
            }
            soc_ING_MPLS_EXP_MAPPINGm_field32_set(unit, &ing_pri_entry,
                                              DSCPf, map->dscp);
        }
        BCM_IF_ERROR_RETURN(WRITE_ING_MPLS_EXP_MAPPINGm(unit, MEM_BLOCK_ALL, 
                                                        index, &ing_pri_entry));
    } else if 
        ((exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_MASK) == 
         _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS) {
        int alloc_size, offset;
        egr_mpls_pri_mapping_entry_t *egr_mpls_pri_map;
        egr_mpls_exp_mapping_1_entry_t *egr_mpls_exp_map;
        egr_mpls_exp_mapping_2_entry_t *egr_mpls_exp_map2;
        void *entries[3];
        uint32 *entry;
        /* EGR_MPLS_EXP_MAPPING_1 profiles indexed by ((int_pri << 2) + cng) */
        num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 64;

        if ((map == NULL) || (table_num >= num_exp_map) ||
            (map->priority < BCM_PRIO_MIN) || (map->priority > max_int_pri) ||
            (map->exp > 7) || (map->pkt_pri > 7) || (map->pkt_cfi > 1)) {
            return BCM_E_PARAM;
        }

        if (!_BCM_EGR_MPLS_MAP_USED_GET(unit, table_num)) {
            return BCM_E_NOT_FOUND;
        }

        /* Allocate memory for DMA */
        alloc_size = 64 * sizeof (egr_mpls_pri_mapping_entry_t);
        egr_mpls_pri_map = soc_cm_salloc(unit, alloc_size,
                                         "TR2 egr mpls pri map");
        if (NULL == egr_mpls_pri_map) {
            return (BCM_E_MEMORY);
        }
        sal_memset(egr_mpls_pri_map, 0, alloc_size);

        alloc_size = 64 * sizeof (egr_mpls_exp_mapping_1_entry_t);
        egr_mpls_exp_map = soc_cm_salloc(unit, alloc_size,
                                         "TR2 egr mpls exp map");
        if (NULL == egr_mpls_exp_map) {
            sal_free(egr_mpls_pri_map);
            return (BCM_E_MEMORY);
        }
        sal_memset(egr_mpls_exp_map, 0, alloc_size);

        alloc_size = 64 * sizeof (egr_mpls_exp_mapping_2_entry_t);
        egr_mpls_exp_map2 = soc_cm_salloc(unit, alloc_size,
                                         "TR2 egr mpls exp map2");
        if (NULL == egr_mpls_exp_map2) {
            sal_free(egr_mpls_pri_map);
            sal_free(egr_mpls_exp_map);
            return (BCM_E_MEMORY);
        }
        sal_memset(egr_mpls_exp_map2, 0, alloc_size);

        /* Base index of table in hardware */
        index = MPLS_INFO(unit)->egr_mpls_hw_idx[table_num] * 64;

        /* Offset within table */
        cng = _BCM_COLOR_ENCODING(unit, map->color);
        offset = (map->priority << 2) | cng;

        /* Read the old profile chunk */
        rv = soc_mem_read_range(unit, EGR_MPLS_PRI_MAPPINGm, MEM_BLOCK_ANY, 
                                index, index + 63, egr_mpls_pri_map);
        if (BCM_FAILURE(rv)) {
            soc_cm_sfree(unit, egr_mpls_pri_map);
            soc_cm_sfree(unit, egr_mpls_exp_map);
            return (rv);
        }
        if (SOC_MEM_IS_VALID(unit, EGR_MPLS_EXP_MAPPING_1m)) {
            rv = soc_mem_read_range(unit, EGR_MPLS_EXP_MAPPING_1m, MEM_BLOCK_ANY, 
                                    index, index + 63, egr_mpls_exp_map);
            if (BCM_FAILURE(rv)) {
                soc_cm_sfree(unit, egr_mpls_pri_map);
                soc_cm_sfree(unit, egr_mpls_exp_map);
                return (rv);
            }
        }

        if (SOC_MEM_IS_VALID(unit, EGR_MPLS_EXP_MAPPING_2m)) {
            rv = soc_mem_read_range(unit, EGR_MPLS_EXP_MAPPING_2m, MEM_BLOCK_ANY, 
                                    index, index + 63, egr_mpls_exp_map2);
            if (BCM_FAILURE(rv)) {
                soc_cm_sfree(unit, egr_mpls_pri_map);
                soc_cm_sfree(unit, egr_mpls_exp_map);
                soc_cm_sfree(unit, egr_mpls_exp_map2);
                return (rv);
            }
        }

        /* Modify what's needed */
        entry = (uint32 *)&egr_mpls_pri_map[offset];
        sal_memcpy(entry, soc_mem_entry_null(unit, EGR_MPLS_PRI_MAPPINGm), 
                   soc_mem_entry_words(unit, EGR_MPLS_PRI_MAPPINGm) * 4);
        soc_mem_field32_set(unit, EGR_MPLS_PRI_MAPPINGm, entry, 
                            NEW_PRIf, map->pkt_pri);
        soc_mem_field32_set(unit, EGR_MPLS_PRI_MAPPINGm, entry,
                            NEW_CFIf, map->pkt_cfi);
        if (SOC_MEM_IS_VALID(unit, EGR_MPLS_EXP_MAPPING_1m)) {
            entry = (uint32 *)&egr_mpls_exp_map[offset];
            sal_memcpy(entry, soc_mem_entry_null(unit, EGR_MPLS_EXP_MAPPING_1m), 
                       soc_mem_entry_words(unit, EGR_MPLS_EXP_MAPPING_1m) * 4);
            soc_mem_field32_set(unit, EGR_MPLS_EXP_MAPPING_1m, entry, 
                                MPLS_EXPf, map->exp);
        }
        if (SOC_MEM_IS_VALID(unit, EGR_MPLS_EXP_MAPPING_2m)) {
            entry = (uint32 *)&egr_mpls_exp_map2[offset];
            sal_memcpy(entry, soc_mem_entry_null(unit, EGR_MPLS_EXP_MAPPING_2m), 
                       soc_mem_entry_words(unit, EGR_MPLS_EXP_MAPPING_2m) * 4);
            soc_mem_field32_set(unit, EGR_MPLS_EXP_MAPPING_2m, entry, 
                                MPLS_EXPf, map->exp);
        }

        /* Update hardware tables */
        entries[0] = egr_mpls_pri_map;
        entries[1] = egr_mpls_exp_map;
        entries[2] = egr_mpls_exp_map2;
        rv = _bcm_egr_mpls_combo_map_entry_update(unit, entries, index);
        if (rv == SOC_E_NONE) {
            /* decrease the profile ref_count increased by entry_update */
            rv = _bcm_egr_mpls_combo_map_entry_delete(unit, index);
        }
  
        /* Free the DMA memory */
        soc_cm_sfree(unit, egr_mpls_pri_map);
        soc_cm_sfree(unit, egr_mpls_exp_map);
        soc_cm_sfree(unit, egr_mpls_exp_map2);

#ifdef BCM_WARM_BOOT_SUPPORT
        SOC_CONTROL_LOCK(unit);
        SOC_CONTROL(unit)->scache_dirty = 1;
        SOC_CONTROL_UNLOCK(unit);
#endif
    } else if 
        ((exp_map_id  & _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_MASK) == 
         _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS_L2) {

        if ((map->exp > 7) || (map->pkt_pri > 7) || (map->pkt_cfi > 1)) {
            return BCM_E_PARAM;
        }

        /* EGR_MPLS_EXP_PRI_MAPPINGm  profiles indexed by EXP */
        num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_PRI_MAPPINGm) / 8;

        /* index = Base index + Offset  for this EXP map */
        index = (table_num * 8) + map->exp; 

        if (!_BCM_MPLS_EGR_EXP_MAP_USED_GET(unit, table_num)) {
            return BCM_E_NOT_FOUND;
        }

        BCM_IF_ERROR_RETURN(READ_EGR_MPLS_EXP_PRI_MAPPINGm(unit, MEM_BLOCK_ANY,
                                                           index, &exp_pri_entry));
        soc_EGR_MPLS_EXP_PRI_MAPPINGm_field32_set(unit, &exp_pri_entry, 
                                                  NEW_PRIf, map->pkt_pri);
        soc_EGR_MPLS_EXP_PRI_MAPPINGm_field32_set(unit, &exp_pri_entry, 
                                                  NEW_CFIf, map->pkt_cfi);
        BCM_IF_ERROR_RETURN(WRITE_EGR_MPLS_EXP_PRI_MAPPINGm(unit, MEM_BLOCK_ALL, 
                                                            index, &exp_pri_entry));

    } else {
        return BCM_E_PARAM;
    }

    return BCM_E_NONE;
}
    
/*
 * Function:
 *      bcm_mpls_exp_map_get
 * Purpose:
 *      Get the mapping of { internal priority, color }
 *      to a EXP value for MPLS headers
 *      in the specified EXP map instance.
 * Parameters:
 *      unit        - (IN)  SOC unit #
 *      exp_map_id  - (IN)  EXP map ID
 *      map         - (OUT) EXP map config structure 
 * Returns:
 *      BCM_E_XXX
 */     
int     
bcm_tr_mpls_exp_map_get(int unit, int exp_map_id,
                        bcm_mpls_exp_map_t *map)
{
    int num_exp_map, cng, index, table_num;
    egr_mpls_pri_mapping_entry_t pri_entry;
    egr_mpls_exp_mapping_1_entry_t exp_entry;
    ing_mpls_exp_mapping_entry_t ing_pri_entry;
    egr_mpls_exp_pri_mapping_entry_t exp_pri_entry;
    int max_int_pri = 15;


    if (exp_map_id < 0) {
        return BCM_E_PARAM;
    }

    table_num = exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK;

    if ((exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_MASK) == 
        _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_INGRESS) {

        /* ING_MPLS_EXP_MAPPING profiles indexed by EXP */
        num_exp_map = soc_mem_index_count(unit, ING_MPLS_EXP_MAPPINGm) / 8;

        if ((map == NULL) || (table_num >= num_exp_map)) {
            return BCM_E_PARAM;
        }
        if (!_BCM_MPLS_ING_EXP_MAP_USED_GET(unit, table_num)) {
            return BCM_E_NOT_FOUND;
        }

        /* Get the base index for this EXP map */
        index = (table_num * 8) + map->exp; 

        /* Commit to HW */
        BCM_IF_ERROR_RETURN(READ_ING_MPLS_EXP_MAPPINGm(unit, MEM_BLOCK_ANY,
                                                       index, &ing_pri_entry));
        map->priority = soc_mem_field32_get(unit, ING_MPLS_EXP_MAPPINGm, 
                                            &ing_pri_entry, PRIf);
        cng = soc_mem_field32_get(unit, ING_MPLS_EXP_MAPPINGm,
                                  &ing_pri_entry, CNGf);
        map->color = _BCM_COLOR_DECODING(unit, cng);
        if (SOC_MEM_FIELD_VALID(unit,ING_MPLS_EXP_MAPPINGm,DSCPf)) {
            map->dscp = soc_mem_field32_get(unit, ING_MPLS_EXP_MAPPINGm,
                                              &ing_pri_entry,
                                              DSCPf);
        }
    } else if
        ((exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_MASK) == 
         _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS) {
        /* EGR_MPLS_EXP_MAPPING_1 profiles indexed by ((int_pri << 2) + cng) */
        num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 64;
    
        if ((map == NULL) || (table_num >= num_exp_map) ||
            (map->priority < BCM_PRIO_MIN) || (map->priority > max_int_pri)) {
            return BCM_E_PARAM;
        }

        if (!_BCM_EGR_MPLS_MAP_USED_GET(unit, table_num)) {
            return BCM_E_NOT_FOUND;
        }
        cng = _BCM_COLOR_ENCODING(unit, map->color);

        /* Base index of table in hardware */
        index = MPLS_INFO(unit)->egr_mpls_hw_idx[table_num] * 64;

        /* Add the offset based on priority and color values */
        index += ((map->priority << 2) | (cng & 3));

        /* Get the values from HW */
        BCM_IF_ERROR_RETURN(READ_EGR_MPLS_EXP_MAPPING_1m(unit, MEM_BLOCK_ANY,
                                                         index, &exp_entry));
        map->exp = soc_mem_field32_get(unit, EGR_MPLS_EXP_MAPPING_1m, 
                                       &exp_entry, MPLS_EXPf);
    
        BCM_IF_ERROR_RETURN(READ_EGR_MPLS_PRI_MAPPINGm(unit, MEM_BLOCK_ANY,
                                                       index, &pri_entry));
        map->pkt_pri = soc_mem_field32_get(unit, EGR_MPLS_PRI_MAPPINGm, 
                                           &pri_entry, NEW_PRIf);
        map->pkt_cfi = soc_mem_field32_get(unit, EGR_MPLS_PRI_MAPPINGm, 
                                           &pri_entry, NEW_CFIf);
    } else if    
        ((exp_map_id  & _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_MASK) == 
         _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS_L2) {

        /* EGR_MPLS_EXP_PRI_MAPPINGm profiles indexed by EXP */
        num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_PRI_MAPPINGm) / 8;

        if ((map == NULL) || (table_num >= num_exp_map)) {
            return BCM_E_PARAM;
        }

        if (!_BCM_MPLS_EGR_EXP_MAP_USED_GET(unit, table_num)) {
            return BCM_E_NOT_FOUND;
        }

        /* index = Base index + Offset  for this EXP map */
        index = (table_num * 8) + map->exp; 

        /* Read from HW */
        BCM_IF_ERROR_RETURN(READ_EGR_MPLS_EXP_PRI_MAPPINGm(unit, MEM_BLOCK_ANY,
                                                           index, &exp_pri_entry));
        map->pkt_pri = soc_mem_field32_get(unit, EGR_MPLS_EXP_PRI_MAPPINGm, 
                                           &exp_pri_entry, NEW_PRIf);
        map->pkt_cfi = soc_mem_field32_get(unit, EGR_MPLS_EXP_PRI_MAPPINGm,
                                           &exp_pri_entry, NEW_CFIf);
    } else {
        return BCM_E_PARAM;
    }
    return BCM_E_NONE;
}

#ifdef BCM_APACHE_SUPPORT
/*
 * Function:
 *      bcm_tr_mpls_exp_to_phb_cng_map_create
 * Purpose:
 *      Create an MPLS EXP to PBH and CNG map instance.
 * Parameters:
 *      unit        - (IN)  SOC unit #
 *      flags       - (IN)  MPLS flags
 *      exp_map_id  - (OUT) Allocated EXP map ID
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_exp_to_phb_cng_map_create(int unit, uint32 flags, int *exp_map_id)
{
    int i, table_num, num_exp_map;

    /* Check for unsupported Flag */
    if (flags & (~(BCM_MPLS_EXP_MAP_WITH_ID |
                   BCM_MPLS_EXP_MAP_INGRESS |
                   BCM_MPLS_EXP_MAP_EGRESS |
                   BCM_QOS_MAP_OAM_MPLS_EXP))) {
        return BCM_E_PARAM;
    }

    /* Input parameters check. */
    if (NULL == exp_map_id) {
        return BCM_E_PARAM;
    }

    if (flags & BCM_MPLS_EXP_MAP_INGRESS) {
        /* ING_MPLS_EXP_MAPPING_1 profiles indexed by EXP */
        num_exp_map = soc_mem_index_count(unit, ING_MPLS_EXP_MAPPING_1m) / 16;

        if (flags & BCM_MPLS_EXP_MAP_WITH_ID) {
            table_num = (*exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK);
            if (table_num >= num_exp_map) {
                return BCM_E_PARAM;
            }
            if (_BCM_MPLS_ING_EXP_TO_PHB_CNG_MAP_USED_GET(unit, table_num)) {
                return BCM_E_EXISTS;
            }
            _BCM_MPLS_ING_EXP_TO_PHB_CNG_MAP_USED_SET(unit, table_num);
#ifdef BCM_WARM_BOOT_SUPPORT
            SOC_CONTROL_LOCK(unit);
            SOC_CONTROL(unit)->scache_dirty = 1;
            SOC_CONTROL_UNLOCK(unit);
#endif

            *exp_map_id = table_num | (_BCM_QOS_MAP_TYPE_EXP_TO_PHB_CNG_MAP
                    << _BCM_QOS_MAP_SHIFT);
            *exp_map_id = (*exp_map_id |
                           _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_INGRESS);
            return BCM_E_NONE;
        }

        for (i = 0; i < num_exp_map; i++) {
            if (!_BCM_MPLS_ING_EXP_TO_PHB_CNG_MAP_USED_GET(unit, i)) {
                _BCM_MPLS_ING_EXP_TO_PHB_CNG_MAP_USED_SET(unit, i);
                *exp_map_id = i | (_BCM_QOS_MAP_TYPE_EXP_TO_PHB_CNG_MAP
                                                     << _BCM_QOS_MAP_SHIFT);
                *exp_map_id = (*exp_map_id |
                               _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_INGRESS);
#ifdef BCM_WARM_BOOT_SUPPORT
                SOC_CONTROL_LOCK(unit);
                SOC_CONTROL(unit)->scache_dirty = 1;
                SOC_CONTROL_UNLOCK(unit);
#endif
                return BCM_E_NONE;
            }
        }

    } else if (flags & BCM_MPLS_EXP_MAP_EGRESS) {
        /* EGR_MPLS_EXP_MAPPING_3 profiles indexed by MPLS_EXP */
        num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_1m) / 8;

        if (flags & BCM_MPLS_EXP_MAP_WITH_ID) {
            table_num = (*exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK);
            if (table_num >= num_exp_map) {
                return BCM_E_PARAM;
            }
            if (!_BCM_MPLS_EGR_EXP_TO_PHB_CNG_MAP_USED_GET(unit, table_num)) {
                /* Reserve a chunk in the EGR_MPLS_EXP_MAPPING_3 */
                _BCM_MPLS_EGR_EXP_TO_PHB_CNG_MAP_USED_SET(unit, table_num);
            } else {
                return BCM_E_EXISTS;
            }
#ifdef BCM_WARM_BOOT_SUPPORT
            SOC_CONTROL_LOCK(unit);
            SOC_CONTROL(unit)->scache_dirty = 1;
            SOC_CONTROL_UNLOCK(unit);
#endif
            *exp_map_id = table_num | (_BCM_QOS_MAP_TYPE_EXP_TO_PHB_CNG_MAP
                    << _BCM_QOS_MAP_SHIFT);
            *exp_map_id = (*exp_map_id |
                           _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS);
            return BCM_E_NONE;
        }

        for (i = 0; i < num_exp_map; i++) {
            if (!_BCM_MPLS_EGR_EXP_TO_PHB_CNG_MAP_USED_GET(unit, i)) {
                *exp_map_id = i | (_BCM_QOS_MAP_TYPE_EXP_TO_PHB_CNG_MAP
                                                     << _BCM_QOS_MAP_SHIFT);
                *exp_map_id = (*exp_map_id |
                               _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS);

                _BCM_MPLS_EGR_EXP_TO_PHB_CNG_MAP_USED_SET(unit, i);
#ifdef BCM_WARM_BOOT_SUPPORT
                SOC_CONTROL_LOCK(unit);
                SOC_CONTROL(unit)->scache_dirty = 1;
                SOC_CONTROL_UNLOCK(unit);
#endif
                return BCM_E_NONE;
            }
        }
    } else {
        return BCM_E_PARAM;
    }
    return BCM_E_FULL;
}

/*
 * Function:
 *      bcm_tr_mpls_exp_to_phb_cng_map_destroy
 * Purpose:
 *      Destroy an existing MPLS EXP to PBH and CNG map instance.
 * Parameters:
 *      unit       - (IN) SOC unit #
 *      exp_map_id - (IN) EXP map ID
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_exp_to_phb_cng_map_destroy(int unit, int exp_map_id)
{
    int num_exp_map, table_num;

    if (exp_map_id < 0) {
        return BCM_E_PARAM;
    }
    table_num = exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK;

    if ((exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_MASK) == 
        _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_INGRESS) {

        /* ING_MPLS_EXP_MAPPING_1 profiles indexed by EXP */
        num_exp_map = soc_mem_index_count(unit, ING_MPLS_EXP_MAPPING_1m) / 16;
        if (table_num >= num_exp_map) {
            return BCM_E_PARAM;
        }
        if (!_BCM_MPLS_ING_EXP_TO_PHB_CNG_MAP_USED_GET(unit, table_num)) {
            return BCM_E_NOT_FOUND;
        }
        _BCM_MPLS_ING_EXP_TO_PHB_CNG_MAP_USED_CLR(unit, table_num);
#ifdef BCM_WARM_BOOT_SUPPORT
        SOC_CONTROL_LOCK(unit);
        SOC_CONTROL(unit)->scache_dirty = 1;
        SOC_CONTROL_UNLOCK(unit);
#endif
    } else if ((exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_MASK) == 
               _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS) {

        /* EGR_MPLS_EXP_MAPPING_3m profiles indexed by EXP */
        num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_3m) / 8;
        if (table_num >= num_exp_map) {
            return BCM_E_PARAM;
        }
        if (!_BCM_MPLS_EGR_EXP_TO_PHB_CNG_MAP_USED_GET(unit, table_num)) {
            return BCM_E_NOT_FOUND;
        }
        _BCM_MPLS_EGR_EXP_TO_PHB_CNG_MAP_USED_CLR(unit, table_num);
#ifdef BCM_WARM_BOOT_SUPPORT
        SOC_CONTROL_LOCK(unit);
        SOC_CONTROL(unit)->scache_dirty = 1;
        SOC_CONTROL_UNLOCK(unit);
#endif        
    } else {
        return BCM_E_PARAM;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_tr_mpls_exp_to_phb_cng_map_set
 * Purpose:
 *      Set the mapping of { PHB, CNG }
 *      to a EXP value for MPLS headers
 *      in the specified EXP map instance.
 * Parameters:
 *      unit        - (IN) SOC unit #
 *      flags       - (IN) flags
 *      exp_map_id  - (IN) EXP map ID
 *      map         - (IN) EXP map config structure 
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_exp_to_phb_cng_map_set(int unit, int exp_map_id,
                        bcm_mpls_exp_map_t *map)
{
    int table_num, num_exp_map, cng, index;
    egr_mpls_exp_mapping_3_entry_t exp_pri_entry;
    ing_mpls_exp_mapping_1_entry_t ing_pri_entry;
    
    /* Check validity of exp_map_id */
    if (exp_map_id < 0) {
        return BCM_E_PARAM;
    }

    if (!map) {
        /* invalid pointer to map data */
        return BCM_E_PARAM;
    }

    table_num = exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK;

    if ((exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_MASK) == 
        _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_INGRESS) {

        /* ING_MPLS_EXP_MAPPING_1 profiles indexed by EXP */
        num_exp_map = soc_mem_index_count(unit, ING_MPLS_EXP_MAPPING_1m) / 16;
        if (table_num >= num_exp_map) {
            return BCM_E_PARAM;
        }

        if (!_BCM_MPLS_ING_EXP_TO_PHB_CNG_MAP_USED_GET(unit, table_num)) {
            return BCM_E_NOT_FOUND;
        }

        cng = _BCM_COLOR_ENCODING(unit, map->color);
 
        /* Get the base index for this EXP map */
        index = (table_num * 8) + map->exp; 
 
        /* Commit to HW */
        BCM_IF_ERROR_RETURN(READ_ING_MPLS_EXP_MAPPING_1m(unit, MEM_BLOCK_ANY,
                                                       index, &ing_pri_entry));
        soc_ING_MPLS_EXP_MAPPING_1m_field32_set(unit, &ing_pri_entry, 
                                              PHBf, map->priority);
        soc_ING_MPLS_EXP_MAPPING_1m_field32_set(unit, &ing_pri_entry, 
                                              CNGf, cng);
        BCM_IF_ERROR_RETURN(WRITE_ING_MPLS_EXP_MAPPING_1m(unit, MEM_BLOCK_ALL, 
                                                        index, &ing_pri_entry));
    } else if 
        ((exp_map_id  & _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_MASK) == 
         _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS) {

        if ((map->exp > 7) || (map->pkt_pri > 7) || (map->pkt_cfi > 1)) {
            return BCM_E_PARAM;
        }

        /* EGR_MPLS_EXP_MAPPING_3m  profiles indexed by EXP */
        num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_PRI_MAPPINGm) / 8;
        if (table_num >= num_exp_map) {
            return BCM_E_PARAM;
        }

        cng = _BCM_COLOR_ENCODING(unit, map->color);

        /* index = Base index + Offset  for this EXP map */
        index = (table_num * 8) + map->exp; 

        if (!_BCM_MPLS_EGR_EXP_TO_PHB_CNG_MAP_USED_GET(unit, table_num)) {
            return BCM_E_NOT_FOUND;
        }

        BCM_IF_ERROR_RETURN(READ_EGR_MPLS_EXP_MAPPING_3m(unit, MEM_BLOCK_ANY,
                                                           index, &exp_pri_entry));
        soc_EGR_MPLS_EXP_MAPPING_3m_field32_set(unit, &exp_pri_entry, 
                                                  PHBf, map->priority);
        soc_EGR_MPLS_EXP_MAPPING_3m_field32_set(unit, &exp_pri_entry, 
                                                  CNGf, cng);
        BCM_IF_ERROR_RETURN(WRITE_EGR_MPLS_EXP_MAPPING_3m(unit, MEM_BLOCK_ALL, 
                                                            index, &exp_pri_entry));

    } else {
        return BCM_E_PARAM;
    }

    return BCM_E_NONE;
}
    
/*
 * Function:
 *      bcm_tr_mpls_exp_to_phb_cng_map_get
 * Purpose:
 *      Get the mapping of { PHB, CNG }
 *      to a EXP value for MPLS headers
 *      in the specified EXP map instance.
 * Parameters:
 *      unit        - (IN)  SOC unit #
 *      exp_map_id  - (IN)  EXP map ID
 *      map         - (OUT) EXP map config structure 
 * Returns:
 *      BCM_E_XXX
 */     
int     
bcm_tr_mpls_exp_to_phb_cng_map_get(int unit, int exp_map_id,
                        bcm_mpls_exp_map_t *map)
{
    int num_exp_map, cng, index, table_num;
    egr_mpls_exp_mapping_3_entry_t exp_entry;
    ing_mpls_exp_mapping_1_entry_t ing_pri_entry;


    if (exp_map_id < 0) {
        return BCM_E_PARAM;
    }

    table_num = exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_NUM_MASK;

    if ((exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_MASK) == 
        _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_INGRESS) {

        /* ING_MPLS_EXP_MAPPING_1 profiles indexed by EXP */
        num_exp_map = soc_mem_index_count(unit, ING_MPLS_EXP_MAPPING_1m) / 16;

        if ((map == NULL) || (table_num >= num_exp_map)) {
            return BCM_E_PARAM;
        }
        if (!_BCM_MPLS_ING_EXP_TO_PHB_CNG_MAP_USED_GET(unit, table_num)) {
            return BCM_E_NOT_FOUND;
        }

        /* Get the base index for this EXP map */
        index = (table_num * 8) + map->exp; 

        /* Commit to HW */
        BCM_IF_ERROR_RETURN(READ_ING_MPLS_EXP_MAPPING_1m(unit, MEM_BLOCK_ANY,
                                                       index, &ing_pri_entry));
        map->priority = soc_mem_field32_get(unit, ING_MPLS_EXP_MAPPING_1m, 
                                            &ing_pri_entry, PHBf);
        cng = soc_mem_field32_get(unit, ING_MPLS_EXP_MAPPING_1m,
                                  &ing_pri_entry, CNGf);
        map->color = _BCM_COLOR_DECODING(unit, cng);

    } else if
        ((exp_map_id & _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_MASK) == 
         _BCM_TR_MPLS_EXP_MAP_TABLE_TYPE_EGRESS) {
        /* EGR_MPLS_EXP_MAPPING_1 profiles indexed by EXP*/
        num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_3m) / 8;
    
        if ((map == NULL) || (table_num >= num_exp_map)) {
            return BCM_E_PARAM;
        }

        if (!_BCM_MPLS_EGR_EXP_TO_PHB_CNG_MAP_USED_GET(unit, table_num)) {
            return BCM_E_NOT_FOUND;
        }

        /* Get the base index for this EXP map */
        index = (table_num * 8) + map->exp;

        /* Get the values from HW */
        BCM_IF_ERROR_RETURN(READ_EGR_MPLS_EXP_MAPPING_3m(unit, MEM_BLOCK_ANY,
                                                         index, &exp_entry));
        map->priority = soc_mem_field32_get(unit, EGR_MPLS_EXP_MAPPING_3m, 
                                       &exp_entry, PHBf);
    
        cng = soc_mem_field32_get(unit, EGR_MPLS_EXP_MAPPING_3m,
                                  &exp_entry, CNGf);
        map->color = _BCM_COLOR_DECODING(unit, cng);
    } else {
        return BCM_E_PARAM;
    }
    return BCM_E_NONE;
}

int 
bcm_tr_mpls_exp_to_phb_cng_map_usage_get(int unit, uint32 flags, 
                                           int exp_map_id)
{   
    if((flags & BCM_QOS_MAP_INGRESS) && 
            (!_BCM_MPLS_ING_EXP_TO_PHB_CNG_MAP_USED_GET(unit, exp_map_id))) {
        return BCM_E_NOT_FOUND;
    }
    else if ((flags & BCM_QOS_MAP_INGRESS) && 
            (!_BCM_MPLS_EGR_EXP_TO_PHB_CNG_MAP_USED_GET(unit, exp_map_id))) {
        return BCM_E_NOT_FOUND;
    }

    return BCM_E_NONE;
}

int
bcm_tr_mpls_exp_to_phb_cng_map_profile_id_get(int unit, uint32 flags, 
                                           int map_id, int *profile_id)
{
    if(flags & BCM_QOS_MAP_INGRESS){
        *profile_id = MPLS_INFO(unit)->ing_exp_to_phb_cng_map_bitmap[map_id];
    } else if (flags & BCM_QOS_MAP_EGRESS){
        *profile_id = MPLS_INFO(unit)->egr_exp_to_phb_cng_map_bitmap[map_id];
    } else {
        return BCM_E_PARAM; 
    }
    return BCM_E_NONE;
}

int
bcm_tr_mpls_exp_to_phb_cng_map_profile_range_get(int unit, uint32 flags, 
                                              int *count)
{
    int num_exp_map = 0, value = 0;

    if(flags & BCM_QOS_MAP_INGRESS){
        num_exp_map = soc_mem_index_count(unit, ING_MPLS_EXP_MAPPING_1m) / 16;
        SHR_BITCOUNT_RANGE(MPLS_INFO(unit)->ing_exp_to_phb_cng_map_bitmap, 
                           value, 0, num_exp_map);
    } else if (flags & BCM_QOS_MAP_EGRESS){
        num_exp_map = soc_mem_index_count(unit, EGR_MPLS_EXP_MAPPING_3m) / 8;
        SHR_BITCOUNT_RANGE(MPLS_INFO(unit)->egr_exp_to_phb_cng_map_bitmap, 
                           value, 0, num_exp_map);
    }
    *count = value;
    return BCM_E_NONE;
}


#endif

#ifdef BCM_TRIUMPH2_SUPPORT
STATIC int
_bcm_tr2_mpls_label_flex_stat_hw_index_set(int unit,
                                     _bcm_flex_stat_handle_t handle,
                                               int fs_idx, void *cookie)
{
    int rv;
    mpls_entry_entry_t *ment;

    /* The entry was found earlier, so we can use it here */
    ment = (mpls_entry_entry_t *) cookie;

    /* Update the entry appropriately */
    soc_mem_field32_set(unit, MPLS_ENTRYm, ment,
                        VINTF_CTR_IDXf, fs_idx);
    soc_mem_field32_set(unit, MPLS_ENTRYm, ment,
                        USE_VINTF_CTR_IDXf, (fs_idx > 0) ? 1 : 0);
 
    /* Replace the existing entry */
    rv = soc_mem_insert(unit, MPLS_ENTRYm, MEM_BLOCK_ALL, ment);
    if (BCM_E_EXISTS == rv) {
        return BCM_E_NONE;
    } else if (BCM_SUCCESS(rv)) {
        rv = BCM_E_NOT_FOUND;
    }

    return rv;
}

STATIC _bcm_flex_stat_t
_bcm_esw_mpls_label_stat_to_flex_stat(bcm_mpls_stat_t stat)
{
    _bcm_flex_stat_t flex_stat;

    /* Only ingress stats from the MPLS_ENTRY table. */
    switch (stat) {
    case bcmMplsInPkts:
        flex_stat = _bcmFlexStatIngressPackets;
        break;
    case bcmMplsInBytes:
        flex_stat = _bcmFlexStatIngressBytes;
        break;
    default:
        flex_stat = _bcmFlexStatNum;
    }

    return flex_stat;
}

/*
 * Function:
 *      bcm_tr2_mpls_label_stat_enable_set
 * Purpose:
 *      Enable statistics collection for MPLS label or MPLS gport
 * Parameters:
 *      unit - (IN) Unit number.
 *      label - (IN) MPLS label
 *      port - (IN) MPLS gport
 *      enable - (IN) Non-zero to enable counter collection, zero to disable.
 *      cnt_inx - (IN) counter index. zero means not allocated
 * Returns:
 *      BCM_E_xxx
 * Notes:
 */
int 
bcm_tr2_mpls_label_stat_enable_set(int unit, bcm_mpls_label_t label, 
                                   bcm_gport_t port, int enable,int cnt_inx)
{
    /* This enables the flexible statistics gathering for MPLS labels */
    int rv=BCM_E_UNAVAIL;
    int index;
    bcm_mpls_tunnel_switch_t info;
    mpls_entry_entry_t ment;
    _bcm_flex_stat_handle_t mpls_fsh;

    if (BCM_GPORT_IS_MPLS_PORT(port)) {
        /* When the gport is an MPLS port, we'll turn on the
         * port-based tracking for backward-compatibility.
         */
        return _bcm_esw_flex_stat_enable_set(unit, _bcmFlexStatTypeGport,
                     _bcm_esw_port_flex_stat_hw_index_set,
                     cnt_inx? INT_TO_PTR(_BCM_FLEX_STAT_HW_INGRESS): NULL,
                                          port, enable,cnt_inx);
    }

    sal_memset(&info, 0, sizeof(bcm_mpls_tunnel_switch_t));
    info.port = port;
    if (BCM_XGS3_L3_MPLS_LBL_VALID(label)) {
        info.label = label;
    } else {
        return BCM_E_PARAM;
    }

    rv = _bcm_tr_mpls_entry_set_key(unit, &info, &ment);
    BCM_IF_ERROR_RETURN(rv);

    /* We don't use the handle retrieval function here because
     * we've already found the matching entry from which to extract
     * the key.  
     */
    /* Fill handle for MPLS label info */
    _BCM_FLEX_STAT_HANDLE_CLEAR(mpls_fsh);
    /* Handle is key of MPLS entry */
    _BCM_FLEX_STAT_HANDLE_COPY(mpls_fsh, ment);

    rv = soc_mem_search(unit, MPLS_ENTRYm, MEM_BLOCK_ANY, &index,
                        &ment, &ment, 0);

    if (rv == SOC_E_NONE) {
        /* Entry exists, and we are inside MPLS lock, so proceed with
         * attaching the counter. */
        rv = _bcm_esw_flex_stat_ext_enable_set(unit,
                                               _bcmFlexStatTypeMplsLabel,
                             _bcm_tr2_mpls_label_flex_stat_hw_index_set,
                                    (void *) &ment, mpls_fsh, enable,cnt_inx);
    }

    return rv;
}
#endif /* BCM_TRIUMPH2_SUPPORT */

/*
 * Function:
 *      bcm_tr_mpls_label_stat_get
 * Purpose:
 *      Get L2 MPLS PW Stats
 *      if sync_mode is set, sync the sw accumulated count
 *      with hw count value first, else return sw count.  
 * Parameters:
 *      unit      - (IN) SOC unit #
 *      sync_mode - (IN) hwcount is to be synced to sw count 
 *      label     - (IN) MPLS label
 *      port      - (IN) MPLS gport
 *      stat      - (IN)  specify the Stat type
 *      val       - (OUT) 64-bit Stats value
 * Returns:
 *      BCM_E_XXX
 */     

int
bcm_tr_mpls_label_stat_get(int unit, int sync_mode, bcm_mpls_label_t label, 
                           bcm_gport_t port, bcm_mpls_stat_t stat, uint64 *val)
{
    int rv=BCM_E_UNAVAIL, index;
    bcm_mpls_tunnel_switch_t info;
    mpls_entry_entry_t ment;
    ing_pw_term_counters_entry_t cent;
    egr_pw_init_counters_entry_t pw_init_entry;
    egr_l3_next_hop_entry_t egr_nh;
    ing_dvp_table_entry_t dvp;
    int vp, nh_index;
    uint32  hw_val[2];
    uint32 val_high, val_low;
    uint32 pkt_counter;
    uint32 pw_term_cnt, pw_init_cnt;

    /* Based on Switch Type */
#ifdef BCM_TRIUMPH2_SUPPORT
    if (soc_feature(unit, soc_feature_gport_service_counters)) {
        _bcm_flex_stat_handle_t mpls_fsh;

        COMPILER_64_ZERO(*val);

        if (BCM_GPORT_IS_MPLS_PORT(port)) {
            bcm_port_stat_t flex_stat=0;
            switch (stat) {
            case bcmMplsInPkts:
                flex_stat = bcmPortStatIngressPackets;
                break;
            case bcmMplsInBytes:
                flex_stat = bcmPortStatIngressBytes;
                break;
            case bcmMplsOutPkts:
                flex_stat = bcmPortStatEgressPackets;
                break;
            case bcmMplsOutBytes:
                flex_stat = bcmPortStatEgressBytes;
                break;
            }

            rv = _bcm_esw_port_stat_get(unit, sync_mode, port, flex_stat, val);
        } else {

            /* When not an MPLS gport, we use the label-based stats. */
            sal_memset(&info, 0, sizeof(bcm_mpls_tunnel_switch_t));
            info.port = port;
            if (BCM_XGS3_L3_MPLS_LBL_VALID(label)) {
                info.label = label;
            } else {
                return BCM_E_PARAM;
            }

            rv = _bcm_tr_mpls_entry_set_key(unit, &info, &ment);
            BCM_IF_ERROR_RETURN(rv);

            _BCM_FLEX_STAT_HANDLE_CLEAR(mpls_fsh);
            _BCM_FLEX_STAT_HANDLE_COPY(mpls_fsh, ment);


            rv = _bcm_esw_flex_stat_ext_get(unit, sync_mode,
                                             _bcmFlexStatTypeMplsLabel,
                                            mpls_fsh,
                            _bcm_esw_mpls_label_stat_to_flex_stat(stat),
                                            val);
        }
    } else
#endif /* BCM_TRIUMPH2_SUPPORT */
    if (SOC_IS_TR_VL(unit)) {
   
        if ((stat == bcmMplsInBytes) || (stat == bcmMplsInPkts)) {
            sal_memset(&info, 0, sizeof(bcm_mpls_tunnel_switch_t));
            info.port = port;
            if (BCM_XGS3_L3_MPLS_LBL_VALID(label)) {
                info.label = label;
            } else {
                return BCM_E_PARAM;
            }

            rv = _bcm_tr_mpls_entry_set_key(unit, &info, &ment);
            BCM_IF_ERROR_RETURN(rv);

            rv = soc_mem_search(unit, MPLS_ENTRYm, MEM_BLOCK_ANY, &index,
                                &ment, &ment, 0);

            if (rv == SOC_E_NONE) {
                if (!soc_MPLS_ENTRYm_field32_get(unit, &ment, PW_TERM_NUM_VALIDf)) {
                    return BCM_E_CONFIG;
                }
                pw_term_cnt = soc_MPLS_ENTRYm_field32_get(unit, &ment, PW_TERM_NUMf);

                if (SOC_MEM_IS_VALID(unit, ING_PW_TERM_COUNTERSm)) {
                    rv = READ_ING_PW_TERM_COUNTERSm(unit, MEM_BLOCK_ANY, pw_term_cnt, &cent);
                    if (rv < 0) {
                        return rv;
                    }
                } else {
                    return BCM_E_UNAVAIL;
                }
                COMPILER_64_ZERO(*val);
                switch (stat) {
                case bcmMplsInBytes:
                    if(SOC_IS_ENDURO(unit)) {
                        hw_val[0] = hw_val[1] = 0;
                        soc_mem_field_get(unit, ING_PW_TERM_COUNTERSm, 
                                          (uint32 *) &cent, BYTE_COUNTERf, hw_val);
                        COMPILER_64_SET(*val,  hw_val[1],hw_val[0]);
                    }
                    else {
                        val_high = soc_mem_field32_get(unit, ING_PW_TERM_COUNTERSm, 
                                                       &cent, BYTE_COUNTER_HIf);
                        val_low  = soc_mem_field32_get(unit, ING_PW_TERM_COUNTERSm, 
                                                       &cent, BYTE_COUNTER_LOf);
                        COMPILER_64_SET(*val, val_high, val_low);
                    }
                    break;

                case bcmMplsInPkts:
                    val_low  = soc_mem_field32_get(unit, ING_PW_TERM_COUNTERSm, 
                                                   &cent, PACKET_COUNTERf);
                    COMPILER_64_SET(*val, 0, val_low);
                    break;
                    /*
                     * COVERITY
                     *
                     * This default is unreachable. It is kept intentionally as a defensive default for future development. 
                     */ 
                    /* coverity[dead_error_begin] */
                default:
                    break;
                }
            }
        } else if ((stat == bcmMplsOutBytes) || (stat == bcmMplsOutPkts) ) {

            /* Get egress next-hop index from MPLS GPORT */
            vp = BCM_GPORT_MPLS_PORT_ID_GET(port);
            if (vp == -1) {
                 return BCM_E_PARAM;
            }

            rv = READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp, &dvp);
            if (rv < 0) {
                return rv;
            }
            nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp, 
                                                      NEXT_HOP_INDEXf);
     
            /* Retrieve EGR L3 NHOP Entry */
            BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                                              nh_index, &egr_nh));

            /* Retrieve PW_INIT Index from EGR L3 NHOP Entry */
            if (soc_feature(unit, soc_feature_mpls_enhanced)) {
                 pw_init_cnt =
                        soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                     MPLS__PW_INIT_NUMf);
            } else {
                 pw_init_cnt =
                        soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh,
                                     PW_INIT_NUMf);
            }

            /* Retrieve the Counter Value */
            sal_memset(&pw_init_entry, 0, sizeof(egr_pw_init_counters_entry_t));
            if (SOC_MEM_IS_VALID(unit, EGR_PW_INIT_COUNTERSm)) {	
                rv = READ_EGR_PW_INIT_COUNTERSm(unit, MEM_BLOCK_ANY, pw_init_cnt, &pw_init_entry);
                if (rv < 0) {
                    return rv;
                }
            } else {
                return BCM_E_UNAVAIL;
            }
            COMPILER_64_ZERO(*val);
            switch (stat) {
            case bcmMplsOutBytes:
                if (SOC_MEM_FIELD_VALID (unit, EGR_PW_INIT_COUNTERSm, BYTE_COUNTERf)) {
                    hw_val[0] = hw_val[1] = 0;
                    soc_mem_field_get(unit, EGR_PW_INIT_COUNTERSm, 
                                  (uint32 *) &pw_init_entry, BYTE_COUNTERf, hw_val);
                    COMPILER_64_SET(*val,  hw_val[1],hw_val[0]);
                } else {
                    return BCM_E_UNAVAIL;
                }
                break;

            case bcmMplsOutPkts:
                if (SOC_MEM_FIELD_VALID (unit, EGR_PW_INIT_COUNTERSm, PACKET_COUNTERf)) {
                    pkt_counter  = soc_mem_field32_get(unit, EGR_PW_INIT_COUNTERSm, 
                                                   &pw_init_entry, PACKET_COUNTERf); 
                    val_low = (pkt_counter - (pkt_counter/65536));
                    COMPILER_64_SET(*val, 0, val_low);
                } else {
                    return BCM_E_UNAVAIL;
                }
                break;
            default:
                break;
            }
        }
    }
    return rv;
}

int
bcm_tr_mpls_label_stat_get32(int unit, int sync_mode, 
                             bcm_mpls_label_t label, bcm_gport_t port,
                             bcm_mpls_stat_t stat, uint32 *val)
{
    int rv;
    uint64 val64;


    rv = bcm_tr_mpls_label_stat_get(unit, sync_mode, label, 
                                    port, stat, &val64);
    if (rv == BCM_E_NONE) {
        *val = COMPILER_64_LO(val64);
    }
    return rv;
}

/*
 * Function:
 *      bcm_tr_mpls_label_stat_clear
 * Purpose:
 *      Clear L2 MPLS PW Stats
 * Parameters:
 *      unit   - (IN) SOC unit #
 *      label  - (IN) MPLS label
 *      port   - (IN) MPLS gport
 *      stat   - (IN)  specify the Stat type
 * Returns:
 *      BCM_E_XXX
 */     

int
bcm_tr_mpls_label_stat_clear(int unit, bcm_mpls_label_t label, bcm_gport_t port,
                             bcm_mpls_stat_t stat)
{
    int rv=BCM_E_UNAVAIL, pw_term_cnt, pw_init_cnt, index;
    bcm_mpls_tunnel_switch_t info;
    mpls_entry_entry_t ment;
    ing_pw_term_counters_entry_t cent;
    egr_pw_init_counters_entry_t pw_init_entry;
    egr_l3_next_hop_entry_t egr_nh;
    ing_dvp_table_entry_t dvp;
    int vp, nh_index;

    /* Based on Switch Type */
#ifdef BCM_TRIUMPH2_SUPPORT
    if (soc_feature(unit, soc_feature_gport_service_counters)) {
        _bcm_flex_stat_handle_t mpls_fsh;
        bcm_port_stat_t flex_stat=0;
        uint64 flex_value;

        COMPILER_64_ZERO(flex_value);

        if (BCM_GPORT_IS_MPLS_PORT(port)) {
            switch (stat) {
            case bcmMplsInPkts:
                flex_stat = bcmPortStatIngressPackets;
                break;
            case bcmMplsInBytes:
                flex_stat = bcmPortStatIngressBytes;
                break;
            case bcmMplsOutPkts:
                flex_stat = bcmPortStatEgressPackets;
                break;
            case bcmMplsOutBytes:
                flex_stat = bcmPortStatEgressBytes;
                break;
            }

            COMPILER_64_ZERO(flex_value);
            rv = bcm_esw_port_stat_set(unit, port, flex_stat, flex_value);
        } else {
            sal_memset(&info, 0, sizeof(bcm_mpls_tunnel_switch_t));
            info.port = port;
            if (BCM_XGS3_L3_MPLS_LBL_VALID(label)) {
                info.label = label;
            } else {
                return BCM_E_PARAM;
            }

            rv = _bcm_tr_mpls_entry_set_key(unit, &info, &ment);
            BCM_IF_ERROR_RETURN(rv);

            _BCM_FLEX_STAT_HANDLE_CLEAR(mpls_fsh);
            _BCM_FLEX_STAT_HANDLE_COPY(mpls_fsh, ment);

            rv =  _bcm_esw_flex_stat_ext_set(unit, _bcmFlexStatTypeMplsLabel,
                                             mpls_fsh,
                            _bcm_esw_mpls_label_stat_to_flex_stat(stat),
                                             flex_value);
        }
    } else
#endif /* BCM_TRIUMPH2_SUPPORT */
    if (SOC_IS_TR_VL(unit)) {

        if ((stat == bcmMplsInBytes) || (stat == bcmMplsInPkts)) {
            sal_memset(&info, 0, sizeof(bcm_mpls_tunnel_switch_t));
            info.port = port;
            if (BCM_XGS3_L3_MPLS_LBL_VALID(label)) {
                info.label = label;
            } else {
                return BCM_E_PARAM;
            }

            rv = _bcm_tr_mpls_entry_set_key(unit, &info, &ment);
            BCM_IF_ERROR_RETURN(rv);

            rv = soc_mem_search(unit, MPLS_ENTRYm, MEM_BLOCK_ANY, &index,
                                &ment, &ment, 0);
            if (rv == SOC_E_NONE) {
                if (!soc_MPLS_ENTRYm_field32_get(unit, &ment, PW_TERM_NUM_VALIDf)) {
                    return BCM_E_CONFIG;
                }

                /* Retrieve PW_TERM Index from MPLS_Entry */
                pw_term_cnt = soc_MPLS_ENTRYm_field32_get(unit, &ment, PW_TERM_NUMf);

                /* Clear the Counter Value */
                sal_memset(&cent, 0, sizeof(ing_pw_term_counters_entry_t));
                rv = WRITE_ING_PW_TERM_COUNTERSm(unit, MEM_BLOCK_ANY, pw_term_cnt, &cent);
                if (rv < 0) {
                    return rv;
                }
            }
        } else if ((stat == bcmMplsOutBytes) || (stat == bcmMplsOutPkts) ) {

            /* Get egress next-hop index from MPLS GPORT */
            vp = BCM_GPORT_MPLS_PORT_ID_GET(port);
            if (vp == -1) {
               return BCM_E_PARAM;
            }

            rv = READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp, &dvp);
            if (rv < 0) {
                return rv;
            }
            nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp, 
                                                      NEXT_HOP_INDEXf);
     
            /* Retrieve EGR L3 NHOP Entry */
            BCM_IF_ERROR_RETURN (soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                                              nh_index, &egr_nh));

            /* Retrieve PW_INIT Index from EGR L3 NHOP Entry */
            pw_init_cnt = soc_EGR_L3_NEXT_HOPm_field32_get(unit, &egr_nh, PW_INIT_NUMf);

            /* Clear the Counter Value */
            sal_memset(&pw_init_entry, 0, sizeof(egr_pw_init_counters_entry_t));
            rv = WRITE_EGR_PW_INIT_COUNTERSm(unit, MEM_BLOCK_ANY, pw_init_cnt, &pw_init_entry);
            if (rv < 0) {
                return rv;
            }
        }
    }
    return rv;
}

/*
 * Function:
 *      bcm_tr_mpls_port_learn_set
 * Purpose:
 *      Set the CML bits for an mpls port.
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_port_learn_set(int unit, bcm_gport_t mpls_port_id, uint32 flags)
{
    int vp, cml = 0, rv = BCM_E_NONE;
    source_vp_entry_t svp;

    rv = _bcm_tr_mpls_check_init (unit);
    if (rv < 0) {
        return rv;
    }

    cml = 0;
    if (!(flags & BCM_PORT_LEARN_FWD)) {
       cml |= (1 << 0);
    }
    if (flags & BCM_PORT_LEARN_CPU) {
       cml |= (1 << 1);
    }
    if (flags & BCM_PORT_LEARN_PENDING) {
       cml |= (1 << 2);
    }
    if (flags & BCM_PORT_LEARN_ARL) {
       cml |= (1 << 3);
    }

    /* Get the VP index from the gport */
    vp = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port_id);
    if (vp == -1) {
        return BCM_E_PARAM;
    }

    /* Be sure the entry is used and is set for VPLS */
    if (!_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
        return BCM_E_NOT_FOUND;
    }
    rv = READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp);
    if (rv < 0) {
        return rv;
    }
    if (soc_SOURCE_VPm_field32_get(unit, &svp, ENTRY_TYPEf) != 1) { /* VPLS */
        return BCM_E_NOT_FOUND;
    }
    soc_SOURCE_VPm_field32_set(unit, &svp, CML_FLAGS_MOVEf, cml);
    soc_SOURCE_VPm_field32_set(unit, &svp, CML_FLAGS_NEWf, cml);
    rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp, &svp);
    return rv;
}

/*
 * Function:    
 *      bcm_tr_mpls_port_learn_get
 * Purpose:
 *      Get the CML bits for an mpls port
 * Returns: 
 *      BCM_E_XXX
 */     
int     
bcm_tr_mpls_port_learn_get(int unit, bcm_gport_t mpls_port_id, uint32 *flags)
{
    int rv, vp, cml = 0;
    source_vp_entry_t svp;
    
    rv = _bcm_tr_mpls_check_init (unit);
    if (rv < 0) {
        return rv;
    }
    
    /* Get the VP index from the gport */
    vp = BCM_GPORT_MPLS_PORT_ID_GET(mpls_port_id);
    if (vp == -1) {
       return BCM_E_PARAM;
    }

    /* Be sure the entry is used and is set for VPLS */
    if (!_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
        return BCM_E_NOT_FOUND;
    }
    rv = READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp);
    if (rv < 0) {
        return rv;
    }
    if (soc_SOURCE_VPm_field32_get(unit, &svp, ENTRY_TYPEf) != 1) { /* VPLS */
        return BCM_E_NOT_FOUND;
    }
    cml = soc_SOURCE_VPm_field32_get(unit, &svp, CML_FLAGS_NEWf);
    
    *flags = 0;
    if (!(cml & (1 << 0))) {
       *flags |= BCM_PORT_LEARN_FWD;
    }
    if (cml & (1 << 1)) {
       *flags |= BCM_PORT_LEARN_CPU;
    }
    if (cml & (1 << 2)) {
       *flags |= BCM_PORT_LEARN_PENDING;
    }
    if (cml & (1 << 3)) {
       *flags |= BCM_PORT_LEARN_ARL;
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_tr_mpls_mcast_flood_set
 * Purpose:
 *      Set the PFM for a VPLS VFI.
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_tr_mpls_mcast_flood_set(int unit, bcm_vlan_t vlan,
                            bcm_vlan_mcast_flood_t mode)
{
    int vfi, pfm = 0, rv = BCM_E_NONE;

    rv = _bcm_tr_mpls_check_init (unit);
    if (rv < 0) {
        return rv;
    }

    _BCM_MPLS_VPN_GET(vfi, _BCM_MPLS_VPN_TYPE_VPLS, vlan);

    switch (mode) {
    case BCM_VLAN_MCAST_FLOOD_ALL:
        pfm = 0;
        break;
    case BCM_VLAN_MCAST_FLOOD_NONE:
        pfm = 2;
        break;
    default:
        pfm = 1;
        break;
    }

    if (!_bcm_vfi_used_get(unit, vfi, _bcmVfiTypeMpls)) {
        return BCM_E_NOT_FOUND;
    }
    /* Update the HW entry */
    rv = soc_mem_field32_modify(unit, VFIm, vfi, PFMf, pfm);
    return rv;
}

/*
 * Function:
 *      bcm_tr_mpls_mcast_flood_get
 * Purpose:
 *      Get the PFM for a VPLS VFI.
 * Returns: 
 *      BCM_E_XXX
 */     
int
bcm_tr_mpls_mcast_flood_get(int unit, bcm_vlan_t vlan,
                            bcm_vlan_mcast_flood_t *mode)
{
    int vfi, pfm = 0, rv = BCM_E_NONE;
    vfi_entry_t vfi_entry;

    rv = _bcm_tr_mpls_check_init (unit);
    if (rv < 0) {
        return rv;
    }

    _BCM_MPLS_VPN_GET(vfi, _BCM_MPLS_VPN_TYPE_VPLS, vlan);

    if (!_bcm_vfi_used_get(unit, vfi, _bcmVfiTypeMpls)) {
        return BCM_E_NOT_FOUND;
    }
    rv = READ_VFIm(unit, MEM_BLOCK_ANY, vfi, &vfi_entry);
    if (rv < 0) {
        return rv;
    }
    pfm = soc_VFIm_field32_get(unit, &vfi_entry, PFMf);

    switch (pfm) {
    case 0:
        *mode = BCM_VLAN_MCAST_FLOOD_ALL;
        break;
    case 2:
        *mode = BCM_VLAN_MCAST_FLOOD_NONE;
        break;
    default:
        *mode = BCM_VLAN_MCAST_FLOOD_UNKNOWN;
        break;
    }
    return rv;
}

/*
 * Function:
 *      bcm_tr_mpls_port_phys_gport_get
 * Purpose:
 *      Get the PFM for a VPLS VFI.
 * Returns: 
 *      BCM_E_XXX
 */     

int 
bcm_tr_mpls_port_phys_gport_get(int unit, int vp, bcm_gport_t *gp)
{
    int nh_index;
    ing_dvp_table_entry_t dvp;
    ing_l3_next_hop_entry_t ing_nh;
    bcm_module_t mod_out, mod_in;
    bcm_port_t port_out, port_in;
    bcm_trunk_t trunk_id;

    BCM_IF_ERROR_RETURN(READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp, &dvp));
    nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp, NEXT_HOP_INDEXf);
    BCM_IF_ERROR_RETURN(soc_mem_read(unit, ING_L3_NEXT_HOPm, MEM_BLOCK_ANY, 
                                     nh_index, &ing_nh));

    if (soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, Tf)) {
        trunk_id = soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, TGIDf);
        BCM_GPORT_TRUNK_SET(*gp, trunk_id);
    } else {
        mod_in = soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, MODULE_IDf);
        port_in = soc_ING_L3_NEXT_HOPm_field32_get(unit, &ing_nh, PORT_NUMf);
        BCM_IF_ERROR_RETURN
            (_bcm_esw_stk_modmap_map(unit, BCM_STK_MODMAP_GET, mod_in, port_in, 
                                    &mod_out, &port_out));
        BCM_GPORT_MODPORT_SET(*gp, mod_out, port_out);
    }

    return BCM_E_NONE;
}

#ifndef BCM_SW_STATE_DUMP_DISABLE
/*
 * Function:
 *     _bcm_mpls_sw_dump
 * Purpose:
 *     Displays MPLS State information maintained by software.
 * Parameters:
 *     unit - Device unit number
 * Returns:
 *     None
 */
void
_bcm_mpls_sw_dump(int unit)
{
    _bcm_tr_mpls_bookkeeping_t *mpls_info = MPLS_INFO(unit);
    int i, num_vp, num_vc, num_tnl, num_trunk;
#ifdef BCM_KATANA_SUPPORT
    int num_queue = 0;
#endif /* BCM_KATANA_SUPPORT */

    if (!mpls_info->initialized) {
        LOG_CLI((BSL_META_U(unit,
                            "\nMPLS data structure is not initialized\n")));
        return;
    }

    LOG_CLI((BSL_META_U(unit,
                        "\nSW Information MPLS - Unit %d\n"), unit));

    num_vp = soc_mem_index_count(unit, SOURCE_VPm);

    LOG_CLI((BSL_META_U(unit,
                        "\n  Match Info    : \n")));
    for (i = 0; i < num_vp; i++) {
        if ((mpls_info->match_key[i].trunk_id == -1) && 
            (mpls_info->match_key[i].modid == -1) &&
            (mpls_info->match_key[i].port == 0)) {
            continue;
        }
        LOG_CLI((BSL_META_U(unit,
                            "\n  MPLS port vp = %d\n"), i));
        LOG_CLI((BSL_META_U(unit,
                            "Flags = %x\n"), mpls_info->match_key[i].flags));
        LOG_CLI((BSL_META_U(unit,
                            "Index = %x\n"), mpls_info->match_key[i].index));
        LOG_CLI((BSL_META_U(unit,
                            "TGID = %d\n"), mpls_info->match_key[i].trunk_id));
        LOG_CLI((BSL_META_U(unit,
                            "Modid = %d\n"), mpls_info->match_key[i].modid));
        LOG_CLI((BSL_META_U(unit,
                            "Port = %d\n"), mpls_info->match_key[i].port));
        LOG_CLI((BSL_META_U(unit,
                            "Match VLAN = %d\n"), 
                 mpls_info->match_key[i].match_vlan));
        LOG_CLI((BSL_META_U(unit,
                            "Match Inner VLAN = %d\n"), 
                 mpls_info->match_key[i].match_inner_vlan));
        LOG_CLI((BSL_META_U(unit,
                            "Match Label = %d\n"), 
                 mpls_info->match_key[i].match_label));
        LOG_CLI((BSL_META_U(unit,
                            "Match Count = %d\n"),
                 mpls_info->match_key[i].match_count));
    }
    
    num_vc = soc_mem_index_count(unit, EGR_MPLS_VC_AND_SWAP_LABEL_TABLEm);
    for (i = 0; i < num_vc; i++) {
        if (mpls_info->vc_swap_ref_count[i]) {
              LOG_CLI((BSL_META_U(unit,
                                  "Label = %d  RefCount=%d\n"), 
                       i, mpls_info->vc_swap_ref_count[i]));
        }
    }

    /* VPWS VPs to VPN mapping */
    
    LOG_CLI((BSL_META_U(unit,
                        "\nVPN to VPWS VP mapping:\n")));
    LOG_CLI((BSL_META_U(unit,
                        "-----------------------\n")));
    LOG_CLI((BSL_META_U(unit,
                        "VPN      VPWS VPs\n")));
    for (i=0; i<(num_vp/2); i++) {
        if (mpls_info->vpws_vp_map[i].vp1 != -1 &&
            mpls_info->vpws_vp_map[i].vp2 != -1) {
            LOG_CLI((BSL_META_U(unit,
                                "%d     %d & %d\n"),(0x2000+i), 
                     mpls_info->vpws_vp_map[i].vp1,
                     mpls_info->vpws_vp_map[i].vp2));
        }
    }

    num_tnl = soc_mem_index_count(unit, EGR_IP_TUNNEL_MPLSm);
    for (i = 0; i < num_tnl * _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit); i++) {
        if (mpls_info->egr_tunnel_ref_count[i]) {
              LOG_CLI((BSL_META_U(unit,
                                  "Egress MPLS Tunnel = %d  RefCount=%d\n"), 
                       i, mpls_info->egr_tunnel_ref_count[i]));
        }
    }

    /* Print VP - NH mapping */
    LOG_CLI((BSL_META_U(unit,
                        "\n VP to NH mapping:\n")));
    LOG_CLI((BSL_META_U(unit,
                        "-----------------------\n")));
    _bcm_tr_mpls_dump_egrNhop(unit);
    LOG_CLI((BSL_META_U(unit,
                        "\n")));
#ifdef BCM_KATANA_SUPPORT
    if (soc_feature(unit, soc_feature_extended_queueing)) {
        num_queue = soc_mem_index_count(unit, ING_QUEUE_MAPm);
        LOG_CLI((BSL_META_U(unit,"\n Ingress Queue map reference count:\n")));
        LOG_CLI((BSL_META_U(unit,"-----------------------\n")));
        for (i = 0; i < num_queue; i++) {
            if (mpls_info->ing_qmap_ref_count[i]) {
                  LOG_CLI((BSL_META_U(unit,
                               "\n Ingress Queue Map Index = %d  RefCount=%d\n"),
                                      i, mpls_info->ing_qmap_ref_count[i]));
            }
        }
    }
#endif /* BCM_KATANA_SUPPORT */

    /* Trunk to VP mapping */
    num_trunk = soc_mem_index_count(unit, TRUNK_GROUPm);
    LOG_CLI((BSL_META_U(unit,
                        "\nTrunk to VP mapping:\n")));
    LOG_CLI((BSL_META_U(unit,
                        "-----------------------\n")));
    LOG_CLI((BSL_META_U(unit,
                        "TRUNK      VP\n")));
    for (i = 0; i < num_trunk; i++) {
        if (mpls_info->trunk_vp_map[i] != -1) {
            LOG_CLI((BSL_META_U(unit,
                                "%d     %d\n"),
                    i, mpls_info->trunk_vp_map[i]));
        }
    }
#if defined BCM_TRIUMPH2_SUPPORT
    if (soc_feature(unit, soc_feature_mpls_failover)) { 
        for (i = 0; i < num_vp; i++) {
            if (mpls_info->failover_vp[i] == 0) {
                continue;
            }
            LOG_CLI((BSL_META_U(unit,
                                "\n  MPLS vp %d failover vp = %d\n"), i, 
                                mpls_info->failover_vp[i]));
        }
    }
#endif
    return;
}
#endif /* BCM_SW_STATE_DUMP_DISABLE */


#ifdef BCM_TRIUMPH2_SUPPORT
int
_bcm_esw_mpls_flex_stat_index_set(int unit, bcm_gport_t port, int fs_idx,uint32 flags)
{
    int rv, vp, nh_index=0;
    source_vp_entry_t svp;
    ing_dvp_table_entry_t dvp;
    egr_l3_next_hop_entry_t egr_nh;
    uint32 entry_type=0;
    initial_prot_nhi_table_entry_t prot_entry;
    int vpless_failover_port = FALSE;

    if (_BCM_MPLS_GPORT_FAILOVER_VPLESS_GET(port)) {
        vpless_failover_port = TRUE;
        port = _BCM_MPLS_GPORT_FAILOVER_VPLESS_CLEAR(port);
    }

    vp = BCM_GPORT_MPLS_PORT_ID_GET(port);
    if (vp == -1) {
        return BCM_E_PARAM;
    }

    rv = bcm_tr_mpls_lock(unit);
    if (BCM_FAILURE(rv)) {
        return rv;
    }
    if (!_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
        bcm_tr_mpls_unlock(unit);
        return BCM_E_NOT_FOUND;
    } else {
        /* Ingress side */
        if (flags & _BCM_FLEX_STAT_HW_INGRESS) {
            rv = soc_mem_read(unit, SOURCE_VPm, MEM_BLOCK_ANY, vp, &svp);
            if (BCM_SUCCESS(rv)) {
                if (soc_mem_field_valid(unit, SOURCE_VPm, USE_VINTF_CTR_IDXf)) {
                    soc_mem_field32_set(unit, SOURCE_VPm, &svp, USE_VINTF_CTR_IDXf,
                                        fs_idx > 0 ? 1 : 0);
                }
                soc_mem_field32_set(unit, SOURCE_VPm, &svp, VINTF_CTR_IDXf,
                                    fs_idx);
                rv = soc_mem_write(unit, SOURCE_VPm, MEM_BLOCK_ALL, vp, &svp);
            }
        }
        /* Egress side */
        if (flags & _BCM_FLEX_STAT_HW_EGRESS) {
            if (BCM_SUCCESS(rv)) {
                rv = READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp, &dvp);
                if (BCM_SUCCESS(rv)) {
                    nh_index = soc_mem_field32_get(unit, ING_DVP_TABLEm, &dvp,
                                                   NEXT_HOP_INDEXf);
                    if (vpless_failover_port == TRUE) {
                        rv = READ_INITIAL_PROT_NHI_TABLEm(unit,MEM_BLOCK_ANY, nh_index, &prot_entry);
                        if (BCM_SUCCESS(rv)) {
                            nh_index = soc_INITIAL_PROT_NHI_TABLEm_field32_get(unit,&prot_entry,
                                       PROT_NEXT_HOP_INDEXf);
                        }
                    }
                }
            }
            if (BCM_SUCCESS(rv)) {
                rv = soc_mem_read(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ANY, nh_index,
                                  &egr_nh);
            }
            if (BCM_SUCCESS(rv)) {
                entry_type = soc_mem_field32_get(unit, EGR_L3_NEXT_HOPm, &egr_nh, ENTRY_TYPEf);
                if(entry_type == 1) {
                     if (soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm,
                                    MPLS__USE_VINTF_CTR_IDXf)) {
                            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                    MPLS__USE_VINTF_CTR_IDXf,
                                    fs_idx > 0 ? 1 : 0);
                     }
                     soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                    MPLS__VINTF_CTR_IDXf, fs_idx);
                  } else if (entry_type == 2) {
                     if (soc_mem_field_valid(unit, EGR_L3_NEXT_HOPm,
                                    SD_TAG__USE_VINTF_CTR_IDXf)) {
                            soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                    SD_TAG__USE_VINTF_CTR_IDXf,
                                    fs_idx > 0 ? 1 : 0);
                     }
                     soc_mem_field32_set(unit, EGR_L3_NEXT_HOPm, &egr_nh,
                                    SD_TAG__VINTF_CTR_IDXf, fs_idx);
                }
                rv = soc_mem_write(unit, EGR_L3_NEXT_HOPm, MEM_BLOCK_ALL, nh_index,
                                     &egr_nh);
            }
        }
    }

    bcm_tr_mpls_unlock(unit);
    return rv;
}

int
_bcm_tr2_mpls_svp_field_set(int unit, bcm_gport_t vp, 
                           soc_field_t field, int value)
{
    int rv = BCM_E_NONE;
    source_vp_entry_t svp;

    rv = bcm_tr_mpls_lock(unit);
    if (BCM_FAILURE(rv)) {
        return rv;
    }
    rv = READ_SOURCE_VPm(unit, MEM_BLOCK_ANY, vp, &svp);
    if (rv < 0) {
        bcm_tr_mpls_unlock(unit);
        return rv;
    }
    if (SOC_MEM_FIELD_VALID(unit, SOURCE_VPm, field)) {
        soc_SOURCE_VPm_field32_set(unit, &svp, field, value);
    } else {
        bcm_tr_mpls_unlock(unit);
        return BCM_E_UNAVAIL;
    }
    rv = WRITE_SOURCE_VPm(unit, MEM_BLOCK_ALL, vp, &svp);
    bcm_tr_mpls_unlock(unit);
    return rv;
}

#endif /* BCM_TRIUMPH2_SUPPORT */

int _bcm_tr_mpls_vpws_egress_if_get(int unit, bcm_gport_t gport, bcm_if_t *egress_if)
{
    int vp = -1;
    uint32 nh_index;
    ing_dvp_table_entry_t dvp;

    vp = BCM_GPORT_MPLS_PORT_ID_GET(gport);
    if (vp == -1) {
        return BCM_E_PARAM;
    }

    if (!_bcm_vp_used_get(unit, vp, _bcmVpTypeMpls)) {
        return BCM_E_PARAM;
    }
    
    BCM_IF_ERROR_RETURN (READ_ING_DVP_TABLEm(unit, MEM_BLOCK_ANY, vp, &dvp));
    nh_index = soc_ING_DVP_TABLEm_field32_get(unit, &dvp, NEXT_HOP_INDEXf);
    *egress_if = nh_index + BCM_XGS3_DVP_EGRESS_IDX_MIN;

    return BCM_E_NONE;
}
#else /* BCM_TRIUMPH_SUPPORT && BCM_MPLS_SUPPORT && INCLUDE_L3 */
int bcm_tr_mpls_not_empty;
#endif /* BCM_TRIUMPH_SUPPORT && BCM_MPLS_SUPPORT && INCLUDE_L3 */
