/*
 * $Id: $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * COS Queue Management
 * Purpose: API to set different cosq, priorities, and scheduler registers.
 */

#include <shared/bsl.h>

#include <soc/defs.h>
#include <sal/core/libc.h>

#if defined(BCM_TOMAHAWK_SUPPORT)
#include <soc/drv.h>
#include <soc/scache.h>
#include <soc/profile_mem.h>
#include <soc/debug.h>
#include <soc/tomahawk.h>
#include <soc/mmu_config.h>


#include <bcm/debug.h>
#include <bcm/error.h>
#include <bcm/cosq.h>
#include <bcm_int/esw/mbcm.h>
#include <bcm_int/esw/stack.h>
#include <bcm_int/esw/cosq.h>
#include <bcm_int/esw/virtual.h>
#include <bcm_int/esw/tomahawk.h>
#include <bcm_int/esw/ecn.h>
#include <bcm_int/esw/oob.h>
#if defined(INCLUDE_PSTATS)
#include <bcm_int/esw/pstats.h>
#endif /* INCLUDE_PSTATS */
#if defined(BCM_TOMAHAWK2_SUPPORT)
#include <bcm_int/esw/tomahawk2.h>
#include <soc/tomahawk2.h>
#endif
#include <bcm_int/esw/pfc_deadlock.h>
#if defined (INCLUDE_TCB) && defined (BCM_TCB_SUPPORT)
#include <bcm_int/esw/tcb.h>
#endif
#include <bcm_int/esw_dispatch.h>

#include <bcm_int/bst.h>

#ifdef BCM_WARM_BOOT_SUPPORT
#include <bcm_int/esw/switch.h>
#endif /* BCM_WARM_BOOT_SUPPORT */

#ifdef BCM_CMICM_SUPPORT
#include <soc/cmicm.h>
#endif

#define _BCM_TH_NUM_UCAST_QUEUE_PER_PORT 10
#define _BCM_TH_NUM_MCAST_QUEUE_PER_PORT 10
#define _BCM_TH_NUM_SCHEDULER_PER_PORT   10
#define _BCM_TH_NUM_CPU_MCAST_QUEUE      48
#define _BCM_TH_NUM_BUFFER_PER_PIPE      2

#define TH_WRED_CELL_FIELD_MAX       0xffff

#define _TH_NUM_PIPES 4

/* COS default value for TH is assumed to be 8 */
#define BCM_TH_COS_DEFAULT  8
/* COS max value for TH is assumed to be 10 */
#define BCM_TH_COS_MAX  10

/* TH supports 16 SAFC priority */
#define _BCM_TH_NUM_SAFC_CLASS  16

/* TH supports 8 PFC priority */
#define _BCM_TH_NUM_PFC_CLASS   8

#define _BCM_TH_NUM_TIME_DOMAIN    4

typedef struct _bcm_th_cosq_time_info_s {
    uint32 ref_count;   
} _bcm_th_cosq_time_info_t;

/* WRED can be enabled at per UC queue / port / service pool basis
MMU_WRED_AVG_QSIZE used entry: (330 + 132 + 4) = 466
329 ~   0: Unicast Queue
461 ~ 330: Port Service Pool (port 0 to 32)           
                   Address = 330 + {PORTID[5:0], SPID[1:0]} per PIPE           
                   Each Port's service pool is addressed by Address[1:0] of this memory address.
465 ~ 462: Global Service Pool      
                   462: Global Service Pool 0      
                   463: Global Service Pool 1      
                   464: Global Service Pool 2      
                   465: Global Service Pool 3
*/
STATIC _bcm_th_cosq_time_info_t time_domain_info[_BCM_TH_NUM_TIME_DOMAIN] = {
    {466}, /* Default all entries are pointing to TIME_0 */
    {0},
    {0},
    {0}
};

typedef struct _bcm_th_cosq_cpu_cosq_config_s {
    /* All MC queues have DB[idx 0] and MCQE[idx 1] space */
    int q_min_limit[2];
    int q_shared_limit[2];
    uint8 q_limit_dynamic[2];
    uint8 q_limit_enable[2];
    uint8 q_color_limit_enable[2];
    uint8 enable;
} _bcm_th_cosq_cpu_cosq_config_t;

typedef struct _bcm_th_cosq_node_s {
    bcm_gport_t gport;
    int numq;
    int level;
    int hw_index;
    int in_use;
    bcm_gport_t parent_gport;
} _bcm_th_cosq_node_t;

typedef struct _bcm_th_cosq_port_info_s {
    _bcm_th_cosq_node_t ucast[_BCM_TH_NUM_UCAST_QUEUE_PER_PORT];
    _bcm_th_cosq_node_t mcast[_BCM_TH_NUM_MCAST_QUEUE_PER_PORT];
    _bcm_th_cosq_node_t sched[_BCM_TH_NUM_SCHEDULER_PER_PORT];
} _bcm_th_cosq_port_info_t;

typedef struct _bcm_th_cosq_cpu_port_info_s {
    _bcm_th_cosq_node_t sched[_BCM_TH_NUM_SCHEDULER_PER_PORT];
    _bcm_th_cosq_node_t mcast[_BCM_TH_NUM_CPU_MCAST_QUEUE];
} _bcm_th_cosq_cpu_port_info_t;

typedef struct _bcm_th_mmu_info_s {
    int gport_tree_created; /* FALSE: No GPORT tree exists */
    _bcm_th_cosq_port_info_t *_bcm_th_port_info;
    _bcm_th_cosq_cpu_port_info_t *_bcm_th_cpu_port_info;
    int ing_shared_limit[_TH_XPES_PER_DEV];
    int egr_db_shared_limit[_TH_XPES_PER_DEV];
    int egr_qe_shared_limit[_TH_XPES_PER_DEV];
} _bcm_th_mmu_info_t;

STATIC _bcm_th_mmu_info_t *_bcm_th_mmu_info[BCM_MAX_NUM_UNITS]; /* MMU info */
STATIC _bcm_th_cosq_port_info_t *_bcm_th_cosq_port_info[BCM_MAX_NUM_UNITS];
STATIC _bcm_th_cosq_cpu_port_info_t *_bcm_th_cosq_cpu_port_info[BCM_MAX_NUM_UNITS];
STATIC soc_profile_mem_t *_bcm_th_cos_map_profile[BCM_MAX_NUM_UNITS];
STATIC soc_profile_mem_t *_bcm_th_wred_profile[BCM_MAX_NUM_UNITS];
STATIC soc_profile_reg_t *_bcm_th_prio2cos_profile[BCM_MAX_NUM_UNITS];
STATIC soc_profile_mem_t *_bcm_th_ifp_cos_map_profile[BCM_MAX_NUM_UNITS];
STATIC _bcm_th_cosq_cpu_cosq_config_t
            *_bcm_th_cosq_cpu_cosq_config[BCM_MAX_NUM_UNITS][SOC_TH_NUM_CPU_QUEUES];

typedef enum {
    _BCM_TH_COSQ_TYPE_UCAST,
    _BCM_TH_COSQ_TYPE_MCAST
} _bcm_th_cosq_type_t;

/* Number of COSQs configured for this unit */
#define _TH_NUM_COS(unit) NUM_COS(unit)

#define _TH_PORT_NUM_COS(unit, port) \
    ((IS_CPU_PORT(unit, port)) ? NUM_CPU_COSQ(unit) : \
        (IS_LB_PORT(unit, port)) ? 10 : _TH_NUM_COS(unit))


#define _BCM_TH_MMU_INFO(unit) _bcm_th_mmu_info[(unit)]

#ifdef BCM_WARM_BOOT_SUPPORT
#define BCM_WB_VERSION_1_0                SOC_SCACHE_VERSION(1,0)
#define BCM_WB_VERSION_1_1                SOC_SCACHE_VERSION(1,1)
#define BCM_WB_VERSION_1_2                SOC_SCACHE_VERSION(1,2)
#define BCM_WB_VERSION_1_3                SOC_SCACHE_VERSION(1,3)
#define BCM_WB_DEFAULT_VERSION            BCM_WB_VERSION_1_3

STATIC int
_bcm_th_cosq_wb_alloc(int unit)
{
    _bcm_th_mmu_info_t *mmu_info = NULL;
    soc_scache_handle_t scache_handle;
    uint8 *scache_ptr = NULL;
    int rv, alloc_size = 0, max_size = 4096 * 2;
    _bcm_th_cosq_cpu_port_info_t *cpu_port_info = NULL;

    mmu_info = _bcm_th_mmu_info[unit];
    if (mmu_info == NULL) {
        return BCM_E_INIT;
    }

    cpu_port_info = _bcm_th_cosq_cpu_port_info[unit];
    if (cpu_port_info == NULL) {
        return BCM_E_INIT;
    }

    /*
     * Sync _bcm_th_mmu_info[unit]->ing_shared_limit/
     * egr_db_shared_limit/egr_db_shared_limit
     */
    alloc_size += 3 * sizeof(int);

    /*
     * Sync _bcm_th_cosq_cpu_port_info structure
     */

    alloc_size += sizeof(_bcm_th_cosq_cpu_port_info_t);

    /*
     * Sync the ref_count of IFP_COS_MAPm/COS_MAPm
     */
    alloc_size += (SOC_MEM_SIZE(unit, IFP_COS_MAPm) / _TH_MMU_NUM_INT_PRI) *
                   sizeof(uint16);

    /*
     * Added in BCM_WB_VERSION_1_1
     * Sync _bcm_th_cosq_cpu_cosq_config_t structure for all CPU queues
     */
    alloc_size += (SOC_TH_NUM_CPU_QUEUES *
                   sizeof(_bcm_th_cosq_cpu_cosq_config_t));

    /*
     * Added in BCM_WB_VERSION_1_1
     * Sync num_cos value
     */
    alloc_size += sizeof(int);

    /*
     * Added in BCM_WB_VERSION_1_1
     * Sync _bcm_pfc_deadlock_control_t structure
     */
    if (soc_feature(unit, soc_feature_pfc_deadlock)) {
        alloc_size += sizeof(_bcm_pfc_deadlock_control_t);
    }  

   /*
     * Added TIME_DOMAIN ref count
     */
    alloc_size += sizeof(_bcm_th_cosq_time_info_t);

#ifdef BCM_TOMAHAWK_SUPPORT 
    if (soc_feature(unit, soc_feature_ecn_wred)) {
        uint32 scache_size = 0;
        BCM_IF_ERROR_RETURN(
            bcmi_xgs5_ecn_scache_size_get(unit, &scache_size));
        alloc_size += scache_size;
    }
#endif
#if defined (BCM_TCB_SUPPORT) && defined (INCLUDE_TCB)
    if (soc_feature(unit, soc_feature_tcb)){
#ifdef BCM_TOMAHAWK2_SUPPORT
        alloc_size += sizeof(uint16);
#endif
    }
#endif
    /* Reserving some space scache for future usecase (max_size) */
    if (alloc_size < max_size) {
        alloc_size = max_size;
    } else {
        /* Required size more than max size allocated for Cosq */
        return BCM_E_INIT;
    }

    SOC_SCACHE_HANDLE_SET(scache_handle, unit, BCM_MODULE_COSQ, 0);

    rv = _bcm_esw_scache_ptr_get(unit, scache_handle, TRUE, alloc_size,
                                 &scache_ptr, BCM_WB_DEFAULT_VERSION, NULL);

    if (rv == BCM_E_NOT_FOUND) {
        rv = BCM_E_NONE;
    }

    return rv;
}
#endif /* BCM_WARM_BOOT_SUPPORT */

/*
 * index: degree, value: contangent(degree) * 100
 * max value is 0xffff (16-bit) at 0 degree
 * Same as TD2.
 */
STATIC int
_bcm_th_cotangent_lookup_table[] =
{
    /*  0.. 5 */  65535, 5728, 2863, 1908, 1430, 1143,
    /*  6..11 */    951,  814,  711,  631,  567,  514,
    /* 12..17 */    470,  433,  401,  373,  348,  327,
    /* 18..23 */    307,  290,  274,  260,  247,  235,
    /* 24..29 */    224,  214,  205,  196,  188,  180,
    /* 30..35 */    173,  166,  160,  153,  148,  142,
    /* 36..41 */    137,  132,  127,  123,  119,  115,
    /* 42..47 */    111,  107,  103,  100,   96,   93,
    /* 48..53 */     90,   86,   83,   80,   78,   75,
    /* 54..59 */     72,   70,   67,   64,   62,   60,
    /* 60..65 */     57,   55,   53,   50,   48,   46,
    /* 66..71 */     44,   42,   40,   38,   36,   34,
    /* 72..77 */     32,   30,   28,   26,   24,   23,
    /* 78..83 */     21,   19,   17,   15,   14,   12,
    /* 84..89 */     10,    8,    6,    5,    3,    1,
    /* 90     */      0
};

/*
 * Given a slope (angle in degrees) from 0 to 90, return the number of cells
 * in the range from 0% drop probability to 100% drop probability.
 * Same as TD2.
 */
STATIC int
_bcm_th_angle_to_cells(int angle)
{
    return (_bcm_th_cotangent_lookup_table[angle]);
}

/*
 * Given a number of packets in the range from 0% drop probability
 * to 100% drop probability, return the slope (angle in degrees).
 * Same as TD2.
 */
STATIC int
_bcm_th_cells_to_angle(int packets)
{
    int angle;

    for (angle = 90; angle >= 0 ; angle--) {
        if (packets <= _bcm_th_cotangent_lookup_table[angle]) {
            break;
        }
    }
    return angle;
}


/*
 *  Convert HW drop probability to percent value
 *  Same as TD2.
 */
STATIC int
_bcm_th_hw_drop_prob_to_percent[] = {
    0,     /* 0  */
    1,     /* 1  */
    2,     /* 2  */
    3,     /* 3  */
    4,     /* 4  */
    5,     /* 5  */
    6,     /* 6  */
    7,     /* 7  */
    8,     /* 8  */
    9,     /* 9  */
    10,    /* 10 */
    25,    /* 11 */
    50,    /* 12 */
    75,    /* 13 */
    100,   /* 14 */
    -1     /* 15 */
};

STATIC int
_bcm_th_percent_to_drop_prob(int percent)
{
   int i;

   for (i = 14; i > 0 ; i--) {
      if (percent >= _bcm_th_hw_drop_prob_to_percent[i]) {
          break;
      }
   }
   return i;
}

STATIC int
_bcm_th_drop_prob_to_percent(int drop_prob) {
   return (_bcm_th_hw_drop_prob_to_percent[drop_prob]);
}

/*
 * Function:
 *     _bcm_th_cosq_node_get
 * Purpose:
 *     Get internal information of queue group or scheduler type GPORT
 * Parameters:
 *     unit       - (IN) unit number
 *     gport      - (IN) queue group/scheduler GPORT identifier
 *     modid      - (OUT) module identifier
 *     port       - (OUT) port number
 *     id         - (OUT) queue group or scheduler identifier
 *     node       - (OUT) node structure for the specified GPORT
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_th_cosq_node_get(int unit, bcm_gport_t gport, bcm_module_t *modid,
                      bcm_port_t *port, int *id, _bcm_th_cosq_node_t **node)
{
    _bcm_th_cosq_port_info_t *port_info = NULL;
    _bcm_th_cosq_cpu_port_info_t *cpu_port_info = NULL;
    _bcm_th_cosq_node_t *node_base = NULL;
    bcm_module_t modid_out;
    bcm_port_t port_out;
    uint32 encap_id;
    int index;

    if (_bcm_th_cosq_port_info[unit] == NULL) {
        return BCM_E_INIT;
    }
    if (_bcm_th_cosq_cpu_port_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &modid_out));
        port_out = BCM_GPORT_UCAST_QUEUE_GROUP_SYSPORTID_GET(gport);
    } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &modid_out));
        port_out = BCM_GPORT_MCAST_QUEUE_GROUP_SYSPORTID_GET(gport);
    } else if (BCM_GPORT_IS_SCHEDULER(gport)) {
        BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &modid_out));
        port_out = BCM_GPORT_SCHEDULER_GET(gport) & 0xff;
    } else {
        return BCM_E_PORT;
    }

    if (!SOC_PORT_VALID(unit, port_out)) {
        return BCM_E_PORT;
    }
    if (IS_CPU_PORT(unit, port_out)) {
        cpu_port_info = &_bcm_th_cosq_cpu_port_info[unit][port_out];
    } else {
        port_info = &_bcm_th_cosq_port_info[unit][port_out];
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
        encap_id = BCM_GPORT_UCAST_QUEUE_GROUP_QID_GET(gport);
        index = encap_id;
        if (index >= _BCM_TH_NUM_UCAST_QUEUE_PER_PORT) {
            return BCM_E_PORT;
        }
        node_base = port_info->ucast;
    } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        encap_id = BCM_GPORT_MCAST_QUEUE_GROUP_QID_GET(gport);
        index = encap_id;
        if (!IS_CPU_PORT(unit, port_out) && (index >= _BCM_TH_NUM_MCAST_QUEUE_PER_PORT)) {
            return BCM_E_PORT;
        }
        if (IS_CPU_PORT(unit, port_out) && (index >= _BCM_TH_NUM_CPU_MCAST_QUEUE)) {
            return BCM_E_PORT;
        }
        if (IS_CPU_PORT(unit, port_out)) {
            node_base = cpu_port_info->mcast;
        } else {
            node_base = port_info->mcast;
        }
    } else { /* scheduler */
        encap_id = (BCM_GPORT_SCHEDULER_GET(gport) >> 16) & 0x3ff;
        index = encap_id;
        if (index >= _BCM_TH_NUM_SCHEDULER_PER_PORT) {
            return BCM_E_PORT;
        }
        if (IS_CPU_PORT(unit, port_out)) {
            node_base = cpu_port_info->sched;
        } else {
            node_base = port_info->sched;
        }
    }

    if ((node_base == NULL) ||
        (node_base[index].numq == 0) ||
        (node_base[index].in_use == 0)) {
        return BCM_E_NOT_FOUND;
    }

    if (modid != NULL) {
        *modid = modid_out;
    }
    if (port != NULL) {
        *port = port_out;
    }
    if (id != NULL) {
        *id = encap_id;
    }
    if (node != NULL) {
        *node = &node_base[index];
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_th_cosq_localport_resolve
 * Purpose:
 *     Resolve GPORT if it is a local port
 * Parameters:
 *     unit       - (IN) unit number
 *     gport      - (IN) GPORT identifier
 *     local_port - (OUT) local port number
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_th_cosq_localport_resolve(int unit, bcm_gport_t gport,
                               bcm_port_t *local_port)
{
    bcm_module_t module;
    bcm_port_t port;
    bcm_trunk_t trunk;
    int id, result, rv;

    if (!BCM_GPORT_IS_SET(gport)) {
        if (!SOC_PORT_VALID(unit, gport)) {
            return BCM_E_PORT;
        }
        *local_port = gport;
        return BCM_E_NONE;
    }

    rv = _bcm_esw_gport_resolve(unit, gport, &module, &port, &trunk, &id);
    if (rv == BCM_E_PORT) {
        /* returning E_PARAM when invalid gport is given as param */
        return BCM_E_PARAM;
    } else if (BCM_FAILURE(rv)) {
        return rv;
    }

    BCM_IF_ERROR_RETURN(_bcm_esw_modid_is_local(unit, module, &result));
    if (result == FALSE) {
        return BCM_E_PARAM;
    }

    *local_port = port;

    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_egress_sp_get(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq,
                           int *p_pool_start, int *p_pool_end)
{
    int local_port;
    int pool;

    if (cosq == BCM_COS_INVALID) {
        if (p_pool_start && p_pool_end) {
            *p_pool_start = 0;
            *p_pool_end = 3;
            return BCM_E_NONE;
        } else {
            return BCM_E_PARAM;
        }
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                    _BCM_TH_COSQ_INDEX_STYLE_EGR_POOL,
                                    &local_port, &pool, NULL));

    if (p_pool_start) {
        *p_pool_start = pool;
    }
    if (p_pool_end) {
        *p_pool_end = pool;
    }
    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_ingress_pg_get(int unit, bcm_gport_t gport, bcm_cos_queue_t pri,
                            int *p_pg_start, int *p_pg_end)
{
    bcm_port_t local_port;
    uint32  rval,pool;
    soc_reg_t reg = INVALIDr;
    static const soc_reg_t prigroup_reg[] = {
        THDI_PORT_PRI_GRP0r, THDI_PORT_PRI_GRP1r
    };
    static const soc_field_t prigroup_field[] = {
        PRI0_GRPf, PRI1_GRPf, PRI2_GRPf, PRI3_GRPf,
        PRI4_GRPf, PRI5_GRPf, PRI6_GRPf, PRI7_GRPf,
        PRI8_GRPf, PRI9_GRPf, PRI10_GRPf, PRI11_GRPf,
        PRI12_GRPf, PRI13_GRPf, PRI14_GRPf, PRI15_GRPf
    };

    if (pri == BCM_COS_INVALID) {
        *p_pg_start = 0;
        *p_pg_end = 7;
        return BCM_E_NONE;
    }

    BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));

    if (!SOC_PORT_VALID(unit, local_port)) {
        return BCM_E_PORT;
    }

    if (pri >= _TH_MMU_NUM_INT_PRI) {
        return BCM_E_PARAM;
    }
       /* get PG for port using Port+Cos */
    if (pri < (_TH_MMU_NUM_INT_PRI / 2)) {
        reg = prigroup_reg[0];
    } else {
        reg = prigroup_reg[1];
    }

    SOC_IF_ERROR_RETURN
        (soc_reg32_get(unit, reg, local_port, 0, &rval));
        pool = soc_reg_field_get(unit, reg, rval, prigroup_field[pri]);

    if (p_pg_start) {
        *p_pg_start = pool;
    }

    if (p_pg_end) {
        *p_pg_end = pool;
    }

    return BCM_E_NONE;
}


/*
 * This function is used to get shared pool
 * for given Ingress [Port, Priority]
 */
STATIC int
_bcm_th_cosq_ingress_sp_get(int unit, bcm_gport_t gport, bcm_cos_t pri,
                            int *p_pool_start, int *p_pool_end)
{
    int local_port;
    uint32 rval, pool;
    int pri_grp;
    static const soc_field_t prigroup_spid_field[] = {
        PG0_SPIDf, PG1_SPIDf, PG2_SPIDf, PG3_SPIDf,
        PG4_SPIDf, PG5_SPIDf, PG6_SPIDf, PG7_SPIDf
    };

    if (pri == BCM_COS_INVALID) {
        if (p_pool_start && p_pool_end) {
            *p_pool_start = 0;
            *p_pool_end = 3;
            return BCM_E_NONE;
        } else {
            return BCM_E_PARAM;
        }
    }

    if ((pri < 0) || (pri >= _TH_MMU_NUM_INT_PRI)) {
        /* Error. Input pri > Max Pri_Grp */
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_ingress_pg_get(unit, gport, pri, &pri_grp, &pri_grp));

    BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));

    if (!SOC_PORT_VALID(unit, local_port)) {
        return BCM_E_PORT;
    }

    if (pri_grp >= _TH_MMU_NUM_PG) {
        return BCM_E_PARAM;
    }

    SOC_IF_ERROR_RETURN(READ_THDI_PORT_PG_SPIDr(unit, local_port, &rval));
    pool = soc_reg_field_get(unit, THDI_PORT_PG_SPIDr, rval,
                             prigroup_spid_field[pri_grp]);

    if (p_pool_start) {
        *p_pool_start = pool;
    }
    if (p_pool_end) {
        *p_pool_end = pool;
    }
    return BCM_E_NONE;
}

/*
 * This function is used to get shared pool
 * for given Ingress [Port, Priority_group]
 */
STATIC int
_bcm_th_cosq_ingress_sp_get_by_pg(int unit, bcm_gport_t gport, bcm_cos_queue_t pri_grp,
                            int *p_pool_start, int *p_pool_end)
{
    int local_port;
    uint32 rval, pool;
    static const soc_field_t prigroup_spid_field[] = {
        PG0_SPIDf, PG1_SPIDf, PG2_SPIDf, PG3_SPIDf,
        PG4_SPIDf, PG5_SPIDf, PG6_SPIDf, PG7_SPIDf
    };

    if (pri_grp == BCM_COS_INVALID) {
        if (p_pool_start && p_pool_end) {
            *p_pool_start = 0;
            *p_pool_end = 3;
            return BCM_E_NONE;
        } else {
            return BCM_E_PARAM;
        }
    }

    BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));

    if (!SOC_PORT_VALID(unit, local_port)) {
        return BCM_E_PORT;
    }

    if (pri_grp >= _TH_MMU_NUM_PG) {
        return BCM_E_PARAM;
    }

    SOC_IF_ERROR_RETURN(READ_THDI_PORT_PG_SPIDr(unit, local_port, &rval));
    pool = soc_reg_field_get(unit, THDI_PORT_PG_SPIDr, rval,
                             prigroup_spid_field[pri_grp]);

    if (p_pool_start) {
        *p_pool_start = pool;
    }
    if (p_pool_end) {
        *p_pool_end = pool;
    }
    return BCM_E_NONE;
}


/*
 * This function is used to get headroom pool
 * for given Ingress [Port, Priority]
 */
STATIC int
_bcm_th_cosq_ingress_hdrm_pool_get(int unit, bcm_gport_t gport,
                                   bcm_cos_t pri,
                                   int *p_hdrm_pool_start,
                                   int *p_hdrm_pool_end)
{
    int local_port;
    uint32 rval, hdrm_pool;
    int pri_grp;
    static const soc_field_t prigroup_hpid_field[] = {
        PG0_HPIDf, PG1_HPIDf, PG2_HPIDf, PG3_HPIDf,
        PG4_HPIDf, PG5_HPIDf, PG6_HPIDf, PG7_HPIDf
    };

    if (pri == BCM_COS_INVALID) {
        if (p_hdrm_pool_start && p_hdrm_pool_end) {
            *p_hdrm_pool_start = 0;
            *p_hdrm_pool_end = 3;
            return BCM_E_NONE;
        } else {
            return BCM_E_PARAM;
        }
    }

    if ((pri < 0) || (pri >= _TH_MMU_NUM_INT_PRI)) {
        /* Error. Input pri > Max Pri_Grp */
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_ingress_pg_get(unit, gport, pri, &pri_grp, &pri_grp));

    BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));

    if (!SOC_PORT_VALID(unit, local_port)) {
        return BCM_E_PORT;
    }

    if (pri_grp >= _TH_MMU_NUM_PG) {
        return BCM_E_PARAM;
    }

    SOC_IF_ERROR_RETURN(READ_THDI_HDRM_PORT_PG_HPIDr(unit, local_port, &rval));
    hdrm_pool = soc_reg_field_get(unit, THDI_HDRM_PORT_PG_HPIDr, rval,
                                  prigroup_hpid_field[pri_grp]);

    if (p_hdrm_pool_start) {
        *p_hdrm_pool_start = hdrm_pool;
    }
    if (p_hdrm_pool_end) {
        *p_hdrm_pool_end = hdrm_pool;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_th_cosq_port_cos_resolve
 * Purpose:
 *     Find UC/MC queue gport for a given logical port
 *     and cos value.
 * Parameters:
 *     unit      - (IN) unit number
 *     port      - (IN) logical port
 *     cos       - (IN) cos value
 *     style     - (IN) index style
 *                 (_BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE or
 *                 _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE)
 *     gport     - (OUT) Queue gport
 * Returns:
 *     BCM_E_XXX
 */
int
_bcm_th_cosq_port_cos_resolve(int unit, bcm_port_t port, bcm_cos_t cos,
                              _bcm_th_cosq_index_style_t style,
                              bcm_gport_t *gport)
{
    _bcm_th_cosq_port_info_t *port_info = NULL;
    _bcm_th_cosq_cpu_port_info_t *cpu_port_info = NULL;

    if (gport == NULL) {
        return BCM_E_PARAM;
    }

    if (_bcm_th_cosq_port_info[unit] == NULL) {
        return BCM_E_INIT;
    }
    if (_bcm_th_cosq_cpu_port_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    if ((IS_CPU_PORT(unit, port)) &&
        (style == _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE)) {
        return BCM_E_PARAM;
    }

    if (cos < 0) {
        return BCM_E_PARAM;
    }

    if (!SOC_PORT_VALID(unit, port)) {
        return BCM_E_PORT;
    }

    if (_BCM_TH_MMU_INFO(unit)->gport_tree_created != TRUE) {
        return BCM_E_INTERNAL;
    }

    if (IS_CPU_PORT(unit, port)) {
        cpu_port_info = _bcm_th_cosq_cpu_port_info[unit];
    } else {
        port_info = &_bcm_th_cosq_port_info[unit][port];
    }

    if (style == _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE) {
        if (cos >= _BCM_TH_NUM_UCAST_QUEUE_PER_PORT) {
            return BCM_E_PARAM;
        }
        *gport = port_info->ucast[cos].gport;
    } else if (style == _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE) {
        if (IS_CPU_PORT(unit, port)) {
            if (cos >= _BCM_TH_NUM_CPU_MCAST_QUEUE) {
                return BCM_E_PARAM;
            }
            *gport = cpu_port_info->mcast[cos].gport;
        } else {
            if (cos >= _BCM_TH_NUM_MCAST_QUEUE_PER_PORT) {
                return BCM_E_PARAM;
            }
            *gport = port_info->mcast[cos].gport;
        }
    } else {
        return BCM_E_INTERNAL;
    }

    return BCM_E_NONE;
}

int
_bcm_th_cosq_port_resolve(int unit, bcm_gport_t gport, bcm_module_t *modid,
                          bcm_port_t *port, bcm_trunk_t *trunk_id, int *id,
                          int *qnum)
{
    _bcm_th_cosq_node_t *node;

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_node_get(unit, gport, modid, port, id, &node));
    *trunk_id = -1;

    if (qnum) {
        if (node->hw_index == -1) {
            return BCM_E_PARAM;
        }
        *qnum = node->hw_index;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_th_cosq_index_resolve
 * Purpose:
 *     Find hardware index for the given port/cosq
 * Parameters:
 *     unit       - (IN) unit number
 *     port       - (IN) port number or GPORT identifier
 *     cosq       - (IN) COS queue number
 *     style      - (IN) index style (_BCM_KA_COSQ_INDEX_STYLE_XXX)
 *     local_port - (OUT) local port number
 *     index      - (OUT) result index
 *     count      - (OUT) number of index
 * Returns:
 *     BCM_E_XXX
 */
int
_bcm_th_cosq_index_resolve(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                           _bcm_th_cosq_index_style_t style,
                           bcm_port_t *local_port, int *index, int *count)
{
    bcm_port_t resolved_port;
    soc_info_t *si;
    int resolved_index = -1;
    int id, startq, numq = 0;
    int phy_port, pipe;
    int uc_base = 0, mc_base = 0, cpu_mc_base = 0; /* Q base number */
    int mmu_port; /* global mmu port number */
    int local_mmu_port; /* local mmu port number - local to per pipe */
    _bcm_th_cosq_node_t *node;
    uint32 entry0[SOC_MAX_MEM_WORDS];
    soc_mem_t mem;

    si = &SOC_INFO(unit);

    if (cosq < -1) {
        return BCM_E_PARAM;
    } else if (cosq == -1) {
        startq = 0;
        numq = NUM_COS(unit);
    } else {
        startq = cosq;
        numq = 1;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
        BCM_GPORT_IS_SCHEDULER(port)) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_node_get(unit, port, NULL, &resolved_port, &id,
                                   &node));
        if (resolved_port < 0) {
            return BCM_E_PORT;
        }
    } else {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, port, &resolved_port));
        if (resolved_port < 0) {
            return BCM_E_PORT;
        }
        node = NULL;
        numq = (IS_CPU_PORT(unit, resolved_port)) ? NUM_CPU_COSQ(unit) : NUM_COS(unit);
    }
    phy_port = si->port_l2p_mapping[resolved_port];
    mmu_port = si->port_p2m_mapping[phy_port];
    local_mmu_port = mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1);
    mc_base = si->port_cosq_base[resolved_port];
    uc_base = si->port_uc_cosq_base[resolved_port];

    SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, resolved_port, &pipe));

    switch (style) {
        case _BCM_TH_COSQ_INDEX_STYLE_WRED_DEVICE:
            if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
                return BCM_E_PARAM;
            }
            if ((port == -1) || (cosq == -1)) {
                numq = _TH_MMU_NUM_POOL; /* 4 Global Service Pools */
                resolved_index = 462;
            } else {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_egress_sp_get(unit, port, cosq,
                                                &resolved_index,
                                                NULL));
                numq = 1;
                resolved_index += 462;
            }
            break;
        case _BCM_TH_COSQ_INDEX_STYLE_WRED_PORT:
            if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
                return BCM_E_PARAM;
            }
            resolved_index = 0;
            if (cosq != -1) {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_egress_sp_get(unit, port, cosq,
                                                &resolved_index,
                                                NULL));
            }
            resolved_index += SOC_TH_NUM_UC_QUEUES_PER_PIPE +
                              (local_mmu_port * 4);
            numq = (cosq == -1) ? 4: 1;
            break;
        case _BCM_TH_COSQ_INDEX_STYLE_WRED_QUEUE:
            if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
                return BCM_E_PARAM;
            }
            resolved_index = local_mmu_port * _BCM_TH_NUM_UCAST_QUEUE_PER_PORT;
            if (cosq != -1) {
                if (cosq >= NUM_COS(unit)) {
                    return BCM_E_PARAM;
                }
                resolved_index += cosq;
            }
            numq = (cosq == -1) ? NUM_COS(unit): 1;
            break;
        case _BCM_TH_COSQ_INDEX_STYLE_EGR_POOL:
            if ((node != NULL) && (!BCM_GPORT_IS_MODPORT(port))) {
                if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port)) {
                    /* regular unicast queue */
                    resolved_index = (uc_base + (node->hw_index %
                                       _BCM_TH_NUM_UCAST_QUEUE_PER_PORT));
                    mem = MMU_THDU_Q_TO_QGRP_MAPm;
                } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
                    if (IS_CPU_PORT(unit, resolved_port)) {
                        cpu_mc_base = si->port_cosq_base[CMIC_PORT(unit)];
                        resolved_index = cpu_mc_base +
                                         ((node->hw_index - cpu_mc_base) %
                                           _BCM_TH_NUM_CPU_MCAST_QUEUE);
                    } else {
                        resolved_index = (mc_base + (node->hw_index %
                                           _BCM_TH_NUM_MCAST_QUEUE_PER_PORT));
                    }
                    mem = MMU_THDM_MCQE_QUEUE_CONFIGm;
                } else { /* scheduler */
                    return BCM_E_PARAM;
                }
            } else { /* node == NULL */
                numq = si->port_num_cosq[resolved_port];
                if (startq >= numq) {
                    return BCM_E_PARAM;
                }
                resolved_index = (local_mmu_port *
                                  _BCM_TH_NUM_UCAST_QUEUE_PER_PORT) + startq;
                mem = MMU_THDU_Q_TO_QGRP_MAPm;
            }
            mem = SOC_MEM_UNIQUE_ACC(unit, mem)[pipe];

            SOC_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, resolved_index,
                                entry0));
            resolved_index = soc_mem_field32_get(unit, mem, entry0, Q_SPIDf);
            break;
        case _BCM_TH_COSQ_INDEX_STYLE_COS:
            if (node) {
                if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port)) {
                     resolved_index = node->hw_index %
                                      _BCM_TH_NUM_UCAST_QUEUE_PER_PORT;
                } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
                     if (IS_CPU_PORT(unit, resolved_port)) {
                         cpu_mc_base = si->port_cosq_base[CMIC_PORT(unit)];
                         resolved_index = (node->hw_index - cpu_mc_base) %
                                          NUM_CPU_COSQ(unit);
                     } else {
                         resolved_index = node->hw_index %
                                          _BCM_TH_NUM_MCAST_QUEUE_PER_PORT;
                     }
                } else {
                     resolved_index = node->hw_index %
                                      _BCM_TH_NUM_SCHEDULER_PER_PORT;
                }
            } else {
                resolved_index = startq;
            }
            break;
        case _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE:
            /* Return hw_index for given unicast queue gport */
            if (node) {
                if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port)) {
                     resolved_index = (uc_base + (node->hw_index %
                                        _BCM_TH_NUM_UCAST_QUEUE_PER_PORT));
                } else {
                    return BCM_E_PARAM;
                }
            } else {
                if (startq >= numq) {
                    return BCM_E_PARAM;
                }
                resolved_index = uc_base + startq;
            }
            break;
        case _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE:
            /* Return hw_index for given multicast queue gport */
            if (node) {
                if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
                     if (IS_CPU_PORT(unit, resolved_port)) {
                         cpu_mc_base = si->port_cosq_base[CMIC_PORT(unit)];
                         resolved_index = cpu_mc_base +
                                          ((node->hw_index - cpu_mc_base) %
                                              _BCM_TH_NUM_CPU_MCAST_QUEUE);
                     } else {
                         resolved_index = (mc_base + (node->hw_index %
                                            _BCM_TH_NUM_MCAST_QUEUE_PER_PORT));
                     }
                } else {
                    return BCM_E_PARAM;
                }
            } else {
                if (startq >= numq) {
                    return BCM_E_PARAM;
                }
                resolved_index = mc_base + startq;
            }
            break;
        default:
            return BCM_E_PARAM;
    }

    if (index) {
        *index = resolved_index;
    }
    if (local_port) {
        *local_port = resolved_port;
    }
    if (count) {
        *count = (cosq == -1) ? numq : 1;
    }

    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_cpu_port_info_dump(int unit)
{
    _bcm_th_cosq_cpu_port_info_t *port_info;
    bcm_port_t port = 0; /* CPU Port */
    _bcm_th_cosq_node_t *node;
    int index;
    soc_th_sched_mode_e sched_mode = 0;
    int weight = 0;
    char *sched_modes[] = {"X", "SP", "WRR", "WERR"};

    if (!SOC_PORT_VALID(unit, port)) {
        return BCM_E_PORT;
    }
    port_info = _bcm_th_cosq_cpu_port_info[unit];

    LOG_CLI((BSL_META_U(unit,
                        "=== CPU PORT ===\n")));

    LOG_CLI((BSL_META_U(unit,
                        "L0 Nodes\n")));
    for (index = 0; index < _BCM_TH_NUM_SCHEDULER_PER_PORT; index++) {
        /* Node */
        node = &port_info->sched[index];

        BCM_IF_ERROR_RETURN(
                soc_th_cosq_sched_mode_get(unit, port, node->level, index,
                                           &sched_mode, &weight, 0));
        LOG_CLI((BSL_META_U(unit,
                            "    LO.%d: GPORT=0x%x MODE=%s WT=%d\n"),
                             index, node->gport,
                             sched_modes[sched_mode], weight));
    }

    LOG_CLI((BSL_META_U(unit,
                        "\nMC Queues\n")));
    for (index = 0; index < _BCM_TH_NUM_CPU_MCAST_QUEUE; index++) {
        int parent_id = 0;
        /* MC Queue */
        node = &port_info->mcast[index];
        BCM_IF_ERROR_RETURN(
                soc_th_cosq_sched_mode_get(unit, port, node->level, index,
                                           &sched_mode, &weight, 1));
        BCM_IF_ERROR_RETURN(
            _bcm_th_cosq_node_get(unit, node->parent_gport, NULL, NULL,
                                  &parent_id, NULL));
        LOG_CLI((BSL_META_U(unit,
                            "    MC.%d: GPORT=0x%x MODE=%s WT=%d"
                            " PARENT=L0.%d\n"),
                             index, node->gport,
                             sched_modes[sched_mode], weight,
                             parent_id));
    }
    LOG_CLI((BSL_META_U(unit,
                        "=========== \n")));

    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_port_info_dump(int unit, bcm_port_t port)
{
    _bcm_th_cosq_port_info_t *port_info;
    _bcm_th_cosq_node_t *node;
    int index, empty;
    soc_th_sched_mode_e sched_mode = 0;
    int weight = 0;
    char *sched_modes[] = {"X", "SP", "WRR", "WERR"};

    if (!SOC_PORT_VALID(unit, port)) {
        return BCM_E_PORT;
    }
    port_info = &_bcm_th_cosq_port_info[unit][port];

    empty = TRUE;
    for (index = 0; index < _BCM_TH_NUM_SCHEDULER_PER_PORT; index++) {
        if (port_info->sched[index].numq > 0) {
            empty = FALSE;
            break;
        }
    }
    if (empty) {
        for (index = 0; index < _BCM_TH_NUM_UCAST_QUEUE_PER_PORT; index++) {
            if (port_info->ucast[index].numq > 0) {
                empty = FALSE;
                break;
            }
        }
    }
    if (empty) {
        for (index = 0; index < _BCM_TH_NUM_MCAST_QUEUE_PER_PORT; index++) {
            if (port_info->mcast[index].numq > 0) {
                empty = FALSE;
                break;
            }
        }
    }

    if (empty == TRUE) {
        return BCM_E_NOT_FOUND;
    }

    LOG_CLI((BSL_META_U(unit,
                        "=== PORT %d\n"), port));

    for (index = 0; index < _BCM_TH_NUM_SCHEDULER_PER_PORT; index++) {
        /* Node */
        node = &port_info->sched[index];

        BCM_IF_ERROR_RETURN(
                soc_th_cosq_sched_mode_get(unit, port, node->level, index,
                                           &sched_mode, &weight, 0));
        LOG_CLI((BSL_META_U(unit,
                            "LO.%d: GPORT=0x%x MODE=%s WT=%d\n"),
                             index, node->gport,
                             sched_modes[sched_mode], weight));

        /* UC Queue */
        node = &port_info->ucast[index % _BCM_TH_NUM_UCAST_QUEUE_PER_PORT];
        BCM_IF_ERROR_RETURN(
                soc_th_cosq_sched_mode_get(unit, port, node->level, index,
                                           &sched_mode, &weight, 0));
        LOG_CLI((BSL_META_U(unit,
                            "    UC.%d: GPORT=0x%x MODE=%s WT=%d\n"),
                             index, node->gport,
                             sched_modes[sched_mode], weight));

        /* MC Queue */
        node = &port_info->mcast[index % _BCM_TH_NUM_MCAST_QUEUE_PER_PORT];
        BCM_IF_ERROR_RETURN(
                soc_th_cosq_sched_mode_get(unit, port, node->level, index,
                                           &sched_mode, &weight, 1));
        LOG_CLI((BSL_META_U(unit,
                            "    MC.%d: GPORT=0x%x MODE=%s WT=%d\n"),
                             index, node->gport,
                             sched_modes[sched_mode], weight));
    }
    LOG_CLI((BSL_META_U(unit,
                        "=========== \n")));

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_gport_get
 * Purpose:
 *     Get a cosq gport structure
 * Parameters:
 *     unit  - (IN) unit number
 *     gport - (IN) GPORT identifier
 *     port  - (OUT) port number
 *     numq  - (OUT) number of COS queues
 *     flags - (OUT) flags (BCM_COSQ_GPORT_XXX)
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_gport_get(int unit, bcm_gport_t gport, bcm_gport_t *port,
                      int *numq, uint32 *flags)
{
    _bcm_th_cosq_node_t *node;
    bcm_port_t local_port;
    bcm_module_t modid;
    _bcm_gport_dest_t dest;

    if (port == NULL || numq == NULL || flags == NULL) {
        return BCM_E_PARAM;
    }

    LOG_INFO(BSL_LS_BCM_COSQ,
             (BSL_META_U(unit,
                         "bcm_th_cosq_gport_get: unit=%d gport=0x%x\n"),
              unit, gport));

    if ((BCM_GPORT_IS_SCHEDULER(gport)) ||
         BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
         BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport) ||
         BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_node_get(unit, gport, NULL, &local_port,
                                   NULL, &node));

        *numq = node->numq;

        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            *flags = BCM_COSQ_GPORT_UCAST_QUEUE_GROUP;
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            *flags = BCM_COSQ_GPORT_MCAST_QUEUE_GROUP;
        } else if (BCM_GPORT_IS_SCHEDULER(gport)) {
            *flags = BCM_COSQ_GPORT_SCHEDULER;
        } else {
            *flags = 0;
        }
    } else {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
        *flags = 0;
        *numq = _BCM_TH_NUM_SCHEDULER_PER_PORT;
    }

    if (SOC_USE_GPORT(unit)) {
        BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &modid));
        dest.gport_type = _SHR_GPORT_TYPE_MODPORT;
        dest.modid = modid;
        dest.port = local_port;
        BCM_IF_ERROR_RETURN(_bcm_esw_gport_construct(unit, &dest, port));
    } else {
        *port = local_port;
    }

    LOG_INFO(BSL_LS_BCM_COSQ,
             (BSL_META_U(unit,
                         "                       port=0x%x numq=%d flags=0x%x\n"),
              *port, *numq, *flags));

    return BCM_E_NONE;
}

int
bcm_th_cosq_config_set(int unit, int numq)
{
    port_cos_map_entry_t cos_map_entries[16];
    void *entries[1];
    uint32 index;
    int count;
    bcm_port_t port;
    int cos, prio, ref_count = -1;
    uint32 i;
    soc_mem_t mem = INVALIDm;

    if (numq < 1 || numq > BCM_TH_COS_MAX) {
        return BCM_E_PARAM;
    }

    index = 0;
    while (index < soc_mem_index_count(unit, PORT_COS_MAPm)) {
        BCM_IF_ERROR_RETURN(soc_profile_mem_ref_count_get(unit,
                                      _bcm_th_cos_map_profile[unit],
                                      index, &ref_count));
        if (ref_count > 0) {
            while (ref_count) {
                if (!soc_profile_mem_delete(unit,
                            _bcm_th_cos_map_profile[unit], index)) {
                    ref_count -= 1;
                }
            }
        }
        index += 16;
    }

    /* Distribute first 8 internal priority levels into the specified number
     * of cosq, map remaining internal priority levels to highest priority
     * cosq */
    sal_memset(cos_map_entries, 0, sizeof(cos_map_entries));
    entries[0] = &cos_map_entries;
    prio = 0;
    mem = PORT_COS_MAPm;

    for (cos = 0; cos < numq; cos++) {
        for (i = 8 / numq + (cos < 8 % numq ? 1 : 0); i > 0; i--) {
            soc_mem_field32_set(unit, mem, &cos_map_entries[prio],
                                UC_COS1f, cos);
            soc_mem_field32_set(unit, mem, &cos_map_entries[prio],
                                MC_COS1f, cos);
            prio++;
        }
    }
    for (prio = 8; prio < 16; prio++) {
        /*TH may have up to 10 cosq.*/
        if (prio < numq) {
            soc_mem_field32_set(unit, mem, &cos_map_entries[prio], UC_COS1f,
                            prio);
            soc_mem_field32_set(unit, mem, &cos_map_entries[prio], MC_COS1f,
                            prio);
        } else {
            soc_mem_field32_set(unit, mem, &cos_map_entries[prio], UC_COS1f,
                            numq - 1);
            soc_mem_field32_set(unit, mem, &cos_map_entries[prio], MC_COS1f,
                            numq - 1);
        }
    }

    /* Map internal priority levels to CPU CoS queues */
    BCM_IF_ERROR_RETURN(_bcm_esw_cpu_cosq_mapping_default_set(unit, numq));

    BCM_IF_ERROR_RETURN
        (soc_profile_mem_add(unit, _bcm_th_cos_map_profile[unit], entries, 16,
                             &index));
    count = 0;
    PBMP_ALL_ITER(unit, port) {
        BCM_IF_ERROR_RETURN
            (soc_mem_field32_modify(unit, COS_MAP_SELm, port, SELECTf,
                                    index / 16));
        count++;
    }
    if (SOC_INFO(unit).cpu_hg_index != -1) {
        BCM_IF_ERROR_RETURN
            (soc_mem_field32_modify(unit, COS_MAP_SELm,
                                    SOC_INFO(unit).cpu_hg_index, SELECTf,
                                    index / 16));
        count++;
    }
    for (i = 1; i < count; i++) {
        soc_profile_mem_reference(unit, _bcm_th_cos_map_profile[unit], index,
                                  0);
    }

    _TH_NUM_COS(unit) = numq;

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_config_get
 * Purpose:
 *     Get the number of cos queues
 * Parameters:
 *     unit - unit number
 *     numq - (Output) number of cosq
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_config_get(int unit, int *numq)
{
    if (numq != NULL) {
        *numq = _TH_NUM_COS(unit);
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_th_cosq_sched_set
 * Purpose:
 *     Set scheduling mode
 * Parameters:
 *     unit                - (IN) unit number
 *     port                - (IN) port number or GPORT identifier
 *     cosq                - (IN) COS queue number
 *     mode                - (IN) scheduling mode (BCM_COSQ_XXX)
 *     weight              - (IN) weight
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_th_cosq_sched_set(int unit, bcm_port_t gport, bcm_cos_queue_t cosq,
                        int mode, int weight)
{
    soc_th_sched_mode_e sched_mode;
    bcm_port_t local_port;
    int lwts = 1, child_index=0, lvl = SOC_TH_NODE_LVL_L0, mc = 0;

    if (cosq < 0) {
        if (cosq == -1) {
            /* caller needs to resolve the wild card value (-1) */
            return BCM_E_INTERNAL;
        } else { /* reject all other negative value */
            return BCM_E_PARAM;
        }
    }
    if ((weight < 0) || (weight > 127)) {
        return BCM_E_PARAM;
    }

    switch(mode) {
        case BCM_COSQ_STRICT:
            sched_mode = SOC_TH_SCHED_MODE_STRICT;
            lwts = 0;
            break;
        case BCM_COSQ_ROUND_ROBIN:
            sched_mode = SOC_TH_SCHED_MODE_WRR;
            lwts = 1;
            break;
        case BCM_COSQ_WEIGHTED_ROUND_ROBIN:
            sched_mode = SOC_TH_SCHED_MODE_WRR;
            lwts = weight;
            break;
        case BCM_COSQ_DEFICIT_ROUND_ROBIN:
            sched_mode = SOC_TH_SCHED_MODE_WERR;
            lwts = weight;
            break;
        default:
            return BCM_E_PARAM;
    }

    /* Weight = 0 corresponds to Strict Mode */
    if (lwts == 0) {
        sched_mode = SOC_TH_SCHED_MODE_STRICT;
    }

    child_index = cosq;
    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));

    if (BCM_GPORT_IS_SCHEDULER(gport)) {
        /* We need to set lvl = L0(schedular node) so that sched mode is set at it's parent */
        lvl = SOC_TH_NODE_LVL_L0;
    } else if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
               BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        /* We need to set lvl = L1(UC/MC nodes) so that sched mode is set at it's parent */
        lvl = SOC_TH_NODE_LVL_L1;
        mc = BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport) ? 1 : 0;
    } else if (IS_CPU_PORT(unit, local_port)) {
        lvl = SOC_TH_NODE_LVL_L1;
        mc = 1;
    }
    BCM_IF_ERROR_RETURN(soc_th_cosq_sched_mode_set(unit, local_port,
                                                   lvl,
                                                   child_index,
                                                   sched_mode, lwts, mc));

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_th_cosq_sched_get
 * Purpose:
 *     Get scheduling mode setting
 * Parameters:
 *     unit                - (IN) unit number
 *     port                - (IN) port number or GPORT identifier
 *     cosq                - (IN) COS queue number
 *     mode                - (OUT) scheduling mode (BCM_COSQ_XXX)
 *     weight              - (OUT) weight
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_th_cosq_sched_get(int unit, bcm_port_t gport, bcm_cos_queue_t cosq,
                       int *mode, int *weight)
{
    soc_th_sched_mode_e sched_mode;
    bcm_port_t local_port;
    int lvl = SOC_TH_NODE_LVL_L0, mc = 0;

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
    if (BCM_GPORT_IS_SCHEDULER(gport)) {
        lvl = SOC_TH_NODE_LVL_L0;
    } else if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
               BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        lvl = SOC_TH_NODE_LVL_L1;
        mc = BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport) ? 1 : 0;
    } else if (IS_CPU_PORT(unit, local_port)) {
        lvl = SOC_TH_NODE_LVL_L1;
        mc = 1;
    }
    BCM_IF_ERROR_RETURN(
            soc_th_cosq_sched_mode_get(unit, local_port, lvl, cosq,
                            &sched_mode, weight, mc));
    switch(sched_mode) {
        case SOC_TH_SCHED_MODE_STRICT:
            *mode = BCM_COSQ_STRICT;
        break;
        case SOC_TH_SCHED_MODE_WRR:
            *mode = BCM_COSQ_WEIGHTED_ROUND_ROBIN;
        break;
        case SOC_TH_SCHED_MODE_WERR:
            *mode = BCM_COSQ_DEFICIT_ROUND_ROBIN;
        break;
        default:
            return BCM_E_INTERNAL;
        break;
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_th_cosq_bucket_set
 * Purpose:
 *     Configure COS queue bandwidth control bucket
 * Parameters:
 *     unit          - (IN) unit number
 *     gport         - (IN) port number or GPORT identifier
 *     cosq          - (IN) COS queue number
 *     min_quantum   - (IN) kbps or packet/second
 *     max_quantum   - (IN) kbps or packet/second
 *     burst_quantum - (IN) kbps or packet/second
 *     flags         - (IN)
 * Returns:
 *     BCM_E_XXX
 * Notes:
 *     If port is any form of local port, cosq is the hardware queue index.
 */
STATIC int
_bcm_th_cosq_bucket_set(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq,
                        uint32 min_quantum, uint32 max_quantum,
                        uint32 kbits_burst_min, uint32 kbits_burst_max,
                        uint32 flags)
{
    _bcm_th_cosq_node_t *node = NULL;
    bcm_port_t local_port;
    uint32 rval, meter_flags;
    uint32 bucketsize_max, bucketsize_min;
    uint32 granularity_max, granularity_min;
    uint32 refresh_rate_max, refresh_rate_min;
    int refresh_bitsize = 0, bucket_bitsize = 0;
    int pipe, phy_port, mmu_port, index;
    soc_mem_t config_mem = INVALIDm;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_info_t *si;
    int cpu_mc_base = 0;

    /* caller should resolve the cosq. */
    if (cosq < 0) {
        if (cosq == -1) {
            /* caller needs to resolve the wild card value (-1) */
            return BCM_E_INTERNAL;
        } else { /* reject all other negative value */
            return BCM_E_PARAM;
        }
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));

    SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

    si = &SOC_INFO(unit);
    phy_port = si->port_l2p_mapping[local_port];
    mmu_port = si->port_p2m_mapping[phy_port];

    if (BCM_GPORT_IS_SET(gport) &&
        ((BCM_GPORT_IS_SCHEDULER(gport)) ||
          BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
          BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport) ||
          BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(gport))) {

        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_node_get(unit, gport, NULL,
                                   NULL, NULL, &node));
    }

    if (node) {
        if (BCM_GPORT_IS_SCHEDULER(gport)) {
            if (node->level == SOC_TH_NODE_LVL_L0) {
                config_mem = MMU_MTRO_L0_MEMm;
                cosq = node->hw_index % _BCM_TH_NUM_SCHEDULER_PER_PORT;
                index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) *
                         _BCM_TH_NUM_SCHEDULER_PER_PORT) + cosq;
            } else {
                return BCM_E_PARAM;
            }
        } else if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
                   BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            config_mem = MMU_MTRO_L1_MEMm;
            cosq = node->hw_index % si->port_num_cosq[local_port];

            if (IS_CPU_PORT(unit, local_port)) {
                /*Adjust mc cosq for cpu port*/
                cpu_mc_base = si->port_cosq_base[CMIC_PORT(unit)];
                cosq = (node->hw_index - cpu_mc_base) %
                       _BCM_TH_NUM_CPU_MCAST_QUEUE;
                /* For CPU port's MC queue starting index: 660(L1 Memory) */
                config_mem = MMU_MTRO_L1_MEMm;
                index = SOC_TH_MMU_CPU_MCQ_OFFSET + cosq;
            } else if (SOC_PBMP_MEMBER(si->management_pbm, local_port)) {
                config_mem = MMU_MTRO_L1_MEMm;
                index = (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport) ?
                    SOC_TH_MMU_MGMT_MCQ_OFFSET : SOC_TH_MMU_MGMT_UCQ_OFFSET) + cosq;
            } else {
                index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) *
                         _BCM_TH_NUM_SCHEDULER_PER_PORT) + cosq;
                if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                     index += SOC_TH_NUM_UC_QUEUES_PER_PIPE;
                }
            }
       } else {
            return BCM_E_PARAM;
       }
    } else {
        if ((cosq < 0) || (cosq >= si->port_num_cosq[local_port])) {
            return BCM_E_PARAM;
        }
        if (!IS_CPU_PORT(unit, local_port)) {
            index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) *
                     _BCM_TH_NUM_SCHEDULER_PER_PORT) + cosq;
            config_mem = MMU_MTRO_L0_MEMm;
        } else {
            /* For CPU port, refer L1 memory (logical queue, starting index 660)
             * directly */
            config_mem = MMU_MTRO_L1_MEMm;
            index = (2 * SOC_TH_NUM_UC_QUEUES_PER_PIPE) + cosq;
        }
    }

    /*
     * COVERITY
     *
     * This checker will not be True.
     * It is kept intentionally as a defensive check for future development.
     */
    /* coverity[dead_error_line] */
    if (config_mem == INVALIDm) {
        return BCM_E_PARAM;
    }
    config_mem = SOC_MEM_UNIQUE_ACC(unit, config_mem)[pipe];

    if (config_mem == INVALIDm) {
        return BCM_E_PARAM;
    }

    meter_flags = flags & BCM_COSQ_BW_PACKET_MODE ?
                            _BCM_TD_METER_FLAG_PACKET_MODE : 0;
#ifdef BCM_TOMAHAWK2_SUPPORT
    if (soc_feature(unit, soc_feature_mmu_sed)) {
        BCM_IF_ERROR_RETURN(READ_MMU_SEDCFG_MISCCONFIGr(unit, &rval));
        if (soc_reg_field_get(unit, MMU_SEDCFG_MISCCONFIGr, rval, ITU_MODE_SELf)) {
            meter_flags |= _BCM_TD_METER_FLAG_NON_LINEAR;
        }
    } else
#endif
    {
        BCM_IF_ERROR_RETURN(READ_MMU_SCFG_MISCCONFIGr(unit, &rval));
        if (soc_reg_field_get(unit, MMU_SCFG_MISCCONFIGr, rval, ITU_MODE_SELf)) {
            meter_flags |= _BCM_TD_METER_FLAG_NON_LINEAR;
        }
    }

    refresh_bitsize = soc_mem_field_length(unit, config_mem, MAX_REFRESHf);
    bucket_bitsize = soc_mem_field_length(unit, config_mem, MAX_THD_SELf);

    BCM_IF_ERROR_RETURN
        (_bcm_td_rate_to_bucket_encoding(unit, max_quantum, kbits_burst_max,
                                           meter_flags, refresh_bitsize,
                                           bucket_bitsize, &refresh_rate_max,
                                           &bucketsize_max, &granularity_max));

    BCM_IF_ERROR_RETURN
        (_bcm_td_rate_to_bucket_encoding(unit, min_quantum, kbits_burst_min,
                                           meter_flags, refresh_bitsize,
                                           bucket_bitsize, &refresh_rate_min,
                                           &bucketsize_min, &granularity_min));

    sal_memset(entry, 0, sizeof(uint32)*SOC_MAX_MEM_WORDS);

    soc_mem_field32_set(unit, config_mem, &entry, MAX_METER_GRANf, granularity_max);
    soc_mem_field32_set(unit, config_mem, &entry, MAX_REFRESHf, refresh_rate_max);
    soc_mem_field32_set(unit, config_mem, &entry, MAX_THD_SELf, bucketsize_max);

    soc_mem_field32_set(unit, config_mem, &entry, MIN_METER_GRANf, granularity_min);
    soc_mem_field32_set(unit, config_mem, &entry, MIN_REFRESHf, refresh_rate_min);
    soc_mem_field32_set(unit, config_mem, &entry, MIN_THD_SELf, bucketsize_min);

    soc_mem_field32_set(unit, config_mem, &entry, SHAPER_CONTROLf,
                        (flags & BCM_COSQ_BW_PACKET_MODE) ? 1 : 0);

    BCM_IF_ERROR_RETURN
        (soc_mem_write(unit, config_mem, MEM_BLOCK_ALL, index, &entry));

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_th_cosq_bucket_get
 * Purpose:
 *     Get COS queue bandwidth control bucket setting
 * Parameters:
 *     unit          - (IN) unit number
 *     gport         - (IN) port number or GPORT identifier
 *     cosq          - (IN) COS queue number
 *     min_quantum   - (OUT) kbps or packet/second
 *     max_quantum   - (OUT) kbps or packet/second
 *     burst_quantum - (OUT) kbps or packet/second
 *     flags         - (IN)
 * Returns:
 *     BCM_E_XXX
 * Notes:
 *     If port is any form of local port, cosq is the hardware queue index.
 */
STATIC int
_bcm_th_cosq_bucket_get(int unit, bcm_port_t gport, bcm_cos_queue_t cosq,
                        uint32 *min_quantum, uint32 *max_quantum,
                        uint32 *kbits_burst_min, uint32 *kbits_burst_max,
                        uint32 *flags)
{
    _bcm_th_cosq_node_t *node = NULL;
    bcm_port_t local_port;
    uint32 rval;
    uint32 refresh_rate, bucketsize, granularity, meter_flags;
    int pipe, phy_port, mmu_port, index;
    soc_mem_t config_mem = INVALIDm;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_info_t *si;
    int cpu_mc_base = 0;

    if (cosq < 0) {
        if (cosq == -1) {
            /* caller needs to resolve the wild card value (-1) */
            return BCM_E_INTERNAL;
        } else { /* reject all other negative value */
            return BCM_E_PARAM;
        }
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));

    si = &SOC_INFO(unit);
    phy_port = si->port_l2p_mapping[local_port];
    mmu_port = si->port_p2m_mapping[phy_port];
    pipe = si->port_pipe[local_port];

    if (BCM_GPORT_IS_SET(gport) &&
        ((BCM_GPORT_IS_SCHEDULER(gport)) ||
          BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
          BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport) ||
          BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(gport))) {

        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_node_get(unit, gport, NULL,
                                   NULL, NULL, &node));
    }

    if (node) {
        if (BCM_GPORT_IS_SCHEDULER(gport)) {
            if (node->level == SOC_TH_NODE_LVL_L0) {
                config_mem = MMU_MTRO_L0_MEMm;
                cosq = node->hw_index % _BCM_TH_NUM_SCHEDULER_PER_PORT;
                index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) *
                         _BCM_TH_NUM_SCHEDULER_PER_PORT) + cosq;
            } else {
                return BCM_E_PARAM;
            }
        } else if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
                   BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            config_mem = MMU_MTRO_L1_MEMm;
            cosq = node->hw_index % si->port_num_cosq[local_port];

            if (IS_CPU_PORT(unit, local_port)) {
                /*Adjust mc cosq for cpu port*/
                cpu_mc_base = si->port_cosq_base[CMIC_PORT(unit)];
                cosq = (node->hw_index - cpu_mc_base) %
                       _BCM_TH_NUM_CPU_MCAST_QUEUE;
                /* For CPU port's MC queue starting index: 660(L1 Memory) */
                config_mem = MMU_MTRO_L1_MEMm;
                index = SOC_TH_MMU_CPU_MCQ_OFFSET + cosq;
            } else if (SOC_PBMP_MEMBER(si->management_pbm, local_port)) {
                config_mem = MMU_MTRO_L1_MEMm;
                index = (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport) ?
                    SOC_TH_MMU_MGMT_MCQ_OFFSET : SOC_TH_MMU_MGMT_UCQ_OFFSET) + cosq;
            } else {
                index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) *
                         _BCM_TH_NUM_SCHEDULER_PER_PORT) + cosq;
                if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                     index += SOC_TH_NUM_UC_QUEUES_PER_PIPE;
                }
            }
        } else {
            return BCM_E_PARAM;
        }
    } else {
        if ((cosq < 0) || (cosq >= SOC_PORT_NUM_COSQ(unit, local_port))) {
            return BCM_E_PARAM;
        }
        if (!IS_CPU_PORT(unit, local_port)) {
            index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) *
                     _BCM_TH_NUM_SCHEDULER_PER_PORT) + cosq;
            config_mem = MMU_MTRO_L0_MEMm;
        } else {
            /* For CPU port, refer L1 memory (logical queue, starting index 660)
             * directly */
            config_mem = MMU_MTRO_L1_MEMm;
            index = (2 * SOC_TH_NUM_UC_QUEUES_PER_PIPE) + cosq;
        }
    }

    /*
     * COVERITY
     *
     * This checker will not be True.
     * It is kept intentionally as a defensive case for future development.
     */
    /* coverity[dead_error_line] */
    if (config_mem == INVALIDm) {
        return BCM_E_PARAM;
    }
    config_mem = SOC_MEM_UNIQUE_ACC(unit, config_mem)[pipe];

    if (config_mem == INVALIDm) {
        return BCM_E_PARAM;
    }

    index = SOC_TH_MMU_PIPED_MEM_INDEX(unit, local_port, config_mem, index);

    if (min_quantum == NULL || max_quantum == NULL ||
        kbits_burst_max == NULL || kbits_burst_min == NULL) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (soc_mem_read(unit, config_mem, MEM_BLOCK_ALL, index, &entry));

    meter_flags = 0;
    *flags = 0;
#ifdef BCM_TOMAHAWK2_SUPPORT
    if (soc_feature(unit, soc_feature_mmu_sed)) {
        BCM_IF_ERROR_RETURN(READ_MMU_SEDCFG_MISCCONFIGr(unit, &rval));
        if (soc_reg_field_get(unit, MMU_SEDCFG_MISCCONFIGr, rval, ITU_MODE_SELf)) {
            meter_flags |= _BCM_TD_METER_FLAG_NON_LINEAR;
        }
    } else
#endif
    {
        BCM_IF_ERROR_RETURN(READ_MMU_SCFG_MISCCONFIGr(unit, &rval));
        if (soc_reg_field_get(unit, MMU_SCFG_MISCCONFIGr, rval, ITU_MODE_SELf)) {
            meter_flags |= _BCM_TD_METER_FLAG_NON_LINEAR;
        }
    }
    if (soc_mem_field32_get(unit, config_mem, &entry, SHAPER_CONTROLf)) {
        meter_flags |= _BCM_TD_METER_FLAG_PACKET_MODE;
        *flags |= BCM_COSQ_BW_PACKET_MODE;
    }

    granularity = soc_mem_field32_get(unit, config_mem, &entry, MAX_METER_GRANf);
    refresh_rate = soc_mem_field32_get(unit, config_mem, &entry, MAX_REFRESHf);
    bucketsize = soc_mem_field32_get(unit, config_mem, &entry, MAX_THD_SELf);
    BCM_IF_ERROR_RETURN
        (_bcm_td_bucket_encoding_to_rate(unit, refresh_rate, bucketsize,
                                           granularity, meter_flags,
                                           max_quantum, kbits_burst_max));

    granularity = soc_mem_field32_get(unit, config_mem, &entry, MIN_METER_GRANf);
    refresh_rate = soc_mem_field32_get(unit, config_mem, &entry, MIN_REFRESHf);
    bucketsize = soc_mem_field32_get(unit, config_mem, &entry, MIN_THD_SELf);
    BCM_IF_ERROR_RETURN
        (_bcm_td_bucket_encoding_to_rate(unit, refresh_rate, bucketsize,
                                           granularity, meter_flags,
                                           min_quantum, kbits_burst_min));
    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_time_domain_get(int unit, int time_id, int *time_value)
{
    uint32 time_domain_val;
    soc_reg_t time_domain_reg = TIME_DOMAINr;
    
    if (time_id < 0 || time_id > _BCM_TH_NUM_TIME_DOMAIN - 1) {
        return SOC_E_PARAM;
    }

    if (time_value == NULL) {
        return SOC_E_PARAM;
    }
    
    BCM_IF_ERROR_RETURN(
        soc_reg32_get(unit, time_domain_reg, REG_PORT_ANY, time_id, &time_domain_val));
    *time_value = soc_reg_field_get(unit, time_domain_reg,
                                   time_domain_val, TIME_DOMAIN_FIELDf);
    return SOC_E_NONE;
}

STATIC int
_bcm_th_cosq_time_domain_set(int unit, int time_value, int *time_id)
{
    int id;
    uint32 time_domain_val;
    soc_reg_t time_domain_reg = TIME_DOMAINr;

    if (time_value < 0 || time_value > 63) {
        return SOC_E_PARAM;
    }
   
    for (id = 0; id < _BCM_TH_NUM_TIME_DOMAIN; id++) {
        BCM_IF_ERROR_RETURN(
            soc_reg32_get(unit, time_domain_reg, REG_PORT_ANY, id, &time_domain_val));
        if (!time_domain_info[id].ref_count) {
            soc_reg_field_set(unit, time_domain_reg, &time_domain_val, 
                              TIME_DOMAIN_FIELDf, time_value);
            time_domain_info[id].ref_count++;
            break;
        }
    }
    
    if (id == _BCM_TH_NUM_TIME_DOMAIN) {/* No field in TIME_DOMAINr is free,return ERR*/
        return BCM_E_RESOURCE;
    }

    if (time_id) {
        *time_id = id;
    }     
    SOC_IF_ERROR_RETURN(
        soc_reg32_set(unit, time_domain_reg, REG_PORT_ANY, id, time_domain_val));
    return SOC_E_NONE;
}

/*
 * Function:
 *     _bcm_th_cosq_wred_set
 * Purpose:
 *     Configure unicast queue WRED setting
 * Parameters:
 *     unit                - (IN) unit number
 *     port                - (IN) port number or GPORT identifier
 *     cosq                - (IN) COS queue number
 *     flags               - (IN) BCM_COSQ_DISCARD_XXX
 *     min_thresh          - (IN) Min Threshold
 *     max_thresh          - (IN) Max Threshold
 *     drop_probablity     - (IN) Probabilistic drop rate
 *     gain                - (IN)
 *     ignore_enable_flags - (IN)
 *     lflags              - (IN) Config Flags
 *     refresh_time        - (IN) actual refresh time
 * Returns:
 *     BCM_E_XXX
 * Notes:
 *     If port is any form of local port, cosq is the hardware queue index.
 */
STATIC int
_bcm_th_cosq_wred_set(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                      bcm_cosq_buffer_id_t buffer,
                      uint32 flags, uint32 min_thresh, uint32 max_thresh,
                      int drop_probability, int gain, int ignore_enable_flags,
                      uint32 lflags, int refresh_time)
{
    /* Local Variables */
    soc_mem_t wred_config_mem = INVALIDm;
    bcm_port_t local_port = -1;
    int index, i, start_index, end_index, count = 0;
    int end_offset, need_profile = 0, pipe, xpe, xpe_map, accessed = 0;
    uint32 profile_index, old_profile_index;
    uint32 wred_flags = 0;
    void *entries[9];
    soc_mem_t mems[9];
    static soc_mem_t wred_mems[9] = {
        MMU_WRED_DROP_CURVE_PROFILE_0m, MMU_WRED_DROP_CURVE_PROFILE_1m,
        MMU_WRED_DROP_CURVE_PROFILE_2m, MMU_WRED_DROP_CURVE_PROFILE_3m,
        MMU_WRED_DROP_CURVE_PROFILE_4m, MMU_WRED_DROP_CURVE_PROFILE_5m,
        MMU_WRED_DROP_CURVE_PROFILE_6m, MMU_WRED_DROP_CURVE_PROFILE_7m,
        MMU_WRED_DROP_CURVE_PROFILE_8m
    };
    mmu_wred_drop_curve_profile_0_entry_t entry_tcp_green;
    mmu_wred_drop_curve_profile_1_entry_t entry_tcp_yellow;
    mmu_wred_drop_curve_profile_2_entry_t entry_tcp_red;
    mmu_wred_drop_curve_profile_3_entry_t entry_nontcp_green;
    mmu_wred_drop_curve_profile_4_entry_t entry_nontcp_yellow;
    mmu_wred_drop_curve_profile_5_entry_t entry_nontcp_red;
    mmu_wred_drop_curve_profile_6_entry_t entry_ecn_green;
    mmu_wred_drop_curve_profile_7_entry_t entry_ecn_yellow;
    mmu_wred_drop_curve_profile_8_entry_t entry_ecn_red;
    mmu_wred_config_entry_t entry;
    int rate = 0;
    int from_pipe, to_pipe;
    int time_id,time,old_time,current_time_sel;
    int rv = SOC_E_NONE, exists = 0;

    if ((buffer != BCM_COSQ_BUFFER_ID_INVALID) && 
        ((buffer < 0) || (buffer >= _TH_XPES_PER_DEV))) {
        return BCM_E_PARAM;
    }
    
    wred_flags = flags | lflags;
    end_offset = 0;

    /* Check Flags for WRED res level - Device-SP/Port-SP/Queues */
    if ((wred_flags & BCM_COSQ_DISCARD_DEVICE) && (port == BCM_GPORT_INVALID)) {
        /* (Port == -1) For all Global SPs */
        index = 462;
        count = _TH_MMU_NUM_POOL;
        /*For all pipe*/
        from_pipe = 0;
        to_pipe = NUM_PIPE(unit) - 1;

    } else {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, port, &local_port));
        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
        from_pipe = to_pipe = pipe;

        if (wred_flags & BCM_COSQ_DISCARD_DEVICE) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, port, cosq,
                                            _BCM_TH_COSQ_INDEX_STYLE_WRED_DEVICE,
                                            NULL, &index, &count));
            /* (cosq == -1) For all Global SPs */
        } else if (wred_flags & BCM_COSQ_DISCARD_PORT) {
            /* Port SP - Given or all SPs of the port */
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, port, cosq,
                                            _BCM_TH_COSQ_INDEX_STYLE_WRED_PORT,
                                            NULL, &index, &count));
            /* (cosq == -1) For all SPs of the given port */
        } else {
            /* Per Queue WRED settings */
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, port, cosq,
                                            _BCM_TH_COSQ_INDEX_STYLE_WRED_QUEUE,
                                            NULL, &index, &count));
            /* (cosq == -1) For all Queues of the given port */
        }
    }


    end_offset = (count > 0) ? (count - 1) : 0;

    if (index < 0) {
        return BCM_E_PARAM;
    }
    if (refresh_time < 1) {
        return BCM_E_PARAM;
    }
    start_index = index;
    end_index = start_index + end_offset;

    rate = _bcm_th_percent_to_drop_prob(drop_probability);

    for (pipe = from_pipe; pipe <= to_pipe; pipe++ ) {
        /* Config MMU_WRED_CONFIGm for the respective resource */
        wred_config_mem = MMU_WRED_CONFIGm;

        for (i=0; i < 9; i++) {
            mems[i] = INVALIDm;
        }

        if (wred_flags & (BCM_COSQ_DISCARD_COLOR_ALL |
                          BCM_COSQ_DISCARD_TCP |
                          BCM_COSQ_DISCARD_NONTCP |
                          BCM_COSQ_DISCARD_ECT_MARKED |
                          BCM_COSQ_DISCARD_RESPONSIVE_DROP |
                          BCM_COSQ_DISCARD_NON_RESPONSIVE_DROP)) {
            need_profile = 1;

            if (!(flags & (BCM_COSQ_DISCARD_COLOR_GREEN |
                           BCM_COSQ_DISCARD_COLOR_YELLOW |
                           BCM_COSQ_DISCARD_COLOR_RED))) {
                flags |= BCM_COSQ_DISCARD_COLOR_ALL;
            }

            if (!(wred_flags & (BCM_COSQ_DISCARD_NONTCP |
                                BCM_COSQ_DISCARD_NON_RESPONSIVE_DROP |
                                BCM_COSQ_DISCARD_ECT_MARKED)) ||
                 ((wred_flags & BCM_COSQ_DISCARD_TCP) ||
                 (wred_flags & BCM_COSQ_DISCARD_RESPONSIVE_DROP))) {
                if (flags & BCM_COSQ_DISCARD_COLOR_GREEN) {
                    mems[0] = wred_mems[0];
                }
                if (flags & BCM_COSQ_DISCARD_COLOR_YELLOW) {
                    mems[1] = wred_mems[1];
                }
                if (flags & BCM_COSQ_DISCARD_COLOR_RED) {
                    mems[2] = wred_mems[2];
                }
            }
            if ((wred_flags & BCM_COSQ_DISCARD_NONTCP) ||
                (wred_flags & BCM_COSQ_DISCARD_NON_RESPONSIVE_DROP)){
                if (flags & BCM_COSQ_DISCARD_COLOR_GREEN) {
                    mems[3] = wred_mems[3];
                }
                if (flags & BCM_COSQ_DISCARD_COLOR_YELLOW) {
                    mems[4] = wred_mems[4];
                }
                if (flags & BCM_COSQ_DISCARD_COLOR_RED) {
                    mems[5] = wred_mems[5];
                }
            }
            if (wred_flags & BCM_COSQ_DISCARD_ECT_MARKED) {
                if (flags & BCM_COSQ_DISCARD_COLOR_GREEN) {
                    mems[6] = wred_mems[6];
                }
                if (flags & BCM_COSQ_DISCARD_COLOR_YELLOW) {
                    mems[7] = wred_mems[7];
                }
                if (flags & BCM_COSQ_DISCARD_COLOR_RED) {
                    mems[8] = wred_mems[8];
                }
            }
        }

        entries[0] = &entry_tcp_green;
        entries[1] = &entry_tcp_yellow;
        entries[2] = &entry_tcp_red;
        entries[3] = &entry_nontcp_green;
        entries[4] = &entry_nontcp_yellow;
        entries[5] = &entry_nontcp_red;
        entries[6] = &entry_ecn_green;
        entries[7] = &entry_ecn_yellow;
        entries[8] = &entry_ecn_red;

        if (buffer == BCM_COSQ_BUFFER_ID_INVALID) {
            xpe_map = SOC_INFO(unit).epipe_xpe_map[pipe];
        } else if (port == BCM_GPORT_INVALID) {
            xpe_map = (1 << buffer) & SOC_INFO(unit).epipe_xpe_map[pipe];
        } else {
            xpe_map = (1 << buffer);
        }

        for (xpe = 0; xpe < NUM_XPE(unit); xpe ++) {
            if (!(xpe_map & (1 << xpe))) {
                continue;
            }

            accessed = 1;
            for (index = start_index; index <= end_index; index++) {
                old_profile_index = 0xffffffff;
                BCM_IF_ERROR_RETURN
                     (soc_tomahawk_xpe_mem_read(unit, wred_config_mem, xpe, pipe,
                                                MEM_BLOCK_ALL, index, &entry));
            
                if (need_profile) {
                    old_profile_index = soc_mem_field32_get(unit, wred_config_mem,
                                                            &entry, PROFILE_INDEXf);
            
                    BCM_IF_ERROR_RETURN
                        (soc_profile_mem_get(unit, _bcm_th_wred_profile[unit],
                                             old_profile_index, 1, entries));
            
                    for (i = 0; i < 9; i++) {
                        if (mems[i] == INVALIDm) {
                            continue;
                        }
                        soc_mem_field32_set(unit, wred_mems[i], entries[i],
                                            MIN_THDf, min_thresh);
                        soc_mem_field32_set(unit, wred_mems[i], entries[i],
                                            MAX_THDf, max_thresh);
                        soc_mem_field32_set(unit, wred_mems[i], entries[i],
                                            MAX_DROP_RATEf, rate);
                    }
                    BCM_IF_ERROR_RETURN
                        (soc_profile_mem_add(unit, _bcm_th_wred_profile[unit],
                                             entries, 1, &profile_index));
                    soc_mem_field32_set(unit, wred_config_mem, &entry, PROFILE_INDEXf, profile_index);
                }
            
                soc_mem_field32_set(unit, wred_config_mem, &entry, WEIGHTf, gain);
                /* Some APIs only modify profiles */
                if (!ignore_enable_flags) {
                    soc_mem_field32_set(unit, wred_config_mem, &entry, CAP_AVERAGEf,
                                        flags & BCM_COSQ_DISCARD_CAP_AVERAGE ? 1 : 0);
                    soc_mem_field32_set(unit, wred_config_mem, &entry, WRED_ENf,
                                        flags & BCM_COSQ_DISCARD_ENABLE ? 1 : 0);
                    soc_mem_field32_set(unit, wred_config_mem, &entry, ECN_MARKING_ENf,
                                        flags & BCM_COSQ_DISCARD_MARK_CONGESTION ?  1 : 0);
                }
            
                current_time_sel = soc_mem_field32_get(unit, wred_config_mem, 
                                                       &entry, TIME_DOMAIN_SELf);
                time = refresh_time - 1;
                exists = 0;
                /* If the time value exist, update reference count only */
                for (time_id = 0; time_id < _BCM_TH_NUM_TIME_DOMAIN; time_id++) {
                    BCM_IF_ERROR_RETURN
                        (_bcm_th_cosq_time_domain_get(unit, time_id, &old_time));            
                    if (time == old_time) {
                        /* Only set exist flag if this entry reference count already update,
                                      otherwise update reference count */
                        if(time_id != current_time_sel){ 
                            soc_mem_field32_set(unit, wred_config_mem, &entry, 
                                                TIME_DOMAIN_SELf, time_id);
                            time_domain_info[time_id].ref_count++;
                            time_domain_info[current_time_sel].ref_count--;
                        }
                        exists = 1;
                        break;
                    }
                }
            
                if(!exists){
                    rv = _bcm_th_cosq_time_domain_set(unit, time, &time_id);
                    if(rv == SOC_E_NONE){
                        soc_mem_field32_set(unit, wred_config_mem, &entry, 
                                            TIME_DOMAIN_SELf, time_id);
                        time_domain_info[current_time_sel].ref_count--;
                    } else {
                        return rv;
                    }
                }
                BCM_IF_ERROR_RETURN
                    (soc_tomahawk_xpe_mem_write(unit, wred_config_mem, xpe, pipe,
                                                MEM_BLOCK_ALL, index, &entry));
            
                if (old_profile_index != 0xffffffff) {
                    SOC_IF_ERROR_RETURN
                        (soc_profile_mem_delete(unit, _bcm_th_wred_profile[unit],
                                                old_profile_index));
                }
            
            }
        }

    }

    /* pipe (port) and xpe (buffer) are not related */
    if (!accessed) {
        return BCM_E_PARAM;
    }
    
    return rv;
}

/*
 * Function:
 *     _bcm_th_cosq_wred_get
 * Purpose:
 *     Configure unicast queue WRED setting
 * Parameters:
 *     unit                - (IN) unit number
 *     port                - (IN) port number or GPORT identifier
 *     cosq                - (IN) COS queue number
 *     flags               - (IN/OUT) BCM_COSQ_DISCARD_XXX
 *     min_thresh          - (OUT) Min Threshold
 *     max_thresh          - (OUT) Max Threshold
 *     drop_probablity     - (OUT) Probabilistic drop rate
 *     gain                - (OUT)
 *     lflags              - (IN) Config flags
 *     refresh_time        - (OUT) Time Domain
 * Returns:
 *     BCM_E_XXX
 * Notes:
 *     If port is any form of local port, cosq is the hardware queue index.
 */
STATIC int
_bcm_th_cosq_wred_get(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                      bcm_cosq_buffer_id_t buffer,
                      uint32 *flags, uint32 *min_thresh, uint32 *max_thresh,
                      int *drop_probability, int *gain, uint32 lflags, int *refresh_time)
{
    soc_mem_t wred_config_mem = INVALIDm, mem = INVALIDm;
    bcm_port_t local_port = -1;
    mmu_wred_drop_curve_profile_0_entry_t entry_tcp_green;
    mmu_wred_drop_curve_profile_1_entry_t entry_tcp_yellow;
    mmu_wred_drop_curve_profile_2_entry_t entry_tcp_red;
    mmu_wred_drop_curve_profile_3_entry_t entry_nontcp_green;
    mmu_wred_drop_curve_profile_4_entry_t entry_nontcp_yellow;
    mmu_wred_drop_curve_profile_5_entry_t entry_nontcp_red;
    mmu_wred_drop_curve_profile_6_entry_t entry_ecn_green;
    mmu_wred_drop_curve_profile_7_entry_t entry_ecn_yellow;
    mmu_wred_drop_curve_profile_8_entry_t entry_ecn_red;
    void *entries[9];
    void *entry_p;
    int index, profile_index;
    mmu_wred_config_entry_t entry;
    uint32 wred_flags = 0;
    int rate = 0, pipe = -1, xpe;
    int time_id, time;

    if ((buffer != BCM_COSQ_BUFFER_ID_INVALID) && 
        ((buffer < 0) || (buffer >= _TH_XPES_PER_DEV))) {
        return BCM_E_PARAM;
    }

    if (flags) {
        wred_flags = *flags | lflags;
    } else {
        wred_flags = lflags;
    }

    if ((wred_flags & BCM_COSQ_DISCARD_DEVICE) && (port == BCM_GPORT_INVALID)) {
        /* (Port == -1) For all Global SPs */
        index = 462;
        pipe = 0;

    } else {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, port, &local_port));
        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

        /* Check Flags for WRED res level - Device-SP/Port-SP/Queues */
        if(wred_flags & BCM_COSQ_DISCARD_DEVICE) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, port, cosq,
                                            _BCM_TH_COSQ_INDEX_STYLE_WRED_DEVICE,
                                            NULL, &index, NULL));
        } else if (wred_flags & BCM_COSQ_DISCARD_PORT) {
            /* Port SP - Given or all SPs of the port */
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, port, cosq,
                                            _BCM_TH_COSQ_INDEX_STYLE_WRED_PORT,
                                            NULL, &index, NULL));
        } else {
            /* Per Queue WRED settings */
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, port, cosq,
                                            _BCM_TH_COSQ_INDEX_STYLE_WRED_QUEUE,
                                            NULL, &index, NULL));
        }
    }

    if ((index < 0) || (pipe == -1)) {
        return BCM_E_PARAM;
    }

    if (buffer == BCM_COSQ_BUFFER_ID_INVALID) {
        xpe = -1;
    } else if (port == BCM_GPORT_INVALID) {
        xpe = buffer;
        pipe = -1;
    } else if (SOC_INFO(unit).epipe_xpe_map[pipe] & (1 << buffer)) {
        xpe = buffer;
    } else {
        return BCM_E_PARAM;
    }

    wred_config_mem = MMU_WRED_CONFIGm;
    BCM_IF_ERROR_RETURN
        (soc_tomahawk_xpe_mem_read(unit, wred_config_mem, xpe, pipe,
                                   MEM_BLOCK_ALL, index, &entry));

    profile_index = soc_mem_field32_get(unit, wred_config_mem,
                                        &entry, PROFILE_INDEXf);

    if (!(wred_flags & (BCM_COSQ_DISCARD_NONTCP |
                        BCM_COSQ_DISCARD_NON_RESPONSIVE_DROP |
                        BCM_COSQ_DISCARD_ECT_MARKED))) {
        /* By default we will assume
           BCM_COSQ_DISCARD_TCP */
        if (wred_flags & BCM_COSQ_DISCARD_COLOR_YELLOW) {
            mem = MMU_WRED_DROP_CURVE_PROFILE_1m;
            entry_p = &entry_tcp_yellow;
        } else if (wred_flags & BCM_COSQ_DISCARD_COLOR_RED) {
            mem = MMU_WRED_DROP_CURVE_PROFILE_2m;
            entry_p = &entry_tcp_red;
        } else {
            /* By default we will assume
               BCM_COSQ_DISCARD_COLOR_GREEN */
            mem = MMU_WRED_DROP_CURVE_PROFILE_0m;
            entry_p = &entry_tcp_green;
        }
    } else if ((wred_flags & BCM_COSQ_DISCARD_NONTCP) ||
               (wred_flags & BCM_COSQ_DISCARD_NON_RESPONSIVE_DROP)) {
        if (wred_flags & BCM_COSQ_DISCARD_COLOR_YELLOW) {
            mem = MMU_WRED_DROP_CURVE_PROFILE_4m;
            entry_p = &entry_nontcp_yellow;
        } else if (wred_flags & BCM_COSQ_DISCARD_COLOR_RED) {
            mem = MMU_WRED_DROP_CURVE_PROFILE_5m;
            entry_p = &entry_nontcp_red;
        } else {
            /* By default we will assume
               BCM_COSQ_DISCARD_COLOR_GREEN */
            mem = MMU_WRED_DROP_CURVE_PROFILE_3m;
            entry_p = &entry_nontcp_green;
        }
    } else {
        if (wred_flags & BCM_COSQ_DISCARD_COLOR_YELLOW) {
            mem = MMU_WRED_DROP_CURVE_PROFILE_7m;
            entry_p = &entry_ecn_yellow;
        } else if (wred_flags & BCM_COSQ_DISCARD_COLOR_RED) {
            mem = MMU_WRED_DROP_CURVE_PROFILE_8m;
            entry_p = &entry_ecn_red;
        } else {
            /* By default we will assume
               BCM_COSQ_DISCARD_COLOR_GREEN */
            mem = MMU_WRED_DROP_CURVE_PROFILE_6m;
            entry_p = &entry_ecn_green;
        }
    }

    entries[0] = &entry_tcp_green;
    entries[1] = &entry_tcp_yellow;
    entries[2] = &entry_tcp_red;
    entries[3] = &entry_nontcp_green;
    entries[4] = &entry_nontcp_yellow;
    entries[5] = &entry_nontcp_red;
    entries[6] = &entry_ecn_green;
    entries[7] = &entry_ecn_yellow;
    entries[8] = &entry_ecn_red;

    BCM_IF_ERROR_RETURN
        (soc_profile_mem_get(unit, _bcm_th_wred_profile[unit],
                             profile_index, 1, entries));
    if (min_thresh != NULL) {
        *min_thresh = soc_mem_field32_get(unit, mem, entry_p, MIN_THDf);
    }
    if (max_thresh != NULL) {
        *max_thresh = soc_mem_field32_get(unit, mem, entry_p, MAX_THDf);
    }
    if (drop_probability != NULL) {
        rate = soc_mem_field32_get(unit, mem, entry_p, MAX_DROP_RATEf);
        *drop_probability = _bcm_th_drop_prob_to_percent(rate);
    }
    if (gain) {
        *gain = soc_mem_field32_get(unit, wred_config_mem, &entry, WEIGHTf);
    }

    /* Get the Time Domain value */
    if (refresh_time) {
        time_id = soc_mem_field32_get(unit, wred_config_mem, &entry, 
                                      TIME_DOMAIN_SELf);
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_time_domain_get(unit,time_id,&time));
        *refresh_time = time + 1;
    }

    /* Get flags */
    if (flags) {
        *flags &= ~(BCM_COSQ_DISCARD_CAP_AVERAGE | BCM_COSQ_DISCARD_ENABLE |
                    BCM_COSQ_DISCARD_MARK_CONGESTION);
        if (soc_mem_field32_get(unit, wred_config_mem, &entry, CAP_AVERAGEf)) {
            *flags |= BCM_COSQ_DISCARD_CAP_AVERAGE;
        }
        if (soc_mem_field32_get(unit, wred_config_mem, &entry, WRED_ENf)) {
            *flags |= BCM_COSQ_DISCARD_ENABLE;
        }
        if (soc_mem_field32_get(unit, wred_config_mem, &entry, ECN_MARKING_ENf)) {
            *flags |= BCM_COSQ_DISCARD_MARK_CONGESTION;
        }
    }

    return BCM_E_NONE;
}

STATIC _bcm_bst_cb_ret_t
_bcm_th_bst_index_resolve(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq,
                          bcm_bst_stat_id_t bid, int *pipe, int *start_hw_index,
                          int *end_hw_index, int *rcb_data, void **cb_data, int *bcm_rv)
{
    int phy_port, mmu_port, local_port;
    _bcm_th_cosq_node_t *node = NULL;
    soc_info_t *si;

    *bcm_rv = _bcm_th_cosq_localport_resolve(unit, gport, &local_port);

    if (BCM_E_NONE != (*bcm_rv)) {
        goto error;
    }

    si = &SOC_INFO(unit);
    phy_port = si->port_l2p_mapping[local_port];
    mmu_port = si->port_p2m_mapping[phy_port];

    *pipe = mmu_port / SOC_TH_MMU_PORT_STRIDE;

    if (bid == bcmBstStatIdDevice) {
        *start_hw_index = *end_hw_index = 0;
        *bcm_rv = BCM_E_NONE;
        return _BCM_BST_RV_OK;
    }

    if ((bid == bcmBstStatIdEgrPool) || (bid == bcmBstStatIdEgrMCastPool)) {
        if(_bcm_th_cosq_egress_sp_get(unit, gport, cosq, start_hw_index,
                                      end_hw_index)) {
            goto error;
        }
        return _BCM_BST_RV_OK;
    } else if (bid == bcmBstStatIdHeadroomPool) {
        /* ingress headroom pool */
        if (_bcm_th_cosq_ingress_hdrm_pool_get(unit, gport, cosq,
                                               start_hw_index, end_hw_index)) {
            goto error;
        }
        return _BCM_BST_RV_OK;
    } else if (bid == bcmBstStatIdIngPool) {
        /* ingress pool */
        if (_bcm_th_cosq_ingress_sp_get(unit, gport, cosq,
                                        start_hw_index, end_hw_index)) {
            goto error;
        }
        return _BCM_BST_RV_OK;
    }

    if (bid == bcmBstStatIdPortPool) {
        /* ingress pool */
        if (_bcm_th_cosq_ingress_sp_get(unit, gport, cosq,
                                        start_hw_index, end_hw_index)) {
            goto error;
        }
        *start_hw_index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) * 4) +
                          *start_hw_index;
        *end_hw_index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) * 4) +
                        *end_hw_index;
    } else if ((bid == bcmBstStatIdPriGroupShared) ||
               (bid == bcmBstStatIdPriGroupHeadroom)) {
        /* ingress PG */
        if (_bcm_th_cosq_ingress_pg_get(unit, gport, cosq,
                                        start_hw_index, end_hw_index)) {
            goto error;
        }
        *start_hw_index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) * 8) +
                          *start_hw_index;
        *end_hw_index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) * 8) +
                        *end_hw_index;
    } else if (bid == bcmBstStatIdEgrPortPoolSharedUcast ||
               bid == bcmBstStatIdEgrPortPoolSharedMcast) {
        /* egress pool */
        if (_bcm_th_cosq_egress_sp_get(unit, gport, cosq,
                                       start_hw_index, end_hw_index)) {
            goto error;
        }
        *start_hw_index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) * 4) +
                          *start_hw_index;
        *end_hw_index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) * 4) +
                        *end_hw_index;
    } else {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            if (bid != bcmBstStatIdUcast) {
                goto error;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_node_get(unit, gport, NULL,
                                       &local_port, NULL, &node));
            if (!node) {
                goto error;
            }
            cosq = node->hw_index % _BCM_TH_NUM_UCAST_QUEUE_PER_PORT;
            *start_hw_index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) *
                               _BCM_TH_NUM_UCAST_QUEUE_PER_PORT) + cosq;
            *end_hw_index = *start_hw_index;
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            if (bid != bcmBstStatIdMcast) {
                goto error;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_node_get(unit, gport, NULL,
                                       &local_port, NULL, &node));
            if (!node) {
                goto error;
            }
            cosq = node->hw_index % _BCM_TH_NUM_MCAST_QUEUE_PER_PORT;
            *start_hw_index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) *
                               _BCM_TH_NUM_MCAST_QUEUE_PER_PORT) + cosq;
            *end_hw_index = *start_hw_index;
        } else {
            if (cosq < 0) {
                if (bid == bcmBstStatIdUcast) {
                    if (IS_LB_PORT(unit, local_port)) {
                        goto error;
                    }
                    *start_hw_index = (mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) *
                                      _BCM_TH_NUM_UCAST_QUEUE_PER_PORT;
                    *end_hw_index = *start_hw_index +
                                    _BCM_TH_NUM_UCAST_QUEUE_PER_PORT - 1;
                } else {
                    *start_hw_index = (mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) *
                                      _BCM_TH_NUM_MCAST_QUEUE_PER_PORT;
                    *end_hw_index = *start_hw_index +
                                    _BCM_TH_NUM_MCAST_QUEUE_PER_PORT - 1;
                }
            } else {
                if (bid == bcmBstStatIdUcast) {
                    if (IS_LB_PORT(unit, local_port)) {
                        goto error;
                    }
                    *start_hw_index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) *
                                       _BCM_TH_NUM_UCAST_QUEUE_PER_PORT) + cosq;
                    *end_hw_index = *start_hw_index;
                } else {
                    *start_hw_index = ((mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1)) *
                                       _BCM_TH_NUM_MCAST_QUEUE_PER_PORT) + cosq;
                    *end_hw_index = *start_hw_index;
                }
            }
        }
    }

    *bcm_rv = BCM_E_NONE;
    return _BCM_BST_RV_OK;

error:
    *bcm_rv = BCM_E_PARAM;
    return _BCM_BST_RV_ERROR;
}

STATIC int
_bcm_th_cosq_bst_map_resource_to_gport_cos(int unit,
    bcm_bst_stat_id_t bid, int port, int index, bcm_gport_t *gport,
    bcm_cos_t *cosq)
{
    soc_info_t *si = &SOC_INFO(unit);
    _bcm_bst_resource_info_t *pres = NULL;
    int phy_port = 0;
    int mmu_port = 0;
    int pipe = 0;
    int mmu_port_base = 0;
    int stats_per_inst = 1;
    int stats_per_pipe = 1;

    if (bid >= bcmBstStatIdMaxCount) {
        LOG_INFO(BSL_LS_BCM_COSQ,
            (BSL_META_U(unit,
            "_bcm_th_cosq_bst_map_resource_to_gport_cos: unit=%d bid=0x%x\n"),
            unit, bid));
        return BCM_E_NONE;
    }

    pres = _BCM_BST_RESOURCE(unit, bid);
    stats_per_inst = (pres->num_stat_pp) / (pres->num_instance);
    index %= stats_per_inst;
    if (pres->flags & _BCM_BST_CMN_RES_F_PIPED) {
        stats_per_pipe = stats_per_inst / NUM_PIPE(unit);
        pipe = index / stats_per_pipe;
        index %= stats_per_pipe;
        mmu_port_base = SOC_TH_MMU_PORT_STRIDE * pipe;
    }

    switch (bid) {
    case bcmBstStatIdDevice:
        *gport = -1;
        *cosq = BCM_COS_INVALID;
        break;

    case bcmBstStatIdIngPool:
    case bcmBstStatIdEgrPool:
    case bcmBstStatIdEgrMCastPool:
        *gport = -1;
        *cosq = index % _TH_MMU_NUM_POOL;
        break;

    case bcmBstStatIdPortPool:
        mmu_port= index/_TH_MMU_NUM_POOL;
        phy_port = si->port_m2p_mapping[mmu_port + mmu_port_base];
        *gport = si->port_p2l_mapping[phy_port];
        *cosq = index % _TH_MMU_NUM_POOL;
        break;

    case bcmBstStatIdPriGroupHeadroom:
    case bcmBstStatIdPriGroupShared:
        mmu_port = index/_TH_MMU_NUM_PG;
        phy_port = si->port_m2p_mapping[mmu_port + mmu_port_base];
        *gport = si->port_p2l_mapping[phy_port];
        *cosq = index % _TH_MMU_NUM_PG;
        break;

    case bcmBstStatIdUcast:
    case bcmBstStatIdMcast:
        mmu_port = index/10;
        phy_port = si->port_m2p_mapping[mmu_port + mmu_port_base];
        *gport = si->port_p2l_mapping[phy_port];
        *cosq = index % 10;
        break;
    case bcmBstStatIdEgrPortPoolSharedUcast:
    case bcmBstStatIdEgrPortPoolSharedMcast:
        mmu_port = index/_TH_MMU_NUM_POOL;
        phy_port = si->port_m2p_mapping[mmu_port + mmu_port_base];
        *gport = si->port_p2l_mapping[phy_port];
        *cosq = index % _TH_MMU_NUM_POOL;
        break;
    default:
        break;
    }

    return BCM_E_NONE;
}


int
_bcm_th_cosq_pfc_class_resolve(bcm_switch_control_t sctype, int *type,
                               int *class)
{
    switch (sctype) {
        case bcmSwitchPFCClass7Queue:
        case bcmSwitchPFCClass6Queue:
        case bcmSwitchPFCClass5Queue:
        case bcmSwitchPFCClass4Queue:
        case bcmSwitchPFCClass3Queue:
        case bcmSwitchPFCClass2Queue:
        case bcmSwitchPFCClass1Queue:
        case bcmSwitchPFCClass0Queue:
            *type = _BCM_TH_COSQ_TYPE_UCAST;
            break;
        case bcmSwitchPFCClass7McastQueue:
        case bcmSwitchPFCClass6McastQueue:
        case bcmSwitchPFCClass5McastQueue:
        case bcmSwitchPFCClass4McastQueue:
        case bcmSwitchPFCClass3McastQueue:
        case bcmSwitchPFCClass2McastQueue:
        case bcmSwitchPFCClass1McastQueue:
        case bcmSwitchPFCClass0McastQueue:
            *type = _BCM_TH_COSQ_TYPE_MCAST;
            break;
        default:
            /* Unsupported Switch Control types should return E_UNAVAIL */
            return BCM_E_UNAVAIL;
    }

    switch (sctype) {
        case bcmSwitchPFCClass7Queue:
        case bcmSwitchPFCClass7McastQueue:
            *class = 7;
            break;
        case bcmSwitchPFCClass6Queue:
        case bcmSwitchPFCClass6McastQueue:
            *class = 6;
            break;
        case bcmSwitchPFCClass5Queue:
        case bcmSwitchPFCClass5McastQueue:
            *class = 5;
            break;
        case bcmSwitchPFCClass4Queue:
        case bcmSwitchPFCClass4McastQueue:
            *class = 4;
            break;
        case bcmSwitchPFCClass3Queue:
        case bcmSwitchPFCClass3McastQueue:
            *class = 3;
            break;
        case bcmSwitchPFCClass2Queue:
        case bcmSwitchPFCClass2McastQueue:
            *class = 2;
            break;
        case bcmSwitchPFCClass1Queue:
        case bcmSwitchPFCClass1McastQueue:
            *class = 1;
            break;
        case bcmSwitchPFCClass0Queue:
        case bcmSwitchPFCClass0McastQueue:
            *class = 0;
            break;
        /*
         * COVERITY
         *
         * This default is unreachable but needed for some compiler.
         */
        /* coverity[dead_error_begin] */
        default:
            return BCM_E_INTERNAL;
    }
    return BCM_E_NONE;
}

/* Function to Set Default Scheduler Configuration for all the ports */
STATIC int
_bcm_th_cosq_sched_default_config(int unit)
{
    _bcm_th_cosq_port_info_t *port_info = NULL;
    _bcm_th_cosq_cpu_port_info_t *cpu_port_info = NULL;
    bcm_port_t port;
    int default_mode = BCM_COSQ_WEIGHTED_ROUND_ROBIN, default_weight = 1;
    int i;

    port = 0;
    cpu_port_info = &_bcm_th_cosq_cpu_port_info[unit][port];
    for (i = 0; i < _BCM_TH_NUM_SCHEDULER_PER_PORT; i++) {
        BCM_IF_ERROR_RETURN
            (bcm_th_cosq_gport_sched_set(unit, cpu_port_info->sched[i].gport,
                -1, default_mode, default_weight));
    }
    for (i = 0; i < _BCM_TH_NUM_CPU_MCAST_QUEUE; i++) {
        BCM_IF_ERROR_RETURN
            (bcm_th_cosq_gport_sched_set(unit, cpu_port_info->mcast[i].gport,
                -1, default_mode, default_weight));
    }

    PBMP_ALL_ITER(unit, port) {
        if (IS_CPU_PORT(unit, port) || IS_LB_PORT(unit, port)) {
            continue;
        } else {
            port_info = &_bcm_th_cosq_port_info[unit][port];
        }
        for (i = 0; i < _BCM_TH_NUM_SCHEDULER_PER_PORT; i++) {
            BCM_IF_ERROR_RETURN
                (bcm_th_cosq_gport_sched_set(unit, port_info->sched[i].gport,
                    -1, default_mode, default_weight));
        }
        for (i = 0; i < _BCM_TH_NUM_UCAST_QUEUE_PER_PORT; i++) {
            BCM_IF_ERROR_RETURN
                (bcm_th_cosq_gport_sched_set(unit, port_info->ucast[i].gport,
                    -1, default_mode, default_weight));
        }
        for (i = 0; i < _BCM_TH_NUM_MCAST_QUEUE_PER_PORT; i++) {
            BCM_IF_ERROR_RETURN
                (bcm_th_cosq_gport_sched_set(unit, port_info->mcast[i].gport,
                    -1, default_mode, default_weight));
        }
    }
    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_gport_default_config(int unit)
{
    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_sched_default_config(unit));

    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_cpu_tree_create(int unit, bcm_gport_t *sched_gport)
{
    bcm_gport_t cpu_mc_gport[_BCM_TH_NUM_CPU_MCAST_QUEUE];
    int id, cmc, pci_cmc = SOC_PCI_CMC(unit);
    int ch_l0_map[SOC_CMCS_NUM_MAX];

    if (SOC_CMCS_NUM(unit) > SOC_CMCS_NUM_MAX) {
        return BCM_E_INIT;
    }

    for (cmc = 0; cmc < SOC_CMCS_NUM(unit); cmc++) {
        if (cmc == pci_cmc) {
            ch_l0_map[cmc] = 0;
        } else if (cmc == SOC_ARM_CMC(unit, 0)) {
            ch_l0_map[cmc] = 7;
        } else if (cmc == SOC_ARM_CMC(unit, 1)) {
            ch_l0_map[cmc] = 8;
        } else {
            return BCM_E_PARAM;
        }
    }

    /* Adding MC queues and attaching them to Schedular nodes */
    for (id = 0; id < _BCM_TH_NUM_CPU_MCAST_QUEUE; id++) {
        cmc = (id < NUM_CPU_ARM_COSQ(unit, pci_cmc)) ? pci_cmc :
              (id < (NUM_CPU_ARM_COSQ(unit, pci_cmc) +
                     NUM_CPU_ARM_COSQ(unit, 1))) ? SOC_ARM_CMC(unit, 0) :
                     SOC_ARM_CMC(unit, 1);
        BCM_IF_ERROR_RETURN
            (bcm_th_cosq_gport_add(unit, 0, 1,
                                   BCM_COSQ_GPORT_MCAST_QUEUE_GROUP,
                                   &cpu_mc_gport[id]));
        BCM_IF_ERROR_RETURN
            (bcm_th_cosq_gport_attach(unit, cpu_mc_gport[id],
                                      sched_gport[ch_l0_map[cmc]], id));
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_th_cosq_gport_tree_create
 * Purpose:
 *      During cosq init create the default cosq gport tree.
 *      Schedualar (L0) nodes are created and attach to physical ports.
 *      UC and MC queues are added and attached to each L0 nodes.
 * Parameters:
 *      unit - (IN) unit number.
 *      port - (IN) port number
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_th_cosq_gport_tree_create(int unit, bcm_port_t port)
{
    int id;
    bcm_gport_t sched_gport[_BCM_TH_NUM_SCHEDULER_PER_PORT];
    bcm_gport_t uc_gport[_BCM_TH_NUM_UCAST_QUEUE_PER_PORT];
    bcm_gport_t mc_gport[_BCM_TH_NUM_MCAST_QUEUE_PER_PORT];
    int numq = 0;

    if (IS_CPU_PORT(unit, port)) {
        numq = 48;
    } else {
        numq = 2;
    }
    /* Adding L0 (schedular) nodes and attaching them to front panel and cpu port */
    for (id = 0; id < _BCM_TH_NUM_SCHEDULER_PER_PORT; id++) {
        BCM_IF_ERROR_RETURN
            (bcm_th_cosq_gport_add(unit, port, numq, BCM_COSQ_GPORT_SCHEDULER,
                                   &sched_gport[id]));
        BCM_IF_ERROR_RETURN
            (bcm_th_cosq_gport_attach(unit, sched_gport[id], port, id));
    }

    numq = 1;
    if (IS_CPU_PORT(unit, port)) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_cpu_tree_create(unit, sched_gport));
    } else {
        /* Adding UC queues and attaching them to Schedular nodes */
        for (id = 0; id < _BCM_TH_NUM_UCAST_QUEUE_PER_PORT; id++) {
            BCM_IF_ERROR_RETURN
                (bcm_th_cosq_gport_add(unit, port, numq,
                                       BCM_COSQ_GPORT_UCAST_QUEUE_GROUP,
                                       &uc_gport[id]));
            BCM_IF_ERROR_RETURN
                (bcm_th_cosq_gport_attach(unit, uc_gport[id],
                                          sched_gport[id], id));
        }

        /* Adding MC queues and attaching them to Schedular nodes */
        for (id = 0; id < _BCM_TH_NUM_MCAST_QUEUE_PER_PORT; id++) {
            BCM_IF_ERROR_RETURN
                (bcm_th_cosq_gport_add(unit, port, numq,
                                       BCM_COSQ_GPORT_MCAST_QUEUE_GROUP,
                                       &mc_gport[id]));
            BCM_IF_ERROR_RETURN
                (bcm_th_cosq_gport_attach(unit, mc_gport[id],
                                          sched_gport[id], id));
        }
    }

    return SOC_E_NONE;
}

/*
 * Function:
 *     bcm_th_ecn_init
 * Purpose:
 *     Initialize (clear) all ECN memories/registers
 * Parameters:
 *     unit - unit number
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_th_ecn_init(int unit)
{
    egr_ip_to_int_cn_mapping_entry_t entry_egr_ip_int_cn;
    int count, i;
    uint32  fld_val;

    BCM_IF_ERROR_RETURN(_bcm_esw_ecn_init(unit));

    /* Perform additional Tomahawk specific ECN initialization */
    /* EGR_IP_TO_INT_CN_MAPPING is identical to IP_TO_INT_CN_MAPPING table
     */
    if (!SOC_WARM_BOOT(unit)) {
        count = soc_mem_index_count(unit, EGR_IP_TO_INT_CN_MAPPINGm);
        for (i = 0; i < count; i++) {
            sal_memset(&entry_egr_ip_int_cn, 0, sizeof(entry_egr_ip_int_cn));
            switch (i) {
                case 0:
                    fld_val = _BCM_ECN_INT_CN_NON_RESPONSIVE_DROP;
                    break;
                case 1:
                    fld_val = _BCM_ECN_INT_CN_RESPONSIVE_DROP;
                    break;
                case 6:
                case 7:
                    fld_val = _BCM_ECN_INT_CN_CONGESTION_MARK_ECN;
                    break;
                default:
                    fld_val = _BCM_ECN_INT_CN_NON_CONGESTION_MARK_ECN;
                    break;
            }
            soc_mem_field32_set(unit, EGR_IP_TO_INT_CN_MAPPINGm,
                &entry_egr_ip_int_cn, INT_CNf, fld_val);
            SOC_IF_ERROR_RETURN
                (soc_mem_write(unit, EGR_IP_TO_INT_CN_MAPPINGm, MEM_BLOCK_ALL,
                               i, (void *)&entry_egr_ip_int_cn));
        }
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_th_cosq_cleanup
 * Purpose:
 *     Clean resource in case of error during Cosq Init
 * Parameters:
 *     unit - unit number
 * Returns:
 *     No return value
 */
STATIC void
_bcm_th_cosq_cleanup(int unit)
{
    if (_bcm_th_cosq_port_info[unit] != NULL) {
        sal_free(_bcm_th_cosq_port_info[unit]);
        _bcm_th_cosq_port_info[unit] = NULL;
    }

    if (_bcm_th_cosq_cpu_port_info[unit] != NULL) {
        sal_free(_bcm_th_cosq_cpu_port_info[unit]);
        _bcm_th_cosq_cpu_port_info[unit] = NULL;
    }

    if (_bcm_th_mmu_info[unit] != NULL) {
        sal_free(_bcm_th_mmu_info[unit]);
        _bcm_th_mmu_info[unit] = NULL;
    }

    if (_bcm_th_cos_map_profile[unit] != NULL) {
        soc_profile_mem_destroy(unit, _bcm_th_cos_map_profile[unit]);
        sal_free(_bcm_th_cos_map_profile[unit]);
        _bcm_th_cos_map_profile[unit] = NULL;
    }

    if (_bcm_th_ifp_cos_map_profile[unit] != NULL) {
        soc_profile_mem_destroy(unit, _bcm_th_ifp_cos_map_profile[unit]);
        sal_free(_bcm_th_ifp_cos_map_profile[unit]);
        _bcm_th_ifp_cos_map_profile[unit] = NULL;
    }

    if (_bcm_th_wred_profile[unit] != NULL) {
        soc_profile_mem_destroy(unit, _bcm_th_wred_profile[unit]);
        sal_free(_bcm_th_wred_profile[unit]);
        _bcm_th_wred_profile[unit] = NULL;
    }

    if (_bcm_th_prio2cos_profile[unit] != NULL) {
        soc_profile_reg_destroy(unit, _bcm_th_prio2cos_profile[unit]);
        sal_free(_bcm_th_prio2cos_profile[unit]);
        _bcm_th_prio2cos_profile[unit] = NULL;
    }

#if defined(INCLUDE_PSTATS)
    if (_BCM_PSTATS_UNIT_DRIVER(unit) != NULL) {
        _bcm_pstats_deinit(unit);
    }
#endif /* INCLUDE_PSTATS */

#if defined (INCLUDE_TCB) && defined (BCM_TCB_SUPPORT)
#ifdef BCM_TOMAHAWK2_SUPPORT
    _bcm_th2_cosq_tcb_deinit(unit);
#endif
#endif

    (void)_bcm_bst_detach(unit);

#if defined(BCM_TOMAHAWK_SUPPORT) && defined(INCLUDE_L3)
    bcmi_ecn_map_clean_up(unit);
#endif
    return;
}

/*
 * Function:
 *     bcm_th_cosq_init
 * Purpose:
 *     Initialize (clear) all COS schedule/mapping state.
 * Parameters:
 *     unit - unit number
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_init(int unit)
{
    int i, numq, alloc_size, pipe = -1, xpe = -1;
    soc_mem_t mem = INVALIDm;
    bcm_port_t port;
    soc_reg_t reg;
    static soc_mem_t wred_mems[9] = {
        MMU_WRED_DROP_CURVE_PROFILE_0m, MMU_WRED_DROP_CURVE_PROFILE_1m,
        MMU_WRED_DROP_CURVE_PROFILE_2m, MMU_WRED_DROP_CURVE_PROFILE_3m,
        MMU_WRED_DROP_CURVE_PROFILE_4m, MMU_WRED_DROP_CURVE_PROFILE_5m,
        MMU_WRED_DROP_CURVE_PROFILE_6m, MMU_WRED_DROP_CURVE_PROFILE_7m,
        MMU_WRED_DROP_CURVE_PROFILE_8m
    };
    int entry_words[9];
    mmu_wred_drop_curve_profile_0_entry_t entry_tcp_green;
    mmu_wred_drop_curve_profile_1_entry_t entry_tcp_yellow;
    mmu_wred_drop_curve_profile_2_entry_t entry_tcp_red;
    mmu_wred_drop_curve_profile_3_entry_t entry_nontcp_green;
    mmu_wred_drop_curve_profile_4_entry_t entry_nontcp_yellow;
    mmu_wred_drop_curve_profile_5_entry_t entry_nontcp_red;
    mmu_wred_drop_curve_profile_6_entry_t entry_ecn_green;
    mmu_wred_drop_curve_profile_7_entry_t entry_ecn_yellow;
    mmu_wred_drop_curve_profile_8_entry_t entry_ecn_red;
    void *entries[9];
    int ii, wred_prof_count;
    soc_mem_t wred_config_mem = INVALIDm;
    uint64 rval64s[16], *prval64s[1];
    uint32 profile_index;
    _bcm_bst_device_handlers_t bst_callbks;
    _bcm_th_cosq_cpu_cosq_config_t *cpu_cosq_config_p = NULL;
    int rv = BCM_E_NONE;
    soc_info_t *si = &SOC_INFO(unit);

    if (!SOC_WARM_BOOT(unit)) {
        _bcm_th_cosq_cleanup(unit);
    }

    numq = soc_property_get(unit, spn_BCM_NUM_COS, BCM_TH_COS_DEFAULT);

    if (numq < 1) {
        numq = 1;
    } else if (numq > BCM_TH_COS_MAX) {
        numq = BCM_TH_COS_MAX;
    }

    /* Create Port Information structure */
    alloc_size = sizeof(_bcm_th_cosq_port_info_t) * 136;
    if (_bcm_th_cosq_port_info[unit] == NULL) {
        _bcm_th_cosq_port_info[unit] =
            sal_alloc(alloc_size, "_bcm_th_cosq_port_info");
            if (_bcm_th_cosq_port_info[unit] == NULL) {
                return BCM_E_MEMORY;
            }
    }
    sal_memset(_bcm_th_cosq_port_info[unit], 0, alloc_size);

    /* Create CPU Port Information structure */
    alloc_size = sizeof(_bcm_th_cosq_cpu_port_info_t);
    if (_bcm_th_cosq_cpu_port_info[unit] == NULL) {
        _bcm_th_cosq_cpu_port_info[unit] =
            sal_alloc(alloc_size, "_bcm_th_cosq_cpu_port_info");
        if (_bcm_th_cosq_cpu_port_info[unit] == NULL) {
            _bcm_th_cosq_cleanup(unit);
            return BCM_E_MEMORY;
        }
    }
    sal_memset(_bcm_th_cosq_cpu_port_info[unit], 0, alloc_size);

    /* Create MMU Port Information structure */
    alloc_size = sizeof(_bcm_th_mmu_info_t);
    if (_bcm_th_mmu_info[unit] == NULL) {
        _bcm_th_mmu_info[unit] =
            sal_alloc(alloc_size, "_bcm_th_mmu_info");
        if (_bcm_th_mmu_info[unit] == NULL) {
            _bcm_th_cosq_cleanup(unit);
            return BCM_E_MEMORY;
        }
    }
    sal_memset(_bcm_th_mmu_info[unit], 0, alloc_size);
    _BCM_TH_MMU_INFO(unit)->_bcm_th_port_info =
            _bcm_th_cosq_port_info[unit];
    _BCM_TH_MMU_INFO(unit)->_bcm_th_cpu_port_info =
            _bcm_th_cosq_cpu_port_info[unit];

    /* Create profile for PORT_COS_MAP table */
    if (_bcm_th_cos_map_profile[unit] == NULL) {
        _bcm_th_cos_map_profile[unit] = sal_alloc(sizeof(soc_profile_mem_t),
                                                  "COS_MAP Profile Mem");
        if (_bcm_th_cos_map_profile[unit] == NULL) {
            _bcm_th_cosq_cleanup(unit);
            return BCM_E_MEMORY;
        }
        soc_profile_mem_t_init(_bcm_th_cos_map_profile[unit]);
    }
    mem = PORT_COS_MAPm;
    entry_words[0] = sizeof(port_cos_map_entry_t) / sizeof(uint32);

    rv = soc_profile_mem_create(unit, &mem, entry_words, 1,
                                _bcm_th_cos_map_profile[unit]);
    if (rv != SOC_E_NONE) {
        _bcm_th_cosq_cleanup(unit);
        return rv;
    }

    /* Create profile for IFP_COS_MAP table */
    if (_bcm_th_ifp_cos_map_profile[unit] == NULL) {
        _bcm_th_ifp_cos_map_profile[unit]
            = sal_alloc(sizeof(soc_profile_mem_t), "IFP_COS_MAP Profile Mem");
        if (_bcm_th_ifp_cos_map_profile[unit] == NULL) {
            _bcm_th_cosq_cleanup(unit);
            return BCM_E_MEMORY;
        }
        soc_profile_mem_t_init(_bcm_th_ifp_cos_map_profile[unit]);
    }
    mem = IFP_COS_MAPm;
    entry_words[0] = sizeof(ifp_cos_map_entry_t) / sizeof(uint32);

    rv = soc_profile_mem_create(unit, &mem, entry_words, 1,
                                _bcm_th_ifp_cos_map_profile[unit]);
    if (rv != SOC_E_NONE) {
        _bcm_th_cosq_cleanup(unit);
        return rv;
    }

    /* Create profile for MMU_WRED_DROP_CURVE_PROFILE_x tables */
    if (_bcm_th_wred_profile[unit] == NULL) {
        _bcm_th_wred_profile[unit] =
                sal_alloc(sizeof(soc_profile_mem_t), "WRED Profile Mem");
        if (_bcm_th_wred_profile[unit] == NULL) {
            _bcm_th_cosq_cleanup(unit);
            return BCM_E_MEMORY;
        }
        soc_profile_mem_t_init(_bcm_th_wred_profile[unit]);
    }


    entry_words[0] =
        sizeof(mmu_wred_drop_curve_profile_0_entry_t) / sizeof(uint32);
    entry_words[1] =
        sizeof(mmu_wred_drop_curve_profile_1_entry_t) / sizeof(uint32);
    entry_words[2] =
        sizeof(mmu_wred_drop_curve_profile_2_entry_t) / sizeof(uint32);
    entry_words[3] =
        sizeof(mmu_wred_drop_curve_profile_3_entry_t) / sizeof(uint32);
    entry_words[4] =
        sizeof(mmu_wred_drop_curve_profile_4_entry_t) / sizeof(uint32);
    entry_words[5] =
        sizeof(mmu_wred_drop_curve_profile_5_entry_t) / sizeof(uint32);
    entry_words[6] =
        sizeof(mmu_wred_drop_curve_profile_6_entry_t) / sizeof(uint32);
    entry_words[7] =
        sizeof(mmu_wred_drop_curve_profile_7_entry_t) / sizeof(uint32);
    entry_words[8] =
        sizeof(mmu_wred_drop_curve_profile_8_entry_t) / sizeof(uint32);

    rv = soc_profile_mem_create(unit, wred_mems, entry_words, 9,
                                _bcm_th_wred_profile[unit]);
    if (rv != SOC_E_NONE) {
        _bcm_th_cosq_cleanup(unit);
        return rv;
    }

    /* Create profile for PRIO2COS_PROFILE register */
    if (_bcm_th_prio2cos_profile[unit] == NULL) {
        _bcm_th_prio2cos_profile[unit] =
            sal_alloc(sizeof(soc_profile_reg_t), "PRIO2COS_PROFILE Profile Reg");
        if (_bcm_th_prio2cos_profile[unit] == NULL) {
            _bcm_th_cosq_cleanup(unit);
            return BCM_E_MEMORY;
        }
        soc_profile_reg_t_init(_bcm_th_prio2cos_profile[unit]);
    }
    reg = PRIO2COS_PROFILEr;
    rv = soc_profile_reg_create(unit, &reg, 1, _bcm_th_prio2cos_profile[unit]);
    if (rv != SOC_E_NONE) {
        _bcm_th_cosq_cleanup(unit);
        return rv;
    }

    if (_bcm_th_cosq_cpu_cosq_config[unit][0] == NULL) {
        cpu_cosq_config_p = sal_alloc(
                SOC_TH_NUM_CPU_QUEUES * sizeof(_bcm_th_cosq_cpu_cosq_config_t),
                "CPU Cosq Config");
        if (cpu_cosq_config_p == NULL) {
            _bcm_th_cosq_cleanup(unit);
            return BCM_E_MEMORY;
        }
        if (NUM_CPU_COSQ(unit) > SOC_TH_NUM_CPU_QUEUES) {
            /* Invalid CPU Queues */
            if (cpu_cosq_config_p != NULL) {
                sal_free(cpu_cosq_config_p);
                cpu_cosq_config_p = NULL;
            }
            _bcm_th_cosq_cleanup(unit);
            return BCM_E_INIT;
        }
        for (i = 0; i < NUM_CPU_COSQ(unit); i++) {
            _bcm_th_cosq_cpu_cosq_config[unit][i] = cpu_cosq_config_p + i;
        }
    }

    /* Requires ECN init to program the default Responsive/Non-responsive
     * settings in Hardware
     */
    if(soc_feature(unit, soc_feature_ecn_wred)) {
        rv = _bcm_th_ecn_init(unit);
        if (rv != BCM_E_NONE) {
            if (cpu_cosq_config_p != NULL) {
                sal_free(cpu_cosq_config_p);
                cpu_cosq_config_p = NULL;
            }
            _bcm_th_cosq_cleanup(unit);
            return rv;
        }
    }

#if defined(INCLUDE_PSTATS)
    if (soc_feature(unit, soc_feature_pstats)) {
        rv = _bcm_pstats_init(unit);
        if (rv != BCM_E_NONE) {
            if (cpu_cosq_config_p != NULL) {
                sal_free(cpu_cosq_config_p);
                cpu_cosq_config_p = NULL;
            }
            _bcm_th_cosq_cleanup(unit);
            return rv;
        }
    }
#endif /* INCLUDE_PSTATS */

    rv = _bcm_oob_init(unit);
    if (rv != BCM_E_NONE) {
        if (cpu_cosq_config_p != NULL) {
            sal_free(cpu_cosq_config_p);
            cpu_cosq_config_p = NULL;
        }
        _bcm_th_cosq_cleanup(unit);
        return rv;
    }


#if defined (INCLUDE_TCB) && defined (BCM_TCB_SUPPORT)
    if(soc_feature(unit, soc_feature_tcb)) {
#ifdef BCM_TOMAHAWK2_SUPPORT
        rv = bcm_th2_cosq_tcb_init(unit);
#endif        
    }        
    if (rv != BCM_E_NONE) {
        if (cpu_cosq_config_p != NULL) {
            sal_free(cpu_cosq_config_p);
            cpu_cosq_config_p = NULL;
        }
        _bcm_th_cosq_cleanup(unit);
        return rv;
    }
#endif

#ifdef BCM_WARM_BOOT_SUPPORT
    if (SOC_WARM_BOOT(unit)) {
        rv = bcm_th_cosq_reinit(unit);
        if (rv != BCM_E_NONE) {
            if (cpu_cosq_config_p != NULL) {
                sal_free(cpu_cosq_config_p);
                cpu_cosq_config_p = NULL;
            }
            _bcm_th_cosq_cleanup(unit);
        }
    /*
     * COVERITY
     *
     * This checker will not be True.
     * cpu_cosq_config_p pointer is assigned to
     * _bcm_th_cosq_cpu_cosq_config global data
     * structure.
     */
    /* coverity[resource_leak] */
        return rv;
    } else {
        rv = _bcm_th_cosq_wb_alloc(unit);
        if (rv != BCM_E_NONE) {
            if (cpu_cosq_config_p != NULL) {
                sal_free(cpu_cosq_config_p);
                cpu_cosq_config_p = NULL;
            }
            _bcm_th_cosq_cleanup(unit);
            return rv;
        }
    }
#endif /* BCM_WARM_BOOT_SUPPORT */

    /* Assign default init values for CPU info */
    for (i = 0; i < NUM_CPU_COSQ(unit); i++) {
        if (_bcm_th_cosq_cpu_cosq_config[unit][i] == NULL) {
            if (cpu_cosq_config_p != NULL) {
                sal_free(cpu_cosq_config_p);
                cpu_cosq_config_p = NULL;
            }
            _bcm_th_cosq_cleanup(unit);
            return BCM_E_MEMORY;
        }
        sal_memset(_bcm_th_cosq_cpu_cosq_config[unit][i], 0,
                   sizeof(_bcm_th_cosq_cpu_cosq_config_t));
        _bcm_th_cosq_cpu_cosq_config[unit][i]->enable = 1;
    }

    /* Add default entries for PORT_COS_MAP memory profile */
    rv = bcm_th_cosq_config_set(unit, numq);
    if (rv != BCM_E_NONE) {
        if (cpu_cosq_config_p != NULL) {
            sal_free(cpu_cosq_config_p);
            cpu_cosq_config_p = NULL;
        }
        _bcm_th_cosq_cleanup(unit);
        return rv;
    }

    for (pipe = 0; pipe < NUM_PIPE(unit); pipe++) {
        for (xpe = 0; xpe < NUM_XPE(unit); xpe++) {
            if (si->epipe_xpe_map[pipe] & (1 << xpe)) {
                mem = SOC_MEM_UNIQUE_ACC_XPE_PIPE(unit, MMU_WRED_CONFIGm,
                                                  xpe, pipe);
                rv = soc_mem_clear(unit, mem, COPYNO_ALL, 1);
                if (rv != SOC_E_NONE) {
                    if (cpu_cosq_config_p != NULL) {
                        sal_free(cpu_cosq_config_p);
                        cpu_cosq_config_p = NULL;
                    }
                    _bcm_th_cosq_cleanup(unit);
                    return rv;
                }
            }
        }
    }

    /* Add default entries for MMU_WRED_DROP_CURVE_PROFILE_x memory profile */
    sal_memset(&entry_tcp_green, 0, sizeof(entry_tcp_green));
    sal_memset(&entry_tcp_yellow,0, sizeof(entry_tcp_yellow));
    sal_memset(&entry_tcp_red, 0, sizeof(entry_tcp_red));
    sal_memset(&entry_nontcp_green, 0, sizeof(entry_nontcp_green));
    sal_memset(&entry_nontcp_yellow, 0, sizeof(entry_nontcp_yellow));
    sal_memset(&entry_nontcp_red, 0, sizeof(entry_nontcp_red));
    sal_memset(&entry_ecn_green, 0, sizeof(entry_ecn_green));
    sal_memset(&entry_ecn_yellow,0, sizeof(entry_ecn_yellow));
    sal_memset(&entry_ecn_red, 0, sizeof(entry_ecn_red));
    entries[0] = &entry_tcp_green;
    entries[1] = &entry_tcp_yellow;
    entries[2] = &entry_tcp_red;
    entries[3] = &entry_nontcp_green;
    entries[4] = &entry_nontcp_yellow;
    entries[5] = &entry_nontcp_red;
    entries[6] = &entry_ecn_green;
    entries[7] = &entry_ecn_yellow;
    entries[8] = &entry_ecn_red;

    for (ii = 0; ii < 9; ii++) {
        soc_mem_field32_set(unit, wred_mems[ii], entries[ii],
                            MIN_THDf, 0xffff);
        soc_mem_field32_set(unit, wred_mems[ii], entries[ii],
                            MAX_THDf, 0xffff);
    }
    profile_index = 0xffffffff;
    wred_config_mem = SOC_MEM_UNIQUE_ACC(unit, MMU_WRED_CONFIGm)[0];
    wred_prof_count = soc_mem_index_count(unit, wred_config_mem);

    /* In TH Profile table duplicate across all pipes all (related) xpes.
     * Ref count needs to be updated too */
    wred_prof_count *= (2 * NUM_PIPE(unit));
    while(wred_prof_count) {
        if (profile_index == 0xffffffff) {
            rv = soc_profile_mem_add(unit, _bcm_th_wred_profile[unit],
                                     entries, 1, &profile_index);
        } else {
            rv = soc_profile_mem_reference(unit, _bcm_th_wred_profile[unit],
                                           profile_index, 0);
        }
        if (rv != SOC_E_NONE) {
            if (cpu_cosq_config_p != NULL) {
                sal_free(cpu_cosq_config_p);
                cpu_cosq_config_p = NULL;
            }
            _bcm_th_cosq_cleanup(unit);
            return rv;
        }
        wred_prof_count -= 1;
    }

    /* Add default entries for PRIO2COS_PROFILE register profile */
    sal_memset(rval64s, 0, sizeof(rval64s));
    prval64s[0] = rval64s;
    profile_index = 0xffffffff;
    PBMP_PORT_ITER(unit, port) {
        if (profile_index == 0xffffffff) {
            rv = soc_profile_reg_add(unit, _bcm_th_prio2cos_profile[unit],
                                     prval64s, 16, &profile_index);
        } else {
            rv = soc_profile_reg_reference(unit, _bcm_th_prio2cos_profile[unit],
                                           profile_index, 0);
        }
        if (rv != SOC_E_NONE) {
            if (cpu_cosq_config_p != NULL) {
                sal_free(cpu_cosq_config_p);
                cpu_cosq_config_p = NULL;
            }
            _bcm_th_cosq_cleanup(unit);
            return rv;
        }
    }

    _BCM_TH_MMU_INFO(unit)->gport_tree_created = FALSE;
    PBMP_ALL_ITER(unit, port) {
        if (IS_LB_PORT(unit, port)) {
            continue;
        }
        rv = _bcm_th_cosq_gport_tree_create(unit, port);
        if (rv != BCM_E_NONE) {
            if (cpu_cosq_config_p != NULL) {
                sal_free(cpu_cosq_config_p);
                cpu_cosq_config_p = NULL;
            }
            _bcm_th_cosq_cleanup(unit);
            return rv;
        }
    }
    _BCM_TH_MMU_INFO(unit)->gport_tree_created = TRUE;

    rv = _bcm_th_cosq_gport_default_config(unit);
    if (rv != BCM_E_NONE) {
        if (cpu_cosq_config_p != NULL) {
            sal_free(cpu_cosq_config_p);
            cpu_cosq_config_p = NULL;
        }
        _bcm_th_cosq_cleanup(unit);
        return rv;
    }

    sal_memset(&bst_callbks, 0, sizeof(_bcm_bst_device_handlers_t));
    bst_callbks.resolve_index = &_bcm_th_bst_index_resolve;
    bst_callbks.reverse_resolve_index =
        &_bcm_th_cosq_bst_map_resource_to_gport_cos;
    rv = _bcm_bst_attach(unit, &bst_callbks);
    if (rv != BCM_E_NONE) {
        if (cpu_cosq_config_p != NULL) {
            sal_free(cpu_cosq_config_p);
            cpu_cosq_config_p = NULL;
        }
        _bcm_th_cosq_cleanup(unit);
        return rv;
    }

    if (soc_feature(unit, soc_feature_pfc_deadlock)) {
        rv = _bcm_pfc_deadlock_init(unit);
        if (rv != BCM_E_NONE) {
            if (cpu_cosq_config_p != NULL) {
                sal_free(cpu_cosq_config_p);
                cpu_cosq_config_p = NULL;
            }
            _bcm_th_cosq_cleanup(unit);
            return rv;
        }
    }

    rv = soc_th_mmu_get_shared_size(unit,
                     _BCM_TH_MMU_INFO(unit)->ing_shared_limit,
                     _BCM_TH_MMU_INFO(unit)->egr_db_shared_limit,
                     _BCM_TH_MMU_INFO(unit)->egr_qe_shared_limit);
    if (rv != SOC_E_NONE) {
        if (cpu_cosq_config_p != NULL) {
            sal_free(cpu_cosq_config_p);
            cpu_cosq_config_p = NULL;
        }
        _bcm_th_cosq_cleanup(unit);
        return rv;
    }
    
    /* reset time_domain ref_count so that entries can be added upon
     * rc */
    time_domain_info[0].ref_count = 466;
    time_domain_info[1].ref_count = 0;
    time_domain_info[2].ref_count = 0;
    time_domain_info[3].ref_count = 0;
    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_port_sched_set
 * Purpose:
 *     Set up class-of-service policy and corresponding weights and delay
 * Parameters:
 *     unit    - (IN) unit number
 *     pbm     - (IN) port bitmap
 *     mode    - (IN) Scheduling mode (BCM_COSQ_xxx)
 *     weights - (IN) Weights for each COS queue
 *     delay   - This parameter is not used
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_port_sched_set(int unit, bcm_pbmp_t pbm,
                           int mode, const int *weights, int delay)
{
    bcm_port_t port;
    int num_weights, i;

    BCM_PBMP_ITER(pbm, port) {
        num_weights = _TH_NUM_COS(unit);
        for (i = 0; i < num_weights; i++) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_sched_set(unit, port, i, mode, weights[i]));
        }
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_port_sched_get
 * Purpose:
 *     Retrieve class-of-service policy and corresponding weights and delay
 * Parameters:
 *     unit     - (IN) unit number
 *     pbm      - (IN) port bitmap
 *     mode     - (OUT) Scheduling mode (BCM_COSQ_XXX)
 *     weights  - (OUT) Weights for each COS queue
 *     delay    - This parameter is not used
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_port_sched_get(int unit, bcm_pbmp_t pbm,
                           int *mode, int *weights, int *delay)
{
    bcm_port_t port;
    int i, num_weights;

    BCM_PBMP_ITER(pbm, port) {
        if (IS_CPU_PORT(unit, port) && SOC_PBMP_NEQ(pbm, PBMP_CMIC(unit))) {
            continue;
        }
        num_weights = _TH_NUM_COS(unit);
        for (i = 0; i < num_weights; i++) {
            BCM_IF_ERROR_RETURN(
                _bcm_th_cosq_sched_get(unit, port, i, mode, &weights[i]));
        }
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_sched_weight_max_get
 * Purpose:
 *     Retrieve maximum weights for given COS policy
 * Parameters:
 *     unit    - (IN) unit number
 *     mode    - (IN) Scheduling mode (BCM_COSQ_xxx)
 *     weight_max - (OUT) Maximum weight for COS queue.
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_sched_weight_max_get(int unit, int mode, int *weight_max)
{
    switch (mode) {
    case BCM_COSQ_STRICT:
        *weight_max = BCM_COSQ_WEIGHT_STRICT;
        break;
    case BCM_COSQ_ROUND_ROBIN:
    case BCM_COSQ_WEIGHTED_ROUND_ROBIN:
    case BCM_COSQ_DEFICIT_ROUND_ROBIN:
         /* In TH the maximum weight will be 127 */
        *weight_max =
            (1 << soc_mem_field_length(unit, Q_SCHED_L0_WEIGHT_MEMm,
                                       WEIGHTf)) - 1;
        break;
    default:
        return BCM_E_PARAM;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_port_bandwidth_set
 * Purpose:
 *     Configure COS queue bandwidth control
 * Parameters:
 *     unit          - (IN) unit number
 *     port          - (IN) port number or GPORT identifier
 *     cosq          - (IN) COS queue number
 *     min_quantum   - (IN)
 *     max_quantum   - (IN)
 *     burst_quantum - (IN)
 *     flags         - (IN)
 * Returns:
 *     BCM_E_XXX
 * Notes:
 *     If port is any form of local port, cosq is the hardware queue index.
 */
int
bcm_th_cosq_port_bandwidth_set(int unit, bcm_port_t port,
                               bcm_cos_queue_t cosq,
                               uint32 min_quantum, uint32 max_quantum,
                               uint32 burst_quantum, uint32 flags)
{
    uint32 burst_min, burst_max;

    if (cosq < 0) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_SET(port) &&
        ((BCM_GPORT_IS_SCHEDULER(port)) ||
          BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
          BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
          BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port))) {
        return BCM_E_UNAVAIL;
    }

    burst_min = (min_quantum > 0) ?
          _bcm_td_default_burst_size(unit, port, min_quantum) : 0;

    burst_max = (max_quantum > 0) ?
          _bcm_td_default_burst_size(unit, port, max_quantum) : 0;

    return _bcm_th_cosq_bucket_set(unit, port, cosq, min_quantum, max_quantum,
                                    burst_min, burst_max, flags);
}

/*
 * Function:
 *     bcm_th_cosq_port_bandwidth_get
 * Purpose:
 *     Get COS queue bandwidth control configuration
 * Parameters:
 *     unit          - (IN) unit number
 *     port          - (IN) port number or GPORT identifier
 *     cosq          - (IN) COS queue number
 *     min_quantum   - (OUT)
 *     max_quantum   - (OUT)
 *     burst_quantum - (OUT)
 *     flags         - (OUT)
 * Returns:
 *     BCM_E_XXX
 * Notes:
 *     If port is any form of local port, cosq is the hardware queue index.
 */
int
bcm_th_cosq_port_bandwidth_get(int unit, bcm_port_t port,
                               bcm_cos_queue_t cosq,
                               uint32 *min_quantum, uint32 *max_quantum,
                               uint32 *burst_quantum, uint32 *flags)
{
    uint32 kbit_burst_min;

    if (cosq < -1) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_SET(port) &&
        ((BCM_GPORT_IS_SCHEDULER(port)) ||
          BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
          BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
          BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port))) {
        return BCM_E_UNAVAIL;
    }

    BCM_IF_ERROR_RETURN(_bcm_th_cosq_bucket_get(unit, port, cosq,
                        min_quantum, max_quantum, &kbit_burst_min,
                        burst_quantum, flags));
    return BCM_E_NONE;
}

int
bcm_th_cosq_port_pps_set(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                         int pps)
{
    uint32 min, max, burst, burst_min, flags;
    bcm_port_t  local_port =  port;

    if (!IS_CPU_PORT(unit, port)) {
        return BCM_E_PORT;
    } else if (cosq < 0 || cosq >= NUM_CPU_COSQ(unit)) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_SET(port) &&
        ((BCM_GPORT_IS_SCHEDULER(port)) ||
          BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
          BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
          BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port))) {
        return BCM_E_UNAVAIL;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_bucket_get(unit, port, cosq, &min, &max, &burst_min,
                                    &burst, &flags));
    min = pps;
    burst_min = (min > 0) ?
          _bcm_td_default_burst_size(unit, local_port, min) : 0;
    burst = burst_min;

    return _bcm_th_cosq_bucket_set(unit, port, cosq, min, pps, burst_min,
                                   burst, flags | BCM_COSQ_BW_PACKET_MODE);
}

int
bcm_th_cosq_port_pps_get(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                         int *pps)
{
    uint32 min, max, burst, flags;

    if (!IS_CPU_PORT(unit, port)) {
        return BCM_E_PORT;
    } else if (cosq < 0 || cosq >= NUM_CPU_COSQ(unit)) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_SET(port) &&
        ((BCM_GPORT_IS_SCHEDULER(port)) ||
          BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
          BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
          BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port))) {
        return BCM_E_UNAVAIL;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_bucket_get(unit, port, cosq, &min, &max, &burst, &burst,
                                 &flags));
    *pps = max;

    return BCM_E_NONE;
}

int
bcm_th_cosq_port_burst_set(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                           int burst)
{
    uint32 min, max, cur_burst, flags;

    if (!IS_CPU_PORT(unit, port)) {
        return BCM_E_PORT;
    } else if (cosq < 0 || cosq >= NUM_CPU_COSQ(unit)) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_SET(port) &&
        ((BCM_GPORT_IS_SCHEDULER(port)) ||
          BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
          BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
          BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port))) {
        return BCM_E_UNAVAIL;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_bucket_get(unit, port, cosq, &min, &max, &cur_burst,
                                 &cur_burst, &flags));

    /* Replace the current BURST setting, keep PPS the same */

    return _bcm_th_cosq_bucket_set(unit, port, cosq, min, max, burst, burst,
                                   flags | BCM_COSQ_BW_PACKET_MODE);
    return BCM_E_NONE;
}

int
bcm_th_cosq_port_burst_get(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                           int *burst)
{
    uint32 min, max, cur_burst, cur_burst_min, flags;

    if (!IS_CPU_PORT(unit, port)) {
        return BCM_E_PORT;
    } else if (cosq < 0 || cosq >= NUM_CPU_COSQ(unit)) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_SET(port) &&
        ((BCM_GPORT_IS_SCHEDULER(port)) ||
          BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
          BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
          BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port))) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_bucket_get(unit, port, cosq, &min, &max,
                                 &cur_burst_min, &cur_burst, &flags));
    *burst = cur_burst;
    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_gport_traverse
 * Purpose:
 *     Walks through the valid COSQ GPORTs and calls
 *     the user supplied callback function for each entry.
 * Parameters:
 *     unit       - (IN) unit number
 *     trav_fn    - (IN) Callback function.
 *     user_data  - (IN) User data to be passed to callback function
 * Returns:
 *     BCM_E_NONE - Success.
 *     BCM_E_XXX
 */
int
bcm_th_cosq_gport_traverse(int unit, bcm_cosq_gport_traverse_cb cb,
                           void *user_data)
{
    _bcm_th_cosq_port_info_t *port_info = NULL;
    _bcm_th_cosq_cpu_port_info_t *cpu_port_info = NULL;
    bcm_module_t my_modid, modid_out;
    bcm_port_t port, port_out, local_port;
    bcm_gport_t gport;
    int id, rv = SOC_E_NONE;

    if (_bcm_th_cosq_port_info[unit] == NULL) {
        return BCM_E_INIT;
    }
    if (_bcm_th_cosq_cpu_port_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &my_modid));
    PBMP_ALL_ITER(unit, port) {
        BCM_IF_ERROR_RETURN
            (_bcm_esw_stk_modmap_map(unit, BCM_STK_MODMAP_GET,
                                     my_modid, port, &modid_out, &port_out));
        BCM_GPORT_MODPORT_SET(gport, modid_out, port_out);

        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, port, &local_port));

        if (IS_CPU_PORT(unit, local_port)) {
            cpu_port_info = &_bcm_th_cosq_cpu_port_info[unit][local_port];
        } else {
            port_info = &_bcm_th_cosq_port_info[unit][local_port];
        }
        if (!IS_CPU_PORT(unit, local_port)) {
            for (id = 0; id < _BCM_TH_NUM_UCAST_QUEUE_PER_PORT; id++) {
                if (port_info->ucast[id].numq == 0) {
                    continue;
                }
                rv = cb(unit, gport, port_info->ucast[id].numq,
                        BCM_COSQ_GPORT_UCAST_QUEUE_GROUP,
                        port_info->ucast[id].gport, user_data);
#ifdef BCM_CB_ABORT_ON_ERR
                if (BCM_FAILURE(rv) && SOC_CB_ABORT_ON_ERR(unit)) {
                    return rv;
                }
#else
                /*
                 * If CB_ABORT_ON_ERR is not defined don't return error
                 * and continue.
                 */
                rv = BCM_E_NONE;
#endif
            }
        }
        if (IS_CPU_PORT(unit, local_port)) {
            for (id = 0; id < _BCM_TH_NUM_CPU_MCAST_QUEUE; id++) {
                if (cpu_port_info->mcast[id].numq == 0) {
                    continue;
                }
                rv = cb(unit, gport, cpu_port_info->mcast[id].numq,
                        BCM_COSQ_GPORT_MCAST_QUEUE_GROUP,
                        cpu_port_info->mcast[id].gport, user_data);
#ifdef BCM_CB_ABORT_ON_ERR
                if (BCM_FAILURE(rv) && SOC_CB_ABORT_ON_ERR(unit)) {
                    return rv;
                }
#else
                /*
                 * If CB_ABORT_ON_ERR is not defined don't return error
                 * and continue.
                 */
                rv = BCM_E_NONE;
#endif
            }
        } else {
            for (id = 0; id < _BCM_TH_NUM_MCAST_QUEUE_PER_PORT; id++) {
                if (port_info->mcast[id].numq == 0) {
                    continue;
                }
                rv = cb(unit, gport, port_info->mcast[id].numq,
                        BCM_COSQ_GPORT_MCAST_QUEUE_GROUP,
                        port_info->mcast[id].gport, user_data);
#ifdef BCM_CB_ABORT_ON_ERR
                if (BCM_FAILURE(rv) && SOC_CB_ABORT_ON_ERR(unit)) {
                    return rv;
                }
#else
                /*
                 * If CB_ABORT_ON_ERR is not defined don't return error
                 * and continue.
                 */
                rv = BCM_E_NONE;
#endif
            }
        }
        for (id = 0; id < _BCM_TH_NUM_SCHEDULER_PER_PORT; id++) {
            if (IS_CPU_PORT(unit, local_port)) {
                if (cpu_port_info->sched[id].numq == 0) {
                    continue;
                }
                rv = cb(unit, gport, cpu_port_info->sched[id].numq,
                        BCM_COSQ_GPORT_SCHEDULER,
                        cpu_port_info->sched[id].gport, user_data);
            } else {
                if (port_info->sched[id].numq == 0) {
                    continue;
                }
                rv = cb(unit, gport, port_info->sched[id].numq,
                        BCM_COSQ_GPORT_SCHEDULER,
                        port_info->sched[id].gport, user_data);
            }
#ifdef BCM_CB_ABORT_ON_ERR
            if (BCM_FAILURE(rv) && SOC_CB_ABORT_ON_ERR(unit)) {
                return rv;
            }
#else
            /*
             * If CB_ABORT_ON_ERR is not defined don't return error
             * and continue.
             */
            rv = BCM_E_NONE;
#endif
        }
    }
    return rv;
}

/*
 * Function:
 *     bcm_th_cosq_gport_sched_get
 * Purpose:
 *     Get COS queue schedular setting
 * Parameters:
 *     unit     - (IN) unit number
 *     gport    - (IN) GPORT identifier
 *     cosq     - (IN) COS queue to get, -1 for any queue
 *     mode     - (OUT) Scheduling mode (BCM_COSQ_XXX)
 *     weights  - (OUT) Weights for queue
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_gport_sched_get(int unit, bcm_gport_t gport,
                            bcm_cos_queue_t cosq, int *mode, int *weight)
{
    _bcm_th_cosq_node_t *node = NULL;
    int local_port = -1, cpu_mc_base = 0;
    soc_info_t *si = &SOC_INFO(unit);

    if (BCM_GPORT_IS_SET(gport)) {
        if ((BCM_GPORT_IS_SCHEDULER(gport)) ||
             BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
             BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {

            BCM_IF_ERROR_RETURN
                 (_bcm_th_cosq_node_get(unit, gport, NULL, &local_port, NULL, &node));

            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
                /* For Non-CPU and non-LB ports, num UC queues are 10 */
                cosq = node->hw_index % si->port_num_cosq[local_port];
            } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                /* For Non-CPU ports, num MC queues are 10
                 * For CPU ports, num MC queues are 48
                 */
                if (IS_CPU_PORT(unit, local_port)) {
                    cpu_mc_base = si->port_cosq_base[CMIC_PORT(unit)];
                    cosq = (node->hw_index - cpu_mc_base) %
                           _BCM_TH_NUM_CPU_MCAST_QUEUE;
                } else {
                    cosq = node->hw_index % si->port_num_cosq[local_port];
                }
            } else {
                /* For all ports, total scheduler nodes(L0) are 10 */
                cosq = node->hw_index % _BCM_TH_NUM_SCHEDULER_PER_PORT;
            }
            BCM_IF_ERROR_RETURN(
                _bcm_th_cosq_sched_get(unit, gport, cosq, mode, weight));
            return BCM_E_NONE;
        } else {
            BCM_IF_ERROR_RETURN(_bcm_th_cosq_localport_resolve(unit, gport,
                                                               &local_port));

            if ((cosq < 0) || (cosq >= SOC_PORT_NUM_COSQ(unit, local_port))) {
                return BCM_E_PARAM;
            }

            BCM_IF_ERROR_RETURN(
                _bcm_th_cosq_sched_get(unit, local_port, cosq, mode, weight));
            return BCM_E_NONE;
        }
    } else {
        return BCM_E_PORT;
    }

    return BCM_E_NOT_FOUND;
}

/*
 * Function:
 *     bcm_th_cosq_gport_sched_set
 * Purpose:
 *     Set up class-of-service policy and corresponding weights and delay
 * Parameters:
 *     unit    - (IN) unit number
 *     gport   - (IN) GPORT ID
 *     cosq    - (IN) COS queue to configure
 *     mode    - (IN) Scheduling mode (BCM_COSQ_xxx)
 *     weight - (IN) Weight for COS queue
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_gport_sched_set(int unit, bcm_gport_t gport,
                           bcm_cos_queue_t cosq, int mode, int weight)
{
    _bcm_th_cosq_node_t *node = NULL;
    soc_info_t *si = &SOC_INFO(unit);
    int local_port = -1, cpu_mc_base = 0;

    if (BCM_GPORT_IS_SET(gport)) {
        if (BCM_GPORT_IS_SCHEDULER(gport) ||
            BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
            BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {

            BCM_IF_ERROR_RETURN
                 (_bcm_th_cosq_node_get(unit, gport, NULL, &local_port,
                                        NULL, &node));

            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
                /* For Non-CPU and non-LB ports, num UC queues are 10 */
                cosq = node->hw_index % si->port_num_cosq[local_port];
            } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                /* For Non-CPU ports, num MC queues are 10
                 * For CPU ports, num MC queues are 48
                 */
                if (IS_CPU_PORT(unit, local_port)) {
                    cpu_mc_base = si->port_cosq_base[CMIC_PORT(unit)];
                    cosq = (node->hw_index - cpu_mc_base) %
                           _BCM_TH_NUM_CPU_MCAST_QUEUE;
                } else {
                    cosq = node->hw_index % si->port_num_cosq[local_port];
                }
            } else {
                /* For all ports, total scheduler nodes(L0) are 10 */
                cosq = node->hw_index % _BCM_TH_NUM_SCHEDULER_PER_PORT;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_sched_set(unit, gport, cosq, mode, weight));
            return BCM_E_NONE;
        } else {
            BCM_IF_ERROR_RETURN(_bcm_th_cosq_localport_resolve(unit, gport,
                                                               &local_port));

            if ((cosq < 0) || (cosq >= SOC_PORT_NUM_COSQ(unit, local_port))) {
                return BCM_E_PARAM;
            }

            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_sched_set(unit, local_port, cosq, mode, weight));
            return BCM_E_NONE;
        }
    } else {
        return BCM_E_PORT;
    }

    return BCM_E_NOT_FOUND;
}

/*
 * Function:
 *     bcm_th_cosq_gport_bandwidth_set
 * Purpose:
 *     Configure COS queue bandwidth control
 * Parameters:
 *     unit   - (IN) unit number
 *     gport  - (IN) GPORT identifier
 *     cosq   - (IN) COS queue to configure, -1 for all COS queues
 *     kbits_sec_min - (IN) minimum bandwidth, kbits/sec
 *     kbits_sec_max - (IN) maximum bandwidth, kbits/sec
 *     flags  - (IN) BCM_COSQ_BW_*
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_gport_bandwidth_set(int unit, bcm_gport_t gport,
                                bcm_cos_queue_t cosq, uint32 kbits_sec_min,
                                uint32 kbits_sec_max, uint32 flags)
{
    int i, start_cos=0, end_cos=0, local_port;
    _bcm_th_cosq_node_t *node = NULL;
    uint32 burst_min, burst_max;

    BCM_IF_ERROR_RETURN(_bcm_th_cosq_localport_resolve(unit, gport,
                                                       &local_port));
    if (BCM_GPORT_IS_SET(gport)) {
        if ((BCM_GPORT_IS_SCHEDULER(gport)) ||
          BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
          BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_node_get(unit, gport, NULL,
                                        NULL, NULL, &node));
            if (node != NULL) {
                start_cos = end_cos = node->hw_index;
            } else {
                cosq = 0;
            }
        } else {
            if (cosq < 0) {
                return BCM_E_PARAM;
            }
            start_cos = end_cos = cosq;
        }
    } else {
        if (cosq < 0) {
            return BCM_E_PARAM;
        }
        start_cos = end_cos = cosq;
    }

    burst_min = (kbits_sec_min > 0) ?
          _bcm_td_default_burst_size(unit, local_port, kbits_sec_min) : 0;

    burst_max = (kbits_sec_max > 0) ?
          _bcm_td_default_burst_size(unit, local_port, kbits_sec_max) : 0;

    for (i = start_cos; i <= end_cos; i++) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_bucket_set(unit, gport, i, kbits_sec_min,
                         kbits_sec_max, burst_min, burst_max, flags));
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_gport_bandwidth_get
 * Purpose:
 *     Get COS queue bandwidth control configuration
 * Parameters:
 *     unit   - (IN) unit number
 *     gport  - (IN) GPORT identifier
 *     cosq   - (IN) COS queue to get, -1 for any COS queue
 *     kbits_sec_min - (OUT) minimum bandwidth, kbits/sec
 *     kbits_sec_max - (OUT) maximum bandwidth, kbits/sec
 *     flags  - (OUT) BCM_COSQ_BW_*
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_gport_bandwidth_get(int unit, bcm_gport_t gport,
                                bcm_cos_queue_t cosq, uint32 *kbits_sec_min,
                                uint32 *kbits_sec_max, uint32 *flags)
{
    uint32 kbits_sec_burst;

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_bucket_get(unit, gport, cosq,
                                 kbits_sec_min, kbits_sec_max,
                                 &kbits_sec_burst, &kbits_sec_burst, flags));
    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_gport_bandwidth_burst_set
 * Purpose:
 *     Configure COS queue bandwidth burst setting
 * Parameters:
 *      unit        - (IN) unit number
 *      gport       - (IN) GPORT identifier
 *      cosq        - (IN) COS queue to configure, -1 for all COS queues
 *      kbits_burst - (IN) maximum burst, kbits
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_th_cosq_gport_bandwidth_burst_set(int unit, bcm_gport_t gport,
                                      bcm_cos_queue_t cosq,
                                      uint32 kbits_burst_min,
                                      uint32 kbits_burst_max)
{
    int i, start_cos=0, end_cos=0;
    uint32 kbits_sec_min, kbits_sec_max, kbits_sec_burst, flags;
    bcm_port_t local_port;

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));

    start_cos = end_cos = cosq;

    if (cosq == BCM_COS_INVALID) {
        start_cos = 0;
        end_cos = (IS_CPU_PORT(unit, local_port)) ?
                   NUM_CPU_COSQ(unit) - 1 : SOC_PORT_NUM_COSQ(unit, local_port)
                   - 1;
    }

    if (BCM_GPORT_IS_SET(gport)) {
        if ((BCM_GPORT_IS_SCHEDULER(gport)) ||
             BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
             BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            start_cos = end_cos = 0;
        }
    }

    for (i = start_cos; i <= end_cos; i++) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_bucket_get(unit, gport, i, &kbits_sec_min,
                                     &kbits_sec_max, &kbits_sec_burst,
                                     &kbits_sec_burst, &flags));
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_bucket_set(unit, gport, i, kbits_sec_min,
                                     kbits_sec_max, kbits_burst_min,
                                     kbits_burst_max, flags));
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_gport_bandwidth_burst_get
 * Purpose:
 *     Get COS queue bandwidth burst setting
 * Parameters:
 *     unit        - (IN) unit number
 *     gport       - (IN) GPORT identifier
 *     cosq        - (IN) COS queue to get, -1 for Queue 0
 *     kbits_burst - (OUT) maximum burst, kbits
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_gport_bandwidth_burst_get(int unit, bcm_gport_t gport,
                                      bcm_cos_queue_t cosq,
                                      uint32 *kbits_burst_min,
                                      uint32 *kbits_burst_max)
{
    uint32 kbits_sec_min, kbits_sec_max, flags;

    if (cosq < -1) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_bucket_get(unit, gport, cosq == -1 ? 0 : cosq,
                                 &kbits_sec_min, &kbits_sec_max, kbits_burst_min,
                                 kbits_burst_max, &flags));

    return BCM_E_NONE;
}

int
bcm_th_cosq_discard_set(int unit, uint32 flags)
{
    bcm_port_t port;

    if (_bcm_th_cosq_port_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    if (flags & ~(BCM_COSQ_DISCARD_DEVICE |
                  BCM_COSQ_DISCARD_NONTCP |
                  BCM_COSQ_DISCARD_COLOR_ALL |
                  BCM_COSQ_DISCARD_PORT |
                  BCM_COSQ_DISCARD_TCP |
                  BCM_COSQ_DISCARD_COLOR_GREEN |
                  BCM_COSQ_DISCARD_COLOR_YELLOW |
                  BCM_COSQ_DISCARD_COLOR_RED |
                  BCM_COSQ_DISCARD_CAP_AVERAGE |
                  BCM_COSQ_DISCARD_ENABLE |
                  BCM_COSQ_DISCARD_MARK_CONGESTION)) {
        return BCM_E_PARAM;
    }

    flags &= ~(BCM_COSQ_DISCARD_NONTCP | BCM_COSQ_DISCARD_COLOR_ALL);

    PBMP_PORT_ITER(unit, port) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_wred_set(unit, port, 0, BCM_COSQ_BUFFER_ID_INVALID, 
                                   flags, 0, 0, 0, 0,
                                   FALSE, BCM_COSQ_DISCARD_PORT, 1));
    }

    return BCM_E_NONE;
}

int
bcm_th_cosq_discard_get(int unit, uint32 *flags)
{
    bcm_port_t port;

    PBMP_PORT_ITER(unit, port) {
        *flags = 0;
        /* use setting from hardware cosq index 0 of the first port */
        return _bcm_th_cosq_wred_get(unit, port, 0, BCM_COSQ_BUFFER_ID_INVALID, 
                                     flags, NULL, NULL, NULL,
                                     NULL, BCM_COSQ_DISCARD_PORT, NULL);
    }

    return BCM_E_NOT_FOUND;
}

/*
 * Function:
 *     bcm_th_cosq_discard_port_set
 * Purpose:
 *     Configure port WRED setting
 * Parameters:
 *     unit          - (IN) unit number
 *     port          - (IN) port number or GPORT identifier
 *     cosq          - (IN) COS queue number
 *     color         - (IN)
 *     drop_start    - (IN)
 *     drop_slot     - (IN)
 *     average_time  - (IN)
 * Returns:
 *     BCM_E_XXX
 * Notes:
 *     If port is any form of local port, cosq is the hardware queue index.
 *     Same as TD2 except for Reg changes.
 */
int
bcm_th_cosq_discard_port_set(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                             uint32 color, int drop_start, int drop_slope,
                             int average_time)
{
    bcm_port_t local_port;
    bcm_pbmp_t pbmp;
    int gain;
    uint32 min_thresh, max_thresh, shared_limit, xpe_map;
    uint32 bits, flags = 0;
    int numq, i;

    if (drop_start < 0 || drop_start > 100 ||
        drop_slope < 0 || drop_slope > 90) {
        return BCM_E_PARAM;
    }

    /*
     * average queue size is reculated every 1us, the formula is
     * avg_qsize(t + 1) =
     *     avg_qsize(t) + (cur_qsize - avg_qsize(t)) / (2 ** gain)
     * gain = log2(average_time)
     */
    bits = average_time & 0xffff;
    if (bits != 0) {
        bits |= bits >> 1;
        bits |= bits >> 2;
        bits |= bits >> 4;
        bits |= bits >> 8;
        gain = _shr_popcount(bits) - 1;
    } else {
        gain = 0;
    }

    /*
     * per-port cell limit may be disabled.
     * per-port per-cos cell limit may be set to use dynamic method.
     * therefore drop_start percentage is based on per-device total shared
     * cells.
     */
    xpe_map = SOC_INFO(unit).ipipe_xpe_map[0]|SOC_INFO(unit).ipipe_xpe_map[1];
    _TH_ARRAY_MIN(_BCM_TH_MMU_INFO(unit)->egr_db_shared_limit, NUM_XPE(unit),
                  xpe_map, shared_limit);
    min_thresh = ((drop_start * shared_limit) + (100 - 1)) / 100;

    /* Calculate the max threshold. For a given slope (angle in
     * degrees), determine how many packets are in the range from
     * 0% drop probability to 100% drop probability. Add that
     * number to the min_treshold to the the max_threshold.
     */
    max_thresh = min_thresh + _bcm_th_angle_to_cells(drop_slope);
    if (max_thresh > TH_WRED_CELL_FIELD_MAX) {
        max_thresh = TH_WRED_CELL_FIELD_MAX;
    }

    if (BCM_GPORT_IS_SET(port)) {
        numq = 1;
        for (i = 0; i < numq; i++) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_wred_set(unit, port, cosq + i, 
                                       BCM_COSQ_BUFFER_ID_INVALID, color,
                                       min_thresh, max_thresh, 100, gain,
                                       TRUE, flags, 1));
        }
    } else {
        if (port == -1) {
            BCM_PBMP_ASSIGN(pbmp, PBMP_PORT_ALL(unit));
        } else if (!SOC_PORT_VALID(unit, port)) {
            return BCM_E_PORT;
        } else {
            BCM_PBMP_PORT_SET(pbmp, port);
        }

        BCM_PBMP_ITER(pbmp, local_port) {
            numq = 1;
            for (i = 0; i < numq; i++) {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_wred_set(unit, local_port, cosq + i,
                                           BCM_COSQ_BUFFER_ID_INVALID, color, 
                                           min_thresh, max_thresh, 100, gain,
                                           TRUE, 0, 1));
            }
        }
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_discard_port_get
 * Purpose:
 *     Get port WRED setting
 * Parameters:
 *     unit          - (IN) unit number
 *     port          - (IN) port number or GPORT identifier
 *     cosq          - (IN) COS queue number
 *     color         - (OUT)
 *     drop_start    - (OUT)
 *     drop_slot     - (OUT)
 *     average_time  - (OUT)
 * Returns:
 *     BCM_E_XXX
 * Notes:
 *     If port is any form of local port, cosq is the hardware queue index.
 *     Same as TD2 except for Reg changes.
 */
int
bcm_th_cosq_discard_port_get(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                             uint32 color, int *drop_start, int *drop_slope,
                             int *average_time)
{
    bcm_port_t local_port;
    bcm_pbmp_t pbmp;
    int gain, drop_prob;
    uint32 min_thresh, max_thresh, shared_limit, xpe_map;
    int refresh_time = 0;

    if (drop_start == NULL || drop_slope == NULL || average_time == NULL) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_SET(port)) {
        local_port = port;
    } else {
        if (port == -1) {
            BCM_PBMP_ASSIGN(pbmp, PBMP_PORT_ALL(unit));
        } else if (!SOC_PORT_VALID(unit, port)) {
            return BCM_E_PORT;
        } else {
            BCM_PBMP_PORT_SET(pbmp, port);
        }
        BCM_PBMP_ITER(pbmp, local_port) {
            break;
        }
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_wred_get(unit, local_port, cosq == -1 ? 0 : cosq,
                               BCM_COSQ_BUFFER_ID_INVALID,
                               &color, &min_thresh, &max_thresh, &drop_prob,
                               &gain, 0, &refresh_time));

    /*
     * average queue size is reculated every 8us, the formula is
     * avg_qsize(t + 1) =
     *     avg_qsize(t) + (cur_qsize - avg_qsize(t)) / (2 ** gain)
     */
    *average_time = (1 << gain);

    xpe_map = SOC_INFO(unit).ipipe_xpe_map[0]|SOC_INFO(unit).ipipe_xpe_map[1];
    _TH_ARRAY_MIN(_BCM_TH_MMU_INFO(unit)->egr_db_shared_limit, NUM_XPE(unit),
                  xpe_map, shared_limit);

    if (min_thresh > shared_limit) {
        min_thresh = shared_limit;
    }

    if (max_thresh > shared_limit) {
        max_thresh = shared_limit;
    }

    *drop_start = min_thresh * 100 / shared_limit;

    /* Calculate the slope using the min and max threshold.
     * The angle is calculated knowing drop probability at min
     * threshold is 0% and drop probability at max threshold is 100%.
     */
    *drop_slope = _bcm_th_cells_to_angle(max_thresh - min_thresh);

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_gport_discard_set
 * Purpose:
 *     Configure port WRED setting
 * Parameters:
 *     unit    - (IN) unit number
 *     port    - (IN) GPORT identifier
 *     cosq    - (IN) COS queue to configure, -1 for all COS queues
 *     buffer  - (IN) XPE ID
 *     discard - (IN) discard settings
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_gport_discard_set(int unit, bcm_gport_t gport,
                              bcm_cos_queue_t cosq,
                              bcm_cosq_buffer_id_t buffer,
                              bcm_cosq_gport_discard_t *discard)
{
    int numq, i;
    uint32 min_thresh, max_thresh, flags = 0;
    int cell_size, cell_field_max;
    bcm_port_t local_port = -1;

    /* Time Domain */
    /* the reasonable range of refresh_time  is from 1 to 64 */
    if (discard == NULL ||
        discard->gain < 0 || discard->gain > 15 ||
        discard->drop_probability < 0 || discard->drop_probability > 100 ||
        discard->refresh_time <= 0 || discard->refresh_time > 64) {
        return BCM_E_PARAM;
    }

    if (discard == NULL ||
        discard->gain < 0 || discard->gain > 15 ||
        discard->drop_probability < 0 || discard->drop_probability > 100 ||
        (discard->min_thresh > discard->max_thresh) ||
        (discard->min_thresh < 0)) {
        return BCM_E_PARAM;
    }

    cell_size = _TH_MMU_BYTES_PER_CELL;
    cell_field_max = 0xffff;

    min_thresh = discard->min_thresh;
    max_thresh = discard->max_thresh;
    if (discard->flags & BCM_COSQ_DISCARD_BYTES) {
        /* Convert bytes to cells */
        min_thresh += (cell_size - 1);
        min_thresh /= cell_size;

        max_thresh += (cell_size - 1);
        max_thresh /= cell_size;

        if ((min_thresh > cell_field_max) ||
            (max_thresh > cell_field_max)) {
            return BCM_E_PARAM;
        }
    } else if (discard->flags & BCM_COSQ_DISCARD_PACKETS) {
        /* Packet mode not supported */
        return BCM_E_PARAM;
    } else {
        /* Lower layer function will update thresh when these flags are
         * configured, this interface only allow such opration when
         * BCM_COSQ_DISCARD_BYTES is also configured.
         */
        if (discard->flags & (BCM_COSQ_DISCARD_COLOR_ALL |
            BCM_COSQ_DISCARD_TCP |
            BCM_COSQ_DISCARD_NONTCP |
            BCM_COSQ_DISCARD_ECT_MARKED |
            BCM_COSQ_DISCARD_RESPONSIVE_DROP |
            BCM_COSQ_DISCARD_NON_RESPONSIVE_DROP)) {
            return BCM_E_PARAM;
        }
    }

    if (gport != BCM_GPORT_INVALID) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
        if (IS_CPU_PORT(unit,local_port) || IS_LB_PORT(unit,local_port)) {
            return BCM_E_PORT;
        }
        if ((cosq < -1) || (cosq >= SOC_PORT_NUM_COSQ(unit, local_port))) {
            return BCM_E_PARAM;
        }
    }

    numq = 1;

    for (i = 0; i < numq; i++) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_wred_set(unit, gport, cosq + i, buffer,
                                   discard->flags, min_thresh, max_thresh,
                                   discard->drop_probability, discard->gain,
                                   FALSE, flags, discard->refresh_time));
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_gport_discard_get
 * Purpose:
 *     Get port WRED setting
 * Parameters:
 *     unit    - (IN) unit number
 *     port    - (IN) GPORT identifier
 *     cosq    - (IN) COS queue to get, -1 for any queue
 *     buffer  - (IN) XPE ID
 *     discard - (OUT) discard settings
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_gport_discard_get(int unit, bcm_gport_t gport,
                              bcm_cos_queue_t cosq,
                              bcm_cosq_buffer_id_t buffer,
                              bcm_cosq_gport_discard_t *discard)
{
    uint32 min_thresh, max_thresh;
    bcm_port_t local_port = -1;

    if (discard == NULL) {
        return BCM_E_PARAM;
    }

    if (discard->flags & BCM_COSQ_DISCARD_PACKETS) {
        /* Packet mode not supported */
        return BCM_E_PARAM;
    }

    if (gport != BCM_GPORT_INVALID) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
        if (IS_CPU_PORT(unit,local_port) || IS_LB_PORT(unit,local_port)) {
            return BCM_E_PORT;
        }
        if ((cosq < -1) || (cosq >= SOC_PORT_NUM_COSQ(unit, local_port))) {
            return BCM_E_PARAM;
        }
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_wred_get(unit, gport, cosq == -1 ? 0 : cosq, buffer,
                               &discard->flags, &min_thresh, &max_thresh,
                               &discard->drop_probability, &discard->gain,
                               0, &discard->refresh_time));

    /* Convert number of cells to number of bytes */
    discard->min_thresh = min_thresh * _TH_MMU_BYTES_PER_CELL;
    discard->max_thresh = max_thresh * _TH_MMU_BYTES_PER_CELL;

    return BCM_E_NONE;
}

int
bcm_th_pfc_deadlock_ignore_pfc_xoff_gen(int unit, int priority,
                                        bcm_port_t port, uint32 *rval32)
{
    uint32 profile_index, fc_class;
    uint32 rval, uc_cos_bmp;
    uint64 rval64[16], *rval64s[1];
    static const soc_reg_t port_fc_reg = PORT_LLFC_CFGr;

    rval64s[0] = rval64;
    BCM_IF_ERROR_RETURN
        (soc_reg32_get(unit, port_fc_reg, port, 0, &rval));
    profile_index = soc_reg_field_get(unit, port_fc_reg, rval, PROFILE_INDEXf) * 16;

    BCM_IF_ERROR_RETURN
        (soc_profile_reg_get(unit, _bcm_th_prio2cos_profile[unit],
                             profile_index, 16, rval64s));

    for (fc_class = 0; fc_class < _BCM_TH_NUM_PFC_CLASS; ++fc_class ) {

        uc_cos_bmp = soc_reg64_field32_get(unit, PRIO2COS_PROFILEr, rval64[fc_class],
                                           UC_COS_BMPf);
        if (uc_cos_bmp & (1 << priority)) {
            *rval32 = uc_cos_bmp;
            return BCM_E_NONE;
        }
    }

    return BCM_E_NOT_FOUND;
}
/* Flow control APIs */

/*
 * Function:
 *     _bcm_th_cosq_port_fc_get
 * Purpose:
 *     Get the UC|MC gport values associated with
 *     the given PFC/SAFC class.
 * Parameters:
 *     unit             - (IN) unit number
 *     port             - (IN) port number
 *     fc_class       - (IN) PFC/SAFC internal priority class
 *     gport_list       - (OUT) UC|MC gport list
 *     gport_count      - (OUT) gport count
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_th_cosq_port_fc_get(int unit, bcm_gport_t local_port,
                         int fc_class, bcm_gport_t *gport_list,
                         int *gport_count)
{
    int hw_cosq, count = 0;
    uint32 profile_index;
    uint32 rval, uc_cos_bmp, mc_cos_bmp;
    uint64 rval64[16], *rval64s[1];
    static const soc_reg_t port_fc_reg = PORT_LLFC_CFGr;

    if ((gport_list == NULL) || (gport_count == NULL)) {
        return BCM_E_PARAM;
    }

    rval64s[0] = rval64;
    BCM_IF_ERROR_RETURN
        (soc_reg32_get(unit, port_fc_reg, local_port, 0, &rval));
    profile_index = soc_reg_field_get(unit, port_fc_reg, rval, PROFILE_INDEXf) * 16;

    BCM_IF_ERROR_RETURN
        (soc_profile_reg_get(unit, _bcm_th_prio2cos_profile[unit],
                             profile_index, 16, rval64s));

    uc_cos_bmp = soc_reg64_field32_get(unit, PRIO2COS_PROFILEr, rval64[fc_class],
                                       UC_COS_BMPf);
    mc_cos_bmp = soc_reg64_field32_get(unit, PRIO2COS_PROFILEr, rval64[fc_class],
                                       MC_COS_BMPf);

    for (hw_cosq = 0; hw_cosq < _BCM_TH_NUM_UCAST_QUEUE_PER_PORT; hw_cosq++) {
        if (!(uc_cos_bmp & (1 << hw_cosq))) {
            continue;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_port_cos_resolve(unit, local_port, hw_cosq,
                                           _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                           &gport_list[count++]));
    }

    for (hw_cosq = 0; hw_cosq < _BCM_TH_NUM_MCAST_QUEUE_PER_PORT; hw_cosq++) {
        if (!(mc_cos_bmp & (1 << hw_cosq))) {
            continue;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_port_cos_resolve(unit, local_port, hw_cosq,
                                           _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                           &gport_list[count++]));
    }

    if (count == 0) {
        return BCM_E_NOT_FOUND;
    }

    *gport_count = count;

    return BCM_E_NONE;
}

/*
 * Function:
 *    _bcm_th_cosq_port_fc_profile_set
 * Purpose:
 *     Set PFC/SAFC Prio 2 Cos Map and set the profile index for the Port
 * Parameters:
 *     unit         - (IN) unit number
 *     port         - (IN) GPORT identifier
 *     fc_class   - (IN) Input Class
 *     uc_cos_bmp   - (IN) UC Cos bitmap array
 *     mc_cos_bmp   - (IN) MC Cos bitmap array
 *     num_class   - (IN) Number of input class
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_th_cosq_port_fc_profile_set(int unit, bcm_port_t local_port,
                int *fc_class, uint32 *uc_cos_bmp, uint32 *mc_cos_bmp,
                int num_class)
{
    uint32 profile_index, old_profile_index;
    uint32 rval;
    uint64 rval64[16], *rval64s[1];
    static const soc_reg_t port_fc_reg = PORT_LLFC_CFGr;
    int entry, cur_class;
    uint32 cur_uc_bmp, cur_mc_bmp;

    if ((fc_class == NULL) ||
        (uc_cos_bmp == NULL) ||
        (mc_cos_bmp == NULL)) {
        return BCM_E_PARAM;
    }

    rval64s[0] = rval64;
    BCM_IF_ERROR_RETURN
        (soc_reg32_get(unit, port_fc_reg, local_port, 0, &rval));

    old_profile_index = soc_reg_field_get(unit, port_fc_reg, rval, PROFILE_INDEXf) * 16;

    BCM_IF_ERROR_RETURN
        (soc_profile_reg_get(unit, _bcm_th_prio2cos_profile[unit],
                             old_profile_index, 16, rval64s));

    for (entry = 0; entry < num_class; entry++) {
        cur_class = fc_class[entry];
        cur_uc_bmp = uc_cos_bmp[entry];
        cur_mc_bmp = mc_cos_bmp[entry];
        soc_reg64_field32_set(unit, PRIO2COS_PROFILEr, &rval64[cur_class],
                              UC_COS_BMPf, cur_uc_bmp);
        soc_reg64_field32_set(unit, PRIO2COS_PROFILEr, &rval64[cur_class],
                              MC_COS_BMPf, cur_mc_bmp);

    }

    BCM_IF_ERROR_RETURN
        (soc_profile_reg_add(unit, _bcm_th_prio2cos_profile[unit], rval64s,
                             16, &profile_index));

    soc_reg_field_set(unit, port_fc_reg, &rval, PROFILE_INDEXf,
                      profile_index / 16);

    BCM_IF_ERROR_RETURN
        (soc_profile_reg_delete(unit, _bcm_th_prio2cos_profile[unit],
                                old_profile_index));

    BCM_IF_ERROR_RETURN
        (soc_reg32_set(unit, port_fc_reg, local_port, 0, rval));

    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_th_fc_status_map_gport
 * Purpose:
 *      Set mapping in PFC/SAFC for gport, and update
 *      uc_cos_bmp/mc_cos_bmp with gport cosq.
 * Parameters:
 *      unit        - (IN) unit number
 *      port        - (IN) port number
 *      gport       - (IN) gport to be mapped
 *      uc_cos_map     - (OUT) unicast hw_cosq of gport
 *      mc_cos_map     - (OUT) multicast hw_cosq of gport
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
_bcm_th_fc_status_map_gport(int unit, bcm_port_t port,
                bcm_gport_t gport, uint32 *uc_cos_bmp, uint32 *mc_cos_bmp)
{
    _bcm_th_cosq_node_t *node = NULL;
    bcm_port_t local_port = -1;
    bcm_cos_queue_t cosq = -1;

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_node_get(unit, gport, NULL, &local_port, NULL,
                               &node));

    if (node == NULL) {
        return BCM_E_PARAM;
    }

    if (local_port < 0) {
        return BCM_E_PARAM;
    }

    if (local_port != port) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
        cosq = node->hw_index %
                   _BCM_TH_NUM_UCAST_QUEUE_PER_PORT;
        *uc_cos_bmp |= 1 << cosq;
    } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        cosq = node->hw_index %
                   _BCM_TH_NUM_MCAST_QUEUE_PER_PORT;
        *mc_cos_bmp |= 1 << cosq;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_port_pfc_op
 * Purpose:
 *     Set PFC Prio 2 Cos Map and set the profile index for the Port
 * Parameters:
 *     unit     - (IN) unit number
 *     port     - (IN) GPORT identifier
 *     sctype   - (IN) Input Class
 *     op       - (IN) Operation to be performed (ADD|SET|CLEAR)
 *     gport    - (IN) Gport/Cos array
 *     gport_ct - (IN) Count of values in the array
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_port_pfc_op(int unit, bcm_port_t port,
                        bcm_switch_control_t sctype, _bcm_cosq_op_t op,
                        bcm_gport_t *gport, int gport_count)
{
    bcm_port_t local_port;
    int type = -1, class = -1, index, hw_cosq, hw_cosq1;
    uint32 profile_index, old_profile_index;
    uint32 rval, fval, cos_bmp;
    uint64 rval64[16], *rval64s[1];
    _bcm_th_cosq_node_t *node;
    static const soc_reg_t port_fc_reg = PORT_LLFC_CFGr;
    soc_field_t field = INVALIDf;
    soc_info_t *si = &SOC_INFO(unit);
    int rv = SOC_E_NONE;

    if (gport_count < 0) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, port, &local_port));

    BCM_IF_ERROR_RETURN(_bcm_th_cosq_pfc_class_resolve(sctype, &type, &class));

    cos_bmp = 0;

    if (type == _BCM_TH_COSQ_TYPE_MCAST) {
        field = MC_COS_BMPf;
    } else {
        field = UC_COS_BMPf;
    }

    for (index = 0; index < gport_count; index++) {
        hw_cosq = hw_cosq1 = -1;
        if (BCM_GPORT_IS_SET(gport[index]) &&
            ((BCM_GPORT_IS_SCHEDULER(gport[index])) ||
              BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport[index]) ||
              BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport[index]))) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_node_get(unit, gport[index], NULL,
                                       NULL, NULL, &node));
            if (BCM_GPORT_IS_SCHEDULER(gport[index])) {
                hw_cosq = node->hw_index % _BCM_TH_NUM_SCHEDULER_PER_PORT;
            } else if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport[index])) {
                if (type != _BCM_TH_COSQ_TYPE_UCAST) {
                    return BCM_E_PARAM;
                }
                hw_cosq = node->hw_index % si->port_num_cosq[local_port];
            } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport[index])) {
                if (type != _BCM_TH_COSQ_TYPE_MCAST) {
                    return BCM_E_PARAM;
                }
                hw_cosq = node->hw_index % si->port_num_cosq[local_port];
            }
        } else {
            /* Find hardware index for given local_port, cosq */
            hw_cosq = gport[index] % 10;
        }
        cos_bmp |= 1 << hw_cosq;
    }

    rval64s[0] = rval64;
    BCM_IF_ERROR_RETURN(soc_reg32_get(unit, port_fc_reg, local_port, 0, &rval));

    if (op == _BCM_COSQ_OP_CLEAR || cos_bmp != 0) {
        old_profile_index = soc_reg_field_get(unit, port_fc_reg, rval, PROFILE_INDEXf) * 16;

        BCM_IF_ERROR_RETURN
            (soc_profile_reg_get(unit, _bcm_th_prio2cos_profile[unit],
                                 old_profile_index, 16, rval64s));
        if (op == _BCM_COSQ_OP_SET) {
            soc_reg64_field32_set(unit, PRIO2COS_PROFILEr, &rval64[class],
                                  field, cos_bmp);
        } else if (op == _BCM_COSQ_OP_CLEAR || cos_bmp != 0) {
            fval = soc_reg64_field32_get(unit, PRIO2COS_PROFILEr, rval64[class],
                                         field);
            if (op == _BCM_COSQ_OP_ADD) {
                fval |= cos_bmp;
            } else { /* _BCM_COSQ_OP_DELETE */
                fval = 0;
            }
            soc_reg64_field32_set(unit, PRIO2COS_PROFILEr, &rval64[class],
                                  field, fval);
        }
        BCM_IF_ERROR_RETURN
            (soc_profile_reg_delete(unit, _bcm_th_prio2cos_profile[unit],
                                    old_profile_index));
        rv = soc_profile_reg_add(unit, _bcm_th_prio2cos_profile[unit], rval64s,
                                 16, &profile_index);
        if (rv != BCM_E_NONE) {
            SOC_IF_ERROR_RETURN
                (soc_profile_reg_reference(unit, _bcm_th_prio2cos_profile[unit],
                                           old_profile_index, 16));
            return rv;
        }
        soc_reg_field_set(unit, port_fc_reg, &rval, PROFILE_INDEXf,
                          profile_index / 16);
    }
    BCM_IF_ERROR_RETURN(soc_reg32_set(unit, port_fc_reg, local_port, 0, rval));

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_port_pfc_get
 * Purpose:
 *     Get PFC Prio2Cos Map for a given Traffic Class
 * Parameters:
 *     unit     - (IN) unit number
 *     port     - (IN) GPORT identifier
 *     sctype   - (IN) Input Traffic Class
 *     gport    - (OUT) Gport/Cos array
 *     gport_ct - (IN) Input Count of gport required
 *     actual_gport_ct - (OUT) Actual Count of gport/cos in the array
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_port_pfc_get(int unit, bcm_port_t port,
                         bcm_switch_control_t sctype,
                         bcm_gport_t *gport, int gport_count,
                         int *actual_gport_count)
{
    bcm_port_t local_port;
    int type = 0, class = -1, hw_cosq, count = 0;
    uint32 profile_index;
    uint32 rval, cos_bmp;
    uint64 rval64[16], *rval64s[1];
    static const soc_reg_t port_fc_reg = PORT_LLFC_CFGr;
    soc_field_t field = INVALIDf;

    if (gport == NULL || gport_count <= 0 || actual_gport_count == NULL) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, port, &local_port));

    BCM_IF_ERROR_RETURN(_bcm_th_cosq_pfc_class_resolve(sctype, &type, &class));

    rval64s[0] = rval64;
    BCM_IF_ERROR_RETURN(soc_reg32_get(unit, port_fc_reg, local_port, 0, &rval));
    profile_index = soc_reg_field_get(unit, port_fc_reg, rval, PROFILE_INDEXf) * 16;

    if (type == _BCM_TH_COSQ_TYPE_MCAST) {
        field = MC_COS_BMPf;
    } else {
        field = UC_COS_BMPf;
    }

    BCM_IF_ERROR_RETURN
        (soc_profile_reg_get(unit, _bcm_th_prio2cos_profile[unit],
                             profile_index, 16, rval64s));

    cos_bmp = soc_reg64_field32_get(unit, PRIO2COS_PROFILEr, rval64[class],
                                    field);
    for (hw_cosq = 0; hw_cosq < 10; hw_cosq++) {
        if (!(cos_bmp & (1 << hw_cosq))) {
            continue;
        }
        gport[count++] = hw_cosq;

        if (count == gport_count) {
            break;
        }
    }
    if (count == 0) {
        return BCM_E_NOT_FOUND;
    }
    *actual_gport_count = count;

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_pfc_class_mapping_set
 * Purpose:
 *     Set PFC mapping for port.
 *     PFC class is mapped to cosq or scheduler node gports.
 * Parameters:
 *     unit             - (IN) unit number
 *     port             - (IN) port number
 *     array_count      - (IN) count of mapping
 *     mapping_array    - (IN) mappings
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_pfc_class_mapping_set(int unit, bcm_gport_t port,
                                  int array_count,
                                  bcm_cosq_pfc_class_mapping_t *mapping_array)
{
    int index, count;
    int cur_class = 0;
    int pfc_class[_BCM_TH_NUM_PFC_CLASS];
    bcm_gport_t cur_gport = 0;
    uint32 uc_cos_bmp[_BCM_TH_NUM_PFC_CLASS] = {0};
    uint32 mc_cos_bmp[_BCM_TH_NUM_PFC_CLASS] = {0};
    bcm_port_t local_port = -1;

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, port, &local_port));

    if (local_port < 0) {
        return BCM_E_PORT;
    }

    if (IS_CPU_PORT(unit, local_port)) {
        return BCM_E_PORT;
    }

    if ((array_count < 0) || (array_count > _BCM_TH_NUM_PFC_CLASS)) {
        return BCM_E_PARAM;
    }

    if (mapping_array == NULL) {
        return BCM_E_PARAM;
    }

    for (count = 0; count < array_count; count++) {
        cur_class = mapping_array[count].class_id;
        if ((cur_class < 0) || (cur_class >= _BCM_TH_NUM_PFC_CLASS)) {
            return BCM_E_PARAM;
        } else {
            uc_cos_bmp[cur_class] = 0;
            mc_cos_bmp[cur_class] = 0;
        }

        for (index = 0; index < BCM_COSQ_PFC_GPORT_COUNT; index++) {
            cur_gport = mapping_array[count].gport_list[index];
            if (cur_gport == BCM_GPORT_INVALID) {
                break;
            }

            if ((BCM_GPORT_IS_UCAST_QUEUE_GROUP(cur_gport)) ||
                (BCM_GPORT_IS_MCAST_QUEUE_GROUP(cur_gport))) {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_fc_status_map_gport(unit, local_port, cur_gport,
                                                   &uc_cos_bmp[cur_class],
                                                   &mc_cos_bmp[cur_class]));
            } else {
                return BCM_E_PARAM;
            }
        }
    }

    for (index = 0; index < _BCM_TH_NUM_PFC_CLASS; index++) {
        pfc_class[index] = index;
    }
    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_port_fc_profile_set(unit, local_port, pfc_class,
                                          uc_cos_bmp, mc_cos_bmp,
                                          _BCM_TH_NUM_PFC_CLASS));

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_pfc_class_mapping_get
 * Purpose:
 *     Get PFC mapping for port.
 *     Retrieves cosq or scheduler node gports
 *     associated to PFC classes.
 * Parameters:
 *     unit             - (IN) unit number
 *     port             - (IN) port number
 *     array_max        - (IN) max retrieve count
 *     mapping_array    - (OUT) mappings
 *     array_count      - (OUT) retrieved mapping count
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_pfc_class_mapping_get(int unit, bcm_gport_t port,
                                  int array_max,
                                  bcm_cosq_pfc_class_mapping_t *mapping_array,
                                  int *array_count)
{
    int rv = 0;
    int cur_class = 0;
    int actual_gport_count, class_count = 0;
    bcm_port_t local_port = -1;

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, port, &local_port));

    if (local_port < 0) {
        return BCM_E_PORT;
    }

    if (IS_CPU_PORT(unit, local_port)) {
        return BCM_E_PORT;
    }

    if (((mapping_array == NULL) && (array_max > 0)) ||
        ((mapping_array != NULL) && (array_max <= 0)) ||
        (array_count == NULL)) {
        return BCM_E_PARAM;
    }

    if (array_max <= 0) {
        *array_count = _BCM_TH_NUM_PFC_CLASS;
        return BCM_E_NONE;
    }

    for (cur_class = 0; cur_class < _BCM_TH_NUM_PFC_CLASS; cur_class++) {
        actual_gport_count = 0;
        rv = _bcm_th_cosq_port_fc_get(unit, local_port, cur_class,
                                      mapping_array[class_count].gport_list,
                                      &actual_gport_count);

        if (rv == BCM_E_NONE) {
            mapping_array[class_count].class_id = cur_class;
            if (actual_gport_count < BCM_COSQ_PFC_GPORT_COUNT) {
                mapping_array[class_count].gport_list[actual_gport_count] =
                                                        BCM_GPORT_INVALID;
            }
            class_count++;
        } else if (rv != BCM_E_NOT_FOUND) {
            return rv;
        }

        if (class_count == array_max) {
            break;
        }
    }

    *array_count = class_count;
    if (class_count == 0) {
        return BCM_E_NOT_FOUND;
    }

    return BCM_E_NONE;
}


/*
 * Function:
 *      bcm_th_port_priority_group_mapping_set
 * Purpose:
 *      Assign a Priority Group for the input priority.
 * Parameters:
 *      unit       - (IN) device id.
 *      gport     - (IN) generic port.
 *      prio       - (IN) input priority.
 *      priority_group  - (IN) PG ID that the input priority mapped to.
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_th_port_priority_group_mapping_set(int unit, bcm_gport_t gport,
                                        int prio, int priority_group)
{
    bcm_port_t port;
    uint32 value;
    int field_count;
    soc_field_t fields_0[]={PRI0_GRPf, PRI1_GRPf, PRI2_GRPf, PRI3_GRPf,
                            PRI4_GRPf, PRI5_GRPf, PRI6_GRPf, PRI7_GRPf};
    soc_field_t fields_1[]={PRI8_GRPf, PRI9_GRPf, PRI10_GRPf, PRI11_GRPf,
                            PRI12_GRPf, PRI13_GRPf, PRI14_GRPf, PRI15_GRPf};

    if (!soc_feature(unit, soc_feature_priority_flow_control)) {
        return BCM_E_UNAVAIL;
    }
    /* PFC : 0-7, SAFC : 0-15. Taking the larger range for comparison */
    if ((prio < TH_SAFC_INPUT_PRIORITY_MIN) || 
        (prio > TH_SAFC_INPUT_PRIORITY_MAX)) {
        return BCM_E_PARAM;
    }

    if ((priority_group < TH_PRIOROTY_GROUP_ID_MIN) || 
        (priority_group > TH_PRIOROTY_GROUP_ID_MAX)) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN(_bcm_th_cosq_localport_resolve(unit, gport, &port));
    if (!SOC_PORT_VALID(unit, port)) {
        return BCM_E_PORT;
    }
    field_count = COUNTOF(fields_0);
    if (prio < field_count) {
        /* input priority 0~7 */
        BCM_IF_ERROR_RETURN(READ_THDI_PORT_PRI_GRP0r(unit, port, &value));
        soc_reg_field_set(unit, THDI_PORT_PRI_GRP0r, &value, 
                          fields_0[prio], priority_group);
        BCM_IF_ERROR_RETURN(WRITE_THDI_PORT_PRI_GRP0r(unit, port, value));
    } else {
        /* input priority 8~15 */
        BCM_IF_ERROR_RETURN(READ_THDI_PORT_PRI_GRP1r(unit, port, &value));
        soc_reg_field_set(unit, THDI_PORT_PRI_GRP1r, &value, 
                          fields_1[prio - field_count], priority_group);
        BCM_IF_ERROR_RETURN(WRITE_THDI_PORT_PRI_GRP1r(unit, port, value));
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_th_port_priority_group_mapping_get
 * Purpose:
 *      Get the assigned Priority Group ID of the input priority.
 * Parameters:
 *      unit       - (IN) device id.
 *      gport     - (IN) generic port.
 *      prio       - (IN) input priority.
 *      priority_group  - (OUT) PG ID that the input priority mapped to.
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_th_port_priority_group_mapping_get(int unit, bcm_gport_t gport,
                                        int prio, int *priority_group)
{
    bcm_port_t port;
    uint32 value;
    int field_count;
    soc_field_t fields_0[]={PRI0_GRPf, PRI1_GRPf, PRI2_GRPf, PRI3_GRPf,
                            PRI4_GRPf, PRI5_GRPf, PRI6_GRPf, PRI7_GRPf};
    soc_field_t fields_1[]={PRI8_GRPf, PRI9_GRPf, PRI10_GRPf, PRI11_GRPf,
                            PRI12_GRPf, PRI13_GRPf, PRI14_GRPf, PRI15_GRPf};

    if (!soc_feature(unit, soc_feature_priority_flow_control)) {
        return BCM_E_UNAVAIL;
    }
    if (priority_group == NULL) {
        return BCM_E_PARAM;
    }
    /* PFC : 0-7, SAFC : 0-15. Taking the larger range for comparison */
    if ((prio < TH_SAFC_INPUT_PRIORITY_MIN) || 
        (prio > TH_SAFC_INPUT_PRIORITY_MAX)) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN(_bcm_th_cosq_localport_resolve(unit, gport, &port));
    if (!SOC_PORT_VALID(unit, port)) {
        return BCM_E_PORT;
    }
    field_count = COUNTOF(fields_0);
    if (prio < field_count) {
        /* input priority 0~7 */
        BCM_IF_ERROR_RETURN(READ_THDI_PORT_PRI_GRP0r(unit, port, &value));
        *priority_group = soc_reg_field_get(unit, THDI_PORT_PRI_GRP0r,
                                            value, fields_0[prio]);
    } else {
        /* input priority 8~15 */
        BCM_IF_ERROR_RETURN(READ_THDI_PORT_PRI_GRP1r(unit, port, &value));
        *priority_group = soc_reg_field_get(unit, THDI_PORT_PRI_GRP1r,
                                            value, 
                                            fields_1[prio - field_count]);
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_th_port_priority_group_config_set
 * Purpose:
 *      Set the port priority group configuration.
 * Parameters:
 *      unit       - (IN) device id.
 *      gport     - (IN) generic port.
 *      priority_group - (IN) priority group id.
 *      prigrp_config  - (IN) structure describes port PG configuration.
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_th_port_priority_group_config_set(int unit, bcm_gport_t gport,
    int priority_group, bcm_port_priority_group_config_t *prigrp_config)
{
    bcm_port_t port;
    uint32 rval, pri_bmp;

    if (!soc_feature(unit, soc_feature_priority_flow_control)) {
        return BCM_E_UNAVAIL;
    }
    if ((priority_group < TH_PRIOROTY_GROUP_ID_MIN) || 
        (priority_group > TH_PRIOROTY_GROUP_ID_MAX)) {
        return BCM_E_PARAM;
    }
    if (prigrp_config == NULL) {
        return BCM_E_PARAM;
    }
    if (prigrp_config->shared_pool_xoff_enable ||
        prigrp_config->shared_pool_discard_enable ||
        prigrp_config->priority_enable_vector_mask) {
        return BCM_E_UNAVAIL;
    }
    BCM_IF_ERROR_RETURN(_bcm_th_cosq_localport_resolve(unit, gport, &port));
    if (!SOC_PORT_VALID(unit, port)) {
        return BCM_E_PORT;
    }

    SOC_IF_ERROR_RETURN(READ_THDI_INPUT_PORT_XON_ENABLESr(unit, port, &rval));
    pri_bmp = soc_reg_field_get(unit, THDI_INPUT_PORT_XON_ENABLESr, rval,
                                PORT_PRI_XON_ENABLEf);

    if (prigrp_config->pfc_transmit_enable) {
        pri_bmp |= (1 << priority_group);
    } else {
        pri_bmp &= ~(1 << priority_group);
    }
    /* To cover higher 8 PFC classes that have no corresponding PG. */
    if ((pri_bmp & 0xff) == 0) {
        pri_bmp = 0;
    }

    soc_reg_field_set(unit, THDI_INPUT_PORT_XON_ENABLESr, &rval, 
                                PORT_PRI_XON_ENABLEf, pri_bmp);
    BCM_IF_ERROR_RETURN(WRITE_THDI_INPUT_PORT_XON_ENABLESr(unit, port, rval));

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_th_port_priority_group_config_get
 * Purpose:
 *      Get the port priority group configuration.
 * Parameters:
 *      unit       - (IN) device id.
 *      gport     - (IN) generic port.
 *      priority_group - (IN) priority group id.
 *      prigrp_config  - (OUT) structure describes port PG configuration.
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_th_port_priority_group_config_get(int unit, bcm_gport_t gport,
    int priority_group, bcm_port_priority_group_config_t *prigrp_config)
{
    bcm_port_t port;
    uint32 rval, pri_bmp;

    if (!soc_feature(unit, soc_feature_priority_flow_control)) {
        return BCM_E_UNAVAIL;
    }
    if ((priority_group < TH_PRIOROTY_GROUP_ID_MIN) || 
        (priority_group > TH_PRIOROTY_GROUP_ID_MAX)) {
        return BCM_E_PARAM;
    }
    if (prigrp_config == NULL) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN(_bcm_th_cosq_localport_resolve(unit, gport, &port));
    if (!SOC_PORT_VALID(unit, port)) {
        return BCM_E_PORT;
    }

    SOC_IF_ERROR_RETURN(READ_THDI_INPUT_PORT_XON_ENABLESr(unit, port, &rval));
    pri_bmp = soc_reg_field_get(unit, THDI_INPUT_PORT_XON_ENABLESr, rval,
                                PORT_PRI_XON_ENABLEf);
    if (pri_bmp & (1U << priority_group)) {
        prigrp_config->pfc_transmit_enable = 1;
    } else {
        prigrp_config->pfc_transmit_enable = 0;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_gport_add
 * Purpose:
 *     Create a cosq gport structure
 * Parameters:
 *     unit  - (IN) unit number
 *     port  - (IN) port number
 *     numq  - (IN) number of COS queues
 *     flags - (IN) flags (BCM_COSQ_GPORT_XXX)
 *     gport - (OUT) GPORT identifier
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_gport_add(int unit, bcm_gport_t port, int numq, uint32 flags,
                      bcm_gport_t *gport)
{
    _bcm_th_cosq_port_info_t *port_info = NULL;
    _bcm_th_cosq_cpu_port_info_t *cpu_port_info = NULL;
    _bcm_th_cosq_node_t *node;
    uint32 sched_encap;
    bcm_port_t local_port;
    int id;

    LOG_INFO(BSL_LS_BCM_COSQ,
             (BSL_META_U(unit,
                         "bcm_th_cosq_gport_add: unit=%d port=0x%x "
                         "numq=%d flags=0x%x\n"),
              unit, port, numq, flags));

    if (gport == NULL) {
        return BCM_E_PARAM;
    }

    if (_bcm_th_cosq_port_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    if (_bcm_th_cosq_cpu_port_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, port, &local_port));

    if (local_port < 0 || IS_LB_PORT(unit, local_port)) {
        return BCM_E_PORT;
    }

    if (IS_CPU_PORT(unit, local_port)) {
        cpu_port_info = &_bcm_th_cosq_cpu_port_info[unit][local_port];
    } else  {
        port_info = &_bcm_th_cosq_port_info[unit][local_port];
    }
    switch (flags) {
        case BCM_COSQ_GPORT_UCAST_QUEUE_GROUP:
            if (IS_CPU_PORT(unit, local_port)) {
                return BCM_E_PARAM;
            }
            if (numq != 1) {
                return BCM_E_PARAM;
            }
            for (id = 0; id < _BCM_TH_NUM_UCAST_QUEUE_PER_PORT; id++) {
                if ((port_info->ucast[id].numq == 0) ||
                    (port_info->ucast[id].in_use == 0)) {
                    break;
                }
            }
            if (id == _BCM_TH_NUM_UCAST_QUEUE_PER_PORT) {
                return BCM_E_RESOURCE;
            }
            BCM_GPORT_UCAST_QUEUE_GROUP_SYSQID_SET(*gport, local_port, id);
            node = &port_info->ucast[id];
            node->level = SOC_TH_NODE_LVL_L1;
            break;
        case BCM_COSQ_GPORT_MCAST_QUEUE_GROUP:
            if (numq != 1) {
                return BCM_E_PARAM;
            }
            if (IS_CPU_PORT(unit, local_port)) {
                for (id = 0; id < _BCM_TH_NUM_CPU_MCAST_QUEUE; id++) {
                    if ((cpu_port_info->mcast[id].numq == 0) ||
                        (cpu_port_info->mcast[id].in_use == 0)) {
                        break;
                    }
                }
                if (id == _BCM_TH_NUM_CPU_MCAST_QUEUE) {
                    return BCM_E_RESOURCE;
                }
            } else {
                for (id = 0; id < _BCM_TH_NUM_MCAST_QUEUE_PER_PORT; id++) {
                    if ((port_info->mcast[id].numq == 0) ||
                        (port_info->mcast[id].in_use == 0)) {
                        break;
                    }
                }
                if (id == _BCM_TH_NUM_MCAST_QUEUE_PER_PORT) {
                    return BCM_E_RESOURCE;
                }
            }
            BCM_GPORT_MCAST_QUEUE_GROUP_SYSQID_SET(*gport, local_port, id);
            if (IS_CPU_PORT(unit, local_port)) {
                node = &cpu_port_info->mcast[id];
            } else {
                node = &port_info->mcast[id];
            }
            node->level = SOC_TH_NODE_LVL_L1;
            break;
        case BCM_COSQ_GPORT_SCHEDULER:
        case 0:
            if (numq <= 0) {
                return BCM_E_PARAM;
            }
            for (id = 0; id < _BCM_TH_NUM_SCHEDULER_PER_PORT; id++) {
                if (IS_CPU_PORT(unit, local_port)) {
                    if (cpu_port_info->sched[id].numq == 0) {
                        break;
                    }
                } else {
                    if (port_info->sched[id].numq == 0) {
                        break;
                    }
                }
            }
            if (id == _BCM_TH_NUM_SCHEDULER_PER_PORT) {
                return BCM_E_RESOURCE;
            }

            sched_encap = (id << 16) | local_port;
            BCM_GPORT_SCHEDULER_SET(*gport, sched_encap);
            if (IS_CPU_PORT(unit, local_port)) {
                node = &cpu_port_info->sched[id];
            } else {
                node = &port_info->sched[id];
            }
            node->level = SOC_TH_NODE_LVL_L0;
            break;
        default:
            return BCM_E_UNAVAIL;
    }

    node->gport = *gport;
    node->numq = numq;
    node->in_use = 1;

    LOG_INFO(BSL_LS_BCM_COSQ,
             (BSL_META_U(unit,
                         "                       gport=0x%x\n"),
              *gport));

    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_rx_queue_channel_set(int unit,
                                  bcm_cos_queue_t parent_cos,
                                  bcm_cos_queue_t queue_id,
                                  int enable)
{
#ifdef BCM_CMICM_SUPPORT
    int pci_cmc = SOC_PCI_CMC(unit);
    int chan_id = -1;
    int start_chan_id = 0;
    uint32 chan_off;
    uint32 reg_addr, reg_val;
    uint32 ix;

    if (parent_cos == 0) {
        chan_id = 1;
    } else if (parent_cos == 1) {
        chan_id = 2;
    } else if (parent_cos == 2) {
        chan_id = 3;
    } else if (parent_cos == 3) {
        chan_id = 0;
    } else {
        return BCM_E_PARAM;
    }

    if (!SHR_BITGET(CPU_ARM_QUEUE_BITMAP(unit, pci_cmc), queue_id)) {
        return BCM_E_PARAM;
    }

    for (ix = start_chan_id; ix < (start_chan_id + BCM_RX_CHANNELS); ix++) {
        chan_off = ix % BCM_RX_CHANNELS;
        reg_addr = (queue_id < 32) ?
                CMIC_CMCx_CHy_COS_CTRL_RX_0_OFFSET(pci_cmc, chan_off) :
                CMIC_CMCx_CHy_COS_CTRL_RX_1_OFFSET(pci_cmc, chan_off);
        reg_val = soc_pci_read(unit, reg_addr);

        if (enable == 1) {
            if (ix == (uint32)chan_id) {
                reg_val |= ((uint32)1 << (queue_id % 32));
            } else {
                /* Clear all other channels */
                reg_val &= ~((uint32)1 << (queue_id % 32));
            }
        } else {
            if (ix == (uint32)chan_id) {
                reg_val &= ~((uint32)1 << (queue_id % 32));
            }
        }
        /* Incoporate the reserved queues (if any on this device)
         * into the CMIC programming,  */
        reg_val |=
            CPU_ARM_RSVD_QUEUE_BITMAP(unit, pci_cmc)[queue_id / 32];
        soc_pci_write(unit, reg_addr, reg_val);
    }
#endif /* CMICM Support */
    return BCM_E_NONE;
}

/*
 * This function does two things:
 * 1. Tests whether the corresponding COS_BMP bit is set or not.
 * 2. Tests whether the queue corresponding to given cos is
 *     mapped to correct L0 or not.
 *
 * L0.0 is associated with CMC0 CH1(Rx) (pci)
 * L0.1 is associated with CMC0 CH2(Rx) (pci)
 * L0.2 is associated with CMC0 CH3(Rx) (pci)
 * L0.3 is associated with CMC0 CH0(Rx) (pci)
 * L0.7 is associated with CMC1 CH1(Rx) (uC0)
 * L0.8 is associated with CMC2 CH1(Rx) (uC1)
 * L0.9 is reserved
 */
int
bcm_th_rx_queue_channel_set_test(int unit,
                                 bcm_cos_queue_t queue_id,
                                 bcm_rx_chan_t chan_id)
{
#ifdef BCM_CMICM_SUPPORT
    int pci_cmc = SOC_PCI_CMC(unit);
    uint32 reg_addr, reg_val;
    uint32 bit;
    _bcm_th_cosq_cpu_port_info_t *cpu_port_info = NULL;
    bcm_gport_t cpu_l0_gport = -1;
    bcm_gport_t parent_gport = -1;

    if ((chan_id < 0) || (chan_id >= BCM_RX_CHANNELS)) {
        /* Return error for non- pci_cmc channels */
        return BCM_E_PARAM;
    }

    if (_bcm_th_cosq_cpu_port_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    cpu_port_info = _bcm_th_cosq_cpu_port_info[unit];

    if (chan_id == 0) {
        /* l0.3 gport */
        cpu_l0_gport = cpu_port_info->sched[3].gport;
    } else if (chan_id == 1) {
        /* l0.0 gport */
        cpu_l0_gport = cpu_port_info->sched[0].gport;
    } else if (chan_id == 2) {
        /* l0.1 gport */
        cpu_l0_gport = cpu_port_info->sched[1].gport;
    } else if (chan_id == 3) {
        /* l0.2 gport */
        cpu_l0_gport = cpu_port_info->sched[2].gport;
    }

    if (queue_id < 0) { /* All COS Queues */
        for (queue_id = 0; queue_id < NUM_CPU_COSQ(unit); queue_id++) {
            reg_addr = (queue_id < 32) ?
                        CMIC_CMCx_CHy_COS_CTRL_RX_0_OFFSET(pci_cmc, chan_id) :
                        CMIC_CMCx_CHy_COS_CTRL_RX_1_OFFSET(pci_cmc, chan_id);
            bit = 1 << (queue_id % 32);
            reg_val = soc_pci_read(unit, reg_addr);
            if (reg_val & bit) {
                /* find the parent gport of this queue
                 if parent gport is not equal to l0 gport
                 for this channel return error */
                parent_gport = cpu_port_info->mcast[queue_id].parent_gport;
                if (parent_gport != cpu_l0_gport) {
                    return BCM_E_PARAM;
                }
            }
        }
    } else {
        reg_addr = (queue_id < 32) ?
                    CMIC_CMCx_CHy_COS_CTRL_RX_0_OFFSET(pci_cmc, chan_id) :
                    CMIC_CMCx_CHy_COS_CTRL_RX_1_OFFSET(pci_cmc, chan_id);
        bit = 1 << (queue_id % 32);
        reg_val = soc_pci_read(unit, reg_addr);
        if (reg_val & bit) {
            /* find the parent gport of this queue
             if parent gport is not equal to l0 gport
             for this channel return error */
            parent_gport = cpu_port_info->mcast[queue_id].parent_gport;
            if (parent_gport != cpu_l0_gport) {
                return BCM_E_PARAM;
            }
        } else {
            return BCM_E_PARAM;
        }
    }
#endif /* CMICM Support */
    return BCM_E_NONE;
}

#ifdef BCM_WARM_BOOT_SUPPORT
STATIC int
_bcm_cosq_rx_queue_channel_reinit(int unit)
{
    _bcm_th_cosq_cpu_port_info_t *cpu_port_info = NULL;
    int idx;
    /*
     * L0.0 is associated with CMC0 CH1(Rx) (pci)
     * L0.1 is associated with CMC0 CH2(Rx) (pci)
     * L0.2 is associated with CMC0 CH3(Rx) (pci)
     * L0.3 is associated with CMC0 CH0(Rx) (pci)
     */
    bcm_gport_t sched_gport[4];
    bcm_cos_queue_t parent_cos;

    cpu_port_info = _bcm_th_cosq_cpu_port_info[unit];
    if (cpu_port_info == NULL) {
        return BCM_E_INIT;
    }

    for (idx = 0; idx < 4; idx++) {
        sched_gport[idx] = cpu_port_info->sched[idx].gport;
    }

    for (idx = 0; idx < _BCM_TH_NUM_CPU_MCAST_QUEUE; idx++) {
        parent_cos = -1;
        if (cpu_port_info->mcast[idx].parent_gport == sched_gport[0]) {
            parent_cos = 0;
        } else if (cpu_port_info->mcast[idx].parent_gport == sched_gport[1]) {
            parent_cos = 1;
        } else if (cpu_port_info->mcast[idx].parent_gport == sched_gport[2]) {
            parent_cos = 2;
        } else if (cpu_port_info->mcast[idx].parent_gport == sched_gport[3]) {
            parent_cos = 3;
        }
        if ((parent_cos >= 0) && (parent_cos < 4)) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_rx_queue_channel_set(unit, parent_cos, idx, 1));
        }
    }
    return BCM_E_NONE;
}
#endif

/* Function to attach any of the 48 L1 MC Queue to one of the 10 L0 nodes
 */
STATIC int
bcm_th_cosq_cpu_gport_attach(int unit, bcm_gport_t input_gport,
                             bcm_gport_t parent_gport, bcm_cos_queue_t cosq)
{
    bcm_port_t parent_port, input_port;
    _bcm_th_cosq_node_t *input_node = NULL, *parent_node = NULL;
    int phy_port, mmu_port;
    int input_cos = 0, parent_cos = 0;
    int cpu_sched_base = 0, cpu_mc_base = 0;
    soc_info_t *si = &SOC_INFO(unit);

    /* Post Default tree created, expected Input Gport is of MC Gport
     * and expected Parent Gport is new L0 Parent
     */
    if (_BCM_TH_MMU_INFO(unit)->gport_tree_created == FALSE) {
        if (((BCM_GPORT_IS_MCAST_QUEUE_GROUP(input_gport)) &&
             (!BCM_GPORT_IS_SCHEDULER(parent_gport))) ||
            (!(BCM_GPORT_IS_MCAST_QUEUE_GROUP(input_gport)) &&
             (!BCM_GPORT_IS_SCHEDULER(input_gport)))) {
            return BCM_E_PARAM;
        }
    } else if ((!BCM_GPORT_IS_SCHEDULER(parent_gport)) ||
        (!BCM_GPORT_IS_MCAST_QUEUE_GROUP(input_gport))) {
        return BCM_E_PARAM;
    }
    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_node_get(unit, input_gport, NULL, &input_port, NULL,
                               &input_node));
    if (BCM_GPORT_IS_SCHEDULER(parent_gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_node_get(unit, parent_gport, NULL, &parent_port, NULL,
                                   &parent_node));
    } else {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, parent_gport, &parent_port));
        parent_node = NULL;
    }

    if (!IS_CPU_PORT(unit, parent_port)) {
        return BCM_E_PARAM;
    }

    /* Return BCM_E_EXISTS when queue to be attached is
       already attached to the given gport */
    if ((_BCM_TH_MMU_INFO(unit)->gport_tree_created == TRUE) &&
        (input_node->parent_gport == parent_gport)) {
        return BCM_E_EXISTS;
    }

    phy_port = si->port_l2p_mapping[input_port];
    mmu_port = si->port_p2m_mapping[phy_port];
    cpu_sched_base = mmu_port * _BCM_TH_NUM_SCHEDULER_PER_PORT;
    input_cos = cosq % _BCM_TH_NUM_CPU_MCAST_QUEUE;
    input_node->parent_gport = parent_port;

    if (BCM_GPORT_IS_SCHEDULER(parent_gport)) {
        cpu_mc_base = si->port_cosq_base[CMIC_PORT(unit)];
        parent_cos = (parent_node->hw_index - cpu_sched_base) %
                     _BCM_TH_NUM_CPU_MCAST_QUEUE;
        SOC_IF_ERROR_RETURN
            (soc_th_cosq_cpu_parent_set(unit, input_cos, SOC_TH_NODE_LVL_L1,
                                        parent_cos));
        if ((parent_cos >= 0) && (parent_cos < 4)) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_rx_queue_channel_set(unit, parent_cos, input_cos, 1));
        }
        input_node->parent_gport = parent_node->gport;
        if (_BCM_TH_MMU_INFO(unit)->gport_tree_created == FALSE) {
            input_node->hw_index = cpu_mc_base + input_cos;
        }
    } else {
        input_node->hw_index = cpu_sched_base + input_cos;
    }

    return BCM_E_NONE;
}

/* Function to detach any of the 48 L1 MC Queue from its parent 10 L0 nodes
 * Post detach, L1 MC queue is attached to Parent L0.9 node
 */
STATIC int
bcm_th_cosq_cpu_gport_detach(int unit, bcm_gport_t input_gport,
                             bcm_gport_t parent_gport, bcm_cos_queue_t cosq)
{
    bcm_port_t parent_port, input_port;
    _bcm_th_cosq_node_t *input_node = NULL, *parent_node = NULL;
    int phy_port, mmu_port;
    int input_cos = 0, parent_cos = 0, config_cos = 0;
    int cpu_mc_base = 0, cpu_sched_base = 0;
    soc_info_t *si = &SOC_INFO(unit);

    /* Post Default tree created, expected Input Gport is of MC Gport
     * and expected Parent Gport is new L0 Parent
     */
    if ((!BCM_GPORT_IS_SCHEDULER(parent_gport)) ||
        (!BCM_GPORT_IS_MCAST_QUEUE_GROUP(input_gport))) {
        return BCM_E_PARAM;
    }
    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_node_get(unit, input_gport, NULL, &input_port, NULL,
                               &input_node));
    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_node_get(unit, parent_gport, NULL, &parent_port, NULL,
                               &parent_node));

    if (!IS_CPU_PORT(unit, parent_port)) {
        return BCM_E_PARAM;
    }

    /* Return BCM_E_PARAM when queue to be detached is
       not attached to the given gport */
    if (input_node->parent_gport != parent_gport) {
        return BCM_E_PARAM;
    }

    phy_port = si->port_l2p_mapping[input_port];
    mmu_port = si->port_p2m_mapping[phy_port];
    cpu_sched_base = mmu_port * _BCM_TH_NUM_SCHEDULER_PER_PORT;
    parent_cos = (parent_node->hw_index - cpu_sched_base) %
                _BCM_TH_NUM_CPU_MCAST_QUEUE;
    cpu_mc_base = si->port_cosq_base[CMIC_PORT(unit)];
    input_cos = (input_node->hw_index - cpu_mc_base) %
                _BCM_TH_NUM_CPU_MCAST_QUEUE;

    /* Reset parent to L0.9 */
    SOC_IF_ERROR_RETURN
        (soc_th_cosq_cpu_parent_get(unit, input_cos, SOC_TH_NODE_LVL_L1,
                                    &config_cos));
    if (config_cos != parent_cos) {
        /* Error when detaching from wrong Parent (L0 level) */
        return BCM_E_PARAM;
    }

    /*
     * L0.0 is associated with CMC0 CH1(Rx)
     * L0.1 is associated with CMC0 CH2(Rx)
     * L0.2 is associated with CMC0 CH3(Rx)
     * L0.3 is associated with CMC0 CH0(Rx)
     */
    if ((parent_cos >= 0) && (parent_cos < 4)) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_rx_queue_channel_set(unit, parent_cos, input_cos, 0));
    }

    /* Reset parent to L0.9 */
    SOC_IF_ERROR_RETURN
        (soc_th_cosq_cpu_parent_set(unit, input_cos, SOC_TH_NODE_LVL_L1,
                                    _BCM_TH_NUM_SCHEDULER_PER_PORT - 1));
    input_node->parent_gport =
        _BCM_TH_MMU_INFO(unit)->_bcm_th_cpu_port_info->
            sched[_BCM_TH_NUM_SCHEDULER_PER_PORT -1].gport;

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_gport_attach
 * Purpose:
 *     Attach sched_port to the specified index (cosq) of input_port
 * Parameters:
 *     unit         - (IN) unit number
 *     input_gport  - (IN) GPORT id, that will attach to parent gport
 *     parent_gport - (IN) Parent Gport to which current gport will be attached
 *     cosq         - (IN) COS queue to attach to (-1 for first available cosq)
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_gport_attach(int unit, bcm_gport_t input_gport,
                         bcm_gport_t parent_gport, bcm_cos_queue_t cosq)
{
    _bcm_th_cosq_node_t *input_node = NULL, *parent_node = NULL;
    bcm_port_t input_port, parent_port;
    int phy_port, mmu_port;
    int input_hw_index = -1, parent_hw_index = -1;
    soc_info_t *si = &SOC_INFO(unit);

    LOG_INFO(BSL_LS_BCM_COSQ,
             (BSL_META_U(unit,
                         "bcm_th_cosq_gport_attach: unit=%d parent_gport=0x%x "
                         "input_gport=0x%x cosq=%d\n"),
              unit, parent_gport, input_gport, cosq));

    if (!(BCM_GPORT_IS_SCHEDULER(input_gport) ||
          BCM_GPORT_IS_UCAST_QUEUE_GROUP(input_gport) ||
          BCM_GPORT_IS_MCAST_QUEUE_GROUP(input_gport))) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_node_get(unit, input_gport, NULL, &input_port, NULL,
                               &input_node));

    if (!input_node) {
        return BCM_E_PARAM;
    }
    if (input_port < 0) {
        return BCM_E_PORT;
    }

    if (IS_CPU_PORT(unit, input_port)) {
        return bcm_th_cosq_cpu_gport_attach(unit, input_gport,
                                                parent_gport, cosq);
    } else if (_BCM_TH_MMU_INFO(unit)->gport_tree_created == TRUE) {
        return BCM_E_PARAM;
    }

    phy_port = si->port_l2p_mapping[input_port];
    mmu_port = si->port_p2m_mapping[phy_port];

    if (BCM_GPORT_IS_SCHEDULER(parent_gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_node_get(unit, parent_gport, NULL, &parent_port, NULL,
                                   &parent_node));
    } else {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, parent_gport, &parent_port));
        parent_node = NULL;
    }

    if (input_port != parent_port) {
        return BCM_E_PARAM;
    }

    /* TH - Scheduler heirarchy is fixed. Hence check for wrong config. */
    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(input_gport)) {
        input_node->hw_index = (mmu_port * _BCM_TH_NUM_UCAST_QUEUE_PER_PORT) +
                               cosq;
        input_hw_index = input_node->hw_index %
                         _BCM_TH_NUM_UCAST_QUEUE_PER_PORT;
    } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(input_gport)) {
        input_node->hw_index = (mmu_port * _BCM_TH_NUM_MCAST_QUEUE_PER_PORT) +
                               cosq;
        input_hw_index = input_node->hw_index %
                         _BCM_TH_NUM_MCAST_QUEUE_PER_PORT;
    } else {
        input_node->hw_index = (mmu_port * _BCM_TH_NUM_SCHEDULER_PER_PORT) +
                               cosq;
        input_hw_index = input_node->hw_index %
                         _BCM_TH_NUM_SCHEDULER_PER_PORT;
    }

    if (parent_node) {
        parent_hw_index = parent_node->hw_index %
                          _BCM_TH_NUM_SCHEDULER_PER_PORT;
        if (input_hw_index != parent_hw_index) {
            return BCM_E_PARAM;
        }
    }

    if (BCM_GPORT_IS_SCHEDULER(input_gport)) {
        BCM_IF_ERROR_RETURN
            (bcm_esw_port_gport_get(unit, input_port,
                                    &(input_node->parent_gport)));
    } else if ((BCM_GPORT_IS_UCAST_QUEUE_GROUP(input_gport)) ||
               (BCM_GPORT_IS_MCAST_QUEUE_GROUP(input_gport))) {
        input_node->parent_gport = parent_gport;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_gport_detach
 * Purpose:
 *     Detach sched_port from the specified index (cosq) of input_port
 * Parameters:
 *     unit        - (IN) unit number
 *     input_port  - (IN) scheduler GPORT identifier
 *     parent_port - (IN) GPORT to detach from
 *     cosq        - (IN) COS queue to detach from
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_gport_detach(int unit, bcm_gport_t input_gport,
                         bcm_gport_t parent_gport, bcm_cos_queue_t cosq)
{
    _bcm_th_cosq_node_t *input_node = NULL, *parent_node = NULL;
    bcm_port_t input_port, parent_port;
    int input_hw_index = -1, parent_hw_index = -1;

    LOG_INFO(BSL_LS_BCM_COSQ,
             (BSL_META_U(unit,
                         "bcm_th_cosq_gport_detach: unit=%d input_gport=0x%x "
                         "parent_gport=0x%x cosq=%d\n"),
              unit, input_gport, parent_gport, cosq));

    if (!(BCM_GPORT_IS_SCHEDULER(input_gport) ||
          BCM_GPORT_IS_UCAST_QUEUE_GROUP(input_gport) ||
          BCM_GPORT_IS_MCAST_QUEUE_GROUP(input_gport))) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_node_get(unit, input_gport, NULL, &input_port, NULL,
                               &input_node));

    if (!input_node) {
        return BCM_E_PARAM;
    }

    if (_BCM_TH_MMU_INFO(unit)->gport_tree_created == TRUE) {
        if (IS_CPU_PORT(unit, input_port)) {
            return bcm_th_cosq_cpu_gport_detach(unit, input_gport,
                                                parent_gport, cosq);
        } else {
            return BCM_E_PARAM;
        }
    }

    if (BCM_GPORT_IS_SCHEDULER(parent_gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_node_get(unit, parent_gport, NULL, &parent_port, NULL,
                                   &parent_node));
    } else {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, parent_gport, &parent_port));
        parent_node = NULL;
    }

    if (input_port != parent_port) {
        return BCM_E_PARAM;
    }

    /* TH - Scheduler heirarchy is fixed. Hence check for wrong config. */
    if (parent_node) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(input_gport)) {
            input_hw_index = input_node->hw_index %
                             _BCM_TH_NUM_UCAST_QUEUE_PER_PORT;
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(input_gport)) {
            input_hw_index = input_node->hw_index %
                             _BCM_TH_NUM_MCAST_QUEUE_PER_PORT;
        } else {
            input_hw_index = input_node->hw_index %
                             _BCM_TH_NUM_SCHEDULER_PER_PORT;
        }

        parent_hw_index = parent_node->hw_index % _BCM_TH_NUM_SCHEDULER_PER_PORT;

        if (input_hw_index != parent_hw_index) {
            return BCM_E_PARAM;
        }
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_gport_attach_get
 * Purpose:
 *     Get attached status of a scheduler port
 * Parameters:
 *     unit       - (IN) unit number
 *     sched_port - (IN) scheduler GPORT identifier
 *     input_port - (OUT) GPORT to attach to
 *     cosq       - (OUT) COS queue to attached to
 * Returns:
 *     BCM_E_XXX
 * Notes:
 */
int
bcm_th_cosq_gport_attach_get(int unit, bcm_gport_t sched_gport,
                             bcm_gport_t *input_gport, bcm_cos_queue_t *cosq)
{
    _bcm_th_cosq_node_t *node = NULL;
    int numq = -1, cpu_mc_base = 0;
    bcm_port_t local_port = -1;
    soc_info_t *si = &SOC_INFO(unit);

    if (!(BCM_GPORT_IS_SET(sched_gport) ||
          BCM_GPORT_IS_SCHEDULER(sched_gport) ||
          BCM_GPORT_IS_UCAST_QUEUE_GROUP(sched_gport) ||
          BCM_GPORT_IS_MCAST_QUEUE_GROUP(sched_gport))) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_node_get(unit, sched_gport, NULL, &local_port, NULL,
                               &node));
    if (node == NULL) {
        return BCM_E_PARAM;
    }

    if (local_port < 0) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_SCHEDULER(sched_gport)) {
        *cosq = node->hw_index % _BCM_TH_NUM_SCHEDULER_PER_PORT;
    } else if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(sched_gport)) {
        *cosq = node->hw_index % _BCM_TH_NUM_UCAST_QUEUE_PER_PORT;
    } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(sched_gport)) {
        if (IS_CPU_PORT(unit, local_port)) {
            cpu_mc_base = si->port_cosq_base[CMIC_PORT(unit)];
            numq = _BCM_TH_NUM_CPU_MCAST_QUEUE;
            *cosq = (node->hw_index - cpu_mc_base) % numq;
        } else {
            numq = _BCM_TH_NUM_MCAST_QUEUE_PER_PORT;
            *cosq = node->hw_index % numq;
        }
    } else {
        return BCM_E_PARAM;
    }
    *input_gport = node->parent_gport;

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_th_cosq_gport_child_get
 * Purpose:
 *      Get the child node GPORT atatched to N-th index (cosq)
 *      of the scheduler GPORT.
 * Parameters:
 *      unit       - (IN)  Unit number.
 *      in_gport   - (IN)  Scheduler GPORT ID.
 *      cosq       - (IN)  COS queue attached to.
 *      out_gport  - (OUT) child GPORT ID.
 * Returns:
 *      BCM_E_XXX
 * Notes:
 */
int
bcm_th_cosq_gport_child_get(int unit, bcm_gport_t in_gport,
                            bcm_cos_queue_t cosq,
                            bcm_gport_t *out_gport)
{
    _bcm_th_cosq_node_t *tmp_node = NULL;
    bcm_port_t local_port = -1;
    _bcm_th_cosq_cpu_port_info_t *port_info = NULL;
    int index = -1, cpu_mc_base = 0;
    soc_info_t *si = &SOC_INFO(unit);

    if (out_gport == NULL) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, in_gport, &local_port));

    if (!SOC_PORT_VALID(unit, local_port)) {
        return BCM_E_PORT;
    }

    /* This API is only implemented to CPU ports because
       for all other ports we have fixed hierarchy */
    if (!IS_CPU_PORT(unit, local_port)) {
        return BCM_E_PARAM;
    }

    if ((cosq < 0) || (cosq >= NUM_CPU_COSQ(unit))) {
        return BCM_E_PARAM;
    }

    if (_bcm_th_cosq_cpu_port_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    port_info = _bcm_th_cosq_cpu_port_info[unit];
    cpu_mc_base = si->port_cosq_base[CMIC_PORT(unit)];

    if (BCM_GPORT_IS_SCHEDULER(in_gport)) {
        for (index = 0; index < _BCM_TH_NUM_CPU_MCAST_QUEUE; index++) {
            if (port_info->mcast[index].parent_gport == in_gport) {
                if (cosq == ((port_info->mcast[index].hw_index - cpu_mc_base) %
                             _BCM_TH_NUM_CPU_MCAST_QUEUE)) {
                    tmp_node = &port_info->mcast[index];
                    break;
                }
            }
        }
    } else {
        return BCM_E_PARAM;
    }

    if (tmp_node == NULL) {
        return BCM_E_NOT_FOUND;
    }

    *out_gport = tmp_node->gport;

    return BCM_E_NONE;
}

#ifdef DEAD_CODE
/*
 * Function:
 *     bcm_th_cosq_gport_delete
 * Purpose:
 *     Destroy a cosq gport structure
 * Parameters:
 *     unit  - (IN) unit number
 *     gport - (IN) GPORT identifier
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_gport_delete(int unit, bcm_gport_t gport)
{
    _bcm_th_cosq_node_t *node = NULL;
    _bcm_th_cosq_port_info_t *port_info = NULL;
    int local_port, i;

    LOG_INFO(BSL_LS_BCM_COSQ,
             (BSL_META_U(unit,
                         "bcm_th_cosq_gport_delete: unit=%d gport=0x%x\n"),
              unit, gport));

    if (_bcm_th_cosq_port_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    
    if (gport == -2) {
        bcm_th_cosq_sw_dump(unit);
    }

    if (BCM_GPORT_IS_SCHEDULER(gport) ||
          BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
          BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_node_get(unit, gport, NULL, NULL, NULL, &node));

        if (node->in_use == 0) {
            return BCM_E_UNAVAIL;
        }
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));

    if (!SOC_PORT_VALID(unit, local_port)) {
        return BCM_E_PORT;
    }
    port_info = &_bcm_th_cosq_port_info[unit][local_port];

    for (i = 0; i < _BCM_TH_NUM_SCHEDULER_PER_PORT; i++) {
        port_info->sched[i].in_use = 0;
        port_info->sched[i].numq = 0;
    }
    for (i = 0; i < _BCM_TH_NUM_UCAST_QUEUE_PER_PORT; i++) {
        port_info->ucast[i].in_use = 0;
        port_info->ucast[i].numq = 0;
    }
    for (i = 0; i < _BCM_TH_NUM_MCAST_QUEUE_PER_PORT; i++) {
        port_info->mcast[i].in_use = 0;
        port_info->mcast[i].numq = 0;
    }
    return BCM_E_NONE;
}
#endif

/*
 * Function:
 *     _bcm_th_cosq_mapping_set
 * Purpose:
 *     Set COS queue for the specified priority of an ingress port
 * Parameters:
 *     unit     - (IN) unit number
 *     ing_port - (IN) ingress port
 *     gport    - (IN) queue group GPORT identifier
 *     priority - (IN) priority value to map
 *     flags    - (IN) BCM_COSQ_GPORT_XXX_QUEUE_GROUP
 *     gport    - (IN) queue group GPORT identifier
 *     cosq     - (IN) COS queue number
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_th_cosq_mapping_set(int unit, bcm_port_t ing_port, bcm_cos_t priority,
                         uint32 flags, bcm_gport_t gport, bcm_cos_queue_t cosq)
{
    bcm_port_t local_port, out_port;
    bcm_cos_queue_t hw_cosq;
    int rv, idx;
    soc_field_t field[2] = {INVALIDf, INVALIDf}; /* [0]:UC, [1]:MC cos */
    void *entries[1];
    cos_map_sel_entry_t cos_map_sel_entry;
    port_cos_map_entry_t cos_map_entries[_TH_MMU_NUM_INT_PRI];
    uint32 old_index, new_index;

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, ing_port, &local_port));

    if (gport != -1) {
        BCM_IF_ERROR_RETURN(_bcm_th_cosq_localport_resolve(unit, gport,
                                                           &out_port));
    }

    switch (flags) {
        case BCM_COSQ_GPORT_UCAST_QUEUE_GROUP:
            if (gport == -1) {
                hw_cosq = cosq;
            } else {
                if (!BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
                    return BCM_E_PARAM;
                }
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_index_resolve
                     (unit, gport, cosq, _BCM_TH_COSQ_INDEX_STYLE_COS,
                      NULL, &hw_cosq, NULL));
            }
            hw_cosq %= _BCM_TH_NUM_UCAST_QUEUE_PER_PORT;
            field[0] = UC_COS1f;
            break;
        case BCM_COSQ_GPORT_MCAST_QUEUE_GROUP:
            if (gport == -1) {
                hw_cosq = cosq;
            } else {
                if (!BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                    return BCM_E_PARAM;
                }
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_index_resolve
                     (unit, gport, cosq, _BCM_TH_COSQ_INDEX_STYLE_COS,
                      NULL, &hw_cosq, NULL));
            }
            hw_cosq %= _BCM_TH_NUM_MCAST_QUEUE_PER_PORT;
            field[0] = MC_COS1f;
            break;
        case (BCM_COSQ_GPORT_UCAST_QUEUE_GROUP |
              BCM_COSQ_GPORT_MCAST_QUEUE_GROUP):
            if (gport == -1) {
                hw_cosq = cosq;
            } else {
                return BCM_E_PARAM;
            }
            hw_cosq %= _BCM_TH_NUM_SCHEDULER_PER_PORT;
            field[0] = UC_COS1f;
            field[1] = MC_COS1f;
            break;
        default:
            return BCM_E_PARAM;
    }

    entries[0] = &cos_map_entries;

    BCM_IF_ERROR_RETURN
        (READ_COS_MAP_SELm(unit, MEM_BLOCK_ANY, local_port,
                           &cos_map_sel_entry));
    old_index = soc_mem_field32_get(unit, COS_MAP_SELm, &cos_map_sel_entry,
                                    SELECTf);
    old_index *= _TH_MMU_NUM_INT_PRI;

    BCM_IF_ERROR_RETURN
        (soc_profile_mem_get(unit, _bcm_th_cos_map_profile[unit],
                             old_index, _TH_MMU_NUM_INT_PRI, entries));

    for (idx = 0; idx < 2; idx++) {
        if (field[idx] != INVALIDf) {
            soc_mem_field32_set(unit, PORT_COS_MAPm,
                                &cos_map_entries[priority],
                                field[idx], hw_cosq);
        }
    }

    soc_mem_lock(unit, PORT_COS_MAPm);

    rv = soc_profile_mem_delete(unit, _bcm_th_cos_map_profile[unit],
                                old_index);
    if (BCM_FAILURE(rv)) {
        soc_mem_unlock(unit, PORT_COS_MAPm);
        return rv;
    }

    rv = soc_profile_mem_add(unit, _bcm_th_cos_map_profile[unit], entries,
                             _TH_MMU_NUM_INT_PRI, &new_index);

    soc_mem_unlock(unit, PORT_COS_MAPm);

    if (BCM_FAILURE(rv)) {
        return rv;
    }

    soc_mem_field32_set(unit, COS_MAP_SELm, &cos_map_sel_entry, SELECTf,
                        new_index / _TH_MMU_NUM_INT_PRI);
    BCM_IF_ERROR_RETURN
        (WRITE_COS_MAP_SELm(unit, MEM_BLOCK_ANY, local_port,
                            &cos_map_sel_entry));
#ifndef BCM_COSQ_HIGIG_MAP_DISABLE
    if (IS_CPU_PORT(unit, local_port)) {
        BCM_IF_ERROR_RETURN
            (soc_mem_field32_modify(unit, COS_MAP_SELm,
                                    SOC_INFO(unit).cpu_hg_index, SELECTf,
                                    new_index / _TH_MMU_NUM_INT_PRI));
    }
#endif /* BCM_COSQ_HIGIG_MAP_DISABLE */

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_mapping_set
 * Purpose:
 *     Set which cosq a given priority should fall into
 * Parameters:
 *     unit     - (IN) unit number
 *     gport    - (IN) queue group GPORT identifier
 *     priority - (IN) priority value to map
 *     cosq     - (IN) COS queue to map to
 * Returns:
 *     BCM_E_XXX
 */

int
bcm_th_cosq_mapping_set(int unit, bcm_port_t port, bcm_cos_t priority,
                        bcm_cos_queue_t cosq)
{
    bcm_pbmp_t pbmp;
    bcm_port_t local_port;

    BCM_PBMP_CLEAR(pbmp);

    if ((priority < 0) || (priority >= _TH_MMU_NUM_INT_PRI)) {
        return BCM_E_PARAM;
    }

    if (cosq < 0) {
        return BCM_E_PARAM;
    }

    if (port == -1) {
        /* port value of -1 will cause the cosq mapping set for
           all types of local ports except Loopback ports.
           So we do the sanity check of cos value with Default Number
           of COS */
        if (cosq >= _TH_NUM_COS(unit)) {
            return BCM_E_PARAM;
        }
        BCM_PBMP_ASSIGN(pbmp, PBMP_ALL(unit));
    } else {
        if (BCM_GPORT_IS_SET(port)) {
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_SCHEDULER(port)) {
                return BCM_E_PARAM;
            } else if (BCM_GPORT_IS_LOCAL(port)) {
                local_port = BCM_GPORT_LOCAL_GET(port);
            } else if (BCM_GPORT_IS_MODPORT(port)) {
                BCM_IF_ERROR_RETURN(
                    bcm_esw_port_local_get(unit, port, &local_port));
            } else {
                return BCM_E_PARAM;
            }
        } else {
            local_port = port;
        }

        if (!SOC_PORT_VALID(unit, local_port)) {
            return BCM_E_PORT;
        }

        BCM_PBMP_PORT_ADD(pbmp, local_port);
        if (cosq >= _TH_PORT_NUM_COS(unit, local_port)) {
            return BCM_E_PARAM;
        }
    }

    PBMP_ITER(pbmp, local_port) {
        if (IS_LB_PORT(unit, local_port)) {
            continue;
        }

        /* If no ETS/gport, map the int prio symmetrically for ucast and
         * mcast */
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_mapping_set(unit, local_port,
                                      priority,
                                      BCM_COSQ_GPORT_UCAST_QUEUE_GROUP |
                                      BCM_COSQ_GPORT_MCAST_QUEUE_GROUP,
                                      -1, cosq));
    }

    return BCM_E_NONE;
}

int
bcm_th_cosq_gport_mapping_set(int unit, bcm_port_t ing_port,
                              bcm_cos_t int_pri, uint32 flags,
                              bcm_gport_t gport, bcm_cos_queue_t cosq)
{
    bcm_port_t local_port;

    if ((int_pri < 0) || (int_pri >= _TH_MMU_NUM_INT_PRI)) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN(_bcm_th_cosq_localport_resolve(unit, ing_port,
                                                       &local_port));

    if ((cosq < 0) || (cosq >= _TH_PORT_NUM_COS(unit, local_port))) {
        return BCM_E_PARAM;
    }

    return _bcm_th_cosq_mapping_set(unit, ing_port, int_pri, flags, gport,
                                    cosq);
}

/* Routine to perform Inverse mapping for given Queue port back to Port/Priority
 * which was defined by default or through cosq_mapping_set functions
 */
int
_bcm_th_cosq_inv_mapping_get(int unit, bcm_gport_t gport,
                             bcm_cos_queue_t cosq,
                             uint32 flags, bcm_port_t *ing_port,
                             bcm_cos_t *priority)
{
    bcm_port_t local_port;
    cos_map_sel_entry_t cos_map_sel_entry;
    void *entries[1];
    int num_queue = 1, index, ii;
    bcm_cos_queue_t in_cosq = BCM_COS_INVALID, pri_cosq = BCM_COS_INVALID;
    bcm_module_t    modid;
    soc_field_t cos_field = INVALIDf;
    port_cos_map_entry_t cos_map_entries[_TH_MMU_NUM_INT_PRI];

    if ((flags != BCM_COSQ_GPORT_UCAST_QUEUE_GROUP) &&
        (flags != BCM_COSQ_GPORT_MCAST_QUEUE_GROUP)) {
        return BCM_E_PARAM;
    }

    if ((ing_port == NULL) || (priority == NULL)) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_SET(gport)) {
        if (!(BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) &&
            !(BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport))) {
            return BCM_E_PARAM;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_node_get(unit, gport, &modid, &local_port,
                                   &in_cosq, NULL));
    } else {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
        in_cosq = cosq;
    }

    if ((cosq < -1) || (cosq >= _TH_PORT_NUM_COS(unit, local_port))) {
        return BCM_E_PARAM;
    }

    if ((IS_CPU_PORT(unit, local_port)) || (IS_LB_PORT(unit, local_port))) {
        return BCM_E_PARAM;
    }

    *ing_port = local_port;

    if (flags == BCM_COSQ_GPORT_MCAST_QUEUE_GROUP) {
        cos_field = MC_COS1f;
        num_queue = _BCM_TH_NUM_MCAST_QUEUE_PER_PORT;
    } else {
        cos_field = COSf;
        num_queue = _BCM_TH_NUM_UCAST_QUEUE_PER_PORT;
    }

    if (in_cosq == BCM_COS_INVALID) {
        return BCM_E_PARAM;
    } else {
        in_cosq %= num_queue;
    }

    entries[0] = &cos_map_entries;
    BCM_IF_ERROR_RETURN
        (READ_COS_MAP_SELm(unit, MEM_BLOCK_ANY, local_port,
                           &cos_map_sel_entry));
    index = soc_mem_field32_get(unit, COS_MAP_SELm, &cos_map_sel_entry,
                                SELECTf);
    index *= _TH_MMU_NUM_INT_PRI;

    BCM_IF_ERROR_RETURN
        (soc_profile_mem_get(unit, _bcm_th_cos_map_profile[unit],
                             index, _TH_MMU_NUM_INT_PRI, entries));

    for (ii = 0; ii < _TH_MMU_NUM_INT_PRI; ii++) {
        pri_cosq = soc_mem_field32_get(unit, PORT_COS_MAPm,
                                       &cos_map_entries[ii], cos_field);
        if (pri_cosq == in_cosq) {
            /* Required Cosq found. Break as the index is the priority */
            *priority = ii;
            break;
        }
    }

    if (ii == _TH_MMU_NUM_INT_PRI) {
        /* Mapping cannot be found */
        return BCM_E_NOT_FOUND;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_mapping_get
 * Purpose:
 *     Determine which COS queue a given priority currently maps to.
 * Parameters:
 *     unit     - (IN) unit number
 *     gport    - (IN) queue group GPORT identifier
 *     priority - (IN) priority value to map
 *     cosq     - (OUT) COS queue to map to
 * Returns:
 *     BCM_E_XXX
 */
int
_bcm_th_cosq_mapping_get(int unit, bcm_port_t ing_port, bcm_cos_t priority,
                         uint32 flags, bcm_gport_t *gport,
                         bcm_cos_queue_t *cosq)
{
    bcm_port_t local_port;
    cos_map_sel_entry_t cos_map_sel_entry;
    _bcm_th_cosq_port_info_t *port_info = NULL;
    _bcm_th_cosq_cpu_port_info_t *cpu_port_info = NULL;
    _bcm_th_cosq_node_t *node;
    int ii, index, cosq_base = 0;
    void *entry_p;
    bcm_cos_queue_t hw_cosq = BCM_COS_INVALID, node_cosq = BCM_COS_INVALID;
    int numq = -1;
    soc_info_t *si = &SOC_INFO(unit);

    if ((flags != BCM_COSQ_GPORT_UCAST_QUEUE_GROUP) &&
        (flags != BCM_COSQ_GPORT_MCAST_QUEUE_GROUP)) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, ing_port, &local_port));

    if (gport) {
        *gport = BCM_GPORT_INVALID;
    }

    *cosq = BCM_COS_INVALID;

    BCM_IF_ERROR_RETURN
        (READ_COS_MAP_SELm(unit, MEM_BLOCK_ANY, local_port,
                           &cos_map_sel_entry));
    index = soc_mem_field32_get(unit, COS_MAP_SELm, &cos_map_sel_entry,
                                SELECTf);
    index *= _TH_MMU_NUM_INT_PRI;

    entry_p = SOC_PROFILE_MEM_ENTRY(unit, _bcm_th_cos_map_profile[unit],
                                    port_cos_map_entry_t *,
                                    index + priority);
    if (IS_CPU_PORT(unit, local_port)) {
        cpu_port_info = &_bcm_th_cosq_cpu_port_info[unit][local_port];
    } else {
        port_info = &_bcm_th_cosq_port_info[unit][local_port];
    }

    switch (flags) {
        case BCM_COSQ_GPORT_UCAST_QUEUE_GROUP:
            hw_cosq = soc_mem_field32_get(unit, PORT_COS_MAPm, entry_p,
                                          UC_COS1f);
            if ((IS_CPU_PORT(unit, local_port) == FALSE) && (gport)) {
                for (ii = 0; ii < _BCM_TH_NUM_UCAST_QUEUE_PER_PORT; ii++) {
                    node = &port_info->ucast[ii];
                    if (node == NULL) {
                        return BCM_E_NOT_FOUND;
                    }
                    node_cosq = (node->hw_index %
                                 _BCM_TH_NUM_UCAST_QUEUE_PER_PORT);
                    if (node && (node_cosq == hw_cosq)) {
                        *gport = node->gport;
                        *cosq = hw_cosq;
                        break;
                    }
                }
            } else {
                *cosq = hw_cosq;
            }
            break;
        case BCM_COSQ_GPORT_MCAST_QUEUE_GROUP:
            hw_cosq = soc_mem_field32_get(unit, PORT_COS_MAPm, entry_p,
                                          MC_COS1f);
            if (gport) {
                if (IS_CPU_PORT(unit, local_port)) {
                    numq = _BCM_TH_NUM_CPU_MCAST_QUEUE;
                    cosq_base = si->port_cosq_base[CMIC_PORT(unit)];
                } else {
                    numq = _BCM_TH_NUM_MCAST_QUEUE_PER_PORT;
                    cosq_base = 0;
                }
                for (ii = 0; ii < numq; ii++) {
                    if (IS_CPU_PORT(unit, local_port)) {
                        node = &cpu_port_info->mcast[ii];
                    } else {
                        node = &port_info->mcast[ii];
                    }
                    if (!node) {
                        return BCM_E_NOT_FOUND;
                    }
                    node_cosq = ((node->hw_index - cosq_base) % numq);
                    if (node_cosq == hw_cosq) {
                        *gport = node->gport;
                        *cosq = hw_cosq;
                        break;
                    }
                }
            } else {
                *cosq = hw_cosq;
            }
            break;
        /*
         * COVERITY
         *
         * This default is unreachable. It is kept intentionally
         * as a defensive default for future development.
         */
        /* coverity[dead_error_begin] */
        default:
            return BCM_E_PARAM;
    }
    if (((gport &&
        (*gport == BCM_GPORT_INVALID) && (*cosq == BCM_COS_INVALID))) ||
        (*cosq == BCM_COS_INVALID)) {
        return BCM_E_NOT_FOUND;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_mapping_get
 * Purpose:
 *     Determine which COS queue a given priority currently maps to.
 * Parameters:
 *     unit     - (IN) unit number
 *     gport    - (IN) queue group GPORT identifier
 *     priority - (IN) priority value to map
 *     cosq     - (OUT) COS queue to map to
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_mapping_get(int unit, bcm_port_t port, bcm_cos_t priority,
                        bcm_cos_queue_t *cosq)
{
    bcm_port_t local_port;
    bcm_pbmp_t pbmp;

    if ((priority < 0) || (priority >= _TH_MMU_NUM_INT_PRI)) {
        return BCM_E_PARAM;
    }

    BCM_PBMP_CLEAR(pbmp);

    if (port == -1) {
        BCM_PBMP_ASSIGN(pbmp, PBMP_ALL(unit));
    } else {
        if (BCM_GPORT_IS_SET(port)) {
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_SCHEDULER(port)) {
                return BCM_E_PARAM;
            } else if (BCM_GPORT_IS_LOCAL(port)) {
                local_port = BCM_GPORT_LOCAL_GET(port);
            } else if (BCM_GPORT_IS_MODPORT(port)) {
                BCM_IF_ERROR_RETURN
                    (bcm_esw_port_local_get(unit, port, &local_port));
            } else {
                return BCM_E_PARAM;
            }
        } else {
            local_port = port;
        }

        if (!SOC_PORT_VALID(unit, local_port)) {
            return BCM_E_PORT;
        }

        BCM_PBMP_PORT_ADD(pbmp, local_port);
    }

    PBMP_ITER(pbmp, local_port) {
        if (IS_LB_PORT(unit, local_port)) {
            continue;
        }

        BCM_IF_ERROR_RETURN(_bcm_th_cosq_mapping_get(unit, local_port,
                 priority, BCM_COSQ_GPORT_UCAST_QUEUE_GROUP, NULL, cosq));
        break;
    }
    return BCM_E_NONE;
}

int
bcm_th_cosq_gport_mapping_get(int unit, bcm_port_t ing_port,
                              bcm_cos_t int_pri, uint32 flags,
                              bcm_gport_t *gport, bcm_cos_queue_t *cosq)
{
    if (gport == NULL || cosq == NULL) {
        return BCM_E_PARAM;
    }

    if ((int_pri < 0) || (int_pri >= _TH_MMU_NUM_INT_PRI)) {
        return BCM_E_PARAM;
    }

    return _bcm_th_cosq_mapping_get(unit, ing_port, int_pri, flags, gport,
                                    cosq);
}

/*
 * Function:
 *      bcm_th_cosq_field_classifier_get
 * Purpose:
 *      Get classifier type information.
 * Parameters:
 *      unit          - (IN) Unit number.
 *      classifier_id - (IN) Classifier ID
 *      classifier    - (OUT) Classifier info
 * Returns:
 *      BCM_E_xxx
 */
int
bcm_th_cosq_field_classifier_get(
    int unit,
    int classifier_id,
    bcm_cosq_classifier_t *classifier)
{
    if (classifier == NULL) {
        return BCM_E_PARAM;
    }
    sal_memset(classifier, 0, sizeof(bcm_cosq_classifier_t));

    if (_BCM_COSQ_CLASSIFIER_IS_FIELD(classifier_id)) {
        classifier->flags |= BCM_COSQ_CLASSIFIER_FIELD;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_th_cosq_field_classifier_id_create
 * Purpose:
 *      Create a cosq classifier.
 * Parameters:
 *      unit          - (IN) Unit number.
 *      classifier    - (IN) Classifier attributes
 *      classifier_id - (OUT) Classifier ID
 * Returns:
 *      BCM_E_xxx
 */
int
bcm_th_cosq_field_classifier_id_create(
    int unit,
    bcm_cosq_classifier_t *classifier,
    int *classifier_id)
{
    int rv;
    int ref_count = 0;
    int ent_per_set = _TH_MMU_NUM_INT_PRI;
    int i;

    if (NULL == classifier || NULL == classifier_id) {
        return BCM_E_PARAM;
    }

    for (i = 0; i < SOC_MEM_SIZE(unit, IFP_COS_MAPm); i += ent_per_set) {
        rv = soc_profile_mem_ref_count_get(
                unit, _bcm_th_ifp_cos_map_profile[unit], i, &ref_count);
        if (SOC_FAILURE(rv)) {
            return rv;
        }
        if (0 == ref_count) {
            break;
        }
    }

    if (i >= SOC_MEM_SIZE(unit, IFP_COS_MAPm) && ref_count != 0) {
        *classifier_id = 0;
        return BCM_E_RESOURCE;
    }

    _BCM_COSQ_CLASSIFIER_FIELD_SET(*classifier_id, (i / ent_per_set));

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_th_cosq_field_classifier_map_set
 * Purpose:
 *      Set internal priority to ingress field processor CoS queue
 *      override mapping.
 * Parameters:
 *      unit           - (IN) Unit number.
 *      classifier_id  - (IN) Classifier ID
 *      count          - (IN) Number of elements in priority_array and
 *                            cosq_array.
 *      priority_array - (IN) Array of internal priorities
 *      cosq_array     - (IN) Array of COS queues
 * Returns:
 *      BCM_E_xxx
 * Notes:
 *      Supported count value is [0-16] and 48
 *      When count = [0-16]
 *          Array Index [15-0] : Maps Prio[15-0] to UC, MC
 *                               and RQE cos[15-0]
 *      When count = 48
 *      Prio Array Index Map
 *          Array Index [15-0] : Maps Prio[15-0] to UC cos[15-0]
 *          Array Index [31-16] : Maps Prio[15-0] to MC cos[15-0]
 *          Array Index [47-32] : Maps Prio[15-0] to RQE cos[15-0]
 */
int
bcm_th_cosq_field_classifier_map_set(
    int unit,
    int classifier_id,
    int count,
    bcm_cos_t *priority_array,
    bcm_cos_queue_t *cosq_array)
{
    int rv;
    int i;
    int max_entries = _TH_MMU_NUM_INT_PRI;
    uint32 index, new_index;
    void *entries[1];
    ifp_cos_map_entry_t ent_buf[_TH_MMU_NUM_INT_PRI];
    soc_field_t field = -1;

    /* Input parameter check. */
    if (!_BCM_COSQ_CLASSIFIER_IS_FIELD(classifier_id)) {
        return BCM_E_PARAM;
    }

    if (NULL == priority_array || NULL == cosq_array) {
        return BCM_E_PARAM;
    }

    if ((count > max_entries) && (count != (max_entries * 3))) {
        return BCM_E_PARAM;
    }

    sal_memset(ent_buf, 0, sizeof(ifp_cos_map_entry_t) * max_entries);
    entries[0] = ent_buf;

    if (count != (max_entries * 3)) {
        for (i = 0; i < count; i++) {
            if (priority_array[i] < max_entries) {
                /* When count != max_entries * 3
                   We program the same mapping for all three
                   fields UC/MC/RQE queues.
                   Hence cos_array[i] cannot have value more than
                   equal to 10 */
                if ((cosq_array[i] < 0) ||
                    (cosq_array[i] >= _BCM_TH_NUM_UCAST_QUEUE_PER_PORT)) {
                    return BCM_E_PARAM;
                }
                soc_mem_field32_set(unit, IFP_COS_MAPm, &ent_buf[priority_array[i]],
                                    IFP_MC_COSf, cosq_array[i]);
                soc_mem_field32_set(unit, IFP_COS_MAPm, &ent_buf[priority_array[i]],
                                    IFP_UC_COSf, cosq_array[i]);
                soc_mem_field32_set(unit, IFP_COS_MAPm, &ent_buf[priority_array[i]],
                                    IFP_RQE_Q_NUMf, cosq_array[i]);
            }
        }
    } else {
        for (i = 0; i < count; i++) {
            if (priority_array[i] < max_entries) {
                if (i < max_entries) {
                    /* Number of UCAST queue per port = 10 */
                    if ((cosq_array[i] < 0) ||
                        (cosq_array[i] >= _BCM_TH_NUM_UCAST_QUEUE_PER_PORT)) {
                        return BCM_E_PARAM;
                    }
                    field = IFP_UC_COSf;
                } else if (i < (max_entries * 2)) {
                    /* Number of MCAST queue per port = 10 */
                    if ((cosq_array[i] < 0) ||
                        (cosq_array[i] >= _BCM_TH_NUM_MCAST_QUEUE_PER_PORT)) {
                        return BCM_E_PARAM;
                    }
                    field = IFP_MC_COSf;
                } else {
                    /* Number of RQE queue per unit = 11 */
                    if ((cosq_array[i] < 0) ||
                        (cosq_array[i] >= _TH_MMU_NUM_RQE_QUEUES)) {
                        return BCM_E_PARAM;
                    }
                    field = IFP_RQE_Q_NUMf;
                }
                soc_mem_field32_set(unit, IFP_COS_MAPm, &ent_buf[priority_array[i]],
                                    field, cosq_array[i]);
            }
        }
    }

    index = _BCM_COSQ_CLASSIFIER_FIELD_GET(classifier_id);

    rv = soc_profile_mem_delete(unit, _bcm_th_ifp_cos_map_profile[unit],
                                index * max_entries);

    /* If rv = SOC_E_NONE means that there exists a
     * mapping for given classifier_id which is now
     * successfully deleted and we can overwrite.
     * rv = SOC_E_NOT_FOUND means there was no mapping
     * for given classifier_id and we can create a new mapping
     * Otherwise there is something else wrong and we return
     * the error value returned by soc_profile_mem_delete()
     */
    if ((rv != SOC_E_NONE) && (rv != SOC_E_NOT_FOUND)) {
        return rv;
    }

    SOC_IF_ERROR_RETURN
        (soc_profile_mem_add(unit, _bcm_th_ifp_cos_map_profile[unit],
                             entries, max_entries, &new_index));

    /* If we get (index == new_index / max_entries) means we have
     * a successful mapping operation.
     * But if they are not equal which means there already exists
     * a similar mapping associated with other classifier_id hence
     * we return BCM_E_EXISTS.
     */

    if (index != (new_index / max_entries)) {
        SOC_IF_ERROR_RETURN
            (soc_profile_mem_delete(unit, _bcm_th_ifp_cos_map_profile[unit],
                                    new_index));
        return BCM_E_EXISTS;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_th_cosq_field_classifier_map_get
 * Purpose:
 *      Get internal priority to ingress field processor CoS queue
 *      override mapping information.
 * Parameters:
 *      unit           - (IN) Unit number.
 *      classifier_id  - (IN) Classifier ID
 *      array_max      - (IN) Size of priority_array and cosq_array
 *      priority_array - (IN) Array of internal priorities
 *      cosq_array     - (OUT) Array of COS queues
 *      array_count    - (OUT) Size of cosq_array
 * Returns:
 *      BCM_E_xxx
 * Notes:
 *      Supported array_max = [0-16] and 48
 *      When array_max = [0-16]
 *          Array Index [15-0] : Maps Prio[15-0] to UC cos[15-0]
 *      When array_max = 48
 *      Prio Array Index Map
 *          Array Index [15-0] : Maps Prio[15-0] to UC cos[15-0]
 *          Array Index [31-16] : Maps Prio[15-0] to MC cos[15-0]
 *          Array Index [47-32] : Maps Prio[15-0] to RQE cos[15-0]
 */
int
bcm_th_cosq_field_classifier_map_get(
    int unit,
    int classifier_id,
    int array_max,
    bcm_cos_t *priority_array,
    bcm_cos_queue_t *cosq_array,
    int *array_count)
{
    int rv;
    int i;
    int ent_per_set = _TH_MMU_NUM_INT_PRI;
    uint32 index;
    void *entries[1];
    ifp_cos_map_entry_t ent_buf[_TH_MMU_NUM_INT_PRI];
    soc_field_t field = -1;

    /* Input parameter check. */
    if (NULL == priority_array || NULL == cosq_array || NULL == array_count) {
        return BCM_E_PARAM;
    }

    sal_memset(ent_buf, 0, sizeof(ifp_cos_map_entry_t) * ent_per_set);
    entries[0] = ent_buf;

    /* Get profile table entry set base index. */
    index = _BCM_COSQ_CLASSIFIER_FIELD_GET(classifier_id);

    /* Read out entries from profile table into allocated buffer. */
    rv = soc_profile_mem_get(unit, _bcm_th_ifp_cos_map_profile[unit],
                             index * ent_per_set, ent_per_set, entries);
    if (SOC_FAILURE(rv)) {
        return rv;
    }

    if (array_max <= ent_per_set) {
        *array_count = array_max;
    } else if ((array_max > ent_per_set) && (array_max < (ent_per_set * 3))) {
        return BCM_E_PARAM;
    } else {
        *array_count = ent_per_set * 3;
    }
        /* Copy values into API OUT parameters. */
    for (i = 0; i < *array_count; i++) {
        if (priority_array[i % 16] >= _TH_MMU_NUM_INT_PRI) {
            return BCM_E_PARAM;
        }
        if (i < ent_per_set) {
            field = IFP_UC_COSf;
        } else if (i < (ent_per_set * 2)) {
            field = IFP_MC_COSf;
        } else {
            field = IFP_RQE_Q_NUMf;
        }
        cosq_array[i] = soc_mem_field32_get(unit, IFP_COS_MAPm,
                                            &ent_buf[priority_array[i]],
                                            field);
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_th_cosq_field_classifier_map_clear
 * Purpose:
 *      Delete an internal priority to ingress field processor CoS queue
 *      override mapping profile set.
 * Parameters:
 *      unit          - (IN) Unit number.
 *      classifier_id - (IN) Classifier ID
 * Returns:
 *      BCM_E_xxx
 */
int
bcm_th_cosq_field_classifier_map_clear(int unit, int classifier_id)
{
    int ent_per_set = _TH_MMU_NUM_INT_PRI;
    uint32 index;

    /* Get profile table entry set base index. */
    index = _BCM_COSQ_CLASSIFIER_FIELD_GET(classifier_id);

    /* Delete the profile entries set. */
    SOC_IF_ERROR_RETURN
        (soc_profile_mem_delete(unit,
                                _bcm_th_ifp_cos_map_profile[unit],
                                index * ent_per_set));
    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_th_cosq_field_classifier_id_destroy
 * Purpose:
 *      Free resource associated with this field classifier id.
 * Parameters:
 *      unit          - (IN) Unit number.
 *      classifier_id - (IN) Classifier ID
 * Returns:
 *      BCM_E_xxx
 */
int
bcm_th_cosq_field_classifier_id_destroy(int unit, int classifier_id)
{
    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_egr_queue_set(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq,
                            bcm_cosq_control_t type, int arg)
{
    bcm_port_t local_port;
    int pipe, startq;
    uint32 max_val;
    uint32 entry[SOC_MAX_MEM_WORDS];
    uint32 entry2[SOC_MAX_MEM_WORDS];
    soc_mem_t mem = INVALIDm, mem2 = INVALIDm;
    soc_field_t fld_limit = INVALIDf;
    int granularity = 1;
    int from_cos, to_cos, ci;
    int egr_db_shd_size[_TH_XPES_PER_DEV], egr_qe_shd_size[_TH_XPES_PER_DEV];
    int cur_val;
    int limit;
    int disable_queuing = 0;
    int uc_qlimit_enable = 0, mc_qlimit_enable = 0;

    if (arg < 0) {
        return BCM_E_PARAM;
    }

    arg = (arg + _TH_MMU_BYTES_PER_CELL - 1) / _TH_MMU_BYTES_PER_CELL;

    if ((type == bcmCosqControlEgressUCQueueMinLimitBytes) ||
        (type == bcmCosqControlEgressUCQueueSharedLimitBytes)) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                             _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                             &local_port, &startq, NULL));
            SOC_IF_ERROR_RETURN
                (soc_port_pipe_get(unit, local_port, &pipe));
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            return BCM_E_PARAM;
        }
        else {
            if (cosq == BCM_COS_INVALID) {
                from_cos = to_cos = 0;
            } else {
                from_cos = to_cos = cosq;
            }

            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
            if (local_port < 0) {
                return BCM_E_PORT;
            }
            SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
            for (ci = from_cos; ci <= to_cos; ci++) {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_index_resolve
                     (unit, local_port, ci, _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                      NULL, &startq, NULL));
            }
        }
        mem = SOC_MEM_UNIQUE_ACC(unit, MMU_THDU_CONFIG_QUEUEm)[pipe];
        mem2 = SOC_MEM_UNIQUE_ACC(unit, MMU_THDU_Q_TO_QGRP_MAPm)[pipe];
    } else if ((type == bcmCosqControlEgressMCQueueMinLimitBytes) ||
               (type == bcmCosqControlEgressMCQueueSharedLimitBytes)) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            return BCM_E_PARAM;
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                             _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                             &local_port, &startq, NULL));
            SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
        }
        else {
            if (cosq == BCM_COS_INVALID) {
                from_cos = to_cos = 0;
            } else {
                from_cos = to_cos = cosq;
            }

            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
            if (local_port < 0) {
                return BCM_E_PORT;
            }
            SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
            for (ci = from_cos; ci <= to_cos; ci++) {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_index_resolve
                     (unit, local_port, ci, _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                      NULL, &startq, NULL));
            }
        }
        mem = SOC_MEM_UNIQUE_ACC(unit, MMU_THDM_DB_QUEUE_CONFIGm)[pipe];
        mem2 = SOC_MEM_UNIQUE_ACC(unit, MMU_THDM_MCQE_QUEUE_CONFIGm)[pipe];
    } else {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
    if (mem2 != INVALIDm) {
        BCM_IF_ERROR_RETURN
            (soc_mem_read(unit, mem2, MEM_BLOCK_ALL, startq, entry2));
    }

    switch (type) {
        case bcmCosqControlEgressUCQueueSharedLimitBytes:
            fld_limit = Q_SHARED_LIMIT_CELLf;

            uc_qlimit_enable = 1;

            /* Set MMU_THDU_XPIPE_Q_TO_QGRP_MAP.DISABLE_QUEUING to 1 when both MIN
             * and SHARED LIMIT are 0. Set it to 0 when either MIN or SHARED LIMIT
             * is non-zero.
             */
            limit = soc_mem_field32_get(unit, mem, entry, Q_MIN_LIMIT_CELLf);
            if (arg == 0 && limit == 0) {
                disable_queuing = 1;
            } else if (arg != 0 || limit != 0) {
                disable_queuing = 0;
            }
            granularity = 1;
            break;
        case bcmCosqControlEgressMCQueueSharedLimitBytes:
            fld_limit = Q_SHARED_LIMITf;
            mc_qlimit_enable = 1;
            granularity = 1;
            break;
        case bcmCosqControlEgressUCQueueMinLimitBytes:
            fld_limit = Q_MIN_LIMIT_CELLf;

            /* Set MMU_THDU_Q_TO_QGRP_MAP.DISABLE_QUEUING to 1 when both MIN
             * and SHARED LIMIT are 0. Set it to 0 when either MIN or
             * SHARED LIMIT is non-zero
             */
            limit = soc_mem_field32_get(unit, mem, entry, Q_SHARED_LIMIT_CELLf);
            if (arg == 0 && limit == 0) {
                disable_queuing = 1;
            } else if (arg != 0 || limit != 0) {
                disable_queuing = 0;
            }

            granularity = 1;
            break;
        case bcmCosqControlEgressMCQueueMinLimitBytes:
            fld_limit = Q_MIN_LIMITf;
            granularity = 1;
            break;
        default:
            return BCM_E_UNAVAIL;
    }

    /* Check for min/max values */
    max_val = (1 << soc_mem_field_length(unit, mem, fld_limit)) - 1;
    if ((arg < 0) || ((arg/granularity) > max_val)) {
        return BCM_E_PARAM;
    }

    /* guarantee limits need adjust shared limits */
    if ((type == bcmCosqControlEgressUCQueueMinLimitBytes) ||
         (type == bcmCosqControlEgressMCQueueMinLimitBytes)) {
        int xpe, xpe_map;
        int delta[_TH_XPES_PER_DEV] = {0}, update;
        int *egr_shd_size;

        
        cur_val = soc_mem_field32_get(unit, mem, entry, fld_limit);
        sal_memcpy(egr_db_shd_size, _BCM_TH_MMU_INFO(unit)->egr_db_shared_limit, 
            sizeof(egr_db_shd_size));
        sal_memcpy(egr_qe_shd_size, _BCM_TH_MMU_INFO(unit)->egr_qe_shared_limit, 
            sizeof(egr_qe_shd_size));
        
        egr_shd_size = egr_db_shd_size;
        
        /* adjustment delta */
        xpe_map = SOC_INFO(unit).epipe_xpe_map[pipe];
        for (xpe = 0; xpe < NUM_XPE(unit); xpe ++) {
            if (xpe_map & (1 << xpe)) {
                delta[xpe] = ((arg / granularity) - cur_val) * granularity;
            }
        }
        
        /* shared limits to be decreased */
        for (xpe = 0, update = 0; xpe < NUM_XPE(unit); xpe ++) {
            if ((xpe_map & (1 << xpe)) && (delta[xpe] > 0)) {
                /* no space to decrease shared limit */
                if (delta[xpe] > egr_shd_size[xpe]) {
                    return BCM_E_PARAM;
                }
                egr_shd_size[xpe] -= delta[xpe];
                update = 1;
            }
        }
        if (update) {
            BCM_IF_ERROR_RETURN
                (soc_th_mmu_config_res_limits_update(unit, 2 /* egr */,
                    NULL, egr_db_shd_size, egr_qe_shd_size, 1));
        }
        
        /* set new guarantee */
        /* the hw value may be changed by soc_th_mmu_config_res_limits_update
         * so read the register again in order to keep consistent  */
        BCM_IF_ERROR_RETURN
            (soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
        soc_mem_field32_set(unit, mem, entry, fld_limit, (arg/granularity));
        BCM_IF_ERROR_RETURN
            (soc_mem_write(unit, mem, MEM_BLOCK_ALL, startq, entry));
        
        /* shared limits to be increased */
        for (xpe = 0, update = 0; xpe < NUM_XPE(unit); xpe ++) {
            if ((xpe_map & (1 << xpe)) && (delta[xpe] < 0)) {
                egr_shd_size[xpe] -= delta[xpe];
                update = 1;
            }
        }
        if (update) {
            BCM_IF_ERROR_RETURN
                (soc_th_mmu_config_res_limits_update(unit, 2 /* egr */,
                    NULL, egr_db_shd_size, egr_qe_shd_size, 0));
        }
        
        sal_memcpy(_BCM_TH_MMU_INFO(unit)->egr_db_shared_limit, egr_db_shd_size,
            sizeof(egr_db_shd_size));
        sal_memcpy(_BCM_TH_MMU_INFO(unit)->egr_qe_shared_limit, egr_qe_shd_size,
            sizeof(egr_qe_shd_size));
    } else {
        soc_mem_field32_set(unit, mem, entry, fld_limit, (arg/granularity));
        BCM_IF_ERROR_RETURN
            (soc_mem_write(unit, mem, MEM_BLOCK_ALL, startq, entry));
    }

    /* limit enable */
    BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
    BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem2, MEM_BLOCK_ALL, startq, entry2));

    if ((type == bcmCosqControlEgressUCQueueMinLimitBytes) ||
        (type == bcmCosqControlEgressUCQueueSharedLimitBytes)) {
        soc_mem_field32_set(unit, mem2, entry2, DISABLE_QUEUINGf, disable_queuing);
    }

    if (mc_qlimit_enable) {
        soc_mem_field32_set(unit, mem, entry, Q_LIMIT_ENABLEf, 1);
        soc_mem_field32_set(unit, mem2, entry2, Q_LIMIT_ENABLEf, 1);
    }

    if (uc_qlimit_enable) {
        soc_mem_field32_set(unit, mem2, entry2, Q_LIMIT_ENABLEf, 1);
    }

    BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ALL, startq, entry));
    BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem2, MEM_BLOCK_ALL, startq, entry2));

    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_egr_queue_get(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq,
                            bcm_cosq_control_t type, int *arg)
{
    bcm_port_t local_port;
    int pipe, startq;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_mem_t mem = INVALIDm;
    soc_field_t fld_limit = INVALIDf;
    int granularity = 1;
    int from_cos, to_cos, ci;

    if (!arg) {
        return BCM_E_PARAM;
    }

    if ((type == bcmCosqControlEgressUCQueueMinLimitBytes) ||
        (type == bcmCosqControlEgressUCQueueSharedLimitBytes)) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                             _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                             &local_port, &startq, NULL));
            SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            return BCM_E_PARAM;
        }
        else {
            if (cosq == BCM_COS_INVALID) {
                from_cos = to_cos = 0;
            } else {
                from_cos = to_cos = cosq;
            }

            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
            if (local_port < 0) {
                return BCM_E_PORT;
            }
            SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
            for (ci = from_cos; ci <= to_cos; ci++) {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_index_resolve
                     (unit, local_port, ci, _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                      NULL, &startq, NULL));
            }
        }
        mem = SOC_MEM_UNIQUE_ACC(unit, MMU_THDU_CONFIG_QUEUEm)[pipe];
    } else if ((type == bcmCosqControlEgressMCQueueMinLimitBytes) ||
               (type == bcmCosqControlEgressMCQueueSharedLimitBytes)) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            return BCM_E_PARAM;
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                             _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                             &local_port, &startq, NULL));
            SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
        }
        else {
            if (cosq == BCM_COS_INVALID) {
                from_cos = to_cos = 0;
            } else {
                from_cos = to_cos = cosq;
            }

            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
            if (local_port < 0) {
                return BCM_E_PORT;
            }
            SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
            for (ci = from_cos; ci <= to_cos; ci++) {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_index_resolve
                     (unit, local_port, ci, _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                      NULL, &startq, NULL));
            }
        }
        mem = SOC_MEM_UNIQUE_ACC(unit, MMU_THDM_DB_QUEUE_CONFIGm)[pipe];
    } else {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL,
                             startq, entry));

    switch (type) {
        case bcmCosqControlEgressUCQueueSharedLimitBytes:
            fld_limit = Q_SHARED_LIMIT_CELLf;
            granularity = 1;
            break;
        case bcmCosqControlEgressMCQueueSharedLimitBytes:
            fld_limit = Q_SHARED_LIMITf;
            granularity = 1;
            break;
        case bcmCosqControlEgressUCQueueMinLimitBytes:
            fld_limit = Q_MIN_LIMIT_CELLf;
            granularity = 1;
            break;
        case bcmCosqControlEgressMCQueueMinLimitBytes:
            fld_limit = Q_MIN_LIMITf;
            granularity = 1;
            break;
        default:
            return BCM_E_UNAVAIL;
    }
    *arg = soc_mem_field32_get(unit, mem, entry, fld_limit);
    *arg = (*arg) * granularity * _TH_MMU_BYTES_PER_CELL;
    return BCM_E_NONE;
}

/*
 * This function is used to get ingress pipe, shared pool id
 * priority group and headroom pool id for a given [Port, Prio]
 */
STATIC int
_bcm_th_cosq_ing_pool_pg_pipe_get(int unit, bcm_gport_t gport,
                                  bcm_cos_t prio, int *pipe,
                                  int *sp, int *pg, int *hdrm_pool)
{
    bcm_port_t local_port;

    if (pipe) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
        if (local_port < 0) {
            return BCM_E_PORT;
        }
        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, pipe));
    }

    if (pg) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_ingress_pg_get(unit, gport, prio, pg,
                                         NULL ));

        if ((*pg < 0) || (*pg >= _TH_MMU_NUM_PG)) {
            return BCM_E_PARAM;
        }
    }

    if (sp) {
        BCM_IF_ERROR_RETURN(
            _bcm_th_cosq_ingress_sp_get(unit, gport, prio, sp,
                                        NULL));

        if ((*sp < 0) || (*sp >= _TH_MMU_NUM_POOL)) {
            return BCM_E_PARAM;
        }
    }

    if (hdrm_pool) {
        BCM_IF_ERROR_RETURN(
            _bcm_th_cosq_ingress_hdrm_pool_get(unit, gport,
                                               prio, hdrm_pool,
                                               NULL));

        if ((*hdrm_pool < 0) || (*hdrm_pool >= _TH_MMU_NUM_POOL)) {
            return BCM_E_PARAM;
        }
    }

    return BCM_E_NONE;
}


/*
 * This function is used to get the
 * Min and Max value of ingress PG or SP
 */
STATIC int
_bcm_th_cosq_ing_res_get(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq,
                          bcm_cosq_control_t type, int *arg)
{
    bcm_port_t local_port;
    int midx;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_mem_t mem = INVALIDm, base_mem = INVALIDm;
    soc_field_t fld_limit = INVALIDf;
    int pipe, port_pg;
    int pool, granularity = 1;

    if ((cosq < 0) || (cosq >= _TH_MMU_NUM_INT_PRI)) {
        /* Error. Input pri > Max Pri_Grp */
        return BCM_E_PARAM;
    }

    if (arg == NULL) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_SCHEDULER(gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
    if (!SOC_PORT_VALID(unit, local_port)) {
        return BCM_E_PORT;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_ing_pool_pg_pipe_get(unit, gport, cosq,
                                           &pipe, &pool, &port_pg,
                                           NULL));

    if ((type == bcmCosqControlIngressPortPGSharedLimitBytes) ||
        (type == bcmCosqControlIngressPortPGMinLimitBytes) ||
        (type == bcmCosqControlIngressPortPGHeadroomLimitBytes)||
        (type == bcmCosqControlIngressPortPGResetFloorBytes)) {
        base_mem = THDI_PORT_PG_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        /* get index for Port-PG */
        midx = SOC_TH_MMU_PIPED_MEM_INDEX(unit, local_port, base_mem, port_pg);
    } else if ((type == bcmCosqControlIngressPortPoolMaxLimitBytes) ||
               (type == bcmCosqControlIngressPortPoolMinLimitBytes)) {
        base_mem = THDI_PORT_SP_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        /* get index for Port-SP */
        midx = SOC_TH_MMU_PIPED_MEM_INDEX(unit, local_port, base_mem, pool);
    } else {
        return BCM_E_UNAVAIL;
    }

    if (midx < 0) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, midx, entry));

    switch (type) {
        case bcmCosqControlIngressPortPGSharedLimitBytes:
            fld_limit = PG_SHARED_LIMITf;
            granularity = 1;
            break;
        case bcmCosqControlIngressPortPoolMaxLimitBytes:
            fld_limit = PORT_SP_MAX_LIMITf;
            granularity = 1;
            break;
        case bcmCosqControlIngressPortPGMinLimitBytes:
            fld_limit = PG_MIN_LIMITf;
            granularity = 1;
            break;
        case bcmCosqControlIngressPortPoolMinLimitBytes:
            fld_limit = PORT_SP_MIN_LIMITf;
            granularity = 1;
            break;
        case bcmCosqControlIngressPortPGHeadroomLimitBytes:
            fld_limit = PG_HDRM_LIMITf;
            granularity = 1;
            break;
        case bcmCosqControlIngressPortPGResetFloorBytes:
            fld_limit = PG_RESET_FLOORf;
            granularity = 1;
            break;
        /* coverity[dead_error_begin] */
        default:
            return BCM_E_UNAVAIL;
    }

    *arg = soc_mem_field32_get(unit, mem, entry, fld_limit);
    *arg = (*arg) * granularity * _TH_MMU_BYTES_PER_CELL;

    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_ing_res_set(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq, 
                          bcm_cosq_control_t type, int arg)
{
    bcm_port_t local_port;
    int midx;
    uint32 max_val;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_mem_t mem = INVALIDm, base_mem = INVALIDm;
    soc_field_t fld_limit = INVALIDf;
    int pipe, port_pg;
    int pool, granularity = 1;
    int  ing_shd_size[_TH_XPES_PER_DEV], cur_val;

    if ((cosq < 0) || (cosq >= _TH_MMU_NUM_INT_PRI)) {
        /* Error. Input pri > Max Pri_Grp */
        return BCM_E_PARAM;
    }

    if (arg < 0) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_SCHEDULER(gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        return BCM_E_PARAM;
    }

    arg = (arg + _TH_MMU_BYTES_PER_CELL - 1) / _TH_MMU_BYTES_PER_CELL;
    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
    if (!SOC_PORT_VALID(unit, local_port)) {
        return BCM_E_PORT;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_ing_pool_pg_pipe_get(unit, gport, cosq,
                                           &pipe, &pool, &port_pg,
                                           NULL));

    if ((type == bcmCosqControlIngressPortPGSharedLimitBytes) ||
        (type == bcmCosqControlIngressPortPGMinLimitBytes) ||
        (type == bcmCosqControlIngressPortPGHeadroomLimitBytes)||
        (type == bcmCosqControlIngressPortPGResetFloorBytes)) {
        base_mem = THDI_PORT_PG_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        /* get index for Port-PG */
        midx = SOC_TH_MMU_PIPED_MEM_INDEX(unit, local_port, base_mem, port_pg);
    } else if ((type == bcmCosqControlIngressPortPoolMaxLimitBytes) ||
               (type == bcmCosqControlIngressPortPoolMinLimitBytes)) {
        base_mem = THDI_PORT_SP_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        /* get index for Port-SP */
        midx = SOC_TH_MMU_PIPED_MEM_INDEX(unit, local_port, base_mem, pool);
    } else {
        return BCM_E_UNAVAIL;
    }

    if (midx < 0) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, midx, entry));

    switch (type) {
        case bcmCosqControlIngressPortPGSharedLimitBytes:
            fld_limit = PG_SHARED_LIMITf;
            granularity = 1;
            break;
        case bcmCosqControlIngressPortPoolMaxLimitBytes:
            fld_limit = PORT_SP_MAX_LIMITf;
            granularity = 1;
            break;
        case bcmCosqControlIngressPortPGMinLimitBytes:
            fld_limit = PG_MIN_LIMITf;
            granularity = 1;
            break;
        case bcmCosqControlIngressPortPoolMinLimitBytes:
            fld_limit = PORT_SP_MIN_LIMITf;
            granularity = 1;
            break;
        case bcmCosqControlIngressPortPGHeadroomLimitBytes:
            fld_limit = PG_HDRM_LIMITf;
            granularity = 1;
            break;
        case bcmCosqControlIngressPortPGResetFloorBytes:
            fld_limit = PG_RESET_FLOORf;
            granularity = 1;
            break;
        /* coverity[dead_error_begin] */
        default:
            return BCM_E_UNAVAIL;
    }

    /* Check for min/max values */
    max_val = (1 << soc_mem_field_length(unit, mem, fld_limit)) - 1;
    if ((arg < 0) || ((arg/granularity) > max_val)) {
        return BCM_E_PARAM;
    }

    /* guarantee limits need adjust shared limits */
    if (type == bcmCosqControlIngressPortPGMinLimitBytes) {
        int xpe, xpe_map;
        int delta[_TH_XPES_PER_DEV] = {0}, update;
        
        cur_val = soc_mem_field32_get(unit, mem, entry, fld_limit);
        sal_memcpy(ing_shd_size, _BCM_TH_MMU_INFO(unit)->ing_shared_limit, 
            sizeof(ing_shd_size));
        
        /* adjustment delta */
        xpe_map = SOC_INFO(unit).ipipe_xpe_map[pipe];
        for (xpe = 0; xpe < NUM_XPE(unit); xpe ++) {
            if (xpe_map & (1 << xpe)) {
                delta[xpe] = ((arg / granularity) - cur_val) * granularity;
            }
        }
        
        /* shared limits to be decreased */
        for (xpe = 0, update = 0; xpe < NUM_XPE(unit); xpe ++) {
            if ((xpe_map & (1 << xpe)) && (delta[xpe] > 0)) {
                /* no space to decrease shared limit */
                if (delta[xpe] > ing_shd_size[xpe]) {
                    return BCM_E_PARAM;
                }
                ing_shd_size[xpe] -= delta[xpe];
                update = 1;
            }
        }
        if (update) {
            BCM_IF_ERROR_RETURN
                (soc_th_mmu_config_res_limits_update(unit, 1 /* ing */,
                    ing_shd_size, NULL, NULL, 1));
        }

        /* set new guarantee */
        /* the hw value may be changed by soc_th_mmu_config_res_limits_update
         * so read the register again in order to keep consistent  */
        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, midx, entry));
        soc_mem_field32_set(unit, mem, entry, fld_limit, (arg/granularity));
        BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ALL, midx, entry));

        /* shared limits to be increased */
        for (xpe = 0, update = 0; xpe < NUM_XPE(unit); xpe ++) {
            if ((xpe_map & (1 << xpe)) && (delta[xpe] < 0)) {
                ing_shd_size[xpe] -= delta[xpe];
                update = 1;
            }
        }
        if (update) {
            BCM_IF_ERROR_RETURN
                (soc_th_mmu_config_res_limits_update(unit, 1 /* ing */,
                    ing_shd_size, NULL, NULL, 0));
        }

        sal_memcpy(_BCM_TH_MMU_INFO(unit)->ing_shared_limit, ing_shd_size, 
            sizeof(ing_shd_size));
    } else {
        soc_mem_field32_set(unit, mem, entry, fld_limit, (arg/granularity));
        BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ALL, midx, entry));
    }

    return BCM_E_NONE;
}

/*
 * This function is used to set headroom pool
 * or shared pool for given Ingress [Port, Priority]
 */
STATIC int
_bcm_th_cosq_ing_pool_set(int unit, bcm_gport_t gport, bcm_cos_t pri,
                          bcm_cosq_control_t type, int arg)
{
    int local_port;
    uint32 rval;
    int pri_grp;
    static const soc_field_t prigroup_spid_field[] = {
        PG0_SPIDf, PG1_SPIDf, PG2_SPIDf, PG3_SPIDf,
        PG4_SPIDf, PG5_SPIDf, PG6_SPIDf, PG7_SPIDf
    };
    static const soc_field_t prigroup_hpid_field[] = {
        PG0_HPIDf, PG1_HPIDf, PG2_HPIDf, PG3_HPIDf,
        PG4_HPIDf, PG5_HPIDf, PG6_HPIDf, PG7_HPIDf
    };

    if((arg < 0) || (arg >= _TH_MMU_NUM_POOL)) {
            return BCM_E_PARAM;
    }

    if ((pri < 0) || (pri >= _TH_MMU_NUM_INT_PRI)) {
        /* Error. Input pri > Max Pri_Grp */
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_SCHEDULER(gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));

    if (!SOC_PORT_VALID(unit, local_port)) {
        return BCM_E_PORT;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_ingress_pg_get(unit, gport, pri, &pri_grp, &pri_grp));

    /* coverity fix : pri_grp can take value from 0 to 7 */
    if ((pri_grp < 0) || (pri_grp >= _TH_MMU_NUM_PG)) {
        return BCM_E_PARAM;
    }

    if (type == bcmCosqControlIngressPool) {
        SOC_IF_ERROR_RETURN(READ_THDI_PORT_PG_SPIDr(unit, local_port, &rval));
        soc_reg_field_set(unit, THDI_PORT_PG_SPIDr, &rval,
                          prigroup_spid_field[pri_grp],arg);
        BCM_IF_ERROR_RETURN(WRITE_THDI_PORT_PG_SPIDr(unit, local_port, rval));
    } else if (type == bcmCosqControlIngressHeadroomPool) {
        SOC_IF_ERROR_RETURN(READ_THDI_HDRM_PORT_PG_HPIDr(unit, local_port, &rval));
        soc_reg_field_set(unit, THDI_HDRM_PORT_PG_HPIDr, &rval,
                          prigroup_hpid_field[pri_grp], arg);
        BCM_IF_ERROR_RETURN(WRITE_THDI_HDRM_PORT_PG_HPIDr(unit, local_port, rval));
    } else {
        return BCM_E_PARAM;
    }
    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_ing_pool_get(int unit, bcm_gport_t gport,
                          bcm_cos_t pri,
                          bcm_cosq_control_t type, int *arg)
{
    int pool_start_idx = 0, pool_end_idx = 0;

    if (arg == NULL) {
        return BCM_E_PARAM;
    }

    if ((pri < 0) || (pri >= _TH_MMU_NUM_INT_PRI)) {
        /* Error. Input pri > Max Pri_Grp */
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_SCHEDULER(gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        return BCM_E_PARAM;
    }

    if (type == bcmCosqControlIngressPool) {
        BCM_IF_ERROR_RETURN(
            _bcm_th_cosq_ingress_sp_get(unit, gport, pri,
                                        &pool_start_idx,
                                        &pool_end_idx));
    } else if (type == bcmCosqControlIngressHeadroomPool) {
        BCM_IF_ERROR_RETURN(
            _bcm_th_cosq_ingress_hdrm_pool_get(unit, gport, pri,
                                               &pool_start_idx,
                                               &pool_end_idx));
    } else {
        return BCM_E_PARAM;
    }
    *arg = pool_start_idx;
    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_egr_pool_set(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq,
                          bcm_cosq_buffer_id_t buffer, 
                          bcm_cosq_control_t type, int arg)
{
    bcm_port_t local_port;
    int startq, pool, midx;
    uint32 max_val;
    uint32 entry[SOC_MAX_MEM_WORDS];
    uint32 entry2[SOC_MAX_MEM_WORDS];
    soc_mem_t base_mem = INVALIDm, base_mem2 = INVALIDm;
    soc_mem_t mem = INVALIDm, mem2 = INVALIDm;
    soc_field_t fld_limit = INVALIDf;
    int granularity = 1, pipe;

    if (arg < 0) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                    _BCM_TH_COSQ_INDEX_STYLE_EGR_POOL,
                                    &local_port, &pool, NULL));

    SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

	base_mem = MMU_THDM_DB_PORTSP_CONFIGm;
    mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];

    if (type == bcmCosqControlEgressPoolLimitEnable) {
        midx = SOC_TH_MMU_PIPED_MEM_INDEX(unit, local_port, base_mem, pool);
        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL,
                            midx, entry));

        soc_mem_field32_set(unit, mem, entry, SHARED_LIMIT_ENABLEf, !!arg);

        return soc_mem_write(unit, mem, MEM_BLOCK_ALL, midx, entry);
    }
    if (type == bcmCosqControlEgressPool || type == bcmCosqControlUCEgressPool ||
        type == bcmCosqControlMCEgressPool) {
        if((arg < 0) || (arg >= _TH_MMU_NUM_POOL)) {
            return BCM_E_PARAM;
        }
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            if (type != bcmCosqControlEgressPool) {
                return BCM_E_PARAM;
            }
            /* regular unicast queue */
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                     _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                     &local_port, &startq, NULL));
            base_mem = MMU_THDU_Q_TO_QGRP_MAPm;
            mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            if (type != bcmCosqControlEgressPool) {
                return BCM_E_PARAM;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                     _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                     &local_port, &startq, NULL));
            base_mem = MMU_THDM_MCQE_QUEUE_CONFIGm;
            mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
            base_mem2 = MMU_THDM_DB_QUEUE_CONFIGm;
            mem2 = SOC_MEM_UNIQUE_ACC(unit, base_mem2)[pipe];
        } else  { /* local port */
            if (type == bcmCosqControlUCEgressPool) {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                         _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                         &local_port, &startq, NULL));
                base_mem = MMU_THDU_Q_TO_QGRP_MAPm;
                mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
            } else {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                             _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                             &local_port, &startq, NULL));
                base_mem = MMU_THDM_MCQE_QUEUE_CONFIGm;
                mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
                base_mem2 = MMU_THDM_DB_QUEUE_CONFIGm;
                mem2 = SOC_MEM_UNIQUE_ACC(unit, base_mem2)[pipe];
            }
        }

        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
        soc_mem_field32_set(unit, mem, entry, Q_SPIDf, arg);

        BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ALL, startq, entry));
        if (mem2 != INVALIDm) {
            BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem2, MEM_BLOCK_ALL, startq, entry2));
            soc_mem_field32_set(unit, mem2, entry2, Q_SPIDf, arg);
            BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem2, MEM_BLOCK_ALL, startq, entry2));
        }
        return BCM_E_NONE;
    }

    arg = (arg + _TH_MMU_BYTES_PER_CELL - 1) / _TH_MMU_BYTES_PER_CELL;

    /* per service pool settings */
    if (type == bcmCosqControlEgressPoolSharedLimitBytes ||
        type == bcmCosqControlEgressPoolResumeLimitBytes ||
        type == bcmCosqControlEgressPoolYellowSharedLimitBytes ||
        type == bcmCosqControlEgressPoolYellowResumeLimitBytes ||
        type == bcmCosqControlEgressPoolRedSharedLimitBytes ||
        type == bcmCosqControlEgressPoolRedResumeLimitBytes) {
        int xpe, xpe_map;
        uint32 rval;
        soc_reg_t reg;
        switch (type) {
        case bcmCosqControlEgressPoolSharedLimitBytes:
            reg = MMU_THDM_DB_POOL_SHARED_LIMITr;
            fld_limit = SHARED_LIMITf;
            granularity = 1;
            break;
        case bcmCosqControlEgressPoolResumeLimitBytes:
            reg = MMU_THDM_DB_POOL_RESUME_LIMITr;
            fld_limit = RESUME_LIMITf;
            granularity = 8;
            break;
        case bcmCosqControlEgressPoolYellowSharedLimitBytes:
            reg = MMU_THDM_DB_POOL_YELLOW_SHARED_LIMITr;
            fld_limit = YELLOW_SHARED_LIMITf;
            granularity = 8;
            break;
        case bcmCosqControlEgressPoolYellowResumeLimitBytes:
            reg = MMU_THDM_DB_POOL_YELLOW_RESUME_LIMITr;
            fld_limit = YELLOW_RESUME_LIMITf;
            granularity = 8;
            break;
        case bcmCosqControlEgressPoolRedSharedLimitBytes:
            reg = MMU_THDM_DB_POOL_RED_SHARED_LIMITr;
            fld_limit = RED_SHARED_LIMITf;
            granularity = 8;
            break;
        case bcmCosqControlEgressPoolRedResumeLimitBytes:
            reg = MMU_THDM_DB_POOL_RED_RESUME_LIMITr;
            fld_limit = RED_RESUME_LIMITf;
            granularity = 8;
            break;
        /* coverity[dead_error_begin] */
        default:
            return BCM_E_UNAVAIL;
        }

        /* These regs are chip unique, buffer = -1 writes on all 4 xpes. 
           No need to do pipe-xpe checking */
        xpe_map = SOC_INFO(unit).ipipe_xpe_map[0] | 
                  SOC_INFO(unit).ipipe_xpe_map[1];
        if (buffer == BCM_COSQ_BUFFER_ID_INVALID) {
            xpe = -1;
        } else if (xpe_map & (1 << buffer)) {
            xpe = buffer;
        } else {
            return BCM_E_PARAM;
        }
        
        SOC_IF_ERROR_RETURN(soc_tomahawk_xpe_reg32_get
            (unit, reg, xpe, -1, pool, &rval));

        /* Check for min/max values */
        max_val = (1 << soc_reg_field_length(unit, reg, fld_limit)) - 1;
        if ((arg < 0) || ((arg/granularity) > max_val)) {
            return BCM_E_PARAM;
        } else {
            soc_reg_field_set(unit, reg, &rval, fld_limit, (arg/granularity));
        }

        SOC_IF_ERROR_RETURN(soc_tomahawk_xpe_reg32_set
            (unit, reg, xpe, -1, pool, rval));

        return BCM_E_NONE;
    }

    /* per port settings */
    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                         _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                         &local_port, &startq, NULL));
        base_mem = MMU_THDU_CONFIG_QUEUEm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        base_mem2 = MMU_THDU_Q_TO_QGRP_MAPm;
        mem2 = SOC_MEM_UNIQUE_ACC(unit, base_mem2)[pipe];
    } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                         _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                         &local_port, &startq, NULL));
        base_mem = MMU_THDM_DB_QUEUE_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
    } else {
        if (soc_property_get(unit, spn_PORT_UC_MC_ACCOUNTING_COMBINE, 0) == 0) {
            /* Need to have the UC_MC_Combine config enabled */
            return BCM_E_PARAM;
        }
        base_mem = MMU_THDM_DB_PORTSP_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        startq = SOC_TH_MMU_PIPED_MEM_INDEX(unit, local_port, base_mem, pool);
    }

    BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));

    if (mem2 != INVALIDm) {
        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem2, MEM_BLOCK_ALL, startq, entry2));
    }

    switch (type) {
        case bcmCosqControlEgressPoolLimitBytes:
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
                fld_limit = Q_SHARED_LIMIT_CELLf;

                if (mem2 != INVALIDm) {
                    soc_mem_field32_set(unit, mem2, entry2, Q_LIMIT_ENABLEf, 1);
                }
            } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                fld_limit = Q_SHARED_LIMITf;
                soc_mem_field32_set(unit, mem, entry, Q_LIMIT_ENABLEf, 1);
            } else {
                fld_limit = SHARED_LIMITf;
                soc_mem_field32_set(unit, mem, entry, SHARED_LIMIT_ENABLEf, 1);
            }
            granularity = 1;
            break;
        case bcmCosqControlEgressPoolYellowLimitBytes:
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
                fld_limit = LIMIT_YELLOW_CELLf;
            } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                fld_limit = YELLOW_SHARED_LIMITf;
            } else {
                fld_limit = YELLOW_SHARED_LIMITf;
            }
            granularity = 8;
            break;
        case bcmCosqControlEgressPoolRedLimitBytes:
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
                fld_limit = LIMIT_RED_CELLf;
            } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                fld_limit = RED_SHARED_LIMITf;
            } else {
                fld_limit = RED_SHARED_LIMITf;
            }
            granularity = 8;
            break;
        default:
            return BCM_E_UNAVAIL;
    }

    /* Check for min/max values */
    max_val = (1 << soc_mem_field_length(unit, mem, fld_limit)) - 1;
    if ((arg < 0) || ((arg/granularity) > max_val)) {
        return BCM_E_PARAM;
    } else {
        soc_mem_field32_set(unit, mem, entry, fld_limit, (arg/granularity));
    }
    BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ALL, startq, entry));

    if (mem2 != INVALIDm) {
        BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem2, MEM_BLOCK_ALL, startq, entry2));
    }
    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_egr_pool_get(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq,
                          bcm_cosq_buffer_id_t buffer, 
                          bcm_cosq_control_t type, int *arg)
{
    bcm_port_t local_port;
    int startq, pool, midx, pipe;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_mem_t mem, base_mem;
    int granularity = 1;

    if (!arg) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                    _BCM_TH_COSQ_INDEX_STYLE_EGR_POOL,
                                    &local_port, &pool, NULL));

    SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

    if (type == bcmCosqControlEgressPool || type == bcmCosqControlUCEgressPool ||
        type == bcmCosqControlMCEgressPool) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            if (type != bcmCosqControlEgressPool) {
                return BCM_E_PARAM;
            }
            /* regular unicast queue */
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                     _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                     &local_port, &startq, NULL));
            base_mem = MMU_THDU_Q_TO_QGRP_MAPm;
            mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            if (type != bcmCosqControlEgressPool) {
                return BCM_E_PARAM;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                     _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                     &local_port, &startq, NULL));
            base_mem = MMU_THDM_MCQE_QUEUE_CONFIGm;
            mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        } else  { /* local port */
            if (type == bcmCosqControlUCEgressPool) {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                         _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                         &local_port, &startq, NULL));
                base_mem = MMU_THDU_Q_TO_QGRP_MAPm;
                mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
            } else {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                             _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                             &local_port, &startq, NULL));
                base_mem = MMU_THDM_MCQE_QUEUE_CONFIGm;
                mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
            }
        }

        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
        *arg = soc_mem_field32_get(unit, mem, entry, Q_SPIDf);
        return BCM_E_NONE;
    }

    /* per service pool settings */
    if (type == bcmCosqControlEgressPoolSharedLimitBytes ||
        type == bcmCosqControlEgressPoolResumeLimitBytes ||
        type == bcmCosqControlEgressPoolYellowSharedLimitBytes ||
        type == bcmCosqControlEgressPoolYellowResumeLimitBytes ||
        type == bcmCosqControlEgressPoolRedSharedLimitBytes ||
        type == bcmCosqControlEgressPoolRedResumeLimitBytes) {
        int xpe, xpe_map;
        uint32 rval;
        soc_reg_t reg;
        soc_field_t fld_limit = INVALIDf;
        switch (type) {
        case bcmCosqControlEgressPoolSharedLimitBytes:
            reg = MMU_THDM_DB_POOL_SHARED_LIMITr;
            fld_limit = SHARED_LIMITf;
            granularity = 1;
            break;
        case bcmCosqControlEgressPoolResumeLimitBytes:
            reg = MMU_THDM_DB_POOL_RESUME_LIMITr;
            fld_limit = RESUME_LIMITf;
            granularity = 8;
            break;
        case bcmCosqControlEgressPoolYellowSharedLimitBytes:
            reg = MMU_THDM_DB_POOL_YELLOW_SHARED_LIMITr;
            fld_limit = YELLOW_SHARED_LIMITf;
            granularity = 8;
            break;
        case bcmCosqControlEgressPoolYellowResumeLimitBytes:
            reg = MMU_THDM_DB_POOL_YELLOW_RESUME_LIMITr;
            fld_limit = YELLOW_RESUME_LIMITf;
            granularity = 8;
            break;
        case bcmCosqControlEgressPoolRedSharedLimitBytes:
            reg = MMU_THDM_DB_POOL_RED_SHARED_LIMITr;
            fld_limit = RED_SHARED_LIMITf;
            granularity = 8;
            break;
        case bcmCosqControlEgressPoolRedResumeLimitBytes:
            reg = MMU_THDM_DB_POOL_RED_RESUME_LIMITr;
            fld_limit = RED_RESUME_LIMITf;
            granularity = 8;
            break;
        /* coverity[dead_error_begin] */
        default:
            return BCM_E_UNAVAIL;
        }

        /* These regs are chip unique, buffer = -1 writes on all 4 xpes. 
           No need to do pipe-xpe checking */
        xpe_map = SOC_INFO(unit).ipipe_xpe_map[0] | 
                  SOC_INFO(unit).ipipe_xpe_map[1];
        if (buffer == BCM_COSQ_BUFFER_ID_INVALID) {
            xpe_map = SOC_INFO(unit).epipe_xpe_map[pipe];
            /*find the first valid xpe*/
            for (xpe = 0; xpe < NUM_XPE(unit); xpe++) {
                if ((xpe_map & (1 << xpe))) {
                    break;
                }
            }
            if(xpe == NUM_XPE(unit)) {
                return BCM_E_INTERNAL;
            }
        } else if (xpe_map & (1 << buffer)) {
            xpe = buffer;
        } else {
            return BCM_E_PARAM;
        }
        
        SOC_IF_ERROR_RETURN(soc_tomahawk_xpe_reg32_get
            (unit, reg, xpe, -1, pool, &rval));

        *arg = soc_reg_field_get(unit, reg, rval, fld_limit);
        *arg = (*arg) * granularity * _TH_MMU_BYTES_PER_CELL;

        return BCM_E_NONE;
    }

    base_mem = MMU_THDM_DB_PORTSP_CONFIGm;
    mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];

    if (type == bcmCosqControlEgressPoolLimitEnable) {
        midx = SOC_TH_MMU_PIPED_MEM_INDEX(unit, local_port, base_mem, pool);
        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL,
                                 midx, entry));

        *arg = soc_mem_field32_get(unit, mem, entry, SHARED_LIMIT_ENABLEf);
        return BCM_E_NONE;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                        _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                        &local_port, &startq, NULL));
        base_mem = MMU_THDU_CONFIG_QUEUEm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
    } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                        _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                        &local_port, &startq, NULL));

        base_mem = MMU_THDM_DB_QUEUE_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
    } else {
        if (soc_property_get(unit, spn_PORT_UC_MC_ACCOUNTING_COMBINE, 0) == 0) {
            /* Need to have the UC_MC_Combine config enabled */
            return BCM_E_PARAM;
        }
        base_mem = MMU_THDM_DB_PORTSP_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        startq = SOC_TH_MMU_PIPED_MEM_INDEX(unit, local_port, base_mem, pool);
    }
    BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL,
                             startq, entry));

    switch (type) {
        case bcmCosqControlEgressPoolLimitBytes:
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
                *arg = soc_mem_field32_get(unit, mem, entry, Q_SHARED_LIMIT_CELLf);
            } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                *arg = soc_mem_field32_get(unit, mem, entry, Q_SHARED_LIMITf);
            } else {
                *arg = soc_mem_field32_get(unit, mem, entry, SHARED_LIMITf);
            }
            granularity = 1;
            break;
        case bcmCosqControlEgressPoolYellowLimitBytes:
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
                *arg = soc_mem_field32_get(unit, mem, entry, LIMIT_YELLOW_CELLf);
            } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                *arg = soc_mem_field32_get(unit, mem, entry, YELLOW_SHARED_LIMITf);
            } else {
                *arg = soc_mem_field32_get(unit, mem, entry, YELLOW_SHARED_LIMITf);
            }
            granularity = 8;
            break;
        case bcmCosqControlEgressPoolRedLimitBytes:
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
                *arg = soc_mem_field32_get(unit, mem, entry, LIMIT_RED_CELLf);
            } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                *arg = soc_mem_field32_get(unit, mem, entry, RED_SHARED_LIMITf);
            } else {
                *arg = soc_mem_field32_get(unit, mem, entry, RED_SHARED_LIMITf);
            }
            granularity = 8;
            break;
        default:
            return BCM_E_UNAVAIL;
    }
    *arg = (*arg) * granularity * _TH_MMU_BYTES_PER_CELL;
    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_egr_port_pool_set(int unit, bcm_gport_t gport,
                               bcm_cos_queue_t cosq,
                               bcm_cosq_control_t type, int arg)
{
    bcm_port_t local_port;
    uint32 max_val;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_mem_t mem = INVALIDm, base_mem = INVALIDm;
    soc_field_t fld_limit = INVALIDf;
    int granularity = 1;
    int pool, midx, pipe;

    if (arg < 0) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                    _BCM_TH_COSQ_INDEX_STYLE_EGR_POOL,
                                    &local_port, &pool, NULL));
    SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

    if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        base_mem = MMU_THDM_DB_PORTSP_CONFIGm;
    } else {
        base_mem = MMU_THDU_CONFIG_PORTm;
    }
    mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
    midx = SOC_TH_MMU_PIPED_MEM_INDEX(unit, local_port, base_mem, pool);
    BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, midx, entry));

    switch (type) {
        case bcmCosqControlEgressPortPoolYellowLimitBytes:
            if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                fld_limit = YELLOW_SHARED_LIMITf;
            } else {
                fld_limit = YELLOW_LIMITf;
            }
            granularity = 8;
            break;
        case bcmCosqControlEgressPortPoolRedLimitBytes:
            if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                fld_limit = RED_SHARED_LIMITf;
            } else {
                fld_limit = RED_LIMITf;
            }
            granularity = 8;
            break;
        default:
            return BCM_E_UNAVAIL;
    }

    arg = (arg + _TH_MMU_BYTES_PER_CELL - 1) / _TH_MMU_BYTES_PER_CELL;

    /* Check for min/max values */
    max_val = (1 << soc_mem_field_length(unit, mem, fld_limit)) - 1;
    if ((arg < 0) || ((arg/granularity) > max_val)) {
        return BCM_E_PARAM;
    } else {
        soc_mem_field32_set(unit, mem, entry, fld_limit, (arg/granularity));
    }
    BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ALL, midx, entry));

    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_egr_port_pool_get(int unit, bcm_gport_t gport,
                               bcm_cos_queue_t cosq,
                               bcm_cosq_control_t type, int *arg)
{
    bcm_port_t local_port;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_mem_t mem = INVALIDm, base_mem = INVALIDm;
    int granularity = 1;
    int pool, midx, pipe;

    if (!arg) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                    _BCM_TH_COSQ_INDEX_STYLE_EGR_POOL,
                                    &local_port, &pool, NULL));
    SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

    if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        base_mem = MMU_THDM_DB_PORTSP_CONFIGm;
    } else {
        base_mem = MMU_THDU_CONFIG_PORTm;
    }
    mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
    midx = SOC_TH_MMU_PIPED_MEM_INDEX(unit, local_port, base_mem, pool);
    BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, midx, entry));

    switch (type) {
        case bcmCosqControlEgressPortPoolYellowLimitBytes:
            if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                *arg = soc_mem_field32_get(unit, mem, entry, YELLOW_SHARED_LIMITf);
            } else {
                *arg = soc_mem_field32_get(unit, mem, entry, YELLOW_LIMITf);
            }
            granularity = 8;
            break;
        case bcmCosqControlEgressPortPoolRedLimitBytes:
            if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                *arg = soc_mem_field32_get(unit, mem, entry, RED_SHARED_LIMITf);
            } else {
                *arg = soc_mem_field32_get(unit, mem, entry, RED_LIMITf);
            }
            granularity = 8;
            break;
        default:
            return BCM_E_UNAVAIL;
    }

    *arg = (*arg) * granularity * _TH_MMU_BYTES_PER_CELL;
    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_dynamic_thresh_enable_get(int unit,
                        bcm_gport_t gport, bcm_cos_queue_t cosq,
                        bcm_cosq_control_t type, int *arg)
{
    bcm_port_t local_port;
    int startq, pipe, from_cos, to_cos, ci, midx, port_pg;
    uint32 rval;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_mem_t mem = INVALIDm, base_mem = INVALIDm;
    soc_reg_t reg = INVALIDr;
    static const soc_reg_t prigroup_reg[] = {
        THDI_PORT_PRI_GRP0r, THDI_PORT_PRI_GRP1r
    };
    static const soc_field_t prigroup_field[] = {
        PRI0_GRPf, PRI1_GRPf, PRI2_GRPf, PRI3_GRPf,
        PRI4_GRPf, PRI5_GRPf, PRI6_GRPf, PRI7_GRPf,
        PRI8_GRPf, PRI9_GRPf, PRI10_GRPf, PRI11_GRPf,
        PRI12_GRPf, PRI13_GRPf, PRI14_GRPf, PRI15_GRPf
    };

    if (!arg) {
        return BCM_E_PARAM;
    }

    if (type == bcmCosqControlIngressPortPGSharedDynamicEnable) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
            BCM_GPORT_IS_SCHEDULER(gport) ||
            BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            return BCM_E_PARAM;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
        if (local_port < 0) {
            return BCM_E_PORT;
        }
        if ((cosq < 0) || (cosq >= 16)) {
            return BCM_E_PARAM;
        }
        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

        if ((cosq < 0) || (cosq >= _TH_MMU_NUM_INT_PRI)) {
            /* Error. Input pri > Max Pri_Grp */
            /* coverity[dead_error_line] */
            return BCM_E_PARAM;
        }

        /* get PG for port using Port+Cos */
        if (cosq < _TH_MMU_NUM_PG) {
            reg = prigroup_reg[0];
        } else {
            reg = prigroup_reg[1];
        }

        SOC_IF_ERROR_RETURN
            (soc_reg32_get(unit, reg, local_port, 0, &rval));
        port_pg = soc_reg_field_get(unit, reg, rval, prigroup_field[cosq]);
        base_mem = THDI_PORT_PG_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        /* get index for Port-PG */
        midx = SOC_TH_MMU_PIPED_MEM_INDEX(unit, local_port, base_mem, port_pg);

        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL,
                                 midx, entry));
        *arg = soc_mem_field32_get(unit, mem, entry, PG_SHARED_DYNAMICf);

    } else if (type == bcmCosqControlEgressUCSharedDynamicEnable) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                         _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                         &local_port, &startq, NULL));
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            return BCM_E_PARAM;
        } else {
            if (cosq == BCM_COS_INVALID) {
                from_cos = to_cos = 0;
            } else {
                from_cos = to_cos = cosq;
            }

            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
            if (local_port < 0) {
                return BCM_E_PORT;
            }
            for (ci = from_cos; ci <= to_cos; ci++) {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_index_resolve(unit, local_port, ci,
                          _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                          NULL, &startq, NULL));
            }
        }
        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
        base_mem = MMU_THDU_CONFIG_QUEUEm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];

        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL,
                                 startq, entry));
        *arg = soc_mem_field32_get(unit, mem, entry, Q_LIMIT_DYNAMIC_CELLf);
    } else if (type == bcmCosqControlEgressMCSharedDynamicEnable) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            return BCM_E_PARAM;
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                         _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                         &local_port, &startq, NULL));
        }
        else {
            if (cosq == BCM_COS_INVALID) {
                from_cos = to_cos = 0;
            } else {
                from_cos = to_cos = cosq;
            }

            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
            if (local_port < 0) {
                return BCM_E_PORT;
            }
            for (ci = from_cos; ci <= to_cos; ci++) {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_index_resolve(unit, local_port, ci,
                                         _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                         NULL, &startq, NULL));
            }
        }
        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
        base_mem = MMU_THDM_DB_QUEUE_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];

        BCM_IF_ERROR_RETURN
            (soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
        *arg = soc_mem_field32_get(unit, mem, entry, Q_LIMIT_DYNAMICf);
    } else {
        return BCM_E_PARAM;
    }

    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_alpha_set(int unit,
                       bcm_gport_t gport, bcm_cos_queue_t cosq,
                       bcm_cosq_control_drop_limit_alpha_value_t arg)
{
    bcm_port_t local_port;
    int startq, midx, pipe;
    uint32 rval;
    uint32 entry[SOC_MAX_MEM_WORDS];
    uint32 entry2[SOC_MAX_MEM_WORDS];
    soc_mem_t base_mem = INVALIDm, base_mem2 = INVALIDm;
    soc_mem_t mem = INVALIDm, mem2 = INVALIDm;
    int alpha, dynamic_thresh_mode, port_pg;
    soc_reg_t reg = INVALIDr;
    static const soc_reg_t prigroup_reg[] = {
        THDI_PORT_PRI_GRP0r, THDI_PORT_PRI_GRP1r
    };
    static const soc_field_t prigroup_field[] = {
        PRI0_GRPf, PRI1_GRPf, PRI2_GRPf, PRI3_GRPf,
        PRI4_GRPf, PRI5_GRPf, PRI6_GRPf, PRI7_GRPf,
        PRI8_GRPf, PRI9_GRPf, PRI10_GRPf, PRI11_GRPf,
        PRI12_GRPf, PRI13_GRPf, PRI14_GRPf, PRI15_GRPf
    };
    int phy_port, mmu_port, local_mmu_port;

    soc_info_t *si;
    si = &SOC_INFO(unit);

    switch(arg) {
    case bcmCosqControlDropLimitAlpha_1_128:
        alpha = SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_128;
        break;
    case bcmCosqControlDropLimitAlpha_1_64:
        alpha = SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_64;
        break;
    case bcmCosqControlDropLimitAlpha_1_32:
        alpha = SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_32;
        break;
    case bcmCosqControlDropLimitAlpha_1_16:
        alpha = SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_16;
        break;
    case bcmCosqControlDropLimitAlpha_1_8:
        alpha = SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_8;
        break;
    case bcmCosqControlDropLimitAlpha_1_4:
        alpha = SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_4;
        break;
    case bcmCosqControlDropLimitAlpha_1_2:
        alpha = SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_2;
        break;
    case bcmCosqControlDropLimitAlpha_1:
        alpha = SOC_TH_COSQ_DROP_LIMIT_ALPHA_1;
        break;
    case bcmCosqControlDropLimitAlpha_2:
        alpha = SOC_TH_COSQ_DROP_LIMIT_ALPHA_2;
        break;
    case bcmCosqControlDropLimitAlpha_4:
        alpha = SOC_TH_COSQ_DROP_LIMIT_ALPHA_4;
        break;
    case bcmCosqControlDropLimitAlpha_8:
        alpha = SOC_TH_COSQ_DROP_LIMIT_ALPHA_8;
        break;
    default:
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_dynamic_thresh_enable_get(unit, gport, cosq,
                                   bcmCosqControlEgressUCSharedDynamicEnable,
                                   &dynamic_thresh_mode));
        if (!dynamic_thresh_mode){
            return BCM_E_CONFIG;
        }

        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                     _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                     &local_port, &startq, NULL));
        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

        base_mem = MMU_THDU_CONFIG_QUEUEm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];

        BCM_IF_ERROR_RETURN
            (soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
        soc_mem_field32_set(unit, mem, entry,
                            Q_SHARED_ALPHA_CELLf, alpha);
        SOC_IF_ERROR_RETURN
            (soc_mem_write(unit, mem, MEM_BLOCK_ALL, startq, entry));
    } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_dynamic_thresh_enable_get(unit, gport, cosq,
                                   bcmCosqControlEgressMCSharedDynamicEnable,
                                   &dynamic_thresh_mode));
        if (!dynamic_thresh_mode){
            return BCM_E_CONFIG;
        }

        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                     _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                     &local_port, &startq, NULL));
        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

        base_mem = MMU_THDM_DB_QUEUE_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];

        base_mem2 = MMU_THDM_MCQE_QUEUE_CONFIGm;
        mem2 = SOC_MEM_UNIQUE_ACC(unit, base_mem2)[pipe];

        BCM_IF_ERROR_RETURN
            (soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
        soc_mem_field32_set(unit, mem, entry, Q_SHARED_ALPHAf, alpha);
        SOC_IF_ERROR_RETURN
            (soc_mem_write(unit, mem, MEM_BLOCK_ALL, startq, entry));

        BCM_IF_ERROR_RETURN
            (soc_mem_read(unit, mem2, MEM_BLOCK_ALL, startq, entry2));
        soc_mem_field32_set(unit, mem2, entry2, Q_SHARED_ALPHAf, alpha);
        SOC_IF_ERROR_RETURN
            (soc_mem_write(unit, mem2, MEM_BLOCK_ALL, startq, entry2));
    } else if (BCM_GPORT_IS_LOCAL(gport)) {
        /*Set the alpha for Qgroup associated with the gport*/
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_dynamic_thresh_enable_get(unit, gport, cosq,
                                   bcmCosqControlEgressUCQueueGroupSharedDynamicEnable,
                                   &dynamic_thresh_mode));
        if (!dynamic_thresh_mode){
            return BCM_E_CONFIG;
        }

        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

        phy_port = si->port_l2p_mapping[local_port];
        mmu_port = si->port_p2m_mapping[phy_port];
        local_mmu_port = mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1);

        base_mem =  MMU_THDU_CONFIG_QGROUPm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];

        BCM_IF_ERROR_RETURN
            (soc_mem_read(unit, mem, MEM_BLOCK_ALL,local_mmu_port , entry));
        soc_mem_field32_set(unit, mem, entry,
                            Q_SHARED_ALPHA_CELLf, alpha);
        SOC_IF_ERROR_RETURN
            (soc_mem_write(unit, mem, MEM_BLOCK_ALL,local_mmu_port , entry));
    } else {
        BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_dynamic_thresh_enable_get(unit, gport, cosq,
                                   bcmCosqControlIngressPortPGSharedDynamicEnable,
                                   &dynamic_thresh_mode));
        if (!dynamic_thresh_mode){
            return BCM_E_CONFIG;
        }

        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
        if (local_port < 0) {
            return BCM_E_PORT;
        }
        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

        if ((cosq < 0) || (cosq >= _TH_MMU_NUM_INT_PRI)) {
            /* Error. Input pri > Max Pri_Grp */
            return BCM_E_PARAM;
        }

        /* get PG for port using Port+Cos */
        if (cosq < _TH_MMU_NUM_PG) {
            reg = prigroup_reg[0];
        } else {
            reg = prigroup_reg[1];
        }

        SOC_IF_ERROR_RETURN
            (soc_reg32_get(unit, reg, local_port, 0, &rval));
        port_pg = soc_reg_field_get(unit, reg, rval, prigroup_field[cosq]);
        base_mem = THDI_PORT_PG_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        /* get index for Port-PG */
        midx = SOC_TH_MMU_PIPED_MEM_INDEX(unit, local_port, base_mem, port_pg);
        BCM_IF_ERROR_RETURN
            (soc_mem_read(unit, mem, MEM_BLOCK_ALL, midx, entry));
        soc_mem_field32_set(unit, mem, entry, PG_SHARED_LIMITf, alpha);
        SOC_IF_ERROR_RETURN
            (soc_mem_write(unit, mem, MEM_BLOCK_ALL, midx, entry));
    }

    return BCM_E_NONE;
}


STATIC int
_bcm_th_cosq_alpha_get(int unit,
                        bcm_gport_t gport, bcm_cos_queue_t cosq,
                        bcm_cosq_control_drop_limit_alpha_value_t *arg)
{
    bcm_port_t local_port;
    int startq, midx, pipe;
    uint32 rval;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_mem_t mem = INVALIDm, base_mem = INVALIDm;
    int alpha, dynamic_thresh_mode, port_pg;
    soc_reg_t reg = INVALIDr;
    static const soc_reg_t prigroup_reg[] = {
        THDI_PORT_PRI_GRP0r, THDI_PORT_PRI_GRP1r
    };
    static const soc_field_t prigroup_field[] = {
        PRI0_GRPf, PRI1_GRPf, PRI2_GRPf, PRI3_GRPf,
        PRI4_GRPf, PRI5_GRPf, PRI6_GRPf, PRI7_GRPf,
        PRI8_GRPf, PRI9_GRPf, PRI10_GRPf, PRI11_GRPf,
        PRI12_GRPf, PRI13_GRPf, PRI14_GRPf, PRI15_GRPf
    };

    if (!arg) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_dynamic_thresh_enable_get(unit, gport, cosq,
                                   bcmCosqControlEgressUCSharedDynamicEnable,
                                   &dynamic_thresh_mode));
        if (!dynamic_thresh_mode){
            return BCM_E_CONFIG;
        }

        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                     _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                     &local_port, &startq, NULL));
        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

        base_mem = MMU_THDU_CONFIG_QUEUEm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];

        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL,
                                 startq, entry));
        alpha = soc_mem_field32_get(unit, mem, entry, Q_SHARED_ALPHA_CELLf);

    } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_dynamic_thresh_enable_get(unit, gport, cosq,
                                   bcmCosqControlEgressMCSharedDynamicEnable,
                                   &dynamic_thresh_mode));
        if (!dynamic_thresh_mode){
            return BCM_E_CONFIG;
        }

        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                     _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                     &local_port, &startq, NULL));
        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

        base_mem = MMU_THDM_DB_QUEUE_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        BCM_IF_ERROR_RETURN
            (soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
        alpha = soc_mem_field32_get(unit, mem, entry, Q_SHARED_ALPHAf);

    } else {
        BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_dynamic_thresh_enable_get(unit, gport, cosq,
                                   bcmCosqControlIngressPortPGSharedDynamicEnable,
                                   &dynamic_thresh_mode));
        if (!dynamic_thresh_mode){
            return BCM_E_CONFIG;
        }

        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
        if (local_port < 0) {
            return BCM_E_PORT;
        }

        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

        if ((cosq < 0) || (cosq >= _TH_MMU_NUM_INT_PRI)) {
            /* Error. Input pri > Max Pri_Grp */
            return BCM_E_PARAM;
        }

        /* get PG for port using Port+Cos */
        if (cosq < _TH_MMU_NUM_PG) {
            reg = prigroup_reg[0];
        } else {
            reg = prigroup_reg[1];
        }

        SOC_IF_ERROR_RETURN
            (soc_reg32_get(unit, reg, local_port, 0, &rval));
        port_pg = soc_reg_field_get(unit, reg, rval, prigroup_field[cosq]);

        base_mem = THDI_PORT_PG_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        /* get index for Port-PG */
        midx = SOC_TH_MMU_PIPED_MEM_INDEX(unit, local_port, base_mem, port_pg);

        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL,
                                 midx, entry));
        alpha = soc_mem_field32_get(unit, mem, entry, PG_SHARED_LIMITf);

    }

    switch(alpha) {
    case SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_128:
        *arg = bcmCosqControlDropLimitAlpha_1_128;
        break;
    case SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_64:
        *arg = bcmCosqControlDropLimitAlpha_1_64;
        break;
    case SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_32:
        *arg = bcmCosqControlDropLimitAlpha_1_32;
        break;
    case SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_16:
        *arg = bcmCosqControlDropLimitAlpha_1_16;
        break;
    case SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_8:
        *arg = bcmCosqControlDropLimitAlpha_1_8;
        break;
    case SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_4:
        *arg = bcmCosqControlDropLimitAlpha_1_4;
        break;
    case SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_2:
        *arg = bcmCosqControlDropLimitAlpha_1_2;
        break;
    case SOC_TH_COSQ_DROP_LIMIT_ALPHA_1:
        *arg = bcmCosqControlDropLimitAlpha_1;
        break;
    case SOC_TH_COSQ_DROP_LIMIT_ALPHA_2:
        *arg = bcmCosqControlDropLimitAlpha_2;
        break;
    case SOC_TH_COSQ_DROP_LIMIT_ALPHA_4:
        *arg = bcmCosqControlDropLimitAlpha_4;
        break;
    case SOC_TH_COSQ_DROP_LIMIT_ALPHA_8:
        *arg = bcmCosqControlDropLimitAlpha_8;
        break;
    default:
        return BCM_E_PARAM;
    }

    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_dynamic_thresh_enable_set(int unit,
                        bcm_gport_t gport, bcm_cos_queue_t cosq,
                        bcm_cosq_control_t type, int arg)
{
    bcm_port_t local_port;
    int startq, pipe;
    int from_cos, to_cos, ci;
    int midx;
    uint32 rval;
    uint32 entry[SOC_MAX_MEM_WORDS];
    uint32 entry2[SOC_MAX_MEM_WORDS];
    soc_mem_t base_mem = INVALIDm, base_mem2 = INVALIDm;
    soc_mem_t mem = INVALIDm, mem2 = INVALIDm;
    soc_reg_t reg = INVALIDr;
    int port_pg;
    static const soc_reg_t prigroup_reg[] = {
        THDI_PORT_PRI_GRP0r, THDI_PORT_PRI_GRP1r
    };
    static const soc_field_t prigroup_field[] = {
        PRI0_GRPf, PRI1_GRPf, PRI2_GRPf, PRI3_GRPf,
        PRI4_GRPf, PRI5_GRPf, PRI6_GRPf, PRI7_GRPf,
        PRI8_GRPf, PRI9_GRPf, PRI10_GRPf, PRI11_GRPf,
        PRI12_GRPf, PRI13_GRPf, PRI14_GRPf, PRI15_GRPf
    };

    if (type == bcmCosqControlIngressPortPGSharedDynamicEnable) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
            BCM_GPORT_IS_SCHEDULER(gport) ||
            BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            return BCM_E_PARAM;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
        if (local_port < 0) {
            return BCM_E_PORT;
        }
        if ((cosq < 0) || (cosq >= 16)) {
            return BCM_E_PARAM;
        }
        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

        if ((cosq < 0) || (cosq >= _TH_MMU_NUM_INT_PRI)) {
            /* Error. Input pri > Max Pri_Grp */
            /* coverity[dead_error_line] */
            return BCM_E_PARAM;
        }

        /* get PG for port using Port+Cos */
        if (cosq < _TH_MMU_NUM_PG) {
            reg = prigroup_reg[0];
        } else {
            reg = prigroup_reg[1];
        }

        SOC_IF_ERROR_RETURN
            (soc_reg32_get(unit, reg, local_port, 0, &rval));
        port_pg = soc_reg_field_get(unit, reg, rval, prigroup_field[cosq]);
        base_mem = THDI_PORT_PG_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        /* get index for Port-PG */
        midx = SOC_TH_MMU_PIPED_MEM_INDEX(unit, local_port, base_mem, port_pg);
        BCM_IF_ERROR_RETURN
            (soc_mem_read(unit, mem, MEM_BLOCK_ALL, midx, entry));
        soc_mem_field32_set(unit, mem, entry,
                            PG_SHARED_DYNAMICf, arg ? 1 : 0);
        if(arg) {
            /* Set default alpha value*/
            soc_mem_field32_set(unit, mem, entry, PG_SHARED_LIMITf,
                                SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_128);
        }
        SOC_IF_ERROR_RETURN
            (soc_mem_write(unit, mem, MEM_BLOCK_ALL, midx, entry));
    } else if (type == bcmCosqControlEgressUCSharedDynamicEnable) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                            _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                            &local_port, &startq, NULL));
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            return BCM_E_PARAM;
        } else {
            if (cosq == BCM_COS_INVALID) {
                from_cos = to_cos = 0;
            } else {
                from_cos = to_cos = cosq;
            }

            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
            if (local_port < 0) {
                return BCM_E_PORT;
            }
            for (ci = from_cos; ci <= to_cos; ci++) {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_index_resolve(unit, local_port, ci,
                                         _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                         NULL, &startq, NULL));

            }
        }
        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
        base_mem = MMU_THDU_CONFIG_QUEUEm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];

        BCM_IF_ERROR_RETURN
            (soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
        soc_mem_field32_set(unit, mem, entry,
                            Q_LIMIT_DYNAMIC_CELLf, arg ? 1 : 0);
        if(arg) {
            /* Set default alpha value*/
            soc_mem_field32_set(unit, mem, entry, Q_SHARED_ALPHA_CELLf,
                                SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_128);
        }
        SOC_IF_ERROR_RETURN
            (soc_mem_write(unit, mem, MEM_BLOCK_ALL, startq, entry));
    } else if (type == bcmCosqControlEgressMCSharedDynamicEnable) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            return BCM_E_PARAM;
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                      _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                      &local_port, &startq, NULL));
        }
        else {
            if (cosq == BCM_COS_INVALID) {
                from_cos = to_cos = 0;
            } else {
                from_cos = to_cos = cosq;
            }

            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
            if (local_port < 0) {
                return BCM_E_PORT;
            }
            for (ci = from_cos; ci <= to_cos; ci++) {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_index_resolve(unit, local_port, ci,
                                         _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                         NULL, &startq, NULL));

            }
        }

        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
        base_mem = MMU_THDM_DB_QUEUE_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];

        base_mem2 = MMU_THDM_MCQE_QUEUE_CONFIGm;
        mem2 = SOC_MEM_UNIQUE_ACC(unit, base_mem2)[pipe];

        BCM_IF_ERROR_RETURN
            (soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
        soc_mem_field32_set(unit, mem, entry, Q_LIMIT_DYNAMICf, arg ? 1 : 0);
        if(arg) {
            /* Set default alpha value*/
            soc_mem_field32_set(unit, mem, entry, Q_SHARED_ALPHAf,
                                SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_128);
        }
        SOC_IF_ERROR_RETURN
            (soc_mem_write(unit, mem, MEM_BLOCK_ALL, startq, entry));

        BCM_IF_ERROR_RETURN
            (soc_mem_read(unit, mem2, MEM_BLOCK_ALL, startq, entry2));
        soc_mem_field32_set(unit, mem2, entry2, Q_LIMIT_DYNAMICf, arg ? 1 : 0);
        if(arg) {
            /* Set default alpha value*/
            soc_mem_field32_set(unit, mem2, entry2, Q_SHARED_ALPHAf,
                                SOC_TH_COSQ_DROP_LIMIT_ALPHA_1_128);
        }
        SOC_IF_ERROR_RETURN
            (soc_mem_write(unit, mem2, MEM_BLOCK_ALL, startq, entry2));

    } else {
        return BCM_E_PARAM;
    }

    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_egr_queue_limit_enable_set(int unit, bcm_gport_t gport,
                                        bcm_cos_queue_t cosq,
                                        bcm_cosq_control_t type,
                                        int arg)
{
    bcm_port_t local_port;
    int startq, pipe;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_mem_t mem = INVALIDm, base_mem = INVALIDm;
    int enable;

    if (arg < 0) {
        return BCM_E_PARAM;
    }

    arg = arg ? 1 : 0;

    if (type == bcmCosqControlEgressUCQueueLimitEnable) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                            _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                            &local_port, &startq, NULL));
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            return BCM_E_PARAM;
        } else {
            if (cosq < 0) {
                return BCM_E_PARAM;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
            if (local_port < 0) {
                return BCM_E_PORT;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve
                 (unit, local_port, cosq, _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                  NULL, &startq, NULL));
        }

        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

        base_mem = MMU_THDU_Q_TO_QGRP_MAPm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
        enable = soc_mem_field32_get(unit, mem, entry, Q_LIMIT_ENABLEf);
        if (enable != arg) {
            soc_mem_field32_set(unit, mem, entry, Q_LIMIT_ENABLEf, arg);
            BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ALL, startq, entry));
        }
    } else if (type == bcmCosqControlEgressMCQueueLimitEnable) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            return BCM_E_PARAM;
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                            _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                            &local_port, &startq, NULL));
        } else {
            if (cosq < 0) {
                return BCM_E_PARAM;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
            if (local_port < 0) {
                return BCM_E_PORT;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, local_port, cosq,
                                            _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                            NULL, &startq, NULL));
        }

        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

        base_mem = MMU_THDM_MCQE_QUEUE_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
        enable = soc_mem_field32_get(unit, mem, entry, Q_LIMIT_ENABLEf);
        if (enable != arg) {
            soc_mem_field32_set(unit, mem, entry, Q_LIMIT_ENABLEf, arg);
            BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ALL, startq, entry));
        }

        base_mem = MMU_THDM_DB_QUEUE_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
        enable = soc_mem_field32_get(unit, mem, entry, Q_LIMIT_ENABLEf);
        if (enable != arg) {
            soc_mem_field32_set(unit, mem, entry, Q_LIMIT_ENABLEf, arg);
            BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ALL, startq, entry));
        }
    } else {
        return BCM_E_PARAM;
    }

    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_egr_queue_limit_enable_get(int unit, bcm_gport_t gport,
                                         bcm_cos_queue_t cosq,
                                         bcm_cosq_control_t type,
                                         int *arg)
{
    bcm_port_t local_port;
    int startq, pipe;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_mem_t mem = INVALIDm, base_mem = INVALIDm;

    if (type == bcmCosqControlEgressUCQueueLimitEnable) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                          _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                          &local_port, &startq, NULL));
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            return BCM_E_PARAM;
        } else {
            if (cosq < 0) {
                return BCM_E_PARAM;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
            if (local_port < 0) {
                return BCM_E_PORT;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve
                    (unit, local_port, cosq, _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                    NULL, &startq, NULL));
        }

        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

        base_mem = MMU_THDU_Q_TO_QGRP_MAPm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
        *arg = soc_mem_field32_get(unit, mem, entry, Q_LIMIT_ENABLEf);
    } else if (type == bcmCosqControlEgressMCQueueLimitEnable) {
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            return BCM_E_PARAM;
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                     _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                     &local_port, &startq, NULL));
        } else {
            if (cosq < 0) {
                return BCM_E_PARAM;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
            if (local_port < 0) {
                return BCM_E_PORT;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_index_resolve(unit, local_port, cosq,
                                    _BCM_TH_COSQ_INDEX_STYLE_MCAST_QUEUE,
                                     NULL, &startq, NULL));
        }
        SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

        base_mem = MMU_THDM_MCQE_QUEUE_CONFIGm;
        mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];

        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
        *arg = soc_mem_field32_get(unit, mem, entry, Q_LIMIT_ENABLEf);
    } else {
        return BCM_E_PARAM;
    }
    return BCM_E_NONE;
}

/*
 * This function is used to get ingress pool
 * limit given Ingress [Port, Priority]
 */
STATIC int
_bcm_th_cosq_ing_res_limit_get(int unit, bcm_gport_t gport, bcm_cos_t cosq,
                               bcm_cosq_buffer_id_t buffer, 
                               bcm_cosq_control_t type, int *arg)
{
    uint32 rval = 0x0;
    int pool;
    int local_port, pipe, layer, xpe;
    soc_reg_t reg = INVALIDr;

    if (arg == NULL) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_SCHEDULER(gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));

    if (!SOC_PORT_VALID(unit, local_port)) {
        return BCM_E_PORT;
    }

    if (type == bcmCosqControlIngressPoolLimitBytes) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_ing_pool_get(unit, gport, cosq,
                                       bcmCosqControlIngressPool,
                                       &pool));
        reg = THDI_BUFFER_CELL_LIMIT_SPr;
    } else if (type == bcmCosqControlIngressHeadroomPoolLimitBytes) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_ing_pool_get(unit, gport, cosq,
                                       bcmCosqControlIngressHeadroomPool,
                                       &pool));
        reg = THDI_HDRM_BUFFER_CELL_LIMIT_HPr;
    } else {
        return BCM_E_PARAM;
    }

    SOC_IF_ERROR_RETURN(
        soc_port_pipe_get(unit, local_port, &pipe));
    SOC_IF_ERROR_RETURN(
        soc_tomahawk_mmu_layer_get(unit, pipe, &layer));
    
    if (buffer == BCM_COSQ_BUFFER_ID_INVALID) {
        xpe = -1;
    } else if (SOC_INFO(unit).ipipe_xpe_map[pipe] & (1 << buffer)) {
        xpe = buffer;
    } else {
        return BCM_E_PARAM;
    }
    
    SOC_IF_ERROR_RETURN(
        soc_tomahawk_xpe_reg32_get(unit,reg,
                                   xpe, layer, pool, &rval));
    *arg = soc_reg_field_get(unit, reg, rval, LIMITf);

    *arg *= _TH_MMU_BYTES_PER_CELL;

    return BCM_E_NONE;
}

/*
 * This function is used to set ingress pool
 * limit given Ingress [Port, Priority]
 */
STATIC int
_bcm_th_cosq_ing_res_limit_set(int unit, bcm_gport_t gport, bcm_cos_t cosq,
                               bcm_cosq_buffer_id_t buffer,
                               bcm_cosq_control_t type, int arg)
{
    uint32 rval = 0x0;
    int pool;
    int local_port, pipe, layer, xpe;
    soc_reg_t reg = INVALIDr;
    int max_value;

    if (arg < 0) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_SCHEDULER(gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        return BCM_E_PARAM;
    }
    BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));

    if (!SOC_PORT_VALID(unit, local_port)) {
        return BCM_E_PORT;
    }


    if (type == bcmCosqControlIngressPoolLimitBytes) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_ing_pool_get(unit, gport, cosq,
                                       bcmCosqControlIngressPool,
                                       &pool));
        reg = THDI_BUFFER_CELL_LIMIT_SPr;
#ifdef BCM_TOMAHAWK2_SUPPORT
        if (SOC_IS_TOMAHAWK2(unit)) {
            max_value = _TH2_THDI_BUFFER_CELL_LIMIT_SP_MAX;
        } else
#endif
#ifdef BCM_TOMAHAWKPLUS_SUPPORT
        if (SOC_IS_TOMAHAWKPLUS(unit)) {
            max_value = _THP_THDI_BUFFER_CELL_LIMIT_SP_MAX;
        } else
#endif
        {
            max_value = _TH_THDI_BUFFER_CELL_LIMIT_SP_MAX;
        }
    } else if (type == bcmCosqControlIngressHeadroomPoolLimitBytes) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_ing_pool_get(unit, gport, cosq,
                                       bcmCosqControlIngressHeadroomPool,
                                       &pool));
        reg = THDI_HDRM_BUFFER_CELL_LIMIT_HPr;
#ifdef BCM_TOMAHAWK2_SUPPORT
        if (SOC_IS_TOMAHAWK2(unit)) {
            max_value = _TH2_THDI_HDRM_BUFFER_CELL_LIMIT_HP_MAX;
        } else
#endif
#ifdef BCM_TOMAHAWKPLUS_SUPPORT
        if (SOC_IS_TOMAHAWKPLUS(unit)) {
            max_value = _THP_THDI_HDRM_BUFFER_CELL_LIMIT_HP_MAX;
        } else
#endif
        {
            max_value = _TH_THDI_HDRM_BUFFER_CELL_LIMIT_HP_MAX;
        }
    } else {
        return BCM_E_PARAM;
    }

    arg = (arg + _TH_MMU_BYTES_PER_CELL - 1) / _TH_MMU_BYTES_PER_CELL;
    if (arg > max_value) {
        return BCM_E_PARAM;
    }
    soc_reg_field_set(unit, reg, &rval, LIMITf, arg);
    SOC_IF_ERROR_RETURN(
        soc_port_pipe_get(unit, local_port, &pipe));
    SOC_IF_ERROR_RETURN(
        soc_tomahawk_mmu_layer_get(unit, pipe, &layer));
    
    if (buffer == BCM_COSQ_BUFFER_ID_INVALID) {
        xpe = -1;
    } else if (SOC_INFO(unit).ipipe_xpe_map[pipe] & (1 << buffer)) {
        xpe = buffer;
    } else {
        return BCM_E_PARAM;
    }

    SOC_IF_ERROR_RETURN(
        soc_tomahawk_xpe_reg32_set(unit, reg,
                                   xpe, layer, pool, rval));

    return BCM_E_NONE;
}

/*
 * This function is used to set enables
 * for Qgroup Min and Shared Limits for a given Egress Unicast Queue
 */
STATIC int
_bcm_th_cosq_qgroup_limit_enables_set(int unit, bcm_gport_t gport,
                               bcm_cos_t cosq, bcm_cosq_control_t type,
                               int arg)
{
    bcm_port_t local_port;
    int startq=0, pipe=0, count=1, lc=0, to_cos=0;
    uint32 entry[SOC_MAX_MEM_WORDS], entry2[SOC_MAX_MEM_WORDS];
    soc_mem_t mem = INVALIDm, base_mem = INVALIDm;
    int enable;
    int valid;
    soc_field_t field = INVALIDf;
    soc_field_t field2 = INVALIDf;
    int new_shared_limit_cell, egr_qe_shd_size[_TH_XPES_PER_DEV], egr_db_shd_size[_TH_XPES_PER_DEV];
    int rv;
    int xpe, temp_val, pipe2=-1;
    uint32 pipe_map;
    soc_info_t *si=&SOC_INFO(unit);

    if (arg < 0) {
        return BCM_E_PARAM;
    }
    arg = arg ? 1 : 0;

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                        _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                        &local_port, &startq, NULL));
    } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        return BCM_E_PARAM;
    } else {
        if (cosq < -1) {
            return BCM_E_PARAM;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
        if (local_port < 0) {
            return BCM_E_PORT;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_index_resolve
             (unit, local_port, cosq, _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
              NULL, &startq, &count));
    }

    SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
    base_mem = MMU_THDU_Q_TO_QGRP_MAPm;
    mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
    if (type == bcmCosqControlEgressUCQueueGroupSharedLimitEnable) {
       field = QGROUP_LIMIT_ENABLEf;
       field2 = QGROUP_VALIDf;
    } else if (type == bcmCosqControlEgressUCQueueGroupMinEnable) {
        field = USE_QGROUP_MINf;
        field2 = QGROUP_VALIDf;
    } else {
        return BCM_E_PARAM;
    }

    to_cos = startq + count;
    for (lc = startq; lc < to_cos; lc++) {
        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, 
                    lc, entry));
        enable = soc_mem_field32_get(unit, mem, entry, field);
        if (enable != arg) {
            soc_mem_field32_set(unit, mem, entry, field, arg);
            BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ALL,
                        lc, entry));
        }
        BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL,
                    lc, entry2));
        valid = soc_mem_field32_get(unit, mem, entry2, field2);
        if (valid != 1) {
            soc_mem_field32_set(unit, mem, entry2, field2, 1);
            BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ALL,
                        lc, entry2));
        }
    }

    /*If qgroup min limit enable is being set, 
     *buffer shared limits need to be recalculated
     */
    if (type == bcmCosqControlEgressUCQueueGroupMinEnable) {
        sal_memcpy(egr_db_shd_size, _BCM_TH_MMU_INFO(unit)->egr_db_shared_limit, 
            sizeof(egr_db_shd_size));
        sal_memcpy(egr_qe_shd_size, _BCM_TH_MMU_INFO(unit)->egr_qe_shared_limit, 
            sizeof(egr_qe_shd_size));
        /*Calculate the new value of shared limit*/
        soc_th_recalc_new_egress_shared_limit(unit, pipe,
                &new_shared_limit_cell);
        /*Getting the two egress pipes with the new value of shared limit*/
        for (xpe = 0; xpe < NUM_XPE(unit); xpe++) {
            pipe_map = si->xpe_epipe_map[xpe];
            if (pipe_map & (1 << pipe)) {
                count = 0;
               while(pipe_map) {
                    temp_val = (pipe_map >> 1);
                    if ((temp_val != 0) && (count != pipe)) {
                        pipe2=count;
                        break;
                    }
                    count++;
               }
            }
        }
        /*Assign new value to both xpes for the egress pipe*/
        egr_db_shd_size[pipe] = new_shared_limit_cell;
        if (pipe2!=-1) {
            egr_db_shd_size[pipe2] = new_shared_limit_cell;
        }
        /*Passing the modified values to the funtion to re-adjust shared limits*/
        rv = soc_th_mmu_config_res_limits_update(unit, 2 /* egr */, NULL,
                                                     egr_db_shd_size,
                                                     egr_qe_shd_size, 1);
        if (rv < 0) {
            return rv;
        }
        sal_memcpy( _BCM_TH_MMU_INFO(unit)->egr_db_shared_limit,egr_db_shd_size,
            sizeof(egr_db_shd_size));
    }
    return BCM_E_NONE;
}

/*
 * This function is used to get enables
 * for Qgroup Min and Shared Limits for a given Egress Unicast Queue
 */
STATIC int
_bcm_th_cosq_qgroup_limit_enables_get(int unit, bcm_gport_t gport,
                                         bcm_cos_queue_t cosq,
                                         bcm_cosq_control_t type,
                                         int *arg)
{
    bcm_port_t local_port;
    int startq, pipe;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_mem_t mem = INVALIDm, base_mem = INVALIDm;
    soc_field_t field = INVALIDf;
    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_index_resolve(unit, gport, cosq,
                                      _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                                      &local_port, &startq, NULL));
    } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        return BCM_E_PARAM;
    } else {
        if (cosq < 0) {
            return BCM_E_PARAM;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
        if (local_port < 0) {
            return BCM_E_PORT;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_index_resolve
                (unit, local_port, cosq, _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE,
                NULL, &startq, NULL));
    }
    SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
    base_mem = MMU_THDU_Q_TO_QGRP_MAPm;
    mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
    BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, startq, entry));
     if (type == bcmCosqControlEgressUCQueueGroupMinEnable) {
        field = USE_QGROUP_MINf;
    } else if (type == bcmCosqControlEgressUCQueueGroupSharedLimitEnable) {
        field = QGROUP_LIMIT_ENABLEf;
    } else {
        return BCM_E_PARAM;
    }
        *arg = soc_mem_field32_get(unit, mem, entry, field);
    return BCM_E_NONE;
}

/*
 * This function is used to set limits
 * for Qgroup Min and Shared Limits for a given Egress Unicast Queue
 */
STATIC int
_bcm_th_cosq_qgroup_limit_set(int unit, bcm_gport_t gport, bcm_cos_t cosq,
                               bcm_cosq_control_t type, int arg)
{
    bcm_port_t local_port;
    int phy_port, pipe;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_mem_t mem = INVALIDm, base_mem = INVALIDm;
    soc_field_t field = INVALIDf;
    int mmu_port; /* global mmu port number */
    int local_mmu_port; /* local mmu port number - local to per pipe */
    soc_info_t *si;
    int granularity;
    int egr_db_shd_size[_TH_XPES_PER_DEV], egr_qe_shd_size[_TH_XPES_PER_DEV], cur_val;
    int rv, xpe;
    int delta[_TH_XPES_PER_DEV] = {0};
    int max_val;
    uint32 xpe_map;
    si = &SOC_INFO(unit);

    if (cosq < 0) {
        return BCM_E_PARAM;
    }
    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
    if (local_port < 0) {
        return BCM_E_PORT;
    }

    if (type == bcmCosqControlEgressUCQueueGroupMinLimitBytes ||
            type == bcmCosqControlEgressUCQueueGroupSharedLimitBytes) {
        arg = (arg + _TH_MMU_BYTES_PER_CELL - 1) / _TH_MMU_BYTES_PER_CELL;
    }

    SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
    phy_port = si->port_l2p_mapping[local_port];
    mmu_port = si->port_p2m_mapping[phy_port];
    local_mmu_port = mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1);
    base_mem = MMU_THDU_CONFIG_QGROUPm;
    mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
    BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, local_mmu_port,
                entry));

    if (type == bcmCosqControlEgressUCQueueGroupMinLimitBytes) {
        field = Q_MIN_LIMIT_CELLf;
        granularity =1;
        /* Recalculate Shared Values if Min Changed */
        sal_memcpy(egr_db_shd_size, _BCM_TH_MMU_INFO(unit)->egr_db_shared_limit, 
             sizeof(egr_db_shd_size));
        sal_memcpy(egr_qe_shd_size, _BCM_TH_MMU_INFO(unit)->egr_qe_shared_limit, 
             sizeof(egr_qe_shd_size));

        cur_val = soc_mem_field32_get(unit, mem, entry, field);
        /* adjustment delta */
        xpe_map = SOC_INFO(unit).epipe_xpe_map[pipe];
        for (xpe = 0; xpe < NUM_XPE(unit); xpe ++) {
            if (xpe_map & (1 << xpe)) {
                delta[xpe] = ((arg / granularity) - cur_val) * granularity;
            }
        }
        /* shared limits to be increased/decreased */
        for (xpe = 0; xpe < NUM_XPE(unit); xpe ++) {
            if ((xpe_map & (1 << xpe)) && (delta[xpe] > 0)) {
                /* no space to decrease shared limit */
                if (delta[xpe] > egr_db_shd_size[xpe]) {
                    return BCM_E_PARAM;
                }
                egr_db_shd_size[xpe] -= delta[xpe];
            }
            if ((xpe_map & (1 << xpe)) && (delta[xpe] < 0)) {
                egr_db_shd_size[xpe] -= delta[xpe];
            }
        }

        rv = soc_th_mmu_config_res_limits_update(unit, 2 /* egr */, NULL,
                                                     egr_db_shd_size ,
                                                     egr_qe_shd_size, 1);
        if (rv < 0) {
            return rv;
        }

        sal_memcpy(_BCM_TH_MMU_INFO(unit)->egr_db_shared_limit,egr_db_shd_size,
             sizeof(egr_db_shd_size));
    } else if (type == bcmCosqControlEgressUCQueueGroupSharedLimitBytes) {
        field = Q_SHARED_LIMIT_CELLf;
        granularity = 1;
    } else if (type == bcmCosqControlEgressUCQueueGroupSharedDynamicEnable) {
        field = Q_LIMIT_DYNAMIC_CELLf;
        granularity = 1;
    } else {
        return BCM_E_PARAM;
    }

    /* Check for min/max values */
    max_val = (1 << soc_mem_field_length(unit, mem, field)) - 1;
    if ((arg < 0) || ((arg/granularity) > max_val)) {
        return BCM_E_PARAM;
    } else {
        soc_mem_field32_set(unit, mem, entry, field, (arg/granularity));
    }
    BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ALL, local_mmu_port,
                entry));
    return BCM_E_NONE;
}

/*
 * This function is used to get limits
 * for Qgroup Min and Shared Limits for a given Egress Unicast Queue
 */

STATIC int
_bcm_th_cosq_qgroup_limit_get(int unit, bcm_gport_t gport, bcm_cos_t cosq,
                               bcm_cosq_control_t type, int *arg)
{
    bcm_port_t local_port;
    int phy_port, pipe;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_mem_t mem = INVALIDm, base_mem = INVALIDm ;
    soc_field_t field = INVALIDf;
    int mmu_port; /* global mmu port number */
    int local_mmu_port; /* local mmu port number - local to per pipe */
    soc_info_t *si;
    si = &SOC_INFO(unit);
    if (cosq < 0) {
        return BCM_E_PARAM;
    }
    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, gport, &local_port));
    if (local_port < 0) {
        return BCM_E_PORT;
    }
    SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));
    phy_port = si->port_l2p_mapping[local_port];
    mmu_port = si->port_p2m_mapping[phy_port];
    local_mmu_port = mmu_port & (SOC_TH_MMU_PORT_STRIDE - 1);
    base_mem = MMU_THDU_CONFIG_QGROUPm;
    mem = SOC_MEM_UNIQUE_ACC(unit, base_mem)[pipe];
    BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ALL, local_mmu_port,
                entry));
    if (type == bcmCosqControlEgressUCQueueGroupMinLimitBytes) {
       field = Q_MIN_LIMIT_CELLf;
    } else if (type == bcmCosqControlEgressUCQueueGroupSharedLimitBytes) {
        field = Q_SHARED_LIMIT_CELLf;
    } else if (type == bcmCosqControlEgressUCQueueGroupSharedDynamicEnable) {
        field = Q_LIMIT_DYNAMIC_CELLf;
    } else {
        return BCM_E_PARAM;
    }
    *arg = soc_mem_field32_get(unit, mem, entry, field);
    if (type == bcmCosqControlEgressUCQueueGroupMinLimitBytes ||
            type == bcmCosqControlEgressUCQueueGroupSharedLimitBytes) {
        *arg = (*arg) * _TH_MMU_BYTES_PER_CELL;
    }
    return BCM_E_NONE;
}


/*
 * Function:
 *      bcm_th_cosq_control_set
 * Purpose:
 *      Set specified feature configuration
 *
 * Parameters:
 *      unit - (IN) Unit number.
 *      port - (IN) GPORT ID.
 *      cosq - (IN) COS queue.
 *      buffer-(IN) XPE ID
 *      type - (IN) feature
 *      arg  - (IN) feature value
 * Returns:
 *      BCM_E_XXX
 * Notes:
 */
int
bcm_th_cosq_control_set(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq,
                        bcm_cosq_buffer_id_t buffer,
                        bcm_cosq_control_t type, int arg)
{
    uint32 kbits_burst_min, kbits_burst_max;
    uint32 kbits_burst;

    kbits_burst_min = 0;
    kbits_burst_max = 0;

    COMPILER_REFERENCE(kbits_burst_min);
    COMPILER_REFERENCE(kbits_burst_max);

    switch (type) {
        case bcmCosqControlBandwidthBurstMax:
            kbits_burst = arg & 0x7fffffff; /* Convert to uint32  */
            BCM_IF_ERROR_RETURN
                (bcm_th_cosq_gport_bandwidth_burst_get(unit, gport, cosq,
                                                       &kbits_burst_min,
                                                       &kbits_burst_max));
            return bcm_th_cosq_gport_bandwidth_burst_set(unit, gport, cosq,
                                                         kbits_burst_min,
                                                         kbits_burst);
        case bcmCosqControlBandwidthBurstMin:
            kbits_burst = arg & 0x7fffffff; /* Convert to uint32  */
            BCM_IF_ERROR_RETURN
                (bcm_th_cosq_gport_bandwidth_burst_get(unit, gport, cosq,
                                                       &kbits_burst_min,
                                                       &kbits_burst_max));
            return bcm_th_cosq_gport_bandwidth_burst_set(unit, gport, cosq,
                                                         kbits_burst,
                                                         kbits_burst_max);
        case bcmCosqControlEgressPool:
        case bcmCosqControlEgressPoolLimitBytes:
        case bcmCosqControlEgressPoolYellowLimitBytes:
        case bcmCosqControlEgressPoolRedLimitBytes:
        case bcmCosqControlEgressPoolLimitEnable:
        case bcmCosqControlUCEgressPool:
        case bcmCosqControlMCEgressPool:
        case bcmCosqControlEgressPoolSharedLimitBytes:
        case bcmCosqControlEgressPoolResumeLimitBytes:
        case bcmCosqControlEgressPoolYellowSharedLimitBytes:
        case bcmCosqControlEgressPoolYellowResumeLimitBytes:
        case bcmCosqControlEgressPoolRedSharedLimitBytes:
        case bcmCosqControlEgressPoolRedResumeLimitBytes:
            return _bcm_th_cosq_egr_pool_set(unit, gport, cosq, buffer, type, arg);
        case bcmCosqControlEgressUCQueueSharedLimitBytes:
        case bcmCosqControlEgressMCQueueSharedLimitBytes:
        case bcmCosqControlEgressUCQueueMinLimitBytes:
        case bcmCosqControlEgressMCQueueMinLimitBytes:
            return _bcm_th_cosq_egr_queue_set(unit, gport, cosq, type, arg);
        case bcmCosqControlIngressPortPGSharedLimitBytes:
        case bcmCosqControlIngressPortPGMinLimitBytes:
        case bcmCosqControlIngressPortPoolMaxLimitBytes:
        case bcmCosqControlIngressPortPoolMinLimitBytes:
        case bcmCosqControlIngressPortPGHeadroomLimitBytes:
        case bcmCosqControlIngressPortPGResetFloorBytes:
            return _bcm_th_cosq_ing_res_set(unit, gport, cosq, type, arg);
        case bcmCosqControlIngressPool:
        case bcmCosqControlIngressHeadroomPool:
            return _bcm_th_cosq_ing_pool_set(unit, gport, cosq, type, arg);
        case bcmCosqControlDropLimitAlpha:
            return _bcm_th_cosq_alpha_set(unit, gport, cosq,
                                (bcm_cosq_control_drop_limit_alpha_value_t)arg);
        case bcmCosqControlIngressPortPGSharedDynamicEnable:
        case bcmCosqControlEgressUCSharedDynamicEnable:
        case bcmCosqControlEgressMCSharedDynamicEnable:
            return _bcm_th_cosq_dynamic_thresh_enable_set(unit, gport, cosq,
                                                          type, arg);
        case bcmCosqControlEgressPortPoolYellowLimitBytes:
        case bcmCosqControlEgressPortPoolRedLimitBytes:
            return _bcm_th_cosq_egr_port_pool_set(unit, gport, cosq, type, arg);
        case bcmCosqControlEgressUCQueueLimitEnable:
        case bcmCosqControlEgressMCQueueLimitEnable:
            return _bcm_th_cosq_egr_queue_limit_enable_set(unit, gport, cosq,
                                                           type, arg);
        case bcmCosqControlIngressPoolLimitBytes:
        case bcmCosqControlIngressHeadroomPoolLimitBytes:
            return _bcm_th_cosq_ing_res_limit_set(unit, gport, cosq, buffer, type, arg);
        case bcmCosqControlEgressUCQueueGroupMinEnable:
        case bcmCosqControlEgressUCQueueGroupSharedLimitEnable:
            return _bcm_th_cosq_qgroup_limit_enables_set(unit, gport, cosq,
                    type, arg);
        case bcmCosqControlEgressUCQueueGroupMinLimitBytes:
        case bcmCosqControlEgressUCQueueGroupSharedLimitBytes:
        case bcmCosqControlEgressUCQueueGroupSharedDynamicEnable:
            return _bcm_th_cosq_qgroup_limit_set(unit, gport, cosq, type, arg);
        default:
            break;
    }
    return BCM_E_UNAVAIL;
}

/*
 * Function:
 *      bcm_th_cosq_control_get
 * Purpose:
 *      Get specified feature configuration
 *
 * Parameters:
 *      unit - (IN) Unit number.
 *      port - (IN) GPORT ID.
 *      cosq - (IN) COS queue.
 *      buffer-(IN) XPE ID
 *      type - (IN) feature
 *      arg  - (OUT) feature value
 * Returns:
 *      BCM_E_XXX
 * Notes:
 */
int
bcm_th_cosq_control_get(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq,
                        bcm_cosq_buffer_id_t buffer,
                        bcm_cosq_control_t type, int *arg)
{
    uint32 kburst_tmp, kbits_burst;

    switch (type) {
        case bcmCosqControlBandwidthBurstMax:
            BCM_IF_ERROR_RETURN
                (bcm_th_cosq_gport_bandwidth_burst_get(unit, gport, cosq,
                                                       &kburst_tmp,
                                                       &kbits_burst));
            *arg = kbits_burst & 0x7fffffff; /* Convert to int  */
            return BCM_E_NONE;

        case bcmCosqControlBandwidthBurstMin:
            return bcm_th_cosq_gport_bandwidth_burst_get(unit, gport, cosq,
                                                         (uint32*)arg,
                                                         &kburst_tmp);
        case bcmCosqControlEgressPool:
        case bcmCosqControlEgressPoolLimitBytes:
        case bcmCosqControlEgressPoolYellowLimitBytes:
        case bcmCosqControlEgressPoolRedLimitBytes:
        case bcmCosqControlEgressPoolLimitEnable:
        case bcmCosqControlUCEgressPool:
        case bcmCosqControlMCEgressPool:
        case bcmCosqControlEgressPoolSharedLimitBytes:
        case bcmCosqControlEgressPoolResumeLimitBytes:
        case bcmCosqControlEgressPoolYellowSharedLimitBytes:
        case bcmCosqControlEgressPoolYellowResumeLimitBytes:
        case bcmCosqControlEgressPoolRedSharedLimitBytes:
        case bcmCosqControlEgressPoolRedResumeLimitBytes:
            return _bcm_th_cosq_egr_pool_get(unit, gport, cosq, buffer, type, arg);
        case bcmCosqControlEgressUCQueueSharedLimitBytes:
        case bcmCosqControlEgressMCQueueSharedLimitBytes:
        case bcmCosqControlEgressUCQueueMinLimitBytes:
        case bcmCosqControlEgressMCQueueMinLimitBytes:
            return _bcm_th_cosq_egr_queue_get(unit, gport, cosq, type, arg);
        case bcmCosqControlIngressPortPGSharedLimitBytes:
        case bcmCosqControlIngressPortPGMinLimitBytes:
        case bcmCosqControlIngressPortPoolMaxLimitBytes:
        case bcmCosqControlIngressPortPoolMinLimitBytes:
        case bcmCosqControlIngressPortPGHeadroomLimitBytes:
        case bcmCosqControlIngressPortPGResetFloorBytes:
            return _bcm_th_cosq_ing_res_get(unit, gport, cosq, type, arg);
        case bcmCosqControlIngressPool:
        case bcmCosqControlIngressHeadroomPool:
            return _bcm_th_cosq_ing_pool_get(unit, gport, cosq, type, arg);
        case bcmCosqControlDropLimitAlpha:
            return _bcm_th_cosq_alpha_get(unit, gport, cosq,
                            (bcm_cosq_control_drop_limit_alpha_value_t *)arg);
        case bcmCosqControlIngressPortPGSharedDynamicEnable:
        case bcmCosqControlEgressUCSharedDynamicEnable:
        case bcmCosqControlEgressMCSharedDynamicEnable:
            return _bcm_th_cosq_dynamic_thresh_enable_get(unit, gport, cosq,
                                                          type, arg);
        case bcmCosqControlEgressPortPoolYellowLimitBytes:
        case bcmCosqControlEgressPortPoolRedLimitBytes:
            return _bcm_th_cosq_egr_port_pool_get(unit, gport, cosq, type, arg);
        case bcmCosqControlEgressUCQueueLimitEnable:
        case bcmCosqControlEgressMCQueueLimitEnable:
            return _bcm_th_cosq_egr_queue_limit_enable_get(unit, gport, cosq,
                                                           type, arg);
        case bcmCosqControlIngressPoolLimitBytes:
        case bcmCosqControlIngressHeadroomPoolLimitBytes:
            return _bcm_th_cosq_ing_res_limit_get(unit, gport, cosq, buffer, type, arg);
        case bcmCosqControlEgressUCQueueGroupMinEnable:
        case bcmCosqControlEgressUCQueueGroupSharedLimitEnable:
            return _bcm_th_cosq_qgroup_limit_enables_get(unit, gport, cosq,
                    type, arg);
        case bcmCosqControlEgressUCQueueGroupMinLimitBytes:
        case bcmCosqControlEgressUCQueueGroupSharedLimitBytes:
        case bcmCosqControlEgressUCQueueGroupSharedDynamicEnable:
            return _bcm_th_cosq_qgroup_limit_get(unit, gport, cosq, type, arg);
        default:
            break;
    }
    return BCM_E_UNAVAIL;
}

STATIC int
_bcm_th_cosq_obmhighwatermark_set(int unit, bcm_port_t port, uint64 value)
{
    int pipe, obm_id, obm_max, lane;
    soc_reg_t reg;
    uint64 rval64;
    soc_info_t *si;
    soc_reg_t obm_high_watermark_reg[] = {
        IDB_OBM0_MAX_USAGEr, IDB_OBM1_MAX_USAGEr,
        IDB_OBM2_MAX_USAGEr, IDB_OBM3_MAX_USAGEr,
        IDB_OBM4_MAX_USAGEr, IDB_OBM5_MAX_USAGEr,
        IDB_OBM6_MAX_USAGEr, IDB_OBM7_MAX_USAGEr,
#ifdef BCM_TOMAHAWK2_SUPPORT
        IDB_OBM8_MAX_USAGEr, IDB_OBM9_MAX_USAGEr,
        IDB_OBM10_MAX_USAGEr, IDB_OBM11_MAX_USAGEr,
        IDB_OBM12_MAX_USAGEr, IDB_OBM13_MAX_USAGEr,
        IDB_OBM14_MAX_USAGEr, IDB_OBM15_MAX_USAGEr,
#endif
    };

    soc_field_t obm_high_watermark_fields[] = {
        PORT0_MAX_USAGEf, PORT1_MAX_USAGEf,
        PORT2_MAX_USAGEf, PORT3_MAX_USAGEf
    };

    SOC_IF_ERROR_RETURN
            (soc_tomahawk_port_obm_info_get(unit, port, &obm_id, &lane));
#ifdef BCM_TOMAHAWK2_SUPPORT
    obm_max = SOC_IS_TOMAHAWK2(unit) ? _TH2_PBLKS_PER_PIPE : _TH_PBLKS_PER_PIPE;
#else
    obm_max = _TH_PBLKS_PER_PIPE;
#endif
    if (obm_id >= obm_max || (lane < 0 || lane >= _TH_PORTS_PER_PBLK)) {
            return BCM_E_PARAM;
    }
    if(!COMPILER_64_IS_ZERO(value)) {
        return BCM_E_PARAM;
    }
    si = &SOC_INFO(unit);
    pipe = si->port_pipe[port];

    reg = SOC_REG_UNIQUE_ACC(unit, obm_high_watermark_reg[obm_id])[pipe];
    COMPILER_64_ZERO(rval64);
    BCM_IF_ERROR_RETURN
            (soc_reg64_get(unit, reg, REG_PORT_ANY, 0, &rval64));

    soc_reg64_field32_set(unit, reg, &rval64, obm_high_watermark_fields[lane], 0);
    BCM_IF_ERROR_RETURN
            (soc_reg64_set(unit, reg, REG_PORT_ANY, 0, rval64));
    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_obmhighwatermark_get(int unit, bcm_port_t port, uint64 *value)
{
    int pipe, obm_id, obm_max, lane, gran;
    soc_reg_t reg;
    uint64 rval64;
    soc_info_t *si;
    soc_reg_t obm_high_watermark_reg[] = {
        IDB_OBM0_MAX_USAGEr, IDB_OBM1_MAX_USAGEr,
        IDB_OBM2_MAX_USAGEr, IDB_OBM3_MAX_USAGEr,
        IDB_OBM4_MAX_USAGEr, IDB_OBM5_MAX_USAGEr,
        IDB_OBM6_MAX_USAGEr, IDB_OBM7_MAX_USAGEr,
#ifdef BCM_TOMAHAWK2_SUPPORT
        IDB_OBM8_MAX_USAGEr, IDB_OBM9_MAX_USAGEr,
        IDB_OBM10_MAX_USAGEr, IDB_OBM11_MAX_USAGEr,
        IDB_OBM12_MAX_USAGEr, IDB_OBM13_MAX_USAGEr,
        IDB_OBM14_MAX_USAGEr, IDB_OBM15_MAX_USAGEr,
#endif
    };

    soc_field_t obm_high_watermark_fields[] = {
        PORT0_MAX_USAGEf, PORT1_MAX_USAGEf,
        PORT2_MAX_USAGEf, PORT3_MAX_USAGEf
    };

    SOC_IF_ERROR_RETURN
            (soc_tomahawk_port_obm_info_get(unit, port, &obm_id, &lane));
#ifdef BCM_TOMAHAWK2_SUPPORT
    obm_max = SOC_IS_TOMAHAWK2(unit) ? _TH2_PBLKS_PER_PIPE : _TH_PBLKS_PER_PIPE;
#else
    obm_max = _TH_PBLKS_PER_PIPE;
#endif
    if (obm_id >= obm_max || (lane < 0 || lane >= _TH_PORTS_PER_PBLK)) {
            return BCM_E_PARAM;
    }
    si = &SOC_INFO(unit);
    pipe = si->port_pipe[port];

    reg = SOC_REG_UNIQUE_ACC(unit, obm_high_watermark_reg[obm_id])[pipe];
    COMPILER_64_ZERO(rval64);
    BCM_IF_ERROR_RETURN
            (soc_reg64_get(unit, reg, REG_PORT_ANY, 0, &rval64));

    *value = soc_reg64_field_get(unit, reg, rval64, obm_high_watermark_fields[lane]);
    gran = SOC_IS_TOMAHAWK2(unit) ? 16 : 48;
    COMPILER_64_UMUL_32(*value, gran);

    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_obm_buf_use_cnt_set(int unit, bcm_port_t port, uint64 value)
{
    int pipe, obm_id, obm_max, lane;
    soc_reg_t reg;
    soc_field_t fld;
    uint64 rval64;
    soc_info_t *si;
    soc_reg_t obm_usage_reg[] = {
        IDB_OBM0_USAGEr, IDB_OBM1_USAGEr,
        IDB_OBM2_USAGEr, IDB_OBM3_USAGEr,
        IDB_OBM4_USAGEr, IDB_OBM5_USAGEr,
        IDB_OBM6_USAGEr, IDB_OBM7_USAGEr,
#ifdef BCM_TOMAHAWK2_SUPPORT
        IDB_OBM8_USAGEr, IDB_OBM9_USAGEr,
        IDB_OBM10_USAGEr, IDB_OBM11_USAGEr,
        IDB_OBM12_USAGEr, IDB_OBM13_USAGEr,
        IDB_OBM14_USAGEr, IDB_OBM15_USAGEr,
#endif
    };

    SOC_IF_ERROR_RETURN
            (soc_tomahawk_port_obm_info_get(unit, port, &obm_id, &lane));
#ifdef BCM_TOMAHAWK2_SUPPORT
    obm_max = SOC_IS_TOMAHAWK2(unit) ? _TH2_PBLKS_PER_PIPE : _TH_PBLKS_PER_PIPE;
#else
    obm_max = _TH_PBLKS_PER_PIPE;
#endif
    if (obm_id >= obm_max || (lane < 0 || lane >= _TH_PORTS_PER_PBLK)) {
        return BCM_E_PARAM;
    }
    if(!COMPILER_64_IS_ZERO(value)) {
        return BCM_E_PARAM;
    }
    si = &SOC_INFO(unit);
    pipe = si->port_pipe[port];

    reg = SOC_REG_UNIQUE_ACC(unit, obm_usage_reg[obm_id])[pipe];
    fld = si->mmu_lossless ? LOSSLESS0_COUNTf : LOSSY_COUNTf;
    COMPILER_64_ZERO(rval64);
    BCM_IF_ERROR_RETURN
            (soc_reg64_get(unit, reg, REG_PORT_ANY, lane, &rval64));

    soc_reg64_field_set(unit, reg, &rval64, fld, value);
    BCM_IF_ERROR_RETURN
            (soc_reg64_set(unit, reg, REG_PORT_ANY, lane, rval64));
    return BCM_E_NONE;
}

STATIC int
_bcm_th_cosq_obm_buf_use_cnt_get(int unit, bcm_port_t port, uint64 *value)
{
    int pipe, obm_id, obm_max, lane, gran;
    soc_reg_t reg;
    soc_field_t fld;
    uint64 rval64;
    soc_info_t *si;
    soc_reg_t obm_usage_reg[] = {
        IDB_OBM0_USAGEr, IDB_OBM1_USAGEr,
        IDB_OBM2_USAGEr, IDB_OBM3_USAGEr,
        IDB_OBM4_USAGEr, IDB_OBM5_USAGEr,
        IDB_OBM6_USAGEr, IDB_OBM7_USAGEr,
#ifdef BCM_TOMAHAWK2_SUPPORT
        IDB_OBM8_USAGEr, IDB_OBM9_USAGEr,
        IDB_OBM10_USAGEr, IDB_OBM11_USAGEr,
        IDB_OBM12_USAGEr, IDB_OBM13_USAGEr,
        IDB_OBM14_USAGEr, IDB_OBM15_USAGEr,
#endif
    };

    SOC_IF_ERROR_RETURN
            (soc_tomahawk_port_obm_info_get(unit, port, &obm_id, &lane));
#ifdef BCM_TOMAHAWK2_SUPPORT
    obm_max = SOC_IS_TOMAHAWK2(unit) ? _TH2_PBLKS_PER_PIPE : _TH_PBLKS_PER_PIPE;
#else
    obm_max = _TH_PBLKS_PER_PIPE;
#endif
    if (obm_id >= obm_max || (lane < 0 || lane >= _TH_PORTS_PER_PBLK)) {
            return BCM_E_PARAM;
    }
    si = &SOC_INFO(unit);
    pipe = si->port_pipe[port];

    reg = SOC_REG_UNIQUE_ACC(unit, obm_usage_reg[obm_id])[pipe];
    fld = si->mmu_lossless ? LOSSLESS0_COUNTf : LOSSY_COUNTf;
    COMPILER_64_ZERO(rval64);
    BCM_IF_ERROR_RETURN
            (soc_reg64_get(unit, reg, REG_PORT_ANY, lane, &rval64));

    *value = soc_reg64_field_get(unit, reg, rval64, fld);
    gran = SOC_IS_TOMAHAWK2(unit) ? 16 : 48;
    COMPILER_64_UMUL_32(*value, gran);

    return BCM_E_NONE;
}

int
bcm_th_cosq_stat_set(int unit, bcm_gport_t port, bcm_cos_queue_t cosq,
                     bcm_cosq_stat_t stat, uint64 value)
{
    soc_ctr_control_info_t ctrl_info;
    soc_reg_t ctr_reg, ctr_reg_uc, ctr_reg_mc;
    _bcm_th_cosq_index_style_t style = _BCM_TH_COSQ_INDEX_STYLE_COUNT;
    int startq = 0, ci, from_cos, to_cos, i, start_hw_index, end_hw_index;
    bcm_port_t local_port = -1;

    ctrl_info.instance = -1;
    ctrl_info.instance_type = SOC_CTR_INSTANCE_TYPE_XPE;

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, port, &local_port));
    if ((cosq < -1) || (cosq >= _TH_PORT_NUM_COS(unit, local_port))) {
        return BCM_E_PARAM;
    }

    switch (stat) {
        case bcmCosqStatDroppedPackets:
        case bcmCosqStatDroppedBytes:
        case bcmCosqStatOutPackets:
        case bcmCosqStatOutBytes:
        case bcmCosqStatEgressMCQueueBytesPeak:
        case bcmCosqStatEgressMCQueueBytesCurrent:
        case bcmCosqStatEgressUCQueueBytesPeak:
        case bcmCosqStatEgressUCQueueBytesCurrent:
            style = _BCM_TH_COSQ_INDEX_STYLE_COS;
            if (stat == bcmCosqStatEgressMCQueueBytesPeak) {
                if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
                    BCM_GPORT_IS_SCHEDULER(port)) {
                    return BCM_E_PARAM;
                }
                ctr_reg_uc = INVALIDr;
                ctr_reg_mc = SOC_COUNTER_NON_DMA_QUEUE_PEAK;
            } else if (stat == bcmCosqStatEgressMCQueueBytesCurrent) {
                if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
                    BCM_GPORT_IS_SCHEDULER(port)) {
                    return BCM_E_PARAM;
                }
                ctr_reg_uc = INVALIDr;
                ctr_reg_mc = SOC_COUNTER_NON_DMA_QUEUE_CURRENT;
            } else if (stat == bcmCosqStatEgressUCQueueBytesPeak) {
                if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
                    BCM_GPORT_IS_SCHEDULER(port)) {
                    return BCM_E_PARAM;
                }
                ctr_reg_uc = SOC_COUNTER_NON_DMA_UC_QUEUE_PEAK;
                ctr_reg_mc = INVALIDr;
            } else if (stat == bcmCosqStatEgressUCQueueBytesCurrent) {
                if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
                    BCM_GPORT_IS_SCHEDULER(port)) {
                    return BCM_E_PARAM;
                }
                ctr_reg_uc = SOC_COUNTER_NON_DMA_UC_QUEUE_CURRENT;
                ctr_reg_mc = INVALIDr;
            } else if (stat == bcmCosqStatDroppedBytes) {
                ctr_reg_uc = SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE_UC;
                ctr_reg_mc = SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE;
            } else if (stat == bcmCosqStatDroppedPackets) {
                ctr_reg_uc = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT_UC;
                ctr_reg_mc = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT;
            } else if (stat == bcmCosqStatOutBytes) {
                ctr_reg_uc = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_BYTE_UC;
                ctr_reg_mc = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_BYTE;
                ctrl_info.instance_type = SOC_CTR_INSTANCE_TYPE_PORT;
            } else { /* (stat == bcmCosqStatOutPackets) */
                ctr_reg_uc = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_PKT_UC;
                ctr_reg_mc = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_PKT;
                ctrl_info.instance_type = SOC_CTR_INSTANCE_TYPE_PORT;
            }
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port)) {
                if (ctr_reg_uc != INVALIDr) {
                    BCM_IF_ERROR_RETURN
                        (_bcm_th_cosq_index_resolve
                         (unit, port, cosq, style, &local_port, &startq, NULL));

                    ctrl_info.instance = local_port;
                    BCM_IF_ERROR_RETURN
                        (soc_counter_generic_set(unit, ctr_reg_uc, ctrl_info, 0,
                                                 startq, value));
                }
            } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
                if (ctr_reg_mc != INVALIDr) {
                    BCM_IF_ERROR_RETURN
                        (_bcm_th_cosq_index_resolve
                         (unit, port, cosq, style, &local_port, &startq, NULL));

                    ctrl_info.instance = local_port;
                    BCM_IF_ERROR_RETURN
                        (soc_counter_generic_set(unit, ctr_reg_mc, ctrl_info, 0,
                                                 startq, value));
                }
            } else {
                if (BCM_GPORT_IS_SCHEDULER(port)) {
                    BCM_IF_ERROR_RETURN
                        (_bcm_th_cosq_index_resolve
                         (unit, port, cosq, style, &local_port, &startq, NULL));
                    from_cos = to_cos = startq;
                } else {
                    BCM_IF_ERROR_RETURN
                        (_bcm_th_cosq_localport_resolve(unit, port,
                                                        &local_port));

                    if (cosq == BCM_COS_INVALID) {
                        /* if (cosq == -1) set value for the first cosq */
                        cosq = 0;
                    }
                    from_cos = to_cos = cosq;
                }
                ctrl_info.instance = local_port;
                for (ci = from_cos; ci <= to_cos; ci++) {

                    if (!IS_CPU_PORT(unit, local_port)) {
                        if (ctr_reg_uc != INVALIDr) {
                            /* Get UC values if NOT CPU port */
                            BCM_IF_ERROR_RETURN
                                (soc_counter_generic_set(unit, ctr_reg_uc,
                                                         ctrl_info, 0,
                                                         ci, value));
                        }
                    }

                    if (ctr_reg_mc != INVALIDr) {
                        BCM_IF_ERROR_RETURN
                            (soc_counter_generic_set(unit, ctr_reg_mc,
                                                     ctrl_info, 0, ci, value));
                    }
                }
            }
            break;
        case bcmCosqStatYellowCongestionDroppedPackets:
        case bcmCosqStatRedCongestionDroppedPackets:
        case bcmCosqStatGreenDiscardDroppedPackets:
        case bcmCosqStatYellowDiscardDroppedPackets:
        case bcmCosqStatRedDiscardDroppedPackets:
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_SCHEDULER(port) ||
                BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
                return BCM_E_PARAM;
            }
            if (cosq != BCM_COS_INVALID) {
                return BCM_E_UNAVAIL;
            }
            if (stat == bcmCosqStatYellowCongestionDroppedPackets) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_PKT_YELLOW;
            } else if (stat == bcmCosqStatRedCongestionDroppedPackets) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_PKT_RED;
            } else if (stat == bcmCosqStatYellowDiscardDroppedPackets) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_WRED_PKT_YELLOW;
            } else if (stat == bcmCosqStatRedDiscardDroppedPackets) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_WRED_PKT_RED;
            } else {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_WRED_PKT_GREEN;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, port, &local_port));

            ctrl_info.instance = local_port;
            BCM_IF_ERROR_RETURN
                (soc_counter_generic_set(unit, ctr_reg, ctrl_info, 0,
                                         startq, value));
            break;
        case bcmCosqStatIngressPortPGMinBytesPeak:
        case bcmCosqStatIngressPortPGMinBytesCurrent:
        case bcmCosqStatIngressPortPGSharedBytesPeak:
        case bcmCosqStatIngressPortPGSharedBytesCurrent:
        case bcmCosqStatIngressPortPGHeadroomBytesPeak:
        case bcmCosqStatIngressPortPGHeadroomBytesCurrent:
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_SCHEDULER(port) ||
                BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
                return BCM_E_PARAM;
            }
            if (stat == bcmCosqStatIngressPortPGMinBytesPeak) {
                ctr_reg = SOC_COUNTER_NON_DMA_PG_MIN_PEAK;
            } else if (stat == bcmCosqStatIngressPortPGMinBytesCurrent) {
                ctr_reg = SOC_COUNTER_NON_DMA_PG_MIN_CURRENT;
            } else if (stat == bcmCosqStatIngressPortPGSharedBytesPeak) {
                ctr_reg = SOC_COUNTER_NON_DMA_PG_SHARED_PEAK;
            } else if (stat == bcmCosqStatIngressPortPGSharedBytesCurrent) {
                ctr_reg = SOC_COUNTER_NON_DMA_PG_SHARED_CURRENT;
            } else if (stat == bcmCosqStatIngressPortPGHeadroomBytesPeak) {
                ctr_reg = SOC_COUNTER_NON_DMA_PG_HDRM_PEAK;
            } else {
                /* (stat == bcmCosqStatIngressPortPGHeadroomBytesCurrent) */
                ctr_reg = SOC_COUNTER_NON_DMA_PG_HDRM_CURRENT;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, port, &local_port));
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_ingress_pg_get(unit, port, cosq,
                                             &start_hw_index, &end_hw_index));

            ctrl_info.instance = local_port;
            for (i = start_hw_index; i <= end_hw_index; i++) {
                BCM_IF_ERROR_RETURN
                    (soc_counter_generic_set(unit, ctr_reg, ctrl_info, 0,
                                             i, value));
            }
            break;
        case bcmCosqStatHighPriDroppedPackets:
        case bcmCosqStatOBMLossyHighPriDroppedPackets:
        case bcmCosqStatLowPriDroppedPackets:
        case bcmCosqStatOBMLossyLowPriDroppedPackets:
        case bcmCosqStatHighPriDroppedBytes:
        case bcmCosqStatOBMLossyHighPriDroppedBytes:
        case bcmCosqStatLowPriDroppedBytes:
        case bcmCosqStatOBMLossyLowPriDroppedBytes:
        case bcmCosqStatOBMLossless0DroppedPackets:
        case bcmCosqStatOBMLossless1DroppedPackets:
        case bcmCosqStatOBMLossless0DroppedBytes:
        case bcmCosqStatOBMLossless1DroppedBytes:
        case bcmCosqStatOBMFlowControlEvents:
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_SCHEDULER(port) ||
                BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
                return BCM_E_PARAM;
            }
            if ((stat == bcmCosqStatHighPriDroppedPackets) ||
                (stat == bcmCosqStatOBMLossyHighPriDroppedPackets)) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM_LOSSY_HI;
            } else if ((stat == bcmCosqStatLowPriDroppedPackets) ||
                       (stat == bcmCosqStatOBMLossyLowPriDroppedPackets)) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM_LOSSY_LO;
            } else if ((stat == bcmCosqStatHighPriDroppedBytes) ||
                       (stat == bcmCosqStatOBMLossyHighPriDroppedBytes)) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM_LOSSY_HI;
            } else if ((stat == bcmCosqStatLowPriDroppedBytes) ||
                       (stat == bcmCosqStatOBMLossyLowPriDroppedBytes)) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM_LOSSY_LO;
            } else if (stat == bcmCosqStatOBMLossless0DroppedPackets) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM_LOSSLESS0;
            } else if (stat == bcmCosqStatOBMLossless1DroppedPackets) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM_LOSSLESS1;
            } else if (stat == bcmCosqStatOBMLossless0DroppedBytes) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM_LOSSLESS0;
            } else if (stat == bcmCosqStatOBMLossless1DroppedBytes) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM_LOSSLESS1;
            } else { /* (stat == bcmCosqStatOBMFlowControlEvents)  */
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_OBM_FC_EVENTS;
            }
            ctrl_info.instance_type = SOC_CTR_INSTANCE_TYPE_PORT;
            ctrl_info.instance = local_port;
            BCM_IF_ERROR_RETURN
                (soc_counter_generic_set(unit, ctr_reg, ctrl_info, 0,
                                         0, value));
            break;
        case bcmCosqStatOBMHighWatermark:
        case bcmCosqStatOBMBufferBytes:
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_SCHEDULER(port) ||
                BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
                return BCM_E_PARAM;
            }
            if (stat == bcmCosqStatOBMHighWatermark) {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_obmhighwatermark_set(unit, local_port, value));
            } else {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_cosq_obm_buf_use_cnt_set(unit, local_port, value));
            }
            break;
        default:
            return BCM_E_PARAM;
    }

    return BCM_E_NONE;
}

int
bcm_th_cosq_stat_get(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                     bcm_cosq_stat_t stat, int sync_mode, uint64 *value)
{
    soc_ctr_control_info_t ctrl_info;
    soc_reg_t ctr_reg, ctr_reg_uc, ctr_reg_mc;
    _bcm_th_cosq_index_style_t style = _BCM_TH_COSQ_INDEX_STYLE_COUNT;
    uint64 sum, value64;
    int startq = 0, ci, from_cos, to_cos, i, start_hw_index, end_hw_index;
    bcm_port_t local_port = -1;
    int sync = sync_mode ? SOC_COUNTER_SYNC_ENABLE : 0;

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, port, &local_port));
    ctrl_info.instance = -1;
    ctrl_info.instance_type = SOC_CTR_INSTANCE_TYPE_XPE;

    switch (stat) {
        case bcmCosqStatDroppedPackets:
        case bcmCosqStatDroppedBytes:
        case bcmCosqStatOutPackets:
        case bcmCosqStatOutBytes:
        case bcmCosqStatEgressMCQueueBytesPeak:
        case bcmCosqStatEgressMCQueueBytesCurrent:
        case bcmCosqStatEgressUCQueueBytesPeak:
        case bcmCosqStatEgressUCQueueBytesCurrent:
            if ((cosq < -1) || (cosq >= _TH_PORT_NUM_COS(unit, local_port))) {
                return BCM_E_PARAM;
            }
            style = _BCM_TH_COSQ_INDEX_STYLE_COS;
            if (stat == bcmCosqStatEgressMCQueueBytesPeak) {
                if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
                    BCM_GPORT_IS_SCHEDULER(port)) {
                    return BCM_E_PARAM;
                }
                ctr_reg_uc = INVALIDr;
                ctr_reg_mc = SOC_COUNTER_NON_DMA_QUEUE_PEAK;
            } else if (stat == bcmCosqStatEgressMCQueueBytesCurrent) {
                if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
                    BCM_GPORT_IS_SCHEDULER(port)) {
                    return BCM_E_PARAM;
                }
                ctr_reg_uc = INVALIDr;
                ctr_reg_mc = SOC_COUNTER_NON_DMA_QUEUE_CURRENT;
            } else if (stat == bcmCosqStatEgressUCQueueBytesPeak) {
                if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
                    BCM_GPORT_IS_SCHEDULER(port)) {
                    return BCM_E_PARAM;
                }
                if (!BCM_GPORT_IS_UCAST_QUEUE_GROUP(port)) {
                    BCM_IF_ERROR_RETURN
                        (_bcm_th_cosq_localport_resolve(unit, port,
                                                        &local_port));
                    if (IS_CPU_PORT(unit, local_port)) {
                        return BCM_E_PARAM;
                    }
                }
                ctr_reg_uc = SOC_COUNTER_NON_DMA_UC_QUEUE_PEAK;
                ctr_reg_mc = INVALIDr;
            } else if (stat == bcmCosqStatEgressUCQueueBytesCurrent) {
                if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
                    BCM_GPORT_IS_SCHEDULER(port)) {
                    return BCM_E_PARAM;
                }
                if (!BCM_GPORT_IS_UCAST_QUEUE_GROUP(port)) {
                    BCM_IF_ERROR_RETURN
                        (_bcm_th_cosq_localport_resolve(unit, port,
                                                        &local_port));
                    if (IS_CPU_PORT(unit, local_port)) {
                        return BCM_E_PARAM;
                    }
                }
                ctr_reg_uc = SOC_COUNTER_NON_DMA_UC_QUEUE_CURRENT;
                ctr_reg_mc = INVALIDr;
            } else if (stat == bcmCosqStatDroppedBytes) {
                ctr_reg_uc = SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE_UC;
                ctr_reg_mc = SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE;
            } else if (stat == bcmCosqStatDroppedPackets) {
                ctr_reg_uc = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT_UC;
                ctr_reg_mc = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT;
            } else if (stat == bcmCosqStatOutBytes) {
                ctr_reg_uc = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_BYTE_UC;
                ctr_reg_mc = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_BYTE;
                ctrl_info.instance_type = SOC_CTR_INSTANCE_TYPE_PORT;
            } else { /* (stat == bcmCosqStatOutPackets) */
                ctr_reg_uc = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_PKT_UC;
                ctr_reg_mc = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_PKT;
                ctrl_info.instance_type = SOC_CTR_INSTANCE_TYPE_PORT;
            }
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port)) {
                if (ctr_reg_uc != INVALIDr) {
                    BCM_IF_ERROR_RETURN
                        (_bcm_th_cosq_index_resolve
                         (unit, port, cosq, style, &local_port, &startq, NULL));

                    ctrl_info.instance = local_port;
                    BCM_IF_ERROR_RETURN
                        (soc_counter_generic_get(unit, ctr_reg_uc, ctrl_info,
                                                 sync, startq, value));
                }
            } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
                if (ctr_reg_mc != INVALIDr) {
                    BCM_IF_ERROR_RETURN
                        (_bcm_th_cosq_index_resolve
                         (unit, port, cosq, style, &local_port, &startq, NULL));
                    ctrl_info.instance = local_port;
                    BCM_IF_ERROR_RETURN
                        (soc_counter_generic_get(unit, ctr_reg_mc, ctrl_info,
                                                 sync, startq, value));
                }
            } else {
                COMPILER_64_ZERO(sum);
                if (BCM_GPORT_IS_SCHEDULER(port)) {
                    BCM_IF_ERROR_RETURN
                        (_bcm_th_cosq_index_resolve
                         (unit, port, cosq, style, &local_port, &startq, NULL));
                    from_cos = to_cos = startq;
                } else {
                    BCM_IF_ERROR_RETURN
                        (_bcm_th_cosq_localport_resolve(unit, port,
                                                        &local_port));

                    if (cosq == BCM_COS_INVALID) {
                        from_cos = 0;
                        to_cos = (IS_CPU_PORT(unit, local_port)) ?
                                    NUM_CPU_COSQ(unit) - 1 : _TH_NUM_COS(unit)
                                    - 1;
                    } else {
                        from_cos = to_cos = cosq;
                    }
                }
                ctrl_info.instance = local_port;
                for (ci = from_cos; ci <= to_cos; ci++) {

                    if (!IS_CPU_PORT(unit, local_port)) {
                        /* Get UC values if NOT CPU port */
                        if (ctr_reg_uc != INVALIDr) {
                            BCM_IF_ERROR_RETURN
                                (soc_counter_generic_get(unit, ctr_reg_uc,
                                                         ctrl_info,
                                                         sync, ci, &value64));
                            COMPILER_64_ADD_64(sum, value64);
                        }
                    }

                    if (ctr_reg_mc != INVALIDr) {
                        BCM_IF_ERROR_RETURN
                            (soc_counter_generic_get(unit, ctr_reg_mc,
                                                     ctrl_info,
                                                     sync, ci, &value64));
                        COMPILER_64_ADD_64(sum, value64);
                    }
                }
                *value = sum;
            }
            break;
        case bcmCosqStatYellowCongestionDroppedPackets:
        case bcmCosqStatRedCongestionDroppedPackets:
        case bcmCosqStatGreenDiscardDroppedPackets:
        case bcmCosqStatYellowDiscardDroppedPackets:
        case bcmCosqStatRedDiscardDroppedPackets:
            if ((cosq < -1) || (cosq >= _TH_PORT_NUM_COS(unit, local_port))) {
                return BCM_E_PARAM;
            }
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_SCHEDULER(port) ||
                BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
                return BCM_E_PARAM;
            }
            if (cosq != BCM_COS_INVALID) {
                return BCM_E_UNAVAIL;
            }
            if (stat == bcmCosqStatYellowCongestionDroppedPackets) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_PKT_YELLOW;
            } else if (stat == bcmCosqStatRedCongestionDroppedPackets) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_PKT_RED;
            } else if (stat == bcmCosqStatYellowDiscardDroppedPackets) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_WRED_PKT_YELLOW;
            } else if (stat == bcmCosqStatRedDiscardDroppedPackets) {
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_WRED_PKT_RED;
            } else { /* (stat == bcmCosqStatGreenDiscardDroppedPackets) */
                ctr_reg = SOC_COUNTER_NON_DMA_PORT_WRED_PKT_GREEN;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, port, &local_port));

            ctrl_info.instance = local_port;
            BCM_IF_ERROR_RETURN
                (soc_counter_generic_get(unit, ctr_reg, ctrl_info,
                                         sync, startq, value));
            break;
        case bcmCosqStatIngressPortPGMinBytesPeak:
        case bcmCosqStatIngressPortPGMinBytesCurrent:
        case bcmCosqStatIngressPortPGSharedBytesPeak:
        case bcmCosqStatIngressPortPGSharedBytesCurrent:
        case bcmCosqStatIngressPortPGHeadroomBytesPeak:
        case bcmCosqStatIngressPortPGHeadroomBytesCurrent:
            if ( cosq < -1 ) {
                return BCM_E_PARAM;
            }
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_SCHEDULER(port) ||
                BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
                return BCM_E_PARAM;
            }
            COMPILER_64_ZERO(sum);
            if (stat == bcmCosqStatIngressPortPGMinBytesPeak) {
                ctr_reg = SOC_COUNTER_NON_DMA_PG_MIN_PEAK;
            } else if (stat == bcmCosqStatIngressPortPGMinBytesCurrent) {
                ctr_reg = SOC_COUNTER_NON_DMA_PG_MIN_CURRENT;
            } else if (stat == bcmCosqStatIngressPortPGSharedBytesPeak) {
                ctr_reg = SOC_COUNTER_NON_DMA_PG_SHARED_PEAK;
            } else if (stat == bcmCosqStatIngressPortPGSharedBytesCurrent) {
                ctr_reg = SOC_COUNTER_NON_DMA_PG_SHARED_CURRENT;
            } else if (stat == bcmCosqStatIngressPortPGHeadroomBytesPeak) {
                ctr_reg = SOC_COUNTER_NON_DMA_PG_HDRM_PEAK;
            } else {
                /* (stat == bcmCosqStatIngressPortPGHeadroomBytesCurrent) */
                ctr_reg = SOC_COUNTER_NON_DMA_PG_HDRM_CURRENT;
            }
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_localport_resolve(unit, port, &local_port));
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_ingress_pg_get(unit, port, cosq,
                                             &start_hw_index, &end_hw_index));

            for (i = start_hw_index; i <= end_hw_index; i++) {
                ctrl_info.instance = local_port;
                BCM_IF_ERROR_RETURN
                    (soc_counter_generic_get(unit, ctr_reg, ctrl_info,
                                             sync, i, &value64));
                COMPILER_64_ADD_64(sum, value64);
            }

            COMPILER_64_UMUL_32(sum, _TH_MMU_BYTES_PER_CELL);
            *value = sum;
            break;
    case bcmCosqStatIngressPortPoolSharedBytesPeak:
        if ( cosq < -1 ) {
            return BCM_E_PARAM;
        }
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
            BCM_GPORT_IS_SCHEDULER(port) ||
            BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
            return BCM_E_PARAM;
        }
        COMPILER_64_ZERO(sum);
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_ingress_sp_get_by_pg(unit, port, cosq,
                                         &start_hw_index, &end_hw_index));

        BCM_IF_ERROR_RETURN(
            _bcm_th_cosq_localport_resolve(unit, port, &local_port));

        ctrl_info.instance_type = SOC_CTR_INSTANCE_TYPE_XPE;
        ctrl_info.instance = local_port;
        ctr_reg = SOC_COUNTER_NON_DMA_POOL_PEAK;

        for (i = start_hw_index; i <= end_hw_index; i++) {
            BCM_IF_ERROR_RETURN
                (soc_counter_generic_get(unit, ctr_reg, ctrl_info,
                                         sync, i, &value64));
            COMPILER_64_ADD_64(sum, value64);
        }

        COMPILER_64_UMUL_32(sum, _TH_MMU_BYTES_PER_CELL);
        *value = sum;
        break;
    case bcmCosqStatIngressPortPoolSharedBytesCurrent:
        if ( cosq < -1 ) {
            return BCM_E_PARAM;
        }
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
            BCM_GPORT_IS_SCHEDULER(port) ||
            BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
            return BCM_E_PARAM;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_ingress_sp_get_by_pg(unit, port, cosq,
                                         &start_hw_index, &end_hw_index));
        BCM_IF_ERROR_RETURN(
            _bcm_th_cosq_localport_resolve(unit, port, &local_port));

        ctrl_info.instance_type = SOC_CTR_INSTANCE_TYPE_XPE;
        ctrl_info.instance = local_port;
        ctr_reg = SOC_COUNTER_NON_DMA_POOL_CURRENT;

        COMPILER_64_ZERO(sum);
        for (i = start_hw_index; i <= end_hw_index; i++) {
            BCM_IF_ERROR_RETURN
                (soc_counter_generic_get(unit, ctr_reg, ctrl_info,
                                         sync, i, &value64));
           COMPILER_64_ADD_64(sum, value64);
        }

        COMPILER_64_UMUL_32(sum, _TH_MMU_BYTES_PER_CELL);
        *value = sum;
        break;
    case bcmCosqStatHighPriDroppedPackets:
    case bcmCosqStatOBMLossyHighPriDroppedPackets:
    case bcmCosqStatLowPriDroppedPackets:
    case bcmCosqStatOBMLossyLowPriDroppedPackets:
    case bcmCosqStatHighPriDroppedBytes:
    case bcmCosqStatOBMLossyHighPriDroppedBytes:
    case bcmCosqStatLowPriDroppedBytes:
    case bcmCosqStatOBMLossyLowPriDroppedBytes:
    case bcmCosqStatOBMLossless0DroppedPackets:
    case bcmCosqStatOBMLossless1DroppedPackets:
    case bcmCosqStatOBMLossless0DroppedBytes:
    case bcmCosqStatOBMLossless1DroppedBytes:
    case bcmCosqStatOBMFlowControlEvents:
        if ((cosq < -1) || (cosq >= _TH_PORT_NUM_COS(unit, local_port))) {
            return BCM_E_PARAM;
        }
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
            BCM_GPORT_IS_SCHEDULER(port) ||
            BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
            return BCM_E_PARAM;
        }
        if ((stat == bcmCosqStatHighPriDroppedPackets) ||
            (stat == bcmCosqStatOBMLossyHighPriDroppedPackets)) {
            ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM_LOSSY_HI;
        } else if ((stat == bcmCosqStatLowPriDroppedPackets) ||
                   (stat == bcmCosqStatOBMLossyLowPriDroppedPackets)) {
            ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM_LOSSY_LO;
        } else if ((stat == bcmCosqStatHighPriDroppedBytes) ||
                   (stat == bcmCosqStatOBMLossyHighPriDroppedBytes)) {
            ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM_LOSSY_HI;
        } else if ((stat == bcmCosqStatLowPriDroppedBytes) ||
                   (stat == bcmCosqStatOBMLossyLowPriDroppedBytes)) {
            ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM_LOSSY_LO;
        } else if (stat == bcmCosqStatOBMLossless0DroppedPackets) {
            ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM_LOSSLESS0;
        } else if (stat == bcmCosqStatOBMLossless1DroppedPackets) {
            ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM_LOSSLESS1;
        } else if (stat == bcmCosqStatOBMLossless0DroppedBytes) {
            ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM_LOSSLESS0;
        } else if (stat == bcmCosqStatOBMLossless1DroppedBytes) {
            ctr_reg = SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM_LOSSLESS1;
        } else { /* (stat == bcmCosqStatOBMFlowControlEvents)  */
            ctr_reg = SOC_COUNTER_NON_DMA_PORT_OBM_FC_EVENTS;
        }
        ctrl_info.instance_type = SOC_CTR_INSTANCE_TYPE_PORT;
        ctrl_info.instance = local_port;
        BCM_IF_ERROR_RETURN
            (soc_counter_generic_get(unit, ctr_reg, ctrl_info, 0,
                                     0, value));
        break;
    case bcmCosqStatOBMHighWatermark:
    case bcmCosqStatOBMBufferBytes:
        if ((cosq < -1) || (cosq >= _TH_PORT_NUM_COS(unit, local_port))) {
            return BCM_E_PARAM;
        }
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
            BCM_GPORT_IS_SCHEDULER(port) ||
            BCM_GPORT_IS_MCAST_QUEUE_GROUP(port)) {
            return BCM_E_PARAM;
        }
        if (stat == bcmCosqStatOBMHighWatermark) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_obmhighwatermark_get(unit, local_port, value));
        } else {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_obm_buf_use_cnt_get(unit, local_port, value));
        }
        break;
        default:
            return BCM_E_PARAM;
    }

    return BCM_E_NONE;
}


/*
 * Function:
 *     bcm_th_cosq_bst_profile_set
 * Purpose:
 *     Set the BST profile of a given BST stat id.
 * Parameters:
 *     unit     - (IN) unit number
 *     gport    - (IN) gport of the port
 *     cosq     - (IN) COS queue
 *     bid      - (IN) BST statistics ID
 *     profile  - (IN) profile for given BST ID
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_bst_profile_set(int unit,
                            bcm_gport_t gport,
                            bcm_cos_queue_t cosq,
                            bcm_bst_stat_id_t bid,
                            bcm_cosq_bst_profile_t *profile)
{
    if (bid == bcmBstStatIdHeadroomPool) {
        /* Do not have BST profile to set */
        return BCM_E_PARAM;
    } else {
        return _bcm_bst_cmn_profile_set
            (unit, gport, cosq, BCM_BST_DUP_XPE, bid, profile);
    }
}

/*
 * Function:
 *     bcm_th_cosq_bst_profile_get
 * Purpose:
 *     Get the BST profile of a given BST stat id.
 * Parameters:
 *     unit     - (IN) unit number
 *     gport    - (IN) gport of the port
 *     cosq     - (IN) COS queue
 *     bid      - (IN) BST statistics ID
 *     profile  - (OUT) profile retrieved for given BST ID
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_bst_profile_get(int unit,
                            bcm_gport_t gport,
                            bcm_cos_queue_t cosq,
                            bcm_bst_stat_id_t bid,
                            bcm_cosq_bst_profile_t *profile)
{
    if (bid == bcmBstStatIdHeadroomPool) {
        /* Do not have BST profile to get */
        return BCM_E_PARAM;
    } else {
        return _bcm_bst_cmn_profile_get
            (unit, gport, cosq, BCM_BST_DUP_XPE, bid, profile);
    }
}

/*
 * Function:
 *     bcm_th_cosq_bst_multi_profile_set
 * Purpose:
 *     Set the BST profiles of a given BST stat id, per XPE.
 * Parameters:
 *     unit     - (IN) unit number
 *     gport    - (IN) gport of the port
 *     cosq     - (IN) COS queue
 *     bid      - (IN) BST statistics ID
 *     size     - (IN) size of the profile array
 *     profile  - (IN) profile array for given BST ID, index with XPE
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_bst_multi_profile_set(int unit,
                            bcm_gport_t gport,
                            bcm_cos_queue_t cosq,
                            bcm_bst_stat_id_t bid,
                            int array_size,
                            bcm_cosq_bst_profile_t *profile_array)
{
    int cnt;
    bcm_cosq_bst_profile_t profile[_TH_XPES_PER_DEV];

    if ((!profile_array) || (array_size <=0)) {
        return BCM_E_PARAM;
    }
    if (bid == bcmBstStatIdHeadroomPool) {
        /* Do not have BST profile to set */
        return BCM_E_PARAM;
    }
    if ((bid == bcmBstStatIdDevice)
        || (bid == bcmBstStatIdUcast)
        || (bid == bcmBstStatIdEgrPortPoolSharedUcast)) {
        return BCM_E_UNAVAIL;
    }

    cnt = NUM_XPE(unit);
    if (array_size < cnt) {
        cnt = array_size;
    }
    sal_memset(profile, 0, sizeof(profile));
    sal_memcpy(profile, profile_array, cnt*sizeof(bcm_cosq_bst_profile_t));
    
    return _bcm_bst_cmn_profile_set
        (unit, gport, cosq, BCM_BST_MLT_XPE, bid, profile);
}

/*
 * Function:
 *     bcm_th_cosq_bst_multi_profile_get
 * Purpose:
 *     Get the BST profiles of a given BST stat id, per XPE.
 * Parameters:
 *     unit     - (IN) unit number
 *     gport    - (IN) gport of the port
 *     cosq     - (IN) COS queue
 *     bid      - (IN) BST statistics ID
 *     size     - (IN) max size of the profile array
 *     profile  - (OUT) profile array retrieved for given BST ID, index with XPE
 *     count    - (OUT) actual size of the profile array
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_bst_multi_profile_get(int unit,
                            bcm_gport_t gport,
                            bcm_cos_queue_t cosq,
                            bcm_bst_stat_id_t bid,
                            int array_size,
                            bcm_cosq_bst_profile_t *profile_array,
                            int *count)
{
    int cnt;
    bcm_cosq_bst_profile_t profile[_TH_XPES_PER_DEV];
    
    if (bid == bcmBstStatIdHeadroomPool) {
        /* Do not have BST profile to get */
        return BCM_E_PARAM;
    }
    if ((bid == bcmBstStatIdDevice)
        || (bid == bcmBstStatIdUcast)
        || (bid == bcmBstStatIdEgrPortPoolSharedUcast)) {
        return BCM_E_UNAVAIL;
    }

    cnt = NUM_XPE(unit);
    if ((array_size > 0) && (array_size < cnt)) {
        cnt = array_size;
    }
    if (count) {
        *count  = cnt;
    }
    
    if ((array_size == 0) && (!profile_array)) {
        return BCM_E_NONE;
    } else if ((array_size > 0) && (profile_array)) {
        sal_memset(profile, 0, sizeof(profile));
        BCM_IF_ERROR_RETURN(_bcm_bst_cmn_profile_get
            (unit, gport, cosq, BCM_BST_MLT_XPE, bid, profile));
        sal_memcpy(profile_array, profile, cnt*sizeof(bcm_cosq_bst_profile_t));
        return BCM_E_NONE;
    } else {
        return BCM_E_PARAM;
    }
}

/*
 * Function:
 *     bcm_th_cosq_bst_stat_get
 * Purpose:
 *     Get the statistics of a given BST stat id.
 * Parameters:
 *     unit     - (IN) unit number
 *     gport    - (IN) gport of the port
 *     cosq     - (IN) COS queue
 *     buffer   - (IN) Buffer ID
 *     bid      - (IN) BST statistics ID
 *     options  - (IN) Clear-on-read or not
 *     pvalues  - (OUT) values retrieved for given BST ID
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_bst_stat_get(int unit,
                         bcm_gport_t gport,
                         bcm_cos_queue_t cosq,
                         bcm_cosq_buffer_id_t buffer,
                         bcm_bst_stat_id_t bid,
                         uint32 options,
                         uint64 *pvalue)
{
    if (bid == bcmBstStatIdHeadroomPool) {
        return _bcm_th_cosq_bst_hdrm_stat_get
            (unit, gport, cosq, buffer, options, pvalue);
    } else {
        return _bcm_bst_cmn_stat_get(unit, gport, cosq, bid, options, pvalue);
    }
}

/*
 * Function:
 *     bcm_th_cosq_bst_stat_multi_get
 * Purpose:
 *     Get the statistics of a given BST stat id.
 * Parameters:
 *     unit         - (IN) unit number
 *     gport        - (IN) gport of the port
 *     cosq         - (IN) COS queue
 *     options      - (IN) Clear-on-read or not
 *     max_values   - (IN) max value in id_list.
 *     id_list      - (IN) IDs to retrieve.
 *     pvalues      - (OUT) values retrieved for id_list
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_bst_stat_multi_get(int unit,
                               bcm_gport_t gport,
                               bcm_cos_queue_t cosq,
                               uint32 options,
                               int max_values,
                               bcm_bst_stat_id_t *id_list,
                               uint64 *pvalues)
{
    int i;

    for (i = 0; i < max_values; i++) {
        if (id_list[i] == bcmBstStatIdHeadroomPool) {
            BCM_IF_ERROR_RETURN
                (_bcm_th_cosq_bst_hdrm_stat_get(unit, gport, cosq, 
                    BCM_COSQ_BUFFER_ID_INVALID, options, &pvalues[i]));
        } else {
            BCM_IF_ERROR_RETURN(_bcm_bst_cmn_stat_get(unit, gport, cosq,
                                id_list[i], options, &pvalues[i]));
        }
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_bst_stat_clear
 * Purpose:
 *     Clear the statistics from both hardware and
 *     software for a given BST stat id.
 * Parameters:
 *     unit     - (IN) unit number
 *     gport    - (IN) gport of the port
 *     cosq     - (IN) COS queue
 *     buffer   - (IN) Buffer ID
 *     bid      - (IN) BST statistics ID
 * Returns:
 *     BCM_E_XXX
 */

int bcm_th_cosq_bst_stat_clear(int unit, bcm_gport_t gport,
                               bcm_cos_queue_t cosq, bcm_cosq_buffer_id_t buffer,
                               bcm_bst_stat_id_t bid)
{
    if (bid == bcmBstStatIdHeadroomPool) {
        return _bcm_th_bst_hdrm_stat_clear(unit, gport, cosq, buffer);
    } else {
        return _bcm_bst_cmn_stat_clear(unit, gport, cosq, bid);
    }
}

int bcm_th_cosq_bst_stat_sync(int unit, bcm_bst_stat_id_t bid)
{
    return _bcm_bst_cmn_stat_sync(unit, bid);
}

/*
 * Function:
 *     _bcm_th_port_enqueue_set
 * Purpose:
 *      Set the enqueue state of the port
 * Parameters:
 *     unit       - (IN) unit number
 *     gport      - (IN) gport of the port
 *     enable     - (IN) TRUE to enable
 *                      FALSE to disable
 * Returns:
 *     BCM_E_XXX
 */

int
_bcm_th_port_enqueue_set(int unit, bcm_port_t gport, int enable)
{
    int         rv = BCM_E_NONE;
    pbmp_t      mask;
    bcm_port_t port;
    uint64 ctrl;

    BCM_IF_ERROR_RETURN(
    _bcm_esw_port_gport_validate(unit, gport, &port));

    PORT_LOCK(unit);
    SOC_IF_ERROR_RETURN(READ_CLMAC_CTRLr(unit, port, &ctrl));

    /* Don't disable TX since it stops egress and hangs if CPU sends */
    soc_reg64_field32_set(unit, CLMAC_CTRLr, &ctrl, TX_ENf, 1);
    soc_reg64_field32_set(unit, CLMAC_CTRLr, &ctrl, RX_ENf, enable ? 1 : 0);

    if (enable) {
        /* Deassert SOFT_RESET */
        soc_reg64_field32_set(unit, CLMAC_CTRLr, &ctrl, SOFT_RESETf, 0);
        /* Deassert EGR_XLPORT_BUFFER_SFT_RESET */
        rv = soc_tomahawk_edb_buf_reset(unit, port, 0);
        if (BCM_FAILURE(rv)) {
            PORT_UNLOCK(unit);
            return (rv);
        }
        /* Release Ingress buffers from reset */
        rv = soc_tomahawk_idb_buf_reset(unit, port, 0);
        if (BCM_FAILURE(rv)) {
            PORT_UNLOCK(unit);
            return (rv);
        }

        rv = WRITE_CLMAC_CTRLr(unit, port, ctrl);
        if (BCM_FAILURE(rv)) {
            PORT_UNLOCK(unit);
            return (rv);
        }

#ifdef  BCM_TOMAHAWKPLUS_SUPPORT
        /* Special handling for new mac version for TH+. Internally MAC
         * loopback looks for rising edge on MAC loopback configuration
         * to enter loopback state.
         */
        if (SOC_IS_TOMAHAWKPLUS(unit)) {
            uint32 ctrl_32;
            COMPILER_64_TO_32_LO(ctrl_32, ctrl);
            if (ctrl_32 & 0x4) {
                /* Do only if loopback bit is intented to be set */
                soc_reg_field32_modify(unit, CLMAC_CTRLr, port, LOCAL_LPBKf, 0);
                sal_usleep(10);
                /* Wait 10usec as suggested by MAC designer */
                soc_reg_field32_modify(unit, CLMAC_CTRLr, port, LOCAL_LPBKf, 1);
            }
        }
#endif /* BCM_TOMAHAWKPLUS_SUPPORT */

        soc_link_mask2_get(unit, &mask);
        SOC_PBMP_PORT_ADD(mask, port);
        rv = soc_link_mask2_set(unit, mask);
        if (BCM_FAILURE(rv)) {
            PORT_UNLOCK(unit);
            return (rv);
        }

    } else {
        /* Disable MAC RX */
        rv = WRITE_CLMAC_CTRLr(unit, port, ctrl);
        if (BCM_FAILURE(rv)) {
            PORT_UNLOCK(unit);
            return (rv);
        }
        soc_link_mask2_get(unit, &mask);
        SOC_PBMP_PORT_REMOVE(mask, port);
        rv = soc_link_mask2_set(unit, mask);
        if (BCM_FAILURE(rv)) {
            PORT_UNLOCK(unit);
            return (rv);
        }
        /* Delay to ensure EOP is received at Ingress */
        sal_udelay(1000);

        /* Reset Ingress buffers */
        rv = soc_tomahawk_idb_buf_reset(unit, port, 1);
        if (BCM_FAILURE(rv)) {
            PORT_UNLOCK(unit);
            return (rv);
        }
        /* Reset egress_buffer */
        rv = soc_tomahawk_edb_buf_reset(unit, port, 1);
        if (BCM_FAILURE(rv)) {
            PORT_UNLOCK(unit);
            return (rv);
        }
        /* Put port into SOFT_RESET */
        soc_reg64_field32_set(unit, CLMAC_CTRLr, &ctrl, SOFT_RESETf, 1);
        rv = WRITE_CLMAC_CTRLr(unit, port, ctrl);
        if (BCM_FAILURE(rv)) {
            PORT_UNLOCK(unit);
            return (rv);
        }

    }

    PORT_UNLOCK(unit);
    return BCM_E_NONE;

}

/*
 * Function:
 *     _bcm_th_port_enqueue_get
 * Purpose:
 *      Getting the enqueing state of the port
 * Parameters:
 *     unit       - (IN) unit number
 *     gport      - (IN) gport of the port
 *     enable     - (IN) TRUE to enable
 *                      FALSE to disable
 * Returns:
 *     BCM_E_XXX
 */

int
_bcm_th_port_enqueue_get(int unit, bcm_port_t gport,
                         int *enable)
{
    return BCM_E_UNAVAIL;
}

/*
 * Function:
 *     bcm_th_cosq_safc_class_mapping_set
 * Purpose:
 *     Set SAFC mapping for port.
 *     SAFC class is mapped to cosq or scheduler node gports.
 * Parameters:
 *     unit             - (IN) unit number
 *     port             - (IN) port number
 *     array_count      - (IN) count of mapping
 *     mapping_array    - (IN) mappings
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_safc_class_mapping_set(int unit, bcm_gport_t port,
                                   int array_count,
                                   bcm_cosq_safc_class_mapping_t *mapping_array)
{
    int index, count;
    int cur_class = 0;
    int safc_class[_BCM_TH_NUM_SAFC_CLASS];
    bcm_gport_t cur_gport = 0;
    uint32 uc_cos_bmp[_BCM_TH_NUM_SAFC_CLASS] = {0};
    uint32 mc_cos_bmp[_BCM_TH_NUM_SAFC_CLASS] = {0};
    bcm_port_t local_port = -1;

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, port, &local_port));

    if (local_port < 0) {
        return BCM_E_PORT;
    }

    if (IS_CPU_PORT(unit, local_port)) {
        return BCM_E_PARAM;
    }

    if ((array_count < 0) || (array_count > _BCM_TH_NUM_SAFC_CLASS)) {
        return BCM_E_PARAM;
    }

    if (mapping_array == NULL) {
        return BCM_E_PARAM;
    }

    for (count = 0; count < array_count; count++) {
        cur_class = mapping_array[count].class_id;
        if ((cur_class < 0) || (cur_class >= _BCM_TH_NUM_SAFC_CLASS)) {
            return BCM_E_PARAM;
        } else {
            uc_cos_bmp[cur_class] = 0;
            mc_cos_bmp[cur_class] = 0;
        }

        for (index = 0; index < BCM_COSQ_SAFC_GPORT_COUNT; index++) {
            cur_gport = mapping_array[count].gport_list[index];
            if (cur_gport == BCM_GPORT_INVALID) {
                break;
            }

            if ((BCM_GPORT_IS_UCAST_QUEUE_GROUP(cur_gport)) ||
                (BCM_GPORT_IS_MCAST_QUEUE_GROUP(cur_gport))) {
                BCM_IF_ERROR_RETURN
                    (_bcm_th_fc_status_map_gport(unit, local_port, cur_gport,
                                                 &uc_cos_bmp[cur_class],
                                                 &mc_cos_bmp[cur_class]));
            } else {
                return BCM_E_PARAM;
            }
        }
    }

    for (index = 0; index < _BCM_TH_NUM_SAFC_CLASS; index++) {
        safc_class[index] = index;
    }
    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_port_fc_profile_set(unit, local_port, safc_class,
                                          uc_cos_bmp, mc_cos_bmp,
                                          _BCM_TH_NUM_SAFC_CLASS));

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_th_cosq_safc_class_mapping_get
 * Purpose:
 *     Get SAFC mapping for port.
 *     Retrieves cosq or scheduler node gports
 *     associated to SAFC classes.
 * Parameters:
 *     unit             - (IN) unit number
 *     port             - (IN) port number
 *     array_max        - (IN) max retrieve count
 *     mapping_array    - (OUT) mappings
 *     array_count      - (OUT) retrieved mapping count
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_th_cosq_safc_class_mapping_get(int unit, bcm_gport_t port,
                                   int array_max,
                                   bcm_cosq_safc_class_mapping_t *mapping_array,
                                   int *array_count)
{
    int rv = 0;
    int cur_class = 0;
    int actual_gport_count, class_count = 0;
    bcm_port_t local_port = -1;

    BCM_IF_ERROR_RETURN
        (_bcm_th_cosq_localport_resolve(unit, port, &local_port));

    if (local_port < 0) {
        return BCM_E_PORT;
    }

    if (IS_CPU_PORT(unit, local_port)) {
        return BCM_E_PARAM;
    }

    if (((mapping_array == NULL) && (array_max > 0)) ||
        ((mapping_array != NULL) && (array_max <= 0)) ||
        (array_count == NULL)) {
        return BCM_E_PARAM;
    }

    if (array_max <= 0) {
        *array_count = _BCM_TH_NUM_SAFC_CLASS;
        return BCM_E_NONE;
    }

    for (cur_class = 0; cur_class < _BCM_TH_NUM_SAFC_CLASS; cur_class++) {
        actual_gport_count = 0;
        rv = _bcm_th_cosq_port_fc_get(unit, local_port, cur_class,
                                      mapping_array[class_count].gport_list,
                                      &actual_gport_count);

        if (rv == BCM_E_NONE) {
            mapping_array[class_count].class_id = cur_class;
            if (actual_gport_count < BCM_COSQ_SAFC_GPORT_COUNT) {
                mapping_array[class_count].gport_list[actual_gport_count] =
                                                        BCM_GPORT_INVALID;
            }
            class_count++;
        } else if (rv != BCM_E_NOT_FOUND) {
            return rv;
        }

        if (class_count == array_max) {
            break;
        }
    }

    *array_count = class_count;
    if (class_count == 0) {
        return BCM_E_NOT_FOUND;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_th_cosq_cpu_cosq_enable_set
 * Purpose:
 *      To enable/disable Rx of packets on the specified CPU cosq.
 * Parameters:
 *      unit          - (IN) Unit number.
 *      cosq          - (IN) CPU Cosq ID
 *      enable        - (IN) Enable/Disable
 * Returns:
 *      BCM_E_xxx
 */
int
bcm_th_cosq_cpu_cosq_enable_set(int unit, bcm_cos_queue_t cosq, int enable)
{
    uint32 entry[SOC_MAX_MEM_WORDS];
    int i, index, enable_status;
    _bcm_th_cosq_cpu_cosq_config_t *cpu_cosq_config;
    soc_info_t *si;
    soc_mem_t thdm_mem[] = {
        MMU_THDM_DB_QUEUE_CONFIG_PIPE0m,
        MMU_THDM_MCQE_QUEUE_CONFIG_PIPE0m
    };

    si = &SOC_INFO(unit);

    if ((cosq < 0) || (cosq >= NUM_CPU_COSQ(unit))) {
        return BCM_E_PARAM;
    }

    cpu_cosq_config = _bcm_th_cosq_cpu_cosq_config[unit][cosq];

    if (!cpu_cosq_config) {
        return BCM_E_INTERNAL;
    }
    if (enable) {
        enable = 1;
    }

    BCM_IF_ERROR_RETURN(bcm_th_cosq_cpu_cosq_enable_get(unit, cosq,
                                                         &enable_status));
    if (enable == enable_status) {
        return BCM_E_NONE;
    }

    /* CPU Port (0) - MC cosq's range from 330 - 378. */
    index = si->port_cosq_base[CMIC_PORT(unit)] + cosq;

    /* To set DB and QE entries */
    for (i = 0; i < 2; i++) {
        BCM_IF_ERROR_RETURN
            (soc_mem_read(unit, thdm_mem[i], MEM_BLOCK_ALL, index, &entry));

        /* When enabled, restore the Min and Shared from the cached value */
        if (!enable) {
            cpu_cosq_config->q_min_limit[i] = soc_mem_field32_get(unit,
                                                           thdm_mem[i],
                                                           &entry,
                                                           Q_MIN_LIMITf);
            cpu_cosq_config->q_shared_limit[i] = soc_mem_field32_get(unit,
                                                              thdm_mem[i],
                                                              &entry,
                                                              Q_SHARED_LIMITf);
            cpu_cosq_config->q_limit_dynamic[i] = soc_mem_field32_get(unit,
                                                               thdm_mem[i],
                                                               &entry,
                                                               Q_LIMIT_DYNAMICf);
            cpu_cosq_config->q_limit_enable[i] = soc_mem_field32_get(unit,
                                                              thdm_mem[i],
                                                              &entry,
                                                              Q_LIMIT_ENABLEf);
            cpu_cosq_config->q_color_limit_enable[i] = soc_mem_field32_get(unit,
                                                              thdm_mem[i],
                                                              &entry,
                                                              Q_COLOR_LIMIT_ENABLEf);
        }
        soc_mem_field32_set(unit, thdm_mem[i], &entry, Q_LIMIT_DYNAMICf,
                            enable ? cpu_cosq_config->q_limit_dynamic[i] : 0);
        soc_mem_field32_set(unit, thdm_mem[i], &entry, Q_LIMIT_ENABLEf,
                            enable ? cpu_cosq_config->q_limit_enable[i] : 1);
        soc_mem_field32_set(unit, thdm_mem[i], &entry, Q_COLOR_LIMIT_ENABLEf,
                            enable ? cpu_cosq_config->q_color_limit_enable[i] : 1);
        soc_mem_field32_set(unit, thdm_mem[i], &entry, Q_MIN_LIMITf,
                            enable ? cpu_cosq_config->q_min_limit[i] : 0);
        soc_mem_field32_set(unit, thdm_mem[i], &entry, Q_SHARED_LIMITf,
                            enable ? cpu_cosq_config->q_shared_limit[i] : 0);
        cpu_cosq_config->enable = enable;
        BCM_IF_ERROR_RETURN(
            soc_mem_write(unit, thdm_mem[i], MEM_BLOCK_ALL, index, &entry));
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_th_cosq_cpu_cosq_enable_get
 * Purpose:
 *      To get enable/disable status on Rx of packets on the specified CPU cosq.
 * Parameters:
 *      unit          - (IN) Unit number.
 *      cosq          - (IN) CPU Cosq ID
 *      enable        - (OUT) Enable/Disable
 * Returns:
 *      BCM_E_xxx
 */
int
bcm_th_cosq_cpu_cosq_enable_get(int unit, bcm_cos_queue_t cosq, int *enable)
{
    _bcm_th_cosq_cpu_cosq_config_t *cpu_cosq;
    soc_info_t *si;
    int index, hw_enable;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_field_t min_limit, shared_limit, limit_enable, dynamic_enable;
    soc_mem_t thdm_mem = MMU_THDM_DB_QUEUE_CONFIG_PIPE0m;

    si = &SOC_INFO(unit);

    if ((cosq < 0) || (cosq >= NUM_CPU_COSQ(unit))) {
        return BCM_E_PARAM;
    }

    cpu_cosq = _bcm_th_cosq_cpu_cosq_config[unit][cosq];
    if (!cpu_cosq) {
        return BCM_E_INTERNAL;
    }

    /* CPU Port (0) - MC cosq's range from 330 - 378. */
    index = si->port_cosq_base[CMIC_PORT(unit)] + cosq;
    hw_enable = 1;

    /* Sync up software record with hardware status. */
    BCM_IF_ERROR_RETURN(soc_mem_read(unit, thdm_mem, MEM_BLOCK_ALL,
                                     index, &entry));
    min_limit = soc_mem_field32_get(unit, thdm_mem, &entry,
                                    Q_MIN_LIMITf);
    shared_limit = soc_mem_field32_get(unit, thdm_mem, &entry,
                                    Q_SHARED_LIMITf);
    limit_enable = soc_mem_field32_get(unit, thdm_mem, &entry,
                                    Q_LIMIT_ENABLEf);
    dynamic_enable = soc_mem_field32_get(unit, thdm_mem, &entry,
                                    Q_LIMIT_DYNAMICf);
    if ((limit_enable != 0) && (dynamic_enable == 0) &&
        (min_limit == 0) && (shared_limit == 0)) {
        hw_enable = 0;
    }
    cpu_cosq->enable = hw_enable;

    *enable = cpu_cosq->enable;
    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_th2_cosq_buffer_id_multi_get
 * Purpose:
 *      Get mutliple buffer ids associated with a specified port.
 * Parameters:
 *      unit - (IN) BCM device number.
 *      gport - (IN) Port     
 *      cosq - (IN) Reserved field
 *      direction - (IN) Specify ingress or egress direction
 *      array_max - (IN) number of entries to be retrieved  
 *      buf_id_array - (OUT) Buffer id array
 *      array_count - (OUT) Actural buffer id count in buf_id_array 
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_th_cosq_buffer_id_multi_get(int unit, bcm_gport_t gport,
                              bcm_cos_queue_t cosq, bcm_cosq_dir_t direction,
                              int array_max, bcm_cosq_buffer_id_t *buf_id_array,
                              int *array_count)
{
    uint8 ipipe2buf[4][2] = {{0, 1}, {2, 3}, {2, 3}, {0, 1}};
    uint8 epipe2buf[4][2] = {{0, 2}, {0, 2}, {1, 3}, {1, 3}};

    bcm_port_t local_port;
    int pipe, i;

    if ((direction < bcmCosqIngress) ||
        (direction >= bcmCosqDirectionMaxCount)) {
        return BCM_E_PARAM;
    }

    if (!BCM_GPORT_IS_SET(gport)) {
        local_port = gport;
    } else if (BCM_GPORT_IS_LOCAL(gport)) {
        local_port = BCM_GPORT_LOCAL_GET(gport);
    } else if (BCM_GPORT_IS_MODPORT(gport)) {
        BCM_IF_ERROR_RETURN(bcm_esw_port_local_get(unit, gport, &local_port));
    } else if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
        int idx;
        BCM_IF_ERROR_RETURN(_bcm_th_cosq_index_resolve(unit, gport, cosq,
            _BCM_TH_COSQ_INDEX_STYLE_UCAST_QUEUE, &local_port, &idx, NULL));
    } else {
        return BCM_E_PARAM;
    }

    if (!SOC_PORT_VALID(unit, local_port)) {
        return BCM_E_PORT;
    }

    SOC_IF_ERROR_RETURN(soc_port_pipe_get(unit, local_port, &pipe));

    if (array_max > _BCM_TH_NUM_BUFFER_PER_PIPE) {
        *array_count = _BCM_TH_NUM_BUFFER_PER_PIPE;
    } else if (array_max >= 0){
        *array_count = array_max;
    } else {
        return BCM_E_PARAM;
    }

    for (i = 0; i < *array_count; i++) {
        if (bcmCosqIngress == direction) {
            buf_id_array[i] = ipipe2buf[pipe][i];
        } else {
            buf_id_array[i] = epipe2buf[pipe][i];
        }
    }
    for (i = *array_count; i < array_max; i++) {
        buf_id_array[i] = BCM_COSQ_INVALID_BUF_ID;
    }

    return BCM_E_NONE;
}



#ifdef BCM_WARM_BOOT_SUPPORT
int
bcm_th_cosq_reinit(int unit)
{
    soc_scache_handle_t scache_handle;
    uint8 *scache_ptr = NULL;
    int *int_scache_ptr = NULL;
    uint16 *u16_scache_ptr = NULL;
    _bcm_th_mmu_info_t *mmu_info = NULL;
    _bcm_th_cosq_cpu_port_info_t *cpu_port_info = NULL;
    int alloc_size = 0, i, j, pipe = -1, xpe;
    soc_profile_mem_t *profile_mem;
    soc_mem_t wred_config_mem = INVALIDm;
    int index, profile_index;
    int stable_size, rv, num, set_index;
    mmu_wred_config_entry_t qentry;
    uint32 entry[SOC_MAX_MEM_WORDS];
    bcm_port_t port;
    static const soc_reg_t port_fc_reg = PORT_LLFC_CFGr;
    uint32 rval;
    _bcm_bst_device_handlers_t bst_callbks;
    _bcm_th_cosq_cpu_cosq_config_t *cpu_cosq_config_p;
    _bcm_pfc_deadlock_control_t *pfc_deadlock_control = NULL;
    uint16 recovered_ver = 0;
    int index_min, index_max;
    int additional_scache_sz = 0;

    SOC_IF_ERROR_RETURN(soc_stable_size_get(unit, &stable_size));
    if ((stable_size == 0) || (SOC_WARM_BOOT_SCACHE_IS_LIMITED(unit))) {
        /* COSQ warmboot requires extended scache support i.e. level2 warmboot*/
        return BCM_E_NONE;
    }

    SOC_SCACHE_HANDLE_SET(scache_handle, unit, BCM_MODULE_COSQ, 0);
    rv = _bcm_esw_scache_ptr_get(unit, scache_handle, FALSE, 0, &scache_ptr,
                                 BCM_WB_DEFAULT_VERSION, &recovered_ver);
    if (BCM_FAILURE(rv)) {
        return rv;
    }

    mmu_info = _bcm_th_mmu_info[unit];

    if (mmu_info == NULL) {
        return BCM_E_INIT;
    }

    cpu_port_info = _bcm_th_cosq_cpu_port_info[unit];
    if (cpu_port_info == NULL) {
        return BCM_E_INIT;
    }

    /* Update mmu_info shared limit parameters */
    int_scache_ptr = (int *) scache_ptr;
    /* shared limits are now recovered from hw */
    int_scache_ptr += 3;
    BCM_IF_ERROR_RETURN(
        soc_th_mmu_get_shared_size(unit,
             mmu_info->ing_shared_limit,
             mmu_info->egr_db_shared_limit,
             mmu_info->egr_qe_shared_limit));

    alloc_size = sizeof(_bcm_th_cosq_cpu_port_info_t);
    scache_ptr = (uint8*) int_scache_ptr;
    sal_memcpy(cpu_port_info, scache_ptr, alloc_size);

    /* Restore the Queue to Channel Mapping */
    if (recovered_ver <= BCM_WB_VERSION_1_1) {
        BCM_IF_ERROR_RETURN
            (_bcm_cosq_rx_queue_channel_reinit(unit));
    }

    scache_ptr += alloc_size;

    /* Update IFP_COS_MAP memory profile reference counter */
    profile_mem = _bcm_th_ifp_cos_map_profile[unit];
    num = SOC_MEM_SIZE(unit, IFP_COS_MAPm) / _TH_MMU_NUM_INT_PRI;
    u16_scache_ptr = (uint16 *)scache_ptr;
    for (i = 0; i < num; i++) {
        for (j = 0; j < *u16_scache_ptr; j++) {
            SOC_IF_ERROR_RETURN(
                soc_profile_mem_reference(unit, profile_mem,
                                          i * _TH_MMU_NUM_INT_PRI,
                                          _TH_MMU_NUM_INT_PRI));
        }
        u16_scache_ptr++;
    }

    /* Recover CPU queue config information from scache */
    if (recovered_ver >= BCM_WB_VERSION_1_1) {
        cpu_cosq_config_p = _bcm_th_cosq_cpu_cosq_config[unit][0];
        if (cpu_cosq_config_p == NULL) {
            return BCM_E_INIT;
        }
        alloc_size = (SOC_TH_NUM_CPU_QUEUES *
                       sizeof(_bcm_th_cosq_cpu_cosq_config_t));
        scache_ptr = (uint8*) u16_scache_ptr;
        sal_memcpy(cpu_cosq_config_p, scache_ptr, alloc_size);
        scache_ptr += alloc_size;
    }

    /* Retriving num_cos value */
    if (recovered_ver >= BCM_WB_VERSION_1_1) {
        int_scache_ptr = (int *) scache_ptr;
        _TH_NUM_COS(unit) = *int_scache_ptr++;
        scache_ptr = (uint8*) int_scache_ptr;
    }

    /* Update PRIO2COS_PROFILE register profile reference counter */
    PBMP_PORT_ITER(unit, port) {
        BCM_IF_ERROR_RETURN
            (soc_reg32_get(unit, port_fc_reg, port, 0, &rval));
        profile_index = soc_reg_field_get(unit, port_fc_reg, rval, PROFILE_INDEXf) * 16;
        SOC_IF_ERROR_RETURN
            (soc_profile_reg_reference(unit, _bcm_th_prio2cos_profile[unit],
                                       profile_index, 16));
    }

    /* Update PORT_COS_MAP memory profile reference counter */
    profile_mem = _bcm_th_cos_map_profile[unit];
    PBMP_ALL_ITER(unit, port) {
        SOC_IF_ERROR_RETURN
            (soc_mem_read(unit, COS_MAP_SELm, MEM_BLOCK_ALL, port, &entry));
        set_index = soc_mem_field32_get(unit, COS_MAP_SELm, &entry, SELECTf);
        SOC_IF_ERROR_RETURN
            (soc_profile_mem_reference(unit, profile_mem, set_index * 16, 16));
    }
    if (SOC_INFO(unit).cpu_hg_index != -1) {
        SOC_IF_ERROR_RETURN(soc_mem_read(unit, COS_MAP_SELm, MEM_BLOCK_ALL,
                                         SOC_INFO(unit).cpu_hg_index, &entry));
        set_index = soc_mem_field32_get(unit, COS_MAP_SELm, &entry, SELECTf);
        SOC_IF_ERROR_RETURN
            (soc_profile_mem_reference(unit, profile_mem, set_index * 16, 16));
    }

    index_min = soc_mem_index_min(unit, SOC_MEM_UNIQUE_ACC_XPE_PIPE(unit,
                                                MMU_WRED_CONFIGm, 0, 0));
    index_max = soc_mem_index_max(unit, SOC_MEM_UNIQUE_ACC_XPE_PIPE(unit,
                                                MMU_WRED_CONFIGm, 0, 0));

    wred_config_mem = MMU_WRED_CONFIGm;
    for (pipe = 0; pipe < NUM_PIPE(unit); pipe++) {
        for (xpe = 0; xpe < NUM_XPE(unit); xpe++) {
            if (SOC_INFO(unit).epipe_xpe_map[pipe] & (1 << xpe)) {
                for (index = index_min; index < index_max; index++) {
                    BCM_IF_ERROR_RETURN
                        (soc_tomahawk_xpe_mem_read(unit, wred_config_mem, 
                            xpe, pipe, MEM_BLOCK_ALL, index, &qentry));
                    profile_index = soc_mem_field32_get(unit, wred_config_mem,
                                                    &qentry, PROFILE_INDEXf);
                    BCM_IF_ERROR_RETURN(soc_profile_mem_reference
                        (unit, _bcm_th_wred_profile[unit], profile_index, 1));
                }
            }
        }
    }

    /* Create gport tree */
    _BCM_TH_MMU_INFO(unit)->gport_tree_created = FALSE;
    PBMP_ALL_ITER(unit, port) {
        if (IS_LB_PORT(unit, port) || IS_CPU_PORT(unit, port)) {
            continue;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_gport_tree_create(unit, port));
    }
    _BCM_TH_MMU_INFO(unit)->gport_tree_created = TRUE;

    /* BST reinit */
    sal_memset(&bst_callbks, 0, sizeof(_bcm_bst_device_handlers_t));
    bst_callbks.resolve_index = &_bcm_th_bst_index_resolve;
    bst_callbks.reverse_resolve_index = NULL;
    BCM_IF_ERROR_RETURN(_bcm_bst_attach(unit, &bst_callbks));
#if defined (BCM_TCB_SUPPORT) && defined (INCLUDE_TCB)
    if (soc_feature(unit, soc_feature_tcb)){
#ifdef BCM_TOMAHAWK2_SUPPORT
        BCM_IF_ERROR_RETURN(bcm_th2_cosq_tcb_reinit(unit));
#endif
    }
#endif
    /* PFC Deadlock recovery should be initialize
       irrespective of scache version to handle
       upgrades */
    if (soc_feature(unit, soc_feature_pfc_deadlock)) {
        BCM_IF_ERROR_RETURN(_bcm_pfc_deadlock_init(unit));
    }

    /* Recover PFC Deadlock recovery information from scache */
    if (soc_feature(unit, soc_feature_pfc_deadlock)) {
        if (recovered_ver >= BCM_WB_VERSION_1_1) {
            pfc_deadlock_control = _BCM_UNIT_PFC_DEADLOCK_CONTROL(unit);
            if (pfc_deadlock_control == NULL) {
                return BCM_E_INIT;
            }
            alloc_size = sizeof(_bcm_pfc_deadlock_control_t);
            sal_memcpy(pfc_deadlock_control, scache_ptr, alloc_size);
            scache_ptr += alloc_size;
            /* Re-set the Hardware Registers/Memory info */
            BCM_IF_ERROR_RETURN(_bcm_th_pfc_deadlock_init(unit));
            BCM_IF_ERROR_RETURN(_bcm_pfc_deadlock_recovery_reset(unit));
            pfc_deadlock_control->cosq_inv_mapping_get = _bcm_th_cosq_inv_mapping_get;
        } else {
            additional_scache_sz += sizeof(_bcm_pfc_deadlock_control_t);
        }
    }
    /* Recovery TIME_DOMAINr count reference */
    if (recovered_ver >= BCM_WB_VERSION_1_2) {
        int ii;
        for (ii = 0; ii <  _BCM_TH_NUM_TIME_DOMAIN; ii++) {
            alloc_size =  sizeof(uint32);
            sal_memcpy(&time_domain_info[ii].ref_count, scache_ptr, alloc_size);
            scache_ptr += alloc_size;
        }
    } else {
        additional_scache_sz += _BCM_TH_NUM_TIME_DOMAIN * sizeof(uint32);
    }

#ifdef BCM_TOMAHAWK_SUPPORT
    if (soc_feature(unit, soc_feature_ecn_wred)) {
        if (recovered_ver >= BCM_WB_VERSION_1_3) {
            BCM_IF_ERROR_RETURN(bcmi_xgs5_ecn_reinit(unit, &scache_ptr));
        } else {
            uint32 mpls_ecn_map_size = 0;
            BCM_IF_ERROR_RETURN(
                bcmi_xgs5_ecn_scache_size_get(unit, &mpls_ecn_map_size));
            additional_scache_sz += mpls_ecn_map_size;
        }
    }
#endif

#if defined (BCM_TCB_SUPPORT) && defined (INCLUDE_TCB)
    if (soc_feature(unit, soc_feature_tcb)){
#ifdef BCM_TOMAHAWK2_SUPPORT
        if (recovered_ver >= BCM_WB_VERSION_1_3) {
            _bcm_cosq_tcb_unit_ctrl_t *tcb_unit_info;
            tcb_unit_info = TCB_UNIT_CONTROL(unit);
            TCB_INIT(unit);

            alloc_size = sizeof(uint16);
            sal_memcpy(&tcb_unit_info->tcb_config_init_bmp, scache_ptr, alloc_size);
            scache_ptr += alloc_size;
        } else {
            additional_scache_sz += sizeof(uint16);
        }
#endif
    }
#endif


    /* realloc scache_handle resource*/
    if (additional_scache_sz != 0) {
        rv = soc_scache_realloc(unit, scache_handle, additional_scache_sz);
        if (SOC_FAILURE(rv)) {
            return rv;
        }
    }
    return BCM_E_NONE;
}

int
bcm_th_cosq_sync(int unit)
{
    soc_scache_handle_t scache_handle;
    uint8 *scache_ptr;
    uint32 *u32_scache_ptr;
    uint16 *u16_scache_ptr;
    int *int_scache_ptr;
    _bcm_th_mmu_info_t *mmu_info = NULL;
    _bcm_th_cosq_cpu_port_info_t *cpu_port_info = NULL;
    int size = 0, ii;
    int ing_shared_limit_min, egr_db_shared_limit_min, egr_qe_shared_limit_min;
    int xpe_map;
    int rv = BCM_E_NONE, ref_count;
    _bcm_th_cosq_cpu_cosq_config_t **cpu_cosq_config_p;
    _bcm_pfc_deadlock_control_t *pfc_deadlock_control = NULL;

    mmu_info = _bcm_th_mmu_info[unit];
    if (mmu_info == NULL) {
        return BCM_E_INIT;
    }

    cpu_port_info = _bcm_th_cosq_cpu_port_info[unit];
    if (cpu_port_info == NULL) {
        return BCM_E_INIT;
    }

    SOC_SCACHE_HANDLE_SET(scache_handle, unit, BCM_MODULE_COSQ, 0);
    BCM_IF_ERROR_RETURN
        (_bcm_esw_scache_ptr_get(unit, scache_handle, FALSE, 0,
                                 &scache_ptr, BCM_WB_DEFAULT_VERSION, NULL));

    /* Cache minimal ing_shared_limit/egr_db_shared_limit/egr_qe_shared_limit */
    xpe_map = SOC_INFO(unit).ipipe_xpe_map[0]|SOC_INFO(unit).ipipe_xpe_map[1];
    _TH_ARRAY_MIN(mmu_info->ing_shared_limit, NUM_XPE(unit), xpe_map, 
        ing_shared_limit_min);
    _TH_ARRAY_MIN(mmu_info->egr_db_shared_limit, NUM_XPE(unit), xpe_map, 
        egr_db_shared_limit_min);
    _TH_ARRAY_MIN(mmu_info->egr_qe_shared_limit, NUM_XPE(unit), xpe_map, 
        egr_qe_shared_limit_min);
    u32_scache_ptr = (uint32*) scache_ptr;
    *u32_scache_ptr++ = ing_shared_limit_min;
    *u32_scache_ptr++ = egr_db_shared_limit_min;
    *u32_scache_ptr++ = egr_qe_shared_limit_min / 4;  /* to keep consistence with old version */

    /* Cache _bcm_th_cosq_cpu_port_info_t structure */
    size = sizeof(_bcm_th_cosq_cpu_port_info_t);
    scache_ptr = (uint8*)u32_scache_ptr;
    sal_memcpy(scache_ptr, cpu_port_info, size);
    scache_ptr += size/sizeof(uint8);

    /* Cache _bcm_th_ifp_cos_map_profile reference count */
    u16_scache_ptr = (uint16 *)scache_ptr;
    for (ii = 0;
         ii < (SOC_MEM_SIZE(unit, IFP_COS_MAPm) / _TH_MMU_NUM_INT_PRI);
         ii++) {
        rv = soc_profile_mem_ref_count_get(unit,
                                           _bcm_th_ifp_cos_map_profile[unit],
                                           ii * _TH_MMU_NUM_INT_PRI,
                                           &ref_count);
        if (!(rv == SOC_E_NOT_FOUND || rv == SOC_E_NONE)) {
            return rv;
        }
        *u16_scache_ptr++ = (uint16)(ref_count & 0xffff);
    }

    /* Sync CPU Queue info */
    cpu_cosq_config_p = _bcm_th_cosq_cpu_cosq_config[unit];
    size = (SOC_TH_NUM_CPU_QUEUES * sizeof(_bcm_th_cosq_cpu_cosq_config_t));
    scache_ptr = (uint8*) u16_scache_ptr;
    sal_memcpy(scache_ptr, *cpu_cosq_config_p, size);
    scache_ptr += (size / sizeof(uint8));

    /* Sync num_cos value */
    int_scache_ptr = (int*) scache_ptr;
    *int_scache_ptr++ = _TH_NUM_COS(unit);
    scache_ptr = (uint8 *)int_scache_ptr;

    /* Sync PFC Deadlock Control info */
    if (soc_feature(unit, soc_feature_pfc_deadlock)) {
        pfc_deadlock_control = _BCM_UNIT_PFC_DEADLOCK_CONTROL(unit);
        if (pfc_deadlock_control == NULL) {
            return BCM_E_INIT;
        }
        size = sizeof(_bcm_pfc_deadlock_control_t);
        sal_memcpy(scache_ptr, pfc_deadlock_control, size);
        scache_ptr += (size / sizeof(uint8));
    }
    /* Sync TIME_DOMAINr field and count reference */
    u32_scache_ptr = (uint32*) scache_ptr;
    size = sizeof(uint32);
    for (ii = 0; ii <  _BCM_TH_NUM_TIME_DOMAIN; ii++) {
        sal_memcpy(u32_scache_ptr, &time_domain_info[ii].ref_count, size);
        u32_scache_ptr++;
    }

    scache_ptr = (uint8*)u32_scache_ptr;
#ifdef BCM_TOMAHAWK_SUPPORT
    if (soc_feature(unit, soc_feature_ecn_wred)) {
        BCM_IF_ERROR_RETURN(bcmi_xgs5_ecn_sync(unit, &scache_ptr));
    }
#endif
#if defined (BCM_TCB_SUPPORT) && defined (INCLUDE_TCB)
    if (soc_feature(unit, soc_feature_tcb)){
#ifdef BCM_TOMAHAWK2_SUPPORT
        _bcm_cosq_tcb_unit_ctrl_t *tcb_unit_info;
        tcb_unit_info = TCB_UNIT_CONTROL(unit);
        TCB_INIT(unit);

        u16_scache_ptr = (uint16*) scache_ptr;
        size = sizeof(uint16);
        sal_memcpy(u16_scache_ptr, &tcb_unit_info->tcb_config_init_bmp, size);
        u16_scache_ptr ++;
#endif
    }
#endif
    return BCM_E_NONE;
}

#endif /* BCM_WARM_BOOT_SUPPORT */

/*
 * Function:
 *     bcm_th_cosq_sw_dump
 * Purpose:
 *     Displays COS Queue information maintained by software.
 * Parameters:
 *     unit - Device unit number
 * Returns:
 *     None
 */
void
bcm_th_cosq_sw_dump(int unit)
{
    bcm_port_t port;
    int ii;
    LOG_CLI((BSL_META_U(unit,
                        "\nSW Information COSQ - Unit %d\n"), unit));
    PBMP_ALL_ITER(unit, port) {
        if (IS_CPU_PORT(unit, port)) {
            (void)_bcm_th_cosq_cpu_port_info_dump(unit);
        }
        (void)_bcm_th_cosq_port_info_dump(unit, port);
    }
    for (ii = 0; ii <  _BCM_TH_NUM_TIME_DOMAIN; ii++) {
        LOG_CLI((BSL_META_U(unit,
                            "\nTIME_DOMAIN[%d] - refcnt %d\n"), ii, 
                            time_domain_info[ii].ref_count));
    }

#ifndef BCM_SW_STATE_DUMP_DISABLE
#if defined (INCLUDE_TCB) && defined (BCM_TCB_SUPPORT)
    if(soc_feature(unit, soc_feature_tcb)) {
#ifdef BCM_TOMAHAWK2_SUPPORT
        (void)bcm_th2_cosq_tcb_sw_dump(unit);
#endif
    }
#endif
#endif /* BCM_SW_STATE_DUMP_DISABLE */

    return;
}

#ifdef BCM_TOMAHAWK2_SUPPORT
/*
 * Function:
 *      bcm_th_cosq_port_attach
 * Purpose:
 *      Initialize Cosq for newly added port
 * Parameters:
 *      unit    - (IN) Unit number.
 *      port    - (IN) Logical port.
 * Returns:
 *      SOC_E_XXX
 */
int
bcm_th_cosq_port_attach(int unit, bcm_port_t port)
{
    _bcm_th_cosq_port_info_t *port_info = NULL;
    int default_mode = BCM_COSQ_WEIGHTED_ROUND_ROBIN, default_weight = 1;
    int i;
    uint64 rval64s[16], *prval64s[1];
    uint32 profile_index;

    if (IS_CPU_PORT(unit, port) || IS_LB_PORT(unit, port) ||
        IS_MANAGEMENT_PORT(unit, port)) {
        return BCM_E_PARAM;
    }

    /* Increase the reference of defalt PRIO2COS_PROFILE */
    sal_memset(rval64s, 0, sizeof(rval64s));
    prval64s[0] = rval64s;
    BCM_IF_ERROR_RETURN
        (soc_profile_reg_add(unit, _bcm_th_prio2cos_profile[unit],
                             prval64s, 16, &profile_index));

    /* Create gport tree */
    _BCM_TH_MMU_INFO(unit)->gport_tree_created = FALSE;
    BCM_IF_ERROR_RETURN(_bcm_th_cosq_gport_tree_create(unit, port));
    _BCM_TH_MMU_INFO(unit)->gport_tree_created = TRUE;

    /* Set Default Scheduler Configuration on the ports */
    port_info = &_bcm_th_cosq_port_info[unit][port];
    for (i = 0; i < _BCM_TH_NUM_SCHEDULER_PER_PORT; i++) {
        BCM_IF_ERROR_RETURN
            (bcm_th_cosq_gport_sched_set(unit, port_info->sched[i].gport,
                -1, default_mode, default_weight));
    }
    for (i = 0; i < _BCM_TH_NUM_UCAST_QUEUE_PER_PORT; i++) {
        BCM_IF_ERROR_RETURN
            (bcm_th_cosq_gport_sched_set(unit, port_info->ucast[i].gport,
                -1, default_mode, default_weight));
    }
    for (i = 0; i < _BCM_TH_NUM_MCAST_QUEUE_PER_PORT; i++) {
        BCM_IF_ERROR_RETURN
            (bcm_th_cosq_gport_sched_set(unit, port_info->mcast[i].gport,
                -1, default_mode, default_weight));
    }

    /* Update shared limit of new ports in case the shared limit has been
     * adjusted by bcm_cosq_control_set() API */
    BCM_IF_ERROR_RETURN
        (soc_th2_mmu_config_shared_buf_set_hw_port(unit, port,
                                _BCM_TH_MMU_INFO(unit)->ing_shared_limit,
                                _BCM_TH_MMU_INFO(unit)->egr_db_shared_limit,
                                _BCM_TH_MMU_INFO(unit)->egr_qe_shared_limit));
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_th2_cosq_port_guarantee_reset
 * Purpose:
 *      Reset the guarantee of priority group, uc queue, mc queue to the default
 * Parameters:
 *      unit    - (IN) Unit number.
 *      port    - (IN) Logical port.
 * Returns:
 *      BCM_E_XXX
 */
STATIC int
_bcm_th2_cosq_port_guarantee_reset(int unit, bcm_port_t port)
{
    soc_info_t *si = &SOC_INFO(unit);
    _soc_mmu_device_info_t devcfg;
    int lossless, override, default_mtu_cells, idx, numq;
    int pg_guarantee, uc_guarantee, mc_guarantee;

    lossless = soc_property_get(unit, spn_MMU_LOSSLESS, 0);
    override = soc_property_get(unit, spn_MMU_CONFIG_OVERRIDE, 1);

    _soc_th2_mmu_init_dev_config(unit, &devcfg, lossless);
    default_mtu_cells = _MMU_CFG_MMU_BYTES_TO_CELLS(devcfg.default_mtu_size +
                                                    devcfg.mmu_hdr_byte,
                                                    devcfg.mmu_cell_size);

    /* priority group */
    for (idx = 0; idx < _TH_MMU_NUM_PG; idx++) {
        pg_guarantee = 0;
        if (idx == 7 && lossless) {
            pg_guarantee = default_mtu_cells;
        }
        if (!override) {
            _soc_mmu_cfg_property_get_cells
                (unit, port, spn_PRIGROUP, idx, spn_GUARANTEE, FALSE,
                 &pg_guarantee, devcfg.mmu_cell_size,
                 _MMU_DEFAULT_THRES_TYPE);
            pg_guarantee *= devcfg.mmu_cell_size;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_ing_res_set(unit, port, idx,
                                      bcmCosqControlIngressPortPGMinLimitBytes,
                                      pg_guarantee));
    }

    /* UC */
    numq = MIN(si->port_num_uc_cosq[port], NUM_COS(unit));
    for (idx = 0; idx < numq; idx++) {
        uc_guarantee = 0;
        if (!override) {
            _soc_mmu_cfg_property_get_cells
                (unit, port, spn_QUEUE, idx, spn_GUARANTEE, FALSE,
                 &uc_guarantee, devcfg.mmu_cell_size,
                 _MMU_DEFAULT_THRES_TYPE);
            uc_guarantee *= devcfg.mmu_cell_size;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_egr_queue_set(unit, port, idx,
                                       bcmCosqControlEgressUCQueueMinLimitBytes,
                                       uc_guarantee));
    }

    /* MC */
    numq = MIN(si->port_num_cosq[port], NUM_COS(unit));
    for (idx = 0; idx < numq; idx++) {
        mc_guarantee = 0;
        if (!override) {
            _soc_mmu_cfg_property_get_cells
                (unit, port, spn_MQUEUE, idx, spn_GUARANTEE, FALSE,
                 &mc_guarantee, devcfg.mmu_cell_size,
                 _MMU_DEFAULT_THRES_TYPE);
            mc_guarantee *= devcfg.mmu_cell_size;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_th_cosq_egr_queue_set(unit, port, idx,
                                       bcmCosqControlEgressMCQueueMinLimitBytes,
                                       mc_guarantee));
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_th_cosq_port_detach
 * Purpose:
 *      Clear cosq configuration of port
 * Parameters:
 *      unit    - (IN) Unit number.
 *      port    - (IN) Logical port.
 * Returns:
 *      SOC_E_XXX
 */
int
bcm_th_cosq_port_detach(int unit, bcm_port_t port)
{
    soc_info_t *si = &SOC_INFO(unit);
    _bcm_th_cosq_port_info_t *port_info = NULL;
    uint32 rval, profile_index;

    if (IS_CPU_PORT(unit, port) || IS_LB_PORT(unit, port) ||
        IS_MANAGEMENT_PORT(unit, port)) {
        return BCM_E_PARAM;
    }

    /* When update guarantee of PG/UC/MC via API, the shared limit is adjusted
     * accordingly. So MUST reset the gurarntee to the default value before
     * deleting the port to recover the shared limit. */
    BCM_IF_ERROR_RETURN
        (_bcm_th2_cosq_port_guarantee_reset(unit, port));

    /* Destroy cosq gport tree on port */
    port_info = &_bcm_th_cosq_port_info[unit][port];
    sal_memset(port_info, 0, sizeof(_bcm_th_cosq_port_info_t));

    /* Decrease the reference of defalt PRIO2COS_PROFILE */
    BCM_IF_ERROR_RETURN(soc_reg32_get(unit, PORT_LLFC_CFGr, port, 0, &rval));
    profile_index =
        soc_reg_field_get(unit, PORT_LLFC_CFGr, rval, PROFILE_INDEXf) * 16;
    BCM_IF_ERROR_RETURN
        (soc_profile_reg_delete(unit, _bcm_th_prio2cos_profile[unit],
                                profile_index));

    /* Clear cosq */
    si->port_num_cosq[port] = 0;
    si->port_cosq_base[port] = 0;
    si->port_num_uc_cosq[port] = 0;
    si->port_uc_cosq_base[port] = 0;
    si->port_num_ext_cosq[port] = 0;

    return BCM_E_NONE;
}
#endif /* BCM_TOMAHAWK2_SUPPORT */
#endif /* BCM_TOMAHAWK_SUPPORT */

