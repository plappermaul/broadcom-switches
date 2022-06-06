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
#include <phymod/phymod_util.h>
#include <phymod/phymod_config.h>
#include <phymod/phymod_diagnostics.h>
#include <phymod/phymod_diagnostics_dispatch.h>
#ifdef INCLUDE_FCMAP
  #include <soc/phy/phymod_ctrl.h>
  #include <soc/phy/phyctrl.h>
  #include <soc/phyctrl.h>
#endif

#include "../../tscf/tier1/tefmod_enum_defines.h" 
#include "../../phy8806x/tier1/phy8806x_cfg_seq.h"
#include "../../phy8806x/tier1/phy8806x_tsc_enum.h"
#include "../../phy8806x/tier1/phy8806x_tsc_common.h"
#include "../../phy8806x/tier1/phy8806x_tsc_interface.h"
#include "../../phy8806x/tier1/phy8806x_tsc_dependencies.h"
#include "../../phy8806x/tier1/phy8806x_tsc_debug_functions.h"

#ifdef PHYMOD_PHY8806X_SUPPORT


/*phymod, internal enum mappings*/
STATIC
int _phy8806x_prbs_poly_phymod_to_phy8806x(phymod_prbs_poly_t phymod_poly, enum srds_prbs_polynomial_enum *phy8806x_poly)
{
    switch(phymod_poly){
    case phymodPrbsPoly7:
        *phy8806x_poly = PRBS_7;
        break;
    case phymodPrbsPoly9:
        *phy8806x_poly = PRBS_9;
        break;
    case phymodPrbsPoly11:
        *phy8806x_poly = PRBS_11;
        break;
    case phymodPrbsPoly15:
        *phy8806x_poly = PRBS_15;
        break;
    case phymodPrbsPoly23:
        *phy8806x_poly = PRBS_23;
        break;
    case phymodPrbsPoly31:
        *phy8806x_poly = PRBS_31;
        break;
    case phymodPrbsPoly58:
        *phy8806x_poly = PRBS_58;
        break;
    default:
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("unsupported poly for tscf %u"), phymod_poly));
    }
    return PHYMOD_E_NONE;
}

STATIC
int _phy8806x_prbs_poly_tscf_to_phymod(phy8806x_prbs_polynomial_type_t tscf_poly, phymod_prbs_poly_t *phymod_poly)
{
    switch(tscf_poly){
    case PHY8806X_PRBS_POLYNOMIAL_7:
        *phymod_poly = phymodPrbsPoly7;
        break;
    case PHY8806X_PRBS_POLYNOMIAL_9:
        *phymod_poly = phymodPrbsPoly9;
        break;
    case PHY8806X_PRBS_POLYNOMIAL_11:
        *phymod_poly = phymodPrbsPoly11;
        break;
    case PHY8806X_PRBS_POLYNOMIAL_15:
        *phymod_poly = phymodPrbsPoly15;
        break;
    case PHY8806X_PRBS_POLYNOMIAL_23:
        *phymod_poly = phymodPrbsPoly23;
        break;
    case PHY8806X_PRBS_POLYNOMIAL_31:
        *phymod_poly = phymodPrbsPoly31;
        break;
    case PHY8806X_PRBS_POLYNOMIAL_58:
        *phymod_poly = phymodPrbsPoly58;
        break;
    default:
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_INTERNAL, (_PHYMOD_MSG("uknown poly %u"), tscf_poly));
    }
    return PHYMOD_E_NONE;
}

int phy8806x_phy_rx_slicer_position_set(const phymod_phy_access_t* phy, uint32_t flags, const phymod_slicer_position_t* position)
{
    /* Not supported */
    PHYMOD_DEBUG_ERROR(("phy8806x_phy_rx_slicer_position_set function is NOT SUPPORTED!!\n"));
    return PHYMOD_E_NONE;
}

