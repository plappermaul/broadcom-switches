/*
 * $Id: ramon_fe1600_fabric_cell.c,v 1.16 Broadcom SDK $
 *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * SOC RAMON_FE1600 FABRIC CELL
 */

#ifdef _ERR_MSG_MODULE_NAME
  #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_SOC_FABRIC


#include <shared/bsl.h>

#include <soc/mcm/memregs.h>
#include <soc/error.h>
#include <soc/dnxf/cmn/dnxf_drv.h>
#include <shared/bitop.h>

#if defined(BCM_88790_A0)

#include <soc/dnxc/legacy/error.h>
#include <soc/dnxc/legacy/dnxc_fabric_cell.h>
#include <soc/dnxc/legacy/dnxc_fabric_source_routed_cell.h>
#include <soc/dnxc/legacy/dnxc_captured_buffer.h>
#include <soc/dnxc/legacy/dnxc_cells_buffer.h>

#include <soc/dnxf/cmn/dnxf_multicast.h>
#include <soc/dnxf/cmn/dnxf_fabric_source_routed_cell.h>

#include <soc/dnxf/fe1600/fe1600_defs.h>
#include <soc/dnxf/fe1600/fe1600_fabric_cell.h>
#include <soc/dnxf/fe1600/fe1600_fabric_links.h>



STATIC soc_error_t 
soc_ramon_fe1600_sr_cell_format(int unit, const dnxc_vsc256_sr_cell_t* cell, int buf_size_bytes, uint32* buf) 
{
    soc_dnxc_device_type_actual_value_t tmp_actual_value;
    soc_dnxf_fabric_link_cell_size_t vsc_format;
    soc_error_t rc;
    uint32 tmp_u32_data_cell_field;
    DNXC_INIT_FUNC_DEFS;

    sal_memset(buf, 0, buf_size_bytes);

    if(buf_size_bytes < RAMON_FE1600_DATA_CELL_BYTE_SIZE) {
        DNXC_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_DNXC_MSG("SR cell minimum buffer size is %d"),RAMON_FE1600_DATA_CELL_BYTE_SIZE));
    }

    rc = soc_dnxf_sr_cell_format_type_get(unit, cell, &vsc_format);
    DNXC_IF_ERR_EXIT(rc);

    if((vsc_format == soc_dnxf_fabric_link_cell_size_VSC128)) {
         /*
         * bits 512 + 87:86 - Copy of the cell type
         */
        tmp_u32_data_cell_field = cell->header.cell_type;
        SHR_BITCOPY_RANGE(buf, RAMON_FE1600_VSC128_DATA_CELL_CELL_TYPE_START, &(tmp_u32_data_cell_field), 0, RAMON_FE1600_VSC128_DATA_CELL_CELL_TYPE_LENGTH);  
    
        /*
         * bits 512+ 61:51 - source id
         */
        tmp_u32_data_cell_field = cell->header.source_device;
        SHR_BITCOPY_RANGE(buf, RAMON_FE1600_VSC128_DATA_CELL_SOURCE_ID_START, &(tmp_u32_data_cell_field), 0, RAMON_FE1600_VSC128_DATA_CELL_SOURCE_ID_LENGTH);  
    
    
        /*
         * bits  - source level
         */
        DNXC_IF_ERR_EXIT(soc_dnxc_actual_entity_value(unit,cell->header.source_level, &tmp_actual_value));
        tmp_u32_data_cell_field = (uint32)tmp_actual_value;
        SHR_BITCOPY_RANGE(buf, RAMON_FE1600_VSC128_DATA_CELL_SRC_LEVEL_START, &(tmp_u32_data_cell_field), 0, RAMON_FE1600_VSC128_DATA_CELL_SRC_LEVEL_LENGTH);
   
        /*
         * bits  - destination level
         */
        DNXC_IF_ERR_EXIT(soc_dnxc_actual_entity_value(unit, cell->header.destination_level, &tmp_actual_value));
        tmp_u32_data_cell_field = (uint32)tmp_actual_value;
        SHR_BITCOPY_RANGE(buf, RAMON_FE1600_VSC128_DATA_CELL_DEST_LEVEL_START, &(tmp_u32_data_cell_field), 0, RAMON_FE1600_VSC128_DATA_CELL_DEST_LEVEL_LENGTH);
    
        /*
         * bits  - fip switch
         */
        tmp_u32_data_cell_field = cell->header.fip_link;
        SHR_BITCOPY_RANGE(buf, RAMON_FE1600_VSC128_DATA_CELL_FIP_SWITCH_START, &(tmp_u32_data_cell_field), 0,  RAMON_FE1600_VSC128_DATA_CELL_FIP_SWITCH_LENGTH);
        SHR_BITCOPY_RANGE(buf, RAMON_FE1600_VSC128_SR_DATA_CELL_FIP_SWITCH_POSITION, &(tmp_u32_data_cell_field), RAMON_FE1600_VSC128_DATA_CELL_FIP_SWITCH_LENGTH,  RAMON_FE1600_VSC128_SR_DATA_CELL_FIP_SWITCH_LENGTH);
    
        /*
         * bits  - fe1 switch
         */
        tmp_u32_data_cell_field = cell->header.fe1_link;
        SHR_BITCOPY_RANGE(buf, RAMON_FE1600_VSC128_DATA_CELL_FE1_SWITCH_START, &(tmp_u32_data_cell_field), 0,  RAMON_FE1600_VSC128_DATA_CELL_FE1_SWITCH_LENGTH);
        SHR_BITCOPY_RANGE(buf, RAMON_FE1600_VSC128_SR_DATA_CELL_FE1_SWITCH_POSITION, &(tmp_u32_data_cell_field), RAMON_FE1600_VSC128_DATA_CELL_FE1_SWITCH_LENGTH,  RAMON_FE1600_VSC128_SR_DATA_CELL_FE1_SWITCH_LENGTH);
    
        /*
         * bits  - fe2 switch
         */
        tmp_u32_data_cell_field = cell->header.fe2_link;
        SHR_BITCOPY_RANGE(buf, RAMON_FE1600_VSC128_DATA_CELL_FE2_SWITCH_START, &(tmp_u32_data_cell_field), 0,  RAMON_FE1600_VSC128_DATA_CELL_FE2_SWITCH_LENGTH);
        SHR_BITCOPY_RANGE(buf, RAMON_FE1600_VSC128_SR_DATA_CELL_FE2_SWITCH_POSITION, &(tmp_u32_data_cell_field), RAMON_FE1600_VSC128_DATA_CELL_FE2_SWITCH_LENGTH,  RAMON_FE1600_VSC128_SR_DATA_CELL_FE2_SWITCH_LENGTH);
    
        /*
         * bits  - fe3 switch
         */
        tmp_u32_data_cell_field = cell->header.fe3_link;
        SHR_BITCOPY_RANGE(buf, RAMON_FE1600_VSC128_DATA_CELL_FE3_SWITCH_START, &(tmp_u32_data_cell_field), 0,  RAMON_FE1600_VSC128_DATA_CELL_FE3_SWITCH_LENGTH);
        SHR_BITCOPY_RANGE(buf, RAMON_FE1600_VSC128_SR_DATA_CELL_FE3_SWITCH_POSITION, &(tmp_u32_data_cell_field), RAMON_FE1600_VSC128_DATA_CELL_FE3_SWITCH_LENGTH,  RAMON_FE1600_VSC128_SR_DATA_CELL_FE3_SWITCH_LENGTH);
    
        /*
        * bit - In-band cell
        */
        tmp_u32_data_cell_field = cell->header.is_inband;
        SHR_BITCOPY_RANGE(buf, RAMON_FE1600_VSC128_SR_DATA_CELL_INBAND_CELL_POSITION, &(tmp_u32_data_cell_field), 0,  RAMON_FE1600_VSC128_SR_DATA_CELL_INBAND_CELL_LENGTH);
    
        /*
        * bits 511:0 - payload
        */        
        SHR_BITCOPY_RANGE(buf, RAMON_FE1600_VSC128_SR_DATA_CELL_PAYLOAD_START, cell->payload.data, 0,  RAMON_FE1600_VSC128_SR_DATA_CELL_PAYLOAD_LENGTH);
        
    } 
    else
    {   
        if(cell->header.is_inband && DNXC_VSC256_CELL_FORMAT_FE600 == cell->payload.inband.cell_format) {
            DNXC_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_DNXC_MSG("VSC256 can't be sent in FE600 format")));
        }
        soc_dnxc_vsc256_build_header(unit, cell, buf_size_bytes - (RAMON_FE1600_SR_DATA_CELL_PAYLOAD_LENGTH/8), (uint32*)(((uint8*)buf) + (RAMON_FE1600_SR_DATA_CELL_PAYLOAD_LENGTH/8) + SOC_DNXF_DEFS_GET(unit, source_routed_cells_header_offset)/8));             
        /*
        * bits 511:0 - payload
        */
        SHR_BITCOPY_RANGE(buf, RAMON_FE1600_SR_DATA_CELL_PAYLOAD_START, cell->payload.data, 0,  RAMON_FE1600_SR_DATA_CELL_PAYLOAD_LENGTH);
    }  
    
