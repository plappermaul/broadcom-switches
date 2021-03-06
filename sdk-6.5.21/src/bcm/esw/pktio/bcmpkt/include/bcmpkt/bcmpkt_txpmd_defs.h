#ifndef BCMPKT_TXPMD_DEFS_H
#define BCMPKT_TXPMD_DEFS_H
/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated from the registers file.
 * Edits to this file will be lost when it is regenerated.
 * Tool: INTERNAL/regs/xgs/generate-pmd.pl
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * This file provides field macros for the TX Packet MetaData (TXPMD, called
 * SOBMH in hardware.) access.
 */

/*!
 * \name TX packet metadata field IDs.
 * \anchor BCMPKT_TXPMD_XXX
 */
/*! \{ */
/*! Invalid BCMPKT_TXPMD FID indicator */
#define BCMPKT_TXPMD_FID_INVALID                 -1
/*! Start of frame indicator. */
#define BCMPKT_TXPMD_START                       0
/*!
 * 64 Header Types Supported. 1 means Packets from CPU with SOBMH header format
 * to Passthru NLF
 */
#define BCMPKT_TXPMD_HEADER_TYPE                 1
/*! PKT_LENGTH (set by hardware - in Iarb). */
#define BCMPKT_TXPMD_PKT_LENGTH                  2
/*! Pointer to cell buffer of this SOBMH cell. (Set by hardware) */
#define BCMPKT_TXPMD_IPCF_PTR                    3
/*! SOP indicator (set by hardware - in Iarb). */
#define BCMPKT_TXPMD_SOP                         4
/*! EOP indicator (set by hardware - in Iarb). */
#define BCMPKT_TXPMD_EOP                         5
/*! CELL_LENGTH (set by hardware - in Iarb). */
#define BCMPKT_TXPMD_CELL_LENGTH                 6
/*! CELL_ERROR or PURGE */
#define BCMPKT_TXPMD_CELL_ERROR                  7
/*! Indicates the local port to send a SOBMH packet out. */
#define BCMPKT_TXPMD_LOCAL_DEST_PORT             8
/*! Source module ID, must be programmed to MY_MODID. */
#define BCMPKT_TXPMD_SRC_MODID                   9
/*!
 * Class of service for MMU queueing for this packet - sets COS values,
 * PBI.UC_COS, PBI.MC_COS1, and PBI.MC_COS2.
 */
#define BCMPKT_TXPMD_COS                         10
/*! Traffic priority to be applied to MMU via PBI.INPUT_PRIORITY. */
#define BCMPKT_TXPMD_INPUT_PRI                   11
/*! Indicates that PBI.UNICAST should be set to queue as unicast packet. */
#define BCMPKT_TXPMD_UNICAST                     12
/*! Value for CCBI.RSQ_Q_NUM. */
#define BCMPKT_TXPMD_RQE_Q_NUM                   13
/*! Indicates that PBI.L2_BITMAP should be set (to queue as L2MC packet). */
#define BCMPKT_TXPMD_SET_L2BM                    14
/*! ONE STEP TIME STAMPING ENABLE */
#define BCMPKT_TXPMD_IEEE1588_ONE_STEP_ENABLE    15
/*! Regenerate UDP Checksum */
#define BCMPKT_TXPMD_IEEE1588_REGEN_UDP_CHECKSUM 16
/*! ITS_SIGN bit */
#define BCMPKT_TXPMD_IEEE1588_INGRESS_TIMESTAMP_SIGN 17
/*! HDR_OFFSET */
#define BCMPKT_TXPMD_IEEE1588_TIMESTAMP_HDR_OFFSET 18
/*!
 * Indicates for TS packet transmitted from CPU into IP that the outgoing packet
 * needs to have its transmit timestamp captured by the port.
 */
#define BCMPKT_TXPMD_TX_TS                       19
/*! For PBI.SPID_Override */
#define BCMPKT_TXPMD_SPID_OVERRIDE               20
/*! For PBI.SPID - Service Pool ID */
#define BCMPKT_TXPMD_SPID                        21
/*! For PBI.SPAP - Service Pool Priority (color) */
#define BCMPKT_TXPMD_SPAP                        22
/*! Will set CCBI_B.UNICAST_PKT */
#define BCMPKT_TXPMD_UNICAST_PKT                 23
/*! Timestamp action LSB. */
#define BCMPKT_TXPMD_TS_ACTION_LSB               24
/*! Timestamp action MSB. */
#define BCMPKT_TXPMD_TS_ACTION_MSB               25
/*! Time statmp type.  Encodings are 0-NTP TOD(64 bit), 1-PTP TOD(64bit) */
#define BCMPKT_TXPMD_TS_TYPE                     26
/*! Destination subport number */
#define BCMPKT_TXPMD_DST_SUBPORT_NUM             27
/*! Enable UDP incremental checksum */
#define BCMPKT_TXPMD_UDP_CHECKSUM_UPDATE_ENABLE  28
/*! Offset to UDP checksum field from start of MACS-SA. */
#define BCMPKT_TXPMD_UDP_CHECKSUM_OFFSET         29
/*! TXPMD FIELD ID NUMBER */
#define BCMPKT_TXPMD_FID_COUNT                   30
/*! \} */

