/*
 * $Id: shmoo_combo16.c,v 1.1.2.21 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * DDR4 & GDDR5 Memory Support
 */

#include <shared/bsl.h>

#include <sal/core/boot.h>
#include <sal/core/libc.h>
#include <shared/alloc.h>
#include <soc/memtune.h>

#include <soc/drv.h>
#include <soc/cm.h>

#ifdef BCM_CMICM_SUPPORT
#include <soc/cmicm.h>
#endif

/* BEGIN: TEMPORARY */
#ifndef BCM_DDRC16_SUPPORT
#define BCM_DDRC16_SUPPORT
#endif
/* END: TEMPORARY */

#ifdef BCM_DDRC16_SUPPORT
#include <soc/ddrc16.h>
#include <soc/shmoo_combo16.h>

#ifdef _ERR_MSG_MODULE_NAME 
#error "_ERR_MSG_MODULE_NAME redefined" 
#endif
#define _ERR_MSG_MODULE_NAME BSL_LS_SOC_SHMOO

soc_combo16_phy_reg_read_t soc_combo16_phy_reg_read = NULL;
soc_combo16_phy_reg_write_t soc_combo16_phy_reg_write = NULL;
soc_combo16_phy_reg_modify_t soc_combo16_phy_reg_modify = NULL;
static _shmoo_combo16_drc_bist_conf_set_t _shmoo_combo16_drc_bist_conf_set = NULL;
static _shmoo_combo16_drc_bist_err_cnt_t _shmoo_combo16_drc_bist_err_cnt = NULL;
static _shmoo_combo16_drc_dram_init_t _shmoo_combo16_drc_dram_init = NULL;
static _shmoo_combo16_drc_pll_set_t _shmoo_combo16_drc_pll_set = NULL;
static _shmoo_combo16_drc_modify_mrs_t _shmoo_combo16_drc_modify_mrs = NULL;
static _shmoo_combo16_drc_enable_adt_t _shmoo_combo16_drc_enable_adt = NULL;
static _shmoo_combo16_drc_enable_wck2ck_training_t _shmoo_combo16_drc_enable_wck2ck_training = NULL;
static _shmoo_combo16_drc_enable_write_leveling_t _shmoo_combo16_drc_enable_write_leveling = NULL;
static _shmoo_combo16_drc_mpr_en_t _shmoo_combo16_drc_mpr_en = NULL;
static _shmoo_combo16_drc_mpr_load_t _shmoo_combo16_drc_mpr_load = NULL;
static _shmoo_combo16_drc_enable_gddr5_training_protocol_t _shmoo_combo16_drc_enable_gddr5_training_protocol = NULL;
static _shmoo_combo16_drc_vendor_info_get_t _shmoo_combo16_drc_vendor_info_get = NULL;
static _shmoo_combo16_drc_dqs_pulse_gen_t _shmoo_combo16_drc_dqs_pulse_gen = NULL;
static _shmoo_combo16_drc_gddr5_bist_conf_set_t _shmoo_combo16_drc_gddr5_bist_conf_set = NULL;
static _shmoo_combo16_drc_gddr5_bist_err_cnt_t _shmoo_combo16_drc_gddr5_bist_err_cnt = NULL;
static _shmoo_combo16_drc_gddr5_dq_byte_pairs_swap_info_get_t _shmoo_combo16_drc_gddr5_dq_byte_pairs_swap_info_get = NULL;
static _shmoo_combo16_drc_enable_wr_crc_t _shmoo_combo16_drc_enable_wr_crc = NULL;
static _shmoo_combo16_drc_enable_rd_crc_t _shmoo_combo16_drc_enable_rd_crc = NULL;
static _shmoo_combo16_drc_enable_wr_dbi_t _shmoo_combo16_drc_enable_wr_dbi = NULL;
static _shmoo_combo16_drc_enable_rd_dbi_t _shmoo_combo16_drc_enable_rd_dbi = NULL;
static _shmoo_combo16_drc_enable_refresh_t _shmoo_combo16_drc_enable_refresh = NULL;
static _shmoo_combo16_drc_force_dqs_t _shmoo_combo16_drc_force_dqs = NULL;
static _shmoo_combo16_drc_soft_reset_drc_without_dram_t _shmoo_combo16_drc_soft_reset_drc_without_dram = NULL;
static _shmoo_combo16_drc_dram_info_access_t _shmoo_combo16_drc_dram_info_access = NULL;
static _shmoo_combo16_drc_vendor_info_access_t _shmoo_combo16_drc_vendor_info_access = NULL;

const uint32 shmoo_order_combo16_ddr4[SHMOO_COMBO16_DDR4_SEQUENCE_COUNT] =
{
    SHMOO_COMBO16_DQS2CK,
    SHMOO_COMBO16_RD_EN_FISH,
    SHMOO_COMBO16_RD_SHORT,
    SHMOO_COMBO16_WR_DESKEW,
    SHMOO_COMBO16_WR_EXTENDED,
    SHMOO_COMBO16_RD_DESKEW,
    SHMOO_COMBO16_RD_EXTENDED,
    SHMOO_COMBO16_ADDR_CTRL_EXTENDED
};

const uint32 shmoo_order_combo16_gddr5[SHMOO_COMBO16_GDDR5_SEQUENCE_COUNT] =
{
    SHMOO_COMBO16_ADDR_CTRL_SHORT,
    SHMOO_COMBO16_WCK2CK,
    SHMOO_COMBO16_RD_START_FISH,
    SHMOO_COMBO16_EDC_START_FISH,
    SHMOO_COMBO16_RD_DESKEW,
    SHMOO_COMBO16_DBI_EDC_RD_DESKEW,
    SHMOO_COMBO16_RD_SHORT,
    SHMOO_COMBO16_WR_DESKEW,
    SHMOO_COMBO16_DBI_WR_DESKEW,
    SHMOO_COMBO16_WR_SHORT,
    SHMOO_COMBO16_RD_EXTENDED,
    SHMOO_COMBO16_WR_EXTENDED
};

const uint32 shmoo_order_combo16_gddr5_restore[SHMOO_COMBO16_GDDR5_RESTORE_SEQUENCE_COUNT] =
{
    SHMOO_COMBO16_WCK2CK,
    SHMOO_COMBO16_RD_START_FISH,
    SHMOO_COMBO16_EDC_START_FISH,
    SHMOO_COMBO16_RD_SHORT,
    SHMOO_COMBO16_WR_SHORT,
    SHMOO_COMBO16_RD_EXTENDED,
    SHMOO_COMBO16_WR_EXTENDED
};

const int shmoo_combo16_cdr_adjustments[SHMOO_COMBO16_CDR_ADJUSTMENT_COUNT][3] =
{
    {0, 0, 0},
    {10, 10, 10},
    {-10, -10, -10},
    {20, 20, 20},
    {-20, -20, -20},
    {0, 0, 0}
};

/*
const uint32 shmoo_combo16_cdr_adjustments[SHMOO_COMBO16_CDR_ADJUSTMENT_COUNT][3] =
{
    {0, 0, 0},
    {0, 0, 1},
    {0, 1, 1},
    {1, 0, 1},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
    {0, 0, 2},
    {0, 1, 2},
    {1, 0, 2},
    {0, 2, 2},
    {2, 0, 2},
    {0, 2, 0},
    {0, 2, 1},
    {1, 2, 0},
    {2, 2, 0},
    {2, 0, 0},
    {2, 0, 1},
    {2, 1, 0},
    {0, 0, 3},
    {0, 1, 3},
    {1, 0, 3},
    {0, 2, 3},
    {2, 0, 3},
    {0, 3, 3},
    {3, 0, 3},
    {0, 3, 0},
    {0, 3, 1},
    {1, 3, 0},
    {0, 3, 2},
    {2, 3, 0},
    {3, 3, 0},
    {3, 0, 0},
    {3, 0, 1},
    {3, 1, 0},
    {3, 0, 2},
    {3, 2, 0},
    {0, 0, 4},
    {0, 1, 4},
    {1, 0, 4},
    {0, 2, 4},
    {2, 0, 4},
    {0, 3, 4},
    {3, 0, 4},
    {0, 4, 4},
    {4, 0, 4},
    {0, 4, 0},
    {0, 4, 1},
    {1, 4, 0},
    {0, 4, 2},
    {2, 4, 0},
    {0, 4, 3},
    {3, 4, 0},
    {4, 4, 0},
    {4, 0, 0},
    {4, 0, 1},
    {4, 1, 0},
    {4, 0, 2},
    {4, 2, 0},
    {4, 0, 3},
    {4, 3, 0},
    {0, 0, 5},
    {0, 1, 5},
    {1, 0, 5},
    {0, 2, 5},
    {2, 0, 5},
    {0, 3, 5},
    {3, 0, 5},
    {0, 4, 5},
    {4, 0, 5},
    {0, 5, 5},
    {5, 0, 5},
    {0, 5, 0},
    {0, 5, 1},
    {1, 5, 0},
    {0, 5, 2},
    {2, 5, 0},
    {0, 5, 3},
    {3, 5, 0},
    {0, 5, 4},
    {4, 5, 0},
    {5, 5, 0},
    {5, 0, 0},
    {5, 0, 1},
    {5, 1, 0},
    {5, 0, 2},
    {5, 2, 0},
    {5, 0, 3},
    {5, 3, 0},
    {5, 0, 4},
    {5, 4, 0}
};
*/

/* BEGIN: HELPER FUNCTIONS */
STATIC uint32
_combo16_get_random(void)
{
    static uint32 m_w = 6483;       /* must not be zero */
    static uint32 m_z = 31245;      /* must not be zero */
    
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;       /* 32-bit result */
}
/* END: HELPER FUNCTIONS */

uint32
_shmoo_combo16_check_dram(int unit, uint32 drc_ndx)
{
    combo16_shmoo_dram_info_t* shmoo_dram_info;
    SOC_IF_ERROR_RETURN( _shmoo_combo16_drc_dram_info_access(unit, &shmoo_dram_info));
    return ((shmoo_dram_info->dram_bitmap >> drc_ndx) & 0x1);
}


uint32
_shmoo_combo16_validate_config(int unit)
{
    uint32 ndx;
    combo16_shmoo_dram_info_t* shmoo_dram_info;
    SOC_IF_ERROR_RETURN( _shmoo_combo16_drc_dram_info_access(unit, &shmoo_dram_info));

    for(ndx = 0; ndx < SHMOO_COMBO16_MAX_INTERFACES; ndx++)
    {
        if(!_shmoo_combo16_check_dram(unit, ndx))
        {
            continue;
        }

        if(shmoo_dram_info->zq_cal_array[ndx] == 0xff)
        {
            return 0;
        }
    }

    return  (
                (shmoo_dram_info->ctl_type != 0)
                && (shmoo_dram_info->dram_type != 0)
                && (shmoo_dram_info->num_columns != -1)
                && (shmoo_dram_info->num_rows != -1)
                && (shmoo_dram_info->num_banks != -1)
                && (shmoo_dram_info->data_rate_mbps != -1)
                && (shmoo_dram_info->ref_clk_mhz != -1)
            /*  && (shmoo_dram_info->refi != 0) */
            /*  && (shmoo_dram_info->command_parity_latency != 0) */
            /*  && (shmoo_dram_info->sim_system_mode != 0) */
                && (shmoo_dram_info->ref_clk_bitmap != 0)
            );
}

uint32
_shmoo_combo16_validate_cbi(void)
{
    return  (
                (soc_combo16_phy_reg_read != NULL)
                && (soc_combo16_phy_reg_write != NULL)
                && (soc_combo16_phy_reg_modify != NULL)
                && (_shmoo_combo16_drc_bist_conf_set != NULL)
                && (_shmoo_combo16_drc_bist_err_cnt != NULL)
                && (_shmoo_combo16_drc_dram_init != NULL)
                && (_shmoo_combo16_drc_pll_set != NULL)
                && (_shmoo_combo16_drc_modify_mrs != NULL)
                && (_shmoo_combo16_drc_enable_adt != NULL)
                && (_shmoo_combo16_drc_enable_wck2ck_training != NULL)
                && (_shmoo_combo16_drc_enable_write_leveling != NULL)
                && (_shmoo_combo16_drc_mpr_en != NULL)
                && (_shmoo_combo16_drc_mpr_load != NULL)
                && (_shmoo_combo16_drc_enable_gddr5_training_protocol != NULL)
                && (_shmoo_combo16_drc_vendor_info_get != NULL)
                && (_shmoo_combo16_drc_dqs_pulse_gen != NULL)
                && (_shmoo_combo16_drc_gddr5_bist_conf_set != NULL)
                && (_shmoo_combo16_drc_gddr5_bist_err_cnt != NULL)
                && (_shmoo_combo16_drc_gddr5_dq_byte_pairs_swap_info_get != NULL)
                && (_shmoo_combo16_drc_enable_wr_crc != NULL)
                && (_shmoo_combo16_drc_enable_rd_crc != NULL)
                && (_shmoo_combo16_drc_enable_wr_dbi != NULL)
                && (_shmoo_combo16_drc_enable_rd_dbi != NULL)
                && (_shmoo_combo16_drc_enable_refresh != NULL)
                && (_shmoo_combo16_drc_force_dqs != NULL)
                && (_shmoo_combo16_drc_soft_reset_drc_without_dram != NULL)
                && (_shmoo_combo16_drc_dram_info_access != NULL)
                && (_shmoo_combo16_drc_vendor_info_access != NULL)
            );
}

STATIC int
_combo16_initialize_bist(int unit, int drc_ndx, int bit, combo16_shmoo_container_t *scPtr, combo16_bist_info_t *biPtr, combo16_gddr5_bist_info_t *gbiPtr)
{
    uint32 i;
    uint8 dbi_pattern;
    uint8 edc_pattern;
    combo16_vendor_info_t* shmoo_vendor_info;

    SOC_IF_ERROR_RETURN( _shmoo_combo16_drc_vendor_info_access(unit, &shmoo_vendor_info));    
        
    switch((*scPtr).shmooType)
    {
        case SHMOO_COMBO16_ADDR_CTRL_SHORT:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                (*gbiPtr).fifo_depth = 0;
                (*gbiPtr).num_commands = 512;
                (*gbiPtr).bist_mode = SHMOO_COMBO16_GDDR5_BIST_ADDR_TRAINING_MODE;
                
                for(i = 0; i < SHMOO_COMBO16_BIST_NOF_PATTERNS; i++)
                {
                    COMPILER_64_SET((*gbiPtr).data_pattern[i], 0xFFFFFFFF, 0xFFFFFFFF);
                    (*gbiPtr).dbi_pattern[i] = 0xFF;
                    (*gbiPtr).edc_pattern[i] = 0xFF;
                }
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_DQS2CK:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                /* NO INIT TASK */
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_WCK2CK:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                /* NO INIT TASK */
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_EN_FISH:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                (*biPtr).write_weight = 0;
                (*biPtr).read_weight = 1;
                (*biPtr).bist_timer_us = 0;
                (*biPtr).bist_num_actions = 1;
                (*biPtr).bist_start_address = 0x00000000;
                (*biPtr).bist_end_address = 0x00FFFFFF;
                (*biPtr).mpr_mode = 0;
                (*biPtr).prbs_mode = 0;
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_START_FISH:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                (*gbiPtr).fifo_depth = shmoo_vendor_info->fifo_depth;
                (*gbiPtr).num_commands = shmoo_vendor_info->fifo_depth;
                (*gbiPtr).bist_mode = SHMOO_COMBO16_GDDR5_BIST_LOAD_READ_FIFO_MODE;
                
                for(i = 0; i < SHMOO_COMBO16_BIST_NOF_PATTERNS; i++)
                {
                    COMPILER_64_SET((*gbiPtr).data_pattern[i], _combo16_get_random(), _combo16_get_random());
                    (*gbiPtr).dbi_pattern[i]  = (uint8) (_combo16_get_random() & 0xFF);
                    (*gbiPtr).edc_pattern[i]  = (uint8) (_combo16_get_random() & 0xFF);
                }
                
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_conf_set(unit, drc_ndx, (*gbiPtr)));
                
                (*gbiPtr).bist_mode = SHMOO_COMBO16_GDDR5_BIST_READ_FROM_PREV_LOADED_GDDR5_READ_FIFO;
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_EDC_START_FISH:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                (*gbiPtr).fifo_depth = shmoo_vendor_info->fifo_depth;
                (*gbiPtr).num_commands = shmoo_vendor_info->fifo_depth;
                (*gbiPtr).bist_mode = SHMOO_COMBO16_GDDR5_BIST_LOAD_READ_FIFO_MODE;
                
                for(i = 0; i < SHMOO_COMBO16_BIST_NOF_PATTERNS; i++)
                {
                    COMPILER_64_SET((*gbiPtr).data_pattern[i], _combo16_get_random(), _combo16_get_random());
                    (*gbiPtr).dbi_pattern[i]  = (uint8) (_combo16_get_random() & 0xFF);
                    (*gbiPtr).edc_pattern[i]  = (uint8) (_combo16_get_random() & 0xFF);
                }
                
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_conf_set(unit, drc_ndx, (*gbiPtr)));
                
                (*gbiPtr).bist_mode = SHMOO_COMBO16_GDDR5_BIST_READ_FROM_PREV_LOADED_GDDR5_READ_FIFO;
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                (*gbiPtr).fifo_depth = shmoo_vendor_info->fifo_depth;
                (*gbiPtr).num_commands = shmoo_vendor_info->fifo_depth;
                (*gbiPtr).bist_mode = SHMOO_COMBO16_GDDR5_BIST_LOAD_READ_FIFO_MODE;
                
                for(i = 0; i < SHMOO_COMBO16_BIST_NOF_PATTERNS; i++)
                {
                    COMPILER_64_SET((*gbiPtr).data_pattern[i], 0xFFFFFFFF, ~(0x01010101 << bit));
                    (*gbiPtr).dbi_pattern[i] = 0xFF;
                    (*gbiPtr).edc_pattern[i] = 0xFF;
                }
                
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_conf_set(unit, drc_ndx, (*gbiPtr)));
                
                (*gbiPtr).num_commands = (shmoo_vendor_info->fifo_depth) << 5;
                (*gbiPtr).bist_mode = SHMOO_COMBO16_GDDR5_BIST_READ_FROM_PREV_LOADED_GDDR5_READ_FIFO;
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                (*biPtr).write_weight = 255;
                (*biPtr).read_weight = 255;
                (*biPtr).bist_timer_us = 0;
                (*biPtr).bist_num_actions = 510;
                (*biPtr).bist_start_address = 0x00000000;
                (*biPtr).bist_end_address = 0x00FFFFFF;
                (*biPtr).mpr_mode = 0;
                (*biPtr).prbs_mode = 0;
                
                for(i = 0; i < SHMOO_COMBO16_BIST_MPR_NOF_PATTERNS; i+=2)
                {
                    (*biPtr).data_pattern[i] = ~(0x01010101 << bit);
                }
                for(i = 1; i < SHMOO_COMBO16_BIST_MPR_NOF_PATTERNS; i+=2)
                {
                    (*biPtr).data_pattern[i] = 0xFFFFFFFF;
                }
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_DBI_EDC_RD_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                (*gbiPtr).fifo_depth = shmoo_vendor_info->fifo_depth;
                (*gbiPtr).num_commands = shmoo_vendor_info->fifo_depth;
                (*gbiPtr).bist_mode = SHMOO_COMBO16_GDDR5_BIST_LOAD_READ_FIFO_MODE;
                
                if(bit)
                {
                    dbi_pattern = 0xFF;
                    edc_pattern = 0xF0;
                }
                else
                {
                    dbi_pattern = 0xF0;
                    edc_pattern = 0xFF;
                }
                
                for(i = 0; i < SHMOO_COMBO16_BIST_NOF_PATTERNS; i++)
                {
                    COMPILER_64_SET((*gbiPtr).data_pattern[i], 0xFFFFFFFF, 0xFFFFFFFF);
                    (*gbiPtr).dbi_pattern[i] = dbi_pattern;
                    (*gbiPtr).edc_pattern[i] = edc_pattern;
                }
                
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_conf_set(unit, drc_ndx, (*gbiPtr)));
                
                (*gbiPtr).num_commands = (shmoo_vendor_info->fifo_depth) << 5;
                (*gbiPtr).bist_mode = SHMOO_COMBO16_GDDR5_BIST_READ_FROM_PREV_LOADED_GDDR5_READ_FIFO;
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_SHORT:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                (*gbiPtr).fifo_depth = shmoo_vendor_info->fifo_depth;
                
                for(i = 0; i < SHMOO_COMBO16_BIST_NOF_PATTERNS; i++)
                {
                    (*gbiPtr).dbi_pattern[i] = 0xFF;
                    (*gbiPtr).edc_pattern[i] = 0xFF;
                }
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                (*biPtr).write_weight = 0;
                (*biPtr).read_weight = 255;
                (*biPtr).bist_timer_us = 0;
                (*biPtr).bist_num_actions = 255;
                (*biPtr).bist_start_address = 0x00000000;
                (*biPtr).bist_end_address = 0x00FFFFFF;
                (*biPtr).mpr_mode = 1;
                (*biPtr).prbs_mode = 0;
                
                for(i = 0; i < SHMOO_COMBO16_BIST_MPR_NOF_PATTERNS; i++)
                {
                    (*biPtr).mpr_pattern[i] = (uint8) (_combo16_get_random() & 0xFF);
                }

                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_mpr_load(unit, drc_ndx, (*biPtr).mpr_pattern));
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_WR_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                (*gbiPtr).fifo_depth = shmoo_vendor_info->fifo_depth;
                (*gbiPtr).num_commands = shmoo_vendor_info->fifo_depth << 5;
                (*gbiPtr).bist_mode = SHMOO_COMBO16_GDDR5_BIST_READ_WRITE_FROM_GDDR5_READ_FIFO;
                
                for(i = 0; i < SHMOO_COMBO16_BIST_NOF_PATTERNS; i++)
                {
                    COMPILER_64_SET((*gbiPtr).data_pattern[i], 0xFFFFFFFF, ~(0x01010101 << bit));
                    (*gbiPtr).dbi_pattern[i] = 0xFF;
                    (*gbiPtr).edc_pattern[i] = 0xFF;
                }
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                (*biPtr).write_weight = 255;
                (*biPtr).read_weight = 255;
                (*biPtr).bist_timer_us = 0;
                (*biPtr).bist_num_actions = 510;
                (*biPtr).bist_start_address = 0x00000000;
                (*biPtr).bist_end_address = 0x00FFFFFF;
                (*biPtr).mpr_mode = 0;
                (*biPtr).prbs_mode = 0;
                
                for(i = 0; i < SHMOO_COMBO16_BIST_MPR_NOF_PATTERNS; i+=2)
                {
                    (*biPtr).data_pattern[i] = ~(0x01010101 << bit);
                }
                for(i = 1; i < SHMOO_COMBO16_BIST_MPR_NOF_PATTERNS; i+=2)
                {
                    (*biPtr).data_pattern[i] = 0xFFFFFFFF;
                }
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_DBI_WR_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                (*gbiPtr).fifo_depth = shmoo_vendor_info->fifo_depth;
                (*gbiPtr).num_commands = shmoo_vendor_info->fifo_depth << 5;
                (*gbiPtr).bist_mode = SHMOO_COMBO16_GDDR5_BIST_READ_WRITE_FROM_GDDR5_READ_FIFO;
                
                for(i = 0; i < SHMOO_COMBO16_BIST_NOF_PATTERNS; i++)
                {
                    COMPILER_64_SET((*gbiPtr).data_pattern[i], 0xFFFFFFFF, 0xFFFFFFFF);
                    (*gbiPtr).dbi_pattern[i] = 0xF0;
                    (*gbiPtr).edc_pattern[i] = 0xFF;
                }
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_WR_SHORT:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                (*gbiPtr).fifo_depth = shmoo_vendor_info->fifo_depth;
                (*gbiPtr).num_commands = shmoo_vendor_info->fifo_depth << 5;
                (*gbiPtr).bist_mode = SHMOO_COMBO16_GDDR5_BIST_READ_WRITE_FROM_GDDR5_READ_FIFO;
                
                for(i = 0; i < SHMOO_COMBO16_BIST_NOF_PATTERNS; i++)
                {
                    (*gbiPtr).dbi_pattern[i] = 0xFF;
                    (*gbiPtr).edc_pattern[i] = 0xFF;
                }
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                (*biPtr).write_weight = 255;
                (*biPtr).read_weight = 255;
                (*biPtr).bist_timer_us = 0;
                (*biPtr).bist_num_actions = 510;
                (*biPtr).bist_start_address = 0x00000000;
                (*biPtr).bist_end_address = 0x00FFFFFF;
                (*biPtr).mpr_mode = 0;
                (*biPtr).prbs_mode = 1;
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_EXTENDED:
            (*biPtr).write_weight = 255;
            (*biPtr).read_weight = 255;
            (*biPtr).bist_timer_us = 0;
            (*biPtr).bist_num_actions = 51510;
            (*biPtr).bist_start_address = 0x00000000;
            (*biPtr).bist_end_address = 0x00FFFFFF;
            (*biPtr).mpr_mode = 0;
            (*biPtr).prbs_mode = 1;
            break;
        case SHMOO_COMBO16_WR_EXTENDED:
            (*biPtr).write_weight = 255;
            (*biPtr).read_weight = 255;
            (*biPtr).bist_timer_us = 0;
            (*biPtr).bist_num_actions = 51510;
            (*biPtr).bist_start_address = 0x00000000;
            (*biPtr).bist_end_address = 0x00FFFFFF;
            (*biPtr).mpr_mode = 0;
            (*biPtr).prbs_mode = 1;
            break;
        case SHMOO_COMBO16_ADDR_CTRL_EXTENDED:
            (*biPtr).write_weight = 255;
            (*biPtr).read_weight = 255;
            (*biPtr).bist_timer_us = 0;
            (*biPtr).bist_num_actions = 51510;
            (*biPtr).bist_start_address = 0x00000000;
            (*biPtr).bist_end_address = 0x00FFFFFF;
            (*biPtr).mpr_mode = 0;
            (*biPtr).prbs_mode = 1;
            break;
        default:
            LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported shmoo type: %02d\n"), (*scPtr).shmooType));
            return SOC_E_FAIL;
    }
    return SOC_E_NONE;
}

STATIC int
_combo16_run_bist(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr, combo16_bist_info_t *biPtr, combo16_gddr5_bist_info_t *gbiPtr, combo16_shmoo_error_array_t *seaPtr)
{
    uint32 i;
    combo16_bist_err_cnt_t be;
    combo16_gddr5_bist_err_cnt_t gbe;
    combo16_vendor_info_t* shmoo_vendor_info;
    
    switch((*scPtr).shmooType)
    {
        case SHMOO_COMBO16_ADDR_CTRL_SHORT:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_conf_set(unit, drc_ndx, (*gbiPtr)));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_err_cnt(unit, drc_ndx, &gbe));
                
                /* In GDDR5 do not look at bits 18 and 19, they are remnants of GDDR5M which is not used */  
                (*seaPtr)[0] = gbe.bist_adt_err_occur & 0x3ffff;

            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_DQS2CK:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                /* NO RUN TASK */
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_WCK2CK:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                /* NO RUN TASK */
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_EN_FISH:
            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_bist_conf_set(unit, drc_ndx, (*biPtr)));
            break;
        case SHMOO_COMBO16_RD_START_FISH:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_conf_set(unit, drc_ndx, (*gbiPtr)));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_err_cnt(unit, drc_ndx, &gbe));
                
                (*seaPtr)[0] = gbe.bist_data_err_occur;
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_EDC_START_FISH:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_conf_set(unit, drc_ndx, (*gbiPtr)));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_err_cnt(unit, drc_ndx, &gbe));
                
                (*seaPtr)[0] = gbe.bist_edc_err_occur;
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_conf_set(unit, drc_ndx, (*gbiPtr)));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_err_cnt(unit, drc_ndx, &gbe));
                
                (*seaPtr)[0] = gbe.bist_data_err_occur;
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_bist_conf_set(unit, drc_ndx, (*biPtr)));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_bist_err_cnt(unit, drc_ndx, &be));
                
                (*seaPtr)[0] = be.bist_err_occur;
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_DBI_EDC_RD_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_conf_set(unit, drc_ndx, (*gbiPtr)));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_err_cnt(unit, drc_ndx, &gbe));
                
                (*seaPtr)[0] = gbe.bist_dbi_err_occur;
                (*seaPtr)[1] = gbe.bist_edc_err_occur;
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_SHORT:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN( _shmoo_combo16_drc_vendor_info_access(unit, &shmoo_vendor_info));
                (*gbiPtr).num_commands = shmoo_vendor_info->fifo_depth;
                (*gbiPtr).bist_mode = SHMOO_COMBO16_GDDR5_BIST_LOAD_READ_FIFO_MODE;
                
                
                for(i = 0; i < SHMOO_COMBO16_BIST_NOF_PATTERNS; i++)
                {
                    COMPILER_64_SET((*gbiPtr).data_pattern[i], _combo16_get_random(), _combo16_get_random());
                }
                
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_conf_set(unit, drc_ndx, (*gbiPtr)));
                
                (*gbiPtr).num_commands = (shmoo_vendor_info->fifo_depth) << 5;
                (*gbiPtr).bist_mode = SHMOO_COMBO16_GDDR5_BIST_READ_FROM_PREV_LOADED_GDDR5_READ_FIFO;
                
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_conf_set(unit, drc_ndx, (*gbiPtr)));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_err_cnt(unit, drc_ndx, &gbe));
                
                (*seaPtr)[0] = gbe.bist_data_err_occur;
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_bist_conf_set(unit, drc_ndx, (*biPtr)));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_bist_err_cnt(unit, drc_ndx, &be));
                
                (*seaPtr)[0] = be.bist_err_occur;
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_WR_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_conf_set(unit, drc_ndx, (*gbiPtr)));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_err_cnt(unit, drc_ndx, &gbe));
                
                (*seaPtr)[0] = gbe.bist_data_err_occur;
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_bist_conf_set(unit, drc_ndx, (*biPtr)));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_bist_err_cnt(unit, drc_ndx, &be));
                
                (*seaPtr)[0] = be.bist_err_occur;
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_DBI_WR_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_conf_set(unit, drc_ndx, (*gbiPtr)));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_err_cnt(unit, drc_ndx, &gbe));
                
                (*seaPtr)[0] = gbe.bist_dbi_err_occur;
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_WR_SHORT:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                
                for(i = 0; i < SHMOO_COMBO16_BIST_NOF_PATTERNS; i++)
                {
                    COMPILER_64_SET((*gbiPtr).data_pattern[i], _combo16_get_random(), _combo16_get_random());
                }
                
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_conf_set(unit, drc_ndx, (*gbiPtr)));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_bist_err_cnt(unit, drc_ndx, &gbe));
                
                (*seaPtr)[0] = gbe.bist_data_err_occur;
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_bist_conf_set(unit, drc_ndx, (*biPtr)));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_bist_err_cnt(unit, drc_ndx, &be));
            
                (*seaPtr)[0] = be.bist_err_occur;
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type and shmoo type combination: %02d / %02d\n"), (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_EXTENDED:
            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_bist_conf_set(unit, drc_ndx, (*biPtr)));
            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_bist_err_cnt(unit, drc_ndx, &be));
            
            (*seaPtr)[0] = be.bist_err_occur;
            break;
        case SHMOO_COMBO16_WR_EXTENDED:
            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_bist_conf_set(unit, drc_ndx, (*biPtr)));
            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_bist_err_cnt(unit, drc_ndx, &be));
            
            (*seaPtr)[0] = be.bist_err_occur;
            break;
        case SHMOO_COMBO16_ADDR_CTRL_EXTENDED:
            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_bist_conf_set(unit, drc_ndx, (*biPtr)));
            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_bist_err_cnt(unit, drc_ndx, &be));
            
            (*seaPtr)[0] = be.bist_err_occur;
            break;
        default:
            LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported shmoo type: %02d\n"), (*scPtr).shmooType));
            return SOC_E_FAIL;
    }
    return SOC_E_NONE;
}

STATIC int
_combo16_calculate_ui_position(uint32 linearPos, combo16_shmoo_container_t *scPtr, combo16_ui_position_t *upPtr)
{
    uint32 ui;
    uint32 position;
    
    position = linearPos;
    
    for(ui = 0; ui < SHMOO_COMBO16_MAX_EFFECTIVE_UI_COUNT; ui++)
    {
        if(linearPos <= (*scPtr).endUI[ui])
        {
            if(ui != 0)
            {
                position = linearPos - ((*scPtr).endUI[ui - 1] + 1);
            }
            break;
        }
    }
    
    if(ui == SHMOO_COMBO16_MAX_EFFECTIVE_UI_COUNT)
    {
        ui--;
        position = linearPos - ((*scPtr).endUI[ui] + 1);
    }
    
    (*upPtr).ui = ui;
    (*upPtr).position = position;
    
    return SOC_E_NONE;
}

