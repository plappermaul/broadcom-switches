/* 
 *  
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#include <phymod/phymod_diag.h>
#include <phymod/phymod_system.h>
#include <phymod/phymod_util.h>
#include <phymod/phymod_debug.h>

#ifdef PHYMOD_QSGMIIE_SUPPORT
#include <phymod/chip/bcmi_qsgmiie_serdes_sym.h>
#endif /*PHYMOD_QSGMIIE_SUPPORT  */
#ifdef PHYMOD_TSCE_SUPPORT
#include <phymod/chip/bcmi_tsce_xgxs_sym.h>
#endif /*PHYMOD_TSCE_SUPPORT  */
#ifdef PHYMOD_TSCF_SUPPORT
#include <phymod/chip/bcmi_tscf_xgxs_sym.h> 
#include <phymod/chip/bcmi_tscf_xgxs_defs.h> 
#include <phymod/chip/bcmi_tscf_xgxs_gen2_sym.h>
#endif /*PHYMOD_TSCF_SUPPORT  */
#if 0 /* #ifdef PHYMOD_PHY8806X_SUPPORT */
#include <phymod/chip/bcmi_phy8806x_xgxs_sym.h>
#endif /*PHYMOD_PHY8806X_SUPPORT  */
#ifdef PHYMOD_EAGLE_SUPPORT
#include <phymod/chip/bcmi_eagle_xgxs_sym.h>
#endif /*PHYMOD_EAGLE_SUPPORT  */
#ifdef PHYMOD_FALCON_SUPPORT
#include <phymod/chip/bcmi_falcon_xgxs_sym.h>
#endif /*PHYMOD_FALCON_SUPPORT  */
#ifdef PHYMOD_VIPER_SUPPORT
#include <phymod/chip/bcmi_viper_xgxs_defs.h>
#include <phymod/chip/bcmi_viper_xgxs_sym.h>
#include <phymod/chip/bcmi_sgmiip2x4_serdes_sym.h>
#endif /*PHYMOD_VIPER_SUPPORT  */
#ifdef PHYMOD_TSCF16_SUPPORT
#include <phymod/chip/bcmi_tscf_16nm_xgxs_sym.h>
#endif /*PHYMOD_TSCF16_SUPPORT  */
#ifdef PHYMOD_TSCE_DPLL_SUPPORT
#include <phymod/chip/bcmi_tsce_dpll_xgxs_sym.h>
#endif /*PHYMOD_TSCE_DPLL_SUPPORT  */
#ifdef PHYMOD_BLACKHAWK_SUPPORT
#include <phymod/chip/bcmi_blackhawk_xgxs_sym.h>
#endif /*PHYMOD_BLACKHAWK_SUPPORT  */
#ifdef PHYMOD_TSCE16_SUPPORT
#include <phymod/chip/bcmi_tsce16_xgxs_sym.h>
#endif
#ifdef PHYMOD_TSCBH_SUPPORT
#include <phymod/chip/bcmi_tscbh_xgxs_sym.h>
#endif /*PHYMOD_TSCBH_SUPPORT  */
#ifdef PHYMOD_TSCE16_SUPPORT
#include <phymod/chip/bcmi_tsce16_xgxs_sym.h>
#endif
#ifdef PHYMOD_TSCF_GEN3_SUPPORT
#include <phymod/chip/bcmi_tscf_gen3_xgxs_sym.h>
#endif /*PHYMOD_TSCF_GEN3_SUPPORT  */
#ifdef PHYMOD_BLACKHAWK_SUPPORT
#include <phymod/chip/bcmi_blackhawk_xgxs_sym.h>
#endif /*PHYMOD_BLACKHAWK_SUPPORT  */
#ifdef PHYMOD_QTCE16_SUPPORT
#include <phymod/chip/bcmi_qtce16_serdes_sym.h>
#endif /*PHYMOD_QTCE16_SUPPORT  */
#ifdef PHYMOD_TSCF16_GEN3_SUPPORT
#include <phymod/chip/bcmi_tscf16_gen3_xgxs_sym.h>
#endif /*PHYMOD_TSCF16_GEN3_SUPPORT  */
#ifdef PHYMOD_TSCBH_GEN2_SUPPORT
#include <phymod/chip/bcmi_tscbh_gen2_xgxs_sym.h>
#endif /*PHYMOD_TSCBH_GEN2_SUPPORT*/
#ifdef PHYMOD_TSCE7_SUPPORT
#include <phymod/chip/bcmi_tsce7_xgxs_sym.h>
#endif /*PHYMOD_TSCE7_SUPPORT*/
#ifdef PHYMOD_TSCBH_FLEXE_SUPPORT
#include <phymod/chip/bcmi_tscbh_flexe_xgxs_sym.h>
#endif /*PHYMOD_TSCBH_FLEXE_SUPPORT*/
#ifdef PHYMOD_TSCO_SUPPORT
#include <phymod/chip/bcmi_tsco_xgxs_sym.h>
#endif /*PHYMOD_TSCO_SUPPORT*/
#ifdef PHYMOD_TSCBH_GEN3_SUPPORT
#include <phymod/chip/bcmi_tscbh_gen3_xgxs_sym.h>
#endif /*PHYMOD_TSCBH_GEN3_SUPPORT*/
#ifdef PHYMOD_BLACKHAWK7_V1L8P1_SUPPORT
#include <phymod/chip/bcmi_blackhawk7_v1l8p1_xgxs_sym.h>
#endif

