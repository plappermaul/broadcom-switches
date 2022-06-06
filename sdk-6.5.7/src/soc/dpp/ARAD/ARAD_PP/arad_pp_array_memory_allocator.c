/*
 * $Id: arad_pp_array_memory_allocator.c,v 1.19 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */

#ifdef _ERR_MSG_MODULE_NAME
  #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_SOC_MEM

/*************
 * INCLUDES  *
 *************/
/* { */
#include <shared/bsl.h>
#include <soc/dcmn/error.h>
#include <soc/dpp/SAND/Utils/sand_header.h>

#include <soc/dpp/ARAD/ARAD_PP/arad_pp_array_memory_allocator.h>
#include <soc/dpp/SAND/Management/sand_general_macros.h>
#include <soc/dpp/SAND/Management/sand_error_code.h>
#include <soc/dpp/SAND/Utils/sand_os_interface.h>
#include <soc/dpp/drv.h>
#include <soc/dpp/dpp_wb_engine.h>

/* } */

/*************
* DEFINES   *
*************/
/* { */
/*#define DEFRAG_DEBUG 0*/
#define ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END      0x1FFFFFFF

#define ARAD_PP_ARR_MEM_ALLOCATOR_FREE_ENTRY_LEN      3

/* org */
#define ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE_LSB      0 
#define ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE_MSB      19 

/* alignment of used entry
   0: alignment is 2.
   1: alignment is 4.
*/
#define ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_ALIGN_MSB      30 
#define ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_ALIGN_LSB      30 

#define ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_USE_LSB      31
#define ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_USE_MSB      31
/* size type, if 0 then this is block of size 2, otherwise > 2 */
#define ARAD_PP_ARR_MEM_ALLOCATOR_FREE_ENTRY_TYPE_LSB      0

/* information in extended entry, can be size, next, prev */
#define ARAD_PP_ARR_MEM_ALLOCATOR_EXT_ENTRY_INFO_LSB      1
#define ARAD_PP_ARR_MEM_ALLOCATOR_EXT_ENTRY_INFO_MSB      29


/* flags control copying a block of memory */
#define ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_COPY           (0x1)  /* copy into link list */
#define ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_FREE           (0x2)  /* remove old copy from link list */
#define ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_UPDATE_CONT    (0x4)  /* update data content */
#define ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_UPDATE_REF     (0x8)  /* update entry point to moved entry */
#define ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_BLK_SIZE_VLD   (0x10) /* block size is given */


/*************
*  MACROS   *
*************/
/* { */
#define ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_IS_USED(entry)   \
            (SOC_SAND_GET_BITS_RANGE(entry,ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_USE_MSB,ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_USE_LSB))


/* is small free entry, assuming it's free entry */
#define ARAD_PP_ARR_MEM_ALLOCATOR_FREE_SMALL(entry)   \
(SOC_SAND_GET_BIT(entry,ARAD_PP_ARR_MEM_ALLOCATOR_FREE_ENTRY_TYPE_LSB))

/* get 1-29 from extended entry, assuming it's extended entry */
#define ARAD_PP_ARR_MEM_ALLOCATOR_EXT_INFO(entry)   \
  (SOC_SAND_GET_BITS_RANGE(entry,ARAD_PP_ARR_MEM_ALLOCATOR_EXT_ENTRY_INFO_MSB,ARAD_PP_ARR_MEM_ALLOCATOR_EXT_ENTRY_INFO_LSB))

/* get size of free entry.assuming it's free entry */
#define ARAD_PP_ARR_MEM_ALLOCATOR_FREE_ENTRY_SIZE(entry)  \
  (ARAD_PP_ARR_MEM_ALLOCATOR_FREE_SMALL(entry)?2:(ARAD_PP_ARR_MEM_ALLOCATOR_EXT_INFO(entry)))

/* get size of used entry,assuming it's used entry */
#define ARAD_PP_ARR_MEM_ALLOCATOR_USED_ENTRY_SIZE(entry)  \
  (SOC_SAND_GET_BITS_RANGE(entry,ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE_MSB,ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE_LSB))

/* get alignment of entry assume it used  */
#define ARAD_PP_ARR_MEM_ALLOCATOR_USED_ENTRY_ALIGN_GET(entry)  \
  (entry & (1 << ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_ALIGN_MSB))?4:2;

/* get size of entry */
/*org*/
#define ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(entry)   \
  ((ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_IS_USED(entry))?     \
  (ARAD_PP_ARR_MEM_ALLOCATOR_USED_ENTRY_SIZE(entry)):    \
  (ARAD_PP_ARR_MEM_ALLOCATOR_FREE_ENTRY_SIZE(entry)))

/*org*/
/* get previous pointer assuming this is prev entry in free block*/
#define ARAD_PP_ARR_MEM_ALLOCATOR_FREE_PREV_PTR(entry)   \
            ARAD_PP_ARR_MEM_ALLOCATOR_EXT_INFO(entry)

/*org*/
/* get next pointer assuming this is next entry in free block*/
#define ARAD_PP_ARR_MEM_ALLOCATOR_FREE_NEXT_PTR(entry)   \
  ARAD_PP_ARR_MEM_ALLOCATOR_EXT_INFO(entry)

/*
 * MACRO given address get the next aligned address
 */
#define ARAD_PP_ARR_MEM_ALLOCATOR_GET_ALIGNED(addr,align)  \
        (addr == 0)? 0:((((addr) - 1)/(align) + 1) * (align))

#define ARAD_PP_ARR_MEM_ALLOCATOR_CACHE_INST(_inst) (SOC_SAND_BIT(31)|(_inst))

#define ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(_arr_mem_info, _inst) \
      (_arr_mem_info->arr_mem_allocator_data.cache_enabled)?ARAD_PP_ARR_MEM_ALLOCATOR_CACHE_INST(_inst):_inst
      
/* return actual size of memory, reduce size needed for defragment */
#define ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(_arr_mem_info)   (((_arr_mem_info)->support_defragment)?((_arr_mem_info)->nof_entries - 1 - (_arr_mem_info)->max_block_size):((_arr_mem_info)->nof_entries-1))

/* } */


/*************
* TYPE DEFS *
*************/
/* { */
typedef struct
{
 /*
  * array to include entries
  */
  uint8 use;
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR indx1;
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR indx2;
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR prev;
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR next;
  uint32 size;
} ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_INFO;


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

/************************************************************************/
/* internal functions                                                   */
/************************************************************************/

STATIC uint32
  arad_pp_arr_mem_allocator_get_next_free(
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
    SOC_SAND_IN  uint8                down,
    SOC_SAND_INOUT  ARAD_PP_ARR_MEM_ALLOCATOR_PTR   *free_ptr
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    tmp_ptr;
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    entry;
  uint32
    block_size,
    next_ptr;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);

  tmp_ptr = *free_ptr;


  /* read current free */
  res = arr_mem_info->entry_get_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          tmp_ptr,
          &entry
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  /* if prev, then get it from footer */
  if (!down)
  {
    block_size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(entry);
    next_ptr = tmp_ptr + block_size - 1;

    /* read footer */
    res = arr_mem_info->entry_get_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            next_ptr,
            &entry
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
    *free_ptr = ARAD_PP_ARR_MEM_ALLOCATOR_FREE_PREV_PTR(entry);
    goto exit;
  }

  
  /* if next, get from first entry or next entry, depends on entry type */
  if (ARAD_PP_ARR_MEM_ALLOCATOR_FREE_SMALL(entry))
  {
    *free_ptr = ARAD_PP_ARR_MEM_ALLOCATOR_FREE_NEXT_PTR(entry);
    goto exit;
  }
  /* get from next entry */
  /* read next free-entry in the block */
  res = arr_mem_info->entry_get_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          tmp_ptr+1,
          &entry
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
  
  *free_ptr = ARAD_PP_ARR_MEM_ALLOCATOR_FREE_NEXT_PTR(entry);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_get_next_free()",0,0);
}

