/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * File:        intr_cmicx.h
 * Purpose:     CMICX interrupt include file
 */
#ifndef _SOC_INTR_CMICX_H
#define _SOC_INTR_CMICX_H

#include <soc/intr.h>
#include <soc/mcm/intr_cmicx.h>
#include <soc/mcm/intr_iproc.h>

#define CMICX_INTR_REG_NUM     (8)
#define CMCx_SBUSDMA_CHy_BASE  (CMC0_SBUSDMA_CH0_DONE)
#define INTR_SBUSDMA(x, y)     (CMC0_SBUSDMA_CH0_DONE + 48*x + y)
#define INTR_SCHAN(x)          (SCHAN_CH0_OP_DONE + x)
#define INTR_SCHAN_FIFO(x)     (SCHAN_FIFO_CH0_DONE + x)
#define INTR_FIFO_DMA(x)       (FIFO_CH0_DMA_INTR + x)

/* supplement for iproc17, adding more FIFO/SBUSDMA channel */
typedef enum cmicx_intr_num_type_e {
    CMC0_CROSS_COUPLED_MEMORYDMA_CH2_DONE=168,
    CMC0_CROSS_COUPLED_MEMORYDMA_CH3_DONE=169,
    CMC0_SBUSDMA_CH4_DONE=170,
    CMC0_SBUSDMA_CH5_DONE=171,
    CMC0_SBUSDMA_CH6_DONE=172,
    CMC0_SBUSDMA_CH7_DONE=173,
    CMC1_CROSS_COUPLED_MEMORYDMA_CH2_DONE=216,
    CMC1_CROSS_COUPLED_MEMORYDMA_CH3_DONE=217,
    CMC1_SBUSDMA_CH4_DONE=218,
    CMC1_SBUSDMA_CH5_DONE=219,
    CMC1_SBUSDMA_CH6_DONE=220,
    CMC1_SBUSDMA_CH7_DONE=221,
    SCHAN_FIFO_0_CH0_DONE=241,
    SCHAN_FIFO_0_CH1_DONE=242,
    SCHAN_FIFO_1_CH0_DONE=245,
    SCHAN_FIFO_1_CH1_DONE=246,
    SCHAN_CH5_OP_DONE=247
} cmicx_intr_num_type_t;