#define _PHYMOD_PHY_MSG(phy, string) ("Phy 0x%x lanes 0x%02x: " string), (phy)->access.addr, (phy)->access.lane_mask
#define _PHYMOD_CORE_MSG(core, string) ("Core 0x%x: " string), (core)->access.addr

#define PHYMOD_DIAG_PRINT_FUNC_VALIDATE if(phymod_diag_print_func == NULL) return PHYMOD_E_IO

print_func_f phymod_diag_print_func = NULL;


int phymod_diag_symbols_table_get(phymod_phy_access_t *phy, phymod_symbols_t **symbols)
{
    switch(phy->type){
#ifdef PHYMOD_QSGMIIE_SUPPORT
    case phymodDispatchTypeQsgmiie:
        *symbols = &bcmi_qsgmiie_serdes_symbols;
        break;
#endif /*PHYMOD_QSGMIIE_SUPPORT  */
#ifdef PHYMOD_TSCE_SUPPORT
    case phymodDispatchTypeTsce:
        *symbols = &bcmi_tsce_xgxs_symbols;
        break;
#endif /*PHYMOD_TSCE_SUPPORT  */
#ifdef PHYMOD_TSCF_SUPPORT
    case phymodDispatchTypeTscf:
    {
        /* first need to get the tscf model number */
      MAIN0_SERDESIDr_t serdesid;
      uint32_t model; 

      /* next check serdes ID to see if gen2 or not */
      READ_MAIN0_SERDESIDr(&phy->access, &serdesid);
      model = MAIN0_SERDESIDr_MODEL_NUMBERf_GET(serdesid);

      /* check if tscf_gen2 version */
      if (model == 0x15)  {
        *symbols = &bcmi_tscf_xgxs_gen2_symbols;
      } else {
        *symbols = &bcmi_tscf_xgxs_symbols;
      }
        break;
    }
#endif /*PHYMOD_TSCF_SUPPORT  */
#if 0 /* #ifdef PHYMOD_PHY8806X_SUPPORT */
    case phymodDispatchTypePhy8806x:
        *symbols = &bcmi_phy8806x_xgxs_symbols;
        break;
#endif /*PHYMOD_PHY8806X_SUPPORT  */
#ifdef PHYMOD_EAGLE_SUPPORT
    case phymodDispatchTypeEagle:
        *symbols = &bcmi_eagle_xgxs_symbols;
        break;
#endif /*PHYMOD_EAGLE_SUPPORT  */
#ifdef PHYMOD_FALCON_SUPPORT
    case phymodDispatchTypeFalcon:
        *symbols = &bcmi_falcon_xgxs_symbols;
        break;
#endif /*PHYMOD_FALCON_SUPPORT  */
#ifdef PHYMOD_VIPER_SUPPORT
    case phymodDispatchTypeViper:
    {
        SERDESID0r_t serdes_id;
        uint32_t model = 0;

        READ_SERDESID0r(&phy->access, &serdes_id);
        model = SERDESID0r_MODEL_NUMBERf_GET(serdes_id);

        /* check if SGMIIPLUS2x4 core */
        if (model == 0xf) {
            *symbols = &bcmi_sgmiip2x4_serdes_symbols;
        } else {
            *symbols = &bcmi_viper_xgxs_symbols;
        }

        break;
    }
#endif /*PHYMOD_VIPER_SUPPORT  */
#ifdef PHYMOD_TSCF16_SUPPORT
    case phymodDispatchTypeTscf16:
        *symbols = &bcmi_tscf_16nm_xgxs_symbols;
        break;
#endif /*PHYMOD_TSCF16_SUPPORT  */
#ifdef PHYMOD_TSCE_DPLL_SUPPORT
    case phymodDispatchTypeTsce_dpll:
        *symbols = &bcmi_tsce_dpll_xgxs_symbols;
        break;
#endif
#ifdef PHYMOD_TSCE16_SUPPORT
    case phymodDispatchTypeTsce16:
        *symbols = &bcmi_tsce16_xgxs_symbols;
        break;
#endif
#ifdef PHYMOD_TSCF_GEN3_SUPPORT
    case phymodDispatchTypeTscf_gen3:
        *symbols = &bcmi_tscf_gen3_xgxs_symbols;
        break;
#endif /*PHYMOD_TSCF_GEN3_SUPPORT  */
#ifdef PHYMOD_BLACKHAWK_SUPPORT
    case phymodDispatchTypeBlackhawk:
        *symbols = &bcmi_blackhawk_xgxs_symbols;
        break;
#endif
#ifdef PHYMOD_TSCBH_SUPPORT
    case phymodDispatchTypeTscbh:
        *symbols = &bcmi_tscbh_xgxs_symbols;
        break;
#endif
#ifdef PHYMOD_QTCE16_SUPPORT
    case phymodDispatchTypeQtce16:
        *symbols = &bcmi_qtce16_xgxs_symbols;
        break;
#endif /*PHYMOD_QTCE16_SUPPORT  */
#ifdef PHYMOD_TSCF16_GEN3_SUPPORT
    case phymodDispatchTypeTscf16_gen3:
        *symbols = &bcmi_tscf16_gen3_xgxs_symbols;
        break;
#endif /*PHYMOD_TSCF16_GEN3_SUPPORT  */
#ifdef PHYMOD_TSCBH_GEN2_SUPPORT
    case phymodDispatchTypeTscbh_gen2:
        *symbols = &bcmi_tscbh_gen2_xgxs_symbols;
        break;
#endif /*PHYMOD_TSCBH_GEN2_SUPPORT  */
#ifdef PHYMOD_TSCE7_SUPPORT
    case phymodDispatchTypeTsce7:
        *symbols = &bcmi_tsce7_xgxs_symbols;
        break;
#endif /*PHYMOD_TSCE7_SUPPORT  */
#ifdef PHYMOD_TSCBH_FLEXE_SUPPORT
    case phymodDispatchTypeTscbh_flexe:
        *symbols = &bcmi_tscbh_flexe_xgxs_symbols;
        break;
#endif /*PHYMOD_TSCBH_FLEXE_SUPPORT  */
#ifdef PHYMOD_TSCO_SUPPORT
    case phymodDispatchTypeTsco:
        *symbols = &bcmi_tsco_xgxs_symbols;
        break;
#endif /*PHYMOD_TSCO_SUPPORT  */
#ifdef PHYMOD_TSCBH_GEN3_SUPPORT
    case phymodDispatchTypeTscbh_gen3:
        *symbols = &bcmi_tscbh_gen3_xgxs_symbols;
        break;
#endif /*PHYMOD_TSCBH_GEN3_SUPPORT  */
#ifdef PHYMOD_BLACKHAWK7_V1L8P1_SUPPORT
        case phymodDispatchTypeBlackhawk7_v1l8p1:
            *symbols = &bcmi_blackhawk7_v1l8p1_xgxs_symbols;
            break;
#endif /*PHYMOD_BLACKHAWK7_V1L8P1_SUPPORT  */

    default:
        PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(phy,"unsupported SerDes type(%d) for symbolic access \n"), phy->type));
        return PHYMOD_E_UNAVAIL;
    }
    return PHYMOD_E_NONE;
}