STATIC uint32
  arad_pp_arr_mem_allocator_header_get(
    SOC_SAND_INOUT  ARAD_PP_ARR_MEM_ALLOCATOR_INFO        *arr_mem_info,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR            footer_ptr,
    SOC_SAND_OUT  ARAD_PP_ARR_MEM_ALLOCATOR_PTR           *header_ptr
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    prev;
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    entry;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);
  SOC_SAND_CHECK_NULL_INPUT(header_ptr);

  res = arr_mem_info->entry_get_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          footer_ptr,
          &entry
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  prev = ARAD_PP_ARR_MEM_ALLOCATOR_FREE_PREV_PTR(entry);

  if (prev == ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END)
  {
    res = arr_mem_info->free_get_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            header_ptr
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
  }
  else
  {
   res = arad_pp_arr_mem_allocator_get_next_free(
            arr_mem_info,
            TRUE,
            &prev
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    *header_ptr = prev;
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_header_get()",0,0);
}


STATIC uint32
  arad_pp_arr_mem_allocator_set_next_free(
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR      block_ptr_in,
    SOC_SAND_IN  uint8                       down,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR      free_ptr
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    tmp_ptr;
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR      
    block_ptr=block_ptr_in;
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    entry;
  uint32
    block_size,
    next_ptr;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);

  tmp_ptr = free_ptr;

  /* next of head */
  if (block_ptr == ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END && down)
  {
    res = arr_mem_info->free_set_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            free_ptr
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    goto exit;
  }
  /* next of end, nothing to do*/
  else if (block_ptr == ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END)
  {
	  goto exit;
  }

  /* get current entry, to set next/prev pointer for */
  res = arr_mem_info->entry_get_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          block_ptr,
          &entry
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  block_size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(entry);

  if (down)
  {
    /* if this is not small, block, then next, set in next entry, read it */
    if (!ARAD_PP_ARR_MEM_ALLOCATOR_FREE_SMALL(entry))
    {
      ++block_ptr;
      res = arr_mem_info->entry_get_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            block_ptr,
            &entry
          );
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    }
    /* set next pointer, in header [+1] */
    res = soc_sand_set_field(
            &entry,
            ARAD_PP_ARR_MEM_ALLOCATOR_EXT_ENTRY_INFO_MSB,
            ARAD_PP_ARR_MEM_ALLOCATOR_EXT_ENTRY_INFO_LSB,
            tmp_ptr
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

    /* update content in header [+1] */
    res = arr_mem_info->entry_set_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            block_ptr,
            &entry
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

    goto exit;
  }
  /* if to set prev, update in footer*/
  next_ptr = block_ptr + block_size - 1;

  /* read footer */
  res = arr_mem_info->entry_get_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          next_ptr,
          &entry
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  
  /* update pointer */
  res = soc_sand_set_field(
          &entry,
          ARAD_PP_ARR_MEM_ALLOCATOR_EXT_ENTRY_INFO_MSB,
          ARAD_PP_ARR_MEM_ALLOCATOR_EXT_ENTRY_INFO_LSB,
          tmp_ptr
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  /* update footer content */
  res = arr_mem_info->entry_set_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          next_ptr,
          &entry
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_set_next_free()",0,0);
}


STATIC uint32
  arad_pp_arr_mem_allocator_remove_free_entry(
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
    SOC_SAND_INOUT  ARAD_PP_ARR_MEM_ALLOCATOR_PTR   free_ptr
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    next,
    prev;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);

  next = free_ptr;
  res = arad_pp_arr_mem_allocator_get_next_free(
          arr_mem_info,
          TRUE,
          &next
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  prev = free_ptr;
  res = arad_pp_arr_mem_allocator_get_next_free(
          arr_mem_info,
          FALSE,
          &prev
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

  res = arad_pp_arr_mem_allocator_set_next_free(
          arr_mem_info,
          next,
          FALSE,
          prev
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

  res = arad_pp_arr_mem_allocator_set_next_free(
          arr_mem_info,
          prev,
          TRUE,
          next
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);


exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_remove_free_entry()",0,0);
}

STATIC uint32
  arad_pp_arr_mem_allocator_upadte_list(
    SOC_SAND_INOUT  ARAD_PP_ARR_MEM_ALLOCATOR_INFO        *arr_mem_info,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR            prev,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR            curr,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR            next
  )
{
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);
 /*
  * update next to point to me
  */

  if (next != ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END)
  {
    res = arad_pp_arr_mem_allocator_set_next_free(
            arr_mem_info,
            next,
            FALSE,
            curr
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
  }
 /*
  * update prev to point to me
  */
  if (prev != ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END)
  {
    res = arad_pp_arr_mem_allocator_set_next_free(
            arr_mem_info,
            prev,
            TRUE,
            curr
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
  }
  else
  {
    res = arr_mem_info->free_set_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            curr
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_upadte_list()",0,0);
}


STATIC uint32
  arad_pp_arr_mem_allocator_build_free_entry(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO *arr_mem_info,
    SOC_SAND_IN  uint32                        size,
    SOC_SAND_IN   ARAD_PP_ARR_MEM_ALLOCATOR_PTR     next_ptr,
    SOC_SAND_IN   ARAD_PP_ARR_MEM_ALLOCATOR_PTR     prev_ptr,
    SOC_SAND_IN   ARAD_PP_ARR_MEM_ALLOCATOR_PTR     block_place, /* if not null, then write, entry[] into this block */
    SOC_SAND_OUT  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY   entry[ARAD_PP_ARR_MEM_ALLOCATOR_FREE_ENTRY_LEN]
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    write_place = block_place;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(entry);

  soc_sand_os_memset(entry,0x0,sizeof(ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY)*ARAD_PP_ARR_MEM_ALLOCATOR_FREE_ENTRY_LEN);

  /*1. build header */

  /* cannot support free block < 2 */
  if (size < 2 )
  {
    SOC_SAND_SET_ERROR_CODE(SOC_SAND_GEN_ERR, 10, exit);
  }
  if (size == 2 ){
    /* indicate small block */
    entry[0] = 1 << ARAD_PP_ARR_MEM_ALLOCATOR_FREE_ENTRY_TYPE_LSB;
    /* set next pointer */
    entry[0] |= next_ptr << ARAD_PP_ARR_MEM_ALLOCATOR_EXT_ENTRY_INFO_LSB;
  }
  else
  { /* > 2, set size.*/
    entry[0] |= size << ARAD_PP_ARR_MEM_ALLOCATOR_EXT_ENTRY_INFO_LSB;
  }

  if (write_place != ARAD_PP_ARR_MEM_ALLOCATOR_NULL)
  {
    res = arr_mem_info->entry_set_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            write_place,
            &(entry[0])
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  }


  /*2. build header+1 if needed */
  if (size > 2 ){
    entry[2] |= next_ptr << ARAD_PP_ARR_MEM_ALLOCATOR_EXT_ENTRY_INFO_LSB;

    if (write_place != ARAD_PP_ARR_MEM_ALLOCATOR_NULL)
    {
      res = arr_mem_info->entry_set_fun(
              arr_mem_info->instance_prim_handle,
              ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
              write_place+1,
              &(entry[2])
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
    }
  }
  /*3. build footer, to include prev */
  entry[1] |= prev_ptr << ARAD_PP_ARR_MEM_ALLOCATOR_EXT_ENTRY_INFO_LSB;
  if (write_place != ARAD_PP_ARR_MEM_ALLOCATOR_NULL)
  {
    res = arr_mem_info->entry_set_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            write_place+size-1,
            &(entry[1])
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_build_free_entry()",0,0);
}

STATIC uint32
  arad_pp_arr_mem_allocator_build_used_entry(
    SOC_SAND_IN  uint32                 size,
    SOC_SAND_IN  uint32                 alignment,
    SOC_SAND_OUT  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY   *entry
  )
{

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(entry);

  *entry = size;
  *entry |=  1 << ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_USE_LSB;
  if (alignment == 4) {
      *entry |=  1 << ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_ALIGN_LSB;
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_build_used_entry()",0,0);
}


/* allocate given entry (aligned_ptr)
   free_block_ptr points to start of free-block*/
STATIC
uint32
  arad_pp_arr_mem_allocator_malloc_entry(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR          prev_ptr,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR          free_block_ptr,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR          aligned_ptr,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR          next_ptr,
    SOC_SAND_IN  uint32                                 alloced_size,
    SOC_SAND_IN  uint32                                 alignment
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    new_prev,
    prev = prev_ptr,
    next = next_ptr,
    aligned = aligned_ptr,
    tmp_ptr = free_block_ptr,
    new_next;
  uint32
    block_size,
    tmp_size = alloced_size,
    align_free_size;
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    free_entry[ARAD_PP_ARR_MEM_ALLOCATOR_FREE_ENTRY_LEN],
    align_free_entry[ARAD_PP_ARR_MEM_ALLOCATOR_FREE_ENTRY_LEN],
    used_entry;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_MALLOC);
 /*
  * read the free block entries
  */
  res = arr_mem_info->entry_get_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          tmp_ptr,
          &(free_entry[0])
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

  block_size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(free_entry[0]);
  align_free_size = aligned - tmp_ptr;

 /*
  * build and write the used block entries
  */
  res = arad_pp_arr_mem_allocator_build_used_entry(
          tmp_size,
          alignment,
          &used_entry
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);

  res = arr_mem_info->entry_set_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          aligned,
          &used_entry
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 50, exit);

  res = arr_mem_info->entry_set_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          aligned + tmp_size - 1,
          &used_entry
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 60, exit);

 /*
  * build and write the free block entries
  */
 /*
  * if were alignment
  */
  if (align_free_size != 0)
  {
   /*
    * build free block on the beginning of the free block.
    */
    res = arad_pp_arr_mem_allocator_build_free_entry(
          arr_mem_info,
          align_free_size,
          next,
          prev,
          tmp_ptr,
          align_free_entry
        );
    SOC_SAND_CHECK_FUNC_RESULT(res, 70, exit);
/*
    res = arr_mem_info->entry_set_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            tmp_ptr,
            &(align_free_entry[0])
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 80, exit);

    res = arr_mem_info->entry_set_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            tmp_ptr + align_free_size - 1,
            &(align_free_entry[1])
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 90, exit);
*/
    block_size -= align_free_size;
   /*
    * the prev of current free block, is the free block for alignment.
    */
    prev = tmp_ptr;
   /*
    * the pointer to the memory to use is in aligned.
    */
    tmp_ptr = aligned;
  }
  block_size -= tmp_size;
 /*
  * update free list
  */
  if (block_size > 0)
  {
    new_prev = tmp_ptr + tmp_size;
    new_next = tmp_ptr + tmp_size;
  }
  else
  {
    new_prev = prev;
    new_next = next;
  }
  if (prev == ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END)
  {
    res = arr_mem_info->free_set_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            new_next
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 100, exit);
  }
  else
  {
    res = arad_pp_arr_mem_allocator_set_next_free(
            arr_mem_info,
            prev,
            TRUE,
            new_next
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 110, exit);
  }

  if (next != ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END)
  {
    res = arad_pp_arr_mem_allocator_set_next_free(
            arr_mem_info,
            next,
            FALSE,
            new_prev
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 120, exit);
  }

  if (block_size > 0)
  {
   /*
    * always build from scratch,
    */
    if (0)
    {

      /* new function, set size of free block, start at tmp-ptr, new size is block-size.*/
      res = soc_sand_set_field(
              &(free_entry[0]),
              ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE_MSB,
              ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE_LSB,
              block_size
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 130, exit);

      res = arr_mem_info->entry_set_fun(
              arr_mem_info->instance_prim_handle,
              ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
              tmp_ptr + tmp_size,
              &(free_entry[0])
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 140, exit);
    }
    else /* there need to calculate again*/
    {
      /*
       * This calculation works even if was no alignment
       * but there is no need and to save time.
       */
      res = arad_pp_arr_mem_allocator_build_free_entry(
              arr_mem_info,
              block_size,
              next,
              prev,
              tmp_ptr + tmp_size,
              free_entry
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 150, exit);
/*
      res = arr_mem_info->entry_set_fun(
              arr_mem_info->instance_prim_handle,
              ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
              tmp_ptr + tmp_size,
              &(free_entry[0])
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 160, exit);

      res = arr_mem_info->entry_set_fun(
              arr_mem_info->instance_prim_handle,
              ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
              tmp_ptr + tmp_size + block_size - 1,
              &(free_entry[1])
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 170, exit);
*/
    }
  }
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_malloc()",0,0);
}

/*
 * move block of entries
 */

STATIC uint32
  arad_pp_arr_mem_allocator_move_block(
    SOC_SAND_INOUT  ARAD_PP_ARR_MEM_ALLOCATOR_INFO   *arr_mem_info,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR       from,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR       to,
    SOC_SAND_IN  uint32                              alignment,
   SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR        align_shift,
    SOC_SAND_IN  uint32                         nof_entris,
    SOC_SAND_IN  uint32                         flags
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    next_free,
    prev_free;
  uint32
    block_size=0,
    entry_index,
    from_indx,
    data[3],
    to_indx;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);



  block_size = nof_entris;

  /* if size given by input parameters */
  if (flags & ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_BLK_SIZE_VLD)
  {
  }
  else
  {
    SOC_SAND_SET_ERROR_CODE(SOC_SAND_GEN_ERR, 40, exit);
  }

  /* copy into free link list */
  if (flags & ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_COPY)
  {
    /* get allocated block size, prev-free, next-free */
    prev_free = to;
    res = arad_pp_arr_mem_allocator_get_next_free(
            arr_mem_info,
            FALSE,
            &prev_free
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

    next_free = to;
    res = arad_pp_arr_mem_allocator_get_next_free(
            arr_mem_info,
            TRUE,
            &next_free
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

    /* not assuming, moving to start of empty block */
    res = arad_pp_arr_mem_allocator_malloc_entry(arr_mem_info,prev_free,to,to+align_shift,next_free,block_size,alignment);
    SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);
  }

  /* update content*/
  if (flags & ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_UPDATE_CONT)
  {
    from_indx = from;
    to_indx = to+align_shift;

    for (entry_index = 0 ; entry_index < block_size; ++from_indx,++to_indx,++entry_index)
    {
      res = arr_mem_info->read_fun(
              arr_mem_info->instance_prim_handle,
              ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
              from_indx,
              data
            );
     SOC_SAND_CHECK_FUNC_RESULT(res, 50, exit);

      res = arr_mem_info->write_fun(
              arr_mem_info->instance_prim_handle,
              ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
              to_indx,
              data
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 60, exit);
      }
  }

  
  if (flags & ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_UPDATE_REF && arr_mem_info->entry_move_fun)
  {
    from_indx = from;
    to_indx = to+align_shift;

     /* call alignment callback */
      res = arr_mem_info->entry_move_fun(
              arr_mem_info->instance_prim_handle,
              ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
              data, /* not used */
              from_indx,
              to_indx
            );
        SOC_SAND_CHECK_FUNC_RESULT(res, 80, exit);
  }
  /* remove old */
  if (flags & ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_FREE)
  {
    /* free old block */
    res = arad_pp_arr_mem_allocator_free(arr_mem_info,from);
    SOC_SAND_CHECK_FUNC_RESULT(res, 90, exit);
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_move_block()",0,0);
}


/*
 * move block of entries
 */
STATIC uint32
  arad_pp_arr_mem_allocator_move_block_aux(
    SOC_SAND_INOUT  ARAD_PP_ARR_MEM_ALLOCATOR_INFO   *arr_mem_info,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR       from,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR       to,
    SOC_SAND_IN  uint32                              nof_entris,
    SOC_SAND_IN  uint32                              used_alignment,
    SOC_SAND_IN  uint32                             free_block_size,
    SOC_SAND_IN  uint32                             flags,
    SOC_SAND_OUT  uint32                            *moved_place,
    SOC_SAND_OUT  uint32                            *nof_moves /* count how many entries where moved */
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    reserved_mem_ptr;
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    entry;
  uint32
    block_size=0,
    align_shift,
    reserved_flags,
    alignment=2;
  uint8
    pass_reserved=0; /* need to pass through reserved area, if no enough space to move directly to target place */
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);


  /* no moves */
  *nof_moves = 0;

  /* if size given by input parameters */
  if (flags & ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_BLK_SIZE_VLD)
  {
    block_size = nof_entris;
    alignment = used_alignment;
  }
  else
  {
    /* if size is required */
    if (flags & (ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_UPDATE_REF|ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_UPDATE_CONT|ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_COPY))
    {
      res = arr_mem_info->entry_get_fun(
              arr_mem_info->instance_prim_handle,
              ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
              from,
              &entry
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
      if (ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_IS_USED(entry))
      {
        block_size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(entry);
        alignment = ARAD_PP_ARR_MEM_ALLOCATOR_USED_ENTRY_ALIGN_GET(entry);
      }
      else /*no used entry jump to next block.*/
      {
        block_size = 0;
        SOC_SAND_SET_ERROR_CODE(SOC_SAND_GEN_ERR, 40, exit);
      }
    }
 }

  /* if 4 aligned and 'to' is not aligned then need to skip first 2 entries */
  if (alignment == 4 && (to % 4) != 0) {
      align_shift = 2;
  }
  else{
      align_shift = 0;
  }

  if (to == from - align_shift) { /* no where to move actually */
    *moved_place = to + align_shift;
    *nof_moves = 0;
    goto exit;
  }
  if (align_shift + block_size > free_block_size) {
      pass_reserved = 1; /* copy to reserved then to target */
  }

#ifdef DEFRAG_DEBUG
  LOG_INFO(BSL_LS_SOC_MEM,
           (BSL_META_U(unit,
                       "move from:%u to:%u size:%u align:%u align_shift:%u free_size:%u pass_reserved:%u\n"),from,to,nof_entris,used_alignment,align_shift,free_block_size,pass_reserved));
#endif


  if (pass_reserved) {
      /* move to reserved area */
      reserved_flags = ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_UPDATE_CONT|ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_UPDATE_REF|
               ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_FREE|ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_BLK_SIZE_VLD;

      /* has to be 4 aligned */
      reserved_mem_ptr = arr_mem_info->nof_entries - arr_mem_info->max_block_size;;
      res = arad_pp_arr_mem_allocator_move_block(arr_mem_info,from,reserved_mem_ptr,alignment,0,nof_entris,reserved_flags);
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

      /* move from reserved area to final place */
      reserved_flags = ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_COPY|ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_UPDATE_CONT|
        ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_UPDATE_REF|ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_BLK_SIZE_VLD;
      res = arad_pp_arr_mem_allocator_move_block(arr_mem_info,reserved_mem_ptr,to,alignment,align_shift,nof_entris,reserved_flags);
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

      /* block moved twice, to reserved area, then to target */
      *nof_moves += nof_entris * 2; 

  }
  else{ /* just move to target place */
      res = arad_pp_arr_mem_allocator_move_block(arr_mem_info,from,to,alignment,align_shift,nof_entris,flags);
      SOC_SAND_CHECK_FUNC_RESULT(res, 50, exit);
      /* block moved directly to target */
      *nof_moves += nof_entris; 
  }

  *moved_place = to + align_shift;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_move_block_aux()",0,0);
}



STATIC uint32
  arad_pp_arr_mem_allocator_get_first_fit(
    SOC_SAND_INOUT  ARAD_PP_ARR_MEM_ALLOCATOR_INFO   *arr_mem_info,
    SOC_SAND_INOUT  uint32                      *size,
    SOC_SAND_IN  uint32                         alignment,
    SOC_SAND_OUT  ARAD_PP_ARR_MEM_ALLOCATOR_PTR      *ptr,
    SOC_SAND_OUT  ARAD_PP_ARR_MEM_ALLOCATOR_PTR      *aligned_ptr,
    SOC_SAND_OUT  ARAD_PP_ARR_MEM_ALLOCATOR_PTR      *prev_ptr,
    SOC_SAND_OUT  ARAD_PP_ARR_MEM_ALLOCATOR_PTR      *next_ptr
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    free_ptr,
    aligned_start;
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    entry;
  uint32
    entry_size;
  uint32
    best_size=0xffffffff;
  uint32
    res;
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    best_ptr=0,
    best_prev,
    this_is_best = 0;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);
  SOC_SAND_CHECK_NULL_INPUT(ptr);

  *prev_ptr = ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END;
  *next_ptr = ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END;
  best_prev = ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END;

  res = arr_mem_info->free_get_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          &free_ptr
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

take_best:

  while (free_ptr != ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END)
  {
    res = arr_mem_info->entry_get_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            free_ptr,
            &entry
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

    entry_size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(entry);
    if (entry_size >= *size)
    {
      aligned_start = ARAD_PP_ARR_MEM_ALLOCATOR_GET_ALIGNED(free_ptr,alignment);
     /*
      * don't leave behind free blocks of size 1.
      */
      if (aligned_start - free_ptr == 1)
      {
        aligned_start += alignment;
      }
     /*
      * check if entry can contain block with alignment
      */
      if (entry_size >= *size + (aligned_start - free_ptr))
      {
          if (entry_size<best_size) {
              best_size = entry_size;
              best_ptr = free_ptr;
              best_prev = *prev_ptr;
          }

          if (!this_is_best && best_size != *size) {
              goto try_more;
          }

        if (entry_size - (*size + (aligned_start - free_ptr)) == 1)
        {
          ++(*size);
        }
        *ptr = free_ptr;
        *next_ptr = *ptr;
        res = arad_pp_arr_mem_allocator_get_next_free(
                arr_mem_info,
                TRUE,
                next_ptr
              );
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
        *aligned_ptr = aligned_start;

        /* if support defragment then check this malloced entry doesn't get into reserved area */
        if (arr_mem_info->support_defragment)
        {
          if (*aligned_ptr + *size - 1 >= arr_mem_info->nof_entries - arr_mem_info->max_block_size)
          {
            *ptr = ARAD_PP_ARR_MEM_ALLOCATOR_NULL;
          }
        }
        
        goto exit;
      }
    }
try_more:
    *prev_ptr = free_ptr;
    res = arad_pp_arr_mem_allocator_get_next_free(
            arr_mem_info,
            TRUE,
            &free_ptr
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  }

  if (best_size==0xffffffff) {
      *ptr = ARAD_PP_ARR_MEM_ALLOCATOR_NULL;
  }
  else{
      this_is_best = 1;
      free_ptr = best_ptr;
      *prev_ptr = best_prev;
      goto take_best;
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_get_first_fit()",0,0);
}

/*
* } end of internal functions
*/

uint32
  arad_pp_arr_mem_allocator_create(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO      *arr_mem_info)
{

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_CREATE);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);
  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info->entry_get_fun);
  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info->entry_set_fun);

  if (arr_mem_info->nof_entries < ARAD_PP_ARR_MEM_ALLOCATOR_MIN_MALLOC_SIZE || arr_mem_info->nof_entries - 1 > ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END )
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_ARR_MEM_ALLOCATOR_NOF_LINES_OUT_OF_RANGE_ERR, 10, exit);
  }


 /*
  * allocate the array
  */
  arr_mem_info->arr_mem_allocator_data.array = 
      (ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY*) soc_sand_os_malloc_any_size(sizeof(ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY) * arr_mem_info->nof_entries, "arr_mem_allocator_data.array");
  if (!arr_mem_info->arr_mem_allocator_data.array)
  {
    SOC_SAND_SET_ERROR_CODE(SOC_SAND_MALLOC_FAIL, 20, exit);
  }
 /*
  * reset memory
  */
  soc_sand_os_memset(arr_mem_info->arr_mem_allocator_data.array,0x0,sizeof(ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY) * arr_mem_info->nof_entries);

 /*
  * allocate mem shadow
  */
  arr_mem_info->arr_mem_allocator_data.mem_shadow = 
      (uint32*) soc_sand_os_malloc_any_size(sizeof(uint32) * arr_mem_info->nof_entries * arr_mem_info->entry_size, "arr_mem_allocator_data.mem_shadow ");
  if (!arr_mem_info->arr_mem_allocator_data.mem_shadow)
  {
    SOC_SAND_SET_ERROR_CODE(SOC_SAND_MALLOC_FAIL, 20, exit);
  }
  soc_sand_os_memset(arr_mem_info->arr_mem_allocator_data.mem_shadow,0x0,sizeof(uint32) * arr_mem_info->nof_entries * arr_mem_info->entry_size);



  if (arr_mem_info->support_caching)
  {
   /*
    * allocate cache array
    */
    arr_mem_info->arr_mem_allocator_data.array_cache = 
        (ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY*) soc_sand_os_malloc_any_size(sizeof(ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY) * arr_mem_info->nof_entries, "arr_mem_allocator_data.array_cache");
    if (!arr_mem_info->arr_mem_allocator_data.array_cache)
    {
      SOC_SAND_SET_ERROR_CODE(SOC_SAND_MALLOC_FAIL, 20, exit);
    }
   /*
    * reset memory
    */
    soc_sand_os_memset(arr_mem_info->arr_mem_allocator_data.array_cache,0x0,sizeof(ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY) * arr_mem_info->nof_entries);

    /* updated indexes */
    arr_mem_info->arr_mem_allocator_data.update_indexes = 
        (ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY*) soc_sand_os_malloc_any_size(sizeof(ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY) * arr_mem_info->nof_entries, "arr_mem_allocator_data.update_indexes");
    if (!arr_mem_info->arr_mem_allocator_data.update_indexes)
    {
      SOC_SAND_SET_ERROR_CODE(SOC_SAND_MALLOC_FAIL, 20, exit);
    }
    soc_sand_os_memset(arr_mem_info->arr_mem_allocator_data.update_indexes, 0, sizeof(ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY) * arr_mem_info->nof_entries);
    arr_mem_info->arr_mem_allocator_data.nof_updates = 0;
   /*
    * allocate mem shadow
    */
    arr_mem_info->arr_mem_allocator_data.mem_shadow_cache = 
        (uint32*) soc_sand_os_malloc_any_size(sizeof(uint32) * arr_mem_info->nof_entries * arr_mem_info->entry_size, "arr_mem_allocator_data.mem_shadow_cache");
    if (!arr_mem_info->arr_mem_allocator_data.mem_shadow_cache)
    {
      SOC_SAND_SET_ERROR_CODE(SOC_SAND_MALLOC_FAIL, 20, exit);
    }
    soc_sand_os_memset(arr_mem_info->arr_mem_allocator_data.mem_shadow_cache,0x0,sizeof(uint32) * arr_mem_info->nof_entries * arr_mem_info->entry_size);
  }




exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_create()",0,0);
}