int phy8806x_phy_rx_slicer_position_get(const phymod_phy_access_t* phy, uint32_t flags, phymod_slicer_position_t* position)
{
    /* Not supported */
    PHYMOD_DEBUG_ERROR(("phy8806x_phy_rx_slicer_position_get function is NOT SUPPORTED!!\n"));
    return PHYMOD_E_NONE;
}


int phy8806x_phy_rx_slicer_position_max_get(const phymod_phy_access_t* phy, uint32_t flags, const phymod_slicer_position_t* position_min, const phymod_slicer_position_t* position_max)
{
    /* Not supported */
    PHYMOD_DEBUG_ERROR(("phy8806x_phy_rx_slicer_position_max_get function is NOT SUPPORTED!!\n"));
    return PHYMOD_E_NONE;
}


int phy8806x_phy_prbs_config_set(const phymod_phy_access_t* phy, uint32_t flags , const phymod_prbs_t* prbs)
{
    enum srds_prbs_polynomial_enum phy8806x_poly;

    PHYMOD_IF_ERR_RETURN(_phy8806x_prbs_poly_phymod_to_phy8806x(prbs->poly, &phy8806x_poly));
    /*first check which direction */
    if (PHYMOD_PRBS_DIRECTION_RX_GET(flags)) {
        PHYMOD_IF_ERR_RETURN
            (phy8806x_tsc_config_rx_prbs(&phy->access, phy8806x_poly, PRBS_INITIAL_SEED_HYSTERESIS,  prbs->invert));
    } else if (PHYMOD_PRBS_DIRECTION_TX_GET(flags)) {
        PHYMOD_IF_ERR_RETURN
            (phy8806x_tsc_config_tx_prbs(&phy->access, phy8806x_poly, prbs->invert));
    } else {
        PHYMOD_IF_ERR_RETURN
            (phy8806x_tsc_config_rx_prbs(&phy->access, phy8806x_poly, PRBS_INITIAL_SEED_HYSTERESIS,  prbs->invert));
        PHYMOD_IF_ERR_RETURN
            (phy8806x_tsc_config_tx_prbs(&phy->access, phy8806x_poly, prbs->invert));
    }
    return PHYMOD_E_NONE;
}

int phy8806x_phy_prbs_config_get(const phymod_phy_access_t* phy, uint32_t flags , phymod_prbs_t* prbs)
{
    phymod_prbs_t config_tmp;
    phy8806x_prbs_polynomial_type_t tscf_poly;

    if (PHYMOD_PRBS_DIRECTION_TX_GET(flags)) {
        PHYMOD_IF_ERR_RETURN(phy8806x_prbs_tx_inv_data_get(&phy->access, &config_tmp.invert));
        PHYMOD_IF_ERR_RETURN(phy8806x_prbs_tx_poly_get(&phy->access, &tscf_poly));
        PHYMOD_IF_ERR_RETURN(_phy8806x_prbs_poly_tscf_to_phymod(tscf_poly, &config_tmp.poly));
        prbs->invert = config_tmp.invert;
        prbs->poly = config_tmp.poly;
    } else if (PHYMOD_PRBS_DIRECTION_RX_GET(flags)) {
        PHYMOD_IF_ERR_RETURN(phy8806x_prbs_rx_inv_data_get(&phy->access, &config_tmp.invert));
        PHYMOD_IF_ERR_RETURN(phy8806x_prbs_rx_poly_get(&phy->access, &tscf_poly));
        PHYMOD_IF_ERR_RETURN(_phy8806x_prbs_poly_tscf_to_phymod(tscf_poly, &config_tmp.poly));
        prbs->invert = config_tmp.invert;
        prbs->poly = config_tmp.poly;
    } else {
        PHYMOD_IF_ERR_RETURN(phy8806x_prbs_tx_inv_data_get(&phy->access, &config_tmp.invert));
        PHYMOD_IF_ERR_RETURN(phy8806x_prbs_tx_poly_get(&phy->access, &tscf_poly));
        PHYMOD_IF_ERR_RETURN(_phy8806x_prbs_poly_tscf_to_phymod(tscf_poly, &config_tmp.poly));
        prbs->invert = config_tmp.invert;
        prbs->poly = config_tmp.poly;
    }
    return PHYMOD_E_NONE;
}