exit:
    DNXC_FUNC_RETURN;  
}

soc_error_t 
soc_ramon_fe1600_sr_cell_send(int unit, const dnxc_vsc256_sr_cell_t* cell)
{
    uint8 buf[RAMON_FE1600_DATA_CELL_BYTE_SIZE]; 
    soc_error_t rc;
    soc_port_t first_link, inner_link;
    soc_reg_above_64_val_t reg_val;
    uint32 reg_val32, blk_id;
    DNXC_INIT_FUNC_DEFS;

    /*Format cell*/
    rc = soc_ramon_fe1600_sr_cell_format(unit, cell, RAMON_FE1600_DATA_CELL_BYTE_SIZE, (uint32*)buf); 
    DNXC_IF_ERR_EXIT(rc);

    /*find first link*/
    switch(cell->header.source_level) {
        case     bcmFabricDeviceTypeFIP:
            first_link = cell->header.fip_link;
            break;
        case     bcmFabricDeviceTypeFE1:
            first_link = cell->header.fe1_link;
            break;
        case     bcmFabricDeviceTypeFE2:
            first_link = cell->header.fe2_link;
            break;
        case     bcmFabricDeviceTypeFE3:
            first_link = cell->header.fe3_link;
            break;
        case     bcmFabricDeviceTypeFAP:
        case     bcmFabricDeviceTypeFE13:
        case     bcmFabricDeviceTypeFOP:
        case     bcmFabricDeviceTypeUnknown:
        default:
            DNXC_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_DNXC_MSG("Can't retrive first link from cell")));
    }

    blk_id = INT_DEVIDE(first_link,SOC_DNXF_DEFS_GET(unit, nof_links_in_dcq));
    inner_link = first_link%SOC_DNXF_DEFS_GET(unit, nof_links_in_dcq);

    SOC_REG_ABOVE_64_CLEAR(reg_val);
    SHR_BITCOPY_RANGE(reg_val, 0, (uint32*)buf, 0*128, 128);
    DNXC_IF_ERR_EXIT(WRITE_DCL_CPU_DATA_CELL_0r(unit, blk_id, reg_val));

    SOC_REG_ABOVE_64_CLEAR(reg_val);
    SHR_BITCOPY_RANGE(reg_val, 0, (uint32*)buf, 1*128, 128);
    DNXC_IF_ERR_EXIT(WRITE_DCL_CPU_DATA_CELL_1r(unit, blk_id, reg_val));

    SOC_REG_ABOVE_64_CLEAR(reg_val);
    SHR_BITCOPY_RANGE(reg_val, 0, (uint32*)buf, 2*128, 128);
    DNXC_IF_ERR_EXIT(WRITE_DCL_CPU_DATA_CELL_2r(unit, blk_id, reg_val));

    SOC_REG_ABOVE_64_CLEAR(reg_val);
    SHR_BITCOPY_RANGE(reg_val, 0, (uint32*)buf, 3*128, 128);
    DNXC_IF_ERR_EXIT(WRITE_DCL_CPU_DATA_CELL_3r(unit, blk_id, reg_val));

    SOC_REG_ABOVE_64_CLEAR(reg_val);
    SHR_BITCOPY_RANGE(reg_val, 0, (uint32*)buf, 4*128, 120);
    DNXC_IF_ERR_EXIT(WRITE_DCL_CPU_DATA_CELL_4r(unit, blk_id, reg_val));

    reg_val32 = 0;
    soc_reg_field_set(unit, DCL_TRANSMIT_CELL_OUTPUT_LINK_NUMBERr, &reg_val32, CPU_LINK_NUMf, inner_link);


    /* cell size can be maximum of 128 bytes */
    soc_reg_field_set(unit, DCL_TRANSMIT_CELL_OUTPUT_LINK_NUMBERr, &reg_val32, CPU_CELL_SIZEf, 127);
    
    DNXC_IF_ERR_EXIT(WRITE_DCL_TRANSMIT_CELL_OUTPUT_LINK_NUMBERr(unit, blk_id, reg_val32));
    DNXC_IF_ERR_EXIT(WRITE_DCL_TRANSMIT_DATA_CELL_TRIGGERr(unit, blk_id, 0x1));

