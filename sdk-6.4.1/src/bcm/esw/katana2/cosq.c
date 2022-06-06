/*
 * $Id: cosq.c,v 1.16 Broadcom SDK $
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
 * COS Queue Management
 * Purpose: API to set different cosq, priorities, and scheduler registers.
 */

#include <shared/bsl.h>

#include <sal/core/libc.h>
#include <soc/defs.h>
#if defined(BCM_KATANA2_SUPPORT)
#include <soc/drv.h>
#include <soc/mem.h>
#include <soc/katana.h>
#include <soc/katana2.h>
#include <soc/profile_mem.h>
#ifdef BCM_CMICM_SUPPORT
#include <soc/cmicm.h>
#endif
    
#include <bcm/error.h>
#include <bcm/cosq.h>
#include <bcm_int/esw/mbcm.h>
#include <bcm_int/esw/firebolt.h>
#include <bcm_int/esw/katana.h>
#include <bcm_int/esw/katana2.h>
#include <bcm_int/esw/stack.h>
#include <bcm_int/esw/cosq.h>
#include <bcm_int/esw_dispatch.h>
#include <bcm_int/esw/subport.h>
#include <bcm_int/esw/trx.h>
#include <bcm/types.h>

#ifdef BCM_WARM_BOOT_SUPPORT
#include <bcm_int/esw/switch.h>
#endif /* BCM_WARM_BOOT_SUPPORT */

#if defined(BCM_KATANA2_SUPPORT)
#include <soc/katana2.h>
#endif

#define KT2_CELL_FIELD_MAX       0xffff

/* MMU cell size in bytes */
#define _BCM_KT2_COSQ_CELLSIZE   192

#define _BCM_KT2_NUM_UCAST_QUEUE_GROUP            8
#define _BCM_KT2_NUM_VLAN_UCAST_QUEUE_GROUP       16

#define _BCM_KT2_IS_UC_QUEUE(mmu_info,qid)   \
    (((qid) < mmu_info->num_base_queues) ? 1 : 0)

#define _BCM_KT2_COSQ_LIST_NODE_INIT(list, node)        \
    list[node].in_use            = FALSE;               \
    list[node].wrr_in_use        = 0;                   \
    list[node].wrr_mode          = 0;                   \
    list[node].gport             = -1;                  \
    list[node].base_index        = -1;                  \
    list[node].mc_base_index     = -1;                  \
    list[node].numq              = 0;                   \
    list[node].hw_index          = -1;                  \
    list[node].level             = -1;                  \
    list[node].cosq_attached_to  = -1;                  \
    list[node].num_child         = 0;                   \
    list[node].first_child       = 4095;                \
    list[node].linkphy_enabled   = 0;                   \
    list[node].subport_enabled   = 0;                   \
    list[node].cosq_map          = NULL;                \
    list[node].parent            = NULL;                \
    list[node].sibling           = NULL;                \
    list[node].child             = NULL;                \
    list[node].local_port        = -1;                  \
    BCM_PBMP_CLEAR(list[node].fabric_pbmp);              \
    list[node].remote_modid      = -1;                  \
    list[node].remote_port      = -1;

#define _BCM_KT2_COSQ_NODE_INIT(node)                   \
    node->in_use            = FALSE;                    \
    node->wrr_in_use        = 0;                        \
    node->wrr_mode          = 0;                        \
    node->gport             = -1;                       \
    node->base_index        = -1;                       \
    node->mc_base_index     = -1;                       \
    node->numq              = 0;                        \
    node->hw_index          = -1;                       \
    node->level             = -1;                       \
    node->cosq_attached_to  = -1;                       \
    node->num_child         = 0;                        \
    node->first_child       = 4095;                     \
    node->linkphy_enabled   = 0;                        \
    node->subport_enabled   = 0;                        \
    node->cosq_map          = NULL;                     \
    node->parent            = NULL;                     \
    node->sibling           = NULL;                     \
    node->child             = NULL;                     \
    BCM_PBMP_CLEAR(node->fabric_pbmp);                  \
    node->remote_modid      = -1;                       \
    node->remote_port       = -1;
 
#define _BCM_KT2_LLS_DYN_CHG_REG_B 0x3FF
#define _BCM_KT2_LLS_DYN_CHG_REG_C 0xFFFFFFFF

#define _BCM_KT2_LB_BASE_QUEUE     0x40

typedef enum {
    _BCM_KT2_COSQ_NODE_LEVEL_ROOT,
    _BCM_KT2_COSQ_NODE_LEVEL_S0,
    _BCM_KT2_COSQ_NODE_LEVEL_S1,
    _BCM_KT2_COSQ_NODE_LEVEL_L0,
    _BCM_KT2_COSQ_NODE_LEVEL_L1,
    _BCM_KT2_COSQ_NODE_LEVEL_L2,
    _BCM_KT2_COSQ_NODE_LEVEL_MAX
}_bcm_kt2_cosq_node_level_t;

typedef enum {
    _BCM_KT2_COSQ_FC_PAUSE = 0,
    _BCM_KT2_COSQ_FC_PFC,
    _BCM_KT2_COSQ_FC_VOQFC,
    _BCM_KT2_COSQ_FC_E2ECC
} _bcm_kt2_fc_type_t;

typedef enum {
    _BCM_KT2_COSQ_DMVOQ = 0,
    _BCM_KT2_COSQ_DPVOQ
} _bcm_kt2_voq_type_t;


typedef enum {
    _BCM_KT2_COSQ_STATE_NO_CHANGE,
    _BCM_KT2_COSQ_STATE_DISABLE,
    _BCM_KT2_COSQ_STATE_ENABLE,
    _BCM_KT2_COSQ_STATE_SPRI_TO_WRR,
    _BCM_KT2_COSQ_STATE_WRR_TO_SPRI,
    _BCM_KT2_COSQ_STATE_MAX
}_bcm_kt2_cosq_state_t;
typedef enum {
    _BCM_KT2_NODE_UNKNOWN = 0,
    _BCM_KT2_NODE_UCAST,
    _BCM_KT2_NODE_MCAST,
    _BCM_KT2_NODE_VOQ,
    _BCM_KT2_NODE_VLAN_UCAST,
    _BCM_KT2_NODE_VM_UCAST,
    _BCM_KT2_NODE_SERVICE_UCAST,
    _BCM_KT2_NODE_SCHEDULER
} _bcm_kt2_node_type_e;

typedef struct _bcm_kt2_cosq_node_s{
    struct _bcm_kt2_cosq_node_s *parent;
    struct _bcm_kt2_cosq_node_s *sibling;
    struct _bcm_kt2_cosq_node_s *child;
    bcm_gport_t gport;
    SHR_BITDCL *cosq_map;
    int in_use;
    int wrr_in_use;
    int wrr_mode;
    int base_index;
    int mc_base_index;
    int numq;
    int hw_index;
    _bcm_kt2_cosq_node_level_t level;
    _bcm_kt2_node_type_e type;
    int cosq_attached_to;
    int num_child;
    int first_child;
    int linkphy_enabled;
    int subport_enabled;
    bcm_port_t local_port;

    /* DPVOQ specific information */
    bcm_module_t remote_modid;
    bcm_port_t   remote_port;
    bcm_pbmp_t   fabric_pbmp;
}_bcm_kt2_cosq_node_t;

typedef struct _bcm_kt2_cosq_list_s {
    int count;
    SHR_BITDCL *bits;
}_bcm_kt2_cosq_list_t;

typedef struct _bcm_kt2_cosq_port_info {
    int q_offset;
    int q_limit;
    int mcq_offset;
    int mcq_limit;
}_bcm_kt2_cosq_port_info_t;

typedef struct _bcm_kt2_mmu_info_s {
    int num_base_queues;   /* Number of classical queues */
    int num_ext_queues;    /* Number of extended queues */
    int qset_size;         /* subscriber queue set size */
    int num_dmvoq_queues;  /* Number of dmvoq queues */
    int base_dmvoq_queue;  /* Base DMVOQ queue */
    uint32 num_queues;     /* total number of queues */
    uint32 num_nodes;      /* max number of sched nodes */
    uint32 max_nodes[_BCM_KT2_COSQ_NODE_LEVEL_MAX];/* max nodes in each level */
    bcm_gport_t *l1_gport_pair;        /* L1 gport mapping for dynamic update */
    _bcm_kt2_cosq_port_info_t *port;   /* port information */
    _bcm_kt2_cosq_list_t ext_qlist;    /* list of extended queues */
    _bcm_kt2_cosq_list_t dmvoq_qlist;    /* list of dmvoq queues */
    _bcm_kt2_cosq_list_t sched_list;   /* list of sched nodes */
    _bcm_kt2_cosq_list_t sched_hw_list[_BCM_KT2_COSQ_NODE_LEVEL_MAX]; 
                                       /* list of sched hw list */
    _bcm_kt2_cosq_list_t l2_base_qlist;  
                           /* list of l2  base uc queue index list */
    _bcm_kt2_cosq_list_t l2_base_mcqlist;  
                           /* list of l2  base mc queue index list */ 
    _bcm_kt2_cosq_list_t l2_ext_qlist;   
                           /* list of l2  extended queue index list */
    _bcm_kt2_cosq_list_t l2_dmvoq_qlist;   
                           /* list of l2  dmvoq queue index list */
    _bcm_kt2_cosq_node_t *sched_node;
                                         /* sched nodes */
    _bcm_kt2_cosq_node_t *queue_node;
                                         /* queue nodes */
    int     curr_merger_index;
}_bcm_kt2_mmu_info_t;

STATIC _bcm_kt2_mmu_info_t *_bcm_kt2_mmu_info[BCM_MAX_NUM_UNITS];  

STATIC soc_profile_mem_t *_bcm_kt2_cos_map_profile[BCM_MAX_NUM_UNITS];
STATIC soc_profile_mem_t *_bcm_kt2_wred_profile[BCM_MAX_NUM_UNITS];
STATIC soc_profile_mem_t *_bcm_kt2_ifp_cos_map_profile[BCM_MAX_NUM_UNITS];

/* Number of COSQs configured for this device */
STATIC int _bcm_kt2_num_cosq[SOC_MAX_NUM_DEVICES];

STATIC int num_pp_ports=-1;
STATIC int num_port_schedulers=-1;

STATIC int num_s0_schedulers=-1;
STATIC int num_s1_schedulers=-1;
STATIC int num_l0_schedulers=-1;
STATIC int num_l1_schedulers=-1;
STATIC int num_total_schedulers=-1;

STATIC int num_l2_queues=-1;


/* Function:
 *    _bcm_kt2_cosq_source_intf_set
 * Purpose:
 *    Function to set the SRC_INTF table and  
 *    return the index
 * Parameters:
 *     unit       - (IN) unit number
 *     src_modid  - (IN) source module ID 
 *     src_port_num - (IN) source port number
 * Returns:
 *     BCM_E_XXX
 */

STATIC int
_bcm_kt2_cosq_source_intf_set(int unit, bcm_port_t src_modid, 
        bcm_port_t src_port_num, _bcm_kt2_voq_type_t type)
{
    int index, free_index = -1;
    uint32 rval, modid = 0, port_num = 0, valid;

    for (index = 0; index < 16; index++) {
        modid=0;
        port_num=0;
        BCM_IF_ERROR_RETURN(READ_SRC_INTFr(unit, index, &rval));
        valid = soc_reg_field_get(unit, SRC_INTFr, rval, SRC_MODID_VALIDf);
        if (1 == valid) {
            modid = soc_reg_field_get(unit, SRC_INTFr, rval, SOURCEf);
            if (src_modid == modid) {
                return index;
            }
        } 
        valid = soc_reg_field_get(unit, SRC_INTFr, rval, SRC_PORT_NUM_VALIDf);
        if (1 == valid) {
            port_num = soc_reg_field_get(unit, SRC_INTFr, rval, SOURCEf);
            if (src_port_num == port_num) {
                return index;
            }
        }
        if ((port_num == 0) && (modid == 0) &&
                (free_index == -1)) {
            free_index = index;
        }
    }
    if (free_index == -1) {
        return BCM_E_RESOURCE;
    }

    rval = 0;
    if (type == _BCM_KT2_COSQ_DMVOQ) {
        soc_reg_field_set(unit, SRC_INTFr, &rval, SRC_MODID_VALIDf, 1);
        soc_reg_field_set(unit, SRC_INTFr, &rval, SOURCEf, src_modid);
    } else {
        soc_reg_field_set(unit, SRC_INTFr, &rval, SRC_PORT_NUM_VALIDf, 1);
        soc_reg_field_set(unit, SRC_INTFr, &rval, SOURCEf, src_port_num);
    }
    BCM_IF_ERROR_RETURN(WRITE_SRC_INTFr(unit, free_index, rval));

    return free_index;
}

/*
 * Function:
 *  _bcm_kt2_cosq_map_fc_status_to_node    
 * Purpose:
 *     Function will fill the appropriate indexes and SEL
 *     in MMU_INTFI_FC_MAP_TBL as per the table requirement
 * Parameters:
 *     unit      - (IN) unit number
 *     fct       - (IN) type is PFC/VOQ 
 *     hw_index  - (IN) hw_index of the queue 
 *     level     - (IN) Node level L2/L1/L0 
 * Returns:
 *     BCM_E_XXX
 */

STATIC int
_bcm_kt2_cosq_map_fc_status_to_node(int unit, _bcm_kt2_fc_type_t fct,
                              uint32 hw_index, int level, uint32 base_offset )
{
    int map_entry_index, eindex, pfc;
    uint32 map_entry[SOC_MAX_MEM_WORDS];
    static const soc_field_t self[] = {
        SEL0f, SEL1f
    };
    static const soc_field_t indexf[] = {
        INDEX0f, INDEX1f
    };
    static const soc_mem_t mem[] = {
        INVALIDm,
        MMU_INTFI_FC_MAP_TBL0m,
        MMU_INTFI_FC_MAP_TBL1m,
        MMU_INTFI_FC_MAP_TBL2m
    };

    pfc = (fct == _BCM_KT2_COSQ_FC_PFC) ? 1 : 0;

    map_entry_index = hw_index / 16;
    eindex = (hw_index % 16) / 8;
    BCM_IF_ERROR_RETURN
        (soc_mem_read(unit, mem[level], MEM_BLOCK_ALL, map_entry_index, &map_entry));
    soc_mem_field32_set(unit, mem[level], &map_entry,
            indexf[eindex], base_offset);
    soc_mem_field32_set(unit, mem[level], &map_entry, self[eindex], pfc);
    BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem[level],
                MEM_BLOCK_ALL, map_entry_index, &map_entry));
    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_localport_resolve
 * Purpose:
 *     Resolve GRPOT if it is a local port
 * Parameters:
 *     unit       - (IN) unit number
 *     gport      - (IN) GPORT identifier
 *     local_port - (OUT) local port number
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_cosq_localport_resolve(int unit, bcm_gport_t gport,
                               bcm_port_t *local_port)
{
    bcm_module_t module;
    bcm_port_t port;
    bcm_trunk_t trunk;
    int id/*, result*/;

    if (!BCM_GPORT_IS_SET(gport)) {
        if (!SOC_PORT_VALID_RANGE(unit, gport)) {
            return BCM_E_PORT;
        }
        *local_port = gport;
        return BCM_E_NONE;
    } else if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_SCHEDULER(gport)) {
        return BCM_E_PORT;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_esw_gport_resolve(unit, gport, &module, &port, &trunk, &id));

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_modport_to_pp_port_get(unit, module, port, &port));

    *local_port = port;

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_node_get
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
_bcm_kt2_cosq_node_get(int unit, bcm_gport_t gport, bcm_module_t *modid,
                      bcm_port_t *port, int *id, _bcm_kt2_cosq_node_t **node)
{
    _bcm_kt2_mmu_info_t *mmu_info;
    _bcm_kt2_cosq_node_t *node_base;
    bcm_module_t modid_out = 0;
    bcm_port_t port_out = 0;
    uint32 encap_id = 0;
    int index;


    if (_bcm_kt2_mmu_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &modid_out));
        port_out = BCM_GPORT_UCAST_QUEUE_GROUP_SYSPORTID_GET(gport);
    } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &modid_out));
        port_out = BCM_GPORT_MCAST_QUEUE_GROUP_SYSPORTID_GET(gport);    
    } else if (BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &modid_out));
        port_out = BCM_GPORT_DESTMOD_QUEUE_GROUP_SYSPORTID_GET(gport);
    } else if (BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &modid_out));
        port_out = (gport >> 16) & 0xff;
    } else if (BCM_GPORT_IS_SCHEDULER(gport)) {
        port_out = BCM_GPORT_SCHEDULER_GET(gport) & 0xff;
    } else if (BCM_GPORT_IS_LOCAL(gport)) {
        encap_id = BCM_GPORT_LOCAL_GET(gport);
        port_out = encap_id;
    } else if (BCM_GPORT_IS_MODPORT(gport)) {
        encap_id = BCM_GPORT_MODPORT_PORT_GET(gport);
        port_out = encap_id;
    } else if (BCM_GPORT_IS_SUBPORT_PORT(gport)) {
        encap_id = BCM_GPORT_SUBPORT_PORT_GET(gport);
        port_out = encap_id;
    } else {
        return BCM_E_PORT;
    }

    mmu_info = _bcm_kt2_mmu_info[unit];

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) || 
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        encap_id = (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) ?
                   BCM_GPORT_UCAST_QUEUE_GROUP_QID_GET(gport) :
                   BCM_GPORT_MCAST_QUEUE_GROUP_QID_GET(gport);
        index = encap_id;
        node_base = mmu_info->queue_node;
    } else if (BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(gport)) {  
        encap_id = BCM_GPORT_DESTMOD_QUEUE_GROUP_QID_GET(gport); 
        index = encap_id;
        if (mmu_info->num_dmvoq_queues > 0) { 
            index += mmu_info->base_dmvoq_queue;
        } else {
            index += mmu_info->num_base_queues;
        }
        node_base = mmu_info->queue_node;
    } else if (BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(gport)) {
        encap_id = BCM_GPORT_UCAST_SUBSCRIBER_QUEUE_GROUP_QID_GET(gport);
        index = encap_id;
        node_base = mmu_info->queue_node;
    } else { 
        if (BCM_GPORT_IS_SCHEDULER(gport)) {
            /* scheduler */
            encap_id = (BCM_GPORT_SCHEDULER_GET(gport) >> 8) & 0x7ff;
            if (encap_id == 0) {
                encap_id = (BCM_GPORT_SCHEDULER_GET(gport) & 0xff);
            }
        }
        index = encap_id;
        if (index >= num_total_schedulers) {
            return BCM_E_PORT;
        }
        node_base = mmu_info->sched_node;
    }

    if (!(BCM_GPORT_IS_LOCAL(gport) || BCM_GPORT_IS_MODPORT(gport)) &&
            node_base[index].numq == 0) {
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

STATIC int
_bcm_kt2_voq_base_node_get(int unit,int modid,
                      _bcm_kt2_cosq_node_t **node)
{
    _bcm_kt2_mmu_info_t *mmu_info;
    _bcm_kt2_cosq_node_t *node_base;
    int index;
    *node = NULL; 
    mmu_info = _bcm_kt2_mmu_info[unit];
    node_base = mmu_info->queue_node;
    mmu_info = _bcm_kt2_mmu_info[unit];
    for(index = mmu_info->num_base_queues ; 
            index < mmu_info->num_queues ; index++) {
        if (node_base[index].remote_modid == modid) {
            *node = &node_base[index];
            break;
        }
    }
    return BCM_E_NONE;
}
/*
 * Function:
 *     _bcm_kt2_node_index_get
 * Purpose:
 *     Allocate free index from the given list
 * Parameters:
 *     list       - (IN) bit list
 *     start      - (IN) start index
 *     end        - (IN) end index
 *     qset       - (IN) size of queue set
 *     range      - (IN) range bits
 *     id         - (OUT) start index
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_node_index_get(SHR_BITDCL *list, int start, int end,
                       int qset, int range, int *id)
{
    int i, j, inc;

    *id = -1;

    if (range != qset) {
        inc = 1;
        for (i = start; i < end; i = (i + inc)) {
            for (j = i; j < (i + range); j++, inc++) {
                if (SHR_BITGET(list, j) != 0) {
                    break;
                }
            }

            if (j == (i + range)) {
                *id = i;
                return BCM_E_NONE;
            }
        }
        if (i == end) {
            return BCM_E_RESOURCE;
         }
    }

    for (i = start; i < end;  i = i + range) {
        for (j = i; j < (i + range); j++) {
            if (SHR_BITGET(list, j) != 0) {
                break;
            }
        }

        if (j == (i + range)) {
            *id  =  i;
            return BCM_E_NONE;
           }
    }

    if (i == end) {
        return BCM_E_RESOURCE;
      }

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_node_index_set
 * Purpose:
 *     Mark indices as allocated
 * Parameters:
 *     list       - (IN) bit list
 *     start      - (IN) start index
 *     range      - (IN) range bits
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_node_index_set(_bcm_kt2_cosq_list_t *list, int start, int range)
{
    list->count += range;
    SHR_BITSET_RANGE(list->bits, start, range);

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_node_index_clear
 * Purpose:
 *     Mark indices as free
 * Parameters:
 *     list       - (IN) bit list
 *     start      - (IN) start index
 *     range      - (IN) range bits
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_node_index_clear(_bcm_kt2_cosq_list_t *list, int start, int range)
{
    list->count -= range;
    SHR_BITCLR_RANGE(list->bits, start, range);

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_map_index_get
 * Purpose:
 *     Allocate free index from the given list
 * Parameters:
 *     list       - (IN) bit list
 *     start      - (IN) start index
 *     end        - (IN) end index
 *     id         - (OUT) start index
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_cosq_map_index_get(SHR_BITDCL *list, int start, int end, int *id)
{
    int i;

    for (i = start; i < end; i++) {
        if (SHR_BITGET(list, i) != 0) {
            continue;
        }
        *id  =  i;
        return BCM_E_NONE;
    }

    return BCM_E_RESOURCE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_map_index_set
 * Purpose:
 *     Mark indices as allocated
 * Parameters:
 *     list       - (IN) bit list
 *     start      - (IN) start index
 *     range      - (IN) range bits
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_cosq_map_index_set(SHR_BITDCL *list, int start, int range)
{
    SHR_BITSET_RANGE(list, start, range);
    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_map_index_clear
 * Purpose:
 *     Mark indices as free
 * Parameters:
 *     list       - (IN) bit list
 *     start      - (IN) start index
 *     range      - (IN) range bits
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_cosq_map_index_clear(SHR_BITDCL *list, int start, int range)
{
    SHR_BITCLR_RANGE(list, start, range);
    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_map_index_is_set
 * Purpose:
 *     Check index is allocated
 * Parameters:
 *     list       - (IN) bit list
 *     index      - (IN) index
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_cosq_map_index_is_set(SHR_BITDCL *list, int index)
{
    if (SHR_BITGET(list, index) != 0) {
        return TRUE;
    }

    return FALSE;
}

STATIC int
_bcm_kt2_cosq_max_nodes_get(int unit, _bcm_kt2_cosq_node_level_t level,
                           int *id)
{
    _bcm_kt2_mmu_info_t *mmu_info;

    if (level < _BCM_KT2_COSQ_NODE_LEVEL_ROOT ||
        level >= _BCM_KT2_COSQ_NODE_LEVEL_MAX) {
        return BCM_E_PARAM;
    }
    mmu_info = _bcm_kt2_mmu_info[unit];
    *id = mmu_info->max_nodes[level];

    return BCM_E_NONE;
}

typedef struct _bcm_kt2_lls_info_s {
    int node_count[_BCM_KT2_COSQ_NODE_LEVEL_MAX];
    uint32 *min_entries[_BCM_KT2_COSQ_NODE_LEVEL_MAX];
    uint32 *max_entries[_BCM_KT2_COSQ_NODE_LEVEL_MAX];
} _bcm_kt2_lls_info_t;

/* Max 1G rate to expedite draining packets during congestion */
#define _BCM_KT2_COSQ_MAX_FLUSH_RATE   1000000 

STATIC int 
_bcm_kt2_cosq_lls_shapers_remove(int unit, int port, 
                            _bcm_kt2_cosq_node_t *node,
                            _bcm_kt2_lls_info_t *lls_tree)
{
    int alloc_size, speed;
    _bcm_kt2_cosq_node_t *cur_node, *parent;
    uint32 *min_entries, *max_entries;
    uint32 max_shaper, min, max, flags;
    uint32 burst, mode;
    int count = 0;
    int num_entries = 0;

    parent = node->parent;
    if ((parent == NULL) && 
        (node->level != _BCM_KT2_COSQ_NODE_LEVEL_ROOT)) {
        return BCM_E_INTERNAL;
    }

    num_entries = (node->level != _BCM_KT2_COSQ_NODE_LEVEL_ROOT) ?
                  parent->num_child : 1;

    /* Allocate memory based on child nodes */
    alloc_size = sizeof(uint32) * num_entries;
    lls_tree->min_entries[node->level] = sal_alloc(alloc_size, "lls_war_buf");
    lls_tree->max_entries[node->level] = sal_alloc(alloc_size, "lls_war_buf");

    min_entries = lls_tree->min_entries[node->level];
    max_entries = lls_tree->max_entries[node->level];

    sal_memset(min_entries, 0, alloc_size);
    sal_memset(max_entries, 0, alloc_size);
    lls_tree->node_count[node->level] = num_entries; 

    /*
     *  Reserve 10% for the node which is being flushes and 
     *  distribute remaining bandwidth across other nodes
     */
    speed = _BCM_KT2_COSQ_MAX_FLUSH_RATE;
    max_shaper = ((speed * 9)/10)/lls_tree->node_count[node->level];

    if (node->level == _BCM_KT2_COSQ_NODE_LEVEL_ROOT) {
        BCM_IF_ERROR_RETURN
            (bcm_kt_port_rate_egress_get(unit, port, &max, &burst, &mode));
        min_entries[count] = 0;
        max_entries[count] = max;
        BCM_IF_ERROR_RETURN
            (bcm_kt_port_rate_egress_set(unit, port, speed, burst, mode));
        return BCM_E_NONE;
    }

    /* store configured bandwidth of all the nodes */
    for (cur_node = parent->child; cur_node != NULL;
         cur_node = cur_node->sibling) {
         BCM_IF_ERROR_RETURN
             (bcm_kt2_cosq_gport_bandwidth_get(unit, cur_node->gport, 0, 
                                          &min, &max, &flags));
         min_entries[count] = min;
         max_entries[count] = max;
         count++;
         if (cur_node->hw_index == node->hw_index) {
             BCM_IF_ERROR_RETURN
                 (bcm_kt2_cosq_gport_bandwidth_set(unit, cur_node->gport, 0, 
                                          (speed / 10), speed, flags));
         } else {
             BCM_IF_ERROR_RETURN
                 (bcm_kt2_cosq_gport_bandwidth_set(unit, cur_node->gport, 0, 
                                              0, max_shaper, flags));
         }
    }

    return BCM_E_NONE;
}

STATIC int 
_bcm_kt2_cosq_lls_shapers_restore(int unit, int port,
                            _bcm_kt2_cosq_node_t *node,
                            _bcm_kt2_lls_info_t *lls_tree)
{
    _bcm_kt2_cosq_node_t *cur_node, *parent;
    uint32 *min_entries, *max_entries;
    uint32 min, max;
    uint32 burst, mode;
    int count = 0;

    parent = node->parent;
    if ((parent == NULL) && 
        (node->level != _BCM_KT2_COSQ_NODE_LEVEL_ROOT)) {
        return BCM_E_INTERNAL;
    }

    min_entries = lls_tree->min_entries[node->level];
    max_entries = lls_tree->max_entries[node->level];

    if (node->level == _BCM_KT2_COSQ_NODE_LEVEL_ROOT) {
        BCM_IF_ERROR_RETURN
            (bcm_kt_port_rate_egress_get(unit, port, &max, &burst, &mode));
        max = max_entries[count];    
        BCM_IF_ERROR_RETURN
            (bcm_kt_port_rate_egress_set(unit, port, max, burst, mode));
        return BCM_E_NONE;
    }

    /* restore bandwidth */
    for (cur_node = parent->child; cur_node != NULL;
         cur_node = cur_node->sibling) {
        min = min_entries[count];
        max = max_entries[count];    
        count++;

        BCM_IF_ERROR_RETURN
            (bcm_kt2_cosq_gport_bandwidth_set(unit, cur_node->gport, 0, 
                                         min, max, 0));
    }

    return BCM_E_NONE;
}

STATIC int
_bcm_kt2_cosq_poll_flush_active(int unit, bcm_port_t port, 
                                _bcm_kt2_cosq_node_t *node)
{
    soc_timeout_t timeout;
    int flush_active = 1;
    uint32 timeout_val;
    uint32 rval = 0;

    timeout_val = soc_property_get(unit, spn_MMU_QUEUE_FLUSH_TIMEOUT, 
                                   20000);
    soc_timeout_init(&timeout, timeout_val, 0);

    while (flush_active) {                              
        BCM_IF_ERROR_RETURN(READ_TOQ_FLUSH0r(unit, &rval)); 
        flush_active = soc_reg_field_get(unit, TOQ_FLUSH0r, 
                                         rval, FLUSH_ACTIVEf);
        if (soc_timeout_check(&timeout)) {
            return (BCM_E_TIMEOUT);
         }
    }

    return BCM_E_NONE;
}

STATIC int
_bcm_kt2_cosq_adjust_lls_bw(int unit, bcm_gport_t gport, 
                            _bcm_kt2_cosq_node_t *node)
{
    int rv = BCM_E_NONE;
    _bcm_kt2_lls_info_t lls_tree;
    _bcm_kt2_cosq_node_t *cur_node;
    bcm_port_t port;
    int count = 0, i;

    /* resolve the port */
    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(gport) || 
        BCM_GPORT_IS_SCHEDULER(gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_node_get(unit, gport, NULL, &port, NULL,
                                   NULL));
    } else {
        /* optionally validate port */
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_localport_resolve(unit, gport, &port));
        if (port < 0) {
            return BCM_E_PORT;
        }
    }

    sal_memset(&lls_tree, 0, sizeof(_bcm_kt2_lls_info_t));
    cur_node =  node;
    while (cur_node != NULL) {
        count++;
        rv = _bcm_kt2_cosq_lls_shapers_remove(unit, port, cur_node, &lls_tree); 
        if (rv != BCM_E_NONE) {
            goto cleanup;
        }

        rv = _bcm_kt2_cosq_poll_flush_active(unit, port, node);
        if (rv == BCM_E_NONE) {
            break;
        } else {
            cur_node = cur_node->parent;
        }
    }

cleanup:
    cur_node = node; 
    for (i = 0; i < count; i++) {
        rv = _bcm_kt2_cosq_lls_shapers_restore(unit, port, cur_node, &lls_tree);
        if (rv != BCM_E_NONE) {
            goto cleanup;
        }
        cur_node = cur_node->parent;
    } 

    for (i = _BCM_KT2_COSQ_NODE_LEVEL_ROOT; i < _BCM_KT2_COSQ_NODE_LEVEL_MAX;
         i++) {
        if (lls_tree.min_entries[i]) {
            sal_free(lls_tree.min_entries[i]);
            lls_tree.min_entries[i] = NULL;
        }
        if (lls_tree.max_entries[i]) {
            sal_free(lls_tree.max_entries[i]);
            lls_tree.max_entries[i] = NULL;
        }
    }

    return rv;
}


/*
 * Function:
 *     _bcm_kt2_cosq_alloc_clear
 * Purpose:
 *     Allocate cosq memory and clear
 * Parameters:
 *     size       - (IN) size of memory required
 *     description- (IN) description about the structure
 * Returns:
 *     BCM_E_XXX
 */
STATIC void *
_bcm_kt2_cosq_alloc_clear(unsigned int size, char *description)
{
    void *block_p;

    block_p = sal_alloc(size, description);

    if (block_p != NULL) {
        sal_memset(block_p, 0, size);
    }

    return block_p;
}

/*
 * Function:
 *     _bcm_kt2_cosq_free_memory
 * Purpose:
 *     Free memory allocated for mmu info members
 * Parameters:
 *     mmu_info_p       - (IN) MMU info pointer
 * Returns:
 *     BCM_E_XXX
 */
STATIC void
_bcm_kt2_cosq_free_memory(_bcm_kt2_mmu_info_t *mmu_info_p)
{
    int i;

    if (mmu_info_p->port != NULL) {
        sal_free(mmu_info_p->port);
    }
    if (mmu_info_p->l1_gport_pair != NULL) {
        sal_free(mmu_info_p->l1_gport_pair);
    }
    if (mmu_info_p->ext_qlist.bits != NULL) {
        sal_free(mmu_info_p->ext_qlist.bits);
    }
    if (mmu_info_p->dmvoq_qlist.bits != NULL) {
        sal_free(mmu_info_p->dmvoq_qlist.bits);
    }
    if (mmu_info_p->sched_list.bits != NULL) {
        sal_free(mmu_info_p->sched_list.bits);
    }
    if (mmu_info_p->l2_base_qlist.bits != NULL) {
        sal_free(mmu_info_p->l2_base_qlist.bits);
    }
    if (mmu_info_p->l2_base_mcqlist.bits != NULL) {
        sal_free(mmu_info_p->l2_base_mcqlist.bits);
    }    
    if (mmu_info_p->l2_ext_qlist.bits != NULL) {
        sal_free(mmu_info_p->l2_ext_qlist.bits);
    }
    if (mmu_info_p->l2_dmvoq_qlist.bits != NULL) {
        sal_free(mmu_info_p->l2_dmvoq_qlist.bits);
    }
    for (i = 0; i < _BCM_KT2_COSQ_NODE_LEVEL_L2; i++) { 
        if (mmu_info_p->sched_hw_list[i].bits != NULL) {
            sal_free(mmu_info_p->sched_hw_list[i].bits);
        }
    } 
    if (mmu_info_p->sched_node != NULL) {
        sal_free(mmu_info_p->sched_node);
    }

    if (mmu_info_p->queue_node != NULL) {
        sal_free(mmu_info_p->queue_node);
    }
}

int
_bcm_kt2_cosq_port_resolve(int unit, bcm_gport_t gport, bcm_module_t *modid,
                          bcm_port_t *port, bcm_trunk_t *trunk_id, int *id,
                          int *qnum)
{
    _bcm_kt2_cosq_node_t *node;

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_node_get(unit, gport, modid, port, NULL, &node));
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
 *     _bcm_kt2_cosq_gport_traverse
 * Purpose:
 *     Walks through the valid COSQ GPORTs and calls
 *     the user supplied callback function for each entry.
 * Parameters:
 *     unit       - (IN) unit number
 *     gport      - (IN)
 *     cb         - (IN) Callback function.
 *     user_data  - (IN) User data to be passed to callback function
 * Returns:
 *     BCM_E_NONE - Success.
 *     BCM_E_XXX
 */
int
_bcm_kt2_cosq_gport_traverse(int unit, bcm_gport_t gport,
                            bcm_cosq_gport_traverse_cb cb,
                            void *user_data)
{
    _bcm_kt2_cosq_node_t *node;
    uint32 flags = BCM_COSQ_GPORT_SCHEDULER;
    int rv = BCM_E_NONE;

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_node_get(unit, gport, NULL, NULL, NULL, &node));

    if (node != NULL) {
       if (node->level == _BCM_KT2_COSQ_NODE_LEVEL_L2) {
           if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(node->gport)) {
               flags = BCM_COSQ_GPORT_UCAST_QUEUE_GROUP;
           } else if (BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(gport)) {
               flags = BCM_COSQ_GPORT_DESTMOD_UCAST_QUEUE_GROUP; 
           } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(node->gport)) {
               flags = BCM_COSQ_GPORT_MCAST_QUEUE_GROUP;
           } else {
               flags = BCM_COSQ_GPORT_SUBSCRIBER;
           }       
       }
       rv = cb(unit, gport, node->numq, flags,
                  node->gport, user_data);
       if (BCM_FAILURE(rv)) {
#ifdef BCM_CB_ABORT_ON_ERR
           if (SOC_CB_ABORT_ON_ERR(unit)) {
               return rv;
           }
#endif
       }
    } else {
            return BCM_E_NONE;
    }

    if (node->sibling != NULL) {
        _bcm_kt2_cosq_gport_traverse(unit, node->sibling->gport, cb, user_data);
    }

    if (node->child != NULL) {
        _bcm_kt2_cosq_gport_traverse(unit, node->child->gport, cb, user_data);
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_index_resolve
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
_bcm_kt2_cosq_index_resolve(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                           _bcm_kt2_cosq_index_style_t style,
                           bcm_port_t *local_port, int *index, int *count)
{
    _bcm_kt2_cosq_node_t *node, *cur_node;
    bcm_port_t resolved_port;
    int resolved_index;
    int id, startq, numq = 0;
    _bcm_kt2_mmu_info_t *mmu_info;

    if (cosq < -1) {
        return BCM_E_PARAM;
    } else if (cosq == -1) {
        startq = 0;
    } else {
        startq = cosq;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
        BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port) || 
        BCM_GPORT_IS_SCHEDULER(port)) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_node_get(unit, port, NULL, &resolved_port, &id,
                                   &node));
        if (node->cosq_attached_to < 0) { /* Not resolved yet */
            return BCM_E_NOT_FOUND;
        }
        if (BCM_GPORT_IS_SCHEDULER(port)) {
            numq = node->numq;
            if (startq >= numq) {
                return BCM_E_PARAM;
            }
        } else {
            numq = node->numq;
        }    
    } else {
        /* optionally validate port */
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_localport_resolve(unit, port, &resolved_port));
        if (resolved_port < 0) {
            return BCM_E_PORT;
        }
        node = NULL;
        numq = 0;
    }

    switch (style) {
    case _BCM_KT2_COSQ_INDEX_STYLE_BUCKET:
        if (node != NULL) {
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port)) {
                resolved_index = node->hw_index;
            } else if (BCM_GPORT_IS_SCHEDULER(port)) {
                if (node->level > _BCM_KT2_COSQ_NODE_LEVEL_L1) {
                    return BCM_E_PARAM;
                }
                resolved_index = node->hw_index;
            } else {
                    return BCM_E_PARAM;
            }
        } else { /* node == NULL */
            mmu_info = _bcm_kt2_mmu_info[unit];
            numq = mmu_info->port[resolved_port].q_limit - 
                    mmu_info->port[resolved_port].q_offset;
            
            if (cosq >= numq) {
                return BCM_E_PARAM;
            }
            resolved_index = mmu_info->port[resolved_port].q_offset + startq;
        }
        break;

    case _BCM_KT2_COSQ_INDEX_STYLE_WRED:
        if (node != NULL) {
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port)) {
                resolved_index = node->hw_index;
            } else if (BCM_GPORT_IS_SCHEDULER(port)) {
                if (node->level != _BCM_KT2_COSQ_NODE_LEVEL_L1) {
                    return BCM_E_PARAM;
                }
                resolved_index = node->hw_index;
            } else {
                    return BCM_E_PARAM;
            }
        } else { /* node == NULL */
            mmu_info = _bcm_kt2_mmu_info[unit];
            numq = mmu_info->port[resolved_port].q_limit - 
                   mmu_info->port[resolved_port].q_offset;

            if (cosq >= numq) {
                return BCM_E_PARAM;
            }
            resolved_index = mmu_info->port[resolved_port].q_offset + startq;
        }
        break;

    case _BCM_KT2_COSQ_INDEX_STYLE_SCHEDULER:
        if (node != NULL) {
            if (BCM_GPORT_IS_SCHEDULER(port)) {
                for (cur_node = node->child; cur_node != NULL;
                    cur_node = cur_node->sibling) {
                    if (cur_node->cosq_attached_to == startq) {
                        break;
                    }
                }

                if (cur_node == NULL) {
                    /* this cosq is not yet attached */
                    return BCM_E_INTERNAL;
                }

                if (node->numq <= 8) {
                    if (soc_feature(unit, soc_feature_mmu_packing)) {
                        resolved_index = cur_node->hw_index;
                    } else {
                        resolved_index = node->base_index + startq;
                    }
                } else {
                    resolved_index = cur_node->hw_index;
                }
            } else { /* unicast queue group or multicast queue group */
                return BCM_E_PARAM;
            }
        } else { /* node == NULL */
            mmu_info = _bcm_kt2_mmu_info[unit];
            numq = mmu_info->port[resolved_port].q_limit -
                   mmu_info->port[resolved_port].q_offset;

            if (cosq >= numq) {
                return BCM_E_PARAM;
            }
            resolved_index = mmu_info->port[resolved_port].q_offset + startq;
        }
        break;
    case _BCM_KT2_COSQ_INDEX_STYLE_UCAST_QUEUE:
        mmu_info = _bcm_kt2_mmu_info[unit];
        if (node != NULL) {
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
                BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port)) {
                resolved_index = node->hw_index;
            } else if (BCM_GPORT_IS_SCHEDULER(port)) {
                if (node->level != _BCM_KT2_COSQ_NODE_LEVEL_L1) {
                    return BCM_E_PARAM;
                }
                resolved_index = node->base_index;
            } else {
                return BCM_E_PARAM;
            }
            
            if ((BCM_GPORT_IS_UCAST_QUEUE_GROUP(port)) || (BCM_GPORT_IS_MCAST_QUEUE_GROUP(port))) {
                resolved_index -= mmu_info->port[resolved_port].q_offset;
            }
        } else { /* node == NULL */
            mmu_info = _bcm_kt2_mmu_info[unit];
            numq = mmu_info->port[resolved_port].q_limit -
                   mmu_info->port[resolved_port].q_offset;

            if (startq >= numq) {
                return BCM_E_PARAM;
            }
            resolved_index = startq;        
        }
        break;

    default:
        return BCM_E_INTERNAL;
    }

    if (local_port != NULL) {
        *local_port = resolved_port;
    }
    if (index != NULL) {
        *index = resolved_index;
    }
    if (count != NULL) {
        *count = (cosq == -1) ? numq : 1;
    }

    return BCM_E_NONE;
}
/*
 * Function:
 *     _bcm_kt2_cosq_get_dmvoq_index
 * Purpose:
 *  This logic is to make sure that the base index is always
 *  multiple of 8
 *     mmu_info       - (IN) MMU info structure 
 *     list       - (IN)list from which index need to be selected 
 *     numq       - (IN) number of queues
 *     id       - (IN) START ID 
 *     limit       - (IN) limit
 * Returns:
 *     BCM_E_XXX
 */


STATIC int
_bcm_kt2_cosq_get_dmvoq_index(
        _bcm_kt2_mmu_info_t *mmu_info,
        _bcm_kt2_cosq_list_t *list,
        int numq, int *id , int limit ) 
{
     while (((mmu_info->num_base_queues + *id) % 8) != 0) {
              *id = (*id + 8 - ((mmu_info->num_base_queues + *id) % 8)); 
               BCM_IF_ERROR_RETURN
                  (_bcm_kt2_node_index_get(list->bits, *id, limit,
                                           mmu_info->qset_size, numq, id));
           }
     return BCM_E_NONE;
}
/*
 * Function:
 *     _bcm_kt2_cosq_node_resolve
 * Purpose:
 *     Resolve queue number in the specified scheduler tree and its subtree
 * Parameters:
 *     unit       - (IN) unit number
 *     node       - (IN) node structure for the specified scheduler node
 *     cosq       - (IN) COS queue to attach to
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_cosq_node_resolve(int unit, _bcm_kt2_cosq_node_t *node,
                          bcm_cos_queue_t cosq)
{
    _bcm_kt2_cosq_node_t *parent;
    _bcm_kt2_mmu_info_t *mmu_info;
    _bcm_kt2_cosq_list_t *list;
    bcm_port_t local_port;
    int start_idx, end_idx;
    int base_idx;
    int alloc_size;
    int numq, id = 0;
    int offset, limit;

    parent = node->parent;
    if (parent == NULL) {
        /* Not attached, should never happen */
        return BCM_E_NOT_FOUND;
    }

    if (parent->numq == 0 || parent->numq < -1) {
        return BCM_E_PARAM;
    }
   
    if (parent->cosq_map == NULL) {
        alloc_size = SHR_BITALLOCSIZE(parent->numq);
        parent->cosq_map = _bcm_kt2_cosq_alloc_clear(alloc_size,
                                           "cosq_map");
        if (parent->cosq_map == NULL) {
            return BCM_E_MEMORY;
        }
    }
 
    if (cosq < 0) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_map_index_get(parent->cosq_map,
                                        0, parent->numq, &id));
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_map_index_set(parent->cosq_map, id, 1));
        node->cosq_attached_to = id;
    } else {
        if (_bcm_kt2_cosq_map_index_is_set(parent->cosq_map,
                                  cosq) == TRUE) {
            return BCM_E_EXISTS;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_map_index_set(parent->cosq_map, cosq, 1));
        node->cosq_attached_to = cosq;
    } 

    mmu_info = _bcm_kt2_mmu_info[unit];
    numq = (parent->wrr_mode == 0) ? ((parent->numq > 8) ? 8 : parent->numq) : 1;

    switch (parent->level) {
        case _BCM_KT2_COSQ_NODE_LEVEL_ROOT:
        case _BCM_KT2_COSQ_NODE_LEVEL_S1:    
        case _BCM_KT2_COSQ_NODE_LEVEL_L0:
            list = &mmu_info->sched_hw_list[node->level];
            if (node->level == _BCM_KT2_COSQ_NODE_LEVEL_S0) {
                node->hw_index = (BCM_GPORT_SCHEDULER_GET(node->gport) >> 8);
                node->hw_index &= 0x7FF;
                /* s0 index = pp_port_id - 42 */
                node->hw_index -= num_port_schedulers;
                if (parent->base_index < 0) {
                    parent->base_index = node->hw_index;
                }
                _bcm_kt2_node_index_set(list, node->hw_index, 1);
                break;
            }
            if (parent->level == _BCM_KT2_COSQ_NODE_LEVEL_S1 &&
                node->level == _BCM_KT2_COSQ_NODE_LEVEL_L0) {
                /* S1 == L0 incase of linkphy */
                parent->base_index = parent->hw_index;
                node->hw_index = parent->hw_index;   
                break;
            }

            /* Indexes from 0 to num_s1_schedulers is used for S1 nodes, 
               remaining num_s1_schedulers++ are used for L0 and S0 */
            start_idx = (node->level == _BCM_KT2_COSQ_NODE_LEVEL_L0) ? 
                         num_s1_schedulers : 0;
            end_idx = mmu_info->max_nodes[node->level];
            if (parent->base_index < 0) {
                BCM_IF_ERROR_RETURN
                    (_bcm_kt2_node_index_get(list->bits, start_idx, end_idx,
                                    mmu_info->qset_size, numq, &id));
                _bcm_kt2_node_index_set(list, id, numq);
                parent->base_index = id;
                
            } else if (node->cosq_attached_to >= numq) {
                BCM_IF_ERROR_RETURN
                    (_bcm_kt2_node_index_get(list->bits, start_idx, end_idx,
                                    mmu_info->qset_size, 1, &id));
                _bcm_kt2_node_index_set(list, id, 1);
            }    
            break;
            
        case _BCM_KT2_COSQ_NODE_LEVEL_S0:
            list = &mmu_info->sched_hw_list[node->level];
            if (parent->linkphy_enabled) {
                if (node->cosq_attached_to > 1) {
                    return BCM_E_PARAM;
                }
                parent->base_index = parent->hw_index;
                node->hw_index = (parent->hw_index * 2) + 
                                 node->cosq_attached_to;
                node->linkphy_enabled = 1;
                _bcm_kt2_node_index_set(list, node->hw_index, 1);
            } else {
                if (node->level != _BCM_KT2_COSQ_NODE_LEVEL_L0) {
                    return BCM_E_PARAM;
                }
                end_idx = mmu_info->max_nodes[node->level];
                if (parent->base_index < 0) {
                    BCM_IF_ERROR_RETURN
                    (_bcm_kt2_node_index_get(list->bits, num_s1_schedulers, end_idx,
                                    mmu_info->qset_size, numq, &id));
                    _bcm_kt2_node_index_set(list, id, numq);
                    parent->base_index = id;
                } else if (node->cosq_attached_to >= numq) {
                    BCM_IF_ERROR_RETURN
                    (_bcm_kt2_node_index_get(list->bits, num_s1_schedulers, end_idx,
                                    mmu_info->qset_size, 1, &id));
                    _bcm_kt2_node_index_set(list, id, 1);
               }    
            }
            break;
        
        case _BCM_KT2_COSQ_NODE_LEVEL_L1:
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(node->gport)) {
                list = &mmu_info->l2_base_qlist;
                local_port = BCM_GPORT_UCAST_QUEUE_GROUP_SYSPORTID_GET
                                                         (node->gport);
                offset = mmu_info->port[local_port].q_offset;
                limit =  mmu_info->port[local_port].q_limit;
            } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(node->gport)) {
                list = &mmu_info->l2_base_mcqlist;
                local_port = BCM_GPORT_MCAST_QUEUE_GROUP_SYSPORTID_GET
                                                         (node->gport);
                offset = mmu_info->port[local_port].q_offset;
                limit =  mmu_info->port[local_port].q_limit;
            } else if (BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(node->gport)) {
                local_port = BCM_GPORT_DESTMOD_QUEUE_GROUP_SYSPORTID_GET
                                                         (node->gport);
                offset = 0;
                if (mmu_info->num_dmvoq_queues > 0) {
                    list = &mmu_info->l2_dmvoq_qlist;
                    limit = mmu_info->num_dmvoq_queues;
                } else {
                    list = &mmu_info->l2_ext_qlist;
                    limit = mmu_info->num_ext_queues;
                }
            } else {
                list = &mmu_info->l2_ext_qlist;
                offset = 0;
                limit = mmu_info->num_ext_queues;
            }
   
            if (node->hw_index < 0) {    
                if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(node->gport) && 
                    parent->mc_base_index < 0 && 
                    node->cosq_attached_to < 8) {
                    BCM_IF_ERROR_RETURN
                        (_bcm_kt2_node_index_get(list->bits, offset, limit,
                                            mmu_info->qset_size, numq, &id));
                    _bcm_kt2_node_index_set(list, id, numq);
                    id += 2048;
                    parent->mc_base_index = id;
                    node->mc_base_index = parent->mc_base_index;  
                } else if ((BCM_GPORT_IS_UCAST_QUEUE_GROUP(node->gport) ||
                       BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(node->gport)) &&
                       parent->base_index < 0 && 
                       node->cosq_attached_to < 8) {
                    BCM_IF_ERROR_RETURN
                        (_bcm_kt2_node_index_get(list->bits, offset, limit,
                                            mmu_info->qset_size, numq, &id));
                    _bcm_kt2_node_index_set(list, id, numq);
                    parent->base_index = (BCM_GPORT_IS_UCAST_QUEUE_GROUP
                            (node->gport)) ? id :
                        (id + mmu_info->num_base_queues);
                    node->base_index = parent->base_index;  
                } else if (BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(node->gport) && 
                        parent->base_index < 0 && 
                        node->cosq_attached_to < 8) {
                    BCM_IF_ERROR_RETURN
                        (_bcm_kt2_node_index_get(list->bits, offset, limit,
                                                 mmu_info->qset_size, numq, &id));
                    if (((mmu_info->num_base_queues + id) % 8) != 0) {
                        BCM_IF_ERROR_RETURN
                            (_bcm_kt2_cosq_get_dmvoq_index(mmu_info, list,
                                                           numq, &id, limit)); 
                    }
                    _bcm_kt2_node_index_set(list, id, numq);
                    if (mmu_info->num_dmvoq_queues > 0) { 
                        parent->base_index = id + mmu_info->base_dmvoq_queue ;
                    } else {
                        parent->base_index = id + mmu_info->num_base_queues ;
                    }
                    node->base_index = parent->base_index;  
                } else if (node->cosq_attached_to >= 8) {
                    BCM_IF_ERROR_RETURN
                        (_bcm_kt2_node_index_get(list->bits, offset, limit,
                                           mmu_info->qset_size, numq, &id));
                    if (node->type == _BCM_KT2_NODE_VOQ) {
                        return BCM_E_PARAM; 
                    }
                    _bcm_kt2_node_index_set(list, id, 1);                
                }
                id += (BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(node->gport)) ? 
                       mmu_info->num_base_queues : 0;    
            } else {
                if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(node->gport) ||
                    BCM_GPORT_IS_MCAST_QUEUE_GROUP(node->gport) ||
                    BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(node->gport)) {
                    /*input 0 should be atttached first to get the correct base*/
                    if (node->cosq_attached_to < 8) {
                        if ((BCM_GPORT_IS_UCAST_QUEUE_GROUP(node->gport) ||
                            (BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(node->gport))) && 
                            parent->base_index < 0) {
                            parent->base_index = node->hw_index;
                        }    
                        if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(node->gport) && 
                            parent->mc_base_index < 0) {
                            parent->mc_base_index = node->hw_index;
                        }
                    }
                    
                } 
            }
            break;

        case _BCM_KT2_COSQ_NODE_LEVEL_L2:
            /* passthru */
        default:
            return BCM_E_PARAM;

    }

    if (parent->numq <= 8) {
        if (parent->level <= _BCM_KT2_COSQ_NODE_LEVEL_L1) {
            if (node->hw_index < 0) {
                base_idx = (BCM_GPORT_IS_MCAST_QUEUE_GROUP(node->gport)) ?
                            parent->mc_base_index : parent->base_index;
                node->hw_index = base_idx + node->cosq_attached_to;
            }    
        }

        parent->num_child++;
        /* get the lowest cosq as a first child */
        if (node->hw_index < parent->first_child) {
            parent->first_child = node->hw_index;
        }
    } else {
        if (parent->level <= _BCM_KT2_COSQ_NODE_LEVEL_L1) {
            if (node->hw_index < 0) {
                base_idx = (BCM_GPORT_IS_MCAST_QUEUE_GROUP(node->gport)) ?
                            parent->mc_base_index : parent->base_index;
                node->hw_index = (node->cosq_attached_to < numq) ? 
                    (base_idx + node->cosq_attached_to) : id;
            }
            if (node->cosq_attached_to >= numq) {
                node->wrr_in_use = 1;
            }    
        }
        parent->num_child++;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_node_unresolve
 * Purpose:
 *     Unresolve queue number in the specified scheduler tree and its subtree
 * Parameters:
 *     unit       - (IN) unit number
 *     node       - (IN) node structure for the specified scheduler node
 *     cosq       - (IN) COS queue to attach to
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_cosq_node_unresolve(int unit, _bcm_kt2_cosq_node_t *node, 
                             bcm_cos_queue_t cosq)
{
    _bcm_kt2_cosq_node_t *cur_node, *parent;
    _bcm_kt2_mmu_info_t *mmu_info;
    _bcm_kt2_cosq_list_t *list;
    uint32 map;
    int min_index;
    int id, offset, numq;
    int min_flag = 0;

    if (node->cosq_attached_to < 0) {
        /* Has been unresolved already */
        return BCM_E_NONE;
    }

    parent = node->parent;

    if (parent->numq == 0 || parent->numq < -1) {
        return BCM_E_PARAM;
    }

    if (parent->numq <= 8) {
         /* find the current cosq_attached_to usage */
        map = 0;
        if (parent->first_child == node->hw_index && node->wrr_in_use == 0) {
            min_flag = 1;
            min_index = (node->level == _BCM_KT2_COSQ_NODE_LEVEL_L2) ? 4095 :
                        ((node->level == _BCM_KT2_COSQ_NODE_LEVEL_L1) ? 1023 :
                          255);
        } else {
            min_index = parent->first_child;
        }

        for (cur_node = parent->child; cur_node != NULL;
            cur_node = cur_node->sibling) {
            if (cur_node->cosq_attached_to >= 0) {
                map |= 1 << cur_node->cosq_attached_to;
                /* dont update the min node is not this node */
                if (min_flag && cur_node->hw_index != node->hw_index) {
                if (cur_node->hw_index < min_index) {
                    min_index = cur_node->hw_index;
                }
               }
            }
        }

        if (!(map & (1 << node->cosq_attached_to))) {
            return BCM_E_PARAM;
        }

        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_map_index_clear(parent->cosq_map,
             node->cosq_attached_to, 1));
        parent->first_child = min_index;

    } else {

        /* check whether the entry is available */
        for (cur_node = parent->child; cur_node != NULL;
            cur_node = cur_node->sibling) {
            if (cur_node->gport == node->gport) {
                break;
            }
        }

        if (cur_node == NULL) {
            return BCM_E_PARAM;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_map_index_clear(parent->cosq_map,
             cur_node->cosq_attached_to, 1));
    }

    mmu_info = _bcm_kt2_mmu_info[unit];
    numq = (parent->wrr_mode == 0) ? ((parent->numq > 8) ? 8 : parent->numq) : 1;

    switch (parent->level) {
        case _BCM_KT2_COSQ_NODE_LEVEL_ROOT:
        case _BCM_KT2_COSQ_NODE_LEVEL_S0:
        case _BCM_KT2_COSQ_NODE_LEVEL_S1:
        case _BCM_KT2_COSQ_NODE_LEVEL_L0:    
            list = &mmu_info->sched_hw_list[node->level];
            parent->num_child--;
            if ((node->level != _BCM_KT2_COSQ_NODE_LEVEL_S0) && 
                (parent->num_child >= 0)) {
                if (node->cosq_attached_to >= numq) {
                    _bcm_kt2_node_index_clear(list, node->hw_index, 1);
                }
                if (parent->num_child == 0 &&
                    parent->base_index >= 0) {
                    /* release contiguous queue ids */
                    _bcm_kt2_node_index_clear(list, parent->base_index, numq);
                    parent->base_index = -1;
                }    
            } 
            if (node->level == _BCM_KT2_COSQ_NODE_LEVEL_S0) {
                _bcm_kt2_node_index_clear(list, node->hw_index, 1);
            }
            break;
            
        case _BCM_KT2_COSQ_NODE_LEVEL_L1:
            if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(node->gport)) {
                list = &mmu_info->l2_base_qlist;
                offset = 0;
            } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(node->gport)) {
                list = &mmu_info->l2_base_mcqlist;
                offset = 2048;
            } else if (BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(node->gport) 
                    && (mmu_info->num_dmvoq_queues > 0)) {
                list = &mmu_info->l2_dmvoq_qlist;
                offset = mmu_info->base_dmvoq_queue;
            } else {
                list = &mmu_info->l2_ext_qlist;
                offset = mmu_info->num_base_queues;
            }            

            parent->num_child--;
            id = node->hw_index - offset;
            if (parent->num_child >= 0) {
                if (node->cosq_attached_to >= 8) {
                    _bcm_kt2_node_index_clear(list, id, 1);
                }
                if (parent->num_child == 0 &&
                    parent->base_index >= 0) {
                    /* release contiguous queue ids */
                    id = parent->base_index - offset;
                    _bcm_kt2_node_index_clear(list, id, numq);
                    parent->base_index = -1;
                }    
            }    
            break;

        case _BCM_KT2_COSQ_NODE_LEVEL_L2:
            /* passthru */
        default:
            return BCM_E_PARAM;

        }

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_valid_dynamic_request
 * Purpose:
 *     Validate the new dynamic request
 * Parameters:
 *     unit       - (IN) unit number
 *     port      - (IN) port
 *     level      - (IN) scheduler level
 *     node_id    - (IN) node index
 *     parent_id  - (IN) parent index
 *     rval        - (IN) dynamic reg value
 * Returns:
 *     BCM_E_XXX
 */

STATIC int
_bcm_kt2_cosq_valid_dynamic_request(int unit, bcm_port_t port, int node_level, 
                                   int node_id, int parent_id, uint32 rval) 
{
    soc_reg_t dyn_reg = LLS_SP_WERR_DYN_CHANGE_0Ar;
    lls_l0_parent_entry_t l0_parent;
    lls_l1_parent_entry_t l1_parent;
    bcm_port_t req_port, req_parent;

    /* dont allow more than one req per parent  */
    req_parent = soc_reg_field_get(unit, dyn_reg, rval, PARENT_IDf);
    if ((node_level == soc_reg_field_get(unit, dyn_reg, rval, NODE_LEVELf)) &&
        (parent_id == req_parent)) {
        return FALSE;
    }

    switch (node_level) {
        case _BCM_KT2_COSQ_NODE_LEVEL_L0:
            req_port = parent_id;
            break;
        case _BCM_KT2_COSQ_NODE_LEVEL_L1:
            SOC_IF_ERROR_RETURN
                (READ_LLS_L0_PARENTm(unit, MEM_BLOCK_ALL, req_parent,
                                     &l0_parent));
            req_port = soc_mem_field32_get(unit, LLS_L0_PARENTm, &l0_parent, 
                                           C_PARENTf); 
            break;
        case _BCM_KT2_COSQ_NODE_LEVEL_L2:
            SOC_IF_ERROR_RETURN
                (READ_LLS_L1_PARENTm(unit, MEM_BLOCK_ALL, req_parent,
                                     &l1_parent));
            req_port = soc_mem_field32_get(unit, LLS_L1_PARENTm, &l1_parent, 
                                           C_PARENTf); 
            SOC_IF_ERROR_RETURN
                (READ_LLS_L0_PARENTm(unit, MEM_BLOCK_ALL, req_port,
                                     &l0_parent));
            req_port = soc_mem_field32_get(unit, LLS_L0_PARENTm, &l0_parent, 
                                           C_PARENTf);    
            break;
        default:
            return FALSE;
    }

    /* dont allow more than one req per port */
    if (port == req_port) {
        return FALSE;
    }    
    return TRUE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_dynamic_sp_werr_change
 * Purpose:
 *     Set LLS dynamic scheduler registers based on level and hw index
 * Parameters:
 *     unit       - (IN) unit number
 *     level      - (IN) scheduler level
 *     node_id    - (IN) node index
 *     parent_id  - (IN) parent index
 *     config       - (IN) config details
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_cosq_dynamic_sp_werr_change(int unit, bcm_port_t port, int node_level, 
                                    int node_id, int parent_id, uint32 *config,
                                    _bcm_kt2_cosq_state_t state)
{
    soc_timeout_t timeout;
    uint32 timeout_val, rval;
    int i, in_use;
    soc_reg_t dyn_reg_a, dyn_reg_b, dyn_reg_c;
    lls_l0_xoff_entry_t l0_xoff;
    uint32 entry_b, entry_c;
    lls_l0_parent_entry_t l0_parent;
    
    soc_reg_t dyn_change_a[] = 
        {
        LLS_SP_WERR_DYN_CHANGE_0Ar,
        LLS_SP_WERR_DYN_CHANGE_1Ar,
        LLS_SP_WERR_DYN_CHANGE_2Ar,
        LLS_SP_WERR_DYN_CHANGE_3Ar
        };
    soc_reg_t dyn_change_b[] = 
        {
        LLS_SP_WERR_DYN_CHANGE_0Br,
        LLS_SP_WERR_DYN_CHANGE_1Br,
        LLS_SP_WERR_DYN_CHANGE_2Br,
        LLS_SP_WERR_DYN_CHANGE_3Br
        };
    soc_reg_t dyn_change_c[] = 
        {
        LLS_SP_WERR_DYN_CHANGE_0Cr,
        LLS_SP_WERR_DYN_CHANGE_1Cr,
        LLS_SP_WERR_DYN_CHANGE_2Cr,
        LLS_SP_WERR_DYN_CHANGE_3Cr
        };


    /* New value for the node_ids parent config bits 41:32
     * to be programmed into dynamic change register
     * LLS_SP_WERR_DYN_CHANGE_XB
     */
    entry_b = (config[1] & _BCM_KT2_LLS_DYN_CHG_REG_B);

    /* New value for the node_ids parent config bits 31:0.
     * to be programmed into dynamic change register 
     * LLS_SP_WERR_DYN_CHANGE_XC
     */
    entry_c = (config[0] & _BCM_KT2_LLS_DYN_CHG_REG_C);

    /* Get one of the free register to place the request */    
    for (i = 0; i < 4; i++) {
        dyn_reg_a =  dyn_change_a[i];
        dyn_reg_b =  dyn_change_b[i];
        dyn_reg_c =  dyn_change_c[i];
        SOC_IF_ERROR_RETURN(soc_reg32_get(unit, dyn_reg_a, REG_PORT_ANY, 
                                          0, &rval));
        if (SAL_BOOT_SIMULATION) {
            break;
        }    
        
        if (soc_reg_field_get(unit, dyn_reg_a, rval, IN_USEf) == 0) {
            break;
        } else {
        /* verify any other req pending from this port or same parent */
            if (_bcm_kt2_cosq_valid_dynamic_request(unit, port, node_level, 
                     node_id, parent_id, rval) == FALSE) {
                return BCM_E_UNAVAIL;     
            }                                                     
        }
    }    

    if (i == 4) {
        /* None of the registers are available */
        return BCM_E_UNAVAIL;
    }    

    rval = 0 ;
    /* subtract 2 due to addional level of s0 and s1 */
    soc_reg_field_set(unit, dyn_reg_a, &rval, NODE_LEVELf, (node_level - 2));
    soc_reg_field_set(unit, dyn_reg_a, &rval, NODE_IDf, node_id);
    soc_reg_field_set(unit, dyn_reg_a, &rval, IN_USEf, 1);
    if (node_level == _BCM_KT2_COSQ_NODE_LEVEL_L0) {
        SOC_IF_ERROR_RETURN
            (READ_LLS_L0_PARENTm(unit, MEM_BLOCK_ALL, node_id,
                                 &l0_parent));
        if (soc_mem_field32_get(unit, LLS_L0_PARENTm,
                                &l0_parent, C_TYPEf) == 1) {
            /* 
              For Ports, set parent_id[9:7] = 0x2, 
              set parent_id[6:0] = {1b0, port_id[5:0]}.
            */
            parent_id = ((0x2 << 7) | (parent_id & 0x3F));
        }
    }
    soc_reg_field_set(unit, dyn_reg_a, &rval, PARENT_IDf, parent_id);

    /* Write DYN_CHANGE_XB register first */
    SOC_IF_ERROR_RETURN(soc_reg32_set(unit, dyn_reg_c, REG_PORT_ANY, 
                                      0, entry_c));
    SOC_IF_ERROR_RETURN(soc_reg32_set(unit, dyn_reg_b, REG_PORT_ANY, 
                                      0, entry_b));
    /* Write DYN_CHANGE_XA register next. this triggers the request */
    SOC_IF_ERROR_RETURN(soc_reg32_set(unit, dyn_reg_a, REG_PORT_ANY, 
                                      0, rval));
    if (SAL_BOOT_SIMULATION) {
        return BCM_E_NONE;
    }
    
    in_use = 1;
    timeout_val = soc_property_get(unit, spn_MMU_QUEUE_FLUSH_TIMEOUT, 20000);
    soc_timeout_init(&timeout, timeout_val, 0);

    while (in_use) {                              
        SOC_IF_ERROR_RETURN
            (soc_reg32_get(unit, dyn_reg_a, REG_PORT_ANY, 0, &rval)); 
        in_use = soc_reg_field_get(unit, dyn_reg_a, rval, IN_USEf);
        if (soc_timeout_check(&timeout)) {
            LOG_ERROR(BSL_LS_BCM_COMMON,
                      (BSL_META_U(unit,
                                  "ERROR: dynamic sp <-> werr change operation failed \n")));
            LOG_ERROR(BSL_LS_BCM_COMMON,
                      (BSL_META_U(unit,
                                  " node_level %d node_id %d parent_id %d \n"), 
                       node_level, node_id, parent_id));
            return BCM_E_TIMEOUT;
         }
    }

    if (node_level == _BCM_KT2_COSQ_NODE_LEVEL_L1 && 
        state == _BCM_KT2_COSQ_STATE_WRR_TO_SPRI) {
        sal_memset(&l0_xoff, 0, sizeof(lls_l0_xoff_entry_t));
        soc_mem_field32_set(unit, LLS_L0_XOFFm, &l0_xoff, C_XOFFf,
                            (1 << (parent_id & 0xf)));     
        SOC_IF_ERROR_RETURN
            (WRITE_LLS_L0_XOFFm(unit, MEM_BLOCK_ALL, ((parent_id >> 4) & 0xf),
                                &l0_xoff));
        /* Add single lls register read delay */
        SOC_IF_ERROR_RETURN(soc_reg32_get(unit, dyn_reg_a, REG_PORT_ANY, 
                                          0, &rval));
        soc_mem_field32_set(unit, LLS_L0_XOFFm, &l0_xoff, C_XOFFf, 0);     
        SOC_IF_ERROR_RETURN
            (WRITE_LLS_L0_XOFFm(unit, MEM_BLOCK_ALL, ((parent_id >> 4) & 0xf),
                                &l0_xoff));
        SOC_IF_ERROR_RETURN(READ_LLS_DEBUG_INJECT_ACTIVATIONr(unit, &rval));
        soc_reg_field_set(unit, LLS_DEBUG_INJECT_ACTIVATIONr, &rval, 
                          INJECTf, 1);
        soc_reg_field_set(unit, LLS_DEBUG_INJECT_ACTIVATIONr, &rval, 
                          ENTITY_TYPEf, 2);
        soc_reg_field_set(unit, LLS_DEBUG_INJECT_ACTIVATIONr, &rval, 
                          ENTITY_LEVELf, 1);
        soc_reg_field_set(unit, LLS_DEBUG_INJECT_ACTIVATIONr, &rval, 
                          ENTITY_IDENTIFIERf, parent_id);
        SOC_IF_ERROR_RETURN(WRITE_LLS_DEBUG_INJECT_ACTIVATIONr(unit, rval));
    }

    return BCM_E_NONE;
}

int
_bcm_kt2_cosq_subport_get(int unit, bcm_gport_t sched_gport,
                          int *pp_port)
{
    int rv = BCM_E_NOT_FOUND;
    bcm_cos_queue_t cosq;
    bcm_gport_t gport;
    _bcm_kt2_cosq_node_t *node;
    int encap_id = 0;
    bcm_gport_t input_gport;

    gport = sched_gport; 
    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_node_get(unit, gport, NULL, NULL, NULL,
                               &node));

    while (node->level != _BCM_KT2_COSQ_NODE_LEVEL_ROOT) {
        BCM_IF_ERROR_RETURN
        (bcm_kt2_cosq_gport_attach_get(unit, gport, &input_gport, &cosq));
        gport = input_gport;
        if (BCM_GPORT_IS_SCHEDULER(gport)) {
            encap_id = (BCM_GPORT_SCHEDULER_GET(node->gport) >> 8) & 0x7ff;
            if (node->subport_enabled == 1) {
                *pp_port = encap_id;
                /*BCM_GPORT_SUBPORT_PORT_SET(*input_gport, encap_id);*/
                rv = BCM_E_NONE;
                break;
            }
        }
        BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_node_get(unit, gport, NULL, NULL, NULL,
                               &node));
    }

    return rv;
}

int
_bcm_kt2_cosq_packing_mode_set(int unit, int index, int set)
{
    int rv = BCM_E_NONE;
    mmu_toq_qpack_mode_entry_t qpack_entry;
    uint32 mode = 0;

    SOC_IF_ERROR_RETURN
        (READ_MMU_TOQ_QPACK_MODEm(unit, MEM_BLOCK_ALL, 
                                  index/16, &qpack_entry));
    soc_mem_field_get(unit, MMU_TOQ_QPACK_MODEm,
                      (uint32 *)&qpack_entry,
                      QMODEf, &mode);
    if (set) {
        mode |= (1 << (index % 16));
    } else {
        mode &= ~(1 << (index % 16));
    }

    soc_mem_field32_set(unit, MMU_TOQ_QPACK_MODEm, 
                        &qpack_entry, QMODEf, mode);           
    SOC_IF_ERROR_RETURN
        (WRITE_MMU_TOQ_QPACK_MODEm(unit, MEM_BLOCK_ALL, 
                                   index/16, &qpack_entry));

    return rv;
}

STATIC 
int _bcm_kt2_cosq_flush_queues(int unit, 
                               bcm_port_t port,
                               _bcm_kt2_cosq_node_t *node, 
                               bcm_gport_t gport)
{
   int flush_active = 0;
   uint32 rval = 0; 
   uint32 timeout_val = 0;
   soc_timeout_t timeout;

   /* flush the queue  */
   if (!(SAL_BOOT_SIMULATION)) {
       BCM_IF_ERROR_RETURN(READ_TOQ_FLUSH0r(unit, &rval)); 
       flush_active = soc_reg_field_get(unit, TOQ_FLUSH0r, rval, FLUSH_ACTIVEf);
       if (flush_active == 1) {
           /* some other queue is in flush state, return failure */
           LOG_ERROR(BSL_LS_BCM_COMMON,
                     (BSL_META_U(unit,
                                 "ERROR: Queue %d is already in flush state \n"),
                      (soc_reg_field_get(unit, TOQ_FLUSH0r, rval, FLUSH_ID0f))));
           return BCM_E_BUSY;  
       }
       soc_reg_field_set(unit, TOQ_FLUSH0r, &rval, FLUSH_TYPEf, 0);
       soc_reg_field_set(unit, TOQ_FLUSH0r, &rval, FLUSH_ID0f, node->hw_index);
       soc_reg_field_set(unit, TOQ_FLUSH0r, &rval, FLUSH_NUMf, 1);
       soc_reg_field_set(unit, TOQ_FLUSH0r, &rval, FLUSH_ACTIVEf, 1);
       if (IS_EXT_MEM_PORT(unit, port)) {
           soc_reg_field_set(unit, TOQ_FLUSH0r, &rval, FLUSH_EXTERNALf, 1);
       } else {
           soc_reg_field_set(unit, TOQ_FLUSH0r, &rval, FLUSH_EXTERNALf, 0);
       }
       BCM_IF_ERROR_RETURN(WRITE_TOQ_FLUSH0r(unit, rval)); 
       flush_active = 1;
       timeout_val = soc_property_get(unit, spn_MMU_QUEUE_FLUSH_TIMEOUT, 20000);
       soc_timeout_init(&timeout, timeout_val, 0);

       while (flush_active) {                              
              BCM_IF_ERROR_RETURN (READ_TOQ_FLUSH0r(unit, &rval)); 
              flush_active = soc_reg_field_get(unit, TOQ_FLUSH0r, rval, 
                                               FLUSH_ACTIVEf);
              if (soc_timeout_check(&timeout)) {
                  BCM_IF_ERROR_RETURN(_bcm_kt2_cosq_adjust_lls_bw(unit, gport, 
                                                                  node));
                  BCM_IF_ERROR_RETURN(READ_TOQ_FLUSH0r(unit, &rval)); 
                  flush_active = soc_reg_field_get(unit, TOQ_FLUSH0r, 
                                                   rval, FLUSH_ACTIVEf);
                  if (flush_active) {
                      LOG_ERROR(BSL_LS_BCM_COMMON,
                                (BSL_META_U(unit,
                                            "ERROR: Queue %d flush operation failed \n"), 
                                 node->hw_index));
                      return (BCM_E_TIMEOUT);
                  }
              }
       }
   }
   return BCM_E_NONE;
}
/*
 * Function:
 *     _bcm_kt2_cosq_sched_config_set
 * Purpose:
 *     Set LLS scheduler registers based on GPORT
 * Parameters:
 *     unit       - (IN) unit number
 *     gport     - (IN) queue group/scheduler GPORT identifier
 *     state     - (IN) sched state change
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_cosq_sched_config_set(int unit, bcm_gport_t gport, 
                               _bcm_kt2_cosq_state_t state)
{
    int rv = BCM_E_NONE;
    _bcm_kt2_mmu_info_t *mmu_info;
    _bcm_kt2_cosq_node_t *node, *parent;
    mmu_rqe_queue_op_node_map_entry_t rqe_op_node;
    mmu_wred_queue_op_node_map_entry_t wred_op_node;
    egr_queue_to_pp_port_map_entry_t pp_port_map;
    _bcm_kt2_subport_info_t info;
    bcm_gport_t pp_port;
    lls_l2_child_state1_entry_t l2_child_state;
    uint32 entry_c[SOC_MAX_MEM_WORDS];
    uint32 entry_p[SOC_MAX_MEM_WORDS];
    int port, physical_port, start_spri = 0;
    int update_spri_vector = 0;
    uint8 spri_vector = 0, spri_select = 0;
    int parent_index, numq;
    uint8 vec_3_0, vec_7_4;
    uint32 rval = 0, q_bmp; 
    uint32 spare, min_list, ef_list, not_empty;
    soc_mem_t config_mem, parent_mem;
    soc_field_t start_spri_field = P_START_UC_SPRIf;
    bcm_port_t local_port;
    uint32 *bmap = NULL;
    soc_mem_t mem_c[] = { 
                        LLS_PORT_CONFIGm, 
                        LLS_S1_CONFIGm,
                        INVALIDm,
                        LLS_L0_CONFIGm,
                        LLS_L1_CONFIGm
                      };
    soc_mem_t mem_p[] = { 
                        LLS_L0_PARENTm,
                        LLS_L0_PARENTm,
                        INVALIDm,
                        LLS_L1_PARENTm, 
                        LLS_L2_PARENTm
                      };
    mmu_thdo_opnconfig_cell_entry_t     mmu_thdo_opnconfig_cell_entry={{0}};
    mmu_thdo_opnconfig_qentry_entry_t   mmu_thdo_opnconfig_qentry_entry={{0}};
    
    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_node_get(unit, gport, NULL, &port, NULL,
                               &node));

    if ((node->level == _BCM_KT2_COSQ_NODE_LEVEL_S0 ||
        node->level == _BCM_KT2_COSQ_NODE_LEVEL_S1) && 
        (state == _BCM_KT2_COSQ_STATE_ENABLE || 
        state == _BCM_KT2_COSQ_STATE_DISABLE)) {
        return BCM_E_NONE;
    }

    if (node->level == _BCM_KT2_COSQ_NODE_LEVEL_S0 &&
        (state == _BCM_KT2_COSQ_STATE_WRR_TO_SPRI ||
         state == _BCM_KT2_COSQ_STATE_SPRI_TO_WRR)) {
         return BCM_E_NONE;
    }
    
    parent = node->parent;

    if (parent->level == _BCM_KT2_COSQ_NODE_LEVEL_S1 &&
        node->level == _BCM_KT2_COSQ_NODE_LEVEL_L0) {
        return BCM_E_NONE;
    }

    numq = (parent->wrr_mode == 0) ? ((parent->numq > 8) ? 8 : parent->numq) : 1;
    if ((node->cosq_attached_to < numq) && (parent->base_index < 0) && 
        (parent->mc_base_index < 0)) {
        return BCM_E_PARAM;
    }

    config_mem = mem_c[parent->level];
    if (node->cosq_attached_to < numq) {
        start_spri = (BCM_GPORT_IS_MCAST_QUEUE_GROUP(node->gport)) ?
                      parent->mc_base_index : parent->base_index; 
        spri_vector = 1 << node->cosq_attached_to;
        update_spri_vector =  1;

        sal_memset(entry_c, 0, sizeof(uint32) * SOC_MAX_MEM_WORDS);    
        SOC_IF_ERROR_RETURN
            (soc_mem_read(unit, config_mem, MEM_BLOCK_ALL, parent->hw_index, 
                          &entry_c));
        vec_3_0 = soc_mem_field32_get(unit, config_mem, &entry_c, 
                                      P_NUM_SPRIf);
        vec_7_4 = soc_mem_field32_get(unit, config_mem, &entry_c, 
                                      P_VECT_SPRI_7_4f);
        if (state == _BCM_KT2_COSQ_STATE_ENABLE || 
            state == _BCM_KT2_COSQ_STATE_WRR_TO_SPRI) {
            vec_3_0 |= (spri_vector & 0xF);
            vec_7_4 |= (spri_vector >> 4); 
        } else if (state == _BCM_KT2_COSQ_STATE_DISABLE ||
                   state == _BCM_KT2_COSQ_STATE_SPRI_TO_WRR) {
            vec_3_0 &= ~(spri_vector & 0xF);
            vec_7_4 &= ~(spri_vector >> 4);
        } 

        /* update LLS_CONFIG with the updated vector */ 
        if (config_mem == LLS_L1_CONFIGm) {
            spri_select = soc_mem_field32_get(unit, config_mem, &entry_c,
                                              P_SPRI_SELECTf);
            if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(node->gport)) {
                start_spri_field = P_START_MC_SPRIf;
                spri_select |= spri_vector;    
            }
            soc_mem_field32_set(unit, config_mem, &entry_c, start_spri_field, 
                                start_spri);
            soc_mem_field32_set(unit, config_mem, &entry_c, P_SPRI_SELECTf, 
                                spri_select);
        } else {
            soc_mem_field32_set(unit, config_mem, &entry_c, P_START_SPRIf,
                                start_spri);
        }
        soc_mem_field32_set(unit, config_mem, &entry_c, P_NUM_SPRIf, vec_3_0);
        soc_mem_field32_set(unit, config_mem, &entry_c, P_VECT_SPRI_7_4f, vec_7_4); 
    }

    parent_mem = mem_p[parent->level]; 
    sal_memset(entry_p, 0, sizeof(uint32) * SOC_MAX_MEM_WORDS);
    sal_memset(&wred_op_node, 0, sizeof(mmu_wred_queue_op_node_map_entry_t));

    physical_port = port;
    rv = _bcm_kt2_cosq_subport_get(unit, gport, &pp_port);
    if (rv == BCM_E_NONE) {
        if (state == _BCM_KT2_COSQ_STATE_ENABLE) {
            BCM_IF_ERROR_RETURN
                (bcm_kt2_subport_pp_port_subport_info_get(unit, pp_port,
                                                          &info));
            physical_port = info.port;
            node->local_port = physical_port;
        } else {
            physical_port = node->local_port;
        } 
    }

    if (node->level == _BCM_KT2_COSQ_NODE_LEVEL_S0) {
        bmap = SOC_CONTROL(unit)->port_lls_s0_bmap[physical_port];
    } else if (node->level == _BCM_KT2_COSQ_NODE_LEVEL_S1) {
        bmap = SOC_CONTROL(unit)->port_lls_s1_bmap[physical_port];
    } else if (node->level == _BCM_KT2_COSQ_NODE_LEVEL_L0) {
        bmap = SOC_CONTROL(unit)->port_lls_l0_bmap[physical_port];
    } else if (node->level == _BCM_KT2_COSQ_NODE_LEVEL_L1) {
        bmap = SOC_CONTROL(unit)->port_lls_l1_bmap[physical_port];
    } else if (node->level == _BCM_KT2_COSQ_NODE_LEVEL_L2) {
        bmap = SOC_CONTROL(unit)->port_lls_l2_bmap[physical_port];
    } else {
        return BCM_E_PARAM;
    }

    switch (state) {
        case _BCM_KT2_COSQ_STATE_ENABLE:
            if ((parent_mem == LLS_L0_PARENTm) &&
                (node->hw_index >= num_s1_schedulers)) {
                soc_mem_field32_set(unit, parent_mem, &entry_p, C_TYPEf, 1); 
            }
            if (parent->level != _BCM_KT2_COSQ_NODE_LEVEL_S1) {
                soc_mem_field32_set(unit, parent_mem, &entry_p, C_PARENTf, 
                                    parent->hw_index);
                if (parent->level == _BCM_KT2_COSQ_NODE_LEVEL_S0) {
                    soc_mem_field32_set(unit, parent_mem, &entry_p, C_TYPEf, 0);
                }
                SOC_IF_ERROR_RETURN
                    (soc_mem_write(unit, parent_mem, MEM_BLOCK_ANY, 
                                   node->hw_index, &entry_p));
            }
            if (update_spri_vector && parent->linkphy_enabled == 0) {
                SOC_IF_ERROR_RETURN
                    (soc_mem_write(unit, config_mem, MEM_BLOCK_ANY, 
                                   parent->hw_index, &entry_c));
            }
            
            if (parent->level == _BCM_KT2_COSQ_NODE_LEVEL_L1) {
                /* Read the entry from the RQE map in order to preserve 
                 * the E_TYPE field 
                 */
                SOC_IF_ERROR_RETURN
                    (READ_MMU_RQE_QUEUE_OP_NODE_MAPm(unit, MEM_BLOCK_ALL, 
                                                     node->hw_index,
                                                     &rqe_op_node));
                soc_mem_field32_set(unit, MMU_RQE_QUEUE_OP_NODE_MAPm, 
                                    &rqe_op_node,
                                    OP_NODEf, parent->hw_index);           
                soc_mem_field32_set(unit, MMU_WRED_QUEUE_OP_NODE_MAPm, 
                                    &wred_op_node,
                                    OP_NODEf, parent->hw_index);    
                SOC_IF_ERROR_RETURN
                    (WRITE_MMU_RQE_QUEUE_OP_NODE_MAPm(unit, MEM_BLOCK_ALL, 
                                                      node->hw_index,
                                                      &rqe_op_node));
                SOC_IF_ERROR_RETURN
                    (WRITE_MMU_WRED_QUEUE_OP_NODE_MAPm(unit, MEM_BLOCK_ALL, 
                                                       node->hw_index,
                                                       &wred_op_node)); 

                if (soc_feature(unit, soc_feature_mmu_packing) &&
                    BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) &&
                    IS_EXT_MEM_PORT(unit, port)) {
                    BCM_IF_ERROR_RETURN
                        (_bcm_kt2_cosq_packing_mode_set(unit, node->hw_index, 1));
                }

                BCM_IF_ERROR_RETURN(READ_THDO_QUEUE_DISABLE_CFG2r(unit, &rval));
                soc_reg_field_set(unit, THDO_QUEUE_DISABLE_CFG2r, &rval, 
                                  QUEUE_NUMf,
                                  (node->hw_index & 0xfe0));
                BCM_IF_ERROR_RETURN(WRITE_THDO_QUEUE_DISABLE_CFG2r(unit, rval));

                BCM_IF_ERROR_RETURN(READ_THDO_QUEUE_DISABLE_CFG1r(unit, &rval));
                soc_reg_field_set(unit, THDO_QUEUE_DISABLE_CFG1r, &rval,
                                  QUEUE_RDf, 1);
                BCM_IF_ERROR_RETURN(WRITE_THDO_QUEUE_DISABLE_CFG1r(unit, rval));

                BCM_IF_ERROR_RETURN
                    (READ_THDO_QUEUE_DISABLE_STATUSr(unit, &rval));
                q_bmp = soc_reg_field_get(unit, THDO_QUEUE_DISABLE_STATUSr, 
                                          rval, QUEUE_BITMAPf);

                if (q_bmp & (1 << (node->hw_index % 32))) {
                    BCM_IF_ERROR_RETURN
                        (READ_THDO_QUEUE_DISABLE_CFG2r(unit, &rval));
                    soc_reg_field_set(unit, THDO_QUEUE_DISABLE_CFG2r, &rval, 
                                      QUEUE_NUMf, node->hw_index);
                    BCM_IF_ERROR_RETURN
                        (WRITE_THDO_QUEUE_DISABLE_CFG2r(unit, rval));

                    BCM_IF_ERROR_RETURN
                        (READ_THDO_QUEUE_DISABLE_CFG1r(unit, &rval));
                    soc_reg_field_set(unit, THDO_QUEUE_DISABLE_CFG1r, &rval, 
                                      QUEUE_WRf, 1);
                    BCM_IF_ERROR_RETURN
                        (WRITE_THDO_QUEUE_DISABLE_CFG1r(unit, rval));
                }

                if ((BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(gport)) ||
                       (BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(gport)) ) {
                    SOC_IF_ERROR_RETURN
                        (READ_EGR_QUEUE_TO_PP_PORT_MAPm(unit, MEM_BLOCK_ALL, 
                                 node->hw_index,
                                 &pp_port_map));
                    if(BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(gport)) {
                        BCM_IF_ERROR_RETURN
                            (_bcm_kt2_cosq_node_get(unit, gport, NULL ,&local_port, NULL,
                                                    NULL));
                    } else {
                        BCM_IF_ERROR_RETURN
                            (_bcm_kt2_cosq_localport_resolve(unit, gport, &local_port));
                    }

                    soc_mem_field32_set(unit, EGR_QUEUE_TO_PP_PORT_MAPm, 
                            &pp_port_map, PP_PORTf, local_port);
                    SOC_IF_ERROR_RETURN
                        (WRITE_EGR_QUEUE_TO_PP_PORT_MAPm(unit, MEM_BLOCK_ALL, 
                                                         node->hw_index, 
                                                         &pp_port_map));    
                }              
                
                rv = _bcm_kt2_cosq_subport_get(unit, gport, &pp_port);
                if (rv == BCM_E_NONE) {
                    BCM_IF_ERROR_RETURN
                       (bcm_kt2_subport_pp_port_subport_info_get(unit, pp_port,
                                                        &info));
                    port = info.port;
                    SOC_IF_ERROR_RETURN
                    (READ_EGR_QUEUE_TO_PP_PORT_MAPm(unit, MEM_BLOCK_ALL, 
                                                    node->hw_index,
                                                    &pp_port_map));
                    soc_mem_field32_set(unit, EGR_QUEUE_TO_PP_PORT_MAPm, 
                                        &pp_port_map, PP_PORTf, pp_port);           
                    if (info.port_type == _BCM_KT2_SUBPORT_TYPE_LINKPHY) {
                        soc_mem_field32_set(unit, EGR_QUEUE_TO_PP_PORT_MAPm, 
                                    &pp_port_map, DEVICE_STREAM_IDf,
                                    info.dev_int_stream_id[0]);           
                    } else {
                        soc_mem_field32_set(unit, EGR_QUEUE_TO_PP_PORT_MAPm, 
                                    &pp_port_map, SUBPORT_TAGf,
                                    info.subtag);           
                    }
                    SOC_IF_ERROR_RETURN
                    (WRITE_EGR_QUEUE_TO_PP_PORT_MAPm(unit, MEM_BLOCK_ALL, 
                                                     node->hw_index, 
                                                     &pp_port_map));
                }
                SOC_IF_ERROR_RETURN(READ_MMU_THDO_OPNCONFIG_CELLm(unit,
                                    MEM_BLOCK_ANY, parent->hw_index,     
                                    &mmu_thdo_opnconfig_cell_entry));
                soc_mem_field32_set(unit, MMU_THDO_OPNCONFIG_CELLm,
                                    &mmu_thdo_opnconfig_cell_entry, 
                                    PIDf, port);
                SOC_IF_ERROR_RETURN(WRITE_MMU_THDO_OPNCONFIG_CELLm(unit,
                                    MEM_BLOCK_ALL, parent->hw_index,     
                                    &mmu_thdo_opnconfig_cell_entry));
                SOC_IF_ERROR_RETURN(READ_MMU_THDO_OPNCONFIG_QENTRYm(unit,
                                    MEM_BLOCK_ANY, parent->hw_index ,        
                                    &mmu_thdo_opnconfig_qentry_entry));
                soc_mem_field32_set(unit, MMU_THDO_OPNCONFIG_QENTRYm,
                                    &mmu_thdo_opnconfig_qentry_entry,
                                    PIDf, port);
                SOC_IF_ERROR_RETURN(WRITE_MMU_THDO_OPNCONFIG_QENTRYm(unit,
                                    MEM_BLOCK_ALL, parent->hw_index ,        
                                    &mmu_thdo_opnconfig_qentry_entry));
            }
            SHR_BITSET(bmap, node->hw_index);
            break;
        case _BCM_KT2_COSQ_STATE_DISABLE:
            mmu_info = _bcm_kt2_mmu_info[unit];
            if (parent->level == _BCM_KT2_COSQ_NODE_LEVEL_ROOT) {
                parent_index = (1 << soc_mem_field_length(unit, parent_mem, 
                                                          C_PARENTf)) - 1;
            } else {    
                parent_index = mmu_info->max_nodes[parent->level] - 1;
            }    
            soc_mem_field32_set(unit, parent_mem, &entry_p, C_PARENTf, 
                                parent_index); 
                                        
            if (parent->level == _BCM_KT2_COSQ_NODE_LEVEL_L1) {
                /* flush the queue  */
                BCM_IF_ERROR_RETURN(_bcm_kt2_cosq_flush_queues(
                                    unit, physical_port, node, gport));
                SOC_IF_ERROR_RETURN
                    (READ_LLS_L2_CHILD_STATE1m(unit, MEM_BLOCK_ALL,
                                               node->hw_index,
                                               &l2_child_state));
                if (SOC_MEM_FIELD_VALID(unit, LLS_L2_CHILD_STATE1m, SPAREf)) {
                    soc_mem_field_get(unit, LLS_L2_CHILD_STATE1m,
                                      (uint32 *)&l2_child_state, SPAREf,
                                      &spare);
                } else {
                    soc_mem_field_get(unit, LLS_L2_CHILD_STATE1m,
                                      (uint32 *)&l2_child_state, 
                                      C_ON_WERR_LISTf, &spare);
                }
                soc_mem_field_get(unit, LLS_L2_CHILD_STATE1m,
                                  (uint32 *)&l2_child_state,
                                  C_ON_MIN_LIST_0f, &min_list);
                soc_mem_field_get(unit, LLS_L2_CHILD_STATE1m,
                                  (uint32 *)&l2_child_state,
                                  C_ON_EF_LIST_0f, &ef_list);
                soc_mem_field_get(unit, LLS_L2_CHILD_STATE1m,
                                  (uint32 *)&l2_child_state,
                                  C_NOT_EMPTY_0f, &not_empty);
                /* check LLS_L2_CHILD_STATE[7:4] == 4'b0000 to confirm
                 * that the node is empty */
                if (spare || min_list || ef_list || not_empty) {
                    return BCM_E_BUSY;
                }
                SOC_IF_ERROR_RETURN
                    (READ_LLS_L2_CHILD_STATE1m(unit, MEM_BLOCK_ALL, 
                                               node->hw_index,
                                               &l2_child_state));
                if (SOC_MEM_FIELD_VALID(unit, LLS_L2_CHILD_STATE1m, SPAREf)) {
                    soc_mem_field_get(unit, LLS_L2_CHILD_STATE1m,
                                      (uint32 *)&l2_child_state, SPAREf,
                                      &spare);
                } else {
                    soc_mem_field_get(unit, LLS_L2_CHILD_STATE1m,
                                      (uint32 *)&l2_child_state, 
                                      C_ON_WERR_LISTf, &spare);
                }
                soc_mem_field_get(unit, LLS_L2_CHILD_STATE1m, 
                                  (uint32 *)&l2_child_state, 
                                  C_ON_MIN_LIST_0f, &min_list);
                soc_mem_field_get(unit, LLS_L2_CHILD_STATE1m, 
                                  (uint32 *)&l2_child_state, 
                                  C_ON_EF_LIST_0f, &ef_list);
                soc_mem_field_get(unit, LLS_L2_CHILD_STATE1m, 
                                  (uint32 *)&l2_child_state, 
                                  C_NOT_EMPTY_0f, &not_empty);
                /* check LLS_L2_CHILD_STATE[7:4] == 4'b0000 to confirm 
                 * that the node is empty */
                if (spare || min_list || ef_list || not_empty) {
                    return BCM_E_BUSY;  
                }    

                if (soc_feature(unit, soc_feature_mmu_packing) &&
                    BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) &&
                    IS_EXT_MEM_PORT(unit, port)) {
                    BCM_IF_ERROR_RETURN
                        (_bcm_kt2_cosq_packing_mode_set(unit, node->hw_index, 0));
                }

                SOC_IF_ERROR_RETURN
                    (READ_MMU_RQE_QUEUE_OP_NODE_MAPm(unit, MEM_BLOCK_ALL, 
                                                     node->hw_index,
                                                     &rqe_op_node));
                soc_mem_field32_set(unit, MMU_RQE_QUEUE_OP_NODE_MAPm, 
                                    &rqe_op_node,
                                    OP_NODEf, parent_index);
                soc_mem_field32_set(unit, MMU_WRED_QUEUE_OP_NODE_MAPm, 
                                    &wred_op_node,
                                    OP_NODEf, parent_index);
                SOC_IF_ERROR_RETURN
                    (soc_mem_write(unit, parent_mem, MEM_BLOCK_ANY, 
                                   node->hw_index, &entry_p));
                SOC_IF_ERROR_RETURN
                    (WRITE_MMU_RQE_QUEUE_OP_NODE_MAPm(unit, MEM_BLOCK_ALL, 
                                                      node->hw_index,
                                                      &rqe_op_node));
                SOC_IF_ERROR_RETURN
                    (WRITE_MMU_WRED_QUEUE_OP_NODE_MAPm(unit, MEM_BLOCK_ALL, 
                                                       node->hw_index,
                                                       &wred_op_node)); 
            }else {
                if (parent->level != _BCM_KT2_COSQ_NODE_LEVEL_S1) {
                    SOC_IF_ERROR_RETURN
                        (soc_mem_write(unit, parent_mem, MEM_BLOCK_ANY, 
                                       node->hw_index, 
                                       &entry_p));
                }
            }
            if (update_spri_vector && parent->linkphy_enabled == 0) {
                SOC_IF_ERROR_RETURN
                    (soc_mem_write(unit, config_mem, MEM_BLOCK_ANY, 
                                   parent->hw_index, 
                                   &entry_c));
            }
            SHR_BITCLR(bmap, node->hw_index);
            break;
        case _BCM_KT2_COSQ_STATE_WRR_TO_SPRI:
        case _BCM_KT2_COSQ_STATE_SPRI_TO_WRR:
            if (node->level == _BCM_KT2_COSQ_NODE_LEVEL_S1) {
                SOC_IF_ERROR_RETURN
                    (soc_kt2_cosq_s0_sched_set(unit, parent->hw_index,
                     (state == _BCM_KT2_COSQ_STATE_WRR_TO_SPRI) ? 1 : 0));
                return BCM_E_NONE;
            }

            if (update_spri_vector) {
                BCM_IF_ERROR_RETURN
                    (_bcm_kt2_cosq_dynamic_sp_werr_change(unit, port, 
                                                  node->level, 
                                                  node->hw_index,
                                                  parent->hw_index, entry_c,
                                                  state));
            }
            break;
        default:
            break;
    }            

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_sched_node_set
 * Purpose:
 *     Set LLS scheduler registers based on GPORT
 * Parameters:
 *     unit       - (IN) unit number
 *     gport      - (IN) queue group/scheduler GPORT identifier
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_cosq_sched_node_set(int unit, bcm_gport_t gport, 
                                    _bcm_kt2_cosq_state_t state)
{
    if (soc_feature(unit, soc_feature_vector_based_spri)) {
        return _bcm_kt2_cosq_sched_config_set(unit, gport, state);
    }    
    
    return BCM_E_PARAM;
}

/*
 * Function:
 *     _bcm_kt2_cosq_sched_set
 * Purpose:
 *     Set scheduling mode
 * Parameters:
 *     unit                - (IN) unit number
 *     port                - (IN) port number or GPORT identifier
 *     cosq                - (IN) COS queue number
 *     mode                - (IN) scheduling mode (BCM_COSQ_XXX)
 *     num_weights         - (IN) number of entries in weights array
 *     weights             - (IN) weights array
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_cosq_sched_set(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                       int mode, int num_weights, const int weights[])
{
    _bcm_kt2_mmu_info_t *mmu_info;
    _bcm_kt2_cosq_node_t *node, *cur_node, *l0_node;
    bcm_port_t local_port;
    int index;
    int i;
    int max_weight;
    lls_l0_child_weight_cfg_cnt_entry_t l0_weight_cfg;
    lls_l1_child_weight_cfg_cnt_entry_t l1_weight_cfg; 
    lls_l2_child_weight_cfg_cnt_entry_t l2_weight_cfg;
    lls_l2_parent_entry_t l2_parent;
    _bcm_kt2_cosq_state_t state = _BCM_KT2_COSQ_STATE_NO_CHANGE;

    if (cosq < 0) {
        if (cosq == -1) {
            /* caller needs to resolve the wild card value (-1) */
            return BCM_E_INTERNAL;
        } else { /* reject all other negative value */
            return BCM_E_PARAM;
        }
    }

    /* Validate weight values */
    if (mode == BCM_COSQ_ROUND_ROBIN ||
        mode == BCM_COSQ_WEIGHTED_ROUND_ROBIN ||
        mode == BCM_COSQ_DEFICIT_ROUND_ROBIN ||
        mode == BCM_COSQ_STRICT) {
        max_weight =
            (1 << soc_mem_field_length(unit, LLS_L0_CHILD_WEIGHT_CFG_CNTm, 
                                       C_WEIGHTf)) - 1;
        for (i = 0; i < num_weights; i++) {
            if (weights[i] < 0 || weights[i] > max_weight) {
                return BCM_E_PARAM;
            }
        }
    } else {
        return BCM_E_PARAM;
    }

    /* To support legacy device compatibility to configure STRICT
       mode if weight is "0".
     */
    if ((num_weights == 1) && (weights[0] == BCM_COSQ_WEIGHT_STRICT)) {
        mode =  BCM_COSQ_STRICT;
    }

    mmu_info = _bcm_kt2_mmu_info[unit];
    
    if (BCM_GPORT_IS_SCHEDULER(port)) { /* ETS style scheduler */
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_node_get(unit, port, NULL, &local_port, NULL,
                                   &node));        
        if ((node->wrr_mode == 1) && (mode == BCM_COSQ_STRICT)) {
            return BCM_E_PARAM;
        }
    }else {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_localport_resolve(unit, port, &local_port));

        node = &mmu_info->sched_node[local_port];
        
        if (node->gport < 0) {
            return BCM_E_PARAM;
        }

        if ((l0_node = node->child) == NULL) {
            return BCM_E_PARAM;
        }
        if ((node = l0_node->child) == NULL) {
            return BCM_E_PARAM;
        }
        if(IS_CPU_PORT(unit, local_port) ) {
            /*
             * In case when number of cpu queues are not multiples of 8
             * there is chance last siblings gets numq < 8,
             * which may result in failure , So node is moved to the 
             * first sibling, which guarantee to have numq = 8
             */

            for (cur_node = node; cur_node->sibling != NULL;
                    cur_node = cur_node->sibling) ;
            node = cur_node;
        }
        if (soc_feature(unit, soc_feature_mmu_packing)) {
            for (cur_node = node; cur_node != NULL;
                    cur_node = cur_node->sibling) {
                if(cur_node->cosq_attached_to == cosq) {
                    node = cur_node;
                    break;
                }
            }
            if (cur_node == NULL) {
                /* this cosq is not yet attached */
                return BCM_E_INTERNAL;
            }
        }
    }

    if (!soc_feature(unit, soc_feature_mmu_packing)) {
        if ((mode != BCM_COSQ_STRICT) && 
                (node->numq > 0 && (cosq + num_weights) > node->numq)) {
            return BCM_E_PARAM;
        }


        if (node->cosq_attached_to < 0) { /* Not resolved yet */
            return BCM_E_NOT_FOUND;
        }

        for (cur_node = node->child; cur_node != NULL;
                cur_node = cur_node->sibling) {
            if (cur_node->cosq_attached_to == cosq) {
                break;
            }
        }
    } else {
        for (cur_node = node->child; cur_node != NULL;
                cur_node = cur_node->sibling) {
            if (cur_node->cosq_attached_to == 0) {
                break;
            }
        }
    }
    if (cur_node == NULL) {
        /* this cosq is not yet attached */
        return BCM_E_INTERNAL;
    }

    if (cur_node->wrr_in_use == 1 && mode == BCM_COSQ_STRICT) {
        if (cur_node->cosq_attached_to >= 8) {
            return BCM_E_PARAM;
        }  
        state = _BCM_KT2_COSQ_STATE_WRR_TO_SPRI;        
    }    

    if (cur_node->wrr_in_use == 0 && mode != BCM_COSQ_STRICT) {
        state = _BCM_KT2_COSQ_STATE_SPRI_TO_WRR;
    }    

    if (state == _BCM_KT2_COSQ_STATE_SPRI_TO_WRR && 
        cur_node->level == _BCM_KT2_COSQ_NODE_LEVEL_L2) {
        SOC_IF_ERROR_RETURN
            (READ_LLS_L2_PARENTm(unit, MEM_BLOCK_ALL, cur_node->hw_index, 
                                 &l2_parent));
        if (soc_mem_field32_get(unit, LLS_L2_PARENTm, 
                                &l2_parent, C_EFf) == 1) {
            return BCM_E_PARAM;
        }
    }

    cur_node->wrr_in_use = (mode != BCM_COSQ_STRICT) ? 1 : 0;
    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_sched_node_set(unit, cur_node->gport, state));

    switch (node->level) {
    case _BCM_KT2_COSQ_NODE_LEVEL_ROOT: /* port scheduler */
    case _BCM_KT2_COSQ_NODE_LEVEL_S0: /* S0 scheduler */
    case _BCM_KT2_COSQ_NODE_LEVEL_S1: /* S1 scheduler */
        for (i = 0; i < num_weights; i++) {
            BCM_IF_ERROR_RETURN
                   (_bcm_kt2_cosq_index_resolve(unit, port, cosq + i,
                                       _BCM_KT2_COSQ_INDEX_STYLE_SCHEDULER,
                                       NULL, &index, NULL));
            SOC_IF_ERROR_RETURN
                   (READ_LLS_L0_CHILD_WEIGHT_CFG_CNTm(unit, MEM_BLOCK_ALL, 
                                                      index, &l0_weight_cfg));
            soc_mem_field32_set(unit, LLS_L0_CHILD_WEIGHT_CFG_CNTm, 
                                (uint32 *)&l0_weight_cfg, 
                                C_WEIGHTf, weights[i]);
            soc_mem_field32_set(unit, LLS_L0_CHILD_WEIGHT_CFG_CNTm, 
                                (uint32 *)&l0_weight_cfg, 
                                C_WEIGHT_SURPLUS_COUNTf, 0);
            soc_mem_field32_set(unit, LLS_L0_CHILD_WEIGHT_CFG_CNTm, 
                                (uint32 *)&l0_weight_cfg, 
                                C_WEIGHT_COUNTf, 0);
            SOC_IF_ERROR_RETURN
                   (WRITE_LLS_L0_CHILD_WEIGHT_CFG_CNTm(unit, MEM_BLOCK_ALL, 
                                                       index, &l0_weight_cfg));

        }
        break;

    case _BCM_KT2_COSQ_NODE_LEVEL_L0: /* L0 scheduler */
        for (i = 0; i < num_weights; i++) {
            BCM_IF_ERROR_RETURN
                   (_bcm_kt2_cosq_index_resolve(unit, port, cosq + i,
                                       _BCM_KT2_COSQ_INDEX_STYLE_SCHEDULER,
                                        NULL, &index, NULL));
            SOC_IF_ERROR_RETURN
                   (READ_LLS_L1_CHILD_WEIGHT_CFG_CNTm(unit, MEM_BLOCK_ALL, 
                                                      index, &l1_weight_cfg));
            soc_mem_field32_set(unit, LLS_L1_CHILD_WEIGHT_CFG_CNTm, 
                                (uint32 *)&l1_weight_cfg, 
                                C_WEIGHTf, weights[i]);
            soc_mem_field32_set(unit, LLS_L1_CHILD_WEIGHT_CFG_CNTm, 
                                (uint32 *)&l1_weight_cfg, 
                                C_WEIGHT_SURPLUS_COUNTf, 0);
            soc_mem_field32_set(unit, LLS_L1_CHILD_WEIGHT_CFG_CNTm, 
                                (uint32 *)&l1_weight_cfg, 
                                C_WEIGHT_COUNTf, 0);
            SOC_IF_ERROR_RETURN
                   (WRITE_LLS_L1_CHILD_WEIGHT_CFG_CNTm(unit, MEM_BLOCK_ALL, 
                                                       index, &l1_weight_cfg));

        }
        break;

    case _BCM_KT2_COSQ_NODE_LEVEL_L1: /* L1 scheduler */
        for (i = 0; i < num_weights; i++) {
            BCM_IF_ERROR_RETURN
                   (_bcm_kt2_cosq_index_resolve(unit, port, cosq + i,
                                       _BCM_KT2_COSQ_INDEX_STYLE_SCHEDULER,
                                        NULL, &index, NULL));
            SOC_IF_ERROR_RETURN
                   (READ_LLS_L2_CHILD_WEIGHT_CFG_CNTm(unit, MEM_BLOCK_ALL, 
                                                      index, &l2_weight_cfg));
            soc_mem_field32_set(unit, LLS_L2_CHILD_WEIGHT_CFG_CNTm, 
                                (uint32 *)&l2_weight_cfg, 
                                C_WEIGHTf, weights[i]);
            soc_mem_field32_set(unit, LLS_L2_CHILD_WEIGHT_CFG_CNTm, 
                                (uint32 *)&l2_weight_cfg, 
                                C_WEIGHT_SURPLUS_COUNTf, 0);
            soc_mem_field32_set(unit, LLS_L2_CHILD_WEIGHT_CFG_CNTm, 
                                (uint32 *)&l2_weight_cfg, 
                                C_WEIGHT_COUNTf, 0);
            SOC_IF_ERROR_RETURN
                   (WRITE_LLS_L2_CHILD_WEIGHT_CFG_CNTm(unit, MEM_BLOCK_ALL, 
                                                       index, &l2_weight_cfg));

        }
        break;

    default:
        return BCM_E_INTERNAL;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_sched_get
 * Purpose:
 *     Get scheduling mode setting
 * Parameters:
 *     unit                - (IN) unit number
 *     port                - (IN) port number or GPORT identifier
 *     cosq                - (IN) COS queue number
 *     mode                - (OUT) scheduling mode (BCM_COSQ_XXX)
 *     num_weights         - (IN) number of entries in weights array
 *     weights             - (OUT) weights array
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_cosq_sched_get(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                       int *mode, int num_weights, int weights[])
{
    _bcm_kt2_mmu_info_t *mmu_info; 
    _bcm_kt2_cosq_node_t *node, *l0_node = NULL;
    _bcm_kt2_cosq_node_t *cur_node;
    bcm_port_t local_port;
    int index;
    int i;
    lls_l0_child_weight_cfg_cnt_entry_t l0_weight_cfg;
    lls_l1_child_weight_cfg_cnt_entry_t l1_weight_cfg;
    lls_l2_child_weight_cfg_cnt_entry_t l2_weight_cfg;

    if (cosq < 0) {
        if (cosq == -1) {
            /* caller needs to resolve the wild card value (-1) */
            return BCM_E_INTERNAL;
        } else { /* reject all other negative value */
            return BCM_E_PARAM;
        }
    }

    if (BCM_GPORT_IS_SCHEDULER(port)) { /* ETS style scheduler */
           BCM_IF_ERROR_RETURN
               (_bcm_kt2_cosq_node_get(unit, port, NULL, &local_port, NULL,
                                      &node));
    } else {
        if (_BCM_KT2_GPORT_IS_LINKPHY_OR_SUBTAG_SUBPORT_GPORT(unit, port)) {
            local_port = _BCM_KT2_SUBPORT_PORT_ID_GET(port);
        } else {
             BCM_IF_ERROR_RETURN
                 (_bcm_kt2_cosq_localport_resolve(unit, port, &local_port));
        }  
        mmu_info = _bcm_kt2_mmu_info[unit];

        node = &mmu_info->sched_node[local_port];
        
        if (node->gport < 0) {
            return BCM_E_PARAM;
        }

        if ((l0_node = node->child) == NULL) {
            return BCM_E_PARAM;
        }
        if ((node = l0_node->child) == NULL) {
            return BCM_E_PARAM;
        }

        if(IS_CPU_PORT(unit, local_port) ) {
            /*
             * In case when number of cpu queues are not multiples of 8
             * there is chance last siblings gets numq < 8,
             * which may result in failure , So node is moved to the 
             * first sibling, which guarantee to have numq = 8
             */

            for (cur_node = node; cur_node->sibling != NULL;
                    cur_node = cur_node->sibling) ;
            node = cur_node;
        }
    }

    if (node->numq > 0 && (cosq + num_weights) > node->numq) {
        return BCM_E_PARAM;
    }

    if (node->cosq_attached_to < 0) { /* Not resolved yet */
        return BCM_E_NOT_FOUND;
    }

    for (cur_node = node->child; cur_node != NULL;
        cur_node = cur_node->sibling) {
        if (cur_node->cosq_attached_to == cosq) {
            break;
        }
    }

    if (cur_node == NULL) {
        /* this cosq is not yet attached */
        return BCM_E_INTERNAL;
    }
    
    switch (node->level) {
    case _BCM_KT2_COSQ_NODE_LEVEL_ROOT: /* port scheduler */
        for (i = 0; i < num_weights; i++) {
            BCM_IF_ERROR_RETURN
                (_bcm_kt2_cosq_index_resolve(unit, port, cosq + i,
                                        _BCM_KT2_COSQ_INDEX_STYLE_SCHEDULER,
                                        NULL, &index, NULL));
            SOC_IF_ERROR_RETURN
                (READ_LLS_L0_CHILD_WEIGHT_CFG_CNTm(unit, MEM_BLOCK_ALL, 
                                                   index, &l0_weight_cfg));
            weights[i] = soc_mem_field32_get(unit, LLS_L0_CHILD_WEIGHT_CFG_CNTm,
                                             (uint32 *)&l0_weight_cfg, 
                                             C_WEIGHTf);
        }
        break;

    case _BCM_KT2_COSQ_NODE_LEVEL_L0: /* L0 scheduler */
        for (i = 0; i < num_weights; i++) {
            BCM_IF_ERROR_RETURN
                (_bcm_kt2_cosq_index_resolve(unit, port, cosq + i,
                                        _BCM_KT2_COSQ_INDEX_STYLE_SCHEDULER,
                                        NULL, &index, NULL));
            SOC_IF_ERROR_RETURN
                (READ_LLS_L1_CHILD_WEIGHT_CFG_CNTm(unit, MEM_BLOCK_ALL,
                                                   index, &l1_weight_cfg));
            weights[i] = soc_mem_field32_get(unit, LLS_L1_CHILD_WEIGHT_CFG_CNTm,
                                             (uint32 *)&l1_weight_cfg, 
                                             C_WEIGHTf);

        }
        break;

    case _BCM_KT2_COSQ_NODE_LEVEL_L1: /* L1 scheduler */
        for (i = 0; i < num_weights; i++) {
            BCM_IF_ERROR_RETURN
                (_bcm_kt2_cosq_index_resolve(unit, port, cosq + i,
                                        _BCM_KT2_COSQ_INDEX_STYLE_SCHEDULER,
                                        NULL, &index, NULL));
            SOC_IF_ERROR_RETURN
                (READ_LLS_L2_CHILD_WEIGHT_CFG_CNTm(unit, MEM_BLOCK_ALL, 
                                                   index, &l2_weight_cfg));
            weights[i] = soc_mem_field32_get(unit, LLS_L2_CHILD_WEIGHT_CFG_CNTm,
                                             (uint32 *)&l2_weight_cfg, 
                                             C_WEIGHTf);

        }
        break;

    case _BCM_KT2_COSQ_NODE_LEVEL_S0: /* S0 scheduler */
    case _BCM_KT2_COSQ_NODE_LEVEL_S1: /* S1 scheduler */
        break;

    default:
        return BCM_E_INTERNAL;
    }

    if (cur_node->wrr_in_use == 0) {
        *mode = BCM_COSQ_STRICT;   
    } else {    
        *mode = BCM_COSQ_WEIGHTED_ROUND_ROBIN;
    }    
    
    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_port_sched_set
 * Purpose:
 *     Set scheduling mode setting for a port
 * Parameters:
 *     unit                - (IN) unit number
 *     port                - (IN) port number or GPORT identifier
 *     cosq                - (IN) COS queue number
 *     mode                - (IN) scheduling mode (BCM_COSQ_XXX)
 *     num_weights         - (IN) number of entries in weights array
 *     weights             - (IN) weights array
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_cosq_port_sched_set(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                             int mode, int num_weights, int *weights)
{
    int i;

    for (i = 0; i < num_weights; i++) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_sched_set(unit, port, (cosq + i), 
                                     mode, 1, &weights[i])); 
                                     
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_port_sched_get
 * Purpose:
 *     Set scheduling mode setting for a port
 * Parameters:
 *     unit                - (IN) unit number
 *     port                - (IN) port number or GPORT identifier
 *     cosq                - (IN) COS queue number
 *     mode                - (OUT) scheduling mode (BCM_COSQ_XXX)
 *     num_weights         - (IN) number of entries in weights array
 *     weights             - (OUT) weights array
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_cosq_port_sched_get(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                             int *mode, int num_weights, int *weights)
{
    int i;

    for (i = 0; i < num_weights; i++) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_sched_get(unit, port, (cosq + i), 
                                     mode, 1, &weights[i])); 
                                     
    }
    
    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_mapping_set_regular
 * Purpose:
 *     Determine which COS queue a given priority currently maps to.
 * Parameters:
 *     unit     - (IN) unit number
 *     gport    - (IN) queue group GPORT identifier
 *     priority - (IN) priority value to map
 *     cosq     - (IN) COS queue to map to
 * Returns:
 *     BCM_E_XXX
 */
STATIC int
_bcm_kt2_cosq_mapping_set_regular(int unit, bcm_port_t gport, 
                                  bcm_cos_t priority,
                                  bcm_cos_queue_t cosq)
{
    _bcm_kt2_mmu_info_t *mmu_info;
    bcm_port_t port;
    int numq;
    bcm_cos_queue_t hw_cosq;
    soc_field_t field = COSf;
    cos_map_sel_entry_t cos_map_sel_entry;
    port_cos_map_entry_t cos_map_entries[16];
    void *entries[1];     
    uint32 old_index, new_index;
    int rv;
    int cpu_hg_index = 0;
    
    if (priority < 0 || priority >= 16) {
        return BCM_E_PARAM;
    }

    hw_cosq = cosq;
    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_index_resolve
            (unit, gport, cosq, _BCM_KT2_COSQ_INDEX_STYLE_UCAST_QUEUE,
             &port, &hw_cosq, NULL));
    } else if(_BCM_KT2_GPORT_IS_LINKPHY_OR_SUBTAG_SUBPORT_GPORT (unit, gport)) {
        port = _BCM_KT2_SUBPORT_PORT_ID_GET(gport);
    } else { 
        if (BCM_GPORT_IS_SET(gport)) {
            BCM_IF_ERROR_RETURN(
                bcm_esw_port_local_get(unit, gport, &port));
        } else {
            port = gport;
        }

        if (!SOC_PORT_VALID(unit, port) || !IS_ALL_PORT(unit, port)) {
            return BCM_E_PORT;
        }

    }

    mmu_info = _bcm_kt2_mmu_info[unit];
    numq = mmu_info->port[port].q_limit - 
           mmu_info->port[port].q_offset;

    if (cosq >= numq) {
        return BCM_E_PARAM;
    }
    
    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        field = (numq <= 8) ? COSf : HG_COSf;
    }

    entries[0] = &cos_map_entries;

    BCM_IF_ERROR_RETURN
        (READ_COS_MAP_SELm(unit, MEM_BLOCK_ANY, port, &cos_map_sel_entry));
    old_index = soc_mem_field32_get(unit, COS_MAP_SELm, &cos_map_sel_entry,
                                    SELECTf);
    old_index *= 16;

    BCM_IF_ERROR_RETURN
        (soc_profile_mem_get(unit, _bcm_kt2_cos_map_profile[unit],
                             old_index, 16, entries));
    soc_mem_field32_set(unit, PORT_COS_MAPm, &cos_map_entries[priority], field,
                        hw_cosq);

    soc_mem_lock(unit, PORT_COS_MAPm);

    rv = soc_profile_mem_delete(unit, _bcm_kt2_cos_map_profile[unit],
                                old_index);
#ifndef BCM_COSQ_HIGIG_MAP_DISABLE
    if (BCM_SUCCESS(rv) && IS_CPU_PORT(unit, port)) {
        rv = soc_profile_mem_delete(unit, _bcm_kt2_cos_map_profile[unit],
                                    old_index);
    }
#endif /* BCM_COSQ_HIGIG_MAP_DISABLE */
    if (BCM_FAILURE(rv)) {
        soc_mem_unlock(unit, PORT_COS_MAPm);
        return rv;
    }

    rv = soc_profile_mem_add(unit, _bcm_kt2_cos_map_profile[unit], entries,
                             16, &new_index);
#ifndef BCM_COSQ_HIGIG_MAP_DISABLE
    if (BCM_SUCCESS(rv) && IS_CPU_PORT(unit, port)) {
        rv = soc_profile_mem_reference(unit, _bcm_kt2_cos_map_profile[unit],
                                       new_index, 16);
    }
#endif /* BCM_COSQ_HIGIG_MAP_DISABLE */

    if (rv == SOC_E_RESOURCE) {
        (void)soc_profile_mem_reference(unit, _bcm_kt2_cos_map_profile[unit],
                                        old_index, 16);
#ifndef BCM_COSQ_HIGIG_MAP_DISABLE
        if (IS_CPU_PORT(unit, port)) {
            (void)soc_profile_mem_reference(unit,
                                            _bcm_kt2_cos_map_profile[unit],
                                            old_index, 16);
        }
#endif /* BCM_COSQ_HIGIG_MAP_DISABLE */
    }

    soc_mem_unlock(unit, PORT_COS_MAPm);

    if (BCM_FAILURE(rv)) {
        return rv;
    }

    soc_mem_field32_set(unit, COS_MAP_SELm, &cos_map_sel_entry, SELECTf,
                        new_index / 16);
    BCM_IF_ERROR_RETURN
        (WRITE_COS_MAP_SELm(unit, MEM_BLOCK_ANY, port, &cos_map_sel_entry));

#ifndef BCM_COSQ_HIGIG_MAP_DISABLE
    if (IS_CPU_PORT(unit, port)) {
        cpu_hg_index = SOC_INFO(unit).cpu_hg_pp_port_index;
        BCM_IF_ERROR_RETURN
            (soc_mem_field32_modify(unit, COS_MAP_SELm,
                                    cpu_hg_index, SELECTf,
                                    new_index / 16));
    }
#endif /* BCM_COSQ_HIGIG_MAP_DISABLE */

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_bucket_set
 * Purpose:
 *     Configure COS queue bandwidth control bucket
 * Parameters:
 *     unit          - (IN) unit number
 *     port          - (IN) port number or GPORT identifier
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
_bcm_kt2_cosq_bucket_set(int unit, bcm_gport_t port, bcm_cos_queue_t cosq,
                        uint32 min_quantum, uint32 max_quantum,
                        uint32 burst_min_quantum, uint32 burst_max_quantum,
                        uint32 flags)
{
    int rv;
    _bcm_kt2_cosq_node_t *node;
    bcm_port_t local_port;
    int index;
    soc_mem_t config_mem[2];
    lls_port_shaper_config_c_entry_t port_entry;
    lls_l0_shaper_config_c_entry_t  l0_entry;
    lls_l1_shaper_config_c_entry_t  l1_entry;
    lls_l2_shaper_config_lower_entry_t l2_entry;
    lls_s0_shaper_config_c_entry_t s0_entry;
    uint32 rate_exp[2], rate_mantissa[2];
    uint32 burst_exp[2], burst_mantissa[2];
    uint32 cycle_sel[2];
    uint32 burst_min, burst_max;
    int i, idx;
    int level = _BCM_KT2_COSQ_NODE_LEVEL_L2;
    soc_field_t rate_exp_f[] = {
        C_MAX_REF_RATE_EXPf, C_MIN_REF_RATE_EXPf
    };
    soc_field_t rate_mant_f[] = {
        C_MAX_REF_RATE_MANTf, C_MIN_REF_RATE_MANTf
    };
    soc_field_t burst_exp_f[] = {
        C_MAX_THLD_EXPf, C_MIN_THLD_EXPf
    };
    soc_field_t burst_mant_f[] = {
        C_MAX_THLD_MANTf, C_MIN_THLD_MANTf
    };
    soc_field_t cycle_sel_f[] = {
        C_MAX_CYCLE_SELf, C_MIN_CYCLE_SELf
    };
    static const soc_field_t rate_exp_fields[] = {
       C_MAX_REF_RATE_EXP_0f, C_MAX_REF_RATE_EXP_1f,
       C_MAX_REF_RATE_EXP_2f, C_MAX_REF_RATE_EXP_3f,
       C_MIN_REF_RATE_EXP_0f, C_MIN_REF_RATE_EXP_1f,
       C_MIN_REF_RATE_EXP_2f, C_MIN_REF_RATE_EXP_3f
    };
    static const soc_field_t rate_mant_fields[] = {
       C_MAX_REF_RATE_MANT_0f, C_MAX_REF_RATE_MANT_1f,
       C_MAX_REF_RATE_MANT_2f, C_MAX_REF_RATE_MANT_3f,
       C_MIN_REF_RATE_MANT_0f, C_MIN_REF_RATE_MANT_1f,
       C_MIN_REF_RATE_MANT_2f, C_MIN_REF_RATE_MANT_3f
    };
    static const soc_field_t burst_exp_fields[] = {
       C_MAX_THLD_EXP_0f, C_MAX_THLD_EXP_1f,
       C_MAX_THLD_EXP_2f, C_MAX_THLD_EXP_3f,
       C_MIN_THLD_EXP_0f, C_MIN_THLD_EXP_1f,
       C_MIN_THLD_EXP_2f, C_MIN_THLD_EXP_3f
    };
    static const soc_field_t burst_mant_fields[] = {
       C_MAX_THLD_MANT_0f, C_MAX_THLD_MANT_1f,
       C_MAX_THLD_MANT_2f, C_MAX_THLD_MANT_3f,
       C_MIN_THLD_MANT_0f, C_MIN_THLD_MANT_1f,
       C_MIN_THLD_MANT_2f, C_MIN_THLD_MANT_3f
    };
    soc_field_t cycle_sel_fields[] = {
        C_MAX_CYCLE_SEL_0f, C_MAX_CYCLE_SEL_1f,
        C_MAX_CYCLE_SEL_2f, C_MAX_CYCLE_SEL_3f,    
        C_MIN_CYCLE_SEL_0f, C_MIN_CYCLE_SEL_1f,
        C_MIN_CYCLE_SEL_2f, C_MIN_CYCLE_SEL_3f
    };

    if (cosq < 0) {
        if (cosq == -1) {
            /* caller needs to resolve the wild card value (-1) */
            return BCM_E_INTERNAL;
        } else { /* reject all other negative value */
            return BCM_E_PARAM;
        }
    }


    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_index_resolve(unit, port, cosq,
                                    _BCM_KT2_COSQ_INDEX_STYLE_BUCKET,
                                    &local_port, &index, NULL));
    
    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
        BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port) ||
        BCM_GPORT_IS_SCHEDULER(port)) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_node_get(unit, port, NULL, NULL, NULL, &node));
        level = node->level;    

        if ((level == _BCM_KT2_COSQ_NODE_LEVEL_S0 || 
            level == _BCM_KT2_COSQ_NODE_LEVEL_S1) && 
            (min_quantum > 0)) {
            /* min shaper is not supported */
            return BCM_E_PARAM;
        }
    }     

    /* compute exp and mantissa and program the registers */
    rv = soc_katana_get_shaper_rate_info(unit, max_quantum,
                                         &rate_mantissa[0], &rate_exp[0]);
    if (BCM_FAILURE(rv)) {
        return(rv);
    }

    rv = soc_katana_get_shaper_rate_info(unit, min_quantum,
                                         &rate_mantissa[1], &rate_exp[1]);
    if (BCM_FAILURE(rv)) {
        return(rv);
    }

    burst_max = burst_max_quantum;
    burst_min = burst_min_quantum;
    rv = soc_katana_get_shaper_burst_info(unit, burst_max, &burst_mantissa[0], 
                                          &burst_exp[0], flags);
    if (BCM_FAILURE(rv)) {
        return(rv);
    }
    
    rv = soc_katana_get_shaper_burst_info(unit, burst_min, &burst_mantissa[1], 
                                          &burst_exp[1], flags);
    if (BCM_FAILURE(rv)) {
        return(rv);
    }    

    if (!(flags & _BCM_XGS_METER_FLAG_NON_BURST_CORRECTION)) {
        /* set recommended burst settings */
        for (i = 0; i < 2; i++) {
            if (rate_exp[i] < 10) {
                burst_exp[i] = 5;
                burst_mantissa[i] = 0;
            } else {
                burst_exp[i] = rate_exp[i] - 4;
                burst_mantissa[i] = (rate_mantissa[i] >> 3);
            }
        }
        /* set recommended refresh rate/cycle_sel settings */
        for (i = 0; i < 2; i++) {
            if (rate_exp[i] > 4) {
                cycle_sel[i] = 0;
            } else if (rate_exp[i] > 1) {
                cycle_sel[i] = 5 - rate_exp[i];
            } else {
                cycle_sel[i] = 4;
            }
        }
    } else {
        for (i = 0; i < 2; i++) {
            if ((burst_exp[i] >= 14) && (burst_mantissa[i] >= 124)) {
                /* maximum supported is 2064384 byte */
                burst_exp[i] = 14;
                burst_mantissa[i] = 124;
                cycle_sel[i] = 9;
            } else {
                rv =  soc_katana_compute_refresh_rate(unit, rate_exp[i],
                                rate_mantissa[i], burst_exp[i], 
                                burst_mantissa[i], &cycle_sel[i]);
                if (BCM_FAILURE(rv)) {
                    return(rv);
                }
            }
        }
    }
    
    switch (level) {
        case _BCM_KT2_COSQ_NODE_LEVEL_ROOT:
            config_mem[0] = LLS_PORT_SHAPER_CONFIG_Cm;            
            SOC_IF_ERROR_RETURN
               (soc_mem_read(unit, config_mem[0], MEM_BLOCK_ALL,
                             index, &port_entry));
            soc_mem_field32_set(unit, config_mem[0], &port_entry,
                                rate_exp_f[0], rate_exp[0]);
            soc_mem_field32_set(unit, config_mem[0], &port_entry,
                                rate_mant_f[0], rate_mantissa[0]);
            soc_mem_field32_set(unit, config_mem[0], &port_entry,
                                burst_exp_f[0], burst_exp[0]);
            soc_mem_field32_set(unit, config_mem[0], &port_entry,
                                burst_mant_f[0], burst_mantissa[0]);
            soc_mem_field32_set(unit, config_mem[0], &port_entry,
                                cycle_sel_f[0], cycle_sel[0]); 
            SOC_IF_ERROR_RETURN
                (soc_mem_write(unit, config_mem[0],
                               MEM_BLOCK_ALL, index, &port_entry));
            break;

        case _BCM_KT2_COSQ_NODE_LEVEL_L0:
            config_mem[0] = LLS_L0_SHAPER_CONFIG_Cm;
            config_mem[1] = LLS_L0_MIN_CONFIG_Cm;            
            for (i = 0; i < 2; i++) {
                SOC_IF_ERROR_RETURN
                       (soc_mem_read(unit, config_mem[i], MEM_BLOCK_ALL,
                                   index, &l0_entry));
                soc_mem_field32_set(unit, config_mem[i], &l0_entry,
                                    rate_exp_f[i], rate_exp[i]);
                soc_mem_field32_set(unit, config_mem[i], &l0_entry,
                                    rate_mant_f[i], rate_mantissa[i]);
                soc_mem_field32_set(unit, config_mem[i], &l0_entry,
                                    burst_exp_f[i], burst_exp[i]);
                soc_mem_field32_set(unit, config_mem[i], &l0_entry,
                                    burst_mant_f[i], burst_mantissa[i]);
                soc_mem_field32_set(unit, config_mem[i], &l0_entry,
                                    cycle_sel_f[i], cycle_sel[i]);   
                SOC_IF_ERROR_RETURN
                    (soc_mem_write(unit, config_mem[i],
                                     MEM_BLOCK_ALL, index, &l0_entry));
             }
            break;

        case _BCM_KT2_COSQ_NODE_LEVEL_L1:
            config_mem[0] = LLS_L1_SHAPER_CONFIG_Cm;
            config_mem[1] = LLS_L1_MIN_CONFIG_Cm;            
            for (i = 0; i < 2; i++) {
                idx = (i * 4) + (index % 4);
                SOC_IF_ERROR_RETURN
                       (soc_mem_read(unit, config_mem[i], MEM_BLOCK_ALL,
                                   index/4, &l1_entry));
                soc_mem_field32_set(unit, config_mem[i], &l1_entry,
                                    rate_exp_fields[idx], rate_exp[i]);
                soc_mem_field32_set(unit, config_mem[i], &l1_entry,
                                    rate_mant_fields[idx], rate_mantissa[i]);
                soc_mem_field32_set(unit, config_mem[i], &l1_entry,
                                    burst_exp_fields[idx], burst_exp[i]);
                soc_mem_field32_set(unit, config_mem[i], &l1_entry,
                                    burst_mant_fields[idx], burst_mantissa[i]);
                soc_mem_field32_set(unit, config_mem[i], &l1_entry,
                                    cycle_sel_fields[idx], cycle_sel[i]); 
                SOC_IF_ERROR_RETURN
                    (soc_mem_write(unit, config_mem[i],
                                   MEM_BLOCK_ALL, index/4, &l1_entry));
            }
            break;

        case _BCM_KT2_COSQ_NODE_LEVEL_L2:
            if ((index % 8) < 4) {
                config_mem[0] = LLS_L2_SHAPER_CONFIG_LOWERm;
                config_mem[1] = LLS_L2_MIN_CONFIG_LOWER_Cm;
            } else {
                config_mem[0] = LLS_L2_SHAPER_CONFIG_UPPERm;
                config_mem[1] = LLS_L2_MIN_CONFIG_UPPER_Cm;
            }
            for (i = 0; i < 2; i++) {
                idx = (i * 4) + (index % 4);
                SOC_IF_ERROR_RETURN
                       (soc_mem_read(unit, config_mem[i], MEM_BLOCK_ALL,
                                 index/8, &l2_entry));
                soc_mem_field32_set(unit, config_mem[i], &l2_entry,
                                    rate_exp_fields[idx], rate_exp[i]);
                soc_mem_field32_set(unit, config_mem[i], &l2_entry,
                                    rate_mant_fields[idx], rate_mantissa[i]);
                soc_mem_field32_set(unit, config_mem[i], &l2_entry,
                                    burst_exp_fields[idx], burst_exp[i]);
                soc_mem_field32_set(unit, config_mem[i], &l2_entry,
                                    burst_mant_fields[idx], burst_mantissa[i]);
                soc_mem_field32_set(unit, config_mem[i], &l2_entry,
                                    cycle_sel_fields[idx], cycle_sel[i]);       
                SOC_IF_ERROR_RETURN
                    (soc_mem_write(unit, config_mem[i],
                                   MEM_BLOCK_ALL, index/8, &l2_entry));
            }
            break;

        case _BCM_KT2_COSQ_NODE_LEVEL_S0:
        case _BCM_KT2_COSQ_NODE_LEVEL_S1:
            config_mem[0] = (level == _BCM_KT2_COSQ_NODE_LEVEL_S0 &&
                             (node->linkphy_enabled)) ?
                            LLS_S0_SHAPER_CONFIG_Cm :
                            LLS_S1_SHAPER_CONFIG_Cm;
            SOC_IF_ERROR_RETURN
               (soc_mem_read(unit, config_mem[0], MEM_BLOCK_ALL,
                             index, &s0_entry));
            soc_mem_field32_set(unit, config_mem[0], &s0_entry,
                                rate_exp_f[0], rate_exp[0]);
            soc_mem_field32_set(unit, config_mem[0], &s0_entry,
                                rate_mant_f[0], rate_mantissa[0]);
            soc_mem_field32_set(unit, config_mem[0], &s0_entry,
                                burst_exp_f[0], burst_exp[0]);
            soc_mem_field32_set(unit, config_mem[0], &s0_entry,
                                burst_mant_f[0], burst_mantissa[0]);
            soc_mem_field32_set(unit, config_mem[0], &s0_entry,
                                cycle_sel_f[0], cycle_sel[0]);
            SOC_IF_ERROR_RETURN
                (soc_mem_write(unit, config_mem[0],
                               MEM_BLOCK_ALL, index, &s0_entry)); 
            break;
            
        default:
            return BCM_E_PARAM;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_bucket_get
 * Purpose:
 *     Get COS queue bandwidth control bucket setting
 * Parameters:
 *     unit          - (IN) unit number
 *     port          - (IN) port number or GPORT identifier
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
_bcm_kt2_cosq_bucket_get(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                        uint32 *min_quantum, uint32 *max_quantum,
                        uint32 *burst_min_quantum, uint32 *burst_max_quantum,
                        uint32 flags)
{
    int rv;
    _bcm_kt2_cosq_node_t *node;
    bcm_port_t local_port;
    int index;
    soc_mem_t config_mem[2];
    lls_port_shaper_config_c_entry_t port_entry;
    lls_l0_shaper_config_c_entry_t  l0_entry;
    lls_l1_shaper_config_c_entry_t  l1_entry;
    lls_l2_shaper_config_lower_entry_t l2_entry;
    lls_s0_shaper_config_c_entry_t s0_entry;
    uint32 rate_exp[2], rate_mantissa[2];
    uint32 burst_exp[2], burst_mantissa[2];
    int i, idx;
    int level = _BCM_KT2_COSQ_NODE_LEVEL_L2;
    soc_field_t rate_exp_f[] = {
        C_MAX_REF_RATE_EXPf, C_MIN_REF_RATE_EXPf
    };
    soc_field_t rate_mant_f[] = {
        C_MAX_REF_RATE_MANTf, C_MIN_REF_RATE_MANTf
    };
    soc_field_t burst_exp_f[] = {
        C_MAX_THLD_EXPf, C_MIN_THLD_EXPf
    };
    soc_field_t burst_mant_f[] = {
        C_MAX_THLD_MANTf, C_MIN_THLD_MANTf
    };
    static const soc_field_t rate_exp_fields[] = {
       C_MAX_REF_RATE_EXP_0f, C_MAX_REF_RATE_EXP_1f,
       C_MAX_REF_RATE_EXP_2f, C_MAX_REF_RATE_EXP_3f,
       C_MIN_REF_RATE_EXP_0f, C_MIN_REF_RATE_EXP_1f,
       C_MIN_REF_RATE_EXP_2f, C_MIN_REF_RATE_EXP_3f
    };
    static const soc_field_t rate_mant_fields[] = {
       C_MAX_REF_RATE_MANT_0f, C_MAX_REF_RATE_MANT_1f,
       C_MAX_REF_RATE_MANT_2f, C_MAX_REF_RATE_MANT_3f,
       C_MIN_REF_RATE_MANT_0f, C_MIN_REF_RATE_MANT_1f,
       C_MIN_REF_RATE_MANT_2f, C_MIN_REF_RATE_MANT_3f
    };
    static const soc_field_t burst_exp_fields[] = {
       C_MAX_THLD_EXP_0f, C_MAX_THLD_EXP_1f,
       C_MAX_THLD_EXP_2f, C_MAX_THLD_EXP_3f,
       C_MIN_THLD_EXP_0f, C_MIN_THLD_EXP_1f,
       C_MIN_THLD_EXP_2f, C_MIN_THLD_EXP_3f       
    };
    static const soc_field_t burst_mant_fields[] = {
       C_MAX_THLD_MANT_0f, C_MAX_THLD_MANT_1f,
       C_MAX_THLD_MANT_2f, C_MAX_THLD_MANT_3f,
       C_MIN_THLD_MANT_0f, C_MIN_THLD_MANT_1f,
       C_MIN_THLD_MANT_2f, C_MIN_THLD_MANT_3f
    };

    if (cosq < 0) {
        if (cosq == -1) {
            /* caller needs to resolve the wild card value (-1) */
            return BCM_E_INTERNAL;
        } else { /* reject all other negative value */
            return BCM_E_PARAM;
        }
    }

    if (min_quantum == NULL || max_quantum == NULL || 
        burst_min_quantum == NULL || burst_max_quantum == NULL) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_index_resolve(unit, port, cosq,
                                    _BCM_KT2_COSQ_INDEX_STYLE_BUCKET,
                                    &local_port, &index, NULL));
    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
        BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port) ||
        BCM_GPORT_IS_SCHEDULER(port)) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_node_get(unit, port, NULL, NULL, NULL, &node));
        level = node->level;    
    } 

    switch (level) {
        case _BCM_KT2_COSQ_NODE_LEVEL_ROOT:
            config_mem[0] = LLS_PORT_SHAPER_CONFIG_Cm;
            SOC_IF_ERROR_RETURN
               (soc_mem_read(unit, config_mem[0], MEM_BLOCK_ALL,
                             index, &port_entry));
            rate_exp[0] = soc_mem_field32_get(unit, config_mem[0], &port_entry,
                                rate_exp_f[0]);
            rate_mantissa[0] = soc_mem_field32_get(unit, config_mem[0], 
                                                   &port_entry,
                                                   rate_mant_f[0]);
            rate_exp[1] = rate_exp[0];
            rate_mantissa[1] = rate_mantissa[0];
            burst_exp[0] = soc_mem_field32_get(unit, config_mem[0], &port_entry,
                                burst_exp_f[0]);
            burst_mantissa[0] = soc_mem_field32_get(unit, config_mem[0],
                                                    &port_entry,
                                                    burst_mant_f[0]);
            burst_exp[1] = burst_exp[0];
            burst_mantissa[1] = burst_mantissa[0];
            break;

        case _BCM_KT2_COSQ_NODE_LEVEL_L0:
            config_mem[0] = LLS_L0_SHAPER_CONFIG_Cm;
            config_mem[1] = LLS_L0_MIN_CONFIG_Cm;

            for (i = 0; i < 2; i++) {
                SOC_IF_ERROR_RETURN
                       (soc_mem_read(unit, config_mem[i], MEM_BLOCK_ALL,
                                 index, &l0_entry));
                rate_exp[i] = soc_mem_field32_get(unit, config_mem[i], 
                                                  &l0_entry,
                                                  rate_exp_f[i]);
                rate_mantissa[i] = soc_mem_field32_get(unit, config_mem[i], 
                                                       &l0_entry,
                                                       rate_mant_f[i]);
                burst_exp[i] = soc_mem_field32_get(unit, config_mem[i], 
                                                   &l0_entry,
                                                   burst_exp_f[i]);
                burst_mantissa[i] = soc_mem_field32_get(unit, config_mem[i], 
                                                        &l0_entry,
                                                        burst_mant_f[i]);
            }
            break;

        case _BCM_KT2_COSQ_NODE_LEVEL_L1:
            config_mem[0] = LLS_L1_SHAPER_CONFIG_Cm;
            config_mem[1] = LLS_L1_MIN_CONFIG_Cm;

            for (i = 0; i < 2; i++) {
                idx = (i * 4) + (index % 4);
                SOC_IF_ERROR_RETURN
                       (soc_mem_read(unit, config_mem[i], MEM_BLOCK_ALL,
                                     index/4, &l1_entry));
                rate_exp[i] = soc_mem_field32_get(unit, config_mem[i], 
                                                  &l1_entry,
                                                  rate_exp_fields[idx]);
                rate_mantissa[i] = soc_mem_field32_get(unit, config_mem[i], 
                                                       &l1_entry,
                                                       rate_mant_fields[idx]);
                burst_exp[i] = soc_mem_field32_get(unit, config_mem[i], 
                                                   &l1_entry,
                                                   burst_exp_fields[idx]);
                burst_mantissa[i] = soc_mem_field32_get(unit, config_mem[i], 
                                                        &l1_entry,
                                                        burst_mant_fields[idx]);
             }
            break;

        case _BCM_KT2_COSQ_NODE_LEVEL_L2:
            if ((index % 8) < 4) {
                config_mem[0] = LLS_L2_SHAPER_CONFIG_LOWERm;
                config_mem[1] = LLS_L2_MIN_CONFIG_LOWER_Cm;
            } else {
                config_mem[0] = LLS_L2_SHAPER_CONFIG_UPPERm;
                config_mem[1] = LLS_L2_MIN_CONFIG_UPPER_Cm;
            }

            for (i = 0; i < 2; i++) {
                idx = (i * 4) + (index % 4);
                SOC_IF_ERROR_RETURN
                       (soc_mem_read(unit, config_mem[i], MEM_BLOCK_ALL,
                                 index/8, &l2_entry));
                rate_exp[i] = soc_mem_field32_get(unit, config_mem[i], 
                                                  &l2_entry,
                                                  rate_exp_fields[idx]);
                rate_mantissa[i] = soc_mem_field32_get(unit, config_mem[i], 
                                                       &l2_entry,
                                                       rate_mant_fields[idx]);
                burst_exp[i] = soc_mem_field32_get(unit, config_mem[i], 
                                                   &l2_entry,
                                                   burst_exp_fields[idx]);
                burst_mantissa[i] = soc_mem_field32_get(unit, config_mem[i], 
                                                        &l2_entry,
                                                        burst_mant_fields[idx]);
            }
            break;

        case _BCM_KT2_COSQ_NODE_LEVEL_S0:
        case _BCM_KT2_COSQ_NODE_LEVEL_S1:
            config_mem[0] = (level == _BCM_KT2_COSQ_NODE_LEVEL_S0 &&
                             (node->linkphy_enabled)) ?
                            LLS_S0_SHAPER_CONFIG_Cm :
                            LLS_S1_SHAPER_CONFIG_Cm;
            SOC_IF_ERROR_RETURN
               (soc_mem_read(unit, config_mem[0], MEM_BLOCK_ALL,
                             index, &s0_entry));
            rate_exp[0] = soc_mem_field32_get(unit, config_mem[0], &s0_entry,
                                              rate_exp_f[0]);
            rate_mantissa[0] = soc_mem_field32_get(unit, config_mem[0], 
                                                   &s0_entry,
                                                   rate_mant_f[0]);
            burst_exp[0] = soc_mem_field32_get(unit, config_mem[0], &s0_entry,
                                               burst_exp_f[0]);
            burst_mantissa[0] = soc_mem_field32_get(unit, config_mem[0], 
                                                    &s0_entry,
                                                    burst_mant_f[0]);
            break;

        default:
            return BCM_E_PARAM;
    }

    /* convert exp and mantissa to bps */
    rv = soc_katana_compute_shaper_rate(unit, rate_mantissa[0], rate_exp[0],
                                        max_quantum);
    if (BCM_FAILURE(rv)) {
        return(rv);
    }

    rv = soc_katana_compute_shaper_rate(unit, rate_mantissa[1], rate_exp[1],
                                        min_quantum);
    if (BCM_FAILURE(rv)) {
        return(rv);
    }

    rv = soc_katana_compute_shaper_burst(unit, burst_mantissa[0], burst_exp[0],
                                         burst_max_quantum);
    if (BCM_FAILURE(rv)) {
        return(rv);
    }

    rv = soc_katana_compute_shaper_burst(unit, burst_mantissa[1], burst_exp[1],
                                         burst_min_quantum);
    if (BCM_FAILURE(rv)) {
        return(rv);
    }    

    return BCM_E_NONE;
}

/*
 *  Convert HW drop probability to percent value
 */
STATIC int
_bcm_kt2_hw_drop_prob_to_percent[] = {
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
_bcm_kt2_percent_to_drop_prob(int percent)
{
   int i;

   for (i = 14; i > 0 ; i--) {
      if (percent >= _bcm_kt2_hw_drop_prob_to_percent[i]) {
          break;
      }
   }
   return i;
}

STATIC int
_bcm_kt2_drop_prob_to_percent(int drop_prob) {
   return (_bcm_kt2_hw_drop_prob_to_percent[drop_prob]);
}

/*
 * Function:
 *     _bcm_kt2_cosq_wred_set
 * Purpose:
 *     Configure unicast queue WRED setting
 * Parameters:
 *     unit                - (IN) unit number
 *     port                - (IN) port number or GPORT identifier
 *     cosq                - (IN) COS queue number
 *     flags               - (IN) BCM_COSQ_DISCARD_XXX
 *     min_thresh          - (IN)
 *     max_thresh          - (IN)
 *     drop_probablity     - (IN)
 *     gain                - (IN)
 *     ignore_enable_flags - (IN)
 * Returns:
 *     BCM_E_XXX
 * Notes:
 *     If port is any form of local port, cosq is the hardware queue index.
 */
STATIC int
_bcm_kt2_cosq_wred_set(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                      uint32 flags, uint32 min_thresh, uint32 max_thresh,
                      int drop_probability, int gain, int ignore_enable_flags)
{
    soc_reg_t reg = 0;
    soc_field_t field;
    uint32 rval = 0;
    bcm_port_t local_port;
    int index;
    uint32 profile_index, old_profile_index, partner_index;
    mmu_wred_drop_curve_profile_0_entry_t entry_tcp_green;
    mmu_wred_drop_curve_profile_1_entry_t entry_tcp_yellow;
    mmu_wred_drop_curve_profile_2_entry_t entry_tcp_red;
    mmu_wred_drop_curve_profile_3_entry_t entry_nontcp_green;
    mmu_wred_drop_curve_profile_4_entry_t entry_nontcp_yellow;
    mmu_wred_drop_curve_profile_5_entry_t entry_nontcp_red;
    void *entries[6];
    soc_mem_t mems[6];
    _bcm_kt2_cosq_node_t *node;
    soc_mem_t wred_mem = MMU_WRED_QUEUE_CONFIG_BUFFERm;
    soc_mem_t partner_mem = MMU_WRED_QUEUE_CONFIG_QENTRYm;
    mmu_wred_queue_config_buffer_entry_t qentry, entry;
    int rate, i;
    int wred_enabled = 0;
    int ext_mem_port_count = 0;

    reg = WRED_CONFIGr;
    if (flags & BCM_COSQ_DISCARD_DEVICE) {
        if (flags & BCM_COSQ_DISCARD_PACKETS) {
            reg = GLOBAL_WRED_CONFIG_QENTRYr;
            field = WRED_ENABLEf;
            local_port = 0;
            index = 0; /* mmu init code uses service pool 0 only */
        } else {
            return BCM_E_PARAM;
        }
    } else {
        if (cosq == -1) {
           return BCM_E_PARAM;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_index_resolve(unit, port, cosq,
                                        _BCM_KT2_COSQ_INDEX_STYLE_WRED,
                                        &local_port, &index, NULL));
        if (flags & BCM_COSQ_DISCARD_PACKETS) {
            wred_mem = MMU_WRED_QUEUE_CONFIG_QENTRYm;
            partner_mem = MMU_WRED_QUEUE_CONFIG_BUFFERm;
        }
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
            BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
            BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port) ||
            BCM_GPORT_IS_SCHEDULER(port)) {
                BCM_IF_ERROR_RETURN
                    (_bcm_kt2_cosq_node_get(unit, port, NULL, &local_port, NULL,
                                       &node));
 
                if (node->level == _BCM_KT2_COSQ_NODE_LEVEL_L1) {
                    wred_mem = (flags & BCM_COSQ_DISCARD_PACKETS) ? 
                                MMU_WRED_OPN_CONFIG_QENTRYm : 
                                MMU_WRED_OPN_CONFIG_BUFFERm;
                    partner_mem = (flags & BCM_COSQ_DISCARD_PACKETS) ? 
                                MMU_WRED_OPN_CONFIG_BUFFERm : 
                                MMU_WRED_OPN_CONFIG_QENTRYm; 
                }
            }
        field = WRED_ENf;
    }

    old_profile_index = 0xffffffff;
    if (reg == GLOBAL_WRED_CONFIG_QENTRYr) {
        BCM_IF_ERROR_RETURN(soc_reg32_get(unit, reg, local_port, index, &rval));
    } else {
        SOC_IF_ERROR_RETURN
           (soc_mem_read(unit, wred_mem, MEM_BLOCK_ALL,
                         index, &qentry));
    }
    if (flags & (BCM_COSQ_DISCARD_NONTCP | BCM_COSQ_DISCARD_COLOR_ALL)) {
        if (reg == GLOBAL_WRED_CONFIG_QENTRYr) {
            old_profile_index = soc_reg_field_get(unit, reg, rval, 
                                                  PROFILE_INDEXf);
        } else {
            old_profile_index = soc_mem_field32_get(unit, wred_mem,
                                                    &qentry, PROFILE_INDEXf);
        } 
        entries[0] = &entry_tcp_green;
        entries[1] = &entry_tcp_yellow;
        entries[2] = &entry_tcp_red;
        entries[3] = &entry_nontcp_green;
        entries[4] = &entry_nontcp_yellow;
        entries[5] = &entry_nontcp_red;
        BCM_IF_ERROR_RETURN
            (soc_profile_mem_get(unit, _bcm_kt2_wred_profile[unit],
                                 old_profile_index, 1, entries));
        for (i = 0; i < 6; i++) {
            mems[i] = INVALIDm;
        }
        if (!(flags & BCM_COSQ_DISCARD_NONTCP)) {
            if (flags & BCM_COSQ_DISCARD_COLOR_GREEN) {
                mems[0] = MMU_WRED_DROP_CURVE_PROFILE_0m;
            }
            if (flags & BCM_COSQ_DISCARD_COLOR_YELLOW) {
                mems[1] = MMU_WRED_DROP_CURVE_PROFILE_1m;
            }
            if (flags & BCM_COSQ_DISCARD_COLOR_RED) {
                mems[2] = MMU_WRED_DROP_CURVE_PROFILE_2m;
            }
        } else if (!(flags & (BCM_COSQ_DISCARD_COLOR_GREEN |
                              BCM_COSQ_DISCARD_COLOR_YELLOW |
                              BCM_COSQ_DISCARD_COLOR_RED))) {
            mems[3] = MMU_WRED_DROP_CURVE_PROFILE_3m;
            mems[4] = MMU_WRED_DROP_CURVE_PROFILE_4m;
            mems[5] = MMU_WRED_DROP_CURVE_PROFILE_5m;

        } else {
            if (flags & BCM_COSQ_DISCARD_COLOR_GREEN) {
                mems[3] = MMU_WRED_DROP_CURVE_PROFILE_3m;
            }
            if (flags & BCM_COSQ_DISCARD_COLOR_YELLOW) {
                mems[4] = MMU_WRED_DROP_CURVE_PROFILE_4m;
            }
            if (flags & BCM_COSQ_DISCARD_COLOR_RED) {
                mems[5] = MMU_WRED_DROP_CURVE_PROFILE_5m;
            }
        }
        rate = _bcm_kt2_percent_to_drop_prob(drop_probability);
        for (i = 0; i < 6; i++) {
            if (mems[i] == INVALIDm) {
                continue;
            }
            soc_mem_field32_set(unit, mems[i], entries[i], MIN_THDf,
                                min_thresh);
            soc_mem_field32_set(unit, mems[i], entries[i], MAX_THDf,
                                max_thresh);
            soc_mem_field32_set(unit, mems[i], entries[i], MAX_DROP_RATEf,
                                rate);
        }
        BCM_IF_ERROR_RETURN
            (soc_profile_mem_add(unit, _bcm_kt2_wred_profile[unit], entries, 1,
                                 &profile_index));
        if (reg == GLOBAL_WRED_CONFIG_QENTRYr) {
            soc_reg_field_set(unit, reg, &rval, PROFILE_INDEXf, profile_index);
            soc_reg_field_set(unit, reg, &rval, WEIGHTf, gain);
        } else {
            soc_mem_field32_set(unit, wred_mem, &qentry, PROFILE_INDEXf, 
                                profile_index);
            soc_mem_field32_set(unit, wred_mem, &qentry, WEIGHTf, gain);
        }
    }

    /* Some APIs only modify profiles */
    if (!ignore_enable_flags) {
        if ( reg == GLOBAL_WRED_CONFIG_QENTRYr) {
            soc_reg_field_set(unit, reg, &rval, CAP_AVERAGEf,
                              flags & BCM_COSQ_DISCARD_CAP_AVERAGE ? 1 : 0);
            soc_reg_field_set(unit, reg, &rval, field,
                              flags & BCM_COSQ_DISCARD_ENABLE ? 1 : 0);
        } else {
            soc_mem_field32_set(unit, wred_mem, &qentry, CAP_AVERAGEf,
                              flags & BCM_COSQ_DISCARD_CAP_AVERAGE ? 1 : 0);
            soc_mem_field32_set(unit, wred_mem, &qentry, field,
                              flags & BCM_COSQ_DISCARD_ENABLE ? 1 : 0);
            if (wred_mem == MMU_WRED_QUEUE_CONFIG_BUFFERm || 
                wred_mem == MMU_WRED_QUEUE_CONFIG_QENTRYm) {
                /* ECN marking is applicable only for queues */
                soc_mem_field32_set(unit, wred_mem, &qentry, ECN_MARKING_ENf,
                                  flags & BCM_COSQ_DISCARD_MARK_CONGESTION ?
                                  1 : 0);
                if (wred_mem == MMU_WRED_QUEUE_CONFIG_QENTRYm) {
                    /* ECN marking needs to be enabled in BUFFER entry */
                    SOC_IF_ERROR_RETURN
                           (soc_mem_read(unit, MMU_WRED_QUEUE_CONFIG_BUFFERm, 
                                         MEM_BLOCK_ALL, index, &entry));
                    soc_mem_field32_set(unit, MMU_WRED_QUEUE_CONFIG_BUFFERm, 
                                     &entry, ECN_MARKING_ENf,
                                     flags & BCM_COSQ_DISCARD_MARK_CONGESTION ?
                                     1 : 0);
                    SOC_IF_ERROR_RETURN
                        (soc_mem_write(unit, MMU_WRED_QUEUE_CONFIG_BUFFERm, 
                                       MEM_BLOCK_ALL, index, &entry));
                }    
            }
        }
    }

    partner_index = 0xffffffff;
    if (reg == GLOBAL_WRED_CONFIG_QENTRYr) {
        BCM_IF_ERROR_RETURN(soc_reg32_set(unit, reg, local_port, index, rval));

        /* update interna/external buffer entries based on port configuration */
        BCM_IF_ERROR_RETURN(soc_reg32_get(unit, GLOBAL_WRED_CONFIG_BUFFERIr,
                                          local_port, index, &rval));
        soc_reg_field_set(unit, GLOBAL_WRED_CONFIG_BUFFERIr, &rval, field,
                          flags & BCM_COSQ_DISCARD_ENABLE ? 1 : 0);
        soc_reg_field_set(unit, GLOBAL_WRED_CONFIG_BUFFERIr, &rval,
                          PROFILE_INDEXf, 0);
        BCM_IF_ERROR_RETURN(soc_reg32_set(unit, GLOBAL_WRED_CONFIG_BUFFERIr,
                            local_port, index, rval));
        BCM_PBMP_COUNT(PBMP_EXT_MEM (unit), ext_mem_port_count);
        if (ext_mem_port_count > 0) {
            BCM_IF_ERROR_RETURN(soc_reg32_get(unit, GLOBAL_WRED_CONFIG_BUFFEREr,
                                              local_port, index, &rval));
            soc_reg_field_set(unit, GLOBAL_WRED_CONFIG_BUFFEREr, &rval, field,
                              flags & BCM_COSQ_DISCARD_ENABLE ? 1 : 0);
            soc_reg_field_set(unit, GLOBAL_WRED_CONFIG_BUFFEREr, &rval,
                              PROFILE_INDEXf, 0);
            BCM_IF_ERROR_RETURN(soc_reg32_set(unit, GLOBAL_WRED_CONFIG_BUFFEREr,
                                local_port, index, rval));
        }
    } else {
        SOC_IF_ERROR_RETURN
            (soc_mem_write(unit, wred_mem, MEM_BLOCK_ALL,
                           index, &qentry));
        SOC_IF_ERROR_RETURN
            (soc_mem_read(unit, partner_mem, MEM_BLOCK_ALL,
                              index, &qentry));
        wred_enabled = soc_mem_field32_get(unit, partner_mem,
                                           &qentry, WRED_ENf);
        
        /* if buffer/qentry_mem WRED enabled, make sure the partner_mem 
         * (qentry/buffer_mem) also WRED enabled.
         * if buffer/qentry_mem WRED disabled, make sure the partner_mem
         *  (qentry/buffer_mem) also WRED disabled. 
         */
        if (((flags & BCM_COSQ_DISCARD_ENABLE) && (wred_enabled == 0)) ||
            (!(flags & BCM_COSQ_DISCARD_ENABLE) && (wred_enabled == 1))) {
            partner_index = soc_mem_field32_get(unit, partner_mem,
                                                &qentry, PROFILE_INDEXf); 
            soc_mem_field32_set(unit, partner_mem, &qentry, PROFILE_INDEXf, 0);
            soc_mem_field32_set(unit, partner_mem, &qentry, WRED_ENf, 
                                flags & BCM_COSQ_DISCARD_ENABLE ? 1 : 0);
            SOC_IF_ERROR_RETURN
                (soc_mem_write(unit, partner_mem, MEM_BLOCK_ALL,
                               index, &qentry));    
        }    
    }

    if (old_profile_index != 0xffffffff && old_profile_index > 0) {
        BCM_IF_ERROR_RETURN
            (soc_profile_mem_delete(unit, _bcm_kt2_wred_profile[unit],
                                    old_profile_index));
    }
    
    if (partner_index != 0xffffffff && partner_index > 0) {
        BCM_IF_ERROR_RETURN
            (soc_profile_mem_delete(unit, _bcm_kt2_wred_profile[unit],
                                    partner_index));
    }    

    return BCM_E_NONE;
}

/*
 * Function:
 *     _bcm_kt2_cosq_wred_get
 * Purpose:
 *     Get unicast queue WRED setting
 * Parameters:
 *     unit                - (IN) unit number
 *     port                - (IN) port number or GPORT identifier
 *     cosq                - (IN) COS queue number
 *     flags               - (IN/OUT) BCM_COSQ_DISCARD_XXX
 *     min_thresh          - (OUT)
 *     max_thresh          - (OUT)
 *     drop_probablity     - (OUT)
 *     gain                - (OUT)
 * Returns:
 *     BCM_E_XXX
 * Notes:
 *     If port is any form of local port, cosq is the hardware queue index.
 */
STATIC int
_bcm_kt2_cosq_wred_get(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                      uint32 *flags, uint32 *min_thresh, uint32 *max_thresh,
                      int *drop_probability, int *gain)
{
    soc_reg_t reg = 0;
    soc_field_t field;
    uint32 rval;
    bcm_port_t local_port;
    int index, profile_index;
    mmu_wred_drop_curve_profile_0_entry_t entry_tcp_green;
    mmu_wred_drop_curve_profile_1_entry_t entry_tcp_yellow;
    mmu_wred_drop_curve_profile_2_entry_t entry_tcp_red;
    mmu_wred_drop_curve_profile_3_entry_t entry_nontcp_green;
    mmu_wred_drop_curve_profile_4_entry_t entry_nontcp_yellow;
    mmu_wred_drop_curve_profile_5_entry_t entry_nontcp_red;
    void *entries[6];
    void *entry_p;
    soc_mem_t mem;
    _bcm_kt2_cosq_node_t *node;
    soc_mem_t wred_mem = MMU_WRED_QUEUE_CONFIG_BUFFERm;
    mmu_wred_queue_config_buffer_entry_t qentry;
    int rate;

    if (*flags & BCM_COSQ_DISCARD_DEVICE) {
        if (*flags & BCM_COSQ_DISCARD_PACKETS) {
            reg = GLOBAL_WRED_CONFIG_QENTRYr;
            field = WRED_ENABLEf;
            local_port = 0;
            index = 0; /* mmu init code uses service pool 0 only */
            wred_mem = 0;
        } else {
            return BCM_E_PARAM;
        }
    } else {
        if (cosq == -1) {
           return BCM_E_PARAM;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_index_resolve(unit, port, cosq,
                                        _BCM_KT2_COSQ_INDEX_STYLE_WRED,
                                        &local_port, &index, NULL));
        if (*flags & BCM_COSQ_DISCARD_PACKETS) {
            wred_mem = MMU_WRED_QUEUE_CONFIG_QENTRYm;
        }
        if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(port) ||
            BCM_GPORT_IS_MCAST_QUEUE_GROUP(port) ||
            BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port) ||
            BCM_GPORT_IS_SCHEDULER(port)) {        
                BCM_IF_ERROR_RETURN
                    (_bcm_kt2_cosq_node_get(unit, port, NULL, &local_port, NULL,
                                       &node));
                if (node->level == _BCM_KT2_COSQ_NODE_LEVEL_L1) {
                    wred_mem = (*flags & BCM_COSQ_DISCARD_PACKETS) ? 
                                MMU_WRED_OPN_CONFIG_QENTRYm : 
                                MMU_WRED_OPN_CONFIG_BUFFERm;
                }
            }
         
        field = WRED_ENf;
    }

    if (reg == GLOBAL_WRED_CONFIG_QENTRYr) {
        BCM_IF_ERROR_RETURN(soc_reg32_get(unit, reg, local_port, index, &rval));
        profile_index = soc_reg_field_get(unit, reg, rval, PROFILE_INDEXf);
    } else {
            SOC_IF_ERROR_RETURN
               (soc_mem_read(unit, wred_mem, MEM_BLOCK_ALL,
                             index, &qentry));
            profile_index = soc_mem_field32_get(unit, wred_mem,
                                                &qentry, PROFILE_INDEXf);
    }

    if (!(*flags & BCM_COSQ_DISCARD_NONTCP)) {
        if (*flags & BCM_COSQ_DISCARD_COLOR_GREEN) {
            mem = MMU_WRED_DROP_CURVE_PROFILE_0m;
            entry_p = &entry_tcp_green;
        } else if (*flags & BCM_COSQ_DISCARD_COLOR_YELLOW) {
            mem = MMU_WRED_DROP_CURVE_PROFILE_1m;
            entry_p = &entry_tcp_yellow;
        } else if (*flags & BCM_COSQ_DISCARD_COLOR_RED) {
            mem = MMU_WRED_DROP_CURVE_PROFILE_2m;
            entry_p = &entry_tcp_red;
        } else {
           mem = MMU_WRED_DROP_CURVE_PROFILE_0m;
           entry_p = &entry_tcp_green;
        }
    } else {
        if (*flags & BCM_COSQ_DISCARD_COLOR_GREEN) {
            mem = MMU_WRED_DROP_CURVE_PROFILE_3m;
            entry_p = &entry_nontcp_green;
        } else if (*flags & BCM_COSQ_DISCARD_COLOR_YELLOW) {
            mem = MMU_WRED_DROP_CURVE_PROFILE_4m;
            entry_p = &entry_nontcp_yellow;
        } else if (*flags & BCM_COSQ_DISCARD_COLOR_RED) {
            mem = MMU_WRED_DROP_CURVE_PROFILE_5m;
            entry_p = &entry_nontcp_red;
        } else {
           mem = MMU_WRED_DROP_CURVE_PROFILE_3m;
           entry_p = &entry_nontcp_green;
        }
    }
    entries[0] = &entry_tcp_green;
    entries[1] = &entry_tcp_yellow;
    entries[2] = &entry_tcp_red;
    entries[3] = &entry_nontcp_green;
    entries[4] = &entry_nontcp_yellow;
    entries[5] = &entry_nontcp_red;
    BCM_IF_ERROR_RETURN
        (soc_profile_mem_get(unit, _bcm_kt2_wred_profile[unit],
                             profile_index, 1, entries));
    if (min_thresh != NULL) {
        *min_thresh = soc_mem_field32_get(unit, mem, entry_p, MIN_THDf);
    }
    if (max_thresh != NULL) {
        *max_thresh = soc_mem_field32_get(unit, mem, entry_p, MAX_THDf);
    }
    if (drop_probability != NULL) {
        rate = soc_mem_field32_get(unit, mem, entry_p, MAX_DROP_RATEf);
        *drop_probability = _bcm_kt2_drop_prob_to_percent(rate);
    }

    if (gain != NULL && reg == GLOBAL_WRED_CONFIG_QENTRYr) {
        *gain = soc_reg_field_get(unit, reg, rval, WEIGHTf);
    }
    if (gain != NULL && wred_mem != 0) {
        *gain = soc_mem_field32_get(unit, wred_mem, &qentry, WEIGHTf);
    }

    *flags &= ~(BCM_COSQ_DISCARD_CAP_AVERAGE | BCM_COSQ_DISCARD_ENABLE);
    if ((reg == GLOBAL_WRED_CONFIG_QENTRYr) &&
        soc_reg_field_get(unit, reg, rval, CAP_AVERAGEf)) {
        *flags |= BCM_COSQ_DISCARD_CAP_AVERAGE;
    }
    if ((wred_mem != 0) &&
         soc_mem_field32_get(unit, wred_mem, &qentry, CAP_AVERAGEf)) {
        *flags |= BCM_COSQ_DISCARD_CAP_AVERAGE;
    }

    if ((reg == GLOBAL_WRED_CONFIG_QENTRYr) &&
        soc_reg_field_get(unit, reg, rval, field)) {
        *flags |= BCM_COSQ_DISCARD_ENABLE;
    }
    if ((wred_mem != 0) &&
         soc_mem_field32_get(unit, wred_mem, &qentry, field)) {
        *flags |= BCM_COSQ_DISCARD_ENABLE;
    }

    if (wred_mem == MMU_WRED_QUEUE_CONFIG_BUFFERm || 
        wred_mem == MMU_WRED_QUEUE_CONFIG_QENTRYm) {
        if (soc_mem_field32_get(unit, wred_mem, &qentry, ECN_MARKING_ENf)) {
            *flags |= BCM_COSQ_DISCARD_MARK_CONGESTION;
        }
    }

    return BCM_E_NONE;
}

STATIC int
_bcm_kt2_cosq_gport_delete_node(int unit, bcm_gport_t gport)
{
    _bcm_kt2_cosq_node_t *node;
    _bcm_kt2_mmu_info_t *mmu_info;
    int encap_id;    

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_node_get(unit, gport, NULL, NULL, NULL, &node));

    if (node->level != _BCM_KT2_COSQ_NODE_LEVEL_ROOT && 
            node->cosq_attached_to >= 0) {
        BCM_IF_ERROR_RETURN (bcm_kt2_cosq_gport_detach(unit, node->gport,
                    node->parent->gport, 
                    node->cosq_attached_to));
    }

    mmu_info = _bcm_kt2_mmu_info[unit];

    if (BCM_GPORT_IS_SCHEDULER(gport)) {
        /* scheduler node */
        encap_id = (BCM_GPORT_SCHEDULER_GET(gport) >> 8) & 0x7ff;

        if (encap_id == 0) {
            encap_id = (BCM_GPORT_SCHEDULER_GET(gport) & 0xff);
        }
    } else if (BCM_GPORT_IS_LOCAL(gport)) {    
        encap_id = BCM_GPORT_LOCAL_GET(gport);
    } else if (BCM_GPORT_IS_MODPORT(gport)) {
        encap_id = BCM_GPORT_MODPORT_PORT_GET(gport);
    } else if (BCM_GPORT_IS_SUBPORT_PORT(gport)) {
        encap_id = BCM_GPORT_SUBPORT_PORT_GET(gport);
    } else {
        return BCM_E_PARAM;
    }

    _bcm_kt2_node_index_clear(&mmu_info->sched_list, encap_id, 1);
    _BCM_KT2_COSQ_NODE_INIT(node);

    return BCM_E_NONE;
}


STATIC int
_bcm_kt2_cosq_gport_delete_all(int unit, bcm_gport_t gport, int *rv)
{
    _bcm_kt2_cosq_node_t *node;
    _bcm_kt2_mmu_info_t *mmu_info;
    int encap_id;    
    
    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_node_get(unit, gport, NULL, NULL, NULL, &node));

    if (node->child != NULL) {
        _bcm_kt2_cosq_gport_delete_all(unit, node->child->gport, rv);
        BCM_IF_ERROR_RETURN(*rv);
    }

    if (node->sibling != NULL) {
        _bcm_kt2_cosq_gport_delete_all(unit, node->sibling->gport, rv);
        BCM_IF_ERROR_RETURN(*rv);
    }

    if (node->level != _BCM_KT2_COSQ_NODE_LEVEL_ROOT && 
        node->cosq_attached_to >= 0) {
        *rv = bcm_kt2_cosq_gport_detach(unit, node->gport, node->parent->gport, 
                                       node->cosq_attached_to);
        BCM_IF_ERROR_RETURN(*rv);
    }

    mmu_info = _bcm_kt2_mmu_info[unit];

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) { /* unicast queue group */
        encap_id = BCM_GPORT_UCAST_QUEUE_GROUP_QID_GET(gport);
    } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) { 
        /* multicast queue group */
        encap_id = BCM_GPORT_MCAST_QUEUE_GROUP_QID_GET(gport); 
    } else if (BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(gport)) {
        encap_id = BCM_GPORT_DESTMOD_QUEUE_GROUP_QID_GET(gport); 
        if (mmu_info->num_dmvoq_queues > 0) {
            _bcm_kt2_node_index_clear(&mmu_info->dmvoq_qlist,
                    encap_id , 1);
        } else {
            _bcm_kt2_node_index_clear(&mmu_info->ext_qlist,
                    encap_id , 1);
        }
    } else if (BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(gport)) {
        /* subscriber queue group */
        encap_id = BCM_GPORT_UCAST_SUBSCRIBER_QUEUE_GROUP_QID_GET(gport);
        _bcm_kt2_node_index_clear(&mmu_info->ext_qlist,
                                (encap_id - mmu_info->num_base_queues) , 1);
    } else {
        if (BCM_GPORT_IS_SCHEDULER(gport)) {
           /* scheduler node */
           encap_id = (BCM_GPORT_SCHEDULER_GET(gport) >> 8) & 0x7ff;
        
           if (encap_id == 0) {
               encap_id = (BCM_GPORT_SCHEDULER_GET(gport) & 0xff);
           }
        } else if (BCM_GPORT_IS_LOCAL(gport)) {    
           encap_id = BCM_GPORT_LOCAL_GET(gport);
        } else if (BCM_GPORT_IS_MODPORT(gport)) {
           encap_id = BCM_GPORT_MODPORT_PORT_GET(gport);
        } else if (BCM_GPORT_IS_SUBPORT_PORT(gport)) {
           encap_id = BCM_GPORT_SUBPORT_PORT_GET(gport);
        } else {
            return BCM_E_PARAM;
        }

        _bcm_kt2_node_index_clear(&mmu_info->sched_list, encap_id, 1);
   }

   if (node->cosq_map != NULL) {
       sal_free(node->cosq_map);
   }

    _BCM_KT2_COSQ_NODE_INIT(node);

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_gport_add
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
bcm_kt2_cosq_gport_add(int unit, bcm_gport_t port, int numq, uint32 flags,
                      bcm_gport_t *gport)
{
    _bcm_kt2_mmu_info_t *mmu_info;
    _bcm_kt2_cosq_port_info_t *port_info;
    _bcm_kt2_cosq_node_t *node = NULL;
    bcm_module_t local_modid, modid_out;
    bcm_port_t local_port, port_out;
    int id, max_nodes;
    uint32 sched_encap;
    _bcm_kt2_cosq_list_t *list;
    int max_queues;

    LOG_INFO(BSL_LS_BCM_COSQ,
             (BSL_META_U(unit,
                         "bcm_kt2_cosq_gport_add: unit=%d port=0x%x numq=%d flags=0x%x\n"),
              unit, port, numq, flags));

    if (gport == NULL) {
        return BCM_E_PARAM;
    }

    if (_bcm_kt2_mmu_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_localport_resolve(unit, port, &local_port));

    if (local_port < 0) {
        return BCM_E_PORT;
    }

    mmu_info = _bcm_kt2_mmu_info[unit];
    switch (flags) {
    case BCM_COSQ_GPORT_UCAST_QUEUE_GROUP:
        if (numq != 1) {
            return BCM_E_PARAM;
        }
        port_info = &mmu_info->port[local_port];
        for (id = port_info->q_offset; id < port_info->q_limit; id++) {
            if (mmu_info->queue_node[id].numq == 0) {
                break;
            }
        }
        
        if (id == port_info->q_limit) {
            return BCM_E_RESOURCE;
        }            

        BCM_GPORT_UCAST_QUEUE_GROUP_SYSQID_SET(*gport, local_port, id);
        node = &mmu_info->queue_node[id];
        node->gport = *gport;
        node->numq = numq;
        node->hw_index = id;
        break;

    case BCM_COSQ_GPORT_MCAST_QUEUE_GROUP:
        if (numq != 1) {
            return BCM_E_PARAM;
        }
        port_info = &mmu_info->port[local_port];
        for (id = port_info->mcq_offset; id < port_info->mcq_limit; id++) {
            if (mmu_info->queue_node[id].numq == 0) {
                break;
            }
        }
        
        if (id == port_info->mcq_limit) {
            return BCM_E_RESOURCE;
        }            

        BCM_GPORT_MCAST_QUEUE_GROUP_SYSQID_SET(*gport, local_port, id);
        node = &mmu_info->queue_node[id];
        node->gport = *gport;
        node->numq = numq;
        node->hw_index = id;
        break;        

    case BCM_COSQ_GPORT_VLAN_UCAST_QUEUE_GROUP:
    case BCM_COSQ_GPORT_SUBSCRIBER:
        if (numq != 1) {
            return BCM_E_PARAM;
        }

        list = &mmu_info->ext_qlist;
        max_queues = mmu_info->num_ext_queues;
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_node_index_get(list->bits, 0, max_queues,
                                mmu_info->qset_size, 1, &id));
        _bcm_kt2_node_index_set(list, id, 1);
        id += mmu_info->num_base_queues;
        BCM_GPORT_UCAST_SUBSCRIBER_QUEUE_GROUP_QID_SET(*gport, id);
        *gport |= (local_port << 16);
        node = &mmu_info->queue_node[id];
        node->gport = *gport;
        node->numq = numq;
        break;

    case BCM_COSQ_GPORT_DESTMOD_UCAST_QUEUE_GROUP:
        if (numq != 1) {
            return BCM_E_PARAM;
        }

        if (!IS_HG_PORT(unit, local_port)) {
            return BCM_E_PORT;
        }
        if (mmu_info->num_dmvoq_queues == 0) { 
            list = &mmu_info->ext_qlist;
            max_queues = mmu_info->num_ext_queues;
        } else {
            list = &mmu_info->dmvoq_qlist;
            max_queues = mmu_info->num_dmvoq_queues;
        }
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_node_index_get(list->bits, 0, max_queues,
                                     mmu_info->qset_size, 1, &id));
        _bcm_kt2_node_index_set(list, id, 1);
        BCM_GPORT_DESTMOD_QUEUE_GROUP_SYSQID_SET(*gport, local_port, id);
        if (mmu_info->num_dmvoq_queues == 0) { 
            id += mmu_info->num_base_queues;
        } else {
            id += mmu_info->base_dmvoq_queue;
        }
        node = &mmu_info->queue_node[id];
        node->gport = *gport;
        node->numq = numq;
        node->level = _BCM_KT2_COSQ_NODE_LEVEL_L2;
        node->type = _BCM_KT2_NODE_VOQ;
        break;

    case (BCM_COSQ_GPORT_WITH_ID | BCM_COSQ_GPORT_DESTMOD_UCAST_QUEUE_GROUP):
        if (numq != 1) {
            return BCM_E_PARAM;
        }

        if (!IS_HG_PORT(unit, local_port)) {
            return BCM_E_PORT;
        }

        if (BCM_GPORT_DESTMOD_QUEUE_GROUP_SYSPORTID_GET(*gport) 
                != local_port) {
            return BCM_E_PARAM;
        }    

        id = BCM_GPORT_DESTMOD_QUEUE_GROUP_QID_GET(*gport);
        if ((id >= mmu_info->num_dmvoq_queues) ||
             (mmu_info->num_dmvoq_queues == 0)) {
            return BCM_E_PARAM;
        }
        max_queues = mmu_info->num_dmvoq_queues;
        node = &mmu_info->queue_node[id + mmu_info->base_dmvoq_queue];
        if(node->numq != 0) {
            return BCM_E_EXISTS;
        }
        _bcm_kt2_node_index_set(&mmu_info->dmvoq_qlist, id, 1);
        _bcm_kt2_node_index_set(&mmu_info->l2_dmvoq_qlist, id, 1);
        id += mmu_info->base_dmvoq_queue;
        node->hw_index  = id;  
        node->gport = *gport;
        node->numq = numq;          
        node->level = _BCM_KT2_COSQ_NODE_LEVEL_L2;
        node->type = _BCM_KT2_NODE_VOQ;
        break;
       
    case BCM_COSQ_GPORT_SCHEDULER_WFQ:
    case BCM_COSQ_GPORT_SCHEDULER:
    case (BCM_COSQ_GPORT_SCHEDULER | BCM_COSQ_GPORT_SCHEDULER_WFQ):
        /* passthru */
    case 0:  

        if (numq == 0 || numq < -1) {
            return BCM_E_PARAM;
        }

        if (flags & BCM_COSQ_GPORT_SCHEDULER) {
            BCM_IF_ERROR_RETURN
                (_bcm_kt2_cosq_max_nodes_get(unit, _BCM_KT2_COSQ_NODE_LEVEL_L2,
                                            &max_nodes));
        } else {
            BCM_IF_ERROR_RETURN
                (_bcm_kt2_cosq_max_nodes_get(unit, _BCM_KT2_COSQ_NODE_LEVEL_L0,
                                            &max_nodes));
        }
        if (numq > max_nodes) {
            return BCM_E_PARAM;
        }

        BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &local_modid));
        BCM_IF_ERROR_RETURN
            (_bcm_esw_stk_modmap_map(unit, BCM_STK_MODMAP_GET, local_modid,
                                     local_port, &modid_out, &port_out));

        if ((flags == 0) || (flags == BCM_COSQ_GPORT_SCHEDULER_WFQ)) {
            /* this is a port level scheduler */
            id = port_out;

            if ( id < 0 || id >= num_port_schedulers) {
                return BCM_E_PARAM;
            }

            _bcm_kt2_node_index_set(&mmu_info->sched_list, id, 1);
            node = &mmu_info->sched_node[id];
            sched_encap = (id << 8) | port_out;
            BCM_GPORT_SCHEDULER_SET(*gport, sched_encap);
            node->gport = *gport;
            node->level = _BCM_KT2_COSQ_NODE_LEVEL_ROOT;
            node->hw_index = id;
            node->numq = numq;
            node->cosq_attached_to = 0;
            if (flags == BCM_COSQ_GPORT_SCHEDULER_WFQ) {
                node->wrr_mode = 1;
            }
        } else {
            if (BCM_GPORT_IS_SUBPORT_PORT(port)) {
                id = BCM_GPORT_SUBPORT_PORT_GET(port);
            } else {
                BCM_IF_ERROR_RETURN
                    (_bcm_kt2_node_index_get(mmu_info->sched_list.bits,
                                            SOC_MAX_NUM_PP_PORTS,
                                            mmu_info->num_nodes, 1, 1, &id));
            }    
            _bcm_kt2_node_index_set(&mmu_info->sched_list, id, 1);
            node = &mmu_info->sched_node[id];
            sched_encap = (id << 8) | port_out;
            BCM_GPORT_SCHEDULER_SET(*gport, sched_encap);
            node->gport = *gport;
            node->numq = numq;
            node->cosq_attached_to = -1;
            if (flags & BCM_COSQ_GPORT_SCHEDULER_WFQ) {
                node->wrr_mode = 1;
            }
            if (BCM_GPORT_IS_SUBPORT_PORT(port)) {
                node->subport_enabled = 1;
            }
       }
       break;
        
    case BCM_COSQ_GPORT_WITH_ID:
        if (BCM_GPORT_IS_SCHEDULER(*gport)) {
            if (numq == 0 || numq < -1 || numq > 8) {
                return BCM_E_PARAM;
            }
            id = BCM_GPORT_SCHEDULER_GET(*gport);

            if (id != local_port) {
                return BCM_E_PARAM;
            }    
            
            /* allow only port or subport level scheduler with id */
            if ( id < 0 || id >= SOC_MAX_NUM_PP_PORTS) {
                return BCM_E_PARAM;
            }

            _bcm_kt2_node_index_set(&mmu_info->sched_list, id, 1);
            node = &mmu_info->sched_node[id];
            node->gport = *gport;
            node->level = _BCM_KT2_COSQ_NODE_LEVEL_ROOT;
            node->hw_index = id;
            node->numq = numq;
            node->cosq_attached_to = 0;
        } else if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(*gport)) {
            if (numq != 1) {
                return BCM_E_PARAM;
            }

            if (BCM_GPORT_UCAST_QUEUE_GROUP_SYSPORTID_GET(*gport) 
                                                   != local_port) {
                return BCM_E_PARAM;
            }    
            
            id = BCM_GPORT_UCAST_QUEUE_GROUP_QID_GET(*gport);
            port_info = &mmu_info->port[local_port];
            /* allow only base queues */
            if (id < port_info->q_offset || id >= port_info->q_limit) {
                return BCM_E_PARAM;
            }

            list = &mmu_info->l2_base_qlist; 
            if (SHR_BITGET(list->bits, id) != 0) {
                return BCM_E_RESOURCE;
            }    

            node = &mmu_info->queue_node[id];
            if (node->numq != 0) {
                return BCM_E_EXISTS;
            }
            
            _bcm_kt2_node_index_set(&mmu_info->l2_base_qlist, id, 1);
            node->hw_index  = id;  
            node->gport = *gport;
            node->numq = numq;          
        } else {
            return BCM_E_PARAM;
        }
        break;
        
    default:
        return BCM_E_PARAM;
    }

    LOG_INFO(BSL_LS_BCM_COSQ,
             (BSL_META_U(unit,
                         "                       gport=0x%x\n"),
              *gport));

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_gport_delete
 * Purpose:
 *     Destroy a cosq gport structure
 * Parameters:
 *     unit  - (IN) unit number
 *     gport - (IN) GPORT identifier
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_kt2_cosq_gport_delete(int unit, bcm_gport_t gport)
{
    _bcm_kt2_cosq_node_t *node;
    _bcm_kt2_mmu_info_t *mmu_info;
    int encap_id;
    int rv = BCM_E_NONE;

    LOG_INFO(BSL_LS_BCM_COSQ,
             (BSL_META_U(unit,
                         "bcm_kt2_cosq_gport_delete: unit=%d gport=0x%x\n"),
              unit, gport));

    if (_bcm_kt2_mmu_info[unit] == NULL) {
        return BCM_E_INIT;
    }
    mmu_info = _bcm_kt2_mmu_info[unit];
    
    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_node_get(unit, gport, NULL, NULL, NULL, &node));

    if (node->gport < 0) {
        return BCM_E_NOT_FOUND;
    }    
    
    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(gport)) {
        if (node->cosq_attached_to >= 0) {
            BCM_IF_ERROR_RETURN
                (bcm_kt2_cosq_gport_detach(unit, gport, node->parent->gport, 
                                          node->cosq_attached_to));
        }
        if (BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(gport)) {
            encap_id = BCM_GPORT_UCAST_SUBSCRIBER_QUEUE_GROUP_QID_GET(gport);
            _bcm_kt2_node_index_clear(&mmu_info->ext_qlist,
                                  (encap_id - mmu_info->num_base_queues) , 1);
        }   
        if (BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(gport)) {
            encap_id = BCM_GPORT_DESTMOD_QUEUE_GROUP_QID_GET(gport);
            if (mmu_info->num_dmvoq_queues > 0) {
                _bcm_kt2_node_index_clear(&mmu_info->dmvoq_qlist,
                        encap_id, 1);
            } else {
                _bcm_kt2_node_index_clear(&mmu_info->ext_qlist,
                        encap_id, 1);
            }
        }
        _BCM_KT2_COSQ_NODE_INIT(node);
    } else {
        /* 
         *  Deleting the SUB-Tree 
         */
        if (node->child != NULL) {
            BCM_IF_ERROR_RETURN
                (_bcm_kt2_cosq_gport_delete_all(unit, node->child->gport, &rv));
        }
        /* 
         *  Deleting the parent Node 
         */
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_gport_delete_node(unit, gport));
    }

    return rv;
}

/*
 * Function:
 *     bcm_kt2_cosq_gport_get
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
bcm_kt2_cosq_gport_get(int unit, bcm_gport_t gport, bcm_gport_t *port,
                      int *numq, uint32 *flags)
{
    _bcm_kt2_cosq_node_t *node;
    bcm_module_t modid;
    bcm_port_t local_port;
    int id;
    _bcm_gport_dest_t dest;

    if (port == NULL || numq == NULL || flags == NULL) {
        return BCM_E_PARAM;
    }

    LOG_INFO(BSL_LS_BCM_COSQ,
             (BSL_META_U(unit,
                         "bcm_kt2_cosq_gport_get: unit=%d gport=0x%x\n"),
              unit, gport));

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_node_get(unit, gport, NULL, &local_port, &id, &node));

    if (SOC_USE_GPORT(unit)) {
        BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &modid));
        dest.gport_type = _SHR_GPORT_TYPE_MODPORT;
        dest.modid = modid;
        dest.port = local_port;
        BCM_IF_ERROR_RETURN(_bcm_esw_gport_construct(unit, &dest, port));
    } else {
        *port = local_port;
    }

    *numq = node->numq;

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
        *flags = BCM_COSQ_GPORT_UCAST_QUEUE_GROUP;
    } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        *flags = BCM_COSQ_GPORT_MCAST_QUEUE_GROUP; 
    } else if (BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(gport)) {
        *flags = BCM_COSQ_GPORT_DESTMOD_UCAST_QUEUE_GROUP; 
    } else if (BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(gport)) {
        *flags = BCM_COSQ_GPORT_SUBSCRIBER;
    } else if (BCM_GPORT_IS_SCHEDULER(gport)) {
        *flags = BCM_COSQ_GPORT_SCHEDULER;
    } else {
        *flags = 0;
    }

    LOG_INFO(BSL_LS_BCM_COSQ,
             (BSL_META_U(unit,
                         "                       port=0x%x numq=%d flags=0x%x\n"),
              *port, *numq, *flags));

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_gport_traverse
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
bcm_kt2_cosq_gport_traverse(int unit, bcm_cosq_gport_traverse_cb cb,
                           void *user_data)
{
    _bcm_kt2_cosq_node_t *port_info;
    _bcm_kt2_mmu_info_t *mmu_info;
    bcm_module_t my_modid, modid_out;
    bcm_port_t port, port_out;

    if (_bcm_kt2_mmu_info[unit] == NULL) {
        return BCM_E_INIT;
    }
    mmu_info = _bcm_kt2_mmu_info[unit];

    BCM_IF_ERROR_RETURN(bcm_esw_stk_my_modid_get(unit, &my_modid));
    PBMP_ALL_ITER(unit, port) {
        BCM_IF_ERROR_RETURN
            (_bcm_esw_stk_modmap_map(unit, BCM_STK_MODMAP_GET,
                                    my_modid, port, &modid_out, &port_out));
        /* root node */
        port_info = &mmu_info->sched_node[port_out];

        if (port_info->gport >= 0) {
            _bcm_kt2_cosq_gport_traverse(unit, port_info->gport, cb, user_data);
        }
      }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_mapping_set
 * Purpose:
 *     Determine which COS queue a given priority currently maps to.
 * Parameters:
 *     unit     - (IN) unit number
 *     gport    - (IN) queue group GPORT identifier
 *     priority - (IN) priority value to map
 *     cosq     - (IN) COS queue to map to
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_kt2_cosq_mapping_set(int unit, bcm_port_t gport, bcm_cos_t priority,
                        bcm_cos_queue_t cosq)
{ 
    bcm_pbmp_t ports;
    bcm_port_t local_port;

    if (gport == -1) {    /* all ports */
        BCM_PBMP_ASSIGN(ports, PBMP_ALL(unit));
    } else {
        return _bcm_kt2_cosq_mapping_set_regular(unit, gport, priority, cosq);
    }

    PBMP_ITER(ports, local_port) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_mapping_set_regular(unit, local_port, priority, 
                                               cosq));

    }    

    return BCM_E_NONE;
}


/*
 * Function:
 *     bcm_kt2_cosq_mapping_get
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
bcm_kt2_cosq_mapping_get(int unit, bcm_port_t gport, bcm_cos_t priority,
                        bcm_cos_queue_t *cosq)
{
    _bcm_kt2_mmu_info_t *mmu_info;
    bcm_port_t port;
    int numq;
    bcm_cos_queue_t hw_cosq;
    soc_field_t field = COSf;
    int index;
    cos_map_sel_entry_t cos_map_sel_entry;
    void *entry_p;
    bcm_pbmp_t ports;

    if (priority < 0 || priority >= 16) {
        return BCM_E_PARAM;
    }

    if (gport == -1) {
        BCM_PBMP_ASSIGN(ports, PBMP_ALL(unit));
        PBMP_ITER(ports, port) {
            BCM_IF_ERROR_RETURN
                (READ_COS_MAP_SELm(unit, MEM_BLOCK_ANY, port, 
                                   &cos_map_sel_entry));
            index = soc_mem_field32_get(unit, COS_MAP_SELm, 
                                        &cos_map_sel_entry,
                                        SELECTf);
            index *= 16;

            entry_p = SOC_PROFILE_MEM_ENTRY(unit, _bcm_kt2_cos_map_profile[unit],
                                            port_cos_map_entry_t *,
                                            index + priority);
            *cosq = soc_mem_field32_get(unit, PORT_COS_MAPm, entry_p, field);

            return BCM_E_NONE;
        }
    }
    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_index_resolve
            (unit, gport, -1, _BCM_KT2_COSQ_INDEX_STYLE_UCAST_QUEUE,
             &port, NULL, NULL));
    } else if(_BCM_KT2_GPORT_IS_LINKPHY_OR_SUBTAG_SUBPORT_GPORT (unit, gport)) {
        port = _BCM_KT2_SUBPORT_PORT_ID_GET(gport);
    } else { 
        if (BCM_GPORT_IS_SET(gport)) {
            BCM_IF_ERROR_RETURN(
                bcm_esw_port_local_get(unit, gport, &port));
        } else {
            port = gport;
        }

        if (!SOC_PORT_VALID(unit, port) || !IS_ALL_PORT(unit, port)) {
            return BCM_E_PORT;
        }
    } 

    mmu_info = _bcm_kt2_mmu_info[unit];
    numq = mmu_info->port[port].q_limit - 
           mmu_info->port[port].q_offset;
    
    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
        field = (numq <= 8) ? COSf : HG_COSf;
    }

    BCM_IF_ERROR_RETURN
        (READ_COS_MAP_SELm(unit, MEM_BLOCK_ANY, port, &cos_map_sel_entry));
    index = soc_mem_field32_get(unit, COS_MAP_SELm, &cos_map_sel_entry,
                                SELECTf);
    index *= 16;

    entry_p = SOC_PROFILE_MEM_ENTRY(unit, _bcm_kt2_cos_map_profile[unit],
                                    port_cos_map_entry_t *,
                                    index + priority);
    hw_cosq = soc_mem_field32_get(unit, PORT_COS_MAPm, entry_p, field);

    if (hw_cosq >= numq) {
        return BCM_E_NOT_FOUND;
    }
    *cosq = hw_cosq;

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_port_sched_set
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
 * Notes:
 *     For non-ETS style scheduler (CPU port) only.
 */
int
bcm_kt2_cosq_port_sched_set(int unit, bcm_pbmp_t pbm,
                           int mode, const int *weights, int delay)
{
    bcm_port_t port;
    int num_cos;
    int num_weights;

    num_cos = soc_property_get(unit, spn_BCM_NUM_COS, BCM_COS_DEFAULT);
    
    BCM_PBMP_ITER(pbm, port) {
        num_weights = IS_CPU_PORT(unit, port) ? BCM_COS_COUNT : num_cos;
            
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_port_sched_set(unit, port, 0, mode, num_weights,
                                         (int *)weights));
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_port_sched_get
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
 * Notes:
 *     For non-ETS style scheduler (CPU port) only.
 */
int
bcm_kt2_cosq_port_sched_get(int unit, bcm_pbmp_t pbm,
                           int *mode, int *weights, int *delay)
{
    bcm_port_t port;
    int num_cos;
    int num_weights;
    
    num_cos = soc_property_get(unit, spn_BCM_NUM_COS, BCM_COS_DEFAULT);
    
    BCM_PBMP_ITER(pbm, port) {
        num_weights = IS_CPU_PORT(unit, port) ? BCM_COS_COUNT : num_cos;
        
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_port_sched_get(unit, port, 0, mode, num_weights, 
                                         weights));        
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_sched_weight_max_get
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
bcm_kt2_cosq_sched_weight_max_get(int unit, int mode, int *weight_max)
{
    switch (mode) {
    case BCM_COSQ_STRICT:
        *weight_max = BCM_COSQ_WEIGHT_STRICT;
        break;
    case BCM_COSQ_ROUND_ROBIN:
        *weight_max = BCM_COSQ_WEIGHT_MIN;
        break;
    case BCM_COSQ_WEIGHTED_ROUND_ROBIN:
    case BCM_COSQ_DEFICIT_ROUND_ROBIN:
        *weight_max =
            (1 << soc_mem_field_length(unit, LLS_L0_CHILD_WEIGHT_CFG_CNTm, 
                                       C_WEIGHTf)) - 1;
        break;
    default:
        return BCM_E_PARAM;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_bandwidth_set
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
bcm_kt2_cosq_port_bandwidth_set(int unit, bcm_port_t port,
                               bcm_cos_queue_t cosq,
                               uint32 min_quantum, uint32 max_quantum,
                               uint32 burst_quantum, uint32 flags)
{
    if (cosq < 0) {
        return BCM_E_PARAM;
    }

    return _bcm_kt2_cosq_bucket_set(unit, port, cosq, min_quantum,
                                   max_quantum, min_quantum, 
                                   burst_quantum, flags);
}

/*
 * Function:
 *     bcm_kt2_cosq_bandwidth_get
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
bcm_kt2_cosq_port_bandwidth_get(int unit, bcm_port_t port,
                               bcm_cos_queue_t cosq,
                               uint32 *min_quantum, uint32 *max_quantum,
                               uint32 *burst_quantum, uint32 *flags)
{

    if (cosq < 0) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN(_bcm_kt2_cosq_bucket_get(unit, port, cosq, min_quantum,
                                                max_quantum, burst_quantum,
                                                burst_quantum, *flags));
    *flags = 0;

    return BCM_E_NONE;
}

int
bcm_kt2_cosq_port_pps_set(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                         int pps)
{
    uint32 min, max, burst_min, burst_max;

    if (!IS_CPU_PORT(unit, port)) {
        return BCM_E_PORT;
    } else if (cosq < 0 || cosq >= NUM_CPU_COSQ(unit)) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_bucket_get(unit, port, cosq, &min, &max, &burst_min,
                                 &burst_max, _BCM_XGS_METER_FLAG_PACKET_MODE));

    return _bcm_kt2_cosq_bucket_set(unit, port, cosq, min, pps, burst_min,
                                   burst_max, _BCM_XGS_METER_FLAG_PACKET_MODE);
}

int
bcm_kt2_cosq_port_pps_get(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                         int *pps)
{
    uint32 min, max, burst_min, burst_max;

    if (!IS_CPU_PORT(unit, port)) {
        return BCM_E_PORT;
    } else if (cosq < 0 || cosq >= NUM_CPU_COSQ(unit)) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_bucket_get(unit, port, cosq, &min, &max, &burst_min,
                                 &burst_max, _BCM_XGS_METER_FLAG_PACKET_MODE));
    *pps = max;

    return BCM_E_NONE;
}

int
bcm_kt2_cosq_port_burst_set(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                           int burst)
{
    uint32 min, max, cur_burst_min, cur_burst_max;

    if (!IS_CPU_PORT(unit, port)) {
        return BCM_E_PORT;
    } else if (cosq < 0 || cosq >= NUM_CPU_COSQ(unit)) {
        return BCM_E_PARAM;
    }

    /* Get the current PPS and BURST settings */
    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_bucket_get(unit, port, cosq, &min, &max, &cur_burst_min,
                             &cur_burst_max, _BCM_XGS_METER_FLAG_PACKET_MODE));

    /* Replace the current BURST setting, keep PPS the same */
    return _bcm_kt2_cosq_bucket_set(unit, port, cosq, min, max, cur_burst_min,
                               burst, _BCM_XGS_METER_FLAG_PACKET_MODE);
}

int
bcm_kt2_cosq_port_burst_get(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                           int *burst)
{
    uint32 min, max, cur_burst_min, cur_burst_max;

    if (!IS_CPU_PORT(unit, port)) {
        return BCM_E_PORT;
    } else if (cosq < 0 || cosq >= NUM_CPU_COSQ(unit)) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_bucket_get(unit, port, cosq, &min, &max, &cur_burst_min,
                              &cur_burst_max, _BCM_XGS_METER_FLAG_PACKET_MODE));
    *burst = cur_burst_max;

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_gport_sched_set
 * Purpose:
 *     Configure COS queue scheduler setting
 * Parameters:
 *      unit   - (IN) unit number
 *      gport  - (IN) GPORT identifier
 *      cosq   - (IN) COS queue to configure, -1 for all COS queues
 *      mode   - (IN) Scheduling mode, one of BCM_COSQ_xxx
 *      weight - (IN) queue weight
 * Returns:
 *      BCM_E_XXX
 */
int
bcm_kt2_cosq_gport_sched_set(int unit, bcm_gport_t gport,
                            bcm_cos_queue_t cosq, int mode, int weight)
{
    int rv, numq, i, count;

    if (cosq == -1) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_index_resolve(unit, gport, cosq,
                                        _BCM_KT2_COSQ_INDEX_STYLE_SCHEDULER,
                                        NULL, NULL, &numq));
        cosq = 0;
    } else {
        numq = 1;
    }

    count = 0;
    for (i = 0; i < numq; i++) {
        rv = _bcm_kt2_cosq_sched_set(unit, gport, cosq + i, mode, 1, &weight);
        if (rv == BCM_E_NOT_FOUND) {
            continue;
        } else if (BCM_FAILURE(rv)) {
            return rv;
        } else {
            count++;
        }
    }

    return count > 0 ? BCM_E_NONE : BCM_E_NOT_FOUND;
}

/*
 * Function:
 *     bcm_kt2_cosq_gport_sched_get
 * Purpose:
 *     Get COS queue scheduler setting
 * Parameters:
 *     unit   - (IN) unit number
 *     gport  - (IN) GPORT identifier
 *     cosq   - (IN) COS queue to get, -1 for any queue
 *     mode   - (OUT) Scheduling mode, one of BCM_COSQ_xxx
 *     weight - (OUT) queue weight
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_kt2_cosq_gport_sched_get(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq,
                            int *mode, int *weight)
{
    int rv, numq, i;

    if (cosq == -1) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_index_resolve(unit, gport, cosq,
                                        _BCM_KT2_COSQ_INDEX_STYLE_SCHEDULER,
                                        NULL, NULL, &numq));
        cosq = 0;
    } else {
        numq = 1;
    }

    for (i = 0; i < numq; i++) {
        rv = _bcm_kt2_cosq_sched_get(unit, gport, cosq + i, mode, 1, weight);
        if (rv == BCM_E_NOT_FOUND) {
            continue;
        } else {
            return rv;
        }
    }

    return BCM_E_NOT_FOUND;
}

/*
 * Function:
 *     bcm_kt2_cosq_gport_bandwidth_set
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
bcm_kt2_cosq_gport_bandwidth_set(int unit, bcm_gport_t gport,
                                bcm_cos_queue_t cosq, uint32 kbits_sec_min,
                                uint32 kbits_sec_max, uint32 flags)
{
    int numq, i;

    if (cosq == -1) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_index_resolve(unit, gport, cosq,
                                        _BCM_KT2_COSQ_INDEX_STYLE_BUCKET,
                                        NULL, NULL, &numq));
        cosq = 0;
    } else {
        numq = 1;
    }

    for (i = 0; i < numq; i++) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_bucket_set(unit, gport, cosq + i, kbits_sec_min,
                                      kbits_sec_max, kbits_sec_min,
                                      kbits_sec_max, flags));
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_gport_bandwidth_get
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
bcm_kt2_cosq_gport_bandwidth_get(int unit, bcm_gport_t gport,
                                bcm_cos_queue_t cosq, uint32 *kbits_sec_min,
                                uint32 *kbits_sec_max, uint32 *flags)
{
    uint32 burst_min, burst_max;

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_bucket_get(unit, gport, 
                                  cosq == -1 ? 0 : cosq,
                                  kbits_sec_min, kbits_sec_max,
                                  &burst_min, &burst_max, *flags));
    
    *flags = 0;

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_gport_bandwidth_burst_set
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
bcm_kt2_cosq_gport_bandwidth_burst_set(int unit, bcm_gport_t gport,
                                      bcm_cos_queue_t cosq,
                                      uint32 kbits_burst_min,
                                      uint32 kbits_burst_max)
{
    int numq, i;
    uint32 kbits_sec_min, kbits_sec_max;
    uint32 kbits_sec_burst_min, kbits_sec_burst_max;

    if (cosq == -1) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_index_resolve(unit, gport, cosq,
                                        _BCM_KT2_COSQ_INDEX_STYLE_BUCKET,
                                        NULL, NULL, &numq));
        cosq = 0;
    } else {
        numq = 1;
    }

    for (i = 0; i < numq; i++) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_bucket_get(unit, gport, cosq + i, &kbits_sec_min,
                                     &kbits_sec_max, &kbits_sec_burst_min, 
                                     &kbits_sec_burst_max, 0));
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_bucket_set(unit, gport, cosq + i, kbits_sec_min,
                                     kbits_sec_max, kbits_burst_min, 
                                     kbits_burst_max, 
                                     _BCM_XGS_METER_FLAG_NON_BURST_CORRECTION));
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_gport_bandwidth_burst_get
 * Purpose:
 *     Get COS queue bandwidth burst setting
 * Parameters:
 *     unit        - (IN) unit number
 *     gport       - (IN) GPORT identifier
 *     cosq        - (IN) COS queue to get, -1 for any queue
 *     kbits_burst - (OUT) maximum burst, kbits
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_kt2_cosq_gport_bandwidth_burst_get(int unit, bcm_gport_t gport,
                                       bcm_cos_queue_t cosq,
                                       uint32 *kbits_burst_min,
                                       uint32 *kbits_burst_max)
{
    uint32 kbits_sec_min, kbits_sec_max;

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_bucket_get(unit, gport, cosq == -1 ? 0 : cosq,
                                 &kbits_sec_min, &kbits_sec_max, 
                                 kbits_burst_min, kbits_burst_max, 0));

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_gport_discard_set
 * Purpose:
 *     Configure gport WRED setting
 * Parameters:
 *     unit    - (IN) unit number
 *     port    - (IN) GPORT identifier
 *     cosq    - (IN) COS queue to configure, -1 for all COS queues
 *     discard - (IN) discard settings
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_kt2_cosq_gport_discard_set(int unit, bcm_gport_t gport,
                               bcm_cos_queue_t cosq,
                               bcm_cosq_gport_discard_t *discard)
{
    int numq, i;
    uint32 min_thresh, max_thresh;
    int cell_size, cell_field_max;

    if (discard == NULL ||
        discard->gain < 0 || discard->gain > 15 ||
        discard->drop_probability < 0 || discard->drop_probability > 100) {
        return BCM_E_PARAM;
    }

    if (cosq < -1) {
        return BCM_E_PARAM;
    }

    cell_size = _BCM_KT2_COSQ_CELLSIZE;
    cell_field_max = KT2_CELL_FIELD_MAX;

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
    } else { /* BCM_COSQ_DISCARD_PACKETS */
        if ((min_thresh > KT2_CELL_FIELD_MAX) ||
            (max_thresh > KT2_CELL_FIELD_MAX)) {
            return BCM_E_PARAM;
        }
    }

    if (cosq == -1) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_index_resolve(unit, gport, cosq,
                                        _BCM_KT2_COSQ_INDEX_STYLE_WRED,
                                        NULL, NULL, &numq));
        cosq = 0;
    } else {
        numq = 1;
    }

    for (i = 0; i < numq; i++) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_wred_set(unit, gport, cosq + i, discard->flags,
                                   min_thresh, max_thresh,
                                   discard->drop_probability, discard->gain,
                                   FALSE));
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_gport_discard_get
 * Purpose:
 *     Get port WRED setting
 * Parameters:
 *     unit    - (IN) unit number
 *     port    - (IN) GPORT identifier
 *     cosq    - (IN) COS queue to get, -1 for any queue
 *     discard - (OUT) discard settings
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_kt2_cosq_gport_discard_get(int unit, bcm_gport_t gport,
                              bcm_cos_queue_t cosq,
                              bcm_cosq_gport_discard_t *discard)
{
    uint32 min_thresh, max_thresh;

    if (discard == NULL) {
        return BCM_E_PARAM;
    }

    if (cosq < -1) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_wred_get(unit, gport, cosq == -1 ? 0 : cosq,
                               &discard->flags, &min_thresh, &max_thresh,
                               &discard->drop_probability, &discard->gain));

    /* Convert number of cells to number of bytes */
    if (discard->flags & BCM_COSQ_DISCARD_BYTES) {
        discard->min_thresh = min_thresh * _BCM_KT2_COSQ_CELLSIZE;
        discard->max_thresh = max_thresh * _BCM_KT2_COSQ_CELLSIZE;
    } else {
        discard->min_thresh = min_thresh;
        discard->max_thresh = max_thresh;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_discard_set
 * Purpose:
 *     Get port WRED setting
 * Parameters:
 *     unit    - (IN) unit number
 *     flags   - (IN) flags
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_kt2_cosq_discard_set(int unit, uint32 flags)
{
    bcm_port_t port;
    bcm_cos_queue_t cosq;
    int numq;

    flags &= ~(BCM_COSQ_DISCARD_NONTCP | BCM_COSQ_DISCARD_COLOR_ALL);

    PBMP_PORT_ITER(unit, port) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_index_resolve(unit, port, -1,
                                        _BCM_KT2_COSQ_INDEX_STYLE_WRED, NULL,
                                        NULL, &numq));
        for (cosq = 0; cosq < numq; cosq++) {
            BCM_IF_ERROR_RETURN
                (_bcm_kt2_cosq_wred_set(unit, port, cosq, flags, 0, 0, 0, 0,
                                       FALSE));
        }
    }

    return BCM_E_NONE;

}

/*
 * Function:
 *     bcm_kt2_cosq_discard_get
 * Purpose:
 *     Get port WRED setting
 * Parameters:
 *     unit    - (IN) unit number
 *     flags   - (OUT) flags
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_kt2_cosq_discard_get(int unit, uint32 *flags)
{
    bcm_port_t port;

    PBMP_PORT_ITER(unit, port) {
        *flags = 0;
        /* use setting from hardware cosq index 0 of the first port */
        return _bcm_kt2_cosq_wred_get(unit, port, 0, flags, NULL, NULL, NULL,
                                     NULL);
    }

    return BCM_E_NOT_FOUND;

}

/*
 * Function:
 *     bcm_kt2_cosq_discard_port_set
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
 */
int
bcm_kt2_cosq_discard_port_set(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                             uint32 color, int drop_start, int drop_slope,
                             int average_time)
{
    return SOC_E_UNAVAIL;
}

/*
 * Function:
 *     bcm_kt2_cosq_discard_port_get
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
 */
int
bcm_kt2_cosq_discard_port_get(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                             uint32 color, int *drop_start, int *drop_slope,
                             int *average_time)
{
    return SOC_E_UNAVAIL;
}

int
bcm_kt2_cosq_stat_set(int unit, bcm_gport_t port, bcm_cos_queue_t cosq,
                     bcm_cosq_stat_t stat, uint64 value)
{
    bcm_port_t local_port;
    int startq, numq, i;
    uint64 value64;
    _bcm_kt2_mmu_info_t *mmu_info;
    soc_counter_non_dma_id_t dma_id;

    if (BCM_GPORT_IS_SCHEDULER(port) && cosq != BCM_COS_INVALID) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_index_resolve
        (unit, port, cosq, _BCM_KT2_COSQ_INDEX_STYLE_UCAST_QUEUE,
         &local_port, &startq, &numq));

    mmu_info = _bcm_kt2_mmu_info[unit];
    if (BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port) ||
        startq >= mmu_info->num_base_queues) {
        /* pass port as queue number */
        local_port = startq;
        startq = 0;
    }

    switch (stat) {
    case bcmCosqStatDroppedPackets:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT;
        break;
    case bcmCosqStatDroppedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE;
        break;
    case bcmCosqStatRedCongestionDroppedPackets:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT_RED;
        break;
    case bcmCosqStatOutPackets:
        dma_id = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_PKT;
        break;
    case bcmCosqStatOutBytes:
        dma_id = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_BYTE; 
        break;
    default:
        return BCM_E_PARAM;
    }

    if (!BCM_GPORT_IS_SCHEDULER(port)) {      
        BCM_IF_ERROR_RETURN
             (soc_counter_set(unit, local_port, dma_id, startq, value));
    } else {
        COMPILER_64_ZERO(value64);
        for (i = 0; i < numq; i++) {
            BCM_IF_ERROR_RETURN
                (soc_counter_set(unit, local_port,dma_id, 
                                 startq + i, value64));
        }
    }

    return BCM_E_NONE;
}

int
bcm_kt2_cosq_stat_get(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                     bcm_cosq_stat_t stat, int sync_mode, uint64 *value)
{
    bcm_port_t local_port;
    int startq, numq, i;
    uint64 sum, value64;
    _bcm_kt2_mmu_info_t *mmu_info;
    soc_counter_non_dma_id_t dma_id;
    int (*counter_get) (int , soc_port_t , soc_reg_t , int , uint64 *);

    if (value == NULL) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_SCHEDULER(port) && cosq != BCM_COS_INVALID) {
        return BCM_E_PARAM;
    }    

    /*
     * if sync-mode is set, update the software cached counter value, 
     * with the hardware count and then retrieve the count.
     * else return the software cache counter value.
     */
    counter_get = (sync_mode == 1)? soc_counter_sync_get: soc_counter_get;

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_index_resolve
        (unit, port, cosq, _BCM_KT2_COSQ_INDEX_STYLE_UCAST_QUEUE,
         &local_port, &startq, &numq));

    mmu_info = _bcm_kt2_mmu_info[unit];
    if (BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port) ||
        startq >= mmu_info->num_base_queues) {
        /* pass port as queue number */
        local_port = startq;
        startq = 0;
    }

    switch (stat) {
    case bcmCosqStatDroppedPackets:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT;
        break;
    case bcmCosqStatDroppedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE;
        break;
    case bcmCosqStatRedCongestionDroppedPackets:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT_RED;
        break;
    case bcmCosqStatOutPackets:
        dma_id = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_PKT;
        break;
    case bcmCosqStatOutBytes:
        dma_id = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_BYTE;
        break;
    default:
        return BCM_E_PARAM;
    }

    if (!BCM_GPORT_IS_SCHEDULER(port)) { 
        BCM_IF_ERROR_RETURN
             (counter_get(unit, local_port, dma_id, startq, value));
    } else {
        COMPILER_64_ZERO(sum);
        for (i = 0; i < numq; i++) {
            BCM_IF_ERROR_RETURN
                (counter_get(unit, local_port, dma_id,
                             startq + i, &value64));
            COMPILER_64_ADD_64(sum, value64);
        }
        *value = sum;
    }

    return BCM_E_NONE;
}

int
bcm_kt2_cosq_gport_stat_set(int unit, bcm_gport_t port, bcm_cos_queue_t cosq,
                                 bcm_cosq_gport_stats_t stat, uint64 value)
{
    bcm_port_t local_port;
    int startq, numq, i;
    uint64 value64;
    _bcm_kt2_mmu_info_t *mmu_info;
    soc_counter_non_dma_id_t dma_id;

    if (BCM_GPORT_IS_SCHEDULER(port) && cosq != BCM_COS_INVALID) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_index_resolve
        (unit, port, cosq, _BCM_KT2_COSQ_INDEX_STYLE_UCAST_QUEUE,
         &local_port, &startq, &numq));

    mmu_info = _bcm_kt2_mmu_info[unit];
    if (BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port) ||
        startq >= mmu_info->num_base_queues) {
        /* pass port as queue number */
        local_port = startq;
        startq = 0;
    }

    switch (stat) {
    case bcmCosqGportGreenAcceptedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_PKT_GREEN;
        break;
    case bcmCosqGportGreenAcceptedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_BYTE_GREEN;
        break;
    case bcmCosqGportYellowAcceptedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_PKT_YELLOW;
        break;
    case bcmCosqGportYellowAcceptedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_BYTE_YELLOW;
        break;    
    case bcmCosqGportRedAcceptedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_PKT_RED;
        break;
    case bcmCosqGportRedAcceptedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_BYTE_RED;
        break;        
    case bcmCosqGportReceivedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_PKT;
        break;
    case bcmCosqGportReceivedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_BYTE;
        break;
    case bcmCosqGportGreenDroppedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT_GREEN;
        break;
    case bcmCosqGportGreenDroppedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE_GREEN;
        break;    
    case bcmCosqGportYellowDroppedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT_YELLOW;
        break;
    case bcmCosqGportYellowDroppedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE_YELLOW;
        break;    
    case bcmCosqGportRedDroppedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT_RED;
        break;
    case bcmCosqGportRedDroppedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE_RED;
        break;    
    case bcmCosqGportDroppedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT;
        break;
    case bcmCosqGportDroppedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE;
        break;
    case bcmCosqGportOutPkts:
        dma_id = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_PKT;
        break;
    case bcmCosqGportOutBytes:
        dma_id = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_BYTE;
        break;    
    default:
        return BCM_E_PARAM;
    }

    if (!BCM_GPORT_IS_SCHEDULER(port)) {      
        BCM_IF_ERROR_RETURN
             (soc_counter_set(unit, local_port, dma_id, startq, value));
    } else {
        COMPILER_64_ZERO(value64);
        for (i = 0; i < numq; i++) {
            BCM_IF_ERROR_RETURN
                (soc_counter_set(unit, local_port, dma_id, startq + i, 
                                 value64));
        }
    }    

    return BCM_E_NONE;
}

int
bcm_kt2_cosq_gport_stat_get(int unit, bcm_port_t port, bcm_cos_queue_t cosq,
                                 bcm_cosq_gport_stats_t stat, uint64 *value)
{
    bcm_port_t local_port;
    int startq, numq, i;
    uint64 sum, value64;
    _bcm_kt2_mmu_info_t *mmu_info;
    soc_counter_non_dma_id_t dma_id;
    
    if (value == NULL) {
        return BCM_E_PARAM;
    }

    if (BCM_GPORT_IS_SCHEDULER(port) && cosq != BCM_COS_INVALID) {
        return BCM_E_PARAM;
    }    

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_index_resolve
        (unit, port, cosq, _BCM_KT2_COSQ_INDEX_STYLE_UCAST_QUEUE,
         &local_port, &startq, &numq));

    mmu_info = _bcm_kt2_mmu_info[unit];
    if (BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(port) ||
        startq >= mmu_info->num_base_queues) {
        /* pass port as queue number */
        local_port = startq;
        startq = 0;
    }

    switch (stat) {
    case bcmCosqGportGreenAcceptedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_PKT_GREEN;
        break;
    case bcmCosqGportGreenAcceptedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_BYTE_GREEN;
        break;
    case bcmCosqGportYellowAcceptedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_PKT_YELLOW;
        break;
    case bcmCosqGportYellowAcceptedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_BYTE_YELLOW;
        break;    
    case bcmCosqGportRedAcceptedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_PKT_RED;
        break;
    case bcmCosqGportRedAcceptedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_BYTE_RED;
        break;        
    case bcmCosqGportReceivedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_PKT;
        break;
    case bcmCosqGportReceivedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_ACCEPT_BYTE;
        break;
    case bcmCosqGportGreenDroppedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT_GREEN;
        break;
    case bcmCosqGportGreenDroppedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE_GREEN;
        break;    
    case bcmCosqGportYellowDroppedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT_YELLOW;
        break;
    case bcmCosqGportYellowDroppedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE_YELLOW;
        break;    
    case bcmCosqGportRedDroppedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT_RED;
        break;
    case bcmCosqGportRedDroppedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE_RED;
        break;    
    case bcmCosqGportDroppedPkts:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_PKT;
        break;
    case bcmCosqGportDroppedBytes:
        dma_id = SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE;
        break;
    case bcmCosqGportOutPkts:
        dma_id = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_PKT;
        break;
    case bcmCosqGportOutBytes:
        dma_id = SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_BYTE;
        break;    
    default:
        return BCM_E_PARAM;
    }

    if (!BCM_GPORT_IS_SCHEDULER(port)) { 
        BCM_IF_ERROR_RETURN
             (soc_counter_get(unit, local_port, dma_id, startq, value)); 
    } else {
        COMPILER_64_ZERO(sum);
        for (i = 0; i < numq; i++) {
            BCM_IF_ERROR_RETURN
                (soc_counter_get(unit, local_port, dma_id, startq + i, 
                                 &value64));
            COMPILER_64_ADD_64(sum, value64);
        }
        *value = sum;
    }

    return BCM_E_NONE;
}

int bcm_kt2_cosq_port_get(int unit, int queue_id, bcm_port_t *port) 
{
    _bcm_kt2_mmu_info_t *mmu_info;
    lls_l0_parent_entry_t l0_parent;
    lls_l1_parent_entry_t l1_parent;
    lls_l2_parent_entry_t l2_parent;
    int index;

    if (_bcm_kt2_mmu_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    mmu_info = _bcm_kt2_mmu_info[unit];
    
    if (queue_id >= mmu_info->num_queues) {
        return BCM_E_PARAM;
    }    

    SOC_IF_ERROR_RETURN
        (READ_LLS_L2_PARENTm(unit, MEM_BLOCK_ALL, queue_id, &l2_parent));
    index = soc_mem_field32_get(unit, LLS_L2_PARENTm, &l2_parent, C_PARENTf);

    SOC_IF_ERROR_RETURN
            (READ_LLS_L1_PARENTm(unit, MEM_BLOCK_ALL, index, &l1_parent));
    index = soc_mem_field32_get(unit, LLS_L1_PARENTm, &l1_parent, C_PARENTf);

    SOC_IF_ERROR_RETURN
            (READ_LLS_L0_PARENTm(unit, MEM_BLOCK_ALL, index, &l0_parent));
    
    *port = soc_mem_field32_get(unit, LLS_L0_PARENTm, &l0_parent, C_PARENTf);;

    return BCM_E_NONE;
}

#ifdef BCM_WARM_BOOT_SUPPORT

#define BCM_WB_VERSION_1_0                     SOC_SCACHE_VERSION(1,0)
#define BCM_WB_DEFAULT_VERSION                 BCM_WB_VERSION_1_0

#define _BCM_KT2_COSQ_WB_END_NODE_VALUE         0xffffffff

#define _BCM_KT2_COSQ_WB_NODE_COSQ_MASK         0x3f    /* LW-1 [5:0] */
#define _BCM_KT2_COSQ_WB_NODE_COSQ_SHIFT        0
#define _BCM_KT2_COSQ_WB_NODE_NUMQ_MASK         0x3f    /* LW-1 [11:6] */
#define _BCM_KT2_COSQ_WB_NODE_NUMQ_SHIFT        6
#define _BCM_KT2_COSQ_WB_NODE_HW_INDEX_MASK     0xfff   /* LW-1 [23:12] */
#define _BCM_KT2_COSQ_WB_NODE_HW_INDEX_SHIFT    12
#define _BCM_KT2_COSQ_WB_NODE_LEVEL_MASK        0x7     /* LW-1 [26:24] */
#define _BCM_KT2_COSQ_WB_NODE_LEVEL_SHIFT       24
#define _BCM_KT2_COSQ_WB_NODE_WRR_MASK          0x1     /* LW-1 [27] */
#define _BCM_KT2_COSQ_WB_NODE_WRR_SHIFT         27
#define _BCM_KT2_COSQ_WB_NODE_SUBPORT_MASK      0x1     /* LW-1 [28] */
#define _BCM_KT2_COSQ_WB_NODE_SUBPORT_SHIFT     28
#define _BCM_KT2_COSQ_WB_NODE_LINKPHY_MASK      0x1     /* LW-1 [29] */
#define _BCM_KT2_COSQ_WB_NODE_LINKPHY_SHIFT     29


#define SET_FIELD(_field, _value)                       \
    (((_value) & _BCM_KT2_COSQ_WB_## _field## _MASK) <<  \
     _BCM_KT2_COSQ_WB_## _field## _SHIFT)
#define GET_FIELD(_field, _byte)                        \
    (((_byte) >> _BCM_KT2_COSQ_WB_## _field## _SHIFT) &  \
     _BCM_KT2_COSQ_WB_## _field## _MASK)

STATIC int
_bcm_kt2_cosq_wb_alloc(int unit)
{
    soc_scache_handle_t scache_handle;
    uint8 *scache_ptr;
    _bcm_kt2_mmu_info_t *mmu_info;
    int rv, alloc_size;

    mmu_info = _bcm_kt2_mmu_info[unit];

    alloc_size = 0;
    alloc_size += sizeof(mmu_info->num_base_queues);
    alloc_size += sizeof(mmu_info->num_ext_queues);
    alloc_size += sizeof(mmu_info->qset_size);
    alloc_size += sizeof(mmu_info->num_queues);
    alloc_size += sizeof(mmu_info->num_nodes);
    alloc_size += sizeof(mmu_info->max_nodes);
    alloc_size += (num_total_schedulers * sizeof(uint32) * 3);
    alloc_size += (num_l2_queues * sizeof(uint32) * 2);
    alloc_size += sizeof(uint32);

    SOC_SCACHE_HANDLE_SET(scache_handle, unit, BCM_MODULE_COSQ, 0);
    rv = _bcm_esw_scache_ptr_get(unit, scache_handle, TRUE, alloc_size,
                                 &scache_ptr, BCM_WB_DEFAULT_VERSION, NULL);
    if (rv == BCM_E_NOT_FOUND) {
        rv = BCM_E_NONE;
    }

    return rv;
}

STATIC uint32
_bcm_kt2_cosq_wb_encode_node(int unit, _bcm_kt2_cosq_node_t *node)
{
    uint32 data = 0;

    data  = SET_FIELD(NODE_COSQ, node->cosq_attached_to);
    data |= SET_FIELD(NODE_NUMQ, node->numq);
    data |= SET_FIELD(NODE_LEVEL, node->level);
    data |= SET_FIELD(NODE_HW_INDEX, node->hw_index);
    data |= SET_FIELD(NODE_WRR, node->wrr_in_use);
    data |= SET_FIELD(NODE_SUBPORT, node->subport_enabled);
    data |= SET_FIELD(NODE_LINKPHY, node->linkphy_enabled);
    
    return data;
}

STATIC int
_bcm_kt2_cosq_update_scache(int unit, uint8 *scache_ptr, 
                            _bcm_kt2_cosq_node_t *node, int *length)
{
    uint8 *ptr;
    uint32 node_data;

    ptr = scache_ptr;

    *(((uint32 *)ptr)) = node->gport;
    ptr += sizeof(uint32);
            
    node_data = _bcm_kt2_cosq_wb_encode_node(unit, node);
    *(((uint32 *)ptr)) = node_data;
    ptr += sizeof(uint32);

    if (node->level != _BCM_KT2_COSQ_NODE_LEVEL_L2) {
        *(((uint32 *)ptr)) = node->base_index;
        ptr += sizeof(uint32);
    }

    *length = ptr - scache_ptr;
    return BCM_E_NONE;
}

STATIC int
_bcm_kt2_cosq_update_l0_tree(int unit, uint8 *scache_ptr, 
                              _bcm_kt2_cosq_node_t *node, 
                             int *length)
{
    uint8 *ptr;
    _bcm_kt2_cosq_node_t *l0_node, *l1_node, *l2_node;
    int inc;

    ptr = scache_ptr;

    for (l0_node = node; l0_node != NULL;
         l0_node = l0_node->sibling) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_update_scache(unit, ptr, l0_node, &inc));
        ptr += inc;

        for (l1_node = l0_node->child; l1_node != NULL;
             l1_node = l1_node->sibling) {
            BCM_IF_ERROR_RETURN
                (_bcm_kt2_cosq_update_scache(unit, ptr, l1_node, &inc));
            ptr += inc;
                
            for (l2_node = l1_node->child; l2_node != NULL;
                 l2_node = l2_node->sibling) {
                BCM_IF_ERROR_RETURN
                    (_bcm_kt2_cosq_update_scache(unit, ptr, l2_node, &inc));
                ptr += inc;
            }
        }
    }

    *length = ptr - scache_ptr;
    return BCM_E_NONE;
}

int
bcm_kt2_cosq_sync(int unit)
{
    soc_scache_handle_t scache_handle;
    uint8 *scache_ptr, *ptr;
    _bcm_kt2_mmu_info_t *mmu_info;
    _bcm_kt2_cosq_node_t *port_node, *l0_node;
    _bcm_kt2_cosq_node_t *s0_node, *s1_node;
    bcm_port_t port;
    int i, inc;

    SOC_SCACHE_HANDLE_SET(scache_handle, unit, BCM_MODULE_COSQ, 0);
    BCM_IF_ERROR_RETURN
        (_bcm_esw_scache_ptr_get(unit, scache_handle, FALSE, 0,
                                 &scache_ptr, BCM_WB_DEFAULT_VERSION, NULL));

    if (_bcm_kt2_mmu_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    mmu_info = _bcm_kt2_mmu_info[unit];

    ptr = scache_ptr;

    *(((uint32 *)ptr)) = mmu_info->num_base_queues;
    ptr += sizeof(uint32);


    *(((uint32 *)ptr)) = mmu_info->num_ext_queues;
    ptr += sizeof(uint32);

    *(((uint32 *)ptr)) = mmu_info->qset_size;
    ptr += sizeof(uint32);

    *(((uint32 *)ptr)) = mmu_info->num_queues;
    ptr += sizeof(uint32);

    *(((uint32 *)ptr)) = mmu_info->num_nodes;
    ptr += sizeof(uint32);

    for (i = 0; i < _BCM_KT2_COSQ_NODE_LEVEL_MAX; i++) {
        *(((uint32 *)ptr)) = mmu_info->max_nodes[i];
        ptr += sizeof(uint32);
    }

    PBMP_ALL_ITER(unit, port) {
        port_node = &mmu_info->sched_node[port];

        if (port_node->cosq_attached_to < 0) {
            continue;
        }

        if (port_node->child != NULL) {
            BCM_IF_ERROR_RETURN
                (_bcm_kt2_cosq_update_scache(unit, ptr, port_node, &inc));
            ptr += inc;
        }

        s0_node = port_node->child;
        if (s0_node != NULL && 
            s0_node->level == _BCM_KT2_COSQ_NODE_LEVEL_S0) {
            for (s0_node = port_node->child; s0_node != NULL;
                 s0_node = s0_node->sibling) {
                BCM_IF_ERROR_RETURN
                    (_bcm_kt2_cosq_update_scache(unit, ptr, s0_node, &inc));
                ptr += inc;

                s1_node = s0_node->child;
                if (s1_node != NULL && 
                    s1_node->level == _BCM_KT2_COSQ_NODE_LEVEL_S1) {
                    for (s1_node = s0_node->child; s1_node != NULL;
                         s1_node = s1_node->sibling) {
                        BCM_IF_ERROR_RETURN
                            (_bcm_kt2_cosq_update_scache(unit, ptr, s1_node, &inc));
                        ptr += inc;
                        for (l0_node = s1_node->child; l0_node != NULL;
                             l0_node = l0_node->sibling) {
                            BCM_IF_ERROR_RETURN
                                (_bcm_kt2_cosq_update_l0_tree(unit, ptr, l0_node, &inc));
                            ptr += inc;
                        }
                    }
                } else {
                    l0_node = s1_node;
                    BCM_IF_ERROR_RETURN
                        (_bcm_kt2_cosq_update_l0_tree(unit, ptr, l0_node, &inc));
                    ptr += inc;
                }
            }
        } else {
            l0_node = port_node->child;
            BCM_IF_ERROR_RETURN
                (_bcm_kt2_cosq_update_l0_tree(unit, ptr, l0_node, &inc));
            ptr += inc;
        }
    }

    *(((uint32 *)ptr)) = _BCM_KT2_COSQ_WB_END_NODE_VALUE;

    return BCM_E_NONE; 
}

int
bcm_kt2_cosq_reinit(int unit)
{
    soc_scache_handle_t scache_handle;
    uint8 *scache_ptr, *ptr;
    _bcm_kt2_mmu_info_t *mmu_info;
    _bcm_kt2_cosq_node_t *port_node = NULL;
    _bcm_kt2_cosq_node_t *queue_node = NULL;
    _bcm_kt2_cosq_node_t *node = NULL;
    _bcm_kt2_cosq_node_t *s0_node = NULL;
    _bcm_kt2_cosq_node_t *s1_node = NULL;
    _bcm_kt2_cosq_node_t *l0_node = NULL;
    _bcm_kt2_cosq_node_t *l1_node = NULL;
    _bcm_kt2_cosq_node_t *tmp_node = NULL;
    int rv, stable_size = 0;
    bcm_port_t port = 0;
    bcm_gport_t gport;
    int i, index, numq;
    uint32 node_data;
    uint32 encap_id;
    int set_index, cosq;
    uint32 entry[SOC_MAX_MEM_WORDS];
    soc_profile_mem_t *profile_mem;
    _bcm_kt2_cosq_list_t *list;
    soc_mem_t mem;
    mmu_wred_queue_config_buffer_entry_t qentry;
    mmu_wred_queue_config_qentry_entry_t qentry1;
    int cpu_hg_index = 0;
    bcm_pbmp_t all_pbmp;

    SOC_IF_ERROR_RETURN(soc_stable_size_get(unit, &stable_size));
    if ((stable_size == 0) || (SOC_WARM_BOOT_SCACHE_IS_LIMITED(unit))) {
        /* COSQ warmboot requires extended scache support i.e. level2 warmboot*/
        return BCM_E_NONE;
    }

    SOC_SCACHE_HANDLE_SET(scache_handle, unit, BCM_MODULE_COSQ, 0);
    rv = _bcm_esw_scache_ptr_get(unit, scache_handle, FALSE, 0, &scache_ptr,
                                 BCM_WB_DEFAULT_VERSION, NULL);
    if (BCM_FAILURE(rv)) {
        return rv;
    }

    if (_bcm_kt2_mmu_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    mmu_info = _bcm_kt2_mmu_info[unit];

    ptr = scache_ptr;

    mmu_info->num_base_queues = *(((uint32 *)ptr));
    ptr += sizeof(uint32);


    mmu_info->num_ext_queues = *(((uint32 *)ptr));
    ptr += sizeof(uint32);

    mmu_info->qset_size = *(((uint32 *)ptr));
    ptr += sizeof(uint32);

    mmu_info->num_queues = *(((uint32 *)ptr));
    ptr += sizeof(uint32);

    mmu_info->num_nodes = *(((uint32 *)ptr));
    ptr += sizeof(uint32);

    for (i = 0; i < _BCM_KT2_COSQ_NODE_LEVEL_MAX; i++) {
        mmu_info->max_nodes[i] = *(((uint32 *)ptr));
        ptr += sizeof(uint32);
    }

    while (*(((uint32 *)ptr)) != _BCM_KT2_COSQ_WB_END_NODE_VALUE) {
        gport = *(((uint32 *)ptr));
        ptr += sizeof(uint32);
        node_data = *(((uint32 *)ptr));
        ptr += sizeof(uint32);
                     
        if (BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(gport)) {
            list = &mmu_info->ext_qlist;
            encap_id = BCM_GPORT_UCAST_SUBSCRIBER_QUEUE_GROUP_QID_GET(gport);
            node = &mmu_info->queue_node[encap_id]; 
            encap_id -= mmu_info->num_base_queues;
            _bcm_kt2_node_index_set(list, encap_id, 1);
        } else if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
            encap_id = BCM_GPORT_UCAST_QUEUE_GROUP_QID_GET(gport);
            node = &mmu_info->queue_node[encap_id]; 
        } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
            encap_id = BCM_GPORT_MCAST_QUEUE_GROUP_QID_GET(gport);
            node = &mmu_info->queue_node[encap_id];        
        } else {
            encap_id = (BCM_GPORT_SCHEDULER_GET(gport) >> 8) & 0x7ff;
            if (encap_id == 0) {
                encap_id = (BCM_GPORT_SCHEDULER_GET(gport) & 0xff);
            }
            list = &mmu_info->sched_list;
            _bcm_kt2_node_index_set(list, encap_id, 1);
            node = &mmu_info->sched_node[encap_id]; 
            node->first_child = 4095;
            node->base_index = *(((uint32 *)ptr));
            ptr += sizeof(uint32);
        }

        node->gport = gport;
        node->numq  = GET_FIELD(NODE_NUMQ, node_data);
        node->cosq_attached_to = GET_FIELD(NODE_COSQ, node_data);
        node->level = GET_FIELD(NODE_LEVEL, node_data);
        node->hw_index = GET_FIELD(NODE_HW_INDEX, node_data);
        node->wrr_in_use = GET_FIELD(NODE_WRR, node_data);
        node->subport_enabled = GET_FIELD(NODE_SUBPORT, node_data);
        node->linkphy_enabled = GET_FIELD(NODE_LINKPHY, node_data);

        switch (node->level) {
            case _BCM_KT2_COSQ_NODE_LEVEL_ROOT:
                port_node = node;
                if (port_node != NULL) {
                    port_node->parent = NULL;
                    port_node->sibling = NULL;
                    port_node->child = NULL;
                }
                break;

            case _BCM_KT2_COSQ_NODE_LEVEL_S0:
                s0_node = node;
                list = &mmu_info->sched_hw_list[_BCM_KT2_COSQ_NODE_LEVEL_S0];
                
                if (port_node != NULL && s0_node != NULL) {
                    s0_node->parent = port_node;
                    s0_node->sibling = port_node->child;
                    
                    port_node->child = s0_node;
                    port_node->num_child++;
                    
                    if (port_node->numq > 0) {
                        if (s0_node->hw_index < port_node->first_child) {
                            port_node->first_child = s0_node->hw_index;
                        }
                        if (port_node->num_child == 1) {
                            _bcm_kt2_node_index_set(list, port_node->base_index, 
                                                    port_node->numq);
                        }
                    } else {
                        _bcm_kt2_node_index_set(list, s0_node->hw_index, 1);
                    }
                }
                break;

            case _BCM_KT2_COSQ_NODE_LEVEL_S1:
                s1_node = node;
                list = &mmu_info->sched_hw_list[_BCM_KT2_COSQ_NODE_LEVEL_S1];
                
                if (s0_node != NULL && s1_node != NULL) {
                    s1_node->parent = s0_node;
                    s1_node->sibling = s0_node->child;
                    
                    s0_node->child = s1_node;
                    s0_node->num_child++;
                    
                    if (s0_node->numq > 0) {
                        if (s1_node->hw_index < s0_node->first_child) {
                            s0_node->first_child = s1_node->hw_index;
                        }
                        if (s0_node->num_child == 1) {
                            _bcm_kt2_node_index_set(list, s0_node->base_index, 
                                                    s0_node->numq);
                        }
                    } else {
                        _bcm_kt2_node_index_set(list, s1_node->hw_index, 1);
                    }
                }
                break;

            case _BCM_KT2_COSQ_NODE_LEVEL_L0:
                l0_node = node;
                list = &mmu_info->sched_hw_list[_BCM_KT2_COSQ_NODE_LEVEL_L0];
               
                tmp_node = (s1_node != NULL) ? 
                            s1_node : ((s0_node != NULL) ? s0_node : port_node);
                if (tmp_node != NULL && l0_node != NULL) {
                    l0_node->parent = tmp_node;
                    l0_node->sibling = tmp_node->child;
                    
                    tmp_node->child = l0_node;
                    tmp_node->num_child++;
                    
                    if (tmp_node->numq > 0) {
                        if (l0_node->hw_index < tmp_node->first_child) {
                            tmp_node->first_child = l0_node->hw_index;
                        }
                        if (tmp_node->num_child == 1) {
                            _bcm_kt2_node_index_set(list, tmp_node->base_index, 
                                                    tmp_node->numq);
                        }
                    } else {
                        _bcm_kt2_node_index_set(list, l0_node->hw_index, 1);
                    }
                }
                break;

            case _BCM_KT2_COSQ_NODE_LEVEL_L1:
                l1_node = node;
                list = &mmu_info->sched_hw_list[_BCM_KT2_COSQ_NODE_LEVEL_L1];
                if (l0_node != NULL && l1_node != NULL) {
                    l1_node->parent = l0_node;
                    l1_node->sibling = l0_node->child;
                    
                    l0_node->child = l1_node;
                    l0_node->num_child++;

                    if (l0_node->numq > 0) {
                        if (l1_node->hw_index < l0_node->first_child) {
                            l0_node->first_child = l1_node->hw_index;
                        }
                        if (l0_node->num_child == 1) {
                            _bcm_kt2_node_index_set(list, l0_node->base_index, 
                                                    l0_node->numq);
                        }
                    } else {
                        _bcm_kt2_node_index_set(list, l1_node->hw_index, 1);
                    }
                }
                break;

            case _BCM_KT2_COSQ_NODE_LEVEL_L2:
                queue_node = node;
                if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport)) {
                    list = &mmu_info->l2_base_qlist;
                } else if (BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport)) {
                    list = &mmu_info->l2_base_mcqlist;
                } else {
                    list = &mmu_info->l2_ext_qlist;
                }
                if (l1_node != NULL && queue_node != NULL) {
                    queue_node->parent = l1_node;
                    queue_node->sibling = l1_node->child;
                    queue_node->hw_index = GET_FIELD(NODE_HW_INDEX, node_data);
                    l1_node->child = queue_node;
                    l1_node->num_child++;

                    if (l1_node->numq > 0) {    
                        if (queue_node->hw_index < l1_node->first_child) {
                            l1_node->first_child = queue_node->hw_index;
                        }
                        if (l1_node->num_child == 1) {
                            _bcm_kt2_node_index_set(list, l1_node->base_index, 
                                                    l1_node->numq);
                        }    
                    } else {
                        _bcm_kt2_node_index_set(list, queue_node->hw_index, 1);
                    }
                }
                break;

            default:
               break;
        }
    }

    /* Update PORT_COS_MAP memory profile reference counter */
    profile_mem = _bcm_kt2_cos_map_profile[unit];
    PBMP_ALL_ITER(unit, port) {
        SOC_IF_ERROR_RETURN
            (soc_mem_read(unit, COS_MAP_SELm, MEM_BLOCK_ALL, port, &entry));
        set_index = soc_mem_field32_get(unit, COS_MAP_SELm, &entry, SELECTf);
        SOC_IF_ERROR_RETURN
            (soc_profile_mem_reference(unit, profile_mem, set_index * 16, 16));
    }

    cpu_hg_index = SOC_INFO(unit).cpu_hg_pp_port_index;

    if (cpu_hg_index != -1) {
        SOC_IF_ERROR_RETURN(soc_mem_read(unit, COS_MAP_SELm, MEM_BLOCK_ALL,
                                         cpu_hg_index, &entry));
        set_index = soc_mem_field32_get(unit, COS_MAP_SELm, &entry, SELECTf);
        SOC_IF_ERROR_RETURN
            (soc_profile_mem_reference(unit, profile_mem, set_index, 1));
    }

    /* Update reference counts for DSCP_TABLE profile*/
    BCM_PBMP_CLEAR(all_pbmp);
    BCM_PBMP_ASSIGN(all_pbmp, PBMP_ALL(unit));
#ifdef BCM_KATANA2_SUPPORT
    if (SOC_IS_KATANA2(unit) && soc_feature(unit, soc_feature_flex_port)) {
        BCM_IF_ERROR_RETURN(_bcm_kt2_flexio_pbmp_update(unit, &all_pbmp));
    }
    if (soc_feature(unit, soc_feature_linkphy_coe) ||
        soc_feature(unit, soc_feature_subtag_coe)) {
        _bcm_kt2_subport_pbmp_update(unit, &all_pbmp);
    }
#endif

    PBMP_ITER(all_pbmp, port) {
        if (IS_LB_PORT(unit, port)) {
            /* Loopback port base starts from 64 */
            continue;
        }
        SOC_IF_ERROR_RETURN(soc_mem_read(unit, PORT_TABm, MEM_BLOCK_ALL,
                                          port, &entry));
        index = soc_mem_field32_get(unit, PORT_TABm, &entry, TRUST_DSCP_PTRf);

        SOC_IF_ERROR_RETURN(_bcm_dscp_table_entry_reference(unit, index * 64,
                                                            64));
    }

    /* Update MMU_WRED_DROP_CURVE_PROFILE_x memory profile reference counter */
    profile_mem = _bcm_kt2_wred_profile[unit];
    PBMP_PORT_ITER(unit, port) {
   
        /*
         * Wred data can be in two tables depending on packet based or byte based
         * we scan both and update the software profile
         */
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_index_resolve(unit, port, -1,
                                        _BCM_KT2_COSQ_INDEX_STYLE_WRED, NULL,
                                        NULL, &numq));
        for (cosq = 0; cosq < numq; cosq++) {
            BCM_IF_ERROR_RETURN
                (_bcm_kt2_cosq_index_resolve(unit, port, cosq,
                                            _BCM_KT2_COSQ_INDEX_STYLE_WRED,
                                            NULL, &index, NULL));
            /* scan byte based wred table */
            mem = MMU_WRED_QUEUE_CONFIG_BUFFERm;
            SOC_IF_ERROR_RETURN
            (soc_mem_read(unit, mem, MEM_BLOCK_ALL, index, &qentry));
            set_index = soc_mem_field32_get(unit, mem, &qentry, PROFILE_INDEXf);
            SOC_IF_ERROR_RETURN
                (soc_profile_mem_reference(unit, profile_mem, set_index, 1));

            /* scan packet based wred table */
            mem=MMU_WRED_QUEUE_CONFIG_QENTRYm; 
            set_index = 0;
            memset(&qentry1, 0, sizeof(mmu_wred_queue_config_qentry_entry_t));

            SOC_IF_ERROR_RETURN
            (soc_mem_read(unit, mem, MEM_BLOCK_ALL, index, &qentry1));
            set_index = soc_mem_field32_get(unit, mem, &qentry1, PROFILE_INDEXf);
            SOC_IF_ERROR_RETURN
                (soc_profile_mem_reference(unit, profile_mem, set_index, 1));
        }
    }

    return BCM_E_NONE;
}

#endif /* BCM_WARM_BOOT_SUPPORT */

int
_bcm_kt2_subport_config_set(int unit)
{
    soc_info_t *si;
    bcm_port_t port;
    bcm_port_t lp_ports = 0;
    int start = 0, end = 0;

    si = &SOC_INFO(unit);

    SOC_PBMP_ITER(si->linkphy_pbm, port) {
        SOC_IF_ERROR_RETURN
            (soc_kt2_cosq_port_coe_linkphy_status_set(unit, port, 1));
        start = si->port_subport_base[port] - KT2_MIN_SUBPORT_INDEX;
        end = start + si->port_num_subport[port] - 1;
        SOC_IF_ERROR_RETURN
            (soc_kt2_cosq_s1_range_set(unit, port, start, end, 1)); 
        lp_ports++;
    }

    if (lp_ports > 0) {
        SOC_IF_ERROR_RETURN
            (soc_kt2_cosq_stream_mapping_set(unit));
    }

    SOC_PBMP_ITER(si->subtag_pbm, port) {
        SOC_IF_ERROR_RETURN
            (soc_kt2_cosq_port_coe_linkphy_status_set(unit, port, 1));
        start = si->port_subport_base[port] - KT2_MIN_SUBPORT_INDEX;
        end = start + si->port_num_subport[port] - 1;
        SOC_IF_ERROR_RETURN
            (soc_kt2_cosq_s1_range_set(unit, port, start, end, 0)); 
    }

    return BCM_E_NONE;
}

int
_bcm_kt2_packing_config_set(int unit, bcm_port_t port, int numq)
{
    bcm_gport_t port_sched, l0_sched;
    bcm_gport_t l1_sched;
    bcm_gport_t cosq;
    int cos, i;
    uint32 type;

    BCM_IF_ERROR_RETURN
        (bcm_kt2_cosq_gport_add(unit, port, 1, 0, &port_sched));
    
    BCM_IF_ERROR_RETURN
        (bcm_kt2_cosq_gport_add(unit, port, numq, 
                                 BCM_COSQ_GPORT_SCHEDULER, 
                                 &l0_sched));
     BCM_IF_ERROR_RETURN
        (bcm_kt2_cosq_gport_attach(unit, l0_sched, port_sched, 0)); 

    for (cos = 0; cos < numq; cos++) {
        BCM_IF_ERROR_RETURN
            (bcm_kt2_cosq_gport_add(unit, port, 2, BCM_COSQ_GPORT_SCHEDULER, 
                                    &l1_sched));
        BCM_IF_ERROR_RETURN
            (bcm_kt2_cosq_gport_attach(unit, l1_sched, l0_sched, cos));
        for (i = 0; i < 2; i++) {
            type = (i == 0) ? BCM_COSQ_GPORT_UCAST_QUEUE_GROUP :
                    BCM_COSQ_GPORT_MCAST_QUEUE_GROUP;
            BCM_IF_ERROR_RETURN
                (bcm_kt2_cosq_gport_add(unit, port, 1, type, &cosq));
            BCM_IF_ERROR_RETURN
                (bcm_kt2_cosq_gport_attach(unit, cosq, l1_sched, i)); 
            BCM_IF_ERROR_RETURN
                (bcm_kt2_cosq_gport_sched_set(unit, l1_sched, i, 
                                          BCM_COSQ_WEIGHTED_ROUND_ROBIN, 1));
        }        
    }

    return BCM_E_NONE;
} 
    
int
_bcm_kt2_port_lls_config_set(int unit, 
                             bcm_port_t port, 
                             int startq, 
                             int total_queues, 
                             int numq)
{
    bcm_gport_t port_sched, l0_sched;
    bcm_gport_t l1_sched;
    bcm_gport_t cosq;
    int cos, i;
    uint32 type;
    uint32 total_opnodes = (total_queues + 7)/8; 

    BCM_IF_ERROR_RETURN
        (bcm_kt2_cosq_gport_add(unit, port, 1, 0, &port_sched));
 
    BCM_IF_ERROR_RETURN
        (bcm_kt2_cosq_gport_add(unit, port, total_opnodes,
                                 BCM_COSQ_GPORT_SCHEDULER, 
                                 &l0_sched));
     BCM_IF_ERROR_RETURN
        (bcm_kt2_cosq_gport_attach(unit, l0_sched, port_sched, 0)); 

    for (i = 0; i < total_opnodes; i++) {
        BCM_IF_ERROR_RETURN
            (bcm_kt2_cosq_gport_add(unit, port, numq, BCM_COSQ_GPORT_SCHEDULER, 
                                    &l1_sched));
        BCM_IF_ERROR_RETURN
            (bcm_kt2_cosq_gport_attach(unit, l1_sched, l0_sched, i));
        for (cos = 0; cos < numq; cos++) {
            type = BCM_COSQ_GPORT_WITH_ID;
            BCM_GPORT_UCAST_QUEUE_GROUP_SYSQID_SET(cosq, port, (startq + cos)); 
            BCM_IF_ERROR_RETURN
                (bcm_kt2_cosq_gport_add(unit, port, 1, type, &cosq));
            BCM_IF_ERROR_RETURN
                (bcm_kt2_cosq_gport_attach(unit, cosq, l1_sched, cos)); 
            BCM_IF_ERROR_RETURN
                (bcm_kt2_cosq_gport_sched_set(unit, l1_sched, cos, 
                                          BCM_COSQ_WEIGHTED_ROUND_ROBIN, 1));
        } 
        startq += 8;
    }

    return BCM_E_NONE;
} 

/*
 * Function:
 *     bcm_kt2_cosq_init
 * Purpose:
 *     Initialize (clear) all COS schedule/mapping state.
 * Parameters:
 *     unit - unit number
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_kt2_cosq_init(int unit)
{
    soc_info_t *si;
    STATIC int _kt2_max_cosq = -1;
    int cosq, numq, alloc_size;
    bcm_port_t port;
    soc_reg_t mem;
    STATIC soc_mem_t wred_mems[6] = {
        MMU_WRED_DROP_CURVE_PROFILE_0m, MMU_WRED_DROP_CURVE_PROFILE_1m,
        MMU_WRED_DROP_CURVE_PROFILE_2m, MMU_WRED_DROP_CURVE_PROFILE_3m,
        MMU_WRED_DROP_CURVE_PROFILE_4m, MMU_WRED_DROP_CURVE_PROFILE_5m
    };
    int entry_words[6];
    mmu_wred_drop_curve_profile_0_entry_t entry_tcp_green;
    mmu_wred_drop_curve_profile_1_entry_t entry_tcp_yellow;
    mmu_wred_drop_curve_profile_2_entry_t entry_tcp_red;
    mmu_wred_drop_curve_profile_3_entry_t entry_nontcp_green;
    mmu_wred_drop_curve_profile_4_entry_t entry_nontcp_yellow;
    mmu_wred_drop_curve_profile_5_entry_t entry_nontcp_red;
    void *entries[6];
    uint32 profile_index, rval;
    _bcm_kt2_mmu_info_t *mmu_info_p;
    int i, cmc;
    bcm_gport_t port_sched, l0_sched, l1_sched, queue;
    uint32 max_threshold, min_threshold;
    int num_queues, num_inputs;
    int num_base_queues = 0;
    int start_pp_port = 0, end_pp_port = 0;

    if (_kt2_max_cosq < 0) {
        _kt2_max_cosq = NUM_COS(unit);
        NUM_COS(unit) = 8;
    }

    if (!SOC_WARM_BOOT(unit)) {    /* Cold Boot */
        BCM_IF_ERROR_RETURN (bcm_kt2_cosq_detach(unit, 0));
    }

    numq = soc_property_get(unit, spn_BCM_NUM_COS, BCM_COS_DEFAULT);

    if (numq < 1) {
        numq = 1;
    } else if (numq > 8) {
        numq = 8;
    }

    /* Create profile for PORT_COS_MAP table */
    if (_bcm_kt2_cos_map_profile[unit] == NULL) {
        _bcm_kt2_cos_map_profile[unit] = sal_alloc(sizeof(soc_profile_mem_t),
                                                  "COS_MAP Profile Mem");
        if (_bcm_kt2_cos_map_profile[unit] == NULL) {
            return BCM_E_MEMORY;
        }
        soc_profile_mem_t_init(_bcm_kt2_cos_map_profile[unit]);
    }
    mem = PORT_COS_MAPm;
    entry_words[0] = sizeof(port_cos_map_entry_t) / sizeof(uint32);
    BCM_IF_ERROR_RETURN(soc_profile_mem_create(unit, &mem, entry_words, 1,
                                               _bcm_kt2_cos_map_profile[unit]));

    /* Create profile for IFP_COS_MAP table. */
    if (_bcm_kt2_ifp_cos_map_profile[unit] == NULL) {
        _bcm_kt2_ifp_cos_map_profile[unit]
            = sal_alloc(sizeof(soc_profile_mem_t), "IFP_COS_MAP Profile Mem");
        if (_bcm_kt2_ifp_cos_map_profile[unit] == NULL) {
            return BCM_E_MEMORY;
        }
        soc_profile_mem_t_init(_bcm_kt2_ifp_cos_map_profile[unit]);
    }
    mem = IFP_COS_MAPm;
    entry_words[0] = sizeof(ifp_cos_map_entry_t) / sizeof(uint32);
    BCM_IF_ERROR_RETURN
        (soc_profile_mem_create(unit, &mem, entry_words, 1,
                                _bcm_kt2_ifp_cos_map_profile[unit]));    

    /* Create profile for MMU_WRED_DROP_CURVE_PROFILE_x tables */
    if (_bcm_kt2_wred_profile[unit] == NULL) {
        _bcm_kt2_wred_profile[unit] = sal_alloc(sizeof(soc_profile_mem_t),
                                               "WRED Profile Mem");
        if (_bcm_kt2_wred_profile[unit] == NULL) {
            return BCM_E_MEMORY;
        }
        soc_profile_mem_t_init(_bcm_kt2_wred_profile[unit]);
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
    BCM_IF_ERROR_RETURN(soc_profile_mem_create(unit, wred_mems, entry_words, 6,
                                               _bcm_kt2_wred_profile[unit]));

    alloc_size = sizeof(_bcm_kt2_mmu_info_t) * 1;
    if (_bcm_kt2_mmu_info[unit] == NULL) {
        _bcm_kt2_mmu_info[unit] =
            sal_alloc(alloc_size, "_bcm_kt2_mmu_info");

        if (_bcm_kt2_mmu_info[unit] == NULL) {
            return BCM_E_MEMORY;
        }
    } else {
        _bcm_kt2_cosq_free_memory(_bcm_kt2_mmu_info[unit]);
    }

    sal_memset(_bcm_kt2_mmu_info[unit], 0, alloc_size);

    mmu_info_p = _bcm_kt2_mmu_info[unit];

    /* ======================== */
    /* Time Being commenting below part under if 0 */
    /* ======================== */
    /*
       if (soc_feature(unit, soc_feature_ddr3)) {
       mmu_info_p->num_queues = soc_mem_index_count(unit, LLS_L2_PARENTm);
       } else {

       mmu_info_p->num_queues = 1024;
       }
     */
    mmu_info_p->num_queues = soc_mem_index_count(unit, LLS_L2_PARENTm);
    
    mmu_info_p->max_nodes[_BCM_KT2_COSQ_NODE_LEVEL_ROOT] = 
                                  soc_mem_index_count(unit, LLS_PORT_CONFIGm);
    mmu_info_p->max_nodes[_BCM_KT2_COSQ_NODE_LEVEL_S0]   = 
                                  soc_mem_index_count(unit, LLS_S1_CONFIGm);
    mmu_info_p->max_nodes[_BCM_KT2_COSQ_NODE_LEVEL_S1]   = 
                                  2 * soc_mem_index_count(unit, LLS_S1_CONFIGm);
    mmu_info_p->max_nodes[_BCM_KT2_COSQ_NODE_LEVEL_L0]   = 
                                  soc_mem_index_count(unit, LLS_L0_CONFIGm);
    mmu_info_p->max_nodes[_BCM_KT2_COSQ_NODE_LEVEL_L1]   = 
                                  soc_mem_index_count(unit, LLS_L1_CONFIGm);
    mmu_info_p->max_nodes[_BCM_KT2_COSQ_NODE_LEVEL_L2]   = 
                                  mmu_info_p->num_queues;

    mmu_info_p->num_nodes = 0;


    for (i= 0; i < _BCM_KT2_COSQ_NODE_LEVEL_L2; i++) {
        mmu_info_p->num_nodes += mmu_info_p->max_nodes[i];
    }

    si = &SOC_INFO(unit);

    num_pp_ports =  si->cpu_hg_pp_port_index;
    num_port_schedulers = si->cpu_hg_index;

    num_s0_schedulers = soc_mem_index_count(unit, LLS_S0_SHAPER_CONFIG_Cm);
    num_s1_schedulers = soc_mem_index_count(unit, LLS_S1_SHAPER_CONFIG_Cm);
    num_l0_schedulers = soc_mem_index_count(unit,LLS_L0_PARENTm);
    num_l1_schedulers = soc_mem_index_count(unit,LLS_L1_PARENTm);;
    num_l2_queues     = soc_mem_index_count(unit,LLS_L2_PARENTm);

    num_total_schedulers = num_port_schedulers + num_s0_schedulers +
                           num_s1_schedulers + num_l0_schedulers + 
                           num_l1_schedulers ;
    LOG_ERROR(BSL_LS_BCM_COSQ,
              (BSL_META_U(unit,
                          "Useful Info:\n"
                          "num_pp_ports:%d num_port_schedulers:%d num_s0_schedulers:%d\n"
                          "num_s1_schedulers:%d num_l0_schedulers:%d num_l1_schedulers:%d\n"
                          "num_l2_queues:%d num_total_schedulers:%d\n"),
               num_pp_ports, num_port_schedulers, num_s0_schedulers,
               num_s1_schedulers, num_l0_schedulers, num_l1_schedulers,
               num_l2_queues, num_total_schedulers));

    mmu_info_p->port = _bcm_kt2_cosq_alloc_clear((SOC_MAX_NUM_PP_PORTS * 
                                            sizeof(_bcm_kt2_cosq_port_info_t)),
                                            "port_info");
    if (mmu_info_p->port == NULL) {
        _bcm_kt2_cosq_free_memory(mmu_info_p);
        return BCM_E_MEMORY;
    }

    mmu_info_p->sched_node = _bcm_kt2_cosq_alloc_clear(((num_total_schedulers) *
                                            sizeof(_bcm_kt2_cosq_node_t)),
                                            "sched_node");
    if (mmu_info_p->sched_node == NULL) {
        _bcm_kt2_cosq_free_memory(mmu_info_p);
        return BCM_E_MEMORY;
    }

    mmu_info_p->queue_node = _bcm_kt2_cosq_alloc_clear(((num_l2_queues) *
                                            sizeof(_bcm_kt2_cosq_node_t)),
                                            "queue_node");
    if (mmu_info_p->queue_node == NULL) {
        _bcm_kt2_cosq_free_memory(mmu_info_p);
        return BCM_E_MEMORY;
    }

    
    for (port = 0; port < (si->lb_port + 1); port++) {  
        mmu_info_p->port[port].q_offset = si->port_uc_cosq_base[port];
        mmu_info_p->port[port].q_limit = si->port_uc_cosq_base[port] + 
                                         si->port_num_uc_cosq[port];
        if (soc_feature(unit, soc_feature_mmu_packing)) {
            mmu_info_p->port[port].mcq_offset = si->port_cosq_base[port];
            mmu_info_p->port[port].mcq_limit = si->port_cosq_base[port] + 
                                               si->port_num_cosq[port];
        }

        SOC_IF_ERROR_RETURN(READ_ING_COS_MODEr(unit, port, &rval));
        soc_reg_field_set(unit, ING_COS_MODEr, &rval, BASE_QUEUE_NUMf,
                      mmu_info_p->port[port].q_offset);
        if (IS_LB_PORT(unit, port)) {
            /* Loopback port base starts from 64 */
            soc_reg_field_set(unit, ING_COS_MODEr, &rval, BASE_QUEUE_NUMf,
                              _BCM_KT2_LB_BASE_QUEUE);
        }
        if (IS_HG_PORT(unit, port)) {
            soc_reg_field_set(unit, ING_COS_MODEr, &rval, COS_MODEf, 2);  
        }
        SOC_IF_ERROR_RETURN(WRITE_ING_COS_MODEr(unit, port, rval));
        
        if (si->port_num_subport[port] > 0) {
            start_pp_port = si->port_subport_base[port];
            end_pp_port = si->port_subport_base[port] +
                          si->port_num_subport[port];
            for (i = start_pp_port; i < end_pp_port; i++) {
                mmu_info_p->port[i].q_offset = si->port_uc_cosq_base[i];
                mmu_info_p->port[i].q_limit = si->port_uc_cosq_base[i] +
                                              si->port_num_uc_cosq[i];
                soc_reg_field_set(unit, ING_COS_MODEr, &rval, BASE_QUEUE_NUMf,
                                  mmu_info_p->port[i].q_offset);
                SOC_IF_ERROR_RETURN(WRITE_ING_COS_MODEr(unit, i, rval)); 
            }
        }

        SOC_IF_ERROR_RETURN(READ_RQE_PP_PORT_CONFIGr(unit, port, &rval));
        soc_reg_field_set(unit, RQE_PP_PORT_CONFIGr, &rval, BASE_QUEUEf,
                          (soc_feature(unit, soc_feature_mmu_packing)) ? 
                          mmu_info_p->port[port].mcq_offset :
                          mmu_info_p->port[port].q_offset);
        if (IS_EXT_MEM_PORT(unit, port)) {
            soc_reg_field_set(unit, RQE_PP_PORT_CONFIGr,
                              &rval, BUFF_TYPEf, 1);
        }    
        if (IS_LB_PORT(unit, port)) {
            soc_reg_field_set(unit, RQE_PP_PORT_CONFIGr, &rval, BASE_QUEUEf,
                              _BCM_KT2_LB_BASE_QUEUE);
        }
        if (IS_HG_PORT(unit, port)) {
            soc_reg_field_set(unit, RQE_PP_PORT_CONFIGr,
                              &rval, COS_MODEf, 2);
        }   
        SOC_IF_ERROR_RETURN(WRITE_RQE_PP_PORT_CONFIGr(unit, port, rval));

        if (si->port_num_subport[port] > 0) {
            for (i = start_pp_port; i < end_pp_port; i++) {
                soc_reg_field_set(unit, RQE_PP_PORT_CONFIGr, &rval, BASE_QUEUEf,
                                  (soc_feature(unit, soc_feature_mmu_packing)) ?
                                  mmu_info_p->port[port].mcq_offset :
                                  mmu_info_p->port[i].q_offset);
                SOC_IF_ERROR_RETURN(WRITE_RQE_PP_PORT_CONFIGr(unit, i, rval)); 
            }
        }
        
        rval = 0;
        soc_reg_field_set(unit, OP_E2ECC_PORT_CONFIGr, &rval, BASE_QUEUEf,
                          mmu_info_p->port[port].q_offset);
        if (IS_EXT_MEM_PORT(unit, port)) {
            soc_reg_field_set(unit, OP_E2ECC_PORT_CONFIGr, &rval, 
                              BUFF_TYPEf, 1);  
        }    
        if (!IS_LB_PORT(unit, port)) {
            SOC_IF_ERROR_RETURN(WRITE_OP_E2ECC_PORT_CONFIGr(unit, port, rval));
        }
        for (i = mmu_info_p->port[port].q_offset;
             i < mmu_info_p->port[port].q_limit; i++) {
            SOC_IF_ERROR_RETURN (soc_mem_field32_modify(unit,
                    EGR_QUEUE_TO_PP_PORT_MAPm, i, PP_PORTf, port));
        }

        if (soc_feature(unit, soc_feature_mmu_packing)) {
            for (i = mmu_info_p->port[port].mcq_offset;
                 i < mmu_info_p->port[port].mcq_limit; i++) {
                SOC_IF_ERROR_RETURN (soc_mem_field32_modify(unit,
                              EGR_QUEUE_TO_PP_PORT_MAPm, i, PP_PORTf, port));
            }
        }
        if (!IS_LB_PORT(unit, port)) {
            num_base_queues = mmu_info_p->port[port].q_limit;
        }
    }

    mmu_info_p->num_base_queues = soc_property_get(unit, 
                                                   spn_MMU_MAX_CLASSIC_QUEUES, 
                                                   num_base_queues);

    mmu_info_p->num_dmvoq_queues = soc_property_get(unit, 
                                                   spn_MMU_NUM_DMVOQ_QUEUES, 
                                                   0);

    mmu_info_p->num_ext_queues  = mmu_info_p->num_queues - 
                                  mmu_info_p->num_base_queues;

    if (mmu_info_p->num_dmvoq_queues > mmu_info_p->num_ext_queues ) {
        _bcm_kt2_cosq_free_memory(mmu_info_p);
        return BCM_E_MEMORY;
    }

    mmu_info_p->base_dmvoq_queue =  (mmu_info_p->num_base_queues + 7) & 0xfffffff8;

    mmu_info_p->qset_size       = soc_property_get(unit, 
                                              spn_MMU_SUBSCRIBER_NUM_COS_LEVEL, 
                                              1);

    mmu_info_p->l1_gport_pair = _bcm_kt2_cosq_alloc_clear(
                                 (mmu_info_p->num_nodes * sizeof(bcm_gport_t)),
                                 "l1_pair_info");
    if (mmu_info_p->l1_gport_pair == NULL) {
        _bcm_kt2_cosq_free_memory(mmu_info_p);
        return BCM_E_MEMORY;
    }

    mmu_info_p->ext_qlist.bits  = _bcm_kt2_cosq_alloc_clear(
                                  SHR_BITALLOCSIZE(mmu_info_p->num_ext_queues),
                                   "ext_qlist");
    if (mmu_info_p->ext_qlist.bits == NULL) {
        _bcm_kt2_cosq_free_memory(mmu_info_p);
        return BCM_E_MEMORY;
    }

    if (mmu_info_p->num_dmvoq_queues > 0) {
        mmu_info_p->dmvoq_qlist.bits  = _bcm_kt2_cosq_alloc_clear(
                SHR_BITALLOCSIZE(mmu_info_p->num_dmvoq_queues),
                "dmvoq_qlist");
        if (mmu_info_p->dmvoq_qlist.bits == NULL) {
            _bcm_kt2_cosq_free_memory(mmu_info_p);
            return BCM_E_MEMORY;
        }
    }


    mmu_info_p->sched_list.bits = _bcm_kt2_cosq_alloc_clear(
                                  SHR_BITALLOCSIZE(mmu_info_p->num_nodes),
                                  "sched_list");
    if (mmu_info_p->sched_list.bits == NULL) {
        _bcm_kt2_cosq_free_memory(mmu_info_p);
        return BCM_E_MEMORY;
    }

    for (i = _BCM_KT2_COSQ_NODE_LEVEL_S0; i < _BCM_KT2_COSQ_NODE_LEVEL_L2; 
              i++) { 
        mmu_info_p->sched_hw_list[i].bits = _bcm_kt2_cosq_alloc_clear(
                                            SHR_BITALLOCSIZE
                                            (mmu_info_p->max_nodes[i]),
                                            "sched_hw_list");
        if (mmu_info_p->sched_hw_list[i].bits == NULL) {
            _bcm_kt2_cosq_free_memory(mmu_info_p);
            return BCM_E_MEMORY;
        }
    }

    mmu_info_p->l2_base_qlist.bits = _bcm_kt2_cosq_alloc_clear(
                                     SHR_BITALLOCSIZE
                                     (mmu_info_p->num_base_queues),
                                     "l2_base_qlist");
    if (mmu_info_p->l2_base_qlist.bits == NULL) {
        _bcm_kt2_cosq_free_memory(mmu_info_p);
        return BCM_E_MEMORY;
    }
    
    mmu_info_p->l2_base_mcqlist.bits = _bcm_kt2_cosq_alloc_clear(
                                     SHR_BITALLOCSIZE
                                     (mmu_info_p->num_base_queues),
                                     "l2_base_mcqlist");
    if (mmu_info_p->l2_base_mcqlist.bits == NULL) {
        _bcm_kt2_cosq_free_memory(mmu_info_p);
        return BCM_E_MEMORY;
    }

    mmu_info_p->l2_ext_qlist.bits = _bcm_kt2_cosq_alloc_clear(
                                    SHR_BITALLOCSIZE
                                    (mmu_info_p->num_ext_queues),
                                    "l2_ext_qlist");
    if (mmu_info_p->l2_ext_qlist.bits == NULL) {
        _bcm_kt2_cosq_free_memory(mmu_info_p);
        return BCM_E_MEMORY;
    }

    if (mmu_info_p->num_dmvoq_queues > 0) {
        mmu_info_p->l2_dmvoq_qlist.bits = _bcm_kt2_cosq_alloc_clear(
                SHR_BITALLOCSIZE
                (mmu_info_p->num_dmvoq_queues),
                "l2_dmvoq_qlist");
        if (mmu_info_p->l2_dmvoq_qlist.bits == NULL) {
            _bcm_kt2_cosq_free_memory(mmu_info_p);
            return BCM_E_MEMORY;
        }
    }


    for (i = 0; i < num_total_schedulers; i++) {
        _BCM_KT2_COSQ_LIST_NODE_INIT(mmu_info_p->sched_node, i);
    }

    for (i = 0; i < num_l2_queues; i++) {
        _BCM_KT2_COSQ_LIST_NODE_INIT(mmu_info_p->queue_node, i);
    }

    /* reserve last entry of S0, S1, L0 and L1 nodes */
    for (i = _BCM_KT2_COSQ_NODE_LEVEL_S0; i < _BCM_KT2_COSQ_NODE_LEVEL_L2; 
             i++) { 
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_node_index_set(&mmu_info_p->sched_hw_list[i], 
            (mmu_info_p->max_nodes[i] - 1), 1));
    }

    /* remove mc queues from the extended queue list */
    if (soc_feature(unit, soc_feature_mmu_packing)) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_node_index_set(&mmu_info_p->l2_ext_qlist,
                           (2048 - mmu_info_p->num_base_queues),
                           mmu_info_p->num_base_queues));
    }
    /* Reserving the dmvoq hw_queues */
    if (mmu_info_p->num_dmvoq_queues > 0) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_node_index_set(&mmu_info_p->l2_ext_qlist,
                                     (mmu_info_p->base_dmvoq_queue -
                                      mmu_info_p->num_base_queues), 
                                     mmu_info_p->num_dmvoq_queues));
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_node_index_set(&mmu_info_p->ext_qlist,
                                     (mmu_info_p->base_dmvoq_queue -
                                      mmu_info_p->num_base_queues), 
                                     mmu_info_p->num_dmvoq_queues));
    }
    
    if (!SOC_WARM_BOOT(unit)) {    /* Cold Boot */
        /* create LLS tree for CPU */
        port = KT2_CMIC_PORT;
        BCM_IF_ERROR_RETURN
            (bcm_kt2_cosq_gport_add(unit, port, SOC_CMCS_NUM(unit), 
                           0, &port_sched));
        for (cmc = 0; cmc < SOC_CMCS_NUM(unit); cmc++) {
             if (NUM_CPU_ARM_COSQ(unit, cmc) == 0) {
                 continue;
             } 
             cosq = (NUM_CPU_ARM_COSQ(unit, cmc) + 7) / 8; 
             BCM_IF_ERROR_RETURN
                 (bcm_kt2_cosq_gport_add(unit, port, cosq, 
                               BCM_COSQ_GPORT_SCHEDULER, &l0_sched));
             BCM_IF_ERROR_RETURN
                 (bcm_kt2_cosq_gport_attach(unit, l0_sched, port_sched, cmc)); 
             cosq = 0;
             num_queues = NUM_CPU_ARM_COSQ(unit, cmc);
             for (i = 0; i < NUM_CPU_ARM_COSQ(unit, cmc); i++) {
                 if (i % 8 == 0) {
                     num_inputs = (num_queues >= 8) ? 8 : num_queues;
                     BCM_IF_ERROR_RETURN
                         (bcm_kt2_cosq_gport_add(unit, port, num_inputs, 
                                        BCM_COSQ_GPORT_SCHEDULER, 
                                        &l1_sched));
                     BCM_IF_ERROR_RETURN
                          (bcm_kt2_cosq_gport_attach(unit, l1_sched, 
                                                     l0_sched, cosq));  
                     cosq++;
                     num_queues -= num_inputs;
                 } 
                 BCM_IF_ERROR_RETURN
                     (bcm_kt2_cosq_gport_add(unit, port, 1,
                                  BCM_COSQ_GPORT_UCAST_QUEUE_GROUP, &queue));
                 BCM_IF_ERROR_RETURN
                     (bcm_kt2_cosq_gport_attach(unit, queue, l1_sched, 
                                                (i % 8)));  

             }
        }
        /* create LLS tree for Loopback port*/
        port = KT2_LPBK_PORT;
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_port_lls_config_set(unit, port, 
             si->port_uc_cosq_base[port], si->port_num_uc_cosq[port], numq));

        
        for (port = 1; port < si->lb_port ; port++) {
             if (IS_HG_PORT(unit, port)) {
                 BCM_IF_ERROR_RETURN
                     (_bcm_kt2_port_lls_config_set(unit, port, 
                      si->port_uc_cosq_base[port], 
                      si->port_num_uc_cosq[port], 
                      numq));
             } else {
                 if (!soc_feature(unit, soc_feature_mmu_packing)) {
                    BCM_IF_ERROR_RETURN
                    (bcm_kt2_cosq_gport_add(unit, port, 1, 0, &port_sched));
                    BCM_IF_ERROR_RETURN
                    (bcm_kt2_cosq_gport_add(unit, port, 1, 
                                        BCM_COSQ_GPORT_SCHEDULER, 
                                        &l0_sched));
                    BCM_IF_ERROR_RETURN
                    (bcm_kt2_cosq_gport_attach(unit, l0_sched, port_sched, 0)); 
                    BCM_IF_ERROR_RETURN
                    (bcm_kt2_cosq_gport_add(unit, port, numq, 
                                        BCM_COSQ_GPORT_SCHEDULER, 
                                        &l1_sched));
                    BCM_IF_ERROR_RETURN
                    (bcm_kt2_cosq_gport_attach(unit, l1_sched, l0_sched, 0));  
                    for (cosq = 0; cosq < numq; cosq++) {
                        BCM_IF_ERROR_RETURN
                        (bcm_kt2_cosq_gport_add(unit, port, 1,
                                BCM_COSQ_GPORT_UCAST_QUEUE_GROUP, &queue));
                        BCM_IF_ERROR_RETURN
                        (bcm_kt2_cosq_gport_attach(unit, queue, l1_sched, 
                                                cosq));  
                   }
                 }else {
                    BCM_IF_ERROR_RETURN
                    (_bcm_kt2_packing_config_set(unit, port, numq));
                 }
             }
        }
   }

    /* Add default entries for PORT_COS_MAP memory profile */
    BCM_IF_ERROR_RETURN(bcm_kt2_cosq_config_set(unit, numq));

    /* setup subport specific LLS configuration */
    BCM_IF_ERROR_RETURN(_bcm_kt2_subport_config_set(unit));

#ifdef BCM_WARM_BOOT_SUPPORT
    if (SOC_WARM_BOOT(unit)) {
        return bcm_kt2_cosq_reinit(unit);
    } else {
        BCM_IF_ERROR_RETURN(_bcm_kt2_cosq_wb_alloc(unit));
    }
#endif /* BCM_WARM_BOOT_SUPPORT */

    
    sal_memset(&entry_tcp_green, 0, sizeof(entry_tcp_green));
    sal_memset(&entry_tcp_yellow, 0, sizeof(entry_tcp_yellow));
    sal_memset(&entry_tcp_red, 0, sizeof(entry_tcp_red));
    sal_memset(&entry_nontcp_green, 0, sizeof(entry_nontcp_green));
    sal_memset(&entry_nontcp_yellow, 0, sizeof(entry_nontcp_yellow));
    sal_memset(&entry_nontcp_red, 0, sizeof(entry_nontcp_red));
    entries[0] = &entry_tcp_green;
    entries[1] = &entry_tcp_yellow;
    entries[2] = &entry_tcp_red;
    entries[3] = &entry_nontcp_green;
    entries[4] = &entry_nontcp_yellow;
    entries[5] = &entry_nontcp_red;
    max_threshold = (1 << soc_mem_field_length(unit, 
                     MMU_WRED_DROP_CURVE_PROFILE_0m, MAX_THDf)) - 1;
    min_threshold = (1 << soc_mem_field_length(unit, 
                     MMU_WRED_DROP_CURVE_PROFILE_0m, MIN_THDf)) - 1;    
    for (i = 0; i < 6; i++) {
        soc_mem_field32_set(unit, wred_mems[i], entries[i], MIN_THDf,
                            max_threshold);
        soc_mem_field32_set(unit, wred_mems[i], entries[i], MAX_THDf,
                            min_threshold);
    }    
    profile_index = 0xffffffff;
    for (port = 0; port < (si->lb_port + 1); port++) {  
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_index_resolve(unit, port, -1,
                                        _BCM_KT2_COSQ_INDEX_STYLE_WRED, NULL,
                                        NULL, &numq));
        for (cosq = 0; cosq < numq; cosq++) {
            if (profile_index == 0xffffffff) {
                BCM_IF_ERROR_RETURN
                    (soc_profile_mem_add(unit, _bcm_kt2_wred_profile[unit],
                                         entries, 1, &profile_index));
            } else {
                BCM_IF_ERROR_RETURN
                    (soc_profile_mem_reference(unit,
                                               _bcm_kt2_wred_profile[unit],
                                               profile_index, 0));
            }
        }
    }
    /*
     * Set all CPU queues to disable enqueue.  Enqueue will be enabled when
     * RX DMA is enabled. This is to prevent packets from getting wedged in
     * the CMIC when they don't have a means of egressing from the CMIC.
     */
    if (!(SAL_BOOT_SIMULATION) && !SOC_IS_RCPU_ONLY(unit)) {
        for (i = 0; i < NUM_CPU_COSQ(unit); i++) {
            rval = 0;
            soc_reg_field_set(unit, TOQ_FLUSH0r, &rval, FLUSH_ID0f, i);
            soc_reg_field_set(unit, TOQ_FLUSH0r, &rval, FLUSH_NUMf, 1);
            soc_reg_field_set(unit, TOQ_FLUSH0r, &rval, FLUSH_ACTIVEf, 1);
            if (SOC_PBMP_MEMBER (PBMP_EXT_MEM (unit), CMIC_PORT(unit))) {
                soc_reg_field_set(unit, TOQ_FLUSH0r, &rval, FLUSH_EXTERNALf, 1);
            }
            SOC_IF_ERROR_RETURN(WRITE_TOQ_FLUSH0r(unit, rval));
            while (rval) {
                SOC_IF_ERROR_RETURN(READ_TOQ_FLUSH0r(unit, &rval));
                rval = soc_reg_field_get(unit, TOQ_FLUSH0r, rval,
                                         FLUSH_ACTIVEf);
            }
            rval = 0;
            soc_reg_field_set(unit, TOQ_ERROR2r, &rval, FLUSH_COMPLETEf, 1);
            SOC_IF_ERROR_RETURN(WRITE_TOQ_ERROR2r(unit, rval));
        }
    }

      mmu_info_p->curr_merger_index = 1;
    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_detach
 * Purpose:
 *     Discard all COS schedule/mapping state.
 * Parameters:
 *     unit - unit number
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_kt2_cosq_detach(int unit, int software_state_only)
{
    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_config_set
 * Purpose:
 *     Set the number of COS queues
 * Parameters:
 *     unit - unit number
 *     numq - number of COS queues (1-8).
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_kt2_cosq_config_set(int unit, int numq)
{
    port_cos_map_entry_t cos_map_entries[16];
    void *entries[1], *hg_entries[1];
    uint32 index, hg_index;
    int count, hg_count;
    bcm_port_t port;
    int cos, prio;
    uint32 i;
    int cpu_hg_index = 0;
#ifdef BCM_COSQ_HIGIG_MAP_DISABLE
    port_cos_map_entry_t hg_cos_map_entries[16];
#endif

    if (numq < 1 || numq > 8) {
        return BCM_E_PARAM;
    }

    /* Distribute first 8 internal priority levels into the specified number
     * of cosq, map remaining internal priority levels to highest priority
     * cosq */
    sal_memset(cos_map_entries, 0, sizeof(cos_map_entries));
    entries[0] = &cos_map_entries;
    hg_entries[0] = &cos_map_entries;
    prio = 0;
    for (cos = 0; cos < numq; cos++) {
        for (i = 8 / numq + (cos < 8 % numq ? 1 : 0); i > 0; i--) {
            soc_mem_field32_set(unit, PORT_COS_MAPm, &cos_map_entries[prio],
                                COSf, cos);
            prio++;
        }
    }
    for (prio = 8; prio < 16; prio++) {
        soc_mem_field32_set(unit, PORT_COS_MAPm, &cos_map_entries[prio], COSf,
                            numq - 1);
    }

    /* Map internal priority levels to CPU CoS queues */
    BCM_IF_ERROR_RETURN(_bcm_esw_cpu_cosq_mapping_default_set(unit, numq));

#ifdef BCM_COSQ_HIGIG_MAP_DISABLE
    /* Use identical mapping for Higig port */
    sal_memset(hg_cos_map_entries, 0, sizeof(hg_cos_map_entries));
    hg_entries[0] = &hg_cos_map_entries;
    prio = 0;
    for (prio = 0; prio < 8; prio++) {
        soc_mem_field32_set(unit, PORT_COS_MAPm, &hg_cos_map_entries[prio],
                            COSf, prio);
    }
    for (prio = 8; prio < 16; prio++) {
        soc_mem_field32_set(unit, PORT_COS_MAPm, &hg_cos_map_entries[prio],
                            COSf, 7);
    }

#endif /* BCM_COSQ_HIGIG_MAP_DISABLE */

    BCM_IF_ERROR_RETURN
        (soc_profile_mem_add(unit, _bcm_kt2_cos_map_profile[unit], entries, 16,
                             &index));
    BCM_IF_ERROR_RETURN
        (soc_profile_mem_add(unit, _bcm_kt2_cos_map_profile[unit], hg_entries,
                             16, &hg_index));
    count = 0;
    hg_count = 0;
    PBMP_ALL_ITER(unit, port) {
        if (IS_HG_PORT(unit, port)) {
            BCM_IF_ERROR_RETURN
                (soc_mem_field32_modify(unit, COS_MAP_SELm, port, SELECTf,
                                        hg_index / 16));
            hg_count++;
        } else {
            BCM_IF_ERROR_RETURN
                (soc_mem_field32_modify(unit, COS_MAP_SELm, port, SELECTf,
                                        index / 16));
            count++;
        }
    }

    cpu_hg_index = SOC_INFO(unit).cpu_hg_pp_port_index;
    if (cpu_hg_index != -1) {
        BCM_IF_ERROR_RETURN
            (soc_mem_field32_modify(unit, COS_MAP_SELm,
                                    cpu_hg_index, SELECTf,
                                    hg_index / 16));
        hg_count++;
    }


    for (i = 1; i < count; i++) {
        soc_profile_mem_reference(unit, _bcm_kt2_cos_map_profile[unit], index,
                                  0);
    }
    for (i = 1; i < hg_count; i++) {
        soc_profile_mem_reference(unit, _bcm_kt2_cos_map_profile[unit],
                                  hg_index, 0);
    }

    _bcm_kt2_num_cosq[unit] = numq;

    return BCM_E_NONE;
}


/*
 * Function:
 *     bcm_kt2_cosq_config_get
 * Purpose:
 *     Get the number of cos queues
 * Parameters:
 *     unit - unit number
 *     numq - (Output) number of cosq
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_kt2_cosq_config_get(int unit, int *numq)
{
    if (_bcm_kt2_num_cosq[unit] == 0) {
        return BCM_E_INIT;
    }

    if (numq != NULL) {
        *numq = _bcm_kt2_num_cosq[unit];
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_gport_attach
 * Purpose:
 *     Attach sched_port to the specified index (cosq) of input_port
 * Parameters:
 *     unit       - (IN) unit number
 *     sched_port - (IN) scheduler GPORT identifier
 *     input_port - (IN) GPORT to attach to
 *     cosq       - (IN) COS queue to attach to (-1 for first available cosq)
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_kt2_cosq_gport_attach(int unit, bcm_gport_t sched_gport,
                         bcm_gport_t input_gport, bcm_cos_queue_t cosq)
{
    _bcm_kt2_cosq_node_t *sched_node, *input_node;
    bcm_port_t sched_port, input_port, local_port;
    int rv, max_nodes;
    _bcm_kt2_subport_info_t subport_info;

    if (_bcm_kt2_mmu_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(input_gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(input_gport) || 
        BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(input_gport)) {
        return BCM_E_PORT;
    }

    if(!(BCM_GPORT_IS_UCAST_QUEUE_GROUP(sched_gport) ||
         BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(sched_gport) ||
         BCM_GPORT_IS_MCAST_QUEUE_GROUP(sched_gport) ||
         BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(sched_gport) ||
         BCM_GPORT_IS_SCHEDULER(sched_gport))) {
        return BCM_E_PORT;
     }

    if (cosq < -1) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_node_get(unit, sched_gport, NULL, &sched_port, NULL,
                               &sched_node));

    if (sched_node->cosq_attached_to >= 0) {
        /* Has already attached */
        return BCM_E_EXISTS;
    }

    if (BCM_GPORT_IS_SCHEDULER(input_gport) ||
        BCM_GPORT_IS_LOCAL(input_gport) ||
        BCM_GPORT_IS_MODPORT(input_gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_node_get(unit, input_gport, NULL, &input_port, NULL,
                                   &input_node));
    } else {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_localport_resolve(unit, input_gport, &input_port));
        input_node = NULL;
    }

    if ((sched_port < num_port_schedulers) && 
        (input_port < num_port_schedulers) && 
        (sched_port != input_port)) {
        return BCM_E_PORT;
    }

    if (input_node != NULL) {
        if (!BCM_GPORT_IS_SCHEDULER(input_gport)) {
            if (input_node->numq == 0) {
                local_port = (BCM_GPORT_IS_LOCAL(input_gport)) ?
                              BCM_GPORT_LOCAL_GET(input_gport) :
                              BCM_GPORT_MODPORT_PORT_GET(input_gport);
                input_node->gport = input_gport;
                input_node->hw_index = local_port;
                input_node->level = _BCM_KT2_COSQ_NODE_LEVEL_ROOT;
                input_node->cosq_attached_to = 0;
                input_node->numq = 4; /* Assign max of 4 L0 nodes */
                input_node->base_index = -1;
            }

            if (!BCM_GPORT_IS_SCHEDULER(sched_gport)) {
                return BCM_E_PARAM;
            }

            sched_node->level = (sched_port < num_port_schedulers) ?
                                 _BCM_KT2_COSQ_NODE_LEVEL_L0 : 
                                 _BCM_KT2_COSQ_NODE_LEVEL_S0; 
        } else {
             if (input_node->level == _BCM_KT2_COSQ_NODE_LEVEL_ROOT) {
                 if (!BCM_GPORT_IS_SCHEDULER(input_gport)) {
                     return BCM_E_PARAM;
                 }
                 sched_node->level = 
                                (sched_port < num_port_schedulers) ?
                                _BCM_KT2_COSQ_NODE_LEVEL_L0 : 
                                _BCM_KT2_COSQ_NODE_LEVEL_S0; 
             }

             if (BCM_GPORT_IS_SCHEDULER(input_gport) &&
                 BCM_GPORT_IS_SCHEDULER(sched_gport)) {
                 if (input_node->level == _BCM_KT2_COSQ_NODE_LEVEL_S0) {
                     BCM_IF_ERROR_RETURN
                         (bcm_kt2_subport_pp_port_subport_info_get(unit, 
                                                         input_port, 
                                                         &subport_info));
                     if (subport_info.port_type == _BCM_KT2_SUBPORT_TYPE_LINKPHY) {
                         sched_node->level = _BCM_KT2_COSQ_NODE_LEVEL_S1;
                         input_node->linkphy_enabled = 1;
                     } else {
                         sched_node->level = _BCM_KT2_COSQ_NODE_LEVEL_L0; 
                     }
                 }
                 if (input_node->level == _BCM_KT2_COSQ_NODE_LEVEL_S1) {
                      sched_node->level = _BCM_KT2_COSQ_NODE_LEVEL_L0;
                 }
                 if (input_node->level == _BCM_KT2_COSQ_NODE_LEVEL_L0) {
                      sched_node->level = _BCM_KT2_COSQ_NODE_LEVEL_L1;
                 }    
             }

             if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(sched_gport) ||
                  BCM_GPORT_IS_MCAST_QUEUE_GROUP(sched_gport) ||
                  BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(sched_gport) ||
                 BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(sched_gport)) {
                  if (!BCM_GPORT_IS_SCHEDULER(input_gport)) {
                     return BCM_E_PARAM;
                  }
                 if (input_node->level != _BCM_KT2_COSQ_NODE_LEVEL_L1) {
                     return BCM_E_PARAM;
                 }
                 sched_node->level = _BCM_KT2_COSQ_NODE_LEVEL_L2;
             }

             BCM_IF_ERROR_RETURN
                 (_bcm_kt2_cosq_max_nodes_get(unit, sched_node->level, &max_nodes));
             if (input_node->numq == -1) {
                 input_node->numq = max_nodes;
             } else {
                 if (input_node->numq > max_nodes) {
                     return BCM_E_PARAM;
                 }
             }
        }

        if (input_node->cosq_attached_to < 0) {
            /* Only allow to attach to a node that has already attached */
            return BCM_E_PARAM;
        }
        if (cosq < -1 || (input_node->numq != -1 && cosq >= input_node->numq)) {
            return BCM_E_PARAM;
        }
    }

    if (BCM_GPORT_IS_SCHEDULER(input_gport) || 
        BCM_GPORT_IS_LOCAL(input_gport) ||
        BCM_GPORT_IS_MODPORT(input_gport)) {
        if (input_node->cosq_attached_to < 0) {
            /* dont allow to attach to a node that has already attached */
            return BCM_E_PARAM;
        }

        sched_node->parent = input_node;
        sched_node->sibling = input_node->child;
        input_node->child = sched_node;
        /* resolve the nodes */
        rv = _bcm_kt2_cosq_node_resolve(unit, sched_node, cosq);
        if (BCM_FAILURE(rv)) {
            input_node->child = sched_node->sibling;
            return rv;
        }
        BCM_IF_ERROR_RETURN(_bcm_kt2_cosq_sched_node_set(unit, sched_gport, 
                            _BCM_KT2_COSQ_STATE_ENABLE));

        LOG_INFO(BSL_LS_BCM_COSQ,
                 (BSL_META_U(unit,
                             "                         hw_cosq=%d\n"),
                  sched_node->cosq_attached_to));
    } else {
            return BCM_E_PORT;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_gport_detach
 * Purpose:
 *     Detach sched_port to the specified index (cosq) of input_port
 * Parameters:
 *     unit       - (IN) unit number
 *     sched_port - (IN) scheduler GPORT identifier
 *     input_port - (IN) GPORT to attach to
 *     cosq       - (IN) COS queue to attach to (-1 for first available cosq)
 * Returns:
 *     BCM_E_XXX
 */
int
bcm_kt2_cosq_gport_detach(int unit, bcm_gport_t sched_gport,
                         bcm_gport_t input_gport, bcm_cos_queue_t cosq)
{
    _bcm_kt2_cosq_node_t *sched_node, *input_node, *prev_node;
    bcm_port_t sched_port, input_port;

    if (_bcm_kt2_mmu_info[unit] == NULL) {
        return BCM_E_INIT;
    }

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(input_gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(input_gport) ||
        BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(input_gport)) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_node_get(unit, sched_gport, NULL, &sched_port, NULL,
                               &sched_node));

    if (sched_node->cosq_attached_to < 0) {
        /* Not attached yet */
        return BCM_E_PORT;
    }

    if (input_gport != BCM_GPORT_INVALID) {

        if (BCM_GPORT_IS_SCHEDULER(input_gport) ||
            BCM_GPORT_IS_LOCAL(input_gport) ||
            BCM_GPORT_IS_MODPORT(input_gport)) {
               BCM_IF_ERROR_RETURN
                (_bcm_kt2_cosq_node_get(unit, input_gport, NULL, &input_port, 
                                        NULL, &input_node));
        }
        else {
            if (!(BCM_GPORT_IS_SCHEDULER(sched_gport) || 
                  BCM_GPORT_IS_UCAST_QUEUE_GROUP(sched_gport) ||
                  BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(sched_gport) ||
                  BCM_GPORT_IS_MCAST_QUEUE_GROUP(sched_gport))) {
                return BCM_E_PARAM;
            }
            else {
                BCM_IF_ERROR_RETURN
                (_bcm_kt2_cosq_localport_resolve(unit, input_gport,
                                                &input_port));
                input_node = NULL;
            }
        }
    }

    if ((sched_port < num_port_schedulers) && 
        (input_port < num_port_schedulers) && 
        (sched_port != input_port)) {
        return BCM_E_PORT;
    }

    if (sched_node->parent != input_node) {
        return BCM_E_PORT;
    }

    if (cosq < -1 || (input_node->numq != -1 && cosq >= input_node->numq)) {
        return BCM_E_PARAM;
    }    

    if (cosq != -1) {
        if (sched_node->cosq_attached_to != cosq) {
            return BCM_E_PARAM;
        }
    }

    /* update the hw accordingly */
    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_sched_node_set(unit, sched_gport, 
                                     _BCM_KT2_COSQ_STATE_DISABLE));
    /* unresolve the node - delete this node from parent's child list */
    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_node_unresolve(unit, sched_node, cosq));

    /* now remove from the sw tree */
    if (sched_node->parent != NULL) {
        if (sched_node->parent->child == sched_node) {
            sched_node->parent->child = sched_node->sibling;
        } else {
            prev_node = sched_node->parent->child;
            while (prev_node != NULL && prev_node->sibling != sched_node) {
                prev_node = prev_node->sibling;
            }
            if (prev_node == NULL) {
                return BCM_E_INTERNAL;
            }
            prev_node->sibling = sched_node->sibling;
       }
        sched_node->parent = NULL;
        sched_node->sibling = NULL;
        sched_node->wrr_in_use = 0;
        sched_node->cosq_attached_to = -1;        
        if (!(BCM_GPORT_IS_UCAST_QUEUE_GROUP(input_gport) ||
              BCM_GPORT_IS_MCAST_QUEUE_GROUP(input_gport) ||        
              BCM_GPORT_IS_DESTMOD_QUEUE_GROUP(input_gport))) {        
            sched_node->hw_index = -1;
        }
     }

      LOG_INFO(BSL_LS_BCM_COSQ,
               (BSL_META_U(unit,
                           "                         hw_cosq=%d\n"),
                sched_node->cosq_attached_to));

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_gport_attach_get
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
bcm_kt2_cosq_gport_attach_get(int unit, bcm_gport_t sched_gport,
                             bcm_gport_t *input_gport, bcm_cos_queue_t *cosq)
{
    _bcm_kt2_cosq_node_t *sched_node;
    bcm_module_t modid, modid_out;
    bcm_port_t port, port_out;

    if (input_gport == NULL || cosq == NULL) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_node_get(unit, sched_gport, &modid, &port, NULL,
                               &sched_node));

    if (sched_node->level == _BCM_KT2_COSQ_NODE_LEVEL_ROOT) {
        return BCM_E_PARAM;
    }    
    
    if (sched_node->cosq_attached_to < 0) {
        /* Not attached yet */
        return BCM_E_NOT_FOUND;
    }

    if (sched_node->parent != NULL) { 
        *input_gport = sched_node->parent->gport;
    } else {  /* sched_node is in L2 level */
        BCM_IF_ERROR_RETURN
            (_bcm_esw_stk_modmap_map(unit, BCM_STK_MODMAP_GET, modid, port,
                                    &modid_out, &port_out));
        BCM_GPORT_MODPORT_SET(*input_gport, modid_out, port_out);
    }
    *cosq = sched_node->cosq_attached_to;

    return BCM_E_NONE;
}

STATIC int
_bcm_kt2_cosq_ef_update_war(int unit, bcm_gport_t gport,  bcm_cos_queue_t cosq,
                           int hw_index, lls_l2_parent_entry_t *entry)
{
    lls_l1_config_entry_t l1_config;
    uint32 min_bw, max_bw, min_burst, max_burst, flags = 0;
    int parent_index, spri_vector;
    uint32 start_pri, vec_3_0 =0, vec_7_4 = 0;
    uint32 rval = 0, bit_offset;

    LOG_INFO(BSL_LS_BCM_COSQ,
             (BSL_META_U(unit,
                         "_bcm_kt2_cosq_ef_update_war:unit=%d gport=0x%x cosq=%d idx=%d\n"),
              unit, gport, cosq, hw_index));
   
    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_bucket_get(unit, gport, cosq, &min_bw, &max_bw,
                                 &min_burst, &max_burst, flags)); 

    if ((min_bw == 0) && (max_bw == 0)) {
        SOC_IF_ERROR_RETURN
            (soc_mem_write(unit, LLS_L2_PARENTm, MEM_BLOCK_ALL,
                           hw_index, entry));
    } else {
        parent_index = soc_mem_field32_get(unit, LLS_L2_PARENTm, entry, 
                                           C_PARENTf); 
        SOC_IF_ERROR_RETURN
            (soc_mem_read(unit, LLS_L1_CONFIGm, MEM_BLOCK_ALL, parent_index, 
                          &l1_config));
        start_pri = soc_mem_field32_get(unit, LLS_L1_CONFIGm, &l1_config, 
                                        P_START_UC_SPRIf); 
        vec_3_0 = soc_mem_field32_get(unit, LLS_L1_CONFIGm, &l1_config, 
                                      P_NUM_SPRIf);
        vec_7_4 = soc_mem_field32_get(unit, LLS_L1_CONFIGm, &l1_config, 
                                      P_VECT_SPRI_7_4f);
        spri_vector = vec_3_0 | (vec_7_4 << 4);
        bit_offset = hw_index - start_pri;
        if (bit_offset >= 8) {
            return BCM_E_PARAM;
        }
        LOG_INFO(BSL_LS_BCM_COSQ,
                 (BSL_META_U(unit,
                             "parent=%d spri_vector=0x%x bit_offset=%d\n"),
                  parent_index, spri_vector, bit_offset));
        spri_vector &= ~(1 << bit_offset);
        soc_mem_field32_set(unit, LLS_L1_CONFIGm, &l1_config, P_NUM_SPRIf, 
                            (spri_vector & 0xF));
        soc_mem_field32_set(unit, LLS_L1_CONFIGm, &l1_config, P_VECT_SPRI_7_4f, 
                            ((spri_vector >> 4) & 0xF));
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_bucket_set(unit, gport, cosq, 0, 0,
                                     min_burst, max_burst, flags)); 
        SOC_IF_ERROR_RETURN
            (soc_mem_write(unit, LLS_L2_PARENTm, MEM_BLOCK_ALL,
                           hw_index, entry));
        SOC_IF_ERROR_RETURN
            (soc_mem_write(unit, LLS_L1_CONFIGm, MEM_BLOCK_ALL,
                           parent_index, &l1_config));

        soc_reg_field_set(unit, LLS_DEBUG_INJECT_ACTIVATIONr, &rval, 
                          INJECTf, 1);
        /* 0 = enqueue event,1 = shaper event,2 = xon event,3 = xoff event */ 
        soc_reg_field_set(unit, LLS_DEBUG_INJECT_ACTIVATIONr, &rval, 
                          ENTITY_TYPEf, 1);
        /* 0 = port, 1 = level 0, 2 = level 1, 3 = level 2 */
        soc_reg_field_set(unit, LLS_DEBUG_INJECT_ACTIVATIONr, &rval, 
                          ENTITY_LEVELf, 3);
        soc_reg_field_set(unit, LLS_DEBUG_INJECT_ACTIVATIONr, &rval, 
                          ENTITY_IDENTIFIERf, hw_index); 
        SOC_IF_ERROR_RETURN(WRITE_LLS_DEBUG_INJECT_ACTIVATIONr(unit, rval)); 
        spri_vector |= (1 << bit_offset);
        soc_mem_field32_set(unit, LLS_L1_CONFIGm, &l1_config, P_NUM_SPRIf, 
                            (spri_vector & 0xF));
        soc_mem_field32_set(unit, LLS_L1_CONFIGm, &l1_config, P_VECT_SPRI_7_4f, 
                            ((spri_vector >> 4) & 0xF));
        SOC_IF_ERROR_RETURN
            (soc_mem_write(unit, LLS_L1_CONFIGm, MEM_BLOCK_ALL,
                           parent_index, &l1_config));
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_bucket_set(unit, gport, cosq, min_bw, max_bw,
                                     min_burst, max_burst, flags)); 
    }

    return BCM_E_NONE;
}

STATIC int
_bcm_kt2_cosq_valid_ef_node(int unit, lls_l1_config_entry_t *entry, 
                           int hw_index)
{
    lls_l2_parent_entry_t l2_parent;
    int index, start_pri, spri_vector, num_spri, vec_7_4;
    uint32 bit_offset;
    int i;

    start_pri = soc_mem_field32_get(unit, LLS_L1_CONFIGm, entry, 
                                    P_START_UC_SPRIf); 
    num_spri = soc_mem_field32_get(unit, LLS_L1_CONFIGm, entry, 
                                   P_NUM_SPRIf);
    if (soc_feature(unit, soc_feature_vector_based_spri)) {
        vec_7_4 = soc_mem_field32_get(unit, LLS_L1_CONFIGm, entry, 
                                      P_VECT_SPRI_7_4f);
        spri_vector = num_spri | (vec_7_4 << 4);
        bit_offset = hw_index - start_pri;
        if (bit_offset >= 8) {
            return BCM_E_PARAM;
        }
        if (!(spri_vector & (1 << bit_offset))) {
            /* werr node cannot be EF */
            return BCM_E_PARAM;
        }
        for (i = 0; i < 8; i++) {
            if (spri_vector & (1 << i)) {
                index = start_pri + i;
                SOC_IF_ERROR_RETURN
                     (soc_mem_read(unit, LLS_L2_PARENTm, MEM_BLOCK_ALL,
                                   index, &l2_parent));
                if (soc_mem_field32_get(unit, LLS_L2_PARENTm, 
                                        &l2_parent, C_EFf) == 1) {
                    /* maximum of 1 EF node is supported */
                    return FALSE;
                }
            }
        }
    } else {
        if ((hw_index < start_pri) || (hw_index >= (start_pri + num_spri))) {
            /* werr node cannot be EF */
            return BCM_E_PARAM;
        } 
        for (i = start_pri; i < (start_pri + num_spri); i++) {
            SOC_IF_ERROR_RETURN
                (soc_mem_read(unit, LLS_L2_PARENTm, MEM_BLOCK_ALL,
                              i, &l2_parent));
            if (soc_mem_field32_get(unit, LLS_L2_PARENTm,
                                    &l2_parent, C_EFf) == 1) {
                /* maximum of 1 EF node is supported */
                return FALSE;
            }
        }
    }
   
    return TRUE;
}

STATIC int
_bcm_kt2_cosq_ef_op_at_index(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq,
                            int hw_index, _bcm_cosq_op_t op, int *ef_val)
{
    lls_l2_parent_entry_t entry;
    lls_l1_parent_entry_t l1_entry;
    lls_l1_config_entry_t l1_config;
    lls_l0_config_entry_t l0_config;
    uint32 current_ef = 0, set_ef = 0;
    uint32 index;

    SOC_IF_ERROR_RETURN
        (soc_mem_read(unit, LLS_L2_PARENTm, MEM_BLOCK_ALL, hw_index, &entry));

    if (op == _BCM_COSQ_OP_GET) {
        *ef_val = soc_mem_field32_get(unit, LLS_L2_PARENTm, &entry, C_EFf);
    } else {
        current_ef = soc_mem_field32_get(unit, LLS_L2_PARENTm, &entry, C_EFf);
        set_ef = (*ef_val != 0) ? 1 : 0;
        soc_mem_field32_set(unit, LLS_L2_PARENTm, &entry, C_EFf, set_ef);
        if ((current_ef == 0) && (set_ef == 1)) {
            index = soc_mem_field32_get(unit, LLS_L2_PARENTm, &entry, 
                                        C_PARENTf); 
            SOC_IF_ERROR_RETURN
                (soc_mem_read(unit, LLS_L1_CONFIGm, MEM_BLOCK_ALL, 
                              index, &l1_config));
            if (_bcm_kt2_cosq_valid_ef_node(unit, &l1_config, 
                                           hw_index) != TRUE) {
                 return BCM_E_PARAM;
            }  
            soc_mem_field32_set(unit, LLS_L1_CONFIGm, &l1_config, 
                                P_CFG_EF_PROPAGATEf, set_ef);
            SOC_IF_ERROR_RETURN
                (soc_mem_write(unit, LLS_L1_CONFIGm, MEM_BLOCK_ALL,
                               index, &l1_config));
            SOC_IF_ERROR_RETURN
                (soc_mem_read(unit, LLS_L1_PARENTm, MEM_BLOCK_ALL, 
                              index, &l1_entry));
            index = soc_mem_field32_get(unit, LLS_L1_PARENTm, &l1_entry, 
                                        C_PARENTf); 
            SOC_IF_ERROR_RETURN
                (soc_mem_read(unit, LLS_L0_CONFIGm, MEM_BLOCK_ALL, 
                              index, &l0_config));
            soc_mem_field32_set(unit, LLS_L0_CONFIGm, &l0_config, 
                                P_CFG_EF_PROPAGATEf, set_ef);
            SOC_IF_ERROR_RETURN
                (soc_mem_write(unit, LLS_L0_CONFIGm, MEM_BLOCK_ALL,
                               index, &l0_config));
            SOC_IF_ERROR_RETURN
                (soc_mem_write(unit, LLS_L2_PARENTm, MEM_BLOCK_ALL,
                               hw_index, &entry));
        } else if ((current_ef == 1) && (set_ef == 0)) {
            if (soc_feature(unit, soc_feature_vector_based_spri)) {
                return _bcm_kt2_cosq_ef_update_war(unit, gport, cosq, 
                                                  hw_index, &entry);
            }  
            SOC_IF_ERROR_RETURN
                (soc_mem_write(unit, LLS_L2_PARENTm, MEM_BLOCK_ALL,
                               hw_index, &entry));
        } else {
            return BCM_E_NONE;     
        }
    }

    return BCM_E_NONE;
}

STATIC int
_bcm_kt2_cosq_port_qnum_get(int unit, bcm_gport_t gport,
        bcm_cos_queue_t cosq, bcm_cosq_control_t type, int *arg)
{
    bcm_port_t local_port;
    if (!arg) {
        return BCM_E_PARAM;
    }

    if (!BCM_COSQ_QUEUE_VALID(unit, cosq)) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_localport_resolve(unit, gport, &local_port));
    if (local_port < 0) {
        return BCM_E_PORT;
    }

    if (type == bcmCosqControlPortQueueUcast) {
        *arg = SOC_INFO(unit).port_uc_cosq_base[local_port] + cosq;
    } else {
        *arg = SOC_INFO(unit).port_cosq_base[local_port] + cosq;
    }

    return BCM_E_NONE;
}

STATIC int
_bcm_kt2_cosq_ef_op(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq, int *arg,
                   _bcm_cosq_op_t op)
{
    bcm_port_t local_port;
    int index, numq, from_cos, to_cos, ci;

    if (BCM_GPORT_IS_UCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_MCAST_QUEUE_GROUP(gport) ||
        BCM_GPORT_IS_UCAST_SUBSCRIBER_QUEUE_GROUP(gport)) {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_index_resolve
             (unit, gport, cosq, _BCM_KT2_COSQ_INDEX_STYLE_BUCKET,
              &local_port, &index, NULL));
        return _bcm_kt2_cosq_ef_op_at_index(unit, gport, cosq, 
                                           index, op, arg);
    } else if (BCM_GPORT_IS_SCHEDULER(gport)) {
        return BCM_E_PARAM;
    } else {
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_index_resolve
             (unit, gport, cosq, _BCM_KT2_COSQ_INDEX_STYLE_BUCKET,
              &local_port, &index, &numq));
        if (cosq == BCM_COS_INVALID) {
            /* Maximum 1 spri can be EF enabled */
            from_cos = to_cos = 0;
        } else {
            from_cos = to_cos = cosq;
        }
        for (ci = from_cos; ci <= to_cos; ci++) {
            BCM_IF_ERROR_RETURN
                (_bcm_kt2_cosq_index_resolve
                 (unit, gport, ci, _BCM_KT2_COSQ_INDEX_STYLE_BUCKET,
                  &local_port, &index, NULL));

            BCM_IF_ERROR_RETURN(_bcm_kt2_cosq_ef_op_at_index(unit,
                                gport, ci, index, op, arg));
            if (op == _BCM_COSQ_OP_GET) {
                return BCM_E_NONE;
            }
        }
    }
    return BCM_E_NONE;
}

int
bcm_kt2_cosq_control_set(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq,
                        bcm_cosq_control_t type, int arg)
{
    LOG_INFO(BSL_LS_BCM_COSQ,
             (BSL_META_U(unit,
                         "bcm_kt2_cosq_control_set: unit=%d gport=0x%x cosq=%d \
                         type=%d arg=%d\n"),
              unit, gport, cosq, type, arg));
    switch (type) {
    case bcmCosqControlEfPropagation:
        return _bcm_kt2_cosq_ef_op(unit, gport, cosq, &arg, _BCM_COSQ_OP_SET);
    default:
        break;
    }

    return BCM_E_UNAVAIL;
}

int
bcm_kt2_cosq_control_get(int unit, bcm_gport_t gport, bcm_cos_queue_t cosq,
                        bcm_cosq_control_t type, int *arg)
{
    LOG_INFO(BSL_LS_BCM_COSQ,
             (BSL_META_U(unit,
                         "bcm_kt2_cosq_control_get: unit=%d gport=0x%x cosq=%d type=%d\n"),
              unit, gport, cosq, type));
    switch (type) {
    case bcmCosqControlEfPropagation:
        return _bcm_kt2_cosq_ef_op(unit, gport, cosq, arg, _BCM_COSQ_OP_GET);
    case bcmCosqControlPortQueueUcast:
    case bcmCosqControlPortQueueMcast:
        return _bcm_kt2_cosq_port_qnum_get(unit, gport, cosq, type, arg);
    default:
        break;
    }

    return BCM_E_UNAVAIL;
}

int
bcm_kt2_cosq_port_flush(int unit, 
                        bcm_port_t port,
                        bcm_cosq_flush_type_t type) 
{
    _bcm_kt2_mmu_info_t  *mmu_info;
    _bcm_kt2_cosq_node_t *port_node = NULL;
    _bcm_kt2_cosq_node_t *s0_node = NULL;
    _bcm_kt2_cosq_node_t *s1_node = NULL;
    _bcm_kt2_cosq_node_t *l0_node = NULL;
    _bcm_kt2_cosq_node_t *l1_node = NULL;
    _bcm_kt2_cosq_node_t *l2_node = NULL;
    uint32                flush_reg=0;

    if (!SOC_PORT_VALID(unit, port)) {
        LOG_CLI((BSL_META_U(unit,
                            "Invalid Port:%d \n"), port));
        return BCM_E_PORT;
    }
    switch (type) {
    case bcmCosqFlushTypePort :
        BCM_IF_ERROR_RETURN(READ_TOQ_FLUSH0r(unit, &flush_reg));
        soc_reg_field_set(unit, TOQ_FLUSH0r, &flush_reg,
                                FLUSH_ACTIVEf,1);
        soc_reg_field_set(unit, TOQ_FLUSH0r, &flush_reg, FLUSH_TYPEf,1);
        soc_reg_field_set(unit, TOQ_FLUSH0r, &flush_reg, FLUSH_NUMf,1);
        soc_reg_field_set(unit, TOQ_FLUSH0r, &flush_reg,
                                FLUSH_ID0f, port);
        if (IS_EXT_MEM_PORT(unit, port)) {
            soc_reg_field_set(unit, TOQ_FLUSH0r, &flush_reg, 
                              FLUSH_EXTERNALf, 1);
        } else {
            soc_reg_field_set(unit, TOQ_FLUSH0r, &flush_reg, 
                              FLUSH_EXTERNALf, 0);
        }
        BCM_IF_ERROR_RETURN(WRITE_TOQ_FLUSH0r(unit, flush_reg));
        break; 
    case bcmCosqFlushTypeQueue:
    default:
         /* Queue Flushing one by one so ... */
    mmu_info = _bcm_kt2_mmu_info[unit];
    /* get the root node */
    port_node = &mmu_info->sched_node[port];
    if (port_node && port_node->cosq_attached_to < 0) {
        LOG_CLI((BSL_META_U(unit,
                            "No Scheduling node found for port:%d\n"), port));
        return BCM_E_NONE;
    }   
         s0_node = port_node->child->level == _BCM_KT2_COSQ_NODE_LEVEL_S0 ? 
                   port_node->child : port_node;

         do {
             s1_node = s0_node->child->level == _BCM_KT2_COSQ_NODE_LEVEL_S1 ? 
                       s0_node->child : s0_node;
             do {
                 for (l0_node = s1_node->child; 
                      l0_node != NULL; 
         l0_node = l0_node->sibling) {
                      for (l1_node = l0_node->child; 
                           l1_node != NULL; 
              l1_node = l1_node->sibling) {
                           for (l2_node = l1_node->child; 
                                l2_node != NULL; 
                                l2_node = l2_node->sibling) {
                                /* Get Queue from L2 Node */
                                LOG_CLI((BSL_META_U(unit,
                                                    "Flushing Port:%d => "
                                                    "QueueNumber:%d \n"),
                                         port, l2_node->hw_index));
                                /* Now flush the queue  */
                                BCM_IF_ERROR_RETURN(_bcm_kt2_cosq_flush_queues(
                                                    unit, port,
                                                    l2_node, l2_node->gport));
              }
                   }
              } 
                if (s1_node == s0_node) {
                    break;
              }
                s1_node = s1_node->sibling;
            } while (s1_node != NULL);
            if (s0_node == port_node) {
                break;
         }
            s0_node = s0_node->sibling;
        } while (s0_node != NULL);
        break;
    }
    return BCM_E_NONE;
}

int
bcm_kt2_cosq_drop_status_enable_set(int unit, bcm_port_t port, int enable)
{
    uint32 rval;
    uint64        r64val;

    BCM_IF_ERROR_RETURN(READ_OP_E2ECC_PORT_CONFIGr(unit, port, &rval));
    soc_reg_field_set(unit, OP_E2ECC_PORT_CONFIGr, &rval, 
                      COS_MASKf,  enable ? 0xFF : 0x0);
    soc_reg_field_set(unit, OP_E2ECC_PORT_CONFIGr, &rval, 
                      E2ECC_RPT_ENf, enable ? 1 : 0);
    BCM_IF_ERROR_RETURN(WRITE_OP_E2ECC_PORT_CONFIGr(unit, port, rval));
    

    BCM_IF_ERROR_RETURN(READ_OP_THR_CONFIGr(unit, &rval));
    soc_reg_field_set(unit, OP_THR_CONFIGr, &rval, EARLY_E2E_SELECTf,
                      enable ? 1 : 0);
    BCM_IF_ERROR_RETURN(WRITE_OP_THR_CONFIGr(unit, rval));
    
    COMPILER_64_ZERO(r64val);
    soc_reg64_field32_set(unit, MMU_INTFO_CONGST_STr,
            &r64val, ENf, 1);
    SOC_IF_ERROR_RETURN(WRITE_MMU_INTFO_CONGST_STr(unit,
                port, r64val));

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_kt2_cosq_field_classifier_id_create
 * Purpose:
 *      Create an endpoint.
 * Parameters: 
 *      unit          - (IN) Unit number.
 *      classifier    - (IN) Classifier attributes
 *      classifier_id - (OUT) Classifier ID
 * Returns:
 *      BCM_E_xxx
 */     
int
bcm_kt2_cosq_field_classifier_id_create(
    int unit,
    bcm_cosq_classifier_t *classifier,
    int *classifier_id)
{
    int rv;
    int ref_count = 0;
    int ent_per_set = 16;
    int i;

    if (NULL == classifier || NULL == classifier_id) {
        return BCM_E_PARAM;
    }

    for (i = 0; i < SOC_MEM_SIZE(unit, IFP_COS_MAPm);) {
        rv = soc_profile_mem_ref_count_get
                (unit, _bcm_kt2_ifp_cos_map_profile[unit], i, &ref_count);
        if (SOC_E_NONE != rv) {
            return (rv);
        }
        if (0 == ref_count) {
            break;
        }
        i = i + ent_per_set;
    }

    if (i >= SOC_MEM_SIZE(unit, IFP_COS_MAPm) && ref_count != 0) {
        *classifier_id = 0;
        return (BCM_E_RESOURCE);
    }

    _BCM_COSQ_CLASSIFIER_FIELD_SET(*classifier_id, (i / ent_per_set));

    return (BCM_E_NONE);
}

/*
 * Function:
 *      bcm_kt2_cosq_field_classifier_get
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
bcm_kt2_cosq_field_classifier_get(
    int unit,
    int classifier_id,
    bcm_cosq_classifier_t *classifier)
{
    sal_memset(classifier, 0, sizeof(bcm_cosq_classifier_t));

    if (_BCM_COSQ_CLASSIFIER_IS_FIELD(classifier_id)) {
        classifier->flags |= BCM_COSQ_CLASSIFIER_FIELD;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_kt2_cosq_service_classifier_id_create
 * Purpose:
 *      Create an endpoint.
 * Parameters: 
 *      unit          - (IN) Unit number.
 *      classifier    - (IN) Classifier attributes
 *      classifier_id - (OUT) Classifier ID
 * Returns:
 *      BCM_E_xxx
 */     
int
bcm_kt2_cosq_service_classifier_id_create(
    int unit,
    bcm_cosq_classifier_t *classifier,
    int *classifier_id)
{
    if (NULL == classifier || NULL == classifier_id) {
        return BCM_E_PARAM;
    }
    /* XXX check classifier->vlan for valid vlan/vfi */
    _BCM_COSQ_CLASSIFIER_SERVICE_SET(*classifier_id,classifier->vlan);
    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_kt2_cosq_service_classifier_id_destroy
 * Purpose:
 *      free resource associated with this service classifier id.
 * Parameters:
 *      unit          - (IN) Unit number.
 *      classifier_id - (IN) Classifier ID
 * Returns:
 *      BCM_E_xxx
 */     
int
bcm_kt2_cosq_service_classifier_id_destroy(
    int unit,
    int classifier_id)
{
    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_kt2_cosq_field_classifier_map_set
 * Purpose:
 *      Set internal priority to ingress field processor CoS queue
 *      override mapping.
 * Parameters:
 *      unit           - (IN) Unit number.
 *      classifier_id  - (IN) Classifier ID
 *      count          - (IN) Number of elements in priority_array and
 *                            cosq_array
 *      priority_array - (IN) Array of internal priorities
 *      cosq_array     - (IN) Array of COS queues
 * Returns:
 *      BCM_E_xxx
 */
int
bcm_kt2_cosq_field_classifier_map_set(int unit,
                                      int classifier_id,
                                      int count,
                                      bcm_cos_t *priority_array,
                                      bcm_cos_queue_t *cosq_array)
{
    int rv;
    int i;
    int max_entries = 16;
    uint32 index;
    void *entries[1];
    ifp_cos_map_entry_t *ent_buf;

    /* Input parameter check. */
    if (!_BCM_COSQ_CLASSIFIER_IS_FIELD(classifier_id)) {
        return (BCM_E_PARAM);
    }

    if (count > max_entries) {
        return (BCM_E_PARAM);
    }

    ent_buf = sal_alloc(sizeof(ifp_cos_map_entry_t) * max_entries,
                        "IFP_COS_MAP entry");
    if (ent_buf == NULL) {
        return (BCM_E_MEMORY);
    }

    sal_memset(ent_buf, 0, sizeof(ifp_cos_map_entry_t) * max_entries);
    entries[0] = ent_buf;
    
    for (i = 0; i < count; i++) {
        if (priority_array[i] < max_entries) {
            soc_mem_field32_set(unit, IFP_COS_MAPm, &ent_buf[priority_array[i]],
                                IFP_COSf, cosq_array[i]);
        }
    }
    
    rv = soc_profile_mem_add(unit, _bcm_kt2_ifp_cos_map_profile[unit],
                             entries, max_entries, &index);
    sal_free(ent_buf);
    if (rv != SOC_E_NONE) {
        return rv;
    }
    return (rv);
}

/*
 * Function:
 *      bcm_kt2_cosq_field_classifier_map_get
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
 */
int
bcm_kt2_cosq_field_classifier_map_get(int unit,
                                      int classifier_id,
                                      int array_max,
                                      bcm_cos_t *priority_array,
                                      bcm_cos_queue_t *cosq_array,
                                      int *array_count)
{
    int rv;
    int i;
    int ent_per_set = 16;
    uint32 index;
    void *entries[1];
    ifp_cos_map_entry_t *ent_buf;

    /* Input parameter check. */
    if (NULL == priority_array || NULL == cosq_array || NULL == array_count) {
        return (BCM_E_PARAM);
    }

    /* Allocate entry buffer. */
    ent_buf = sal_alloc(sizeof(ifp_cos_map_entry_t) * ent_per_set,
                        "IFP_COS_MAP entry");
    if (ent_buf == NULL) {
        return (BCM_E_MEMORY);
    }
    sal_memset(ent_buf, 0, sizeof(ifp_cos_map_entry_t) * ent_per_set);
    entries[0] = ent_buf;

    /* Get profile table entry set base index. */
    index = _BCM_COSQ_CLASSIFIER_FIELD_GET(classifier_id);

    /* Read out entries from profile table into allocated buffer. */
    rv = soc_profile_mem_get(unit, _bcm_kt2_ifp_cos_map_profile[unit],
                             index * ent_per_set, ent_per_set, entries);
    if (!(rv == SOC_E_NOT_FOUND || rv == SOC_E_NONE)) {
        sal_free(ent_buf);
        return rv;
    }

    *array_count = array_max > ent_per_set ? ent_per_set : array_max;

    /* Copy values into API OUT parameters. */
    for (i = 0; i < *array_count; i++) {
        if (priority_array[i] >= 16) {
            sal_free(ent_buf);
            return BCM_E_PARAM;
        }
        cosq_array[i] = soc_mem_field32_get(unit, IFP_COS_MAPm,
                                            &ent_buf[priority_array[i]],
                                            IFP_COSf);
    }

    sal_free(ent_buf);
    return (BCM_E_NONE);
}

/*
 * Function:
 *      bcm_kt2_cosq_field_classifier_map_clear
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
bcm_kt2_cosq_field_classifier_map_clear(int unit, int classifier_id)
{
    int ent_per_set = 16;
    uint32 index;

    /* Get profile table entry set base index. */
    index = _BCM_COSQ_CLASSIFIER_FIELD_GET(classifier_id);

    /* Delete the profile entries set. */
    BCM_IF_ERROR_RETURN
        (soc_profile_mem_delete(unit,
                                _bcm_kt2_ifp_cos_map_profile[unit],
                                index * ent_per_set));
    return (BCM_E_NONE);
}

/*
 * Function:
 *      bcm_kt2_cosq_field_classifier_id_destroy
 * Purpose:
 *      Free resource associated with this field classifier id.
 * Parameters:
 *      unit          - (IN) Unit number.
 *      classifier_id - (IN) Classifier ID
 * Returns:
 *      BCM_E_xxx
 */
int
bcm_kt2_cosq_field_classifier_id_destroy(int unit, int classifier_id)
{
    return (BCM_E_NONE);
}

/*
 * Function:
 *      bcm_kt2_cosq_service_map_set
 * Purpose:
 *      Set the mapping from port, classifier, and multiple internal priorities
 *      to multiple COS queues in a queue group.
 * Parameters:
 *      unit           - (IN) Unit number.
 *      port           - (IN) local port ID
 *      classifier_id  - (IN) Classifier ID
 *      queue_group    - (IN) Base queue ID
 *      array_count    - (IN) Number of elements in priority_array and
 *                            cosq_array
 *      priority_array - (IN) Array of internal priorities
 *      cosq_array     - (IN) Array of COS queues
 * Returns:
 *      BCM_E_xxx
 */
int
bcm_kt2_cosq_service_map_set(
    int unit,
    bcm_port_t port,
    int classifier_id,
    bcm_gport_t queue_group,
    int count,
    bcm_cos_t *priority,
    bcm_cos_queue_t *cosq)
{
    int rv = BCM_E_NONE;
    int i;
    service_cos_map_entry_t scm;
    ing_queue_offset_mapping_table_entry_t map_entries[16];
    void *entries[1];
    uint32 old_index = 0;
    uint32 new_index = 0;
    int vid;
    int valid_entry;
    _bcm_kt2_cosq_node_t *node;

    if (!_BCM_COSQ_CLASSIFIER_IS_SERVICE(classifier_id)) {
        return BCM_E_PARAM;
    }

    /* index above 4K is for VFI */
    vid = _BCM_COSQ_CLASSIFIER_SERVICE_GET(classifier_id);
    
    /* validate the vlan id */
    if (vid < 0 || vid >= SOC_MEM_SIZE(unit, SERVICE_COS_MAPm)) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN(_bcm_kt2_cosq_node_get(unit, queue_group, NULL, 
                                               NULL, NULL, &node));

    if (node->cosq_attached_to < 0) {
        return BCM_E_PARAM;
    }    

    /* validate the user parameters */
    if (count > 16) {
        return BCM_E_PARAM;
    }

    /* queues allocated for this queue_group should be contiguous.*/
    /* check if user's queue number is valid */
    for (i = 0; i < count; i++) {
        if (cosq[i] >= 8 || priority[i] >= 16) {
            return BCM_E_PARAM;
        }
    }

    sal_memset(map_entries, 0, sizeof(map_entries));

    BCM_IF_ERROR_RETURN(
        soc_mem_read(unit, SERVICE_COS_MAPm, MEM_BLOCK_ANY, (int)vid, &scm));
    valid_entry = soc_mem_field32_get(unit, SERVICE_COS_MAPm, &scm, VALIDf);

    entries[0] = &map_entries;
    if (valid_entry) {
        old_index = soc_mem_field32_get(unit, SERVICE_COS_MAPm, &scm,
                                    QUEUE_OFFSET_PROFILE_INDEXf);
        old_index *= 16;

        BCM_IF_ERROR_RETURN
            (_bcm_offset_map_table_entry_get(unit, old_index, 16, entries));
        rv = _bcm_offset_map_table_entry_delete(unit, old_index);
        if (!(rv == SOC_E_NOT_FOUND || rv == SOC_E_NONE)) {
            return rv;
        }
    } else {
        soc_mem_field32_set(unit, SERVICE_COS_MAPm, &scm, VALIDf, 1);
    }

    for (i = 0; i < count; i++) {
        soc_mem_field32_set(unit, ING_QUEUE_OFFSET_MAPPING_TABLEm, 
                &map_entries[priority[i]], QUEUE_OFFSETf, cosq[i]);
    }

    BCM_IF_ERROR_RETURN
        (_bcm_offset_map_table_entry_add(unit, entries, 16, &new_index));
    
    soc_mem_field32_set(unit, SERVICE_COS_MAPm, &scm, QUEUE_OFFSET_PROFILE_INDEXf,
                        new_index / 16);
    soc_mem_field32_set(unit, SERVICE_COS_MAPm, &scm, SERVICE_COSf,
                        node->hw_index);
    BCM_IF_ERROR_RETURN(
        soc_mem_write(unit, SERVICE_COS_MAPm, MEM_BLOCK_ANY, (int)vid, &scm));

    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_kt2_cosq_service_classifier_get
 * Purpose:
 *      Get info about an endpoint.
 * Parameters:
 *      unit          - (IN) Unit number.
 *      classifier_id - (IN) Classifier ID
 *      classifier    - (OUT) Classifier info
 * Returns:
 *      BCM_E_xxx
 */
int
bcm_kt2_cosq_service_classifier_get(
    int unit,
    int classifier_id,
    bcm_cosq_classifier_t *classifier)
{
    sal_memset(classifier, 0, sizeof(bcm_cosq_classifier_t));
    classifier->flags = BCM_COSQ_CLASSIFIER_VLAN;
    classifier->vlan  = _BCM_COSQ_CLASSIFIER_SERVICE_GET(classifier_id);
    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_kt2_cosq_service_map_get
 * Purpose:
 *      Get the mapping from port, classifier, and multiple internal priorities
 *      to multiple COS queues in a queue group.
 * Parameters:
 *      unit           - (IN) Unit number.
 *      port           - (IN) Local port ID
 *      classifier_id  - (IN) Classifier ID
 *      queue_group    - (OUT) Queue group
 *      array_max      - (IN) Size of priority_array and cosq_array
 *      priority_array - (IN) Array of internal priorities
 *      cosq_array     - (OUT) Array of COS queues
 *      array_count    - (OUT) Size of cosq_array
 * Returns:
 *      BCM_E_xxx
 */
int
bcm_kt2_cosq_service_map_get(
    int unit,
    bcm_port_t port,
    int classifier_id,
    bcm_gport_t *queue_group,
    int array_max,
    bcm_cos_t *priority,
    bcm_cos_queue_t *cosq,
    int *array_count)
{
    service_cos_map_entry_t scm;
    ing_queue_offset_mapping_table_entry_t map_entries[16];
    void *entries[1];
    int index;
    int vid;
    int valid_entry;
    int i;
    int cos_offset;

    vid = _BCM_COSQ_CLASSIFIER_SERVICE_GET(classifier_id);

    BCM_IF_ERROR_RETURN(
        soc_mem_read(unit, SERVICE_COS_MAPm, MEM_BLOCK_ANY, (int)vid, &scm));

    valid_entry = soc_mem_field32_get(unit, SERVICE_COS_MAPm, &scm, VALIDf);
    if (!valid_entry) {
        return BCM_E_CONFIG;  /* service queue is not configured */
    }

    index = soc_mem_field32_get(unit, SERVICE_COS_MAPm, &scm,
                                QUEUE_OFFSET_PROFILE_INDEXf);
    index *= 16;
    entries[0] = &map_entries;
    BCM_IF_ERROR_RETURN
        (_bcm_offset_map_table_entry_get(unit, index, 16, entries));
    
    cos_offset = soc_mem_field32_get(unit, SERVICE_COS_MAPm,
                                     &scm, SERVICE_COSf);
    BCM_GPORT_UCAST_SUBSCRIBER_QUEUE_GROUP_QID_SET(*queue_group, cos_offset);
    *queue_group |= (port << 16);
    *array_count = array_max > 16 ? 16 :
                   array_max;
                   
    for (i = 0; i < *array_count; i++) {
        if (priority[i] >= 16) {
            return BCM_E_PARAM;
        }
        cosq[i] = soc_mem_field32_get(unit,ING_QUEUE_OFFSET_MAPPING_TABLEm,
                               &map_entries[priority[i]], QUEUE_OFFSETf);
    }
 
    return BCM_E_NONE;      
}

/*
 * Function:
 *      bcm_kt2_cosq_service_map_clear
 * Purpose:
 *      Clear the mapping from port, classifier, and internal priorities
 *      to COS queues in a queue group.
 * Parameters:
 *      unit          - (IN) Unit number.
 *      port          - (IN) Local port ID
 *      classifier_id - (IN) Classifier ID
 * Returns:
 *      BCM_E_xxx
 */
int
bcm_kt2_cosq_service_map_clear(
    int unit,
    bcm_gport_t port,
    int classifier_id)
{
    int rv = BCM_E_NONE;
    service_cos_map_entry_t scm;
    int index;
    int vid;
    int valid_entry;

    vid = _BCM_COSQ_CLASSIFIER_SERVICE_GET(classifier_id);

    BCM_IF_ERROR_RETURN(soc_mem_read(unit, SERVICE_COS_MAPm, MEM_BLOCK_ANY,
                        (int)vid, &scm));

    valid_entry = soc_mem_field32_get(unit, SERVICE_COS_MAPm, &scm, VALIDf);
    if (!valid_entry) {
        return BCM_E_NONE;  /* service queue is already cleared  */
    }

    index = soc_mem_field32_get(unit, SERVICE_COS_MAPm, &scm,
                               QUEUE_OFFSET_PROFILE_INDEXf);
    index *= 16;

    /* delete the profile */
    rv = _bcm_offset_map_table_entry_delete(unit, index);
    if (!(rv == SOC_E_NOT_FOUND || rv == SOC_E_NONE)) {
        return rv;
    }

    sal_memset(&scm,0, sizeof(service_cos_map_entry_t));

    /* Write back updated buffer. */
    BCM_IF_ERROR_RETURN(soc_mem_write(unit, SERVICE_COS_MAPm, MEM_BLOCK_ALL,
                       (int)vid, &scm));
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_kt2_cosq_gport_dmvoq_config_set 
 * Purpose:
 *      Setting SOURCE_MODMAP , FC table and BASEINDEX table
 *      appropriately as per the architecture
 * Parameters:
 *      unit           - (IN) Unit number.
 *      gport          - (IN) gport identifier 
 *      cos            - (IN) cos value 
 *      fabric_modid   - (IN) Fabric Mod ID
 *      dest_modid     - (IN) Destination Mod ID
 *      fabric_port    - (IN) Fabric port ID
 * Returns:
 *      BCM_E_xxx
 */
STATIC int
_bcm_kt2_cosq_gport_dmvoq_config_set(int unit, bcm_gport_t gport, 
                                bcm_cos_queue_t cosq,
                                bcm_module_t dest_modid, 
                                bcm_module_t fabric_modid,
                                bcm_port_t fabric_port)
{
    int  intf_index, rv;
    _bcm_kt2_cosq_node_t *node;
    _bcm_kt2_cosq_node_t *base_node = NULL;
    bcm_port_t local_port;
    mmu_intfi_base_index_tbl_entry_t base_tbl_entry;
    uint32  index;
    uint32  val;
    uint32  modport_map_index;
    modport_map_sw_entry_t sw_entry;
    ing_queue_map_entry_t ing_queue_entry;
    ing_queue_offset_mapping_table_entry_t map_entries[8];
    void *entries[1];
    int map_offset = 0, fabric_port_count, port;
    int count = 0, cng_offset;
    mmu_intfi_offset_map_tbl_entry_t offset_map_tbl_entry; 
    _bcm_kt2_mmu_info_t *mmu_info;
    entries[0] = &map_entries;
    mmu_info = _bcm_kt2_mmu_info[unit];

    if (fabric_modid >= 64) {
        return BCM_E_PARAM;
    }

    if (dest_modid >= 256) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_node_get(unit, gport, NULL ,&local_port, NULL, &node));

    if (!node) {
        return BCM_E_NOT_FOUND;
    }
    if (node->level != _BCM_KT2_COSQ_NODE_LEVEL_L2) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN(_bcm_kt2_voq_base_node_get(
                unit, dest_modid, &base_node));
    if (base_node == NULL) {
        base_node = node ;
    }
    BCM_PBMP_COUNT(base_node->fabric_pbmp, fabric_port_count);
    if (fabric_port_count == 0) {
        map_offset = mmu_info->curr_merger_index;
        if (map_offset >= soc_mem_index_count(unit, MMU_INTFI_MERGE_ST_TBLm)) {
            return BCM_E_RESOURCE;
        }
    } else {
        BCM_PBMP_ITER(base_node->fabric_pbmp, port) {
            BCM_IF_ERROR_RETURN(soc_mem_read(unit, MMU_INTFI_OFFSET_MAP_TBLm, 
                        MEM_BLOCK_ALL, 
                        port , &offset_map_tbl_entry));
            map_offset = soc_mem_field32_get(unit, MMU_INTFI_OFFSET_MAP_TBLm,
                    &offset_map_tbl_entry,
                    MAP_OFFSETf);
            break;
        }
    }
    if (!BCM_PBMP_MEMBER(base_node->fabric_pbmp, fabric_port)) {
        if (map_offset == mmu_info->curr_merger_index ) {
            mmu_info->curr_merger_index++;
        }

        BCM_PBMP_PORT_ADD (base_node->fabric_pbmp, fabric_port);
        BCM_PBMP_COUNT(base_node->fabric_pbmp, fabric_port_count);
        BCM_PBMP_ITER(base_node->fabric_pbmp, port) {
            count++;
            BCM_IF_ERROR_RETURN(soc_mem_read(unit, MMU_INTFI_OFFSET_MAP_TBLm, 
                        MEM_BLOCK_ALL, 
                        port, &offset_map_tbl_entry));
            soc_mem_field32_set(unit, MMU_INTFI_OFFSET_MAP_TBLm, 
                    &offset_map_tbl_entry,
                    MAP_OFFSETf, map_offset);
            soc_mem_field32_set(unit, MMU_INTFI_OFFSET_MAP_TBLm, 
                    &offset_map_tbl_entry,
                    LASTf, count < fabric_port_count ? 0 : 1 );
            BCM_IF_ERROR_RETURN(soc_mem_write(unit, MMU_INTFI_OFFSET_MAP_TBLm, 
                        MEM_BLOCK_ALL, 
                        port , &offset_map_tbl_entry));
        }
    }



    if (node->hw_index % 8 == 0) {

        node->remote_modid = dest_modid;
        SOC_IF_ERROR_RETURN
            (READ_MODPORT_MAP_SELr(unit, local_port, &val));

        index = soc_reg_field_get(unit, MODPORT_MAP_SELr, val,
                MODPORT_MAP_INDEX_UPPERf);

        modport_map_index  = (index * 256) + dest_modid;

        soc_mem_lock(unit, MODPORT_MAP_SWm);
        rv = soc_mem_read(unit, MODPORT_MAP_SWm, MEM_BLOCK_ANY, modport_map_index,
                &sw_entry);
        if (SOC_SUCCESS(rv)) {
            soc_mem_field32_set(unit, MODPORT_MAP_SWm, &sw_entry, DEST0f,
                    local_port);
            soc_mem_field32_set(unit, MODPORT_MAP_SWm, &sw_entry, ENABLE0f, 1);
            soc_mem_field32_set(unit, MODPORT_MAP_SWm, &sw_entry, PER_MODID_QUEUEING_ENABLEf,
                    1);
            soc_mem_field32_set(unit, MODPORT_MAP_SWm, &sw_entry, ISTRUNK0f,0);
            soc_mem_field32_set(unit, MODPORT_MAP_SWm, &sw_entry, ADD_DESTINATION_PORTf,
                    0);
            soc_mem_field32_set(unit, MODPORT_MAP_SWm, &sw_entry, QUEUE_MAP_POINTERf,
                    node->parent->base_index);
            rv = soc_mem_write(unit, MODPORT_MAP_SWm, MEM_BLOCK_ALL, modport_map_index,
                    &sw_entry);
        }
        soc_mem_unlock(unit, MODPORT_MAP_SWm);

        sal_memset(map_entries, 0, sizeof(map_entries));
        /* Hardcoding the ING_QUEUE_OFFSET_MAPPING_TABLE */
        for (index = 0; index < 8; index++) {
            soc_mem_field32_set(unit, ING_QUEUE_OFFSET_MAPPING_TABLEm,
                    &map_entries[index], QUEUE_OFFSETf, index);
        }
        BCM_IF_ERROR_RETURN
            (_bcm_offset_map_table_entry_add(unit, entries, 16, &index));

        sal_memset(&ing_queue_entry, 0, sizeof(ing_queue_map_entry_t));
        soc_mem_field32_set(unit, ING_QUEUE_MAPm,
                &ing_queue_entry, QUEUE_SET_BASEf, node->parent->base_index);
        soc_mem_field32_set(unit, ING_QUEUE_MAPm, &ing_queue_entry,
                QUEUE_OFFSET_PROFILE_INDEXf, index/16);
        BCM_IF_ERROR_RETURN(BCM_XGS3_MEM_WRITE(unit, ING_QUEUE_MAPm,
                    node->parent->base_index, &ing_queue_entry));

        intf_index = _bcm_kt2_cosq_source_intf_set(unit, fabric_modid, 
                0, _BCM_KT2_COSQ_DMVOQ);
        if (intf_index < 0) {
            return BCM_E_RESOURCE;
        }

        /*
         *  SRC_INTF and MMU_INTFI_BASE_INDEX_TBL is mapped one to one,
         *  so writing the base index at same index where SRC_INTF is written
         */

        cng_offset = intf_index * 64 ;

        BCM_IF_ERROR_RETURN
            (soc_mem_read(unit, MMU_INTFI_BASE_INDEX_TBLm, 
                          MEM_BLOCK_ALL, intf_index, &base_tbl_entry));
        soc_mem_field32_set(unit, MMU_INTFI_BASE_INDEX_TBLm, &base_tbl_entry, 
                BASE_ADDRf, cng_offset);
        soc_mem_field32_set(unit, MMU_INTFI_BASE_INDEX_TBLm, &base_tbl_entry, 
                ENf, (node->level - 3 ));

        BCM_IF_ERROR_RETURN(soc_mem_write(unit, MMU_INTFI_BASE_INDEX_TBLm,
                    MEM_BLOCK_ALL, intf_index, &base_tbl_entry));

        /* set the mapping from congestion bits in flow control table
         * to corresponding hw_index */
        BCM_IF_ERROR_RETURN(_bcm_kt2_cosq_map_fc_status_to_node(unit, 
                    _BCM_KT2_COSQ_FC_VOQFC,  
                    node->hw_index, (node->level - 2), cng_offset + map_offset));


    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_kt2_cosq_gport_congestion_config_set 
 * Purpose:
 *      Setting the dmvoq/dpvoq related configuration
 * Parameters:
 *      unit           - (IN) Unit number.
 *      gport          - (IN) gport identifier 
 *      cos            - (IN) cos value 
 *      config         - (IN) congestion data 
 * Returns:
 *      BCM_E_xxx
 */

int bcm_kt2_cosq_gport_congestion_config_set(int unit, bcm_gport_t gport, 
                                         bcm_cos_queue_t cosq, 
                                         bcm_cosq_congestion_info_t *config)
{
    if (!config) {
        return BCM_E_PARAM;
    }

    if ((config->dest_modid != -1)  && 
            (config->fabric_port != -1) &&
            (config->fabric_modid != -1)) {
        return _bcm_kt2_cosq_gport_dmvoq_config_set(unit, gport, cosq, 
                                               config->dest_modid,
                                               config->fabric_modid,
                                               config->fabric_port);
    } 
    return BCM_E_NONE;
}

/*
 * Function:
 *      bcm_kt2_cosq_gport_congestion_config_set 
 * Purpose:
 *      Setting the dmvoq/dpvoq related configuration
 * Parameters:
 *      unit           - (IN) Unit number.
 *      gport          - (IN) gport identifier 
 *      cos            - (IN) cos value 
 *      config         - (IN) congestion data 
 * Returns:
 *      BCM_E_xxx
 */


int bcm_kt2_cosq_gport_congestion_config_get(int unit, bcm_gport_t port, 
                                         bcm_cos_queue_t cosq, 
                                         bcm_cosq_congestion_info_t *config)
{
    _bcm_kt2_cosq_node_t *node;

    if (!config) {
        return BCM_E_PARAM;
    }

    BCM_IF_ERROR_RETURN
      (_bcm_kt2_cosq_node_get(unit, port, NULL, NULL, NULL, &node));


    if (!node) {
        return BCM_E_NOT_FOUND;
    }

    config->dest_modid = node->remote_modid;
    config->dest_port = node->remote_port;

    return BCM_E_NONE;
}

#ifdef BCM_WARM_BOOT_SUPPORT_SW_DUMP

STATIC int
_bcm_kt2_cosq_node_info_dump(int unit, _bcm_kt2_cosq_node_t *node) 
{
    LOG_CLI((BSL_META_U(unit,
                        "base = %d numq = %d hw_index = %d \n"),
             node->base_index, node->numq, node->hw_index));
    LOG_CLI((BSL_META_U(unit,
                        "level = %d cosq_attach = %d \n"),
             node->level, node->cosq_attached_to));
    LOG_CLI((BSL_META_U(unit,
                        "num_child = %d first_child = %d wrr_in_use = %d \n"), 
             node->num_child, node->first_child,
             node->wrr_in_use));
    LOG_CLI((BSL_META_U(unit,
                        "subport_enabled = %d linkphy_enabled = %d \n"),
             node->subport_enabled,
             node->linkphy_enabled)); 
    if (node->parent != NULL) {
        LOG_CLI((BSL_META_U(unit,
                            "  parent gport = 0x%08x\n"), node->parent->gport));
    }
    if (node->sibling != NULL) {
        LOG_CLI((BSL_META_U(unit,
                            "  sibling gport = 0x%08x\n"), node->sibling->gport));
    }
    if (node->child != NULL) {
        LOG_CLI((BSL_META_U(unit,
                            "  child  gport = 0x%08x\n"), node->child->gport));
    }

    return BCM_E_NONE;
}

STATIC int
_bcm_kt2_cosq_l0_tree_dump(int unit, _bcm_kt2_cosq_node_t *node) 
{
    _bcm_kt2_cosq_node_t *l0_node, *l1_node, *l2_node;

    for (l0_node = node; l0_node != NULL;
         l0_node = l0_node->sibling) {
         LOG_CLI((BSL_META_U(unit,
                             "=== L0 gport 0x%08x\n"), l0_node->gport));
         BCM_IF_ERROR_RETURN
             (_bcm_kt2_cosq_node_info_dump(unit, l0_node));

        for (l1_node = l0_node->child; l1_node != NULL;
             l1_node = l1_node->sibling) {
              LOG_CLI((BSL_META_U(unit,
                                  "=== L1 gport 0x%08x\n"), l1_node->gport));
              BCM_IF_ERROR_RETURN
                  (_bcm_kt2_cosq_node_info_dump(unit, l1_node));
                
            for (l2_node = l1_node->child; l2_node != NULL;
                 l2_node = l2_node->sibling) {
                 LOG_CLI((BSL_META_U(unit,
                                     "=== L2 gport 0x%08x\n"), l2_node->gport));
                 BCM_IF_ERROR_RETURN
                      (_bcm_kt2_cosq_node_info_dump(unit, l2_node));
            }
        }
    }

    return BCM_E_NONE;
}

STATIC int
_bcm_kt2_cosq_port_info_dump(int unit, bcm_port_t port) 
{
    _bcm_kt2_mmu_info_t *mmu_info;
    _bcm_kt2_cosq_node_t *port_node = NULL;
    _bcm_kt2_cosq_node_t *l0_node = NULL;
    _bcm_kt2_cosq_node_t *s0_node = NULL;
    _bcm_kt2_cosq_node_t *s1_node = NULL;

    if (!SOC_PORT_VALID(unit, port)) {
        return BCM_E_PORT;
    }

    mmu_info = _bcm_kt2_mmu_info[unit];

    /* get the root node */
    port_node = &mmu_info->sched_node[port];

    if (port_node && port_node->cosq_attached_to < 0) {
        return BCM_E_NONE;
    }    

    LOG_CLI((BSL_META_U(unit,
                        "=== port %d\n"), port));
    BCM_IF_ERROR_RETURN
        (_bcm_kt2_cosq_node_info_dump(unit, port_node));

    s0_node = port_node->child;
    if (s0_node != NULL && 
        s0_node->level == _BCM_KT2_COSQ_NODE_LEVEL_S0) {
        for (s0_node = port_node->child; s0_node != NULL;
             s0_node = s0_node->sibling) {
             LOG_CLI((BSL_META_U(unit,
                                 "=== S0 gport 0x%08x\n"), s0_node->gport));
             BCM_IF_ERROR_RETURN
                 (_bcm_kt2_cosq_node_info_dump(unit, s0_node));

             s1_node = s0_node->child;
             if (s1_node != NULL && 
                 s1_node->level == _BCM_KT2_COSQ_NODE_LEVEL_S1) {
                 for (s1_node = s0_node->child; s1_node != NULL;
                      s1_node = s1_node->sibling) {
                      LOG_CLI((BSL_META_U(unit,
                                          "=== S1 gport 0x%08x\n"), s1_node->gport));
                      BCM_IF_ERROR_RETURN
                          (_bcm_kt2_cosq_node_info_dump(unit, s1_node));
                      for (l0_node = s1_node->child; l0_node != NULL;
                           l0_node = l0_node->sibling) {
                           BCM_IF_ERROR_RETURN
                                (_bcm_kt2_cosq_l0_tree_dump(unit, l0_node));
                        }
                    }
             } else {
                 l0_node = s1_node;
                 BCM_IF_ERROR_RETURN
                     (_bcm_kt2_cosq_l0_tree_dump(unit, l0_node));
            }
        }
    } else {
        l0_node = port_node->child;
        BCM_IF_ERROR_RETURN
            (_bcm_kt2_cosq_l0_tree_dump(unit, l0_node));
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *     bcm_kt2_cosq_sw_dump
 * Purpose:
 *     Displays COS Queue information maintained by software.
 * Parameters:
 *     unit - Device unit number
 * Returns:
 *     None
 */
void
bcm_kt2_cosq_sw_dump(int unit)
{
    _bcm_kt2_mmu_info_t *mmu_info;
    bcm_port_t port;
    int i;

    mmu_info = _bcm_kt2_mmu_info[unit];

    LOG_CLI((BSL_META_U(unit,
                        "\nSW Information COSQ - Unit %d\n"), unit));

    for (port = 0; port <= KT2_LPBK_PORT; port++) {
        (void)_bcm_kt2_cosq_port_info_dump(unit, port);
    }

    LOG_CLI((BSL_META_U(unit,
                        "ext_qlist = %d \n"), mmu_info->ext_qlist.count));
    LOG_CLI((BSL_META_U(unit,
                        "sched_list = %d \n"), mmu_info->sched_list.count));
    LOG_CLI((BSL_META_U(unit,
                        "l2_base_qlist = %d \n"), mmu_info->l2_base_qlist.count));
    LOG_CLI((BSL_META_U(unit,
                        "l2_base_mcqlist = %d \n"), mmu_info->l2_base_mcqlist.count));
    LOG_CLI((BSL_META_U(unit,
                        "l2_ext_qlist = %d \n"), mmu_info->l2_ext_qlist.count));
   
    for (i = 0; i < _BCM_KT2_COSQ_NODE_LEVEL_MAX; i++) {
        LOG_CLI((BSL_META_U(unit,
                            "Level %d count = %d \n"), 
                 i, mmu_info->sched_hw_list[i].count));
    }
    
    return;
}
#endif /* BCM_WARM_BOOT_SUPPORT_SW_DUMP */

#else /* BCM_KATANA2_SUPPORT */
int _kt2_cosq_not_empty;
#endif  /* BCM_KATANA2_SUPPORT */ 