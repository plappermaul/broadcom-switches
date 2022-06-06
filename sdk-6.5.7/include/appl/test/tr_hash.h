/*
 * $Id: tr_hash.h,v 1.2 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * TRX Hash Test definitions.
 *
 */

#ifndef __TR_HASH__H
#define __TR_HASH__H

/*
 * TRX Hash Test Data Structure
 */

typedef struct tr_hash_testdata_s {
    int                 unit;
    int                 opt_count;
    int                 opt_verbose;
    int                 opt_reset;
    int                 opt_hash;
    int                 opt_dual_hash;
    int                 opt_base_ovid;
    int                 opt_base_ivid;
    int                 opt_vid_inc;         /* VID increment */
    int                 opt_base_label;
    int                 opt_label_inc;       /* MPLS label increment */
    COMPILER_DOUBLE     tm;
    uint32              save_hash_control;
    int                 hash_count;          /* Hash Alg count */
} tr_hash_testdata_t;

#define TR_VID_MAX            (0xfff)
#define TR_LABEL_MAX          (0xfffff)
#define TR_DEFAULT_HASH       FB_HASH_LSB

/* Test structure, to retain test settings between runs */
typedef struct tr_hash_test_s {
    int                    lw_set_up;  /* TRUE if tr_hash_setup() done */
    tr_hash_testdata_t     lp_hash;    /* hash Parameters */
    tr_hash_testdata_t     lp_ov;      /* overflow Parameters */
    tr_hash_testdata_t     *lp_cur;    /* Current test parameters */
    int                    lw_unit;    /* Unit # */
} tr_hash_test_t;

#endif /*!__TR_HASH__H */