exit:
    DNXC_FUNC_RETURN;  

}

/*
 * Function:
 *      soc_ramon_fe1600_fabric_cell_get
 * Purpose:
 *      get cpu cell
 * Parameters:
 *      unit            - (IN)  Unit number.
 *      entry           - (OUT) Received data
 * Returns:
 *      SOC_E_XXX
 */
soc_error_t
soc_ramon_fe1600_fabric_cell_get(int unit, soc_dnxc_fabric_cell_entry_t entry)
{
    uint32 reg, blk_idx;
    uint32 success_p_0 =0, success_p_1 =0, success_s_0 =0, success_s_1 =0;
    int found_cell;
    DNXC_INIT_FUNC_DEFS;   
	SOC_RAMON_FE1600_ONLY(unit);

    found_cell = 0;
    
    for(blk_idx=0 ; blk_idx<SOC_DNXF_DEFS_GET(unit, nof_instances_dch) ; blk_idx++) 
    {

        DNXC_IF_ERR_EXIT(READ_DCH_INTERRUPT_REGISTERr(unit, blk_idx, &reg));

        success_p_0 = soc_reg_field_get(unit, DCH_INTERRUPT_REGISTERr, reg, CPUDATACELLFNE_A_0_P_INTf);       
        success_p_1 = soc_reg_field_get(unit, DCH_INTERRUPT_REGISTERr, reg, CPUDATACELLFNE_A_1_P_INTf);
        success_s_0 = soc_reg_field_get(unit, DCH_INTERRUPT_REGISTER_1r, reg, CPUDATACELLFNE_A_0_S_INTf);
        success_s_1 = soc_reg_field_get(unit, DCH_INTERRUPT_REGISTER_1r, reg, CPUDATACELLFNE_A_1_S_INTf);

        sal_memset(entry, 0x00, sizeof(soc_dnxc_fabric_cell_entry_t));
        if(success_p_0) {
            DNXC_IF_ERR_EXIT(READ_DCH_CPU_DATA_CELL_APr(unit, blk_idx, entry));
            found_cell = 1;
            break;
        } else if(success_p_1) {
            DNXC_IF_ERR_EXIT(READ_DCH_CPU_DATA_CELL_BPr(unit, blk_idx, entry));
            found_cell = 1;
            break;
        } else if(SOC_DNXF_CONFIG(unit).is_dual_mode && success_s_0) {
            DNXC_IF_ERR_EXIT(READ_DCH_CPU_DATA_CELL_ASr(unit, blk_idx, entry));
            found_cell = 1;
            break;
        } else if(SOC_DNXF_CONFIG(unit).is_dual_mode && success_s_1) {
            DNXC_IF_ERR_EXIT(READ_DCH_CPU_DATA_CELL_BSr(unit, blk_idx, entry));
            found_cell = 1;
            break;
        }
    }

    if (!found_cell)
    {
        DNXC_EXIT_WITH_ERR_NO_MSG(SOC_E_EMPTY);
    }

exit:
    DNXC_FUNC_RETURN;
}

