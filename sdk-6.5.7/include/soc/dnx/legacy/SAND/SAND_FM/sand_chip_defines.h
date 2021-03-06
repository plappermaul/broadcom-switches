/* $Id: sand_chip_defines.h,v 1.5 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/
#ifndef DNX_SAND_CHIP_DEFINES_H
#define DNX_SAND_CHIP_DEFINES_H
#ifdef  __cplusplus
extern "C" {
#endif
/* $Id: sand_chip_defines.h,v 1.5 Broadcom SDK $
 */
#include <soc/dnx/legacy/SAND/Utils/sand_framework.h>
#include <soc/dnx/legacy/SAND/Utils/sand_tcm.h>

  /*
  * Definitions related to 'version' register.
  * This register is mutual to all Dune's devices.
  */







/*
 * System wide definitions introduced by FMF for use
 * by all upper layers.
 * a. device types and subtypes
 * {
 */
/*
 * The various types of FAPs
 */
typedef enum
{
  DNX_SAND_FAP_TYPE_NONE = 0,
  DNX_SAND_FAP10M_FAP_TYPE,
  DNX_SAND_FAP20V_FAP_TYPE,
  DNX_SAND_MAX_FAP_TYPE
} DNX_SAND_FMF_FAP_TYPE ;
/*
 * The various types of FEs
 */
typedef enum
{
  DNX_SAND_FE_TYPE_NONE = 0,
  DNX_SAND_FE200_FE_TYPE,
  DNX_SAND_MAX_FE_TYPE
} DNX_SAND_FMF_FE_TYPE ;
/*
 * The various types of PPs
 */
typedef enum
{
  DNX_SAND_PP_TYPE_NONE = 0,
  DNX_SAND_PP_MARVELL_PRESTERA,
  DNX_SAND_MAX_PP_TYPE
} DNX_SAND_FMF_PP_TYPE ;
/*
 * The various types of DEVICEs
 * DO NOT change without updating the DNX_SAND_FMF_NUM_DEVICE_TYPES macro
 */
typedef enum
{
  DNX_SAND_DEVICE_TYPE_NONE = 0,
  DNX_SAND_FE2_DEVICE,
  DNX_SAND_FE13_DEVICE,
  DNX_SAND_FAP_DEVICE,
  DNX_SAND_MAX_DEVICE_TYPE
} DNX_SAND_FMF_DEVICE_TYPE ;

/*
 * Value of device identifier (device handle) indicating 'no device'
 */
/*
 * Number of device types in the system. Each device type is entitled to
 * an independent range of values of chip_id (e.g., a FAP may have chip ID
 * of 0x10 and, on the same system, an FE2 may also have a chip ID of 0x10)
 */
#define DNX_SAND_FMF_NUM_DEVICE_TYPES       (DNX_SAND_MAX_DEVICE_TYPE - 1)
/*
 * The various sub-types of DEVICEs
 */
typedef union
{
  DNX_SAND_FMF_FAP_TYPE fap_type ;
  DNX_SAND_FMF_FE_TYPE  fe_type ;
    /*
     * This entry is used when device sub type is
     * not known (yet).
     */
  uint32      any_type ;
} DNX_SAND_FMF_DEVICE_SUB_TYPE ;
/*
 * Macros to convert device type and chip id to internal device id
 * as per FMF convention (and some variations).
 */
#define DNX_SAND_FMF_CHIP_ID_FROM_DEV_ID(unit) \
    (unit / DNX_SAND_FMF_NUM_DEVICE_TYPES)
#define DNX_SAND_FMF_DEV_TYPE_FROM_DEV_ID(unit) \
    ((unit % DNX_SAND_FMF_NUM_DEVICE_TYPES) + 1)
/*
 * }
 */
/*
 * Currently DNX_SAND supports only the following
 * device type.
 */
typedef enum
{
  DNX_SAND_DEV_FE200 = 0,
  DNX_SAND_DEV_FAP10M,
  DNX_SAND_DEV_FAP20V,
  DNX_SAND_DEV_TIMNA,
  DNX_SAND_DEV_HRP,
  DNX_SAND_DEV_FAP20M,
  DNX_SAND_DEV_PETRA,
  DNX_SAND_DEV_FAP21V,
  DNX_SAND_DEV_FE600,
  DNX_SAND_DEV_T20E,
  DNX_SAND_DEV_PB,
  DNX_SAND_DEV_MOA,
  DNX_SAND_DEV_PCP,
  DNX_SAND_DEV_JER2_ARAD,
  DNX_SAND_DEV_JER2_JERICHO,

  /*
   * Last identifier.
   * Do not touch
   */
  DNX_SAND_DEVICE_NOF_TYPES
} DNX_SAND_DEVICE_TYPE ;

  /*
   *	The constants below are declared
   *  deprecated and will be removed in the next 
   *  driver versions
   */
