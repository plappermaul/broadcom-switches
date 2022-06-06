/*
 * $Id: $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * Katana2 STG support
 *
 * These functions set or get port related fields in the Spanning tree
 * table.
 */

#include <shared/bsl.h>

#include <soc/drv.h>
#include <soc/mem.h>
#include <soc/ptable.h>         /* PVP_* defines */

#include <bcm/types.h>
#include <bcm/error.h>
#include <bcm/stg.h>

#include <bcm_int/esw/stg.h>

#define STG_BITS_PER_PORT       2
#define STG_PORT_MASK           ((1 << STG_BITS_PER_PORT)-1)
#define STG_PORTS_PER_WORD      (32 / STG_BITS_PER_PORT)
#define STG_WORD(port)          ((port) / STG_PORTS_PER_WORD)
#define STG_BITS_SHIFT(port)    \
        (STG_BITS_PER_PORT * ((port) % STG_PORTS_PER_WORD))
#define STG_BITS_MASK(port)     (STG_PORT_MASK << (STG_BITS_SHIFT(port)))

/*
 * Function:
 *      _bcm_kt2_stg_stp_init
 * Purpose:
 *      Initialize spanning tree group on a single device. 
 * Parameters:
 *      unit - (IN)SOC unit number.
 *      stg  - (IN)Spanning tree group id.
 *      mem  - (IN)Spanning tree group table memory.
 * Returns:  
 *      BCM_E_XXX
 */
STATIC int
_bcm_kt2_stg_stp_init(int unit, bcm_stg_t stg, soc_mem_t mem)
{
    uint32 entry[SOC_MAX_MEM_WORDS];    /* Spanning tree port state map. */
    int stp_state;              /* STP port state.               */
    bcm_pbmp_t stacked;         /* Bitmap of stacked ports.      */
    bcm_port_t port;            /* Port iterator.                */

    /* Set all ports to PVP_STP_DISABLED */
    sal_memset(entry, 0, sizeof(entry));

    /* Get all stacking ports and set them into forwarding */
    BCM_PBMP_ASSIGN(stacked, PBMP_ST_ALL(unit));
    BCM_PBMP_OR(stacked, SOC_PBMP_STACK_CURRENT(unit));

    stp_state = PVP_STP_FORWARDING;

#ifdef BCM_RCPU_SUPPORT
    if (SOC_IS_RCPU_ONLY(unit)) {
        if (BCM_STG_DEFAULT == stg) {
            PBMP_ALL_ITER(unit, port) {
                entry[STG_WORD(port)] |= stp_state << STG_BITS_SHIFT(port);
            }
        }
    }
#endif /* BCM_RCPU_SUPPORT */

    /* Iterate over stacking ports & set stp state. */
    PBMP_ITER(stacked, port) {
        entry[STG_WORD(port)] |= stp_state << STG_BITS_SHIFT(port);
    }

    /* Write spanning tree group port states to hw. */
    BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ALL, stg, entry));

    return (BCM_E_NONE);
}

/*
 * Function:
 *      _bcm_kt2_stg_stp_set
 * Purpose:
 *      Set the spanning tree state for a port in specified STG.
 * Parameters:
 *      unit      - (IN)SOC unit number.
 *      stg       - (IN)Spanning tree group id.
 *      port      - (IN)StrataSwitch port number.
 *      stp_state - (IN)STP state to place port in.
 *      mem       - (IN)Spanning tree group table memory.
 * Returns: 
 *      BCM_E_XXX
 */
STATIC int
_bcm_kt2_stg_stp_set(int unit, bcm_stg_t stg, bcm_port_t port,
                      int stp_state, soc_mem_t mem)
{
    uint32 entry[SOC_MAX_MEM_WORDS];    /* STP group ports state map.   */
    int hw_stp_state;           /* STP port state in hw format. */
    int rv;                     /* Operation return status.     */

    /* Input parameters verification. */
    if (!IS_LB_PORT(unit, port)) {
        if (BCM_PBMP_MEMBER(SOC_INFO(unit).linkphy_pp_port_pbm, port) ||
            BCM_PBMP_MEMBER(SOC_INFO(unit).subtag_pp_port_pbm, port)) {
            if (!SOC_PP_PORT_VALID(unit, port)) {
                return BCM_E_PORT;
            }
        } else {
            if (!SOC_PORT_VALID(unit, port) || !IS_PORT(unit, port)) {
                return (BCM_E_PORT);
            }
        }
    }

    /* Translate API space port state to hw stp port state. */
    BCM_IF_ERROR_RETURN(_bcm_stg_stp_translate(unit, stp_state, &hw_stp_state));

    soc_mem_lock(unit, mem);
    /* Read ports states for the stp group. */
    rv = soc_mem_read(unit, mem, MEM_BLOCK_ANY, stg, entry);
    if (BCM_FAILURE(rv)) {
        LOG_ERROR(BSL_LS_BCM_L2,
                  (BSL_META_U(unit,
                              "Error: (%d) reading port states for stg(%d)\n"), rv, stg));
        soc_mem_unlock(unit, mem);
        return rv;
    }

    /* Reset port current state bit. */
    entry[STG_WORD(port)] &= ~(STG_BITS_MASK(port));

    /* Set new port state. */
    entry[STG_WORD(port)] |= (hw_stp_state << STG_BITS_SHIFT(port));

    /* Write spanning tree group port states to hw. */
    rv = soc_mem_write(unit, mem, MEM_BLOCK_ALL, stg, entry);
    soc_mem_unlock(unit, mem);

    return rv;
}

