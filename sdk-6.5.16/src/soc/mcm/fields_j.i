/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated from the registers file.
 * Edits to this file will be lost when it is regenerated.
 *
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * File:        fields_j.i
 * Purpose:     Field declarations.
 */

#if defined(BCM_56440_A0) || defined(BCM_56440_B0)
soc_field_info_t soc_JBCASCFG_CHID_0r_fields[] = {
    { BATM_CASPLSZf, 11, 0, SOCF_LE },
    { BATM_RSVDf, 5, 11, SOCF_LE|SOCF_RES }
};

#endif
#if defined(BCM_56440_A0) || defined(BCM_56440_B0)
soc_field_info_t soc_JBCASWINr_fields[] = {
    { BATM_CASWINf, 8, 0, SOCF_LE },
    { BATM_RSVDf, 8, 8, SOCF_LE|SOCF_RES }
};

#endif
#if defined(BCM_56440_A0) || defined(BCM_56440_B0)
soc_field_info_t soc_JBCHCFG1_CHID_0r_fields[] = {
    { BATM_JBBOPf, 7, 0, SOCF_LE },
    { BATM_RSVDf, 3, 7, SOCF_LE|SOCF_RES },
    { BATM_WSZLMTf, 3, 10, SOCF_LE }
};

#endif
#if defined(BCM_56440_A0) || defined(BCM_56440_B0)
soc_field_info_t soc_JBCHCFG2_CHID_0r_fields[] = {
    { BATM_DRPVf, 1, 11, 0 },
    { BATM_EPKTPLSZf, 11, 0, SOCF_LE },
    { BATM_PSIEf, 1, 15, 0 },
    { BATM_RSVDf, 3, 12, SOCF_LE|SOCF_RES }
};

#endif
#if defined(BCM_56440_A0) || defined(BCM_56440_B0)
soc_field_info_t soc_JBCSTAT_CHID_0r_fields[] = {
    { BATM_JBSTATEf, 2, 12, SOCF_LE|SOCF_RO|SOCF_RES|SOCF_W1TC },
    { BATM_RSVDf, 10, 0, SOCF_LE|SOCF_RO|SOCF_RES|SOCF_W1TC },
    { BATM_TRMf, 1, 10, SOCF_RO|SOCF_RES|SOCF_W1TC }
};

#endif
#if defined(BCM_56440_A0) || defined(BCM_56440_B0)
soc_field_info_t soc_JBDEPC_TCID_0r_fields[] = {
    { BATM_ATSCOUNTf, 16, 0, SOCF_LE|SOCF_RO|SOCF_RES }
};

#endif
#if defined(BCM_56440_A0) || defined(BCM_56440_B0)
soc_field_info_t soc_JBDMAX1_CHID_0r_fields[] = {
    { BATM_MAXDEPTHf, 16, 0, SOCF_LE|SOCF_RO|SOCF_RES }
};

#endif
#if defined(BCM_56440_A0) || defined(BCM_56440_B0)
soc_field_info_t soc_JBDMIN1_CHID_0r_fields[] = {
    { BATM_MINDEPTHf, 16, 0, SOCF_LE|SOCF_RO|SOCF_RES }
};

#endif
#if defined(BCM_56440_A0) || defined(BCM_56440_B0)
soc_field_info_t soc_JBDSUM1_TCID_0r_fields[] = {
    { BATM_JBDEPSUMf, 16, 0, SOCF_LE|SOCF_RO|SOCF_RES }
};

#endif
#if defined(BCM_56440_A0) || defined(BCM_56440_B0)
soc_field_info_t soc_JBMISSPC_CHID_0r_fields[] = {
    { BATM_MISPKTCf, 16, 0, SOCF_LE|SOCF_RO|SOCF_RES }
};

#endif
#if defined(BCM_56440_A0) || defined(BCM_56440_B0)
soc_field_info_t soc_JBRSTRT_CHID_0r_fields[] = {
    { BATM_BRSTRTCf, 8, 0, SOCF_LE|SOCF_RO|SOCF_RES },
    { BATM_RSVDf, 8, 8, SOCF_LE|SOCF_RO|SOCF_RES }
};

#endif
#if defined(BCM_56440_A0) || defined(BCM_56440_B0)
soc_field_info_t soc_JBSLPCFGr_fields[] = {
    { BATM_PSLPSZf, 5, 11, SOCF_LE },
    { BATM_RSVDf, 5, 6, SOCF_LE|SOCF_RES },
    { BATM_SLPCHf, 6, 0, SOCF_LE }
};

#endif
#if defined(BCM_56440_A0) || defined(BCM_56440_B0)
soc_field_info_t soc_JBSLPCMDr_fields[] = {
    { BATM_BSLPf, 1, 15, 0 },
    { BATM_BSLPSZf, 11, 0, SOCF_LE },
    { BATM_GTREf, 1, 12, 0 },
    { BATM_PDIRf, 1, 13, 0 },
    { BATM_PSLPf, 1, 14, 0 },
    { BATM_RESERVEDf, 1, 11, SOCF_RES }
};

