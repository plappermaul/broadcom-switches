/*
 * $Id: iproc.c,v 1.17 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * iProc support
 */

#include <shared/bsl.h>

#include <sal/core/boot.h>
#include <sal/core/libc.h>
#include <sal/types.h>
#include <shared/alloc.h>
#include <soc/memtune.h>

#include <soc/drv.h>
#include <soc/debug.h>
#include <soc/cm.h>
#ifdef BCM_CMICM_SUPPORT
#include <soc/cmicm.h>
#endif
#include <ibde.h>

#ifdef BCM_IPROC_SUPPORT
#include <soc/iproc.h>

#ifdef BCM_IPROC_DDR_SUPPORT
#include <soc/shmoo_ddr40.h>
#include <soc/shmoo_and28.h>
#include <sal/appl/config.h>
#endif

/*
 * Function:
 *      soc_iproc_init
 * Purpose:
 *      Initialize iProc subsystem
 * Parameters:
 *      unit - unit number
 * Returns:
 *      SOC_E_XXX
 */
int soc_iproc_init(int unit)
{
    return SOC_E_NONE;
}

/*
 * Function:
 *      soc_iproc_deinit
 * Purpose:
 *      Free up resources aquired by init.
 * Parameters:
 *      unit - unit number
 * Returns:
 *      SOC_E_XXX
 * Notes:
 *       This function is currently not used.
 */
int soc_iproc_deinit(int unit)
{
    return SOC_E_NONE;
}

/*
 * Function:
 *      soc_iproc_getreg
 * Purpose:
 *      Read iProc register outside CMIC
 * Parameters:
 *      unit - unit number
 * Returns:
 *      SOC_E_XXX
 */
int soc_iproc_getreg(int unit, uint32 addr, uint32 *data)
{
    *data = soc_cm_iproc_read(unit, addr);
    return SOC_E_NONE;
}


/*
 * Function:
 *      soc_iproc_setreg
 * Purpose:
 *      Write iProc register outside CMIC
 * Parameters:
 *      unit - unit number
 * Returns:
 *      SOC_E_XXX
 */
int soc_iproc_setreg(int unit, uint32 addr, uint32 data)
{
    soc_cm_iproc_write(unit, addr, data);
    return SOC_E_NONE;
}

/*
 * Function:    soc_iproc_shutdown
 * Purpose:     Put an IPROC into sleep state
 * Parameters:  unit - unit number
 *              cpu_mask - mask of cores to shutdown
 *              level - shutdown level: 0=full, 1=partial
 * Returns:     SOC_E_xxxx
 */
int
soc_iproc_shutdown(int unit, uint32 cpu_mask, int level)
{
    uint32      i, rval;
    uint32      sram_base, sram_size;
    uint32      iproc_addr;

    if (!soc_feature(unit, soc_feature_iproc)) {
        return (SOC_E_FAIL);
    }

    if ((SOC_REG_IS_VALID(unit, CRU_IHOST_PWRDWN_STATUSr)) &&
        (SOC_REG_FIELD_VALID(unit, CRU_IHOST_PWRDWN_STATUSr, LOGIC_PWRDOWN_CPU0f))) {
        READ_CRU_IHOST_PWRDWN_STATUSr(unit, &rval);
        if (soc_reg_field_get(unit, CRU_IHOST_PWRDWN_STATUSr, rval, LOGIC_PWRDOWN_CPU0f) == 1) {
            /* if the uC has been shut down, do nothing */
            LOG_VERBOSE(BSL_LS_SOC_COMMON,
                        (BSL_META_U(unit,
                                    "The uC has been shut down, do nothing.\n")));
            return (SOC_E_NONE);
        }
    }

    LOG_VERBOSE(BSL_LS_SOC_COMMON,
                (BSL_META_U(unit,
                            "iproc_shutdown\n")));

    if (SOC_IS_HELIX4(unit)) {
        /* Assumes booting in QSPI mode with MDIO tied high */
        sram_base = 0x1b000000;
        sram_size = 512 * 1024;
        iproc_addr = sram_base + sram_size - 8;

        /* load WFI loop into SRAM (ARM mode) */
        soc_cm_iproc_write(unit, iproc_addr, 0xe320f003);
        soc_cm_iproc_write(unit, iproc_addr + 4, 0xeafffffd);

        /* Update LUT to point at WFI loop */
        for (i = 0; i < 8; ++i) {
            soc_cm_iproc_write(unit, 0xffff0400 + i*4, iproc_addr);
        }
        /* core 0 should be in WFI now */
        if (level == 0) {
            if (cpu_mask & 0x2) {
                READ_CRU_IHOST_PWRDWN_ENr(unit, &rval);
                soc_reg_field_set(unit, CRU_IHOST_PWRDWN_ENr, &rval,
                                  LOGIC_CLAMP_ON_NEON1f, 1);
                WRITE_CRU_IHOST_PWRDWN_ENr(unit, rval);
                soc_reg_field_set(unit, CRU_IHOST_PWRDWN_ENr, &rval,
                                  LOGIC_PWRDOWN_NEON1f, 1);
                WRITE_CRU_IHOST_PWRDWN_ENr(unit, rval);
                soc_reg_field_set(unit, CRU_IHOST_PWRDWN_ENr, &rval,
                                  LOGIC_CLAMP_ON_CPU1f, 1);
                WRITE_CRU_IHOST_PWRDWN_ENr(unit, rval);
                soc_reg_field_set(unit, CRU_IHOST_PWRDWN_ENr, &rval,
                                  RAM_CLAMP_ON_CPU1f, 1);
                WRITE_CRU_IHOST_PWRDWN_ENr(unit, rval);
                soc_reg_field_set(unit, CRU_IHOST_PWRDWN_ENr, &rval,
                                  LOGIC_PWRDOWN_CPU1f, 1);
                WRITE_CRU_IHOST_PWRDWN_ENr(unit, rval);
                soc_reg_field_set(unit, CRU_IHOST_PWRDWN_ENr, &rval,
                                  RAM_PWRDOWN_CPU1f, 1);
                WRITE_CRU_IHOST_PWRDWN_ENr(unit, rval);
            }
            /* Put cpu0 into shutdown after 100 clocks */
            READ_CRU_CPU0_POWERDOWNr(unit, &rval);
            soc_reg_field_set(unit, CRU_CPU0_POWERDOWNr, &rval,
                              START_CPU0_POWERDOWN_SEQf, 1);
            soc_reg_field_set(unit, CRU_CPU0_POWERDOWNr, &rval,
                              CPU0_POWERDOWN_TIMERf, 100);
            WRITE_CRU_CPU0_POWERDOWNr(unit, rval);
        }
    }
    return (SOC_E_NONE);
}


/*
 * Function:
 *      soc_iproc_config_genpll
 * Purpose:
 *      Initialize IPROC gen_pll
 * Parameters:
 *      unit - unit number
 *      genpll_clk_tbl - the clock table for gen_pll
 * Returns:
 *      SOC_E_XXX
 */
int
soc_iproc_config_genpll(int unit, uint32 *genpll_clk_tbl)
{
    soc_timeout_t to;
    sal_usecs_t to_val = 500000; /* 500ms */
    uint32 rval;
    int i = 0;

    /* Clear Load_en Channel3 & Channel4 */
    SOC_IF_ERROR_RETURN(READ_IPROC_WRAP_GEN_PLL_CTRL3r(unit, &rval));
    rval &= 0xffc0ffff;
    SOC_IF_ERROR_RETURN(WRITE_IPROC_WRAP_GEN_PLL_CTRL3r(unit, rval));

    /* Write fast_lock =1 */
    SOC_IF_ERROR_RETURN(READ_IPROC_WRAP_GEN_PLL_CTRL0r(unit, &rval));
    soc_reg_field_set(unit, IPROC_WRAP_GEN_PLL_CTRL0r, &rval, FAST_LOCKf, 1);
    SOC_IF_ERROR_RETURN(WRITE_IPROC_WRAP_GEN_PLL_CTRL0r(unit, rval));

    /* Write NDIV */
    SOC_IF_ERROR_RETURN(READ_IPROC_WRAP_GEN_PLL_CTRL1r(unit, &rval));
    rval &= 0xfffffc00;
    soc_reg_field_set(unit, IPROC_WRAP_GEN_PLL_CTRL1r, &rval, NDIV_INTf, genpll_clk_tbl[i+1]);
    SOC_IF_ERROR_RETURN(WRITE_IPROC_WRAP_GEN_PLL_CTRL1r(unit, rval));

    /* Write MDIV */
    SOC_IF_ERROR_RETURN(READ_IPROC_WRAP_GEN_PLL_CTRL2r(unit, &rval));
    rval &= 0xff0000ff;
    soc_reg_field_set(unit, IPROC_WRAP_GEN_PLL_CTRL2r, &rval, CH3_MDIVf, genpll_clk_tbl[i+5]);
    soc_reg_field_set(unit, IPROC_WRAP_GEN_PLL_CTRL2r, &rval, CH4_MDIVf, genpll_clk_tbl[i+6]);
    SOC_IF_ERROR_RETURN(WRITE_IPROC_WRAP_GEN_PLL_CTRL2r(unit, rval));

    /* Write PLL_LOAD */
    SOC_IF_ERROR_RETURN(READ_IPROC_WRAP_GEN_PLL_CTRL3r(unit, &rval));
    soc_reg_field_set(unit, IPROC_WRAP_GEN_PLL_CTRL3r, &rval, SW_TO_GEN_PLL_LOADf, 1);
    SOC_IF_ERROR_RETURN(WRITE_IPROC_WRAP_GEN_PLL_CTRL3r(unit, rval));


    /* Load Channel3 & Channel4 */
    rval &= 0xffc0ffff;
    soc_reg_field_set(unit, IPROC_WRAP_GEN_PLL_CTRL3r, &rval, LOAD_EN_CHf, 0x18);
    SOC_IF_ERROR_RETURN(WRITE_IPROC_WRAP_GEN_PLL_CTRL3r(unit, rval));

    /* Wait for IPROC_WWRAP GENPLL lock */
    soc_timeout_init(&to, to_val, 0);
    for(;;) {
        SOC_IF_ERROR_RETURN(READ_IPROC_WRAP_GEN_PLL_STATUSr(unit, &rval));
        if (soc_reg_field_get(unit, IPROC_WRAP_GEN_PLL_STATUSr, rval, GEN_PLL_LOCKf)) {
            break;
        }
        if (soc_timeout_check(&to)) {
            LOG_ERROR(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                    "Timed out waiting for IPROC_WWRAP GENPLL lock\n")));

            return SOC_E_TIMEOUT;
        }
    }

    return SOC_E_NONE;
}

#ifdef BCM_IPROC_DDR_SUPPORT

#ifdef BCM_HURRICANE2_SUPPORT
const static unsigned int hr2_ddr_init_tab[] = {
     14, 0x00000001,
     36, 0x01000000,
     37, 0x10000000,
     38, 0x00100400,
     39, 0x00000400,
     40, 0x00000100,
     42, 0x00000001,
     61, 0x00010100,
     78, 0x01000200,
     79, 0x02000040,
     80, 0x00400100,
     81, 0x00000200,
     83, 0x01ffff0a,
     84, 0x01010101,
     85, 0x01010101,
     86, 0x0f000003,
     87, 0x0000010c,
     88, 0x00010000,
    112, 0x00000200,
    116, 0x0d000000,
    117, 0x00000028,
    119, 0x00010001,
    120, 0x00010001,
    121, 0x00010001,
    122, 0x00010001,
    123, 0x00010001,
    130, 0x00000001,
    139, 0x00000001,
    148, 0x00000001,
    149, 0x00000000,
    150, 0x00000000,
    152, 0x03030303,
    153, 0x03030303,
    156, 0x02006400,
    157, 0x02020202,
    158, 0x00020202,
    160, 0x01000000,
    161, 0x01010064,
    162, 0x01010101,
    163, 0x00000101,
    165, 0x00020000,
    166, 0x00000064,
    168, 0x000b0b00,
    170, 0x02000200,
    171, 0x02000200,
    175, 0x02000200,
    176, 0x02000200,
    180, 0x80000100,
    181, 0x04070303,
    182, 0x0000000a,
    185, 0x0010ffff,
    187, 0x0000000f,
    194, 0x00000204,
    205, 0x00000000,
    0xffffffff
};

