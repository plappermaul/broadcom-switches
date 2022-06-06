/*
* 
* This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
* 
* Copyright 2007-2020 Broadcom Inc. All rights reserved.
*
* $File:  maverick2_idb_flexport.c
*/


#include <shared/bsl.h>
#include <soc/drv.h>
#include <soc/defs.h>
#include <soc/mem.h>
#include <soc/esw/port.h>
#include <soc/tdm/core/tdm_top.h>


#if defined(BCM_MAVERICK2_SUPPORT)
#include <soc/maverick2.h>
#include <soc/maverick2.h>
#include <soc/flexport/maverick2/maverick2_flexport.h>

/*** START SDK API COMMON CODE ***/


/*! @file maverick2_idb_flexport.c
 *  @brief IDB flex for Maverick2.
 *  Details are shown below.
 */


/* Bubble MOP */
typedef struct soc_maverick2_bmop_s {
    uint8 line_rate[SOC_SWITCH_BYPASS_MODE_LOW + 1];   /* Line-rate profile :: 
                                                        * 0->FULL, 1->L3, 2->L2 */
    uint8 oversub[SOC_SWITCH_BYPASS_MODE_LOW + 1];     /* Oversub profile :: 
                                                        * 0->FULL, 1->L3, 2->L2 */
} soc_maverick2_bmop_t;

static const struct soc_maverick2_bmop_cfg_s {
    int speed;
    soc_maverick2_bmop_t bmop_enable;                  /* Bubble MOP enable */
} soc_maverick2_bmop_cfg_tbl[] = {
    {    -1, {{0, 0, 0}, {0, 0, 0}}},       /* SAF     */
    { 10000, {{0, 0, 0}, {0, 0, 0}}},       /* 10GE    */
    { 11000, {{0, 0, 0}, {0, 0, 0}}},       /* HG[11]  */
    { 20000, {{1, 0, 0}, {1, 0, 0}}},       /* 20GE    */
    { 21000, {{1, 0, 0}, {1, 0, 0}}},       /* HG[21]  */
    { 25000, {{1, 0, 0}, {1, 0, 0}}},       /* 25GE    */
    { 27000, {{1, 0, 0}, {1, 0, 0}}},       /* HG[27]  */
    { 40000, {{1, 0, 0}, {1, 0, 0}}},       /* 40GE    */
    { 42000, {{1, 0, 0}, {1, 0, 0}}},       /* HG[42]  */
    { 50000, {{1, 0, 0}, {1, 0, 0}}},       /* 50GE    */
    { 53000, {{1, 0, 0}, {1, 0, 0}}},       /* HG[53]  */
    {100000, {{1, 0, 0}, {1, 0, 0}}},       /* 100GE   */
    {106000, {{1, 0, 0}, {1, 0, 0}}}        /* HG[106] */
};

static const struct soc_maverick2_obm_setting_s {
    int discard_limit;
    int lossless_discard;
    int port_xoff;
    int port_xon;
    int lossless_xoff;
    int lossless_xon;
    int lossy_low_pri;
    int lossy_discard;
    int sop_discard_mode;
} soc_maverick2_obm_settings[2][5] = {
    /* LOSSLESS Settings*/
    { /* indexed by number of lanes used in the port */
        { /* 0 - invalid */
            0,               /* discard_limit */
            0,               /* lossless_discard */
            720,             /* port_xoff : Default value */
            576,             /* port_xon : Default value*/
            384,             /* lossless_xoff : Default value*/
            288,             /* lossless_xon : Default value*/
            0,               /* lossy_low_pri */
            0,               /* lossy_discard */
            1                /* sop_discard_mode */
        },
        { /* 1 lane */
            1148,            /* discard_limit */
            4604,            /* lossless_discard */
            363,             /* port_xoff */
            333,             /* port_xon */
            136,             /* lossless_xoff */
            106,             /* lossless_xon */
            179,             /* lossy_low_pri */
            546,             /* lossy_discard */
            1                /* sop_discard_mode */
        },
        { /* 2 lanes */
            2300,            /* discard_limit */
            4604,            /* lossless_discard */
            1221,            /* port_xoff */
            1191,            /* port_xon */
            324,             /* lossless_xoff */
            294,             /* lossless_xon */
            255,             /* lossy_low_pri */
            622,             /* lossy_discard */
            1                /* sop_discard_mode */
        },
        { /* 3 - invalid  */
            0,               /* discard_limit */
            0,               /* lossless_discard */
            720,             /* port_xoff */
            576,             /* port_xon */
            384,             /* lossless_xoff */
            288,             /* lossless_xon */
            0,               /* lossy_low_pri */
            0,               /* lossy_discard */
            1                /* sop_discard_mode */
        },
        { /* 4 lanes */
            4604,            /* discard_limit */
            4604,            /* lossless_discard */
            2844,            /* port_xoff */
            2814,            /* port_xon */
            774,             /* lossless_xoff */
            744,             /* lossless_xon */
            255,             /* lossy_low_pri */
            546,             /* lossy_discard */
            1                /* sop_discard_mode */
        }
    },
    /* LOSSY Settings*/
    { /* indexed by number of lanes used in the port */
        { /* 0 - invalid */
            0,               /* discard_limit */
            0,               /* lossless_discard */
            720,             /* port_xoff : Default value */
            576,             /* port_xon : Default value*/
            384,             /* lossless_xoff : Default value*/
            288,             /* lossless_xon : Default value*/
            0,               /* lossy_low_pri */
            0,               /* lossy_discard */
            1                /* sop_discard_mode */
        },
        { /* 1 lane */
            1148,            /* discard_limit */
            4604,            /* lossless_discard */
            4604,            /* port_xoff */
            4604,            /* port_xon */
            4604,            /* lossless_xoff */
            4604,            /* lossless_xon */
            777,             /* lossy_low_pri */
            1144,            /* lossy_discard */
            1                /* sop_discard_mode */
        },
        { /* 2 lanes */
            2300,            /* discard_limit */
            4604,            /* lossless_discard */
            4604,            /* port_xoff */
            4604,            /* port_xon */
            4604,            /* lossless_xoff */
            4604,            /* lossless_xon */
            1929,            /* lossy_low_pri */
            2296,            /* lossy_discard */
            1                /* sop_discard_mode */
        },
        { /* 3 - invalid  */
            0,               /* discard_limit */
            0,               /* lossless_discard */
            720,             /* port_xoff */
            576,             /* port_xon */
            384,             /* lossless_xoff */
            288,             /* lossless_xon */
            0,               /* lossy_low_pri */
            0,               /* lossy_discard */
            1                /* sop_discard_mode */
        },
        { /* 4 lanes */
            4604,            /* discard_limit */
            4604,            /* lossless_discard */
            4604,            /* port_xoff */
            4604,            /* port_xon */
            4604,            /* lossless_xoff */
            4604,            /* lossless_xon */
            4233,            /* lossy_low_pri */
            4600,            /* lossy_discard */
            1                /* sop_discard_mode */
        }
    }
};

static const struct soc_maverick2_fsaf_dur_timer {
    int frequency;
    /* ForceSAF detection duration timer. It is 1.5 * core clk in MHz */
    int duration_timer;             
} soc_maverick2_fsaf_dur_timer_tbl[] = {
    { 1525,  2288 },
    { 1425,  2138 },
    { 1325,  1988 },
    { 1012,  2550 },
    {  850,  1275 },      /*MAVERICK2_FORCE_SAF_TIMER_ENTRY_CNT=5*/
    {   -1,  12000}
};


static const soc_reg_t soc_maverick2_obm_ca_ctrl_regs[MAVERICK2_PIPES_PER_DEV][
    MAVERICK2_PBLKS_PER_PIPE] = {
    {IDB_OBM0_CA_CONTROL_PIPE0r, IDB_OBM1_CA_CONTROL_PIPE0r,
     IDB_OBM2_CA_CONTROL_PIPE0r, IDB_OBM3_CA_CONTROL_PIPE0r,
     IDB_OBM4_CA_CONTROL_PIPE0r, IDB_OBM5_CA_CONTROL_PIPE0r,
     IDB_OBM6_CA_CONTROL_PIPE0r, IDB_OBM7_CA_CONTROL_PIPE0r,
     IDB_OBM8_CA_CONTROL_PIPE0r, IDB_OBM9_CA_CONTROL_PIPE0r,
     IDB_OBM10_CA_CONTROL_PIPE0r, IDB_OBM11_CA_CONTROL_PIPE0r,
     IDB_OBM12_CA_CONTROL_PIPE0r, IDB_OBM13_CA_CONTROL_PIPE0r,
     IDB_OBM14_CA_CONTROL_PIPE0r, IDB_OBM15_CA_CONTROL_PIPE0r,
     IDB_OBM16_CA_CONTROL_PIPE0r, IDB_OBM17_CA_CONTROL_PIPE0r,
     IDB_OBM18_CA_CONTROL_PIPE0r, IDB_OBM19_CA_CONTROL_PIPE0r}
};

static const soc_reg_t obm_ctrl_regs[MAVERICK2_PIPES_PER_DEV][
    MAVERICK2_PBLKS_PER_PIPE] = {
    {IDB_OBM0_CONTROL_PIPE0r, IDB_OBM1_CONTROL_PIPE0r, IDB_OBM2_CONTROL_PIPE0r,
     IDB_OBM3_CONTROL_PIPE0r,
     IDB_OBM4_CONTROL_PIPE0r, IDB_OBM5_CONTROL_PIPE0r, IDB_OBM6_CONTROL_PIPE0r,
     IDB_OBM7_CONTROL_PIPE0r,
     IDB_OBM8_CONTROL_PIPE0r, IDB_OBM9_CONTROL_PIPE0r, IDB_OBM10_CONTROL_PIPE0r,
     IDB_OBM11_CONTROL_PIPE0r,
     IDB_OBM12_CONTROL_PIPE0r, IDB_OBM13_CONTROL_PIPE0r, IDB_OBM14_CONTROL_PIPE0r,
     IDB_OBM15_CONTROL_PIPE0r,
     IDB_OBM16_CONTROL_PIPE0r, IDB_OBM17_CONTROL_PIPE0r, IDB_OBM18_CONTROL_PIPE0r,
     IDB_OBM19_CONTROL_PIPE0r}
};

/*! @fn int soc_maverick2_get_pipe_from_phy_pnum(int pnum)
 *  @param pnum Physical por number.
 *  @brief Helper function to calculate pipe number from physical port number.
 *  @returns pipe_number
 */
int
soc_maverick2_get_pipe_from_phy_pnum(int pnum)
{
    return 0;
}

/*! @fn int soc_maverick2_get_pm_from_phy_pnum(int pnum)
 *  @param pnum Physical por number.
 *  @brief Helper function to calculate PM number from physical port number.
 *  @returns PM number
 */
int
soc_maverick2_get_pm_from_phy_pnum(int pnum)
{
    /* Return 0 - 19 for straight order for both pipes. */
    return (((pnum-1)&0x7f)>>2);
}

/*! @fn int soc_maverick2_get_subp_from_phy_pnum(int pnum)
 *  @param pnum Physical por number.
 *  @brief Helper function to calculate subport number from physical port number.
 *  @returns subport number
 */
int
soc_maverick2_get_subp_from_phy_pnum(int pnum)
{
    return ((pnum-1)&0x3);
}

