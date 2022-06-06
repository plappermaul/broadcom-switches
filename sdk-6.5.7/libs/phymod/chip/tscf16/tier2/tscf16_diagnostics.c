/*
 *         
 * $Id: phymod.xml,v 1.1.2.5 Broadcom SDK $
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *         
 *     
 */

#include <phymod/phymod.h>
#include <phymod/phymod_diagnostics.h>
#include <phymod/phymod_diagnostics_dispatch.h>
#include <phymod/phymod_util.h>
#include "../../tscf16/tier1/tefmod16.h"
#include "../../tscf16/tier1/tefmod16_enum_defines.h"
#include "../../tscf16/tier1/tefmod16_device.h"
#include "../../falcon16/tier1/src/falcon16_cfg_seq.h"
#include "../../falcon16/tier1/include/falcon16_tsc_common.h"
#include "../../falcon16/tier1/include/falcon16_tsc_interface.h"
#include "../../falcon16/tier1/include/falcon16_tsc_internal.h"
#include "../../falcon16/tier1/include/falcon16_tsc_dependencies.h"
#include "../../falcon16/tier1/include/falcon16_tsc_types.h"
#include "../../falcon16/tier1/include/common/srds_api_enum.h"

#ifdef PHYMOD_TSCF16_SUPPORT

#define PATTERN_MAX_LENGTH 240


STATIC
int _tscf16_prbs_poly_phymod_to_falcon16(phymod_prbs_poly_t phymod_poly, enum srds_prbs_polynomial_enum *falcon16_poly)
{   
    switch(phymod_poly){
    case phymodPrbsPoly7:
        *falcon16_poly = PRBS_7;
        break;
    case phymodPrbsPoly9:
        *falcon16_poly = PRBS_9;
        break;
    case phymodPrbsPoly11:
        *falcon16_poly = PRBS_11;
        break;
    case phymodPrbsPoly15:
        *falcon16_poly = PRBS_15;
        break;
    case phymodPrbsPoly23:
        *falcon16_poly = PRBS_23;
        break;
    case phymodPrbsPoly31:
        *falcon16_poly = PRBS_31;
        break;
    case phymodPrbsPoly58:
        *falcon16_poly = PRBS_58;
        break;
    default:
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("unsupported poly for tscf %u"), phymod_poly));
    }
    return PHYMOD_E_NONE;
}

STATIC
int _tscf16_prbs_poly_tscf16_to_phymod(falcon16_prbs_polynomial_type_t tscf_poly, phymod_prbs_poly_t *phymod_poly)
{
    switch(tscf_poly){
    case FALCON16_PRBS_POLYNOMIAL_7:
        *phymod_poly = phymodPrbsPoly7;
        break;
    case FALCON16_PRBS_POLYNOMIAL_9:
        *phymod_poly = phymodPrbsPoly9;
        break;
    case FALCON16_PRBS_POLYNOMIAL_11:
        *phymod_poly = phymodPrbsPoly11;
        break;
    case FALCON16_PRBS_POLYNOMIAL_15:
        *phymod_poly = phymodPrbsPoly15;
        break;
    case FALCON16_PRBS_POLYNOMIAL_23:
        *phymod_poly = phymodPrbsPoly23;
        break;
    case FALCON16_PRBS_POLYNOMIAL_31:
        *phymod_poly = phymodPrbsPoly31;
        break;
    case FALCON16_PRBS_POLYNOMIAL_58:
        *phymod_poly = phymodPrbsPoly58;
        break;
    default:
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_INTERNAL, (_PHYMOD_MSG("uknown poly %u"), tscf_poly));
    }
    return PHYMOD_E_NONE;
}