const static unsigned int hr2_ddr_init_tab_800[] = {
      0, 0x00000600,
      1, 0x00000000,
      3, 0x00000050,
      4, 0x000000c8,
      5, 0x0c050c00,
      6, 0x04040405,
      7, 0x06041018,
      8, 0x04101804,
      9, 0x0c040406,
     10, 0x03006db0,
     11, 0x0c040404,
     12, 0x03006db0,
     13, 0x01010004,
     15, 0x000c0c00,
     16, 0x03000200,
     17, 0x00001212,
     18, 0x06060000,
     19, 0x00000000,
     20, 0x00009001,
     21, 0x00900c28,
     22, 0x00050c28,
     23, 0x00000300,
     24, 0x000a0003,
     25, 0x0000000a,
     26, 0x00000000,
     27, 0x02000000,
     28, 0x0200006c,
     29, 0x0000006c,
     30, 0x05000001,
     31, 0x00050505,
     32, 0x00000000,
     35, 0x00000000,
     41, 0x00000000,
     43, 0x00000000,
     44, 0x00042000,
     45, 0x00000046,
     46, 0x00460420,
     47, 0x00000000,
     48, 0x04200000,
     49, 0x00000046,
     50, 0x00460420,
     51, 0x00000000,
     52, 0x04200000,
     53, 0x00000046,
     54, 0x00460420,
     55, 0x00000000,
     56, 0x04200000,
     57, 0x00000046,
     58, 0x00460420,
     59, 0x00000000,
     60, 0x00000000,
     62, 0x00000000,
     63, 0x00000000,
     64, 0x00000000,
     65, 0x00000000,
     66, 0x00000000,
     67, 0x00000000,
     68, 0x00000000,
     69, 0x00000000,
     70, 0x00000000,
     71, 0x00000000,
     72, 0x00000000,
     73, 0x00000000,
     74, 0x00000000,
     75, 0x00000000,
     76, 0x00000000,
     77, 0x00000000,
     82, 0x01010001,
     89, 0x00000000,
     90, 0x00000000,
     91, 0x00000000,
     92, 0x00000000,
     93, 0x00000000,
     94, 0x00000000,
     95, 0x00000000,
     96, 0x00000000,
     97, 0x00000000,
     98, 0x00000000,
     99, 0x00000000,
    100, 0x00000000,
    101, 0x00000000,
    102, 0x00000000,
    103, 0x00000000,
    104, 0x00000000,
    105, 0x00000000,
    106, 0x00000000,
    107, 0x00000000,
    108, 0x02040108,
    109, 0x08010402,
    110, 0x02020002,
    111, 0x01000200,
    113, 0x00000000,
    114, 0x00000000,
    115, 0x00000000,
    118, 0x00000000,
    124, 0x00000000,
    125, 0x00000000,
    126, 0x00000000,
    127, 0x00000000,
    128, 0x00212100,
    129, 0x21210001,
    131, 0x00000000,
    132, 0x00000000,
    133, 0x00012121,
    134, 0x00012121,
    135, 0x00000000,
    136, 0x00000000,
    137, 0x00212100,
    138, 0x21210001,
    140, 0x00000000,
    141, 0x00000000,
    142, 0x00012121,
    143, 0x00012121,
    144, 0x00000000,
    145, 0x00000000,
    146, 0x00212100,
    147, 0x21210001,
    151, 0x00000000,
    167, 0x00000000,
    169, 0x0c280000,
    172, 0x00000c28,
    173, 0x00007990,
    174, 0x0c280505,
    177, 0x00000c28,
    178, 0x00007990,
    179, 0x02020505,
    183, 0x00000000,
    184, 0x00000000,
    186, 0x00070303,
    188, 0x00000000,
    189, 0x00000000,
    190, 0x00000000,
    191, 0x00000000,
    192, 0x00000000,
    193, 0x00000000,
    195, 0x00000000,
    196, 0x00000000,
    197, 0x00000000,
    198, 0x00000000,
    199, 0x00000000,
    200, 0x00000000,
    201, 0x00000000,
    202, 0x00000004,
    203, 0x00000004,
    204, 0x00000000,
    206, 0x02040401,
    207, 0x00000002,
    208, 0x00000000,
    209, 0x00000000,
    210, 0x06060000,
    211, 0x00000606,
    212, 0x00000040,
    213, 0x00000000,
    214, 0x01010606,
    215, 0x00000101,
    216, 0x00001c1c,
    217, 0x00000000,
    0xffffffff
};

#endif /* BCM_HURRICANE2_SUPPORT */

#ifdef BCM_GREYHOUND_SUPPORT

unsigned int gh_ddr_init_tab[] = {
    0xffffffff
};

unsigned int gh_ddr3_init_tab_800[] = {
      0, 0x00000600,
      1, 0x00000000,
      2, 0x00000000,
      3, 0x00000000,
      4, 0x00000000,
      5, 0x00000000,
      6, 0x00000000,
      7, 0x00000400,
      8, 0x00000000,
      9, 0x00000000,
     10, 0x00000000,
     11, 0x00000004,
     12, 0x00000000,
     13, 0x00000000,
     14, 0x01000000,
     15, 0x00000050,
     16, 0x000000c8,
     17, 0x00000000,
     18, 0x02000000,
     19, 0x00050c00,
     20, 0x00000000,
     21, 0x00050c00,
     22, 0x00000000,
     23, 0x05040400,
     24, 0x0f150004,
     25, 0x15060404,
     26, 0x15000405,
     27, 0x0604040f,
     28, 0x04040415,
     29, 0x006db00c,
     30, 0x040c0503,
     31, 0x000c0404,
     32, 0x03006db0,
     33, 0x06000c05,
     34, 0x00060606,
     35, 0x010c003c,
     36, 0x0a140a0a,
     37, 0x0100000a,
     38, 0x0c0c0101,
     39, 0x01060603,
     40, 0x00010100,
     41, 0x00000000,
     42, 0x00000000,
     43, 0x008c0100,
     44, 0x00000c28,
     45, 0x0000008c,
     46, 0x00000c28,
     47, 0x00000005,
     48, 0x00030003,
     49, 0x000a000a,
     50, 0x00000000,
     51, 0x00000000,
     52, 0x02000000,
     53, 0x02000090,
     54, 0x00000090,
     55, 0x05000001,
     56, 0x00050505,
     57, 0x00000000,
     58, 0x00000000,
     59, 0x00010000,
     60, 0x10040010,
     61, 0x00000400,
     62, 0x00000200,
     63, 0x00000000,
     64, 0x00000001,
     65, 0x00000000,
     66, 0x00000000,
     67, 0x00000000,
     68, 0x00000000,
     69, 0x00000000,
     70, 0x00000000,
     71, 0x00000000,
     72, 0x00000000,
     73, 0x00000000,
     74, 0x00000000,
     75, 0x00000000,
     76, 0x00000000,
     77, 0x00000000,
     78, 0x00000000,
     79, 0x00000000,
     80, 0x00000000,
     81, 0x00000000,
     82, 0x00000000,
     83, 0x00000000,
     84, 0x00000420,
     85, 0x00000046,
     86, 0x00000000,
     87, 0x00000420,
     88, 0x00000046,
     89, 0x00000000,
     90, 0x00000000,
     92, 0x00000000,
     93, 0x00000000,
     94, 0x00000000,
     95, 0x00000000,
     96, 0x00000000,
     97, 0x00000000,
     98, 0x00000420,
     99, 0x00000046,
    100, 0x00000000,
    101, 0x00000420,
    102, 0x00000046,
    103, 0x00000000,
    104, 0x00000000,
    106, 0x00000000,
    107, 0x00000000,
    108, 0x00000000,
    109, 0x00000000,
    110, 0x00000000,
    111, 0x00000000,
    139, 0x00000000,
    140, 0x01000000,
    141, 0x00000001,
    142, 0x00000000,
    143, 0x00000000,
    144, 0x00000000,
    145, 0x00000000,
    146, 0x00000000,
    147, 0x00000000,
    148, 0x00000000,
    149, 0x00000000,
    150, 0x00000000,
    152, 0x00000000,
    153, 0x00000000,
    155, 0x00000000,
    156, 0x00000000,
    157, 0x01000200,
    158, 0x02000040,
    159, 0x00400100,
    160, 0x00000200,
    161, 0x00000040,
    162, 0x00000000,
    163, 0x01000000,
    164, 0x0a020101,
    165, 0x0101ffff,
    166, 0x01010101,
    167, 0x01010101,
    168, 0x00000103,
    169, 0x00000c03,
    170, 0x00000000,
    171, 0x00000000,
    172, 0x01000000,
    173, 0x00000100,
    174, 0x00000000,
    175, 0x00000000,
    176, 0x00000000,
    177, 0x00000000,
    178, 0x00000000,
    179, 0x00000000,
    180, 0x00000000,
    181, 0x00000000,
    182, 0x00000000,
    183, 0x00000000,
    184, 0x00000000,
    185, 0x00000000,
    186, 0x00000000,
    187, 0x00000000,
    188, 0x00000000,
    189, 0x00000000,
    190, 0x00000000,
    191, 0x00000000,
    192, 0x00000000,
    193, 0x00000000,
    194, 0x00000000,
    195, 0x00000000,
    196, 0x01010200,
    197, 0x00000002,
    198, 0x06040400,
    199, 0x00000106,
    200, 0x01010000,
    201, 0x02020002,
    202, 0x02020101,
    203, 0x02000102,
    204, 0x00000000,
    205, 0x00000000,
    206, 0x00000000,
    207, 0x00000000,
    208, 0x00000000,
    209, 0x0000280d,
    210, 0x00010000,
    211, 0x00010000,
    212, 0x00000003,
    213, 0x00000000,
    214, 0x00000000,
    215, 0x00000000,
    216, 0x00000000,
    217, 0x00000000,
    218, 0x00000000,
    219, 0x00000000,
    220, 0x00000000,
    221, 0x01000000,
    222, 0x00000001,
    223, 0x00000100,
    224, 0x00030300,
    225, 0x0556AA00,
    226, 0x000aa955,
    227, 0x000aa955,
    228, 0x000556aa,
    229, 0x000556aa,
    230, 0x000aa955,
    231, 0x000aa955,
    232, 0x030556aa,
    233, 0x01000001,
    234, 0x00010300,
    235, 0x00676700,
    236, 0x00676700,
    237, 0x00676700,
    238, 0x00676700,
    239, 0x00676700,
    240, 0x00676700,
    241, 0x00676700,
    242, 0x00676700,
    243, 0x00676700,
    245, 0x01000000,
    246, 0x00000001,
    247, 0x00000101,
    248, 0x00010100,
    249, 0x03000000,
    250, 0x03030303,
    251, 0x00030303,
    252, 0x02020064,
    253, 0x02020202,
    254, 0x00010202,
    255, 0x01010064,
    256, 0x01010101,
    257, 0x00020101,
    258, 0x00000064,
    259, 0x00000000,
    260, 0x000d0d00,
    261, 0x00000000,
    262, 0x00001850,
    263, 0x02000200,
    264, 0x02000200,
    265, 0x00001850,
    266, 0x00007990,
    267, 0x00000404,
    268, 0x00001850,
    269, 0x02000200,
    270, 0x02000200,
    271, 0x00001850,
    272, 0x00007990,
    273, 0x02020404,
    274, 0x000a0100,
    275, 0x0000000a,
    276, 0x00000000,
    277, 0x00000000,
    278, 0x00000f0a,
    279, 0x00000000,
    280, 0x00000000,
    281, 0x00000000,
    282, 0x00000000,
    283, 0x00000000,
    284, 0x00000103,
    285, 0x00010003,
    286, 0x00000003,
    287, 0x00000000,
    288, 0x00000000,
    289, 0x00000000,
    290, 0x05000000,
    291, 0x00040504,
    292, 0x00000000,
    293, 0x00000000,
    294, 0x00000000,
    295, 0x00000000,
    296, 0x00000000,
    297, 0x00000000,
    298, 0x00000000,
    299, 0x00000000,
    300, 0x00000000,
    301, 0x00000000,
    302, 0x00000000,
    303, 0x00000000,
    304, 0x00000000,
    0xffffffff
};
unsigned int gh_ddr3_init_tab_1333[] = {
      0, 0x00000600,
      1, 0x00000000,
      2, 0x00000000,
      3, 0x00000000,
      4, 0x00000000,
      5, 0x00000000,
      6, 0x00000000,
      7, 0x00000700,
      8, 0x00000000,
      9, 0x00000000,
     10, 0x00000000,
     11, 0x00000007,
     12, 0x00000000,
     13, 0x00000000,
     14, 0x01000000,
     15, 0x00000086,
     16, 0x0000014e,
     17, 0x00000000,
     18, 0x02000000,
     19, 0x00071200,
     20, 0x00000000,
     21, 0x00071200,
     22, 0x00000000,
     23, 0x05040400,
     24, 0x18210005,
     25, 0x1e090505,
     26, 0x21000505,
     27, 0x09050518,
     28, 0x0405051e,
     29, 0x00b6d00c,
     30, 0x050c0504,
     31, 0x000c0405,
     32, 0x0400b6d0,
     33, 0x09000c05,
     34, 0x000a090a,
     35, 0x010c0064,
     36, 0x0a140a0a,
     37, 0x0100000a,
     38, 0x13130101,
     39, 0x00090903,
     40, 0x00010100,
     41, 0x00000000,
     42, 0x00000000,
     43, 0x00ea0100,
     44, 0x00001448,
     45, 0x000000ea,
     46, 0x00001448,
     47, 0x00000005,
     48, 0x00040004,
     49, 0x00100010,
     50, 0x00000000,
     51, 0x00000000,
     52, 0x02000000,
     53, 0x020000f0,
     54, 0x000000f0,
     55, 0x07000001,
     56, 0x00070707,
     57, 0x00000000,
     58, 0x00000000,
     59, 0x00010000,
     60, 0x10040010,
     61, 0x00000400,
     62, 0x00000200,
     63, 0x00000000,
     64, 0x00000001,
     65, 0x00000000,
     66, 0x00000000,
     67, 0x00000000,
     68, 0x00000000,
     69, 0x00000000,
     70, 0x00000000,
     71, 0x00000000,
     72, 0x00000000,
     73, 0x00000000,
     74, 0x00000000,
     75, 0x00000000,
     76, 0x00000000,
     77, 0x00000000,
     78, 0x00000000,
     79, 0x00000000,
     80, 0x00000000,
     81, 0x00000000,
     82, 0x00000000,
     83, 0x00000000,
     84, 0x00000a50,
     85, 0x00000046,
     86, 0x00000010,
     87, 0x00000a50,
     88, 0x00000046,
     89, 0x00000010,
     90, 0x00000000,
     92, 0x00000000,
     93, 0x00000000,
     94, 0x00000000,
     95, 0x00000000,
     96, 0x00000000,
     97, 0x00000000,
     98, 0x00000a50,
     99, 0x00000046,
    100, 0x00000010,
    101, 0x00000a50,
    102, 0x00000046,
    103, 0x00000010,
    104, 0x00000000,
    106, 0x00000000,
    107, 0x00000000,
    108, 0x00000000,
    109, 0x00000000,
    110, 0x00000000,
    111, 0x00000000,
    139, 0x00000000,
    140, 0x01000000,
    141, 0x00000001,
    142, 0x00000000,
    143, 0x00000000,
    144, 0x00000000,
    145, 0x00000000,
    146, 0x00000000,
    147, 0x00000000,
    148, 0x00000000,
    149, 0x00000000,
    150, 0x00000000,
    152, 0x00000000,
    153, 0x00000000,
    155, 0x00000000,
    156, 0x00000000,
    157, 0x01000200,
    158, 0x02000040,
    159, 0x00400100,
    160, 0x00000200,
    161, 0x00000040,
    162, 0x00000000,
    163, 0x01000000,
    164, 0x0a020101,
    165, 0x0101ffff,
    166, 0x01010101,
    167, 0x01010101,
    168, 0x00000103,
    169, 0x00000c03,
    170, 0x00000000,
    171, 0x00000000,
    172, 0x01000000,
    173, 0x00000100,
    174, 0x00000000,
    175, 0x00000000,
    176, 0x00000000,
    177, 0x00000000,
    178, 0x00000000,
    179, 0x00000000,
    180, 0x00000000,
    181, 0x00000000,
    182, 0x00000000,
    183, 0x00000000,
    184, 0x00000000,
    185, 0x00000000,
    186, 0x00000000,
    187, 0x00000000,
    188, 0x00000000,
    189, 0x00000000,
    190, 0x00000000,
    191, 0x00000000,
    192, 0x00000000,
    193, 0x00000000,
    194, 0x00000000,
    195, 0x00000000,
    196, 0x01010200,
    197, 0x00000002,
    198, 0x06060600,
    199, 0x00000106,
    200, 0x02020000,
    201, 0x02020002,
    202, 0x02020101,
    203, 0x02000102,
    204, 0x00000000,
    205, 0x00000000,
    206, 0x00000000,
    207, 0x00000000,
    208, 0x00000000,
    209, 0x0000280d,
    210, 0x00010000,
    211, 0x00010000,
    212, 0x00000003,
    213, 0x00000000,
    214, 0x00000000,
    215, 0x00000000,
    216, 0x00000000,
    217, 0x00000000,
    218, 0x00000000,
    219, 0x00000000,
    220, 0x00000000,
    221, 0x01000000,
    222, 0x00000001,
    223, 0x00000100,
    224, 0x00030300,
    225, 0x0556AA00,
    226, 0x000aa955,
    227, 0x000aa955,
    228, 0x000556aa,
    229, 0x000556aa,
    230, 0x000aa955,
    231, 0x000aa955,
    232, 0x030556aa,
    233, 0x01000001,
    234, 0x00010300,
    235, 0x00676700,
    236, 0x00676700,
    237, 0x00676700,
    238, 0x00676700,
    239, 0x00676700,
    240, 0x00676700,
    241, 0x00676700,
    242, 0x00676700,
    243, 0x00676700,
    245, 0x01000000,
    246, 0x00000001,
    247, 0x00000101,
    248, 0x00010100,
    249, 0x03000000,
    250, 0x03030303,
    251, 0x00030303,
    252, 0x02020064,
    253, 0x02020202,
    254, 0x00010202,
    255, 0x01010064,
    256, 0x01010101,
    257, 0x00020101,
    258, 0x00000064,
    259, 0x00000000,
    260, 0x000e0e00,
    261, 0x00000000,
    262, 0x00002890,
    263, 0x02000200,
    264, 0x02000200,
    265, 0x00002890,
    266, 0x0000cad0,
    267, 0x00000708,
    268, 0x00002890,
    269, 0x02000200,
    270, 0x02000200,
    271, 0x00002890,
    272, 0x0000cad0,
    273, 0x02020708,
    274, 0x000a0100,
    275, 0x0000000a,
    276, 0x00000000,
    277, 0x00000000,
    278, 0x00000f0a,
    279, 0x00000000,
    280, 0x00000000,
    281, 0x00000000,
    282, 0x00000000,
    283, 0x00000000,
    284, 0x00000103,
    285, 0x00010003,
    286, 0x00000003,
    287, 0x00000000,
    288, 0x00000000,
    289, 0x00000000,
    290, 0x08000000,
    291, 0x00060806,
    292, 0x00000000,
    293, 0x00000000,
    294, 0x00000000,
    295, 0x00000000,
    296, 0x00000000,
    297, 0x00000000,
    298, 0x00000000,
    299, 0x00000000,
    300, 0x00000000,
    301, 0x00000000,
    302, 0x00000000,
    303, 0x00000000,
    304, 0x00000000,
    0xffffffff
};