soc_error_t 
soc_ramon_fe1600_cell_filter_set(int unit, uint32 flags, uint32 array_size, soc_dnxc_filter_type_t* filter_type_arr, uint32* filter_type_val) 
{
    int i, rc;
    uint32 reg_val, mask_val, reg_val1, mask_val1, reg_val32;
    uint32 table_size;
    soc_dnxf_cell_filter_mode_t filter_mode = soc_dnxf_cell_filter_copy_mode;
    DNXC_INIT_FUNC_DEFS;
	SOC_RAMON_FE1600_ONLY(unit);

    if((flags & SOC_CELL_FILTER_FLAG_DONT_FORWARD) && (flags & SOC_CELL_FILTER_FLAG_DONT_TRAP)) {
        filter_mode = soc_dnxf_cell_filter_filter_mode;
    } else if (flags & SOC_CELL_FILTER_FLAG_DONT_FORWARD) {
        filter_mode = soc_dnxf_cell_filter_capture_mode;
    } else if (flags & SOC_CELL_FILTER_FLAG_DONT_TRAP) {
        filter_mode = soc_dnxf_cell_filter_normal_mode;
    }

    for(i=0 ; i<SOC_DNXF_DEFS_GET(unit, nof_instances_dch) ; i++) {
        DNXC_IF_ERR_EXIT(READ_DCH_REG_0060r(unit, i, &reg_val));
        soc_reg_field_set(unit, DCH_REG_0060r, &reg_val, DCH_MTCH_ACTf, filter_mode);
        DNXC_IF_ERR_EXIT(WRITE_DCH_REG_0060r(unit, i, reg_val));
    }

    reg_val = 0;
    mask_val = 0xFFFFFFFF;
    reg_val1 = 0;
    mask_val1 = 0xFFFFFFFF;

    for(i=0 ; i<array_size ; i++) {
        switch(filter_type_arr[i]) {
        case soc_dnxc_filter_type_source_id:
            if((filter_type_val[i] & (~0xFF)) != 0) {
                DNXC_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_DNXC_MSG("source id %d out of range"), filter_type_val[i]));
            }
            reg_val |= (filter_type_val[i]>>3);
            mask_val &= (~0xFF);
            reg_val1 |= (filter_type_val[i]&0x7)<<29;
            mask_val1 &= (~0xE0000000);
            break;
        case soc_dnxc_filter_type_multicast_id:
        case soc_dnxc_filter_type_dest_id:
            rc = MBCM_DNXF_DRIVER_CALL(unit,mbcm_dnxf_multicast_table_size_get,(unit, &table_size));
            DNXC_IF_ERR_EXIT(rc);
            if(filter_type_val[i] >= table_size) {
                DNXC_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_DNXC_MSG("multicast id out of range")));
            }
            reg_val |= (filter_type_val[i] << 8);
            mask_val &= ~((table_size-1) << 8);
            break;
        case soc_dnxc_filter_type_priority:
            if((filter_type_val[i] & (~0x3)) != 0) {
                DNXC_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_DNXC_MSG("the priority type is out of range")));
            }
            reg_val |= (filter_type_val[i] << 29);
            mask_val &= ~(0x3 << 29);
            break;
        case soc_dnxc_filter_type_traffic_cast:
            if((filter_type_val[i] & (~0x3)) != 0) {
                DNXC_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_DNXC_MSG("the traffic_cast type is out of range")));
            }
            reg_val |= (filter_type_val[i] << 27);
            mask_val &= ~(0x3 << 27);
            break;
        default:
            DNXC_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_DNXC_MSG("unsupported filter type")));
        }
    }
    
    reg_val32 = 0;
    soc_reg_field_set(unit, DCH_FILTER_MATCH_INPUT_LINKr, &reg_val32, DATA_MTCH_ONf, 0x1);

    for(i = 0; i < SOC_DNXF_DEFS_GET(unit, nof_instances_dch); i++) {
        DNXC_IF_ERR_EXIT(WRITE_DCH_FILTER_MATCH_INPUT_LINKr(unit, i, reg_val32));  
    }

   for(i=0 ; i<SOC_DNXF_DEFS_GET(unit, nof_instances_dch) ; i++) {
        DNXC_IF_ERR_EXIT(WRITE_DCH_PROGRAMMABLE_DATA_CELL_COUNTER_0r(unit, i, reg_val));
        DNXC_IF_ERR_EXIT(WRITE_DCH_PROGRAMMABLE_DATA_CELL_COUNTER_1r(unit, i, reg_val1));
        DNXC_IF_ERR_EXIT(WRITE_DCH_PROGRAMMABLE_DATA_CELL_COUNTER_MASK_0r(unit, i, mask_val));
        DNXC_IF_ERR_EXIT(WRITE_DCH_PROGRAMMABLE_DATA_CELL_COUNTER_MASK_1r(unit, i, mask_val1));
        DNXC_IF_ERR_EXIT(WRITE_DCH_PROGRAMMABLE_DATA_CELL_COUNTER_MASK_2r(unit, i, 0xFFFFFFFF));
   }

