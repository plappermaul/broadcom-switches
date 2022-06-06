/*
* $Id: vsc256_fabric_cell.c,v 1.7 Broadcom SDK $
*
* $Copyright: (c) 2016 Broadcom.
* Broadcom Proprietary and Confidential. All rights reserved.$
*
* SOC VSC256 FABRIC CELL
*/

#ifdef _ERR_MSG_MODULE_NAME
  #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_SOC_FABRIC
#include <shared/bsl.h>
#include <soc/dcmn/error.h>

#include <soc/dcmn/dcmn_memregs.h>
#include <soc/dcmn/dcmn_error.h>
#include <soc/dcmn/dcmn_defs.h>
#include <soc/dcmn/dcmn_fabric_source_routed_cell.h>
#include <shared/bitop.h>

#include <soc/dcmn/vsc256_fabric_cell.h>

soc_error_t 
soc_vsc256_build_payload(int unit, const vsc256_sr_cell_t* cell, int buf_size_bytes, uint32* buf, int offset)
/* the offset should be 0 in fe1600 and 512 in arad */ 
{
  uint32 tmp_u32_sr_extra_field;
  int i;
  SOCDNX_INIT_FUNC_DEFS;

  if(VSC256_INBAND_PAYLOAD_CELL_FORMAT_START + offset + VSC256_INBAND_PAYLOAD_CELL_FORMAT_LENGTH > 8*buf_size_bytes) {
    SOCDNX_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_SOCDNX_MSG("Buffer size is not large enough")));
  }
  /*cell format*/
  tmp_u32_sr_extra_field = cell->payload.inband.cell_format;
  SHR_BITCOPY_RANGE(buf, VSC256_INBAND_PAYLOAD_CELL_FORMAT_START + offset, &tmp_u32_sr_extra_field, 0, VSC256_INBAND_PAYLOAD_CELL_FORMAT_LENGTH);

  /*cell id*/
  tmp_u32_sr_extra_field = cell->payload.inband.cell_id;
  SHR_BITCOPY_RANGE(buf, VSC256_INBAND_PAYLOAD_CELL_ID_START + offset, &tmp_u32_sr_extra_field, 0, VSC256_INBAND_PAYLOAD_CELL_ID_LENGTH);

  /*cell seq*/
  tmp_u32_sr_extra_field =  cell->payload.inband.seq_num;
  SHR_BITCOPY_RANGE(buf, VSC256_INBAND_PAYLOAD_SEQUENCE_NUMBER_START + offset, &tmp_u32_sr_extra_field, 0, VSC256_INBAND_PAYLOAD_SEQUENCE_NUMBER_LENGTH);

  /*num of commands*/
  tmp_u32_sr_extra_field = cell->payload.inband.nof_commands;
  SHR_BITCOPY_RANGE(buf, VSC256_INBAND_PAYLOAD_NUMBER_OF_COMMANDS_START + offset, &tmp_u32_sr_extra_field, 0, VSC256_INBAND_PAYLOAD_NUMBER_OF_COMMANDS_LENGTH);

  for(i=0 ; i<cell->payload.inband.nof_commands ; i++) {
    /*command type*/
    switch(cell->payload.inband.commands[i].opcode) {
            case soc_dcmn_inband_reg_read:  tmp_u32_sr_extra_field = 2; break;
            case soc_dcmn_inband_reg_write: tmp_u32_sr_extra_field = 3; break;
            case soc_dcmn_inband_mem_read:  tmp_u32_sr_extra_field = 0; break;
            case soc_dcmn_inband_mem_write: tmp_u32_sr_extra_field = 1; break;
            default: SOCDNX_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_SOCDNX_MSG("unsupported opcode"))); 
    }
    SHR_BITCOPY_RANGE(buf, VSC256_INBAND_PAYLOAD_OPCODE_START-i*VSC256_INBAND_PAYLOAD_DIFF + offset, &tmp_u32_sr_extra_field, 0, VSC256_INBAND_PAYLOAD_OPCODE_LENGTH);

    /*block*/
    tmp_u32_sr_extra_field = cell->payload.inband.commands[i].schan_block;
    SHR_BITCOPY_RANGE(buf, VSC256_INBAND_PAYLOAD_BLOCK_START-i*VSC256_INBAND_PAYLOAD_DIFF + offset, &tmp_u32_sr_extra_field, 0, VSC256_INBAND_PAYLOAD_BLOCK_LENGTH);

    /*data size*/
    if(cell->payload.inband.commands[i].length <= 4) {
      tmp_u32_sr_extra_field = 0; /*32 bit*/
    } else if(cell->payload.inband.commands[i].length <= 8) {
      tmp_u32_sr_extra_field = 1; /*64 bit*/
    } else if(cell->payload.inband.commands[i].length <= 12) {
      tmp_u32_sr_extra_field = 2; /*96 bit*/
    } else if(cell->payload.inband.commands[i].length <= 16) {
      tmp_u32_sr_extra_field = 3; /*128 bit*/
    } else {
      SOCDNX_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_SOCDNX_MSG("Can't read more than 128 bits data"))); 
    }
    SHR_BITCOPY_RANGE(buf, VSC256_INBAND_PAYLOAD_LENGTH_START-i*VSC256_INBAND_PAYLOAD_DIFF + offset, &tmp_u32_sr_extra_field, 0, VSC256_INBAND_PAYLOAD_LENGTH_LENGTH);

    /*data*/
    SHR_BITCOPY_RANGE(buf, VSC256_INBAND_PAYLOAD_DATA_START-i*VSC256_INBAND_PAYLOAD_DIFF + 
      ((3 - tmp_u32_sr_extra_field) * 32) /* MSB allignment*/ 
      + offset, cell->payload.inband.commands[i].data, 0, ((tmp_u32_sr_extra_field + 1) * 32));

    /*offset*/
    tmp_u32_sr_extra_field  = cell->payload.inband.commands[i].offset;
    SHR_BITCOPY_RANGE(buf, VSC256_INBAND_PAYLOAD_ADDRESS_START-i*VSC256_INBAND_PAYLOAD_DIFF + offset, &tmp_u32_sr_extra_field, 0, VSC256_INBAND_PAYLOAD_ADDRESS_LENGTH);

    
  }

