/*
    Copyright 2001, Broadcom Corporation
    All Rights Reserved.
    
    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Broadcom Corporation.
*/
/*
 * Broadcom SiliconBackplane hardware register definitions.
 *
 * Copyright(c) 2000 Broadcom Corporation
 * $Id: sbconfig.h,v 1.3 2012/03/02 15:27:43 yaronm Exp $
 */

#ifndef	_SBCONFIG_H
#define	_SBCONFIG_H

/* cpp contortions to concatenate w/arg prescan */
#ifndef PAD
#define	_PADLINE(line)	pad ## line
#define	_XSTR(line)	_PADLINE(line)
#define	PAD		_XSTR(__LINE__)
#endif

/*
 * SiliconBackplane Address Map.
 * All regions may not exist on all chips.
 */
#define SB_SDRAM_BASE	0x00000000	/* Physical SDRAM */
#define SB_PCI_MEM	0x08000000	/* Host Mode PCI memory access space (64 MB) */
#define SB_PCI_CFG	0x0c000000	/* Host Mode PCI configuration space (64 MB) */
#define	SB_SDRAM_SWAPPED 0x10000000	/* Byteswapped Physical SDRAM */
#define SB_ENUM_BASE    0x18000000	/* Enumeration space base */
#define	SB_ENUM_LIM	0x18010000	/* Enumeration space limit */
#define	SB_EXTIF_BASE	0x1f000000	/* External Interface region base address */
#define SB_PCI_DMA	0x40000000	/* Client Mode PCI memory access space (1 GB) */
#define	SB_EUART	(SB_EXTIF_BASE + 0x00800000)
#define	SB_LED		(SB_EXTIF_BASE + 0x00900000)

/* enumeration space related defs */
#define SB_CORE_SIZE    0x1000		/* each core gets 4Kbytes for registers */
#define	SB_MAXCORES	((SB_ENUM_LIM - SB_ENUM_BASE)/SB_CORE_SIZE)
#define	SBCONFIGOFF	0xf00		/* core sbconfig regs are top 256bytes of regs */
#define	SBCONFIGSIZE	256		/* sizeof (sbconfig_t) */

/* mips address */
#define	SB_EJTAG	0xff200000	/* MIPS EJTAG space (2M) */

#ifndef _LANGUAGE_ASSEMBLY

/*
 * Sonics Configuration Space Registers.
 */
typedef volatile struct _sbconfig {
	uint32	PAD[2];
	uint32	sbipsflag;		/* initiator port ocp slave flag */
	uint32	PAD[3];
	uint32	sbtpsflag;		/* target port ocp slave flag */
	uint32	PAD[17];
	uint32	sbadmatch3;		/* address match3 */
	uint32	PAD;
	uint32	sbadmatch2;		/* address match2 */
	uint32	PAD;
	uint32	sbadmatch1;		/* address match1 */
	uint32	PAD[7];
	uint32	sbimstate;		/* initiator agent state */
	uint32	sbintvec;		/* interrupt mask */
	uint32	sbtmstatelow;		/* target state */
	uint32	sbtmstatehigh;		/* target state */
	uint32	sbbwa0;			/* bandwidth allocation table0 */
	uint32	PAD;
	uint32	sbimconfiglow;		/* initiator configuration */
	uint32	sbimconfighigh;		/* initiator configuration */
	uint32	sbadmatch0;		/* address match0 */
	uint32	PAD;
	uint32	sbtmconfiglow;		/* target configuration */
	uint32	sbtmconfighigh;		/* target configuration */
	uint32	sbbconfig;		/* broadcast configuration */
	uint32	PAD;
	uint32	sbbstate;		/* broadcast state */
	uint32	PAD[3];
	uint32	sbactcnfg;		/* activate configuration */
	uint32	PAD[3];
	uint32	sbflagst;		/* current sbflags */
	uint32	PAD[3];
	uint32	sbidlow;		/* identification */
	uint32	sbidhigh;		/* identification */
} sbconfig_t;

#endif /* _LANGUAGE_ASSEMBLY */

/* sbipsflag */
#define SBIPSFLAG	0x08		/* offset */
#define	SBIPS_INT1_MASK	0x3f		/* which sbflags get routed to mips interrupt 1 */
#define	SBIPS_INT1_SHIFT	0
#define	SBIPS_INT2_MASK	0x3f00		/* which sbflags get routed to mips interrupt 2 */
#define	SBIPS_INT2_SHIFT	8
#define	SBIPS_INT3_MASK	0x3f0000	/* which sbflags get routed to mips interrupt 3 */
#define	SBIPS_INT3_SHIFT	16
#define	SBIPS_INT4_MASK	0x3f000000	/* which sbflags get routed to mips interrupt 4 */
#define	SBIPS_INT4_SHIFT	24

