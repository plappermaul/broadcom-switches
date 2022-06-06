/* $Id: chip_sim_em.h,v 1.6 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/


#ifndef __ARAD_SIM_EM_H__
/* { */
#define __ARAD_SIM_EM_H__

#include <soc/dpp/ARAD/arad_sim.h>
#include <soc/dpp/SAND/Utils/sand_multi_set.h>
#include <soc/dpp/SAND/Utils/sand_integer_arithmetic.h>
#include <soc/dpp/PPD/ppd_api_frwrd_mact.h>
#include <soc/dpp/ARAD/arad_sim_em_block.h>


#include <soc/dpp/ARAD/ARAD_PP/arad_pp_framework.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_esem_access.h>

/*
 * INCLUDE FILES:
 * {
 */

typedef enum
{
  SOC_PB_EM_TABLE_ISEM=0,
  SOC_PB_EM_TABLE_ESEM=1,
  SOC_PB_EM_TABLE_SVEM=2,
  SOC_PB_EM_TABLE_LEM=3,
  SOC_PB_EM_TABLE_ID_LAST = 4,
  ARAD_EM_TABLE_LEM=0,
  ARAD_EM_TABLE_ISEM_A=1,
  ARAD_EM_TABLE_ISEM_B=2,
  ARAD_EM_TABLE_ESEM=3,
  ARAD_EM_TABLE_RMAPEM=4,
  ARAD_EM_TABLE_OAMEM_A=5,
  ARAD_EM_TABLE_OAMEM_B=6,
  ARAD_EM_TABLE_GLEM=7,
  ARAD_EM_TABLE_LEM_CR=8,
  ARAD_EM_TABLE_ID_LAST = 9
}CHIP_SIM_EM_TABLE_TYPE;

/* from soc_pb_pp_chip_tbls.c */
#define CHIP_SIM_ISEM_ADDR_BASE 0x00900000

#define CHIP_SIM_ISEM_KEY_SIZE  ((46 - 13 + 1) + SOC_SAND_NOF_BITS_IN_BYTE - 1)/SOC_SAND_NOF_BITS_IN_BYTE
#define CHIP_SIM_ISEM_DATA_SIZE ((62 - 47 + 1) + SOC_SAND_NOF_BITS_IN_BYTE - 1)/SOC_SAND_NOF_BITS_IN_BYTE
#define CHIP_SIM_ISEM_TABLE_SIZE (16 * 1024)


#define CHIP_SIM_ESEM_ADDR_BASE SOC_PB_PP_ESEM_ADDR
#define CHIP_SIM_ESEM_KEY_SIZE  ((31 - 11 + 1) + SOC_SAND_NOF_BITS_IN_BYTE - 1)/SOC_SAND_NOF_BITS_IN_BYTE
#define CHIP_SIM_ESEM_DATA_SIZE ((46 - 32 + 1) + SOC_SAND_NOF_BITS_IN_BYTE - 1)/SOC_SAND_NOF_BITS_IN_BYTE
#define CHIP_SIM_ESEM_TABLE_SIZE (16 * 1024)  

#define CHIP_SIM_SVEM_ADDR_BASE SOC_PB_PP_SVEM_ADDR
#define CHIP_SIM_SVEM_KEY_SIZE  ((26 - 11 + 1) + SOC_SAND_NOF_BITS_IN_BYTE - 1)/SOC_SAND_NOF_BITS_IN_BYTE
#define CHIP_SIM_SVEM_DATA_SIZE ((38 - 27 + 1) + SOC_SAND_NOF_BITS_IN_BYTE - 1)/SOC_SAND_NOF_BITS_IN_BYTE
#define CHIP_SIM_SVEM_TABLE_SIZE (16 * 1024)  

#define CHIP_SIM_LEM_ADDR_BASE    0x11111111 /* Dummy */
#define CHIP_SIM_LEM_KEY_SIZE(unit) (SOC_DPP_DEFS_GET(unit, lem_width) + SOC_SAND_NOF_BITS_IN_BYTE - 1)/SOC_SAND_NOF_BITS_IN_BYTE
#define CHIP_SIM_LEM_KEY_SIZE_MAX   (SOC_DPP_DEFS_MAX(LEM_WIDTH) + SOC_SAND_NOF_BITS_IN_BYTE - 1)/SOC_SAND_NOF_BITS_IN_BYTE
#define CHIP_SIM_LEM_DATA_SIZE(unit)\
    ((ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_BITS + SOC_SAND_NOF_BITS_IN_BYTE - 1)/SOC_SAND_NOF_BITS_IN_BYTE)
#define CHIP_SIM_LEM_DATA_SIZE_MAX  (ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_BITS + SOC_SAND_NOF_BITS_IN_BYTE - 1)/SOC_SAND_NOF_BITS_IN_BYTE
#define CHIP_SIM_LEM_TABLE_SIZE   (SOC_PB_PP_FRWRD_MACT_TABLE_NOF_ENTRIES)  