/******************************************************************************
 Firmware load
 */

int phymod_diag_firmware_load(phymod_core_access_t *cores, int array_size, char *firwmware_file){
    int i = 0;

    /*unsigned short firmware_crc;
    unsigned short firmware_verision;
    unsigned short firmware_length;*/
    unsigned short phy_type = -1;
    
    PHYMOD_DIAG_PRINT_FUNC_VALIDATE;
    
    
      
    for(i = 0 ; i < array_size ; i++){
        if(cores[i].type != phy_type){
            PHYMOD_DEBUG_ERROR((_PHYMOD_CORE_MSG(&cores[i],"The firmware is not compatible with the core\n")));
            return PHYMOD_E_FAIL;
        }
    }
    return PHYMOD_E_NONE;
}


/******************************************************************************
 Register read/write
 */
int phymod_diag_reg_read(phymod_phy_access_t *phys, int array_size, uint32_t reg_addr){
    uint32_t val = 0;
    int rv;
    int i;

    PHYMOD_DIAG_PRINT_FUNC_VALIDATE;
    
    for(i = 0 ; i < array_size ; i++){
       rv = phymod_phy_reg_read(&phys[i], reg_addr, &val);
       if(rv == PHYMOD_E_NONE){
           PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(&phys[i],"Reg 0x%08x: 0x%04x\n"), reg_addr, val));
       }
       else{
           PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(&phys[i],"Reg 0x%08x: Error read register!\n"), reg_addr));
       }
    }
    return PHYMOD_E_NONE;
}