exit:
     DNXC_FUNC_RETURN;
}

soc_error_t 
soc_ramon_fe1600_cell_filter_clear(int unit)
{
    int i;
    DNXC_INIT_FUNC_DEFS;
	SOC_RAMON_FE1600_ONLY(unit);

    for(i=0 ; i<SOC_DNXF_DEFS_GET(unit, nof_instances_dch) ; i++) {  
        DNXC_IF_ERR_EXIT(WRITE_DCH_PROGRAMMABLE_DATA_CELL_COUNTER_0r(unit, i, 0));
        DNXC_IF_ERR_EXIT(WRITE_DCH_PROGRAMMABLE_DATA_CELL_COUNTER_1r(unit, i, 0));
        DNXC_IF_ERR_EXIT(WRITE_DCH_PROGRAMMABLE_DATA_CELL_COUNTER_2r(unit, i, 0)); 
        DNXC_IF_ERR_EXIT(WRITE_DCH_PROGRAMMABLE_DATA_CELL_COUNTER_MASK_0r(unit, i, 0x0));
        DNXC_IF_ERR_EXIT(WRITE_DCH_PROGRAMMABLE_DATA_CELL_COUNTER_MASK_1r(unit, i, 0x0));
        DNXC_IF_ERR_EXIT(WRITE_DCH_PROGRAMMABLE_DATA_CELL_COUNTER_MASK_2r(unit, i, 0x0));
    }

exit:
     DNXC_FUNC_RETURN;
}

soc_error_t 
soc_ramon_fe1600_cell_filter_count_get(int unit, int *count)
{
    int i;
    int temp_count;
    uint32 reg32_val;
    DNXC_INIT_FUNC_DEFS;
    
    *count = 0;
    for(i=0 ; i<SOC_DNXF_DEFS_GET(unit, nof_instances_dch) ; i++) {  
        DNXC_IF_ERR_EXIT(READ_DCH_PROGRAMMABLE_CELLS_COUNTER_Pr(unit, i, &reg32_val));

        if (*count == -1) {
            continue;
        }

        temp_count = soc_reg_field_get(unit, DCH_PROGRAMMABLE_CELLS_COUNTER_Pr, reg32_val, PRG_CELL_CNT_APf);
        *count += temp_count;
        temp_count = soc_reg_field_get(unit, DCH_PROGRAMMABLE_CELLS_COUNTER_Pr, reg32_val, PRG_CELL_CNT_BPf);
        *count += temp_count;

        /*check overflow*/
        temp_count = soc_reg_field_get(unit, DCH_PROGRAMMABLE_CELLS_COUNTER_Pr, reg32_val, PRG_CELL_CNT_AOPf);
        if (temp_count) {
            *count = -1;
            continue;
        }
        temp_count = soc_reg_field_get(unit, DCH_PROGRAMMABLE_CELLS_COUNTER_Pr, reg32_val, PRG_CELL_CNT_BOPf);
        if (temp_count) {
            *count = -1;
            continue;
        }

    }

    if(SOC_DNXF_CONFIG(unit).is_dual_mode) {
        for(i=0 ; i<SOC_DNXF_DEFS_GET(unit, nof_instances_dch) ; i++) {  
            DNXC_IF_ERR_EXIT(READ_DCH_PROGRAMMABLE_CELLS_COUNTER_Sr(unit, i, &reg32_val));

            if (*count == -1) {
                continue;
            }

            temp_count = soc_reg_field_get(unit, DCH_PROGRAMMABLE_CELLS_COUNTER_Sr, reg32_val, PRG_CELL_CNT_ASf);
            *count += temp_count;
            temp_count = soc_reg_field_get(unit, DCH_PROGRAMMABLE_CELLS_COUNTER_Sr, reg32_val, PRG_CELL_CNT_BSf);
            *count += temp_count;

            /*check overflow*/
            temp_count = soc_reg_field_get(unit, DCH_PROGRAMMABLE_CELLS_COUNTER_Sr, reg32_val, PRG_CELL_CNT_AOSf);
            if(temp_count) {
                *count = -1;
                continue;
            }
            temp_count = soc_reg_field_get(unit, DCH_PROGRAMMABLE_CELLS_COUNTER_Sr, reg32_val, PRG_CELL_CNT_BOSf);
            if(temp_count) {
                *count = -1;
                continue;
            }
        }
    }
    
exit:
     DNXC_FUNC_RETURN;
}