#ifdef PHYMOD_TO_TSCE16_OS_MODE_TRANS
STATIC
int _tscf16_os_mode_phymod_to_tscf16(phymod_osr_mode_t phymod_os_mode, tefmod16_os_mode_type *tscf_os_mode)
{
    switch(phymod_os_mode){
    case phymodOversampleMode1:
        *tscf_os_mode = TEFMOD16_PMA_OS_MODE_1;
        break;
    case phymodOversampleMode2:
        *tscf_os_mode = TEFMOD16_PMA_OS_MODE_2;
        break;
    /*case phymodOversampleMode3:
        *tscf_os_mode = TEFMOD16_PMA_OS_MODE_3;
        break;
    case phymodOversampleMode3P3:
        *tscf_os_mode = TEFMOD16_PMA_OS_MODE_3_3;
        break;*/
    case phymodOversampleMode4:
        *tscf_os_mode = TEFMOD16_PMA_OS_MODE_4;
        break;
    /*case phymodOversampleMode5:
        *tscf_os_mode = TEFMOD16_PMA_OS_MODE_5;
        break;
    case phymodOversampleMode8:
        *tscf_os_mode = TEFMOD16_PMA_OS_MODE_8;
        break;
    case phymodOversampleMode8P25:
        *tscf_os_mode = TEFMOD16_PMA_OS_MODE_8_25;
        break;
    case phymodOversampleMode10:
        *tscf_os_mode = TEFMOD16_PMA_OS_MODE_10;
        break;*/
    default:
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("unsupported over sample mode for tscf16 %u"), phymod_os_mode));
    }
    return PHYMOD_E_NONE;
}

TATIC
int _tscf16_os_mode_tscf16_to_phymod(tefmod16_os_mode_type tscf_os_mode, phymod_osr_mode_t *phymod_os_mode)
{
    switch(tscf_os_mode){
    case TEFMOD16_PMA_OS_MODE_1:
        *phymod_os_mode = phymodOversampleMode1;
        break;
    case TEFMOD16_PMA_OS_MODE_2:
        *phymod_os_mode = phymodOversampleMode2;
        break;
    /*case TEFMOD16_PMA_OS_MODE_3:
        *phymod_os_mode = phymodOversampleMode3;
        break;
    case TEFMOD16_PMA_OS_MODE_3_3:
        *phymod_os_mode = phymodOversampleMode3P3;
        break;*/
    case TEFMOD16_PMA_OS_MODE_4:
        *phymod_os_mode = phymodOversampleMode4;
        break;
    /*case TEFMOD16_PMA_OS_MODE_5:
        *phymod_os_mode = phymodOversampleMode5;
        break;
    case TEFMOD16_PMA_OS_MODE_8:
        *phymod_os_mode = phymodOversampleMode8;
        break;
    case TEFMOD16_PMA_OS_MODE_8_25:
        *phymod_os_mode = phymodOversampleMode8P25;
        break;
    case TEFMOD16_PMA_OS_MODE_10:
        *phymod_os_mode = phymodOversampleMode10;
        break;*/
    default:
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_INTERNAL, (_PHYMOD_MSG("uknown os mode %u"), tscf_os_mode));
    }
    return PHYMOD_E_NONE;
}
#endif

int tscf16_phy_rx_slicer_position_set(const phymod_phy_access_t* phy, uint32_t flags, const phymod_slicer_position_t* position)
{
        
    
    /* Place your code here */

        
    return PHYMOD_E_NONE;
    
}

int tscf16_phy_rx_slicer_position_get(const phymod_phy_access_t* phy, uint32_t flags, phymod_slicer_position_t* position)
{
        
    
    /* Place your code here */

        
    return PHYMOD_E_NONE;
    
}


int tscf16_phy_rx_slicer_position_max_get(const phymod_phy_access_t* phy, uint32_t flags, const phymod_slicer_position_t* position_min, const phymod_slicer_position_t* position_max)
{
        
    
    /* Place your code here */

        
    return PHYMOD_E_NONE;
    
}