int phymod_diag_reg_write(phymod_phy_access_t *phys, int array_size, uint32_t reg_addr, uint32_t val){
    int i;
    int rv;

    PHYMOD_DIAG_PRINT_FUNC_VALIDATE;
    
    for(i = 0 ; i < array_size ; i++){
       rv = phymod_phy_reg_write(&phys[i], reg_addr, val);
       if(rv != PHYMOD_E_NONE){
           PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(&phys[i],"Reg 0x%08x: Error write register!\n"), reg_addr));
       }
    }
    return PHYMOD_E_NONE;
}


/******************************************************************************
 PRBS
 */
STATIC
void phymod_diag_prbs_get_results_print(phymod_phy_access_t *phy, phymod_prbs_status_t *status) {
    if (status->prbs_lock == 0) {
        if (status->error_count) {
            PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG
                                       (phy, "prbs unlocked with %d errors during PRBS test\n"),
                                        status->error_count));   
        } else {
            PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(phy, "prbs unlocked\n")));
        } 
    } else if (status->prbs_lock_loss == 1) {
        PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(phy, "prbs unlocked during the PRBS test\n")));
    } else {
        if (status->error_count) {
            PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG
                                       (phy, "prbs locked with %d errors during PRBS test\n"),
                                        status->error_count));   
        } else { 
            PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(phy, "prbs locked\n")));
            PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(phy, "PRBS OK!\n")));
        }
    }
}