int phy8806x_phy_prbs_enable_set(const phymod_phy_access_t* phy, uint32_t flags , uint32_t enable)
{
    if (PHYMOD_PRBS_DIRECTION_TX_GET(flags)) {
        PHYMOD_IF_ERR_RETURN(phy8806x_tsc_tx_prbs_en(&phy->access, enable));
    } else if (PHYMOD_PRBS_DIRECTION_RX_GET(flags)) {
        PHYMOD_IF_ERR_RETURN(phy8806x_tsc_rx_prbs_en(&phy->access, enable));
    } else {
        PHYMOD_IF_ERR_RETURN(phy8806x_tsc_tx_prbs_en(&phy->access, enable));
        PHYMOD_IF_ERR_RETURN(phy8806x_tsc_rx_prbs_en(&phy->access, enable));
    }
    return PHYMOD_E_NONE;
}

int phy8806x_phy_prbs_enable_get(const phymod_phy_access_t* phy, uint32_t flags , uint32_t* enable)
{
    uint32_t enable_tmp;
    
    if (PHYMOD_PRBS_DIRECTION_TX_GET(flags)) {
        PHYMOD_IF_ERR_RETURN(phy8806x_prbs_tx_enable_get(&phy->access, &enable_tmp));
        *enable = enable_tmp;
    } else if (PHYMOD_PRBS_DIRECTION_RX_GET(flags)) {
        PHYMOD_IF_ERR_RETURN(phy8806x_prbs_rx_enable_get(&phy->access, &enable_tmp));
        *enable = enable_tmp;
    } else {
        PHYMOD_IF_ERR_RETURN(phy8806x_prbs_tx_enable_get(&phy->access, &enable_tmp));
        *enable = enable_tmp;
        PHYMOD_IF_ERR_RETURN(phy8806x_prbs_rx_enable_get(&phy->access, &enable_tmp));
        *enable &= enable_tmp;
    }
    return PHYMOD_E_NONE;
}

int phy8806x_phy_prbs_status_get(const phymod_phy_access_t* phy, uint32_t flags, phymod_prbs_status_t* prbs_status)
{
    uint8_t status = 0;
    uint32_t prbs_err_count = 0;

    PHYMOD_IF_ERR_RETURN(phy8806x_tsc_prbs_chk_lock_state(&phy->access, &status));
    if (status) {
        prbs_status->prbs_lock = 1;
        /*next check the lost of lock and error count */
        status = 0;
        PHYMOD_IF_ERR_RETURN
            (phy8806x_tsc_prbs_err_count_state(&phy->access, &prbs_err_count, &status));
        if (status) {
        /*temp lost of lock */
            prbs_status->prbs_lock_loss = 1;
        } else {
            prbs_status->prbs_lock_loss = 0;
            prbs_status->error_count = prbs_err_count;
        }
    } else {
        prbs_status->prbs_lock = 0;
    }
    return PHYMOD_E_NONE;
}

int phy8806x_phy_pattern_config_set(const phymod_phy_access_t* phy, const phymod_pattern_t* pattern)
{
    /* Not supported */
    PHYMOD_DEBUG_ERROR(("phy8806x_phy_pattern_config_set function is NOT SUPPORTED!!\n"));
    return PHYMOD_E_NONE;
}

int phy8806x_phy_pattern_config_get(const phymod_phy_access_t* phy, phymod_pattern_t* pattern)
{
    /* Not supported */
    PHYMOD_DEBUG_ERROR(("phy8806x_phy_pattern_config_get function is NOT SUPPORTED!!\n"));
    return PHYMOD_E_NONE;
}

