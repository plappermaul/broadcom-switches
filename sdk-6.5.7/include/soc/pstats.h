/*
 * $Id: pstats.h $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File for SOC Pktstats structures and routines
 */

#ifndef __SOC_PSTATS_H__
#define __SOC_PSTATS_H__

typedef struct soc_pstats_mem_desc {
    soc_mem_t mem;
    uint32 width;
    uint32 entries;
    uint32 shift;
} soc_pstats_mem_desc_t;

#define MAX_PSTATS_MEM_PER_BLK  10

typedef struct soc_pstats_tbl_desc {
    soc_block_t blk;
    soc_mem_t bmem;
    int pipe_enum;
    int mor_dma;
    soc_pstats_mem_desc_t desc[MAX_PSTATS_MEM_PER_BLK];
    int pattern_set;
    soc_mem_t mem;
    uint32 index;
    uint32 count;
} soc_pstats_tbl_desc_t;

typedef struct soc_pstats_tbl_ctrl {
    soc_block_t blk;
    uint32 tindex;
    uint32 entries;
    uint8 *buff;
    uint32 size;
    int flags;
    sal_sem_t dma_done;
} soc_pstats_tbl_ctrl_t;

extern int soc_pstats_init(int unit);
extern int soc_pstats_deinit(int unit);
extern int soc_pstats_tbl_pattern_get(int unit, soc_pstats_tbl_desc_t *tdesc,
                                      soc_mem_t *mem, uint32 *index, uint32 *count);
extern int soc_pstats_tbl_pattern_set(int unit, soc_pstats_tbl_desc_t *tdesc,
                                      soc_mem_t mem, uint32 index, uint32 count);
extern int soc_pstats_sync(int unit);
extern int soc_pstats_mem_get(int unit, soc_mem_t mem, uint8 *buf, int sync);

#endif /* __SOC_PSTATS_H__ */