/*! @fn int soc_maverick2_idb_clear_stats_new_ports(int unit,
 *              soc_port_schedule_state_t *port_schedule_state)
 *  @param unit Chip unit number.
 *  @param port_schedule_state Pointer to a soc_port_schedule_state_t struct
 *         variable
 *  @brief Helper function to clear statistics of the new ports that are comming
 *         up during flexport operation.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_clear_stats_new_ports(
    int unit, soc_port_schedule_state_t *port_schedule_state)
{
    int prt;
    soc_reg_t reg;
    uint64 rval64;
    int pnum;
    int pipe_num;
    int pm_num;
    int subp;

    const soc_reg_t obm_usage_regs[MAVERICK2_PIPES_PER_DEV][
    MAVERICK2_PBLKS_PER_PIPE] = {
        {IDB_OBM0_MAX_USAGE_PIPE0r,
         IDB_OBM1_MAX_USAGE_PIPE0r,
         IDB_OBM2_MAX_USAGE_PIPE0r,
         IDB_OBM3_MAX_USAGE_PIPE0r,
         IDB_OBM4_MAX_USAGE_PIPE0r,
         IDB_OBM5_MAX_USAGE_PIPE0r,
         IDB_OBM6_MAX_USAGE_PIPE0r,
         IDB_OBM7_MAX_USAGE_PIPE0r,
         IDB_OBM8_MAX_USAGE_PIPE0r,
         IDB_OBM9_MAX_USAGE_PIPE0r,
         IDB_OBM10_MAX_USAGE_PIPE0r,
         IDB_OBM11_MAX_USAGE_PIPE0r,
         IDB_OBM12_MAX_USAGE_PIPE0r,
         IDB_OBM13_MAX_USAGE_PIPE0r,
         IDB_OBM14_MAX_USAGE_PIPE0r,
         IDB_OBM15_MAX_USAGE_PIPE0r,
         IDB_OBM16_MAX_USAGE_PIPE0r,
         IDB_OBM17_MAX_USAGE_PIPE0r,
         IDB_OBM18_MAX_USAGE_PIPE0r,
         IDB_OBM19_MAX_USAGE_PIPE0r}
        };

    for (prt = 0; prt < port_schedule_state->nport; prt++) {
        if (port_schedule_state->resource[prt].physical_port != -1) {
            pnum =
                port_schedule_state->out_port_map.port_l2p_mapping[
                    port_schedule_state
                    ->resource[
                        prt].logical_port];
            pipe_num = soc_maverick2_get_pipe_from_phy_pnum(pnum);
            pm_num = soc_maverick2_get_pm_from_phy_pnum(pnum);
            subp = soc_maverick2_get_subp_from_phy_pnum(pnum);

            reg = obm_usage_regs[pipe_num][pm_num];
            COMPILER_64_ZERO(rval64);
            SOC_IF_ERROR_RETURN(soc_reg_rawport_get(unit, reg,
                                                    REG_PORT_ANY, 0,
                                                    &rval64));
            switch(subp) {
            case 0 : 
                soc_reg64_field32_set(unit, reg, &rval64, PORT0_MAX_USAGEf, 0);
                break;
            case 1 : 
                soc_reg64_field32_set(unit, reg, &rval64, PORT1_MAX_USAGEf, 0);
                break;
            case 2 : 
                soc_reg64_field32_set(unit, reg, &rval64, PORT2_MAX_USAGEf, 0);
                break;
            case 3 : 
                soc_reg64_field32_set(unit, reg, &rval64, PORT3_MAX_USAGEf, 0);
                break;
            default : 
                soc_reg64_field32_set(unit, reg, &rval64, PORT0_MAX_USAGEf, 0);
                break;
            }
            SOC_IF_ERROR_RETURN(soc_reg_rawport_set(unit, reg,
                                                    REG_PORT_ANY, 0,
                                                    rval64));
        }
    }
    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_reset_buffer(int unit, int pipe_num, int pm_num,
 *              int subp, int reset_buffer)
 *  @param unit Chip unit number.
 *  @param pipe_num Maverick2 pipe number.
 *  @param pm_num PM number within the pipe: Ranges from 0 to 15.
 *  @param subp Subport number within the PM: Ranges from 0 to 3.
 *  @param reset_buffer IDB OBM reset buffer: 1 - Apply reset. 0 - Release reset.
 *  @brief Helper function to apply / or release IDB OBM buffer reset.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_obm_reset_buffer(int unit, int pipe_num, int pm_num, int subp,
                                   int reset_buffer)
{
    soc_reg_t reg;
    uint32 rval;

    reg = obm_ctrl_regs[pipe_num][pm_num];
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_get(unit, reg, REG_PORT_ANY, 0, &rval));
    if (subp == 0) {
        soc_reg_field_set(unit, reg, &rval, PORT0_RESETf, reset_buffer);
    } else if (subp == 1) {
        soc_reg_field_set(unit, reg, &rval, PORT1_RESETf, reset_buffer);
    } else if (subp == 2) {
        soc_reg_field_set(unit, reg, &rval, PORT2_RESETf, reset_buffer);
    } else {
        soc_reg_field_set(unit, reg, &rval, PORT3_RESETf, reset_buffer);
    }
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_set(unit, reg, REG_PORT_ANY, 0, rval));

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_ca_reset_buffer(int unit, int pipe_num, int pm_num,
 *              int subp, int reset_buffer)
 *  @param unit Chip unit number.
 *  @param pipe_num Maverick2 pipe number.
 *  @param pm_num PM number within the pipe: Ranges from 0 to 15.
 *  @param subp Subport number within the PM: Ranges from 0 to 3.
 *  @param reset_buffer IDB Cell Assembly reset buffer: 1 - Apply reset.
 *         0 - Release reset.
 *  @brief Helper function to apply / or release IDB Cell Assembly buffer reset.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_ca_reset_buffer(int unit, int pipe_num, int pm_num, int subp,
                                  int reset_buffer)
{
    soc_reg_t reg;
    uint32 rval;

    reg = soc_maverick2_obm_ca_ctrl_regs[pipe_num][pm_num];
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_get(unit, reg, REG_PORT_ANY, 
                                    0, &rval));
    if (subp == 0) {
        soc_reg_field_set(unit, reg, &rval, PORT0_RESETf, reset_buffer);
    } else if (subp == 1) {
        soc_reg_field_set(unit, reg, &rval, PORT1_RESETf, reset_buffer);
    } else if (subp == 2) {
        soc_reg_field_set(unit, reg, &rval, PORT2_RESETf, reset_buffer);
    } else {
        soc_reg_field_set(unit, reg, &rval, PORT3_RESETf, reset_buffer);
    }
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_set(unit, reg, REG_PORT_ANY, 
                                    0, rval));

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_lpbk_ca_reset_buffer(int unit, int pipe_num, 
                      int reset_buffer)
 *  @param unit Chip unit number.
 *  @param pipe_num Maverick2 pipe number.
 *  @param reset_buffer IDB Cell Assembly reset buffer: 1 - Apply reset.
 *         0 - Release reset.
 *  @brief Helper function to apply / or release IDB Cell Assembly buffer reset
 *                  for loopback port.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_lpbk_ca_reset_buffer(int unit, int pipe_num, int reset_buffer)
{
    soc_reg_t reg;
    uint32 rval;
    const soc_reg_t idb_ca_lpbk_ctrl[MAVERICK2_PIPES_PER_DEV]= {
         IDB_CA_LPBK_CONTROL_PIPE0r
    };


    reg = idb_ca_lpbk_ctrl[pipe_num];
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_get(unit, reg, REG_PORT_ANY, 
                                                        0, &rval));
    soc_reg_field_set(unit, reg, &rval, PORT_RESETf, reset_buffer);
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_set(unit, reg, REG_PORT_ANY, 
                                                        0, rval));

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_cpu_ca_reset_buffer(int unit, int pipe_num, 
                      int reset_buffer)
 *  @param unit Chip unit number.
 *  @param pipe_num Maverick2 pipe number.
 *  @param reset_buffer IDB Cell Assembly reset buffer: 1 - Apply reset.
 *         0 - Release reset.
 *  @brief Helper function to apply / or release IDB Cell Assembly buffer reset
 *                  for CPU port.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_cpu_ca_reset_buffer(int unit, int pipe_num, int reset_buffer)
{
    soc_reg_t reg;
    uint32 rval;
    const soc_reg_t idb_ca_lpbk_ctrl[MAVERICK2_PIPES_PER_DEV]= {
         IDB_CA_CPU_CONTROL_PIPE0r/* ,
         IDB_CA_CPU_CONTROL_PIPE1r */};


    reg = idb_ca_lpbk_ctrl[pipe_num];
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_get(unit, reg, REG_PORT_ANY, 
                                                            0, &rval));
    soc_reg_field_set(unit, reg, &rval, PORT_RESETf, reset_buffer);
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_set(unit, reg, REG_PORT_ANY, 
                                                            0, rval));

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_mgmt_ca_reset_buffer(int unit, int pipe_num, 
                      int reset_buffer)
 *  @param unit Chip unit number.
 *  @param pipe_num Maverick2 pipe number.
 *  @param reset_buffer IDB Cell Assembly reset buffer: 1 - Apply reset.
 *         0 - Release reset.
 *  @brief Helper function to apply / or release IDB Cell Assembly buffer reset
 *                  for CPU port.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_mgmt_ca_reset_buffer(int unit, int pipe_num, int reset_buffer)
{
    soc_reg_t reg;
    uint32 rval;
    const soc_reg_t idb_ca_mgmt_ctrl[MAVERICK2_PIPES_PER_DEV]= {
         IDB_CA_MGMT_CONTROL_PIPE0r/* ,
         IDB_CA_MGMT_CONTROL_PIPE1r */};

    reg = idb_ca_mgmt_ctrl[pipe_num];
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_get(unit, reg, REG_PORT_ANY, 
                                                            0, &rval));
    soc_reg_field_set(unit, reg, &rval, PORT_RESETf, reset_buffer);
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_set(unit, reg, REG_PORT_ANY, 
                                                            0, rval));

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_lpbk_ca_in_reset(int unit, int pipe_num)
 *  @param unit Chip unit number.
 *  @param pipe_num Maverick2 pipe number.
 *  @brief Helper function to check if the IDB loopback CA is in reset.
 *  @returns Reset status.
 */
int
soc_maverick2_idb_lpbk_ca_in_reset(int unit, int pipe_num)
{
    soc_reg_t reg;
    uint32 rval;
    int buffer_in_reset;
    const soc_reg_t idb_ca_lpbk_ctrl[MAVERICK2_PIPES_PER_DEV]= {
         IDB_CA_LPBK_CONTROL_PIPE0r/*,
         IDB_CA_LPBK_CONTROL_PIPE1r */};

    reg = idb_ca_lpbk_ctrl[pipe_num];
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_get(unit, reg, REG_PORT_ANY, 0, &rval));
    buffer_in_reset = soc_reg_field_get(unit, reg, rval, PORT_RESETf);
    return buffer_in_reset;
}

/*! @fn int soc_maverick2_idb_cpu_ca_in_reset(int unit, int pipe_num)
 *  @param unit Chip unit number.
 *  @param pipe_num Maverick2 pipe number.
 *  @brief Helper function to check if the IDB CPU CA is in reset.
 *  @returns Reset status;
 */
int
soc_maverick2_idb_cpu_ca_in_reset(int unit, int pipe_num)
{
    soc_reg_t reg;
    uint32 rval;
    int buffer_in_reset;
    static const soc_reg_t idb_ca_cpu_ctrl[MAVERICK2_PIPES_PER_DEV]= {
         IDB_CA_CPU_CONTROL_PIPE0r/*,
         IDB_CA_CPU_CONTROL_PIPE1r */};
    reg = idb_ca_cpu_ctrl[pipe_num];
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_get(unit, reg, REG_PORT_ANY, 0, &rval));
    buffer_in_reset = soc_reg_field_get(unit, reg, rval, PORT_RESETf);
    return buffer_in_reset;
}

/*! @fn int soc_maverick2_idb_mgmt_ca_in_reset(int unit, int pipe_num)
 *  @param unit Chip unit number.
 *  @param pipe_num Maverick2 pipe number.
 *  @brief Helper function to check if the IDB CPU CA is in reset.
 *  @returns Reset status;
 */
int
soc_maverick2_idb_mgmt_ca_in_reset(int unit, int pipe_num)
{
    soc_reg_t reg;
    uint32 rval;
    int buffer_in_reset;
    static const soc_reg_t idb_ca_cpu_ctrl[MAVERICK2_PIPES_PER_DEV]= {
         IDB_CA_MGMT_CONTROL_PIPE0r/* ,
         IDB_CA_MGMT_CONTROL_PIPE1r */};

    reg = idb_ca_cpu_ctrl[pipe_num];
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_get(unit, reg, REG_PORT_ANY, 0, &rval));
    buffer_in_reset = soc_reg_field_get(unit, reg, rval, PORT_RESETf);
    return buffer_in_reset;
}

