/*
 * $Id: vlan.h,v 1.3 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        vlan.h
 * Purpose:     Firebolt VLAN_MAC hardware table manipulation support
 */

#ifndef _SOC_VLAN_H_
#define _SOC_VLAN_H_

#include <soc/hash.h>

#ifdef BCM_FIREBOLT_SUPPORT

extern int soc_fb_vlan_mac_entry_hash(int unit, int hash_sel, vlan_mac_entry_t *entry);
extern int soc_fb_vlanmac_entry_ins(int unit, vlan_mac_entry_t *entry);
extern int soc_fb_vlanmac_entry_bank_ins(int unit, uint8 banks, vlan_mac_entry_t *entry);
extern int soc_fb_vlanmac_entry_del(int unit, vlan_mac_entry_t *entry);
extern int soc_fb_vlanmac_entry_lkup(int unit, vlan_mac_entry_t *entry,
                                     vlan_mac_entry_t *result, int *index_ptr);

#endif	/* BCM_FIREBOLT_SUPPORT */

#endif	/* !_SOC_VLAN_H_ */