int tscf16_phy_prbs_config_set(const phymod_phy_access_t* phy, uint32_t flags , const phymod_prbs_t* prbs)
{
    enum  srds_prbs_polynomial_enum falcon16_poly;
    int i, start_lane, num_lane;
    phymod_phy_access_t phy_copy;
    PHYMOD_IF_ERR_RETURN(_tscf16_prbs_poly_phymod_to_falcon16(prbs->poly, &falcon16_poly));

    PHYMOD_MEMCPY(&phy_copy, phy, sizeof(phy_copy));

    /* next figure out the lane num and start_lane based on the input */
    PHYMOD_IF_ERR_RETURN
        (phymod_util_lane_config_get(&phy->access, &start_lane, &num_lane));

    /*first check which direction */
    if (PHYMOD_PRBS_DIRECTION_RX_GET(flags)) {
        for (i = 0; i < num_lane; i++) {
            if (!PHYMOD_LANEPBMP_MEMBER(phy->access.lane_mask, start_lane + i)) {
                continue;
            }
            phy_copy.access.lane_mask = 0x1 << (i + start_lane);
            PHYMOD_IF_ERR_RETURN
                    (falcon16_tsc_config_rx_prbs(&phy_copy.access, falcon16_poly, PRBS_INITIAL_SEED_HYSTERESIS,  prbs->invert));
        }
    } else if (PHYMOD_PRBS_DIRECTION_TX_GET(flags)) {
        for (i = 0; i < num_lane; i++) {
            phy_copy.access.lane_mask = 0x1 << (i + start_lane);
            if (!PHYMOD_LANEPBMP_MEMBER(phy->access.lane_mask, start_lane + i)) {
                continue;
            }
            PHYMOD_IF_ERR_RETURN
                    (falcon16_tsc_config_tx_prbs(&phy_copy.access, falcon16_poly, prbs->invert));
        }
    } else {
        for (i = 0; i < num_lane; i++) {
            phy_copy.access.lane_mask = 0x1 << (i + start_lane);
            if (!PHYMOD_LANEPBMP_MEMBER(phy->access.lane_mask, start_lane + i)) {
                continue;
            }
            PHYMOD_IF_ERR_RETURN
                    (falcon16_tsc_config_rx_prbs(&phy_copy.access, falcon16_poly, PRBS_INITIAL_SEED_HYSTERESIS,  prbs->invert));
            PHYMOD_IF_ERR_RETURN
                    (falcon16_tsc_config_tx_prbs(&phy_copy.access, falcon16_poly, prbs->invert));
        }
    }  

    return PHYMOD_E_NONE;
    
}

int tscf16_phy_prbs_config_get(const phymod_phy_access_t* phy, uint32_t flags , phymod_prbs_t* prbs)
{
    phymod_prbs_t config_tmp;
    falcon16_prbs_polynomial_type_t tscf_poly;

    if (PHYMOD_PRBS_DIRECTION_TX_GET(flags)) {
        PHYMOD_IF_ERR_RETURN(falcon16_prbs_tx_inv_data_get(&phy->access, &config_tmp.invert));
        PHYMOD_IF_ERR_RETURN(falcon16_prbs_tx_poly_get(&phy->access, &tscf_poly));
        PHYMOD_IF_ERR_RETURN(_tscf16_prbs_poly_tscf16_to_phymod(tscf_poly, &config_tmp.poly));
        prbs->invert = config_tmp.invert;
        prbs->poly = config_tmp.poly;
    } else if (PHYMOD_PRBS_DIRECTION_RX_GET(flags)) {
        PHYMOD_IF_ERR_RETURN(falcon16_prbs_rx_inv_data_get(&phy->access, &config_tmp.invert));
        PHYMOD_IF_ERR_RETURN(falcon16_prbs_rx_poly_get(&phy->access, &tscf_poly));
        PHYMOD_IF_ERR_RETURN(_tscf16_prbs_poly_tscf16_to_phymod(tscf_poly, &config_tmp.poly));
        prbs->invert = config_tmp.invert;
        prbs->poly = config_tmp.poly;
    } else {
        PHYMOD_IF_ERR_RETURN(falcon16_prbs_tx_inv_data_get(&phy->access, &config_tmp.invert));
        PHYMOD_IF_ERR_RETURN(falcon16_prbs_tx_poly_get(&phy->access, &tscf_poly));
        PHYMOD_IF_ERR_RETURN(_tscf16_prbs_poly_tscf16_to_phymod(tscf_poly, &config_tmp.poly));
        prbs->invert = config_tmp.invert;
        prbs->poly = config_tmp.poly;
    }
        
    return PHYMOD_E_NONE;
    
}