#endif
#if defined(BCM_56440_A0) || defined(BCM_56440_B0)
soc_field_info_t soc_JBUNDRPC_CHID_0r_fields[] = {
    { BATM_UNDRNCf, 16, 0, SOCF_LE|SOCF_RO|SOCF_RES }
};

#endif
#if defined(BCM_56440_A0) || defined(BCM_56440_B0) || \
    defined(BCM_56840_A0) || defined(BCM_56840_B0) || \
    defined(BCM_88732_A0)
soc_field_info_t soc_JTAG_OTP_CNTRL_FORMATfmt_fields[] = {
    { ACCESS_MODEf, 2, 102, SOCF_LE },
    { ADDRESSf, 10, 70, SOCF_LE },
    { ADDRESS1f, 3, 86, SOCF_LE },
    { BITSELf, 5, 64, SOCF_LE },
    { BITSEL_UNUSEDf, 1, 69, 0 },
    { BURST_STAT_SELf, 1, 104, 0 },
    { BYPASS_AUTOLOADf, 1, 101, 0 },
    { BYPASS_OTP_CLKf, 1, 127, 0 },
    { COMMANDf, 5, 81, SOCF_LE },
    { MAX_PROGCNTf, 4, 128, SOCF_LE },
    { OTP_DEBUG_MODEf, 1, 100, 0 },
    { OTP_PROG_ENf, 1, 125, 0 },
    { PROGRAM_VERIFY_FLAGf, 1, 107, 0 },
    { PROG_DATAf, 64, 0, SOCF_LE },
    { READ_FOUTf, 1, 106, 0 },
    { SOFT_RESETf, 1, 105, 0 },
    { STARTf, 1, 80, 0 },
    { TESTCOLf, 1, 126, 0 },
    { WRP_CONTINUE_TO_FAILf, 1, 99, 0 },
    { WRP_CPC_SELf, 4, 95, SOCF_LE },
    { WRP_DOUBLEFUSEf, 1, 91, 0 },
    { WRP_FUSESEL0f, 1, 141, 0 },
    { WRP_PBYPf, 1, 137, 0 },
    { WRP_PCOUNTf, 4, 132, SOCF_LE },
    { WRP_PROG_IN_DEBUGf, 1, 93, 0 },
    { WRP_QUADFUSEf, 1, 92, 0 },
    { WRP_READ2Xf, 1, 89, 0 },
    { WRP_READ4Xf, 1, 90, 0 },
    { WRP_REGC_SELf, 3, 138, SOCF_LE },
    { WRP_SADBYPf, 1, 136, 0 },
    { WRP_TMf, 9, 108, SOCF_LE },
    { WRP_VSELf, 8, 117, SOCF_LE }
};
#endif

#if defined(BCM_53400_A0) || defined(BCM_53540_A0) || \
    defined(BCM_53570_A0) || defined(BCM_53570_B0) || \
    defined(BCM_56160_A0) || defined(BCM_56260_A0) || \
    defined(BCM_56260_B0) || defined(BCM_56270_A0) || \
    defined(BCM_56560_A0) || defined(BCM_56560_B0) || \
    defined(BCM_56670_A0) || defined(BCM_56670_B0) || \
    defined(BCM_56960_A0) || defined(BCM_56965_A0)
soc_field_info_t soc_JTAG_OTP_CNTRL_FORMAT_BCM53400_A0fmt_fields[] = {
    { ADDRESSf, 12, 6, SOCF_LE },
    { COMMANDf, 5, 1, SOCF_LE },
    { PROG_DATAf, 64, 18, SOCF_LE },
    { STARTf, 1, 0, 0 }
};
#endif

#if defined(BCM_56150_A0) || defined(BCM_56340_A0) || \
    defined(BCM_56450_A0) || defined(BCM_56450_B0) || \
    defined(BCM_56450_B1) || defined(BCM_56640_A0) || \
    defined(BCM_56850_A0)
