/*
 * $Id: jer_regs.c,v 1.0 13/07/2014 14:07:36 nhefetz Exp $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */

/* must be at begining of file */
#ifdef _ERR_MSG_MODULE_NAME
#error "_ERR_MSG_MODULE_NAME redefined"
#endif
#define _ERR_MSG_MODULE_NAME BSL_SOC_INIT

/*
 * Includes
 */
#include <shared/bsl.h>

/* SOC includes */
#include <soc/error.h>

/* SOC DPP includes */
#include <soc/dpp/drv.h>
#include <soc/dpp/mbcm.h>

/* SOC DPP JER includes */
#include <soc/dpp/JER/jer_regs.h>
#include <soc/dpp/JER/jer_init.h>

/*****************/
/***  Defines  ***/
/*****************/ 

#define SOC_JER_REGS_ACCESS_CHECK_REF_TEST_VALUE 0xaaff5500
#define SOC_JER_REGS_ACCESS_CHECK_DMA_MEM_ENTRY_COUNT 20
#define SOC_JER_REGS_ALL_ONES 0xffffffff

/* a unique mask for higher bits (32 - 63) for IHB_ACTION_DISABLE reg, if changed, need to be updated according to regs DB file   */
/* the mask assures that only the actual bits in use from the most significant word of the reg are used                         */
#define SOC_JER_REGS_IHB_ACTION_DISABLE_HIGH_BITS_MASK 0x3fffffff

#define SOC_JER_REGS_ACTUAL_SIZE_OF_FULL_DRAM_REJECT_COUNTER_REG 128
#define SOC_JER_REG_IQM_HEADER_APPEND_PTR_TO_COUNTER_HDR_CMP_MAPPING_TABLE_MASK 0xffff
/*******************/
/***  Functions  ***/
/*******************/ 


/********************************/
/***  Check Access Functions  ***/
/********************************/ 

int soc_jer_regs_eci_access_check(int unit)
{
    uint32 reg_val = SOC_JER_REGS_ACCESS_CHECK_REF_TEST_VALUE;
    SOCDNX_INIT_FUNC_DEFS;

    SOCDNX_IF_ERR_EXIT(WRITE_ECI_TEST_REGISTERr(unit, reg_val));
    SOCDNX_IF_ERR_EXIT(READ_ECI_TEST_REGISTERr(unit, &reg_val));

#ifdef SAL_BOOT_PLISIM
    if (SAL_BOOT_PLISIM == 0) {
        reg_val = ~reg_val;
    }
#endif /* SAL_BOOT_PLISIM */

    if (reg_val != (SOC_JER_REGS_ACCESS_CHECK_REF_TEST_VALUE)) {
        SOCDNX_EXIT_WITH_ERR(SOC_E_FAIL, (_BSL_SOCDNX_MSG("Unable to properly access ECI\n")));
    }
    
exit:
    SOCDNX_FUNC_RETURN;
}