exit:
  SOCDNX_FUNC_RETURN; 
}

soc_error_t 
soc_vsc256_parse_payload(int unit, const uint32* buf, vsc256_sr_cell_t* cell, int offset)
{
  uint32 val = 0;
  int i;
  SOCDNX_INIT_FUNC_DEFS;

  sal_memset(&cell->payload.inband, 0, sizeof(vsc256_inband_payload_t));/* Cleaning the data */

  /*cell format*/
  SHR_BITCOPY_RANGE(&val, 0, buf, VSC256_INBAND_PAYLOAD_CELL_FORMAT_START + offset, VSC256_INBAND_PAYLOAD_CELL_FORMAT_LENGTH);

  cell->payload.inband.cell_format = val & 0xff;

  /*cell id*/
  SHR_BITCOPY_RANGE(&(cell->payload.inband.cell_id), 0, buf, VSC256_INBAND_PAYLOAD_CELL_ID_START + offset, VSC256_INBAND_PAYLOAD_CELL_ID_LENGTH);

  /*cell seq*/
  SHR_BITCOPY_RANGE(&(cell->payload.inband.seq_num), 0, buf, VSC256_INBAND_PAYLOAD_SEQUENCE_NUMBER_START + offset, VSC256_INBAND_PAYLOAD_SEQUENCE_NUMBER_LENGTH);

  /*num of commands*/
  SHR_BITCOPY_RANGE(&(cell->payload.inband.nof_commands), 0, buf, VSC256_INBAND_PAYLOAD_NUMBER_OF_COMMANDS_START + offset, VSC256_INBAND_PAYLOAD_NUMBER_OF_COMMANDS_LENGTH);
  if(cell->payload.inband.nof_commands > VSC256_INBAND_PAYLOAD_MAX_VALID_COMMANDS) {
    cell->payload.inband.nof_commands = VSC256_INBAND_PAYLOAD_MAX_VALID_COMMANDS;
  }

  for(i=0 ; i<cell->payload.inband.nof_commands ; i++) {
    /*command type*/
    val = 0;
    SHR_BITCOPY_RANGE(&val, 0, buf, VSC256_INBAND_PAYLOAD_OPCODE_START-i*VSC256_INBAND_PAYLOAD_DIFF + offset, VSC256_INBAND_PAYLOAD_OPCODE_LENGTH);
    val &= (0xffffffffL) >> (32 - VSC256_INBAND_PAYLOAD_OPCODE_LENGTH);
    switch(val) {
          case 0:  cell->payload.inband.commands[i].opcode = soc_dcmn_inband_mem_read; break;
          case 1: cell->payload.inband.commands[i].opcode = soc_dcmn_inband_mem_write; break;
          case 2:  cell->payload.inband.commands[i].opcode = soc_dcmn_inband_reg_read; break;
          case 3: cell->payload.inband.commands[i].opcode = soc_dcmn_inband_reg_write; break;
          /* must default. Otherwise - compilation error */
          /* coverity[dead_error_begin:FALSE] */
          default: SOCDNX_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_SOCDNX_MSG("unsupported opcode"))); 
    }

    /*block*/
    val = 0;
    SHR_BITCOPY_RANGE(&val, 0, buf, VSC256_INBAND_PAYLOAD_BLOCK_START-i*VSC256_INBAND_PAYLOAD_DIFF + offset, VSC256_INBAND_PAYLOAD_BLOCK_LENGTH);
    cell->payload.inband.commands[i].schan_block = val;

    /*data size*/
    val = 0;
    SHR_BITCOPY_RANGE(&val, 0, buf, VSC256_INBAND_PAYLOAD_LENGTH_START-i*VSC256_INBAND_PAYLOAD_DIFF + offset, VSC256_INBAND_PAYLOAD_LENGTH_LENGTH);
    val &= (0xffffffffL) >> (32 - VSC256_INBAND_PAYLOAD_LENGTH_LENGTH);
    cell->payload.inband.commands[i].length = (val+1)*4;

    /*data*/
    SHR_BITCOPY_RANGE(cell->payload.inband.commands[i].data, 0, buf, VSC256_INBAND_PAYLOAD_DATA_START-i*VSC256_INBAND_PAYLOAD_DIFF 
      + ((3-val)*32)/* MSB allignment*/
      + offset, ((val+1)*32) );

    /*offset*/
    SHR_BITCOPY_RANGE(&(cell->payload.inband.commands[i].offset), 0, buf, VSC256_INBAND_PAYLOAD_ADDRESS_START-i*VSC256_INBAND_PAYLOAD_DIFF + offset, VSC256_INBAND_PAYLOAD_ADDRESS_LENGTH);

    
  }