STATIC
int phymod_diag_prbs_set(phymod_phy_access_t *phys, int array_size, phymod_diag_prbs_set_args_t *params){
    int i = 0;
    int rv;
    
    phymod_prbs_t_validate(&params->prbs_options);
    
       
    if(params->loopback == 1){
        for(i = 0 ; i < array_size ; i++){
            rv = phymod_phy_loopback_set(&phys[i], phymodLoopbackGlobal, 1);
            if(rv != PHYMOD_E_NONE){
                PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(&phys[i],"Failed set loopback\n")));
                return rv;
            }
        }
    }    
    for(i = 0 ; i < array_size ; i++){
        if(phymod_phy_prbs_config_set(&phys[i], params->flags, &params->prbs_options) != PHYMOD_E_NONE){
            PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(&phys[i],"Failed set PRBS\n")));
            return PHYMOD_E_FAIL;
        }
        if(phymod_phy_prbs_enable_set(&phys[i], params->flags,  params->enable) != PHYMOD_E_NONE){
            PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(&phys[i],"Failed enable PRBS\n")));
            return PHYMOD_E_FAIL;
        }

    }
    return PHYMOD_E_NONE;
}



STATIC
int phymod_diag_prbs_get(phymod_phy_access_t *phys, int array_size, phymod_diag_prbs_get_args_t *params){
    int i = 0;
    int rv;
    phymod_prbs_status_t status;
    uint32_t enable;
    uint32_t flags = 0;

    
    for(i = 0 ; i < array_size ; i++){

        PHYMOD_PRBS_DIRECTION_RX_SET(flags);
        rv = phymod_phy_prbs_enable_get(&phys[i], flags, &enable);
        if(rv != PHYMOD_E_NONE){
            PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(&phys[i],"Failed get PRBS configuration\n")));
            return rv;
        }
        if(enable == 0){
            PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(&phys[i],"PRBS status get can not be called when rx PRBS is disabled\n")));
            return PHYMOD_E_FAIL;
        }
        /*clear the status*/
        rv = phymod_phy_prbs_status_get(&phys[i], PHYMOD_PRBS_STATUS_F_CLEAR_ON_READ, &status);
        if(rv != PHYMOD_E_NONE){
            PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(&phys[i],"PRBS clear status failed\n")));
            return rv;
        }
    }
    /*wait*/
    PHYMOD_SLEEP(params->time);

    /*read the status*/
    for(i = 0 ; i < array_size ; i++){
        rv = phymod_phy_prbs_status_get(&phys[i], PHYMOD_PRBS_STATUS_F_CLEAR_ON_READ, &status);
        if(rv != PHYMOD_E_NONE){
            PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(&phys[i],"PRBS status read failed\n")));
            return rv;
        }
        phymod_diag_prbs_get_results_print(&phys[i], &status);
    }
    return PHYMOD_E_NONE;
}


STATIC
int phymod_diag_prbs_clear(phymod_phy_access_t *phys, int array_size, phymod_diag_prbs_clear_args_t *params){
    int i = 0;
    int rv;
    
    for(i = 0 ; i < array_size ; i++){
        rv = phymod_phy_prbs_enable_set(&phys[i], params->flags, 0);
        if(rv  != PHYMOD_E_NONE){
            PHYMOD_DEBUG_ERROR((_PHYMOD_PHY_MSG(&phys[i],"Failed clear prbs\n")));
            return rv;
        }
    }
    return PHYMOD_E_NONE;
}