int tscf16_phy_prbs_enable_set(const phymod_phy_access_t* phy, uint32_t flags , uint32_t enable)
{
    int i, start_lane, num_lane;
    phymod_phy_access_t phy_copy;

    PHYMOD_MEMCPY(&phy_copy, phy, sizeof(phy_copy));
    /* next figure out the lane num and start_lane based on the input */
    PHYMOD_IF_ERR_RETURN
        (phymod_util_lane_config_get(&phy->access, &start_lane, &num_lane));

    if (PHYMOD_PRBS_DIRECTION_TX_GET(flags)) {
        for (i = 0; i < num_lane; i++) {
            if (!PHYMOD_LANEPBMP_MEMBER(phy->access.lane_mask, start_lane + i)) {
                continue;
            }
            phy_copy.access.lane_mask = 0x1 << (i + start_lane);
            PHYMOD_IF_ERR_RETURN(falcon16_tsc_tx_prbs_en(&phy_copy.access, enable));
        }
    } else if (PHYMOD_PRBS_DIRECTION_RX_GET(flags)) {
        for (i = 0; i < num_lane; i++) {
            if (!PHYMOD_LANEPBMP_MEMBER(phy->access.lane_mask, start_lane + i)) {
                continue;
            }
            phy_copy.access.lane_mask = 0x1 << (i + start_lane);
            PHYMOD_IF_ERR_RETURN(falcon16_tsc_rx_prbs_en(&phy_copy.access, enable));
        }
    } else {
        for (i = 0; i < num_lane; i++) {
            if (!PHYMOD_LANEPBMP_MEMBER(phy->access.lane_mask, start_lane + i)) {
                continue;
            }
            phy_copy.access.lane_mask = 0x1 << (i + start_lane);
            PHYMOD_IF_ERR_RETURN(falcon16_tsc_tx_prbs_en(&phy_copy.access, enable));
            PHYMOD_IF_ERR_RETURN(falcon16_tsc_rx_prbs_en(&phy_copy.access, enable));
        }
    }
        
    return PHYMOD_E_NONE;
    
}

int tscf16_phy_prbs_enable_get(const phymod_phy_access_t* phy, uint32_t flags , uint32_t* enable)
{
    uint32_t enable_tmp;

    if (PHYMOD_PRBS_DIRECTION_TX_GET(flags)) {
        PHYMOD_IF_ERR_RETURN(falcon16_prbs_tx_enable_get(&phy->access, &enable_tmp));
        *enable = enable_tmp;
    } else if (PHYMOD_PRBS_DIRECTION_RX_GET(flags)) {
        PHYMOD_IF_ERR_RETURN(falcon16_prbs_rx_enable_get(&phy->access, &enable_tmp));
        *enable = enable_tmp;
    } else {
        PHYMOD_IF_ERR_RETURN(falcon16_prbs_tx_enable_get(&phy->access, &enable_tmp));
        *enable = enable_tmp;
        PHYMOD_IF_ERR_RETURN(falcon16_prbs_rx_enable_get(&phy->access, &enable_tmp));
        *enable &= enable_tmp;
    }
        
    return PHYMOD_E_NONE;
    
}