soc_error_t 
soc_ramon_fe1600_control_cell_filter_set(int unit, uint32 flags, soc_dnxc_control_cell_types_t cell_type, uint32 array_size, soc_dnxc_control_cell_filter_type_t* filter_type_arr, uint32* filter_type_val) 
{
    int i = 0;
    uint32 reg_val1;
    soc_reg_above_64_val_t reg_val, mask_val;
    soc_dnxf_cell_filter_mode_t filter_mode = soc_dnxf_control_cell_filter_copy_mode;
    DNXC_INIT_FUNC_DEFS;
	SOC_RAMON_FE1600_ONLY(unit);

    if((flags & SOC_CELL_FILTER_FLAG_DONT_FORWARD) && (flags & SOC_CELL_FILTER_FLAG_DONT_TRAP)) {
        filter_mode = soc_dnxf_control_cell_filter_filter_mode;
    } else if (flags & SOC_CELL_FILTER_FLAG_DONT_FORWARD) {
        filter_mode = soc_dnxf_control_cell_filter_capture_mode;
    } else if (flags & SOC_CELL_FILTER_FLAG_DONT_TRAP) {
        filter_mode = soc_dnxf_control_cell_filter_normal_mode;
    }

    for(i=0 ; i<SOC_RAMON_FE1600_NOF_INSTANCES_CCS ; i++) {
        DNXC_IF_ERR_EXIT(READ_CCS_DEBUG_CONFIGURATIONSr(unit, i, &reg_val1));
        soc_reg_field_set(unit, CCS_DEBUG_CONFIGURATIONSr, &reg_val1, CAPTURE_OPERATIONf, filter_mode);
        /* We would like to capture the control cells in VSC256 mode, even if the cell enter the device in VSC128 mode */
        /*Due the fact that the VSC128 cell bits are on the msbs of the filter/mask */
        soc_reg_field_set(unit, CCS_DEBUG_CONFIGURATIONSr, &reg_val1, CAPTURE_CELL_FORMAT_MODEf, 1);
        /*fe2 filter mode*/
        soc_reg_field_set(unit, CCS_DEBUG_CONFIGURATIONSr, &reg_val1, FIELD_5_5f, (cell_type == soc_dnxc_fe2_filtered_cell) ? 1 : 0);
        DNXC_IF_ERR_EXIT(WRITE_CCS_DEBUG_CONFIGURATIONSr(unit, i, reg_val1));
    }

    reg_val1 = 0;

    SOC_REG_ABOVE_64_CLEAR(reg_val);
    /* mask of "1"s mean to ignore when using the filter */
    SOC_REG_ABOVE_64_ALLONES(mask_val);
    

    if ((cell_type == soc_dnxc_flow_status_cell) || (cell_type == soc_dnxc_credit_cell))
    {
        for(i=0 ; i<array_size ; i++) 
        {
                switch(filter_type_arr[i]) {
                case soc_dnxc_filter_control_cell_type_source_device:
                    if((filter_type_val[i] & (~0x7FF)) != 0) {
                        DNXC_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_DNXC_MSG("source device id is out of range")));
                    }
                    SHR_BITCOPY_RANGE(reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_SOURCE_ID_START, &(filter_type_val[i]), 0, RAMON_FE1600_VSC256_CONTROL_CELL_SOURCE_ID_LENGTH);
                    /* add mask for bits 103:93 in the cell*/
                    SHR_BITCLR_RANGE(mask_val, RAMON_FE1600_VSC256_CONTROL_CELL_SOURCE_ID_START, RAMON_FE1600_VSC256_CONTROL_CELL_SOURCE_ID_LENGTH);
                    break;
                case soc_dnxc_filter_control_cell_type_dest_device:
                    if((filter_type_val[i] & (~0x7FF)) != 0) {
                        DNXC_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_DNXC_MSG("dest device id is out of range")));
                    }
                    SHR_BITCOPY_RANGE(reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_DEST_DEVICE_START, &(filter_type_val[i]), 0, RAMON_FE1600_VSC256_CONTROL_CELL_DEST_DEVICE_LENGTH);
                    /* add mask for bits 114:104 in the cell*/
                    SHR_BITCLR_RANGE(mask_val, RAMON_FE1600_VSC256_CONTROL_CELL_DEST_DEVICE_START, RAMON_FE1600_VSC256_CONTROL_CELL_DEST_DEVICE_LENGTH);
                    break;
                default:
                    DNXC_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_DNXC_MSG("unsupported filter type")));
                }
        }
    } else if (cell_type == soc_dnxc_fe2_filtered_cell) {
        /*do nothing*/
   /* The control cell type is reachability cell (no support for dest_device filter) */
   } else {
       SHR_BITCOPY_RANGE(reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_REACHABILITY_SOURCE_ID_START, &(filter_type_val[0]), 0, RAMON_FE1600_VSC256_CONTROL_CELL_SOURCE_ID_LENGTH);
       /* add mask for bits 111:101 in the cell*/
       SHR_BITCLR_RANGE(mask_val, RAMON_FE1600_VSC256_CONTROL_CELL_REACHABILITY_SOURCE_ID_START, RAMON_FE1600_VSC256_CONTROL_CELL_SOURCE_ID_LENGTH);
   }
   
   /* Add the control cell type to the filter */ 
   SHR_BITCOPY_RANGE(reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_CONTROL_TYPE_START, &cell_type, 0, RAMON_FE1600_VSC256_CONTROL_CELL_CONTROL_TYPE_LENGTH);
   /* add mask for bits 117:115 in the cell*/
   SHR_BITCLR_RANGE(mask_val, RAMON_FE1600_VSC256_CONTROL_CELL_CONTROL_TYPE_START, RAMON_FE1600_VSC256_CONTROL_CELL_CONTROL_TYPE_LENGTH);

   /* Set the filter and mask registers*/
   for(i=0 ; i<SOC_RAMON_FE1600_NOF_INSTANCES_CCS ; i++) {
       DNXC_IF_ERR_EXIT(WRITE_CCS_CAPTURE_FILTER_CELL_0r(unit, i, reg_val));
       DNXC_IF_ERR_EXIT(WRITE_CCS_CAPTURE_FILTER_MASK_0r(unit, i, mask_val));
   }

exit:
     DNXC_FUNC_RETURN;
}