unsigned int gh_ddr3_init_tab_1600[] = {
      0, 0x00000600,
      1, 0x00000000,
      2, 0x00000000,
      3, 0x00000000,
      4, 0x00000000,
      5, 0x00000000,
      6, 0x00000000,
      7, 0x00000800,
      8, 0x00000000,
      9, 0x00000000,
     10, 0x00000000,
     11, 0x00000008,
     12, 0x00000000,
     13, 0x00000000,
     14, 0x01000000,
     15, 0x000000a0,
     16, 0x00000190,
     17, 0x00000000,
     18, 0x02000000,
     19, 0x00081600,
     20, 0x00000000,
     21, 0x00081600,
     22, 0x00000000,
     23, 0x05040400,
     24, 0x1c270006,
     25, 0x200b0607,
     26, 0x27000605,
     27, 0x0b06061c,
     28, 0x04060620,
     29, 0x00db600c,
     30, 0x060c0504,
     31, 0x000c0406,
     32, 0x0400db60,
     33, 0x0b000c05,
     34, 0x000c0b0c,
     35, 0x010c0078,
     36, 0x0a140a0a,
     37, 0x0100000a,
     38, 0x17170101,
     39, 0x000b0b03,
     40, 0x00010100,
     41, 0x00000000,
     42, 0x00000000,
     43, 0x01180100,
     44, 0x00001858,
     45, 0x00000118,
     46, 0x00001858,
     47, 0x00000005,
     48, 0x00050005,
     49, 0x00140014,
     50, 0x00000000,
     51, 0x00000000,
     52, 0x02000000,
     53, 0x02000120,
     54, 0x00000120,
     55, 0x08000001,
     56, 0x00080808,
     57, 0x00000000,
     58, 0x00000000,
     59, 0x00010000,
     60, 0x10040010,
     61, 0x00000400,
     62, 0x00000200,
     63, 0x00000000,
     64, 0x00000001,
     65, 0x00000000,
     66, 0x00000000,
     67, 0x00000000,
     68, 0x00000000,
     69, 0x00000000,
     70, 0x00000000,
     71, 0x00000000,
     72, 0x00000000,
     73, 0x00000000,
     74, 0x00000000,
     75, 0x00000000,
     76, 0x00000000,
     77, 0x00000000,
     78, 0x00000000,
     79, 0x00000000,
     80, 0x00000000,
     81, 0x00000000,
     82, 0x00000000,
     83, 0x00000000,
     84, 0x00000c70,
     85, 0x00000046,
     86, 0x00000018,
     87, 0x00000c70,
     88, 0x00000046,
     89, 0x00000018,
     90, 0x00000000,
     92, 0x00000000,
     93, 0x00000000,
     94, 0x00000000,
     95, 0x00000000,
     96, 0x00000000,
     97, 0x00000000,
     98, 0x00000c70,
     99, 0x00000046,
    100, 0x00000018,
    101, 0x00000c70,
    102, 0x00000046,
    103, 0x00000018,
    104, 0x00000000,
    106, 0x00000000,
    107, 0x00000000,
    108, 0x00000000,
    109, 0x00000000,
    110, 0x00000000,
    111, 0x00000000,
    139, 0x00000000,
    140, 0x01000000,
    141, 0x00000001,
    142, 0x00000000,
    143, 0x00000000,
    144, 0x00000000,
    145, 0x00000000,
    146, 0x00000000,
    147, 0x00000000,
    148, 0x00000000,
    149, 0x00000000,
    150, 0x00000000,
    152, 0x00000000,
    153, 0x00000000,
    155, 0x00000000,
    156, 0x00000000,
    157, 0x01000200,
    158, 0x02000040,
    159, 0x00400100,
    160, 0x00000200,
    161, 0x00000040,
    162, 0x00000000,
    163, 0x01000000,
    164, 0x0a020101,
    165, 0x0101ffff,
    166, 0x01010101,
    167, 0x01010101,
    168, 0x00000103,
    169, 0x00000c03,
    170, 0x00000000,
    171, 0x00000000,
    172, 0x01000000,
    173, 0x00000100,
    174, 0x00000000,
    175, 0x00000000,
    176, 0x00000000,
    177, 0x00000000,
    178, 0x00000000,
    179, 0x00000000,
    180, 0x00000000,
    181, 0x00000000,
    182, 0x00000000,
    183, 0x00000000,
    184, 0x00000000,
    185, 0x00000000,
    186, 0x00000000,
    187, 0x00000000,
    188, 0x00000000,
    189, 0x00000000,
    190, 0x00000000,
    191, 0x00000000,
    192, 0x00000000,
    193, 0x00000000,
    194, 0x00000000,
    195, 0x00000000,
    196, 0x01010200,
    197, 0x00000002,
    198, 0x06070700,
    199, 0x00000106,
    200, 0x03030000,
    201, 0x02020002,
    202, 0x02020101,
    203, 0x02000102,
    204, 0x00000000,
    205, 0x00000000,
    206, 0x00000000,
    207, 0x00000000,
    208, 0x00000000,
    209, 0x0000280d,
    210, 0x00010000,
    211, 0x00010000,
    212, 0x00000003,
    213, 0x00000000,
    214, 0x00000000,
    215, 0x00000000,
    216, 0x00000000,
    217, 0x00000000,
    218, 0x00000000,
    219, 0x00000000,
    220, 0x00000000,
    221, 0x01000000,
    222, 0x00000001,
    223, 0x00000100,
    224, 0x00030300,
    225, 0x0556AA00,
    226, 0x000aa955,
    227, 0x000aa955,
    228, 0x000556aa,
    229, 0x000556aa,
    230, 0x000aa955,
    231, 0x000aa955,
    232, 0x030556aa,
    233, 0x01000001,
    234, 0x00010300,
    235, 0x00676700,
    236, 0x00676700,
    237, 0x00676700,
    238, 0x00676700,
    239, 0x00676700,
    240, 0x00676700,
    241, 0x00676700,
    242, 0x00676700,
    243, 0x00676700,
    245, 0x01000000,
    246, 0x00000001,
    247, 0x00000101,
    248, 0x00010100,
    249, 0x03000000,
    250, 0x03030303,
    251, 0x00030303,
    252, 0x02020064,
    253, 0x02020202,
    254, 0x00010202,
    255, 0x01010064,
    256, 0x01010101,
    257, 0x00020101,
    258, 0x00000064,
    259, 0x00000000,
    260, 0x000e0e00,
    261, 0x00000000,
    262, 0x000030b0,
    263, 0x02000200,
    264, 0x02000200,
    265, 0x000030b0,
    266, 0x0000f370,
    267, 0x0000080a,
    268, 0x000030b0,
    269, 0x02000200,
    270, 0x02000200,
    271, 0x000030b0,
    272, 0x0000f370,
    273, 0x0202080a,
    274, 0x000a0100,
    275, 0x0000000a,
    276, 0x00000000,
    277, 0x00000000,
    278, 0x00000f0a,
    279, 0x00000000,
    280, 0x00000000,
    281, 0x00000000,
    282, 0x00000000,
    283, 0x00000000,
    284, 0x00000103,
    285, 0x00010003,
    286, 0x00000003,
    287, 0x00000000,
    288, 0x00000000,
    289, 0x00000000,
    290, 0x0a000000,
    291, 0x00070a07,
    292, 0x00000000,
    293, 0x00000000,
    294, 0x00000000,
    295, 0x00000000,
    296, 0x00000000,
    297, 0x00000000,
    298, 0x00000000,
    299, 0x00000000,
    300, 0x00000000,
    301, 0x00000000,
    302, 0x00000000,
    303, 0x00000000,
    304, 0x00000000,
    0xffffffff
};
#endif /* BCM_GREYHOUND_SUPPORT */

