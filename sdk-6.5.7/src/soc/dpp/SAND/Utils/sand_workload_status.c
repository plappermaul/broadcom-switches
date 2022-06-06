/* $Id: sand_workload_status.c,v 1.5 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/



#include <shared/bsl.h>
#include <soc/dpp/drv.h>



#include <soc/dpp/SAND/Utils/sand_workload_status.h>
#include <soc/dpp/SAND/Utils/sand_os_interface.h>

#if SOC_SAND_DEBUG
/* { */
/* $Id: sand_workload_status.c,v 1.5 Broadcom SDK $
 */


/*
 * Total work to be done, per device.
 */
static
  uint32
    Soc_sand_workload_status_total[SOC_SAND_MAX_DEVICE]
  = {0};

/*
 * Current percent of work that we already did, per device
 */
static
  uint32
    Soc_sand_workload_status_percent[SOC_SAND_MAX_DEVICE]
  = {0};


/*
 * Printing workload as percentage of total work to be done, per device.
 * Note, It is user responsibility to prevent from several printing to occur together.
 */
STATIC
  void
    soc_sand_workload_status_print(
      int unit
    )
{
  if(unit < SOC_SAND_MAX_DEVICE)
  {
    LOG_CLI((BSL_META_U(unit,
                        "\r%3u%%\r"),
             Soc_sand_workload_status_percent[unit]
             ));
  }
}
/*
 * Set the total work to be done, per device.
 */
void
  soc_sand_workload_status_start(
    int  unit,
    uint32 total_work_load
  )
{
  if(unit >= SOC_SAND_MAX_DEVICE)
  {
    goto exit;
  }

  Soc_sand_workload_status_total[unit] = total_work_load;
  if (0 == Soc_sand_workload_status_total[unit])
  {
    Soc_sand_workload_status_total[unit] = 1;
  }
  /*
   */
  Soc_sand_workload_status_percent[unit] = 0;

  soc_sand_workload_status_print(unit);

exit:
  return;
}

/*
 * Advances workload per device.
 */
void
  soc_sand_workload_status_run_no_print(
    int  unit,
    uint32 current_workload
  )
{
  uint32
    percent;
  /*
   */
  percent = 0;
  /*
   */

  if(unit >= SOC_SAND_MAX_DEVICE)
  {
    goto exit;
  }


  /*
   */
  if (0 == Soc_sand_workload_status_total[unit])
  {
    percent = 100;
  }
  else
  {
    /*
     * Avoid overflow/underflow.
     */
    if ( current_workload < (0xFFFFFFFF/100))
    {
      percent = (current_workload * 100) / Soc_sand_workload_status_total[unit] ;
    }
    else
    {
      percent = current_workload  / (Soc_sand_workload_status_total[unit]/100) ;
    }
  }

  Soc_sand_workload_status_percent[unit] = percent;

exit:
  return;

}

void
  soc_sand_workload_status_get(
    int  unit,
    uint32 *percent
  )
{
  if(unit >= SOC_SAND_MAX_DEVICE)
  {
    goto exit;
  }

  *percent = Soc_sand_workload_status_percent[unit];

exit:
  return;
}

/*
 * Advances workload and prints percentage if change detected, per device.
 */
void
  soc_sand_workload_status_run(
    int  unit,
    uint32 current_workload
  )
{
  uint32
    percent_old,
    percent_new;

  if (unit >= SOC_SAND_MAX_DEVICE)
  {
    goto exit;
  }

  soc_sand_workload_status_get(unit, &percent_old);
  soc_sand_workload_status_run_no_print(unit, current_workload);
  soc_sand_workload_status_get(unit, &percent_new);

  if (percent_old < percent_new)
  {
    soc_sand_workload_status_print(unit);
  }

exit:
  return;

}

/* } */
#else
/* { */

/*
 * If not in debug mode empty implementation.
 */

void
  soc_sand_workload_status_start(
    int unit,
    uint32 total_work_load
  )
{
  return;
}

void
  soc_sand_workload_status_run(
    int unit,
    uint32 current_workload
  )
{
  return;
}

void
  soc_sand_workload_status_get(
    int  unit,
    uint32 *percent
  )
{
  return;
}

void
  soc_sand_workload_status_run_no_print(
    int  unit,
    uint32 current_workload
  )
{
  return;
}


/* } */
#endif