/* Reads and writes to regs of sizes 32, 64 and >64 from IDR and IHB blocks */
int soc_jer_regs_blocks_access_check_regs(int unit)
{
    uint32                  reg32_original_value, reg32_test_value, reg32_test_value_ref = SOC_JER_REGS_ACCESS_CHECK_REF_TEST_VALUE;
    uint64                  reg64_original_value, reg64_test_value_ref, reg64_test_value;
    soc_reg_above_64_val_t  reg_above_64_original_value, reg_above_64_test_value_ref, reg_above_64_test_value, reg_above_64_auxiliary_mask;

    SOCDNX_INIT_FUNC_DEFS;

    COMPILER_64_SET(reg64_test_value_ref, SOC_JER_REGS_ACCESS_CHECK_REF_TEST_VALUE & SOC_JER_REGS_IHB_ACTION_DISABLE_HIGH_BITS_MASK, SOC_JER_REGS_ACCESS_CHECK_REF_TEST_VALUE);
    
    /* just some random pattern, max size of pattern can be 0xff */
    SOC_REG_ABOVE_64_SET_PATTERN(reg_above_64_test_value_ref, 0xaa);

    SOC_REG_ABOVE_64_CREATE_MASK(reg_above_64_auxiliary_mask, 138, 0);
    SOC_REG_ABOVE_64_AND(reg_above_64_test_value_ref, reg_above_64_auxiliary_mask);

    SOC_REG_ABOVE_64_CLEAR(reg_above_64_test_value);


    /* Read and save starting value */
    if (!soc_feature(unit, soc_feature_no_fabric)) {
        SOCDNX_IF_ERR_EXIT(READ_FDT_RESERVED_SPARE_3r(unit, &reg32_original_value));
    }
    SOCDNX_IF_ERR_EXIT(READ_IRE_INDIRECT_COMMAND_DATA_INCREMENTr(unit, &reg64_original_value));
    SOCDNX_IF_ERR_EXIT(READ_OLP_INDIRECT_COMMAND_WR_DATAr(unit, reg_above_64_original_value));

    /* Write test value */
    if (!soc_feature(unit, soc_feature_no_fabric)) {
        SOCDNX_IF_ERR_EXIT(WRITE_FDT_RESERVED_SPARE_3r(unit, reg32_test_value_ref));
    }
    SOCDNX_IF_ERR_EXIT(WRITE_IRE_INDIRECT_COMMAND_DATA_INCREMENTr(unit, reg64_test_value_ref));
    SOCDNX_IF_ERR_EXIT(WRITE_OLP_INDIRECT_COMMAND_WR_DATAr(unit, reg_above_64_test_value_ref));

    /* Read test value */
    if (!soc_feature(unit, soc_feature_no_fabric)) {
        SOCDNX_IF_ERR_EXIT(READ_FDT_RESERVED_SPARE_3r(unit, &reg32_test_value));
    }
    SOCDNX_IF_ERR_EXIT(READ_IRE_INDIRECT_COMMAND_DATA_INCREMENTr(unit, &reg64_test_value));
    SOCDNX_IF_ERR_EXIT(READ_OLP_INDIRECT_COMMAND_WR_DATAr(unit, reg_above_64_test_value));

    /* Check correctness of read values*/ 
    if (!soc_feature(unit, soc_feature_no_fabric)) {
        if (reg32_test_value != reg32_test_value_ref)  {
            SOCDNX_EXIT_WITH_ERR(SOC_E_FAIL, (_BSL_SOCDNX_MSG("Problem with Block access - Failed accessing a 32b Reg\n")));
        }
    }

    if ( COMPILER_64_NE(reg64_test_value, reg64_test_value_ref)) {
        SOCDNX_EXIT_WITH_ERR(SOC_E_FAIL, (_BSL_SOCDNX_MSG("Problem with Block access - Failed accessing a 64b Reg\n")));
    }
    
    if (!SOC_REG_ABOVE_64_IS_EQUAL(reg_above_64_test_value_ref, reg_above_64_test_value))  {
        SOCDNX_EXIT_WITH_ERR(SOC_E_FAIL, (_BSL_SOCDNX_MSG("Problem with Block access - Failed accessing an above 64b Reg\n")));
    }

    /* Restore initial value */
    if (!soc_feature(unit, soc_feature_no_fabric)) {
        SOCDNX_IF_ERR_EXIT(WRITE_FDT_RESERVED_SPARE_3r(unit, reg32_original_value));
    }
    SOCDNX_IF_ERR_EXIT(WRITE_IRE_INDIRECT_COMMAND_DATA_INCREMENTr(unit, reg64_original_value));
    SOCDNX_IF_ERR_EXIT(WRITE_OLP_INDIRECT_COMMAND_WR_DATAr(unit, reg_above_64_original_value));

exit:
    SOCDNX_FUNC_RETURN;
}