STATIC int
_combo16_move_read_max_vdl_glitch_free(int unit, int drc_ndx, uint32 byteMap, uint32 position, combo16_shmoo_container_t *scPtr)
{
    uint32 data;
    uint32 curPosition;
    uint32 halfUI;
    
    halfUI = (*scPtr).endUI[0] << 1;
    
    if(byteMap & 0x1)
    {
        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
        if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
        {
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 0));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        }
        else
        {
            SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, data, SOC_PHY_DDRC16_MAX_VDL_STEPf, &curPosition));
            
            while(curPosition != position)
            {
                if(curPosition > position)
                {
                    if(curPosition - position > halfUI)
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, curPosition - halfUI));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                }
                else
                {
                    if(position - curPosition > halfUI)
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, curPosition + halfUI));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                }
                
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
        }
    }
    
    if(byteMap & 0x2)
    {
        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
        if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
        {
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 0));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        }
        else
        {
            SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr, data, SOC_PHY_DDRC16_MAX_VDL_STEPf, &curPosition));
            
            while(curPosition != position)
            {
                if(curPosition > position)
                {
                    if(curPosition - position > halfUI)
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, curPosition - halfUI));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                }
                else
                {
                    if(position - curPosition > halfUI)
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, curPosition + halfUI));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                }
                
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
        }
    }
    
    if(byteMap & 0x4)
    {
        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
        if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
        {
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 0));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        }
        else
        {
            SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr, data, SOC_PHY_DDRC16_MAX_VDL_STEPf, &curPosition));
            
            while(curPosition != position)
            {
                if(curPosition > position)
                {
                    if(curPosition - position > halfUI)
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, curPosition - halfUI));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                }
                else
                {
                    if(position - curPosition > halfUI)
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, curPosition + halfUI));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                }
                
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
        }
    }
    
    if(byteMap & 0x8)
    {
        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
        if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
        {
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 0));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        }
        else
        {
            SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr, data, SOC_PHY_DDRC16_MAX_VDL_STEPf, &curPosition));
            
            while(curPosition != position)
            {
                if(curPosition > position)
                {
                    if(curPosition - position > halfUI)
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, curPosition - halfUI));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                }
                else
                {
                    if(position - curPosition > halfUI)
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, curPosition + halfUI));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                    }
                }
                
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
        }
    }
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_dqs2ck(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    uint32 x;
    uint32 i;
    uint32 ui;
    uint32 position;
    uint32 addr_ctrl_ui;
    uint32 addr_ctrl_step;
    uint32 data0, data1, data2, data3;
    uint32 result0, result1, result2, result3;
    uint32 stop0, stop1, stop2, stop3;
    uint32 Lcount0, Lcount1, Lcount2, Lcount3;
    uint32 result;
    combo16_ui_position_t up;

    (*scPtr).engageUIshift = 1;
    (*scPtr).sizeY = 1;
    (*scPtr).yCapMin = 0;
    (*scPtr).yCapMax = 1;
    (*scPtr).shmooType = SHMOO_COMBO16_DQS2CK;
    (*scPtr).calibMode = SHMOO_COMBO16_BYTE;
    (*scPtr).calibPos = SHMOO_COMBO16_CALIB_RISING_EDGE;
    
    SOC_IF_ERROR_RETURN( READ_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, drc_ndx, &data0));
    
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, data0, SOC_PHY_DDRC16_UI_SHIFTf, &addr_ctrl_ui));
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, data0, SOC_PHY_DDRC16_MAX_VDL_STEPf, &addr_ctrl_step));
    
    if(addr_ctrl_ui == 3)
    {
        (*scPtr).sizeX = (*scPtr).endUI[4] + 1 + ((*scPtr).endUI[0] >> 1);
        position = (((*scPtr).endUI[4] - (*scPtr).endUI[3]) >> 1) - 15;
        (*scPtr).calibStart = (*scPtr).endUI[3] + 1 + position;
        ui = addr_ctrl_ui;
    }
    else if(addr_ctrl_ui > 0)
    {
        _combo16_calculate_ui_position((*scPtr).endUI[addr_ctrl_ui - 1] + 1 + addr_ctrl_step + ((*scPtr).endUI[0] >> 1) - 15, scPtr, &up);  /* +50% UI - 15 steps */
        position = up.position;
        ui = up.ui;
        if(ui < 3)
        {
            (*scPtr).calibStart = (*scPtr).endUI[ui - 1] + 1 + position;
            (*scPtr).sizeX = (*scPtr).endUI[ui + 1] + 1 + position + 15;
        }
        else
        {
            (*scPtr).calibStart = (*scPtr).endUI[ui - 1] + 1 + position;
            (*scPtr).sizeX = (*scPtr).endUI[ui] + 1 + position + (*scPtr).endUI[0] + 15;
            if((*scPtr).sizeX > ((*scPtr).endUI[4] + 1 + ((*scPtr).endUI[0] >> 1)))
            {
                (*scPtr).sizeX = (*scPtr).endUI[4] + 1 + ((*scPtr).endUI[0] >> 1);
            }
        }
    }
    else
    {
        _combo16_calculate_ui_position(addr_ctrl_step + ((*scPtr).endUI[0] >> 1) - 15, scPtr, &up);                                         /* +50% UI - 15 steps */
        position = up.position;
        ui = up.ui;
        if(ui > 0)
        {
            (*scPtr).calibStart = (*scPtr).endUI[ui - 1] + 1 + position;
            (*scPtr).sizeX = (*scPtr).endUI[ui + 1] + 1 + position + 15;
        }
        else
        {
            (*scPtr).calibStart = position;
            (*scPtr).sizeX = (*scPtr).endUI[ui + 1] + 1 + position + 15;
        }
    }
    
    result = 0;
    
    stop0 = 0;
    stop1 = 0;
    stop2 = 0;
    stop3 = 0;
    
    Lcount0 = 0;
    Lcount1 = 0;
    Lcount2 = 0;
    Lcount3 = 0;
    
    for(x = (*scPtr).calibStart; x < (*scPtr).sizeX; x++)
    {
        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, &data0));
        if((x > (*scPtr).endUI[ui]) && (ui < SHMOO_COMBO16_LAST_EFFECTIVE_UI))
        {
            ui++;
            position = 0;
        }
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data0, SOC_PHY_DDRC16_UI_SHIFTf, ui));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data0, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
        position++;
       
        if(!stop0)
        {
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data0));
        }
        if(!stop1)
        {
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data0));
        }
        if(!stop2)
        {
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data0));
        }
        if(!stop3)
        {
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data0));
        }
        
        result0 = 0;
        result1 = 0;
        result2 = 0;
        result3 = 0;
        
        for(i = 0; i < SHMOO_COMBO16_REPEAT; i++)
        {
            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_dqs_pulse_gen(unit, drc_ndx, 0));
            
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_STATUS_WRITE_LEVELINGr(unit, drc_ndx, &data0));
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_STATUS_WRITE_LEVELINGr(unit, drc_ndx, &data1));
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_STATUS_WRITE_LEVELINGr(unit, drc_ndx, &data2));
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_STATUS_WRITE_LEVELINGr(unit, drc_ndx, &data3));
            
            result0 += (data0 & 0x1);
            result1 += (data1 & 0x1);
            result2 += (data2 & 0x1);
            result3 += (data3 & 0x1);
        }
        
        if(!stop0)
        {
            if(result0 > SHMOO_COMBO16_REPEAT_HALF)
            {
                if(Lcount0 >= 12)
                {
                    stop0 = 1;
                    (*scPtr).resultData[0] = x;
                }
                result = 0x000000FF;
                Lcount0 = 0;
            }
            else
            {
                result = 0x00000000;
                Lcount0++;
            }
        }
        else
        {
            result = (*scPtr).result2D[x - 1] & 0x000000FF;
        }
        
        if(!stop1)
        {
            if(result1 > SHMOO_COMBO16_REPEAT_HALF)
            {
                if(Lcount1 >= 12)
                {
                    stop1 = 1;
                    (*scPtr).resultData[1] = x;
                }
                result |= 0x0000FF00;
                Lcount1 = 0;
            }
            else
            {
                Lcount1++;
            }
        }
        else
        {
            result |= (*scPtr).result2D[x - 1] & 0x0000FF00;
        }
        
        if(!stop2)
        {
            if(result2 > SHMOO_COMBO16_REPEAT_HALF)
            {
                if(Lcount2 >= 12)
                {
                    stop2 = 1;
                    (*scPtr).resultData[2] = x;
                }
                result |= 0x00FF0000;
                Lcount2 = 0;
            }
            else
            {
                Lcount2++;
            }
        }
        else
        {
            result |= (*scPtr).result2D[x - 1] & 0x00FF0000;
        }
        
        if(!stop3)
        {
            if(result3 > SHMOO_COMBO16_REPEAT_HALF)
            {
                if(Lcount3 >= 12)
                {
                    stop3 = 1;
                    (*scPtr).resultData[3] = x;
                }
                result |= 0xFF000000;
                Lcount3 = 0;
            }
            else
            {
                Lcount3++;
            }
        }
        else
        {
            result |= (*scPtr).result2D[x - 1] & 0xFF000000;
        }
        
        (*scPtr).result2D[x] = result;
        
        if(stop0 && stop1 && stop2 && stop3)
        {
            (*scPtr).sizeX = x + 1;
            break;
        }
    }
    
    if(!(stop0 && stop1 && stop2 && stop3))
    {
        LOG_ERROR(BSL_LS_SOC_DDR,
                  (BSL_META_U(unit,
                              "WARNING: DQS2CK alignment failed.\n")));
        return SOC_E_FAIL;
    }
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_wck2ck(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    int32 x;
    uint32 i;
    uint32 data0, data1, data2, data3;
    uint32 result0, result2;
    uint32 stop0, stop2;
    uint32 stopable0, stopable2;
    uint32 Hcount0, Hcount2;
    uint32 Lcount0, Lcount2;
    uint32 result;
    uint32 wckInv;
    uint32 wckInvMR;
    int swap;

    (*scPtr).engageUIshift = 0;
    (*scPtr).sizeY = 1;
    (*scPtr).yCapMin = 0;
    (*scPtr).yCapMax = 1;
    (*scPtr).shmooType = SHMOO_COMBO16_WCK2CK;
    (*scPtr).calibMode = SHMOO_COMBO16_HALFWORD;
    (*scPtr).calibPos = SHMOO_COMBO16_CALIB_ANY_EDGE;
    
    result = 0x00000000;
    wckInv = 0x00000000;
    wckInvMR = 0x00000000;

    stop0 = 0;
    stop2 = 0;
    stopable0 = 0;
    stopable2 = 0;
    
    Hcount0 = 0;
    Hcount2 = 0;
    Lcount0 = 0;
    Lcount2 = 0;
    
    for(x = (*scPtr).sizeX - 1; x >= 0; x--)
    {
        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, &data0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data0, SOC_PHY_DDRC16_MAX_VDL_STEPf, x));
        
        if(!stop0)
        {
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data0));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data0));
        }
        if(!stop2)
        {
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data0));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data0));
        }
        
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        result0 = 0;
        result2 = 0;
        
        for(i = 0; i < SHMOO_COMBO16_REPEAT; i++)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_STATUS_WRITE_LEVELINGr(unit, drc_ndx, &data0));
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_STATUS_WRITE_LEVELINGr(unit, drc_ndx, &data1));
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_STATUS_WRITE_LEVELINGr(unit, drc_ndx, &data2));
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_STATUS_WRITE_LEVELINGr(unit, drc_ndx, &data3));
            
            result0 += (((data0 | data1) >> 9) & 0x1);
            result2 += (((data2 | data3) >> 9) & 0x1);
        
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        }
        
        if(!stop0)
        {
            if(result0 > SHMOO_COMBO16_REPEAT_HALF)
            {
                result = 0x0000FFFF;
                Hcount0++;
                Lcount0 = 0;
            }
            else
            {
                result = 0x00000000;
                Lcount0++;
                Hcount0 = 0;
            }
            
            if(stopable0)
            {
                if(result ^ ((*scPtr).result2D[x + 1] & 0x0000FFFF))
                {
                    stop0 = 1;
                    (*scPtr).resultData[0] = x;
                }
            }
        }
        else
        {
            result = (*scPtr).result2D[x + 1] & 0x0000FFFF;
        }
        
        if(!stop2)
        {
            if(result2 > SHMOO_COMBO16_REPEAT_HALF)
            {
                result |= 0xFFFF0000;
                Hcount2++;
                Lcount2 = 0;
            }
            else
            {
                Lcount2++;
                Hcount2 = 0;
            }
            
            if(stopable2)
            {
                if((result & 0xFFFF0000) ^ ((*scPtr).result2D[x + 1] & 0xFFFF0000))
                {
                    stop2 = 1;
                    (*scPtr).resultData[1] = x;
                }
            }
        }
        else
        {
            result |= (*scPtr).result2D[x + 1] & 0xFFFF0000;
        }
        
        (*scPtr).result2D[x] = result;
        
        if(!stopable0)
        {
            if((Hcount0 == 12) || (Lcount0 == 12))
            {
                stopable0 = 1;
                
                if(Hcount0)
                {
                    wckInv |= 0x00000004;
                }
            }
        }
        
        if(!stopable2)
        {
            if((Hcount2 == 12) || (Lcount2 == 12))
            {
                stopable2 = 1;
                
                if(Hcount2)
                {
                    wckInv |= 0x00000008;
                }
            }
        }
        
        if(stop0 && stop2)
        {
            (*scPtr).calibStart = x;
            break;
        }
    }
    
    if(stop0 && stop2)
    {
        if(wckInv)
        {
            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_dq_byte_pairs_swap_info_get(unit, drc_ndx, &swap));
            
            if(swap)
            {
                wckInvMR = ((wckInv & 0x8) >> 1) | ((wckInv & 0x4) << 1);
            }
            else
            {
                wckInvMR = wckInv;
            }
            
            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 3, wckInvMR, 0x00C));
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        }
        (*scPtr).wckInv = wckInv;
    }
    else
    {
        LOG_ERROR(BSL_LS_SOC_DDR,
                  (BSL_META_U(unit,
                              "WARNING: WCK2CK alignment failed.\n")));
        return SOC_E_FAIL;
    }
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_rd_en_fish(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    uint32 x;
    uint32 d;
    uint32 jump;
    uint32 wp0_pre, wp1_pre, wp2_pre, wp3_pre;
    uint32 wp0_post, wp1_post, wp2_post, wp3_post;
    uint32 fish0, fish1, fish2, fish3;
    uint32 data;
/*    uint32 temp; */
    combo16_bist_info_t bi;
    
    jump = 3;
    
    fish0 = 1;
    fish1 = 1;
    fish2 = 1;
    fish3 = 1;
    
    (*scPtr).engageUIshift = 0;
    (*scPtr).shmooType = SHMOO_COMBO16_RD_EN_FISH;
    
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            "\n\n")));
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            "***** Interface.......: %3d\n"),
                 drc_ndx));
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            " **** Shmoo type......: RD_EN_FISH\n")));
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            "  *** Searching.......: RD_2G_DELAY\n")));
    
    _combo16_initialize_bist(unit, drc_ndx, -1, scPtr, &bi, NULL);
    
    for(d = 0; d < 32; d++)
    {        
        if(fish0)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_FIFO_CONFIGr, &data, SOC_PHY_DDRC16_RD_2G_DELAYf, d));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, data));
        }
        if(fish1)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_DDR4_REN_FIFO_CONFIGr, &data, SOC_PHY_DDRC16_RD_2G_DELAYf, d));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, data));
        }
        if(fish2)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_DDR4_REN_FIFO_CONFIGr, &data, SOC_PHY_DDRC16_RD_2G_DELAYf, d));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, data));
        }
        if(fish3)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_DDR4_REN_FIFO_CONFIGr, &data, SOC_PHY_DDRC16_RD_2G_DELAYf, d));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, data));
        }
        
        for(x = 0; x < (((*scPtr).endUI[1] + 1) >> jump); x++)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, x << jump));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 1));
            
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 0));
            
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            
            SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 0));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
            
            SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 0));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
            
            SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
            
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_STATUS_DDR4_RDATA_FIFOr(unit, drc_ndx, &wp0_pre));
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_STATUS_DDR4_RDATA_FIFOr(unit, drc_ndx, &wp1_pre));
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_STATUS_DDR4_RDATA_FIFOr(unit, drc_ndx, &wp2_pre));
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_STATUS_DDR4_RDATA_FIFOr(unit, drc_ndx, &wp3_pre));
            
            wp0_pre &= 0xF;
            wp1_pre &= 0xF;
            wp2_pre &= 0xF;
            wp3_pre &= 0xF;
            
            _combo16_run_bist(unit, drc_ndx, scPtr, &bi, NULL, NULL);
            
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_STATUS_DDR4_RDATA_FIFOr(unit, drc_ndx, &wp0_post));
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_STATUS_DDR4_RDATA_FIFOr(unit, drc_ndx, &wp1_post));
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_STATUS_DDR4_RDATA_FIFOr(unit, drc_ndx, &wp2_post));
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_STATUS_DDR4_RDATA_FIFOr(unit, drc_ndx, &wp3_post));
            
            wp0_post &= 0xF;
            wp1_post &= 0xF;
            wp2_post &= 0xF;
            wp3_post &= 0xF;
            
            if(wp0_post < wp0_pre)
            {
                wp0_post += 0x10;
            }
            if(wp1_post < wp1_pre)
            {
                wp1_post += 0x10;
            }
            if(wp2_post < wp2_pre)
            {
                wp2_post += 0x10;
            }
            if(wp3_post < wp3_pre)
            {
                wp3_post += 0x10;
            }
            
            if(wp0_post - wp0_pre == 4)
            {
                fish0 = 0;
            }
            if(wp1_post - wp1_pre == 4)
            {
                fish1 = 0;
            }
            if(wp2_post - wp2_pre == 4)
            {
                fish2 = 0;
            }
            if(wp3_post - wp3_pre == 4)
            {
                fish3 = 0;
            }
        }
        
        LOG_VERBOSE(BSL_LS_SOC_DDR,
                    (BSL_META_U(unit,
                                "   ** Delay %02d........: %01d %01d %01d %01d\n"),
                     d, fish3, fish2, fish1, fish0));
        
        if(!(fish0 || fish1 || fish2 || fish3))
        {
            break;
        }
    }
    
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            "\n")));
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_rd_start_fish(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    uint32 x;
    uint32 d;
    uint32 ui;
    uint32 jump;
    uint32 fish0, fish1, fish2, fish3;
    uint32 data;
    uint32 inc;
    combo16_gddr5_bist_info_t gbi;
    combo16_shmoo_error_array_t sea;
    
    jump = 2;
    
    fish0 = 2;
    fish1 = 2;
    fish2 = 2;
    fish3 = 2;
    
    (*scPtr).engageUIshift = 1;
    (*scPtr).shmooType = SHMOO_COMBO16_RD_START_FISH;
    
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            "\n\n")));
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            "***** Interface.......: %3d\n"),
                 drc_ndx));
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            " **** Shmoo type......: RD_START_FISH\n")));
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            "  *** Searching.......: DATA_VALID_WR2RD_DELAY\n")));
    
    _combo16_initialize_bist(unit, drc_ndx, -1, scPtr, NULL, &gbi);
    
    for(d = 0; d < 16; d++)
    {
        for(ui = 0; ui < SHMOO_COMBO16_MAX_EFFECTIVE_UI_COUNT; ui++)
        {
            _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0xF, 0, scPtr);
            
            if(fish0)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_DATA_UI_SHIFTf, ui));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
            }
            if(fish1)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_DATA_UI_SHIFTf, ui));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
            }
            if(fish2)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_DATA_UI_SHIFTf, ui));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
            }
            if(fish3)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_DATA_UI_SHIFTf, ui));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
            }
            
            for(x = 0; x < (((*scPtr).endUI[0] + 1) >> jump); x++)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, x << jump));
                
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                _combo16_run_bist(unit, drc_ndx, scPtr, NULL, &gbi, &sea);
            
                data = sea[0];
                
                if(fish0)
                {
                    if(!(data & 0x000000FF))
                    {
                        fish0--;
                    }
                }
                if(fish1)
                {
                    if(!(data & 0x0000FF00))
                    {
                        fish1--;
                    }
                }
                if(fish2)
                {
                    if(!(data & 0x00FF0000))
                    {
                        fish2--;
                    }
                }
                if(fish3)
                {
                    if(!(data & 0xFF000000))
                    {
                        fish3--;
                    }
                }
            }
        }
        
        LOG_VERBOSE(BSL_LS_SOC_DDR,
                    (BSL_META_U(unit,
                                "   ** Delay %02d........: %01d %01d %01d %01d\n"),
                     d, fish3, fish2, fish1, fish0));
        
        if(!(fish0 || fish1 || fish2 || fish3))
        {
            break;
        }
        
        if(d == 15)
        {
            break;
        }
        
        if(fish0)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr, data, SOC_PHY_DDRC16_DATA_VALID_WR2RD_DELAY_INCf, &inc));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_DATA_VALID_WR2RD_DELAY_INCf, inc ^ 0x1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_DATA_VALID_WR2RD_DELAYf, d + 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
        }
        if(fish1)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr, data, SOC_PHY_DDRC16_DATA_VALID_WR2RD_DELAY_INCf, &inc));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_DATA_VALID_WR2RD_DELAY_INCf, inc ^ 0x1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_DATA_VALID_WR2RD_DELAYf, d + 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
        }
        if(fish2)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr, data, SOC_PHY_DDRC16_DATA_VALID_WR2RD_DELAY_INCf, &inc));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_DATA_VALID_WR2RD_DELAY_INCf, inc ^ 0x1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_DATA_VALID_WR2RD_DELAYf, d + 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
        }
        if(fish3)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr, data, SOC_PHY_DDRC16_DATA_VALID_WR2RD_DELAY_INCf, &inc));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_DATA_VALID_WR2RD_DELAY_INCf, inc ^ 0x1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_DATA_VALID_WR2RD_DELAYf, d + 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
        }
        
    }
    
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            "\n")));
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_edc_start_fish(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    uint32 x;
    uint32 d;
    uint32 ui;
    uint32 jump;
    uint32 fish0, fish1, fish2, fish3;
    uint32 data;
    uint32 inc;
    combo16_gddr5_bist_info_t gbi;
    combo16_shmoo_error_array_t sea;
    
    jump = 2;
    
    fish0 = 2;
    fish1 = 2;
    fish2 = 2;
    fish3 = 2;
    
    (*scPtr).engageUIshift = 1;
    (*scPtr).shmooType = SHMOO_COMBO16_RD_START_FISH;
    
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            "\n\n")));
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            "***** Interface.......: %3d\n"),
                 drc_ndx));
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            " **** Shmoo type......: EDC_START_FISH\n")));
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            "  *** Searching.......: EDC_VALID_WR2RD_DELAY\n")));
    
    _combo16_initialize_bist(unit, drc_ndx, -1, scPtr, NULL, &gbi);
    
    for(d = 0; d < 16; d++)
    {
        for(ui = 0; ui < SHMOO_COMBO16_MAX_EFFECTIVE_UI_COUNT; ui++)
        {
            _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0xF, 0, scPtr);
            
            if(fish0)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_EDC_UI_SHIFTf, ui));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
            }
            if(fish1)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_EDC_UI_SHIFTf, ui));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
            }
            if(fish2)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_EDC_UI_SHIFTf, ui));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
            }
            if(fish3)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_EDC_UI_SHIFTf, ui));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
            }
            
            for(x = 0; x < (((*scPtr).endUI[0] + 1) >> jump); x++)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, x << jump));
                
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                _combo16_run_bist(unit, drc_ndx, scPtr, NULL, &gbi, &sea);
            
                data = sea[0];
                
                if(fish0)
                {
                    if(!(data & 0x000000FF))
                    {
                        fish0--;
                    }
                }
                if(fish1)
                {
                    if(!(data & 0x0000FF00))
                    {
                        fish1--;
                    }
                }
                if(fish2)
                {
                    if(!(data & 0x00FF0000))
                    {
                        fish2--;
                    }
                }
                if(fish3)
                {
                    if(!(data & 0xFF000000))
                    {
                        fish3--;
                    }
                }
            }
        }
        
        LOG_VERBOSE(BSL_LS_SOC_DDR,
                    (BSL_META_U(unit,
                                "   ** Delay %02d........: %01d %01d %01d %01d\n"),
                     d, fish3, fish2, fish1, fish0));
        
        if(!(fish0 || fish1 || fish2 || fish3))
        {
            break;
        }
        
        if(d == 15)
        {
            break;
        }
        
        if(fish0)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr, data, SOC_PHY_DDRC16_EDC_VALID_WR2RD_DELAY_INCf, &inc));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_EDC_VALID_WR2RD_DELAY_INCf, inc ^ 0x1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_EDC_VALID_WR2RD_DELAYf, d + 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
        }
        if(fish1)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr, data, SOC_PHY_DDRC16_EDC_VALID_WR2RD_DELAY_INCf, &inc));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_EDC_VALID_WR2RD_DELAY_INCf, inc ^ 0x1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_EDC_VALID_WR2RD_DELAYf, d + 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
        }
        if(fish2)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr, data, SOC_PHY_DDRC16_EDC_VALID_WR2RD_DELAY_INCf, &inc));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_EDC_VALID_WR2RD_DELAY_INCf, inc ^ 0x1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_EDC_VALID_WR2RD_DELAYf, d + 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
        }
        if(fish3)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr, data, SOC_PHY_DDRC16_EDC_VALID_WR2RD_DELAY_INCf, &inc));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_EDC_VALID_WR2RD_DELAY_INCf, inc ^ 0x1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_EDC_VALID_WR2RD_DELAYf, d + 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
        }
        
    }
    
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            "\n")));
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_wr_deskew(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    uint32 x;
    uint32 ui;
    uint32 position;
    uint32 b;
    uint32 data;
    combo16_bist_info_t bi;
    combo16_gddr5_bist_info_t gbi;
    combo16_shmoo_error_array_t sea;

    (*scPtr).engageUIshift = 1;
    (*scPtr).sizeX =  (*scPtr).endUI[SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT - 1] + 1;
    (*scPtr).sizeY = 1;
    (*scPtr).yCapMin = 0;
    (*scPtr).yCapMax = 1;
    (*scPtr).shmooType = SHMOO_COMBO16_WR_DESKEW;
    
    for(x = 0; x < (*scPtr).sizeX; x++)
    {
        (*scPtr).result2D[x] = 0;
    }

    for(b = 0; b < SHMOO_COMBO16_BYTE; b++)
    {
        _combo16_initialize_bist(unit, drc_ndx, b, scPtr, &bi, &gbi);
        
        position = 0;
        ui = 0;
        
        for(x = 0; x < (*scPtr).sizeX; x++)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
            if((x > (*scPtr).endUI[ui]) && (ui < SHMOO_COMBO16_LAST_EFFECTIVE_UI))
            {
                ui++;
                position = 0;
            }
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, ui));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
            position++;
            
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
            
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            _combo16_run_bist(unit, drc_ndx, scPtr, &bi, &gbi, &sea);
            
            (*scPtr).result2D[x] |= (sea[0] & (0x01010101 << b));
        }
    }
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_dbi_wr_deskew(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    uint32 x;
    uint32 ui;
    uint32 position;
    uint32 data;
    uint32 pos0, pos1, pos2, pos3;
    combo16_bist_info_t bi;
    combo16_gddr5_bist_info_t gbi;
    combo16_shmoo_error_array_t sea;

    (*scPtr).engageUIshift = 1;
    (*scPtr).sizeX =  (*scPtr).endUI[SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT - 1] + 1;
    (*scPtr).sizeY = 1;
    (*scPtr).yCapMin = 0;
    (*scPtr).yCapMax = 1;
    (*scPtr).shmooType = SHMOO_COMBO16_DBI_WR_DESKEW;
    
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &pos0));
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &pos1));
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &pos2));
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &pos3));
    
    for(x = 0; x < (*scPtr).sizeX; x++)
    {
        (*scPtr).result2D[x] = 0;
    }

    _combo16_initialize_bist(unit, drc_ndx, -1, scPtr, &bi, &gbi);
    
    position = 0;
    ui = 0;
    
    for(x = 0; x < (*scPtr).sizeX; x++)
    {
        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
        if((x > (*scPtr).endUI[ui]) && (ui < SHMOO_COMBO16_LAST_EFFECTIVE_UI))
        {
            ui++;
            position = 0;
        }
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, ui));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
        position++;
        
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
        
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        _combo16_run_bist(unit, drc_ndx, scPtr, &bi, &gbi, &sea);
        
        (*scPtr).result2D[x] |= (sea[0] & 0x0000000F);
    }
    
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, pos0));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, pos1));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, pos2));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, pos3));
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_wr_short(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    uint32 x;
    uint32 ui;
    uint32 position;
    uint32 data;
    combo16_bist_info_t bi;
    combo16_gddr5_bist_info_t gbi;
    combo16_shmoo_error_array_t sea;

    (*scPtr).engageUIshift = 1;
    (*scPtr).sizeX = (*scPtr).endUI[SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT - 1] + 1;
    (*scPtr).sizeY = 1;
    (*scPtr).yCapMin = 0;
    (*scPtr).yCapMax = 1;
    (*scPtr).shmooType = SHMOO_COMBO16_WR_SHORT;
    
    _combo16_initialize_bist(unit, drc_ndx, -1, scPtr, &bi, &gbi);
    
    position = 0;
    ui = 0;

    for(x = 0; x < (*scPtr).sizeX; x++)
    {
        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
        if((x > (*scPtr).endUI[ui]) && (ui < SHMOO_COMBO16_LAST_EFFECTIVE_UI))
        {
            ui++;
            position = 0;
        }
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, ui));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
        position++;
        
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
        
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        _combo16_run_bist(unit, drc_ndx, scPtr, &bi, &gbi, &sea);
        
        (*scPtr).result2D[x] = sea[0];
    }
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_wr_extended(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    uint32 x;
    uint32 ui;
    uint32 position;
    uint32 y;
    uint32 yCapMin;
    uint32 yCapMax;
    uint32 sizeY;
    uint32 xStart;
    int MRfield;
    uint32 data;
    uint32 dramType;
    combo16_bist_info_t bi;
    combo16_shmoo_error_array_t sea;
    
    dramType = (*scPtr).dramType;
    sizeY = SHMOO_COMBO16_MAX_VREF_RANGE;
    xStart = 0;

    switch(dramType)
    {
        case SHMOO_COMBO16_DRAM_TYPE_DDR4:
            if((*scPtr).debugMode & SHMOO_COMBO16_CTL_FLAGS_EXT_VREF_RANGE_BIT)
            {
                yCapMin = 15;       /*  0 - 50: Vref Range 2 - 45.00% through 77.50% */
                yCapMax = 74;       /* 51 - 73: Vref Range 1 - 78.20% through 92.50% */
            }                       /* Actual Vref Range crossover at SHMOO_COMBO16_DDR4_VREF_RANGE_CROSSOVER */
            else
            {
                yCapMin = 28;
                yCapMax = 61;
            }
            break;
        case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
            yCapMin = 0;
            yCapMax = 15;
            break;
        default:
            LOG_ERROR(BSL_LS_SOC_DDR,
                      (BSL_META_U(unit,
                                  "Unsupported dram type: %02d\n"),
                       dramType));
            return SOC_E_FAIL;
    }
    
    if((*scPtr).restore)
    {
        yCapMin = 0;
        yCapMax = 1;
        sizeY = 1;
    }

    (*scPtr).engageUIshift = 1;
    (*scPtr).sizeX = (*scPtr).endUI[SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT - 1] + 1;
    (*scPtr).sizeY = sizeY;
    (*scPtr).yCapMin = yCapMin;
    (*scPtr).yCapMax = yCapMax;
    (*scPtr).shmooType = SHMOO_COMBO16_WR_EXTENDED;
    
    _combo16_initialize_bist(unit, drc_ndx, -1, scPtr, &bi, NULL);
    
    for(y = yCapMin; y < yCapMax; y++)
    {
        if(!(*scPtr).restore)
        {
            switch(dramType)
            {
                case SHMOO_COMBO16_DRAM_TYPE_DDR4:
                    if(y > SHMOO_COMBO16_DDR4_VREF_RANGE_CROSSOVER)
                    {   /* Vref Range 1 */
                        MRfield = y - 23;
                    }
                    else
                    {   /* Vref Range 2 */
                        MRfield = y | 0x40;
                    }
                    SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, MRfield, 0x7F));
                    break;
                case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
                    MRfield = (((int) y) - 7) & 0xF;
                    SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, (MRfield << 8) | (MRfield << 4), 0xFF0));
                    break;
                /* must default. Otherwise - compilation error */
                /* coverity[dead_error_begin:FALSE] */
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported dram type: %02d\n"),
                               dramType));
                    return SOC_E_FAIL;
            }
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        }
    
        position = 0;
        ui = 0;

        for(x = 0; x < (*scPtr).sizeX; x++)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
            if((x > (*scPtr).endUI[ui]) && (ui < SHMOO_COMBO16_LAST_EFFECTIVE_UI))
            {
                ui++;
                position = 0;
            }
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, ui));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
            position++;
            
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
            
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
            _combo16_run_bist(unit, drc_ndx, scPtr, &bi, NULL, &sea);
            
            (*scPtr).result2D[x + xStart] = sea[0];
        }
        
        xStart += (*scPtr).sizeX;
    }
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_rd_deskew(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    uint32 x;
    uint32 b;
    uint32 data;
    uint32 result0; /*, resultHalfUI; */
    combo16_bist_info_t bi;
    combo16_gddr5_bist_info_t gbi;
    combo16_shmoo_error_array_t sea;

    (*scPtr).engageUIshift = 0;
    (*scPtr).sizeY = 1;
    (*scPtr).yCapMin = 0;
    (*scPtr).yCapMax = 1;
    (*scPtr).shmooType = SHMOO_COMBO16_RD_DESKEW;
    
    result0 = 0;
/*    resultHalfUI = 0; */
    
    if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
    {
        for(b = 0; b < SHMOO_COMBO16_BYTE; b++)
        {
            _combo16_initialize_bist(unit, drc_ndx, b, scPtr, &bi, &gbi);
            _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0xF, 0, scPtr);
            
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, 0));
            
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            _combo16_run_bist(unit, drc_ndx, scPtr, &bi, &gbi, &sea);
            
            result0 |= (sea[0] & (0x01010101 << b));
        }
        
        if(result0 == 0xFFFFFFFF)
        {
            (*scPtr).sizeX = (*scPtr).endUI[2] + 1;
            (*scPtr).calibStart = 0;
            
            data = 1;
        }
        else
        {
            (*scPtr).sizeX = (*scPtr).endUI[2] + 1;
            (*scPtr).calibStart = 0;
            
            data = 2;
        }
    }
    else
    {
        (*scPtr).sizeX = (*scPtr).endUI[1] + 1;
        (*scPtr).calibStart = 0;
            
        data = 3;
    }
    
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESERVED_REGr(unit, drc_ndx, data));
    
    for(x = (*scPtr).calibStart; x < (*scPtr).sizeX; x++)
    {
        (*scPtr).result2D[x] = 0;
    }

    for(b = 0; b < SHMOO_COMBO16_BYTE; b++)
    {
        _combo16_initialize_bist(unit, drc_ndx, b, scPtr, &bi, &gbi);
        _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0xF, (*scPtr).calibStart, scPtr);
        
        for(x = (*scPtr).calibStart; x <(*scPtr).sizeX; x++)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, x));
            
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 1));
                
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 0));
            }
            
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        
            
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
                    
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                    
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
            }
            
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            _combo16_run_bist(unit, drc_ndx, scPtr, &bi, &gbi, &sea);
            
            (*scPtr).result2D[x] |= (sea[0] & (0x01010101 << b));
        }
    }
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_dbi_edc_rd_deskew(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    uint32 x;
    uint32 b;
    uint32 data;
    uint32 pos0, pos1, pos2, pos3;
    combo16_bist_info_t bi;
    combo16_gddr5_bist_info_t gbi;
    combo16_shmoo_error_array_t sea;

    (*scPtr).engageUIshift = 0;
    (*scPtr).sizeY = 1;
    (*scPtr).yCapMin = 0;
    (*scPtr).yCapMax = 1;
    (*scPtr).shmooType = SHMOO_COMBO16_DBI_EDC_RD_DESKEW;
    
    SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESERVED_REGr(unit, drc_ndx, &data));
    
    switch(data & 0x3)
    {
        case 1:
            (*scPtr).sizeX = (*scPtr).endUI[2] + 1;
            (*scPtr).calibStart = 0;
            break;
        case 2:
            (*scPtr).sizeX = (*scPtr).endUI[2] + 1;
            (*scPtr).calibStart = 0;
            break;
        case 3:
            (*scPtr).sizeX = (*scPtr).endUI[1] + 1;
            (*scPtr).calibStart = 0;
            break;
        default:
            LOG_ERROR(BSL_LS_SOC_DDR,
                      (BSL_META_U(unit,
                                  "WARNING: RD_DESKEW shmoo has not been run to its end, or Control Reserved Register has been tempered with.\n")));
            return SOC_E_FAIL;
    }
    
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &pos0));
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &pos1));
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &pos2));
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &pos3));
    
    (*scPtr).resultData[0] = pos0;
    (*scPtr).resultData[1] = pos1;
    (*scPtr).resultData[2] = pos2;
    (*scPtr).resultData[3] = pos3;
      
    for(x = (*scPtr).calibStart; x < (*scPtr).sizeX; x++)
    {
        (*scPtr).result2D[x] = 0;
    }

    for(b = 0; b < 2; b++)
    {
        _combo16_initialize_bist(unit, drc_ndx, b, scPtr, &bi, &gbi);
        _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0xF, (*scPtr).calibStart, scPtr);
        
        for(x = (*scPtr).calibStart; x <(*scPtr).sizeX; x++)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, x));
            
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            _combo16_run_bist(unit, drc_ndx, scPtr, &bi, &gbi, &sea);
            
            (*scPtr).result2D[x] |= ((sea[b] & 0x0000000F) << (b << 2));
        }
    }
    
    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x1, pos0, scPtr);
    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x2, pos1, scPtr);
    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x4, pos2, scPtr);
    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x8, pos3, scPtr);
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_rd_short(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    uint32 x;
    uint32 data;
    uint32 dramType;
    combo16_bist_info_t bi;
    combo16_gddr5_bist_info_t gbi;
    combo16_shmoo_error_array_t sea;
    
    dramType = (*scPtr).dramType;

    switch(dramType)
    {
        case SHMOO_COMBO16_DRAM_TYPE_DDR4:
            (*scPtr).sizeX = (*scPtr).endUI[1] + 1;
            break;
        case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
            (*scPtr).sizeX = (*scPtr).endUI[2] + 1;
            break;
        default:
            LOG_ERROR(BSL_LS_SOC_DDR,
                      (BSL_META_U(unit,
                                  "Unsupported dram type: %02d\n"),
                       dramType));
            return SOC_E_FAIL;
    }

    (*scPtr).engageUIshift = 0;
    (*scPtr).sizeY = 1;
    (*scPtr).yCapMin = 0;
    (*scPtr).yCapMax = 1;
    (*scPtr).shmooType = SHMOO_COMBO16_RD_SHORT;
    
    _combo16_initialize_bist(unit, drc_ndx, -1, scPtr, &bi, &gbi);
    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0xF, (*scPtr).calibStart, scPtr);

    for(x = 0; x < (*scPtr).sizeX; x++)
    {
        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, x));
            
        if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
        {
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 1));
            
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 0));
        }
        
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        
        if(dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 0));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
            
            SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 0));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
                
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 1));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
        }

        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        _combo16_run_bist(unit, drc_ndx, scPtr, &bi, &gbi, &sea);
        
        (*scPtr).result2D[x] = sea[0];
    }
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_rd_extended(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    uint32 x;
    uint32 y;
    uint32 yCapMin;
    uint32 yCapMax;
    uint32 sizeY;
    uint32 xStart;
    uint32 data;
    uint32 dramType;
    combo16_bist_info_t bi;
    combo16_shmoo_error_array_t sea;
    
    dramType = (*scPtr).dramType;
    sizeY = SHMOO_COMBO16_MAX_VREF_RANGE;
    xStart = 0;

    switch(dramType)
    {
        case SHMOO_COMBO16_DRAM_TYPE_DDR4:
            yCapMin = 56;
            yCapMax = 89;
            (*scPtr).sizeX = (*scPtr).endUI[1] + 1;
            break;
        case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
            yCapMin = 48;
            yCapMax = 81;
            
            SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESERVED_REGr(unit, drc_ndx, &data));
            
            switch((data & 0xC) >> 2)
            {
                case 1:
                    (*scPtr).sizeX = (*scPtr).endUI[2] + 1 + ((data >> 4) & 0x1FF) + 20;
                    (*scPtr).calibStart = (*scPtr).endUI[0] + 1;
                    break;
                case 2:
                    (*scPtr).sizeX = (*scPtr).endUI[1] + 1 + ((data >> 4) & 0x1FF) + 20;
                    (*scPtr).calibStart = 0;
                    break;
                default:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "WARNING: RD_SHORT shmoo has not been run to its end, or Control Reserved Register has been tempered with.\n")));
                    (*scPtr).sizeX = (*scPtr).endUI[2] + 1;
                    (*scPtr).calibStart = 0;
                    break;
            }
            break;
        default:
            LOG_ERROR(BSL_LS_SOC_DDR,
                      (BSL_META_U(unit,
                                  "Unsupported dram type: %02d\n"),
                       dramType));
            return SOC_E_FAIL;
    }
    
    if((*scPtr).debugMode & SHMOO_COMBO16_CTL_FLAGS_EXT_VREF_RANGE_BIT)
    {
        yCapMin = 11;
        yCapMax = 126;
    }
    
    if((*scPtr).restore)
    {
        yCapMin = 0;
        yCapMax = 1;
        sizeY = 1;
    }

    (*scPtr).engageUIshift = 0;
    (*scPtr).sizeY = sizeY;
    (*scPtr).yCapMin = yCapMin;
    (*scPtr).yCapMax = yCapMax;
    (*scPtr).shmooType = SHMOO_COMBO16_RD_EXTENDED;
    
    _combo16_initialize_bist(unit, drc_ndx, -1, scPtr, &bi, NULL);

    for(y = yCapMin; y < yCapMax; y++)
    {
        if(!(*scPtr).restore)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_VREF_DAC_CONFIGr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_VREF_DAC_CONFIGr, &data, SOC_PHY_DDRC16_DATAf, y << 1));
            
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_VREF_DAC_CONFIGr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_VREF_DAC_CONFIGr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_VREF_DAC_CONFIGr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_VREF_DAC_CONFIGr(unit, drc_ndx, data));
        }
        
        _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0xF, (*scPtr).calibStart, scPtr);
        
        for(x = (*scPtr).calibStart; x < (*scPtr).sizeX; x++)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, x));
            
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 1));
                
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
                
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 0));
            }
            
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        
            if(dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
                    
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                    
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
            }
            
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            _combo16_run_bist(unit, drc_ndx, scPtr, &bi, NULL, &sea);
            
            (*scPtr).result2D[x + xStart] = sea[0];
        }
        
        xStart += (*scPtr).sizeX;
    }
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_addr_ctrl_short(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    uint32 x;
    uint32 ui;
    uint32 position;
    uint32 data;
    uint32 prevResult;
    combo16_gddr5_bist_info_t gbi;
    combo16_shmoo_error_array_t sea;

    (*scPtr).engageUIshift = 1;
    (*scPtr).sizeX = (*scPtr).endUI[SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT - 1] + 1;
    (*scPtr).sizeY = 1;
    (*scPtr).yCapMin = 0;
    (*scPtr).yCapMax = 1;
    (*scPtr).shmooType = SHMOO_COMBO16_ADDR_CTRL_SHORT;
    
    _combo16_initialize_bist(unit, drc_ndx, -1, scPtr, NULL, &gbi);
    
    position = 0;
    ui = 0;
    prevResult = 1;

    for(x = 0; x < (*scPtr).sizeX; x++)
    {
        SOC_IF_ERROR_RETURN(READ_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, drc_ndx, &data));
        if((x > (*scPtr).endUI[ui]) && (ui < SHMOO_COMBO16_LAST_EFFECTIVE_UI))
        {
            ui++;
            position = 0;
        }
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, &data, SOC_PHY_DDRC16_UI_SHIFTf, ui));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
        position++;
        
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, drc_ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_ADDRr(unit, drc_ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_CTRLr(unit, drc_ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_CTRLr(unit, drc_ndx, data));
        
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        if(prevResult)
        {
            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_adt(unit, drc_ndx, 0));
            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_dram_init(unit, drc_ndx, 11));
            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_adt(unit, drc_ndx, 1));
            sal_usleep(SHMOO_COMBO16_DEEP_SLEEP);
        }
        
        _combo16_run_bist(unit, drc_ndx, scPtr, NULL, &gbi, &sea);
        
        (*scPtr).result2D[x] = sea[0];
        prevResult = sea[0];
    }
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_addr_ctrl_extended(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    uint32 x;
    uint32 ui;
    uint32 position;
    uint32 data;
    combo16_bist_info_t bi;
    combo16_shmoo_error_array_t sea;

    (*scPtr).engageUIshift = 1;
    (*scPtr).sizeX = (*scPtr).endUI[SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT - 1] + 1;
    (*scPtr).sizeY = 1;
    (*scPtr).yCapMin = 0;
    (*scPtr).yCapMax = 1;
    (*scPtr).shmooType = SHMOO_COMBO16_ADDR_CTRL_EXTENDED;
    
    _combo16_initialize_bist(unit, drc_ndx, -1, scPtr, &bi, NULL);
    
    position = 0;
    ui = 0;

    for(x = 0; x < (*scPtr).sizeX; x++)
    {
        SOC_IF_ERROR_RETURN(READ_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, drc_ndx, &data));
        if((x > (*scPtr).endUI[ui]) && (ui < SHMOO_COMBO16_LAST_EFFECTIVE_UI))
        {
            ui++;
            position = 0;
        }
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, &data, SOC_PHY_DDRC16_UI_SHIFTf, ui));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, position));
        position++;
        
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, drc_ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_ADDRr(unit, drc_ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_CTRLr(unit, drc_ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_CTRLr(unit, drc_ndx, data));
        
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        _combo16_run_bist(unit, drc_ndx, scPtr, &bi, NULL, &sea);
        
        (*scPtr).result2D[x] = sea[0];
        
        if((*scPtr).result2D[x])
        {
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
                    
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                    
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
            }

            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_dram_init(unit, drc_ndx, 10));
            sal_usleep(SHMOO_COMBO16_DEEP_SLEEP);
        }
    }
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_do(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    switch((*scPtr).shmooType)
    {
        case SHMOO_COMBO16_ADDR_CTRL_SHORT:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                return _shmoo_combo16_addr_ctrl_short(unit, drc_ndx, scPtr);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_DQS2CK:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                return _shmoo_combo16_dqs2ck(unit, drc_ndx, scPtr);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_WCK2CK:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                return _shmoo_combo16_wck2ck(unit, drc_ndx, scPtr);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_EN_FISH:
            return _shmoo_combo16_rd_en_fish(unit, drc_ndx, scPtr);
        case SHMOO_COMBO16_RD_START_FISH:
            return _shmoo_combo16_rd_start_fish(unit, drc_ndx, scPtr);
        case SHMOO_COMBO16_EDC_START_FISH:
            return _shmoo_combo16_edc_start_fish(unit, drc_ndx, scPtr);
        case SHMOO_COMBO16_RD_DESKEW:
            return _shmoo_combo16_rd_deskew(unit, drc_ndx, scPtr);
        case SHMOO_COMBO16_DBI_EDC_RD_DESKEW:
            return _shmoo_combo16_dbi_edc_rd_deskew(unit, drc_ndx, scPtr);
        case SHMOO_COMBO16_RD_SHORT:
            return _shmoo_combo16_rd_short(unit, drc_ndx, scPtr);
        case SHMOO_COMBO16_WR_DESKEW:
            return _shmoo_combo16_wr_deskew(unit, drc_ndx, scPtr);
        case SHMOO_COMBO16_DBI_WR_DESKEW:
            return _shmoo_combo16_dbi_wr_deskew(unit, drc_ndx, scPtr);
        case SHMOO_COMBO16_WR_SHORT:
            return _shmoo_combo16_wr_short(unit, drc_ndx, scPtr);
        case SHMOO_COMBO16_RD_EXTENDED:
            return _shmoo_combo16_rd_extended(unit, drc_ndx, scPtr);
        case SHMOO_COMBO16_WR_EXTENDED:
            return _shmoo_combo16_wr_extended(unit, drc_ndx, scPtr);
        case SHMOO_COMBO16_ADDR_CTRL_EXTENDED:
            return _shmoo_combo16_addr_ctrl_extended(unit, drc_ndx, scPtr);
        default:
            LOG_ERROR(BSL_LS_SOC_DDR,
                      (BSL_META_U(unit,
                                  "Unsupported shmoo type: %02d\n"),
                       (*scPtr).shmooType));
            return SOC_E_FAIL;
    }
    return SOC_E_NONE;
}