#ifdef BCM_HURRICANE3_SUPPORT

unsigned int hr3_ddr_init_tab[] = {
    0xffffffff
};

unsigned int hr3_ddr3_init_tab_1600[] = {
      0, 0x00000600,
      1, 0x00000000,
      2, 0x00000000,
      3, 0x00000000,
      4, 0x00000000,
      5, 0x00000000,
      6, 0x00000000,
      7, 0x00000800,
      8, 0x00000000,
      9, 0x00000000,
     10, 0x00000000,
     11, 0x00000008,
     12, 0x00000000,
     13, 0x00000000,
     14, 0x01000000,
     15, 0x000000a0,
     16, 0x00000190,
     17, 0x00000000,
     18, 0x02000000,
     19, 0x00081600,
     20, 0x00000000,
     21, 0x00081600,
     22, 0x00000000,
     23, 0x05040400,
     24, 0x1c270006,
     25, 0x200b0607,
     26, 0x27000605,
     27, 0x0b06061c,
     28, 0x04060620,
     29, 0x00db600c,
     30, 0x060c0504,
     31, 0x000c0406,
     32, 0x0400db60,
     33, 0x0b000c05,
     34, 0x000c0b0c,
     35, 0x010c0078,
     36, 0x0a140a0a,
     37, 0x0100000a,
     38, 0x17170101,
     39, 0x000b0b03,
     40, 0x00010100,
     41, 0x00000000,
     42, 0x00000000,
     43, 0x01180100,
     44, 0x00001858,
     45, 0x00000118,
     46, 0x00001858,
     47, 0x00000005,
     48, 0x00050005,
     49, 0x00140014,
     50, 0x00000000,
     51, 0x00000000,
     52, 0x02000000,
     53, 0x02000120,
     54, 0x00000120,
     55, 0x08000001,
     56, 0x00080808,
     57, 0x00000000,
     58, 0x00000000,
     59, 0x00010000,
     60, 0x10040010,
     61, 0x00000400,
     62, 0x00000200,
     63, 0x00000000,
     64, 0x00000001,
     65, 0x00000000,
     66, 0x00000000,
     67, 0x00000000,
     68, 0x00000000,
     69, 0x00000000,
     70, 0x00000000,
     71, 0x00000000,
     72, 0x00000000,
     73, 0x00000000,
     74, 0x00000000,
     75, 0x00000000,
     76, 0x00000000,
     77, 0x00000000,
     78, 0x00000000,
     79, 0x00000000,
     80, 0x00000000,
     81, 0x00000000,
     82, 0x00000000,
     83, 0x00000000,
     84, 0x00000c70,
     85, 0x00000046,
     86, 0x00000018,
     87, 0x00000c70,
     88, 0x00000046,
     89, 0x00000018,
     90, 0x00000000,
     92, 0x00000000,
     93, 0x00000000,
     94, 0x00000000,
     95, 0x00000000,
     96, 0x00000000,
     97, 0x00000000,
     98, 0x00000c70,
     99, 0x00000046,
    100, 0x00000018,
    101, 0x00000c70,
    102, 0x00000046,
    103, 0x00000018,
    104, 0x00000000,
    106, 0x00000000,
    107, 0x00000000,
    108, 0x00000000,
    109, 0x00000000,
    110, 0x00000000,
    111, 0x00000000,
    139, 0x00000000,
    140, 0x01000000,
    141, 0x00000001,
    142, 0x00000000,
    143, 0x00000000,
    144, 0x00000000,
    145, 0x00000000,
    146, 0x00000000,
    147, 0x00000000,
    148, 0x00000000,
    149, 0x00000000,
    150, 0x00000000,
    152, 0x00000000,
    153, 0x00000000,
    155, 0x00000000,
    156, 0x00000000,
    157, 0x01000200,
    158, 0x02000040,
    159, 0x00400100,
    160, 0x00000200,
    161, 0x00000040,
    162, 0x00000000,
    163, 0x01000000,
    164, 0x0a020101,
    165, 0x0101ffff,
    166, 0x01010101,
    167, 0x01010101,
    168, 0x00000103,
    169, 0x00000c03,
    170, 0x00000001, /* 0 if DDR32 */
    171, 0x00000000,
    172, 0x01000000,
    173, 0x00000100,
    174, 0x00000000,
    175, 0x00000000,
    176, 0x00000000,
    177, 0x00000000,
    178, 0x00000000,
    179, 0x00000000,
    180, 0x00000000,
    181, 0x00000000,
    182, 0x00000000,
    183, 0x00000000,
    184, 0x00000000,
    185, 0x00000000,
    186, 0x00000000,
    187, 0x00000000,
    188, 0x00000000,
    189, 0x00000000,
    190, 0x00000000,
    191, 0x00000000,
    192, 0x00000000,
    193, 0x00000000,
    194, 0x00000000,
    195, 0x00000000,
    196, 0x01010200,
    197, 0x00000002,
    198, 0x06070700,
    199, 0x00000106,
    200, 0x03030000,
    201, 0x02020002,
    202, 0x02020101,
    203, 0x02000102,
    204, 0x00000000,
    205, 0x00000000,
    206, 0x00000000,
    207, 0x00000000,
    208, 0x00000000,
    209, 0x0000280d,
    210, 0x00010000,
    211, 0x00010000,
    212, 0x00000003,
    213, 0x00000000,
    214, 0x00000000,
    215, 0x00000000,
    216, 0x00000000,
    217, 0x00000000,
    218, 0x00000000,
    219, 0x00000000,
    220, 0x00000000,
    221, 0x01000000,
    222, 0x00000001,
    223, 0x00000100,
    224, 0x00030300,
    225, 0x0556AA00,
    226, 0x000aa955,
    227, 0x000aa955,
    228, 0x000556aa,
    229, 0x000556aa,
    230, 0x000aa955,
    231, 0x000aa955,
    232, 0x030556aa,
    233, 0x01000001,
    234, 0x00010300,
    235, 0x00676700,
    236, 0x00676700,
    237, 0x00676700,
    238, 0x00676700,
    239, 0x00676700,
    240, 0x00676700,
    241, 0x00676700,
    242, 0x00676700,
    243, 0x00676700,
    245, 0x01000000,
    246, 0x00000001,
    247, 0x00000101,
    248, 0x00010100,
    249, 0x03000000,
    250, 0x03030303,
    251, 0x00030303,
    252, 0x02020064,
    253, 0x02020202,
    254, 0x00010202,
    255, 0x01010064,
    256, 0x01010101,
    257, 0x00020101,
    258, 0x00000064,
    259, 0x00000000,
    260, 0x000e0e00,
    261, 0x00000000,
    262, 0x000030b0,
    263, 0x02000200,
    264, 0x02000200,
    265, 0x000030b0,
    266, 0x0000f370,
    267, 0x0000080a,
    268, 0x000030b0,
    269, 0x02000200,
    270, 0x02000200,
    271, 0x000030b0,
    272, 0x0000f370,
    273, 0x0202080a,
    274, 0x000a0100,
    275, 0x0000000a,
    276, 0x00000000,
    277, 0x00000000,
    278, 0x00000f0a,
    279, 0x00000000,
    280, 0x00000000,
    281, 0x00000000,
    282, 0x00000000,
    283, 0x00000000,
    284, 0x00000103,
    285, 0x00010003,
    286, 0x00000003,
    287, 0x00000000,
    288, 0x00000000,
    289, 0x00000000,
    290, 0x0a000000,
    291, 0x00070a07,
    292, 0x00000000,
    293, 0x00000000,
    294, 0x00000000,
    295, 0x00000000,
    296, 0x00000000,
    297, 0x00000000,
    298, 0x00000000,
    299, 0x00000000,
    300, 0x00000000,
    301, 0x00000000,
    302, 0x00000000,
    303, 0x00000000,
    304, 0x00000000,
    0xffffffff
};
#endif /* BCM_HURRICANE3_SUPPORT */


#if defined(BCM_GREYHOUND_SUPPORT) || defined(BCM_HURRICANE3_SUPPORT)
STATIC uint32
_get_random28(void)
{
    static uint32 m_w = 6483;       /* must not be zero */
    static uint32 m_z = 31245;      /* must not be zero */

    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;       /* 32-bit result */
}
#endif /* BCM_GREYHOUND_SUPPORT || BCM_HURRICANE3_SUPPORT */


/*
 * Function:
 *      soc_iproc_ddr_reg_table_init
 * Purpose:
 *      Initialize DDR registers based on the supplied table
 * Parameters:
 *      unit - unit number
 * Returns:
 *      SOC_E_XXX
 */
STATIC int
soc_iproc_ddr_reg_table_init(int unit, int start, const unsigned int *tblptr)
{
    int rv;
    uint32 base;
    unsigned int offset;

    if (tblptr == NULL) {
        return SOC_E_PARAM;
    }

    /* Get register base address */
    base = soc_reg_addr(unit, start, REG_PORT_ANY, 0);

    offset = *tblptr;
    while(offset != 0xffffffff) {
        tblptr++;
        rv = soc_iproc_setreg(unit, base + offset * 4, *tblptr);
        if (rv != SOC_E_NONE) {
            return rv;
        }
        tblptr++;
        offset = *tblptr;
    }

    return SOC_E_NONE;
}

#ifdef BCM_HURRICANE2_SUPPORT
/*
 * Function:
 *      soc_iproc_hr2_ddr_init
 * Purpose:
 *      Initialize DDR of the embedded iProc subsystem for Hurricane2
 * Parameters:
 *      unit - unit number
 * Returns:
 *      SOC_E_XXX
 */
