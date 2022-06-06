
/* $Id: arad_reg_access.h,v 1.12 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/


#ifndef __ARAD_REG_ACCESS_H_INCLUDED__
/* { */
#define __ARAD_REG_ACCESS_H_INCLUDED__

/*************
 * INCLUDES  *
 *************/
/* { */
#include <soc/dpp/SAND/Utils/sand_header.h>
#include <soc/dpp/SAND/Utils/sand_framework.h>


#include <soc/dpp/ARAD/arad_chip_regs.h>


/* } */

/*************
 * DEFINES   *
 *************/
/* { */

#define ARAD_MAX_REGS_SEQ        10

/* } */

/*************
 *  MACROS   *
 *************/
/* { */

/* Set the value of a register's field in memory (not in the actual register) */
#define ARAD_FLD_TO_REG(reg_desc, fld_desc, fld_value, reg_value, err_num, exit_place) \
{\
  soc_reg_field_set(unit, reg_desc, &reg_value, fld_desc, fld_value); \
}

/* get the value of a register's field from memory (not in the actual register) */
#define ARAD_FLD_FROM_REG(reg_desc, fld_desc, fld_value, reg_value, err_num, exit_place) \
{\
      fld_value = soc_reg_field_get(unit, reg_desc, reg_value, fld_desc); \
}

#define ARAD_FLD_TO_REG64(reg_desc, fld_desc, fld_value, reg_value, err_num, exit_place) \
{\
  soc_reg64_field_set(unit, reg_desc, &reg_value,     fld_desc, fld_value); \
}

#define ARAD_FLD_FROM_REG64(reg_desc, fld_desc, fld_value, reg_value, err_num, exit_place) \
{\
  fld_value = soc_reg64_field_get(unit, reg_desc, reg_value, fld_desc); \
}

#define ARAD_FLD_TO_REG_ABOVE_64(reg_desc, fld_desc, fld_value, reg_value, err_num, exit_place) \
{\
  soc_reg_above_64_field_set(unit, reg_desc, reg_value, fld_desc, fld_value); \
}

#define ARAD_FLD_FROM_REG_ABOVE_64(reg_desc, fld_desc, fld_value, reg_value, err_num, exit_place) \
{\
      soc_reg_above_64_field_get(unit, reg_desc, reg_value, fld_desc, fld_value); \
}


/*            set new_string [string map {ARAD_FLD_GET ARAD_FIELD_GET ARAD_FLD_SET ARAD_FIELD_SET PB_FLD_GET ARAD_FLD_GET PB_FLD_SET ARAD_FLD_SET ARAD_REG_SET ARAD_REG_SET ARAD_REG_GET ARAD_REG_GET ARAD_FLD_TO_REG ARAD_FLD_TO_REG ARAD_FLD_FROM_REG ARAD_FLD_FROM_REG} $line] */

/*
 * Soc_sand field manipulations based on arad fields
 * (must contain valid msb and lsb) {
 */
#define ARAD_FLD_MSB(fld)                                                    \
           ((fld).bp + (fld).len - 1)
#define ARAD_FLD_NOF_BITS(fld)                                               \
          (fld).len
#define ARAD_FLD_LSB(fld)                                                    \
          (fld).bp
#define ARAD_FLD_MASK(fld)                                                   \
          (uint32)((SOC_SAND_BIT(ARAD_FLD_MSB(fld)) - SOC_SAND_BIT(ARAD_FLD_LSB(fld))) + SOC_SAND_BIT(ARAD_FLD_MSB(fld)))
#define ARAD_FLD_RMASK(fld)                                                   \
          (uint32)(~ARAD_FLD_MASK(fld))
#define ARAD_FLD_SHIFT(fld)                                                  \
          (fld).bp
#define ARAD_FLD_MAX(fld)                                                    \
          (SOC_SAND_BITS_MASK(ARAD_FLD_NOF_BITS(fld) - 1, 0))

#define ARAD_FLD_MSB_OLD(fld)                                                    \
           (fld).msb
#define ARAD_FLD_NOF_BITS_OLD(fld)                                               \
          ((fld).msb - (fld).lsb + 1)
#define ARAD_FLD_LSB_OLD(fld)                                                    \
          (fld).lsb
#define ARAD_FLD_MASK_OLD(fld)                                                   \
          (uint32)((SOC_SAND_BIT((fld).msb) - SOC_SAND_BIT((fld).lsb)) + SOC_SAND_BIT((fld).msb))
#define ARAD_FLD_RMASK_OLD(fld)                                                   \
          (uint32)(~ARAD_FLD_MASK(fld))
#define ARAD_FLD_SHIFT_OLD(fld)                                                  \
          (fld).lsb
#define ARAD_FLD_MAX_OLD(fld)                                                    \
          (SOC_SAND_BITS_MASK((fld).msb - (fld).lsb, 0))


/*
 * Take value and put it in its proper location within a 'long'
 * register (other bits are zeroed).
 */