STATIC int
_combo16_calib_2D(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr, uint32 calibMode, uint32 calibPos, uint32 calibSweep, uint32 decisionMode, uint32 groupOrder, uint32 groupNumBits)
{
    uint32 x;
    uint32 y;
    uint32 xStart;
    uint32 sizeX;
    uint32 calibStart[SHMOO_COMBO16_WORD];
    uint32 calibEnd[SHMOO_COMBO16_WORD];
    uint32 yCapMin;
    uint32 yCapMax;
    uint32 i, j;
    uint32 shiftAmount;
    uint32 dataMask;
    int32 passStart;
    int32 failStart;
    int32 passStartSeen;
    int32 failStartSeen;
    int32 passLength;
    int32 maxPassStart;
    int32 maxPassLength;
    int32 maxMidPointX;
    uint32 maxPassLengthArray[SHMOO_COMBO16_WORD];
    uint32 result;
    uint32 iter = 0;

    xStart = 0;
    sizeX = (*scPtr).sizeX;
    yCapMin = (*scPtr).yCapMin;
    yCapMax = (*scPtr).yCapMax;
    
    switch(calibSweep)
    {
        case SHMOO_COMBO16_CALIB_SWEEP_1:
            for(i = 0; i < SHMOO_COMBO16_WORD; i++)
            {
                calibStart[i] = (*scPtr).calibStart;
                calibEnd[i] = sizeX;
            }
            break;
        case SHMOO_COMBO16_CALIB_SWEEP_BYTE_TO_BIT:
            iter = 4;
            break;
        case SHMOO_COMBO16_CALIB_SWEEP_HALFWORD_TO_BIT:
            iter = 2;
            break;
        case SHMOO_COMBO16_CALIB_SWEEP_WORD_TO_BIT:
            iter = 1;
            break;
        default:
            LOG_ERROR(BSL_LS_SOC_DDR,
                      (BSL_META_U(unit,
                                  "Unsupported 2D calibration sweep: %02d\n"),
                       calibSweep));
            return SOC_E_FAIL;
    }
    
    if(calibSweep != SHMOO_COMBO16_CALIB_SWEEP_1)
    {
        switch(groupOrder)
        {
            case SHMOO_COMBO16_CALIB_GROUP_ORDER_00112233:
                for(i = 0; i < iter; i++)
                {
                    maxMidPointX = (*scPtr).resultData[i] & 0xFFFF;
                    
                    if(maxMidPointX < (*scPtr).endUI[0])
                    {
                        calibStart[i * groupNumBits] = 0;
                    }
                    else
                    {
                        calibStart[i * groupNumBits] = maxMidPointX - (*scPtr).endUI[0];
                    }
                    
                    if(maxMidPointX + (*scPtr).endUI[0] < sizeX)
                    {
                        calibEnd[i * groupNumBits] = maxMidPointX + (*scPtr).endUI[0];
                    }
                    else
                    {
                        calibEnd[i * groupNumBits] = sizeX;
                    }
                    
                    for(j = 1; j < groupNumBits; j++)
                    {
                        calibStart[(i * groupNumBits) + j] = calibStart[i * groupNumBits];
                        calibEnd[(i * groupNumBits) + j] = calibEnd[i * groupNumBits];
                    }
                }
                break;
            case SHMOO_COMBO16_CALIB_GROUP_ORDER_01230123:
                for(i = 0; i < iter; i++)
                {
                    maxMidPointX = (*scPtr).resultData[i] & 0xFFFF;
                    
                    if(maxMidPointX < (*scPtr).endUI[0])
                    {
                        calibStart[i] = 0;
                    }
                    else
                    {
                        calibStart[i] = maxMidPointX - (*scPtr).endUI[0];
                    }
                    
                    if(maxMidPointX + (*scPtr).endUI[0] < sizeX)
                    {
                        calibEnd[i] = maxMidPointX + (*scPtr).endUI[0];
                    }
                    else
                    {
                        calibEnd[i] = sizeX;
                    }
                }
                
                for(j = 1; j < groupNumBits; j++)
                {
                    for(i = 0; i < iter; i++)
                    {
                        calibStart[(j * iter) + i] = calibStart[i];
                        calibEnd[(j * iter) + i] = calibEnd[i];
                    }
                }
                break;
            case SHMOO_COMBO16_CALIB_GROUP_ORDER_DONT_CARE:
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Invalid 2D calibration group order: %02d\n"),
                           groupOrder));
                return SOC_E_FAIL;
            default:
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported 2D calibration group order: %02d\n"),
                           groupOrder));
                return SOC_E_FAIL;
        }
        
        for(i = iter * groupNumBits; i < SHMOO_COMBO16_WORD; i++)
        {
            calibStart[i] = 0;
            calibEnd[i] = 0;
        }
    }

    switch(calibMode)
    {
        case SHMOO_COMBO16_BIT:
            iter = 32;
            shiftAmount = 0;
            dataMask = 0x1;
            break;
        case SHMOO_COMBO16_BYTE:
            iter = 4;
            shiftAmount = 3;
            dataMask = 0xFF;
            break;
        case SHMOO_COMBO16_HALFWORD:
            iter = 2;
            shiftAmount = 4;
            dataMask = 0xFFFF;
            break;
        case SHMOO_COMBO16_WORD:
            iter = 1;
            shiftAmount = 5;
            dataMask = 0xFFFFFFFF;
            break;
        default:
            LOG_ERROR(BSL_LS_SOC_DDR,
                      (BSL_META_U(unit,
                                  "Unsupported 2D calibration mode: %02d\n"),
                       calibMode));
            return SOC_E_FAIL;
    }

    for(i = 0; i < iter; i++)
    {
        (*scPtr).resultData[i] = 0;
        maxPassLengthArray[i] = 0;
    }

    for(y = yCapMin; y < yCapMax; y++)
    {   
        for(i = 0; i < iter; i++)
        {
            passStart = -1;
            failStart = -1;
            passLength = -1;
            passStartSeen = -1;
            failStartSeen = -1;
            maxPassStart = -2;
            maxPassLength = -2;
            maxMidPointX = -2;
            for(x = calibStart[i]; x < calibEnd[i]; x++)
            {                
                switch(decisionMode)
                {
                    case SHMOO_COMBO16_CALIB_PASS_ALL:
                        result = ((*scPtr).result2D[xStart + x] >> (i << shiftAmount)) & dataMask;
                        break;
                    case SHMOO_COMBO16_CALIB_PASS_ANY:
                        result = ((((*scPtr).result2D[xStart + x] >> (i << shiftAmount)) & dataMask) == dataMask);
                        break;
                    default:
                        LOG_ERROR(BSL_LS_SOC_DDR,
                                  (BSL_META_U(unit,
                                              "Unsupported calibration decision mode: %02d\n"),
                                   decisionMode));
                        return SOC_E_FAIL;
                }
                
                if(result)
                {   /* FAIL */
                    if(failStart < 0) {
                        failStart = x;
                        if(maxPassLength < passLength)
                        {
                            maxPassStart = passStart;
                            maxPassLength = passLength;
                        }
                        passStart = -1;
                        passLength = -1;
                        if((failStartSeen < 0) && (maxPassLength > 0))
                        {
                            failStartSeen = x;
                        }
                    }
                }
                else
                {   /* PASS */
                    if(passStart < 0)
                    {
                        passStart = x;
                        passLength = 1;
                        failStart = -1;
                        if((passStartSeen < 0) && (passLength < x))
                        {
                            passStartSeen = x;
                        }
                    }
                    else
                    {
                        passLength++;
                    }
                    
                    if(x == calibEnd[i] - 1)
                    {
                        if(maxPassLength < passLength)
                        {
                            maxPassStart = passStart;
                            maxPassLength = passLength;
                        }
                    }
                }
            }
            
            switch(calibPos)
            {
                case SHMOO_COMBO16_CALIB_FAIL_START:
                case SHMOO_COMBO16_CALIB_RISING_EDGE:
                    if(failStartSeen > 0)
                    {
                        maxMidPointX = failStartSeen;
                        (*scPtr).resultData[i] = (y << 16) | (maxMidPointX & 0xFFFF);
                    }
                    break;
                case SHMOO_COMBO16_CALIB_PASS_START:
                case SHMOO_COMBO16_CALIB_FALLING_EDGE:
                    if(passStartSeen > 0)
                    {
                        maxMidPointX = passStartSeen;
                        (*scPtr).resultData[i] = (y << 16) | (maxMidPointX & 0xFFFF);
                    }
                    break;
                case SHMOO_COMBO16_CALIB_CENTER_PASS:
                    if((maxPassLength > 0) && (maxPassLengthArray[i] < maxPassLength))
                    {
                        maxMidPointX = (maxPassStart + maxPassStart + maxPassLength) >> 1;
                        (*scPtr).resultData[i] = (y << 16) | (maxMidPointX & 0xFFFF);
                        maxPassLengthArray[i] = maxPassLength;
                    }
                    break;
                case SHMOO_COMBO16_CALIB_ANY_EDGE:
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported calibration position: %02d\n"),
                               calibPos));
                    return SOC_E_FAIL;
            }
        }
        xStart += sizeX;
    }

    (*scPtr).calibMode = calibMode;
    (*scPtr).calibPos = calibPos;
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_calib_2D(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    switch((*scPtr).shmooType)
    {
        case SHMOO_COMBO16_ADDR_CTRL_SHORT:
            return _combo16_calib_2D(unit, drc_ndx, scPtr, SHMOO_COMBO16_WORD, SHMOO_COMBO16_CALIB_CENTER_PASS, SHMOO_COMBO16_CALIB_SWEEP_1, SHMOO_COMBO16_CALIB_PASS_ALL, SHMOO_COMBO16_CALIB_GROUP_ORDER_DONT_CARE, 0);
        case SHMOO_COMBO16_DQS2CK:
            /* return _combo16_calib_2D(unit, drc_ndx, scPtr, SHMOO_COMBO16_BYTE, SHMOO_COMBO16_CALIB_RISING_EDGE, SHMOO_COMBO16_CALIB_SWEEP_1, SHMOO_COMBO16_CALIB_PASS_ALL, SHMOO_COMBO16_CALIB_GROUP_ORDER_DONT_CARE, 0); */
            break;
        case SHMOO_COMBO16_WCK2CK:
            /* return _combo16_calib_2D(unit, drc_ndx, scPtr, SHMOO_COMBO16_HALFWORD, SHMOO_COMBO16_CALIB_FALLING_EDGE, SHMOO_COMBO16_CALIB_SWEEP_1, SHMOO_COMBO16_CALIB_PASS_ALL, SHMOO_COMBO16_CALIB_GROUP_ORDER_DONT_CARE, 0); */
            break;
        case SHMOO_COMBO16_RD_EN_FISH:
            break;
        case SHMOO_COMBO16_RD_START_FISH:
            break;
        case SHMOO_COMBO16_EDC_START_FISH:
            break;
        case SHMOO_COMBO16_RD_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                _combo16_calib_2D(unit, drc_ndx, scPtr, SHMOO_COMBO16_BYTE, SHMOO_COMBO16_CALIB_CENTER_PASS, SHMOO_COMBO16_CALIB_SWEEP_1, SHMOO_COMBO16_CALIB_PASS_ANY, SHMOO_COMBO16_CALIB_GROUP_ORDER_DONT_CARE, 0);
                return _combo16_calib_2D(unit, drc_ndx, scPtr, SHMOO_COMBO16_BIT, SHMOO_COMBO16_CALIB_CENTER_PASS, SHMOO_COMBO16_CALIB_SWEEP_BYTE_TO_BIT, SHMOO_COMBO16_CALIB_PASS_ALL, SHMOO_COMBO16_CALIB_GROUP_ORDER_00112233, 8);
            }
            return _combo16_calib_2D(unit, drc_ndx, scPtr, SHMOO_COMBO16_BIT, SHMOO_COMBO16_CALIB_CENTER_PASS, SHMOO_COMBO16_CALIB_SWEEP_1, SHMOO_COMBO16_CALIB_PASS_ALL, SHMOO_COMBO16_CALIB_GROUP_ORDER_DONT_CARE, 0);
        case SHMOO_COMBO16_DBI_EDC_RD_DESKEW:
            return _combo16_calib_2D(unit, drc_ndx, scPtr, SHMOO_COMBO16_BIT, SHMOO_COMBO16_CALIB_CENTER_PASS, SHMOO_COMBO16_CALIB_SWEEP_BYTE_TO_BIT, SHMOO_COMBO16_CALIB_PASS_ALL, SHMOO_COMBO16_CALIB_GROUP_ORDER_01230123, 2);
        case SHMOO_COMBO16_RD_SHORT:
            return _combo16_calib_2D(unit, drc_ndx, scPtr, SHMOO_COMBO16_BYTE, SHMOO_COMBO16_CALIB_CENTER_PASS, SHMOO_COMBO16_CALIB_SWEEP_1, SHMOO_COMBO16_CALIB_PASS_ALL, SHMOO_COMBO16_CALIB_GROUP_ORDER_DONT_CARE, 0);
        case SHMOO_COMBO16_WR_DESKEW:
            return _combo16_calib_2D(unit, drc_ndx, scPtr, SHMOO_COMBO16_BIT, SHMOO_COMBO16_CALIB_CENTER_PASS, SHMOO_COMBO16_CALIB_SWEEP_1, SHMOO_COMBO16_CALIB_PASS_ALL, SHMOO_COMBO16_CALIB_GROUP_ORDER_DONT_CARE, 0);
        case SHMOO_COMBO16_DBI_WR_DESKEW:
            return _combo16_calib_2D(unit, drc_ndx, scPtr, SHMOO_COMBO16_BIT, SHMOO_COMBO16_CALIB_CENTER_PASS, SHMOO_COMBO16_CALIB_SWEEP_1, SHMOO_COMBO16_CALIB_PASS_ALL, SHMOO_COMBO16_CALIB_GROUP_ORDER_DONT_CARE, 0);
        case SHMOO_COMBO16_WR_SHORT:
            return _combo16_calib_2D(unit, drc_ndx, scPtr, SHMOO_COMBO16_BYTE, SHMOO_COMBO16_CALIB_CENTER_PASS, SHMOO_COMBO16_CALIB_SWEEP_1, SHMOO_COMBO16_CALIB_PASS_ALL, SHMOO_COMBO16_CALIB_GROUP_ORDER_DONT_CARE, 0);
        case SHMOO_COMBO16_RD_EXTENDED:
            return _combo16_calib_2D(unit, drc_ndx, scPtr, SHMOO_COMBO16_BYTE, SHMOO_COMBO16_CALIB_CENTER_PASS, SHMOO_COMBO16_CALIB_SWEEP_1, SHMOO_COMBO16_CALIB_PASS_ALL, SHMOO_COMBO16_CALIB_GROUP_ORDER_DONT_CARE, 0);
        case SHMOO_COMBO16_WR_EXTENDED:
            return _combo16_calib_2D(unit, drc_ndx, scPtr, SHMOO_COMBO16_BYTE, SHMOO_COMBO16_CALIB_CENTER_PASS, SHMOO_COMBO16_CALIB_SWEEP_1, SHMOO_COMBO16_CALIB_PASS_ALL, SHMOO_COMBO16_CALIB_GROUP_ORDER_DONT_CARE, 0);
        case SHMOO_COMBO16_ADDR_CTRL_EXTENDED:
            return _combo16_calib_2D(unit, drc_ndx, scPtr, SHMOO_COMBO16_WORD, SHMOO_COMBO16_CALIB_CENTER_PASS, SHMOO_COMBO16_CALIB_SWEEP_1, SHMOO_COMBO16_CALIB_PASS_ALL, SHMOO_COMBO16_CALIB_GROUP_ORDER_DONT_CARE, 0);
        default:
            LOG_ERROR(BSL_LS_SOC_DDR,
                      (BSL_META_U(unit,
                                  "Unsupported shmoo type: %02d\n"),
                       (*scPtr).shmooType));
            return SOC_E_FAIL;
    }
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_set_new_step(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    uint32 calibMode;
    uint32 dramType;
    uint32 engageUIshift;
    uint32 min0, min1, min2, min3;
    uint32 max0, max1, max2, max3;
    uint32 val0, val1, val2, val3;
    uint32 x, xMin, xMax;
    uint32 b;
    int MRfield0, MRfield1, MRfield2, MRfield3;
    uint32 data;
    uint32 availableStep;
    uint32 initAdjustment;
    combo16_ui_position_t up;
    int swap;
    combo16_shmoo_dram_info_t* shmoo_dram_info;

    SOC_IF_ERROR_RETURN( _shmoo_combo16_drc_dram_info_access(unit, &shmoo_dram_info));    
        
    calibMode = (*scPtr).calibMode;
    dramType = (*scPtr).dramType;
    engageUIshift = (*scPtr).engageUIshift;
    
    switch((*scPtr).shmooType)
    {
        case SHMOO_COMBO16_ADDR_CTRL_SHORT:
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_BYTE:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_HALFWORD:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_WORD:
                    SOC_IF_ERROR_RETURN(READ_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, drc_ndx, &data));
                    if(engageUIshift)
                    {
                        _combo16_calculate_ui_position((*scPtr).resultData[0] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[0] & 0xFFFF));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, drc_ndx, data));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_ADDRr(unit, drc_ndx, data));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_CTRLr(unit, drc_ndx, data));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_CTRLr(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_adt(unit, drc_ndx, 0));
                    SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_dram_init(unit, drc_ndx, 11));
                    SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_adt(unit, drc_ndx, 1));
                    sal_usleep(SHMOO_COMBO16_DEEP_SLEEP);
                    break;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_DQS2CK:
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_BYTE:
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, &data));
                    if(engageUIshift)
                    {
                        _combo16_calculate_ui_position((*scPtr).resultData[0] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        
                        _combo16_calculate_ui_position((*scPtr).resultData[1] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        
                        _combo16_calculate_ui_position((*scPtr).resultData[2] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        
                        _combo16_calculate_ui_position((*scPtr).resultData[3] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[0] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));

                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[1] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[2] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));

                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[3] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                    }
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                case SHMOO_COMBO16_HALFWORD:
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, &data));
                    if(engageUIshift)
                    {
                        _combo16_calculate_ui_position((*scPtr).resultData[0] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        
                        _combo16_calculate_ui_position((*scPtr).resultData[1] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[0] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[1] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                    }
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                case SHMOO_COMBO16_WORD:
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, &data));
                    if(engageUIshift)
                    {
                        _combo16_calculate_ui_position((*scPtr).resultData[0] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[0] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                    }
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_WCK2CK:
            break;
        case SHMOO_COMBO16_RD_EN_FISH:
            break;
        case SHMOO_COMBO16_RD_START_FISH:
            break;
        case SHMOO_COMBO16_EDC_START_FISH:
            break;
        case SHMOO_COMBO16_RD_DESKEW:
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    min0 = (*scPtr).sizeX;
                    min1 = min0;
                    min2 = min0;
                    min3 = min0;
                    max0 = 0;
                    max1 = 0;
                    max2 = 0;
                    max3 = 0;
                    
                    for(b = 0; b < SHMOO_COMBO16_BYTE; b++)
                    {
                        val0 = (*scPtr).resultData[b] & 0xFFFF;
                        val1 = (*scPtr).resultData[b + 8] & 0xFFFF;
                        val2 = (*scPtr).resultData[b + 16] & 0xFFFF;
                        val3 = (*scPtr).resultData[b + 24] & 0xFFFF;
                        
                        if(min0 > val0)
                        {
                            min0 = val0;
                        }
                        if(min1 > val1)
                        {
                            min1 = val1;
                        }
                        if(min2 > val2)
                        {
                            min2 = val2;
                        }
                        if(min3 > val3)
                        {
                            min3 = val3;
                        }
                        
                        if(max0 < val0)
                        {
                            max0 = val0;
                        }
                        if(max1 < val1)
                        {
                            max1 = val1;
                        }
                        if(max2 < val2)
                        {
                            max2 = val2;
                        }
                        if(max3 < val3)
                        {
                            max3 = val3;
                        }
                    }
                    
                    if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
                    {
                        if((max0 - min0) > SHMOO_COMBO16_MIN_VDL_LENGTH)
                        {
                            val0 = 1;
                            max0 = min0 + SHMOO_COMBO16_MIN_VDL_LENGTH - 1;
                        }
                        else
                        {
                            val0 = 0;
                        }
                        if((max1 - min1) > SHMOO_COMBO16_MIN_VDL_LENGTH)
                        {
                            val1 = 1;
                            max1 = min1 + SHMOO_COMBO16_MIN_VDL_LENGTH - 1;
                        }
                        else
                        {
                            val1 = 0;
                        }
                        if((max2 - min2) > SHMOO_COMBO16_MIN_VDL_LENGTH)
                        {
                            val2 = 1;
                            max2 = min2 + SHMOO_COMBO16_MIN_VDL_LENGTH - 1;
                        }
                        else
                        {
                            val2 = 0;
                        }
                        if((max3 - min3) > SHMOO_COMBO16_MIN_VDL_LENGTH)
                        {
                            val3 = 1;
                            max3 = min3 + SHMOO_COMBO16_MIN_VDL_LENGTH - 1;
                        }
                        else
                        {
                            val3 = 0;
                        }
                        
                        if(val0 || val1 || val2 || val3)
                        {
                            LOG_VERBOSE(BSL_LS_SOC_DDR,
                                        (BSL_META_U(unit,
                                                    "WARNING: Bit skew beyond available MIN VDL range. Results may not be optimal.\n")));
                        }
                    }
                    else
                    {
                        availableStep = SHMOO_COMBO16_MIN_VDL_LENGTH - SHMOO_COMBO16_DDR4_INIT_READ_MIN_VDL_POS - 1;
                        if((max0 - min0) > availableStep)
                        {
                            val0 = 1;
                            min0 = max0 - availableStep;
                        }
                        else
                        {
                            val0 = 0;
                        }
                        if((max1 - min1) > availableStep)
                        {
                            val1 = 1;
                            min1 = max1 - availableStep;
                        }
                        else
                        {
                            val1 = 0;
                        }
                        if((max2 - min2) > availableStep)
                        {
                            val2 = 1;
                            min1 = max1 - availableStep;
                        }
                        else
                        {
                            val2 = 0;
                        }
                        if((max3 - min3) > availableStep)
                        {
                            val3 = 1;
                            min1 = max1 - availableStep;
                        }
                        else
                        {
                            val3 = 0;
                        }
                        
                        if(val0 || val1 || val2 || val3)
                        {
                            LOG_VERBOSE(BSL_LS_SOC_DDR,
                                        (BSL_META_U(unit,
                                                    "WARNING: Bit skew beyond available MIN VDL range. Results may not be optimal.\n")));
                        }
                    }
                    
                    if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
                    {
                        initAdjustment = SHMOO_COMBO16_DDR4_INIT_READ_MIN_VDL_POS;
                    }
                    else
                    {
                        initAdjustment = 0;
                    }
                    
                    data = 0;
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MIN_VDL_BIT0r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max0 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][0]] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT0r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MIN_VDL_BIT1r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max0 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][1]] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT1r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MIN_VDL_BIT2r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max0 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][2]] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT2r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MIN_VDL_BIT3r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max0 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][3]] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT3r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MIN_VDL_BIT4r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max0 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][4]] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT4r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MIN_VDL_BIT5r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max0 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][5]] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT5r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MIN_VDL_BIT6r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max0 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][6]] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT6r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MIN_VDL_BIT7r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max0 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][7]] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT7r(unit, drc_ndx, data));
                    
                    if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
                    {
                        val0 = ((*scPtr).resultData[0] & 0xFFFF) + ((*scPtr).resultData[1] & 0xFFFF) + ((*scPtr).resultData[2] & 0xFFFF) + ((*scPtr).resultData[3] & 0xFFFF)
                                + ((*scPtr).resultData[4] & 0xFFFF) + ((*scPtr).resultData[5] & 0xFFFF) + ((*scPtr).resultData[6] & 0xFFFF) + ((*scPtr).resultData[7] & 0xFFFF);
                        val0 = val0 >> 3;
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max0 - val0 + initAdjustment));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
                    }
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MIN_VDL_BIT0r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max1 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][0] + 8] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT0r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MIN_VDL_BIT1r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max1 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][1] + 8] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT1r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MIN_VDL_BIT2r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max1 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][2] + 8] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT2r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MIN_VDL_BIT3r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max1 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][3] + 8] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT3r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MIN_VDL_BIT4r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max1 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][4] + 8] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT4r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MIN_VDL_BIT5r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max1 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][5] + 8] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT5r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MIN_VDL_BIT6r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max1 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][6] + 8] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT6r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MIN_VDL_BIT7r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max1 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][7] + 8] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT7r(unit, drc_ndx, data));
                    
                    if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
                    {
                        val1 = ((*scPtr).resultData[8] & 0xFFFF) + ((*scPtr).resultData[9] & 0xFFFF) + ((*scPtr).resultData[10] & 0xFFFF) + ((*scPtr).resultData[11] & 0xFFFF)
                                + ((*scPtr).resultData[12] & 0xFFFF) + ((*scPtr).resultData[13] & 0xFFFF) + ((*scPtr).resultData[14] & 0xFFFF) + ((*scPtr).resultData[15] & 0xFFFF);
                        val1 = val1 >> 3;
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max1 - val1 + initAdjustment));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
                    }
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MIN_VDL_BIT0r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max2 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][0] + 16] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT0r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MIN_VDL_BIT1r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max2 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][1] + 16] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT1r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MIN_VDL_BIT2r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max2 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][2] + 16] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT2r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MIN_VDL_BIT3r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max2 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][3] + 16] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT3r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MIN_VDL_BIT4r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max2 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][4] + 16] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT4r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MIN_VDL_BIT5r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max2 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][5] + 16] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT5r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MIN_VDL_BIT6r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max2 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][6] + 16] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT6r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MIN_VDL_BIT7r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max2 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][7] + 16] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT7r(unit, drc_ndx, data));
                    
                    if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
                    {
                        val2 = ((*scPtr).resultData[16] & 0xFFFF) + ((*scPtr).resultData[17] & 0xFFFF) + ((*scPtr).resultData[18] & 0xFFFF) + ((*scPtr).resultData[19] & 0xFFFF)
                                + ((*scPtr).resultData[20] & 0xFFFF) + ((*scPtr).resultData[21] & 0xFFFF) + ((*scPtr).resultData[22] & 0xFFFF) + ((*scPtr).resultData[23] & 0xFFFF);
                        val2 = val2 >> 3;
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max2 - val2 + initAdjustment));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
                    }
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MIN_VDL_BIT0r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max3 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][0] + 24] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT0r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MIN_VDL_BIT1r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max3 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][1] + 24] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT1r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MIN_VDL_BIT2r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max3 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][2] + 24] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT2r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MIN_VDL_BIT3r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max3 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][3] + 24] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT3r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MIN_VDL_BIT4r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max3 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][4] + 24] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT4r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MIN_VDL_BIT5r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max3 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][5] + 24] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT5r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MIN_VDL_BIT6r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max3 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][6] + 24] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT6r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MIN_VDL_BIT7r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max3 - ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][7] + 24] & 0xFFFF) + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT7r(unit, drc_ndx, data));
                    
                    if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
                    {
                        val3 = ((*scPtr).resultData[24] & 0xFFFF) + ((*scPtr).resultData[25] & 0xFFFF) + ((*scPtr).resultData[26] & 0xFFFF) + ((*scPtr).resultData[27] & 0xFFFF)
                                + ((*scPtr).resultData[28] & 0xFFFF) + ((*scPtr).resultData[29] & 0xFFFF) + ((*scPtr).resultData[30] & 0xFFFF) + ((*scPtr).resultData[31] & 0xFFFF);
                        val3 = val3 >> 3;
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max3 - val3 + initAdjustment));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
                    }
                    
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x1, max0 - initAdjustment, scPtr);
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x2, max1 - initAdjustment, scPtr);
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x4, max2 - initAdjustment, scPtr);
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x8, max3 - initAdjustment, scPtr);
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                case SHMOO_COMBO16_BYTE:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_HALFWORD:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_WORD:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
            }
        
           if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
                    
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                    
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
            }
            break;
        case SHMOO_COMBO16_DBI_EDC_RD_DESKEW:
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, data, SOC_PHY_DDRC16_MAX_VDL_STEPf, &max0));
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr, data, SOC_PHY_DDRC16_MAX_VDL_STEPf, &max1));
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr, data, SOC_PHY_DDRC16_MAX_VDL_STEPf, &max2));
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr, data, SOC_PHY_DDRC16_MAX_VDL_STEPf, &max3));
                    
                    data = 0;
                    if(max0 < ((*scPtr).resultData[0] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max0 - ((*scPtr).resultData[0] & 0xFFFF)));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
                    
                    data = 0;
                    if(max0 < ((*scPtr).resultData[4] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max0 - ((*scPtr).resultData[4] & 0xFFFF)));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
                    
                    data = 0;
                    if(max1 < ((*scPtr).resultData[1] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max1 - ((*scPtr).resultData[1] & 0xFFFF)));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
                    
                    data = 0;
                    if(max1 < ((*scPtr).resultData[5] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max1 - ((*scPtr).resultData[5] & 0xFFFF)));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
                    
                    data = 0;
                    if(max2 < ((*scPtr).resultData[2] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max2 - ((*scPtr).resultData[2] & 0xFFFF)));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
                    
                    data = 0;
                    if(max2 < ((*scPtr).resultData[6] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max2 - ((*scPtr).resultData[6] & 0xFFFF)));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
                    
                    data = 0;
                    if(max3 < ((*scPtr).resultData[3] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max3 - ((*scPtr).resultData[3] & 0xFFFF)));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
                    
                    data = 0;
                    if(max3 < ((*scPtr).resultData[7] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, max3 - ((*scPtr).resultData[7] & 0xFFFF)));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                case SHMOO_COMBO16_BYTE:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_HALFWORD:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_WORD:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_SHORT:
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_BYTE:
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x1, (*scPtr).resultData[0] & 0xFFFF, scPtr);
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x2, (*scPtr).resultData[1] & 0xFFFF, scPtr);
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x4, (*scPtr).resultData[2] & 0xFFFF, scPtr);
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x8, (*scPtr).resultData[3] & 0xFFFF, scPtr);

                    xMin = (*scPtr).resultData[0] & 0xFFFF;
                    xMax = xMin;
                    
                    x = (*scPtr).resultData[1] & 0xFFFF;
                    if(x < xMin)
                    {
                        xMin = x;
                    }
                    if(x > xMax)
                    {
                        xMax = x;
                    }
                    
                    x = (*scPtr).resultData[2] & 0xFFFF;
                    if(x < xMin)
                    {
                        xMin = x;
                    }
                    if(x > xMax)
                    {
                        xMax = x;
                    }
                    
                    x = (*scPtr).resultData[3] & 0xFFFF;
                    if(x < xMin)
                    {
                        xMin = x;
                    }
                    if(x > xMax)
                    {
                        xMax = x;
                    }
                    
                    xMin = xMin % (*scPtr).endUI[1];
                    xMax = xMax % (*scPtr).endUI[1];
                    
                    if(xMax < xMin)
                    {
                        x = xMin;
                        xMin = xMax;
                        xMax = x;
                    }
                    
                    SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESERVED_REGr(unit, drc_ndx, &data));
                    if(xMin < ((*scPtr).endUI[2] >> 2))
                    {
                        if((xMax - xMin) < (*scPtr).endUI[0])
                        {
                            data = (((((xMax - xMin) + ((*scPtr).endUI[0] >> 2)) << 4) | 0x4) & 0x1FFC) | (data & 0xFFFFE003);
                        }
                        else
                        {
                            data = (((((xMin + (*scPtr).endUI[1]) - xMax) << 4) | 0x4) & 0x1FFC) | (data & 0xFFFFE003);
                        }
                    }
                    else
                    {
                        data = (((((*scPtr).endUI[0] >> 2) << 4) | 0x8) & 0x1FFC) | (data & 0xFFFFE003);
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESERVED_REGr(unit, drc_ndx, data));
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                case SHMOO_COMBO16_HALFWORD:
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x3, (*scPtr).resultData[0] & 0xFFFF, scPtr);
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0xC, (*scPtr).resultData[1] & 0xFFFF, scPtr);
                    
                    xMin = (*scPtr).resultData[0] & 0xFFFF;
                    xMax = xMin;
                    
                    x = (*scPtr).resultData[1] & 0xFFFF;
                    if(x < xMin)
                    {
                        xMin = x;
                    }
                    if(x > xMax)
                    {
                        xMax = x;
                    }
                    
                    xMin = xMin % (*scPtr).endUI[1];
                    xMax = xMax % (*scPtr).endUI[1];
                    
                    if(xMax < xMin)
                    {
                        x = xMin;
                        xMin = xMax;
                        xMax = x;
                    }
                    
                    SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESERVED_REGr(unit, drc_ndx, &data));
                    if(xMin < ((*scPtr).endUI[2] >> 2))
                    {
                        if((xMax - xMin) < (*scPtr).endUI[0])
                        {
                            data = (((((xMax - xMin) + ((*scPtr).endUI[0] >> 2)) << 4) | 0x4) & 0x1FFC) | (data & 0xFFFFE003);
                        }
                        else
                        {
                            data = (((((xMin + (*scPtr).endUI[1]) - xMax) << 4) | 0x4) & 0x1FFC) | (data & 0xFFFFE003);
                        }
                    }
                    else
                    {
                        data = (((((*scPtr).endUI[0] >> 2) << 4) | 0x8) & 0x1FFC) | (data & 0xFFFFE003);
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESERVED_REGr(unit, drc_ndx, data));
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                case SHMOO_COMBO16_WORD:
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0xF, (*scPtr).resultData[0] & 0xFFFF, scPtr);
                    
                    xMin = (*scPtr).resultData[0] & 0xFFFF;
                    
                    xMin = xMin % (*scPtr).endUI[1];
                    
                    SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESERVED_REGr(unit, drc_ndx, &data));
                    if(xMin < ((*scPtr).endUI[2] >> 2))
                    {
                        data = ((((0x000) << 4) | 0x4) & 0x1FFC) | (data & 0xFFFFE003);
                    }
                    else
                    {
                        data = ((((0x000) << 4) | 0x8) & 0x1FFC) | (data & 0xFFFFE003);
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESERVED_REGr(unit, drc_ndx, data));
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
            }
        
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
                    
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                    
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
            }
            break;
        case SHMOO_COMBO16_WR_DESKEW:
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    min0 = (*scPtr).sizeX;
                    min1 = min0;
                    min2 = min0;
                    min3 = min0;
                    max0 = 0;
                    max1 = 0;
                    max2 = 0;
                    max3 = 0;
                    
                    for(b = 0; b < SHMOO_COMBO16_BYTE; b++)
                    {
                        val0 = (*scPtr).resultData[b] & 0xFFFF;
                        val1 = (*scPtr).resultData[b + 8] & 0xFFFF;
                        val2 = (*scPtr).resultData[b + 16] & 0xFFFF;
                        val3 = (*scPtr).resultData[b + 24] & 0xFFFF;
                        
                        if(min0 > val0)
                        {
                            min0 = val0;
                        }
                        if(min1 > val1)
                        {
                            min1 = val1;
                        }
                        if(min2 > val2)
                        {
                            min2 = val2;
                        }
                        if(min3 > val3)
                        {
                            min3 = val3;
                        }
                        
                        if(max0 < val0)
                        {
                            max0 = val0;
                        }
                        if(max1 < val1)
                        {
                            max1 = val1;
                        }
                        if(max2 < val2)
                        {
                            max2 = val2;
                        }
                        if(max3 < val3)
                        {
                            max3 = val3;
                        }
                    }
                    
                    if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
                    {
                        if((max0 - min0) > SHMOO_COMBO16_MIN_VDL_LENGTH)
                        {
                            val0 = 1;
                            max0 = min0 + SHMOO_COMBO16_MIN_VDL_LENGTH - 1;
                        }
                        else
                        {
                            val0 = 0;
                        }
                        if((max1 - min1) > SHMOO_COMBO16_MIN_VDL_LENGTH)
                        {
                            val1 = 1;
                            max1 = min1 + SHMOO_COMBO16_MIN_VDL_LENGTH - 1;
                        }
                        else
                        {
                            val1 = 0;
                        }
                        if((max2 - min2) > SHMOO_COMBO16_MIN_VDL_LENGTH)
                        {
                            val2 = 1;
                            max2 = min2 + SHMOO_COMBO16_MIN_VDL_LENGTH - 1;
                        }
                        else
                        {
                            val2 = 0;
                        }
                        if((max3 - min3) > SHMOO_COMBO16_MIN_VDL_LENGTH)
                        {
                            val3 = 1;
                            max3 = min3 + SHMOO_COMBO16_MIN_VDL_LENGTH - 1;
                        }
                        else
                        {
                            val3 = 0;
                        }
                        
                        if(val0 || val1 || val2 || val3)
                        {
                            LOG_VERBOSE(BSL_LS_SOC_DDR,
                                        (BSL_META_U(unit,
                                                    "WARNING: Bit skew beyond available MIN VDL range. Results may not be optimal.\n")));
                        }
                    }
                    else
                    {
                        availableStep = SHMOO_COMBO16_MIN_VDL_LENGTH - SHMOO_COMBO16_DDR4_INIT_WRITE_MIN_VDL_POS - 1;
                        if((max0 - min0) > availableStep)
                        {
                            val0 = 1;
                            max0 = min0 + availableStep;
                        }
                        else
                        {
                            val0 = 0;
                        }
                        if((max1 - min1) > availableStep)
                        {
                            val1 = 1;
                            max1 = min1 + availableStep;
                        }
                        else
                        {
                            val1 = 0;
                        }
                        if((max2 - min2) > availableStep)
                        {
                            val2 = 1;
                            max2 = min2 + availableStep;
                        }
                        else
                        {
                            val2 = 0;
                        }
                        if((max3 - min3) > availableStep)
                        {
                            val3 = 1;
                            max3 = min3 + availableStep;
                        }
                        else
                        {
                            val3 = 0;
                        }
                        
                        if(val0 || val1 || val2 || val3)
                        {
                            LOG_VERBOSE(BSL_LS_SOC_DDR,
                                        (BSL_META_U(unit,
                                                    "WARNING: Bit skew beyond available MIN VDL range. Results may not be optimal.\n")));
                        }
                    }
                    
                    if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
                    {
                        initAdjustment = SHMOO_COMBO16_DDR4_INIT_WRITE_MIN_VDL_POS;
                    }
                    else
                    {
                        initAdjustment = 0;
                    }
                    
                    data = 0;
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MIN_VDL_BIT0r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][0]] & 0xFFFF) - min0 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MIN_VDL_BIT1r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][1]] & 0xFFFF) - min0 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MIN_VDL_BIT2r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][2]] & 0xFFFF) - min0 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MIN_VDL_BIT3r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][3]] & 0xFFFF) - min0 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MIN_VDL_BIT4r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][4]] & 0xFFFF) - min0 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MIN_VDL_BIT5r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][5]] & 0xFFFF) - min0 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MIN_VDL_BIT6r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][6]] & 0xFFFF) - min0 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MIN_VDL_BIT7r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][0][7]] & 0xFFFF) - min0 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, data));
                    
                    if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
                    {
                        val0 = ((*scPtr).resultData[0] & 0xFFFF) + ((*scPtr).resultData[1] & 0xFFFF) + ((*scPtr).resultData[2] & 0xFFFF) + ((*scPtr).resultData[3] & 0xFFFF)
                                + ((*scPtr).resultData[4] & 0xFFFF) + ((*scPtr).resultData[5] & 0xFFFF) + ((*scPtr).resultData[6] & 0xFFFF) + ((*scPtr).resultData[7] & 0xFFFF);
                        val0 = val0 >> 3;
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, val0 - min0 + initAdjustment));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
                    /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data)); */
                    }
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MIN_VDL_BIT0r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][0] + 8] & 0xFFFF) - min1 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MIN_VDL_BIT1r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][1] + 8] & 0xFFFF) - min1 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MIN_VDL_BIT2r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][2] + 8] & 0xFFFF) - min1 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MIN_VDL_BIT3r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][3] + 8] & 0xFFFF) - min1 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MIN_VDL_BIT4r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][4] + 8] & 0xFFFF) - min1 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MIN_VDL_BIT5r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][5] + 8] & 0xFFFF) - min1 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MIN_VDL_BIT6r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][6] + 8] & 0xFFFF) - min1 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MIN_VDL_BIT7r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][1][7] + 8] & 0xFFFF) - min1 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, data));
                    
                    if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
                    {
                        val1 = ((*scPtr).resultData[8] & 0xFFFF) + ((*scPtr).resultData[9] & 0xFFFF) + ((*scPtr).resultData[10] & 0xFFFF) + ((*scPtr).resultData[11] & 0xFFFF)
                                + ((*scPtr).resultData[12] & 0xFFFF) + ((*scPtr).resultData[13] & 0xFFFF) + ((*scPtr).resultData[14] & 0xFFFF) + ((*scPtr).resultData[15] & 0xFFFF);
                        val1 = val1 >> 3;
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, val1 - min1 + initAdjustment));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
                    /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data)); */
                    }
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MIN_VDL_BIT0r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][0] + 16] & 0xFFFF) - min2 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MIN_VDL_BIT1r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][1] + 16] & 0xFFFF) - min2 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MIN_VDL_BIT2r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][2] + 16] & 0xFFFF) - min2 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MIN_VDL_BIT3r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][3] + 16] & 0xFFFF) - min2 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MIN_VDL_BIT4r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][4] + 16] & 0xFFFF) - min2 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MIN_VDL_BIT5r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][5] + 16] & 0xFFFF) - min2 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MIN_VDL_BIT6r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][6] + 16] & 0xFFFF) - min2 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MIN_VDL_BIT7r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][2][7] + 16] & 0xFFFF) - min2 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, data));
                    
                    if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
                    {
                        val2 = ((*scPtr).resultData[16] & 0xFFFF) + ((*scPtr).resultData[17] & 0xFFFF) + ((*scPtr).resultData[18] & 0xFFFF) + ((*scPtr).resultData[19] & 0xFFFF)
                                + ((*scPtr).resultData[20] & 0xFFFF) + ((*scPtr).resultData[21] & 0xFFFF) + ((*scPtr).resultData[22] & 0xFFFF) + ((*scPtr).resultData[23] & 0xFFFF);
                        val2 = val2 >> 3;
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, val2 - min2 + initAdjustment));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
                    /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data)); */
                    }
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MIN_VDL_BIT0r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][0] + 24] & 0xFFFF) - min3 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MIN_VDL_BIT1r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][1] + 24] & 0xFFFF) - min3 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MIN_VDL_BIT2r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][2] + 24] & 0xFFFF) - min3 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MIN_VDL_BIT3r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][3] + 24] & 0xFFFF) - min3 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MIN_VDL_BIT4r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][4] + 24] & 0xFFFF) - min3 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MIN_VDL_BIT5r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][5] + 24] & 0xFFFF) - min3 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MIN_VDL_BIT6r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][6] + 24] & 0xFFFF) - min3 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, data));
                    
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MIN_VDL_BIT7r, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[shmoo_dram_info->dq_swap[drc_ndx][3][7] + 24] & 0xFFFF) - min3 + initAdjustment));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, data));
                    
                    if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
                    {
                        val3 = ((*scPtr).resultData[24] & 0xFFFF) + ((*scPtr).resultData[25] & 0xFFFF) + ((*scPtr).resultData[26] & 0xFFFF) + ((*scPtr).resultData[27] & 0xFFFF)
                                + ((*scPtr).resultData[28] & 0xFFFF) + ((*scPtr).resultData[29] & 0xFFFF) + ((*scPtr).resultData[30] & 0xFFFF) + ((*scPtr).resultData[31] & 0xFFFF);
                        val3 = val3 >> 3;
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, val3 - min3 + initAdjustment));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
                    /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data)); */
                    }
                    
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
                    if(engageUIshift)
                    {
                        _combo16_calculate_ui_position(min0 - initAdjustment, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        _combo16_calculate_ui_position(min1 - initAdjustment, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        _combo16_calculate_ui_position(min2 - initAdjustment, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        _combo16_calculate_ui_position(min3 - initAdjustment, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, min0 - initAdjustment));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, min1 - initAdjustment));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, min2 - initAdjustment));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, min3 - initAdjustment));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                    }
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                case SHMOO_COMBO16_BYTE:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_HALFWORD:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_WORD:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_DBI_WR_DESKEW:
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, data, SOC_PHY_DDRC16_MAX_VDL_STEPf, &min0));
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, data, SOC_PHY_DDRC16_UI_SHIFTf, &val0));
                    if(val0)
                    {
                        min0 += ((*scPtr).endUI[val0 - 1] + 1);
                    }
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MAX_VDL_DATAr, data, SOC_PHY_DDRC16_MAX_VDL_STEPf, &min1));
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MAX_VDL_DATAr, data, SOC_PHY_DDRC16_UI_SHIFTf, &val1));
                    if(val1)
                    {
                        min1 += ((*scPtr).endUI[val1 - 1] + 1);
                    }
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, data, SOC_PHY_DDRC16_MAX_VDL_STEPf, &min2));
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, data, SOC_PHY_DDRC16_UI_SHIFTf, &val2));
                    if(val2)
                    {
                        min2 += ((*scPtr).endUI[val2 - 1] + 1);
                    }
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MAX_VDL_DATAr, data, SOC_PHY_DDRC16_MAX_VDL_STEPf, &min3));
                    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MAX_VDL_DATAr, data, SOC_PHY_DDRC16_UI_SHIFTf, &val3));
                    if(val3)
                    {
                        min3 += ((*scPtr).endUI[val3 - 1] + 1);
                    }
                    
                    data = 0;
                    if(min0 > ((*scPtr).resultData[0] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[0] & 0xFFFF) - min0));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
                    /*
                    data = 0;
                    if(min0 > ((*scPtr).resultData[4] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[4] & 0xFFFF) - min0));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data));
                    */
                    data = 0;
                    if(min1 > ((*scPtr).resultData[1] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[1] & 0xFFFF) - min1));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
                    /*
                    data = 0;
                    if(min1 > ((*scPtr).resultData[5] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[5] & 0xFFFF) - min1));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data));
                    */
                    data = 0;
                    if(min2 > ((*scPtr).resultData[2] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[2] & 0xFFFF) - min2));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
                    /*
                    data = 0;
                    if(min2 > ((*scPtr).resultData[6] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[6] & 0xFFFF) - min2));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data));
                    */
                    data = 0;
                    if(min3 > ((*scPtr).resultData[3] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MIN_VDL_DBIr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[3] & 0xFFFF) - min3));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
                    /*
                    data = 0;
                    if(min3 > ((*scPtr).resultData[7] & 0xFFFF))
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, 0));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MIN_VDL_EDCr, &data, SOC_PHY_DDRC16_MIN_VDL_STEPf, ((*scPtr).resultData[7] & 0xFFFF) - min3));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data));
                    */
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                case SHMOO_COMBO16_BYTE:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_HALFWORD:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_WORD:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_WR_SHORT:
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_BYTE:
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
                    if(engageUIshift)
                    {
                        _combo16_calculate_ui_position((*scPtr).resultData[0] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        _combo16_calculate_ui_position((*scPtr).resultData[1] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        _combo16_calculate_ui_position((*scPtr).resultData[2] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        _combo16_calculate_ui_position((*scPtr).resultData[3] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[0] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[1] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[2] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[3] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                    }
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                case SHMOO_COMBO16_HALFWORD:
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
                    if(engageUIshift)
                    {
                        _combo16_calculate_ui_position((*scPtr).resultData[0] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        _combo16_calculate_ui_position((*scPtr).resultData[1] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[0] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[1] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                    }
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                case SHMOO_COMBO16_WORD:
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
                    if(engageUIshift)
                    {
                        _combo16_calculate_ui_position((*scPtr).resultData[0] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[0] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                    }
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_EXTENDED:
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_BYTE:
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x1, (*scPtr).resultData[0] & 0xFFFF, scPtr);
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x2, (*scPtr).resultData[1] & 0xFFFF, scPtr);
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x4, (*scPtr).resultData[2] & 0xFFFF, scPtr);
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x8, (*scPtr).resultData[3] & 0xFFFF, scPtr);

                    if(!(*scPtr).restore)
                    {
                        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_VREF_DAC_CONFIGr(unit, drc_ndx, &data));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_VREF_DAC_CONFIGr, &data, SOC_PHY_DDRC16_DATAf, (((*scPtr).resultData[0] >> 16) & 0xFFFF) << 1));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_VREF_DAC_CONFIGr(unit, drc_ndx, data));

                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_VREF_DAC_CONFIGr, &data, SOC_PHY_DDRC16_DATAf, (((*scPtr).resultData[1] >> 16) & 0xFFFF) << 1));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_VREF_DAC_CONFIGr(unit, drc_ndx, data));

                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_VREF_DAC_CONFIGr, &data, SOC_PHY_DDRC16_DATAf, (((*scPtr).resultData[2] >> 16) & 0xFFFF) << 1));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_VREF_DAC_CONFIGr(unit, drc_ndx, data));

                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_VREF_DAC_CONFIGr, &data, SOC_PHY_DDRC16_DATAf, (((*scPtr).resultData[3] >> 16) & 0xFFFF) << 1));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                    }
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                case SHMOO_COMBO16_HALFWORD:
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0x3, (*scPtr).resultData[0] & 0xFFFF, scPtr);
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0xC, (*scPtr).resultData[1] & 0xFFFF, scPtr);
                    if(!(*scPtr).restore)
                    {
                        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_VREF_DAC_CONFIGr(unit, drc_ndx, &data));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_VREF_DAC_CONFIGr, &data, SOC_PHY_DDRC16_DATAf, (((*scPtr).resultData[0] >> 16) & 0xFFFF) << 1));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_VREF_DAC_CONFIGr(unit, drc_ndx, data));

                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_VREF_DAC_CONFIGr, &data, SOC_PHY_DDRC16_DATAf, (((*scPtr).resultData[1] >> 16) & 0xFFFF) << 1));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                    }
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                case SHMOO_COMBO16_WORD:
                    _combo16_move_read_max_vdl_glitch_free(unit, drc_ndx, 0xF, (*scPtr).resultData[0] & 0xFFFF, scPtr);

                    if(!(*scPtr).restore)
                    {
                        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_VREF_DAC_CONFIGr(unit, drc_ndx, &data));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_VREF_DAC_CONFIGr, &data, SOC_PHY_DDRC16_DATAf, (((*scPtr).resultData[0] >> 16) & 0xFFFF) << 1));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                    }
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
            }
        
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
                    
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                    
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
            }

            break;
        case SHMOO_COMBO16_WR_EXTENDED:
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_BYTE:
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
                    if(engageUIshift)
                    {
                        _combo16_calculate_ui_position((*scPtr).resultData[0] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        _combo16_calculate_ui_position((*scPtr).resultData[1] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        _combo16_calculate_ui_position((*scPtr).resultData[2] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        _combo16_calculate_ui_position((*scPtr).resultData[3] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[0] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[1] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[2] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[3] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                    }

                    if(!(*scPtr).restore)
                    {
                        switch(dramType)
                        {
                            case SHMOO_COMBO16_DRAM_TYPE_DDR4:
                                MRfield0 = ((int) (((*scPtr).resultData[0] >> 16) & 0xFFFF));
                                MRfield1 = ((int) (((*scPtr).resultData[1] >> 16) & 0xFFFF));
                                MRfield2 = ((int) (((*scPtr).resultData[2] >> 16) & 0xFFFF));
                                MRfield3 = ((int) (((*scPtr).resultData[3] >> 16) & 0xFFFF));
                                
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield0));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield1));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield2));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield3));
                                
                                MRfield0 = (MRfield0 + MRfield1 + MRfield2 + MRfield3) >> 2;
                                
                                if(MRfield0 > SHMOO_COMBO16_DDR4_VREF_RANGE_CROSSOVER)
                                {   /* Vref Range 1 */
                                    MRfield0 = MRfield0 - 23;
                                }
                                else
                                {   /* Vref Range 2 */
                                    MRfield0 = MRfield0 | 0x40;
                                }
                                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, MRfield0, 0x7F));
                                break;
                            case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
                                MRfield0 = ((int) (((*scPtr).resultData[0] >> 16) & 0xFFFF)) - 7;
                                MRfield1 = ((int) (((*scPtr).resultData[1] >> 16) & 0xFFFF)) - 7;
                                MRfield2 = ((int) (((*scPtr).resultData[2] >> 16) & 0xFFFF)) - 7;
                                MRfield3 = ((int) (((*scPtr).resultData[3] >> 16) & 0xFFFF)) - 7;
                                
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield0));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield1));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield2));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield3));
                                
                                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_dq_byte_pairs_swap_info_get(unit, drc_ndx, &swap));
                                
                                if(swap)
                                {
                                    SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, ((((MRfield0 + MRfield1) >> 1) & 0xF) << 8) | ((((MRfield2 + MRfield3) >> 1) & 0xF) << 4), 0xFF0));
                                }
                                else
                                {
                                    SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, ((((MRfield2 + MRfield3) >> 1) & 0xF) << 8) | ((((MRfield0 + MRfield1) >> 1) & 0xF) << 4), 0xFF0));
                                }
                                break;
                            default:
                                LOG_ERROR(BSL_LS_SOC_DDR,
                                          (BSL_META_U(unit,
                                                      "Unsupported dram type: %02d\n"),
                                           dramType));
                                return SOC_E_FAIL;
                        }
                    }
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                case SHMOO_COMBO16_HALFWORD:
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
                    if(engageUIshift)
                    {
                        _combo16_calculate_ui_position((*scPtr).resultData[0] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        _combo16_calculate_ui_position((*scPtr).resultData[1] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[0] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[1] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                    }

                    if(!(*scPtr).restore)
                    {
                        switch(dramType)
                        {
                            case SHMOO_COMBO16_DRAM_TYPE_DDR4:
                                MRfield0 = ((int) (((*scPtr).resultData[0] >> 16) & 0xFFFF));
                                MRfield2 = ((int) (((*scPtr).resultData[1] >> 16) & 0xFFFF));
                                
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield0));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield0));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield2));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield2));
                                
                                MRfield0 = (MRfield0 + MRfield2) >> 1;
                                
                                if(MRfield0 > SHMOO_COMBO16_DDR4_VREF_RANGE_CROSSOVER)
                                {   /* Vref Range 1 */
                                    MRfield0 = MRfield0 - 23;
                                }
                                else
                                {   /* Vref Range 2 */
                                    MRfield0 = MRfield0 | 0x40;
                                }
                                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, MRfield0, 0x7F));
                                break;
                            case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
                                MRfield0 = (((int) (((*scPtr).resultData[0] >> 16) & 0xFFFF)) - 7) & 0xF;
                                MRfield2 = (((int) (((*scPtr).resultData[1] >> 16) & 0xFFFF)) - 7) & 0xF;
                                /* coverity[copy_paste_error:FALSE] */
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield0));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield0));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield2));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield2));
                                /* coverity[copy_paste_error:FALSE] */
                                
                                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_dq_byte_pairs_swap_info_get(unit, drc_ndx, &swap));
                                
                                if(swap)
                                {
                                    SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, (MRfield0 << 8) | (MRfield2 << 4), 0xFF0));
                                }
                                else
                                {
                                    SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, (MRfield2 << 8) | (MRfield0 << 4), 0xFF0));
                                }
                                break;
                            default:
                                LOG_ERROR(BSL_LS_SOC_DDR,
                                          (BSL_META_U(unit,
                                                      "Unsupported dram type: %02d\n"),
                                           dramType));
                                return SOC_E_FAIL;
                        }
                    }
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                case SHMOO_COMBO16_WORD:
                    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
                    if(engageUIshift)
                    {
                        _combo16_calculate_ui_position((*scPtr).resultData[0] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[0] & 0xFFFF));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
                    }

                    if(!(*scPtr).restore)
                    {
                        switch(dramType)
                        {
                            case SHMOO_COMBO16_DRAM_TYPE_DDR4:
                                MRfield0 = ((int) (((*scPtr).resultData[0] >> 16) & 0xFFFF));
                                
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield0));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield0));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield0));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield0));
                                
                                if(MRfield0 > SHMOO_COMBO16_DDR4_VREF_RANGE_CROSSOVER)
                                {   /* Vref Range 1 */
                                    MRfield0 = MRfield0 - 23;
                                }
                                else
                                {   /* Vref Range 2 */
                                    MRfield0 = MRfield0 | 0x40;
                                }
                                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, MRfield0, 0x7F));
                                break;
                            case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
                                MRfield0 = (((int) (((*scPtr).resultData[0] >> 16) & 0xFFFF)) - 7) & 0xF;
                                
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield0));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield0));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield0));
                                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_MACRO_RESERVED_REGr(unit, drc_ndx, MRfield0));
                                
                                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, (MRfield0 << 8) | (MRfield0 << 4), 0xFF0));
                                break;
                            default:
                                LOG_ERROR(BSL_LS_SOC_DDR,
                                          (BSL_META_U(unit,
                                                      "Unsupported dram type: %02d\n"),
                                           dramType));
                                return SOC_E_FAIL;
                        }
                    }
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_ADDR_CTRL_EXTENDED:
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_BYTE:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_HALFWORD:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
                case SHMOO_COMBO16_WORD:
                    SOC_IF_ERROR_RETURN(READ_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, drc_ndx, &data));
                    if(engageUIshift)
                    {
                        _combo16_calculate_ui_position((*scPtr).resultData[0] & 0xFFFF, scPtr, &up);
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, &data, SOC_PHY_DDRC16_UI_SHIFTf, up.ui));
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, up.position));
                    }
                    else
                    {
                        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).resultData[0] & 0xFFFF));
                    }
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, drc_ndx, data));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_ADDRr(unit, drc_ndx, data));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_CTRLr(unit, drc_ndx, data));
                    SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_CTRLr(unit, drc_ndx, data));
                    
                    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    break;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type and calibration mode combination during set new step: %02d / %02d\n"),
                               (*scPtr).shmooType, calibMode));
                    return SOC_E_FAIL;
            }

            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
                    
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                    
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
            }

            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_dram_init(unit, drc_ndx, 10));
            sal_usleep(SHMOO_COMBO16_DEEP_SLEEP);

            break;
        default:
            LOG_ERROR(BSL_LS_SOC_DDR,
                      (BSL_META_U(unit,
                                  "Unsupported shmoo type: %02d\n"),
                       (*scPtr).shmooType));
            return SOC_E_FAIL;
    }
    return SOC_E_NONE;
}