STATIC int
soc_iproc_hr2_ddr_init(int unit)
{
    soc_timeout_t to;
    sal_usecs_t to_val;
    uint32 rval;

    /* Skip if DDR has been already initialized */
    SOC_IF_ERROR_RETURN(READ_DDR_DENALI_CTL_00r(unit, &rval));
    if (soc_reg_field_get(unit, DDR_DENALI_CTL_00r, rval, STARTf) == 1) {
        return SOC_E_NONE;
    }

    /* Set up default DDR configuration */
    SOC_DDR3_CLOCK_MHZ(unit) = DDR_FREQ_400;
    SOC_DDR3_MEM_GRADE(unit) = 0; /* to use default */

    /* Bring DDR controller out of reset */
    SOC_IF_ERROR_RETURN(soc_reg_field32_modify(unit,
                            DDR_S1_IDM_RESET_CONTROLr, REG_PORT_ANY,
                            RESETf, 0));
    SOC_IF_ERROR_RETURN(soc_reg_field32_modify(unit,
                            DDR_S2_IDM_RESET_CONTROLr, REG_PORT_ANY,
                            RESETf, 0));

    /* Set default speed */
    SOC_IF_ERROR_RETURN(READ_DDR_S1_IDM_IO_CONTROL_DIRECTr(unit, &rval));
    soc_reg_field_set(unit, DDR_S1_IDM_IO_CONTROL_DIRECTr, &rval,
                            I_PHY_DDR_MHZf, SOC_DDR3_CLOCK_MHZ(unit));
    SOC_IF_ERROR_RETURN(WRITE_DDR_S1_IDM_IO_CONTROL_DIRECTr(unit, rval));

    /* Wait for PHY ready */
    to_val = 50000;     /* 50 mS */
    soc_timeout_init(&to, to_val, 0);
    for(;;) {
        SOC_IF_ERROR_RETURN(
            READ_DDR_PHY_CONTROL_REGS_REVISIONr(unit, &rval));
        if (rval != 0) {
            break;
        }
        if (soc_timeout_check(&to)) {
            LOG_ERROR(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                        "Timed out waiting for PHY to be ready\n")));

            return SOC_E_TIMEOUT;
        }
    }

    /* Set Strap and parameter per speed and grade */
    SOC_IF_ERROR_RETURN(
        READ_DDR_PHY_CONTROL_REGS_STRAP_CONTROLr(unit, &rval));
    soc_reg_field_set(unit, DDR_PHY_CONTROL_REGS_STRAP_CONTROLr, &rval,
                            MHZf, SOC_DDR3_CLOCK_MHZ(unit));
    soc_reg_field_set(unit, DDR_PHY_CONTROL_REGS_STRAP_CONTROLr, &rval,
                            AD_WIDTHf, 3);
    soc_reg_field_set(unit, DDR_PHY_CONTROL_REGS_STRAP_CONTROLr, &rval,
                            BUS16f, 1);
    soc_reg_field_set(unit, DDR_PHY_CONTROL_REGS_STRAP_CONTROLr, &rval,
                            CHIP_WIDTHf, 1);
    soc_reg_field_set(unit, DDR_PHY_CONTROL_REGS_STRAP_CONTROLr, &rval,
                            CHIP_SIZEf, 3);
    soc_reg_field_set(unit, DDR_PHY_CONTROL_REGS_STRAP_CONTROLr, &rval,
                            JEDEC_TYPEf, 25);
    soc_reg_field_set(unit, DDR_PHY_CONTROL_REGS_STRAP_CONTROLr, &rval,
                            STRAPS_VALIDf, 1);
    SOC_IF_ERROR_RETURN(
        WRITE_DDR_PHY_CONTROL_REGS_STRAP_CONTROLr(unit, rval));
    SOC_IF_ERROR_RETURN(
        READ_DDR_PHY_CONTROL_REGS_STRAP_CONTROL2r(unit, &rval));
    soc_reg_field_set(unit, DDR_PHY_CONTROL_REGS_STRAP_CONTROL2r, &rval,
                            DDR3f, 1);
    soc_reg_field_set(unit, DDR_PHY_CONTROL_REGS_STRAP_CONTROL2r, &rval,
                            ALf, 3);
    soc_reg_field_set(unit, DDR_PHY_CONTROL_REGS_STRAP_CONTROL2r, &rval,
                            CWLf, 9);
    soc_reg_field_set(unit, DDR_PHY_CONTROL_REGS_STRAP_CONTROL2r, &rval,
                            CLf, 13);
    SOC_IF_ERROR_RETURN(
        WRITE_DDR_PHY_CONTROL_REGS_STRAP_CONTROL2r(unit, rval));

    /* Perform 40nm DDR PHY calibration */
    soc_ddr40_set_shmoo_dram_config(unit, 1);
    soc_ddr40_phy_calibrate(unit, 0, DDR_PHYTYPE_HR2, 0);

    /* DDR controller initialization */
    soc_iproc_ddr_reg_table_init(
        unit, DDR_DENALI_CTL_00r, hr2_ddr_init_tab);
    soc_iproc_ddr_reg_table_init(
        unit, DDR_DENALI_CTL_00r, hr2_ddr_init_tab_800);

    /* Start DDR controller */
    SOC_IF_ERROR_RETURN(READ_DDR_DENALI_CTL_00r(unit, &rval));
    soc_reg_field_set(unit, DDR_DENALI_CTL_00r, &rval, STARTf, 1);
    SOC_IF_ERROR_RETURN(WRITE_DDR_DENALI_CTL_00r(unit, rval));

    /* Wait for DDR ready */
    soc_timeout_init(&to, to_val, 0);
    for(;;) {
        SOC_IF_ERROR_RETURN(READ_DDR_DENALI_CTL_89r(unit, &rval));
        if (soc_reg_field_get(unit, DDR_DENALI_CTL_89r, rval, INT_STATUSf)
            & 0x100) {
            break;
        }
        if (soc_timeout_check(&to)) {
            LOG_ERROR(BSL_LS_SOC_COMMON,
                (BSL_META_U(unit,
                  "Timed out waiting for DDR controller to be ready\n")));

            return SOC_E_TIMEOUT;
        }
    }

    /* Connect DDR port to BIST for running SHMOO */
    rval = 0;
    soc_reg_field_set(unit, DDR_BISTCONFIGr, &rval,
                            BUS16_MODEf, 1);
    soc_reg_field_set(unit, DDR_BISTCONFIGr, &rval,
                            ENABLE_8_BANKS_MODEf, 1);
    soc_reg_field_set(unit, DDR_BISTCONFIGr, &rval,
                            AXI_PORT_SELf, 1);
    soc_reg_field_set(unit, DDR_BISTCONFIGr, &rval,
                            BIST_RESETBf, 1);
    WRITE_DDR_BISTCONFIGr(unit, rval);
    rval = 0;

    /* Run or restore SHMOO */
    if (soc_property_get(unit, spn_DDR3_AUTO_TUNE, TRUE)) {
        soc_ddr40_shmoo_ctl(unit, 0, DDR_PHYTYPE_HR2, DDR_CTLR_T1, 0, 1);
        LOG_INFO(BSL_LS_SOC_COMMON,
            (BSL_META_U(unit, "DDR tuning completed.\n")));

        soc_ddr40_shmoo_savecfg(unit, 0);
        if (soc_mem_config_set != NULL) {
            soc_mem_config_set("ddr3_auto_tune","0");
        }
        LOG_INFO(BSL_LS_SOC_COMMON,
            (BSL_META_U(unit,
                "Please save the config to avoid re-tuning.\n")));
    } else {
        soc_ddr40_shmoo_restorecfg(unit, 0);
    }

    /* Connect DDR port back to NIC */
    READ_DDR_BISTCONFIGr(0, &rval);
    soc_reg_field_set(unit, DDR_BISTCONFIGr, &rval, AXI_PORT_SELf, 0);
    WRITE_DDR_BISTCONFIGr(0, rval);

    return SOC_E_NONE;
}
#endif /* BCM_HURRICANE2_SUPPORT */


#ifdef BCM_GREYHOUND_SUPPORT

STATIC int
soc_and28_gh_shmoo_phy_reg_read(int unit, int ci,
                    uint32 reg_addr, uint32 *reg_data)
{
    uint32 base;
    uint32 rval;

    if (reg_data == NULL) {
        return SOC_E_PARAM;
    }

    /* Get DDR PHY register base address */
    base = soc_reg_addr(unit, DDR_PHY_CONTROL_REGS_REVISIONr, REG_PORT_ANY, 0);
    rval = soc_iproc_getreg(unit, base + reg_addr, reg_data);

    return rval;
}

STATIC int
soc_and28_gh_shmoo_phy_reg_write(int unit, int ci,
                        uint32 reg_addr, uint32 reg_data)
{
    int rv;
    uint32 base;
    uint32 dummy;

    /* Get DDR PHY register base address */
    base = soc_reg_addr(unit, DDR_PHY_CONTROL_REGS_REVISIONr, REG_PORT_ANY, 0);
    rv = soc_iproc_setreg(unit, base + reg_addr, reg_data);

    /* Need to read a PHY register after PHY write */
    if (rv == SOC_E_NONE) {
        (void)soc_iproc_getreg(unit, base, &dummy);
    }

    return rv;
}

STATIC int
soc_and28_gh_shmoo_phy_reg_modify(int unit, int ci,
                        uint32 reg_addr, uint32 data, uint32 mask)
{
    int rv = SOC_E_NONE;
    uint32 tmp, otmp;

    data = data & mask;

    SOC_IF_ERROR_RETURN(soc_and28_gh_shmoo_phy_reg_read(
                                    unit, ci, reg_addr, &tmp));
    otmp = tmp;
    tmp &= ~(mask);
    tmp |= data;

    if (otmp != tmp) {
        SOC_IF_ERROR_RETURN(soc_and28_gh_shmoo_phy_reg_write(
                                    unit, ci, reg_addr, tmp));
    }

    return rv;
}

STATIC int
soc_and28_gh_shmoo_drc_bist_conf_set(int unit, int phy_ndx,
                                and28_bist_info_t bist_info)
{
    uint32 rval;
    uint32 poll_count;

    SOC_IF_ERROR_RETURN(READ_DDR_BISTCONFIGURATIONSr(unit, &rval));
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr, &rval, WRITEWEIGHTf, bist_info.write_weight);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr, &rval, READWEIGHTf, bist_info.read_weight);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr, &rval, PATTERNBITMODEf, 0);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr, &rval, PRBSMODEf, bist_info.prbs_mode);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr, &rval, CONSADDR8BANKSf, 1);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr, &rval, ADDRESSSHIFTMODEf, 0);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr, &rval, DATASHIFTMODEf, 0);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr, &rval, DATAADDRMODEf, 0);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr, &rval, INDWRRDADDRMODEf, 1);
    SOC_IF_ERROR_RETURN(WRITE_DDR_BISTCONFIGURATIONSr(unit, rval));

    if(!(bist_info.prbs_mode))
    {
        if(bist_info.mpr_mode)
        {
            if(1 /* YDC_DDR_BIST_PHY_BITWITDH_IS_32 */)
            {
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD0r(unit, 0xFFFFFFFF));
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD1r(unit, 0x00000000));
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD2r(unit, 0xFFFFFFFF));
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD3r(unit, 0x00000000));
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD4r(unit, 0xFFFFFFFF));
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD5r(unit, 0x00000000));
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD6r(unit, 0xFFFFFFFF));
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD7r(unit, 0x00000000));
            }
            else
            {
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD0r(unit, 0xFFFF0000));
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD1r(unit, 0xFFFF0000));
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD2r(unit, 0xFFFF0000));
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD3r(unit, 0xFFFF0000));
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD4r(unit, 0xFFFF0000));
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD5r(unit, 0xFFFF0000));
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD6r(unit, 0xFFFF0000));
                SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD7r(unit, 0xFFFF0000));
            }
        }
        else
        {
            SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD0r(unit, _get_random28()));
            SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD1r(unit, _get_random28()));
            SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD2r(unit, _get_random28()));
            SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD3r(unit, _get_random28()));
            SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD4r(unit, _get_random28()));
            SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD5r(unit, _get_random28()));
            SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD6r(unit, _get_random28()));
            SOC_IF_ERROR_RETURN(WRITE_DDR_BISTPATTERNWORD7r(unit, _get_random28()));
        }
    }

    SOC_IF_ERROR_RETURN(READ_DDR_BISTNUMBEROFACTIONSr(unit, &rval));
    soc_reg_field_set(unit, DDR_BISTNUMBEROFACTIONSr, &rval,
                            BISTNUMACTIONSf, bist_info.bist_num_actions);
    SOC_IF_ERROR_RETURN(WRITE_DDR_BISTNUMBEROFACTIONSr(unit, rval));

    SOC_IF_ERROR_RETURN(READ_DDR_BISTSTARTADDRESSr(unit, &rval));
    soc_reg_field_set(unit, DDR_BISTSTARTADDRESSr, &rval,
                            BISTSTARTADDRESSf, bist_info.bist_start_address);
    SOC_IF_ERROR_RETURN(WRITE_DDR_BISTSTARTADDRESSr(unit, rval));

    SOC_IF_ERROR_RETURN(READ_DDR_BISTENDADDRESSr(unit, &rval));
    soc_reg_field_set(unit, DDR_BISTENDADDRESSr, &rval,
                            BISTENDADDRESSf, bist_info.bist_end_address);
    SOC_IF_ERROR_RETURN(WRITE_DDR_BISTENDADDRESSr(unit, rval));

    /* Start BIST */
    SOC_IF_ERROR_RETURN(READ_DDR_BISTCONFIGURATIONSr(unit, &rval));
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr, &rval, BISTENf, 1);
    SOC_IF_ERROR_RETURN(WRITE_DDR_BISTCONFIGURATIONSr(unit, rval));

    poll_count = 0;

    while(TRUE)
    {
        SOC_IF_ERROR_RETURN(READ_DDR_BISTSTATUSESr(unit, &rval));

        if(soc_reg_field_get(unit, DDR_BISTSTATUSESr, rval, BISTFINISHEDf))
        {
            SOC_IF_ERROR_RETURN(READ_DDR_BISTCONFIGURATIONSr(unit, &rval));
            soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr, &rval, BISTENf, 0);
            SOC_IF_ERROR_RETURN(WRITE_DDR_BISTCONFIGURATIONSr(unit, rval));
            break;
        }

        if(poll_count > 100000)
        {
            SOC_IF_ERROR_RETURN(READ_DDR_BISTCONFIGURATIONSr(unit, &rval));
            soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr, &rval, BISTENf, 0);
            SOC_IF_ERROR_RETURN(WRITE_DDR_BISTCONFIGURATIONSr(unit, rval));

            LOG_ERROR(BSL_LS_SOC_COMMON, (BSL_META_U(unit, "ERROR: YDC DDR BIST timeout!!!\n")));
            return SOC_E_TIMEOUT;
        }

        poll_count++;
        sal_usleep(10);
    }

    return SOC_E_NONE;
}