/*! @fn int soc_maverick2_idb_port_mode_set(int unit,
 *              soc_port_schedule_state_t *port_schedule_state)
 *  @param unit Chip unit number.
 *  @param port_schedule_state Pointer to a soc_port_schedule_state_t struct
 *         variable
 *  @brief Helper function to program IDB port mode for the new ports that are
 *         comming up during flexport operation.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_port_mode_set(int                        unit,
                                soc_port_schedule_state_t *port_schedule_state)
{
    soc_reg_t reg;
    int i;
    uint32 rval;
    int pnum;
    int pipe_num;
    int pm_num;
    int mode;
    int clport;
    static const int soc_maverick2_hw_mode_values[SOC_MV2_PORT_RATIO_COUNT] = {
        0, 1, 1, 1, 3, 5, 4, 6, 2
    };


    for (i = 0; i < port_schedule_state->nport; i++) {
        if (port_schedule_state->resource[i].physical_port != -1) {
            pnum =
                port_schedule_state->out_port_map.port_l2p_mapping[
                    port_schedule_state
                    ->resource[i
                    ].logical_port];
            clport = ((pnum-1)>>2);
            soc_mv2_tdm_get_port_ratio(unit, port_schedule_state, clport,
                                         &mode,
                                         0);
            pipe_num = soc_maverick2_get_pipe_from_phy_pnum(pnum);
            pm_num = soc_maverick2_get_pm_from_phy_pnum(pnum);
            reg = soc_maverick2_obm_ca_ctrl_regs[pipe_num][pm_num];
            SOC_IF_ERROR_RETURN(soc_reg32_rawport_get(unit, reg, REG_PORT_ANY,
                                                      0, &rval));
            soc_reg_field_set(unit, reg, &rval, PORT_MODEf,
                              soc_maverick2_hw_mode_values[mode]);
            SOC_IF_ERROR_RETURN(soc_reg32_rawport_set(unit, reg, REG_PORT_ANY,
                                                      0, rval));
        }
    }

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_wr_obm_shared_config(int unit, int pipe_num,
 *              int pm_num, int subp, int num_lanes)
 *  @param unit Chip unit number.
 *  @param pipe_num Pipe number.
 *  @param pm_num Port Macro number.
 *  @param subp Sub-port number.
 *  @param num_lanes Number of lanes in the port
 *  @param lossless Chip mode - 1: Lossless; 0: Lossy.
 *  @brief Helper function to config OBM shared buffer during flexing
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_wr_obm_shared_config(int unit, int pipe_num, int pm_num,
                                       int subp,
                                       int num_lanes, int lossless)
{
    soc_reg_t reg;
    int sop_discard_mode;
    uint32 rval;

    const soc_reg_t obm_shared_cfg[MAVERICK2_PIPES_PER_DEV][
        MAVERICK2_PBLKS_PER_PIPE] = {
        {IDB_OBM0_SHARED_CONFIG_PIPE0r, IDB_OBM1_SHARED_CONFIG_PIPE0r,
         IDB_OBM2_SHARED_CONFIG_PIPE0r, IDB_OBM3_SHARED_CONFIG_PIPE0r,
         IDB_OBM4_SHARED_CONFIG_PIPE0r, IDB_OBM5_SHARED_CONFIG_PIPE0r,
         IDB_OBM6_SHARED_CONFIG_PIPE0r, IDB_OBM7_SHARED_CONFIG_PIPE0r,
         IDB_OBM8_SHARED_CONFIG_PIPE0r, IDB_OBM9_SHARED_CONFIG_PIPE0r,
         IDB_OBM10_SHARED_CONFIG_PIPE0r, IDB_OBM11_SHARED_CONFIG_PIPE0r,
         IDB_OBM12_SHARED_CONFIG_PIPE0r, IDB_OBM13_SHARED_CONFIG_PIPE0r,
         IDB_OBM14_SHARED_CONFIG_PIPE0r, IDB_OBM15_SHARED_CONFIG_PIPE0r,
         IDB_OBM16_SHARED_CONFIG_PIPE0r, IDB_OBM17_SHARED_CONFIG_PIPE0r,
         IDB_OBM18_SHARED_CONFIG_PIPE0r, IDB_OBM19_SHARED_CONFIG_PIPE0r}
    };

    reg = obm_shared_cfg[pipe_num][pm_num];
    if (lossless) {
        sop_discard_mode =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSLESS_INDEX]
            [num_lanes].sop_discard_mode;
    } else {
        sop_discard_mode =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSY_INDEX]
            [num_lanes].sop_discard_mode;
    }

    rval = 0;
    soc_reg_field_set(unit, reg, &rval, SOP_DISCARD_MODEf, sop_discard_mode);
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_set(unit, reg, REG_PORT_ANY, 
                                        0, rval));
    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_wr_obm_flow_ctrl_cfg(int unit, int pipe_num,
 *              int pm_num, int subp, int num_lanes)
 *  @param unit Chip unit number.
 *  @param pipe_num Pipe number.
 *  @param pm_num Port Macro number.
 *  @param subp Sub-port number.
 *  @brief Helper function to config OBM shared buffer during flexing
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_wr_obm_flow_ctrl_cfg(int unit, int pipe_num, int pm_num,
                                       int subp, int lossless)
{
    soc_reg_t reg;
    uint64 rval64;

    const soc_reg_t obm_flow_ctrl_cfg[MAVERICK2_PIPES_PER_DEV][
            MAVERICK2_PBLKS_PER_PIPE] = {
        {IDB_OBM0_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM1_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM2_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM3_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM4_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM5_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM6_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM7_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM8_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM9_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM10_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM11_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM12_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM13_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM14_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM15_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM16_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM17_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM18_FLOW_CONTROL_CONFIG_PIPE0r,
         IDB_OBM19_FLOW_CONTROL_CONFIG_PIPE0r}
        };

    COMPILER_64_ZERO(rval64);
    reg = obm_flow_ctrl_cfg[pipe_num][pm_num];
    SOC_IF_ERROR_RETURN(soc_reg_rawport_get(unit, reg, REG_PORT_ANY, subp,
                                            &rval64));
    if (lossless) {
        soc_reg64_field32_set(unit, reg, &rval64, XOFF_REFRESH_TIMERf, 256);
        soc_reg64_field32_set(unit, reg, &rval64, FC_TYPEf, 0);
        soc_reg64_field32_set(unit, reg, &rval64, PORT_FC_ENf, 1);
        soc_reg64_field32_set(unit, reg, &rval64, LOSSLESS0_FC_ENf, 1);
        soc_reg64_field32_set(unit, reg, &rval64, LOSSLESS1_FC_ENf, 0);
        soc_reg64_field32_set(unit, reg, &rval64, 
                                    LOSSLESS0_PRIORITY_PROFILEf, 0xffff);
        soc_reg64_field32_set(unit, reg, &rval64, 
                                    LOSSLESS1_PRIORITY_PROFILEf, 0);
    } else {
        soc_reg64_field32_set(unit, reg, &rval64, XOFF_REFRESH_TIMERf, 256);
        soc_reg64_field32_set(unit, reg, &rval64, FC_TYPEf, 0);
        soc_reg64_field32_set(unit, reg, &rval64, PORT_FC_ENf, 0);
        soc_reg64_field32_set(unit, reg, &rval64, LOSSLESS0_FC_ENf, 0);
        soc_reg64_field32_set(unit, reg, &rval64, LOSSLESS1_FC_ENf, 0);
        soc_reg64_field32_set(unit, reg, &rval64, LOSSLESS0_PRIORITY_PROFILEf, 0);
        soc_reg64_field32_set(unit, reg, &rval64, LOSSLESS1_PRIORITY_PROFILEf, 0);
    }
    SOC_IF_ERROR_RETURN(soc_reg_rawport_set(unit, reg, REG_PORT_ANY, subp,
                                            rval64));

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_wr_obm_port_ca_sop(int unit, int pipe_num, int pm_num,
 *              int subp)
 *  @param unit Chip unit number.
 *  @param pipe_num Pipe number.
 *  @param pm_num Port Macro number.
 *  @param subp Sub-port numbber.
 *  @brief Helper function to config OBM CONTROL register during flexing
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_wr_obm_port_ca_sop(int unit, int pipe_num, int pm_num,
                                     int subp, int speed)
{
    soc_reg_t reg;
    uint32 rval, ca_sop;

    reg = obm_ctrl_regs[pipe_num][pm_num];
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_get(unit, reg, REG_PORT_ANY,
                                              0, &rval));
    ca_sop = (speed >= 100000) ? 1 : 0;
    if (subp == 0) {
        soc_reg_field_set(unit, reg, &rval, PORT0_CA_SOPf, ca_sop);
    } else if (subp == 1) {
        soc_reg_field_set(unit, reg, &rval, PORT1_CA_SOPf, ca_sop);
    } else if (subp == 2) {
        soc_reg_field_set(unit, reg, &rval, PORT2_CA_SOPf, ca_sop);
    } else {
        soc_reg_field_set(unit, reg, &rval, PORT3_CA_SOPf, ca_sop);
    }
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_set(unit, reg, REG_PORT_ANY,
                                              0, rval));

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_wr_obm_ovs_en(int unit, int pipe_num, int pm_num,
 *              int subp)
 *  @param unit Chip unit number.
 *  @param pipe_num Pipe number.
 *  @param pm_num Port Macro number.
 *  @param subp Sub-port numbber.
 *  @brief Helper function to config OBM CONTROL register during flexing
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_wr_obm_ovs_en(int unit, int pipe_num, int pm_num, int subp)
{
    soc_reg_t reg;
    uint32 rval;

    reg = obm_ctrl_regs[pipe_num][pm_num];
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_get(unit,
                                    reg, REG_PORT_ANY, 0, &rval));
    if (subp == 0) {
        soc_reg_field_set(unit, reg, &rval, PORT0_OVERSUB_ENABLEf, 1);
    } else if (subp == 1) {
        soc_reg_field_set(unit, reg, &rval, PORT1_OVERSUB_ENABLEf, 1);
    } else if (subp == 2) {
        soc_reg_field_set(unit, reg, &rval, PORT2_OVERSUB_ENABLEf, 1);
    } else {
        soc_reg_field_set(unit, reg, &rval, PORT3_OVERSUB_ENABLEf, 1);
    }
    SOC_IF_ERROR_RETURN(soc_reg32_rawport_set(unit, reg, REG_PORT_ANY, 
                                            0, rval));

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_wr_obm_buffer_limit(int unit, int pipe_num, int pm_num,
 *              int subp, int num_lanes)
 *  @param unit Chip unit number.
 *  @param pipe_num Pipe number.
 *  @param pm_num Port Macro number.
 *  @param subp Sub-port numbber.
 *  @param num_lanes Number of serdes lanes used in the port.
 *  @brief Helper function to config OBM buffer start & end during flexing
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_wr_obm_buffer_limit(int unit, int pipe_num, int pm_num, int subp,
                                int num_lanes)
{
    soc_reg_t reg;
    uint64 rval64;
    uint32 buffer_start, buffer_end;

    const soc_reg_t obm_buffer_config[MAVERICK2_PIPES_PER_DEV][
        MAVERICK2_PBLKS_PER_PIPE] = {
        {IDB_OBM0_BUFFER_CONFIG_PIPE0r,
         IDB_OBM1_BUFFER_CONFIG_PIPE0r,
         IDB_OBM2_BUFFER_CONFIG_PIPE0r,
         IDB_OBM3_BUFFER_CONFIG_PIPE0r,
         IDB_OBM4_BUFFER_CONFIG_PIPE0r,
         IDB_OBM5_BUFFER_CONFIG_PIPE0r,
         IDB_OBM6_BUFFER_CONFIG_PIPE0r,
         IDB_OBM7_BUFFER_CONFIG_PIPE0r,
         IDB_OBM8_BUFFER_CONFIG_PIPE0r,
         IDB_OBM9_BUFFER_CONFIG_PIPE0r,
         IDB_OBM10_BUFFER_CONFIG_PIPE0r,
         IDB_OBM11_BUFFER_CONFIG_PIPE0r,
         IDB_OBM12_BUFFER_CONFIG_PIPE0r,
         IDB_OBM13_BUFFER_CONFIG_PIPE0r,
         IDB_OBM14_BUFFER_CONFIG_PIPE0r,
         IDB_OBM15_BUFFER_CONFIG_PIPE0r,
         IDB_OBM16_BUFFER_CONFIG_PIPE0r,
         IDB_OBM17_BUFFER_CONFIG_PIPE0r,
         IDB_OBM18_BUFFER_CONFIG_PIPE0r,
         IDB_OBM19_BUFFER_CONFIG_PIPE0r}
    };


    buffer_start    = subp * MAVERICK2_OBM_BUFFER_PER_LANE;

    if(num_lanes == 0)  buffer_end = buffer_start;
    else                buffer_end = buffer_start + 
                            (num_lanes * MAVERICK2_OBM_BUFFER_PER_LANE) - 1;
    LOG_DEBUG(BSL_LS_SOC_PORT, (BSL_META_U(unit,
            "soc_maverick2_idb_wr_obm_buffer_limit: pipe: %1d, pm_num: %1d,"
            "subp: %1d, num_lanes: %1d, buffer_start: %1d, buffer_end: %1d \n"),
              pipe_num, pm_num, subp, num_lanes, buffer_start, buffer_end));

    COMPILER_64_ZERO(rval64);
    reg = obm_buffer_config[pipe_num][pm_num];
    SOC_IF_ERROR_RETURN(soc_reg_rawport_get(unit, reg, REG_PORT_ANY,
                                            subp, &rval64));
    soc_reg64_field32_set(unit, reg, &rval64, BUFFER_STARTf,
                                            buffer_start);
    soc_reg64_field32_set(unit, reg, &rval64, BUFFER_ENDf,
                                            buffer_end);
    SOC_IF_ERROR_RETURN(soc_reg_rawport_set(unit, reg, REG_PORT_ANY,
                                            subp, rval64));
    
    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_wr_obm_thresh(int unit, int pipe_num, int pm_num,
 *              int subp)
 *  @param unit Chip unit number.
 *  @param pipe_num Pipe number.
 *  @param pm_num Port Macro number.
 *  @param subp Sub-port numbber.
 *  @param num_lanes Number of serdes lanes used in the port.
 *  @param lossless Chip mode - 1: Lossless; 0: Lossy.
 *  @brief Helper function to config OBM thresholds during flexing
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_wr_obm_thresh(int unit, int pipe_num, int pm_num, int subp,
                                int num_lanes, int lossless)
{
    soc_reg_t reg;
    int i;
    uint32 lossless0_discard, lossy_discard, lossy_low_pri,
           discard_limit, lossless1_discard;
    uint64 rval64;

    const soc_reg_t obm_thresh_pp[MAVERICK2_PIPES_PER_DEV][
        MAVERICK2_PBLKS_PER_PIPE][2] = {
        {{IDB_OBM0_THRESHOLD_PIPE0r,
          IDB_OBM0_THRESHOLD_1_PIPE0r},
         {IDB_OBM1_THRESHOLD_PIPE0r, IDB_OBM1_THRESHOLD_1_PIPE0r},
         {IDB_OBM2_THRESHOLD_PIPE0r,
          IDB_OBM2_THRESHOLD_1_PIPE0r},
         {IDB_OBM3_THRESHOLD_PIPE0r, IDB_OBM3_THRESHOLD_1_PIPE0r},
         {IDB_OBM4_THRESHOLD_PIPE0r,
          IDB_OBM4_THRESHOLD_1_PIPE0r},
         {IDB_OBM5_THRESHOLD_PIPE0r, IDB_OBM5_THRESHOLD_1_PIPE0r},
         {IDB_OBM6_THRESHOLD_PIPE0r,
          IDB_OBM6_THRESHOLD_1_PIPE0r},
         {IDB_OBM7_THRESHOLD_PIPE0r, IDB_OBM7_THRESHOLD_1_PIPE0r},
         {IDB_OBM8_THRESHOLD_PIPE0r,
          IDB_OBM8_THRESHOLD_1_PIPE0r},
         {IDB_OBM9_THRESHOLD_PIPE0r, IDB_OBM9_THRESHOLD_1_PIPE0r},
         {IDB_OBM10_THRESHOLD_PIPE0r,
          IDB_OBM10_THRESHOLD_1_PIPE0r},
         {IDB_OBM11_THRESHOLD_PIPE0r, IDB_OBM11_THRESHOLD_1_PIPE0r},
         {IDB_OBM12_THRESHOLD_PIPE0r,
          IDB_OBM12_THRESHOLD_1_PIPE0r},
         {IDB_OBM13_THRESHOLD_PIPE0r, IDB_OBM13_THRESHOLD_1_PIPE0r},
         {IDB_OBM14_THRESHOLD_PIPE0r,
          IDB_OBM14_THRESHOLD_1_PIPE0r},
         {IDB_OBM15_THRESHOLD_PIPE0r, IDB_OBM15_THRESHOLD_1_PIPE0r},
         {IDB_OBM16_THRESHOLD_PIPE0r,
          IDB_OBM16_THRESHOLD_1_PIPE0r},
         {IDB_OBM17_THRESHOLD_PIPE0r, IDB_OBM17_THRESHOLD_1_PIPE0r},
         {IDB_OBM18_THRESHOLD_PIPE0r,
          IDB_OBM18_THRESHOLD_1_PIPE0r},
         {IDB_OBM19_THRESHOLD_PIPE0r, IDB_OBM19_THRESHOLD_1_PIPE0r}}
    };

    if (lossless) {
        discard_limit =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSLESS_INDEX]
            [num_lanes].discard_limit;
        lossless1_discard =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSLESS_INDEX]
            [num_lanes].lossless_discard;
        lossless0_discard =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSLESS_INDEX]
            [num_lanes].lossless_discard;
        lossy_discard =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSLESS_INDEX]
            [num_lanes].lossy_discard;
        lossy_low_pri =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSLESS_INDEX]
            [num_lanes].lossy_low_pri;
    } else {
        discard_limit =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSY_INDEX]
            [num_lanes].discard_limit;
        lossless1_discard =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSY_INDEX]
            [num_lanes].lossless_discard;
        lossless0_discard =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSY_INDEX]
            [num_lanes].lossless_discard;
        lossy_discard =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSY_INDEX]
            [num_lanes].lossy_discard;
        lossy_low_pri =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSY_INDEX]
            [num_lanes].lossy_low_pri;
    }
    for (i = 0; i < 2; i++) {
        reg = obm_thresh_pp[pipe_num][pm_num][i];
        COMPILER_64_ZERO(rval64);
        if (i == 0) {
            SOC_IF_ERROR_RETURN(soc_reg_rawport_get(unit, reg, REG_PORT_ANY,
                                                    subp, &rval64));
            soc_reg64_field32_set(unit, reg, &rval64, LOSSLESS0_DISCARDf,
                                lossless0_discard);
            soc_reg64_field32_set(unit, reg, &rval64, LOSSY_DISCARDf,
                                lossy_discard);
            soc_reg64_field32_set(unit, reg, &rval64, LOSSY_LOW_PRIf,
                                lossy_low_pri);
            SOC_IF_ERROR_RETURN(soc_reg_rawport_set(unit, reg, REG_PORT_ANY,
                                                    subp, rval64));
        }
        else {
            SOC_IF_ERROR_RETURN(soc_reg_rawport_get(unit, reg, REG_PORT_ANY,
                                                    subp, &rval64));
            soc_reg64_field32_set(unit, reg, &rval64, DISCARD_LIMITf,
                                discard_limit);
            soc_reg64_field32_set(unit, reg, &rval64, LOSSLESS1_DISCARDf,
                                lossless1_discard);
            SOC_IF_ERROR_RETURN(soc_reg_rawport_set(unit, reg, REG_PORT_ANY,
                                                    subp, rval64));
        }
    }

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_wr_obm_fc_threshold(int unit, int pipe_num, int pm_num, *              int subp, int num_lanes)
 *  @param unit Chip unit number.
 *  @param pipe_num Pipe number.
 *  @param pm_num Port Macro number.
 *  @param subp Sub-port number.
 *  @param num_lanes Number of serdeslanes used in the port.
 *  @param lossless Chip mode - 1: Lossless; 0: Lossy.
 *  @brief Helper function to config OBM flow-control thresholds during flexing
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_wr_obm_fc_threshold(int unit, int pipe_num, int pm_num,
                                      int subp,
                                      int num_lanes, int lossless)
{
    soc_reg_t reg;
    int i;
    uint32 lossless0_xon, lossless0_xoff, lossless1_xon, lossless1_xoff,
           port_xon, port_xoff;
    uint64 rval64;
    const soc_reg_t obm_fc_thresh_pp[MAVERICK2_PIPES_PER_DEV][
        MAVERICK2_PBLKS_PER_PIPE][2] = {
        {{IDB_OBM0_FC_THRESHOLD_PIPE0r,
          IDB_OBM0_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM1_FC_THRESHOLD_PIPE0r, IDB_OBM1_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM2_FC_THRESHOLD_PIPE0r,
          IDB_OBM2_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM3_FC_THRESHOLD_PIPE0r, IDB_OBM3_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM4_FC_THRESHOLD_PIPE0r,
          IDB_OBM4_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM5_FC_THRESHOLD_PIPE0r, IDB_OBM5_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM6_FC_THRESHOLD_PIPE0r,
          IDB_OBM6_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM7_FC_THRESHOLD_PIPE0r, IDB_OBM7_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM8_FC_THRESHOLD_PIPE0r,
          IDB_OBM8_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM9_FC_THRESHOLD_PIPE0r, IDB_OBM9_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM10_FC_THRESHOLD_PIPE0r,
          IDB_OBM10_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM11_FC_THRESHOLD_PIPE0r, IDB_OBM11_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM12_FC_THRESHOLD_PIPE0r,
          IDB_OBM12_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM13_FC_THRESHOLD_PIPE0r, IDB_OBM13_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM14_FC_THRESHOLD_PIPE0r,
          IDB_OBM14_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM15_FC_THRESHOLD_PIPE0r, IDB_OBM15_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM16_FC_THRESHOLD_PIPE0r,
          IDB_OBM16_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM17_FC_THRESHOLD_PIPE0r, IDB_OBM17_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM18_FC_THRESHOLD_PIPE0r,
          IDB_OBM18_FC_THRESHOLD_1_PIPE0r},
         {IDB_OBM19_FC_THRESHOLD_PIPE0r, IDB_OBM19_FC_THRESHOLD_1_PIPE0r}}
    };

    if (lossless) {
        lossless0_xon =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSLESS_INDEX]
            [num_lanes].lossless_xon;
        lossless1_xon =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSLESS_INDEX]
            [num_lanes].lossless_xon;
        lossless0_xoff =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSLESS_INDEX]
            [num_lanes].lossless_xoff;
        lossless1_xoff =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSLESS_INDEX]
            [num_lanes].lossless_xoff;
        port_xon =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSLESS_INDEX]
            [num_lanes].port_xon;
        port_xoff =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSLESS_INDEX]
            [num_lanes].port_xoff;
    } else {
        lossless0_xon =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSY_INDEX]
            [num_lanes].lossless_xon;
        lossless1_xon =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSY_INDEX]
            [num_lanes].lossless_xon;
        lossless0_xoff =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSY_INDEX]
            [num_lanes].lossless_xoff;
        lossless1_xoff =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSY_INDEX]
            [num_lanes].lossless_xoff;
        port_xon =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSY_INDEX]
            [num_lanes].port_xon;
        port_xoff =
            soc_maverick2_obm_settings[MAVERICK2_OBM_PARAMS_LOSSY_INDEX]
            [num_lanes].port_xoff;
    }

    COMPILER_64_ZERO(rval64);
    for (i = 0; i < 2; i++) {
        reg = obm_fc_thresh_pp[pipe_num][pm_num][i];
        if (i == 0) {
            soc_reg64_field32_set(unit, reg, &rval64, LOSSLESS0_XONf,
                                lossless0_xon);
            soc_reg64_field32_set(unit, reg, &rval64, LOSSLESS0_XOFFf,
                                lossless0_xoff);
            soc_reg64_field32_set(unit, reg, &rval64, LOSSLESS1_XONf,
                                lossless1_xon);
            soc_reg64_field32_set(unit, reg, &rval64, LOSSLESS1_XOFFf,
                                lossless1_xoff);
            SOC_IF_ERROR_RETURN(soc_reg_rawport_set(unit, reg, REG_PORT_ANY,
                                                    subp, rval64));
        }
        else {
            soc_reg64_field32_set(unit, reg, &rval64, PORT_XONf, port_xon);
            soc_reg64_field32_set(unit, reg, &rval64, PORT_XOFFf, port_xoff);
            SOC_IF_ERROR_RETURN(soc_reg_rawport_set(unit, reg, REG_PORT_ANY,
                                                    subp, rval64));
        }
    }

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_obm_poll_buffer_empty(int unit, int pipe_num,
 *              int pm_num, int subp)
 *  @param unit Chip unit number.
 *  @param pipe_num Maverick2 pipe number.
 *  @param pm_num PM number within the pipe: Ranges from 0 to 15.
 *  @param subp Subport number.
 *  @brief Helper function to poll for IDB OBM buffer empty.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_obm_poll_buffer_empty(int unit, int pipe_num, int pm_num,
                                        int subp)
{
    soc_reg_t reg;
    uint64 rval64;
    int cnt = 0;

    const soc_reg_t obm_usage_regs[MAVERICK2_PIPES_PER_DEV][
        MAVERICK2_PBLKS_PER_PIPE] = { 
        {IDB_OBM0_USAGE_PIPE0r, IDB_OBM1_USAGE_PIPE0r, IDB_OBM2_USAGE_PIPE0r,
         IDB_OBM3_USAGE_PIPE0r,
         IDB_OBM4_USAGE_PIPE0r, IDB_OBM5_USAGE_PIPE0r, IDB_OBM6_USAGE_PIPE0r,
         IDB_OBM7_USAGE_PIPE0r,
         IDB_OBM8_USAGE_PIPE0r, IDB_OBM9_USAGE_PIPE0r, IDB_OBM10_USAGE_PIPE0r,
         IDB_OBM11_USAGE_PIPE0r,
         IDB_OBM12_USAGE_PIPE0r, IDB_OBM13_USAGE_PIPE0r, IDB_OBM14_USAGE_PIPE0r,
         IDB_OBM15_USAGE_PIPE0r,
         IDB_OBM16_USAGE_PIPE0r, IDB_OBM17_USAGE_PIPE0r, IDB_OBM18_USAGE_PIPE0r,
         IDB_OBM19_USAGE_PIPE0r}
    };

    if (SAL_BOOT_XGSSIM || SAL_BOOT_BCMSIM) {
        return SOC_E_NONE;
    }

    reg = obm_usage_regs[pipe_num][pm_num];
    do {
        COMPILER_64_ZERO(rval64);
        SOC_IF_ERROR_RETURN(soc_reg_rawport_get(unit, reg, REG_PORT_ANY, subp,
                                                &rval64));

        sal_usleep(1 + (SAL_BOOT_QUICKTURN ? 1 : 0) * EMULATION_FACTOR);
        cnt++;
        if(cnt >= 20000) {
            LOG_CLI((BSL_META_U(unit, "OBM is not going to empty even after"
                "20000 attempts, pm %0d, subp %0d, phy_port %0d"),
                pm_num, subp, (pm_num * 4 + subp + 1)));
            return SOC_E_FAIL;
        }
    } while (soc_reg64_field32_get(unit, reg, rval64, TOTAL_COUNTf)>0);

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_ca_poll_buffer_empty(int unit, int pipe_num,
 *              int pm_num, int subp)
 *  @param unit Chip unit number.
 *  @param pipe_num Maverick2 pipe number.
 *  @param pm_num PM number within the pipe: Ranges from 0 to 15.
 *  @param subp Subport number.
 *  @brief Helper function to poll for IDB Cell Assembly buffer empty.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_ca_poll_buffer_empty(int unit, int pipe_num, int pm_num,
                                       int subp)
{
    soc_reg_t reg;
    uint64 rval64;
    uint32 ca_fifo_empty;
    int cnt = 0;

    static const soc_reg_t ca_status_regs[MAVERICK2_PIPES_PER_DEV][
        MAVERICK2_PBLKS_PER_PIPE] = {
        {IDB_OBM0_CA_HW_STATUS_PIPE0r, IDB_OBM1_CA_HW_STATUS_PIPE0r,
         IDB_OBM2_CA_HW_STATUS_PIPE0r, IDB_OBM3_CA_HW_STATUS_PIPE0r,
         IDB_OBM4_CA_HW_STATUS_PIPE0r, IDB_OBM5_CA_HW_STATUS_PIPE0r,
         IDB_OBM6_CA_HW_STATUS_PIPE0r, IDB_OBM7_CA_HW_STATUS_PIPE0r,
         IDB_OBM8_CA_HW_STATUS_PIPE0r, IDB_OBM9_CA_HW_STATUS_PIPE0r,
         IDB_OBM10_CA_HW_STATUS_PIPE0r, IDB_OBM11_CA_HW_STATUS_PIPE0r,
         IDB_OBM12_CA_HW_STATUS_PIPE0r, IDB_OBM13_CA_HW_STATUS_PIPE0r,
         IDB_OBM14_CA_HW_STATUS_PIPE0r, IDB_OBM15_CA_HW_STATUS_PIPE0r,
         IDB_OBM16_CA_HW_STATUS_PIPE0r, IDB_OBM17_CA_HW_STATUS_PIPE0r,
         IDB_OBM18_CA_HW_STATUS_PIPE0r, IDB_OBM19_CA_HW_STATUS_PIPE0r}
    };

    if (SAL_BOOT_XGSSIM || SAL_BOOT_BCMSIM) {
        return SOC_E_NONE;
    }

    reg = ca_status_regs[pipe_num][pm_num];
    do {
        COMPILER_64_ZERO(rval64);
        SOC_IF_ERROR_RETURN(soc_reg_rawport_get(unit, reg, REG_PORT_ANY, 0,
                                                &rval64));
        switch (subp) {
        case 0:
            ca_fifo_empty = soc_reg64_field32_get(unit, reg, rval64,
                                                FIFO_EMPTY_PORT0f);
            break;
        case 1:
            ca_fifo_empty = soc_reg64_field32_get(unit, reg, rval64,
                                                FIFO_EMPTY_PORT1f);
            break;
        case 2:
            ca_fifo_empty = soc_reg64_field32_get(unit, reg, rval64,
                                                FIFO_EMPTY_PORT2f);
            break;
        case 3:
            ca_fifo_empty = soc_reg64_field32_get(unit, reg, rval64,
                                                FIFO_EMPTY_PORT3f);
            break;
        default:
            ca_fifo_empty = 1;
            break;
        }

        sal_usleep(1 + (SAL_BOOT_QUICKTURN ? 1 : 0) * EMULATION_FACTOR);
        cnt++;
        if(cnt >= 20000) {
            LOG_CLI((BSL_META_U(unit, "CA is not going to empty even after"
                                "20000 attempts")));
            return SOC_E_FAIL;
        }
    } while (ca_fifo_empty==0);

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_ca_cpu_poll_buffer_empty(int unit, int pipe_num)
 *  @param unit Chip unit number.
 *  @param pipe_num Maverick2 pipe number.
 *  @brief Helper function to poll for IDB CPU Cell Assembly buffer empty.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_ca_cpu_poll_buffer_empty(int unit, int pipe_num)
{
    soc_reg_t reg;
    uint32 rval32;
    uint32 ca_fifo_empty;
    const soc_reg_t idb_ca_cpu_hw_status[MAVERICK2_PIPES_PER_DEV]= {
         IDB_CA_CPU_HW_STATUS_PIPE0r/*,
         IDB_CA_CPU_HW_STATUS_PIPE1r */};
    if (SAL_BOOT_XGSSIM || SAL_BOOT_BCMSIM) {
        return SOC_E_NONE;
    }
    reg = idb_ca_cpu_hw_status[pipe_num];
    do {
        SOC_IF_ERROR_RETURN(soc_reg32_rawport_get(unit,
                   reg, REG_PORT_ANY, 0, &rval32));
        ca_fifo_empty = soc_reg_field_get(unit, reg, rval32,
                                                FIFO_EMPTYf);
    } while (!ca_fifo_empty);
    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_ca_lpbk_poll_buffer_empty(int unit, int pipe_num)
 *  @param unit Chip unit number.
 *  @param pipe_num Maverick2 pipe number.
 *  @brief Helper function to poll for IDB LOOPBACK port
           Cell Assembly buffer empty.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_ca_lpbk_poll_buffer_empty(int unit, int pipe_num)
{
    soc_reg_t reg;
    uint32 rval32;
    uint32 ca_fifo_empty;
    const soc_reg_t idb_ca_lpbk_hw_status[MAVERICK2_PIPES_PER_DEV]= {
         IDB_CA_LPBK_HW_STATUS_PIPE0r/*,
         IDB_CA_LPBK_HW_STATUS_PIPE1r */};
    if (SAL_BOOT_XGSSIM || SAL_BOOT_BCMSIM) {
        return SOC_E_NONE;
    }
    reg = idb_ca_lpbk_hw_status[pipe_num];
    do {
        SOC_IF_ERROR_RETURN(soc_reg32_rawport_get(unit,
                   reg, REG_PORT_ANY, 0, &rval32));
        ca_fifo_empty = soc_reg_field_get(unit, reg, rval32,
                                                FIFO_EMPTYf);
    } while (!ca_fifo_empty);
    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_ca_mgmt_poll_buffer_empty(int unit, int pipe_num)
 *  @param unit Chip unit number.
 *  @param pipe_num Maverick2 pipe number.
 *  @brief Helper function to poll for IDB MGMT Cell Assembly buffer empty.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_ca_mgmt_poll_buffer_empty(int unit, int pipe_num)
{
    soc_reg_t reg;
    uint32 rval32;
    uint32 ca_fifo_empty;
    const soc_reg_t idb_ca_mgmt_hw_status[MAVERICK2_PIPES_PER_DEV]= {
         IDB_CA_MGMT_HW_STATUS_PIPE0r/* ,
         IDB_CA_MGMT_HW_STATUS_PIPE1r */};
    if (SAL_BOOT_XGSSIM || SAL_BOOT_BCMSIM) {
        return SOC_E_NONE;
    }

    reg = idb_ca_mgmt_hw_status[pipe_num];
    do {
        SOC_IF_ERROR_RETURN(soc_reg32_rawport_get(unit,
                   reg, REG_PORT_ANY, 0, &rval32));
        ca_fifo_empty = soc_reg_field_get(unit, reg, rval32,
                                                FIFO_EMPTYf);
    } while (!ca_fifo_empty);
    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_speed_to_bmop_class_map(int speed)
 *  @param speed Port speed.
 *  @brief Helper function to get bubble MOP class as below::
 *         The function look at the port speed & returns the encoding as below::
 *         0 - SAF,
 *         1 - 10GE,
 *         2 - HG[11],
 *         3 - 20GE,
 *         4 - 21[HG},
 *         5 - 25GE,
 *         6 - HG[27],
 *         7 - 40GE,
 *         8 - HG[42],
 *         9 - 50GE,
 *         10 - HG[53],
 *         11 - 100GE,
 *         12 - HG[106],
 *  @returns int
 */
int
soc_maverick2_speed_to_bmop_class_map(int speed)
{
    int bmop_class;
    int i;

    bmop_class=0;
    for (i = 0; i <= 12; i++) {
        if (soc_maverick2_bmop_cfg_tbl[i].speed == speed) {
            bmop_class = i;
            break;
        }
    }

    return bmop_class;
}

/*! @fn int soc_maverick2_idb_obm_bubble_mop_set(int unit,
 *              soc_port_schedule_state_t *port_schedule_state)
 *  @param unit Chip unit number.
 *  @param port_schedule_state Pointer to a soc_port_schedule_state_t struct
 *         variable
 *  @brief Helper function to re-configure BUBBLE MOP settings during flexport
 *         operation. The function looks at the port speed & the latency modes
 *         to determine what the bubble MOP settings should be.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_obm_bubble_mop_set(
    int unit, soc_port_schedule_state_t *port_schedule_state)
{
    soc_reg_t reg;
    uint32 rval32;
    int pipe_num;
    int pm_num;
    int subp;
    int bubble_mop_disable;
    int phy_port;
    int port;
    int latency;
    int i;
    int speed_encoding;

    for (i = 0; i < port_schedule_state->nport; i++) {
        if (port_schedule_state->resource[i].physical_port != -1) {
            port = port_schedule_state->resource[i].logical_port;
            phy_port = port_schedule_state->out_port_map.port_l2p_mapping[port];
            pipe_num = soc_maverick2_get_pipe_from_phy_pnum(phy_port);
            pm_num = soc_maverick2_get_pm_from_phy_pnum(phy_port);
            subp = soc_maverick2_get_subp_from_phy_pnum(phy_port);
            latency = port_schedule_state->cutthru_prop.switch_bypass_mode;
            speed_encoding =
                (port_schedule_state->cutthru_prop.asf_modes[port]==
                 _SOC_ASF_MODE_SAF) ? 0 :
                soc_maverick2_speed_to_bmop_class_map(
                    port_schedule_state->resource[i].speed);
            bubble_mop_disable = 
                  (soc_maverick2_bmop_cfg_tbl[speed_encoding].
                                 bmop_enable.oversub[latency]);
            reg = obm_ctrl_regs[pipe_num][pm_num];
            SOC_IF_ERROR_RETURN(soc_reg32_rawport_get(unit, reg, REG_PORT_ANY,
                                                      0, &rval32));
            if (subp==0) {
                soc_reg_field_set(unit, reg, &rval32, PORT0_BUBBLE_MOP_DISABLEf,
                                  bubble_mop_disable);
            } else if (subp ==1) {
                soc_reg_field_set(unit, reg, &rval32, PORT1_BUBBLE_MOP_DISABLEf,
                                  bubble_mop_disable);
            } else if (subp ==2) {
                soc_reg_field_set(unit, reg, &rval32, PORT2_BUBBLE_MOP_DISABLEf,
                                  bubble_mop_disable);
            } else {
                soc_reg_field_set(unit, reg, &rval32, PORT3_BUBBLE_MOP_DISABLEf,
                                  bubble_mop_disable);
            }
            SOC_IF_ERROR_RETURN(soc_reg32_rawport_set(unit, reg, REG_PORT_ANY,
                                                      0, rval32));
        }
    }

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_obm_port_config_set(int unit,
 *              soc_port_schedule_state_t *port_schedule_state)
 *  @param unit Chip unit number.
 *  @param port_schedule_state Pointer to a soc_port_schedule_state_t struct variable
 *  @brief Helper function to re-configure IDB OBM port config register default
 *         priority if lossless class.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_obm_port_config_set(
    int unit, soc_port_schedule_state_t *port_schedule_state)
{
    soc_reg_t reg;
    uint32 rval32;
    int pipe_num;
    int pm_num;
    int subp;
    int phy_port;
    int port;
    int i;
    static const soc_reg_t obm_port_config_regs[MAVERICK2_PIPES_PER_DEV][
        MAVERICK2_PBLKS_PER_PIPE] = {
        {IDB_OBM0_PORT_CONFIG_PIPE0r, IDB_OBM1_PORT_CONFIG_PIPE0r,
         IDB_OBM2_PORT_CONFIG_PIPE0r, IDB_OBM3_PORT_CONFIG_PIPE0r,
         IDB_OBM4_PORT_CONFIG_PIPE0r, IDB_OBM5_PORT_CONFIG_PIPE0r,
         IDB_OBM6_PORT_CONFIG_PIPE0r, IDB_OBM7_PORT_CONFIG_PIPE0r,
         IDB_OBM8_PORT_CONFIG_PIPE0r, IDB_OBM9_PORT_CONFIG_PIPE0r,
         IDB_OBM10_PORT_CONFIG_PIPE0r, IDB_OBM11_PORT_CONFIG_PIPE0r,
         IDB_OBM12_PORT_CONFIG_PIPE0r, IDB_OBM13_PORT_CONFIG_PIPE0r,
         IDB_OBM14_PORT_CONFIG_PIPE0r, IDB_OBM15_PORT_CONFIG_PIPE0r,
         IDB_OBM16_PORT_CONFIG_PIPE0r, IDB_OBM17_PORT_CONFIG_PIPE0r,
         IDB_OBM18_PORT_CONFIG_PIPE0r, IDB_OBM19_PORT_CONFIG_PIPE0r}
    };

    if (port_schedule_state->lossless) {
        for (i = 0; i < port_schedule_state->nport; i++) {
            if (port_schedule_state->resource[i].physical_port != -1) {
                port = port_schedule_state->resource[i].logical_port;
                phy_port =
                    port_schedule_state->out_port_map.port_l2p_mapping[port];
                pipe_num = soc_maverick2_get_pipe_from_phy_pnum(phy_port);
                pm_num = soc_maverick2_get_pm_from_phy_pnum(phy_port);
                subp = soc_maverick2_get_subp_from_phy_pnum(phy_port);
                rval32=0;
                reg = obm_port_config_regs[pipe_num][pm_num];
                soc_reg_field_set(unit, reg, &rval32, PORT_PRIf, 2);
                SOC_IF_ERROR_RETURN(soc_reg32_rawport_set(unit, reg,
                                                          REG_PORT_ANY, subp,
                                                          rval32));
            }
        }
    }

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_obm_dscp_map_set(int unit,
 *              soc_port_schedule_state_t *port_schedule_state)
 *  @param unit Chip unit number.
 *  @param port_schedule_state Pointer to a soc_port_schedule_state_t struct variable
 *  @brief Helper function to re-configure IDB_OBM0_DSCP_MAP table as per the lossy/lossless configuration.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_obm_dscp_map_set(
    int unit, soc_port_schedule_state_t *port_schedule_state)
{
    soc_mem_t mem;
    uint32 entry[SOC_MAX_MEM_WORDS];
    int pipe_num;
    int pm_num;
    int subp;
    int phy_port;
    int port;
    int i;
    int j;
    int count;
    int lossless;
    uint32 priority;
    static const soc_mem_t obm_pri_map_mem[][MAVERICK2_PIPES_PER_DEV][
        MAVERICK2_PORTS_PER_PBLK] = {
        {{IDB_OBM0_PRI_MAP_PORT0_PIPE0m, IDB_OBM0_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM0_PRI_MAP_PORT2_PIPE0m, IDB_OBM0_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM1_PRI_MAP_PORT0_PIPE0m, IDB_OBM1_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM1_PRI_MAP_PORT2_PIPE0m, IDB_OBM1_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM2_PRI_MAP_PORT0_PIPE0m, IDB_OBM2_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM2_PRI_MAP_PORT2_PIPE0m, IDB_OBM2_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM3_PRI_MAP_PORT0_PIPE0m, IDB_OBM3_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM3_PRI_MAP_PORT2_PIPE0m, IDB_OBM3_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM4_PRI_MAP_PORT0_PIPE0m, IDB_OBM4_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM4_PRI_MAP_PORT2_PIPE0m, IDB_OBM4_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM5_PRI_MAP_PORT0_PIPE0m, IDB_OBM5_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM5_PRI_MAP_PORT2_PIPE0m, IDB_OBM5_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM6_PRI_MAP_PORT0_PIPE0m, IDB_OBM6_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM6_PRI_MAP_PORT2_PIPE0m, IDB_OBM6_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM7_PRI_MAP_PORT0_PIPE0m, IDB_OBM7_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM7_PRI_MAP_PORT2_PIPE0m, IDB_OBM7_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM8_PRI_MAP_PORT0_PIPE0m, IDB_OBM8_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM8_PRI_MAP_PORT2_PIPE0m, IDB_OBM8_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM9_PRI_MAP_PORT0_PIPE0m, IDB_OBM9_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM9_PRI_MAP_PORT2_PIPE0m, IDB_OBM9_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM10_PRI_MAP_PORT0_PIPE0m, IDB_OBM10_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM10_PRI_MAP_PORT2_PIPE0m, IDB_OBM10_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM11_PRI_MAP_PORT0_PIPE0m, IDB_OBM11_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM11_PRI_MAP_PORT2_PIPE0m, IDB_OBM11_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM12_PRI_MAP_PORT0_PIPE0m, IDB_OBM12_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM12_PRI_MAP_PORT2_PIPE0m, IDB_OBM12_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM13_PRI_MAP_PORT0_PIPE0m, IDB_OBM13_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM13_PRI_MAP_PORT2_PIPE0m, IDB_OBM13_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM14_PRI_MAP_PORT0_PIPE0m, IDB_OBM14_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM14_PRI_MAP_PORT2_PIPE0m, IDB_OBM14_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM15_PRI_MAP_PORT0_PIPE0m, IDB_OBM15_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM15_PRI_MAP_PORT2_PIPE0m, IDB_OBM15_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM16_PRI_MAP_PORT0_PIPE0m, IDB_OBM16_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM16_PRI_MAP_PORT2_PIPE0m, IDB_OBM16_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM17_PRI_MAP_PORT0_PIPE0m, IDB_OBM17_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM17_PRI_MAP_PORT2_PIPE0m, IDB_OBM17_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM18_PRI_MAP_PORT0_PIPE0m, IDB_OBM18_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM18_PRI_MAP_PORT2_PIPE0m, IDB_OBM18_PRI_MAP_PORT3_PIPE0m}},
        {{IDB_OBM18_PRI_MAP_PORT0_PIPE0m, IDB_OBM19_PRI_MAP_PORT1_PIPE0m,
          IDB_OBM19_PRI_MAP_PORT2_PIPE0m, IDB_OBM19_PRI_MAP_PORT3_PIPE0m}}
    };
    static const soc_field_t obm_ob_pri_fields[] = {
        OFFSET0_OB_PRIORITYf, OFFSET1_OB_PRIORITYf, OFFSET2_OB_PRIORITYf,
        OFFSET3_OB_PRIORITYf, OFFSET4_OB_PRIORITYf, OFFSET5_OB_PRIORITYf,
        OFFSET6_OB_PRIORITYf, OFFSET7_OB_PRIORITYf, OFFSET8_OB_PRIORITYf,
        OFFSET9_OB_PRIORITYf, OFFSET10_OB_PRIORITYf, OFFSET11_OB_PRIORITYf,
        OFFSET12_OB_PRIORITYf, OFFSET13_OB_PRIORITYf, OFFSET14_OB_PRIORITYf,
        OFFSET15_OB_PRIORITYf
    };

    lossless = port_schedule_state->lossless;
    priority = lossless ? MAVERICK2_OBM_PRIORITY_LOSSLESS0 : 
                            MAVERICK2_OBM_PRIORITY_LOSSY_LO;
    count = COUNTOF(obm_ob_pri_fields);

    for (i = 0; i < port_schedule_state->nport; i++) {
        if (port_schedule_state->resource[i].physical_port != -1) {
            port = port_schedule_state->resource[i].logical_port;
            phy_port =
                port_schedule_state->out_port_map.port_l2p_mapping[port];
            pipe_num = soc_maverick2_get_pipe_from_phy_pnum(phy_port);
            pm_num = soc_maverick2_get_pm_from_phy_pnum(phy_port);
            subp = soc_maverick2_get_subp_from_phy_pnum(phy_port);
            mem = obm_pri_map_mem[pm_num][pipe_num][subp];
            sal_memset(entry, 0, sizeof(entry));
            for (j = 0; j < count; j++) {
                soc_mem_field_set(unit, mem, entry, obm_ob_pri_fields[j], 
                                                &priority);
            }
            SOC_IF_ERROR_RETURN
                (soc_mem_write(unit, mem, MEM_BLOCK_ALL, 0, entry));
        }
    }
    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_obm_force_saf_set(int unit,
 *              soc_port_schedule_state_t *port_schedule_state)
 *  @param unit Chip unit number.
 *  @param port_schedule_state Pointer to a soc_port_schedule_state_t struct variable
 *  @brief Helper function to set the force_SAF setting for OBM.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_idb_obm_force_saf_set(
    int unit, soc_port_schedule_state_t *port_schedule_state)
{
    soc_reg_t reg;
    uint32 rval32;
    int pipe_num;
    int pm_num;
    int subp;
    int phy_port;
    int port;
    int i;
    int unsatisfied_threshold;
    int send_enable;
    int receive_enable;
    int duration_timer;
    uint32 ovs_prt_up;
    static const soc_reg_t obm_force_saf_config_regs[MAVERICK2_PIPES_PER_DEV][
        MAVERICK2_PBLKS_PER_PIPE] = {
        {IDB_OBM0_DBG_A_PIPE0r, IDB_OBM1_DBG_A_PIPE0r, IDB_OBM2_DBG_A_PIPE0r,
         IDB_OBM3_DBG_A_PIPE0r,
         IDB_OBM4_DBG_A_PIPE0r, IDB_OBM5_DBG_A_PIPE0r, IDB_OBM6_DBG_A_PIPE0r,
         IDB_OBM7_DBG_A_PIPE0r,
         IDB_OBM8_DBG_A_PIPE0r, IDB_OBM9_DBG_A_PIPE0r, IDB_OBM10_DBG_A_PIPE0r,
         IDB_OBM11_DBG_A_PIPE0r,
         IDB_OBM12_DBG_A_PIPE0r, IDB_OBM13_DBG_A_PIPE0r, IDB_OBM14_DBG_A_PIPE0r,
         IDB_OBM15_DBG_A_PIPE0r,
         IDB_OBM16_DBG_A_PIPE0r, IDB_OBM17_DBG_A_PIPE0r, IDB_OBM18_DBG_A_PIPE0r,
         IDB_OBM19_DBG_A_PIPE0r}
    };
    static const soc_reg_t idb_force_saf_config_regs[MAVERICK2_PIPES_PER_DEV] =
    {IDB_DBG_B_PIPE0r/* , IDB_DBG_B_PIPE1r */};

    ovs_prt_up=0;
    for (i = 0; i < port_schedule_state->nport; i++) {
        if (port_schedule_state->resource[i].physical_port != -1) {
            port = port_schedule_state->resource[i].logical_port;
            phy_port = port_schedule_state->out_port_map.port_l2p_mapping[port];
            pipe_num = soc_maverick2_get_pipe_from_phy_pnum(phy_port);
            pm_num = soc_maverick2_get_pm_from_phy_pnum(phy_port);
            subp = soc_maverick2_get_subp_from_phy_pnum(phy_port);
            if (port_schedule_state->resource[i].oversub) {
                send_enable=1;
                receive_enable=1;
                ovs_prt_up |= (1<<pipe_num);
            }else{
                send_enable=0;
                receive_enable=0;
            }
            unsatisfied_threshold =
                ((port_schedule_state->resource[i].speed == 25000)||
                 (port_schedule_state->resource[i].speed == 27000)) ? 26 : 
                ((port_schedule_state->resource[i].speed == 1000)||
                 (port_schedule_state->resource[i].speed == 10000)||
                 (port_schedule_state->resource[i].speed == 11000)||
                 (port_schedule_state->resource[i].speed == 20000)||
                 (port_schedule_state->resource[i].speed == 21000)||
                 (port_schedule_state->resource[i].speed == 40000)||
                 (port_schedule_state->resource[i].speed == 42000)) ? 12 : 18;
            rval32=0;
            reg = obm_force_saf_config_regs[pipe_num][pm_num];
            soc_reg_field_set(unit, reg, &rval32, FIELD_Bf, send_enable);
            soc_reg_field_set(unit, reg, &rval32, FIELD_Af, receive_enable);
            soc_reg_field_set(unit, reg, &rval32, FIELD_Cf,
                              unsatisfied_threshold);
            SOC_IF_ERROR_RETURN(soc_reg32_rawport_set(unit, reg, REG_PORT_ANY,
                                                      subp, rval32));
        }
    }
    LOG_DEBUG(BSL_LS_SOC_PORT, (BSL_META_U(unit,
              "soc_maverick2_idb_obm_force_saf_set ovs_prt_up:: %x \n"),
              ovs_prt_up));
    duration_timer=
        soc_maverick2_fsaf_dur_timer_tbl[MAVERICK2_FORCE_SAF_TIMER_ENTRY_CNT].
        duration_timer;
    for (i=0; i<MAVERICK2_FORCE_SAF_TIMER_ENTRY_CNT; i++) {
        if (port_schedule_state->frequency ==
            soc_maverick2_fsaf_dur_timer_tbl[i].frequency) {
            duration_timer=soc_maverick2_fsaf_dur_timer_tbl[i].duration_timer;
        }
    }
    for (i = 0; i < MAVERICK2_PIPES_PER_DEV; i++) {
        pipe_num=i;
        LOG_DEBUG(BSL_LS_SOC_PORT, (BSL_META_U(unit,
                  "soc_maverick2_idb_obm_force_saf_set ovs_prt_up:: %x "
                  "pipe_num %d\n"), ovs_prt_up,pipe_num));
        if (ovs_prt_up>>pipe_num) {
            reg = idb_force_saf_config_regs[pipe_num];
            rval32=0;
            LOG_DEBUG(BSL_LS_SOC_PORT, (BSL_META_U(unit,
                      "soc_maverick2_idb_obm_force_saf_set Write to "
                      "IDB_FORCE_SAF_CONFIG pipe_num %d\n"),
                      pipe_num));
            soc_reg_field_set(unit, reg, &rval32, FIELD_Af, duration_timer);
            SOC_IF_ERROR_RETURN(soc_reg32_rawport_set(unit, reg, REG_PORT_ANY,
                                                      0, rval32));
        }
    }

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_flex_dis_forwarding_traffic(int unit,
 *              soc_port_schedule_state_t *port_schedule_state)
 *  @param unit Chip unit number.
 *  @param port_schedule_state Pointer to a soc_port_schedule_state_t struct variable
 *  @brief Helper function to Write EPC_LINK_BMAP table & wait. 
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_flex_dis_forwarding_traffic(
    int unit, soc_port_schedule_state_t *port_schedule_state)
{
    int i;
    uint32 entry[SOC_MAX_MEM_WORDS];
    uint32 memfld[5];
    uint32 mask;
    int port;
    int wait_us;

    /* ING_DEST_PORT_ENABLEm */
    sal_memset(entry, 0, sizeof(entry));
    sal_memset(memfld, 0, sizeof(memfld));
    SOC_IF_ERROR_RETURN(soc_mem_read(unit, ING_DEST_PORT_ENABLEm, MEM_BLOCK_ALL, 0,
                                     entry));
    soc_mem_field_get(unit, ING_DEST_PORT_ENABLEm, entry, PORT_BITMAPf,
                      memfld);
    mask = 0xffffffff;
    /* Make ING_DEST_PORT_ENABLE_BMAP=0 for all the ports involved in flex */
    for (i = 0; i < port_schedule_state->nport; i++) {
        port = port_schedule_state->resource[i].logical_port;
        memfld[(port>>5)] &= (mask^(1<<(port&0x1f)));
    }
    soc_mem_field_set(unit, ING_DEST_PORT_ENABLEm, entry, PORT_BITMAPf,
                      memfld);
    SOC_IF_ERROR_RETURN(soc_mem_write(unit, ING_DEST_PORT_ENABLEm, MEM_BLOCK_ALL, 0,
                                      entry));

    /* EPC_LINK_BMAP read, field modify and write. */
    sal_memset(entry, 0, sizeof(entry));
    sal_memset(memfld, 0, sizeof(memfld));
    SOC_IF_ERROR_RETURN(soc_mem_read(unit, EPC_LINK_BMAPm, MEM_BLOCK_ALL, 0,
                                     entry));
    soc_mem_field_get(unit, EPC_LINK_BMAPm, entry, PORT_BITMAPf,
                      memfld);

    LOG_DEBUG(BSL_LS_SOC_PORT,
              (BSL_META_U(unit,
                          "Disable EPC_LINK_BITMAP READ:: %x %x %x %x %x \n"),
                          memfld[0],memfld[1], memfld[2],memfld[3],memfld[4]));
    mask = 0xffffffff;
    /* Make EPC_LINK_BMAP=0 for all the ports involved in flex */
    sal_memset(memfld, 0, sizeof(memfld));
    for (i = 0; i < port_schedule_state->nport; i++) {
        port = port_schedule_state->resource[i].logical_port;
        memfld[(port>>5)] &= (mask^(1<<(port&0x1f)));
    }
    LOG_DEBUG(BSL_LS_SOC_PORT,
              (BSL_META_U(unit,
                          "Disable EPC_LINK_BITMAP write:: %x %x %x %x %x \n"),
                          memfld[0],memfld[1], memfld[2],memfld[3],memfld[4]));

    soc_mem_field_set(unit, EPC_LINK_BMAPm, entry, PORT_BITMAPf,
                      memfld);
    SOC_IF_ERROR_RETURN(soc_mem_write(unit, EPC_LINK_BMAPm, MEM_BLOCK_ALL, 0,
                                      entry));

    /* Wait is 8ms if 10/100 Kbps, else 80 us. 
     */
    if ((port_schedule_state->in_port_map.log_port_speed[
            MAVERICK2_LOG_PORT_MNG0] <= 100)) {
        wait_us = 8000;
    } else {
        wait_us = 80;
    }
    sal_usleep(wait_us + (SAL_BOOT_QUICKTURN ? 1: 0) * EMULATION_FACTOR);

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_flex_en_forwarding_traffic(int unit,
 *              soc_port_schedule_state_t *port_schedule_state)
 *  @param unit Chip unit number.
 *  @param port_schedule_state Pointer to a soc_port_schedule_state_t struct variable
 *  @brief Helper function to Write EPC_LINK_BMAP table to enable forwarding traffic
 *         to port. Follows the sequence specified in document "MV2_uA_flexport.pdf",
 *         Section 6.6.2, bullet number 5.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_flex_en_forwarding_traffic(
    int unit, soc_port_schedule_state_t *port_schedule_state)
{
    int i;
    uint32 entry[SOC_MAX_MEM_WORDS];
    uint32 memfld[5];
    int port;

    sal_memset(entry, 0, sizeof(entry));
    sal_memset(memfld, 0, sizeof(memfld));
    SOC_IF_ERROR_RETURN(soc_mem_read(unit, ING_DEST_PORT_ENABLEm, MEM_BLOCK_ALL, 0,
                                     entry));
    soc_mem_field_get(unit, ING_DEST_PORT_ENABLEm, entry, PORT_BITMAPf,
                      memfld);

    for (i = 0; i < port_schedule_state->nport; i++) {
        if (port_schedule_state->resource[i].physical_port != -1) {
            port = port_schedule_state->resource[i].logical_port;
            memfld[(port>>5)] |= (0x1<<(port&0x1f));
        }
    }
    soc_mem_field_set(unit, ING_DEST_PORT_ENABLEm, entry, PORT_BITMAPf,
                      memfld);
    SOC_IF_ERROR_RETURN(soc_mem_write(unit, ING_DEST_PORT_ENABLEm, MEM_BLOCK_ALL, 0,
                                      entry));

    /* EPC_LINK_BMAP read, field modify and write. */
    sal_memset(entry, 0, sizeof(entry));
    sal_memset(memfld, 0, sizeof(memfld));
    SOC_IF_ERROR_RETURN(soc_mem_read(unit, EPC_LINK_BMAPm, MEM_BLOCK_ALL, 0,
                                     entry));
    soc_mem_field_get(unit, EPC_LINK_BMAPm, entry, PORT_BITMAPf,
                      memfld);

    LOG_DEBUG(BSL_LS_SOC_PORT,
              (BSL_META_U(unit,
                          "Enable EPC_LINK_BITMAP READ:: %x %x %x %x %x \n"),
                          memfld[0],memfld[1], memfld[2],memfld[3],memfld[4]));
    /* Make EPC_LINK_BMAP=1 for all the ports going up */
    sal_memset(memfld, 0, sizeof(memfld));
    for (i = 0; i < port_schedule_state->nport; i++) {
        if (port_schedule_state->resource[i].physical_port != -1) {
            port = port_schedule_state->resource[i].logical_port;
            memfld[(port>>5)] |= (0x1<<(port&0x1f));
        }
    }
    LOG_DEBUG(BSL_LS_SOC_PORT,
              (BSL_META_U(unit,
                          "Enable EPC_LINK_BITMAP write:: %x %x %x %x %x \n"),
                          memfld[0],memfld[1], memfld[2],memfld[3],memfld[4]));

    soc_mem_field_set(unit, EPC_LINK_BMAPm, entry, PORT_BITMAPf,
                      memfld);
    SOC_IF_ERROR_RETURN(soc_mem_write(unit, EPC_LINK_BMAPm, MEM_BLOCK_ALL, 0,
                                      entry));

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_idb_port_down(int unit,
 *              soc_port_schedule_state_t *port_schedule_state)
 *  @param unit Chip unit number.
 *  @param port_schedule_state Pointer to a soc_port_schedule_state_t struct
 *         variable
 *  @brief Helper function to follow IDB port down sequence for flexport operation.
 *         Follows the sequence:
 *         Two main parts:
 *            (1) Poll untill IDB buffers are empty.
 *            (2) Hold IDB buffers in reset state.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_flex_idb_port_down(int                        unit,
                                 soc_port_schedule_state_t *port_schedule_state)
{
    int i;
    int pipe_num;
    int pm_num;
    int reset_buffer;
    int phy_port;
    int subp;

    /* Poll until IDB buffers are empty for all the ports going down */
    for (i = 0; i < port_schedule_state->nport; i++) {
        if (port_schedule_state->resource[i].physical_port == -1) {
            phy_port =
                port_schedule_state->in_port_map.port_l2p_mapping[
                    port_schedule_state
                    ->resource[i]
                    .logical_port];
            pipe_num = soc_maverick2_get_pipe_from_phy_pnum(phy_port);
            pm_num = soc_maverick2_get_pm_from_phy_pnum(phy_port);
            subp = soc_maverick2_get_subp_from_phy_pnum(phy_port);
            if(phy_port == MAVERICK2_PHY_PORT_MNG0) {
                if (!soc_maverick2_idb_mgmt_ca_in_reset(unit,pipe_num)){
                    SOC_IF_ERROR_RETURN(soc_maverick2_idb_ca_mgmt_poll_buffer_empty(
                                                             unit, pipe_num));
                }
            } else if((phy_port == MAVERICK2_PHY_PORT_CPU)/*  ||
                      (phy_port == MAVERICK2_PHY_PORT_MNG0) */) {
                if (!soc_maverick2_idb_cpu_ca_in_reset(unit,pipe_num)){
                    SOC_IF_ERROR_RETURN(
                        soc_maverick2_idb_ca_cpu_poll_buffer_empty(unit,
                                                                   pipe_num));
                }
            } else if((phy_port == MAVERICK2_PHY_PORT_LPBK0)||
                      (phy_port == MAVERICK2_PHY_PORT_LPBK1)) {
                if (!soc_maverick2_idb_lpbk_ca_in_reset(unit,pipe_num)){
                    SOC_IF_ERROR_RETURN(
                        soc_maverick2_idb_ca_lpbk_poll_buffer_empty(unit,
                                                                    pipe_num));
                }
            } else {                
                SOC_IF_ERROR_RETURN(
                    soc_maverick2_idb_obm_poll_buffer_empty(unit,
                                                            pipe_num,
                                                            pm_num,
                                                            subp));
                SOC_IF_ERROR_RETURN(
                    soc_maverick2_idb_ca_poll_buffer_empty(unit,
                                                           pipe_num,
                                                           pm_num,
                                                           subp));
            }
        }
    }

    /* Hold IDB buffers in reset state for all the ports going down */
    for (i = 0; i < port_schedule_state->nport; i++) {
        if (port_schedule_state->resource[i].physical_port == -1) {
            phy_port =
                port_schedule_state->in_port_map.port_l2p_mapping[
                    port_schedule_state
                    ->resource[i]
                    .logical_port];
            pipe_num = soc_maverick2_get_pipe_from_phy_pnum(phy_port);
            pm_num = soc_maverick2_get_pm_from_phy_pnum(phy_port);
            subp = soc_maverick2_get_subp_from_phy_pnum(phy_port);
            reset_buffer = 1;
            if (phy_port == MAVERICK2_PHY_PORT_MNG0) {
                SOC_IF_ERROR_RETURN(soc_maverick2_idb_mgmt_ca_reset_buffer(
                                       unit, pipe_num, reset_buffer));
            } else if((phy_port == MAVERICK2_PHY_PORT_CPU) /* ||
                      (phy_port == MAVERICK2_PHY_PORT_MNG0) */) {
                SOC_IF_ERROR_RETURN(soc_maverick2_idb_cpu_ca_reset_buffer(
                                       unit, pipe_num, reset_buffer));
            } else if((phy_port == MAVERICK2_PHY_PORT_LPBK0)||
                      (phy_port == MAVERICK2_PHY_PORT_LPBK1)) {
                SOC_IF_ERROR_RETURN(soc_maverick2_idb_lpbk_ca_reset_buffer(
                                       unit, pipe_num, reset_buffer));
            } else {
                SOC_IF_ERROR_RETURN(soc_maverick2_idb_obm_reset_buffer(unit,
                                                            pipe_num,
                                                            pm_num, subp,
                                                            reset_buffer));
                SOC_IF_ERROR_RETURN(soc_maverick2_idb_ca_reset_buffer(unit,
                                                            pipe_num,
                                                            pm_num, subp,
                                                            reset_buffer));
            }
        }
    }

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_flex_idb_reconfigure(int unit,
 *              soc_port_schedule_state_t *port_schedule_state)
 *  @param unit Chip unit number.
 *  @param port_schedule_state Pointer to a soc_port_schedule_state_t struct variable
 *  @brief Helper function to reconfigure IDB during flexport operation.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_flex_idb_reconfigure(
    int unit, soc_port_schedule_state_t *port_schedule_state)
{
    int i;
    int pipe_num;
    int pm_num;
    int phy_port, idb_port;
    int physical_port, logical_port;
    int subp, speed;
    int num_lanes;
    int lossless;
    uint32 entry[SOC_MAX_MEM_WORDS];
    uint32 memfld;
    soc_mem_t mem;
    static soc_mem_t ing_phy_to_idb_map_mem[MAVERICK2_PIPES_PER_DEV] = {
            ING_PHY_TO_IDB_PORT_MAP_PIPE0m
    };

    SOC_IF_ERROR_RETURN(soc_maverick2_tdm_flexport_idb(unit,
                                                       port_schedule_state));
    SOC_IF_ERROR_RETURN(_soc_mv2_tdm_set_idb_hsp(unit,
                                                       port_schedule_state));
    SOC_IF_ERROR_RETURN(_soc_mv2_tdm_set_idb_calendar(unit,
                                                            port_schedule_state));

    /* Update ING_PHY_TO_IDB_PORT_MAP TABLE */
    for(i = 0; i < port_schedule_state->nport; i++) {
        physical_port = port_schedule_state->resource[i].physical_port;
        logical_port = port_schedule_state->resource[i].logical_port;

        if(physical_port == -1) {
            idb_port = 0x3f;
            phy_port =
            port_schedule_state->in_port_map.port_l2p_mapping[logical_port];

            assert(phy_port > 0);
            assert(phy_port <= 80);

            pipe_num = logical_port / MAVERICK2_DEV_PORTS_PER_PIPE;
            mem = ing_phy_to_idb_map_mem[pipe_num];
            LOG_DEBUG(BSL_LS_SOC_PORT, (BSL_META_U(unit,
                      "PHY_TO_IDB map p %0d, pi %0d, l %0d, "
                      "i %0d\n"),
                      physical_port, phy_port, logical_port, idb_port));

            sal_memset(&entry, 0, sizeof(entry));

            memfld = idb_port;
            soc_mem_field_set(unit, mem, entry, IDB_PORTf, &memfld);

            memfld = 0;
            soc_mem_field_set(unit, mem, entry, VALIDf, &memfld);

            SOC_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ALL,
                                                      (phy_port-1), &entry));
        }
    }
    for(i = 0; i < port_schedule_state->nport; i++) {
        physical_port = port_schedule_state->resource[i].physical_port;
        logical_port = port_schedule_state->resource[i].logical_port;

        if(physical_port != -1) {
            idb_port = port_schedule_state->resource[i].idb_port;
            phy_port = port_schedule_state->resource[i].physical_port;

            assert(phy_port > 0);
            assert(phy_port <= 80);

            pipe_num = logical_port / MAVERICK2_DEV_PORTS_PER_PIPE;
            mem = ing_phy_to_idb_map_mem[pipe_num];
            LOG_DEBUG(BSL_LS_SOC_PORT, (BSL_META_U(unit,
                      "PHY_TO_IDB map p %0d, pi %0d, l %0d, "
                      "i %0d\n"),
                      physical_port, phy_port, logical_port, idb_port));

            sal_memset(&entry, 0, sizeof(entry));

            memfld = idb_port;
            soc_mem_field_set(unit, mem, entry, IDB_PORTf, &memfld);

            memfld = 1;
            soc_mem_field_set(unit, mem, entry, VALIDf, &memfld);

            SOC_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ALL,
                                                      (phy_port-1), &entry));
        }
    }

    /* Reconfigure OBM thresholds for ports going up */
    for (i = 0; i < port_schedule_state->nport; i++) {
        if (port_schedule_state->resource[i].physical_port != -1) {
            speed = port_schedule_state->resource[i].speed;
            logical_port = port_schedule_state->resource[i].logical_port;
            phy_port =
                port_schedule_state->out_port_map.port_l2p_mapping[logical_port];
            num_lanes =
                port_schedule_state->out_port_map.port_num_lanes[logical_port];
            pipe_num = soc_maverick2_get_pipe_from_phy_pnum(phy_port);
            pm_num = soc_maverick2_get_pm_from_phy_pnum(phy_port);
            subp = soc_maverick2_get_subp_from_phy_pnum(phy_port);
            lossless = port_schedule_state->lossless;
            LOG_DEBUG(BSL_LS_SOC_PORT, (BSL_META_U(unit,
                      "Reconfigure IDB: Write OBM thresh i %1d "
                      "phy_port %1d pipe=%1d pm=%1d subp=%1d "
                      "num_lanes=%1d\n"),
                      i,phy_port, pipe_num, pm_num, subp, num_lanes));

            SOC_IF_ERROR_RETURN(soc_maverick2_idb_wr_obm_ovs_en(unit, pipe_num,
                                                                pm_num,subp));

            SOC_IF_ERROR_RETURN(soc_maverick2_idb_wr_obm_port_ca_sop(unit,
                                                            pipe_num, pm_num,
                                                            subp, speed));

            SOC_IF_ERROR_RETURN(soc_maverick2_idb_wr_obm_buffer_limit(unit, pipe_num,
                                                                pm_num,subp,
                                                                num_lanes));

            SOC_IF_ERROR_RETURN(soc_maverick2_idb_wr_obm_thresh(unit, pipe_num,
                                                                pm_num,subp,
                                                                num_lanes,
                                                                lossless));
            LOG_DEBUG(BSL_LS_SOC_PORT, (BSL_META_U(unit,
                      "Write Srared config i %1d pipe=%1d pm=%1d "
                      "subp=%1d num_lanes=%1d\n"),
                      i, pipe_num, pm_num, subp, num_lanes));
            SOC_IF_ERROR_RETURN(soc_maverick2_idb_wr_obm_shared_config(unit,
                                                                      pipe_num,
                                                                      pm_num,
                                                                      subp,
                                                                      num_lanes,
                                                                      lossless));
            LOG_DEBUG(BSL_LS_SOC_PORT, (BSL_META_U(unit,
                      "Write Flow_ctrl_cfg i %1d pipe=%1d pm=%1d "
                      "subp=%1d num_lanes=%1d\n"),
                      i, pipe_num, pm_num, subp, num_lanes));
            SOC_IF_ERROR_RETURN(soc_maverick2_idb_wr_obm_flow_ctrl_cfg(unit,
                                                                      pipe_num,
                                                                      pm_num,
                                                                      subp,
                                                                      lossless));
            LOG_DEBUG(BSL_LS_SOC_PORT, (BSL_META_U(unit,
                      "Write fc_thresh i %1d pipe=%1d pm=%1d subp=%1d "
                      "num_lanes=%1d\n"),
                      i, pipe_num, pm_num, subp, num_lanes));
            SOC_IF_ERROR_RETURN(soc_maverick2_idb_wr_obm_fc_threshold(unit,
                                                                     pipe_num,
                                                                     pm_num,
                                                                     subp,
                                                                     num_lanes,
                                                                     lossless));
        }
    }

    SOC_IF_ERROR_RETURN(soc_maverick2_idb_obm_bubble_mop_set(unit,
                                                     port_schedule_state));
    LOG_DEBUG(BSL_LS_SOC_PORT, (BSL_META_U(unit,
              "Call soc_maverick2_idb_port_mode_set \n")));
    SOC_IF_ERROR_RETURN(soc_maverick2_idb_port_mode_set(unit,
                                                     port_schedule_state));
    LOG_DEBUG(BSL_LS_SOC_PORT, (BSL_META_U(unit,
              "Call soc_maverick2_idb_obm_force_saf_set \n")));
    SOC_IF_ERROR_RETURN(soc_maverick2_idb_obm_force_saf_set(unit,
                                                     port_schedule_state));
    LOG_DEBUG(BSL_LS_SOC_PORT, (BSL_META_U(unit,
              "Call soc_maverick2_idb_obm_port_config_set \n")));
    SOC_IF_ERROR_RETURN(soc_maverick2_idb_obm_port_config_set(unit,
                                                     port_schedule_state));
    LOG_DEBUG(BSL_LS_SOC_PORT, (BSL_META_U(unit,
              "Call soc_maverick2_idb_obm_dscp_map_set \n")));
    SOC_IF_ERROR_RETURN(soc_maverick2_idb_obm_dscp_map_set(unit,
                                                     port_schedule_state));
    LOG_DEBUG(BSL_LS_SOC_PORT, (BSL_META_U(unit,
              "Call soc_maverick2_idb_clear_stats_new_ports \n")));
    SOC_IF_ERROR_RETURN(soc_maverick2_idb_clear_stats_new_ports(unit,
                                                     port_schedule_state));

    return SOC_E_NONE;
}

