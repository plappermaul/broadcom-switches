/* 
 * $Id: kbp.h,v 1.0 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        kbp.h
 * Purpose:     kbp init header file.
 *
 */

#ifndef _DPP_KBP_INIT_H
#define _DPP_KBP_INIT_H

#include <soc/dpp/ARAD/arad_kbp.h>
#include <soc/dpp/ARAD/arad_api_nif.h>
#include <soc/dpp/JER/JER_PP/jer_pp_kaps.h>


#define JER_KBP_APPL_SEC_MDIO_DEFAULT_ID (SOC_IS_JERICHO_PLUS(unit) ? 0x101 : 0x141)
#define ARAD_KBP_APPL_MDIO_DEFAULT_ID (SOC_IS_QAX(unit) ? 0x41 : \
                                       (SOC_IS_JERICHO(unit) ? 0x161 : 0x101))
#define JER_KBP_OP_APPL_MDIO_DEFAULT_ID  0x163
#define JER_KBP_OP_APPL_SEC_MDIO_DEFAULT_ID  0x163
#define ARAD_KBP_APPL_ILKN_REVERSE_DEFAULT 0x1

int dpp_kbp_init_kbp_interface(int unit, uint32 core, uint32 kbp_mdio_id, uint32 kbp_ilkn_rev);

int dpp_kbp_init_appl(int unit, uint32 second_kbp_supported, ARAD_INIT_ELK *elk_ptr);

int dpp_kaps_init(int unit);

#endif /* _DPP_KBP_INIT_H */
