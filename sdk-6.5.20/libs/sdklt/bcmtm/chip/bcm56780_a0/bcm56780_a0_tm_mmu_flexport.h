/*! \file bcm56780_a0_tm_mmu_flexport.h
 *
 * File contains MMU init and flexport related defines for bcm56780_a0.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCM56780_A0_TM_MMU_FLEXPORT_H
#define BCM56780_A0_TM_MMU_FLEXPORT_H

#define TD4_X9_MMU_NUM_L0_NODES_PER_HSP_PORT 12
#define TD4_X9_MMU_NUM_L0_NODES_FOR_CPU_PORT 12
#define TD4_X9_MMU_NUM_L1_NODES_PER_HSP_PORT 2
#define TD4_X9_MMU_NUM_L1_NODES_FOR_CPU_PORT 48
#define TD4_X9_MMU_NUM_Q_PER_HSP_PORT 12
#define TD4_X9_MMU_NUM_Q_FOR_CPU_PORT 48

#define TD4_X9_THDO_INVALID_DEV_PORT 255
#define TD4_X9_INVALID_MMU_PORT 255
#define TD4_X9_INVALID_DEV_PORT 80
#define TD4_X9_MMU_PORT_PIPE_OFFSET 32
#define TD4_X9_PHY_PORT_PIPE_OFFSET 40

#define TD4_X9_MMU_BST_TRACK_MODE 0
#define TD4_X9_MMU_BST_WMARK_MODE 1
#define TD4_X9_MMU_NUM_PGS 8
#define TD4_X9_MMU_NUM_SPS 4
#define TD4_X9_MMU_NUM_COS 12
#define TD4_X9_MMU_THDI_PG_MIN_LIMIT_LOSSLESS 7
#define TD4_X9_MMU_THDI_PG_MIN_LIMIT_LOSSY 0
#define TD4_X9_MMU_THDI_PG_HDRM_LIMIT_LOSSY 0
#define TD4_X9_MMU_NUM_SPEEDS 7
#define TD4_X9_MMU_TCT_ON_TO_CT_OFFSET 5

#define TD4_X9_MMU_PAUSE_PG 7

#define TD4_X9_MMU_FLUSH_OFF 0
#define TD4_X9_MMU_FLUSH_ON 1

typedef enum {
  MODE_SAF          = 1,  /* Store-and-forward. */
  MODE_SAME_SPEED   = 2,  /* Same speed cut-through. */
} ct_mode_e;

extern int
bcm56780_a0_tm_rqe_port_flush(int unit);

#endif /* BCM56780_A0_TM_MMU_FLEXPORT_H */