soc_error_t 
soc_ramon_fe1600_control_cell_filter_clear(int unit)
{
    soc_reg_above_64_val_t reg_val;
    uint32 reg32_val;
    int i,nof_instances_ccs;
    DNXC_INIT_FUNC_DEFS;
	
    SOC_REG_ABOVE_64_CLEAR(reg_val);


    DNXC_IF_ERR_EXIT(WRITE_CCS_CAPTURE_FILTER_CELL_0r(unit, 0, reg_val));
    DNXC_IF_ERR_EXIT(WRITE_CCS_CAPTURE_FILTER_MASK_0r(unit, 0, reg_val));
    DNXC_IF_ERR_EXIT(WRITE_CCS_CAPTURE_FILTER_CELL_0r(unit, 1, reg_val));
    DNXC_IF_ERR_EXIT(WRITE_CCS_CAPTURE_FILTER_MASK_0r(unit, 1, reg_val));
    nof_instances_ccs = SOC_DNXF_DEFS_GET(unit, nof_instances_ccs);
    reg32_val=0;
     for(i=0 ; i<nof_instances_ccs ; i++) {
        DNXC_IF_ERR_EXIT(READ_CCS_DEBUG_CONFIGURATIONSr(unit, i, &reg32_val));
        soc_reg_field_set(unit, CCS_DEBUG_CONFIGURATIONSr, &reg32_val, FIELD_5_5f, 0);
        DNXC_IF_ERR_EXIT(WRITE_CCS_DEBUG_CONFIGURATIONSr(unit, i, reg32_val));
    }

exit:
     DNXC_FUNC_RETURN;
}

soc_error_t 
soc_ramon_fe1600_control_cell_filter_receive(int unit,  soc_dnxc_captured_control_cell_t* data_out)
{
    soc_reg_above_64_val_t  reg_val;
    uint32 valid_cell_ind0, valid_cell_ind1;

    DNXC_INIT_FUNC_DEFS;
	SOC_RAMON_FE1600_ONLY(unit);

    sal_memset(data_out, 0 ,sizeof(soc_dnxc_captured_control_cell_t));
    
    DNXC_IF_ERR_EXIT(READ_CCS_CAPTURED_CELL_VALIDr(unit, 0, &valid_cell_ind0));
    DNXC_IF_ERR_EXIT(READ_CCS_CAPTURED_CELL_VALIDr(unit, 1, &valid_cell_ind1));
    data_out->valid = valid_cell_ind0 || valid_cell_ind1;

    if (data_out->valid) 
    {
        if (valid_cell_ind0)
        {
            DNXC_IF_ERR_EXIT(READ_CCS_CAPTURED_CELLr(unit, 0, reg_val));
        }
        else 
        {
            DNXC_IF_ERR_EXIT(READ_CCS_CAPTURED_CELLr(unit, 1, reg_val));
        }
        
        SHR_BITCOPY_RANGE((uint32*)&(data_out->control_type), 0, reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_CONTROL_TYPE_START, RAMON_FE1600_VSC256_CONTROL_CELL_CONTROL_TYPE_LENGTH);

        switch(data_out->control_type) {
                case soc_dnxc_flow_status_cell:
                    SHR_BITCOPY_RANGE((uint32*)&(data_out->dest_port), 0, reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_FSM_DEST_PORT_START, RAMON_FE1600_VSC256_CONTROL_CELL_FSM_DEST_PORT_LENGTH);
                    SHR_BITCOPY_RANGE((uint32*)&(data_out->src_queue_num), 0, reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_FSM_SRC_QUEUE_NUM_START, RAMON_FE1600_VSC256_CONTROL_CELL_FSM_SRC_QUEUE_NUM_LENGTH);
                    SHR_BITCOPY_RANGE((uint32*)&(data_out->flow_id), 0, reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_FSM_FLOW_ID_START, RAMON_FE1600_VSC256_CONTROL_CELL_FSM_FLOW_ID_LENGTH);
                    SHR_BITCOPY_RANGE((uint32*)&(data_out->source_device), 0, reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_SOURCE_ID_START, RAMON_FE1600_VSC256_CONTROL_CELL_SOURCE_ID_LENGTH);
                    SHR_BITCOPY_RANGE((uint32*)&(data_out->dest_device), 0, reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_DEST_DEVICE_START, RAMON_FE1600_VSC256_CONTROL_CELL_DEST_DEVICE_LENGTH);
                    break;
                case soc_dnxc_credit_cell:
                    SHR_BITCOPY_RANGE((uint32*)&(data_out->dest_queue_num),0 , reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_CREDIT_DEST_Q_NUM_START, RAMON_FE1600_VSC256_CONTROL_CELL_CREDIT_DEST_Q_NUM_LENGTH);
                    SHR_BITCOPY_RANGE((uint32*)&(data_out->sub_flow_id), 0, reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_CREDIT_SUB_FLOW_ID_START, RAMON_FE1600_VSC256_CONTROL_CELL_CREDIT_SUB_FLOW_ID_LENGTH);
                    SHR_BITCOPY_RANGE((uint32*)&(data_out->flow_id), 0, reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_CREDIT_FLOW_ID_START, RAMON_FE1600_VSC256_CONTROL_CELL_CREDIT_FLOW_ID_LENGTH);
                    SHR_BITCOPY_RANGE((uint32*)&(data_out->source_device), 0, reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_SOURCE_ID_START, RAMON_FE1600_VSC256_CONTROL_CELL_SOURCE_ID_LENGTH);
                    SHR_BITCOPY_RANGE((uint32*)&(data_out->dest_device), 0, reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_DEST_DEVICE_START, RAMON_FE1600_VSC256_CONTROL_CELL_DEST_DEVICE_LENGTH);
                    break;
                case soc_dnxc_reachability_cell:
                    SHR_BITCOPY_RANGE((uint32*)&(data_out->reachability_bitmap),0 , reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_REACHABILITY_BITMAP_START, RAMON_FE1600_VSC256_CONTROL_CELL_REACHABILITY_BITMAP_LENGTH);
                    SHR_BITCOPY_RANGE((uint32*)&(data_out->base_index),0 , reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_REACHABILITY_BASE_ID_START, RAMON_FE1600_VSC256_CONTROL_CELL_REACHABILITY_BASE_ID_LENGTH);
                    SHR_BITCOPY_RANGE((uint32*)&(data_out->source_link_number),0 , reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_REACHABILITY_SRC_LINK_NUM_START, RAMON_FE1600_VSC256_CONTROL_CELL_REACHABILITY_SRC_LINK_NUM_LENGTH);
                    SHR_BITCOPY_RANGE((uint32*)&(data_out->source_device),0 , reg_val, RAMON_FE1600_VSC256_CONTROL_CELL_REACHABILITY_SOURCE_ID_START, RAMON_FE1600_VSC256_CONTROL_CELL_SOURCE_ID_LENGTH);
                    break;
                default:
                    DNXC_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_DNXC_MSG("unsupported filter type")));
                }

    }

