/* $Id: arad_pmf_low_level_pgm.h,v 1.4 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/

#ifndef __ARAD_PMF_LOW_LEVEL_PGM_INCLUDED__
/* { */
#define __ARAD_PMF_LOW_LEVEL_PGM_INCLUDED__

/*************
 * INCLUDES  *
 *************/
/* { */

#include <soc/dpp/SAND/Utils/sand_header.h>

#include <soc/dpp/SAND/Management/sand_general_macros.h>
#include <soc/dpp/SAND/Management/sand_error_code.h>

#include <soc/dpp/ARAD/arad_pmf_low_level.h>
#include <soc/dpp/ARAD/arad_api_framework.h>
#include <soc/dpp/TMC/tmc_api_pmf_low_level_pgm.h>
#include <soc/dpp/ARAD/arad_api_ports.h>

/* } */
/*************
 * DEFINES   *
 *************/
/* { */



/* } */
/*************
 * MACROS    *
 *************/
/* { */

/* } */
/*************
 * TYPE DEFS *
 *************/
/* { */


#define ARAD_PMF_PGM_BYTES_TO_RMV_HDR_START               SOC_TMC_PMF_PGM_BYTES_TO_RMV_HDR_START
#define ARAD_PMF_PGM_BYTES_TO_RMV_HDR_1ST                 SOC_TMC_PMF_PGM_BYTES_TO_RMV_HDR_1ST
#define ARAD_PMF_PGM_BYTES_TO_RMV_HDR_FWDING              SOC_TMC_PMF_PGM_BYTES_TO_RMV_HDR_FWDING
#define ARAD_PMF_PGM_BYTES_TO_RMV_HDR_POST_FWDING         SOC_TMC_PMF_PGM_BYTES_TO_RMV_HDR_POST_FWDING
#define ARAD_NOF_PMF_PGM_BYTES_TO_RMV_HDRS                SOC_TMC_NOF_PMF_PGM_BYTES_TO_RMV_HDRS
typedef SOC_TMC_PMF_PGM_BYTES_TO_RMV_HDR                       ARAD_PMF_PGM_BYTES_TO_RMV_HDR;

typedef SOC_TMC_PMF_PGM_SEL_VAL                                ARAD_PMF_PGM_SEL_VAL;
typedef SOC_TMC_PMF_PGM_SELECTION_ENTRY                        ARAD_PMF_PGM_SELECTION_ENTRY;
typedef SOC_TMC_PMF_PGM_VALIDITY_INFO                          ARAD_PMF_PGM_VALIDITY_INFO;
typedef SOC_TMC_PMF_PGM_BYTES_TO_RMV                           ARAD_PMF_PGM_BYTES_TO_RMV;
typedef SOC_TMC_PMF_PGM_INFO                                   ARAD_PMF_PGM_INFO;


#define ARAD_PMF_PGM_HEADER_PROFILE_TM_MULTICAST              SOC_TMC_PMF_PGM_HEADER_PROFILE_TM_MULTICAST
#define ARAD_PMF_PGM_HEADER_PROFILE_TM_OUTLIF                 SOC_TMC_PMF_PGM_HEADER_PROFILE_TM_OUTLIF
#define ARAD_PMF_PGM_HEADER_PROFILE_TM_UNICAST                SOC_TMC_PMF_PGM_HEADER_PROFILE_TM_MULTICAST
#define ARAD_PMF_PGM_HEADER_PROFILE_STACKING                  SOC_TMC_PMF_PGM_HEADER_PROFILE_STACKING
#define ARAD_PMF_PGM_HEADER_PROFILE_ETHERNET                  SOC_TMC_PMF_PGM_HEADER_PROFILE_ETHERNET
#define ARAD_PMF_PGM_HEADER_PROFILE_ETHERNET_DSP              SOC_TMC_PMF_PGM_HEADER_PROFILE_ETHERNET_DSP
#define ARAD_PMF_PGM_HEADER_PROFILE_ETHERNET_LEARN            SOC_TMC_PMF_PGM_HEADER_PROFILE_ETHERNET_LEARN
#define ARAD_TMC_PMF_PGM_HEADER_PROFILE_ETHERNET_LATENCY      SOC_TMC_PMF_PGM_HEADER_PROFILE_ETHERNET_LATENCY