int soc_jer_regs_blocks_access_check_mem(int unit, soc_mem_t test_mem, int block_num)
{
    uint32 mem_buff[SOC_MAX_MEM_WORDS], mask[SOC_MAX_MEM_WORDS];

    int word_index, entry_size;

    SOCDNX_INIT_FUNC_DEFS;    

    /* size of each entry in the memory */
    entry_size = soc_mem_entry_words(unit, test_mem);       

    /* bit mask for all r/w bits in memory entry */
    soc_mem_datamask_rw_get(unit, test_mem, mask);          

    /* filling buffer for writing */
    for (word_index = 0; word_index < entry_size; ++word_index) {
        mem_buff[word_index] = SOC_JER_REGS_ACCESS_CHECK_REF_TEST_VALUE & mask[word_index];
    }

    /* Writing and reading buffer */
    SOCDNX_IF_ERR_EXIT(soc_mem_write(unit, test_mem, block_num, soc_mem_index_min(unit, test_mem), mem_buff));
    sal_memset(mem_buff, 0, SOC_MAX_MEM_WORDS * sizeof(uint32));
    SOCDNX_IF_ERR_EXIT(soc_mem_read(unit, test_mem, block_num, soc_mem_index_min(unit, test_mem), mem_buff));

    /* checking result */
    for (word_index = 0; word_index < entry_size; ++word_index) {
        if ( (mem_buff[word_index] & mask[word_index]) != (SOC_JER_REGS_ACCESS_CHECK_REF_TEST_VALUE & mask[word_index]) ) {
            LOG_DEBUG(BSL_LS_SOC_INIT, (BSL_META_U(unit, "word %d: received:%x \t expected:%x\n"),
                                        word_index, mem_buff[word_index] & mask[word_index], SOC_JER_REGS_ACCESS_CHECK_REF_TEST_VALUE & mask[word_index]));
            SOCDNX_EXIT_WITH_ERR(SOC_E_FAIL, (_BSL_SOCDNX_MSG("Problem with Block access - Failed accessing Mem\n")));      
        }
    }

exit:
    SOCDNX_FUNC_RETURN;
}

int soc_jer_regs_blocks_access_check_dma(int unit)
{
    uint32 *mem_ptr, mask[SOC_MAX_MEM_WORDS];

    /* Should be filled with the checked memory */
    soc_mem_t test_mem = SOC_IS_QAX(unit) ? TAR_MCDBm : IRR_MCDBm;

    int i, j, dma_size, entry_size, word_count;

    SOCDNX_INIT_FUNC_DEFS;

    /* size of each entry in the memory */
    entry_size = soc_mem_entry_words(unit, test_mem);       

    /* bit mask for all r/w bits in memory entry */
    soc_mem_datamask_rw_get(unit, test_mem, mask);          

    word_count = entry_size * SOC_JER_REGS_ACCESS_CHECK_DMA_MEM_ENTRY_COUNT;
    dma_size = sizeof(uint32) * word_count;

    /* Allocation */
    if ((mem_ptr = soc_cm_salloc(unit, dma_size, "Jericho memory DMA access check")) == NULL) {
        SOCDNX_EXIT_WITH_ERR(SOC_E_MEMORY,(_BSL_SOCDNX_MSG("Failed to allocate %u bytes of DMA memory access Check.\n"), dma_size));
    }

    /* filling buffer for writing */
    for (i = 0; i < SOC_JER_REGS_ACCESS_CHECK_DMA_MEM_ENTRY_COUNT; ++i) {
        for (j = 0; j < entry_size; ++j) {
            mem_ptr[i*entry_size + j] = SOC_JER_REGS_ACCESS_CHECK_REF_TEST_VALUE + i;                
        }
    }
     
    /* Writing then Reading using DMA */ 
    SOCDNX_IF_ERR_EXIT( soc_mem_write_range(unit, test_mem, MEM_BLOCK_ANY, 0, SOC_JER_REGS_ACCESS_CHECK_DMA_MEM_ENTRY_COUNT - 1, (void*)mem_ptr));
    sal_memset(mem_ptr, 0, dma_size);
    SOCDNX_IF_ERR_EXIT( soc_mem_read_range(unit, test_mem, MEM_BLOCK_ANY, 0, SOC_JER_REGS_ACCESS_CHECK_DMA_MEM_ENTRY_COUNT - 1, (void*)mem_ptr));

    /* Checking result, every entry should be checked with mask. */
    for (i = 0; i < SOC_JER_REGS_ACCESS_CHECK_DMA_MEM_ENTRY_COUNT; ++i) {
        for (j = 0; j < entry_size; ++j) {
            if ((mem_ptr[i*entry_size + j] & mask[j]) != ((SOC_JER_REGS_ACCESS_CHECK_REF_TEST_VALUE + i) & mask[j])) {
                LOG_DEBUG(BSL_LS_SOC_INIT, (BSL_META_U(unit, "entry %d: received:%x \t expected:%x\n"), i, mem_ptr[i*entry_size + j] & mask[j], (SOC_JER_REGS_ACCESS_CHECK_REF_TEST_VALUE + i) & mask[j]));
                SOCDNX_EXIT_WITH_ERR(SOC_E_FAIL, (_BSL_SOCDNX_MSG("Problem with Block access - Failed accessing DMA\n")));      
            }
        }
    }

exit:
    if (mem_ptr != NULL) {
        soc_cm_sfree(unit, mem_ptr); 
    }
    SOCDNX_FUNC_RETURN;
}