exit:
    DNXC_FUNC_RETURN; 
}

soc_error_t
soc_ramon_fe1600_fabric_cell_parse_table_get(int unit, soc_dnxc_fabric_cell_type_t cell_type, uint32 max_nof_lines, soc_dnxc_fabric_cell_parse_table_t *parse_table, uint32 *nof_lines, int is_two_parts)
{
    DNXC_INIT_FUNC_DEFS;
	SOC_RAMON_FE1600_ONLY(unit);

    
    if (cell_type == soc_dnxc_fabric_cell_type_sr_cell)
    {
        soc_dnxc_fabric_cell_parse_table_t temp_table[] = SOC_RAMON_FE1600_FABRIC_CELL_CAPTURED_SRC_FORMAT;

        if (max_nof_lines < SOC_RAMON_FE1600_FABRIC_CELL_CAPTURED_SRC_FORMAT_NOF_LINES)
        {
            DNXC_EXIT_WITH_ERR(SOC_E_INTERNAL, (_BSL_DNXC_MSG_STR("Bigger table is required\n")));
        } 
        
        sal_memcpy(parse_table, temp_table, sizeof(soc_dnxc_fabric_cell_parse_table_t) * SOC_RAMON_FE1600_FABRIC_CELL_CAPTURED_SRC_FORMAT_NOF_LINES);
        *nof_lines = SOC_RAMON_FE1600_FABRIC_CELL_CAPTURED_SRC_FORMAT_NOF_LINES;
    } else {
        soc_dnxc_fabric_cell_parse_table_t temp_table[] = SOC_RAMON_FE1600_FABRIC_CELL_CAPTURED_CELL_FORMAT;

        if (max_nof_lines < SOC_RAMON_FE1600_FABRIC_CELL_CAPTURED_CELL_FORMAT_NOF_LINES)
        {
            DNXC_EXIT_WITH_ERR(SOC_E_INTERNAL, (_BSL_DNXC_MSG_STR("Bigger table is required\n")));
        } 
        
        sal_memcpy(parse_table, temp_table, sizeof(soc_dnxc_fabric_cell_parse_table_t) * SOC_RAMON_FE1600_FABRIC_CELL_CAPTURED_CELL_FORMAT_NOF_LINES);
        *nof_lines = SOC_RAMON_FE1600_FABRIC_CELL_CAPTURED_CELL_FORMAT_NOF_LINES;
    }
   
exit:
    DNXC_FUNC_RETURN; 
}

#define _SOC_RAMON_FE1600_PARSING_CELL_TYPE_START     (512+63)
#define _SOC_RAMON_FE1600_PARSING_CELL_TYPE_LENGTH    (1)
soc_error_t
soc_ramon_fe1600_fabric_cell_type_get(int unit, soc_dnxc_fabric_cell_entry_t entry, soc_dnxc_fabric_cell_type_t *cell_type)
{
    DNXC_INIT_FUNC_DEFS;
    SOC_RAMON_FE1600_ONLY(unit);

    *cell_type = 0;
    SHR_BITCOPY_RANGE((uint32 *) cell_type, 0, (uint32 *) entry, _SOC_RAMON_FE1600_PARSING_CELL_TYPE_START, _SOC_RAMON_FE1600_PARSING_CELL_TYPE_LENGTH);

    DNXC_FUNC_RETURN; 
}

soc_error_t
soc_ramon_fe1600_fabric_cell_is_cell_in_two_parts(int unit, soc_dnxc_fabric_cell_entry_t entry, int* is_two_parts)
{
    DNXC_INIT_FUNC_DEFS;

    *is_two_parts = 0;

    DNXC_FUNC_RETURN;
}

#endif /*defined(BCM_88790_A0)*/

#undef _ERR_MSG_MODULE_NAME