/*
 * Function:
 *      _bcm_kt2_stg_stp_get
 * Purpose:
 *      Retrieve the spanning tree state for a port in specified STG.
 * Parameters:
 *      unit      - (IN)SOC unit number.
 *      stg       - (IN)Spanning tree group id.
 *      port      - (IN)StrataSwitch port number.
 *      stp_state - (IN/OUT)Port STP state.
 *      mem       - (IN)Spanning tree group table memory.
 * Returns: 
 *      BCM_E_XXX
 */
STATIC int
_bcm_kt2_stg_stp_get(int unit, bcm_stg_t stg, bcm_port_t port,
                      int *stp_state, soc_mem_t mem)
{
    uint32 entry[SOC_MAX_MEM_WORDS];    /* STP group ports state map.   */
    int hw_stp_state;           /* STP port state in hw format. */
    int rv;                     /* Operation return status.     */

    /* Input parameters check. */
    if (!IS_LB_PORT(unit, port)) {
        if (BCM_PBMP_MEMBER(SOC_INFO(unit).linkphy_pp_port_pbm, port) ||
            BCM_PBMP_MEMBER(SOC_INFO(unit).subtag_pp_port_pbm, port)) {
            if (!SOC_PP_PORT_VALID(unit, port)) {
                return BCM_E_PORT;
            }
        } else {
            if (!SOC_PORT_VALID(unit, port) || !IS_PORT(unit, port)) {
                return (BCM_E_PORT);
            }
        }
    }

    soc_mem_lock(unit, mem);
    rv = soc_mem_read(unit, mem, MEM_BLOCK_ANY, stg, entry);
    soc_mem_unlock(unit, mem);
    if (BCM_FAILURE(rv)) {
        return rv;
    }
    /* Get specific port state from the entry. */
    hw_stp_state = entry[STG_WORD(port)] >> STG_BITS_SHIFT(port);
    hw_stp_state &= STG_PORT_MASK;

    /* Translate hw stp port state to API format. */
    BCM_IF_ERROR_RETURN(_bcm_stg_pvp_translate(unit, hw_stp_state, stp_state));

    return (BCM_E_NONE);
}

/*
 * Function:
 *      _bcm_kt2_stg_stp_init
 * Purpose:
 *      Initialize spanning tree group on a single device. 
 * Parameters:
 *      unit - (IN)SOC unit number.
 *      stg  - (IN)Spanning tree group id.
 * Returns:  
 *      BCM_E_XXX
 */
int
bcm_kt2_stg_stp_init(int unit, bcm_stg_t stg)
{
    BCM_IF_ERROR_RETURN(_bcm_kt2_stg_stp_init(unit, stg, STG_TABm));
    if (SOC_IS_FBX(unit)) {
        BCM_IF_ERROR_RETURN(_bcm_kt2_stg_stp_init(unit, stg, EGR_VLAN_STGm));
    }
    return (BCM_E_NONE);
}

/*
 * Function:
 *      bcm_kt2_stg_stp_set
 * Purpose:
 *      Set the spanning tree state for a port in specified STG.
 * Parameters:
 *      unit      - (IN)SOC unit number.
 *      stg       - (IN)Spanning tree group id.
 *      port      - (IN)StrataSwitch port number.
 *      stp_state - (IN)STP state to place port in.
 * Returns: 
 *      BCM_E_XXX
 */
int
bcm_kt2_stg_stp_set(int unit, bcm_stg_t stg,
                         bcm_port_t port, int stp_state)
{
    BCM_IF_ERROR_RETURN
        (_bcm_kt2_stg_stp_set(unit, stg, port, stp_state, STG_TABm));
    BCM_IF_ERROR_RETURN
        (_bcm_kt2_stg_stp_set(unit, stg, port, stp_state, EGR_VLAN_STGm));
    return (BCM_E_NONE);
}

/*
 * Function:
 *      bcm_kt2_stg_stp_get
 * Purpose:
 *      Retrieve the spanning tree state for a port in specified STG.
 * Parameters:
 *      unit      - (IN)SOC unit number.
 *      stg       - (IN)Spanning tree group id.
 *      port      - (IN)StrataSwitch port number.
 *      stp_state - (IN/OUT)Port STP state int the group.
 * Returns: 
 *      BCM_E_XXX
 */
int
bcm_kt2_stg_stp_get(int unit, bcm_stg_t stg,
                         bcm_port_t port, int *stp_state)
{
    /* Input parameters check. */
    if (!stp_state) {
        return (BCM_E_PARAM);
    }

    /* Get port state in specified group. */
    return _bcm_kt2_stg_stp_get(unit, stg, port, stp_state, STG_TABm);
}

