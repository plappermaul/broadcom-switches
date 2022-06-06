/* 
 * $Id: utils.h,v 1.10 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        bcm_sand.h
 * Purpose:     Conversion between BCM and DNX_SAND types, and common macros/function for
 *              handling Dune's code.
 */

#ifndef INCLUDE_DNX_UTILS_H
#define INCLUDE_DNX_UTILS_H

#include <bcm/error.h>
#include <bcm/types.h>
#include <bcm_int/dnx/legacy/cosq.h>
#include <bcm_int/dnx/legacy/multicast.h>
#include <soc/dnx/legacy/drv.h>
#include <soc/dnx/legacy/SAND/Utils/sand_pp_mac.h>
#include <soc/dnx/legacy/SAND/Management/sand_error_code.h>
#include <soc/dnx/legacy/TMC/tmc_api_general.h>
#include <soc/dnx/legacy/SAND/Utils/sand_multi_set.h>

#define BCM_SAND_FAP_ID_TO_BCM_MODID(fap_id) (int)(fap_id)
#define BCM_SAND_FAP_PORT_ID_TO_BCM_PORT(fap_port_id) (int)(fap_port_id)

#define _BCM_DNX_CPU_DEST_PORT (0)

/* snoop comamnd reserved for no snooping */
/* snoop comamnd reserved for snoop all packet to CPU*/
#define _BCM_DNX_SNOOP_CMD_TO_CPU (1)

/* size of snooped packet (all packet) */
#define _BCM_DNX_SNOOP_SIZE (DNX_TMC_ACTION_CMD_SIZE_BYTES_ALL_PCKT)

/* Destination of snooped packet */
#define _BCM_DNX_SNOOP_DEST (_BCM_DNX_CPU_DEST_PORT)

#define FRAGMENT_SIZE                   (32)


/*
 * General Utility Macros
 */
#define BCM_DNX_UNIT_CHECK(unit) \
    do { \
    if (((unit) < 0) || ((unit) >= BCM_LOCAL_UNITS_MAX)) { BCM_RETURN_VAL_EXIT(BCM_E_UNIT); }  \
    } while (0)
    

#define BCM_DNX_MIN_CHECK(arg, min) \
    do {   \
        if ((arg) < min) { return BCM_E_PARAM; } \
    } while (0)



/* Convert PBMP to one single port and verify that PBMP is with one member only */
#define DNX_PBMP_TO_SINGLE_PORT_GET(_pbmp, _local_port) \
    do { \
        int count; \
        BCM_PBMP_COUNT(_pbmp, count); \
        if (count != 1) { \
            return BCM_E_INTERNAL; \
        } \
        BCM_PBMP_ITER(_pbmp, _local_port) { \
           break; \
       } \
   } while (0)

/* Convert PBMP to one single port not matter which */
#define DNX_PBMP_SINGLE_PORT_GET(_pbmp, _local_port) \
    do { \
        int count; \
        BCM_PBMP_COUNT(_pbmp, count); \
        if (count == 0) { \
            return BCM_E_INTERNAL; \
        } \
        BCM_PBMP_ITER(_pbmp, _local_port) { \
           break; \
       } \
   } while (0)

/* If a pointer is not null, fill it with value */
#define DNX_IF_NOT_NULL_FILL(_p, _value) \
   do { \
   if ((_p) != NULL) {  \
       *(_p) = (_value);    \
   }    \
} while (0); 

/* If a flag is set return TRUE, otherwise return FALSE */
#define DNX_IS_FLAG_SET(_flags, _required_flag)  \
    (((_flags) & (_required_flag)) ? TRUE : FALSE)

/* Returns a pointers value if not null, otherwise returns 0 */
#define DNX_VALUE_IF_NOT_NULL(_p) \
    ((_p) ? *(_p) : 0)