/* sbtpsflag */
#define SBTPSFLAG	0x18		/* offset */
#define	SBTPS_NUM0_MASK	0x3f		/* interrupt sbFlag # generated by this core */
#define	SBTPS_F0EN0	0x40		/* interrupt is always sent on the backplane */

/* sbadmatch */
#define SBADMATCH3	0x60		/* offset */
#define SBADMATCH2	0x68		/* offset */
#define SBADMATCH1	0x70		/* offset */

/* sbimstate */
#define SBIMSTATE	0x90		/* offset */
#define	SBIM_PC		0xf		/* pipecount */
#define	SBIM_AP_MASK	0x30		/* arbitration policy */
#define	SBIM_AP_BOTH	0x00		/* use both timeslaces and token */
#define	SBIM_AP_TS	0x10		/* use timesliaces only */
#define	SBIM_AP_TK	0x20		/* use token only */
#define	SBIM_AP_RSV	0x30		/* reserved */
#define	SBIM_IBE	0x20000		/* inbanderror */
#define	SBIM_TO		0x40000		/* timeout */

/* sbintvec */
#define SBINTVEC	0x94		/* offset */

/* sbtmstatelow */
#define SBTMSTATELOW	0x98		/* offset */
#define	SBTML_RESET	0x1		/* reset */
#define	SBTML_REJ	0x2		/* reject */
#define	SBTML_CLK	0x10000		/* clock enable */
#define	SBTML_FGC	0x20000		/* force gated clocks on */
#define	SBTML_FL_MASK	0x3ffc0000	/* core-specific flags */
#define	SBTML_PE	0x40000000	/* pme enable */
#define	SBTML_BE	0x80000000	/* bist enable */

/* sbtmstatehigh */
#define SBTMSTATEHIGH	0x9C		/* offset */
#define	SBTMH_SERR	0x1		/* serror */
#define	SBTMH_INT	0x2		/* interrupt */
#define	SBTMH_BUSY	0x4		/* busy */
#define	SBTMH_TO		0x00000020	/* timeout (sonics >= 2.3) */
#define	SBTMH_FL_MASK		0x1fff0000	/* core-specific flags */
#define	SBTMH_GCR	0x20000000	/* gated clock request */
#define	SBTMH_BISTF	0x40000000	/* bist failed */
#define	SBTMH_BISTD	0x80000000	/* bist done */

/* sbbwa0 */
#define SBBWA0		0xA0		/* offset */
#define	SBBWA_TAB0_MASK	0xffff		/* lookup table 0 */
#define	SBBWA_TAB1_MASK	0xffff		/* lookup table 1 */
#define	SBBWA_TAB1_SHIFT	16

/* sbimconfiglow */
#define SBIMCONFIGLOW	0xA8		/* offset */
#define	SBIMCL_STO_MASK	0x7		/* service timeout */
#define	SBIMCL_RTO_MASK	0x70		/* request timeout */
#define	SBIMCL_RTO_SHIFT	4
#define	SBIMCL_CID_MASK	0xff0000	/* connection id */
#define	SBIMCL_CID_SHIFT	16

/* sbimconfighigh */
#define SBIMCONFIGHIGH	0xAC		/* offset */
#define	SBIMCH_IEM_MASK	0xc		/* inband error mode */
#define	SBIMCH_TEM_MASK	0x30		/* timeout error mode */
#define	SBIMCH_TEM_SHIFT	4
#define	SBIMCH_BEM_MASK	0xc0		/* bus error mode */
#define	SBIMCH_BEM_SHIFT	6

/* sbadmatch0 */
#define SBADMATCH0	0xB0		/* offset */
#define	SBAM_TYPE_MASK	0x3		/* address type */
#define	SBAM_AD64	0x4		/* reserved */
#define	SBAM_ADINT0_MASK	0xf8	/* type0 size */
#define	SBAM_ADINT0_SHIFT	3
#define	SBAM_ADINT1_MASK	0x1f8	/* type1 size */
#define	SBAM_ADINT1_SHIFT	3
#define	SBAM_ADINT2_MASK	0x1f8	/* type2 size */
#define	SBAM_ADINT2_SHIFT	3
#define	SBAM_ADEN		0x400	/* enable */
#define	SBAM_ADNEG		0x800	/* negative decode */
#define	SBAM_BASE0_MASK	0xffffff00	/* type0 base address */
#define	SBAM_BASE0_SHIFT	8
#define	SBAM_BASE1_MASK	0xfffff000	/* type1 base address for the core */
#define	SBAM_BASE1_SHIFT	12
#define	SBAM_BASE2_MASK	0xffff0000	/* type2 base address for the core */
#define	SBAM_BASE2_SHIFT	16