/* mem_allocator must be initialized after created  */
uint32
  arad_pp_arr_mem_allocator_initialize(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO      *arr_mem_info)
{
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    entry[ARAD_PP_ARR_MEM_ALLOCATOR_FREE_ENTRY_LEN];
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_INITIALIZE);


  res = arad_pp_arr_mem_allocator_build_free_entry(
                                                   arr_mem_info,
                                                   ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info)+1,
                                                   ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END,
                                                   ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END,
                                                   0,
                                                   entry
                                                   );
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
 
  res = arr_mem_info->free_set_fun(
                                   arr_mem_info->instance_prim_handle,
                                   ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
                                   0
                                   );
  SOC_SAND_CHECK_FUNC_RESULT(res, 60, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in (arad_pp_arr_mem_allocator_initialize)",0,0);

}

uint32
  arad_pp_arr_mem_allocator_clear(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO      *arr_mem_info
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    entry[ARAD_PP_ARR_MEM_ALLOCATOR_FREE_ENTRY_LEN];
  uint32
      /*indx,*/
      res;
  uint32 zero_uint32 = 0;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_CLEAR);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);
  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info->entry_get_fun);
  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info->entry_set_fun);

  unit = (arr_mem_info->instance_prim_handle);

  if (arr_mem_info->nof_entries < ARAD_PP_ARR_MEM_ALLOCATOR_MIN_MALLOC_SIZE || arr_mem_info->nof_entries - 1 > ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END )
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_ARR_MEM_ALLOCATOR_NOF_LINES_OUT_OF_RANGE_ERR, 10, exit);
  }
 /*
  * reset memory
  */
  if (arr_mem_info->support_caching && arr_mem_info->arr_mem_allocator_data.cache_enabled)
  {
      res = SOC_DPP_WB_ENGINE_MEMSET_ARR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_ARRAY_CACHE, 0);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

      res = SOC_DPP_WB_ENGINE_MEMSET_ARR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_MEM_SHADOW_CACHE, 0);
      SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

      res = SOC_DPP_WB_ENGINE_SET_VAR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_NOF_UPDATES, &zero_uint32);
      SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 30, exit);
  }
  else
  {
      res = SOC_DPP_WB_ENGINE_MEMSET_ARR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_ARRAY, 0);
      SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);

      res = SOC_DPP_WB_ENGINE_MEMSET_ARR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_MEM_SHADOW, 0);
      SOC_SAND_CHECK_FUNC_RESULT(res, 50, exit);
  }
  /*
  * allocate the array
  */