/*! TXPMD field name strings for debugging. */
#define BCMPKT_TXPMD_FIELD_NAME_MAP_INIT \
    {"START", BCMPKT_TXPMD_START},\
    {"HEADER_TYPE", BCMPKT_TXPMD_HEADER_TYPE},\
    {"PKT_LENGTH", BCMPKT_TXPMD_PKT_LENGTH},\
    {"IPCF_PTR", BCMPKT_TXPMD_IPCF_PTR},\
    {"SOP", BCMPKT_TXPMD_SOP},\
    {"EOP", BCMPKT_TXPMD_EOP},\
    {"CELL_LENGTH", BCMPKT_TXPMD_CELL_LENGTH},\
    {"CELL_ERROR", BCMPKT_TXPMD_CELL_ERROR},\
    {"LOCAL_DEST_PORT", BCMPKT_TXPMD_LOCAL_DEST_PORT},\
    {"SRC_MODID", BCMPKT_TXPMD_SRC_MODID},\
    {"COS", BCMPKT_TXPMD_COS},\
    {"INPUT_PRI", BCMPKT_TXPMD_INPUT_PRI},\
    {"UNICAST", BCMPKT_TXPMD_UNICAST},\
    {"RQE_Q_NUM", BCMPKT_TXPMD_RQE_Q_NUM},\
    {"SET_L2BM", BCMPKT_TXPMD_SET_L2BM},\
    {"IEEE1588_ONE_STEP_ENABLE", BCMPKT_TXPMD_IEEE1588_ONE_STEP_ENABLE},\
    {"IEEE1588_REGEN_UDP_CHECKSUM", BCMPKT_TXPMD_IEEE1588_REGEN_UDP_CHECKSUM},\
    {"IEEE1588_INGRESS_TIMESTAMP_SIGN", BCMPKT_TXPMD_IEEE1588_INGRESS_TIMESTAMP_SIGN},\
    {"IEEE1588_TIMESTAMP_HDR_OFFSET", BCMPKT_TXPMD_IEEE1588_TIMESTAMP_HDR_OFFSET},\
    {"TX_TS", BCMPKT_TXPMD_TX_TS},\
    {"SPID_OVERRIDE", BCMPKT_TXPMD_SPID_OVERRIDE},\
    {"SPID", BCMPKT_TXPMD_SPID},\
    {"SPAP", BCMPKT_TXPMD_SPAP},\
    {"UNICAST_PKT", BCMPKT_TXPMD_UNICAST_PKT},\
    {"TS_ACTION_LSB", BCMPKT_TXPMD_TS_ACTION_LSB},\
    {"TS_ACTION_MSB", BCMPKT_TXPMD_TS_ACTION_MSB},\
    {"TS_TYPE", BCMPKT_TXPMD_TS_TYPE},\
    {"DST_SUBPORT_NUM", BCMPKT_TXPMD_DST_SUBPORT_NUM},\
    {"UDP_CHECKSUM_UPDATE_ENABLE", BCMPKT_TXPMD_UDP_CHECKSUM_UPDATE_ENABLE},\
    {"UDP_CHECKSUM_OFFSET", BCMPKT_TXPMD_UDP_CHECKSUM_OFFSET},\
    {"fid count", BCMPKT_TXPMD_FID_COUNT}

/*!
 * \name BCMPKT_TXPMD_START encodings.
 * \anchor BCMPKT_TXPMD_START_XXX
 */
/*! \{ */
/*! The header used internally only */
#define BCMPKT_TXPMD_START_INTERNAL_HEADER       2
/*! Frame type is Higig */
#define BCMPKT_TXPMD_START_HIGIG                 3
/*! \} */

/*! BCMPKT_TXPMD_START encoding name strings for debugging. */
#define BCMPKT_TXPMD_START_NAME_MAP_INIT \
    {"RESERVED_COUNTER", 0},\
    {"RESERVED_COUNTER", 1},\
    {"INTERNAL_HEADER", BCMPKT_TXPMD_START_INTERNAL_HEADER},\
    {"HIGIG", BCMPKT_TXPMD_START_HIGIG},\

/*!
 * \name BCMPKT_TXPMD_HEADER_TYPE encodings.
 * \anchor BCMPKT_TXPMD_HEADER_TYPE_XXX
 */