/* Check access to few random blocks IDR, IHB, IQM, EGQ and IRR blocks */
int soc_jer_regs_blocks_access_check(int unit)
{
    int use_dma;
    SOCDNX_INIT_FUNC_DEFS;

    /* check if we can use DMA */
    use_dma =
#ifdef PLISIM
    SAL_BOOT_PLISIM ? 0 :
#endif /* PLISIM */
      soc_mem_dmaable(unit, IRR_MCDBm, SOC_MEM_BLOCK_ANY(unit, IRR_MCDBm)); 

    /* Check access to blocks */
    SOCDNX_IF_ERR_EXIT(soc_jer_regs_blocks_access_check_regs(unit));
    SOCDNX_IF_ERR_EXIT(soc_jer_regs_blocks_access_check_mem(unit, CFC_NIF_PFC_MAPm, MEM_BLOCK_ANY));
    if (!soc_feature(unit, soc_feature_no_fabric)) {
        SOCDNX_IF_ERR_EXIT(soc_jer_regs_blocks_access_check_mem(unit, FDT_IPT_MESH_MCm, MEM_BLOCK_ANY));
    }

    if (use_dma) 
    {
        SOCDNX_IF_ERR_EXIT(soc_jer_regs_blocks_access_check_dma(unit));
    }



exit:
    SOCDNX_FUNC_RETURN;
}

/* 
 * Initialize the broadcast IDs of the blocks
 */