/*
  for (indx=0; indx < arr_mem_info->nof_entries; ++indx) {
      entry[0] = 0x70000000;
      res = arr_mem_info->write_fun(
              arr_mem_info->instance_prim_handle,
              ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
              indx,
              entry
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  }
  */


  res = arad_pp_arr_mem_allocator_build_free_entry(
          arr_mem_info,
          ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info)+1,
          ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END,
          ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END,
          0,
          entry
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 60, exit);
 /*
  * write to entry zero:
  * free block of whole memory size and next free points to NULL
  
  res = arr_mem_info->entry_set_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          0,
          &(entry[0])
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

  res = arr_mem_info->entry_set_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info),
          &(entry[1])
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);
*/
  res = arr_mem_info->free_set_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          0
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 70, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_clear()",0,0);
}


uint32
  arad_pp_arr_mem_allocator_destroy(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO      *arr_mem_info
  )
{
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_DESTROY);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);

  /* free array used memories*/

  res = soc_sand_os_free_any_size(
    arr_mem_info->arr_mem_allocator_data.array
    );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
  res = soc_sand_os_free_any_size(
    arr_mem_info->arr_mem_allocator_data.mem_shadow
    );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  if (arr_mem_info->support_caching)
  {
    res = soc_sand_os_free_any_size(
      arr_mem_info->arr_mem_allocator_data.array_cache
      );
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
    res = soc_sand_os_free_any_size(
      arr_mem_info->arr_mem_allocator_data.update_indexes
      );
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
      res = soc_sand_os_free_any_size(
        arr_mem_info->arr_mem_allocator_data.mem_shadow_cache
    );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_destroy()",0,0);
}