STATIC int
soc_and28_gh_shmoo_drc_bist_err_cnt(int unit, int phy_ndx,
                                and28_bist_err_cnt_t *error_count)
{
    uint32 rval;

    SOC_IF_ERROR_RETURN(READ_DDR_BISTERROROCCURREDr(unit, &rval));
    error_count->bist_err_occur = soc_reg_field_get(unit,
                                    DDR_BISTERROROCCURREDr, rval, ERROCCURREDf);

    SOC_IF_ERROR_RETURN(READ_DDR_BISTFULLMASKERRORCOUNTERr(unit, &rval));
    error_count->bist_full_err_cnt = soc_reg_field_get(unit,
                                    DDR_BISTFULLMASKERRORCOUNTERr, rval, FULLERRCNTf);

    SOC_IF_ERROR_RETURN(READ_DDR_BISTSINGLEBITMASKERRORCOUNTERr(unit, &rval));
    error_count->bist_single_err_cnt = soc_reg_field_get(unit,
                                    DDR_BISTSINGLEBITMASKERRORCOUNTERr, rval, SINGLEERRCNTf);

    SOC_IF_ERROR_RETURN(READ_DDR_BISTGLOBALERRORCOUNTERr(unit, &rval));
    error_count->bist_global_err_cnt = soc_reg_field_get(unit,
                                    DDR_BISTGLOBALERRORCOUNTERr, rval, GLOBALERRCNTf);

    return SOC_E_NONE;
}


int soc_and28_gh_shmoo_dram_init(int unit, int ci, int phase)
{
    return SOC_E_NONE;
}

/* Fill shmoo CB structure */
static and28_shmoo_cbi_t gh_shmoo_cbi = {
    soc_and28_gh_shmoo_phy_reg_read,
    soc_and28_gh_shmoo_phy_reg_write,
    soc_and28_gh_shmoo_phy_reg_modify,
    soc_and28_gh_shmoo_drc_bist_conf_set,
    soc_and28_gh_shmoo_drc_bist_err_cnt,
    soc_and28_gh_shmoo_dram_init
};


#define CONFIG_DDR_GH_ROW_BITS                 15
#define CONFIG_DDR_GH_COL_BITS                 10
#define CONFIG_DDR_GH_BANK_BITS                3

/*
 * Function:
 *      soc_iproc_gh_ddr_init
 * Purpose:
 *      Initialize DDR of the embedded iProc subsystem for Greyhound
 * Parameters:
 *      unit - unit number
 * Returns:
 *      SOC_E_XXX
 */
STATIC int
soc_iproc_gh_ddr_init(int unit)
{
    soc_timeout_t to;
    sal_usecs_t to_val = 50000; /* 50ms */
    uint32 rval;
    uint32 idx;
    uint16 dev_id;
    uint8 rev_id;
    and28_shmoo_dram_info_t sdi;
    and28_shmoo_config_param_t config_param;
    uint32 genpll_clk_tbl[] =
    {
        /*  mode,   ndiv,   MDIV
                        0   1   2   3   4   5 */
        0,      96,     12, 0,  80, 6,  5,  40,        /* 400 MHz AXI */
        1,      96,     12, 0,  80, 12, 5,  40,        /* 200 MHz AXI */
        0xffffffff
    };

    /* Skip if DDR has been already initialized */
    SOC_IF_ERROR_RETURN(READ_DDR_DENALI_CTL_00r(unit, &rval));
    if (soc_reg_field_get(unit, DDR_DENALI_CTL_00r, rval, STARTf) == 1) {
        return SOC_E_NONE;
    }

    /* Shmoo callback function registration */
    soc_and28_shmoo_interface_cb_register(unit, gh_shmoo_cbi);

    soc_cm_get_id(unit, &dev_id, &rev_id);
    /* Set up default DDR configuration */
    if ((dev_id & 0xfff0) == 0xb060) {
        /* Ranger2 - 5606x: CPU 1250MHz, AXI 400MHz, DDR 800MHz */
        soc_iproc_config_genpll(unit, &genpll_clk_tbl[0]);;

        SOC_DDR3_CLOCK_MHZ(unit) = DDR_FREQ_800;
        SOC_DDR3_MEM_GRADE(unit) = 0; /* To use default */
    } else {
        /* Greyhound - 5341x: CPU 600MHz, AXI 200MHz, DDR 667MHz */
        soc_iproc_config_genpll(unit, &genpll_clk_tbl[8]);

        SOC_DDR3_CLOCK_MHZ(unit) = DDR_FREQ_667;
        SOC_DDR3_MEM_GRADE(unit) = 0; /* to use default */
    }

    /* Initialize ATCM and BTCM for SHMOO (clear to avoid ECC error) */
    for (idx = 0; idx < 64 * 1024; idx = (idx + 4)) {
        soc_iproc_setreg(unit, (0x01000000 + idx), 0);
    }
    for (idx = 0; idx < 128 * 1024; idx = (idx + 4)) {
        soc_iproc_setreg(unit, (0x01080000 + idx), 0);
    }

    SOC_IF_ERROR_RETURN(READ_CRU_DDRPHY_PWR_CTRLr(unit, &rval));

    /* Assert power ON */
    soc_reg_field_set(unit, CRU_DDRPHY_PWR_CTRLr, &rval, I_PWRONIN_PHYf, 1);
    SOC_IF_ERROR_RETURN(WRITE_CRU_DDRPHY_PWR_CTRLr(unit, rval));

    /* Assert power OK */
    sal_usleep(10);
    soc_reg_field_set(unit, CRU_DDRPHY_PWR_CTRLr, &rval, I_PWROKIN_PHYf, 1);
    SOC_IF_ERROR_RETURN(WRITE_CRU_DDRPHY_PWR_CTRLr(unit, rval));

    /* Remove DFI isolation */
    sal_usleep(150);
    soc_reg_field_set(unit, CRU_DDRPHY_PWR_CTRLr, &rval, I_ISO_PHY_DFIf, 0);
    SOC_IF_ERROR_RETURN(WRITE_CRU_DDRPHY_PWR_CTRLr(unit, rval));

    /* Remove PHY register isolation */
    soc_reg_field_set(unit, CRU_DDRPHY_PWR_CTRLr, &rval, I_ISO_PHY_REGSf, 0);
    SOC_IF_ERROR_RETURN(WRITE_CRU_DDRPHY_PWR_CTRLr(unit, rval));

    /* Remove PLL isolation */
    soc_reg_field_set(unit, CRU_DDRPHY_PWR_CTRLr, &rval, I_ISO_PHY_PLLf, 0);
    SOC_IF_ERROR_RETURN(WRITE_CRU_DDRPHY_PWR_CTRLr(unit, rval));

    /* Wait for PHY power up */
    soc_timeout_init(&to, to_val, 0);
    for(;;) {
        SOC_IF_ERROR_RETURN(READ_DDR_S1_IDM_IO_STATUSr(unit, &rval));
        if (soc_reg_field_get(unit, DDR_S1_IDM_IO_STATUSr,
                                    rval, O_PHY_PWRUP_RSBf)) {
            break;
        }
        if (soc_timeout_check(&to)) {
            LOG_ERROR(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                    "Timed out waiting for PHY to power up\n")));

            return SOC_E_TIMEOUT;
        }
    }

    /* Bring DDR controller out of reset */
    SOC_IF_ERROR_RETURN(soc_reg_field32_modify(unit,
                        DDR_S1_IDM_RESET_CONTROLr, REG_PORT_ANY, RESETf, 0));
    SOC_IF_ERROR_RETURN(soc_reg_field32_modify(unit,
                        DDR_S2_IDM_RESET_CONTROLr, REG_PORT_ANY, RESETf, 0));
    sal_usleep(1000);
    SOC_IF_ERROR_RETURN(soc_reg_field32_modify(unit,
                        DDR_S0_IDM_RESET_CONTROLr, REG_PORT_ANY, RESETf, 0));

    /* Set default speed */
    SOC_IF_ERROR_RETURN(READ_DDR_S1_IDM_IO_CONTROL_DIRECTr(unit, &rval));
    soc_reg_field_set(unit, DDR_S1_IDM_IO_CONTROL_DIRECTr, &rval,
                        I_PHY_DDR_MHZf, SOC_DDR3_CLOCK_MHZ(unit)  );
    SOC_IF_ERROR_RETURN(WRITE_DDR_S1_IDM_IO_CONTROL_DIRECTr(unit, rval));

    SOC_IF_ERROR_RETURN(READ_DDR_S2_IDM_IO_CONTROL_DIRECTr(unit, &rval));
    soc_reg_field_set(unit, DDR_S2_IDM_IO_CONTROL_DIRECTr, &rval,
                            MODE_32Bf, 1);
    SOC_IF_ERROR_RETURN(WRITE_DDR_S2_IDM_IO_CONTROL_DIRECTr(unit, rval));

    /* Wait for PHY ready */
    soc_timeout_init(&to, to_val, 0);
    for(;;) {
        SOC_IF_ERROR_RETURN(READ_DDR_S1_IDM_IO_STATUSr(unit, &rval));
        if (soc_reg_field_get(unit, DDR_S1_IDM_IO_STATUSr,
                                    rval, O_PHY_READYf)) {
            break;
        }
        if (soc_timeout_check(&to)) {
            LOG_ERROR(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                    "Timed out waiting for PHY to be ready\n")));

            return SOC_E_TIMEOUT;
        }
    }

    /* Get the DDR S0 out of reset */
    SOC_IF_ERROR_RETURN(soc_reg_field32_modify(unit,
                        DDR_S0_IDM_RESET_CONTROLr, REG_PORT_ANY, RESETf, 0));

    /* Wait for DDR PHY up */
    soc_timeout_init(&to, to_val, 0);
    for(;;) {
        SOC_IF_ERROR_RETURN(
            READ_DDR_PHY_CONTROL_REGS_REVISIONr(unit, &rval));
        if (rval != 0) {
            break;
        }
        if (soc_timeout_check(&to)) {
            LOG_ERROR(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                        "Timed out waiting for PHY to be ready\n")));

            return SOC_E_TIMEOUT;
        }
    }

    sal_memset(&sdi, 0, sizeof(sdi));
    LOG_INFO(BSL_LS_SOC_COMMON, (BSL_META_U(unit, "Calling soc_and28_shmoo_dram_info_set\n")));
    sdi.data_rate_mbps = (SOC_DDR3_CLOCK_MHZ(unit) == DDR_FREQ_667) ?
                                1333 : (SOC_DDR3_CLOCK_MHZ(unit) * 2);
    sdi.ref_clk_mhz = 50;
    sdi.ctl_type = SHMOO_AND28_CTL_TYPE_1;
    sdi.dram_type = SHMOO_AND28_DRAM_TYPE_DDR3;
    sdi.dram_bitmap = 0x00000001;
    sdi.interface_bitwidth = 32;
    sdi.num_columns = (1 << CONFIG_DDR_GH_COL_BITS);
    sdi.num_rows = (1 << CONFIG_DDR_GH_ROW_BITS);
    sdi.num_banks = (1 << CONFIG_DDR_GH_BANK_BITS);
    sdi.refi = 7800;
    sdi.command_parity_latency = 0;
    sdi.sim_system_mode = 0;

    SOC_IF_ERROR_RETURN(soc_and28_shmoo_dram_info_set(unit, &sdi));
    SOC_IF_ERROR_RETURN(soc_and28_shmoo_phy_init(unit, 0));

    /* DDR controller initialization */
    soc_iproc_ddr_reg_table_init(unit, DDR_DENALI_CTL_00r, gh_ddr_init_tab);

    switch(SOC_DDR3_CLOCK_MHZ(unit)) {
        case DDR_FREQ_400:
            soc_iproc_ddr_reg_table_init(
                    unit, DDR_DENALI_CTL_00r, gh_ddr3_init_tab_800);
            break;
        case DDR_FREQ_667:
            soc_iproc_ddr_reg_table_init(
                    unit, DDR_DENALI_CTL_00r, gh_ddr3_init_tab_1333);
            break;
        case DDR_FREQ_800:
            soc_iproc_ddr_reg_table_init(
                    unit, DDR_DENALI_CTL_00r, gh_ddr3_init_tab_1600);
            break;
    }

    /* Start DDR controller */
    SOC_IF_ERROR_RETURN(READ_DDR_DENALI_CTL_00r(unit, &rval));
    soc_reg_field_set(unit, DDR_DENALI_CTL_00r, &rval, STARTf, 1);
    SOC_IF_ERROR_RETURN(WRITE_DDR_DENALI_CTL_00r(unit, rval));

    /* Wait for DDR ready */
    soc_timeout_init(&to, to_val, 0);
    for(;;) {
        SOC_IF_ERROR_RETURN(READ_DDR_DENALI_CTL_175r(unit, &rval));
        if (soc_reg_field_get(unit, DDR_DENALI_CTL_175r, rval, INT_STATUSf)
            & 0x100) {
            break;
        }
        if (soc_timeout_check(&to)) {
            LOG_ERROR(BSL_LS_SOC_COMMON,
                (BSL_META_U(unit,
                  "Timed out waiting for DDR controller to be ready\n")));

            return SOC_E_TIMEOUT;
        }
    }

    /* Connect DDR port to BIST for running SHMOO */
    rval = 0x00100;
    SOC_IF_ERROR_RETURN(WRITE_DDR_DENALI_CTL_177r(unit, rval));

    rval = 0;
    soc_reg_field_set(unit, DDR_BISTCONFIGr, &rval, BUS16_MODEf, 1);
    soc_reg_field_set(unit, DDR_BISTCONFIGr, &rval, ENABLE_8_BANKS_MODEf, 1);
    soc_reg_field_set(unit, DDR_BISTCONFIGr, &rval, AXI_PORT_SELf, 1);
    soc_reg_field_set(unit, DDR_BISTCONFIGr, &rval, BIST_RESETBf, 1);
    SOC_IF_ERROR_RETURN(WRITE_DDR_BISTCONFIGr(unit, rval));

    rval = 0;
    soc_reg_field_set(unit, DDR_BISTGENERALCONFIGURATIONSr, &rval,
                                                    NUMCOLSf, 0x2);
    SOC_IF_ERROR_RETURN(WRITE_DDR_BISTGENERALCONFIGURATIONSr(unit, rval));

    /* Run or restore SHMOO */
    if (soc_property_get(unit, spn_DDR3_AUTO_TUNE, TRUE)) {
        soc_and28_shmoo_ctl(unit, 0, SHMOO_AND28_SHMOO_RSVP, SHMOO_AND28_CTL_FLAGS_PLOT_BIT,
                                SHMOO_AND28_ACTION_RUN, &config_param);
        LOG_INFO(BSL_LS_SOC_COMMON,
            (BSL_META_U(unit, "DDR tuning completed.\n")));

        if (soc_mem_config_set != NULL) {
            soc_mem_config_set("ddr3_auto_tune","0");
        }
        LOG_INFO(BSL_LS_SOC_COMMON,
            (BSL_META_U(unit, "Please save the config to avoid re-tuning.\n")));
    }

    /* Release DDR slave port to AXI */
    READ_DDR_BISTCONFIGr(0, &rval);
    soc_reg_field_set(unit, DDR_BISTCONFIGr, &rval, AXI_PORT_SELf, 0);
    WRITE_DDR_BISTCONFIGr(0, rval);

    return SOC_E_NONE;
}