int soc_jer_init_brdc_blk_id_set(int unit)
{
    int index;

    SOCDNX_INIT_FUNC_DEFS;

    /* fix soc_dpp_defines_t in case of QMX */ 
    if (SOC_IS_QMX(unit)) {
        SOC_DPP_DEFS_SET(unit, nof_fabric_links, 16); 
        SOC_DPP_DEFS_SET(unit, nof_instances_fmac, 4);
    }

    /* Init broadcast in FMAC */
    if (SOC_DPP_DEFS_GET(unit, nof_instances_fmac) > 0)
    {
        for (index=0 ; index < SOC_DPP_DEFS_GET(unit, nof_instances_fmac) - 1 ; index++) {
            SOCDNX_IF_ERR_EXIT(WRITE_FMAC_SBUS_BROADCAST_IDr(unit, index, SOC_BLOCK_INFO(unit,BRDC_FMAC_BLOCK(unit)).schan));
            SOCDNX_IF_ERR_EXIT(WRITE_FMAC_SBUS_LAST_IN_CHAINr(unit, index, 0));
        }
        SOCDNX_IF_ERR_EXIT(WRITE_FMAC_SBUS_BROADCAST_IDr(unit, index, SOC_BLOCK_INFO(unit,BRDC_FMAC_BLOCK(unit)).schan));
        SOCDNX_IF_ERR_EXIT(WRITE_FMAC_SBUS_LAST_IN_CHAINr(unit, index, 1));
    }

    if (!SOC_IS_QAX(unit)) {
        /* Init broadcast in FSRD */
        SOCDNX_IF_ERR_EXIT(MBCM_DPP_SOC_DRIVER_CALL(unit,mbcm_dpp_brdc_fsrd_blk_id_set,(unit)));

        /* Init broadcast in CGM */
        SOCDNX_IF_ERR_EXIT(WRITE_CGM_SBUS_BROADCAST_IDr(unit, 0, SOC_BLOCK_INFO(unit,BRDC_CGM_BLOCK(unit)).schan));
        SOCDNX_IF_ERR_EXIT(WRITE_CGM_SBUS_BROADCAST_IDr(unit, 1, SOC_BLOCK_INFO(unit,BRDC_CGM_BLOCK(unit)).schan));
        SOCDNX_IF_ERR_EXIT(WRITE_CGM_SBUS_LAST_IN_CHAINr(unit, 0, 0));
        SOCDNX_IF_ERR_EXIT(WRITE_CGM_SBUS_LAST_IN_CHAINr(unit, 1, 1));

        /* Init broadcast in EGQ */
        SOCDNX_IF_ERR_EXIT(WRITE_EGQ_SBUS_BROADCAST_IDr(unit, 0, SOC_BLOCK_INFO(unit,BRDC_EGQ_BLOCK(unit)).schan));
        SOCDNX_IF_ERR_EXIT(WRITE_EGQ_SBUS_BROADCAST_IDr(unit, 1, SOC_BLOCK_INFO(unit,BRDC_EGQ_BLOCK(unit)).schan));
        SOCDNX_IF_ERR_EXIT(WRITE_EGQ_SBUS_LAST_IN_CHAINr(unit, 0, 0));
        SOCDNX_IF_ERR_EXIT(WRITE_EGQ_SBUS_LAST_IN_CHAINr(unit, 1, 1));

        /* Init broadcast in EPNI */
        SOCDNX_IF_ERR_EXIT(WRITE_EPNI_SBUS_BROADCAST_IDr(unit, 0, SOC_BLOCK_INFO(unit,BRDC_EPNI_BLOCK(unit)).schan));
        SOCDNX_IF_ERR_EXIT(WRITE_EPNI_SBUS_BROADCAST_IDr(unit, 1, SOC_BLOCK_INFO(unit,BRDC_EPNI_BLOCK(unit)).schan));
        SOCDNX_IF_ERR_EXIT(WRITE_EPNI_SBUS_LAST_IN_CHAINr(unit, 0, 0));
        SOCDNX_IF_ERR_EXIT(WRITE_EPNI_SBUS_LAST_IN_CHAINr(unit, 1, 1));

        /* Init broadcast in IHB */
        SOCDNX_IF_ERR_EXIT(WRITE_IHB_SBUS_BROADCAST_IDr(unit, 0, SOC_BLOCK_INFO(unit,BRDC_IHB_BLOCK(unit)).schan));
        SOCDNX_IF_ERR_EXIT(WRITE_IHB_SBUS_BROADCAST_IDr(unit, 1, SOC_BLOCK_INFO(unit,BRDC_IHB_BLOCK(unit)).schan));
        /* last in chain reversed in IHB */
        SOCDNX_IF_ERR_EXIT(WRITE_IHB_SBUS_LAST_IN_CHAINr(unit, 0, 1));
        SOCDNX_IF_ERR_EXIT(WRITE_IHB_SBUS_LAST_IN_CHAINr(unit, 1, 0));

        /* Init broadcast in IHP */
        SOCDNX_IF_ERR_EXIT(WRITE_IHP_SBUS_BROADCAST_IDr(unit, 0, SOC_BLOCK_INFO(unit,BRDC_IHP_BLOCK(unit)).schan));
        SOCDNX_IF_ERR_EXIT(WRITE_IHP_SBUS_BROADCAST_IDr(unit, 1, SOC_BLOCK_INFO(unit,BRDC_IHP_BLOCK(unit)).schan));
        /* last in chain reversed in IHP */
        SOCDNX_IF_ERR_EXIT(WRITE_IHP_SBUS_LAST_IN_CHAINr(unit, 0, 1));
        SOCDNX_IF_ERR_EXIT(WRITE_IHP_SBUS_LAST_IN_CHAINr(unit, 1, 0));

        /* Init broadcast in IPS */
        SOCDNX_IF_ERR_EXIT(WRITE_IPS_SBUS_BROADCAST_IDr(unit, 0, SOC_BLOCK_INFO(unit,BRDC_IPS_BLOCK(unit)).schan));
        SOCDNX_IF_ERR_EXIT(WRITE_IPS_SBUS_BROADCAST_IDr(unit, 1, SOC_BLOCK_INFO(unit,BRDC_IPS_BLOCK(unit)).schan));
        SOCDNX_IF_ERR_EXIT(WRITE_IPS_SBUS_LAST_IN_CHAINr(unit, 0, 0));
        SOCDNX_IF_ERR_EXIT(WRITE_IPS_SBUS_LAST_IN_CHAINr(unit, 1, 1));


        if (SOC_IS_JERICHO_PLUS(unit)) {
            SOCDNX_IF_ERR_EXIT(soc_reg32_set(unit, KAPS_BBS_SBUS_BROADCAST_IDr, 0, 0, SOC_BLOCK_INFO(unit,BRDC_KAPS_BBS_BLOCK(unit)).schan));
            SOCDNX_IF_ERR_EXIT(soc_reg32_set(unit, KAPS_BBS_SBUS_BROADCAST_IDr, 1, 0, SOC_BLOCK_INFO(unit,BRDC_KAPS_BBS_BLOCK(unit)).schan));
            SOCDNX_IF_ERR_EXIT(soc_reg32_set(unit, KAPS_BBS_SBUS_BROADCAST_IDr, 2, 0, SOC_BLOCK_INFO(unit,BRDC_KAPS_BBS_BLOCK(unit)).schan));
            SOCDNX_IF_ERR_EXIT(soc_reg32_set(unit, KAPS_BBS_SBUS_LAST_IN_CHAINr, 0, 0, 0));
            SOCDNX_IF_ERR_EXIT(soc_reg32_set(unit, KAPS_BBS_SBUS_LAST_IN_CHAINr, 1, 0, 0));
            SOCDNX_IF_ERR_EXIT(soc_reg32_set(unit, KAPS_BBS_SBUS_LAST_IN_CHAINr, 2, 0, 1));
        }

        /* Init broadcast in IQM */
        SOCDNX_IF_ERR_EXIT(WRITE_IQM_SBUS_BROADCAST_IDr(unit, 0, SOC_BLOCK_INFO(unit, BRDC_IQM_BLOCK(unit)).schan));
        SOCDNX_IF_ERR_EXIT(WRITE_IQM_SBUS_BROADCAST_IDr(unit, 1, SOC_BLOCK_INFO(unit, BRDC_IQM_BLOCK(unit)).schan));
        SOCDNX_IF_ERR_EXIT(WRITE_IQM_SBUS_LAST_IN_CHAINr(unit, 0, 0));
        SOCDNX_IF_ERR_EXIT(WRITE_IQM_SBUS_LAST_IN_CHAINr(unit, 1, 1));

        /* Init broadcast in SCH */
        SOCDNX_IF_ERR_EXIT(WRITE_SCH_SBUS_BROADCAST_IDr(unit, 0, SOC_BLOCK_INFO(unit,BRDC_SCH_BLOCK(unit)).schan));
        SOCDNX_IF_ERR_EXIT(WRITE_SCH_SBUS_BROADCAST_IDr(unit, 1, SOC_BLOCK_INFO(unit,BRDC_SCH_BLOCK(unit)).schan));
        SOCDNX_IF_ERR_EXIT(WRITE_SCH_SBUS_LAST_IN_CHAINr(unit, 0, 0));
        SOCDNX_IF_ERR_EXIT(WRITE_SCH_SBUS_LAST_IN_CHAINr(unit, 1, 1));

        if(SOC_DPP_CONFIG(unit)->arad->init.drc_info.dram_num != 0){

            /* Init broadcast in DRC */
            SOCDNX_IF_ERR_EXIT(WRITE_DRCA_SBUS_BROADCAST_IDr(unit, SOC_BLOCK_INFO(unit,DRCBROADCAST_BLOCK(unit)).schan));
            SOCDNX_IF_ERR_EXIT(WRITE_DRCB_SBUS_BROADCAST_IDr(unit, SOC_BLOCK_INFO(unit,DRCBROADCAST_BLOCK(unit)).schan));
            SOCDNX_IF_ERR_EXIT(WRITE_DRCC_SBUS_BROADCAST_IDr(unit, SOC_BLOCK_INFO(unit,DRCBROADCAST_BLOCK(unit)).schan));
            SOCDNX_IF_ERR_EXIT(WRITE_DRCD_SBUS_BROADCAST_IDr(unit, SOC_BLOCK_INFO(unit,DRCBROADCAST_BLOCK(unit)).schan));
            SOCDNX_IF_ERR_EXIT(WRITE_DRCE_SBUS_BROADCAST_IDr(unit, SOC_BLOCK_INFO(unit,DRCBROADCAST_BLOCK(unit)).schan));
            SOCDNX_IF_ERR_EXIT(WRITE_DRCF_SBUS_BROADCAST_IDr(unit, SOC_BLOCK_INFO(unit,DRCBROADCAST_BLOCK(unit)).schan));
            SOCDNX_IF_ERR_EXIT(WRITE_DRCG_SBUS_BROADCAST_IDr(unit, SOC_BLOCK_INFO(unit,DRCBROADCAST_BLOCK(unit)).schan));
            SOCDNX_IF_ERR_EXIT(WRITE_DRCH_SBUS_BROADCAST_IDr(unit, SOC_BLOCK_INFO(unit,DRCBROADCAST_BLOCK(unit)).schan));

            /* Last in chain - DRC */
            SOCDNX_IF_ERR_EXIT(WRITE_DRCA_REG_0087r(unit, 0));
            SOCDNX_IF_ERR_EXIT(WRITE_DRCB_REG_0087r(unit, 0));
            SOCDNX_IF_ERR_EXIT(WRITE_DRCC_REG_0087r(unit, 0));
            SOCDNX_IF_ERR_EXIT(WRITE_DRCD_REG_0087r(unit, 0));
            SOCDNX_IF_ERR_EXIT(WRITE_DRCE_REG_0087r(unit, 0));
            SOCDNX_IF_ERR_EXIT(WRITE_DRCF_REG_0087r(unit, 0));
            SOCDNX_IF_ERR_EXIT(WRITE_DRCG_REG_0087r(unit, 0));
            SOCDNX_IF_ERR_EXIT(WRITE_DRCH_REG_0087r(unit, 1));
        }
    }

exit:
    SOCDNX_FUNC_RETURN;
}