uint32
  arad_pp_arr_mem_allocator_cache_set(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO      *arr_mem_info,
    SOC_SAND_IN uint8                             enable
  )
{
  int res;
  uint32 zero_uint32 = 0;


  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_MALLOC);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);


  unit = (arr_mem_info->instance_prim_handle);


  if (!arr_mem_info->support_caching && enable)
  {
    SOC_SAND_SET_ERROR_CODE(SOC_SAND_ERR, 10, exit);
  }

  /* if already enabled and try to enable then do nothing */
  if (arr_mem_info->arr_mem_allocator_data.cache_enabled && enable)
  {
    goto exit;
  }
  
  res = SOC_DPP_WB_ENGINE_SET_VAR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_CACHE_ENABLED, &enable);
  SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 10, exit);

 /*
  * copy real state to cache state
  */
  if (enable)
  {
      res = SOC_DPP_WB_ENGINE_SET_VAR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_FREE_LIST_CACHE, 
                                      &(arr_mem_info->arr_mem_allocator_data.free_list));
      SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 20, exit);

      res = SOC_DPP_WB_ENGINE_MEMCPY_ARR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_ARRAY_CACHE, 
                                         arr_mem_info->arr_mem_allocator_data.array, 
                                         0,
                                         arr_mem_info->nof_entries);
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

      res = SOC_DPP_WB_ENGINE_MEMCPY_ARR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_MEM_SHADOW_CACHE, 
                                         arr_mem_info->arr_mem_allocator_data.mem_shadow, 
                                         0,
                                         arr_mem_info->nof_entries * arr_mem_info->entry_size);
      SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);
  }
  else 
  {
      /* cache disabled discard all saved changes */
      res = SOC_DPP_WB_ENGINE_SET_VAR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_NOF_UPDATES, &zero_uint32);
      SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 50, exit);
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_cache_set()",0,0);
}


uint32
  arad_pp_arr_mem_allocator_commit(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO      *arr_mem_info,
    SOC_SAND_IN uint32                             flags
  )
{
  uint32
    indx,
    nof_updates,
    offset,
    data[3];
  uint8 
      cache_org=FALSE, false_val=FALSE;
  uint32
    res;
  uint32 zero_uint32 = 0;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_MALLOC);


  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);

  unit = (arr_mem_info->instance_prim_handle);

  if (!arr_mem_info->support_caching)
  {
    SOC_SAND_SET_ERROR_CODE(SOC_SAND_ERR, 10, exit);
  }
  /* if no cache just exit */
  if (!arr_mem_info->arr_mem_allocator_data.cache_enabled)
  {
    goto exit;
  }

  cache_org = arr_mem_info->arr_mem_allocator_data.cache_enabled;

 /* disable caching  */

  res = SOC_DPP_WB_ENGINE_SET_VAR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_CACHE_ENABLED, &false_val);
      SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 50, exit);


  nof_updates = arr_mem_info->arr_mem_allocator_data.nof_updates;
  /* walk over all changes and do them */
  for (indx = 0; indx < nof_updates; ++indx)
  {
    offset = arr_mem_info->arr_mem_allocator_data.update_indexes[indx];

    res = arr_mem_info->read_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_CACHE_INST(arr_mem_info->instance_sec_handle),
            offset,
            data
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

    res = arr_mem_info->write_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            offset,
            data
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

  }
 /* updates are done */
  res = SOC_DPP_WB_ENGINE_SET_VAR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_NOF_UPDATES, &zero_uint32);
  SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 50, exit);

 /*
  * copy cache state to real state
  */
  res = SOC_DPP_WB_ENGINE_SET_VAR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_FREE_LIST, 
                                  &(arr_mem_info->arr_mem_allocator_data.free_list_cache));
  SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 50, exit);

  res = SOC_DPP_WB_ENGINE_MEMCPY_ARR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_ARRAY, 
                                     arr_mem_info->arr_mem_allocator_data.array_cache,
                                     0,
                                     arr_mem_info->nof_entries);
  SOC_SAND_CHECK_FUNC_RESULT(res, 50, exit);

  res = SOC_DPP_WB_ENGINE_MEMCPY_ARR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_MEM_SHADOW, 
                                     arr_mem_info->arr_mem_allocator_data.mem_shadow_cache, 
                                     0,
                                     arr_mem_info->nof_entries * arr_mem_info->entry_size);
  SOC_SAND_CHECK_FUNC_RESULT(res, 50, exit);

  
exit:
  res = SOC_DPP_WB_ENGINE_SET_VAR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_CACHE_ENABLED, &cache_org);
  SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 50, exit2);

exit2:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_commit()",0,0);
}


uint32
  arad_pp_arr_mem_allocator_rollback(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO      *arr_mem_info,
    SOC_SAND_IN uint32                             flags
  )
{
  int res;
  uint32 zero_uint32 = 0;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_MALLOC);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);

  unit = (arr_mem_info->instance_prim_handle);

  if (!arr_mem_info->support_caching)
  {
    SOC_SAND_SET_ERROR_CODE(SOC_SAND_ERR, 10, exit);
  }
  /* if no cache just exit */
  if (!arr_mem_info->arr_mem_allocator_data.cache_enabled)
  {
    goto exit;
  }

  res = SOC_DPP_WB_ENGINE_SET_VAR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_NOF_UPDATES, &zero_uint32);
  SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 10, exit);

  res = SOC_DPP_WB_ENGINE_SET_VAR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_FREE_LIST_CACHE, 
                                  &(arr_mem_info->arr_mem_allocator_data.free_list));
  SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 20, exit);

  res = SOC_DPP_WB_ENGINE_MEMCPY_ARR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_ARRAY_CACHE, 
                                     arr_mem_info->arr_mem_allocator_data.array,
                                     0,
                                     arr_mem_info->nof_entries);
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

  res = SOC_DPP_WB_ENGINE_MEMCPY_ARR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_MEM_SHADOW_CACHE, 
                                     arr_mem_info->arr_mem_allocator_data.mem_shadow, 
                                     0,
                                     arr_mem_info->nof_entries * arr_mem_info->entry_size);
  SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);

    
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_rollback()",0,0);
}


uint32
  arad_pp_arr_mem_allocator_malloc(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
    SOC_SAND_IN  uint32                            size,
    SOC_SAND_IN  uint32                            alignment,
    SOC_SAND_OUT  ARAD_PP_ARR_MEM_ALLOCATOR_PTR         *ptr
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    prev,
    aligned = 0,
    tmp_ptr = 0,
    next;
  uint32
    tmp_size = size,
    act_alignment;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_MALLOC);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);
  SOC_SAND_CHECK_NULL_INPUT(ptr);

  if(tmp_size < ARAD_PP_ARR_MEM_ALLOCATOR_MIN_MALLOC_SIZE)
  {
    tmp_size = ARAD_PP_ARR_MEM_ALLOCATOR_MIN_MALLOC_SIZE;
  }

  if (tmp_size > arr_mem_info->nof_entries)
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_ARR_MEM_ALLOCATOR_MALLOC_SIZE_OUT_OF_RANGE_ERR, 10, exit);
  }
  if (arr_mem_info->support_defragment && tmp_size > arr_mem_info->max_block_size )
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_ARR_MEM_ALLOCATOR_MALLOC_SIZE_OUT_OF_RANGE_ERR, 15, exit);
  }
  
  act_alignment = (alignment > 0)? alignment: 1;

  if (tmp_size % 2)
  {
	  tmp_size+=1;
  }
  
 /*
  * get pointer to the first fit block
  * block with size equal or bigger than the
  * required malloc size
  */
  res = arad_pp_arr_mem_allocator_get_first_fit(
          arr_mem_info,
          &tmp_size,
          act_alignment,
          &tmp_ptr,
          &aligned,
          &prev,
          &next
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);


  if (tmp_ptr == ARAD_PP_ARR_MEM_ALLOCATOR_NULL)
  {
    *ptr = ARAD_PP_ARR_MEM_ALLOCATOR_NULL;
    goto exit;
  }
 /*
  * now update the management info
  */
  res = arad_pp_arr_mem_allocator_malloc_entry(arr_mem_info,prev,tmp_ptr,aligned,next,tmp_size,alignment);
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

  *ptr = aligned;


exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_malloc()",0,0);
}

