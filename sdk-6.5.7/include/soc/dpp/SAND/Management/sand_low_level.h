/* $Id: sand_low_level.h,v 1.6 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * $
*/


#ifndef __SOC_SAND_LOW_LEVEL_H_INCLUDED__
/* { */
#define __SOC_SAND_LOW_LEVEL_H_INCLUDED__
#ifdef  __cplusplus
extern "C" {
#endif

#include <soc/dpp/SAND/Utils/sand_framework.h>

/* $Id: sand_low_level.h,v 1.6 Broadcom SDK $
 * Accessed to device are defined to be volatiles
 */
#define VOLATILE  volatile
#define SOC_SAND_PHYSICAL_PRINT_WHEN_WRITING SOC_SAND_DEBUG


/*
 *  If defined, allows to count read and write accesses 
 *  using the functions under this flag
 */
#undef SOC_SAND_LL_ACCESS_STATISTICS

#ifdef SOC_SAND_LL_ACCESS_STATISTICS
/*
 *  Maximal number of tagged statistics indicators.
 *  A read/write statistics can be collected per tag.
 *  This is in addition to the overall access statistics
 */
#define SOC_SAND_LL_NOF_TAGGED_STAT_MAX   20
#define SOC_SAND_LL_NOF_STAT_IDS          (SOC_SAND_LL_NOF_TAGGED_STAT_MAX+1)

/*
 *  The index of the overall statistics is the last one
 */
#define SOC_SAND_LL_STAT_ID_TOTAL           SOC_SAND_LL_NOF_TAGGED_STAT_MAX

typedef enum
{
  SOC_SAND_LL_ACCESS_DIRECTION_READ   = 0,
  SOC_SAND_LL_ACCESS_DIRECTION_WRITE  = 1,
  SOC_SAND_LL_NOF_ACCESS_DIRECTIONS   = 2
} SOC_SAND_LL_ACCESS_DIRECTION;



/*
 *  Clear all access statistics counters and enablers.
 *  Must be run before using the access statistics.
 */
void
  soc_sand_ll_stat_clear(void);

/*
 *  Activate/diactivate access statistics collection.
 *  When activating, the overall statistics is always activated.
 *  To activate just the overall collection, without per-tag collection, 
 *  use SOC_SAND_LL_STAT_ID_TOTAL as an index.
 *  If per-tag collection is also needed, use the requested tag index.
 */
void 
  soc_sand_ll_stat_is_active_set(
    uint32 stat_ndx,
    uint8 is_active

  );

void 
  soc_sand_ll_stat_print(void);

/*
 *  Increment the access counter, per direction (Read/Write)
 *  if the statistics collection is enabled. 
 */
void
  soc_sand_ll_stat_increment_if_active(
    SOC_SAND_LL_ACCESS_DIRECTION direction
  );

#endif /* SOC_SAND_LL_ACCESS_STATISTICS */

/*
 *  If defined, allows to measure the execution time of 
 *  various source code sections
 */
#define SOC_SAND_LL_TIMER

#ifdef SOC_SAND_LL_TIMER

/*
 *  Maximal number of execution-time timers that can be activated
 */
#define SOC_SAND_LL_TIMER_MAX_NOF_TIMERS               (50)
#define SOC_SAND_LL_TIMER_MAX_NOF_CHARS_IN_TIMER_NAME  (40)

typedef struct
{
  /*
   *  Name (identifier) of the timer.                                     
   *  For example, if the timer is activated upon
   *  a function call, the function name.
   */
  char name[SOC_SAND_LL_TIMER_MAX_NOF_CHARS_IN_TIMER_NAME];

  /*
   *  Number of times the timer  was hit.
   *  For example, if the timer is activated upon
   *  a function call, the number of function calls.
   */
  uint32 nof_hits;

  uint32 active;

  /*
   *  Start time of the timer 
   */
  uint32 start_timer;

  /*
   *  Accumulated time measured by the timer, 
   *  in milliseconds 
   */
  uint32 total_time;

}SOC_SAND_LL_TIMER_FUNCTION;

/* uncomment this flag to enable time measurments */
/*#define ARAD_PP_KBP_TIME_MEASUREMENTS*/

#define ARAD_KBP_TIMERS_NONE 						0
#define ARAD_KBP_TIMERS_APPL 						1
#define ARAD_KBP_TIMERS_BCM							2
#define ARAD_KBP_TIMERS_SOC							3
#define ARAD_KBP_TIMERS_ROUTE_ADD_COMMIT 			4
#define ARAD_KBP_TIMERS_ADD_RECORDS_INTO_TABLES		5

typedef enum
{
	ARAD_KBP_IPV4_TIMERS_LPM_ROUTE_REMOVE 	= ARAD_KBP_TIMERS_ADD_RECORDS_INTO_TABLES+1,
	ARAD_KBP_IPV4_TIMERS_LPM_ROUTE_ADD,
	ARAD_KBP_IPV4_TIMERS_LPM_ROUTE_ADD_ROP,
	ARAD_KBP_IPV4_TIMERS_NOF_TIMERS
} ARAD_KBP_IPV4_TIMERS;

typedef enum
{
	ARAD_KBP_ACL_TIMERS_ROUTE_REMOVE 		= ARAD_KBP_TIMERS_ADD_RECORDS_INTO_TABLES+1,
	ARAD_KBP_ACL_TIMERS_ROUTE_ADD,
	ARAD_KBP_ACL_TIMERS_NOF_TIMERS
} ARAD_KBP_ACL_TIMERS;

extern SOC_SAND_LL_TIMER_FUNCTION Soc_sand_ll_timer_cnt[SOC_SAND_LL_TIMER_MAX_NOF_TIMERS];
extern uint8 Soc_sand_ll_is_any_active;
extern uint32  Soc_sand_ll_timer_total;

/*
 *  Getting the time in ms
 */
uint32
  soc_sand_ll_get_time_in_ms(void);



/*
 *  Init the counters
 */
void
  soc_sand_ll_timer_clear(void);

void
  soc_sand_ll_timer_stop_all(void);
/*
 *  Associate a counter to a function and take the start time
 */
void
  soc_sand_ll_timer_set(
    SOC_SAND_IN char name[SOC_SAND_LL_TIMER_MAX_NOF_CHARS_IN_TIMER_NAME],
    SOC_SAND_IN uint32 fn_ndx
  );

/*
 *  Stop the counter at the end function
 */
void
  soc_sand_ll_timer_stop(
    SOC_SAND_IN uint32 fn_ndx
  );


/*
 *  Print all the results
 */
void 
  soc_sand_ll_timer_print_all(void);



#endif /* SOC_SAND_LL_TIMER */


extern uint32
  Soc_sand_big_endian;
extern uint32
  Soc_sand_big_endian_was_checked;

SOC_SAND_RET
  soc_sand_check_chip_type_decide_big_endian(
    SOC_SAND_IN int  unit,
    SOC_SAND_IN uint32 version_reg,
    SOC_SAND_IN uint32 chip_type_shift,
    SOC_SAND_IN uint32 chip_type_mask
  );

uint32
  soc_sand_system_is_big_endian(
    void
  );

void
  soc_sand_ssr_set_big_endian(
    uint32 soc_sand_big_endian_was_checked,
    uint32 soc_sand_big_endian
  );

/*
 *  Low Level Simulation configuration
 *  When running over a simulation layer, 
 *  some driver code is skipped.
 *  For example, if a HW is expected to raise some indication upon certain conditions,
 *  and the mechanism for raising this indication is not simulated,
 *  the code checking the indication is skipped.
 *  This is implemented as follows:
 *  If SAND_LOW_LEVEL_SIMULATION is not defined, the code is executed - 
 *  assuming running on a real HW.
 *  If SAND_LOW_LEVEL_SIMULATION is defined, code execution can be controlled on-the-fly,
 *  by calling soc_sand_low_is_sim_active_set().
 *  The code that supports this mechanism must follow the below logic:
 *  // uint8 is_real_not_sim;
 *  // #ifndef  SAND_LOW_LEVEL_SIMULATION
 *  //    is_real_not_sim = TRUE;
 *  // #else
 *  //    is_real_not_sim = soc_sand_low_is_sim_active_get();
 *  // endif#
 *  // if(is_real_not_sim)
 *  // {Driver Code to run on real HW only, not on simulation}
 *
 *  Note! this logic is not implemented on the old devices.
 *  In this case, the code is backward compatible, meaning:
 *  The code runs if and only if SAND_LOW_LEVEL_SIMULATION is not defined.
 */
#ifdef  SAND_LOW_LEVEL_SIMULATION
uint8
  soc_sand_low_is_sim_active_get(void);

void soc_sand_low_is_sim_active_set(
       SOC_SAND_IN uint8 is_sim_active
     );
#endif



void
  soc_sand_ssr_get_big_endian(
    uint32 *soc_sand_big_endian_was_checked,
    uint32 *soc_sand_big_endian
  );

extern uint32 Soc_sand_physical_write_enable;

void
  soc_sand_set_physical_write_enable(
    uint32 physical_write_enable
  );

void
  soc_sand_get_physical_write_enable(
    uint32 *physical_write_enable
  );

/*
 * {
 * access hooks
 */

/*****************************************************
*NAME:
* SOC_SAND_PHYSICAL_WRITE_ACCESS_PTR
*DATE:
* 04/SEP/2002
*FUNCTION:
* Writes 'array' (of 'size') into the device at 'offeset'.
*INPUT:
*  SOC_SAND_DIRECT:
*   SOC_SAND_IN    uint32 *array         -
*                 pointer to a memory address allocated by the caller of
*                 this method. Memory address is treated as an array of
*                 32bits size words, with the data to write to the device.
*   SOC_SAND_INOUT uint32 *base_address  -
*                 the beginning of the device address space.
*                 This base address was given by the user to the driver
*                 during device registration,  so this is the means to
*                 distinguish between the different devices
*                 handled by this driver.
*   SOC_SAND_IN    uint32 offset         -
*                 offset in device address space to write to
*   SOC_SAND_IN    uint32 size           -
*                 array size (in bytes)
*  SOC_SAND_INDIRECT:
*OUTPUT:
*  SOC_SAND_DIRECT:
*    Non-zero in case of an error
*  SOC_SAND_INDIRECT:
*    None.
*SEE ALSO:
*****************************************************/
typedef
  SOC_SAND_RET
    (*SOC_SAND_PHYSICAL_WRITE_ACCESS_PTR)(
      SOC_SAND_IN     uint32 *array,
      SOC_SAND_INOUT  uint32 *base_address,
      SOC_SAND_IN     uint32 offset,
      SOC_SAND_IN     uint32 size
   ) ;

/*****************************************************
*NAME:
* SOC_SAND_PHYSICAL_READ_ACCESS_PTR
*DATE:
* 04/SEP/2002
*FUNCTION:
* Reads into an 'array' (of 'size') from the device at 'offeset'.
*INPUT:
*  SOC_SAND_DIRECT:
*   SOC_SAND_INOUT uint32 *array         -
*                 pointer to a memory address  allocated by the caller of
*                 this method. Memory address is treated as an array of
*                 32bits size words, and is loaded by the data from the device.
*   SOC_SAND_IN    uint32 *base_address  -
*                 the beginning of the chip address space. This base
*                 the beginning of the device address space.
*                 This base address was given by the user to the driver
*                 during device registration,  so this is the means to
*                 distinguish between the different devices
*                 handled by this driver.
*   SOC_SAND_IN    uint32 offset         -
*                 offset in device address space to read from
*   SOC_SAND_IN    uint32 size           -
*                 number (of bytes) to read from chip
*  SOC_SAND_INDIRECT:
*OUTPUT:
*  SOC_SAND_DIRECT:
*    Non-zero in case of an error
*  SOC_SAND_INDIRECT:
*    None.
*REMARKS:
*SEE ALSO:
*****************************************************/
typedef
  SOC_SAND_RET
    (*SOC_SAND_PHYSICAL_READ_ACCESS_PTR)(
      SOC_SAND_INOUT  uint32 *array,
      SOC_SAND_IN     uint32 *base_address,
      SOC_SAND_IN     uint32 offset,
      SOC_SAND_IN     uint32 size
    ) ;

typedef struct
{
  /*
   * Write function pointer to the device.
   */
  SOC_SAND_PHYSICAL_WRITE_ACCESS_PTR physical_write;

  /*
   * Read function pointer to the device.
   */
  SOC_SAND_PHYSICAL_READ_ACCESS_PTR  physical_read;
} SOC_SAND_PHYSICAL_ACCESS;


extern uint32 Soc_sand_physical_print_when_writing;
extern uint32 Soc_sand_physical_print_asic_style;
extern uint32 Soc_sand_physical_print_indirect_write;
extern uint32 Soc_sand_physical_print_part_of_indirect_read;
extern uint32 Soc_sand_physical_print_part_of_indirect_write;
extern uint32 Soc_sand_physical_print_part_of_read_modify_write;
extern uint32 Soc_sand_physical_print_unit_or_base_address;

extern uint32 Soc_sand_physical_print_first_reg ;
extern uint32 Soc_sand_physical_print_last_reg ;



/*****************************************************
*NAME
* soc_sand_set_physical_access_hook
*TYPE:
*  PROC
*DATE:
*  21-Sep-03
*FUNCTION:
*  Sets SOC_SAND private 'SOC_SAND_PHYSICAL_ACCESS'.
*INPUT:
*  SOC_SAND_DIRECT:
*    SOC_SAND_IN SOC_SAND_PHYSICAL_ACCESS* physical_access -
*      Pointer to physical access function to the devices.
*      To be supplied by the BSP implementor.
*      If any of the specified physical access
*      functions is set to NULL, the system's default
*      access function is used.
*  SOC_SAND_INDIRECT:
*    None.
*OUTPUT:
*  SOC_SAND_DIRECT:
*    SOC_SAND_RET.
*  SOC_SAND_INDIRECT:
*    None
*REMARKS:
*  None.
*SEE ALSO:
*****************************************************/
SOC_SAND_RET
  soc_sand_set_physical_access_hook(
    SOC_SAND_IN SOC_SAND_PHYSICAL_ACCESS* physical_access
  );
/*****************************************************
*NAME
* soc_sand_get_physical_access_hook
*TYPE:
*  PROC
*DATE:
*  21-Sep-03
*FUNCTION:
*  Gets SOC_SAND private 'SOC_SAND_PHYSICAL_ACCESS'.
*INPUT:
*  SOC_SAND_DIRECT:
*    SOC_SAND_IN SOC_SAND_PHYSICAL_ACCESS* physical_access -
*      Loaded with the physical access function to the devices.
*  SOC_SAND_INDIRECT:
*    None.
*OUTPUT:
*  SOC_SAND_DIRECT:
*    SOC_SAND_RET
*  SOC_SAND_INDIRECT:
*    None
*REMARKS:
*  None.
*SEE ALSO:
*****************************************************/
SOC_SAND_RET
  soc_sand_get_physical_access_hook(
    SOC_SAND_OUT SOC_SAND_PHYSICAL_ACCESS* physical_access
  );
/*
 * }
 */


/*
 * Low level driver - physical read / write to chip
 * {
 */

SOC_SAND_RET
  soc_sand_eci_read(
    SOC_SAND_INOUT  uint32 *array,
    SOC_SAND_IN     uint32 *base_address,
    SOC_SAND_IN     uint32 offset,
    SOC_SAND_IN     uint32 size
 );

SOC_SAND_RET
  soc_sand_eci_write(
    SOC_SAND_IN     uint32 *array,
    SOC_SAND_INOUT  uint32 *base_address,
    SOC_SAND_IN     uint32 offset,
    SOC_SAND_IN     uint32 size
 );

SOC_SAND_RET
  soc_sand_physical_write_to_chip(
    SOC_SAND_IN     uint32 *array,
    SOC_SAND_INOUT  uint32 *base_address,
    SOC_SAND_IN     uint32 offset,
    SOC_SAND_IN     uint32 size
  ) ;
/*
 *
 */
SOC_SAND_RET
  soc_sand_physical_read_from_chip(
    SOC_SAND_INOUT  uint32 *array,
    SOC_SAND_IN     uint32 *base_address,
    SOC_SAND_IN     uint32 offset,
    SOC_SAND_IN     uint32 size
  ) ;

/*
 *
 */
SOC_SAND_RET
  soc_sand_read_modify_write(
    SOC_SAND_INOUT  uint32 *base_address,
    SOC_SAND_IN     uint32 offset,
    SOC_SAND_IN     uint32 shift,
    SOC_SAND_IN     uint32 mask,
    SOC_SAND_IN     uint32 data_to_write
  ) ;
/*
 * }
 */

void
  soc_sand_set_print_when_writing(
    uint32 physical_print_when_writing,
    uint32 asic_style,
    uint32 indirect_write
  );

void
  soc_sand_get_print_when_writing(
    uint32 *physical_print_when_writing,
    uint32 *asic_style,
    uint32 *indirect_write
  );

#if SOC_SAND_DEBUG
/* { */
/*
 */
void
  soc_sand_physical_print_when_writing(
    uint32 do_print
  );
/*
 */
void
  soc_sand_physical_access_print(
    void
  );

void
  soc_sand_set_print_when_writing_part_of_indirect_write(
    uint32 part_of_indirect
  );

void
  soc_sand_set_print_when_writing_part_of_indirect_read(
    uint32 part_of_indirect
  );


/*
 */
SOC_SAND_RET
  soc_sand_eci_write_and_print(
    SOC_SAND_IN     uint32 *array,
    SOC_SAND_INOUT  uint32 *base_address,
    SOC_SAND_IN     uint32 offset,
    SOC_SAND_IN     uint32 size
 );

/*
 */
SOC_SAND_RET
  soc_sand_eci_read_and_print(
    SOC_SAND_INOUT  uint32 *array,
    SOC_SAND_IN     uint32 *base_address,
    SOC_SAND_IN     uint32 offset,
    SOC_SAND_IN     uint32 size
 );

void
  soc_sand_get_print_when_writing_unit_or_base_address(
    uint32 *unit_or_base_address
  );

void
  soc_sand_set_print_when_writing_unit_or_base_address(
    uint32 unit_or_base_address
  );

void
  soc_sand_set_print_when_writing_reg_range(
    uint32 first_reg,
    uint32 last_reg
  );

/* } */
#endif

#ifdef  __cplusplus
}
#endif

/* } __SOC_SAND_LOW_LEVEL_H_INCLUDED__*/
#endif