int phymod_diag_prbs(phymod_phy_access_t *phys, int array_size, phymod_diag_prbs_args_t *prbs_diag_args){
    int rv = PHYMOD_E_NONE;

    PHYMOD_DIAG_PRINT_FUNC_VALIDATE;

    switch(prbs_diag_args->prbs_cmd){
    case PhymodDiagPrbsClear:
        rv = phymod_diag_prbs_clear(phys, array_size, &prbs_diag_args->args.clear_params);
        break;
    case PhymodDiagPrbsGet:
        rv = phymod_diag_prbs_get(phys, array_size, &prbs_diag_args->args.get_params);
        break;
    case PhymodDiagPrbsSet:
        rv = phymod_diag_prbs_set(phys, array_size, &prbs_diag_args->args.set_params);
        break;
    default:
        PHYMOD_DEBUG_ERROR(("Failed parsing PRBS command type\n"));
        rv = PHYMOD_E_FAIL;
    }
    return rv;
}


/******************************************************************************
DSC
 */
int phymod_diag_dsc(phymod_phy_access_t *phys, int array_size){

    int i, rv = PHYMOD_E_NONE;
    uint32_t lane, lane_index;

    for(i = 0 ; i < array_size ; i++){
        lane = phys[i].access.lane_mask;
        for( lane_index = 0 ; lane_index < PHYMOD_MAX_LANES_PER_CORE; lane_index++){
            /*if lane is not selected pass*/
            if( (lane & (1<<lane_index)) == 0){
                continue;
            }
            phys[i].access.lane_mask = lane & (1<<lane_index);
            /*collect info*/
            rv = phymod_phy_pmd_info_dump(&(phys[i]), NULL); 

            if(rv != PHYMOD_E_NONE) {
                break;
            }
        }
        phys[i].access.lane_mask = lane;
        if(rv != PHYMOD_E_NONE) {
            break;
        }
    }
    return rv;
}

/******************************************************************************
DSC
 */
int phymod_diag_dsc_std(phymod_phy_access_t *phys, int array_size){

    int i, rv = PHYMOD_E_NONE;
    uint32_t lane, lane_index;

    for(i = 0 ; i < array_size ; i++){
        lane = phys[i].access.lane_mask;
        for( lane_index = 0 ; lane_index < PHYMOD_MAX_LANES_PER_CORE; lane_index++){
            /*if lane is not selected pass*/
            if( (lane & (1<<lane_index)) == 0){
                continue;
            }
            phys[i].access.lane_mask |= lane & (1<<lane_index);

        }
        if (phys[i].access.lane_mask) {
            rv = phymod_phy_pmd_info_dump(&(phys[i]), "STD"); 

            if(rv != PHYMOD_E_NONE) {
                break;
            }
        }
        phys[i].access.lane_mask = lane; 
    }
    return rv;
}

/******************************************************************************
DSC
 */
int phymod_diag_dsc_config(phymod_phy_access_t *phys, int array_size){

    int i, rv = PHYMOD_E_NONE;
    uint32_t lane, lane_index;

    for(i = 0 ; i < array_size ; i++){
        lane = phys[i].access.lane_mask;
        for( lane_index = 0 ; lane_index < PHYMOD_MAX_LANES_PER_CORE; lane_index++){
            /*if lane is not selected pass*/
            if( (lane & (1<<lane_index)) == 0){
                continue;
            }
            phys[i].access.lane_mask = lane & (1<<lane_index);
            /*collect info*/
            rv = phymod_phy_pmd_info_dump(&(phys[i]), "config"); 

            if(rv != PHYMOD_E_NONE) {
                break;
            }
        }
        phys[i].access.lane_mask = lane;
        if(rv != PHYMOD_E_NONE) {
            break;
        }
    }
    return rv;
}


/******************************************************************************
Eyescan
 */
