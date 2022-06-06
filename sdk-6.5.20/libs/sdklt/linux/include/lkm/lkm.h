/*! \file lkm.h
 *
 * <description>
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef LKM_H
#define LKM_H

#include <linux/init.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
#error Kernel too old
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,1,0)
#include <linux/kconfig.h>
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
#include <linux/slab.h>
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39)
#include <linux/smp_lock.h>
#endif
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/fcntl.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/stat.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/uaccess.h>

#include <asm/io.h>
#include <asm/hardirq.h>

#ifdef CONFIG_DEVFS_FS
#include <linux/devfs_fs_kernel.h>
#endif

/* Compatibility Macros */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
#define PROC_CREATE(_entry, _name, _acc, _path, _fops)                  \
    do {                                                                \
        _entry = proc_create(_name, _acc, _path, _fops);                \
    } while (0)

#define PROC_CREATE_DATA(_entry, _name, _acc, _path, _fops, _data)      \
    do {                                                                \
        _entry = proc_create_data(_name, _acc, _path, _fops, _data);    \
    } while (0)

#define PROC_PDE_DATA(_node) PDE_DATA(_node)
#else
#define PROC_CREATE(_entry, _name, _acc, _path, _fops)                  \
    do {                                                                \
        _entry = create_proc_entry(_name, _acc, _path);                 \
        if (_entry) {                                                   \
            _entry->proc_fops = _fops;                                  \
        }                                                               \
    } while (0)

#define PROC_CREATE_DATA(_entry, _name, _acc, _path, _fops, _data)      \
    do {                                                                \
        _entry = create_proc_entry(_name, _acc, _path);                 \
        if (_entry) {                                                   \
            _entry->proc_fops = _fops;                                  \
            _entry->data=_data;                                         \
        }                                                               \
    } while (0)

#define PROC_PDE_DATA(_node) PROC_I(_node)->pde->data
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,15,0)
#define timer_arg(var, context, timer_fieldname) \
    (typeof(var))(context)
#define timer_context_t unsigned long
#else
#define timer_context_t struct timer_list *
#define timer_arg(var, context, timer_fieldname) \
    from_timer(var, context, timer_fieldname)
#endif

#ifndef setup_timer
#define setup_timer(timer, fn, data) \
    timer_setup(timer, fn, 0)
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,6,0)
static inline void page_ref_inc(struct page *page)
{
    atomic_inc(&page->_count);
}

static inline void page_ref_dec(struct page *page)
{
    atomic_dec(&page->_count);
}
#endif

#endif /* LKM_H */
