/* $Id: sand_link.c,v 1.8 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/
/******************************************************************
*
* FILENAME:       soc_sand_link.c
*
* MODULE PREFIX:  soc_sand_link
*
* FILE DESCRIPTION:
*
* REMARKS:
* SW License Agreement: Dune Networks (c). CONFIDENTIAL PROPRIETARY INFORMATION.
* Any use of this Software is subject to Software License Agreement
* included in the Driver User Manual of this device.
* Any use of this Software constitutes an agreement to the terms
* of the above Software License Agreement.
******************************************************************/

/*************
 * INCLUDES  *
 *************/
/* { */

#include <soc/dpp/SAND/Utils/sand_header.h>
#include <soc/dpp/SAND/Management/sand_error_code.h>

#include <soc/dpp/SAND/Utils/sand_integer_arithmetic.h>

/* } */
/*************
 * DEFINES   *
 *************/
/* { */

#define SOC_SAND_LINK_FE_BKT_FILL_RATE                      (6)
#define SOC_SAND_LINK_FE_UP_LINK_TH                         (32)
#define SOC_SAND_LINK_FE_DN_LINK_TH                         (16)
#define SOC_SAND_LINK_FE_REACHABILITY_RATE                  (50)    /*micro sec*/
#define SOC_SAND_LINK_FE_REACHABILITY_WATCHDOG_PERIOD       (100)   /*micro sec*/
#define SOC_SAND_LINK_FAP_BKT_FILL_RATE                     (6)
#define SOC_SAND_LINK_FAP_UP_LINK_TH                        (32)
#define SOC_SAND_LINK_FAP_DN_LINK_TH                        (16)
#define SOC_SAND_LINK_FAP_REACHABILITY_RATE                 (50)    /*micro sec*/
#define SOC_SAND_LINK_FAP_REACHABILITY_WATCHDOG_PERIOD      (100)   /*micro sec*/


/* } */
/*************
 *  MACROS   *
 *************/
/* { */

/* } */
/*************
 * TYPE DEFS *
 *************/
/* { */

/* } */
/*************
 * GLOBALS   *
 *************/
/* { */

static uint32
   Soc_sand_link_fe_bkt_fill_rate       = SOC_SAND_LINK_FE_BKT_FILL_RATE;
static uint32
   Soc_sand_link_fe_up_link_th          = SOC_SAND_LINK_FE_UP_LINK_TH;
static uint32
   Soc_sand_link_fe_dn_link_th          = SOC_SAND_LINK_FE_DN_LINK_TH;
static uint32
   Soc_sand_link_fe_reachability_rate  = SOC_SAND_LINK_FE_REACHABILITY_RATE;
static uint32
   Soc_sand_link_fe_reachability_watchdog_period  = SOC_SAND_LINK_FE_REACHABILITY_WATCHDOG_PERIOD;
static uint32
   Soc_sand_link_fap_bkt_fill_rate      = SOC_SAND_LINK_FAP_BKT_FILL_RATE;
static uint32
   Soc_sand_link_fap_up_link_th         = SOC_SAND_LINK_FAP_UP_LINK_TH;
static uint32
   Soc_sand_link_fap_dn_link_th         = SOC_SAND_LINK_FAP_DN_LINK_TH;
static uint32
   Soc_sand_link_fap_reachability_rate  = SOC_SAND_LINK_FAP_REACHABILITY_RATE;
static uint32
   Soc_sand_link_fap_reachability_watchdog_period  = SOC_SAND_LINK_FAP_REACHABILITY_WATCHDOG_PERIOD;


/* } */
/*************
 * FUNCTIONS *
 *************/
/* { */

uint32
  soc_sand_link_fe_bkt_fill_rate_get(void)
{
  return Soc_sand_link_fe_bkt_fill_rate;
}

uint32
  soc_sand_link_fe_up_link_th_get(void)
{
  return Soc_sand_link_fe_up_link_th;
}

uint32
  soc_sand_link_fe_dn_link_th_get(void)
{
  return Soc_sand_link_fe_dn_link_th;
}

uint32
  soc_sand_link_fe_reachability_rate_get(
    SOC_SAND_IN  uint32                            core_rate,
    SOC_SAND_IN  uint32                            nof_links,
    SOC_SAND_IN  uint32                            nof_rmgr_units
  )
{
  uint32 rmgr_units_total;
  uint32 rmgr_val;
  uint32 rmgr_at_core_clock_steps;

  rmgr_units_total = nof_rmgr_units * nof_links;
  rmgr_at_core_clock_steps = (Soc_sand_link_fe_reachability_rate /*micro sec*/  * (core_rate /*per micro sec*//1000000));
  rmgr_val = SOC_SAND_DIV_ROUND_DOWN(rmgr_at_core_clock_steps, rmgr_units_total);
  return rmgr_val;
}

uint32
  soc_sand_link_fe_reachability_watchdog_period_get(
    SOC_SAND_IN  uint32                            core_rate,
    SOC_SAND_IN  uint32                            nof_links
  )
{

  uint32 wp_val;
  uint32 wp_at_core_clock_steps;
  wp_at_core_clock_steps = (Soc_sand_link_fe_reachability_watchdog_period /*micro sec*/ * (core_rate /*per micro sec*//1000000));
  wp_val = SOC_SAND_DIV_ROUND_UP(wp_at_core_clock_steps, 4096);
  return wp_val;
}


uint32
  soc_sand_link_fap_bkt_fill_rate_get(void)
{
  return Soc_sand_link_fap_bkt_fill_rate;
}

uint32
  soc_sand_link_fap_up_link_th_get(void)
{
  return Soc_sand_link_fap_up_link_th;
}

uint32
  soc_sand_link_fap_dn_link_th_get(void)
{
  return Soc_sand_link_fap_dn_link_th;
}

uint32
  soc_sand_link_fap_reachability_rate_get(
    SOC_SAND_IN  uint32                            core_rate,
    SOC_SAND_IN  uint32                            nof_links,
    SOC_SAND_IN  uint32                            rmgr_units
  )
{
  uint32 rmgr_units_total;
  uint32 rmgr_val;
  uint32 rmgr_at_core_clock_steps;

  rmgr_units_total = rmgr_units * nof_links;
  rmgr_at_core_clock_steps = (Soc_sand_link_fap_reachability_rate /*micro sec*/  * (core_rate /*per micro sec*//1000000));
  rmgr_val = SOC_SAND_DIV_ROUND_DOWN(rmgr_at_core_clock_steps, rmgr_units_total);
  return rmgr_val;
}

uint32
  soc_sand_link_fap_reachability_watchdog_period_get(
    SOC_SAND_IN  uint32                            core_rate
  )
{

  uint32 wp_val;
  uint32 wp_at_core_clock_steps;
  wp_at_core_clock_steps = (Soc_sand_link_fap_reachability_watchdog_period /*micro sec*/ * (core_rate /*per micro sec*//1000000));
  wp_val = SOC_SAND_DIV_ROUND_UP(wp_at_core_clock_steps, 4096);
  return wp_val;
}

/* } */

#include <soc/dpp/SAND/Utils/sand_footer.h>
