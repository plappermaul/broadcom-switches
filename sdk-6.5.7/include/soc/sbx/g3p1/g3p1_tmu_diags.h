/**
 *
 * $Id: tmu_diags_h.stg,v 1.7 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * 
 *
 * g3p1_tmu_diags.h: Guadalupe2k V1.3 microcode diagnostics routines 
 *
 * This file provides the public interface to the Guadalupe2k V1.3
 * microcode.  Only explicitly documented elements (types  & functions)
 * are supported, external interfaces.  Other elements are exported only
 * for debugging convenience.
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */


#ifndef _G3P1_TMU_DIAGS_H
#define _G3P1_TMU_DIAGS_H
#if defined(BCM_CALADAN3_SUPPORT) && defined(BCM_CALADAN3_G3P1_SUPPORT) 
#include <sal/types.h>
#include <soc/sbx/g3p1/g3p1_defs.h>
#include <soc/sbx/g3p1/g3p1_tmu.h>




/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_mac_print(int unit, soc_sbx_g3p1_mac_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_pvv2e_print(int unit, soc_sbx_g3p1_pvv2e_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_labels_print(int unit, soc_sbx_g3p1_labels_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_v4da_print(int unit, soc_sbx_g3p1_v4da_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_v4sa_print(int unit, soc_sbx_g3p1_v4sa_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_v4mc_sgv_print(int unit, soc_sbx_g3p1_v4mc_sgv_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_v4mc_gv_print(int unit, soc_sbx_g3p1_v4mc_gv_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_v6da_print(int unit, soc_sbx_g3p1_v6da_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_v6sa_print(int unit, soc_sbx_g3p1_v6sa_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_v6mc_gv_print(int unit, soc_sbx_g3p1_v6mc_gv_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_v6mc_sgv_print(int unit, soc_sbx_g3p1_v6mc_sgv_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_oamrx_print(int unit, soc_sbx_g3p1_oamrx_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_maidmep_print(int unit, soc_sbx_g3p1_maidmep_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_emlfilter_print(int unit, soc_sbx_g3p1_emlfilter_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_ete_print(int unit, soc_sbx_g3p1_ete_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_oam_ep_print(int unit, soc_sbx_g3p1_oam_ep_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_oam_local_print(int unit, soc_sbx_g3p1_oam_local_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_oam_peer_print(int unit, soc_sbx_g3p1_oam_peer_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_labels3_print(int unit, soc_sbx_g3p1_labels3_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_bfd_ld2e_print(int unit, soc_sbx_g3p1_bfd_ld2e_t *e);


/**
 * Print mac entry.
 *
 * @param[in]  unit chip unit number
 * @param[in]  e entry parameters
 */
extern void soc_sbx_g3p1_sample_dm1_print(int unit, soc_sbx_g3p1_sample_dm1_t *e);




/*
 * Utility method for updating values of overlapping fields
 */
int soc_sbx_g3p1_update_overlaps(int unit, int table, uint32 *e, char *name, int value);

/**
 * Diagnostic shell print.
 *
 * This function drives printing the state of microcode objects
 * (constants, globals, entries, memories and the memory map) based
 * on the supplied command line arguments.
 *
 * @param[in]  unit chip unit number
 * @param[in]  argc number of argument strings
 * @param[in]  argv pointer to array of argument strings
 */
extern int soc_sbx_g3p1_tmu_shell_print(int unit, int argc, char **argv);

/**
 * Diagnostic shell set.
 *
 * This function drives setting the state of microcode objects
 * (globals, entries, and memories) based
 * on the supplied command line arguments.
 *
 * @param[in]  unit chip unit number
 * @param[in]  argc number of argument strings
 * @param[in]  argv pointer to array of argument strings
 */
extern int soc_sbx_g3p1_tmu_shell_set(int unit, int argc, char **argv);

/**
 * Diagnostic shell delete.
 *
 * This function drives deleting microcode entries that support a 
 * delete operation based on the supplied command line arguments.
 *
 * @param[in]  unit chip unit number
 * @param[in]  argc number of argument strings
 * @param[in]  argv pointer to array of argument strings
 */
extern int soc_sbx_g3p1_tmu_shell_delete(int unit, int argc, char **argv);

/** Diagnostic shell set operation usage message */
extern char soc_sbx_g3p1_tmu_set_usage[];
/** Diagnostic shell print operation usage message */
extern char soc_sbx_g3p1_tmu_get_usage[];
/** Diagnostic shell delete operation usage message */
extern char soc_sbx_g3p1_tmu_delete_usage[];

#endif

#endif