int phymod_diag_eyescan_run( 
    phymod_phy_access_t *phys,
    int unit,
    int* port_ids,
    int* line_rates, 
    int num_phys,
    phymod_eyescan_mode_t mode,
    phymod_phy_eyescan_options_t* eyescan_options
    )   
{
    const phymod_phy_access_t    *pa;
    int                 idx, rc = PHYMOD_E_NONE, rc_done = PHYMOD_E_NONE;
    uint32_t            flags;
    if(eyescan_options) {
      /* new code for BER projection */
      if(mode == phymodEyescanModeBERProj) {
        PHYMOD_DIAG_OUT((" ber_scan_mode   = %d\n", eyescan_options->ber_proj_scan_mode));
        PHYMOD_DIAG_OUT((" timer_control   = %d\n", eyescan_options->ber_proj_timer_cnt));
        PHYMOD_DIAG_OUT((" max_err_control = %d\n", eyescan_options->ber_proj_err_cnt));
      } else {
        PHYMOD_DIAG_OUT((" Timeout = %d ms\n",  eyescan_options->timeout_in_milliseconds));
        PHYMOD_DIAG_OUT((" Hmax    = %d\n",     eyescan_options->horz_max));
        PHYMOD_DIAG_OUT((" Hmin    = %d\n",     eyescan_options->horz_min));
        PHYMOD_DIAG_OUT((" Hstep   = %d\n",     eyescan_options->hstep));
        PHYMOD_DIAG_OUT((" Vmax    = %d\n",     eyescan_options->vert_max));
        PHYMOD_DIAG_OUT((" Vmin    = %d\n",     eyescan_options->vert_min));
        PHYMOD_DIAG_OUT((" Vstep   = %d\n",     eyescan_options->vstep));
        PHYMOD_DIAG_OUT((" mode    = %d\n",     eyescan_options->mode));
      }
    }

    /*enable eyescan*/
    flags = 0;
    PHYMOD_EYESCAN_F_ENABLE_SET(flags);
    PHYMOD_EYESCAN_F_ENABLE_DONT_WAIT_SET(flags);

    for (idx = 0; idx < num_phys; idx++) {
        pa = &(phys[idx]);
        rc = PHYMOD_E_NONE;
        if (pa->access.lane_mask){
            rc = phymod_phy_eyescan_run(pa, flags, mode, NULL);
        }
        if (rc != PHYMOD_E_NONE) {
            PHYMOD_DIAG_OUT(("Failed in phymod_phy_eyescan_run (enable) \n"));
            goto exit;
        }
    }

    PHYMOD_USLEEP(100000);

    /*process eyescan*/
    flags = 0;
    PHYMOD_EYESCAN_F_PROCESS_SET(flags);

    for (idx = 0; idx < num_phys; idx++) {
        pa = &(phys[idx]);

        if(eyescan_options && line_rates) {
            eyescan_options->linerate_in_khz = line_rates[idx] * 1000;
            if((mode == phymodEyescanModeBERProj) && (line_rates[idx] == 0)) {
                PHYMOD_DIAG_OUT(("Parameter error: line rate %d is 0 in phymod_phy_eyescan_run (process) \n", idx));
                goto exit;
            }
        }

        PHYMOD_DIAG_OUT(("for u=%0d p=%0d lane_mask=%0x:\n", unit, port_ids[idx], pa->access.lane_mask));
        rc = phymod_phy_eyescan_run(pa, flags, mode, eyescan_options);
        if (rc != PHYMOD_E_NONE) {
            PHYMOD_DIAG_OUT(("Failed in phymod_phy_eyescan_run (process) \n"));
        }
    }

exit:    
    /*disable eyescan mode*/
    flags = 0;
    PHYMOD_EYESCAN_F_DONE_SET(flags);

    for (idx = 0; idx < num_phys; idx++) {

        pa = &(phys[idx]);

        rc_done = phymod_phy_eyescan_run(pa, flags, mode, eyescan_options);
        if (rc_done != PHYMOD_E_NONE) {
            PHYMOD_DIAG_OUT(("Failed in phymod_phy_eyescan_run (done) \n"));
            if(rc == PHYMOD_E_NONE) { /* Update returned error code */
                rc = rc_done;
            }
        }
    }

    return rc;
}