STATIC int
_combo16_plot(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr, uint32 plotMode)
{
    uint32 x;
    uint32 y;
    uint32 xStart;
    uint32 sizeX;
    uint32 sizeY;
    uint32 yCapMin;
    uint32 yCapMax;
    uint32 i;
    uint32 ui;
    uint32 iter;
    uint32 shiftAmount;
    uint32 dataMask;
    uint32 calibMode;
    uint32 calibPos;
    uint32 calibStart;
    uint32 wckInv;
    uint32 engageUIshift;
    uint32 step1000;
    uint32 size1000UI;
    uint32 calibShiftAmount;
    uint32 maxMidPointX;
    uint32 maxMidPointY;
    char *str0 = NULL;
    char *str1 = NULL;
    char *str2 = NULL;
    int rv = SOC_E_NONE;
    char pass_low[2];
    char fail_high[2];
    char outOfSearch[2];
    
    outOfSearch[0] = ' ';
    outOfSearch[1] = 0;

    sizeX = (*scPtr).sizeX;
    sizeY = (*scPtr).sizeY;
    yCapMin = (*scPtr).yCapMin;
    yCapMax = (*scPtr).yCapMax;
    calibMode = (*scPtr).calibMode;
    calibPos = (*scPtr).calibPos;
    calibStart = (*scPtr).calibStart;
    wckInv = (*scPtr).wckInv;
    engageUIshift = (*scPtr).engageUIshift;
    step1000 = (*scPtr).step1000;
    size1000UI = (*scPtr).size1000UI;

    switch(calibPos)
    {
        case SHMOO_COMBO16_CALIB_ANY_EDGE:
        case SHMOO_COMBO16_CALIB_RISING_EDGE:
        case SHMOO_COMBO16_CALIB_FALLING_EDGE:
            pass_low[0] = '_';
            pass_low[1] = 0;
            fail_high[0] = '|';
            fail_high[1] = 0;
            break;
        case SHMOO_COMBO16_CALIB_CENTER_PASS:
        case SHMOO_COMBO16_CALIB_PASS_START:
        case SHMOO_COMBO16_CALIB_FAIL_START:
            pass_low[0] = '+';
            pass_low[1] = 0;
            fail_high[0] = '-';
            fail_high[1] = 0;
            break;
        default:
            LOG_ERROR(BSL_LS_SOC_DDR,
                      (BSL_META_U(unit,
                                  "Unsupported calibration position: %02d\n"),
                       calibPos));
            return SOC_E_FAIL;
    }

    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            "\n\n")));

    switch(plotMode)
    {
        case SHMOO_COMBO16_BIT:
            if((*scPtr).shmooType == SHMOO_COMBO16_DBI_WR_DESKEW)
            {
                iter = 4;
            }
            else if((*scPtr).shmooType == SHMOO_COMBO16_DBI_EDC_RD_DESKEW)
            {
                iter = 8;
            }
            else
            {
                iter = 32;
            }
            shiftAmount = 0;
            dataMask = 0x1;
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    calibShiftAmount = 0;
                    break;
                case SHMOO_COMBO16_BYTE:
                    calibShiftAmount = 3;
                    break;
                case SHMOO_COMBO16_HALFWORD:
                    calibShiftAmount = 4;
                    break;
                case SHMOO_COMBO16_WORD:
                    calibShiftAmount = 5;
                    break;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported calibration mode during plot: %02d\n"),
                               calibMode));
                    return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_BYTE:
            iter = 4;
            shiftAmount = 3;
            dataMask = 0xFF;
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "WARNING: Plot mode coerced from byte mode to bit mode\n")));
                    iter = 32;
                    shiftAmount = 0;
                    dataMask = 0x1;
                    calibShiftAmount = 0;
                    break;
                case SHMOO_COMBO16_BYTE:
                    calibShiftAmount = 0;
                    break;
                case SHMOO_COMBO16_HALFWORD:
                    calibShiftAmount = 1;
                    break;
                case SHMOO_COMBO16_WORD:
                    calibShiftAmount = 2;
                    break;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported calibration mode during plot: %02d\n"),
                               calibMode));
                    return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_HALFWORD:
            iter = 2;
            shiftAmount = 4;
            dataMask = 0xFFFF;
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "WARNING: Plot mode coerced from halfword mode to bit mode\n")));
                    iter = 32;
                    shiftAmount = 0;
                    dataMask = 0x1;
                    calibShiftAmount = 0;
                    break;
                case SHMOO_COMBO16_BYTE:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "WARNING: Plot mode coerced from halfword mode to byte mode\n")));
                    iter = 4;
                    shiftAmount = 3;
                    dataMask = 0xFF;
                    calibShiftAmount = 0;
                    break;
                case SHMOO_COMBO16_HALFWORD:
                    calibShiftAmount = 0;
                    break;
                case SHMOO_COMBO16_WORD:
                    calibShiftAmount = 1;
                    break;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported calibration mode during plot: %02d\n"),
                               calibMode));
                    return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_WORD:
            iter = 1;
            shiftAmount = 5;
            dataMask = 0xFFFFFFFF;
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "WARNING: Plot mode coerced from word mode to bit mode\n")));
                    iter = 32;
                    shiftAmount = 0;
                    dataMask = 0x1;
                    calibShiftAmount = 0;
                    break;
                case SHMOO_COMBO16_BYTE:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "WARNING: Plot mode coerced from word mode to byte mode\n")));
                    iter = 4;
                    shiftAmount = 3;
                    dataMask = 0xFF;
                    calibShiftAmount = 0;
                    break;
                case SHMOO_COMBO16_HALFWORD:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "WARNING: Plot mode coerced from word mode to halfword mode\n")));
                    iter = 2;
                    shiftAmount = 4;
                    dataMask = 0xFFFF;
                    calibShiftAmount = 0;
                    break;
                case SHMOO_COMBO16_WORD:
                    calibShiftAmount = 0;
                    break;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported calibration mode during plot: %02d\n"),
                               calibMode));
                    return SOC_E_FAIL;
            }
            break;
        default:
            LOG_ERROR(BSL_LS_SOC_DDR,
                      (BSL_META_U(unit,
                                  "Unsupported plot mode: %02d\n"),
                       plotMode));
            return SOC_E_FAIL;
    }