#define SOC_PB_CHIP_SIM_TABLE_KEY_MAX_SIZE       SOC_SAND_MAX(SOC_SAND_MAX(CHIP_SIM_SVEM_KEY_SIZE, CHIP_SIM_LEM_KEY_SIZE_MAX), SOC_SAND_MAX(CHIP_SIM_ISEM_KEY_SIZE, CHIP_SIM_ESEM_KEY_SIZE))
#define SOC_PB_CHIP_SIM_TABLE_DATA_MAX_SIZE      SOC_SAND_MAX(SOC_SAND_MAX(CHIP_SIM_SVEM_DATA_SIZE, CHIP_SIM_LEM_DATA_SIZE_MAX), SOC_SAND_MAX(CHIP_SIM_ISEM_DATA_SIZE, CHIP_SIM_ESEM_DATA_SIZE))


/* arad */
#define ARAD_CHIP_SIM_LEM_KEY  10
#define ARAD_CHIP_SIM_LEM_PAYLOAD  6
#define ARAD_CHIP_SIM_LEM_BASE  0x1111111  /* dummy */
#define ARAD_CHIP_SIM_LEM_TABLE_SIZE  (((SOC_IS_JERICHO_A0(unit)) || (SOC_IS_JERICHO_B0(unit)))? (2048*1024) : SOC_DPP_DEFS_GET(unit, nof_lem_lines))
#define ARAD_CHIP_SIM_ISEM_A_KEY  6
#define ARAD_CHIP_SIM_ISEM_A_PAYLOAD  2
#define ARAD_CHIP_SIM_ISEM_A_BASE  0x7A0000
#define ARAD_CHIP_SIM_ISEM_A_TABLE_SIZE  (((SOC_IS_JERICHO_A0(unit)) || (SOC_IS_JERICHO_B0(unit)))? 65536 : 32768)
#define ARAD_CHIP_SIM_ISEM_B_KEY  6
#define ARAD_CHIP_SIM_ISEM_B_PAYLOAD  2
#define ARAD_CHIP_SIM_ISEM_B_BASE  0xA10000
#define ARAD_CHIP_SIM_ISEM_B_TABLE_SIZE  32768
#define ARAD_CHIP_SIM_ESEM_KEY  8
#define ARAD_CHIP_SIM_ESEM_PAYLOAD  4
#define ARAD_CHIP_SIM_ESEM_BASE  0x520000
#define ARAD_CHIP_SIM_ESEM_TABLE_SIZE  (((SOC_IS_JERICHO_A0(unit)) || (SOC_IS_JERICHO_B0(unit)))? 65535 : 16384)
#define ARAD_CHIP_SIM_RMAPEM_KEY  4
#define ARAD_CHIP_SIM_RMAPEM_PAYLOAD  4
#define ARAD_CHIP_SIM_RMAPEM_BASE  0x100000
#define ARAD_CHIP_SIM_RMAPEM_TABLE_SIZE  (((SOC_IS_JERICHO_A0(unit)) || (SOC_IS_JERICHO_B0(unit)))? 32768 : 16384)
#define ARAD_CHIP_SIM_OEMA_KEY  4
#define ARAD_CHIP_SIM_OEMA_PAYLOAD  5
#define ARAD_CHIP_SIM_OEMA_BASE  0xEA0000
#define ARAD_CHIP_SIM_OEMA_TABLE_SIZE  (((SOC_IS_JERICHO_A0(unit)) || (SOC_IS_JERICHO_B0(unit)))? 32768 : 16384)

#define ARAD_CHIP_SIM_OEMB_KEY  4
#define ARAD_CHIP_SIM_OEMB_PAYLOAD  4
#define ARAD_CHIP_SIM_OEMB_BASE  0x14A0000
#define ARAD_CHIP_SIM_OEMB_TABLE_SIZE  16384

#define ARAD_CHIP_SIM_GLEM_KEY  sizeof(int)
#define ARAD_CHIP_SIM_GLEM_PAYLOAD  sizeof(int)
#define ARAD_CHIP_SIM_GLEM_BASE  0x1800000
#define ARAD_CHIP_SIM_GLEM_TABLE_SIZE  SOC_DPP_DEFS_GET(unit, nof_glem_lines)

/* LEM transaction shadow for crash recovery support */
#define ARAD_CHIP_SIM_LEM_CR_BASE  0x2222222  /* dummy */
#define ARAD_CHIP_SIM_LEM_CR_TABLE_SIZE 50
#define ARAD_CHIP_SIM_LEM_CR_DELETED_ENTRY_INDICATION_BIT   47

#define ARAD_CHIP_SIM_TABLE_KEY_MAX_SIZE       (ARAD_CHIP_SIM_LEM_KEY)
#define ARAD_CHIP_SIM_TABLE_DATA_MAX_SIZE      (ARAD_CHIP_SIM_LEM_PAYLOAD)

#define CHIP_SIM_TABLE_KEY_MAX_SIZE       SOC_SAND_MAX(ARAD_CHIP_SIM_TABLE_KEY_MAX_SIZE,/*SOC_PB_CHIP_SIM_TABLE_DATA_MAX_SIZE*/0)
#define CHIP_SIM_TABLE_DATA_MAX_SIZE      SOC_SAND_MAX(ARAD_CHIP_SIM_TABLE_DATA_MAX_SIZE,/*SOC_PB_CHIP_SIM_TABLE_DATA_MAX_SIZE*/0)