uint32
  arad_pp_arr_mem_allocator_free(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR          ptr
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    prev,
    next;
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    free_entries[ARAD_PP_ARR_MEM_ALLOCATOR_FREE_ENTRY_LEN];
  uint32
    before_size,
    after_size,
    used_block_size;
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    used_entry,
    prev_entry,
    next_entry;
  uint8
    after_free,
    before_free;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_FREE);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);

  if (ptr > ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info))
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_ARR_MEM_ALLOCATOR_POINTER_OF_RANGE_ERR, 10, exit);
  }


  res = arr_mem_info->entry_get_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          ptr,
          &used_entry
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

  if (!ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_IS_USED(used_entry))
  {
    /* free not used entry */
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_ARR_MEM_ALLOCATOR_POINTER_OF_RANGE_ERR, 220, exit);
  }

  used_block_size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(used_entry);

  before_free = FALSE;
  after_free = FALSE;
  prev_entry = 0;
  next_entry = 0;
 /*
  * find the limits of the free block.
  */

 /*
  * check there is a free block above
  */
  if (ptr > 0)
  {
    res = arr_mem_info->entry_get_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            ptr - 1,
            &prev_entry
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
   /*
    * prev block is free.
    */
    before_free = (uint8)!(ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_IS_USED(prev_entry));
  }
  if (ptr + used_block_size <  ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info)+1)
  {
    res = arr_mem_info->entry_get_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            ptr + used_block_size,
            &next_entry
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);
   /*
    * next block is free.
    */
    after_free = (uint8)!(ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_IS_USED(next_entry));
  }

  after_size = 0;
  before_size = 0;
  prev = 0;
  next = 0;

  if (!after_free && !before_free)
  {
    prev = ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END;
    res = arr_mem_info->free_get_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            &next
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 50, exit);
  }
  else if (after_free && before_free)
  {
    res = arad_pp_arr_mem_allocator_header_get(
              arr_mem_info,
              ptr - 1,
              &prev
            );
    SOC_SAND_CHECK_FUNC_RESULT(res, 60, exit);

    res = arr_mem_info->entry_get_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            prev,
            &prev_entry
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 70, exit);

    before_size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(prev_entry);

    next = ptr + used_block_size;
    res = arr_mem_info->entry_get_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            next,
            &next_entry
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 80, exit);
    after_size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(next_entry);
   /*
    * remove one of the free blocks. and update list.
    */
    res = arad_pp_arr_mem_allocator_remove_free_entry(
            arr_mem_info,
            prev
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 90, exit);

    prev = ptr + used_block_size;
    res = arad_pp_arr_mem_allocator_get_next_free(
            arr_mem_info,
            FALSE,
            &prev
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 100, exit);

    next = ptr + used_block_size;
    res = arad_pp_arr_mem_allocator_get_next_free(
            arr_mem_info,
            TRUE,
            &next
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 110, exit);
  }
  else
  {
    if (before_free)
    {
      res = arad_pp_arr_mem_allocator_header_get(
              arr_mem_info,
              ptr - 1,
              &prev
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 120, exit);

      next = prev;
      res = arad_pp_arr_mem_allocator_get_next_free(
              arr_mem_info,
              TRUE,
              &next
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 130, exit);
    }
    if (after_free)
    {
      next = ptr + used_block_size;
      prev = next;
      res = arad_pp_arr_mem_allocator_get_next_free(
              arr_mem_info,
              FALSE,
              &prev
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 140, exit);
    }
   /*
    * get next next
    */
    if (after_free)
    {
      res = arr_mem_info->entry_get_fun(
              arr_mem_info->instance_prim_handle,
              ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
              next,
              &next_entry
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 150, exit);
      after_size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(next_entry);

      res = arad_pp_arr_mem_allocator_get_next_free(
                arr_mem_info,
                TRUE,
                &next
              );
        SOC_SAND_CHECK_FUNC_RESULT(res, 160, exit);
    }
   /*
    * get prev prev
    */
    if (before_free)
    {
      res = arr_mem_info->entry_get_fun(
              arr_mem_info->instance_prim_handle,
              ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
              prev,
              &prev_entry
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 170, exit);

      before_size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(prev_entry);

      res = arad_pp_arr_mem_allocator_get_next_free(
              arr_mem_info,
              FALSE,
              &prev
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 180, exit);
    }
  }
 /*
  * build the unified block.
  */
  res = arad_pp_arr_mem_allocator_build_free_entry(
          arr_mem_info,
          before_size + used_block_size + after_size,
          next,
          prev,
          ptr - before_size,
          free_entries
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 190, exit);
/*
  res = arr_mem_info->entry_set_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          ptr - before_size,
          &(free_entries[0])
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 200, exit);

  res = arr_mem_info->entry_set_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          ptr + after_size + used_block_size - 1,
          &(free_entries[1])
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 210, exit);
*/
  res = arad_pp_arr_mem_allocator_upadte_list(
          arr_mem_info,
          prev,
          ptr - before_size,
          next
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 220, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_free()",0,0);
}

uint32
  arad_pp_arr_mem_allocator_block_size(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR          ptr,
    SOC_SAND_OUT  uint32                           *size
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    used_entry;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_FREE);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);

  if (ptr> ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info))
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_ARR_MEM_ALLOCATOR_POINTER_OF_RANGE_ERR, 10, exit);
  }

  res = arr_mem_info->entry_get_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          ptr,
          &used_entry
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

  *size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(used_entry);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_array_set()",0,0);
}

/*********************************************************************
* NAME:
*     arad_pp_arr_mem_allocator_write
* TYPE:
*   PROC
* DATE:
*   May  6 2008
* FUNCTION:
*  write row of data to the memory.
* INPUT:
*   SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info -
*     The arr_mem_alloc instance
*   SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR     offset -
*     the entry to write to.
*   SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY        *data -
*     to hold the written data.
*     size has to be as row size.
* RETURNS:
*     OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_arr_mem_allocator_write(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR          offset,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY        *data
  )
{

  uint32
    res;
  uint32 nof_updates_new_val;


  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_WRITE);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);

  unit = (arr_mem_info->instance_prim_handle);


  if (offset > ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info))
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_ARR_MEM_ALLOCATOR_POINTER_OF_RANGE_ERR, 10, exit);
  }

  res = arr_mem_info->write_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          offset,
          data
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
 /*
  * if there was caching store this
  */
  if (arr_mem_info->arr_mem_allocator_data.cache_enabled)
  {
    /* check for overflow */
    if (arr_mem_info->arr_mem_allocator_data.nof_updates >= arr_mem_info->nof_entries) {
        SOC_SAND_SET_ERROR_CODE(ARAD_PP_ARR_MEM_ALLOCATOR_NOF_LINES_OUT_OF_RANGE_ERR, 10, exit);
    }


    res = SOC_DPP_WB_ENGINE_SET_ARR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_UPDATE_INDEXES, 
                                    &offset, 
                                    arr_mem_info->arr_mem_allocator_data.nof_updates);
    SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 30, exit);


    nof_updates_new_val = arr_mem_info->arr_mem_allocator_data.nof_updates + 1;

    res = SOC_DPP_WB_ENGINE_SET_VAR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_NOF_UPDATES, 
                                    &(nof_updates_new_val));
    SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 40, exit);
        
  }
  
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_write()",0,0);
}


uint32
  arad_pp_arr_mem_allocator_write_chunk(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR          offset,
    SOC_SAND_IN  uint32                                 nof_entries,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY        *data
  )
{
  uint32
    res;
  uint32
    indx;
  uint32 nof_updates_new_val;
  uint32 update_indexes_new_val;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_WRITE);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);


  unit = (arr_mem_info->instance_prim_handle);

  if (offset + nof_entries -1 > ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info))
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_ARR_MEM_ALLOCATOR_POINTER_OF_RANGE_ERR, 10, exit);
  }

  res = arr_mem_info->write_block_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          offset,
          nof_entries,
          data
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
 /*
  * if there was caching store this
  */
  if (arr_mem_info->arr_mem_allocator_data.cache_enabled)
  {
      for(indx = 0; indx < nof_entries; ++indx) {

        update_indexes_new_val = offset + indx;

        res = SOC_DPP_WB_ENGINE_SET_ARR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_UPDATE_INDEXES, 
                                        &update_indexes_new_val, 
                                        arr_mem_info->arr_mem_allocator_data.nof_updates);
        SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 30, exit);


        nof_updates_new_val = arr_mem_info->arr_mem_allocator_data.nof_updates + 1;

        res = SOC_DPP_WB_ENGINE_SET_VAR(unit, arr_mem_info->wb_var_index + WB_ENGINE_MEM_ALLOCATOR_NOF_UPDATES, 
                                        &(nof_updates_new_val));
        SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 40, exit);


      }
  }
  
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_write_chunk()",0,0);
}



/*********************************************************************
* NAME:
*     arad_pp_arr_mem_allocator_read
* TYPE:
*   PROC
* DATE:
*   May  6 2008
* FUNCTION:
*  read row of data from the memory.
* INPUT:
*   SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info -
*     The arr_mem_alloc instance
*   SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR     offset -
*     the entry to read from.
*   SOC_SAND_OUT  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY        *data -
*     to hold the readen data.
*     size has to be as row size.
* RETURNS:
*     OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_arr_mem_allocator_read(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR          offset,
    SOC_SAND_OUT  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY        *data
  )
{

  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_READ);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);

  if (offset> ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info))
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_ARR_MEM_ALLOCATOR_POINTER_OF_RANGE_ERR, 10, exit);
  }

  res = arr_mem_info->read_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          offset,
          data
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_read()",0,0);
}


uint32
  arad_pp_arr_mem_allocator_read_chunk(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_PTR          offset,
    SOC_SAND_IN  uint32                                  nof_entries,
    SOC_SAND_OUT  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY        *data
  )
{

  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_READ);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);

  if (offset> ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info))
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_ARR_MEM_ALLOCATOR_POINTER_OF_RANGE_ERR, 10, exit);
  }

  res = arr_mem_info->read_block_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          offset,
          nof_entries,
          data
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_read()",0,0);
}

STATIC uint32
  arad_pp_arr_mem_allocator_get_next_used_block(
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
    SOC_SAND_INOUT  ARAD_PP_ARR_MEM_ALLOCATOR_ITER  *iter,
    SOC_SAND_OUT uint32                        *align,   
    SOC_SAND_IN  uint32                        pos
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    indx;
  uint32
    block_size,
    cur_pos=pos;
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    entry;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);

  indx = iter->offset;
  block_size = 0;

  while (block_size == 0 && indx <= ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info))
  {
    res = arr_mem_info->entry_get_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            indx,
            &entry
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    if (ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_IS_USED(entry))
    {
      if (!cur_pos)
      {
        block_size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(entry);
      }
      else /*no used entry jump to next block.*/
      {
        --cur_pos;
        indx += ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(entry);
      } 

    }
    else /*no used entry jump to next block.*/
    {
      indx += ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(entry);
    }
  }

  iter->block_size = block_size;
  *align = ARAD_PP_ARR_MEM_ALLOCATOR_USED_ENTRY_ALIGN_GET(entry);

  if (indx > ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info))
  {
    iter->offset = ARAD_PP_ARR_MEM_ALLOCATOR_NULL;
  }
  else
  {
    iter->offset = indx;
  }


exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_get_next_free()",0,0);
}