/*    
    if(engageUIshift)
    { */
    str0 = sal_alloc(sizeof(char) * SHMOO_COMBO16_STRING_LENGTH, "shmoo combo16 str0");
    if (NULL == str0) {
        return SOC_E_MEMORY;
    }
    str1 = sal_alloc(sizeof(char) * SHMOO_COMBO16_STRING_LENGTH, "shmoo combo16 str1");
    if (NULL == str1) {
        sal_free(str0);
        return SOC_E_MEMORY;
    }
    str2 = sal_alloc(sizeof(char) * SHMOO_COMBO16_STRING_LENGTH, "shmoo combo16 str2");
    if (NULL == str2) {
        sal_free(str0);
        sal_free(str1);
        return SOC_E_MEMORY;
    }

    sal_memset(str0, 0, sizeof(char) * SHMOO_COMBO16_STRING_LENGTH);
    sal_memset(str1, 0, sizeof(char) * SHMOO_COMBO16_STRING_LENGTH);
    sal_memset(str2, 0, sizeof(char) * SHMOO_COMBO16_STRING_LENGTH);

    {
        ui = 0;
        
        for(x = 0; x < sizeX; x++)
        {
            if((ui < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT) && (x > (*scPtr).endUI[ui]))
            {
                str0[x] = ' ';
                str1[x] = ' ';
                str2[x] = ' ';
                ui++;
            }
            else
            {
                str0[x] = '0' + (x / 100);
                str1[x] = '0' + ((x % 100) / 10);
                str2[x] = '0' + (x % 10);
            }
        }
    }
/*    }
    else
    {
        for(x = 0; x < sizeX; x++)
        {
            str0[x] = '0' + (x / 100);
            str1[x] = '0' + ((x % 100) / 10);
            str2[x] = '0' + (x % 10);
        }
    } */
    
    str0[x] = 0;
    str1[x] = 0;
    str2[x] = 0;

    for(i = 0; i < iter; i++)
    {
        xStart = 0;
        maxMidPointX = (*scPtr).resultData[i >> calibShiftAmount] & 0xFFFF;
        maxMidPointY = ((*scPtr).resultData[i >> calibShiftAmount] >> 16) & 0xFFFF;
        
        if((sizeY > 1) || (i == 0))
        {
            LOG_VERBOSE(BSL_LS_SOC_DDR,
                        (BSL_META_U(unit,
                                    "***** Interface.......: %3d\n"),
                         drc_ndx));
            LOG_VERBOSE(BSL_LS_SOC_DDR,
                        (BSL_META_U(unit,
                                    " **** VDL step size...: %3d.%03d ps\n"),
                         (step1000 / 1000), (step1000 % 1000)));
            LOG_VERBOSE(BSL_LS_SOC_DDR,
                        (BSL_META_U(unit,
                                    " **** UI size.........: %3d.%03d steps\n"),
                         (size1000UI / 1000), (size1000UI % 1000)));
            
            switch((*scPtr).shmooType)
            {
                case SHMOO_COMBO16_ADDR_CTRL_SHORT:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            " **** Shmoo type......: ADDR_CTRL_SHORT\n")));
                    break;
                case SHMOO_COMBO16_DQS2CK:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            " **** Shmoo type......: DQS2CK\n")));
                    break;
                case SHMOO_COMBO16_WCK2CK:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            " **** Shmoo type......: WCK2CK\n")));
                    if((wckInv >> 2) & 0x1)
                    {
                        LOG_VERBOSE(BSL_LS_SOC_DDR,
                                    (BSL_META_U(unit,
                                                "  *** WCK01 invert....: On (Post-inversion)\n")));
                    }
                    else
                    {
                        LOG_VERBOSE(BSL_LS_SOC_DDR,
                                    (BSL_META_U(unit,
                                                "  *** WCK01 invert....: Off\n")));
                    }
                    if((wckInv >> 3) & 0x1)
                    {
                        LOG_VERBOSE(BSL_LS_SOC_DDR,
                                    (BSL_META_U(unit,
                                                "  *** WCK23 invert....: On (Post-inversion)\n")));
                    }
                    else
                    {
                        LOG_VERBOSE(BSL_LS_SOC_DDR,
                                    (BSL_META_U(unit,
                                                "  *** WCK23 invert....: Off\n")));
                    }
                    break;
                case SHMOO_COMBO16_RD_DESKEW:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            " **** Shmoo type......: RD_DESKEW\n")));
                    break;
                case SHMOO_COMBO16_DBI_EDC_RD_DESKEW:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            " **** Shmoo type......: DBI_EDC_RD_DESKEW\n")));
                    break;
                case SHMOO_COMBO16_RD_SHORT:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            " **** Shmoo type......: RD_SHORT\n")));
                    break;
                case SHMOO_COMBO16_WR_DESKEW:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            " **** Shmoo type......: WR_DESKEW\n")));
                    break;
                case SHMOO_COMBO16_DBI_WR_DESKEW:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            " **** Shmoo type......: DBI_WR_DESKEW\n")));
                    break;
                case SHMOO_COMBO16_WR_SHORT:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            " **** Shmoo type......: WR_SHORT\n")));
                    break;
                case SHMOO_COMBO16_RD_EXTENDED:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            " **** Shmoo type......: RD_EXTENDED\n")));
                    break;
                case SHMOO_COMBO16_WR_EXTENDED:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            " **** Shmoo type......: WR_EXTENDED\n")));
                    break;
                case SHMOO_COMBO16_ADDR_CTRL_EXTENDED:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            " **** Shmoo type......: ADDR_CTRL_EXTENDED\n")));
                    break;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported shmoo type: %02d\n"),
                               (*scPtr).shmooType));
                    rv = SOC_E_FAIL;
                    goto cleanup_combo16;
            }
            
            if(engageUIshift)
            {
                LOG_VERBOSE(BSL_LS_SOC_DDR,
                            (BSL_META_U(unit,
                                        "  *** UI shift........: On\n")));
            }
            else
            {
                LOG_VERBOSE(BSL_LS_SOC_DDR,
                            (BSL_META_U(unit,
                                        "  *** UI shift........: Off or N/A\n")));
            }
        }
        
        if(sizeY > 1)
        {
            switch(calibMode)
            {
                case SHMOO_COMBO16_BIT:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "  *** Calib mode......: 2D Bit-wise\n")));
                    break;
                case SHMOO_COMBO16_BYTE:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "  *** Calib mode......: 2D Byte-wise\n")));
                    break;
                case SHMOO_COMBO16_HALFWORD:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "  *** Calib mode......: 2D Halfword-wise\n")));
                    break;
                case SHMOO_COMBO16_WORD:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "  *** Calib mode......: 2D Word-wise\n")));
                    break;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported calibration mode during plot: %02d\n"),
                               calibMode));
                    rv = SOC_E_FAIL;
                    goto cleanup_combo16;
            }
            
            switch(plotMode)
            {
                case SHMOO_COMBO16_BIT:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "  *** Plot mode.......: 2D Bit-wise\n")));
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "   ** Bit.............: %03d\n"),
                                 i));
                    break;
                case SHMOO_COMBO16_BYTE:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "  *** Plot mode.......: 2D Byte-wise\n")));
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "   ** Byte............: %03d\n"),
                                 i));
                    break;
                case SHMOO_COMBO16_HALFWORD:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "  *** Plot mode.......: 2D Halfword-wise\n")));
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "   ** Halfword........: %03d\n"),
                                 i));
                    break;
                case SHMOO_COMBO16_WORD:
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "  *** Plot mode.......: 2D Word-wise\n")));
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "   ** Word............: %03d\n"),
                                 i));
                    break;
                default:
                    LOG_ERROR(BSL_LS_SOC_DDR,
                              (BSL_META_U(unit,
                                          "Unsupported plot mode: %02d\n"),
                               plotMode));
                    rv = SOC_E_FAIL;
                    goto cleanup_combo16;
            }

            LOG_VERBOSE(BSL_LS_SOC_DDR,
                        (BSL_META_U(unit,
                                    "    * Center X........: %03d\n"),
                         maxMidPointX));
            LOG_VERBOSE(BSL_LS_SOC_DDR,
                        (BSL_META_U(unit,
                                    "    * Center Y........: %03d\n"),
                         maxMidPointY));
            LOG_VERBOSE(BSL_LS_SOC_DDR,
                        (BSL_META_U(unit,
                                    "      %s\n"),
                         str0));
            LOG_VERBOSE(BSL_LS_SOC_DDR,
                        (BSL_META_U(unit,
                                    "      %s\n"),
                         str1));
            LOG_VERBOSE(BSL_LS_SOC_DDR,
                        (BSL_META_U(unit,
                                    "      %s\n"),
                         str2));

            for(y = yCapMin; y < yCapMax; y++)
            {
                LOG_VERBOSE(BSL_LS_SOC_DDR,
                            (BSL_META_U(unit,
                                        "  %03d "),
                             y));
                
                for(x = 0; x < calibStart; x++)
                {
                    LOG_VERBOSE(BSL_LS_SOC_DDR,
                                (BSL_META_U(unit,
                                            "%s"),
                                 outOfSearch));
                }
                
                for(x = calibStart; x < sizeX; x++)
                {
                    if(((*scPtr).result2D[xStart + x] >> (i << shiftAmount)) & dataMask)
                    {   /* FAIL - RISING EDGE */
                        if(x != maxMidPointX)
                        {   /* REGULAR FAIL */
                            LOG_VERBOSE(BSL_LS_SOC_DDR,
                                        (BSL_META_U(unit,
                                                    "%s"),
                                         fail_high));
                        }
                        else
                        {   /* FAIL - RISING EDGE */
                            if((calibPos == SHMOO_COMBO16_CALIB_ANY_EDGE) || (calibPos == SHMOO_COMBO16_CALIB_RISING_EDGE) || (calibPos == SHMOO_COMBO16_CALIB_FAIL_START))
                            {   /* RISING EDGE */
                                LOG_VERBOSE(BSL_LS_SOC_DDR,
                                            (BSL_META_U(unit,
                                                        "X")));
                            }
                            else
                            {   /* FAIL */
                                LOG_VERBOSE(BSL_LS_SOC_DDR,
                                            (BSL_META_U(unit,
                                                        "%s"),
                                                        fail_high));
                            }
                        }
                    }
                    else
                    {   /* PASS - MIDPOINT - FALLING EDGE */
                        if(x != maxMidPointX)
                        {   /* REGULAR PASS */
                            LOG_VERBOSE(BSL_LS_SOC_DDR,
                                        (BSL_META_U(unit,
                                                    "%s"),
                                         pass_low));
                        }
                        else
                        {   /* POTENTIAL MIDPOINT - FALLING EDGE */
                            if(y == maxMidPointY)
                            {   /* MID POINT - FALLING EDGE */
                                LOG_VERBOSE(BSL_LS_SOC_DDR,
                                            (BSL_META_U(unit,
                                                        "X")));
                            }
                            else
                            {   /* PASS */
                                LOG_VERBOSE(BSL_LS_SOC_DDR,
                                            (BSL_META_U(unit,
                                                        "%s"),
                                             pass_low));
                            }
                        }
                    }
                }
                LOG_VERBOSE(BSL_LS_SOC_DDR,
                            (BSL_META_U(unit,
                                        "\n")));
                xStart += sizeX;
            }
            LOG_VERBOSE(BSL_LS_SOC_DDR,
                        (BSL_META_U(unit,
                                    "\n")));
        }
        else
        {
            if(i == 0)
            {
                switch(calibMode)
                {
                    case SHMOO_COMBO16_BIT:
                        LOG_VERBOSE(BSL_LS_SOC_DDR,
                                    (BSL_META_U(unit,
                                                "  *** Calib mode......: 1D Bit-wise\n")));
                        break;
                    case SHMOO_COMBO16_BYTE:
                        LOG_VERBOSE(BSL_LS_SOC_DDR,
                                    (BSL_META_U(unit,
                                                "  *** Calib mode......: 1D Byte-wise\n")));
                        break;
                    case SHMOO_COMBO16_HALFWORD:
                        LOG_VERBOSE(BSL_LS_SOC_DDR,
                                    (BSL_META_U(unit,
                                                "  *** Calib mode......: 1D Halfword-wise\n")));
                        break;
                    case SHMOO_COMBO16_WORD:
                        LOG_VERBOSE(BSL_LS_SOC_DDR,
                                    (BSL_META_U(unit,
                                                "  *** Calib mode......: 1D Word-wise\n")));
                        break;
                    default:
                        LOG_ERROR(BSL_LS_SOC_DDR,
                                  (BSL_META_U(unit,
                                              "Unsupported calibration mode during plot: %02d\n"),
                                   calibMode));
                        rv = SOC_E_FAIL;
                        goto cleanup_combo16;
                }
                
                switch(plotMode)
                {
                    case SHMOO_COMBO16_BIT:
                        LOG_VERBOSE(BSL_LS_SOC_DDR,
                                    (BSL_META_U(unit,
                                                "  *** Plot mode.......: 1D Bit-wise\n")));
                        break;
                    case SHMOO_COMBO16_BYTE:
                        LOG_VERBOSE(BSL_LS_SOC_DDR,
                                    (BSL_META_U(unit,
                                                "  *** Plot mode.......: 1D Byte-wise\n")));
                        break;
                    case SHMOO_COMBO16_HALFWORD:
                        LOG_VERBOSE(BSL_LS_SOC_DDR,
                                    (BSL_META_U(unit,
                                                "  *** Plot mode.......: 1D Halfword-wise\n")));
                        break;
                    case SHMOO_COMBO16_WORD:
                        LOG_VERBOSE(BSL_LS_SOC_DDR,
                                    (BSL_META_U(unit,
                                                "  *** Plot mode.......: 1D Word-wise\n")));
                        break;
                    default:
                        LOG_ERROR(BSL_LS_SOC_DDR,
                                  (BSL_META_U(unit,
                                              "Unsupported plot mode: %02d\n"),
                                   plotMode));
                        rv = SOC_E_FAIL;
                        goto cleanup_combo16;
                }
                LOG_VERBOSE(BSL_LS_SOC_DDR,
                            (BSL_META_U(unit,
                                        "      %s\n"),
                             str0));
                LOG_VERBOSE(BSL_LS_SOC_DDR,
                            (BSL_META_U(unit,
                                        "      %s\n"),
                             str1));
                LOG_VERBOSE(BSL_LS_SOC_DDR,
                            (BSL_META_U(unit,
                                        "      %s\n"),
                             str2));
            }
            
            LOG_VERBOSE(BSL_LS_SOC_DDR,
                        (BSL_META_U(unit,
                                    "  %03d "),
                         i));
            
            for(x = 0; x < calibStart; x++)
            {
                LOG_VERBOSE(BSL_LS_SOC_DDR,
                            (BSL_META_U(unit,
                                        "%s"),
                             outOfSearch));
            }
            
            for(x = calibStart; x < sizeX; x++)
            {
                if(((*scPtr).result2D[x] >> (i << shiftAmount)) & dataMask)
                {   /* FAIL - RISING EDGE */
                    if(x != maxMidPointX)
                    {   /* REGULAR FAIL */
                        LOG_VERBOSE(BSL_LS_SOC_DDR,
                                    (BSL_META_U(unit,
                                                "%s"),
                                                fail_high));
                    }
                    else
                    {   /* FAIL - RISING EDGE */
                        if((calibPos == SHMOO_COMBO16_CALIB_ANY_EDGE) || (calibPos == SHMOO_COMBO16_CALIB_RISING_EDGE) || (calibPos == SHMOO_COMBO16_CALIB_FAIL_START))
                        {   /* RISING EDGE */
                            LOG_VERBOSE(BSL_LS_SOC_DDR,
                                        (BSL_META_U(unit,
                                                    "X")));
                        }
                        else
                        {   /* FAIL */
                            LOG_VERBOSE(BSL_LS_SOC_DDR,
                                        (BSL_META_U(unit,
                                                    "%s"),
                                         fail_high));
                        }
                    }
                }
                else
                {   /* PASS - MIDPOINT - FALLING EDGE */
                    if(x != maxMidPointX)
                    {   /* REGULAR PASS */
                        LOG_VERBOSE(BSL_LS_SOC_DDR,
                                    (BSL_META_U(unit,
                                                "%s"),
                                     pass_low));
                    }
                    else
                    {   /* MID POINT - FALLING EDGE */
                        LOG_VERBOSE(BSL_LS_SOC_DDR,
                                    (BSL_META_U(unit,
                                                "X")));
                    }
                }
            }
            LOG_VERBOSE(BSL_LS_SOC_DDR,
                        (BSL_META_U(unit,
                                    "\n")));
        }
    }
    LOG_VERBOSE(BSL_LS_SOC_DDR,
                (BSL_META_U(unit,
                            "\n")));