soc_field_info_t soc_JTAG_OTP_CNTRL_FORMAT_BCM56640_A0fmt_fields[] = {
    { ACCESS_MODEf, 2, 102, SOCF_LE },
    { ADDRESSf, 10, 70, SOCF_LE },
    { ADDRESS1f, 3, 86, SOCF_LE },
    { BITSELf, 5, 64, SOCF_LE },
    { BITSEL_UNUSEDf, 1, 69, 0 },
    { BURST_STAT_SELf, 1, 104, 0 },
    { BYPASS_AUTOLOADf, 1, 101, 0 },
    { BYPASS_OTP_CLKf, 1, 127, 0 },
    { COMMANDf, 5, 81, SOCF_LE },
    { MAX_PROGCNTf, 4, 128, SOCF_LE },
    { OTP_DEBUG_MODEf, 1, 100, 0 },
    { OTP_PROG_ENf, 1, 125, 0 },
    { PROGRAM_VERIFY_FLAGf, 1, 107, 0 },
    { PROG_DATAf, 64, 0, SOCF_LE },
    { READ_FOUTf, 1, 106, 0 },
    { SOFT_RESETf, 1, 105, 0 },
    { STARTf, 1, 80, 0 },
    { TESTCOLf, 1, 126, 0 },
    { WRP_CONTINUE_TO_FAILf, 1, 99, 0 },
    { WRP_CPC_SELf, 4, 95, SOCF_LE },
    { WRP_DOUBLEFUSEf, 1, 91, 0 },
    { WRP_FUSESEL0f, 1, 141, 0 },
    { WRP_PBYPf, 1, 137, 0 },
    { WRP_PCOUNTf, 4, 132, SOCF_LE },
    { WRP_PROG_IN_DEBUGf, 1, 93, 0 },
    { WRP_QUADFUSEf, 1, 92, 0 },
    { WRP_READ2Xf, 1, 89, 0 },
    { WRP_READ4Xf, 1, 90, 0 },
    { WRP_REGC_SELf, 3, 138, SOCF_LE },
    { WRP_SADBYPf, 1, 136, 0 },
    { WRP_TMf, 9, 108, SOCF_LE },
    { WRP_VSELf, 8, 117, SOCF_LE }
};
#endif

#if defined(BCM_56860_A0)
soc_field_info_t soc_JTAG_OTP_CNTRL_FORMAT_BCM56860_A0fmt_fields[] = {
    { ADDRESSf, 12, 6, SOCF_LE },
    { COMMANDf, 5, 1, SOCF_LE },
    { PROG_DATAf, 64, 18, SOCF_LE },
    { STARTf, 1, 0, 0 }
};
#endif

#if defined(BCM_56370_A0) || defined(BCM_56770_A0) || \
    defined(BCM_56870_A0)
soc_field_info_t soc_JTAG_OTP_CNTRL_FORMAT_BCM56870_A0fmt_fields[] = {
    { ADDRESSf, 9, 7, SOCF_LE },
    { COMMANDf, 5, 1, SOCF_LE },
    { PROG_DATAf, 72, 23, SOCF_LE },
    { STARTf, 1, 0, 0 }
};
#endif

#if defined(BCM_56970_A0) || defined(BCM_56980_A0) || \
    defined(BCM_56980_B0)
soc_field_info_t soc_JTAG_OTP_CNTRL_FORMAT_BCM56970_A0fmt_fields[] = {
    { ADDRESSf, 8, 7, SOCF_LE },
    { COMMANDf, 5, 1, SOCF_LE },
    { PROG_DATAf, 72, 23, SOCF_LE },
    { STARTf, 1, 0, 0 }
};
#endif

#if defined(BCM_56440_A0) || defined(BCM_56440_B0) || \
    defined(BCM_56840_A0) || defined(BCM_56840_B0) || \
    defined(BCM_88732_A0)
soc_field_info_t soc_JTAG_OTP_STATUS_FORMATfmt_fields[] = {
    { ADDRESSf, 10, 70, SOCF_LE },
    { ADDRESS1f, 4, 120, SOCF_LE },
    { ADDRESSFBf, 16, 99, SOCF_LE },
    { BISR_RSTNf, 1, 115, 0 },
    { BITSELf, 5, 64, SOCF_LE },
    { BITSEL_UNUSEDf, 1, 69, 0 },
    { COMMAND_DONEf, 1, 84, 0 },
    { CONTROL_ERRORf, 1, 119, 0 },
    { DISABLE_CPU_ACCESSf, 1, 96, 0 },
    { INVALID_ACCESS_MODEf, 1, 89, 0 },
    { INVALID_ADDRESSf, 1, 88, 0 },
    { INVALID_COMMANDf, 1, 87, 0 },
    { INVALID_JTAG_MODEf, 1, 92, 0 },
    { INVALID_PROG_REQf, 1, 85, 0 },
    { INVALID_SECURE_ACCESSf, 1, 90, SOCF_RES },
    { JTAG_DATA_MASK_BITf, 1, 97, 0 },
    { JTAG_MODEf, 1, 93, 0 },
    { JTAG_PROG_DISABLEDf, 1, 124, 0 },
    { JTAG_WORD_DOUTf, 64, 0, SOCF_LE },
    { OTP_2ND_ROW_NOWf, 1, 117, 0 },
    { OTP_STBY_REGf, 1, 116, 0 },
    { PROGOKf, 1, 94, 0 },
    { PROG_BLOCKEDf, 1, 86, 0 },
    { RESERVED_0f, 1, 98, SOCF_RES },
    { RESERVED_1f, 3, 125, SOCF_LE|SOCF_RES },
    { TESTCOLf, 1, 95, 0 },
    { WRP_BUSYf, 1, 82, 0 },
    { WRP_DATA_READYf, 1, 80, 0 },
    { WRP_DOUTf, 1, 81, 0 },
    { WRP_ERRORf, 1, 118, 0 },
    { WRP_FAILf, 1, 83, 0 },
    { WRP_FDONEf, 1, 91, 0 }
};
#endif