STATIC uint32
  arad_pp_arr_mem_allocator_get_next_free_in_order(
    SOC_SAND_IN  ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
    SOC_SAND_INOUT  ARAD_PP_ARR_MEM_ALLOCATOR_ITER  *iter
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    indx;
  uint32
    block_size;
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    entry;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);

  indx = iter->offset;
  block_size = 1;

  while (indx <= ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info))
  {
    res = arr_mem_info->entry_get_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            indx,
            &entry
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    if (ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_IS_USED(entry))/* used entry jump to next block.*/
    {
      indx += ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(entry);
    }
    else 
    {
      block_size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(entry);
      break;
    }
  }

  iter->block_size = block_size;
  if (indx > ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info))
  {
    iter->offset = ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END;
  }
  else
  {
    iter->offset = indx;
  }


exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_get_next_free()",0,0);
}




/*********************************************************************
* NAME:
*     arad_pp_arr_mem_allocator_read_block
* TYPE:
*   PROC
* DATE:
*   May  6 2008
* FUNCTION:
*  read block of entries from the the memory.
* INPUT:
*   SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info -
*     The arr_mem_alloc instance to read from.
*   SOC_SAND_INOUT  ARAD_PP_ARR_MEM_ALLOCATOR_ITER       *iter -
*     iterator to traverse the memory.
*     use ARAD_PP_ARR_MEM_ALLOCATOR_ITER_clear(iter) to
*     clear the iterator and start from the beginning of the
*     memory.
*     use ARAD_PP_ARR_MEM_ALLOCATOR_ITER_END(iter) to
*     check if the iterator reached the end of the memory.
*   SOC_SAND_IN  uint32                             entries_to_read -
*     number of valid (used) entries to get.
*   SOC_SAND_OUT  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY        *data -
*     buffer to hold the readen data has to be of size 'entries_to_read'
*     at least.
*   SOC_SAND_OUT  ARAD_PP_ARR_MEM_ALLOCATOR_PTR          *addresses -
*     buffer to hold the addresses of the valid entries.
*     has to be of size 'entries_to_read'
*   SOC_SAND_OUT  uint32                            *nof_entries
*     number of valid entries in data and addresses
* RETURNS:
*     OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_arr_mem_allocator_read_block(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO      *arr_mem_info,
    SOC_SAND_INOUT  ARAD_PP_ARR_MEM_ALLOCATOR_ITER       *iter,
    SOC_SAND_IN  uint32                             entries_to_read,
    SOC_SAND_OUT  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY        *data,
    SOC_SAND_OUT  ARAD_PP_ARR_MEM_ALLOCATOR_PTR          *addresses,
    SOC_SAND_OUT  uint32                            *nof_entries
  )
{
  uint32
    readen,
    align;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(ARAD_PP_ARR_MEM_ALLOCATOR_READ_BLOCK);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);
  SOC_SAND_CHECK_NULL_INPUT(iter);
  SOC_SAND_CHECK_NULL_INPUT(data);
  SOC_SAND_CHECK_NULL_INPUT(addresses);
  SOC_SAND_CHECK_NULL_INPUT(nof_entries);

  readen = 0;

  if (iter->offset > ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info))
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_ARR_MEM_ALLOCATOR_POINTER_OF_RANGE_ERR, 10, exit);
  }

  for (; iter->offset <= ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info) && readen < entries_to_read; ++iter->offset )
  {
    if (iter->block_size == 0)
    {
      res = arad_pp_arr_mem_allocator_get_next_used_block(
              arr_mem_info,
              iter,
              &align,
              0
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit)
    }
    if(ARAD_PP_ARR_MEM_ALLOCATOR_ITER_END(iter))
    {
      break;
    }
    res = arr_mem_info->read_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            iter->offset,
            &(data[readen])
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

    addresses[readen] = iter->offset;
    ++readen;
    --(iter->block_size);
  }
  *nof_entries = readen;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_read_block()",0,0);
}

uint32
  arad_pp_arr_mem_get_first_free(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
	  SOC_SAND_OUT   uint32         *first_free_ptr
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    free_ptr;
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    entry;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);

  res = arr_mem_info->free_get_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          &free_ptr
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  *first_free_ptr = free_ptr;

  while (free_ptr != ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END)
  {
    res = arr_mem_info->entry_get_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            free_ptr,
            &entry
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

    if (*first_free_ptr > free_ptr) {
        *first_free_ptr = free_ptr;
    }
    res = arad_pp_arr_mem_allocator_get_next_free(
            arr_mem_info,
            TRUE,
            &free_ptr
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_get_first_free()",0,0);
}
/*********************************************************************
* NAME:
*     arad_pp_arr_mem_allocator_defrag
* TYPE:
*   PROC
* DATE:
*   May  6 2008
* FUNCTION:
*  perform defragmentation for the memory.
* INPUT:
*   SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info -
*     The arr_mem_alloc instance to read from.
*   SOC_SAND_INOUT  ARAD_PP_ARR_MEM_ALLOCATOR_DEFRAG_INFO       *defrag_info -
*     defragmentation info
* RETURNS:
*     OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_arr_mem_allocator_defrag(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO       *arr_mem_info,
    SOC_SAND_INOUT ARAD_PP_ARR_MEM_ALLOCATOR_DEFRAG_INFO  *defrag_info
  )
{
  uint32
    nof_moves = 0,
    total_nof_moves = 0;
  ARAD_PP_ARR_MEM_ALLOCATOR_ITER  
    cur_used,
    first_free_iter;
  uint8
    done = FALSE;
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    free_ptr;
  SOC_SAND_TABLE_BLOCK_RANGE             
    *block_range;
  uint32
    alignment,
    free_block_size,
    moved_place=0,
    flags;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);
  SOC_SAND_CHECK_NULL_INPUT(defrag_info);


  block_range = &(defrag_info->block_range);

  if (!arr_mem_info->support_defragment)
  {
    SOC_SAND_SET_ERROR_CODE(SOC_SAND_ERR, 10, exit);
  }

  /* check iter status */
  if (SOC_SAND_TBL_ITER_IS_END(&block_range->iter))
  {
    goto exit;
  }


  /* start defragment from this iter-place */
  first_free_iter.offset = defrag_info->block_range.iter;

  arad_pp_arr_mem_get_first_free(arr_mem_info,&first_free_iter.offset);

  res = arad_pp_arr_mem_allocator_get_next_free_in_order(arr_mem_info,&first_free_iter);
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  free_ptr = first_free_iter.offset; /* move entry to this place */

  if (free_ptr == ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END)
  {
    /* no free space */
    SOC_SAND_TBL_ITER_SET_END(&block_range->iter);
    goto exit;
  }
  
  /* get free block-size */
  free_block_size = first_free_iter.block_size;

  flags = ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_FREE|ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_COPY|
          ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_UPDATE_CONT|ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_UPDATE_REF|ARAD_PP_ARR_MEM_ALLOCATOR_MOVE_BLK_SIZE_VLD;
  /* defragment loop */
  while (!done && total_nof_moves < block_range->entries_to_act)
  {
    if (free_ptr == ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END) {
      break; /* done */
    }
    cur_used.offset = free_ptr + free_block_size; /* entry to move up */

    /* get next used entry to move it */
    res = arad_pp_arr_mem_allocator_get_next_used_block(arr_mem_info,&cur_used,&alignment,0);
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    if (cur_used.offset == ARAD_PP_ARR_MEM_ALLOCATOR_NULL)
    {
      done = TRUE;
      break;
    }

    /* move to target place */
    res = arad_pp_arr_mem_allocator_move_block_aux(arr_mem_info,cur_used.offset,free_ptr,cur_used.block_size,alignment,free_block_size,flags, &moved_place, &nof_moves);
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    /* entries moves */

    total_nof_moves += nof_moves;

   /* jump to next used */
    cur_used.offset += cur_used.block_size;
    if (cur_used.offset  > ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info))
    {
      done = TRUE;
      break;
    }

    /* next free node: place after moved block, if not moved due to alignment then this will get next free block */
    first_free_iter.offset = moved_place + cur_used.block_size;
    res = arad_pp_arr_mem_allocator_get_next_free_in_order(arr_mem_info,&first_free_iter);
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

    free_block_size = first_free_iter.block_size;
    free_ptr = first_free_iter.offset;
  }
  
  if (done)
  {
    SOC_SAND_TBL_ITER_SET_END(&defrag_info->block_range.iter);
  }
  else
  {
    defrag_info->block_range.iter = free_ptr;
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_read_block()",0,0);
}