cleanup_combo16:
    if (str0 != NULL) {
        sal_free(str0);
    }
    if (str1 != NULL) {
        sal_free(str1);
    }
    if (str2 != NULL) {
        sal_free(str2);
    }
    return rv;
}

STATIC int
_shmoo_combo16_plot(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr)
{
    switch((*scPtr).shmooType)
    {
        case SHMOO_COMBO16_ADDR_CTRL_SHORT:
            return _combo16_plot(unit, drc_ndx, scPtr, SHMOO_COMBO16_WORD);
        case SHMOO_COMBO16_DQS2CK:
            return _combo16_plot(unit, drc_ndx, scPtr, SHMOO_COMBO16_BYTE);
        case SHMOO_COMBO16_WCK2CK:
            return _combo16_plot(unit, drc_ndx, scPtr, SHMOO_COMBO16_HALFWORD);
        case SHMOO_COMBO16_RD_EN_FISH:
            break;
        case SHMOO_COMBO16_RD_START_FISH:
            break;
        case SHMOO_COMBO16_EDC_START_FISH:
            break;
        case SHMOO_COMBO16_RD_DESKEW:
            return _combo16_plot(unit, drc_ndx, scPtr, SHMOO_COMBO16_BIT);
        case SHMOO_COMBO16_DBI_EDC_RD_DESKEW:
            return _combo16_plot(unit, drc_ndx, scPtr, SHMOO_COMBO16_BIT);
        case SHMOO_COMBO16_RD_SHORT:
            return _combo16_plot(unit, drc_ndx, scPtr, SHMOO_COMBO16_BIT);
        case SHMOO_COMBO16_WR_DESKEW:
            return _combo16_plot(unit, drc_ndx, scPtr, SHMOO_COMBO16_BIT);
        case SHMOO_COMBO16_DBI_WR_DESKEW:
            return _combo16_plot(unit, drc_ndx, scPtr, SHMOO_COMBO16_BIT);
        case SHMOO_COMBO16_WR_SHORT:
            return _combo16_plot(unit, drc_ndx, scPtr, SHMOO_COMBO16_BIT);
        case SHMOO_COMBO16_RD_EXTENDED:
            if((*scPtr).restore)
            {
                return _combo16_plot(unit, drc_ndx, scPtr, SHMOO_COMBO16_BIT);
            }
            else
            {
                return _combo16_plot(unit, drc_ndx, scPtr, SHMOO_COMBO16_BYTE);
            }
        case SHMOO_COMBO16_WR_EXTENDED:
            if((*scPtr).restore)
            {
                return _combo16_plot(unit, drc_ndx, scPtr, SHMOO_COMBO16_BIT);
            }
            else
            {
                return _combo16_plot(unit, drc_ndx, scPtr, SHMOO_COMBO16_BYTE);
            }
        case SHMOO_COMBO16_ADDR_CTRL_EXTENDED:
            return _combo16_plot(unit, drc_ndx, scPtr, SHMOO_COMBO16_WORD);
        default:
            LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported shmoo type: %02d\n"), (*scPtr).shmooType));
            return SOC_E_FAIL;
    }
    return SOC_E_NONE;
}

int
soc_combo16_calculate_step_size(int unit, int drc_ndx, combo16_step_size_t *ssPtr)
{
    uint32 data;
    uint32 obs_interval, ro_vdl_step1, ro_vdl_step2, ucount1, ucount2;
    uint32 reset_ctrl_data;
    uint32 fld_dcount;
    uint32 fld_ro_overflow;
    soc_timeout_t to;
    sal_usecs_t to_val;
    combo16_shmoo_dram_info_t* shmoo_dram_info;

    SOC_IF_ERROR_RETURN( _shmoo_combo16_drc_dram_info_access(unit, &shmoo_dram_info));
    
    if(shmoo_dram_info->sim_system_mode)
    {
        (*ssPtr).step1000 = 1953;
        (*ssPtr).size1000UI = 128000;
        
        return SOC_E_NONE;
    }
    
    data = 0;
    obs_interval = 2000;
    ro_vdl_step1 = 64;
    ro_vdl_step2 = 128;
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_FREQ_CNTR_CONFIGr, &data, SOC_PHY_DDRC16_OBS_INTERVALf, obs_interval));
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_FREQ_CNTR_CONFIGr, &data, SOC_PHY_DDRC16_SEL_FC_REFCLKf, 0)); 
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_FREQ_CNTR_CONFIGr(unit, drc_ndx, data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP); 
    
    SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &reset_ctrl_data));
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_FREQ_CNTR_FC_RESET_Nf, 0));
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_FREQ_CNTR_RO_RESET_Nf, 0));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, reset_ctrl_data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP); 
    
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_FREQ_CNTR_CONFIGr, &data, SOC_PHY_DDRC16_START_OBSf, 0)); 
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_FREQ_CNTR_CONFIGr, &data, SOC_PHY_DDRC16_EN_NLDL_CLKOUT_BARf, 0));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_FREQ_CNTR_CONFIGr(unit, drc_ndx, data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
    
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_FREQ_CNTR_CONFIGr, &data, SOC_PHY_DDRC16_RO_VDL_STEPf, ro_vdl_step1));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_FREQ_CNTR_CONFIGr(unit, drc_ndx, data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP); 
    
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_FREQ_CNTR_CONFIGr, &data, SOC_PHY_DDRC16_EN_NLDL_CLKOUT_BARf, 1));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_FREQ_CNTR_CONFIGr(unit, drc_ndx, data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
    
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_FREQ_CNTR_FC_RESET_Nf, 1));
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_FREQ_CNTR_RO_RESET_Nf, 1));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, reset_ctrl_data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
    
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_FREQ_CNTR_CONFIGr, &data, SOC_PHY_DDRC16_START_OBSf, 1));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_FREQ_CNTR_CONFIGr(unit, drc_ndx, data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
    
    if (SAL_BOOT_QUICKTURN)
    {
        to_val = 10000000;  /* 10 Sec */
    }
    else
    {
        to_val = 50000;     /* 50 mS */
    }
    
    soc_timeout_init(&to, to_val, 0);
    do
    {
        SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_STATUS_FREQ_CNTR_DCOUNTr(unit, drc_ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_STATUS_FREQ_CNTR_DCOUNTr, data, SOC_PHY_DDRC16_DCOUNTf, &fld_dcount));
        if(!fld_dcount)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_STATUS_FREQ_CNTR_UCOUNTr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_STATUS_FREQ_CNTR_UCOUNTr, data, SOC_PHY_DDRC16_RO_OVERFLOWf, &fld_ro_overflow));
            if( fld_ro_overflow & 0x4)
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "VDL step size computation rollover during first pass\n")));
                return SOC_E_FAIL;
            }
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_STATUS_FREQ_CNTR_UCOUNTr, data, SOC_PHY_DDRC16_RO_UCOUNTf, &ucount1));
            break;
        }
        if (soc_timeout_check(&to))
        {
            LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Timed out waiting for first pass of VDL step size computation\n")));
            return SOC_E_TIMEOUT;
        }
    }
    while(TRUE);
    
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_FREQ_CNTR_FC_RESET_Nf, 0));
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_FREQ_CNTR_RO_RESET_Nf, 0));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, reset_ctrl_data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
    
    SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_FREQ_CNTR_CONFIGr(unit, drc_ndx, &data));
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_FREQ_CNTR_CONFIGr, &data, SOC_PHY_DDRC16_START_OBSf, 0)); 
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_FREQ_CNTR_CONFIGr, &data, SOC_PHY_DDRC16_EN_NLDL_CLKOUT_BARf, 0));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_FREQ_CNTR_CONFIGr(unit, drc_ndx, data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
    
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_FREQ_CNTR_CONFIGr, &data, SOC_PHY_DDRC16_RO_VDL_STEPf, ro_vdl_step2));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_FREQ_CNTR_CONFIGr(unit, drc_ndx, data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP); 
    
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_FREQ_CNTR_CONFIGr, &data, SOC_PHY_DDRC16_EN_NLDL_CLKOUT_BARf, 1));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_FREQ_CNTR_CONFIGr(unit, drc_ndx, data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
    
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_FREQ_CNTR_FC_RESET_Nf, 1));
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_FREQ_CNTR_RO_RESET_Nf, 1));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, reset_ctrl_data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
    
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_FREQ_CNTR_CONFIGr, &data, SOC_PHY_DDRC16_START_OBSf, 1));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_FREQ_CNTR_CONFIGr(unit, drc_ndx, data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
    soc_timeout_init(&to, to_val, 0);
    do
    {
        SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_STATUS_FREQ_CNTR_DCOUNTr(unit, drc_ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_STATUS_FREQ_CNTR_DCOUNTr, data, SOC_PHY_DDRC16_DCOUNTf, &fld_dcount));
        if(!fld_dcount)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_STATUS_FREQ_CNTR_UCOUNTr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_STATUS_FREQ_CNTR_UCOUNTr, data, SOC_PHY_DDRC16_RO_OVERFLOWf, &fld_ro_overflow));
            if( fld_ro_overflow & 0x4)
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "VDL step size computation rollover during second pass\n")));
                return SOC_E_FAIL;
            }
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_STATUS_FREQ_CNTR_UCOUNTr, data, SOC_PHY_DDRC16_RO_UCOUNTf, &ucount2));
            break;
        }
        if (soc_timeout_check(&to))
        {
            LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Timed out waiting for second pass of VDL step size computation\n")));
            return SOC_E_TIMEOUT;
        }
    }
    while(TRUE);
    
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_FREQ_CNTR_FC_RESET_Nf, 0));
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_FREQ_CNTR_RO_RESET_Nf, 0));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, reset_ctrl_data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
    
    SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_FREQ_CNTR_CONFIGr(unit, drc_ndx, &data));
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_FREQ_CNTR_CONFIGr, &data, SOC_PHY_DDRC16_START_OBSf, 0));
    SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_FREQ_CNTR_CONFIGr, &data, SOC_PHY_DDRC16_EN_NLDL_CLKOUT_BARf, 0));
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_FREQ_CNTR_CONFIGr(unit, drc_ndx, data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
    
    (*ssPtr).step1000 = ((((((1000000000 / (ro_vdl_step2 - ro_vdl_step1)) << 3) / shmoo_dram_info->data_rate_mbps) * obs_interval) / ucount2) * (ucount1 - ucount2)) / ucount1;
    (*ssPtr).size1000UI = ((1000000000 / shmoo_dram_info->data_rate_mbps) * 1000) / ((*ssPtr).step1000);
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_entry(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr, uint32 mode)
{
    /* Mode 0: Sequential entry
     * Mode 1: Single entry
     */
    
    uint32 i;
    uint32 data;
    combo16_step_size_t ss;
    combo16_shmoo_dram_info_t* shmoo_dram_info;

    SOC_IF_ERROR_RETURN( _shmoo_combo16_drc_dram_info_access(unit, &shmoo_dram_info));
    
    (*scPtr).calibStart = 0;
    
    SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_refresh(unit, drc_ndx , 0, 0, &(*scPtr).trefi));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
    
    switch((*scPtr).shmooType)
    {
        case SHMOO_COMBO16_ADDR_CTRL_SHORT:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, drc_ndx, &ss));
                (*scPtr).step1000 = ss.step1000;
                (*scPtr).size1000UI = ss.size1000UI;
                
                for(i = 0; i < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT; i++)
                {
                    (*scPtr).endUI[i] = ((i + 1) * (ss.size1000UI)) / 1000;
                }
                
                data = 0;                   /* ADDR/CTRL should be in a working position */                         /* No adjustment */
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, &data, SOC_PHY_DDRC16_UI_SHIFTf, SHMOO_COMBO16_GDDR5_INIT_ADDR_CTRL_UI_SHIFT));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_ADDRr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_CTRLr(unit, drc_ndx, data));
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr, &data, SOC_PHY_DDRC16_DATAf, 0x92));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, 0x000, 0xFF0));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_dram_init(unit, drc_ndx, 11));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_adt(unit, drc_ndx, 1));
                sal_usleep(SHMOO_COMBO16_DEEP_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_DQS2CK:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, drc_ndx, &ss));
                (*scPtr).step1000 = ss.step1000;
                (*scPtr).size1000UI = ss.size1000UI;
                
                for(i = 0; i < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT; i++)
                {
                    (*scPtr).endUI[i] = ((i + 1) * (ss.size1000UI)) / 1000;
                }
                
                data = 0;                   /* ADDR/CTRL should be in a working position */                         /* 40% adjustment */
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, &data, SOC_PHY_DDRC16_UI_SHIFTf, SHMOO_COMBO16_DDR4_INIT_ADDR_CTRL_UI_SHIFT));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, ss.size1000UI / 2500));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_ADDRr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_CTRLr(unit, drc_ndx, data));
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr, &data, SOC_PHY_DDRC16_DATAf, 0x92));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_VREF_DAC_CONFIGr(unit, drc_ndx, data));
                
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, 0x10, 0x7F));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);

                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_dram_init(unit, drc_ndx, 10));
                sal_usleep(SHMOO_COMBO16_DEEP_SLEEP);
            
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_write_leveling(unit, drc_ndx, shmoo_dram_info->command_parity_latency, 0, 1));
        
                data = 0;
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_LEVELING_CONFIGr, &data, SOC_PHY_DDRC16_WRITE_LEVELING_MODEf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_WCK2CK:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, drc_ndx, &ss));
                (*scPtr).step1000 = ss.step1000;
                (*scPtr).size1000UI = ss.size1000UI;
                
                for(i = 0; i < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT; i++)
                {
                    (*scPtr).endUI[i] = ((i + 1) * (ss.size1000UI)) / 1000;
                }

                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 3, 0x000, 0x00C));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_force_dqs(unit, drc_ndx , 0, 1));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                (*scPtr).sizeX = (*scPtr).endUI[1] + 35;
                SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, (*scPtr).sizeX - 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
                
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_wck2ck_training(unit, drc_ndx, 1));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);

                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_force_dqs(unit, drc_ndx , 0, 0));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);

                data = 0;
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_LEVELING_CONFIGr, &data, SOC_PHY_DDRC16_WRITE_LEVELING_MODEf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_EN_FISH:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                data = 0;
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_MINUS_3f, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_MINUS_2f, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_MINUS_1f, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_0f, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_PLUS_1f, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_PLUS_2f, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_DDR4_REN_STRETCH_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_DDR4_REN_STRETCH_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_DDR4_REN_STRETCH_CONFIGr(unit, drc_ndx, data));
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
                
                data = 0;
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_FIFO_CONFIGr, &data, SOC_PHY_DDRC16_RD2_2G_DELAYf, SHMOO_COMBO16_DDR4_INIT_REN_RD2_2G_DELAY));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, data));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                    
                SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, drc_ndx, &ss));
                (*scPtr).step1000 = ss.step1000;
                (*scPtr).size1000UI = ss.size1000UI;
                
                for(i = 0; i < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT; i++)
                {
                    (*scPtr).endUI[i] = ((i + 1) * (ss.size1000UI)) / 1000;
                }
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_START_FISH:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 1));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, drc_ndx, &ss));
                (*scPtr).step1000 = ss.step1000;
                (*scPtr).size1000UI = ss.size1000UI;
                
                for(i = 0; i < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT; i++)
                {
                    (*scPtr).endUI[i] = ((i + 1) * (ss.size1000UI)) / 1000;
                }
                
                data = 0;
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_RDATA_WR2RD_DELAYf, 4));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr(unit, drc_ndx, data));
                
                data = 0;
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_FSM_CTRLr, &data, SOC_PHY_DDRC16_SYSTEM_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_FSM_CTRLr, &data, SOC_PHY_DDRC16_DATA_VALID_GEN_ENABLEf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_GDDR5_READ_FSM_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_GDDR5_READ_FSM_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_GDDR5_READ_FSM_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_GDDR5_READ_FSM_CTRLr(unit, drc_ndx, data));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_FSM_CTRLr, &data, SOC_PHY_DDRC16_DATA_VALID_GEN_ENABLEf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_GDDR5_READ_FSM_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_GDDR5_READ_FSM_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_GDDR5_READ_FSM_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_GDDR5_READ_FSM_CTRLr(unit, drc_ndx, data));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_FSM_CTRLr, &data, SOC_PHY_DDRC16_SYSTEM_ENABLEf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_GDDR5_READ_FSM_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_GDDR5_READ_FSM_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_GDDR5_READ_FSM_CTRLr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_GDDR5_READ_FSM_CTRLr(unit, drc_ndx, data));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_EDC_START_FISH:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_wr_crc(unit, drc_ndx, 1));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_rd_crc(unit, drc_ndx, 1));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 1));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, drc_ndx, &ss));
                (*scPtr).step1000 = ss.step1000;
                (*scPtr).size1000UI = ss.size1000UI;
                
                for(i = 0; i < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT; i++)
                {
                    (*scPtr).endUI[i] = ((i + 1) * (ss.size1000UI)) / 1000;
                }
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                data = SHMOO_COMBO16_DDR4_INIT_READ_MIN_VDL_POS;
            }
            else
            {
                data = 0;
            }
            
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT0r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT1r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT2r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT3r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT4r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT5r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT6r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT7r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT0r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT1r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT2r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT3r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT4r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT5r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT6r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT7r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT0r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT1r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT2r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT3r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT4r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT5r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT6r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT7r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT0r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT1r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT2r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT3r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT4r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT5r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT6r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT7r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
            
            SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESERVED_REGr(unit, drc_ndx, &data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESERVED_REGr(unit, drc_ndx, data & 0xFFFFFFFC));
            
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 1));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, drc_ndx, &ss));
            (*scPtr).step1000 = ss.step1000;
            (*scPtr).size1000UI = ss.size1000UI;
            
            for(i = 0; i < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT; i++)
            {
                (*scPtr).endUI[i] = ((i + 1) * (ss.size1000UI)) / 1000;
            }
            break;
        case SHMOO_COMBO16_DBI_EDC_RD_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                data = 0;
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
                
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_wr_crc(unit, drc_ndx, 1));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_rd_crc(unit, drc_ndx, 1));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_wr_dbi(unit, drc_ndx, 1));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_rd_dbi(unit, drc_ndx, 1));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 1));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, drc_ndx, &ss));
            (*scPtr).step1000 = ss.step1000;
            (*scPtr).size1000UI = ss.size1000UI;
            
            for(i = 0; i < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT; i++)
            {
                (*scPtr).endUI[i] = ((i + 1) * (ss.size1000UI)) / 1000;
            }
            break;
        case SHMOO_COMBO16_RD_SHORT:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 1));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_mpr_en(unit, drc_ndx, 1));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, drc_ndx, &ss));
            (*scPtr).step1000 = ss.step1000;
            (*scPtr).size1000UI = ss.size1000UI;
            
            for(i = 0; i < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT; i++)
            {
                (*scPtr).endUI[i] = ((i + 1) * (ss.size1000UI)) / 1000;
            }
            break;
        case SHMOO_COMBO16_WR_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                data = SHMOO_COMBO16_DDR4_INIT_WRITE_MIN_VDL_POS;
            }
            else
            {
                data = 0;
            }
            
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
        /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data)); */
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
        /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data)); */
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
        /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data)); */
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
        /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data)); */
                
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 1));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, drc_ndx, &ss));
            (*scPtr).step1000 = ss.step1000;
            (*scPtr).size1000UI = ss.size1000UI;
            
            for(i = 0; i < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT; i++)
            {
                (*scPtr).endUI[i] = ((i + 1) * (ss.size1000UI)) / 1000;
            }
            break;
        case SHMOO_COMBO16_DBI_WR_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                data = 0;
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
            /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data)); */
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
            /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data)); */
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
            /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data)); */
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
            /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data)); */
                
            /*    SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_wr_crc(unit, drc_ndx, 1));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_rd_crc(unit, drc_ndx, 1)); */
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_wr_dbi(unit, drc_ndx, 1));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_rd_dbi(unit, drc_ndx, 1));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 1));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, drc_ndx, &ss));
            (*scPtr).step1000 = ss.step1000;
            (*scPtr).size1000UI = ss.size1000UI;
            
            for(i = 0; i < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT; i++)
            {
                (*scPtr).endUI[i] = ((i + 1) * (ss.size1000UI)) / 1000;
            }
            break;
        case SHMOO_COMBO16_WR_SHORT:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 1));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            
            SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, drc_ndx, &ss));
            (*scPtr).step1000 = ss.step1000;
            (*scPtr).size1000UI = ss.size1000UI;
            
            for(i = 0; i < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT; i++)
            {
                (*scPtr).endUI[i] = ((i + 1) * (ss.size1000UI)) / 1000;
            }
            break;
        case SHMOO_COMBO16_RD_EXTENDED:
            SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, drc_ndx, &ss));
            (*scPtr).step1000 = ss.step1000;
            (*scPtr).size1000UI = ss.size1000UI;
            
            for(i = 0; i < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT; i++)
            {
                (*scPtr).endUI[i] = ((i + 1) * (ss.size1000UI)) / 1000;
            }
            break;
        case SHMOO_COMBO16_WR_EXTENDED:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, 0x80, 0x80));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, drc_ndx, &ss));
            (*scPtr).step1000 = ss.step1000;
            (*scPtr).size1000UI = ss.size1000UI;
            
            for(i = 0; i < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT; i++)
            {
                (*scPtr).endUI[i] = ((i + 1) * (ss.size1000UI)) / 1000;
            }
            break;
        case SHMOO_COMBO16_ADDR_CTRL_EXTENDED:
            SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, drc_ndx, &ss));
            (*scPtr).step1000 = ss.step1000;
            (*scPtr).size1000UI = ss.size1000UI;
            
            for(i = 0; i < SHMOO_COMBO16_MAX_VISIBLE_UI_COUNT; i++)
            {
                (*scPtr).endUI[i] = ((i + 1) * (ss.size1000UI)) / 1000;
            }
            break;
        default:
            LOG_ERROR(BSL_LS_SOC_DDR,
                      (BSL_META_U(unit,
                                  "Unsupported shmoo type: %02d\n"),
                       (*scPtr).shmooType));
            return SOC_E_FAIL;
    }
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_exit(int unit, int drc_ndx, combo16_shmoo_container_t *scPtr, uint32 mode)
{
    /* Mode 0: Sequential exit
     * Mode 1: Single exit
     */
    
    uint32 data;
/*    uint32 wck01, wck23; */
    combo16_shmoo_dram_info_t* shmoo_dram_info;
    combo16_vendor_info_t* shmoo_vendor_info;

    SOC_IF_ERROR_RETURN( _shmoo_combo16_drc_dram_info_access(unit, &shmoo_dram_info));
    SOC_IF_ERROR_RETURN( _shmoo_combo16_drc_vendor_info_access(unit, &shmoo_vendor_info));
        
    switch((*scPtr).shmooType)
    {
        case SHMOO_COMBO16_ADDR_CTRL_SHORT:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_adt(unit, drc_ndx, 0));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_dram_init(unit, drc_ndx, 12));
                sal_usleep(SHMOO_COMBO16_DEEP_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_DQS2CK:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_write_leveling(unit, drc_ndx, shmoo_dram_info->command_parity_latency, 0, 0));
        
                data = 0;
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_LEVELING_CONFIGr, &data, SOC_PHY_DDRC16_WRITE_LEVELING_MODEf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_WCK2CK:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                sal_usleep(SHMOO_COMBO16_DEEP_SLEEP);
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_wck2ck_training(unit, drc_ndx, 0));

                data = 0;
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_LEVELING_CONFIGr, &data, SOC_PHY_DDRC16_WRITE_LEVELING_MODEf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_LEVELING_CONFIGr(unit, drc_ndx, data));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_vendor_info_get(unit, drc_ndx, shmoo_vendor_info));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_EN_FISH:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                data = 0;
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_MINUS_3f, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_MINUS_2f, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_MINUS_1f, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_0f, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_PLUS_1f, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_PLUS_2f, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_DDR4_REN_STRETCH_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_DDR4_REN_STRETCH_CONFIGr(unit, drc_ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_DDR4_REN_STRETCH_CONFIGr(unit, drc_ndx, data));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_START_FISH:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 0));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_EDC_START_FISH:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 0));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_wr_crc(unit, drc_ndx, 0));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_rd_crc(unit, drc_ndx, 0));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 0));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_DBI_EDC_RD_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 0));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_wr_crc(unit, drc_ndx, 0));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_rd_crc(unit, drc_ndx, 0));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_wr_dbi(unit, drc_ndx, 0));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_rd_dbi(unit, drc_ndx, 0));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_SHORT:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 0));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_mpr_en(unit, drc_ndx, 0));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_WR_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 0));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_DBI_WR_DESKEW:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 0));
            /*    SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_wr_crc(unit, drc_ndx, 0));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_rd_crc(unit, drc_ndx, 0)); */
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_wr_dbi(unit, drc_ndx, 0));
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_rd_dbi(unit, drc_ndx, 0));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_WR_SHORT:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_gddr5_training_protocol(unit, drc_ndx, 0));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_RD_EXTENDED:
            break;
        case SHMOO_COMBO16_WR_EXTENDED:
            if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, 0x00, 0x80));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            }
            else if((*scPtr).dramType == SHMOO_COMBO16_DRAM_TYPE_GDDR5)
            {
            }
            else
            {
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported dram type and shmoo type combination: %02d / %02d\n"),
                           (*scPtr).dramType, (*scPtr).shmooType));
                return SOC_E_FAIL;
            }
            break;
        case SHMOO_COMBO16_ADDR_CTRL_EXTENDED:
            break;
        default:
            LOG_ERROR(BSL_LS_SOC_DDR,
                      (BSL_META_U(unit,
                                  "Unsupported shmoo type: %02d\n"),
                       (*scPtr).shmooType));
            return SOC_E_FAIL;
    }
    
    SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_enable_refresh(unit, drc_ndx , 1, (*scPtr).trefi, &data));
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_save(int unit, int drc_ndx, combo16_shmoo_config_param_t *config_param)
{
    uint32 data;
    
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[0][0] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[0][1] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[0][2] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[0][3] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[0][4] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[0][5] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[0][6] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[0][7] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_DBIr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_dbi[0] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_EDCr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_edc[0] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_max_vdl_data[0] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_max_vdl_dqs[0] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT0r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[0][0] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT1r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[0][1] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT2r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[0][2] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT3r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[0][3] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT4r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[0][4] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT5r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[0][5] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT6r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[0][6] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT7r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[0][7] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MIN_VDL_DBIr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_dbi[0] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MIN_VDL_EDCr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_edc[0] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_max_vdl_dqsp[0] = data;
    /* SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQSDNr(unit, drc_ndx, &data)); */
    (*config_param).dq_byte_rd_max_vdl_dqsn[0] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, &data));
    (*config_param).dq_byte_ren_fifo_config[0] = data;
    /* SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_EDCEN_FIFO_CONFIGr(unit, drc_ndx, &data)); */
    (*config_param).dq_byte_edcen_fifo_config[0] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_DDR4_READ_MAX_VDL_FSMr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_max_vdl_fsm[0] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_VREF_DAC_CONFIGr(unit, drc_ndx, &data));
    (*config_param).dq_byte_vref_dac_config[0] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_MACRO_RESERVED_REGr(unit, drc_ndx, &data));
    (*config_param).dq_byte_macro_reserved_reg[0] = data;
    
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[1][0] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[1][1] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[1][2] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[1][3] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[1][4] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[1][5] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[1][6] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[1][7] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_DBIr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_dbi[1] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_EDCr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_edc[1] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_max_vdl_data[1] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DQSr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_max_vdl_dqs[1] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT0r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[1][0] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT1r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[1][1] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT2r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[1][2] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT3r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[1][3] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT4r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[1][4] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT5r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[1][5] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT6r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[1][6] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT7r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[1][7] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_READ_MIN_VDL_DBIr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_dbi[1] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_READ_MIN_VDL_EDCr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_edc[1] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_max_vdl_dqsp[1] = data;
    /* SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQSDNr(unit, drc_ndx, &data)); */
    (*config_param).dq_byte_rd_max_vdl_dqsn[1] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, &data));
    (*config_param).dq_byte_ren_fifo_config[1] = data;
    /* SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_EDCEN_FIFO_CONFIGr(unit, drc_ndx, &data)); */
    (*config_param).dq_byte_edcen_fifo_config[1] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_DDR4_READ_MAX_VDL_FSMr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_max_vdl_fsm[1] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_VREF_DAC_CONFIGr(unit, drc_ndx, &data));
    (*config_param).dq_byte_vref_dac_config[1] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE1_MACRO_RESERVED_REGr(unit, drc_ndx, &data));
    (*config_param).dq_byte_macro_reserved_reg[1] = data;
    
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[2][0] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[2][1] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[2][2] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[2][3] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[2][4] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[2][5] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[2][6] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[2][7] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_DBIr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_dbi[2] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_EDCr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_edc[2] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_max_vdl_data[2] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DQSr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_max_vdl_dqs[2] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT0r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[2][0] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT1r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[2][1] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT2r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[2][2] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT3r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[2][3] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT4r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[2][4] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT5r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[2][5] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT6r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[2][6] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT7r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[2][7] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_READ_MIN_VDL_DBIr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_dbi[2] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_READ_MIN_VDL_EDCr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_edc[2] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_max_vdl_dqsp[2] = data;
    /* SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQSNr(unit, drc_ndx, &data)); */
    (*config_param).dq_byte_rd_max_vdl_dqsn[2] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, &data));
    (*config_param).dq_byte_ren_fifo_config[2] = data;
    /* SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_EDCEN_FIFO_CONFIGr(unit, drc_ndx, &data)); */
    (*config_param).dq_byte_edcen_fifo_config[2] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_DDR4_READ_MAX_VDL_FSMr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_max_vdl_fsm[2] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_VREF_DAC_CONFIGr(unit, drc_ndx, &data));
    (*config_param).dq_byte_vref_dac_config[2] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE2_MACRO_RESERVED_REGr(unit, drc_ndx, &data));
    (*config_param).dq_byte_macro_reserved_reg[2] = data;
    
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[3][0] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[3][1] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[3][2] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[3][3] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[3][4] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[3][5] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[3][6] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_bit[3][7] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_DBIr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_dbi[3] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_EDCr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_min_vdl_edc[3] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_max_vdl_data[3] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DQSr(unit, drc_ndx, &data));
    (*config_param).dq_byte_wr_max_vdl_dqs[3] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT0r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[3][0] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT1r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[3][1] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT2r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[3][2] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT3r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[3][3] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT4r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[3][4] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT5r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[3][5] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT6r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[3][6] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT7r(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_bit[3][7] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_READ_MIN_VDL_DBIr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_dbi[3] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_READ_MIN_VDL_EDCr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_min_vdl_edc[3] = (uint8) data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_max_vdl_dqsp[3] = data;
    /* SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQSNr(unit, drc_ndx, &data)); */
    (*config_param).dq_byte_rd_max_vdl_dqsn[3] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, &data));
    (*config_param).dq_byte_ren_fifo_config[3] = data;
    /* SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_EDCEN_FIFO_CONFIGr(unit, drc_ndx, &data)); */
    (*config_param).dq_byte_edcen_fifo_config[3] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_DDR4_READ_MAX_VDL_FSMr(unit, drc_ndx, &data));
    (*config_param).dq_byte_rd_max_vdl_fsm[3] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_VREF_DAC_CONFIGr(unit, drc_ndx, &data));
    (*config_param).dq_byte_vref_dac_config[3] = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE3_MACRO_RESERVED_REGr(unit, drc_ndx, &data));
    (*config_param).dq_byte_macro_reserved_reg[3] = data;
    
    SOC_IF_ERROR_RETURN(READ_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, drc_ndx, &data));
    (*config_param).aq_l_max_vdl_addr = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_AQ_L_MAX_VDL_CTRLr(unit, drc_ndx, &data));
    (*config_param).aq_l_max_vdl_ctrl = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_AQ_L_MACRO_RESERVED_REGr(unit, drc_ndx, &data));
    (*config_param).aq_l_macro_reserved_reg = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_AQ_U_MAX_VDL_ADDRr(unit, drc_ndx, &data));
    (*config_param).aq_u_max_vdl_addr = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_AQ_U_MAX_VDL_CTRLr(unit, drc_ndx, &data));
    (*config_param).aq_u_max_vdl_ctrl = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_AQ_U_MACRO_RESERVED_REGr(unit, drc_ndx, &data));
    (*config_param).aq_u_macro_reserved_reg = data;
    
    SOC_IF_ERROR_RETURN(READ_DDRC16_COMMON_MACRO_RESERVED_REGr(unit, drc_ndx, &data));
    (*config_param).common_macro_reserved_reg = data;
    
    SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_READ_CLOCK_CONFIGr(unit, drc_ndx, &data));
    (*config_param).control_regs_read_clock_config = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_INPUT_SHIFT_CTRLr(unit, drc_ndx, &data));
    (*config_param).control_regs_input_shift_ctrl = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
    (*config_param).control_regs_ren_fifo_central_init = data;
    /* SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_EDCEN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data)); */
    (*config_param).control_regs_edcen_fifo_central_init = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr(unit, drc_ndx, &data));
    (*config_param).control_regs_shared_vref_dac_config = data;
    SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESERVED_REGr(unit, drc_ndx, &data));
    (*config_param).control_regs_reserved_reg = data;
    
    return SOC_E_NONE;
}