int phy8806x_phy_pattern_enable_set(const phymod_phy_access_t* phy, uint32_t enable)
{
    /* Not supported */
    PHYMOD_DEBUG_ERROR(("phy8806x_phy_pattern_enable_set function is NOT SUPPORTED!!\n"));
    return PHYMOD_E_NONE;
}

int phy8806x_phy_pattern_enable_get(const phymod_phy_access_t* phy, uint32_t* enable)
{
    /* Not supported */
    PHYMOD_DEBUG_ERROR(("phy8806x_phy_pattern_enable_get function is NOT SUPPORTED!!\n"));
    return PHYMOD_E_NONE;
}

int phy8806x_core_diagnostics_get(const phymod_core_access_t* core, phymod_core_diagnostics_t* diag)
{
    signed short temperature;

    phy8806x_tsc_read_die_temperature(&core->access, &temperature);
    diag->temperature = temperature;
    phy8806x_tsc_read_pll_range(&core->access, &diag->pll_range);

    return PHYMOD_E_NONE;
}

int phy8806x_phy_diagnostics_get(const phymod_phy_access_t* phy, phymod_phy_diagnostics_t* diag)
{
    unsigned char rx_lock;
    int osr_mode;
    phymod_diag_eyescan_t_init(&diag->eyescan);
    phymod_diag_slicer_offset_t_init(&diag->slicer_offset);

    PHYMOD_IF_ERR_RETURN(phy8806x_tsc_pmd_lock_status(&phy->access, &rx_lock));
    diag->rx_lock = (uint32_t ) rx_lock;
    PHYMOD_IF_ERR_RETURN(phy8806x_osr_mode_get(&phy->access, &osr_mode));
    switch(osr_mode) {
        case 0: diag->osr_mode = phymodOversampleMode1; break;
        case 1: diag->osr_mode = phymodOversampleMode2; break;
        case 2: diag->osr_mode = phymodOversampleMode4; break;
        case 8: diag->osr_mode = phymodOversampleMode16P5; break;
        case 12: diag->osr_mode = phymodOversampleMode20P625; break;
        default: 
            PHYMOD_RETURN_WITH_ERR(PHYMOD_E_INTERNAL, (_PHYMOD_MSG("unsupported OS mode %d"), osr_mode));
    }
    PHYMOD_IF_ERR_RETURN(phy8806x_tsc_signal_detect(&phy->access, &diag->signal_detect));
    return PHYMOD_E_NONE;
}

