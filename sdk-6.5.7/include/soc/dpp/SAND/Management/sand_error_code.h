/* $Id: sand_error_code.h,v 1.13 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/


#ifndef __SOC_SAND_ERROR_CODE_H_INCLUDED__
/* { */
#define __SOC_SAND_ERROR_CODE_H_INCLUDED__
#ifdef  __cplusplus
extern "C" {
#endif

#include <soc/dpp/SAND/Utils/sand_framework.h>

#include <soc/dpp/SAND/SAND_FM/sand_chip_defines.h>

/* $Id: sand_error_code.h,v 1.13 Broadcom SDK $
 * Debug Macros {
 */
/*
 * Macros to be used throught the code in order to inform
 * the user inmformation.
 */

#define DEBUG_PRINTS 1
#if DEBUG_PRINTS
#define SOC_SAND_INFO_PRINTF(x)  \
  { \
    soc_sand_os_printf("++soc_sand info: %s:%s\r\n", __FILE__, __LINE__); \
    soc_sand_os_printf(x); \
  }
#define SOC_SAND_ERROR_PRINTF(x)  \
  { \
    soc_sand_os_printf("**soc_sand error: %s:%s\r\n", __FILE__, __LINE__); \
    soc_sand_os_printf(x); \
  }
#else
#define SOC_SAND_INFO_PRINTF(x)
#define SOC_SAND_ERROR_PRINTF(x)
#endif
/*
 * } END Debug Macros
 */
/*
 * System-wide error handling
 * {
 */
/*
 * Severity classification of events:
 * just message, warning, error, fatal error (i.e., unrecoverable error)
 */
#define SOC_SAND_SVR_MSG 0
#define SOC_SAND_SVR_WRN 1
#define SOC_SAND_SVR_ERR 2
#define SOC_SAND_SVR_FTL 3
/*
 * Structure of one element in array of all-system errors.
 * This array is kept sorted in malloc'ed area.
 */
typedef struct
{
  uint32  err_id ;
  char           *err_name ;
  char           *err_text ;
  unsigned short severity ;
    /*
     * Flag. If set then driver is required to not report this
     * event via 'soc_sand_error_handler()'.
     */
  unsigned short keep_internal ;
} SOC_ERROR_DESC_ELEMENT ;
/*
 * }
 */
/*
 * ERROR RETURN VALUES
 * {
 */
#ifdef SOC_SAND_ERROR_CODE_OLD_MODE

/*
 * Error return value is uint32 composed of the
 * fields specified below.
 * Error code -
 *   Identifier of the error. Up to 64K error codes are allowed.
 *   BITS 0 -> 15
 * Procedure id -
 *   Identifier of procedure returning error code. Up to 8k procedures
 *   are allowed.
 *   BITS 16 -> 26
 * Module id -
 *   Identifier of Module returning error code. Up to 8 modules
 *   are allowed.
 *   BITS 27 -> 29
 * Error severity -
 *   Flag. Indicator on whether error level was such that procedure
 *   could not carry out its function or whether it was just a
 *   warning and procedure could carry out its function. One bit.
 *   SOC_SAND_BIT 30
 * Callback Procedure -
 *   Flag. Indicator on whether procedure returning error value
 *   is a callback procedure or direct-return procedure. One bit.
 *   SOC_SAND_BIT 31
 */
#define SOC_ERROR_CODE_MS_BIT     15
#define SOC_ERROR_CODE_NUM_BITS   16
#define SOC_ERROR_CODE_LS_BIT     (SOC_ERROR_CODE_MS_BIT + 1 - SOC_ERROR_CODE_NUM_BITS)
#define SOC_ERROR_CODE_MASK   \
    (((uint32)SOC_SAND_BIT(SOC_ERROR_CODE_MS_BIT) - SOC_SAND_BIT(SOC_ERROR_CODE_LS_BIT)) + SOC_SAND_BIT(SOC_ERROR_CODE_MS_BIT))
#define SOC_ERROR_CODE_SHIFT      SOC_ERROR_CODE_LS_BIT
#define SOC_ERROR_CODE_MAX_VAL    (SOC_SAND_BIT(SOC_ERROR_CODE_NUM_BITS) - 1)
#define SOC_ERROR_CODE_MAX_MASK   (SOC_SAND_BIT(SOC_ERROR_CODE_NUM_BITS) - 1)

#define SOC_PROC_ID_MS_BIT        29
#define SOC_PROC_ID_NUM_BITS      14
#define SOC_PROC_ID_LS_BIT        (SOC_PROC_ID_MS_BIT + 1 - SOC_PROC_ID_NUM_BITS)
#define SOC_PROC_ID_MASK   \
    (((uint32)SOC_SAND_BIT(SOC_PROC_ID_MS_BIT) - SOC_SAND_BIT(SOC_PROC_ID_LS_BIT)) + SOC_SAND_BIT(SOC_PROC_ID_MS_BIT))
#define SOC_PROC_ID_SHIFT         SOC_PROC_ID_LS_BIT
#define SOC_PROC_ID_MAX_VAL       (SOC_SAND_BIT(SOC_PROC_ID_NUM_BITS) - 1)
#define SOC_PROC_ID_MAX_MASK      (SOC_SAND_BIT(SOC_PROC_ID_NUM_BITS) - 1)

#define SOC_ERR_SEVERE_MS_BIT         30
#define SOC_ERR_SEVERE_NUM_BITS       1
#define SOC_ERR_SEVERE_LS_BIT         (SOC_ERR_SEVERE_MS_BIT + 1 - SOC_ERR_SEVERE_NUM_BITS)
#define SOC_ERR_SEVERE_MASK   \
    (((uint32)SOC_SAND_BIT(SOC_ERR_SEVERE_MS_BIT) - SOC_SAND_BIT(SOC_ERR_SEVERE_LS_BIT)) + SOC_SAND_BIT(SOC_ERR_SEVERE_MS_BIT))
#define SOC_ERR_SEVERE_SHIFT          SOC_ERR_SEVERE_LS_BIT
#define SOC_ERR_SEVERE_MAX_VAL        (SOC_SAND_BIT(SOC_ERR_SEVERE_NUM_BITS) - 1)

#define SOC_CALLBACK_PROC_MS_BIT      31
#define SOC_CALLBACK_PROC_NUM_BITS    1
#define SOC_CALLBACK_PROC_LS_BIT      (SOC_CALLBACK_PROC_MS_BIT + 1 - SOC_CALLBACK_PROC_NUM_BITS)
#define SOC_CALLBACK_PROC_MASK   \
    (((uint32)SOC_SAND_BIT(SOC_CALLBACK_PROC_MS_BIT) - SOC_SAND_BIT(SOC_CALLBACK_PROC_LS_BIT)) + SOC_SAND_BIT(SOC_CALLBACK_PROC_MS_BIT))
#define SOC_CALLBACK_PROC_SHIFT       SOC_CALLBACK_PROC_LS_BIT
#define SOC_CALLBACK_PROC_MAX_VAL     (SOC_SAND_BIT(SOC_CALLBACK_PROC_NUM_BITS) - 1)

#define SOC_SAND_MODULE_ID_MS_BIT      29
#define SOC_SAND_MODULE_ID_NUM_BITS    4
#define SOC_SAND_MODULE_ID_LS_BIT      (SOC_SAND_MODULE_ID_MS_BIT + 1 - SOC_SAND_MODULE_ID_NUM_BITS)
#define SOC_SAND_MODULE_ID_MASK   \
    (((uint32)SOC_SAND_BIT(SOC_SAND_MODULE_ID_MS_BIT) - SOC_SAND_BIT(SOC_SAND_MODULE_ID_LS_BIT)) + SOC_SAND_BIT(SOC_SAND_MODULE_ID_MS_BIT))
#define SOC_SAND_MODULE_ID_SHIFT       SOC_SAND_MODULE_ID_LS_BIT
#define SOC_SAND_MODULE_ID_MAX_VAL     (SOC_SAND_BIT(SOC_SAND_MODULE_ID_NUM_BITS) - 1)

#else /*#ifdef SOC_SAND_ERROR_CODE_OLD_MODE*/

/*
 * Error return value is uint32 composed of the
 * fields specified below.
 * Error code -
 *   Identifier of the error. Up to 4K error codes are allowed.
 *   BITS 0 -> 11
 * Procedure id -
 *   Identifier of procedure returning error code. Up to 8k procedures
 *   are allowed, per module type
 *   BITS 12 -> 24
 * Module id -
 *   Identifier of Module returning error code. Up to 32 modules
 *   are allowed.
 *   BITS 25 -> 29
 * Error severity -
 *   Flag. Indicator on whether error level was such that procedure
 *   could not carry out its function or whether it was just a
 *   warning and procedure could carry out its function. One bit.
 *   SOC_SAND_BIT 30
 * Callback Procedure -
 *   Flag. Indicator on whether procedure returning error value
 *   is a callback procedure or direct-return procedure. One bit.
 *   SOC_SAND_BIT 31
 */
#define SOC_ERROR_CODE_MS_BIT     12
#define SOC_ERROR_CODE_NUM_BITS   13
#define SOC_ERROR_CODE_LS_BIT     (SOC_ERROR_CODE_MS_BIT + 1 - SOC_ERROR_CODE_NUM_BITS)
#define SOC_ERROR_CODE_MASK   \
    (((uint32)SOC_SAND_BIT(SOC_ERROR_CODE_MS_BIT) - SOC_SAND_BIT(SOC_ERROR_CODE_LS_BIT)) + SOC_SAND_BIT(SOC_ERROR_CODE_MS_BIT))
#define SOC_ERROR_CODE_SHIFT      SOC_ERROR_CODE_LS_BIT
#define SOC_ERROR_CODE_MAX_VAL    (SOC_SAND_BIT(SOC_ERROR_CODE_NUM_BITS) - 1)
#define SOC_ERROR_CODE_MAX_MASK   (SOC_SAND_BIT(SOC_ERROR_CODE_NUM_BITS) - 1)

#define SOC_PROC_ID_MS_BIT        29
#define SOC_PROC_ID_NUM_BITS      17
#define SOC_PROC_ID_LS_BIT        (SOC_PROC_ID_MS_BIT + 1 - SOC_PROC_ID_NUM_BITS)
#define SOC_PROC_ID_MASK   \
    (((uint32)SOC_SAND_BIT(SOC_PROC_ID_MS_BIT) - SOC_SAND_BIT(SOC_PROC_ID_LS_BIT)) + SOC_SAND_BIT(SOC_PROC_ID_MS_BIT))
#define SOC_PROC_ID_SHIFT         SOC_PROC_ID_LS_BIT
#define SOC_PROC_ID_MAX_VAL       (SOC_SAND_BIT(SOC_PROC_ID_NUM_BITS) - 1)
#define SOC_PROC_ID_MAX_MASK      (SOC_SAND_BIT(SOC_PROC_ID_NUM_BITS) - 1)

#define SOC_ERR_SEVERE_MS_BIT         30
#define SOC_ERR_SEVERE_NUM_BITS       1
#define SOC_ERR_SEVERE_LS_BIT         (SOC_ERR_SEVERE_MS_BIT + 1 - SOC_ERR_SEVERE_NUM_BITS)
#define SOC_ERR_SEVERE_MASK   \
    (((uint32)SOC_SAND_BIT(SOC_ERR_SEVERE_MS_BIT) - SOC_SAND_BIT(SOC_ERR_SEVERE_LS_BIT)) + SOC_SAND_BIT(SOC_ERR_SEVERE_MS_BIT))
#define SOC_ERR_SEVERE_SHIFT          SOC_ERR_SEVERE_LS_BIT
#define SOC_ERR_SEVERE_MAX_VAL        (SOC_SAND_BIT(SOC_ERR_SEVERE_NUM_BITS) - 1)

#define SOC_CALLBACK_PROC_MS_BIT      31
#define SOC_CALLBACK_PROC_NUM_BITS    1
#define SOC_CALLBACK_PROC_LS_BIT      (SOC_CALLBACK_PROC_MS_BIT + 1 - SOC_CALLBACK_PROC_NUM_BITS)
#define SOC_CALLBACK_PROC_MASK   \
    (((uint32)SOC_SAND_BIT(SOC_CALLBACK_PROC_MS_BIT) - SOC_SAND_BIT(SOC_CALLBACK_PROC_LS_BIT)) + SOC_SAND_BIT(SOC_CALLBACK_PROC_MS_BIT))
#define SOC_CALLBACK_PROC_SHIFT       SOC_CALLBACK_PROC_LS_BIT
#define SOC_CALLBACK_PROC_MAX_VAL     (SOC_SAND_BIT(SOC_CALLBACK_PROC_NUM_BITS) - 1)

/* 
 * Note: this field is part of the SOC_PROC_ID field,
 */
#define SOC_SAND_MODULE_ID_MS_BIT      29
#define SOC_SAND_MODULE_ID_NUM_BITS    5
#define SOC_SAND_MODULE_ID_LS_BIT      (SOC_SAND_MODULE_ID_MS_BIT + 1 - SOC_SAND_MODULE_ID_NUM_BITS)
#define SOC_SAND_MODULE_ID_MASK   \
    (((uint32)SOC_SAND_BIT(SOC_SAND_MODULE_ID_MS_BIT) - SOC_SAND_BIT(SOC_SAND_MODULE_ID_LS_BIT)) + SOC_SAND_BIT(SOC_SAND_MODULE_ID_MS_BIT))
#define SOC_SAND_MODULE_ID_SHIFT       SOC_SAND_MODULE_ID_LS_BIT
#define SOC_SAND_MODULE_ID_MAX_VAL     (SOC_SAND_BIT(SOC_SAND_MODULE_ID_NUM_BITS) - 1)

#endif /*#ifdef SOC_SAND_ERROR_CODE_OLD_MODE*/

/*
 * }
 */
/*
 * System-wide procedure identification handling
 * {
 */
/*
 * Structure of one element in array of all-system procedure
 * descriptors.
 * This array is kept sorted in malloc'ed area.
 */
typedef struct
{
  uint32  proc_id ;
  char           *proc_name ;
} SOC_PROCEDURE_DESC_ELEMENT ;

/* To be used to init SOC_PROCEDURE_DESC_ELEMENT with single argument, when one wants
 * to use a macro for both proc_id and proc_name
 */
#define SOC_PROCEDURE_DESC_ELEMENT_DEF(proc_desc) {proc_desc, #proc_desc}
#define SOC_PROCEDURE_DESC_ELEMENT_DEF_LAST SOC_PROCEDURE_DESC_ELEMENT_DEF((uint32)SOC_SAND_END_PROC_LIST)

#define SOC_ERR_DESC_ELEMENT_DEF_LAST \
  { \
    (uint32)SOC_SAND_END_ERR_LIST, \
    "", \
    "", \
    SOC_SAND_SVR_FTL, \
    FALSE \
  }

#define SOC_SAND_PROC_BITS_SET(_module_id)  \
  (_module_id    << (SOC_PROC_ID_NUM_BITS - SOC_SAND_MODULE_ID_NUM_BITS))

/*
 * The following bit masks are or'ed with procedure id's within the various
 * modules, to identify the owner module
 */
#define SOC_SAND_MODULE_IDENTIFIER          1
#define SOC_SAND_PROC_BITS                 SOC_SAND_PROC_BITS_SET(SOC_SAND_MODULE_IDENTIFIER)

#define FE200_MODULE_IDENTIFIER					2

#define FAP10M_MODULE_IDENTIFIER				3

#define FMF_MODULE_IDENTIFIER						4

#define FAP20V_MODULE_IDENTIFIER				5


#define TIMNA_MODULE_IDENTIFIER					7


#define FAP20M_MODULE_IDENTIFIER				9

#define SOC_PETRA_MODULE_IDENTIFIER					10
#define SOC_PETRA_PROC_BITS SOC_SAND_PROC_BITS_SET(SOC_PETRA_MODULE_IDENTIFIER)

#define FAP21V_MODULE_IDENTIFIER				11

#define SOC_PETRA_PP_MODULE_IDENTIFIER			12

#define FE600_MODULE_IDENTIFIER					13

#define T20E_MODULE_IDENTIFIER					14

#define SOC_PB_MODULE_IDENTIFIER						15
#define SOC_PB_PROC_BITS SOC_SAND_PROC_BITS_SET(SOC_PB_MODULE_IDENTIFIER)

#define SOC_PPD_MODULE_IDENTIFIER						16
#define SOC_PPD_PROC_BITS SOC_SAND_PROC_BITS_SET(SOC_PPD_MODULE_IDENTIFIER)

#define PPH_MODULE_IDENTIFIER						17

#define OAM_MODULE_IDENTIFIER						18

#define SOC_PB_PP_MODULE_IDENTIFIER					19
#define SOC_PB_PP_PROC_BITS SOC_SAND_PROC_BITS_SET(SOC_PB_PP_MODULE_IDENTIFIER)

#define TMD_MODULE_IDENTIFIER						20

#define SOC_TMC_MODULE_IDENTIFIER						21
#define SOC_TMC_PROC_BITS SOC_SAND_PROC_BITS_SET(SOC_TMC_MODULE_IDENTIFIER)

#define PCP_MODULE_IDENTIFIER						22
#define SOC_PCP_PROC_BITS SOC_SAND_PROC_BITS_SET(PCP_MODULE_IDENTIFIER)

#define ARAD_MODULE_IDENTIFIER					23
#define ARAD_PROC_BITS SOC_SAND_PROC_BITS_SET(ARAD_MODULE_IDENTIFIER)

#define ARAD_PP_MODULE_IDENTIFIER					25
#define ARAD_PP_PROC_BITS SOC_SAND_PROC_BITS_SET(ARAD_PP_MODULE_IDENTIFIER)


/*
 * }
 */
/*
 * List of SOC_SAND procedure identifiers.
 * {
 */
/*
 * All soc_sand procedures have SOC_SAND_PROC_BIT set. This
 * distinguishes soc_sand procedures from upper layer procedures.
 */
#define SOC_SAND_MODULE_OPEN                                    (1  | SOC_SAND_PROC_BITS)
#define SOC_SAND_MODULE_CLOSE                                   (2  | SOC_SAND_PROC_BITS)
#define SOC_SAND_PHYSICAL_READ_FROM_CHIP                        (3  | SOC_SAND_PROC_BITS)
#define SOC_SAND_PHYSICAL_WRITE_TO_CHIP                         (4  | SOC_SAND_PROC_BITS)
#define SOC_SAND_READ_MODIFY_WRITE                              (5  | SOC_SAND_PROC_BITS)
#define SOC_SAND_DEVICE_REGISTER                                (6  | SOC_SAND_PROC_BITS)
#define SOC_SAND_DEVICE_UNREGISTER                              (7  | SOC_SAND_PROC_BITS)
#define SOC_SAND_HANDLES_INIT_HANDLES                           (8  | SOC_SAND_PROC_BITS)
#define SOC_SAND_HANDLES_SHUT_DOWN_HANDLES                      (9  | SOC_SAND_PROC_BITS)
#define SOC_SAND_HANDLES_REGISTER_HANDLE                        (10 | SOC_SAND_PROC_BITS)
#define SOC_SAND_HANDLES_UNREGISTER_HANDLE                      (11 | SOC_SAND_PROC_BITS)
#define SOC_SAND_HANDLES_UNREGISTER_ALL_DEVICE_HANDLES          (12 | SOC_SAND_PROC_BITS)
#define SOC_SAND_CLEAR_CHIP_DESCRIPTOR                          (13 | SOC_SAND_PROC_BITS)
#define SOC_SAND_INIT_CHIP_DESCRIPTORS                          (14 | SOC_SAND_PROC_BITS)
#define SOC_SAND_DELETE_CHIP_DESCRIPTORS                        (15 | SOC_SAND_PROC_BITS)
#define SOC_SAND_REMOVE_CHIP_DESCRIPTOR                         (16 | SOC_SAND_PROC_BITS)
#define SOC_SAND_ADD_CHIP_DESCRIPTOR                            (17 | SOC_SAND_PROC_BITS)
#define SOC_SAND_REGISTER_EVENT_CALLBACK                        (18 | SOC_SAND_PROC_BITS)
#define SOC_SAND_UNREGISTER_EVENT_CALLBACK                      (19 | SOC_SAND_PROC_BITS)
#define SOC_SAND_COMBINE_2_EVENT_CALLBACK_HANDLES               (20 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_MUTEX_OWNER                (21 | SOC_SAND_PROC_BITS)
#define SOC_SAND_SET_CHIP_DESCRIPTOR_MUTEX_OWNER                (22 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_MUTEX_COUNTER              (23 | SOC_SAND_PROC_BITS)
#define SOC_SAND_SET_CHIP_DESCRIPTOR_MUTEX_COUNTER              (24 | SOC_SAND_PROC_BITS)
#define SOC_SAND_INC_CHIP_DESCRIPTOR_MUTEX_COUNTER              (25 | SOC_SAND_PROC_BITS)
#define SOC_SAND_DEC_CHIP_DESCRIPTOR_MUTEX_COUNTER              (26 | SOC_SAND_PROC_BITS)
#define SOC_SAND_TAKE_CHIP_DESCRIPTOR_MUTEX                     (27 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GIVE_CHIP_DESCRIPTOR_MUTEX                     (28 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_BASE_ADDR                  (29 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_MEMORY_SIZE                (30 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_INTERRUPT_CALLBACK_ARRAY   (31 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_UNMASK_FUNC                (32 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_IS_BIT_AC_FUNC             (33 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_IS_DEVICE_INT_MASKED_FUNC  (34 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_GET_INT_MASK_FUNC          (35 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_DEV_BETWEEN_ISR_AND_TCM    (36 | SOC_SAND_PROC_BITS)
#define SOC_SAND_SET_CHIP_DESCRIPTOR_DEV_BETWEEN_ISR_AND_TCM    (37 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_INT_MASK_COUNTER           (38 | SOC_SAND_PROC_BITS)
#define SOC_SAND_INC_CHIP_DESCRIPTOR_INT_MASK_COUNTER           (39 | SOC_SAND_PROC_BITS)
#define SOC_SAND_DEC_CHIP_DESCRIPTOR_INT_MASK_COUNTER           (40 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_CHIP_TYPE                  (43 | SOC_SAND_PROC_BITS)
#define SOC_SAND_IS_CHIP_DESCRIPTOR_VALID                       (44 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_MAGIC                      (45 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GENERAL_SET_MAX_NUM_DEVICES                    (46 | SOC_SAND_PROC_BITS)
#define SOC_SAND_MODULE_INIT_ALL                                (47 | SOC_SAND_PROC_BITS)
#define SOC_SAND_MODULE_END_ALL                                 (48 | SOC_SAND_PROC_BITS)
#define SOC_SAND_TCM_SEND_MESSAGE_TO_Q_FROM_TASK                (49 | SOC_SAND_PROC_BITS)
#define SOC_SAND_TCM_SEND_MESSAGE_TO_Q_FROM_INT                 (50 | SOC_SAND_PROC_BITS)
#define SOC_SAND_TCM_CALLBACK_ENGINE_START                      (51 | SOC_SAND_PROC_BITS)
#define SOC_SAND_TCM_REGISTER_POLLING_CALLBACK                  (52 | SOC_SAND_PROC_BITS)
#define SOC_SAND_TCM_UNREGISTER_POLLING_CALLBACK                (53 | SOC_SAND_PROC_BITS)
#define SOC_SAND_USER_CALLBACK_UNREGISTER_DEVICE                (54 | SOC_SAND_PROC_BITS)
#define SOC_SAND_UNPACK_RX_SR_DATA_CELL                         (55 | SOC_SAND_PROC_BITS)
#define SOC_SAND_PACK_TX_SR_DATA_CELL                           (56 | SOC_SAND_PROC_BITS)
#define SOC_SAND_INDIRECT_SET_INFO                              (57 | SOC_SAND_PROC_BITS)
#define SOC_SAND_INDIRECT_CLEAR_INFO                            (58 | SOC_SAND_PROC_BITS)
#define SOC_SAND_INDIRECT_CLEAR_INFO_ALL                        (59 | SOC_SAND_PROC_BITS)
#define SOC_SAND_INDIRECT_CHECK_REQUEST_LEGAL                   (60 | SOC_SAND_PROC_BITS)
#define SOC_SAND_INDIRECT_VERIFY_TRIGGER_0                      (61 | SOC_SAND_PROC_BITS)
#define SOC_SAND_INDIRECT_WRITE_ADDRESS                         (62 | SOC_SAND_PROC_BITS)
#define SOC_SAND_INDIRECT_WRITE_VALUE                           (63 | SOC_SAND_PROC_BITS)
#define SOC_SAND_INDIRECT_ASSERT_TRIGGER_1                      (64 | SOC_SAND_PROC_BITS)
#define SOC_SAND_INDIRECT_READ_RESULT                           (65 | SOC_SAND_PROC_BITS)
#define SOC_SAND_MEM_INTERRUPT_MASK_ADDRESS_CLEAR               (66 | SOC_SAND_PROC_BITS)
#define SOC_SAND_MEM_READ                                       (67 | SOC_SAND_PROC_BITS)
#define SOC_SAND_MEM_WRITE                                      (68 | SOC_SAND_PROC_BITS)
#define SOC_SAND_TRIGGER_VERIFY_0                               (69 | SOC_SAND_PROC_BITS)
#define SOC_SAND_TRIGGER_ASSERT_1                               (70 | SOC_SAND_PROC_BITS)
#define SOC_SAND_DELTA_LIST_TAKE_MUTEX                          (71 | SOC_SAND_PROC_BITS)
#define SOC_SAND_DELTA_LIST_GIVE_MUTEX                          (72 | SOC_SAND_PROC_BITS)
#define SOC_SAND_DELTA_LIST_DESTROY                             (73 | SOC_SAND_PROC_BITS)
#define SOC_SAND_DELTA_LIST_INSERT_D                            (74 | SOC_SAND_PROC_BITS)
#define SOC_SAND_DELTA_LIST_DECREASE_TIME_FROM_HEAD             (75 | SOC_SAND_PROC_BITS)
#define SOC_SAND_DELTA_LIST_REMOVE                              (76 | SOC_SAND_PROC_BITS)
#define SOC_SAND_INIT_ERRORS_QUEUE                              (77 | SOC_SAND_PROC_BITS)
#define SOC_SAND_SET_USER_ERROR_HANDLER                         (78 | SOC_SAND_PROC_BITS)
#define SOC_SAND_LOAD_ERRORS_QUEUE                              (79 | SOC_SAND_PROC_BITS)
#define SOC_SAND_DELETE_ERRORS_QUEUE                            (80 | SOC_SAND_PROC_BITS)
#define SOC_SAND_ERROR_HANDLER                                  (81 | SOC_SAND_PROC_BITS)
#define SOC_SAND_UNLOAD_ERRORS_QUEUE                            (82 | SOC_SAND_PROC_BITS)
#define SOC_SAND_TCM_CALLBACK_ENGINE_MAIN                       (83 | SOC_SAND_PROC_BITS)
#define SOC_SAND_MEM_READ_CALLBACK                              (84 | SOC_SAND_PROC_BITS)
#define SOC_SAND_INDIRECT_READ_FROM_CHIP                        (85 | SOC_SAND_PROC_BITS)
#define SOC_SAND_INDIRECT_WRITE_TO_CHIP                         (86 | SOC_SAND_PROC_BITS)
#define SOC_SAND_READ_FIELD                                     (87 | SOC_SAND_PROC_BITS)
#define SOC_SAND_WRITE_FIELD                                    (88 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_DEVICE_AT_INIT             (89 | SOC_SAND_PROC_BITS)
#define SOC_SAND_SET_CHIP_DESCRIPTOR_DEVICE_AT_INIT             (90 | SOC_SAND_PROC_BITS)
#define SOC_SAND_TCM_CALLBACK_DELTA_LIST_TAKE_MUTEX             (91 | SOC_SAND_PROC_BITS)
#define SOC_SAND_SET_CHIP_DESCRIPTOR_VER_INFO                   (92 | SOC_SAND_PROC_BITS)
#define SOC_SAND_DEVICE_MNGMNT_LOAD_CHIP_VER                    (93 | SOC_SAND_PROC_BITS)
#define SOC_SAND_IS_CHIP_DESCRIPTOR_CHIP_VER_BIGGER_EQ          (94 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_CHIP_VER                   (95 | SOC_SAND_PROC_BITS)
#define SOC_SAND_GET_CHIP_DESCRIPTOR_DBG_VER                    (96 | SOC_SAND_PROC_BITS)
#define SOC_SAND_DELTA_LIST_DECREASE_TIME_FROM_SECOND_ITEM      (97 | SOC_SAND_PROC_BITS)
#define SOC_SAND_DEVICE_MNGMNT_GET_DEVICE_TYPE                  (98 | SOC_SAND_PROC_BITS)
#define SOC_SAND_SSR_GET_VER_FROM_HEADER                        (99 | SOC_SAND_PROC_BITS)
#define SOC_SAND_SSR_GET_SIZE_FROM_HEADER                       (100| SOC_SAND_PROC_BITS)
#define SOC_SAND_MEM_WRITE_UNSAFE                               (101| SOC_SAND_PROC_BITS)
#define SOC_SAND_MEM_READ_UNSAFE                                (102| SOC_SAND_PROC_BITS)
#define SOC_SAND_INDIRECT_READ_LOW                              (103| SOC_SAND_PROC_BITS)
#define SOC_SAND_INDIRECT_WRITE_LOW                             (104| SOC_SAND_PROC_BITS)
#define SOC_SAND_BITSTREAM_SET_ANY_FIELD                        (105| SOC_SAND_PROC_BITS)
#define SOC_SAND_BITSTREAM_GET_ANY_FIELD                        (106| SOC_SAND_PROC_BITS)
#define SOC_SAND_IP_ADDR_NUMERIC_TO_STRING                      (107| SOC_SAND_PROC_BITS)
#define SOC_SAND_IP_ADDR_STRING_TO_NUMERIC                      (108| SOC_SAND_PROC_BITS)
#define SOC_SAND_INDIRECT_WRITE_IND_INFO_LOW                    (109| SOC_SAND_PROC_BITS)
#define SOC_SAND_TBL_READ                                       (110| SOC_SAND_PROC_BITS)
#define SOC_SAND_TBL_READ_UNSAFE                                (111| SOC_SAND_PROC_BITS)
#define SOC_SAND_TBL_WRITE                                      (112| SOC_SAND_PROC_BITS)
#define SOC_SAND_TBL_WRITE_UNSAFE                               (113| SOC_SAND_PROC_BITS)
#define SOC_SAND_TBL_READ_LOW                                   (114| SOC_SAND_PROC_BITS)
#define SOC_SAND_TBL_WRITE_LOW                                  (115| SOC_SAND_PROC_BITS)
#define SOC_SAND_INDIRECT_READ_IND_INFO_LOW                     (116| SOC_SAND_PROC_BITS)
/*
 * BITMAP
 */
#define SOC_SAND_OCC_BM_CLEAR                                   (118| SOC_SAND_PROC_BITS)
#define SOC_SAND_OCC_BM_CREATE                                  (119| SOC_SAND_PROC_BITS)
#define SOC_SAND_OCC_BM_DESTROY                                 (120| SOC_SAND_PROC_BITS)
#define SOC_SAND_OCC_BM_GET_NEXT                                (121| SOC_SAND_PROC_BITS)
#define SOC_SAND_OCC_BM_PRINT                                   (122| SOC_SAND_PROC_BITS)
#define SOC_SAND_OCC_BM_GET_NEXT_HELPER                         (123| SOC_SAND_PROC_BITS)
#define SOC_SAND_OCC_BM_OCCUP_STATUS_SET                        (124| SOC_SAND_PROC_BITS)
#define SOC_SAND_OCC_BM_OCCUP_STATUS_SET_HELPER                 (125| SOC_SAND_PROC_BITS)
#define SOC_SAND_OCC_BM_ALLOC_NEXT                              (126| SOC_SAND_PROC_BITS)
#define SOC_SAND_OCC_BM_INIT                                    (127| SOC_SAND_PROC_BITS)
/*
 * Hash Table
 */
#define SOC_SAND_HASH_TABLE_CREATE                              (130| SOC_SAND_PROC_BITS)
#define SOC_SAND_HASH_TABLE_DESTROY                             (131| SOC_SAND_PROC_BITS)
#define SOC_SAND_HASH_TABLE_ENTRY_LOOKUP                        (132| SOC_SAND_PROC_BITS)
#define SOC_SAND_HASH_TABLE_ENTRY_ADD                           (133| SOC_SAND_PROC_BITS)

#define SOC_SAND_HASH_TABLE_ENTRY_REMOVE                        (135| SOC_SAND_PROC_BITS)
#define SOC_SAND_HASH_TABLE_ENTRY_REMOVE_BY_INDEX               (136| SOC_SAND_PROC_BITS)
#define SOC_SAND_HASH_TABLE_GET_NEXT                            (137| SOC_SAND_PROC_BITS)
#define SOC_SAND_HASH_TABLE_PRINT                               (138| SOC_SAND_PROC_BITS)
#define SOC_SAND_HASH_TABLE_FIND_ENTRY                          (139| SOC_SAND_PROC_BITS)
#define SOC_SAND_HASH_TABLE_INIT                                (140| SOC_SAND_PROC_BITS)
/*
 * Group Member Linked List
 */
#define SOC_SAND_GROUP_MEM_LL_IS_GROUP_EMPTY                    (129| SOC_SAND_PROC_BITS)
#define SOC_SAND_GROUP_MEM_LL_CLEAR                             (134| SOC_SAND_PROC_BITS)
#define SOC_SAND_GROUP_MEM_LL_CREATE                            (140| SOC_SAND_PROC_BITS)
#define SOC_SAND_GROUP_MEM_LL_DESTROY                           (141| SOC_SAND_PROC_BITS)
#define SOC_SAND_GROUP_MEM_LL_MEMBERS_SET                       (142| SOC_SAND_PROC_BITS)
#define SOC_SAND_GROUP_MEM_LL_MEMBER_ADD                        (143| SOC_SAND_PROC_BITS)
#define SOC_SAND_GROUP_MEM_LL_MEMBER_REMOVE                     (144| SOC_SAND_PROC_BITS)
#define SOC_SAND_GROUP_MEM_LL_MEMBERS_GET                       (145| SOC_SAND_PROC_BITS)
#define SOC_SAND_GROUP_MEM_LL_MEMBER_GET_GROUP                  (146| SOC_SAND_PROC_BITS)
#define SOC_SAND_GROUP_MEM_LL_MEMBER_CLEAR                      (147| SOC_SAND_PROC_BITS)
#define SOC_SAND_GROUP_MEM_LL_MEMBER_PRINT                      (148| SOC_SAND_PROC_BITS)
#define SOC_SAND_GROUP_MEM_LL_FUNC_RUN                          (149| SOC_SAND_PROC_BITS)
/*
 * memory management
 */
#define SOC_SAND_ARR_MEM_ALLOCATOR_CREATE                       (150| SOC_SAND_PROC_BITS)
#define SOC_SAND_ARR_MEM_ALLOCATOR_DESTROY                      (151| SOC_SAND_PROC_BITS)
#define SOC_SAND_ARR_MEM_ALLOCATOR_MALLOC                       (152| SOC_SAND_PROC_BITS)
#define SOC_SAND_ARR_MEM_ALLOCATOR_FREE                         (153| SOC_SAND_PROC_BITS)
#define SOC_SAND_ARR_MEM_ALLOCATOR_READ                         (154| SOC_SAND_PROC_BITS)
#define SOC_SAND_ARR_MEM_ALLOCATOR_WRITE                        (155| SOC_SAND_PROC_BITS)
#define SOC_SAND_ARR_MEM_ALLOCATOR_READ_BLOCK                   (156| SOC_SAND_PROC_BITS)
#define SOC_SAND_ARR_MEM_ALLOCATOR_CLEAR                        (158| SOC_SAND_PROC_BITS)
#define SOC_SAND_ARR_MEM_ALLOCATOR_BLOCK_SIZE                   (159| SOC_SAND_PROC_BITS)

/*
 * SOC_SAND PP General
 */
#define SOC_SAND_PP_IPV4_SUBNET_VERIFY                          (157| SOC_SAND_PROC_BITS)
#define SOC_SAND_PP_TRILL_DEST_VERIFY                           (158| SOC_SAND_PROC_BITS)

/*
 * SOC_SAND PP MAC
 */
#define SOC_SAND_PP_MAC_ADDRESS_STRUCT_TO_LONG                  (160| SOC_SAND_PROC_BITS)
#define SOC_SAND_PP_MAC_ADDRESS_LONG_TO_STRUCT                  (161| SOC_SAND_PROC_BITS)
#define SOC_SAND_PP_MAC_ADDRESS_STRING_PARSE                    (162| SOC_SAND_PROC_BITS)
#define SOC_SAND_PP_MAC_ADDRESS_INC                             (163| SOC_SAND_PROC_BITS)
#define SOC_SAND_PP_MAC_ADDRESS_ADD                             (164| SOC_SAND_PROC_BITS)
#define SOC_SAND_PP_MAC_ADDRESS_SUB                             (165| SOC_SAND_PROC_BITS)
#define SOC_SAND_PP_MAC_ADDRESS_REVERSE                         (166| SOC_SAND_PROC_BITS)
#define SOC_SAND_PP_MAC_ADDRESS_ARE_EQUAL                       (167| SOC_SAND_PROC_BITS)

/*
 * Multi set
 */
#define SOC_SAND_MULTI_SET_CREATE                              (170| SOC_SAND_PROC_BITS)
#define SOC_SAND_MULTI_SET_DESTROY                             (171| SOC_SAND_PROC_BITS)
#define SOC_SAND_MULTI_SET_MEMBER_LOOKUP                       (172| SOC_SAND_PROC_BITS)
#define SOC_SAND_MULTI_SET_MEMBER_ADD                          (173| SOC_SAND_PROC_BITS)
#define SOC_SAND_MULTI_SET_MEMBER_REMOVE_BY_INDEX              (175| SOC_SAND_PROC_BITS)
#define SOC_SAND_MULTI_SET_MEMBER_REMOVE                       (176| SOC_SAND_PROC_BITS)
#define SOC_SAND_MULTI_SET_GET_NEXT                            (177| SOC_SAND_PROC_BITS)
#define SOC_SAND_MULTI_SET_PRINT                               (178| SOC_SAND_PROC_BITS)
#define SOC_SAND_MULTI_SET_INIT                                (179| SOC_SAND_PROC_BITS)
/*
 * sorted list
 */
#define SOC_SAND_SORTED_LIST_CREATE                            (190| SOC_SAND_PROC_BITS)
#define SOC_SAND_SORTED_LIST_DESTROY                           (191| SOC_SAND_PROC_BITS)
#define SOC_SAND_SORTED_LIST_ENTRY_LOOKUP                      (192| SOC_SAND_PROC_BITS)
#define SOC_SAND_SORTED_LIST_ENTRY_ADD                         (193| SOC_SAND_PROC_BITS)
#define SOC_SAND_SORTED_LIST_ENTRY_UPDATE                      (194| SOC_SAND_PROC_BITS)
#define SOC_SAND_SORTED_LIST_ENTRY_REMOVE                      (195| SOC_SAND_PROC_BITS)
#define SOC_SAND_SORTED_LIST_GET_NEXT                          (196| SOC_SAND_PROC_BITS)
#define SOC_SAND_SORTED_LIST_GET_PREV                          (197| SOC_SAND_PROC_BITS)
#define SOC_SAND_SORTED_LIST_PRINT                             (198| SOC_SAND_PROC_BITS)
#define SOC_SAND_SORTED_LIST_FIND_MATCH_ENTRY                  (199| SOC_SAND_PROC_BITS)
#define SOC_SAND_SORTED_LIST_ENTRY_ADD_BY_ITER                 (200| SOC_SAND_PROC_BITS)
#define SOC_SAND_SORTED_LIST_ENTRY_REMOVE_BY_ITER              (201| SOC_SAND_PROC_BITS)
#define SOC_SAND_SORTED_LIST_ENTRY_VALUE                       (202| SOC_SAND_PROC_BITS)
#define SOC_SAND_SORTED_LIST_GET_FOLLOW                        (203| SOC_SAND_PROC_BITS)
#define SOC_SAND_SORTED_LIST_INIT                              (204| SOC_SAND_PROC_BITS)

/*
 * PAT tree
 */
#define SOC_SAND_PAT_TREE_CREATE                               (210| SOC_SAND_PROC_BITS)
#define SOC_SAND_PAT_TREE_DESTROY                              (211| SOC_SAND_PROC_BITS)
#define SOC_SAND_PAT_TREE_CLEAR                                (212| SOC_SAND_PROC_BITS)
#define SOC_SAND_PAT_TREE_NODE_ADD                             (213| SOC_SAND_PROC_BITS)
#define SOC_SAND_PAT_TREE_NODE_REMOVE                          (214| SOC_SAND_PROC_BITS)
#define SOC_SAND_PAT_TREE_LPM_GET                              (215| SOC_SAND_PROC_BITS)
#define SOC_SAND_PAT_TREE_GET_BLOCK                            (216| SOC_SAND_PROC_BITS)
#define SOC_SAND_PAT_TREE_GET_SIZE                             (217| SOC_SAND_PROC_BITS)
#define SOC_SAND_PAT_TREE_PRINT                                (218| SOC_SAND_PROC_BITS)

/*
* Soc_sand cell
*/
#define SOC_SAND_PACK_DEST_ROUTED_DATA_CELL                    (219| SOC_SAND_PROC_BITS)
#define SOC_SAND_UNPACK_DEST_ROUTED_DATA_CELL                  (220| SOC_SAND_PROC_BITS)
#define SOC_SAND_PACK_SOURCE_ROUTED_DATA_CELL                  (221| SOC_SAND_PROC_BITS)
#define SOC_SAND_UNPACK_SOURCE_ROUTED_DATA_CELL                (222| SOC_SAND_PROC_BITS)
#define SOC_SAND_DATA_CELL_TO_BUFFER                           (223| SOC_SAND_PROC_BITS)
#define SOC_SAND_BUFFER_TO_DATA_CELL                           (224| SOC_SAND_PROC_BITS)

/* 
 * Exact_match 
 */
#define SOC_SAND_EXACT_MATCH_MALLOC_STACK                      (225| SOC_SAND_PROC_BITS)
#define SOC_SAND_EXACT_MATCH_FREE_STACK                        (226| SOC_SAND_PROC_BITS)
#define SOC_SAND_EXACT_MATCH_CREATE                            (227| SOC_SAND_PROC_BITS)
#define SOC_SAND_EXACT_MATCH_SIZE_OUT_OF_RANGE_ERR             (228| SOC_SAND_PROC_BITS)
#define SOC_SAND_EXACT_MATCH_DESTROY                           (229| SOC_SAND_PROC_BITS)
#define SOC_SAND_EXACT_MATCH_ENTRY_ADD                         (230| SOC_SAND_PROC_BITS)
#define SOC_SAND_EXACT_MATCH_ENTRY_REMOVE                      (231| SOC_SAND_PROC_BITS)
#define SOC_SAND_EXACT_MATCH_ENTRY_LOOKUP                      (232| SOC_SAND_PROC_BITS)
#define SOC_SAND_EXACT_MATCH_CLEAR                             (233| SOC_SAND_PROC_BITS)
#define SOC_SAND_EXACT_MATCH_GET_BLOCK                         (234| SOC_SAND_PROC_BITS)

/*
 * Soc_sand error code
 */


/*
 * System wide indicator of end of procedure descriptors list.
 */
#define SOC_SAND_END_PROC_LIST                                  (-1)
/*
 * } END List of SOC_SAND procedure identifiers
 */
/*
 * List of soc_sand error codes
 */
typedef enum
{
  SOC_SAND_NO_ERR = SOC_SAND_OK,
  /*
   * First error code is 1
   */
  SOC_SAND_GEN_ERR = SOC_SAND_ERR,
  /*
   * Failure trying to do memory allocation.
   */
  SOC_SAND_MALLOC_FAIL,
  /*
   * Failure trying to do mutex allocation.
   */
  SOC_SAND_MUTEX_CREATE_FAIL,
  /*
   * NULL pointer was given, were not allowed.
   */
  SOC_SAND_NULL_POINTER_ERR,
  /*
   * One of the driver-usr callback function that was given
   * to the driver was NULL function.
   * E.g., E.g., 'fap10m_register_device()' reset_device_ptr was set to NULL
   * and 'fap10m_reset_device()' was called..
   */
  SOC_SAND_NULL_USER_CALLBACK_FUNC,
  /*
   * Tried to set or illegal/unknown device identifier.
   */
  SOC_SAND_MAX_NUM_DEVICES_OUT_OF_RANGE_ERR,
  /*
   * Driver has not been started. Can not activate
   * driver services.
   */
  SOC_SAND_DRIVER_NOT_STARTED,
  /*
   * Driver has already been started. Can not activate
   * driver again.
   */
  SOC_SAND_DRIVER_ALREADY_STARTED,
  /*
   * Just a message: Driver is currently busy. Try
   * again, after timeout, until driver gets free.
   */
  SOC_SAND_DRIVER_BUSY,
  /*
   * Illegal/unknown device identifier as input parameter
   */
  SOC_SAND_ILLEGAL_DEVICE_ID,
  /*
   * Illegal chip descriptor for specified device handle.
   */
  SOC_SAND_ILLEGAL_CHIP_DESCRIPTOR,
  /*
   * Illegal chip descriptor for specified device handle
   * because it has been deleted.
   */
  SOC_SAND_TRYING_TO_ACCESS_DELETED_DEVICE,
  /*
   * currently undefined.
   */
  SOC_SAND_SYSTEM_TICK_ERR_01,
  /*
   * currently undefined.
   */
  SOC_SAND_TCM_TASK_PRIORITY_ERR_01,
  /*
   * currently undefined.
   */
  SOC_SAND_MIN_TIME_BETWEEN_POLLS_ERR_01,
  /*
   * currently undefined.
   */
  SOC_SAND_TCM_MOCKUP_INTERRUPTS_ERR_01,
  /*
   * Procedure soc_sand_add_sand_errors() returned with
   * error indication in soc_sand_module_open().
   */
  SOC_SAND_USER_ADD_SAND_ERRORS_ERR_01,
  /*
   * Procedure soc_sand_add_sand_procedures() returned with
   * error indication in soc_sand_module_open().
   */
  SOC_SAND_USER_ADD_SAND_PROCEDURES_ERR_01,
  /*
   * Procedure soc_sand_set_user_error_handler() returned with
   * error indication in soc_sand_module_open().
   */
  SOC_SAND_USER_ERROR_HANDLER_ERR_01,
  /*
   * Procedure soc_sand_module_end_all() returned with
   * error indication in soc_sand_module_open().
   */
  SOC_SAND_MODULE_END_ALL_ERR_01,
  /*
   * Procedure soc_sand_module_init_all() returned with
   * error indication in soc_sand_module_open().
   */
  SOC_SAND_MODULE_INIT_ALL_ERR_01,
  /*
   * Procedure soc_sand_mem_interrupt_mask_address_clear_all()
   * returned with error indication in soc_sand_module_open().
   */
  SOC_SAND_INTERRUPT_CLEAR_ALL_ERR_01,
  /*
   * soc_sand_init_chip_descriptors failed.
   */
  SOC_SAND_INIT_CHIP_DESCRIPTORS_ERR_01,
  /*
   * soc_sand_handles_init_handles failed.
   */
  SOC_SAND_HANDLES_INIT_ERR_01,
  /*
   * soc_sand_tcm_callback_engine_start failed.
   */
  SOC_SAND_TCM_CALLBACK_ENGINE_START_ERR_01,
  /*
   * soc_sand_indirect_clear_info_all failed.
   */
  SOC_SAND_INDIRECT_SET_INFO_ERR_01,
  /*
   * Tried to access not exist module in the indirect zone.
   */
  SOC_SAND_INDIRECT_MODULE_NOT_EXIST,
  /*
   * Procedure soc_sand_register_device() reports:
   * Maximal number of devices has already been
   * registered. Can not register more.
   */
  SOC_SAND_REGISTER_DEVICE_001,
  /*
   * Deprecated Error - do not use.
   */
  SOC_SAND_DEPRECATED_ERROR_00001,
  /*
   * Procedure soc_sand_register_device() reports:
   * No live device has been found trying to access
   * specified memory.
   */
  SOC_SAND_REGISTER_DEVICE_003,
  /*
   * Procedure soc_sand_register_device() reports:
   * Input parameter 'base_address' is not
   * long-aligned.
   */
  SOC_SAND_REGISTER_DEVICE_004,
  /*
   * Procedure soc_sand_register_device() reports:
   * Chip type as reported by chip itself does not match
   * registered chip type.
   */
  SOC_SAND_REGISTER_DEVICE_005,
  /*
   * Procedure soc_sand_register_device() reports:
   * User supplied buffer is NULL.
   */
  SOC_SAND_REGISTER_DEVICE_NULL_BUFFER,
  /*
   * Procedure soc_sand_register_device() reports:
   * Descriptors array / mutex are not initialized.
   */
  SOC_SAND_DESC_ARRAY_NOT_INIT,
  /*
   * Procedure soc_sand_register_device() reports:
   * Unknown error return value from soc_sand_add_chip_descriptor().
   */
  SOC_SAND_ADD_CHIP_DESCRIPTOR_ERR,
  /*
   * Failure trying to create semaphore.
   */
  SOC_SAND_SEM_CREATE_FAIL,
  /*
   * Failure trying to delete semaphore.
   */
  SOC_SAND_SEM_DELETE_FAIL,
  /*
   * Failure trying to take semaphore.
   */
  SOC_SAND_SEM_TAKE_FAIL,
  /*
   * When calling RevB+ function while running over RevA chip
   */
  SOC_SAND_CHIP_VER_IS_A,
  /*
   * When calling RevA only function while running over RevB+ chip
   */
  SOC_SAND_CHIP_VER_IS_NOT_A,
  /*
   * Failure trying to take tcm callback delta list semaphore.
   */
  SOC_SAND_TCM_CALLBACK_DELTA_LIST_SEM_TAKE_FAIL,
  /*
   * Failure trying to take chip descriptors semaphore.
   */
  SOC_SAND_CHIP_DESCRIPTORS_SEM_TAKE_ERR_0,
  /*
   * Failure trying to take semaphore.
   */
  SOC_SAND_SEM_GIVE_FAIL,
  /*
   * Semaphore take has succeeded but it is not the one
   * which was intended, initially (It was deleted and
   * replaced).
   */
  SOC_SAND_SEM_CHANGED_ON_THE_FLY,
  /*
   * This error relates to indirect access only.
   * Timeout waiting for chip to deassert the 'trigger'
   * bit.
   */
  SOC_SAND_IND_TRIGGER_TIMEOUT,
  /*
   * Procedure soc_sand_user_callback_unregister_device() or
   * soc_sand_clear_all_device_peding_services() reports:
   * Failed to delete some of the device's active handles
   */
  SOC_SAND_UNREGISTER_DEVICE_001,
  /*
   * Procedure soc_sand_user_callback_unregister_device() reports:
   * Failed to clear device descriptor
   */
  SOC_SAND_UNREGISTER_DEVICE_002,
  /*
   * Procedure soc_sand_mem_read() reports:
   * Input parameter 'offset' is not long-aligned.
   */
   SOC_SAND_MEM_READ_001,
  /*
   * Procedure soc_sand_mem_read() reports:
   * Input parameter 'size' is not a multiple of
   * four.
   */
   SOC_SAND_MEM_READ_002,
  /*
   * Procedure soc_sand_mem_read() reports:
   * This error relates to direct access only.
   * Specified memory range ('offset', 'size')
   * contains some addresses which are not readable
   * by this chip.
   */
   SOC_SAND_MEM_READ_003,
  /*
   * Procedure soc_sand_mem_read() reports:
   * This error relates to indirect access only.
   * Specified module (within 'offset') is illegal.
   */
   SOC_SAND_MEM_READ_004,
  /*
   * Procedure soc_sand_mem_read() reports:
   * This error relates to indirect access only.
   * Specified memory range ('offset', 'size')
   * contains some addresses which are not readable
   * by this chip.
   */
   SOC_SAND_MEM_READ_005,
  /*
   * Procedure soc_sand_mem_read() reports:
   * Failed to read from the chip
   */
   SOC_SAND_MEM_READ_006,
  /*
   * Procedure soc_sand_mem_read() reports:
   * Failed to write to chip (relevant only to indirect)
   */
   SOC_SAND_MEM_READ_007,
  /*
   * Procedure soc_sand_mem_write() reports:
   * Input parameter 'offset' is not long-aligned.
   */
   SOC_SAND_MEM_WRITE_001,
  /*
   * Procedure soc_sand_mem_write() reports:
   * Input parameter 'size' is not a multiple of
   * four.
   */
   SOC_SAND_MEM_WRITE_002,
  /*
   * Procedure soc_sand_mem_write() reports:
   * This error relates to direct access only.
   * Specified memory range ('offset', 'size')
   * contains some addresses which are not writable
   * by this chip.
   */
   SOC_SAND_MEM_WRITE_003,
  /*
   * Procedure soc_sand_mem_write() reports:
   * This error relates to indirect access only.
   * Specified module (within 'offset') is illegal.
   */
   SOC_SAND_MEM_WRITE_004,
  /*
   * Procedure soc_sand_mem_write() reports:
   * This error relates to indirect access only.
   * Specified memory range ('offset', 'size')
   * contains some addresses which are not writable
   * by this chip.
   */
   SOC_SAND_MEM_WRITE_005,
  /*
   * Procedure soc_sand_mem_write() reports:
   * Failed to write to the chip
   */
   SOC_SAND_MEM_WRITE_006,
   /*
    * Timeout on waiting for 'trigger'to be deasserted.
    */
   SOC_SAND_TRG_TIMEOUT,
  /*
   * Called to callback function that do
   * not use interrupt with interrupt request.
   */
  SOC_SAND_CALLBACK_FUNCTION_CANT_BE_INT,
  /*
   * Asked function can not be registered as callback.
   */
  SOC_SAND_NOT_CALLBACK_FUNCTION,
  /*
   * Unrecoverable error encountered in TCM.
   */
  SOC_SAND_TCM_MAIN_ERR_01,
  /*
   * Removed a deferred action due to an error.
   */
  SOC_SAND_TCM_MAIN_ERR_02,
  /*
   * Removed a deferred action due to user registration request.
   */
  SOC_SAND_TCM_MAIN_ERR_03,
  /*
   * Procedure soc_sand_indirect_check_request_legal() reports:
   * Trying to access non-existing indirect module.
   */
  SOC_SAND_NO_SUCH_INDIRECT_MODULE_ERR,
  /*
   * Procedure soc_sand_indirect_check_request_legal() reports:
   * Trying to access non-existing offset/size in the indirect module.
   */
  SOC_SAND_INDIRECT_OFFSET_SIZE_MISMATCH_ERR,
  /*
   * Procedure soc_sand_indirect_check_request_legal() reports:
   * Example to a possible error:
   *   Say specific module answer size is 4 32 bit register (128 bit, 16 byte, 4 longs)
   *   (PEC in SOC_SAND_FAP10M). User gave 5 longs as buffer size.
   *   ((5/4) * 4) ==> 4 longs.
   *   4!=5 ... the result is different than we need. We find a mismatch is sizes.
   *
   */
  SOC_SAND_INDIRECT_SIZE_MISMATCH_ERR,
  /*
   * User gave not valid value in SOC_SAND_CREDIT_SIZE.
   * Please refer to SOC_SAND_CREDIT_SIZE definition.
   */
  SOC_SAND_CREDIT_SIZE_ENUM_ERR,
  /*
   * Functions 'soc_sand_bitstream_get_field()'/'soc_sand_bitstream_set_field()' reports:
   * 'nof_bits' is bigger than 32 (SOC_SAND_BIT_STREAM_FIELD_SET_SIZE).
   * Note, these functions get/set at most 32 bit fields.
   */
  SOC_SAND_BIT_STREAM_FIELD_SET_SIZE_RANGE_ERR,
  /*
   * Function 'XXXX_register_device()' reports:
   * User try to register device in SOC_SAND. SOC_SAND was configured to
   * user real interrupts (via 'soc_sand_module_open()'). This driver
   * do not supports interrupts. Only mock-up interrupts
   * ('soc_tcmmockup_interrupts' indicator in 'soc_sand_module_open()).
   */
  SOC_SAND_DO_NOT_SUPPORTS_INTERRUPTS_ERR,

  /*
   * Divide by Zero error
   */
  SOC_SAND_DIV_BY_ZERO_ERR,

  /*
   * Calculation overflow error
   */
  SOC_SAND_OVERFLOW_ERR,

  /*
   * Different errors use the same error id
   */
  SOC_SAND_ERRORS_FOUND_DUPLICATES_ERR,

  /*
  * Different procs use the same error id
  */
  SOC_SAND_PROCS_FOUND_DUPLICATES_ERR,

  SOC_SAND_ERR_8001, /*  taken */
  SOC_SAND_ERR_8002, /*  taken */
  SOC_SAND_ERR_8003, /*  taken */
  SOC_SAND_ERR_8004, /*  taken */
  SOC_SAND_ERR_8005, /*  taken */
  SOC_SAND_ERR_8006, /*  taken */
  SOC_SAND_ERR_8007, /*  taken */
  SOC_SAND_ERR_8008, /*  taken */
  SOC_SAND_ERR_8009, /*  taken */
  SOC_SAND_ERR_8010, /*  taken */
  SOC_SAND_ERR_8011, /*  taken */
  SOC_SAND_ERR_8012, /*  taken */
  SOC_SAND_ERR_8013, /*  taken */
  SOC_SAND_ERR_8014, /*  taken */
  SOC_SAND_ERR_8015, /*  taken */
  SOC_SAND_ERR_8016, /*  taken */

  /*
   * Internal error!!
   * The indirect information missing offset information.
   */
  SOC_SAND_INDIRECT_CANT_GET_INFO_ERR,

  /*
   * If a tables_prefix A is a subset of tables_prefix B,
   * The structure that contain tables_prefix A, must
   * come before the structure that contains tables_prefix B.
   */
  SOC_SAND_INDIRECT_TABLES_INFO_ORDER_ERR,

  /*
   * Failure trying to free memory.
   */
  SOC_SAND_FREE_FAIL,
  /*
   * Hamming-Code SW module received not correlated data-bit-wide,
   * and p-bit-wide. The correlated number can be found from
   * 'soc_sand_code_hamming_get_p_bit_wide()' func call.
   */
  SOC_SAND_CODE_HAMMING_P_BIT_WIDE_UN_MATCH_ERR,
  /*
   * Hamming-Code SW module received un-supported data-bit-wide.
   * The maximum-p-bit-size we support is 32. Hence,
   * this problem is practically will no happen.
   */
  SOC_SAND_CODE_HAMMING_UN_SUPPORTED_DATA_BIT_WIDE_ERR,

  /*
   * Procedure soc_sand_mem_read_unsafe() reports:
   * Input parameter 'offset' is not long-aligned.
   */
   SOC_SAND_MEM_READ_US_001,
  /*
   * Procedure soc_sand_mem_read_unsafe() reports:
   * Input parameter 'size' is not a multiple of
   * four.
   */
   SOC_SAND_MEM_READ_US_002,
  /*
   * Procedure soc_sand_mem_read_unsafe() reports:
   * This error relates to direct access only.
   * Specified memory range ('offset', 'size')
   * contains some addresses which are not readable
   * by this chip.
   */
   SOC_SAND_MEM_READ_US_003,
  /*
   * Procedure soc_sand_mem_read_unsafe() reports:
   * This error relates to indirect access only.
   * soc_sand_indirect_read_from_chip failed.
   */
   SOC_SAND_MEM_READ_US_005,
  /*
   * Procedure soc_sand_mem_read_unsafe() reports:
   * Failed to read from the chip
   */
   SOC_SAND_MEM_READ_US_006,
  /*
   * Procedure soc_sand_mem_read() reports:
   * error occurred in the soc_sand_mem_read_unsafe() function.
   */
   SOC_SAND_MEM_READ_010,
  /*
   * Procedure soc_sand_mem_write_unsafe() reports:
   * Input parameter 'offset' is not long-aligned.
   */
   SOC_SAND_MEM_WRITE_US_001,
  /*
   * Procedure soc_sand_mem_write_unsafe() reports:
   * This error relates to direct access only.
   * Specified memory range ('offset', 'size')
   * contains some addresses which are not writable
   * by this chip.
   */
   SOC_SAND_MEM_WRITE_US_003,
  /*
   * Procedure soc_sand_mem_write() reports:
   * This error relates to indirect access only.
   * Specified memory range ('offset', 'size')
   * contains some addresses which are not writable
   * by this chip.
   */
   SOC_SAND_MEM_WRITE_US_005,
  /*
   * Procedure soc_sand_mem_write_unsafe() reports:
   * Failed to write to the chip
   */
   SOC_SAND_MEM_WRITE_US_006,
   /*
    * Procedure soc_sand_mem_write() reports:
    * error occurred in the soc_sand_mem_write_unsafe() function.
    */
    SOC_SAND_MEM_WRITE_010,
   /*
    * Failed the 'legal' function.
    */
    SOC_SAND_FAIL_REQUEST_LEGAL,
   /*
    * Failed the verify_0 function.
    */
    SOC_SAND_FAIL_VERIFY_0,
   /*
    * Failed the write_address function.
    */
    SOC_SAND_FAIL_WRITE_ADDRESS,
   /*
    * Failed the write_value function.
    */
    SOC_SAND_FAIL_WRITE_VALUE,
   /*
    * Failed the assert_1 function.
    */
    SOC_SAND_FAIL_ASSERT_1,
   /*
    * Failed the read_result Function.
    */
   SOC_SAND_FAIL_READ_RESULT,


  /*
   * Functions 'soc_sand_set_field() reports:
   * 'ms_bit - ls_bit + 1' is bigger than 32 .
   * Note, these functions set at most 32 bit fields.
   */
  SOC_SAND_BIT_FIELD_SET_SIZE_RANGE_ERR,

  /*
   * Out of range errors
   */
  SOC_SAND_VALUE_OUT_OF_RANGE_ERR,
  SOC_SAND_VALUE_BELOW_MIN_ERR,
  SOC_SAND_VALUE_ABOVE_MAX_ERR,

  /*
   * When a device driver supports the magic number capability,
   *  the user has to call a _clear function, before using
   *  a structure to set information to the device driver.
   */
  SOC_SAND_MAGIC_NUM_ERR,
  /*
   * Illegal IP decimal format. Must be 16 bytes long at least
   * 4 decimal numbers separated by dots
   */
  SOC_SAND_ILLEGAL_IP_FORMAT,

  /*
   * An error in a SOC function occured, with a SOC error code.
   */
  SOC_SAND_SOC_ERR,



 /*
  * Function must not be called when 4k feature is enabled
  */
  SOC_SAND_ILLEGAL_FUNCTION_CALL_WHEN_4K_DISABLED,

  /*
  * Function must not be called when 4k feature is disabled
  */
  SOC_SAND_ILLEGAL_FUNCTION_CALL_WHEN_4K_ENABLED,
 /*
  * Hash table
  */
 /*
  * the given size of the hash table to allocate is out of range.
  * Range is 1-, preferred power of 2
  */
  SOC_SAND_HASH_TABLE_SIZE_OUT_OF_RANGE_ERR,
 /*
  * the hash table is full and trying to add to the hash table,
  * the size of the hash table as allocated in the create.
  * and this size may not change.
  */
  SOC_SAND_HASH_TABLE_IS_FULL_ERR,
 /*
  * trying to add to the hash table key that already present in
  * the hash table. key can be present at most once in the hash table
  */
  SOC_SAND_HASH_TABLE_KEY_ALREADY_EXIST_ERR,
 /*
  * trying to update a key that is not exist in the hash table
  * should use Add.
  */
  SOC_SAND_HASH_TABLE_KEY_IS_NOT_EXIST_ERR,

 /*
  * Group Member Linked List
  */
 /*
  * the number of elements (members) is out of range.
  * Range is 1-
  */
  SOC_SAND_GROUP_MEM_LL_NOF_MEMBERS_OUT_OF_RANGE_ERR,
 /*
  * the number of groups is out of range.
  * Range is 1-
  */
  SOC_SAND_GROUP_MEM_LL_NOF_GROUPS_OUT_OF_RANGE_ERR,
 /*
  * the member (element) id is out of range.
  * Range according to nof_elements given in create
  */
  SOC_SAND_GROUP_MEM_LL_MEMBER_ID_OUT_OF_RANGE_ERR,
 /*
  * the group id is out of range.
  * Range according to nof_groups given in create
  */
  SOC_SAND_GROUP_MEM_LL_GROUP_ID_OUT_OF_RANGE_ERR,
 /*
  * try to add element as member to group when it's already
  * a member in a group and auto_remove is FALSE
  */
  SOC_SAND_GROUP_MEM_LL_ILLEGAL_ADD_ERR,
/*
 * SOC_SAND memory management
 */
 /*
  * the number of lines (memory) is out of range.
  * Range is 1-
  */
  SOC_SAND_ARR_MEM_ALLOCATOR_NOF_LINES_OUT_OF_RANGE_ERR,
 /*
  * malloc size is out of range.
  * malloc can be in size of 2-size of create memory
  */
  SOC_SAND_ARR_MEM_ALLOCATOR_MALLOC_SIZE_OUT_OF_RANGE_ERR,
 /*
  * free pointer is out of range.
  * free pointer range is 0-size of create memory
  */
  SOC_SAND_ARR_MEM_ALLOCATOR_POINTER_OF_RANGE_ERR,


 /*
  * SOC_SAND PP IPV4
  */
 /*
  * the ipv4 prefix len is out of range
  * ipv4 prefix length range 0-32
  */
  SOC_SAND_PP_IPV4_SUBNET_PREF_OUT_OF_RANGE_ERR,

  /*
  * SOC_SAND PP IPV4
  */
  /*
  * the ipv6 address is not ipv6 multicast address (prefix is not 0xff)
  */
  SOC_SAND_PP_IPV6_ADDRESS_IS_NOT_MCAST_ERR,

 /*
  * SOC_SAND PP MAC
  */
 /*
  * the mac address string is not valid,
  * one of the string chars is out of hex values.
  * range of each char is 0-f in hexadecimal.
  */
  SOC_SAND_PP_MAC_ADDRESS_ILLEGAL_STRING_ERR,

 /*
  * SOC_SAND PP Out of range
  */
  SOC_SAND_PP_IPV4_ADDRESS_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_VLAN_ID_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_TC_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_DP_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_DEI_CFI_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_PCP_UP_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_ETHER_TYPE_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_TPID_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_IP_TTL_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_IPV4_TOS_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_IPV6_TC_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_MPLS_LABEL_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_MPLS_EXP_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_ISID_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_MPLS_DSCP_EXP_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_DSCP_EXP_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_IN_RANGE_OUT_OF_RANGE_ERR,
  SOC_SAND_PP_DSCP_OUT_OF_RANGE_ERR,
  
 /*
  * SORTED list
  */
 /*
  * trying to add to the sorted list key that already present in
  * the sorted list. key can be present at most once in the sorted list
  */
  SOC_SAND_SORTED_LIST_KEY_DATA_ALREADY_EXIST_ERR,
 /*
  * trying to add/remove entry using illegal iterator position
  */
  SOC_SAND_SORTED_LIST_ILLEGAL_ITER_ERR,
  /* 
   * Cannot get error text for error id
   */
  SOC_SAND_GET_ERR_TXT_ERR,
  /*
   *
   */
  SOC_SAND_PRDS_START_ERR_NUMBER   = 200,
  SOC_SAND_PRDS_INT_WITHIN_INT,
  /*
   *
   */
#ifdef SOC_SAND_ERROR_CODE_OLD_MODE

  SOC_SAND_FE200_START_ERR_NUMBER = 2000,
  SOC_SAND_FE200_INT_WITHIN_INT,

  SOC_SAND_FAP10M_START_ERR_NUMBER = 4000,
  SOC_SAND_FAP10M_INT_WITHIN_INT,

  SOC_SAND_FAP20V_START_ERR_NUMBER = 12000,
  SOC_SAND_FAP20V_INT_WITHIN_INT,
#else  /*#ifdef SOC_SAND_ERROR_CODE_OLD_MODE*/
   SOC_SAND_FE200_START_ERR_NUMBER = 200,
   SOC_SAND_FE200_INT_WITHIN_INT,

   SOC_SAND_FAP10M_START_ERR_NUMBER = 450,
   SOC_SAND_FAP10M_INT_WITHIN_INT,

   SOC_SAND_TIMNA_START_ERR_NUMBER = 450,
   SOC_SAND_TIMNA_INT_WITHIN_INT,

   SOC_SAND_FAP20V_START_ERR_NUMBER = 550, /* 200 */
   SOC_SAND_FAP20V_INT_WITHIN_INT,

   SOC_SAND_PCP_START_ERR_NUMBER    = 750, /* 540 */
   SOC_SAND_PCP_INT_WITHIN_INT,
    
#endif /*#ifdef SOC_SAND_ERROR_CODE_OLD_MODE*/

  SOC_SAND_FAP21V_START_ERR_NUMBER    = 750, /* 2 */
  SOC_SAND_FAP21V_INT_WITHIN_INT,

  SOC_SAND_FAP20M_START_ERR_NUMBER    = 752, /* 2 */
  SOC_SAND_FAP20M_INT_WITHIN_INT,

  SOC_SAND_ARAD_START_ERR_NUMBER     = 754, /* 950 - some place */
  SOC_SAND_ARAD_INT_WITHIN_INT,

  SOC_SAND_PETRA_START_ERR_NUMBER     = 1704, /* 600 */
  SOC_SAND_PETRA_INT_WITHIN_INT,

  SOC_SAND_PETRA_PP_START_ERR_NUMBER  = 2304, /* 2 */
  SOC_SAND_PETRA_PP_INT_WITHIN_INT,

  SOC_SAND_FE600_START_ERR_NUMBER     = 2306, /* 2 */
  SOC_SAND_FE600_INT_WITHIN_INT,

  SOC_SAND_T20E_START_ERR_NUMBER      = 2308, /* 2 */
  SOC_SAND_T20E_INT_WITHIN_INT,

  SOC_SAND_PB_START_ERR_NUMBER        = 2310, /* 500 */
  SOC_SAND_PB_INT_WITHIN_INT,

  SOC_SAND_PB_PP_START_ERR_NUMBER     = 2810, /* 1170 */
  SOC_SAND_PB_PP_INT_WITHIN_INT,

  SOC_SAND_PPD_START_ERR_NUMBER       = 3980, /* 10 */
  SOC_SAND_PPD_INT_WITHIN_INT,

  SOC_SAND_OAM_START_ERR_NUMBER       = 3990, /* 50 */
  SOC_SAND_OAM_INT_WITHIN_INT,

  SOC_SAND_TMD_START_ERR_NUMBER       = 4040, /* 10 */
  SOC_SAND_TMD_INT_WITHIN_INT,

  SOC_SAND_TMC_START_ERR_NUMBER       = 4050, /* 10 */
  SOC_SAND_TMC_INT_WITHIN_INT,

	SOC_SAND_ARAD_PP_START_ERR_NUMBER     = 4060, /* 1170 */
	SOC_SAND_ARAD_PP_INT_WITHIN_INT,


  /*
   * Indication of end of list of errors. Do not remove.
   */
  SOC_SAND_END_ERR_LIST = -1
} SOC_SAND_ERROR_CODE ;
/*
 * {
 * Error word construction utilities
 */
void
  soc_sand_initialize_error_word(
    uint32   proc_id,
    unsigned char   callback,
    uint32   *error_word) ;
void
  soc_sand_set_proc_id_into_error_word(uint32   proc_id,
    uint32   *error_word) ;
/*****************************************************
*NAME
*  soc_sand_get_proc_id_from_error_word
*TYPE:
*  PROC
*DATE:
*  25/SEP/2002
*FUNCTION:
*  Extract procedure id from the standard output error
*  word.
*INPUT:
*  SOC_SAND_DIRECT:
*    uint32 error_word -
*      Error code constructed as described under
*      ERROR RETURN VALUES (in soc_sand_error_code.h).
*  SOC_SAND_INDIRECT:
*    None.
*OUTPUT:
*  SOC_SAND_DIRECT:
*    unsigned short -
*      Procedure id.
*      See formatting rules in ERROR RETURN VALUES in
*      soc_sand_error_code.h.
*  SOC_SAND_INDIRECT:
*    formatting rules of ERROR RETURN VALUES
*REMARKS:
*  None
*SEE ALSO:
*****************************************************/
uint32
    soc_sand_get_proc_id_from_error_word(
      uint32   error_word
    ) ;
void
  soc_sand_set_error_code_into_error_word(
    unsigned short  error_code,
    uint32   *error_word) ;

uint32
  soc_sand_update_error_code(
    uint32   fun_error_word,
    uint32   *error_word
  );

/*****************************************************
*NAME
*  soc_sand_get_error_code_from_error_word
*TYPE:
*  PROC
*DATE:
*  25/SEP/2002
*FUNCTION:
*  Extract error code from the standard output error
*  word.
*INPUT:
*  SOC_SAND_DIRECT:
*    uint32 error_word -
*      Error code constructed as described under
*      ERROR RETURN VALUES (in soc_sand_error_code.h).
*  SOC_SAND_INDIRECT:
*    None.
*OUTPUT:
*  SOC_SAND_DIRECT:
*    unsigned short -
*      error code.
*      See formatting rules in ERROR RETURN VALUES in
*      soc_sand_error_code.h.
*  SOC_SAND_INDIRECT:
*    formatting rules of ERROR RETURN VALUES
*REMARKS:
*  None
*SEE ALSO:
*****************************************************/
unsigned
  short
    soc_sand_get_error_code_from_error_word(
      uint32 error_word) ;
void
  soc_sand_set_severity_into_error_word(
    unsigned char severity,
    uint32 *error_word) ;
unsigned
  char
    soc_sand_get_severity_from_error_word(
      uint32 error_word) ;
void
  soc_sand_set_callback_into_error_word(
    unsigned char callback,
    uint32 *error_word) ;
unsigned
  char soc_sand_get_callback_from_error_word(
    uint32 error_word) ;
uint32
    soc_sand_build_error_code(
      unsigned short error_code,
      uint32  proc_id,
      unsigned char  severity,
      unsigned char  is_call_back) ;
int
  soc_sand_error_code_to_string(
    SOC_SAND_IN  uint32 in_error_code,
    SOC_SAND_OUT char    **out_error_name,
    SOC_SAND_OUT char    **out_error_description
  ) ;
int
  soc_sand_linear_find_error(
    uint32      error_code,
    SOC_SAND_IN SOC_ERROR_DESC_ELEMENT *in_error_desc_element,
    CONST SOC_ERROR_DESC_ELEMENT **error_desc_element_ptr
  ) ;
/*
 * }
 * Error word construction utilities
 */
/*
 * Handling of all-system errors list plus descriptors.
 * {
 */
int
  soc_sand_add_error_pool(
    SOC_SAND_IN SOC_ERROR_DESC_ELEMENT  *error_pool
  ) ;
int
  soc_sand_add_error_pools(
    SOC_SAND_IN SOC_ERROR_DESC_ELEMENT  **error_id_pools,
    uint32 nof_pools
  ) ;
int
  soc_sand_close_all_error_pool(
    void
  ) ;
int
  soc_sand_add_sand_errors(
    void
  ) ;
CONST SOC_ERROR_DESC_ELEMENT
  *soc_sand_get_errors_ptr(
    void
  ) ;
/*
 * }
 */
/*
 * Handling of all-system procedures list plus descriptors.
 * {
 */
int
  soc_sand_add_proc_id_pool(
    SOC_SAND_IN SOC_PROCEDURE_DESC_ELEMENT  *proc_id_pool
  ) ;
int
  soc_sand_add_proc_id_pools(
    SOC_SAND_IN SOC_PROCEDURE_DESC_ELEMENT  **proc_id_pool,
    uint32 nof_pools
  ) ;
int
  soc_sand_add_sand_procedures(
    void
  ) ;
int
  soc_sand_close_all_proc_id_pool(
    void
  ) ;
int
  soc_sand_proc_id_to_string(
    SOC_SAND_IN  uint32 in_proc_id,
    SOC_SAND_OUT char          **out_module_name,
    SOC_SAND_OUT char    **out_proc_name
  ) ;
CONST SOC_PROCEDURE_DESC_ELEMENT
  *soc_sand_get_procedures_ptr(
    void
  ) ;
int
  soc_sand_linear_find_procedure(
    uint32          proc_id,
    SOC_SAND_IN SOC_PROCEDURE_DESC_ELEMENT *in_procedure_desc_element,
    CONST SOC_PROCEDURE_DESC_ELEMENT **procedure_desc_element_ptr
  ) ;
/*
 * }
 */
SOC_SAND_RET
  soc_sand_init_errors_queue(
    void
  ) ;
SOC_SAND_RET
  soc_sand_delete_errors_queue(
    void
  ) ;
SOC_SAND_RET
  soc_sand_load_errors_queue(
    SOC_SAND_ERRORS_QUEUE_MESSAGE *errors_queue_message
  ) ;
SOC_SAND_RET
  soc_sand_unload_errors_queue(
    void
  ) ;
int
  soc_sand_is_errors_queue_msg(
    uint32 msg
  ) ;
void
  soc_sand_error_code_handler(
    SOC_SAND_IN SOC_SAND_RET error_code,
    SOC_SAND_IN char*    error_file,
    SOC_SAND_IN int      error_line,
    SOC_SAND_IN char*    error_func_name,
    SOC_SAND_IN char*    error_msg
  ) ;
/*
 * If ACTIVATE_DBG_ERROR_REPORT is asserted then built-in compiler-dependent
 * error report is activated at exit of SOC_SAND procedures. This is a debugging
 * tool and should be shut down on operational code.
 */
#define ACTIVATE_DBG_ERROR_REPORT 0

#if ACTIVATE_DBG_ERROR_REPORT
/* { */
#ifdef __GNUC__
/* { */
#define SOC_SAND_DBG_REPORT(soc_sand_error_code, error_msg) \
          soc_sand_error_code_handler(  \
            soc_sand_error_code,        \
            __FILE__,               \
            __LINE__,               \
            __PRETTY_FUNCTION__ ,   \
            error_msg               \
          )
/* } */
#else
/* { */
#define SOC_SAND_DBG_REPORT(soc_sand_error_code, error_msg) \
          soc_sand_error_code_handler(  \
            soc_sand_error_code,        \
            __FILE__,               \
            __LINE__,               \
            NULL,                   \
            error_msg               \
          )
/* } */
#endif
/* } */
#else
/* { */
#define SOC_SAND_DBG_REPORT(soc_sand_error_code, error_msg)
/* } */
#endif

#ifdef BROADCOM_DEBUG

/* If expr is false, then an error will be thrown and the error message will be the expression failed. */
/* This is a version of assert without exit. */
# define SOC_SAND_VERIFY(expr) { if (!(expr)) { SOC_SAND_SET_ERROR_MSG((_ERR_MSG_MODULE_NAME, unit, "ASSERTION FAILED (%s:%d): %s\n", __FILE__, __LINE__, #expr)); } }
#else
# define SOC_SAND_VERIFY(expr) ((void)0)
#endif /* BROADCOM_DEBUG */

/*
 * Macro to activate at exit of all SOC_SAND procedures.
 * Note that each procedure reports itself but may report an error
 * brought in from other (invoked) procedure.
 * A few procedures may report the same error (but each with
 * its procedure id).
 */
#define SOC_SAND_ERROR_REPORT(error_code,error_msg,callback,svr,proc_id,err_text,p1,p2,p3,p4,p5,p6) \
  if (error_code != SOC_SAND_OK)  \
  {               \
    uint32      \
        err_id ;  \
    soc_sand_initialize_error_word(proc_id,callback,&err_id) ;     \
    soc_sand_set_error_code_into_error_word(error_code,&err_id) ;  \
    soc_sand_set_severity_into_error_word(svr,&err_id) ;           \
    soc_sand_error_handler(err_id,err_text,p1,p2,p3,p4,p5,p6) ;    \
    SOC_SAND_DBG_REPORT(error_code,error_msg) ;               \
  }

/*****************************************************
 * See details in soc_sand_error_code.c
 *****************************************************/
SOC_SAND_RET
  soc_sand_check_CREDIT_WORTH(
    SOC_SAND_IN  SOC_SAND_CREDIT_SIZE credit_worth
  );



#if SOC_SAND_DEBUG
/* { */
/*
 */

/*****************************************************
 * See details in soc_sand_error_code.c
 *****************************************************/
int
  soc_sand_disp_result(
    SOC_SAND_IN uint32 driver_api_result
  );
int
  soc_sand_disp_result_proc(
    SOC_SAND_IN uint32 driver_api_result,
    SOC_SAND_IN char              *proc_name
  );

/*****************************************************
 * See details in soc_sand_error_code.c
 *****************************************************/
void
  soc_sand_no_error_printing_set(
    uint8   no_error_printing
  );

uint8
  soc_sand_no_error_printing_get(
       void
    );


/*
 * }
 */
#endif


#ifdef  __cplusplus
}
#endif
/* } __SOC_SAND_ERROR_CODE_H_INCLUDED__*/
#endif