#define DNX_SAND_FE200         DNX_SAND_DEV_FE200
#define DNX_SAND_FE600         DNX_SAND_DEV_FE600
#define DNX_SAND_FAP10M        DNX_SAND_DEV_FAP10M
#define DNX_SAND_FAP20V        DNX_SAND_DEV_FAP20V
#define DNX_SAND_FAP20M        DNX_SAND_DEV_FAP20M
#define DNX_SAND_FAP21V        DNX_SAND_DEV_FAP21V

/*
 * Credit Byte Worth
 */
typedef enum
{
  DNX_SAND_CR_256  = 256,
  DNX_SAND_CR_512  = 512,
  DNX_SAND_CR_1024 = 1024,
  DNX_SAND_CR_2048 = 2048,
  DNX_SAND_CR_4096 = 4096,
  DNX_SAND_CR_8192 = 8192
}DNX_SAND_CREDIT_SIZE ;


/*
 * A sub-class in the device, may be an entity.
 * Example: DNX_SAND_FE200 may have FE1, FE3, FE2, FE13
 *          DNX_SAND_FAP10M may have FIP, FOP, FAP.
 */
typedef enum
{
  DNX_SAND_DONT_CARE_ENTITY = 0,
  /*
   * FE1 entity in DNX_SAND_FE200 configured as FE13.
   */
  DNX_SAND_FE1_ENTITY = 1,
  /*
   * FE2 entity in DNX_SAND_FE200 configured as FE2.
   */
  DNX_SAND_FE2_ENTITY = 2,
  /*
   * FE3 entity in DNX_SAND_FE200 configured as FE13.
   */
  DNX_SAND_FE3_ENTITY = 3,
  /*
   * FAP: Fabric input/output entity.
   */
  DNX_SAND_FAP_ENTITY = 4,
  /*
   * Fabric output entity within FAP.
   */
  DNX_SAND_FOP_ENTITY = 5,
  /*
   * Fabric input entity within FAP.
   */
  DNX_SAND_FIP_ENTITY = 6,
  /*
   * FE13 entity (DNX_SAND_FE200 configured as FE13).
   */
  DNX_SAND_FE13_ENTITY = 7
} DNX_SAND_DEVICE_ENTITY ;

/*
 * Memory structures describing FE registers in a way that
 * enables the cpu to both display and change it.
 * {
 */
/*
 * Maximal number of of 'ELEMENT_OF_GROUP' items per group.
 */
#define MAX_ELEMENT_OF_GROUP_ITEMS 10
/*
 * Maximal number of of 32-bits regsiters per group.
 */
#define MAX_REGS_IN_ONE_GROUP      256
/*
 */
/*
 * Values for 'access_type' field
 */
/*
 */
/*
 */
/*
 * the size of the bit stream must be multiple of 32
 */
#define MAX_INTERRUPT_CAUSE               256
/*
 * since the bit stream is an array of longs,
 * its size is MAX_INTERRUPT_CAUSE divided by 32
 */
#define SIZE_OF_BITSTRAEM_IN_UINT32S   (MAX_INTERRUPT_CAUSE>>5)
/*
 * Every device registered must register this method,
 * Since in our interrupt serving scheme, the interrupt handler
 * masks the inetrrupts, and the interrupt servicing is actually
 * done by the TCM, which have to unmask them at the end.
 * Since all this handling is done over the chip's descriptor,
 * this is the place to put the unmasking func as well.
 */