uint32 
chip_sim_em_offset_to_table_id (
               SOC_SAND_IN   uint32     unit,
               SOC_SAND_IN   uint32     offset,
               SOC_SAND_OUT  uint32     *id
                );
uint32
chip_sim_exact_match_entry_add_unsafe(
              SOC_SAND_IN  int         unit,
              SOC_SAND_IN  uint32      offset, /* ISEM,LEM,ESEM,SVEM */
              SOC_SAND_IN  uint32      *key_ptr,
              SOC_SAND_IN  uint32     key_size,
              SOC_SAND_IN  uint32      *data_ptr,
              SOC_SAND_IN  uint32     data_size,
              SOC_SAND_OUT uint8     *success
              );
uint32
chip_sim_exact_match_entry_get_unsafe(
              SOC_SAND_IN  int         unit,
              SOC_SAND_IN  uint32      offset,
              SOC_SAND_IN  uint32      *key_ptr,
              SOC_SAND_IN  uint32     key_size,
              SOC_SAND_OUT  uint32     *data_ptr,
              SOC_SAND_IN  uint32     data_size,
              SOC_SAND_OUT  uint8    *found
              );
uint32
soc_sand_exact_match_entry_get_by_index_unsafe(
              SOC_SAND_IN  int         unit,
              SOC_SAND_IN  uint32      tbl_offset,
              SOC_SAND_IN  uint32      entry_offset,
              SOC_SAND_OUT  uint32      *key_ptr,
              SOC_SAND_IN  uint32     key_size,
              SOC_SAND_OUT  uint32     *data_ptr,
              SOC_SAND_IN  uint32     data_size,
              SOC_SAND_OUT  uint8    *found
              );
uint32
chip_sim_exact_match_entry_remove_unsafe(
              SOC_SAND_IN  int         unit,
              SOC_SAND_IN  uint32      offset,
              SOC_SAND_IN  uint32      *key_ptr,
              SOC_SAND_IN  uint32     key_size
              );

void chip_sim_em_init_chip_specifics(uint32 device_id, SOC_SAND_DEVICE_TYPE device_type, CHIP_SIM_EM_BLOCK* indirect_blocks);
uint32 chip_sim_em_malloc(uint32 device_id, CHIP_SIM_EM_BLOCK* indirect_blocks, int block_cnt);

uint32 chip_sim_em_init(   int                  unit, 
                           SOC_SAND_DEVICE_TYPE device_type,
                           int                  is_lem_shadow_supported,
                           int                  is_cr_supported);

uint32
chip_sim_em_get_block(
   SOC_SAND_IN       int         unit,
   SOC_SAND_IN       uint32      tbl_offset,
   SOC_SAND_IN       uint32     key_size,
   SOC_SAND_IN       uint32     data_size,
   SOC_SAND_IN       void*      *filter_data,
   SOC_SAND_OUT      uint32     *key_array,
   SOC_SAND_OUT      uint32     *data_array,
   SOC_SAND_OUT      uint32     *cnt,
   SOC_SAND_INOUT    SOC_SAND_TABLE_BLOCK_RANGE  *block_range);

uint32
  chip_sim_em_match_rule_mac(
    SOC_SAND_IN  int                                         unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_MACT_TRAVERSE_MATCH_RULE      *rule,
    SOC_SAND_IN  SOC_PPC_FRWRD_MACT_ENTRY_KEY                *mac_key, 
    SOC_SAND_IN  SOC_PPC_FRWRD_MACT_ENTRY_VALUE              *mac_entry_value,
    SOC_SAND_OUT uint32                                      *match);

/* 
 * this function is used to delete all entries in an EM table 
 * tbl_offset - is the base of the table e.g. SEM/LEM etc. 
 * prefix - is the prefix of the specific table in the specified base 
 *          supply -1 as the value if you want to remove all entries in this base
 *          supply the prefix to match if you want to delete only entries that match
 *          a specific prefix
 */
uint32
  chip_sim_em_delete_all(
    SOC_SAND_IN       int        unit,
    SOC_SAND_IN       uint32     tbl_offset, /*base*/
    SOC_SAND_IN       uint32     key_size,
    SOC_SAND_IN       uint32     key_width,
    SOC_SAND_IN       uint32     data_size,
    SOC_SAND_IN       uint32     prefix,
    SOC_SAND_IN       uint32     prefix_size);

int cmodel_memreg_access(int unit,
                        int cmic_block,
                        uint32 addr,
                        uint32 dwc_read,
                        int is_mem,
                        int is_get,
                        uint32 *data);

#ifdef CRASH_RECOVERY_SUPPORT
uint32 chip_sim_exact_match_cr_shadow_clear(SOC_SAND_IN int unit);
#endif /*CRASH_RECOVERY_SUPPORT*/

/* } __ARAD_SIM_EM_H__*/
#endif
