/*
 * $Id: util.h 1.13.46.1 Broadcom SDK $
 * $Copyright: Copyright 2012 Broadcom Corporation.
 * This program is the proprietary software of Broadcom Corporation
 * and/or its licensors, and may only be used, duplicated, modified
 * or distributed pursuant to the terms and conditions of a separate,
 * written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized
 * License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software
 * and all intellectual property rights therein.  IF YOU HAVE
 * NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 * IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 * ALL USE OF THE SOFTWARE.  
 *  
 * Except as expressly set forth in the Authorized License,
 *  
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 *  
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 * 
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL,
 * INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 * ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 * THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 *
 * General utility routines
 */

#ifndef _SHR_UTIL_H
#define _SHR_UTIL_H

#include <sal/types.h>

extern unsigned short _shr_swap16(unsigned short val);
extern unsigned int _shr_swap32(unsigned int val);

extern int _shr_popcount(unsigned int);

extern uint8 _shr_bit_rev8(uint8 n);
extern uint16 _shr_bit_rev16(uint16 n);
extern uint32 _shr_bit_rev32(uint32 n);
extern uint32 _shr_bit_rev_by_byte_word32(uint32 n);

extern unsigned short _shr_crc16(int start, unsigned char *p, int n);
extern unsigned short _shr_crc16b(int crc, unsigned char *data, int nbits);
extern unsigned short _shr_crc16bd15(int crc, unsigned char *data, int nbits);

#define _SHR_CRC32_CORRECT 0xe320bbde
extern unsigned int _shr_crc32(unsigned int accum, unsigned char *data, 
                               int len);
unsigned int _shr_crc32b(unsigned int crc, unsigned char *data, int nbits);
unsigned int _shr_crc32bd15(unsigned int crc, unsigned char *data, int nbits);

void _shr_sort(void *base, int count, int size,
	       int (*compar)(void *, void *));
int _shr_bsearch(void *base, int count, int size, 
                 void *target, int (*compar)(void *, void *));

void _shr_format_integer(char *buf, unsigned int n, int min_digits, int base);
void _shr_format_long_integer(char *buf, unsigned int *val, int nval);

extern unsigned int _shr_ctoi(const char *s);
extern sal_vaddr_t _shr_ctoa(const char *s);

extern int _shr_div_exp10(int d1, int d2, int exp10);
extern int _shr_atof_exp10(const char *s, int exp10);

extern uint32 _shr_div32r(uint32 d1, uint32 d2);
extern char *_shr_scale_uint64(uint64 d64, int base, int prec, uint32 *d32);

extern unsigned short _shr_ip_chksum(unsigned int length, unsigned char *data);

/* WARNING:  SIDE EFFECTS; AVOID EXPRESSIONS IN THESE MACROS */

/* unsigned char *buf, unsigned short val */
#define _SHR_PACK_SHORT(buf, val) \
    do {                                               \
        (buf)[0] = (val) >> 8;                         \
        (buf)[1] = (val);                              \
    } while (0)

/* unsigned char *buf, unsigned long val */
#define _SHR_PACK_LONG(buf, val) \
    do {                                               \
        (buf)[0] = (val) >> 24;                        \
        (buf)[1] = (val) >> 16;                        \
        (buf)[2] = (val) >> 8;                         \
        (buf)[3] = (val);                              \
    } while (0)

/* unsigned char *buf, unsigned short val */
#define _SHR_UNPACK_SHORT(buf, val) (val) = (((buf)[0] << 8) | (buf)[1])

/* unsigned char *buf, unsigned long val */
#define _SHR_UNPACK_LONG(buf, val) \
    (val) = (((buf)[0] << 24) | ((buf)[1] << 16) | ((buf)[2] << 8) | (buf)[3])

int _shr_parse_macaddr(char *str, uint8 *macaddr);
int _shr_parse_ipaddr(char *s, sal_ip_addr_t *ipaddr);

uint16 _shr_uint16_read(uint8* buffer);
void   _shr_uint16_write(uint8* buffer, const uint16 value);

uint32 _shr_uint32_read(uint8* buffer);
void _shr_uint32_write(uint8* buffer, const uint32 value);

uint64 _shr_uint64_read(uint8* buffer);
void _shr_uint64_write(uint8* buffer, const uint64 value);

int64 _shr_int64_read(uint8* buffer);
void _shr_int64_write(uint8* buffer, const int64 value);

/* To check if particular flag is set */
#define _SHR_IS_FLAG_SET(flags, flag_bit)        (((flags) & (flag_bit)) ? 1 : 0)

#endif	/* !_SHR_UTIL_H */
