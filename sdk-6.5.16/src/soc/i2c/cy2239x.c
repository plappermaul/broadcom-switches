/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2019 Broadcom Inc. All rights reserved.
 *
 * BCM56xx I2C Device Driver for Cypress W229393 Clock generator (PLL) chip.
 * The W229393 is a highly integrated frequency timing generator,
 * supplying a variety of common clock sources.
 *
 * This driver is meant to replace the W229/W311 clock chips found on
 * most StrataSwitch reference designs.
 *
 * The following output below is from the correct programming of the
 * CY22393x device using Cyclocks RT.
 *
 *    _____   _____________________
 *   / __\ \ / / _ \ _ \ __/ __/ __|
 *  | (__ \ ^ /|  _/   / _|\__ \__ \
 *   \___| |_| |_| |_|_\___|___/___/ TIMING TECHNOLOGY
 *  ==================================================
 *                   ______________
 *                 _|     \__/     |_  ________
 *           CLKC |_| 1         16 |_| SHUTDOWN/OE
 *                 _|              |_     _______
 *            VDD |_| 2         15 |_| S2/SUSPEND
 *                 _|              |_
 *           AGND |_| 3         14 |_| AVDD
 *                 _|              |_
 *         XTALIN |_| 4         13 |_| SCLK
 *                 _|   CY22393    |_
 *        XTALOUT |_| 5         12 |_| SDAT
 *                 _|              |_
 *           XBUF |_| 6         11 |_| GND
 *                 _|              |_
 *           CLKD |_| 7         10 |_| CLKA
 *                 _|              |_
 *           CLKE |_| 8          9 |_| CLKB
 *                  |______________|
 *
 * Generated by CyClocksRT R1.05.00
 *
 * Modification Date: 2/14/02
 * Comments: Slave Address is 0x6A which translated to 0xd4 (bit zero
 *           not used by CyClocksRT program).
 * Customer: Broadcom
 * FAE:      n/a
 * License #:n/a
 * Reference Input:   25.000000 MHz External
 * =============================================================
 * | Pll  |  S2   | Source|  Desired   |   Actual   |   PPM    |
 * =============================================================
 * | Pll3 |   -   |  Ref  | 266.000000 | 266.000000 | 0.0      |
 * | Pll2 |   -   |  Ref  | 375.000000 | 375.000000 | 0.0      |
 * | Pll1 |   0   |  Ref  | 200.000000 | 200.000000 | 0.0      |
 * | Pll1 |   1   |  Ref  | 134.000000 | 134.000000 | 0.0      |
 * =============================================================
 *
 * Clock Output
 * =========================================================================
 * |Output|  S2   |  Source  |  Desired   |   Actual   |   PPM    | Voltage|
 * =========================================================================
 * | XBUF |   -   |  Ref     |  25.000000 |  25.000000 |     +0.0 |  3.3V  |
 * | CLKE |   -   |  Pll1/4  |  50.000000 |  50.000000 |     +0.0 |  3.3V  |
 * | CLKD |   -   |  Pll2/3  | 125.000000 | 125.000000 |     +0.0 |  3.3V  |
 * | CLKC |   -   |  Pll2/3  | 125.000000 | 125.000000 |     +0.0 |  3.3V  |
 * | CLKB |   -   |  Pll3/2  | 133.000000 | 133.000000 |     +0.0 |  3.3V  |
 * | CLKA |   -   |  Pll3/2  | 133.000000 | 133.000000 |     +0.0 |  3.3V  |
 * =========================================================================
 *
 *
 * Programming Algorithm
 *
 *  For Core (Outputs AB, PLL3): NOTE: Q defaults to 25 so no adjustmnt
 *  is necessary.
 *
 * 1. Adjust Q to desired value (25).  Default so no action necessary!!!
 * 2. Adjust P to desired value 3. Adjust output divider to desired value
 * (2) - default is 3 for core and SDRAM 4. Adjust LF
 *
 * For SDRAM (Outputs CD, PLL2):
 *
 * 1. Adjust Q to desired value (25)
 * 2. Adjust P to desired value
 * 3. Adjust output divider to desired value (2) - default is 3 for core and
 *    SDRAM
 * 4. Adjust LF
 *
 * For PCI/CPU clock  (OUTPUT E, PLL1)
 *
 * 1. Adjust Q to desired value (25).  Default so no action necessary!!!
 * 2. Adjust P to desired value
 * 3. Adjust output divider to desired value (4) - Default so no action
 *    necessary!!!
 * 4. Adjust LF.
 * 5. Compensate as necessary for baud rate or other parameters affected by
 *    changing the PCI or CPU frequency!!!!!!
 *
 *
 * See the Cypress W22393 Clock chip data sheet(s) for more details.
 * See also: http://www.cypress.com/clock/datasheets.html
 */

#include <sal/types.h>
#include <soc/drv.h>
#include <soc/error.h>
#include <soc/i2c.h>
#include <shared/bsl.h>
#define MHZ(x)    ((x)*1000000)

/* FRef = 25MHz */
#define    CY2239x_REF_CLK    MHZ(25)
#define    CY2239x_DIV_CORE   2
#define    CY2239x_DIV_SDRAM  2
#define    CY2239x_DIV_PCI    4
#define CY_DEBUG(x) { \
        LOG_CLI((BSL_META(x))); sal_usleep(1000)));}

