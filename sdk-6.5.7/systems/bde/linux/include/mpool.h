/*
 * $Id: mpool.h,v 1.2 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */

#ifndef __MPOOL_H__
#define __MPOOL_H__

struct mpool_mem_s;
typedef struct mpool_mem_s* mpool_handle_t;

extern int mpool_init(void);
extern mpool_handle_t mpool_create(void* base_address, int size);
extern void* mpool_alloc(mpool_handle_t pool, int size);
extern void  mpool_free(mpool_handle_t pool, void* ptr);
extern int mpool_destroy(mpool_handle_t pool);

extern int mpool_usage(mpool_handle_t pool);

#endif /* __MPOOL_H__ */