/* } */
/*************
 * GLOBALS   *
 *************/
/* { */

/* } */
/*************
 * FUNCTIONS *
 *************/
/* { */

/*
 *  System Header Profile management functions
 */
uint32
  arad_pmf_low_level_pgm_header_profile_set(
    SOC_SAND_IN  int                            unit,
    SOC_SAND_IN  ARAD_PORTS_FC_TYPE                   fc_type,
    SOC_SAND_IN  ARAD_PORT_HEADER_TYPE                header_type,
    SOC_SAND_IN  uint8				              is_itmh_mc_flow,
    SOC_SAND_OUT uint32                            *header_profile,
    SOC_SAND_OUT uint32                            *internal_profile
  );

uint32
  arad_pmf_low_level_pgm_header_type_get(
    SOC_SAND_IN  int                          unit,
	SOC_SAND_IN  uint32                           internal_profile_ndx,
	SOC_SAND_OUT ARAD_PORTS_FC_TYPE                  *fc_type,
    SOC_SAND_OUT ARAD_PORT_HEADER_TYPE            *header_type
  );

uint32
  arad_pmf_low_level_pgm_init_unsafe(
    SOC_SAND_IN  int                                 unit
  );

/*********************************************************************
* NAME:
 *   arad_pmf_pgm_set_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Set the Program properties, e.g. Lookup-Profile-IDs. The
 *   Copy Engine instructions are set via the PMF Copy Engine
 *   APIs.
 * INPUT:
 *   SOC_SAND_IN  int                            unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                            pmf_pgm_ndx -
 *     PMF-Program-ID. Range: 0 - 31. (Arad-B)
 *   SOC_SAND_IN  ARAD_PMF_PGM_INFO                   *info -
 *     The program properties, except the ones related to the
 *     Copy Engine set in the Copy Engine APIs.
 * REMARKS:
 *   None.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pmf_pgm_set_unsafe(
    SOC_SAND_IN  int                            unit,
    SOC_SAND_IN  uint32                            pmf_pgm_ndx,
    SOC_SAND_IN  ARAD_PMF_PGM_INFO                     *info
  );

uint32
  arad_pmf_pgm_set_verify(
    SOC_SAND_IN  int                            unit,
    SOC_SAND_IN  uint32                            pmf_pgm_ndx,
    SOC_SAND_IN  ARAD_PMF_PGM_INFO                     *info
  );

uint32
  arad_pmf_pgm_get_verify(
    SOC_SAND_IN  int                            unit,
    SOC_SAND_IN  uint32                            pmf_pgm_ndx
  );

/*********************************************************************
*     Gets the configuration set by the
 *     "arad_pmf_pgm_set_unsafe" API.
 *     Refer to "arad_pmf_pgm_set_unsafe" API for details.
*********************************************************************/
uint32
  arad_pmf_pgm_get_unsafe(
    SOC_SAND_IN  int                            unit,
    SOC_SAND_IN  uint32                            pmf_pgm_ndx,
    SOC_SAND_OUT ARAD_PMF_PGM_INFO                   *info
  );


uint32
  ARAD_PMF_PGM_BYTES_TO_RMV_verify(
    SOC_SAND_IN  ARAD_PMF_PGM_BYTES_TO_RMV *info
  );

uint32
  ARAD_PMF_PGM_INFO_verify(
    SOC_SAND_IN  ARAD_PMF_PGM_INFO *info
  );


void
  ARAD_PMF_PGM_BYTES_TO_RMV_clear(
    SOC_SAND_OUT ARAD_PMF_PGM_BYTES_TO_RMV *info
  );

void
  ARAD_PMF_PGM_INFO_clear(
    SOC_SAND_OUT ARAD_PMF_PGM_INFO *info
  );

/* } */

#include <soc/dpp/SAND/Utils/sand_footer.h>

/* } __ARAD_PMF_LOW_LEVEL_PGM_INCLUDED__*/
#endif

