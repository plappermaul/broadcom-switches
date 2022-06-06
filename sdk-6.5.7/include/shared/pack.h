/* 
 * $Id: pack.h,v 1.1.2.4 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        pack.h
 * Purpose:     Pack and unpack macros using Big-Endian (network byte order).
 *
 * Pack and unpack macros:
 *   _SHR_PACK_U8    : Packs a uint8, advances pointer
 *   _SHR_UNPACK_U8  : Unpacks a uint8, advances pointer
 *   _SHR_PACK_U16   : Packs a uint16, advances pointer
 *   _SHR_UNPACK_U16 : Unpacks a uint16, advances pointer
 *   _SHR_PACK_U32   : Packs a uint32, advances pointer
 *   _SHR_UNPACK_U32 : Unpacks a uint32, advances pointer
 *
 * The pack/unpack macros take (_buf, _var)
 * where,
 *     _buf  - is a pointer to buffer where to pack or unpack value;
 *             should be of type 'uint8 *', or 'char *'.
 *     _var  - is the variable with value to pack, or to unpack to.
 * 
 * NOTE:
 * All above macros increment the given buffer pointer based
 * on the corresponding type size.
 *
 * Avoid expressions in these macros (side effects).
 */

#ifndef   _SHR_PACK_H_
#define   _SHR_PACK_H_

/* Type length in bytes */
#define _SHR_PACKLEN_U8     1
#define _SHR_PACKLEN_U16    2
#define _SHR_PACKLEN_U32    4

#define _SHR_PACK_U8(_buf, _var)        \
    *_buf++ = (_var) & 0xff

#define _SHR_UNPACK_U8(_buf, _var)      \
    _var = *_buf++

#define _SHR_PACK_U16(_buf, _var)           \
    do {                                    \
        (_buf)[0] = ((_var) >> 8) & 0xff;   \
        (_buf)[1] = (_var) & 0xff;          \
        (_buf) += _SHR_PACKLEN_U16;         \
    } while (0)

#define _SHR_UNPACK_U16(_buf, _var)         \
    do {                                    \
        (_var) = (((_buf)[0] << 8) |        \
                  (_buf)[1]);               \
        (_buf) += _SHR_PACKLEN_U16;         \
    } while (0)

#define _SHR_PACK_U32(_buf, _var)           \
    do {                                    \
        (_buf)[0] = ((_var) >> 24) & 0xff;  \
        (_buf)[1] = ((_var) >> 16) & 0xff;  \
        (_buf)[2] = ((_var) >> 8) & 0xff;   \
        (_buf)[3] = (_var) & 0xff;          \
        (_buf) += _SHR_PACKLEN_U32;         \
    } while (0)

#define _SHR_UNPACK_U32(_buf, _var)         \
    do {                                    \
        (_var) = (((_buf)[0] << 24) |       \
                  ((_buf)[1] << 16) |       \
                  ((_buf)[2] << 8)  |       \
                  (_buf)[3]);               \
        (_buf) += _SHR_PACKLEN_U32;         \
    } while (0)

#endif /* _SHR_PACK_H_ */