/*! \{ */
/*! EP Copy to CPU format, SOBMH header in EP to Passthru NLF */
#define BCMPKT_TXPMD_HEADER_T_TO_CPU             0
/*! Packets from CPU with SOBMH header format to Passthru NLF */
#define BCMPKT_TXPMD_HEADER_T_FROM_CPU           1
/*! MAC in MAC packets to Passthru NLF */
#define BCMPKT_TXPMD_HEADER_T_MIM                2
/*! MPLS packets to Passthru NLF */
#define BCMPKT_TXPMD_HEADER_T_MPLS_PMP           3
/*! Trill Network Packets to Passthru NLF */
#define BCMPKT_TXPMD_HEADER_T_TRILL_NW           4
/*! Trill Access Layer Packets to Passthru NLF */
#define BCMPKT_TXPMD_HEADER_T_TRILL_AC           5
/*! WLAN Decap packets sent to WRX NLF */
#define BCMPKT_TXPMD_HEADER_T_WLAN_DECAP         6
/*! WLAN Encap packets sent to WTX NLF */
#define BCMPKT_TXPMD_HEADER_T_WLAN_ENCAP         7
/*! QCN Packets to Passthru NLF */
#define BCMPKT_TXPMD_HEADER_T_QCN                8
/*! DPI/Signature Matcher packets sent to SM NLF */
#define BCMPKT_TXPMD_HEADER_T_SM_DPI             9
/*! EP Redirection packets to Passthru NLF */
#define BCMPKT_TXPMD_HEADER_T_EP_REDIR           10
/*! Other generic passthrough to Passthru NLF */
#define BCMPKT_TXPMD_HEADER_T_GENERIC            11
/*! DOWN MEP Transmit OAM Packets from CPU or CCM from HW Engine or OLP */
#define BCMPKT_TXPMD_HEADER_T_OAM_DOWNMEP_TX     12
/*! UP MEP Transmit OAM Packets from CPU or CCM from HW Engine or OLP */
#define BCMPKT_TXPMD_HEADER_T_OAM_UPMEP_TX       13
/*! \} */

/*! BCMPKT_TXPMD_HEADER_TYPE encoding name strings for debugging. */
#define BCMPKT_TXPMD_HEADER_TYPE_NAME_MAP_INIT \
    {"SOBMH_EP_COPY_TO_CPU", BCMPKT_TXPMD_HEADER_T_TO_CPU},\
    {"SOBMH_FROM_CPU", BCMPKT_TXPMD_HEADER_T_FROM_CPU},\
    {"MIM", BCMPKT_TXPMD_HEADER_T_MIM},\
    {"MPLS_PMP", BCMPKT_TXPMD_HEADER_T_MPLS_PMP},\
    {"TRILL_NW", BCMPKT_TXPMD_HEADER_T_TRILL_NW},\
    {"TRILL_AC", BCMPKT_TXPMD_HEADER_T_TRILL_AC},\
    {"WLAN_DECAP", BCMPKT_TXPMD_HEADER_T_WLAN_DECAP},\
    {"WLAN_ENCAP", BCMPKT_TXPMD_HEADER_T_WLAN_ENCAP},\
    {"QCN", BCMPKT_TXPMD_HEADER_T_QCN},\
    {"SM_DPI", BCMPKT_TXPMD_HEADER_T_SM_DPI},\
    {"EP_REDIR", BCMPKT_TXPMD_HEADER_T_EP_REDIR},\
    {"GENERIC", BCMPKT_TXPMD_HEADER_T_GENERIC},\
    {"OAM_DOWNMEP_TX", BCMPKT_TXPMD_HEADER_T_OAM_DOWNMEP_TX},\
    {"OAM_UPMEP_TX", BCMPKT_TXPMD_HEADER_T_OAM_UPMEP_TX},\

/*!
 * \name TX packet metadata internal usage field IDs.
 * \anchor BCMPKT_TXPMD_I_XXX
 */
/*! \{ */
/*! Invalid BCMPKT_TXPMD_I FID indicator */
#define BCMPKT_TXPMD_I_FID_INVALID               -1
/*! TXPMD RX raw data size. */
#define BCMPKT_TXPMD_I_SIZE                      0
/*! TXPMD_I FIELD ID NUMBER */
#define BCMPKT_TXPMD_I_FID_COUNT                 1
/*! \} */

/*! TXPMD_I field name strings for debugging. */
#define BCMPKT_TXPMD_I_FIELD_NAME_MAP_INIT \
    {"SIZE", BCMPKT_TXPMD_I_SIZE},\
    {"fid count", BCMPKT_TXPMD_I_FID_COUNT}

#endif /*! BCMPKT_TXPMD_DEFS_H */