STATIC int
_shmoo_combo16_restore(int unit, int drc_ndx, combo16_shmoo_config_param_t *config_param)
{
    uint32 data;
    int MRfield0, MRfield1, MRfield2, MRfield3;
    int swap;
    combo16_shmoo_dram_info_t* shmoo_dram_info;

    SOC_IF_ERROR_RETURN( _shmoo_combo16_drc_dram_info_access(unit, &shmoo_dram_info));
    
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[0][0];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[0][1];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[0][2];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[0][3];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[0][4];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[0][5];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[0][6];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[0][7];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_dbi[0];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_edc[0];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data));
    if(shmoo_dram_info->dram_type == SHMOO_COMBO16_DRAM_TYPE_DDR4)
    {
        data = (*config_param).dq_byte_wr_max_vdl_data[0];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
        data = (*config_param).dq_byte_wr_max_vdl_dqs[0];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
    }
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[0][0];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT0r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[0][1];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT1r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[0][2];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT2r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[0][3];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT3r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[0][4];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT4r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[0][5];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT5r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[0][6];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT6r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[0][7];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT7r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_dbi[0];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_edc[0];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
    if(shmoo_dram_info->dram_type == SHMOO_COMBO16_DRAM_TYPE_DDR4)
    {
        data = (*config_param).dq_byte_rd_max_vdl_dqsp[0];
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 1));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 0));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        /* data = (*config_param).dq_byte_rd_max_vdl_dqsn[0];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQSDNr(unit, drc_ndx, data)); */
        data = (*config_param).dq_byte_ren_fifo_config[0];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, data));
        /* data = (*config_param).dq_byte_edcen_fifo_config[0];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_EDCEN_FIFO_CONFIGr(unit, drc_ndx, data)); */
        data = (*config_param).dq_byte_rd_max_vdl_fsm[0];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_DDR4_READ_MAX_VDL_FSMr(unit, drc_ndx, data));
    }
    data = (*config_param).dq_byte_vref_dac_config[0];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_VREF_DAC_CONFIGr(unit, drc_ndx, data));
    data = (*config_param).dq_byte_macro_reserved_reg[0];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_MACRO_RESERVED_REGr(unit, drc_ndx, data));
    
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[1][0];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[1][1];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[1][2];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[1][3];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[1][4];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[1][5];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[1][6];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[1][7];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_dbi[1];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_edc[1];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data));
    if(shmoo_dram_info->dram_type == SHMOO_COMBO16_DRAM_TYPE_DDR4)
    {
        data = (*config_param).dq_byte_wr_max_vdl_data[1];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
        data = (*config_param).dq_byte_wr_max_vdl_dqs[1];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
    }
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[1][0];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT0r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[1][1];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT1r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[1][2];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT2r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[1][3];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT3r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[1][4];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT4r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[1][5];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT5r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[1][6];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT6r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[1][7];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT7r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_dbi[1];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_edc[1];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
    if(shmoo_dram_info->dram_type == SHMOO_COMBO16_DRAM_TYPE_DDR4)
    {
        data = (*config_param).dq_byte_rd_max_vdl_dqsp[1];
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 1));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 0));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        /* data = (*config_param).dq_byte_rd_max_vdl_dqsn[1];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQSNr(unit, drc_ndx, data)); */
        data = (*config_param).dq_byte_ren_fifo_config[1];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, data));
        /* data = (*config_param).dq_byte_edcen_fifo_config[1];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_EDCEN_FIFO_CONFIGr(unit, drc_ndx, data)); */
        data = (*config_param).dq_byte_rd_max_vdl_fsm[1];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_DDR4_READ_MAX_VDL_FSMr(unit, drc_ndx, data));
    }
    data = (*config_param).dq_byte_vref_dac_config[1];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_VREF_DAC_CONFIGr(unit, drc_ndx, data));
    data = (*config_param).dq_byte_macro_reserved_reg[1];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_MACRO_RESERVED_REGr(unit, drc_ndx, data));
    
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[2][0];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[2][1];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[2][2];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[2][3];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[2][4];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[2][5];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[2][6];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[2][7];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_dbi[2];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_edc[2];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data));
    if(shmoo_dram_info->dram_type == SHMOO_COMBO16_DRAM_TYPE_DDR4)
    {
        data = (*config_param).dq_byte_wr_max_vdl_data[2];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
        data = (*config_param).dq_byte_wr_max_vdl_dqs[2];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
    }
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[2][0];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT0r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[2][1];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT1r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[2][2];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT2r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[2][3];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT3r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[2][4];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT4r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[2][5];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT5r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[2][6];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT6r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[2][7];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT7r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_dbi[2];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_edc[2];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
    if(shmoo_dram_info->dram_type == SHMOO_COMBO16_DRAM_TYPE_DDR4)
    {
        data = (*config_param).dq_byte_rd_max_vdl_dqsp[2];
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 1));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 0));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        /* data = (*config_param).dq_byte_rd_max_vdl_dqsn[2];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQSNr(unit, drc_ndx, data)); */
        data = (*config_param).dq_byte_ren_fifo_config[2];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, data));
        /* data = (*config_param).dq_byte_edcen_fifo_config[2];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_EDCEN_FIFO_CONFIGr(unit, drc_ndx, data)); */
        data = (*config_param).dq_byte_rd_max_vdl_fsm[2];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_DDR4_READ_MAX_VDL_FSMr(unit, drc_ndx, data));
    }
    data = (*config_param).dq_byte_vref_dac_config[2];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_VREF_DAC_CONFIGr(unit, drc_ndx, data));
    data = (*config_param).dq_byte_macro_reserved_reg[2];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_MACRO_RESERVED_REGr(unit, drc_ndx, data));
    
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[3][0];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT0r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[3][1];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT1r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[3][2];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT2r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[3][3];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT3r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[3][4];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT4r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[3][5];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT5r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[3][6];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT6r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_bit[3][7];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT7r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_dbi[3];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_DBIr(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_wr_min_vdl_edc[3];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_EDCr(unit, drc_ndx, data));
    if(shmoo_dram_info->dram_type == SHMOO_COMBO16_DRAM_TYPE_DDR4)
    {
        data = (*config_param).dq_byte_wr_max_vdl_data[3];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, drc_ndx, data));
        data = (*config_param).dq_byte_wr_max_vdl_dqs[3];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DQSr(unit, drc_ndx, data));
    }
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[3][0];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT0r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[3][1];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT1r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[3][2];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT2r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[3][3];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT3r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[3][4];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT4r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[3][5];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT5r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[3][6];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT6r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_bit[3][7];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT7r(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_dbi[3];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_DBIr(unit, drc_ndx, data));
    data = (uint32) (*config_param).dq_byte_rd_min_vdl_edc[3];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_EDCr(unit, drc_ndx, data));
    if(shmoo_dram_info->dram_type == SHMOO_COMBO16_DRAM_TYPE_DDR4)
    {
        data = (*config_param).dq_byte_rd_max_vdl_dqsp[3];
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 1));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 0));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, drc_ndx, data));
        /* data = (*config_param).dq_byte_rd_max_vdl_dqsn[3];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQSNr(unit, drc_ndx, data)); */
        data = (*config_param).dq_byte_ren_fifo_config[3];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_DDR4_REN_FIFO_CONFIGr(unit, drc_ndx, data));
        /* data = (*config_param).dq_byte_edcen_fifo_config[3];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_EDCEN_FIFO_CONFIGr(unit, drc_ndx, data)); */
        data = (*config_param).dq_byte_rd_max_vdl_fsm[3];
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_DDR4_READ_MAX_VDL_FSMr(unit, drc_ndx, data));
    }
    data = (*config_param).dq_byte_vref_dac_config[3];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_VREF_DAC_CONFIGr(unit, drc_ndx, data));
    data = (*config_param).dq_byte_macro_reserved_reg[3];
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_MACRO_RESERVED_REGr(unit, drc_ndx, data));
    
    data = (*config_param).aq_l_max_vdl_addr;
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, drc_ndx, data));
    data = (*config_param).aq_l_max_vdl_ctrl;
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_CTRLr(unit, drc_ndx, data));
    data = (*config_param).aq_l_macro_reserved_reg;
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MACRO_RESERVED_REGr(unit, drc_ndx, data));
    data = (*config_param).aq_u_max_vdl_addr;
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_ADDRr(unit, drc_ndx, data));
    data = (*config_param).aq_u_max_vdl_ctrl;
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_CTRLr(unit, drc_ndx, data));
    data = (*config_param).aq_u_macro_reserved_reg;
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MACRO_RESERVED_REGr(unit, drc_ndx, data));
    
    data = (*config_param).common_macro_reserved_reg;
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_COMMON_MACRO_RESERVED_REGr(unit, drc_ndx, data));
    
    if(shmoo_dram_info->dram_type == SHMOO_COMBO16_DRAM_TYPE_DDR4)
    {
        data = (*config_param).control_regs_read_clock_config;
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_READ_CLOCK_CONFIGr(unit, drc_ndx, data));
        data = (*config_param).control_regs_input_shift_ctrl;
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_INPUT_SHIFT_CTRLr(unit, drc_ndx, data));
        data = (*config_param).control_regs_ren_fifo_central_init;
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
        /* data = (*config_param).control_regs_edcen_fifo_central_init;
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_EDCEN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data)); */
    }
    data = (*config_param).control_regs_shared_vref_dac_config;
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr(unit, drc_ndx, data));
    data = (*config_param).control_regs_reserved_reg;
    SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESERVED_REGr(unit, drc_ndx, data));

    if(shmoo_dram_info->dram_type == SHMOO_COMBO16_DRAM_TYPE_DDR4)
    {
        SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 0));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
        
        SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 0));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));

        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 1));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 1));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 1));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 1));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 1));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, drc_ndx, data));
        
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 1));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 1));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 1));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 1));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, drc_ndx, data));
    }

    SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_dram_init(unit, drc_ndx, 10));
    sal_usleep(SHMOO_COMBO16_DEEP_SLEEP);
    
    MRfield0 = ((int) (*config_param).dq_byte_macro_reserved_reg[0]);
    MRfield1 = ((int) (*config_param).dq_byte_macro_reserved_reg[1]);
    MRfield2 = ((int) (*config_param).dq_byte_macro_reserved_reg[2]);
    MRfield3 = ((int) (*config_param).dq_byte_macro_reserved_reg[3]);
    
    switch(shmoo_dram_info->dram_type)
    {
        case SHMOO_COMBO16_DRAM_TYPE_DDR4:
            
            MRfield0 = (MRfield0 + MRfield1 + MRfield2 + MRfield3) >> 2;
            
            if(MRfield0 > SHMOO_COMBO16_DDR4_VREF_RANGE_CROSSOVER)
            {   /* Vref Range 1 */
                MRfield0 = MRfield0 - 23;
            }
            else
            {   /* Vref Range 2 */
                MRfield0 = MRfield0 | 0x40;
            }
            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, MRfield0, 0x7F));
            break;
        case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
            SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_gddr5_dq_byte_pairs_swap_info_get(unit, drc_ndx, &swap));
            
            if(swap)
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, ((((MRfield0 + MRfield1) >> 1) & 0xF) << 8) | ((((MRfield2 + MRfield3) >> 1) & 0xF) << 4), 0xFF0));
            }
            else
            {
                SOC_IF_ERROR_RETURN(_shmoo_combo16_drc_modify_mrs(unit, drc_ndx, 6, ((((MRfield2 + MRfield3) >> 1) & 0xF) << 8) | ((((MRfield0 + MRfield1) >> 1) & 0xF) << 4), 0xFF0));
            }
            break;
        default:
            LOG_ERROR(BSL_LS_SOC_DDR,
                      (BSL_META_U(unit,
                                  "Unsupported dram type: %02d\n"),
                       shmoo_dram_info->dram_type));
            return SOC_E_FAIL;
    }
    
    sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
    
    return SOC_E_NONE;
}

/*
 * Function:
 *      soc_combo16_shmoo_ctl
 * Purpose:
 *      Perform shmoo (PHY calibration) on specific DRC index.
 * Parameters:
 *      unit                - unit number
 *      drc_ndx             - DRC index to perform shmoo on.
 *      shmoo_type          - Selects shmoo sub-section to be performs (-1 for full shmoo)
 *      flags               -   SHMOO_COMBO16_CTL_FLAGS_STAT_BIT            
 *                                  0: Runs normal shmoo functions
 *                                  1: Doesn't run shmoo. Only prints statistics. (Nothing is printed at the moment)
 *                              SHMOO_COMBO16_CTL_FLAGS_PLOT_BIT            
 *                                  0: No shmoo plots are printed during shmoo
 *                                  1: Shmoo plots are printed during shmoo
 *                              SHMOO_COMBO16_CTL_FLAGS_EXT_VREF_RANGE_BIT
 *                                  0: Shmoo runs with a regular range for Vref sweep in *EXTENDED shmoos
 *                                  1: Shmoo runs with an extended range for Vref sweep in *EXTENDED shmoos
 *      action              - Save/restore functionality
 *      *config_param       - PHY configuration saved/restored
 * Returns:
 *      SOC_E_XXX
 *      This routine may be called after a device is attached
 *      or whenever a chip reset is required.
 */

int
soc_combo16_shmoo_ctl(int unit, int drc_ndx, int shmoo_type, uint32 flags, int action, combo16_shmoo_config_param_t *config_param)
{
    combo16_shmoo_container_t *scPtr = NULL;
    uint32 dramType;
    uint32 ctlType;
    uint32 i;
    int ndx, ndxEnd;
    uint32 stat, plot;
    const uint32 *seqPtr;
    uint32 seqCount;
    combo16_shmoo_dram_info_t* shmoo_dram_info;
        
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT( _shmoo_combo16_drc_dram_info_access(unit, &shmoo_dram_info));

    if(!_shmoo_combo16_validate_config(unit))
    {
        LOG_ERROR(BSL_LS_SOC_DDR,
                  (BSL_META_U(unit,
                              "Invalid PHY PLL or DRAM configuration identified during Shmoo\n")));
        return SOC_E_FAIL;
    }
    
    dramType = shmoo_dram_info->dram_type;
    ctlType = shmoo_dram_info->ctl_type;

    stat = flags & SHMOO_COMBO16_CTL_FLAGS_STAT_BIT;
    plot = flags & SHMOO_COMBO16_CTL_FLAGS_PLOT_BIT;
    
    if(!stat)
    {
        scPtr = sal_alloc(sizeof(combo16_shmoo_container_t), "Combo16 Shmoo Container");
        if(scPtr == NULL)
        {
            return SOC_E_MEMORY;
        }
        sal_memset(scPtr, 0, sizeof(combo16_shmoo_container_t));
        
        (*scPtr).debugMode = flags & SHMOO_COMBO16_DEBUG_MODE_FLAG_BITS;
        
        if(drc_ndx != SHMOO_COMBO16_INTERFACE_RSVP)
        {
            ndx = drc_ndx;
            ndxEnd = drc_ndx + 1;
        }
        else
        {
            ndx = 0;
            ndxEnd = SHMOO_COMBO16_MAX_INTERFACES;
        }
        
        for(; ndx < ndxEnd; ndx++)
        {
            if(!_shmoo_combo16_check_dram(unit, ndx)) {
                continue;
            }
            
            if(action == SHMOO_COMBO16_ACTION_RESTORE)
            {
                (*scPtr).restore = 1;
                switch(ctlType)
                {
                    case SHMOO_COMBO16_CTL_TYPE_RSVP:
                        break;
                    case SHMOO_COMBO16_CTL_TYPE_1:
                        _SOC_IF_ERR_EXIT(_shmoo_combo16_restore(unit, drc_ndx, config_param));
                        break;
                    default:
                        LOG_ERROR(BSL_LS_SOC_DDR,
                                  (BSL_META_U(unit,
                                              "Unsupported controller type: %02d\n"),
                                   ctlType));
                        _SOC_EXIT_WITH_ERR(SOC_E_FAIL, (BSL_META("%s\n"), soc_errmsg(SOC_E_FAIL)));
                }
            }
            else
            {
                (*scPtr).restore = 0;
            }
            
            if((action == SHMOO_COMBO16_ACTION_RUN) || (action == SHMOO_COMBO16_ACTION_RUN_AND_SAVE) || (action == SHMOO_COMBO16_ACTION_RESTORE))
            {
                switch(ctlType)
                {
                    case SHMOO_COMBO16_CTL_TYPE_RSVP:
                        break;
                    case SHMOO_COMBO16_CTL_TYPE_1:
                        if((*scPtr).restore)
                        {
                            switch(dramType)
                            {
                                case SHMOO_COMBO16_DRAM_TYPE_DDR4:
                                    seqPtr = NULL;
                                    seqCount = SHMOO_COMBO16_DDR4_RESTORE_SEQUENCE_COUNT;
                                    break;
                                case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
                                    seqPtr = &shmoo_order_combo16_gddr5_restore[0];
                                    seqCount = SHMOO_COMBO16_GDDR5_RESTORE_SEQUENCE_COUNT;
                                    break;
                                default:
                                    LOG_ERROR(BSL_LS_SOC_DDR,
                                              (BSL_META_U(unit,
                                                          "Unsupported dram type: %02d\n"),
                                               dramType));
                                    _SOC_EXIT_WITH_ERR(SOC_E_FAIL, (BSL_META("%s\n"), soc_errmsg(SOC_E_FAIL)));
                            }
                        }
                        else
                        {
                            switch(dramType)
                            {
                                case SHMOO_COMBO16_DRAM_TYPE_DDR4:
                                    seqPtr = &shmoo_order_combo16_ddr4[0];
                                    seqCount = SHMOO_COMBO16_DDR4_SEQUENCE_COUNT;
                                    break;
                                case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
                                    seqPtr = &shmoo_order_combo16_gddr5[0];
                                    seqCount = SHMOO_COMBO16_GDDR5_SEQUENCE_COUNT;
                                    break;
                                default:
                                    LOG_ERROR(BSL_LS_SOC_DDR,
                                              (BSL_META_U(unit,
                                                          "Unsupported dram type: %02d\n"),
                                               dramType));
                                    _SOC_EXIT_WITH_ERR(SOC_E_FAIL, (BSL_META("%s\n"), soc_errmsg(SOC_E_FAIL)));
                            }
                        }
                        
                        (*scPtr).dramType = dramType;
                        (*scPtr).ctlType = ctlType;
                        
                        if(shmoo_type != SHMOO_COMBO16_SHMOO_RSVP)
                        {
                            (*scPtr).shmooType = shmoo_type;
                            _SOC_IF_ERR_EXIT(_shmoo_combo16_entry(unit, ndx, scPtr, SHMOO_COMBO16_SINGLE));
                            _SOC_IF_ERR_EXIT(_shmoo_combo16_do(unit, ndx, scPtr));
                            _SOC_IF_ERR_EXIT(_shmoo_combo16_calib_2D(unit, ndx, scPtr));
                            _SOC_IF_ERR_EXIT(_shmoo_combo16_set_new_step(unit, ndx, scPtr));
                            if(plot || LOG_CHECK(BSL_LS_SOC_DDR | BSL_INFO))
                            {
                                _SOC_IF_ERR_EXIT(_shmoo_combo16_plot(unit, ndx, scPtr));
                            }
                            _SOC_IF_ERR_EXIT(_shmoo_combo16_exit(unit, ndx, scPtr, SHMOO_COMBO16_SINGLE));
                        }
                        else
                        {
                            for(i = 0; i < seqCount; i++)
                            {
                                (*scPtr).shmooType = seqPtr[i];
                                _SOC_IF_ERR_EXIT(_shmoo_combo16_entry(unit, ndx, scPtr, SHMOO_COMBO16_SEQUENTIAL));
                                _SOC_IF_ERR_EXIT(_shmoo_combo16_do(unit, ndx, scPtr));
                                _SOC_IF_ERR_EXIT(_shmoo_combo16_calib_2D(unit, ndx, scPtr));
                                _SOC_IF_ERR_EXIT(_shmoo_combo16_set_new_step(unit, ndx, scPtr));
                                if(plot || LOG_CHECK(BSL_LS_SOC_DDR | BSL_INFO))
                                {
                                    _SOC_IF_ERR_EXIT(_shmoo_combo16_plot(unit, ndx, scPtr));
                                }
                                _SOC_IF_ERR_EXIT(_shmoo_combo16_exit(unit, ndx, scPtr, SHMOO_COMBO16_SEQUENTIAL));
                            }
                        }
                        
                        break;
                    default:
                        LOG_ERROR(BSL_LS_SOC_DDR,
                                  (BSL_META_U(unit,
                                              "Unsupported controller type: %02d\n"),
                                   ctlType));
                        _SOC_EXIT_WITH_ERR(SOC_E_FAIL, (BSL_META("%s\n"), soc_errmsg(SOC_E_FAIL)));
                }
            }
            
            if((action == SHMOO_COMBO16_ACTION_RUN_AND_SAVE) || (action == SHMOO_COMBO16_ACTION_SAVE))
            {
                _SOC_IF_ERR_EXIT(_shmoo_combo16_save(unit, drc_ndx, config_param));
            }
        }
        
        LOG_VERBOSE(BSL_LS_SOC_DDR,
                    (BSL_META_U(unit,
                                "DDR Tuning Complete\n")));
    }
    else
    {
        /* Report only */
        switch(ctlType)
        {
            case SHMOO_COMBO16_CTL_TYPE_RSVP:
                break;
            case SHMOO_COMBO16_CTL_TYPE_1:
                break;
            default:
                LOG_ERROR(BSL_LS_SOC_DDR,
                          (BSL_META_U(unit,
                                      "Unsupported controller type: %02d\n"),
                           ctlType));
                _SOC_EXIT_WITH_ERR(SOC_E_FAIL, (BSL_META("%s\n"), soc_errmsg(SOC_E_FAIL)));
        }
    }

exit:
    SOC_FREE(scPtr);
    SOC_FUNC_RETURN;
}

/* Configure PHY PLL and wait for lock */
int
soc_combo16_shmoo_phy_cfg_pll(int unit, int drc_ndx)
{
    int ndx, ndxEnd;
    int fref, fref_post, fref_eff_int, fref_eff_frac, freq_vco, data_rate;
    int pdiv10, ndiv_int10;
    uint32 dramType;
    uint32 frequency_doubler_mode;
    uint32 ref_clk_bitmap;
    combo16_drc_pll_t pll_info;
    int orig_ndx = -1;
    combo16_shmoo_dram_info_t* shmoo_dram_info;

    SOC_IF_ERROR_RETURN( _shmoo_combo16_drc_dram_info_access(unit, &shmoo_dram_info));
    
    if(!_shmoo_combo16_validate_cbi())
    {
        LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Null callback function detected during PLL initialization\n")));
        return SOC_E_FAIL;
    }
    
    if(!_shmoo_combo16_validate_config(unit))
    {
        LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Invalid PHY PLL or DRAM configuration identified during PLL initialization\n")));
        return SOC_E_FAIL;
    }
    
    dramType = shmoo_dram_info->dram_type;
    data_rate = shmoo_dram_info->data_rate_mbps;
    fref = shmoo_dram_info->ref_clk_mhz;
    ref_clk_bitmap = shmoo_dram_info->ref_clk_bitmap;
    
    pll_info.iso_in = 0;
    pll_info.ldo_ctrl = 0x0000;
    pll_info.ssc_limit = 0x000000;
    pll_info.ssc_mode = 0;
    pll_info.ssc_step = 0x0000;
    pll_info.pll_ctrl = 0x00000000;
    
    switch(dramType)
    {
        case SHMOO_COMBO16_DRAM_TYPE_DDR4:
            fref_eff_int = 33;
            fref_eff_frac = 333;
            freq_vco = data_rate << 1;
            break;
        case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
            fref_eff_int = 50;
            fref_eff_frac = 000;
            freq_vco = data_rate;
            break;
        default:
            LOG_ERROR(BSL_LS_SOC_DDR,
                      (BSL_META_U(unit,
                                  "Unsupported dram type: %02d\n"),
                       dramType));
            return SOC_E_FAIL;
    }
    
    frequency_doubler_mode = 0;                                                     /* Frequency doubler OFF */
    pll_info.en_ctrl = frequency_doubler_mode;
    
    fref_post = fref << frequency_doubler_mode;
    pdiv10 = (10000 * fref_post) / ((1000 * fref_eff_int) + fref_eff_frac);
    
    if((pdiv10 % 10) >= 5)
    {
        pll_info.pdiv = (pdiv10 / 10) + 1;
    }
    else
    {
        pll_info.pdiv = (pdiv10 / 10);
    }
    
    ndiv_int10 = (10000 * freq_vco) / ((1000 * fref_eff_int) + fref_eff_frac);
    
    if((ndiv_int10 % 10) >= 5)
    {
        pll_info.ndiv_int = (ndiv_int10 / 10) + 1;
    }
    else
    {
        pll_info.ndiv_int = (ndiv_int10 / 10);
    }
    
    pll_info.ndiv_frac = 0;                                                         /* ndiv fraction OFF */
    pll_info.mdiv = 1;
    pll_info.fref_eff_info = fref_eff_int;
    
    LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "PHY PLL Configuration\n")));
    LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Fref...........: %4d\n"), fref));
    LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "PDIV...........: %4d\n"), pll_info.pdiv));
    LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "NDIV...........: %4d\n"), pll_info.ndiv_int));
    LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Fvco...........: %4d\n"), freq_vco));
    LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Data rate......: %4d\n"), data_rate));
    
    if(freq_vco > 5500)
    {
        pll_info.vco_sel = 1;
    }
    else
    {
        pll_info.vco_sel = 0;
    }

    if(drc_ndx != SHMOO_COMBO16_INTERFACE_RSVP)
    {
        ndx = drc_ndx;
        ndxEnd = drc_ndx + 1;
    }
    else
    {
        ndx = 0;
        ndxEnd = SHMOO_COMBO16_MAX_INTERFACES;
    }

    /* MASTER PHYS -- Reference clock: Differential */
    orig_ndx = ndx;
    
    for(; ndx < ndxEnd; ndx++)
    {
        if(!_shmoo_combo16_check_dram(unit, ndx))
        {
            continue;
        }
        
        if(!(ref_clk_bitmap & (0x1 << ndx)))
        {
            continue;
        }
        
        if(_shmoo_combo16_drc_pll_set(unit, ndx, &pll_info) != SOC_E_NONE)
        {
            return SOC_E_FAIL;
        }
    }
    
    /* SLAVE PHYS -- Reference clock: Single-ended */
    ndx = orig_ndx;
    
    for(; ndx < ndxEnd; ndx++)
    {
        if(!_shmoo_combo16_check_dram(unit, ndx))
        {
            continue;
        }
        
        if(ref_clk_bitmap & (0x1 << ndx))
        {
            continue;
        }
        
        if(_shmoo_combo16_drc_pll_set(unit, ndx, &pll_info) != SOC_E_NONE)
        {
            return SOC_E_FAIL;
        }
    }

    return SOC_E_NONE;
}