exit:
  SOCDNX_FUNC_RETURN; 
}

soc_error_t 
soc_vsc256_build_header(int unit, const vsc256_sr_cell_t* cell, int buf_size_bytes, uint32* buf) 
{
  soc_dcmn_device_type_actual_value_t tmp_actual_value;
  uint32 tmp_u32_data_cell_field;
  SOCDNX_INIT_FUNC_DEFS;

  /* We assume that the buffer is initialized, and its size is (at least) VSC256_SR_DATA_CELL_HEADER_SIZE, and it points to the begin of the header. */

  if(buf_size_bytes < VSC256_SR_DATA_CELL_HEADER_SIZE) {
    SOCDNX_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_SOCDNX_MSG("SR header minimum buffer size is %d"),VSC256_SR_DATA_CELL_HEADER_SIZE));
  }

  if(cell->header.is_inband && VSC256_CELL_FORMAT_FE600 == cell->payload.inband.cell_format) {
    SOCDNX_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_SOCDNX_MSG("VSC256 can't be sent in FE600 format")));
  }

  /*
  * bits - Copy of the cell type
  */
  tmp_u32_data_cell_field = cell->header.cell_type;
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_CELL_TYPE_START, &(tmp_u32_data_cell_field), 0,  VSC256_SR_DATA_CELL_CELL_TYPE_LENGTH);

  /*
   * bits  - source id
   */
  tmp_u32_data_cell_field = cell->header.source_device;
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_SOURCE_ID_START,&(tmp_u32_data_cell_field), 0,  VSC256_SR_DATA_CELL_SOURCE_ID_LENGTH);


  /*
   * bits  - source level
   */
  SOCDNX_IF_ERR_EXIT(soc_dcmn_actual_entity_value(unit, cell->header.source_level, &tmp_actual_value));
  tmp_u32_data_cell_field = (uint32)tmp_actual_value;
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_SRC_LEVEL_START,&(tmp_u32_data_cell_field), 0,  VSC256_SR_DATA_CELL_SRC_LEVEL_LENGTH);

  /*
   * bits  - destination level
   */
  SOCDNX_IF_ERR_EXIT(soc_dcmn_actual_entity_value(unit, cell->header.destination_level, &tmp_actual_value));
  tmp_u32_data_cell_field = (uint32)tmp_actual_value;
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_DEST_LEVEL_START, &(tmp_u32_data_cell_field), 0,  VSC256_SR_DATA_CELL_DEST_LEVEL_LENGTH);

  /*
   * bits  - fip switch
   */
  tmp_u32_data_cell_field = cell->header.fip_link;
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_FIP_SWITCH_START, &(tmp_u32_data_cell_field), 0,  VSC256_SR_DATA_CELL_FIP_SWITCH_LENGTH);
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_FIP_SWITCH_POSITION_1, &(tmp_u32_data_cell_field), VSC256_SR_DATA_CELL_FIP_SWITCH_START_1,  VSC256_SR_DATA_CELL_FIP_SWITCH_LENGTH_1);

  /*
   * bits  - fe1 switch
   */
  tmp_u32_data_cell_field = cell->header.fe1_link;
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_FE1_SWITCH_START, &(tmp_u32_data_cell_field), 0,  VSC256_SR_DATA_CELL_FE1_SWITCH_LENGTH);
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_FE1_SWITCH_POSITION_1, &(tmp_u32_data_cell_field), VSC256_SR_DATA_CELL_FE1_SWITCH_START_1,  VSC256_SR_DATA_CELL_FE1_SWITCH_LENGTH_1);
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_FE1_SWITCH_POSITION_2, &(tmp_u32_data_cell_field), VSC256_SR_DATA_CELL_FE1_SWITCH_START_2,  VSC256_SR_DATA_CELL_FE1_SWITCH_LENGTH_2);

  /*
   * bits  - fe2 switch
   */
  tmp_u32_data_cell_field = cell->header.fe2_link;
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_FE2_SWITCH_START, &(tmp_u32_data_cell_field), 0,  VSC256_SR_DATA_CELL_FE2_SWITCH_LENGTH);
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_FE2_SWITCH_POSITION_1, &(tmp_u32_data_cell_field), VSC256_SR_DATA_CELL_FE2_SWITCH_START_1,  VSC256_SR_DATA_CELL_FE2_SWITCH_LENGTH_1);
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_FE2_SWITCH_POSITION_2, &(tmp_u32_data_cell_field), VSC256_SR_DATA_CELL_FE2_SWITCH_START_2,  VSC256_SR_DATA_CELL_FE2_SWITCH_LENGTH_2);

  /*
   * bits  - fe3 switch
   */
  tmp_u32_data_cell_field = cell->header.fe3_link;
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_FE3_SWITCH_START, &(tmp_u32_data_cell_field), 0,  VSC256_SR_DATA_CELL_FE3_SWITCH_LENGTH);
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_FE3_SWITCH_POSITION_1, &(tmp_u32_data_cell_field), VSC256_SR_DATA_CELL_FE3_SWITCH_START_1,  VSC256_SR_DATA_CELL_FE3_SWITCH_LENGTH_1);
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_FE3_SWITCH_POSITION_2, &(tmp_u32_data_cell_field), VSC256_SR_DATA_CELL_FE3_SWITCH_START_2,  VSC256_SR_DATA_CELL_FE3_SWITCH_LENGTH_2);

  /*
   * bit - In-band cell
   */
  tmp_u32_data_cell_field = cell->header.is_inband;
  SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_INBAND_CELL_POSITION, &(tmp_u32_data_cell_field), 0,  VSC256_SR_DATA_CELL_INBAND_CELL_LENGTH);

  /*
   * bits - pipe index
   */
  if (cell->header.pipe_id != -1)
  {
      tmp_u32_data_cell_field = 0x1;
      SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_PIPE_ID_ENABLE_POSITION, &(tmp_u32_data_cell_field), 0,  VSC256_SR_DATA_CELL_PIPE_ID_ENABLE_LENGTH);
      tmp_u32_data_cell_field = cell->header.pipe_id;
      SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_PIPE_ID_POSITION, &(tmp_u32_data_cell_field), 0,  VSC256_SR_DATA_CELL_PIPE_ID_LENGTH);
  } else {
      tmp_u32_data_cell_field = 0x0;
      SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_PIPE_ID_ENABLE_POSITION, &(tmp_u32_data_cell_field), 0,  VSC256_SR_DATA_CELL_PIPE_ID_ENABLE_LENGTH);
      tmp_u32_data_cell_field = 0x0;
      SHR_BITCOPY_RANGE(buf, VSC256_SR_DATA_CELL_PIPE_ID_POSITION, &(tmp_u32_data_cell_field), 0,  VSC256_SR_DATA_CELL_PIPE_ID_LENGTH);
  }