/*
 * Loop filter : Computed based on PT
 * See datasheet for more info.
 */
STATIC uint8
cy2239x_loop_filter(int val)
{
    uint8 tmp;

    /* Convert from MHz if required */
    if (val > MHZ(1)) {
	val /= MHZ(1);
    }
    if (val >= 16 && val <= 231) {
	tmp = 0;
    } else if (val >= 232 && val <= 626) {
	tmp = 1;
    } else if (val >= 627 && val <= 834) {
	tmp = 2;
    } else if (val >= 835 && val <= 1043) {
	tmp = 3;
    } else if (val >= 1044 && val <= 1600) {
	tmp = 4;
    } else {
	tmp = 0;
    }
    return tmp;
}

STATIC int
cy2239x_ioctl(int unit, int devno,
	   int opcode, void* data, int len)
{
#ifdef COMPILER_HAS_DOUBLE
    double clk, qt;
#else
    int qt;
#endif
    int iclk, P0, ip, iq;
    uint8 tmp, lf, saddr = soc_i2c_addr(unit, devno);

    if ( I2C_XPLL_SET_PCI == opcode ) {
#ifdef COMPILER_HAS_DOUBLE
	clk = *((double *)data);
	/* Convert from MHz if required */
	if (clk < MHZ(1))
	    clk *= MHZ(1);

	/* Setup new core clock */
	iclk = (int) clk;
#else
	iclk = *((int *)data);
#endif
        /* Compute actual frequency in MHz */
        iclk = (CY2239x_DIV_PCI * iclk) / MHZ(1);

        /* This is set only on an odd clock freq */
 	P0 = iclk % 2;

	/* Compute Qt and convert to byte value (in MHz)*/
	qt = CY2239x_REF_CLK - MHZ(2);
	qt = qt / MHZ(1);

	/* Compute P/Q (integer format) */
	ip = ((iclk - P0) / 2) - 3;
	iq = (int) qt;

	/* Compute loop filter */
	lf = cy2239x_loop_filter(iclk);

	/* Read DivSel/PLL_Enable register */
	soc_i2c_read_byte_data(unit, saddr, 0x57, &tmp);

        /* Mask fields to be updated */
	tmp &= ~(0x03);

	/* Add upper P bits */
	tmp |= (ip >> 8) & 3;

	/* Add PO bit */
	tmp |= (P0 << 2);

	/* Add Loop Filter bits */
	tmp |= (lf << 3);

	/* Configure new clock setting */
	soc_i2c_write_byte_data(unit, saddr, 0x55, (uint8)iq);
	soc_i2c_write_byte_data(unit, saddr, 0x56, (uint8)ip);
	soc_i2c_write_byte_data(unit, saddr, 0x57, tmp);

    } else if ( I2C_XPLL_GET_PCI == opcode ) {
	uint8 p_l, p_h, q, pNot;
	uint16 p;

	soc_i2c_read_byte_data(unit, saddr, 0x55, &q);
	soc_i2c_read_byte_data(unit, saddr, 0x56, &p_l);
	soc_i2c_read_byte_data(unit, saddr, 0x57, &p_h);

	pNot = ((p_h & 0x04) >> 2); /* Bit 2 of 0x57 */
	p_h &= 0x03; /* Clear all bits except bits 0-1 */
	p = (p_h << 8) | p_l; /* 10 bits of p value */
#ifdef COMPILER_HAS_DOUBLE
	clk =   2 * ( p + 3 )  + pNot ;
	clk *= CY2239x_REF_CLK;
	clk /= (q+2);
	clk /= CY2239x_DIV_PCI;
	clk /= MHZ(1);
	*((double *)data) = clk;
#else
	iclk = 2 * (p + 3) + pNot ;
	iclk *= CY2239x_REF_CLK / (q + 2);
	iclk /= CY2239x_DIV_PCI;
	*((int *)data) = iclk;
#endif
    } else if ( I2C_XPLL_SET_CORE == opcode ) {
#ifdef COMPILER_HAS_DOUBLE
	clk = *((double *)data);

	/* Convert from MHz if required */
	if (clk < MHZ(1))
	    clk *= MHZ(1);

	/* Setup new core clock */
	iclk = (int) clk;
#else
	iclk = *((int *)data);
#endif
        /* Compute actual frequency in MHz */
        iclk = (CY2239x_DIV_CORE * iclk) / MHZ(1);

        /* This is set only on an odd clock freq */
 	P0 = iclk % 2;

	/* Compute Qt and convert to byte value (in MHz)*/
	qt = CY2239x_REF_CLK - MHZ(2);
	qt = qt / MHZ(1);

	/* Compute P/Q (integer format) */
	ip = ((iclk - P0) / 2) - 3;
	iq = (int) qt;

	/* Compute loop filter */
	lf = cy2239x_loop_filter(iclk);

	/* Read DivSel/PLL_Enable register */
	soc_i2c_read_byte_data(unit, saddr, 0x16, &tmp);

        /* Mask fields to be updated */
	tmp &= ~(0x03);

	/* Add upper P bits */
	tmp |= (ip >> 8) & 3;

	/* Add PO bit */
	tmp |= (P0 << 2);

	/* Add Loop Filter bits */
	tmp |= (lf << 3);

	/* Configure new clock setting */
	soc_i2c_write_byte_data(unit, saddr, 0x15, (uint8)ip);
	soc_i2c_write_byte_data(unit, saddr, 0x14, (uint8)iq);
	soc_i2c_write_byte_data(unit, saddr, 0x08, 2);
	soc_i2c_write_byte_data(unit, saddr, 0x0a, 2);
	soc_i2c_write_byte_data(unit, saddr, 0x16, tmp);

    } else if ( I2C_XPLL_GET_CORE == opcode) {
	uint8 p_l, p_h, q, pNot, div;
	uint16 p;

	soc_i2c_read_byte_data(unit, saddr, 0x15, &p_l);
	soc_i2c_read_byte_data(unit, saddr, 0x16, &p_h);
	soc_i2c_read_byte_data(unit, saddr, 0x14, &q);
	soc_i2c_read_byte_data(unit, saddr, 0x08, &div);
	div &= 0x7f;
	pNot = ((p_h & 0x04) >> 2); /* Bit 2 of 0x57 */
	p_h &= 0x03; /* Clear all bits except bits 0-1 */
	p = (p_h << 8) | p_l; /* 10 bits of p value */
#ifdef COMPILER_HAS_DOUBLE
	clk =   2 * ( p + 3 )  + pNot ;
	clk *= CY2239x_REF_CLK;
	clk /= (q+2);
	clk /= div;
	clk /= MHZ(1);
	*((double *)data) = clk;
#else
	iclk = 2 * (p + 3) + pNot ;
	iclk *= CY2239x_REF_CLK / (q + 2);
	iclk /= div;
	*((int *)data) = iclk;
#endif
    } else if ( I2C_XPLL_SET_SDRAM == opcode) {

#ifdef COMPILER_HAS_DOUBLE
	clk = *((double *)data);

	/* Convert to MHz if required */
	if (clk < MHZ(1))
	    clk *= MHZ(1);

	/* Setup new core clock */
	iclk = (int) clk;
#else
	iclk = *((int *)data);
#endif
        /* Compute actual frequency in MHz */
        iclk = (CY2239x_DIV_SDRAM * iclk) / MHZ(1);

        /* This is set only on an odd clock freq */
 	P0 = iclk % 2;

	/* Compute Qt and convert to byte value (in MHz)*/
	qt = CY2239x_REF_CLK - MHZ(2);
	qt = qt / MHZ(1);

	/* Compute P/Q (integer format) */
	ip = ((iclk - P0) / 2) - 3;
	iq = (int) qt;

	/* Compute loop filter */
	lf = cy2239x_loop_filter(iclk);

	/* Read DivSel/PLL_Enable register */
	soc_i2c_read_byte_data(unit, saddr, 0x13, &tmp);

        /* Mask fields to be updated */
	tmp &= ~(0x03);

	/* Add upper P bits */
	tmp |= (ip >> 8) & 3;

	/* Add PO bit */
	tmp |= (P0 << 2);

	/* Add Loop Filter bits */
	tmp |= (lf << 3);

	/* Configure new clock setting */
	soc_i2c_write_byte_data(unit, saddr, 0x12, (uint8)ip);
	soc_i2c_write_byte_data(unit, saddr, 0x11, (uint8)iq);
	soc_i2c_write_byte_data(unit, saddr, 0x0c, 2);
	soc_i2c_write_byte_data(unit, saddr, 0x0d, 2);
	soc_i2c_write_byte_data(unit, saddr, 0x13, tmp);

    } else if ( I2C_XPLL_GET_SDRAM == opcode) {
	uint8 p_l, p_h, q, pNot, div;
	uint16 p;

	soc_i2c_read_byte_data(unit, saddr, 0x12, &p_l);
	soc_i2c_read_byte_data(unit, saddr, 0x13, &p_h);
	soc_i2c_read_byte_data(unit, saddr, 0x11, &q);
	soc_i2c_read_byte_data(unit, saddr, 0x0c, &div);
	div &= 0x7f;

	pNot = ((p_h & 0x04) >> 2); /* Bit 2 of 0x57 */
	p_h &= 0x03; /* Clear all bits except bits 0-1 */
	p = (p_h << 8) | p_l; /* 10 bits of p value */
#ifdef COMPILER_HAS_DOUBLE
	clk =   2 * ( p + 3 )  + pNot ;
	clk *= CY2239x_REF_CLK;
	clk /= (q+2);
	clk /= div;
	clk /= MHZ(1);
	*((double *)data) = clk;
#else
	iclk = 2 * (p + 3) + pNot ;
	iclk *= CY2239x_REF_CLK / (q + 2);
	iclk /= div;
	*((int *)data) = iclk;
#endif
    } else {
	return SOC_E_PARAM;
    }

    return SOC_E_NONE;
}

