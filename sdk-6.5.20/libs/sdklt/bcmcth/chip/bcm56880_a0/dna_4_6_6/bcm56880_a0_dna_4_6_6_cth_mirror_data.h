/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by xfcr from map.yml files.
 *
 * Tool: $SDK/INTERNAL/fltg/bin/xfcr
 *
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#ifndef BCM56880_A0_DNA_4_6_6_CTH_MIRROR_DATA_H
#define BCM56880_A0_DNA_4_6_6_CTH_MIRROR_DATA_H
static const bcmcth_mirror_flexhdr_info_t bcm56880_a0_dna_4_6_6_cth_flexhdr_info[1];
static const bcmcth_mirror_data_t bcm56880_a0_dna_4_6_6_cth_mirror_data;

#include <bcmpkt/flexhdr/bcmpkt_generic_loopback_t.h>

static const bcmcth_mirror_flexhdr_info_t bcm56880_a0_dna_4_6_6_cth_flexhdr_info[1] = {
    {
        .enable_fid = BCM56880_A0_DNA_4_6_6_MIRROR_ENCAP_GENERIC_LOOPBACKt_ENCAPf,
        .fname = "HDR",
        .get = bcmpkt_generic_loopback_t_field_get,
        .key_fid = BCM56880_A0_DNA_4_6_6_MIRROR_ENCAP_GENERIC_LOOPBACKt_MIRROR_ENCAP_IDf,
        .set = bcmpkt_generic_loopback_t_field_set,
        .sid = BCM56880_A0_DNA_4_6_6_MIRROR_ENCAP_GENERIC_LOOPBACKt,
        .size = 16,
    },
};

static const bcmcth_mirror_data_t bcm56880_a0_dna_4_6_6_cth_mirror_data = {
    .edit_id_base = 4,
    .flexhdr = bcm56880_a0_dna_4_6_6_cth_flexhdr_info,
    .flexhdr_num = 1,
    .hdr_id_mirror_transport = 1,
    .seq_num_0_cont_num = 12,
    .seq_num_0_src_sel = 1,
    .seq_num_1_cont_num = 9,
    .seq_num_1_src_sel = 0,
};


#endif /* BCM56880_A0_DNA_4_6_6_CTH_MIRROR_DATA_H */