#if defined(BCM_SBUSDMA_SUPPORT) || defined(BCM_88650_A0)
extern sbusdma_desc_handle_t _soc_port_tx_handles[FRAGMENT_SIZE][3];
#endif
extern volatile uint32 _soc_tx_pending[BCM_MAX_NUM_UNITS];

typedef enum _bcm_dnx_convert_flag_e{
    _BCM_DNX_CONVERT_FLAG_NONE = 0,
    _BCM_DNX_CONVERT_FLAG_PP_TO_LOCAL,
    _BCM_DNX_CONVERT_FLAG_TM_TO_LOCAL,
    _BCM_DNX_CONVERT_FLAG_LOCAL_TO_PP,
    _BCM_DNX_CONVERT_FLAG_LOCAL_TO_TM
} _bcm_dnx_convert_flag_t;

int
pbmp_from_ppd_port_bitmap(
    int unit, 
    bcm_pbmp_t *pbmp,
    uint32 *ports,
    int ports_len);
    
int
pbmp_to_ppd_port_bitmap(
    int unit, 
    bcm_pbmp_t *pbmp,
    uint32 *ports, 
    int ports_len);



int
dnx_pbmp_from_ppd_port_bitmap_convert(
    int unit,
    int core_id,
    uint32 *ports, 
    int ports_len,
    _bcm_dnx_convert_flag_t flag,
    bcm_pbmp_t *pbmp
    );


    
int
dnx_pbmp_to_ppd_port_bitmap_convert(
    int unit, 
    int core_id,
    bcm_pbmp_t *pbmp,
    int ports_len,
    _bcm_dnx_convert_flag_t flag,
    uint32 *ports 
    );


int
_bcm_dnx_mac_to_sand_mac(
    bcm_mac_t bcm_mac,
    DNX_SAND_PP_MAC_ADDRESS *soc_ppd_mac);

int
_bcm_dnx_mac_from_sand_mac(
    bcm_mac_t bcm_mac,
    DNX_SAND_PP_MAC_ADDRESS *soc_ppd_mac);

int
_bcm_dnx_utils(
    bcm_mac_t bcm_mac,
    DNX_SAND_PP_MAC_ADDRESS *soc_ppd_mac);

/*
 *   Function
 *      _bcm_dnx_pbmp_filter_by_core
 *   Purpose
 *      Returns only ports of the input core. 
 *
 *   Parameters
 *      (IN)  core_id      : required core
 *      (IN)  pbmp         : bcm_pbmp_t of local ports
 *      (OUT) core_pbmp    : returned pbmp: ports of the core
 *   Returns
 *       BCM_E_NONE - success
 *       BCM_E_*    - failure
 */

int
_bcm_pbmp_filter_by_core(
   int unit,
   int core_id,
   bcm_pbmp_t pbmp,
   bcm_pbmp_t *core_pbmp);

/*
 * resources definitions
 */
#define BCM_DNX_MAX_MC_ID(unit)     JER2_ARAD_MAX_MC_ID(unit)

#define BCM_DNX_MAX_TRUNK_ID(unit) (DNX_TMC_PORT_LAGS_MAX)

/* needed to put these somewhere */
extern int bcm_dnx_stat_stop(int unit);

/* Given a uint32 containing flags and a flag(s), sets another uint32 with a different flag(s)*/
#define DNX_TRANSLATE_FLAG(_src, _flag, _target, _target_flag)  \
        if ((_src) & (_flag)) {                                 \
            (_target) |= (_target_flag);                        \
        }

#define DNX_BMP_IS_SUBSET(_bmp, _potential_subset)    \
    ((_bmp) == ((_bmp) | (_potential_subset)))

#define DNX_BMP_IS_SUBSET_AND_NOT_EMPTY(_bmp, _potential_subset)    \
    (DNX_BMP_IS_SUBSET(_bmp, _potential_subset) && ((_potential_subset) != 0))

#endif /* INCLUDE_DNX_UTILS_H */