/* sbtmconfiglow */
#define SBTMCONFIGLOW	0xB8		/* offset */
#define	SBTMCL_CD_MASK	0xff		/* clock divide */
#define	SBTMCL_CO_MASK	0xf800		/* clock offset */
#define	SBTMCL_CO_SHIFT	11
#define	SBTMCL_IF_MASK	0xfc0000	/* interrupt flags */
#define	SBTMCL_IF_SHIFT	18
#define	SBTMCL_IM_MASK	0x3000000	/* interrupt mode */
#define	SBTMCL_IM_SHIFT	24

/* sbtmconfighigh */
#define SBTMCONFIGHIGH	0xBC		/* offset */
#define	SBTMCH_BM_MASK	0x3		/* busy mode */
#define	SBTMCH_RM_MASK	0x3		/* retry mode */
#define	SBTMCH_RM_SHIFT	2
#define	SBTMCH_SM_MASK	0x30		/* stop mode */
#define	SBTMCH_SM_SHIFT	4
#define	SBTMCH_EM_MASK	0x300		/* sb error mode */
#define	SBTMCH_EM_SHIFT	8
#define	SBTMCH_IM_MASK	0xc00		/* int mode */
#define	SBTMCH_IM_SHIFT	10

/* sbbconfig */
#define SBBCONFIG	0xC0		/* offset */
#define	SBBC_LAT_MASK	0x3		/* sb latency */
#define	SBBC_MAX0_MASK	0xf0000		/* maxccntr0 */
#define	SBBC_MAX0_SHIFT	16
#define	SBBC_MAX1_MASK	0xf00000	/* maxccntr1 */
#define	SBBC_MAX1_SHIFT	20

/* sbbstate */
#define SBBSTATE	0xC8		/* offset */
#define	SBBS_SRD	0x1		/* st reg disable */
#define	SBBS_HRD	0x2		/* hold reg disable */

/* sbactcnfg */
#define SBACTCNFG	0xD8		/* offset */

/* sbflagst */
#define	SBFLAGST	0xE8		/* offset */

/* sbidlow */
#define SBIDLOW		0xF8		/* offset */
#define	SBIDL_CS_MASK	0x3		/* config space */
#define	SBIDL_AR_MASK	0x38		/* # address ranges supported */
#define	SBIDL_AR_SHIFT	3
#define	SBIDL_SYNCH	0x40		/* sync */
#define	SBIDL_INIT	0x80		/* initiator */
#define	SBIDL_MINLAT_MASK	0xf00	/* minimum backplane latency */
#define	SBIDL_MINLAT_SHIFT	8
#define	SBIDL_MAXLAT	0xf000		/* maximum backplane latency */
#define	SBIDL_MAXLAT_SHIFT	12
#define	SBIDL_FIRST	0x10000		/* this initiator is first */
#define	SBIDL_CW_MASK	0xc0000		/* cycle counter width */
#define	SBIDL_CW_SHIFT	18
#define	SBIDL_TP_MASK	0xf00000	/* target ports */
#define	SBIDL_TP_SHIFT	20
#define	SBIDL_IP_MASK	0xf000000	/* initiator ports */
#define	SBIDL_IP_SHIFT	24

/* sbidhigh */
#define SBIDHIGH	0xFC		/* offset */
#define	SBIDH_RC_MASK	0xf		/* revision code*/
#define	SBIDH_CC_MASK	0xfff0		/* core code */
#define	SBIDH_CC_SHIFT	4
#define	SBIDH_VC_MASK	0xffff0000	/* vendor code */
#define	SBIDH_VC_SHIFT	16

#define	SB_COMMIT	0xfd8		/* update buffered registers value */

/* vendor codes */
#define	SB_VEND_BCM	0x4243		/* Broadcom's SB vendor code */

/* core codes */
#define	SB_CC		0x800		/* chipcommon core */
#define	SB_ILINE20	0x801		/* iline20 core */
#define	SB_SDRAM	0x803		/* sdram core */
#define	SB_PCI		0x804		/* pci core */
#define	SB_MIPS		0x805		/* mips core */
#define	SB_ENET		0x806		/* enet mac core */
#define	SB_CODEC	0x807		/* v90 codec core */
#define	SB_USB		0x808		/* usb core */
#define	SB_ILINE100	0x80a		/* iline100 core */
#define	SB_IPSEC	0x80b		/* ipsec core */
#define	SB_PCMCIA	0x80d		/* pcmcia core */
#define	SB_MEMC		0x80f		/* memc sdram core */
#define	SB_EXTIF	0x811		/* external interface core */
#define	SB_D11		0x812		/* 802.11 MAC core */
#define	SB_MIPS33	0x816		/* mips3302 core */

#endif	/* _SBCONFIG_H */