int phy8806x_phy_pmd_info_dump(const phymod_phy_access_t* phy, void *arg)
{
    int start_lane, num_lane;
    phymod_phy_access_t phy_copy;
    struct phy8806x_tsc_detailed_lane_status_st lane_st[4];
    int i = 0;
    int j = 0;
    char *cmd_str = (char*)arg;
    uint32_t type;
    uint8_t trace_mem[768];
#ifdef INCLUDE_FCMAP
    soc_phymod_core_t *soc_core;
    phy_ctrl_t *pc;
#endif

    if (!cmd_str)
        type = (uint32_t)TEFMOD_DIAG_STATE;
    else if (!PHYMOD_STRCMP(cmd_str, "ber"))
        type = (uint32_t)TEFMOD_DIAG_BER;
    else if (!PHYMOD_STRCMP(cmd_str, "config"))
        type = (uint32_t)TEFMOD_DIAG_CFG;
    else if (!PHYMOD_STRCMP(cmd_str, "CL72"))
        type = (uint32_t)TEFMOD_DIAG_CL72;
    else if (!PHYMOD_STRCMP(cmd_str, "debug"))
        type = (uint32_t)TEFMOD_DIAG_DEBUG;
    else
        type = (uint32_t)TEFMOD_DIAG_STATE;

    PHYMOD_MEMCPY(&phy_copy, phy, sizeof(phy_copy));

    /*next figure out the lane num and start_lane based on the input*/
    PHYMOD_IF_ERR_RETURN
        (phymod_util_lane_config_get(&phy->access, &start_lane, &num_lane));

#ifdef INCLUDE_FCMAP
    soc_core = PHYMOD_ACC_USER_ACC(&phy->access);
    pc = EXT_PHY_SW_STATE(soc_core->unit, soc_core->port);
    if (pc->fcmap_enable && (!(pc->flags & PHYCTRL_SYS_SIDE_CTRL))) {
        num_lane = 1;
    }
#endif

    for (i = 0; i < num_lane; i++) {
        phy_copy.access.lane_mask = 0x1 << (i + start_lane);

        switch(type) {
            case TEFMOD_DIAG_CFG:
                 PHYMOD_IF_ERR_RETURN
                    (phy8806x_tsc_display_core_config(&phy_copy.access));
                 PHYMOD_IF_ERR_RETURN
                    (phy8806x_tsc_display_lane_config(&phy_copy.access));
                 break;

            case TEFMOD_DIAG_CL72:
                 PHYMOD_IF_ERR_RETURN
                    (phy8806x_tsc_display_cl93n72_status(&phy_copy.access));
                 break;

            case TEFMOD_DIAG_DEBUG:
                 PHYMOD_IF_ERR_RETURN
                    (phy8806x_tsc_display_lane_debug_status(&phy_copy.access));
                 break;

            case TEFMOD_DIAG_BER:
                 break;

            case TEFMOD_DIAG_STATE:
            default:
                 PHYMOD_IF_ERR_RETURN
                    (phy8806x_tsc_display_core_state_hdr(&phy_copy.access));
                 PHYMOD_IF_ERR_RETURN
                    (phy8806x_tsc_display_core_state_line(&phy_copy.access));
                 PHYMOD_IF_ERR_RETURN
                    (phy8806x_tsc_display_core_state(&phy_copy.access));
                 PHYMOD_IF_ERR_RETURN
                    (phy8806x_tsc_display_lane_state_hdr(&phy_copy.access));
                 PHYMOD_IF_ERR_RETURN
                    (phy8806x_tsc_display_lane_state(&phy_copy.access));
                 for (j = 0; j < 4; j++) {
                   PHYMOD_IF_ERR_RETURN
                      (phy8806x_tsc_log_full_pmd_state(&phy_copy.access, &lane_st[j]));
                 }
                 PHYMOD_IF_ERR_RETURN
                    (phy8806x_tsc_disp_full_pmd_state(&phy_copy.access, lane_st, 4));
                 PHYMOD_IF_ERR_RETURN
                    (phy8806x_tsc_read_event_log((&phy_copy.access), trace_mem, 2));
                 break;
        }
    }

    return PHYMOD_E_NONE;
}

STATIC err_code_t phy8806x_phy_meas_lowber_eye (const phymod_access_t *pa, 
                                     const phymod_phy_eyescan_options_t* eyescan_options,
                                     uint32_t *buffer) 
{
    struct phy8806x_tsc_eyescan_options_st e_options;

    e_options.linerate_in_khz = eyescan_options->linerate_in_khz;
    e_options.timeout_in_milliseconds = eyescan_options->timeout_in_milliseconds;
    e_options.horz_max = eyescan_options->horz_max;
    e_options.horz_min = eyescan_options->horz_min;
    e_options.hstep = eyescan_options->hstep;
    e_options.vert_max = eyescan_options->vert_max;
    e_options.vert_min = eyescan_options->vert_min;
    e_options.vstep = eyescan_options->vstep;
    e_options.mode = eyescan_options->mode;

    return (phy8806x_tsc_meas_lowber_eye(pa, e_options, buffer));
}

