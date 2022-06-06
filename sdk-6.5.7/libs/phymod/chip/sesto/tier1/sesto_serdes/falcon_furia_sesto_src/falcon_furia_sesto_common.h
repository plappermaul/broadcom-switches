/********************************************************************************
 ********************************************************************************
 *                                                                              *
 *  Revision      :  $Id: falcon_furia_common.h 869 2015-01-23 00:24:44Z kirand $ *
 *                                                                              *
 *  Description   :  Defines and Enumerations required by Serdes APIs           *
 *                                                                              *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$                                                        *
 *  No portions of this material may be reproduced in any form without          *
 *  the written permission of:                                                  *
 *      Broadcom Corporation                                                    *
 *      5300 California Avenue                                                  *
 *      Irvine, CA  92617                                                       *
 *                                                                              *
 *  All information contained in this document is Broadcom Corporation          *
 *  company private proprietary, and trade secret.                              *
 *                                                                              *
 ********************************************************************************
 ********************************************************************************/

/** @file falcon_furia_common.h
 * Defines and Enumerations shared across Eagle/Merlin/Falcon APIs BUT NOT MICROCODE
 */

#ifndef FALCON_FURIA_API_COMMON_H
#define FALCON_FURIA_API_COMMON_H
#include "../common/srds_api_uc_common.h"

/** Macro to determine sign of a value */
#define sign(x) ((x>=0) ? 1 : -1)

#define UCODE_MAX_SIZE  32768

/*
 * Register Address Defines used by the API that are different between IPs 
 */
#define FALCON_FURIA_DSC_A_DSC_UC_CTRL 0xd03d
#define DSC_E_RX_PI_CNT_BIN_D 0xd075
#define DSC_E_RX_PI_CNT_BIN_P 0xd076
#define DSC_E_RX_PI_CNT_BIN_L 0xd077
#define DSC_E_RX_PI_CNT_BIN_PD 0xd070
#define DSC_E_RX_PI_CNT_BIN_LD 0xd071
#define FALCON_FURIA_TLB_RX_PRBS_CHK_ERR_CNT_MSB_STATUS 0xD16A
#define TLB_RX_PRBS_CHK_ERR_CNT_LSB_STATUS 0xD16B

/*
 * Register Address Defines used by the API that are COMMON across IPs
 */
#define MDIO_MMDSEL_AER_COM_MDIO_MASKDATA  0xFFDB   

#endif   
