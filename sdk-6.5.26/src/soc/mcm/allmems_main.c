/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated from the registers file.
 * Edits to this file will be lost when it is regenerated.
 *
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 *
 * File:        allmems_main.c
 * Purpose:     Independent memory descriptions.
 */

#define SOC_MCM_USE_FIELD_DATA /* needed for the soc_field_info_t variable declarations in include/soc/mcm/allfields.h */
#include <soc/types.h>
#include <soc/mcm/allenum.h>
#include <soc/mcm/intenum.h>
#include <soc/mcm/memregs.h>
#include <soc/mem.h>

/*
 * This array is indexed by soc_mem_int_t; it describes all versions of
 * memories for all chips.  Chip memory arrays index into this summary.
 */

soc_mem_info_t soc_mem_list[] = {
#include "allmems_a.i"
#include "allmems_b.i"
#include "allmems_c.i"
#include "allmems_d.i"
#include "allmems_e.i"
#include "allmems_f.i"
#include "allmems_g.i"
#include "allmems_h.i"
#include "allmems_i.i"
#include "allmems_j.i"
#include "allmems_k.i"
#include "allmems_l.i"
#include "allmems_m.i"
#include "allmems_n.i"
#include "allmems_o.i"
#include "allmems_p.i"
#include "allmems_q.i"
#include "allmems_r.i"
#include "allmems_s.i"
#include "allmems_t.i"
#include "allmems_u.i"
#include "allmems_v.i"
#include "allmems_w.i"
#include "allmems_x.i"
#include "allmems_y.i"
#include "allmems_z.i"
    {0} /* Extra empty entry for compiler */
}; /* soc_mem_list array */

/*
 * The arrays soc_mem_name, soc_mem_alias and soc_mem_desc are indexed
 * by soc_mem_t like the chip specific memory arrays.
 */
#if !defined(SOC_NO_NAMES)
char *soc_mem_name[] = {
#include "allmems_names.i"
    "" /* Extra null entry for compiler */
}; /* soc_mem_name array */
#endif /* !defined(SOC_NO_NAMES) */

#if !defined(SOC_NO_NAMES)
char *soc_mem_ufname[] = {
#include "allmems_ufnames.i"
    "" /* Extra null entry for compiler */
}; /* soc_mem_ufname array */
#endif /* !defined(SOC_NO_NAMES) */

#if !defined(SOC_NO_ALIAS)
char *soc_mem_ufalias[] = {
#include "allmems_ufalias.i"
    "" /* Extra null entry for compiler */
}; /* soc_mem_ufalias array */
#endif /* !defined(SOC_NO_ALIAS) */

#if !defined(SOC_NO_ALIAS_MAP)
soc_mem_t soc_mem_alias_map[] = {
#include "allmems_alias_map.i"
    INVALIDm /* Extra entry for compiler */
}; /* soc_mem_alias_map array */
#endif /* !defined(SOC_NO_ALIAS_MAP) */

#if !defined(SOC_NO_DESC)
char *soc_mem_desc[] = {
#include "allmems_desc.i"
    "" /* Extra null entry for compiler */
}; /* soc_mem_desc array */
#endif /* !defined(SOC_NO_DESC) */