/*! @fn int soc_maverick2_flex_idb_port_up(int unit,
 *              soc_port_schedule_state_t *port_schedule_state)
 *  @param unit Chip unit number.
 *  @param port_schedule_state Pointer to a soc_port_schedule_state_t struct variable
 *  @brief Helper function to bringup IDB port during flex operation.
 *  @returns SOC_E_NONE
 */
int
soc_maverick2_flex_idb_port_up(int                        unit,
                               soc_port_schedule_state_t *port_schedule_state)
{
    int i;
    int pipe_num;
    int pm_num;
    int reset_buffer;
    int phy_port;
    int subp;

    /* Release IDB buffers from reset state for all the ports going up */
    for (i = 0; i < port_schedule_state->nport; i++) {
        if (port_schedule_state->resource[i].physical_port != -1) {
            phy_port =
                port_schedule_state->out_port_map.port_l2p_mapping[
                    port_schedule_state
                    ->resource[i
                    ].logical_port];
            pipe_num = soc_maverick2_get_pipe_from_phy_pnum(phy_port);
            pm_num = soc_maverick2_get_pm_from_phy_pnum(phy_port);
            subp = soc_maverick2_get_subp_from_phy_pnum(phy_port);
            reset_buffer = 1;
            if (MAVERICK2_PHY_IS_FRONT_PANEL_PORT(phy_port)) {
                SOC_IF_ERROR_RETURN(soc_maverick2_idb_obm_reset_buffer(unit,
                                                                pipe_num,
                                                                pm_num, subp,
                                                                reset_buffer));
            }
            if (MAVERICK2_PHY_IS_FRONT_PANEL_PORT(phy_port)) {
                SOC_IF_ERROR_RETURN(soc_maverick2_idb_ca_reset_buffer(unit,
                                                               pipe_num,
                                                               pm_num, subp,
                                                               reset_buffer));
            } else if ((phy_port == MAVERICK2_PHY_PORT_LPBK0)||
                       (phy_port == MAVERICK2_PHY_PORT_LPBK1)) {
                SOC_IF_ERROR_RETURN(soc_maverick2_idb_lpbk_ca_reset_buffer(unit,
                                                               pipe_num,
                                                               reset_buffer));
            } else if ((phy_port == MAVERICK2_PHY_PORT_CPU)/* ||
                       (phy_port == MAVERICK2_PHY_PORT_MNG0) */) {
                SOC_IF_ERROR_RETURN(soc_maverick2_idb_cpu_ca_reset_buffer(unit,
                                                               pipe_num,
                                                               reset_buffer));
            } else if (phy_port == MAVERICK2_PHY_PORT_MNG0) {
                SOC_IF_ERROR_RETURN(soc_maverick2_idb_mgmt_ca_reset_buffer(unit,
                                                               pipe_num,
                                                               reset_buffer));
            }
        }
    }
    sal_usleep(5 + (SAL_BOOT_QUICKTURN ? 1 : 0) * EMULATION_FACTOR);

    for (i = 0; i < port_schedule_state->nport; i++) {
        if (port_schedule_state->resource[i].physical_port != -1) {
            phy_port =
                port_schedule_state->out_port_map.port_l2p_mapping[
                    port_schedule_state
                    ->resource[i
                    ].logical_port];
            pipe_num = soc_maverick2_get_pipe_from_phy_pnum(phy_port);
            pm_num = soc_maverick2_get_pm_from_phy_pnum(phy_port);
            subp = soc_maverick2_get_subp_from_phy_pnum(phy_port);
            LOG_DEBUG(BSL_LS_SOC_PORT,
                      (BSL_META_U(unit,
                             "IDB port Up i %1d phy_port %1d pipe=%1d pm=%1d "
                             "subp=%1d \n"),
                     i,
                     phy_port, pipe_num, pm_num, subp));
            reset_buffer = 0;
            if (MAVERICK2_PHY_IS_FRONT_PANEL_PORT(phy_port)) {
                SOC_IF_ERROR_RETURN(soc_maverick2_idb_obm_reset_buffer(unit,
                                                              pipe_num,
                                                              pm_num, subp,
                                                              reset_buffer));
            }
            if (MAVERICK2_PHY_IS_FRONT_PANEL_PORT(phy_port)) {
                SOC_IF_ERROR_RETURN(soc_maverick2_idb_ca_reset_buffer(unit,
                                                              pipe_num,
                                                              pm_num, subp,
                                                              reset_buffer));
            } else if ((phy_port == MAVERICK2_PHY_PORT_LPBK0)||
                       (phy_port == MAVERICK2_PHY_PORT_LPBK1)) {
                SOC_IF_ERROR_RETURN(soc_maverick2_idb_lpbk_ca_reset_buffer(unit,
                                                               pipe_num,
                                                               reset_buffer));
            } else if ((phy_port == MAVERICK2_PHY_PORT_CPU)/* ||
                       (phy_port == MAVERICK2_PHY_PORT_MNG0) */) {
                SOC_IF_ERROR_RETURN(soc_maverick2_idb_cpu_ca_reset_buffer(unit,
                                                               pipe_num,
                                                               reset_buffer));
            } else if (phy_port == MAVERICK2_PHY_PORT_MNG0) {
                SOC_IF_ERROR_RETURN(soc_maverick2_idb_mgmt_ca_reset_buffer(unit,
                                                               pipe_num,
                                                               reset_buffer));
            }
        }
    }
    return SOC_E_NONE;
}

/*** END SDK API COMMON CODE ***/

int
soc_maverick2_idb_flexport(int                        unit,
                           soc_port_schedule_state_t *port_schedule_state)
{

    SOC_IF_ERROR_RETURN(soc_maverick2_flex_start(unit, port_schedule_state));

    /* IDB Ports down. */
    SOC_IF_ERROR_RETURN(soc_maverick2_flex_idb_port_down(unit,
                                                    port_schedule_state));

    /* startTDM flex related codes: Florin's routines.*/
    SOC_IF_ERROR_RETURN(soc_maverick2_tdm_calculation_flexport(unit,
                                                    port_schedule_state));

    /* Reconfigure IDB. */
    SOC_IF_ERROR_RETURN(soc_maverick2_flex_idb_reconfigure(unit,
                                                    port_schedule_state));

    /* IDB Ports up*/
    SOC_IF_ERROR_RETURN(soc_maverick2_flex_idb_port_up(unit,
                                                    port_schedule_state));

    SOC_IF_ERROR_RETURN(soc_maverick2_flex_end(unit, port_schedule_state));

    return SOC_E_NONE;
}


#endif /* BCM_MAVERICK2_SUPPORT */