STATIC int soc_jer_fsrd_block_enable_get(int unit, int fsrd_block, int *enable)
{

    uint32 field_val;
    uint64 reg_val64;
    SOCDNX_INIT_FUNC_DEFS;

    SOCDNX_IF_ERR_EXIT(READ_ECI_BLOCKS_POWER_DOWNr(unit, &reg_val64));

    switch (fsrd_block)
    {
        case 0:
            field_val = soc_reg64_field32_get(unit, ECI_BLOCKS_POWER_DOWNr, reg_val64, BLOCKS_POWER_DOWN_8f);
            break;
        case 1:
            field_val = soc_reg64_field32_get(unit, ECI_BLOCKS_POWER_DOWNr, reg_val64, BLOCKS_POWER_DOWN_9f);
            break;
        case 2:
            field_val = soc_reg64_field32_get(unit, ECI_BLOCKS_POWER_DOWNr, reg_val64, BLOCKS_POWER_DOWN_10f);
            break;
        case 3:
            field_val = soc_reg64_field32_get(unit, ECI_BLOCKS_POWER_DOWNr, reg_val64, BLOCKS_POWER_DOWN_11f);
            break;
        default:
            SOCDNX_EXIT_WITH_ERR(SOC_E_INTERNAL, (_BSL_SOCDNX_MSG("invalid fsrd block")));
            break;
    }

    *enable = (field_val == 0) ? 1 : 0;


exit:
    SOCDNX_FUNC_RETURN;
}