#endif /* BCM_GREYHOUND_SUPPORT */

#ifdef BCM_HURRICANE3_SUPPORT

STATIC int
soc_and28_hr3_shmoo_phy_reg_read(int unit, int ci,
                    uint32 reg_addr, uint32 *reg_data)
{
    uint32 base;
    uint32 rval;

    if (reg_data == NULL) {
        return SOC_E_PARAM;
    }

    /* Get DDR PHY register base address */
    base = soc_reg_addr(unit, DDR_PHY_CONTROL_REGS_REVISIONr, REG_PORT_ANY, 0);
    rval = soc_iproc_getreg(unit, base + reg_addr, reg_data);

    return rval;
}

STATIC int
soc_and28_hr3_shmoo_phy_reg_write(int unit, int ci,
                        uint32 reg_addr, uint32 reg_data)
{
    int rv;
    uint32 base;
    uint32 dummy;

    /* Get DDR PHY register base address */
    base = soc_reg_addr(unit, DDR_PHY_CONTROL_REGS_REVISIONr, REG_PORT_ANY, 0);
    rv = soc_iproc_setreg(unit, base + reg_addr, reg_data);

    /* Need to read a PHY register after PHY write */
    if (rv == SOC_E_NONE) {
        (void)soc_iproc_getreg(unit, base, &dummy);
    }

    return rv;
}

STATIC int
soc_and28_hr3_shmoo_phy_reg_modify(int unit, int ci,
                        uint32 reg_addr, uint32 data, uint32 mask)
{
    int rv = SOC_E_NONE;
    uint32 tmp, otmp;

    data = data & mask;

    SOC_IF_ERROR_RETURN(soc_and28_hr3_shmoo_phy_reg_read(
                                    unit, ci, reg_addr, &tmp));
    otmp = tmp;
    tmp &= ~(mask);
    tmp |= data;

    if (otmp != tmp) {
        SOC_IF_ERROR_RETURN(soc_and28_hr3_shmoo_phy_reg_write(
                                    unit, ci, reg_addr, tmp));
    }

    return rv;
}

STATIC int
soc_and28_hr3_shmoo_drc_bist_conf_set(int unit, int phy_ndx,
                                and28_bist_info_t bist_info)
{
    uint32 rval;
    uint32 poll_count;

    SOC_IF_ERROR_RETURN(READ_DDR_BISTCONFIGURATIONSr(unit, &rval));
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr,
        &rval, WRITEWEIGHTf, bist_info.write_weight);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr,
        &rval, READWEIGHTf, bist_info.read_weight);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr,
        &rval, PATTERNBITMODEf, 0);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr,
        &rval, PRBSMODEf, bist_info.prbs_mode);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr,
        &rval, CONSADDR8BANKSf, 1);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr,
        &rval, ADDRESSSHIFTMODEf, 0);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr,
        &rval, DATASHIFTMODEf, 0);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr,
        &rval, DATAADDRMODEf, 0);
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr,
        &rval, INDWRRDADDRMODEf, 1);
    SOC_IF_ERROR_RETURN(WRITE_DDR_BISTCONFIGURATIONSr(unit, rval));

    if(!(bist_info.prbs_mode))
    {
        if(bist_info.mpr_mode)
        {
            if(1 /* YDC_DDR_BIST_PHY_BITWITDH_IS_32 */)
            {
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD0r(unit, 0xFFFFFFFF));
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD1r(unit, 0x00000000));
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD2r(unit, 0xFFFFFFFF));
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD3r(unit, 0x00000000));
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD4r(unit, 0xFFFFFFFF));
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD5r(unit, 0x00000000));
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD6r(unit, 0xFFFFFFFF));
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD7r(unit, 0x00000000));
            }
            else
            {
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD0r(unit, 0xFFFF0000));
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD1r(unit, 0xFFFF0000));
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD2r(unit, 0xFFFF0000));
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD3r(unit, 0xFFFF0000));
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD4r(unit, 0xFFFF0000));
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD5r(unit, 0xFFFF0000));
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD6r(unit, 0xFFFF0000));
                SOC_IF_ERROR_RETURN(
                    WRITE_DDR_BISTPATTERNWORD7r(unit, 0xFFFF0000));
            }
        }
        else
        {
            SOC_IF_ERROR_RETURN(
                WRITE_DDR_BISTPATTERNWORD0r(unit, _get_random28()));
            SOC_IF_ERROR_RETURN(
                WRITE_DDR_BISTPATTERNWORD1r(unit, _get_random28()));
            SOC_IF_ERROR_RETURN(
                WRITE_DDR_BISTPATTERNWORD2r(unit, _get_random28()));
            SOC_IF_ERROR_RETURN(
                WRITE_DDR_BISTPATTERNWORD3r(unit, _get_random28()));
            SOC_IF_ERROR_RETURN(
                WRITE_DDR_BISTPATTERNWORD4r(unit, _get_random28()));
            SOC_IF_ERROR_RETURN(
                WRITE_DDR_BISTPATTERNWORD5r(unit, _get_random28()));
            SOC_IF_ERROR_RETURN(
                WRITE_DDR_BISTPATTERNWORD6r(unit, _get_random28()));
            SOC_IF_ERROR_RETURN(
                WRITE_DDR_BISTPATTERNWORD7r(unit, _get_random28()));
        }
    }

    SOC_IF_ERROR_RETURN(READ_DDR_BISTNUMBEROFACTIONSr(unit, &rval));
    soc_reg_field_set(unit, DDR_BISTNUMBEROFACTIONSr, &rval,
                            BISTNUMACTIONSf, bist_info.bist_num_actions);
    SOC_IF_ERROR_RETURN(WRITE_DDR_BISTNUMBEROFACTIONSr(unit, rval));

    SOC_IF_ERROR_RETURN(READ_DDR_BISTSTARTADDRESSr(unit, &rval));
    soc_reg_field_set(unit, DDR_BISTSTARTADDRESSr, &rval,
                            BISTSTARTADDRESSf, bist_info.bist_start_address);
    SOC_IF_ERROR_RETURN(WRITE_DDR_BISTSTARTADDRESSr(unit, rval));

    SOC_IF_ERROR_RETURN(READ_DDR_BISTENDADDRESSr(unit, &rval));
    soc_reg_field_set(unit, DDR_BISTENDADDRESSr, &rval,
                            BISTENDADDRESSf, bist_info.bist_end_address);
    SOC_IF_ERROR_RETURN(WRITE_DDR_BISTENDADDRESSr(unit, rval));

    /* Start BIST */
    SOC_IF_ERROR_RETURN(READ_DDR_BISTCONFIGURATIONSr(unit, &rval));
    soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr, &rval, BISTENf, 1);
    SOC_IF_ERROR_RETURN(WRITE_DDR_BISTCONFIGURATIONSr(unit, rval));

    poll_count = 0;

    while(TRUE)
    {
        SOC_IF_ERROR_RETURN(READ_DDR_BISTSTATUSESr(unit, &rval));

        if(soc_reg_field_get(unit, DDR_BISTSTATUSESr, rval, BISTFINISHEDf))
        {
            SOC_IF_ERROR_RETURN(READ_DDR_BISTCONFIGURATIONSr(unit, &rval));
            soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr, &rval, BISTENf, 0);
            SOC_IF_ERROR_RETURN(WRITE_DDR_BISTCONFIGURATIONSr(unit, rval));
            break;
        }

        if(poll_count > 100000)
        {
            SOC_IF_ERROR_RETURN(READ_DDR_BISTCONFIGURATIONSr(unit, &rval));
            soc_reg_field_set(unit, DDR_BISTCONFIGURATIONSr, &rval, BISTENf, 0);
            SOC_IF_ERROR_RETURN(WRITE_DDR_BISTCONFIGURATIONSr(unit, rval));

            LOG_ERROR(BSL_LS_SOC_COMMON,
                (BSL_META_U(unit, "ERROR: YDC DDR BIST timeout!!!\n")));
            return SOC_E_TIMEOUT;
        }

        poll_count++;
        sal_usleep(10);
    }

    return SOC_E_NONE;
}

STATIC int
soc_and28_hr3_shmoo_drc_bist_err_cnt(int unit, int phy_ndx,
                                and28_bist_err_cnt_t *error_count)
{
    uint32 rval;

    SOC_IF_ERROR_RETURN(READ_DDR_BISTERROROCCURREDr(unit, &rval));
    error_count->bist_err_occur = soc_reg_field_get(
        unit, DDR_BISTERROROCCURREDr, rval, ERROCCURREDf);

    SOC_IF_ERROR_RETURN(READ_DDR_BISTFULLMASKERRORCOUNTERr(unit, &rval));
    error_count->bist_full_err_cnt = soc_reg_field_get(
        unit, DDR_BISTFULLMASKERRORCOUNTERr, rval, FULLERRCNTf);

    SOC_IF_ERROR_RETURN(READ_DDR_BISTSINGLEBITMASKERRORCOUNTERr(unit, &rval));
    error_count->bist_single_err_cnt = soc_reg_field_get(
        unit, DDR_BISTSINGLEBITMASKERRORCOUNTERr, rval, SINGLEERRCNTf);

    SOC_IF_ERROR_RETURN(READ_DDR_BISTGLOBALERRORCOUNTERr(unit, &rval));
    error_count->bist_global_err_cnt = soc_reg_field_get(
        unit, DDR_BISTGLOBALERRORCOUNTERr, rval, GLOBALERRCNTf);

    return SOC_E_NONE;
}