typedef unsigned short (*DNX_SAND_UNMASK_FUNC_PTR) (int, uint32) ;
typedef uint32   (*DNX_SAND_IS_BIT_AUTO_CLEAR_FUNC_PTR)(uint32) ;
typedef unsigned short (*DNX_SAND_IS_DEVICE_INTERRUPTS_MASKED)(int) ;
typedef unsigned short (*DNX_SAND_GET_DEVICE_INTERRUPTS_MASK)(int, uint32 *) ;
typedef unsigned short (*DNX_SAND_MASK_SPECIFIC_INTERRUPT_CAUSE)(int, uint32) ;
/*
 * Every DNX_SAND device has a mean to be reset. The mean is implemented somewhere
 * on the board (as part of the BSP) - so when registering the device, this reset method
 * should be passed and then the f***_device_reset() api method will be valid.
 *
 * INPUT:
 *   first parameter - uint32 -
 *     User identifier of device. DNX_SAND device device identifier.
 *     The number the user got from '***_register_device()' function.
 *   second parameter - uint32 -
 *     If non-zero then device will remain paralyzed,
 *     with 'reset' asserted until this procedure is
 *     called again, this time with this parameter set
 *     to zero.
 * OUTPUT:
 *   unsigned short -
 *     DNX_SAND_OK  - Reset was successfully performed.
 *     DNX_SAND_ERR - Error occurred during reset.
 */
typedef unsigned short (*DNX_SAND_RESET_DEVICE_FUNC_PTR) (int, uint32) ;
/*
 * Every DNX_SAND device support the dnx_sand_mem_read and dnx_sand_mem_write methods.
 * But these methods might try to access some restricted area. in order to
 * prevent such fault, this access verify method is should be registered
 * during device registration.
 */
typedef unsigned short (*DNX_SAND_IS_OFFSET_READ_OR_WRITE_PROTECT_FUNC_PTR) (uint32, uint32, uint32) ;
/*
 */
typedef struct
{
  /*
   * a flag indicating wheather this descriptor is a valid one
   */
  uint32   valid_word ;
  /*
   * mapping of this chip to memory. All read / writes
   * to this chip are done with offset from this base
   */
  uint32   *base_addr ;
  /*
   * the size (in bytes) that this chip is occupying in the
   * memory space.
   */
  uint32   mem_size ;
  /*
   * Every service call that requires working with the chip,
   * requires mutex typedef struct
   */
  sal_mutex_t  mutex_id ;
  /*
   * In order to allow for the same task to take the mutex again,
   * we maintain the current owner task_id. 0 == no owner.
   */
  sal_thread_t  mutex_owner ;
  /*
   * In order for the number of takes to be equal to the number of
   * gives we maintain this counter ;
   */
  int32   mutex_counter ;
  /*
   * This is an array of callbacks, that do not require polling,
   * but rather act as event notifications. The cause for such
   * events are interrupts, so this is actually an arrray of callbacks,
   * each handles a different interrupt.
   */
  struct dnx_sand_tcm_callback_str    interrupt_callback_array[MAX_INTERRUPT_CAUSE] ;
  /*
   * This is the actual connection to the interrupt line. The interrupt serving routine
   * checks what caused the interrupt, and sets the right bit. DNX_SAND_CALLBACK_CAUSE_ID is
   * 32 bit, so we have a 32 different possible interrupt causes that may be triggered
   * together.
   */
  uint32   interrupt_bitstream[SIZE_OF_BITSTRAEM_IN_UINT32S] ;
  /*
   * Interrupt unmasking function specific for this chip.
   * after serving all interrupts, and clearing the interrupt bitstream
   * we can safely unmask them back.
   */
  DNX_SAND_UNMASK_FUNC_PTR         unmask_ptr ;
  /*
   * Every interrupt source in the device architecture is either auto-cleared
   * or not. the tcm uses this information during its interrupt serving loop.
   */
  DNX_SAND_IS_BIT_AUTO_CLEAR_FUNC_PTR  is_bit_auto_clear_ptr ;
  /*
   * Tcm interrupt serving loop must be carried out when device interrupts are masked.
   * if thery are not masked it is a fatal error.
   */
  DNX_SAND_IS_DEVICE_INTERRUPTS_MASKED is_device_interrupts_masked_ptr ;
  /*
   * Every interrupt source in the device architecture has an associated
   * mask bit. the tcm must respect this bit as well, before calling
   * registered callback functions.
   */
  DNX_SAND_GET_DEVICE_INTERRUPTS_MASK  get_device_interrupts_mask_ptr ;
  /*
   * Dnx_soc_sand can unregister a specific interrupt oriented callback, and
   * mask it from the device with this callback
   */
  DNX_SAND_MASK_SPECIFIC_INTERRUPT_CAUSE mask_specific_interrupt_cause_ptr ;
  /*
   * mask / unmask counter and level
   * The logic goes something like this:
   * you can mask the general interrupt bit (fecint at the FE) from 2 places:
   * the interrupt handler, or the user code
   * (through 'device'_logical_write() api method of this logic field_id 'fecint' for FE)
   * The interrupt is superior to the user - meaning, we don't want
   * the user to unmask them while interrupts are still pending to
   * be served (that's the reason they were masked to begin with)
   * That's why we keep a level - Interrupt level = TRUE, user level = FALSE
   * The same apply to the unmasking only now it's the tcm that unmasks the
   * fecint bit, when done serving interrupts.
   * We maintain the counter in order to know to which state to restore.
   * > 0 => masked, < 0 => unmasked
   */
  uint32  device_is_between_isr_to_tcm ;
  int           device_interrupt_mask_counter ;
  /*
   * resetting a device is specific to the board this chip resides in.
   * so we have to let the user register a reset method for us to use.
   */
  DNX_SAND_RESET_DEVICE_FUNC_PTR     reset_device_ptr ;
  /*
   * Access verify method.
   */
  DNX_SAND_IS_OFFSET_READ_OR_WRITE_PROTECT_FUNC_PTR  is_read_write_protect_ptr ;
  /*
   * This is a list of constants representing the chip,
   * might be longer at the future
   */
  DNX_SAND_DEVICE_TYPE logic_chip_type;
  uint32    chip_type ;
  uint32    dbg_ver ;
  uint32    chip_ver ;

  /*
   * Flag indicating whether device corresponding to this descriptor
   * is at 'initialization' stage (some operations are only
   * allowed at 'init').
   */
  uint32   device_at_init ;
  /*
   * magic functions as a unique indetifier of this descriptor
   */
  uint32   magic ;
} DNX_SAND_DEVICE_DESC ;
/*
 */