int tscf16_phy_prbs_status_get(const phymod_phy_access_t* phy, uint32_t flags, phymod_prbs_status_t* prbs_status)
{
    uint8_t status = 0;
    uint32_t prbs_err_count = 0;
    int i, start_lane, num_lane;
    phymod_phy_access_t phy_copy;

    PHYMOD_MEMCPY(&phy_copy, phy, sizeof(phy_copy));
    /* next figure out the lane num and start_lane based on the input */
    PHYMOD_IF_ERR_RETURN
        (phymod_util_lane_config_get(&phy->access, &start_lane, &num_lane));

    prbs_status->prbs_lock = 0;
    prbs_status->error_count = 0;
    prbs_status->prbs_lock_loss = 0;
    prbs_status->prbs_lock = 1;

    for (i = 0; i < num_lane; i++) {
        if (!PHYMOD_LANEPBMP_MEMBER(phy->access.lane_mask, start_lane + i)) {
            continue;
        }
        phy_copy.access.lane_mask = 0x1 << (i + start_lane);
        PHYMOD_IF_ERR_RETURN(falcon16_tsc_prbs_chk_lock_state(&phy_copy.access, &status));
        if (status) {
            /*next check the lost of lock and error count */
            status = 0;
            PHYMOD_IF_ERR_RETURN
                (falcon16_tsc_prbs_err_count_state(&phy_copy.access, &prbs_err_count, &status));
            PHYMOD_DEBUG_VERBOSE((" Lane :: %d PRBS Error count :: %d\n", i, prbs_err_count));
            if (status) {
                /*temp lost of lock */
                prbs_status->prbs_lock_loss = 1;
            } else {
                prbs_status->error_count += prbs_err_count;
            }
        } else {
            PHYMOD_DEBUG_VERBOSE((" Lane :: %d PRBS not locked\n", i ));
            prbs_status->prbs_lock = 0;
            /* return PHYMOD_E_NONE; */
        }
    }
 
    return PHYMOD_E_NONE;
    
}


int tscf16_phy_pattern_config_set(const phymod_phy_access_t* phy, const phymod_pattern_t* pattern)
{
    int i,j, bit;
    char patt[PATTERN_MAX_LENGTH+1];
    for (i=0; i< PATTERN_MAX_SIZE; i++)
    {
      for (j=0; (j<32) && (i*32+j <= PATTERN_MAX_LENGTH); j++)
      {
        if (i*32+j == pattern->pattern_len)
           break;
        bit = pattern->pattern[i] >> j & 00000001;
        switch (bit) {
        case (1):
            patt[i*32+j]='1';
            break;
        default:
            patt[i*32+j] = '0';
            break;
        }
      }
      if (i*32+j == pattern->pattern_len && i*32+j <= PATTERN_MAX_LENGTH) {
          /* coverity[overrun-local] */
          patt[i*32+j] = '\0';
          break;
      }
     }

    PHYMOD_IF_ERR_RETURN
        (falcon16_tsc_config_shared_tx_pattern_idx_set(&phy->access,
                                  &pattern->pattern_len ));
    /* Coverity: divide_by_zero */
    if (pattern->pattern_len == 0){
        PHYMOD_DEBUG_ERROR (("Invalid pattern length %d\n", pattern->pattern_len));

        return ERR_CODE_DIAG;
    } 
    PHYMOD_IF_ERR_RETURN
        (falcon16_tsc_config_shared_tx_pattern (&phy->access,
                                (uint8_t) pattern->pattern_len, (const char *) patt));
    
    return PHYMOD_E_NONE;
    
}

int tscf16_phy_pattern_config_get(const phymod_phy_access_t* phy, phymod_pattern_t* pattern)
{
   PHYMOD_IF_ERR_RETURN
        (falcon16_tsc_config_shared_tx_pattern_idx_get(&phy->access,
                                  &pattern->pattern_len,
                                  pattern->pattern));
    
    return PHYMOD_E_NONE;
    
}


int tscf16_phy_pattern_enable_set(const phymod_phy_access_t* phy, uint32_t enable, const phymod_pattern_t* pattern)
{
     PHYMOD_IF_ERR_RETURN
        (falcon16_tsc_tx_shared_patt_gen_en(&phy->access, (uint8_t) enable, (uint8_t)pattern->pattern_len));
 
    return PHYMOD_E_NONE;
    
}