/* supplement for iproc17, some different with iproc14/15 */
/* Sorted on interrupt bit. Don't modify */
typedef enum iproc17_intr_num_type_e 
{
    IPROC17_WDOG_INTR=0,
    IPROC17_TIM_INTR1=8,
    IPROC17_TIM_INTR2=9,
    IPROC17_TIM_INTR=10,
    IPROC17_UART_INTR=16,
    IPROC17_EVENTBUS_ECC=17,
    IPROC17_CR5_COMMTX=24,
    IPROC17_CR5_COMMRX=25,
    IPROC17_CR5_PMUIRQ=26,
    IPROC17_CR5_VALIRQ=27,
    IPROC17_CR5_VALFIQ=28,
    IPROC17_GENRES_TIM0_INTR=32,
    IPROC17_GENRES_TIM1_INTR=33,
    IPROC17_GENRES_TIM2_INTR=34,
    IPROC17_GENRES_TIM3_INTR=35,
    IPROC17_GENRES_WDOG0_INTR=36,
    IPROC17_MDIO_INTR=37,
    IPROC17_MIIM_INT_PHY_LINK_CHANGE_INTR=38,
    IPROC17_M0SSQ_SRAM_LL_128K_INTR=39,
    IPROC17_PERIPH_UART0_INTR=40,
    IPROC17_PERIPH_UART1_INTR=41,
    IPROC17_PERIPH_SMBUS0_INTR=42,
    IPROC17_PERIPH_SMBUS1_INTR=43,
    IPROC17_PERIPH_GPIO_INTR=44,
    IPROC17_PERIPH_SPI_INTR=45,
    IPROC17_FAS_PASS_INTR=46,
    IPROC17_FAS_FAIL_INTR=47,
    IPROC17_ARMDMA_IRQ=48,
    IPROC17_ARMDMA_IRQ_BIT0=48,
    IPROC17_ARMDMA_IRQ_BIT1=49,
    IPROC17_ARMDMA_IRQ_BIT2=50,
    IPROC17_ARMDMA_IRQ_BIT3=51,
    IPROC17_ARMDMA_IRQ_BIT4=52,
    IPROC17_ARMDMA_IRQ_BIT5=53,
    IPROC17_ARMDMA_IRQ_BIT6=54,
    IPROC17_ARMDMA_IRQ_BIT7=55,
    IPROC17_PCIE_INTR=56,
    IPROC17_PCIE_INTR_BIT0=56,
    IPROC17_PCIE_INTR_BIT1=57,
    IPROC17_PCIE_INTR_BIT2=58,
    IPROC17_PCIE_INTR_BIT3=59,
    IPROC17_PCIE_INTR_BIT4=60,
    IPROC17_PCIE_INTR_BIT5=61,
    IPROC17_TS_INTR=64,
    IPROC17_RTS0_CKSM0_INTR=65,
    IPROC17_RTS0_CKSM1_INTR=66,
    IPROC17_RTS1_CKSM0_INTR=67,
    IPROC17_RTS1_CKSM1_INTR=68,
    IPROC17_SW_PROG_INTR=69,
    IPROC17_QSPI_INTR=72,
    IPROC17_ICFG_PROG_INTR=73,
    IPROC17_ECCERR_INTR=74,
    IPROC17_IDMERR_INTR=75,
    IPROC17_ARMDMA_ABORT=76,
    IPROC17_RTS0_ARMDMA_ABORT=78,
    IPROC17_RTS0_ARMDMA_IRQ=79,
    IPROC17_RTS0_ARMDMA_IRQ_BIT0=79,
    IPROC17_RTS0_ARMDMA_IRQ_BIT1=80,
    IPROC17_RTS0_ARMDMA_IRQ_BIT2=81,
    IPROC17_RTS0_ARMDMA_IRQ_BIT3=82,
    IPROC17_RTS0_ARMDMA_IRQ_BIT4=83,
    IPROC17_RTS0_ARMDMA_IRQ_BIT5=84,
    IPROC17_RTS0_ARMDMA_IRQ_BIT6=85,
    IPROC17_RTS0_ARMDMA_IRQ_BIT7=86,
    IPROC17_RTS0_SRAM_LL_7_INTR=87,
    IPROC17_RTS0_SRAM_LL_6_INTR=88,
    IPROC17_RTS0_SRAM_LL_5_INTR=89,
    IPROC17_RTS0_SRAM_LL_4_INTR=90,
    IPROC17_RTS0_SRAM_LL_3_INTR=91,
    IPROC17_RTS0_SRAM_LL_2_INTR=92,
    IPROC17_RTS0_SRAM_LL_1_INTR=93,
    IPROC17_RTS0_SRAM_LL_0_INTR=94,
    IPROC17_RTS1_ARMDMA_ABORT=95,
    IPROC17_RTS1_ARMDMA_IRQ=96,
    IPROC17_RTS1_SRAM_LL_7_INTR=104,
    IPROC17_RTS1_SRAM_LL_6_INTR=105,
    IPROC17_RTS1_SRAM_LL_5_INTR=106,
    IPROC17_RTS1_SRAM_LL_4_INTR=107,
    IPROC17_RTS1_SRAM_LL_3_INTR=108,
    IPROC17_RTS1_SRAM_LL_2_INTR=109,
    IPROC17_RTS1_SRAM_LL_1_INTR=110,
    IPROC17_RTS1_SRAM_LL_0_INTR=111,
    IPROC17_CHIP_INTR_HIGH_PRIORITY_BIT0=112,
    IPROC17_CHIP_INTR_HIGH_PRIORITY=112,
    IPROC17_CHIP_INTR_HIGH_PRIORITY_BIT1=113,
    IPROC17_CHIP_INTR_HIGH_PRIORITY_BIT2=114,
    IPROC17_CHIP_INTR_HIGH_PRIORITY_BIT3=115,
    IPROC17_CHIP_INTR_HIGH_PRIORITY_BIT4=116,
    IPROC17_CHIP_INTR_HIGH_PRIORITY_BIT5=117,
    IPROC17_CHIP_INTR_HIGH_PRIORITY_BIT6=118,
    IPROC17_CHIP_INTR_HIGH_PRIORITY_BIT7=119,
    IPROC17_CHIP_INTR_HIGH_PRIORITY_BIT8=120,
    IPROC17_CHIP_INTR_HIGH_PRIORITY_BIT9=121,
    IPROC17_CHIP_INTR_HIGH_PRIORITY_BIT10=122,
    IPROC17_CHIP_INTR_HIGH_PRIORITY_BIT11=123,
    IPROC17_CHIP_INTR_HIGH_PRIORITY_BIT12=124,
    IPROC17_CHIP_INTR_HIGH_PRIORITY_BIT13=125,
    IPROC17_CHIP_INTR_HIGH_PRIORITY_BIT14=126,
    IPROC17_CHIP_INTR_LOW_PRIORITY=127,
    IPROC17_CMIC_INTR_BIT0=128,
    IPROC17_CMIC_INTR=128,
    IPROC17_CMIC_INTR_BIT1=129,
    IPROC17_CMIC_INTR_BIT2=130,
    IPROC17_CMIC_INTR_BIT3=131,
    IPROC17_CMIC_INTR_BIT4=132,
    IPROC17_CMIC_INTR_BIT5=133,
    IPROC17_CMIC_INTR_BIT6=134,
    IPROC17_CMIC_INTR_BIT7=135,
    IPROC17_CMIC_INTR_BIT8=136,
    IPROC17_CMIC_INTR_BIT9=137,
    IPROC17_CMIC_INTR_BIT10=138,
    IPROC17_CMIC_INTR_BIT11=139,
    IPROC17_CMIC_INTR_BIT12=140,
    IPROC17_CMIC_INTR_BIT13=141,
    IPROC17_CMIC_INTR_BIT14=142,
    IPROC17_CMIC_INTR_BIT15=143,
    IPROC17_CMIC_INTR_BIT16=144,
    IPROC17_CMIC_INTR_BIT17=145,
    IPROC17_CMIC_INTR_BIT18=146,
    IPROC17_CMIC_INTR_BIT19=147,
    IPROC17_CMIC_INTR_BIT20=148,
    IPROC17_CMIC_INTR_BIT21=149,
    IPROC17_CMIC_INTR_BIT22=150,
    IPROC17_CMIC_INTR_BIT23=151,
    IPROC17_CMIC_INTR_BIT24=152,
    IPROC17_CMIC_INTR_BIT25=153,
    IPROC17_CMIC_INTR_BIT26=154,
    IPROC17_CMIC_INTR_BIT27=155,
    IPROC17_CMIC_INTR_BIT28=156,
    IPROC17_CMIC_INTR_BIT29=157,
    IPROC17_CMIC_INTR_BIT30=158,
    IPROC17_CMIC_INTR_BIT31=159,
    IPROC17_CMIC_INTR_BIT32=160,
    IPROC17_CMIC_INTR_BIT33=161,
    IPROC17_CMIC_INTR_BIT34=162,
    IPROC17_CMIC_INTR_BIT35=163,
    IPROC17_CMIC_INTR_BIT36=164,
    IPROC17_CMIC_INTR_BIT37=165,
    IPROC17_CMIC_INTR_BIT38=166,
    IPROC17_CMIC_INTR_BIT39=167,
    IPROC17_CMIC_INTR_BIT40=168,
    IPROC17_CMIC_INTR_BIT41=169,
    IPROC17_CMIC_INTR_BIT42=170,
    IPROC17_CMIC_INTR_BIT43=171,
    IPROC17_CMIC_INTR_BIT44=172,
    IPROC17_CMIC_INTR_BIT45=173,
    IPROC17_CMIC_INTR_BIT46=174,
    IPROC17_CMIC_INTR_BIT47=175,
    IPROC17_CMIC_INTR_BIT48=176,
    IPROC17_CMIC_INTR_BIT49=177,
    IPROC17_CMIC_INTR_BIT50=178,
    IPROC17_CMIC_INTR_BIT51=179,
    IPROC17_CMIC_INTR_BIT52=180,
    IPROC17_CMIC_INTR_BIT53=181,
    IPROC17_CMIC_INTR_BIT54=182,
    IPROC17_CMIC_INTR_BIT55=183,
    IPROC17_CMIC_INTR_BIT56=184,
    IPROC17_CMIC_INTR_BIT57=185,
    IPROC17_CMIC_INTR_BIT58=186,
    IPROC17_CMIC_INTR_BIT59=187,
    IPROC17_CMIC_INTR_BIT60=188,
    IPROC17_CMIC_INTR_BIT61=189,
    IPROC17_CMIC_INTR_BIT62=190,
    IPROC17_CMIC_INTR_BIT63=191,
    IPROC17_CMIC_INTR_BIT64=192,
    IPROC17_CMIC_INTR_BIT65=193,
    IPROC17_CMIC_INTR_BIT66=194,
    IPROC17_CMIC_INTR_BIT67=195,
    IPROC17_CMIC_INTR_BIT68=196,
    IPROC17_CMIC_INTR_BIT69=197,
    IPROC17_CMIC_INTR_BIT70=198,
    IPROC17_CMIC_INTR_BIT71=199,
    IPROC17_CMIC_INTR_BIT72=200,
    IPROC17_CMIC_INTR_BIT73=201,
    IPROC17_CMIC_INTR_BIT74=202,
    IPROC17_CMIC_INTR_BIT75=203,
    IPROC17_CMIC_INTR_BIT76=204,
    IPROC17_CMIC_INTR_BIT77=205,
    IPROC17_CMIC_INTR_BIT78=206,
    IPROC17_CMIC_INTR_BIT79=207,
    IPROC17_CMIC_INTR_BIT80=208,
    IPROC17_CMIC_INTR_BIT81=209,
    IPROC17_CMIC_INTR_BIT82=210,
    IPROC17_CMIC_INTR_BIT83=211,
    IPROC17_CMIC_INTR_BIT84=212,
    IPROC17_CMIC_INTR_BIT85=213,
    IPROC17_CMIC_INTR_BIT86=214,
    IPROC17_CMIC_INTR_BIT87=215,
    IPROC17_CMIC_INTR_BIT88=216,
    IPROC17_CMIC_INTR_BIT89=217,
    IPROC17_CMIC_INTR_BIT90=218,
    IPROC17_CMIC_INTR_BIT91=219,
    IPROC17_CMIC_INTR_BIT92=220,
    IPROC17_CMIC_INTR_BIT93=221,
    IPROC17_CMIC_INTR_BIT94=222,
    IPROC17_CMIC_INTR_BIT95=223,
    IPROC17_CMIC_INTR_BIT96=224,
    IPROC17_CMIC_INTR_BIT97=225,
    IPROC17_CMIC_INTR_BIT98=226,
    IPROC17_CMIC_INTR_BIT99=227,
    IPROC17_CMIC_INTR_BIT100=228,
    IPROC17_CMIC_INTR_BIT101=229,
    IPROC17_CMIC_INTR_BIT102=230,
    IPROC17_CMIC_INTR_BIT103=231,
    IPROC17_CMIC_INTR_BIT104=232,
    IPROC17_CMIC_INTR_BIT105=233,
    IPROC17_CMIC_INTR_BIT106=234,
    IPROC17_CMIC_INTR_BIT107=235,
    IPROC17_CMIC_INTR_BIT108=236,
    IPROC17_CMIC_INTR_BIT109=237,
    IPROC17_CMIC_INTR_BIT110=238,
    IPROC17_CMIC_INTR_BIT111=239,
    IPROC17_CMIC_INTR_BIT112=240,
    IPROC17_CMIC_INTR_BIT113=241,
    IPROC17_CMIC_INTR_BIT114=242,
    IPROC17_CMIC_INTR_BIT115=243,
    IPROC17_CMIC_INTR_BIT116=244,
    IPROC17_CMIC_INTR_BIT117=245,
    IPROC17_CMIC_INTR_BIT118=246,
    IPROC17_CMIC_INTR_BIT119=247,
    IPROC17_CMIC_INTR_BIT120=248,
    IPROC17_CMIC_INTR_BIT121=249,
    IPROC17_CMIC_INTR_BIT122=250,
    IPROC17_CMIC_INTR_BIT123=251,
    IPROC17_CMIC_INTR_BIT124=252,
    IPROC17_CMIC_INTR_BIT125=253,
    IPROC17_CMIC_INTR_BIT126=254,
    IPROC17_CMIC_INTR_BIT127=255,
    IPROC17_IPROC_INTERRUPT_NUM_MAX
}iproc17_intr_num_type_t;



/*******************************************
* @function soc_cmicx_intr_init
* purpose initialize CMICX interrupt framework
* @param unit [out] soc_cmic_intr_op_t pointer
*
* @param unit [in] unit
*
* @returns SOC_E_NONE
* @returns SOC_E_XXX
*
* @end
 */
extern int
soc_cmicx_intr_init(int unit, soc_cmic_intr_op_t *intr_op);

/*******************************************
* @function soc_cmicx_intr
* purpose SOC CMICX Interrupt Service Routine
*
* @param unit [in] unit
*
*
* @end
 */
extern void
soc_cmicx_intr(void *_unit);

#ifdef SEPARATE_PKTDMA_INTR_HANDLER
/*******************************************
* @function soc_cmicx_pktdma_intr
* purpose SOC CMICX Interrupt Service Routine
*
* @param unit [in] unit
*
*
* @end
 */
extern void
soc_cmicx_pktdma_intr(void *_unit);
#endif

#endif  /* !_SOC_INTR_CMICX_H */