#if defined(BCM_53400_A0) || defined(BCM_53540_A0) || \
    defined(BCM_53570_A0) || defined(BCM_53570_B0) || \
    defined(BCM_56160_A0) || defined(BCM_56260_A0) || \
    defined(BCM_56260_B0) || defined(BCM_56270_A0) || \
    defined(BCM_56560_A0) || defined(BCM_56560_B0) || \
    defined(BCM_56670_A0) || defined(BCM_56670_B0) || \
    defined(BCM_56960_A0) || defined(BCM_56965_A0)
soc_field_info_t soc_JTAG_OTP_STATUS_FORMAT_BCM53400_A0fmt_fields[] = {
    { JTAG_WORD_DOUTf, 64, 24, SOCF_LE },
    { OTP_STATUSf, 24, 0, SOCF_LE }
};
#endif

#if defined(BCM_56150_A0) || defined(BCM_56340_A0) || \
    defined(BCM_56450_A0) || defined(BCM_56450_B0) || \
    defined(BCM_56450_B1) || defined(BCM_56640_A0) || \
    defined(BCM_56850_A0)
soc_field_info_t soc_JTAG_OTP_STATUS_FORMAT_BCM56640_A0fmt_fields[] = {
    { ADDRESSf, 10, 70, SOCF_LE },
    { ADDRESS1f, 4, 120, SOCF_LE },
    { ADDRESSFBf, 16, 99, SOCF_LE },
    { BISR_RSTNf, 1, 115, 0 },
    { BITSELf, 5, 64, SOCF_LE },
    { BITSEL_UNUSEDf, 1, 69, 0 },
    { COMMAND_DONEf, 1, 84, 0 },
    { CONTROL_ERRORf, 1, 119, 0 },
    { DISABLE_CPU_ACCESSf, 1, 96, 0 },
    { INVALID_ACCESS_MODEf, 1, 89, 0 },
    { INVALID_ADDRESSf, 1, 88, 0 },
    { INVALID_COMMANDf, 1, 87, 0 },
    { INVALID_JTAG_MODEf, 1, 92, 0 },
    { INVALID_PROG_REQf, 1, 85, 0 },
    { INVALID_SECURE_ACCESSf, 1, 90, SOCF_RES },
    { JTAG_DATA_MASK_BITf, 1, 97, 0 },
    { JTAG_MODEf, 1, 93, 0 },
    { JTAG_PROG_DISABLEDf, 1, 124, 0 },
    { JTAG_WORD_DOUTf, 64, 0, SOCF_LE },
    { OTP_2ND_ROW_NOWf, 1, 117, 0 },
    { OTP_STBY_REGf, 1, 116, 0 },
    { PROGOKf, 1, 94, 0 },
    { PROG_BLOCKEDf, 1, 86, 0 },
    { RESERVED_0f, 1, 98, SOCF_RES },
    { RESERVED_1f, 3, 125, SOCF_LE|SOCF_RES },
    { TESTCOLf, 1, 95, 0 },
    { WRP_BUSYf, 1, 82, 0 },
    { WRP_DATA_READYf, 1, 80, 0 },
    { WRP_DOUTf, 1, 81, 0 },
    { WRP_ERRORf, 1, 118, 0 },
    { WRP_FAILf, 1, 83, 0 },
    { WRP_FDONEf, 1, 91, 0 }
};
#endif

#if defined(BCM_56860_A0)
soc_field_info_t soc_JTAG_OTP_STATUS_FORMAT_BCM56860_A0fmt_fields[] = {
    { JTAG_WORD_DOUTf, 64, 24, SOCF_LE },
    { OTP_STATUSf, 24, 0, SOCF_LE }
};
#endif

#if defined(BCM_56370_A0) || defined(BCM_56770_A0) || \
    defined(BCM_56870_A0) || defined(BCM_56970_A0) || \
    defined(BCM_56980_A0) || defined(BCM_56980_B0)
soc_field_info_t soc_JTAG_OTP_STATUS_FORMAT_BCM56970_A0fmt_fields[] = {
    { JTAG_WORD_DOUTf, 72, 32, SOCF_LE },
    { OTP_STATUSf, 32, 0, SOCF_LE }
};
#endif