uint32
  arad_pp_arr_mem_allocator_print_free(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    free_ptr;
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    entry;
  uint32
    entry_size;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);

  res = arr_mem_info->free_get_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          &free_ptr
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  LOG_CLI((BSL_META_U(unit,
                      "free blocks:\n")));

  while (free_ptr != ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END)
  {
    res = arr_mem_info->entry_get_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            free_ptr,
            &entry
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

    entry_size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(entry);
    LOG_CLI((BSL_META_U(unit,
                        "start:%u   end:%u \n"), free_ptr, free_ptr + entry_size));

    res = arad_pp_arr_mem_allocator_get_next_free(
            arr_mem_info,
            TRUE,
            &free_ptr
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_print_free()",0,0);
}



uint32
  arad_pp_arr_mem_allocator_print_free_by_order(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_ITER  iter;
  uint32
    entry_size;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);

  iter.offset = 0;

  res = arad_pp_arr_mem_allocator_get_next_free_in_order(
          arr_mem_info,
          &iter
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  LOG_CLI((BSL_META_U(unit,
                      "free blocks by order:\n")));

  while (iter.offset != ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END)
  {
    entry_size = iter.block_size;
    LOG_CLI((BSL_META_U(unit,
                        "start:%u   end:%u \n"), iter.offset, iter.offset + entry_size));

    iter.offset += entry_size;
    res = arad_pp_arr_mem_allocator_get_next_free_in_order(
            arr_mem_info,
            &iter
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_print_free_by_order()",0,0);
}


uint32
  arad_pp_arr_mem_allocator_is_availabe_blocks(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
	  SOC_SAND_IN   ARAD_PP_ARR_MEM_ALLOCATOR_REQ_BLOCKS  *req_blocks,
	  SOC_SAND_OUT   uint8                         *available
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    free_ptr;
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    entry;
  uint32
    block_indx = 0;
  uint32
    entry_size;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);

  res = arr_mem_info->free_get_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          &free_ptr
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  while (free_ptr != ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END)
  {
    res = arr_mem_info->entry_get_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            free_ptr,
            &entry
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

    entry_size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(entry);

	while(entry_size >= req_blocks->block_size[block_indx])
	{
		entry_size -= req_blocks->block_size[block_indx];
		block_indx++;
		if (block_indx >= req_blocks->nof_reqs)
		{
			*available = TRUE;
			goto exit;
		}
	}
    res = arad_pp_arr_mem_allocator_get_next_free(
            arr_mem_info,
            TRUE,
            &free_ptr
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  }

  *available = FALSE; 

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_is_availabe_blocks()",0,0);
}


uint32
  arad_pp_arr_mem_allocator_mem_status_get(
    SOC_SAND_INOUT   ARAD_PP_ARR_MEM_ALLOCATOR_INFO     *arr_mem_info,
	  SOC_SAND_OUT   ARAD_PP_ARR_MEM_ALLOCATOR_MEM_STATUS  *mem_status
  )
{
  ARAD_PP_ARR_MEM_ALLOCATOR_PTR
    free_ptr;
  ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY
    entry;
  uint32
	  total_free = 0,
      total_size_2 = 0,
	  max_free_size = 0;
  uint32
    entry_size;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);

  SOC_SAND_CHECK_NULL_INPUT(arr_mem_info);

  ARAD_PP_ARR_MEM_ALLOCATOR_MEM_STATUS_clear(mem_status);

  res = arr_mem_info->free_get_fun(
          arr_mem_info->instance_prim_handle,
          ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
          &free_ptr
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);


  while (free_ptr != ARAD_PP_ARR_MEM_ALLOCATOR_LIST_END)
  {
    res = arr_mem_info->entry_get_fun(
            arr_mem_info->instance_prim_handle,
            ARAD_PP_ARR_MEM_ALLOCATOR_ACTIVE_INST(arr_mem_info,arr_mem_info->instance_sec_handle),
            free_ptr,
            &entry
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

    entry_size = ARAD_PP_ARR_MEM_ALLOCATOR_ENTRY_SIZE(entry);
    if (entry_size == 2) {
        ++total_size_2;
    }
    total_free += entry_size;
	  if(entry_size > max_free_size)
	  {
		  max_free_size = entry_size;
	  }
    ++mem_status->nof_fragments;

    res = arad_pp_arr_mem_allocator_get_next_free(
            arr_mem_info,
            TRUE,
            &free_ptr
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  }

  mem_status->max_free_block_size = max_free_size;
  mem_status->total_free = total_free;
  mem_status->total_size = ARAD_PP_ARR_MEM_ALLOCATOR_LAST_INDEX(arr_mem_info) + 1;
#ifdef DEFRAG_DEBUG
  arad_pp_arr_mem_allocator_print_free_by_order(arr_mem_info);
  LOG_INFO(BSL_LS_SOC_MEM,
           (BSL_META_U(unit,
                       "total_size_2: %u\n\r"),total_size_2));
#endif

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_arr_mem_allocator_mem_status_get()",0,0);
}





void
  ARAD_PP_ARR_MEM_ALLOCATOR_INFO_clear(
    ARAD_PP_ARR_MEM_ALLOCATOR_INFO *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  soc_sand_os_memset(info, 0x0, sizeof(ARAD_PP_ARR_MEM_ALLOCATOR_INFO));
  info->nof_entries = 0;
  info->instance_prim_handle = 0;
  info->instance_sec_handle = 0;
  info->entry_get_fun = 0;
  info->entry_set_fun = 0;
  info->support_caching = 0;
  info->support_defragment = 0;
  info->entry_size = 1;
  SOC_SAND_MAGIC_NUM_SET;
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}


void
  ARAD_PP_ARR_MEM_ALLOCATOR_ITER_clear(
    ARAD_PP_ARR_MEM_ALLOCATOR_ITER *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  soc_sand_os_memset(info, 0x0, sizeof(ARAD_PP_ARR_MEM_ALLOCATOR_ITER));
  info->block_size = 0;
  info->offset = 0;
  SOC_SAND_MAGIC_NUM_SET;
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  ARAD_PP_ARR_MEM_ALLOCATOR_REQ_BLOCKS_clear(
    ARAD_PP_ARR_MEM_ALLOCATOR_REQ_BLOCKS *info
  )
{
  uint32
    ind;
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  soc_sand_os_memset(info, 0x0, sizeof(ARAD_PP_ARR_MEM_ALLOCATOR_REQ_BLOCKS));

  for (ind = 0; ind < ARAD_PP_ARR_MEM_ALLOCATOR_MAX_NOF_REQS; ++ind)
  {
     info->block_size[ind] = 0;
  }
  info->nof_reqs = 0;
    
  SOC_SAND_MAGIC_NUM_SET;
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  ARAD_PP_ARR_MEM_ALLOCATOR_MEM_STATUS_clear(
    ARAD_PP_ARR_MEM_ALLOCATOR_MEM_STATUS *info
  )
{

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  soc_sand_os_memset(info, 0x0, sizeof(ARAD_PP_ARR_MEM_ALLOCATOR_MEM_STATUS));

  info->total_free = 0;
  info->max_free_block_size = 0;
  info->nof_fragments = 0;
  info->total_size = 0;
    
  SOC_SAND_MAGIC_NUM_SET;
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  ARAD_PP_ARR_MEM_ALLOCATOR_DEFRAG_INFO_clear(
    ARAD_PP_ARR_MEM_ALLOCATOR_DEFRAG_INFO *info
  )
{

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  soc_sand_os_memset(info, 0x0, sizeof(ARAD_PP_ARR_MEM_ALLOCATOR_DEFRAG_INFO));

  info->block_range.iter = 0;
  info->block_range.entries_to_scan = SOC_SAND_TBL_ITER_SCAN_ALL;
  info->block_range.entries_to_act = SOC_SAND_TBL_ITER_SCAN_ALL;
    
  SOC_SAND_MAGIC_NUM_SET;
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

#if SOC_SAND_DEBUG
void
  ARAD_PP_ARR_MEM_ALLOCATOR_INFO_print(
    ARAD_PP_ARR_MEM_ALLOCATOR_INFO *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  LOG_CLI((BSL_META_U(unit,
                      "nof_entries: %u\n\r"),info->nof_entries));
  LOG_CLI((BSL_META_U(unit,
                      "instance_prim_handle: %u\n\r"),info->instance_prim_handle));
  LOG_CLI((BSL_META_U(unit,
                      "instance_sec_handle: %u\n\r"),info->instance_sec_handle));
  LOG_CLI((BSL_META_U(unit,
                      "support_caching: %u\n\r"),info->support_caching));
  LOG_CLI((BSL_META_U(unit,
                      "support_defragment: %u\n\r"),info->support_defragment));
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}


void
  ARAD_PP_ARR_MEM_ALLOCATOR_ITER_print(
    ARAD_PP_ARR_MEM_ALLOCATOR_ITER *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  LOG_CLI((BSL_META_U(unit,
                      "block_size: %u\n\r"),info->block_size));
  LOG_CLI((BSL_META_U(unit,
                      "offset: %u\n\r"),info->offset));
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  ARAD_PP_ARR_MEM_ALLOCATOR_REQ_BLOCKS_print(
    SOC_SAND_IN ARAD_PP_ARR_MEM_ALLOCATOR_REQ_BLOCKS *info
  )
{
  uint32
    ind;
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);


  for (ind = 0; ind < ARAD_PP_ARR_MEM_ALLOCATOR_MAX_NOF_REQS; ++ind)
  {
     LOG_CLI((BSL_META_U(unit,
                         "block_size[ind]: %u\n\r"),info->block_size[ind]));
  }
  LOG_CLI((BSL_META_U(unit,
                      "nof_reqs: %u\n\r"),info->nof_reqs));
    
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  ARAD_PP_ARR_MEM_ALLOCATOR_MEM_STATUS_print(
    SOC_SAND_IN ARAD_PP_ARR_MEM_ALLOCATOR_MEM_STATUS *info
  )
{

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);


  LOG_CLI((BSL_META_U(unit,
                      "total_free: %u\n\r"),info->total_free));
  LOG_CLI((BSL_META_U(unit,
                      "max_free_block_size: %u\n\r"),info->max_free_block_size));
  LOG_CLI((BSL_META_U(unit,
                      "nof_fragments: %u\n\r"),info->nof_fragments));
  LOG_CLI((BSL_META_U(unit,
                      "total_size: %u\n\r"),info->total_size));
    
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  ARAD_PP_ARR_MEM_ALLOCATOR_DEFRAG_INFO_print(
    SOC_SAND_IN ARAD_PP_ARR_MEM_ALLOCATOR_DEFRAG_INFO *info
  )
{

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);


  LOG_CLI((BSL_META_U(unit,
                      "block_range.iter: %u\n\r"),info->block_range.iter));
  LOG_CLI((BSL_META_U(unit,
                      "block_range.entries_to_scan: %u\n\r"),info->block_range.entries_to_scan));
  LOG_CLI((BSL_META_U(unit,
                      "block_range.entries_to_act: %u\n\r"),info->block_range.entries_to_act));
    
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}
#endif /* SOC_SAND_DEBUG */

#include <soc/dpp/SAND/Utils/sand_footer.h>