STATIC err_code_t phy8806x_phy_display_lowber_eye (const phymod_access_t *pa, 
                                        const phymod_phy_eyescan_options_t* eyescan_options,
                                        uint32_t *buffer) 
{
    struct phy8806x_tsc_eyescan_options_st e_options;

    e_options.linerate_in_khz = eyescan_options->linerate_in_khz;
    e_options.timeout_in_milliseconds = eyescan_options->timeout_in_milliseconds;
    e_options.horz_max = eyescan_options->horz_max;
    e_options.horz_min = eyescan_options->horz_min;
    e_options.hstep = eyescan_options->hstep;
    e_options.vert_max = eyescan_options->vert_max*2;
    e_options.vert_min = eyescan_options->vert_min*2;
    e_options.vstep = eyescan_options->vstep*2;
    e_options.mode = eyescan_options->mode;

    return(phy8806x_tsc_display_lowber_eye (pa, e_options, buffer)); 
} 

static void _phy8806x_diag_uc_reg_dump(const phymod_access_t *pa)
{
    err_code_t errc;

    COMPILER_REFERENCE(errc);

    PHYMOD_DIAG_OUT(("+-------------------------------------------------+\n"));
    PHYMOD_DIAG_OUT(("|    MICRO CODE USR CTRL CONFIGURATION REGISTERS  |\n"));
    PHYMOD_DIAG_OUT(("+-------------------------------------------------+\n"));
    PHYMOD_DIAG_OUT(("|    config_word              [0x00]: 0x%04X      |\n",    phy8806x_tsc_rdwl_uc_var(pa,&errc,0x0)));
    PHYMOD_DIAG_OUT(("|    retune_after_restart     [0x02]: 0x%04X      |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x2)));
    PHYMOD_DIAG_OUT(("|    clk90_offset_adjust      [0x03]: 0x%04X      |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x3)));
    PHYMOD_DIAG_OUT(("|    clk90_offset_override    [0x04]: 0x%04X      |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x4)));
    PHYMOD_DIAG_OUT(("|    lane_event_log_level     [0x05]: 0x%04X      |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x5)));
    PHYMOD_DIAG_OUT(("|    disable_startup          [0x06]: 0x%04X      |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x6)));
    PHYMOD_DIAG_OUT(("|    disable_startup_dfe      [0x07]: 0x%04X      |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x7)));
    PHYMOD_DIAG_OUT(("|    disable_steady_state     [0x08]: 0x%04X      |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x8)));
    PHYMOD_DIAG_OUT(("|    disable_steady_state_dfe [0x09]: 0x%04X      |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x9)));
    PHYMOD_DIAG_OUT(("+-------------------------------------------------+\n"));
    PHYMOD_DIAG_OUT(("|         MICRO CODE USER STATUS REGISTERS        |\n"));
    PHYMOD_DIAG_OUT(("+-------------------------------------------------+\n"));
    PHYMOD_DIAG_OUT(("|    restart_counter           [0x0A]: 0x%04X     |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0xa)));
    PHYMOD_DIAG_OUT(("|    reset_counter             [0x0B]: 0x%04X     |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0xb)));
    PHYMOD_DIAG_OUT(("|    pmd_lock_counter          [0x0C]: 0x%04X     |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0xc)));
    PHYMOD_DIAG_OUT(("|    heye_left                 [0x0D]: 0x%04X     |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0xd)));
    PHYMOD_DIAG_OUT(("|    heye_left_lsbyte          [0x0E]: 0x%04X     |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0xe)));
    PHYMOD_DIAG_OUT(("|    heye_right                [0x0F]: 0x%04X     |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0xf)));
    PHYMOD_DIAG_OUT(("|    heye_right_lsbyte         [0x10]: 0x%04X     |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x10)));
    PHYMOD_DIAG_OUT(("|    veye_upper                [0x11]: 0x%04X     |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x11)));
    PHYMOD_DIAG_OUT(("|    veye_upper_lsbyte         [0x12]: 0x%04X     |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x12)));
    PHYMOD_DIAG_OUT(("|    veye_lower                [0x13]: 0x%04X     |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x13)));
    PHYMOD_DIAG_OUT(("|    veye_lower_lsbyte         [0x14]: 0x%04X     |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x14)));
    PHYMOD_DIAG_OUT(("|    micro_stopped             [0x15]: 0x%04X     |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x15)));
    PHYMOD_DIAG_OUT(("|    link_time                 [0x16]: 0x%04X     |\n",    phy8806x_tsc_rdwl_uc_var(pa,&errc,0x16)));
    PHYMOD_DIAG_OUT(("+-------------------------------------------------+\n"));
    PHYMOD_DIAG_OUT(("|            MICRO CODE MISC REGISTERS            |\n"));
    PHYMOD_DIAG_OUT(("+-------------------------------------------------+\n"));
    PHYMOD_DIAG_OUT(("|    usr_diag_status           [0x18]: 0x%04X     |\n",    phy8806x_tsc_rdwl_uc_var(pa,&errc,0x18)));
    PHYMOD_DIAG_OUT(("|    usr_diag_rd_ptr           [0x1A]: 0x%04X     |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x1a)));
    PHYMOD_DIAG_OUT(("|    usr_diag_mode             [0x1B]: 0x%04X     |\n",    phy8806x_tsc_rdbl_uc_var(pa,&errc,0x1b)));
    PHYMOD_DIAG_OUT(("|    usr_var_msb               [0x1C]: 0x%04X     |\n",    phy8806x_tsc_rdwl_uc_var(pa,&errc,0x1c)));
    PHYMOD_DIAG_OUT(("|    usr_var_lsb               [0x1E]: 0x%04X     |\n",    phy8806x_tsc_rdwl_uc_var(pa,&errc,0x1e)));
    PHYMOD_DIAG_OUT(("+-------------------------------------------------+\n"));
}

STATIC int phy8806x_diagnostics_eyescan_run_uc(const phymod_phy_access_t* phy, uint32_t flags)   
{
    int                 ii, rc = PHYMOD_E_NONE;
    uint32_t            stripe[64];
    uint16_t            status;
    int                 j ;
    phymod_phy_access_t phy_copy;

    PHYMOD_MEMCPY(&phy_copy, phy, sizeof(phy_copy));

    for(j=0; j< PHYMOD_CONFIG_MAX_LANES_PER_CORE; j++) { /* Loop for all lanes. */
        if ((phy->access.lane_mask & (1<<j))==0) continue;

        phy_copy.access.lane_mask = (phy->access.lane_mask & (1<<j));

        if(PHYMOD_EYESCAN_F_ENABLE_GET(flags)) {
            PHYMOD_IF_ERR_RETURN(phy8806x_tsc_meas_eye_scan_start(&(phy_copy.access), 0));

            if(!PHYMOD_EYESCAN_F_ENABLE_DONT_WAIT_GET(flags)) {
                PHYMOD_USLEEP(100000);
            }
        }

        if(PHYMOD_EYESCAN_F_PROCESS_GET(flags)) {

            for (ii = 62; ii >= -62; ii=ii-2) { 
                if (ii == 62) {
                    PHYMOD_DIAG_OUT(("\n\n\n"));
                    PHYMOD_DIAG_OUT(("    +--------------------------------------------------------------------+\n"));
                    PHYMOD_DIAG_OUT(("    | EYESCAN Phy: 0x%03x lane_mask: 0x%02x                                 |\n", phy_copy.access.addr, phy_copy.access.lane_mask));
                    PHYMOD_DIAG_OUT(("    +--------------------------------------------------------------------+\n"));
                    /*display eyescan header*/
                    PHYMOD_IF_ERR_RETURN(phy8806x_tsc_display_eye_scan_header(&(phy_copy.access), 1));
                }

                rc  = phy8806x_tsc_read_eye_scan_stripe(&(phy_copy.access), stripe, &status);
                if(rc != PHYMOD_E_NONE) {
                    _phy8806x_diag_uc_reg_dump(&(phy_copy.access));
                    PHYMOD_IF_ERR_RETURN(rc);
                }
                PHYMOD_IF_ERR_RETURN(phy8806x_tsc_display_eye_scan_stripe(&(phy_copy.access), ii, &stripe[0]));

                PHYMOD_DIAG_OUT(("\n"));
            }

            if (rc == PHYMOD_E_NONE) {
                PHYMOD_IF_ERR_RETURN(phy8806x_tsc_display_eye_scan_footer(&(phy_copy.access), 1));
                PHYMOD_DIAG_OUT(("\n"));
            }
        }

        if(PHYMOD_EYESCAN_F_DONE_GET(flags)) {
            PHYMOD_IF_ERR_RETURN(phy8806x_tsc_meas_eye_scan_done(&(phy_copy.access)));
        }
    }
    return PHYMOD_E_NONE;
}

STATIC int phy8806x_diagnostics_eyescan_run_lowber( 
    const phymod_phy_access_t* phy,
    uint32_t flags,
    const phymod_phy_eyescan_options_t* eyescan_options
    )   
{
    uint32_t *buffer;
    int buffer_size = 64*64;
    int rv = PHYMOD_E_NONE;

    buffer = sal_alloc(sizeof(uint32_t) * buffer_size, "buffer");
    if (NULL == buffer) {
        return PHYMOD_E_MEMORY;
    }
    sal_memset(buffer, 0, sizeof(uint32_t) * buffer_size);

    /*enable eyescan*/
    if(PHYMOD_EYESCAN_F_PROCESS_GET(flags)) {

        rv = phy8806x_phy_meas_lowber_eye (&(phy->access), eyescan_options, buffer);
        if (rv < 0) {
            goto cleanup;
        }
        rv = phy8806x_phy_display_lowber_eye (&(phy->access), eyescan_options, buffer);
        if (rv < 0) {
            goto cleanup;
        }
    }
    if (buffer != NULL) {
        sal_free(buffer);
    }

    if(PHYMOD_EYESCAN_F_DONE_GET(flags)) {
        PHYMOD_IF_ERR_RETURN(phy8806x_tsc_pmd_uc_cmd(&(phy->access), CMD_CAPTURE_BER_END, 0, 2000));
    }

    return PHYMOD_E_NONE;

cleanup:
    if (buffer != NULL) {
        sal_free(buffer);
    }

    return rv;
}

int phy8806x_phy_eyescan_run(const phymod_phy_access_t* phy, 
                           uint32_t flags, 
                           phymod_eyescan_mode_t mode, 
                           const phymod_phy_eyescan_options_t* eyescan_options)
{
    uint8_t  pmd_rx_lock=0;

    PHYMOD_IF_ERR_RETURN
      (phy8806x_tsc_pmd_lock_status(&phy->access, &pmd_rx_lock));
    if(pmd_rx_lock == 0) {
      PHYMOD_RETURN_WITH_ERR(PHYMOD_E_INTERNAL, (_PHYMOD_MSG("Can not get eyescan when pmd_rx is not locked\n")));
    }

    /* If stage isn't set - perform all stages*/
    if(!PHYMOD_EYESCAN_F_ENABLE_GET(flags)
       && !PHYMOD_EYESCAN_F_PROCESS_GET(flags)
       && !PHYMOD_EYESCAN_F_DONE_GET(flags)) 
    {
        PHYMOD_EYESCAN_F_ENABLE_SET(flags);
        PHYMOD_EYESCAN_F_PROCESS_SET(flags);
        PHYMOD_EYESCAN_F_DONE_SET(flags);
    }

    switch(mode) {
        case phymodEyescanModeFast:
            return phy8806x_diagnostics_eyescan_run_uc(phy, flags);
        case phymodEyescanModeLowBER:
            return phy8806x_diagnostics_eyescan_run_lowber(phy, flags, eyescan_options);
        default:
            PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("unsupported eyescan mode %u"), mode));
    }
}


#endif /* PHYMOD_PHY8806X_SUPPORT */