/*
 * Function: cy2239x_init
 *
 * Purpose:
 *    Initialize the CY2239X clock chip
 * Parameters:
 *    unit - StrataSwitch device number or I2C bus number
 *    devno - chip device id
 *    data - not used
 *    len - not used
 * Returns:
 *    SOC_E_NONE - no failure
 */
STATIC int
cy2239x_init(int unit, int devno,
             void *data, int len)
{
#if 0
    int i;
    uint8 x;
    uint8 saddr = soc_i2c_addr(unit, devno);

    /* Dump register values */
    for (i = 0x08; i <= 0x17; i++) {
        soc_i2c_read_byte_data(unit, saddr, (uint8)i, &x);
        LOG_CLI((BSL_META_U(unit,
                            "pll[%x] = 0x%x\n"), i, x));
    }
    for (i = 0x40; i <= 0x57; i++) {
        soc_i2c_read_byte_data(unit, saddr, (uint8)i, &x);
        LOG_CLI((BSL_META_U(unit,
                            "pll[%x] = 0x%x\n"), i, x));
    }
#endif
    soc_i2c_devdesc_set(unit, devno, "Cypress W2239x clock Chip");
    return SOC_E_NONE;
}

STATIC int
cy2239x_read(int unit, int devno,
	     uint16 addr, uint8 *data, uint32* len)
{
    *len = 1; /* Byte-write mode */
    return soc_i2c_read_byte_data(unit,
				  soc_i2c_addr(unit, devno),
				  (uint8)addr, data);
}

STATIC int
cy2239x_write(int unit, int devno,
              uint16 addr, uint8 *data, uint32 len)
{
    return soc_i2c_write_byte_data(unit,
				   soc_i2c_addr(unit, devno),
				   (uint8)addr, *data);
}

/* CY2239X/W311 Clock Chip Driver callout */
i2c_driver_t _soc_i2c_cy2239x_driver = {
    0x0, 0x0, /* System assigned bytes */
    CY2239X_DEVICE_TYPE,
    cy2239x_read,
    cy2239x_write,
    cy2239x_ioctl,
    cy2239x_init,
    NULL,
};
