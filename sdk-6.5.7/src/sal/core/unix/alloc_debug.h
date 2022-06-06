/*
 * $Id: alloc_debug.h,v 1.6 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */

#ifndef _SAL_ALLOC_DEBUG_H
#define _SAL_ALLOC_DEBUG_H

#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <execinfo.h>
#include <stdio.h>
#include <pthread.h>

#include <sal/types.h>

typedef struct {
    size_t addr;
    int size;
    void* stack[3];
    CONST char *desc;
} alloc_info;

#define BUF_SIZE 1024
#define MAX_ALLOCS 100000

static alloc_info allocs[MAX_ALLOCS];
static int nof_allocs = 0;
static pthread_mutex_t alloc_test_mutex;
static int _test_ininted = 0;

static void
enter_mutex(void)
{
    if (!_test_ininted) {
        _test_ininted = 1;
        pthread_mutex_init(&alloc_test_mutex, NULL);
    }
    pthread_mutex_lock(&alloc_test_mutex);
}

/* print all the allocations that were not de-alloced in the given size range */
void print_sal_allocs(int min_size, int max_size)
{
  alloc_info *p_a = allocs;
  int i, j=0;

  enter_mutex();
  for (i = nof_allocs; i; --i, ++p_a) {
    if (p_a->size >= min_size && p_a->size <= max_size) {
      printf("allocation %d 0x%lx %d  from %p -> %p -> %p Description : %s\n", j++, (unsigned long)p_a->addr, p_a->size, p_a->stack[2], p_a->stack[1], p_a->stack[0], p_a->desc);
    }
  }
  pthread_mutex_unlock(&alloc_test_mutex);
}

int sal_alloc_debug_nof_allocs_get(void)
{
    int nof_allocs_lcl;
    enter_mutex();
    nof_allocs_lcl = nof_allocs;
    pthread_mutex_unlock(&alloc_test_mutex);
    return nof_allocs_lcl;
}

void sal_alloc_debug_last_allocs_get(alloc_info *array, int nof_last_allocs)
{
    uint32 i;
    int start = 0;

    enter_mutex();
    if(array) {
        if (nof_allocs > nof_last_allocs) {
            start = nof_allocs - nof_last_allocs;
        }
        for(i = start; i < nof_allocs; ++i) {
            *(array++) = allocs[i];
        }
    }
    pthread_mutex_unlock(&alloc_test_mutex);
}

void
sal_alloc_debug_alloc(uint32 *p, unsigned int sz, const char* alloc_note)
{
    size_t addr = (size_t)(p+2);
    alloc_info *p_a = allocs;
    int i;

    enter_mutex();
    for (i = nof_allocs; i; --i, ++p_a) {
        uint32 *a = (uint32*)(p_a->addr);
        assert(a[-1] == 0xaaaaaaaa && a[-2] == p_a->size / 4  && a[a[-2]] == 0xbbbbbbbb);
        if (p_a->addr < addr + sz && addr < p_a->addr + p_a->size) {
            printf("Allocation 0x%lx %d conflicts with previous allocation 0x%lx %d  at %p -> %p -> %p\n",
                   (unsigned long)addr, sz,
                   (unsigned long)p_a->addr, p_a->size,
                   p_a->stack[2], p_a->stack[1], p_a->stack[0]);
            assert(0);
        }
    }

    if (nof_allocs >= MAX_ALLOCS) {
        printf ("too many allocations, the testing mechanism will not work properly from now\n");
        assert(0);
    } else {
        void *stack[4];
        int frames = backtrace(stack, 4);
        allocs[nof_allocs].addr = addr;
        allocs[nof_allocs].size = sz;
        allocs[nof_allocs].desc = alloc_note;
        if (frames > 1) {
            allocs[nof_allocs].stack[0] = stack[1];
            allocs[nof_allocs].stack[1] = stack[2];
            allocs[nof_allocs].stack[2] = stack[3];
        } else {
            allocs[nof_allocs].stack[0] =
                allocs[nof_allocs].stack[1] =
                allocs[nof_allocs].stack[2] = 0;
        }
        ++nof_allocs;
    }
    pthread_mutex_unlock(&alloc_test_mutex);
}

void
sal_alloc_debug_free(void *addr)
{
    size_t iaddr = (size_t)addr;
    alloc_info *p_a = allocs;
    int i, found = -1;

    enter_mutex();
    for (i = 0; i < nof_allocs; ++i, ++p_a) {
        uint32 *a = (uint32*)(p_a->addr);
        assert(a[-1] == 0xaaaaaaaa && a[-2] == p_a->size / 4  && a[a[-2]] == 0xbbbbbbbb);
        if (iaddr == p_a->addr) {
            /* This is alloc matches the free */
            if (found != -1) {
                printf("When freeing %p found two matching allocs: 0x%lx %d at %p -> %p -> %p and 0x%lx %d at %p -> %p -> %p\n",
                       addr,
                       (unsigned long)allocs[found].addr, allocs[found].size,
                       allocs[found].stack[2], allocs[found].stack[1], allocs[found].stack[0],
                       (unsigned long)p_a->addr, p_a->size,
                       p_a->stack[2], p_a->stack[1], p_a->stack[0]);
                assert(0);
            } else {
                found = i;
            }
        } else if (iaddr >= p_a->addr && iaddr < p_a->addr + p_a->size) {
            /* Found an alloc that the address is inside of */
            printf("When freeing 0x%lxi found an alloc 0x%lx %d at %p -> %p -> %p that conflicts with the free\n",
                   (unsigned long)iaddr, (unsigned long)p_a->addr, p_a->size,
                   p_a->stack[2], p_a->stack[1], p_a->stack[0]);
            assert(0);
        }
    }
    if (found == -1) {
        printf("Did not find a matching allocation of %p\n", addr);
        assert(0);
    } else {
#if AGGRESSIVE_ALLOC_DEBUG_TESTING_KEEP_ORDER
        for(; found + 1 < nof_allocs; ++found) {
            allocs[found] = allocs[found + 1];
        }
#else
        --p_a;
        allocs[found] = *p_a;
#endif
        --nof_allocs;
    }
    pthread_mutex_unlock(&alloc_test_mutex);
}

void
sal_alloc_debug_print_bad_addr(void *addr)
{
    printf("sal_free: BAD FREE %p\n", addr);
}

#endif /* _SAL_ALLOC_DEBUG_H */