exit:
  SOCDNX_FUNC_RETURN;  
}

soc_error_t soc_vsc256_parse_header(int unit, soc_reg_above_64_val_t reg_val, vsc256_sr_cell_t* cell)
{
  soc_dcmn_device_type_actual_value_t tmp_actual_value = 0;
  uint32 val;
  SOCDNX_INIT_FUNC_DEFS;

  /* We assume that the data is in bits 0-88 of the register. */

  sal_memset(&cell->header, 0, sizeof(vsc256_sr_cell_header_t));
  /*
  * bits - Copy of the cell type
  */
  SHR_BITCOPY_RANGE(&cell->header.cell_type, 0, reg_val, VSC256_SR_DATA_CELL_CELL_TYPE_START, VSC256_SR_DATA_CELL_CELL_TYPE_LENGTH);

  /*
   * bits  - source id
   */
  SHR_BITCOPY_RANGE(&cell->header.source_device, 0, reg_val, VSC256_SR_DATA_CELL_SOURCE_ID_START, VSC256_SR_DATA_CELL_SOURCE_ID_LENGTH);


  /*
   * bits  - source level
   */
  SHR_BITCOPY_RANGE(&tmp_actual_value, 0, reg_val, VSC256_SR_DATA_CELL_SRC_LEVEL_START, VSC256_SR_DATA_CELL_SRC_LEVEL_LENGTH);
  SOCDNX_IF_ERR_EXIT(soc_dcmn_device_entity_value(unit, tmp_actual_value, &cell->header.source_level));
  
  /*
   * bits  - destination level
   */
  tmp_actual_value = 0;
  SHR_BITCOPY_RANGE(&tmp_actual_value, 0, reg_val, VSC256_SR_DATA_CELL_DEST_LEVEL_START, VSC256_SR_DATA_CELL_DEST_LEVEL_LENGTH);
  SOCDNX_IF_ERR_EXIT(soc_dcmn_device_entity_value(unit, tmp_actual_value, &cell->header.destination_level));
  
  /*
   * bits  - fip switch
   */
  val = 0;
  SHR_BITCOPY_RANGE(&val, 0, reg_val, VSC256_SR_DATA_CELL_FIP_SWITCH_START, VSC256_SR_DATA_CELL_FIP_SWITCH_LENGTH);
  SHR_BITCOPY_RANGE(&val, VSC256_SR_DATA_CELL_FIP_SWITCH_START_1, reg_val, VSC256_SR_DATA_CELL_FIP_SWITCH_POSITION_1, VSC256_SR_DATA_CELL_FIP_SWITCH_LENGTH_1);
  cell->header.fip_link = val;
  /*
   * bits  - fe1 switch
   */
  val = 0;
  SHR_BITCOPY_RANGE(&val, 0, reg_val, VSC256_SR_DATA_CELL_FE1_SWITCH_START, VSC256_SR_DATA_CELL_FE1_SWITCH_LENGTH);
  SHR_BITCOPY_RANGE(&val, VSC256_SR_DATA_CELL_FE1_SWITCH_START_1, reg_val, VSC256_SR_DATA_CELL_FE1_SWITCH_POSITION_1, VSC256_SR_DATA_CELL_FE1_SWITCH_LENGTH_1);
  SHR_BITCOPY_RANGE(&val, VSC256_SR_DATA_CELL_FE1_SWITCH_START_2, reg_val, VSC256_SR_DATA_CELL_FE1_SWITCH_POSITION_2, VSC256_SR_DATA_CELL_FE1_SWITCH_LENGTH_2);
  cell->header.fe1_link = val;

  /*
   * bits  - fe2 switch
   */
  val = 0;
  SHR_BITCOPY_RANGE(&val, 0, reg_val, VSC256_SR_DATA_CELL_FE2_SWITCH_START, VSC256_SR_DATA_CELL_FE2_SWITCH_LENGTH);
  SHR_BITCOPY_RANGE(&val, VSC256_SR_DATA_CELL_FE2_SWITCH_START_1, reg_val, VSC256_SR_DATA_CELL_FE2_SWITCH_POSITION_1, VSC256_SR_DATA_CELL_FE2_SWITCH_LENGTH_1);
  SHR_BITCOPY_RANGE(&val, VSC256_SR_DATA_CELL_FE2_SWITCH_START_2, reg_val, VSC256_SR_DATA_CELL_FE2_SWITCH_POSITION_2, VSC256_SR_DATA_CELL_FE2_SWITCH_LENGTH_2);
  cell->header.fe2_link = val;

  /*
   * bits  - fe3 switch
   */
  val = 0;
  SHR_BITCOPY_RANGE(&val, 0, reg_val, VSC256_SR_DATA_CELL_FE3_SWITCH_START, VSC256_SR_DATA_CELL_FE3_SWITCH_LENGTH);
  SHR_BITCOPY_RANGE(&val, VSC256_SR_DATA_CELL_FE3_SWITCH_START_1, reg_val, VSC256_SR_DATA_CELL_FE3_SWITCH_POSITION_1, VSC256_SR_DATA_CELL_FE3_SWITCH_LENGTH_1);
  SHR_BITCOPY_RANGE(&val, VSC256_SR_DATA_CELL_FE3_SWITCH_START_2, reg_val, VSC256_SR_DATA_CELL_FE3_SWITCH_POSITION_2, VSC256_SR_DATA_CELL_FE3_SWITCH_LENGTH_2);
  cell->header.fe3_link = val;

  /*
   * bit - In-band cell
   */
  SHR_BITCOPY_RANGE(&cell->header.is_inband, 0, reg_val, VSC256_SR_DATA_CELL_INBAND_CELL_POSITION, VSC256_SR_DATA_CELL_INBAND_CELL_LENGTH);

  /*
   * bits - pipe index
   */
  val = 0;
  SHR_BITCOPY_RANGE(&val, 0, reg_val, VSC256_SR_DATA_CELL_PIPE_ID_ENABLE_POSITION, VSC256_SR_DATA_CELL_PIPE_ID_ENABLE_LENGTH);
  if (val)
  {
      val = 0;
      SHR_BITCOPY_RANGE(&val, 0, reg_val, VSC256_SR_DATA_CELL_PIPE_ID_POSITION, VSC256_SR_DATA_CELL_PIPE_ID_LENGTH);
      cell->header.pipe_id = val;
  }

  
exit:
  SOCDNX_FUNC_RETURN;  
}