int
soc_combo16_shmoo_phy_init(int unit, int drc_ndx)
{
    int ndx, ndxEnd;
    uint32 data, reset_ctrl_data, read_fifo_ctrl_data;
    uint32 dramType;
    uint32 temp1 = 0, temp2 = 0;
    combo16_step_size_t ss;
    uint32 step1000;
    uint32 size1000UI;
    uint32 setting;
    uint32 ck_ui, addr_ctrl_ui, dqs_ui, data_ui;
    uint32 error, status;
    uint32 dq_pd, dq_nd, dq_pterm, dq_nterm;
    uint32 aq_pd, aq_nd, aq_pterm, aq_nterm;
    uint32 fld_comp_done;
    soc_timeout_t to;
    sal_usecs_t to_val;
    combo16_shmoo_dram_info_t* shmoo_dram_info;

    SOC_IF_ERROR_RETURN( _shmoo_combo16_drc_dram_info_access(unit, &shmoo_dram_info));

    if(!_shmoo_combo16_validate_cbi())
    {
        LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Null callback function detected during PHY initialization\n")));
        return SOC_E_FAIL;
    }
    
    if(!_shmoo_combo16_validate_config(unit))
    {
        LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Invalid PHY PLL or DRAM configuration identified during PHY initialization\n")));
        return SOC_E_FAIL;
    }
    
    dramType = shmoo_dram_info->dram_type;

    if (SAL_BOOT_QUICKTURN)
    {
        to_val = 10000000;  /* 10 Sec */
    }
    else
    {
        to_val = 50000;     /* 50 mS */
    }
    
    if(drc_ndx != SHMOO_COMBO16_INTERFACE_RSVP)
    {
        ndx = drc_ndx;
        ndxEnd = drc_ndx + 1;
    }
    else
    {
        ndx = 0;
        ndxEnd = SHMOO_COMBO16_MAX_INTERFACES;
    }
    
    for(; ndx < ndxEnd; ndx++)
    {
        if(!_shmoo_combo16_check_dram(unit, ndx))
        {
            continue;
        }
        
        LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A Series - PHY Initialization (DRC index: %02d)\n"), ndx));
        
/*A01*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A01. Reset all PHY logic\n")));
        reset_ctrl_data = 0;
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, ndx, reset_ctrl_data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
/*A02*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A02. Enable Read FIFO\n")));
        read_fifo_ctrl_data = 0;
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_READ_FIFO_CTRLr, &read_fifo_ctrl_data, SOC_PHY_DDRC16_W2R_MIN_DELAY_2f, 0));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_READ_FIFO_CTRLr(unit, ndx, read_fifo_ctrl_data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_READ_FIFO_RESET_Nf, 1));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, ndx, reset_ctrl_data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_READ_FIFO_CTRLr, &read_fifo_ctrl_data, SOC_PHY_DDRC16_ENABLEf, 1));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_READ_FIFO_CTRLr(unit, ndx, read_fifo_ctrl_data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
/*A03*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A03. Configure input shift control\n")));
        data = 0;
        switch(dramType)
        {
            case SHMOO_COMBO16_DRAM_TYPE_DDR4:
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_INPUT_SHIFT_CTRLr, &data, SOC_PHY_DDRC16_DATAPATH_SHIFT_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_INPUT_SHIFT_CTRLr, &data, SOC_PHY_DDRC16_DATAPATH_ADDITIONAL_LATENCYf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_INPUT_SHIFT_CTRLr, &data, SOC_PHY_DDRC16_ADDRPATH_SHIFT_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_INPUT_SHIFT_CTRLr, &data, SOC_PHY_DDRC16_ADDRPATH_ADDITIONAL_LATENCYf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_INPUT_SHIFT_CTRLr, &data, SOC_PHY_DDRC16_RCMD_SHIFT_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_INPUT_SHIFT_CTRLr, &data, SOC_PHY_DDRC16_RCMD_ADDITIONAL_LATENCYf, 3));
                break;
            case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_INPUT_SHIFT_CTRLr, &data, SOC_PHY_DDRC16_DATAPATH_SHIFT_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_INPUT_SHIFT_CTRLr, &data, SOC_PHY_DDRC16_DATAPATH_ADDITIONAL_LATENCYf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_INPUT_SHIFT_CTRLr, &data, SOC_PHY_DDRC16_ADDRPATH_SHIFT_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_INPUT_SHIFT_CTRLr, &data, SOC_PHY_DDRC16_ADDRPATH_ADDITIONAL_LATENCYf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_INPUT_SHIFT_CTRLr, &data, SOC_PHY_DDRC16_RCMD_SHIFT_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_INPUT_SHIFT_CTRLr, &data, SOC_PHY_DDRC16_RCMD_ADDITIONAL_LATENCYf, 18));
                break;
            default:
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type: %02d\n"), dramType));
                return SOC_E_FAIL;
        }
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_INPUT_SHIFT_CTRLr(unit, ndx, data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
/*A04*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A04. Configure power regulation\n")));
        data = 0;
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_LDO_CONFIGr, &data, SOC_PHY_DDRC16_PWRDNf, 1));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_COMMON_LDO_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_LDO_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_LDO_R_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_LDO_W_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_LDO_R_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_LDO_W_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_LDO_R_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_LDO_W_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_LDO_R_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_LDO_W_CONFIGr(unit, ndx, data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_LDO_CONFIGr, &data, SOC_PHY_DDRC16_BIAS_CTRLf, 0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_LDO_CONFIGr, &data, SOC_PHY_DDRC16_BYPASSf, 0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_LDO_CONFIGr, &data, SOC_PHY_DDRC16_CTRLf, 0x06));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_LDO_CONFIGr, &data, SOC_PHY_DDRC16_REF_EXTf, 1));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_LDO_CONFIGr, &data, SOC_PHY_DDRC16_REF_SEL_EXTf, 0));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_COMMON_LDO_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_LDO_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_LDO_R_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_LDO_W_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_LDO_R_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_LDO_W_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_LDO_R_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_LDO_W_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_LDO_R_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_LDO_W_CONFIGr(unit, ndx, data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_LDO_CONFIGr, &data, SOC_PHY_DDRC16_PWRDNf, 0));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_COMMON_LDO_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_LDO_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_LDO_R_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_LDO_W_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_LDO_R_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_LDO_W_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_LDO_R_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_LDO_W_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_LDO_R_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_LDO_W_CONFIGr(unit, ndx, data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
/*A05*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A05. Configure reference voltage\n")));
        data = 0;
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr, &data, SOC_PHY_DDRC16_CTRLf, 0x0001));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_VREF_DAC_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_VREF_DAC_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_VREF_DAC_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_VREF_DAC_CONFIGr(unit, ndx, data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr, &data, SOC_PHY_DDRC16_DATAf, 0x92));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_VREF_DAC_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_VREF_DAC_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_VREF_DAC_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_VREF_DAC_CONFIGr(unit, ndx, data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr, &data, SOC_PHY_DDRC16_CTRLf, 0x0000));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_VREF_DAC_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_VREF_DAC_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_VREF_DAC_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_VREF_DAC_CONFIGr(unit, ndx, data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
/*A06*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A06. Release VDL resets\n")));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_VDL_1G_RESET_Nf, 1));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, ndx, reset_ctrl_data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
/*A07*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A07. Compute VDL step size\n")));
        SOC_IF_ERROR_RETURN(soc_combo16_calculate_step_size(unit, ndx, &ss));
        
        step1000 = ss.step1000;
        size1000UI = ss.size1000UI;
        LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "     VDL step size........: %3d.%03d ps\n"), (step1000 / 1000), (step1000 % 1000)));
        LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "     UI size..............: %3d.%03d steps\n"), (size1000UI / 1000), (size1000UI % 1000)));
        
/*A08*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A08. Configure read clock\n")));
        data = 0;
        switch(dramType)
        {
            case SHMOO_COMBO16_DRAM_TYPE_DDR4:
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_READ_CLOCK_CONFIGr, &data, SOC_PHY_DDRC16_FREE_RUNNING_MODEf, 0));
                break;
            case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_READ_CLOCK_CONFIGr, &data, SOC_PHY_DDRC16_FREE_RUNNING_MODEf, 1));
                break;
            /* must default. Otherwise - compilation error */
            /* coverity[dead_error_begin:FALSE] */
            default:
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type: %02d\n"), dramType));
                return SOC_E_FAIL;
        }
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_READ_CLOCK_CONFIGr(unit, ndx, data));
        
        if(dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
        {
            data = 0;
            setting = size1000UI / 1000;                                                        /* 50% placement on 2G clock */
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_READ_MAX_VDL_FSMr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, setting));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_DDR4_READ_MAX_VDL_FSMr(unit, ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_DDR4_READ_MAX_VDL_FSMr(unit, ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_DDR4_READ_MAX_VDL_FSMr(unit, ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_DDR4_READ_MAX_VDL_FSMr(unit, ndx, data));
            
            data = 0;
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_MINUS_3f, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_MINUS_2f, 0));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_MINUS_1f, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_0f, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_PLUS_1f, 1));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr, &data, SOC_PHY_DDRC16_A_PLUS_2f, 0));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_DDR4_REN_STRETCH_CONFIGr(unit, ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_DDR4_REN_STRETCH_CONFIGr(unit, ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_DDR4_REN_STRETCH_CONFIGr(unit, ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_DDR4_REN_STRETCH_CONFIGr(unit, ndx, data));
        }
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
/*A09*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A09. Configure UI shifts\n")));
        switch(dramType)
        {
            case SHMOO_COMBO16_DRAM_TYPE_DDR4:
                ck_ui = SHMOO_COMBO16_DDR4_INIT_CK_UI_SHIFT;
                addr_ctrl_ui = SHMOO_COMBO16_DDR4_INIT_ADDR_CTRL_UI_SHIFT;
                dqs_ui = SHMOO_COMBO16_DDR4_INIT_DQS_UI_SHIFT;
                data_ui = SHMOO_COMBO16_DDR4_INIT_DATA_UI_SHIFT;
                break;
            case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
                ck_ui = SHMOO_COMBO16_GDDR5_INIT_CK_UI_SHIFT;
                addr_ctrl_ui = SHMOO_COMBO16_GDDR5_INIT_ADDR_CTRL_UI_SHIFT;
                dqs_ui = SHMOO_COMBO16_GDDR5_INIT_DQS_UI_SHIFT;
                data_ui = SHMOO_COMBO16_GDDR5_INIT_DATA_UI_SHIFT;
                break;
            /* must default. Otherwise - compilation error */
            /* coverity[dead_error_begin:FALSE] */
            default:
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type: %02d\n"), dramType));
                return SOC_E_FAIL;
        }
        
        data = 0;
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_MAX_VDL_CKr, &data, SOC_PHY_DDRC16_UI_SHIFTf, ck_ui));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_COMMON_MAX_VDL_CKr(unit, ndx, data));
        
        data = 0;
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, &data, SOC_PHY_DDRC16_UI_SHIFTf, addr_ctrl_ui));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_ADDRr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_CTRLr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_CTRLr(unit, ndx, data));
        
        data = 0;
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_UI_SHIFTf, dqs_ui));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DQSr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DQSr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DQSr(unit, ndx, data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        data = 0;
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DATAr, &data, SOC_PHY_DDRC16_UI_SHIFTf, data_ui));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DATAr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DATAr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DATAr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DATAr(unit, ndx, data));
        
/*A10*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A10. Configure VDLs\n")));
        if(dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
        {
            data = SHMOO_COMBO16_DDR4_INIT_WRITE_MIN_VDL_POS;
        }
        else
        {
            data = 0;
        }
        
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT0r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT1r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT2r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT3r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT4r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT5r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT6r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_BIT7r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_DBIr(unit, ndx, data));
    /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MIN_VDL_EDCr(unit, ndx, data)); */
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT0r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT1r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT2r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT3r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT4r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT5r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT6r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_BIT7r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_DBIr(unit, ndx, data));
    /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MIN_VDL_EDCr(unit, ndx, data)); */
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT0r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT1r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT2r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT3r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT4r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT5r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT6r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_BIT7r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_DBIr(unit, ndx, data));
    /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MIN_VDL_EDCr(unit, ndx, data)); */
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT0r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT1r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT2r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT3r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT4r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT5r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT6r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_BIT7r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_DBIr(unit, ndx, data));
    /*    SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MIN_VDL_EDCr(unit, ndx, data)); */
        
        if(dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
        {
            data = SHMOO_COMBO16_DDR4_INIT_READ_MIN_VDL_POS;
        }
        else
        {
            data = 0;
        }
        
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT0r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT1r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT2r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT3r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT4r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT5r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT6r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_BIT7r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_DBIr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MIN_VDL_EDCr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT0r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT1r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT2r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT3r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT4r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT5r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT6r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_BIT7r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_DBIr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MIN_VDL_EDCr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT0r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT1r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT2r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT3r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT4r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT5r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT6r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_BIT7r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_DBIr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MIN_VDL_EDCr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT0r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT1r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT2r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT3r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT4r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT5r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT6r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_BIT7r(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_DBIr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MIN_VDL_EDCr(unit, ndx, data));
        
        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, ndx, &data));
        if(dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
        {
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, 0));
        }
        else
        {
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_WRITE_MAX_VDL_DQSr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, SHMOO_COMBO16_MAX_VDL_LENGTH - 1));
        }
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_WRITE_MAX_VDL_DQSr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_WRITE_MAX_VDL_DQSr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_WRITE_MAX_VDL_DQSr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_WRITE_MAX_VDL_DQSr(unit, ndx, data));
        
        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, 0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_MASTER_MODEf, 1));            
        if(dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)
        {
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 1));
            
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, ndx, data));
            
            sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
            
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr, &data, SOC_PHY_DDRC16_FORCE_UPDATEf, 0));
        }
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_READ_MAX_VDL_DQS_MASTER_CTRLr(unit, ndx, data));
        
        if(dramType == SHMOO_COMBO16_DRAM_TYPE_DDR4)                                            /* ADDR/CTRL should be in a working position */
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, ndx, &data));
            setting = size1000UI / 2500;                                                        /* 40% adjustment */
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_L_MAX_VDL_ADDRr, &data, SOC_PHY_DDRC16_MAX_VDL_STEPf, setting));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_ADDRr(unit, ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_ADDRr(unit, ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_L_MAX_VDL_CTRLr(unit, ndx, data));
            SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_U_MAX_VDL_CTRLr(unit, ndx, data));
        }
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
/*A11*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A11. ZQ calibration\n")));
        if(shmoo_dram_info->sim_system_mode)
        {
            LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "     Skipped for emulation\n")));
            goto SHMOO_COMBO16_ZQ_CALIBRATION_END;
        }

        if(shmoo_dram_info->zq_cal_array[ndx] != ndx)
        {
            LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "     Copying ZQ calibration results from PHY %02d\n"), shmoo_dram_info->zq_cal_array[ndx]));
            error = 0;
            status = 0;
            goto SHMOO_COMBO16_ZQ_CALIBRATION_SLAVE_READ;
        }

        SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr(unit, ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr, data, SOC_PHY_DDRC16_DATAf, &setting));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr, &data, SOC_PHY_DDRC16_DATAf, 0x42));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr(unit, ndx, data));
        sal_usleep(SHMOO_COMBO16_LONG_SLEEP);
        
        SOC_IF_ERROR_RETURN(READ_DDRC16_COMMON_RESCAL_INIT_CONFIGr(unit, ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_RESCAL_INIT_CONFIGr, &data, SOC_PHY_DDRC16_COMP_INIT_FDEPTHf, 0xB));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_RESCAL_INIT_CONFIGr, &data, SOC_PHY_DDRC16_PNCOMP_INIT_DIFFf, 0x1F));     /* TEMPORARY */
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_COMMON_RESCAL_INIT_CONFIGr(unit, ndx, data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_RESCAL_RESET_Nf, 1));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, ndx, reset_ctrl_data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        SOC_IF_ERROR_RETURN(READ_DDRC16_COMMON_RESCAL_OPERATION_CONFIGr(unit, ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_RESCAL_OPERATION_CONFIGr, &data, SOC_PHY_DDRC16_COMP_ENf, 1));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_COMMON_RESCAL_OPERATION_CONFIGr(unit, ndx, data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        error = 0;
        status = 0;
        soc_timeout_init(&to, to_val, 0);
        do
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_COMMON_STATUS_RESCALr(unit, ndx, &data));
            SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_COMMON_STATUS_RESCALr, data, SOC_PHY_DDRC16_COMP_DONEf, &fld_comp_done));
            if(fld_comp_done)
            {
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_COMMON_STATUS_RESCALr, data, SOC_PHY_DDRC16_COMP_ERRORf, &status));
                error = status & 0x1C;
                if(error)
                {
                    LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "     WARNING: ZQ calibration error - Manual IO programming may be required for correct operation\n")));
                    error |= 0x10020;
                }
                break;
            }
            if (soc_timeout_check(&to))
            {
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "     WARNING: Timed out during ZQ calibration\n")));
                error |= 0x20000;
                break;
            }
        }
        while(TRUE);
        
        SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_COMMON_STATUS_RESCALr, data, SOC_PHY_DDRC16_PCOMP_CODE_2COREf, &temp1));
        SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_COMMON_STATUS_RESCALr, data, SOC_PHY_DDRC16_NCOMP_CODE_2COREf, &temp2));
        
        SOC_IF_ERROR_RETURN(READ_DDRC16_COMMON_RESCAL_OPERATION_CONFIGr(unit, ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_RESCAL_OPERATION_CONFIGr, &data, SOC_PHY_DDRC16_COMP_ENf, 0));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_COMMON_RESCAL_OPERATION_CONFIGr(unit, ndx, data));
        
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_RESCAL_RESET_Nf, 0));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, ndx, reset_ctrl_data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
        SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr(unit, ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr, &data, SOC_PHY_DDRC16_DATAf, setting));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_SHARED_VREF_DAC_CONFIGr(unit, ndx, data));
        
        SHMOO_COMBO16_ZQ_CALIBRATION_SLAVE_READ:

        if(shmoo_dram_info->zq_cal_array[ndx] != ndx)
        {
            SOC_IF_ERROR_RETURN(READ_DDRC16_COMMON_IO_CONFIGr(unit, (uint32) shmoo_dram_info->zq_cal_array[ndx], &data));
            SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_COMMON_IO_CONFIGr, data, SOC_PHY_DDRC16_PDf, &temp1));
            SOC_IF_ERROR_RETURN(soc_phy_ddrc16_reg_field_get(unit, SOC_PHY_DDRC16_REG_COMMON_IO_CONFIGr, data, SOC_PHY_DDRC16_NDf, &temp2));
        }

        dq_pd = temp1;
        dq_nd = temp2;
        dq_pterm = temp1;
        dq_nterm = temp2;
        aq_pd = temp1;
        aq_nd = temp2;
        aq_pterm = temp1;
        aq_nterm = temp2;
        
        if((dq_pd | dq_nd | dq_pterm | dq_nterm | aq_pd | aq_pterm | aq_nterm) & 0xFFFFFFE0)
        {
            LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "     WARNING: ZQ calibration out of bounds - Manual IO programming may be required for correct operation\n")));
            error |= 0x40000;
        }
        
        if(error)
        {
            LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              Status Code......................: 0x%02X\n"), status));
            LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              DQ/DQS/CK Pdrive.................: 0x%02X\n"), 0xFF & dq_pd));
            LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              DQ/DQS/CK Ndrive.................: 0x%02X\n"), 0xFF & dq_nd));
            LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              DQ/DQS/CK Ptermination...........: 0x%02X\n"), 0xFF & dq_pterm));
            LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              DQ/DQS/CK Ntermination (RFU).....: 0x%02X\n"), 0xFF & dq_nterm));
            LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              AQ Pdrive........................: 0x%02X\n"), 0xFF & aq_pd));
            LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              AQ Ndrive........................: 0x%02X\n"), 0xFF & aq_nd));
            LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              AQ Ptermination..................: 0x%02X\n"), 0xFF & aq_pterm));
            LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              AQ Ntermination (RFU)............: 0x%02X\n"), 0xFF & aq_nterm));
            
            if(error & 0x40000)
            {
                dq_pd &= 0x1F;
                dq_nd &= 0x1F;
                dq_pterm &= 0x1F;
                dq_nterm &= 0x1F;
                aq_pd &= 0x1F;
                aq_nd &= 0x1F;
                aq_pterm &= 0x1F;
                aq_nterm &= 0x1F;
            }
            
            /* return SOC_E_FAIL; */
        }
        else
        {
            LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              Status Code......................: 0x%02X\n"), status));
            LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              DQ/DQS/CK Pdrive.................: 0x%02X\n"), 0xFF & dq_pd));
            LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              DQ/DQS/CK Ndrive.................: 0x%02X\n"), 0xFF & dq_nd));
            LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              DQ/DQS/CK Ptermination...........: 0x%02X\n"), 0xFF & dq_pterm));
            LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              DQ/DQS/CK Ntermination (RFU).....: 0x%02X\n"), 0xFF & dq_nterm));
            LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              AQ Pdrive........................: 0x%02X\n"), 0xFF & aq_pd));
            LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              AQ Ndrive........................: 0x%02X\n"), 0xFF & aq_nd));
            LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              AQ Ptermination..................: 0x%02X\n"), 0xFF & aq_pterm));
            LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "              AQ Ntermination (RFU)............: 0x%02X\n"), 0xFF & aq_nterm));
        }
        
        SOC_IF_ERROR_RETURN(READ_DDRC16_COMMON_IO_CONFIGr(unit, ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_IO_CONFIGr, &data, SOC_PHY_DDRC16_PDf, dq_pd));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_IO_CONFIGr, &data, SOC_PHY_DDRC16_PTERMf, dq_pterm));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_IO_CONFIGr, &data, SOC_PHY_DDRC16_NDf, dq_nd));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_IO_CONFIGr, &data, SOC_PHY_DDRC16_NTERMf, dq_nterm));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_IO_CONFIGr, &data, SOC_PHY_DDRC16_PULL_UP_OFF_Bf, 0x0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_IO_CONFIGr, &data, SOC_PHY_DDRC16_RX_BIASf, 0x0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_IO_CONFIGr, &data, SOC_PHY_DDRC16_PEAKf, 0x0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_IO_CONFIGr, &data, SOC_PHY_DDRC16_DEMf, 0x0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_IO_CONFIGr, &data, SOC_PHY_DDRC16_RXENBf, 0x1));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_COMMON_IO_CONFIGr(unit, ndx, data));

        SOC_IF_ERROR_RETURN(READ_DDRC16_AQ_IO_CONFIGr(unit, ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_IO_CONFIGr, &data, SOC_PHY_DDRC16_PDf, aq_pd));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_IO_CONFIGr, &data, SOC_PHY_DDRC16_PTERMf, aq_pterm));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_IO_CONFIGr, &data, SOC_PHY_DDRC16_NDf, aq_nd));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_IO_CONFIGr, &data, SOC_PHY_DDRC16_NTERMf, aq_nterm));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_IO_CONFIGr, &data, SOC_PHY_DDRC16_PULL_UP_OFF_Bf, 0x0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_IO_CONFIGr, &data, SOC_PHY_DDRC16_RX_BIASf, 0x0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_IO_CONFIGr, &data, SOC_PHY_DDRC16_PEAKf, 0x0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_IO_CONFIGr, &data, SOC_PHY_DDRC16_DEMf, 0x0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_IO_CONFIGr, &data, SOC_PHY_DDRC16_RXENBf, 0x1));
        switch(dramType)
        {
            case SHMOO_COMBO16_DRAM_TYPE_DDR4:
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_IO_CONFIGr, &data, SOC_PHY_DDRC16_RXENB_ALERT_Nf, 0x0));
                break;
            case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_AQ_IO_CONFIGr, &data, SOC_PHY_DDRC16_RXENB_ALERT_Nf, 0x1));
                break;
            /* must default. Otherwise - compilation error */
            /* coverity[dead_error_begin:FALSE] */
            default:
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type: %02d\n"), dramType));
                return SOC_E_FAIL;
        }
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_AQ_IO_CONFIGr(unit, ndx, data));

        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_DATA_IO_CONFIGr(unit, ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DATA_IO_CONFIGr, &data, SOC_PHY_DDRC16_PDf, dq_pd));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DATA_IO_CONFIGr, &data, SOC_PHY_DDRC16_PTERMf, dq_pterm));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DATA_IO_CONFIGr, &data, SOC_PHY_DDRC16_NDf, dq_nd));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DATA_IO_CONFIGr, &data, SOC_PHY_DDRC16_NTERMf, dq_nterm));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DATA_IO_CONFIGr, &data, SOC_PHY_DDRC16_PULL_UP_OFF_Bf, 0x0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DATA_IO_CONFIGr, &data, SOC_PHY_DDRC16_RX_BIASf, 0x0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DATA_IO_CONFIGr, &data, SOC_PHY_DDRC16_RXENBf, 0x0));
        switch(dramType)
        {
            case SHMOO_COMBO16_DRAM_TYPE_DDR4:
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DATA_IO_CONFIGr, &data, SOC_PHY_DDRC16_PEAKf, 0x0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DATA_IO_CONFIGr, &data, SOC_PHY_DDRC16_DEMf, 0x0));
                break;
            case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DATA_IO_CONFIGr, &data, SOC_PHY_DDRC16_PEAKf, 0x2));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DATA_IO_CONFIGr, &data, SOC_PHY_DDRC16_DEMf, 0x2));
                break;
            /* must default. Otherwise - compilation error */
            /* coverity[dead_error_begin:FALSE] */
            default:
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type: %02d\n"), dramType));
                return SOC_E_FAIL;
        }
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_DATA_IO_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_DATA_IO_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_DATA_IO_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_DATA_IO_CONFIGr(unit, ndx, data));

        SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_DQS_IO_CONFIGr(unit, ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DQS_IO_CONFIGr, &data, SOC_PHY_DDRC16_PDf, dq_pd));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DQS_IO_CONFIGr, &data, SOC_PHY_DDRC16_PTERMf, dq_pterm));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DQS_IO_CONFIGr, &data, SOC_PHY_DDRC16_NDf, dq_nd));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DQS_IO_CONFIGr, &data, SOC_PHY_DDRC16_NTERMf, dq_nterm));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DQS_IO_CONFIGr, &data, SOC_PHY_DDRC16_PULL_UP_OFF_Bf, 0x0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DQS_IO_CONFIGr, &data, SOC_PHY_DDRC16_RX_BIASf, 0x0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DQS_IO_CONFIGr, &data, SOC_PHY_DDRC16_PEAKf, 0x0));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DQS_IO_CONFIGr, &data, SOC_PHY_DDRC16_DEMf, 0x0));
        switch(dramType)
        {
            case SHMOO_COMBO16_DRAM_TYPE_DDR4:
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DQS_IO_CONFIGr, &data, SOC_PHY_DDRC16_RXENBf, 0x0));
                break;
            case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DQS_IO_CONFIGr, &data, SOC_PHY_DDRC16_RXENBf, 0x1));
                break;
            /* must default. Otherwise - compilation error */
            /* coverity[dead_error_begin:FALSE] */
            default:
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type: %02d\n"), dramType));
                return SOC_E_FAIL;
        }
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_DQS_IO_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_DQS_IO_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_DQS_IO_CONFIGr(unit, ndx, data));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_DQS_IO_CONFIGr(unit, ndx, data));
        sal_usleep(SHMOO_COMBO16_LONG_SLEEP);
        
        SHMOO_COMBO16_ZQ_CALIBRATION_END:
        
/*A12*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A12. Enable CK\n")));
        SOC_IF_ERROR_RETURN(READ_DDRC16_COMMON_CK_CONFIGr(unit, ndx, &data));
        SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_COMMON_CK_CONFIGr, &data, SOC_PHY_DDRC16_CK_ENABLEf, 1));
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_COMMON_CK_CONFIGr(unit, ndx, data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
/*A13*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A13. Configure FIFOs\n")));
        switch(dramType)
        {
            case SHMOO_COMBO16_DRAM_TYPE_DDR4:
                data = 0;
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_RD2_2G_SELECTf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_RD2_2G_SELECTf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_RD2_2G_SELECTf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_RD2_2G_SELECTf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, ndx, data));
                
                data = 0;
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_DDR4_REN_FIFO_CONFIGr, &data, SOC_PHY_DDRC16_RD2_2G_DELAYf, SHMOO_COMBO16_DDR4_INIT_REN_RD2_2G_DELAY));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_DDR4_REN_FIFO_CONFIGr(unit, ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_DDR4_REN_FIFO_CONFIGr(unit, ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_DDR4_REN_FIFO_CONFIGr(unit, ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_DDR4_REN_FIFO_CONFIGr(unit, ndx, data));
                break;
            case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
                data = 0;
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr, &data, SOC_PHY_DDRC16_RDATA_WR2RD_DELAYf, 4));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_GDDR5_READ_ALIGNMENT_CTRLr(unit, ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_GDDR5_READ_ALIGNMENT_CTRLr(unit, ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_GDDR5_READ_ALIGNMENT_CTRLr(unit, ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_GDDR5_READ_ALIGNMENT_CTRLr(unit, ndx, data));
                
                data = 0;
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_FSM_CTRLr, &data, SOC_PHY_DDRC16_SYSTEM_ENABLEf, 0));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_FSM_CTRLr, &data, SOC_PHY_DDRC16_DATA_VALID_GEN_ENABLEf, 0));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_GDDR5_READ_FSM_CTRLr(unit, ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_GDDR5_READ_FSM_CTRLr(unit, ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_GDDR5_READ_FSM_CTRLr(unit, ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_GDDR5_READ_FSM_CTRLr(unit, ndx, data));
                break;
            /* must default. Otherwise - compilation error */
            /* coverity[dead_error_begin:FALSE] */
            default:
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type: %02d\n"), dramType));
                return SOC_E_FAIL;
        }
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
/*A14*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A14. Release PHY resets\n")));
        switch(dramType)
        {
            case SHMOO_COMBO16_DRAM_TYPE_DDR4:
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_DDR4_GLUE_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &data, SOC_PHY_DDRC16_RCMD_FIFO_RESET_Nf, 1));
                break;
            case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_DRC_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_PHY_1G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_PHY_2G_RESET_Nf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_RESET_CTRLr, &reset_ctrl_data, SOC_PHY_DDRC16_CLOCK_DIV_RESET_Nf, 1));
                break;
            /* must default. Otherwise - compilation error */
            /* coverity[dead_error_begin:FALSE] */
            default:
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type: %02d\n"), dramType));
                return SOC_E_FAIL;
        }
        SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_RESET_CTRLr(unit, ndx, reset_ctrl_data));
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
/*A15*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A15. Enable FIFOs\n")));
        switch(dramType)
        {
            case SHMOO_COMBO16_DRAM_TYPE_DDR4:
                SOC_IF_ERROR_RETURN(READ_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE0_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE1_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE2_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr, &data, SOC_PHY_DDRC16_DQ_BYTE3_WRITE_ENABLEf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_CONTROL_REGS_DDR4_REN_FIFO_CENTRAL_INITIALIZERr(unit, ndx, data));
                break;
            case SHMOO_COMBO16_DRAM_TYPE_GDDR5:
                SOC_IF_ERROR_RETURN(READ_DDRC16_DQ_BYTE0_GDDR5_READ_FSM_CTRLr(unit, ndx, &data));
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_FSM_CTRLr, &data, SOC_PHY_DDRC16_DATA_VALID_GEN_ENABLEf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_GDDR5_READ_FSM_CTRLr(unit, ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_GDDR5_READ_FSM_CTRLr(unit, ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_GDDR5_READ_FSM_CTRLr(unit, ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_GDDR5_READ_FSM_CTRLr(unit, ndx, data));
                sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
                
                SOC_IF_ERROR_RETURN( soc_phy_ddrc16_reg_field_set(unit, SOC_PHY_DDRC16_REG_DQ_BYTE0_GDDR5_READ_FSM_CTRLr, &data, SOC_PHY_DDRC16_SYSTEM_ENABLEf, 1));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE0_GDDR5_READ_FSM_CTRLr(unit, ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE1_GDDR5_READ_FSM_CTRLr(unit, ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE2_GDDR5_READ_FSM_CTRLr(unit, ndx, data));
                SOC_IF_ERROR_RETURN(WRITE_DDRC16_DQ_BYTE3_GDDR5_READ_FSM_CTRLr(unit, ndx, data));
                break;
            /* must default. Otherwise - compilation error */
            /* coverity[dead_error_begin:FALSE] */
            default:
                LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Unsupported dram type: %02d\n"), dramType));
                return SOC_E_FAIL;
        }
        sal_usleep(SHMOO_COMBO16_SHORT_SLEEP);
        
/*A16*/ LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A16. Configure CDR default values\n")));
        /* TO BE CODED */
        
        LOG_VERBOSE(BSL_LS_SOC_DDR, (BSL_META_U(unit, "A Series - PHY Initialization complete (DRC index: %02d)\n"), ndx));
    }
    
    return SOC_E_NONE;
}

/* Register Device interface CB to shmoo */
int
soc_combo16_shmoo_interface_cb_register(int unit, combo16_shmoo_cbi_t shmoo_cbi)
{
    soc_combo16_phy_reg_read = shmoo_cbi.combo16_phy_reg_read;
    soc_combo16_phy_reg_write = shmoo_cbi.combo16_phy_reg_write;
    soc_combo16_phy_reg_modify = shmoo_cbi.combo16_phy_reg_modify;
    _shmoo_combo16_drc_bist_conf_set = shmoo_cbi.combo16_drc_bist_conf_set;
    _shmoo_combo16_drc_bist_err_cnt = shmoo_cbi.combo16_drc_bist_err_cnt;
    _shmoo_combo16_drc_dram_init = shmoo_cbi.combo16_drc_dram_init;
    _shmoo_combo16_drc_pll_set = shmoo_cbi.combo16_drc_pll_set;
    _shmoo_combo16_drc_modify_mrs = shmoo_cbi.combo16_drc_modify_mrs;
    _shmoo_combo16_drc_enable_adt = shmoo_cbi.combo16_drc_enable_adt;
    _shmoo_combo16_drc_enable_wck2ck_training = shmoo_cbi.combo16_drc_enable_wck2ck_training;
    _shmoo_combo16_drc_enable_write_leveling = shmoo_cbi.combo16_drc_enable_write_leveling;
    _shmoo_combo16_drc_mpr_en = shmoo_cbi.combo16_drc_mpr_en;
    _shmoo_combo16_drc_mpr_load = shmoo_cbi.combo16_drc_mpr_load;
    _shmoo_combo16_drc_enable_gddr5_training_protocol = shmoo_cbi.combo16_drc_enable_gddr5_training_protocol;
    _shmoo_combo16_drc_vendor_info_get = shmoo_cbi.combo16_drc_vendor_info_get;
    _shmoo_combo16_drc_dqs_pulse_gen = shmoo_cbi.combo16_drc_dqs_pulse_gen;
    _shmoo_combo16_drc_gddr5_bist_conf_set = shmoo_cbi.combo16_gddr5_shmoo_drc_bist_conf_set;
    _shmoo_combo16_drc_gddr5_bist_err_cnt = shmoo_cbi.combo16_gddr5_shmoo_drc_bist_err_cnt;
    _shmoo_combo16_drc_gddr5_dq_byte_pairs_swap_info_get = shmoo_cbi.combo16_gddr5_shmoo_drc_dq_byte_pairs_swap_info_get;
    _shmoo_combo16_drc_enable_wr_crc = shmoo_cbi.combo16_drc_enable_wr_crc;
    _shmoo_combo16_drc_enable_rd_crc = shmoo_cbi.combo16_drc_enable_rd_crc;
    _shmoo_combo16_drc_enable_wr_dbi = shmoo_cbi.combo16_drc_enable_wr_dbi;
    _shmoo_combo16_drc_enable_rd_dbi = shmoo_cbi.combo16_drc_enable_rd_dbi;
    _shmoo_combo16_drc_enable_refresh = shmoo_cbi.combo16_drc_enable_refresh;
    _shmoo_combo16_drc_force_dqs = shmoo_cbi.combo16_drc_force_dqs;
    _shmoo_combo16_drc_soft_reset_drc_without_dram = shmoo_cbi.combo16_drc_soft_reset_drc_without_dram;
    _shmoo_combo16_drc_dram_info_access = shmoo_cbi.combo16_shmoo_dram_info_access;
    _shmoo_combo16_drc_vendor_info_access = shmoo_cbi.combo16_vendor_info_access;

    if(!_shmoo_combo16_validate_cbi())
    {
        LOG_ERROR(BSL_LS_SOC_DDR, (BSL_META_U(unit, "Null callback function detected during CBI registration\n")));
        return SOC_E_FAIL;
    }
    
    return SOC_E_NONE;
}

/* BEGIN: CDR FUNCTIONS */
int
soc_combo16_cdr_ctl(int unit, int drc_ndx, int stat, combo16_cdr_config_param_t *config_param)
{
    /* TO BE CODED */
    
    return SOC_E_NONE;
}
/* END: CDR FUNCTIONS */

#undef _ERR_MSG_MODULE_NAME

#endif /* BCM_DDRC16_SUPPORT */