#define DNX_SAND_DEVICE_DESC_VALID_WORD 0xFAEBDC65
/*
 * the chip handle will act as an index to the
 * DNX_SAND_DEVICE_DESC array of chips on board
 */
typedef uint32 DNX_SAND_CHIP_HANDLE ;
/*
 */

typedef enum{
  DNX_SAND_ACTUAL_FAP_VALUE_1 = 0,
  DNX_SAND_ACTUAL_FAP_VALUE   = 5,
  DNX_SAND_ACTUAL_FIP_VALUE   = 1,
  DNX_SAND_ACTUAL_FOP_VALUE   = 4,
  DNX_SAND_ACTUAL_FE1_VALUE   = 6,
  DNX_SAND_ACTUAL_FE2_VALUE   = 3,
  DNX_SAND_ACTUAL_FE2_VALUE_1 = 7,
  DNX_SAND_ACTUAL_FE3_VALUE   = 2
} DNX_SAND_ENTITY_LEVEL_TYPE;

#define DNX_SAND_ACTUAL_BAD_VALUE               0xFF
#define DNX_SAND_ACTUAL_OUT_OF_RANGE            0xFFFFFFFF
#define DNX_SAND_REAL_ENTITY_VALUE_OUT_OF_RANGE 0xFFFFFFFF


/*****************************************************
*NAME
*   dnx_sand_entity_enum_to_str
*TYPE:
*  PROC
*DATE:
*  30/05/2006
*FUNCTION:
*  returns a string representing the appropriate entity type
*INPUT:
*  DNX_SAND_DIRECT:
*    const char* - the output string representing the entity_type
*  DNX_SAND_INDIRECT:
*    None.
*OUTPUT:
*  DNX_SAND_DIRECT:
*    a string representing the appropriate entity type
*  DNX_SAND_INDIRECT:
*    None.
*REMARKS:
*    None.
*SEE ALSO:
*****************************************************/
char *
  dnx_sand_entity_enum_to_str(
    DNX_SAND_IN DNX_SAND_DEVICE_ENTITY entity_type
  );

/*****************************************************
*NAME
*  dnx_sand_entity_from_level
*TYPE:
*  PROC
*DATE:
*  30/05/2006
*FUNCTION:
*  Returns dnx_sand device entity corresponding to a given source level type value
*INPUT:
*  DNX_SAND_DIRECT:
*    DNX_SAND_ENTITY_LEVEL_TYPE level_val - source level type value
*  DNX_SAND_INDIRECT:
*    None.
*OUTPUT:
*  DNX_SAND_DIRECT:
*    dnx_sand device entity
*  DNX_SAND_INDIRECT:
*    None.
*REMARKS:
*    None.
*SEE ALSO:
*****************************************************/
DNX_SAND_DEVICE_ENTITY
  dnx_sand_entity_from_level(
    DNX_SAND_IN DNX_SAND_ENTITY_LEVEL_TYPE level_val
  );

#ifdef  __cplusplus
}
#endif

#endif