int soc_and28_hr3_shmoo_dram_init(int unit, int ci, int phase)
{
    return SOC_E_NONE;
}

/* Fill shmoo CB structure */
static and28_shmoo_cbi_t hr3_shmoo_cbi = {
    soc_and28_hr3_shmoo_phy_reg_read,
    soc_and28_hr3_shmoo_phy_reg_write,
    soc_and28_hr3_shmoo_phy_reg_modify,
    soc_and28_hr3_shmoo_drc_bist_conf_set,
    soc_and28_hr3_shmoo_drc_bist_err_cnt,
    soc_and28_hr3_shmoo_dram_init
};

/* Hurricane3 DDR3 */
#define CONFIG_DDR3_HR3_ROW_BITS                 16
#define CONFIG_DDR3_HR3_COL_BITS                 10
#define CONFIG_DDR3_HR3_BANK_BITS                3

/*
 * Function:
 *      soc_iproc_hr3_ddr3_init
 * Purpose:
 *      Initialize DDR of the embedded iProc subsystem for Greyhound
 * Parameters:
 *      unit - unit number
 * Returns:
 *      SOC_E_XXX
 */
STATIC int
soc_iproc_hr3_ddr3_init(int unit)
{
    soc_timeout_t to;
    sal_usecs_t to_val = 50000; /* 50ms */
    uint32 rval;
    uint16 dev_id;
    uint8 rev_id;
    and28_shmoo_dram_info_t sdi;
    and28_shmoo_config_param_t config_param;
    uint32 genpll_clk_tbl[] =
    {
        /*  mode,   ndiv,   MDIV
                        0   1   2   3   4   5 */
        0,      96,     12, 0,  80, 6,  5,  40,        /* 400 MHz AXI */
        1,      96,     12, 0,  80, 12, 5,  40,        /* 200 MHz AXI */
        0xffffffff
    };

    /* Skip if DDR has been already initialized */
    SOC_IF_ERROR_RETURN(READ_DDR_DENALI_CTL_00r(unit, &rval));
    if (soc_reg_field_get(unit, DDR_DENALI_CTL_00r, rval, STARTf) == 1) {
        return SOC_E_NONE;
    }

    /* Shmoo callback function registration */
    soc_and28_shmoo_interface_cb_register(unit, hr3_shmoo_cbi);

    soc_cm_get_id(unit, &dev_id, &rev_id);

    /* Set up default DDR configuration */
    soc_iproc_config_genpll(unit, &genpll_clk_tbl[0]);;
    SOC_DDR3_CLOCK_MHZ(unit) = DDR_FREQ_800;
    SOC_DDR3_MEM_GRADE(unit) = 0; /* To use default */

    SOC_IF_ERROR_RETURN(READ_CRU_DDRPHY_PWR_CTRLr(unit, &rval));

    /* Assert power ON */
    soc_reg_field_set(unit, CRU_DDRPHY_PWR_CTRLr, &rval, I_PWRONIN_PHYf, 1);
    SOC_IF_ERROR_RETURN(WRITE_CRU_DDRPHY_PWR_CTRLr(unit, rval));

    /* Assert power OK */
    sal_usleep(10);
    soc_reg_field_set(unit, CRU_DDRPHY_PWR_CTRLr, &rval, I_PWROKIN_PHYf, 1);
    SOC_IF_ERROR_RETURN(WRITE_CRU_DDRPHY_PWR_CTRLr(unit, rval));

    /* Remove DFI isolation */
    sal_usleep(150);
    soc_reg_field_set(unit, CRU_DDRPHY_PWR_CTRLr, &rval, I_ISO_PHY_DFIf, 0);
    SOC_IF_ERROR_RETURN(WRITE_CRU_DDRPHY_PWR_CTRLr(unit, rval));

    /* Remove PHY register isolation */
    soc_reg_field_set(unit, CRU_DDRPHY_PWR_CTRLr, &rval, I_ISO_PHY_REGSf, 0);
    SOC_IF_ERROR_RETURN(WRITE_CRU_DDRPHY_PWR_CTRLr(unit, rval));

    /* Remove PLL isolation */
    soc_reg_field_set(unit, CRU_DDRPHY_PWR_CTRLr, &rval, I_ISO_PHY_PLLf, 0);
    SOC_IF_ERROR_RETURN(WRITE_CRU_DDRPHY_PWR_CTRLr(unit, rval));

    /* Wait for PHY power up */
    soc_timeout_init(&to, to_val, 0);
    for(;;) {
        SOC_IF_ERROR_RETURN(READ_DDR_S1_IDM_IO_STATUSr(unit, &rval));
        if (soc_reg_field_get(unit, DDR_S1_IDM_IO_STATUSr,
                                    rval, O_PHY_PWRUP_RSBf)) {
            break;
        }
        if (soc_timeout_check(&to)) {
            LOG_ERROR(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                    "Timed out waiting for PHY to power up\n")));

            return SOC_E_TIMEOUT;
        }
    }

    /* Bring DDR controller out of reset */
    SOC_IF_ERROR_RETURN(soc_reg_field32_modify(unit,
                        DDR_S1_IDM_RESET_CONTROLr, REG_PORT_ANY, RESETf, 0));
    SOC_IF_ERROR_RETURN(soc_reg_field32_modify(unit,
                        DDR_S2_IDM_RESET_CONTROLr, REG_PORT_ANY, RESETf, 0));
    sal_usleep(1000);

#ifdef CONFIG_DDR32
    SOC_IF_ERROR_RETURN(READ_DDR_S2_IDM_IO_CONTROL_DIRECTr(unit, &rval));
    soc_reg_field_set(unit, DDR_S2_IDM_IO_CONTROL_DIRECTr, &rval,
                            MODE_32Bf, 1);
    SOC_IF_ERROR_RETURN(WRITE_DDR_S2_IDM_IO_CONTROL_DIRECTr(unit, rval));
#endif

    /* Wait for DDR PHY up */
    soc_timeout_init(&to, to_val, 0);
    for(;;) {
        SOC_IF_ERROR_RETURN(
            READ_DDR_PHY_CONTROL_REGS_REVISIONr(unit, &rval));
        if (rval != 0) {
            break;
        }
        if (soc_timeout_check(&to)) {
            LOG_ERROR(BSL_LS_SOC_COMMON,
                    (BSL_META_U(unit,
                        "Timed out waiting for PHY to be ready\n")));

            return SOC_E_TIMEOUT;
        }
    }

    sal_memset(&sdi, 0, sizeof(sdi));
    LOG_INFO(BSL_LS_SOC_COMMON,
        (BSL_META_U(unit, "Calling soc_and28_shmoo_dram_info_set\n")));
    sdi.data_rate_mbps = (SOC_DDR3_CLOCK_MHZ(unit) == DDR_FREQ_667) ?
                                1333 : (SOC_DDR3_CLOCK_MHZ(unit) * 2);
    sdi.ref_clk_mhz = 50;
    sdi.ctl_type = SHMOO_AND28_CTL_TYPE_1;
    sdi.dram_type = SHMOO_AND28_DRAM_TYPE_DDR3;
    sdi.dram_bitmap = 0x00000001;
#ifdef CONFIG_DDR32
    sdi.interface_bitwidth = 32;
#else
    sdi.interface_bitwidth = 16;
#endif
    sdi.num_columns = (1 << CONFIG_DDR3_HR3_COL_BITS);
    sdi.num_rows = (1 << CONFIG_DDR3_HR3_ROW_BITS);
    sdi.num_banks = (1 << CONFIG_DDR3_HR3_BANK_BITS);
    sdi.refi = 7800;
    sdi.command_parity_latency = 0;
    sdi.sim_system_mode = 0;

    SOC_IF_ERROR_RETURN(soc_and28_shmoo_dram_info_set(unit, &sdi));
    SOC_IF_ERROR_RETURN(soc_and28_shmoo_phy_init(unit, 0));

    /* DDR controller initialization */
    soc_iproc_ddr_reg_table_init(unit, DDR_DENALI_CTL_00r, gh_ddr_init_tab);

    soc_iproc_ddr_reg_table_init(
            unit, DDR_DENALI_CTL_00r, hr3_ddr3_init_tab_1600);

    /* Start DDR controller */
    SOC_IF_ERROR_RETURN(READ_DDR_DENALI_CTL_00r(unit, &rval));
    soc_reg_field_set(unit, DDR_DENALI_CTL_00r, &rval, STARTf, 1);
    SOC_IF_ERROR_RETURN(WRITE_DDR_DENALI_CTL_00r(unit, rval));

    /* Wait for DDR ready */
    soc_timeout_init(&to, to_val, 0);
    for(;;) {
        SOC_IF_ERROR_RETURN(READ_DDR_DENALI_CTL_175r(unit, &rval));
        if (soc_reg_field_get(unit, DDR_DENALI_CTL_175r, rval, INT_STATUSf)
            & 0x100) {
            break;
        }
        if (soc_timeout_check(&to)) {
            LOG_ERROR(BSL_LS_SOC_COMMON,
                (BSL_META_U(unit,
                  "Timed out waiting for DDR controller to be ready\n")));

            return SOC_E_TIMEOUT;
        }
    }

    /* Connect DDR port to BIST for running SHMOO */
    rval = 0x00100;
    SOC_IF_ERROR_RETURN(WRITE_DDR_DENALI_CTL_177r(unit, rval));

    rval = 0;
    soc_reg_field_set(unit, DDR_BISTCONFIGr, &rval, BUS16_MODEf, 1);
    soc_reg_field_set(unit, DDR_BISTCONFIGr, &rval, ENABLE_8_BANKS_MODEf, 1);
    soc_reg_field_set(unit, DDR_BISTCONFIGr, &rval, AXI_PORT_SELf, 1);
    soc_reg_field_set(unit, DDR_BISTCONFIGr, &rval, BIST_RESETBf, 1);
    SOC_IF_ERROR_RETURN(WRITE_DDR_BISTCONFIGr(unit, rval));

    rval = 0;
    soc_reg_field_set(unit, DDR_BISTGENERALCONFIGURATIONSr, &rval,
                                                    NUMCOLSf, 0x2);
    SOC_IF_ERROR_RETURN(WRITE_DDR_BISTGENERALCONFIGURATIONSr(unit, rval));

    /* Run or restore SHMOO */
    if (soc_property_get(unit, spn_DDR3_AUTO_TUNE, TRUE)) {
        soc_and28_shmoo_ctl(unit, 0, SHMOO_AND28_SHMOO_RSVP,
            SHMOO_AND28_CTL_FLAGS_PLOT_BIT, SHMOO_AND28_ACTION_RUN,
            &config_param);
        LOG_INFO(BSL_LS_SOC_COMMON,
            (BSL_META_U(unit, "DDR tuning completed.\n")));

        if (soc_mem_config_set != NULL) {
            soc_mem_config_set("ddr3_auto_tune","0");
        }
        LOG_INFO(BSL_LS_SOC_COMMON,
            (BSL_META_U(unit, "Please save the config to avoid re-tuning.\n")));
    }

    /* Release DDR slave port to AXI */
    READ_DDR_BISTCONFIGr(0, &rval);
    soc_reg_field_set(unit, DDR_BISTCONFIGr, &rval, AXI_PORT_SELf, 0);
    WRITE_DDR_BISTCONFIGr(0, rval);

    return SOC_E_NONE;
}

#endif /* BCM_HURRICANE3_SUPPORT */


/*
 * Function:
 *      soc_iproc_ddr_init
 * Purpose:
 *      Initialize DDR of the embedded iProc subsystem
 * Parameters:
 *      unit - unit number
 * Returns:
 *      SOC_E_XXX
 */
int soc_iproc_ddr_init(int unit)
{
    if (!soc_feature(unit, soc_feature_iproc_ddr)) {
        return SOC_E_UNAVAIL;
    }

#ifdef BCM_HURRICANE2_SUPPORT
    if (SOC_IS_HURRICANE2(unit)) {
        return soc_iproc_hr2_ddr_init(unit);
    }
#endif /* BCM_HURRICANE2_SUPPORT */
#ifdef BCM_GREYHOUND_SUPPORT
    if (SOC_IS_GREYHOUND(unit)) {
        return soc_iproc_gh_ddr_init(unit);
    }
#endif /* BCM_GREYHOUND_SUPPORT */
#ifdef BCM_HURRICANE3_SUPPORT
    if (SOC_IS_HURRICANE3(unit)) {
        return soc_iproc_hr3_ddr3_init(unit);
    }
#endif /* BCM_HURRICANE3_SUPPORT */

    return SOC_E_UNAVAIL;
}
#endif /* BCM_IPROC_DDR_SUPPORT */

#endif