/*
 * Function: 
 *      soc_vsc256_to_vsc128_build_header 
 * Role: 
 *      This function get vsc256_sr_cell_t as an input and creats a buffer with the equivalent VSC128 header. 
 * Remarks: 
 *      Implemented and tested just for Source routed cell. Should be tuned for inband cells too. 
 */
soc_error_t 
soc_vsc256_to_vsc128_build_header(int unit, const vsc256_sr_cell_t* cell, int buf_size_bytes, uint32* buf) 
{
  soc_dcmn_device_type_actual_value_t tmp_actual_value;
  uint32 tmp_u32_data_cell_field;
  SOCDNX_INIT_FUNC_DEFS;

    /*
     * Copy of the cell type
     */
    tmp_u32_data_cell_field = cell->header.cell_type;
    SHR_BITCOPY_RANGE(buf, VSC256_TO_VSC128_DATA_CELL_CELL_TYPE_START, &(tmp_u32_data_cell_field), 0, VSC256_TO_VSC128_DATA_CELL_CELL_TYPE_LENGTH);  

    /*
     * Copy source id
     */
    tmp_u32_data_cell_field = cell->header.source_device;
    SHR_BITCOPY_RANGE(buf, VSC256_TO_VSC128_DATA_CELL_SOURCE_ID_START, &(tmp_u32_data_cell_field), 0, VSC256_TO_VSC128_DATA_CELL_SOURCE_ID_LENGTH);  


    /*
     * Copy source level
     */
    SOCDNX_IF_ERR_EXIT(soc_dcmn_actual_entity_value(unit,cell->header.source_level, &tmp_actual_value));
    tmp_u32_data_cell_field = (uint32)tmp_actual_value;
    SHR_BITCOPY_RANGE(buf, VSC256_TO_VSC128_DATA_CELL_SRC_LEVEL_START, &(tmp_u32_data_cell_field), 0, VSC256_TO_VSC128_DATA_CELL_SRC_LEVEL_LENGTH);

    /*
     * Copy destination level
     */
    SOCDNX_IF_ERR_EXIT(soc_dcmn_actual_entity_value(unit, cell->header.destination_level, &tmp_actual_value));
    tmp_u32_data_cell_field = (uint32)tmp_actual_value;
    SHR_BITCOPY_RANGE(buf, VSC256_TO_VSC128_DATA_CELL_DEST_LEVEL_START, &(tmp_u32_data_cell_field), 0, VSC256_TO_VSC128_DATA_CELL_DEST_LEVEL_LENGTH);

    /*
     * Copy fip switch
     */
    tmp_u32_data_cell_field = cell->header.fip_link;
    SHR_BITCOPY_RANGE(buf, VSC256_TO_VSC128_DATA_CELL_FIP_SWITCH_START, &(tmp_u32_data_cell_field), 0,  VSC256_TO_VSC128_DATA_CELL_FIP_SWITCH_LENGTH);
    SHR_BITCOPY_RANGE(buf, VSC256_TO_VSC128_SR_DATA_CELL_FIP_SWITCH_POSITION, &(tmp_u32_data_cell_field), VSC256_TO_VSC128_DATA_CELL_FIP_SWITCH_LENGTH,  VSC256_TO_VSC128_SR_DATA_CELL_FIP_SWITCH_LENGTH);

    /*
     * Copy fe1 switch
     */
    tmp_u32_data_cell_field = cell->header.fe1_link;
    SHR_BITCOPY_RANGE(buf, VSC256_TO_VSC128_DATA_CELL_FE1_SWITCH_START, &(tmp_u32_data_cell_field), 0,  VSC256_TO_VSC128_DATA_CELL_FE1_SWITCH_LENGTH);
    SHR_BITCOPY_RANGE(buf, VSC256_TO_VSC128_SR_DATA_CELL_FE1_SWITCH_POSITION, &(tmp_u32_data_cell_field), VSC256_TO_VSC128_DATA_CELL_FE1_SWITCH_LENGTH,  VSC256_TO_VSC128_SR_DATA_CELL_FE1_SWITCH_LENGTH);

    /*
     * Copy fe2 switch
     */
    tmp_u32_data_cell_field = cell->header.fe2_link;
    SHR_BITCOPY_RANGE(buf, VSC256_TO_VSC128_DATA_CELL_FE2_SWITCH_START, &(tmp_u32_data_cell_field), 0,  VSC256_TO_VSC128_DATA_CELL_FE2_SWITCH_LENGTH);
    SHR_BITCOPY_RANGE(buf, VSC256_TO_VSC128_SR_DATA_CELL_FE2_SWITCH_POSITION, &(tmp_u32_data_cell_field), VSC256_TO_VSC128_DATA_CELL_FE2_SWITCH_LENGTH,  VSC256_TO_VSC128_SR_DATA_CELL_FE2_SWITCH_LENGTH);

    /*
     * Copy fe3 switch
     */
    tmp_u32_data_cell_field = cell->header.fe3_link;
    SHR_BITCOPY_RANGE(buf, VSC256_TO_VSC128_DATA_CELL_FE3_SWITCH_START, &(tmp_u32_data_cell_field), 0,  VSC256_TO_VSC128_DATA_CELL_FE3_SWITCH_LENGTH);
    SHR_BITCOPY_RANGE(buf, VSC256_TO_VSC128_SR_DATA_CELL_FE3_SWITCH_POSITION, &(tmp_u32_data_cell_field), VSC256_TO_VSC128_DATA_CELL_FE3_SWITCH_LENGTH,  VSC256_TO_VSC128_SR_DATA_CELL_FE3_SWITCH_LENGTH);

    /*
    * Copy In-band cell
    */
    tmp_u32_data_cell_field = cell->header.is_inband;
    SHR_BITCOPY_RANGE(buf, VSC256_TO_VSC128_SR_DATA_CELL_INBAND_CELL_POSITION, &(tmp_u32_data_cell_field), 0,  VSC256_TO_VSC128_SR_DATA_CELL_INBAND_CELL_LENGTH);
exit:    
  SOCDNX_FUNC_RETURN;  
}


#undef _ERR_MSG_MODULE_NAME