#define ARAD_FLD_IN_PLACE(val,fld) (uint32)(((uint32)(val) << ((uint32)ARAD_FLD_SHIFT(fld))) & (uint32)(ARAD_FLD_MASK(fld)))
#define ARAD_FLD_IN_PLACE_OLD(val,fld) (uint32)(((uint32)(val) << ((uint32)ARAD_FLD_SHIFT_OLD(fld))) & (uint32)(ARAD_FLD_MASK_OLD(fld)))
/*
 * Get a value out of location within a 'long' register (and make sure it
 * is not effected by bits outside its predefined mask).
 */
#define ARAD_FLD_FROM_PLACE(val,fld) (((uint32)(val) & (ARAD_FLD_MASK(fld))) >> (ARAD_FLD_SHIFT(fld)))
#define ARAD_FLD_FROM_PLACE_OLD(val,fld) (((uint32)(val) & (ARAD_FLD_MASK_OLD(fld))) >> (ARAD_FLD_SHIFT_OLD(fld)))

/*
 * Take value from buff, and split it to two buffers (used when a field is split in HW
 */
#define ARAD_FIELD_VAL_TO_SPLIT_FLDS_SET(buff, fld_lsbs, fld_msbs, fld_buff_lsbs, fld_buff_msbs) \
  do { \
  *fld_buff_lsbs = SOC_SAND_GET_BITS_RANGE(*buff, ARAD_FLD_NOF_BITS(*fld_lsbs)-1, 0); \
  *fld_buff_msbs = SOC_SAND_GET_BITS_RANGE(*buff, ARAD_FLD_NOF_BITS(*fld_msbs)-1, ARAD_FLD_NOF_BITS(*fld_lsbs)); \
  } while (0)

/*
 * Build value from two fields (used when a field is split in HW
 */
#define ARAD_FIELD_VAL_TO_SPLIT_FLDS_GET(fld_buff_lsbs, fld_buff_msbs, fld_lsbs, fld_msbs, buff) \
  *buff = \
    *fld_buff_lsbs | \
    SOC_SAND_SET_BITS_RANGE( \
    *fld_buff_msbs, \
    (ARAD_FLD_NOF_BITS(*fld_lsbs) + ARAD_FLD_NOF_BITS(*fld_msbs)-1), \
    ARAD_FLD_NOF_BITS(*fld_lsbs))

/*
 * Soc_sand field manipulations }
 */

/*
 * Within an array of registers with identical fields-
 * get the index of the register to access.
 */
#define ARAD_REG_IDX_GET(fld_global_idx,nof_flds_in_reg) \
  ((fld_global_idx) / (nof_flds_in_reg))

/*
 * Within an array of registers with identical fields-
 * get the index of the field (inside the register) to access.
 */
#define ARAD_FLD_IDX_GET(fld_global_idx,nof_flds_in_reg) \
  ((fld_global_idx) % (nof_flds_in_reg))

/*
 * Set a variable of type ARAD_REG_FIELD
 */
#define ARAD_FLD_DEFINE(fld_var,fld_offset,fld_size) \
{                                                     \
  fld_var.lsb = fld_offset;                           \
  fld_var.msb = (fld_offset) + (fld_size) - 1;            \
}

/* 
 * Get the attributes of a field
 */
#define ARAD_TBL_REF(unit, tbl_ptr, field, field_dst)       \
{                                                     \
    soc_mem_info_t *meminfo;                            \
    soc_field_info_t    *fieldinfo;                     \
                                                      \
        meminfo = &SOC_MEM_INFO(unit, tbl_ptr);           \
        SOC_FIND_FIELD(field,                             \
                       meminfo->fields,                         \
                       meminfo->nFields,                        \
                       fieldinfo);                              \
                                                      \
        if (NULL != fieldinfo) {                          \
            ARAD_COPY(field_dst, fieldinfo, soc_field_info_t, 1); \
                    }                                           \
        else {\
            SOC_SAND_SET_ERROR_CODE(SOC_SAND_GEN_ERR, 1001, exit);\
        }\
}

/*
#define ARAD_TBL_REF(unit, tbl_ptr, field, field_dst)       \
{                                                     \
    soc_mem_info_t *meminfo;                            \
    soc_field_info_t    *fieldinfo;                     \
                                                      \
    if (SOC_MEM_IS_VALID(unit, tbl_ptr)) {              \
                                                      \
        meminfo = &SOC_MEM_INFO(unit, tbl_ptr);           \
        SOC_FIND_FIELD(field,                             \
                       meminfo->fields,                         \
                       meminfo->nFields,                        \
                       fieldinfo);                              \
                                                      \
        if (NULL != fieldinfo) {                          \
            ARAD_COPY(field_dst, fieldinfo, soc_field_info_t, 1); \
                    }                                           \
        else {\
            SOC_SAND_SET_ERROR_CODE(SOC_SAND_GEN_ERR, 1001, exit);\
        }\
    }                                                    \
    else {\
        SOC_SAND_SET_ERROR_CODE(SOC_SAND_GEN_ERR, 1001, exit);\
    }\
}
*/

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

/* } */

/*************
 * FUNCTIONS *
 *************/
/* { */


#include <soc/dpp/SAND/Utils/sand_footer.h>


/* } __ARAD_REG_ACCESS_H_INCLUDED__*/
#endif