/*
 * Function:
 *      soc_jer_brdc_fsrd_blk_id_set
 * Purpose:
 *      Checks if any of the Fabric Quads are disabled and according to that sets the BROADCAST_ID and LAST_IN_CHAIN for the FSRD
 * Parameters:
 *      unit            - Device Number
 * Returns:
 *      SOC_E_XXX
 */
int soc_jer_brdc_fsrd_blk_id_set(int unit){

    int index;
    int last_in_chain=-1;
    int enable;

    SOCDNX_INIT_FUNC_DEFS;

    for (index=0 ; index < SOC_DPP_DEFS_GET(unit, nof_instances_fsrd); index++) {
        SOCDNX_IF_ERR_EXIT(soc_jer_fsrd_block_enable_get(unit, index, &enable));
        if (!enable) {
            continue;
        } else {
            SOCDNX_IF_ERR_EXIT(WRITE_FSRD_SBUS_LAST_IN_CHAINr(unit, index, 0));
            SOCDNX_IF_ERR_EXIT(WRITE_FSRD_SBUS_BROADCAST_IDr(unit, index, SOC_BLOCK_INFO(unit,BRDC_FSRD_BLOCK(unit)).schan));
            last_in_chain=index;
        }
    }

    if (last_in_chain >= 0)
    {
        SOCDNX_IF_ERR_EXIT(WRITE_FSRD_SBUS_LAST_IN_CHAINr(unit, (last_in_chain), 1));
    }

exit:
    SOCDNX_FUNC_RETURN;
}

/* Must be at end of file */
#undef _ERR_MSG_MODULE_NAME

