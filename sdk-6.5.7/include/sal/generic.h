/*
 * $Id: generic.h,v 1.2 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        generic.h
 * Purpose:     Generic defines, based only on C types.
 */

#ifndef   _SAL_GENERIC_H_
#define   _SAL_GENERIC_H_

/*
 * Create a mask given the width and shift.
 * The width must be <= 32.
 * The mask is "in place", that is, is shifted accordingly.
 */
#define SAL_MASK_MAKE(width, shift)  \
    ((((1 << (width)) - 1) | (1 << (width - 1))) << (shift))

/*
 * Extract a field given the value, mask and shift.
 * The mask is assumed to be "in place" -- already shifted.
 * That is, it is from SAL_MASK_MAKE.
 */
#define SAL_MASK_VAL_GET(val, mask, shift) (((val) & (mask)) >> (shift))

/*
 * Set a field given the current value (val),
 * field value (fval) mask and shift.
 * The mask is assumed to be "in place" -- already shifted.
 * That is, it is from SAL_MASK_MAKE.
 */
#define SAL_MASK_VAL_SET(val, fval, mask, shift) \
        do { \
            (val) &= (~(mask)); \
            (val) |= (((fval) << (shift)) & (mask)); \
        } while (0)

#endif /* _SAL_GENERIC_H_ */