int tscf16_phy_pattern_enable_get(const phymod_phy_access_t* phy, uint32_t* enable)
{
   PHYMOD_IF_ERR_RETURN
       (falcon16_tsc_tx_shared_patt_gen_en_get(&phy->access, (uint8_t *) enable));

    return PHYMOD_E_NONE;
    
}


int tscf16_core_diagnostics_get(const phymod_core_access_t* core, phymod_core_diagnostics_t* diag)
{
    int16_t temperature;
    falcon16_tsc_read_die_temperature(&core->access,&temperature);
    diag->temperature = temperature ;
    falcon16_tsc_read_pll_range(&core->access, &diag->pll_range);

    return PHYMOD_E_NONE;
    
}


int tscf16_phy_pmd_info_dump(const phymod_phy_access_t* phy, char* arg)
{
    phymod_phy_access_t phy_copy;
    struct falcon16_tsc_detailed_lane_status_st lane_st[4] = {{0}};
    char *cmd_str = (char*)arg;
    int start_lane, num_lane;
    int tmp_lane_mask;
    int i = 0;
    int j = 0;
    int tmp_ln_msk;
    uint32_t type;
    int is_in_reset = 0;

    if (!cmd_str) {
        type = (uint32)TEFMOD16_DIAG_DSC;
    } else if((!PHYMOD_STRCMP(cmd_str,"ber"))||(!PHYMOD_STRCMP(cmd_str,"Ber"))||(!PHYMOD_STRCMP(cmd_str,"BER"))) {
        type = (uint32)TEFMOD16_DIAG_BER;
    } else if((!PHYMOD_STRCMP(cmd_str,"config"))||(!PHYMOD_STRCMP(cmd_str,"Config"))||(!PHYMOD_STRCMP(cmd_str,"CONFIG"))) {
        type = (uint32)TEFMOD16_DIAG_CFG;
    } else if((!PHYMOD_STRCMP(cmd_str,"cl72"))||(!PHYMOD_STRCMP(cmd_str,"Cl72"))||(!PHYMOD_STRCMP(cmd_str,"CL72"))) {
        type = (uint32)TEFMOD16_DIAG_CL72;
    } else if((!PHYMOD_STRCMP(cmd_str,"debug"))||(!PHYMOD_STRCMP(cmd_str,"Debug"))||(!PHYMOD_STRCMP(cmd_str,"DEBUG"))) {
        type = (uint32)TEFMOD16_DIAG_DEBUG;
    } else if((!PHYMOD_STRCMP(cmd_str,"state"))||(!PHYMOD_STRCMP(cmd_str,"State"))||(!PHYMOD_STRCMP(cmd_str,"STATE"))) {
        type = (uint32)TEFMOD16_DIAG_STATE;
    } else if((!PHYMOD_STRCMP(cmd_str,"verbose"))||(!PHYMOD_STRCMP(cmd_str,"Verbose"))||(!PHYMOD_STRCMP(cmd_str,"VERBOSE"))) {
        type = (uint32)TEFMOD16_DIAG_ALL;
    } else if (!PHYMOD_STRCMP(cmd_str,"STD")){
        type = (uint32)TEFMOD16_DIAG_DSC_STD;
    } else {
        type = (uint32)TEFMOD16_DIAG_STATE;
    }
    PHYMOD_DEBUG_ERROR((" %s:%d type = %d laneMask  = 0x%X, Address = 0x%X\n", __func__, __LINE__, type, phy->access.lane_mask, phy->access.addr));

    PHYMOD_MEMCPY(&phy_copy, phy, sizeof(phy_copy));

    /*next figure out the lane num and start_lane based on the input*/
    PHYMOD_IF_ERR_RETURN
        (phymod_util_lane_config_get(&phy->access, &start_lane, &num_lane));

    if(type==TEFMOD16_DIAG_DSC) {
       PHYMOD_IF_ERR_RETURN
           (falcon16_tsc_display_core_state(&phy_copy.access));
       PHYMOD_IF_ERR_RETURN
           (falcon16_tsc_display_lane_state_hdr());
       for (i = 0; i < 4; i++) {
            phy_copy.access.lane_mask = 0x1 << i ;
            PHYMOD_IF_ERR_RETURN
                (tefmod16_pmd_x4_reset_get(&phy_copy.access, &is_in_reset));
            if(is_in_reset) {
                PHYMOD_DIAG_OUT(("    - skip lane=%0d\n", i));
                continue;
            }
            PHYMOD_IF_ERR_RETURN
                (falcon16_tsc_display_lane_state(&phy_copy.access));
       }
    } else if(type==TEFMOD16_DIAG_DSC_STD) {
        PHYMOD_DIAG_OUT(("    +--------------------------------------------------------------------+\n"));
        PHYMOD_DIAG_OUT(("    | DSC Phy: 0x%03x lane_mask: 0x%02x                                 |\n", phy->access.addr, phy->access.lane_mask));
        PHYMOD_DIAG_OUT(("    +--------------------------------------------------------------------+\n"));
        PHYMOD_IF_ERR_RETURN
            (falcon16_tsc_display_core_state(&phy_copy.access));
        PHYMOD_IF_ERR_RETURN
            (falcon16_tsc_display_lane_state_hdr());
        for (i = 0; i < num_lane; i++) {
            phy_copy.access.lane_mask = 0x1 << (i + start_lane);
            PHYMOD_IF_ERR_RETURN
                (tefmod16_pmd_x4_reset_get(&phy_copy.access, &is_in_reset));
            if(is_in_reset) {
                PHYMOD_DIAG_OUT(("    - skip lane=%0d\n", (i + start_lane)));
                continue;
            }
            PHYMOD_IF_ERR_RETURN
                (falcon16_tsc_display_lane_state(&phy_copy.access));
        }
        tmp_ln_msk = phy_copy.access.lane_mask;
        for (j = 0; j < 4; j++) {
            phy_copy.access.lane_mask = 0x1 << j;
            PHYMOD_IF_ERR_RETURN
                (tefmod16_pmd_x4_reset_get(&phy_copy.access, &is_in_reset));
            if(is_in_reset) {
                continue;
            }
            PHYMOD_IF_ERR_RETURN
                (falcon16_tsc_log_full_pmd_state(&phy_copy.access, &lane_st[j]));
        }
        phy_copy.access.lane_mask = tmp_ln_msk;
        PHYMOD_IF_ERR_RETURN
            (falcon16_tsc_disp_full_pmd_state(&phy_copy.access, lane_st, 4));
        PHYMOD_IF_ERR_RETURN
            (falcon16_tsc_read_event_log(&phy_copy.access));
    } else {
        for (i = 0; i < num_lane; i++) {
            phy_copy.access.lane_mask = 0x1 << (i + start_lane);

            switch(type) {
            case TEFMOD16_DIAG_CFG:
                PHYMOD_DEBUG_ERROR((" %s:%d type = CFG\n", __func__, __LINE__));
                if(i==0) {
                    tmp_lane_mask = phy_copy.access.lane_mask;
                    phy_copy.access.lane_mask = 1;
                    PHYMOD_IF_ERR_RETURN
                        (falcon16_tsc_display_core_config(&phy_copy.access));
                    phy_copy.access.lane_mask = tmp_lane_mask;
                }
                PHYMOD_IF_ERR_RETURN
                    (falcon16_tsc_display_lane_config(&phy_copy.access));
                break;

            case TEFMOD16_DIAG_CL72:
                PHYMOD_DEBUG_ERROR((" %s:%d type = CL72\n", __func__, __LINE__));
                PHYMOD_IF_ERR_RETURN
                    (falcon16_tsc_display_cl93n72_status(&phy_copy.access));
                break;

            case TEFMOD16_DIAG_DEBUG:
                PHYMOD_DEBUG_ERROR((" %s:%d type = DBG\n", __func__, __LINE__));
                PHYMOD_IF_ERR_RETURN
                    (falcon16_tsc_display_lane_debug_status(&phy_copy.access));
                break;

            case TEFMOD16_DIAG_BER:
                PHYMOD_DEBUG_ERROR((" %s:%d type = BER\n", __func__, __LINE__));
                break;


            case TEFMOD16_DIAG_ALL:
                PHYMOD_DEBUG_ERROR((" %s:%d type = CFG\n", __func__, __LINE__));
                if(i==0) {
                    tmp_lane_mask = phy_copy.access.lane_mask;
                    phy_copy.access.lane_mask = 1;
                    PHYMOD_IF_ERR_RETURN
                        (falcon16_tsc_display_core_config(&phy_copy.access));
                    phy_copy.access.lane_mask = tmp_lane_mask;
                }
                PHYMOD_IF_ERR_RETURN
                    (falcon16_tsc_display_lane_config(&phy_copy.access));

                PHYMOD_DEBUG_ERROR((" %s:%d type = CL72\n", __func__, __LINE__));
                PHYMOD_IF_ERR_RETURN
                    (falcon16_tsc_display_cl93n72_status(&phy_copy.access));

                PHYMOD_DEBUG_ERROR((" %s:%d type = DBG\n", __func__, __LINE__));
                PHYMOD_IF_ERR_RETURN
                    (falcon16_tsc_display_lane_debug_status(&phy_copy.access));
 
                break;                
            case TEFMOD16_DIAG_STATE:
            default:
                PHYMOD_DEBUG_ERROR((" %s:%d type = DEF\n", __func__, __LINE__));
                PHYMOD_IF_ERR_RETURN
                    (falcon16_tsc_display_core_state(&phy_copy.access));
                PHYMOD_IF_ERR_RETURN
                    (falcon16_tsc_display_lane_state_hdr());
                PHYMOD_IF_ERR_RETURN
                    (falcon16_tsc_display_lane_state(&phy_copy.access));
                tmp_ln_msk = phy_copy.access.lane_mask;
                for (j = 0; j < 4; j++) {
                     phy_copy.access.lane_mask = 0x1 << j;
                     PHYMOD_IF_ERR_RETURN
                         (tefmod16_pmd_x4_reset_get(&phy_copy.access, &is_in_reset));
                     if(is_in_reset) {
                         PHYMOD_DIAG_OUT(("    - skip lane=%0d\n", j));
                         continue;
                     }
                     PHYMOD_IF_ERR_RETURN
                         (falcon16_tsc_log_full_pmd_state(&phy_copy.access, &lane_st[j]));
                 }
                phy_copy.access.lane_mask = tmp_ln_msk;
                PHYMOD_IF_ERR_RETURN
                    (falcon16_tsc_disp_full_pmd_state(&phy_copy.access, lane_st, 4));
                PHYMOD_IF_ERR_RETURN
                    (falcon16_tsc_read_event_log(&phy_copy.access));
                break;
            }
        }
    }
     
    return PHYMOD_E_NONE;
    
}


int tscf16_phy_pcs_info_dump(const phymod_phy_access_t* phy, char* type)
{
        
    phymod_phy_access_t phy_copy;

    PHYMOD_MEMCPY(&phy_copy, phy, sizeof(phy_copy));
    PHYMOD_IF_ERR_RETURN (tefmod16_diag_disp(&phy_copy.access, (char*)type));

        
    return PHYMOD_E_NONE;
    
}

int tscf16_phy_fec_correctable_counter_get(const phymod_phy_access_t* phy, uint32_t* count)
{
    PHYMOD_IF_ERR_RETURN(tefmod16_fec_correctable_counter_get(&phy->access, count));
    return PHYMOD_E_NONE;

}


int tscf16_phy_fec_uncorrectable_counter_get(const phymod_phy_access_t* phy, uint32_t* count)
{
    PHYMOD_IF_ERR_RETURN(tefmod16_fec_uncorrectable_counter_get(&phy->access, count));

    return PHYMOD_E_NONE;

}

#endif /* PHYMOD_TSCF16_SUPPORT */
