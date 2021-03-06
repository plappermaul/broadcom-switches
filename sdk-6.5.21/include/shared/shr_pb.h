/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * This file contains APIs for dynamic print buffer support, which is
 * mainly intended for use with the BSL framework.
 *
 * The basic idea is to have a sprintf-like function, which always
 * allocates a buffer sufficiently large to hold the resulting string.
 *
 * Furthermore, subsequent writes to the same buffer will be appended
 * to the existing string instead of overwriting it. This is useful
 * e.g. when a driver wants to build a log message from multiple
 * pieces before sending it to the system log.
 *
 * Example demonstrating how entire lines are sent to printf:
 *
 *     shr_pb_t *pb;
 *     int idx;
 *
 *     pb = shr_pb_create();
 *     for (idx = 0; idx < 23; idx++) {
 *         if (idx > 0 && (idx & 0x7) == 0) {
 *             shr_pb_printf(pb, "\n");
 *             printf("%s", shr_pb_str(pb));
 *             shr_pb_reset(pb);
 *         }
 *         shr_pb_printf(pb, "0x%02x ", idx);
 *     }
 *     shr_pb_printf(pb, "\n");
 *     printf("%s", shr_pb_str(pb));
 *     shr_pb_destroy(pb);
 */

#ifndef _SHR_PB_H_
#define _SHR_PB_H_

#include <shared/bitop.h>

/*! Format checks do not need to be portable */
#ifndef SHR_PB_ATTR
#  if defined(__GNUC__) && !defined(__PEDANTIC__)
#    define SHR_PB_ATTR(_a) __attribute__(_a)
#  else
#    define SHR_PB_ATTR(_a)
#  endif
#endif

/*! By default the print buffer is expanded in chunks of this size. */
#define SHR_PB_CHUNK_SIZE_DEFAULT       512

/*! Minimum chunk size (\ref SHR_PB_CHUNK_SIZE_DEFAULT). */
#define SHR_PB_CHUNK_SIZE_MIN           8

/*! Maximum chunk size (\ref SHR_PB_CHUNK_SIZE_DEFAULT). */
#define SHR_PB_CHUNK_SIZE_MAX           (4 * 1024 * 1024)

/*! Error string to indicate failure from the print buffer. */
#define SHR_PB_ERR_STR                  "<PBERR>"

#ifndef SHR_PB_PRIu32
#define SHR_PB_PRIu32 "u"
#endif

#ifndef SHR_PB_PRIu64
#define SHR_PB_PRIu64 "llu"
#endif

#ifndef SHR_PB_PRIx32
#define SHR_PB_PRIx32 "x"
#endif

#ifndef SHR_PB_PRIx64
#define SHR_PB_PRIx64 "llx"
#endif

/*!
 * \brief Print buffer object.
 */
typedef struct shr_pb_s {

    /*! Allocated buffer holding the current string. */
    char *buf;

    /*! Size of currently allocated buffer. */
    int bufsz;

    /*! Length of the current string. */
    int len;

    /*! Reset marker. */
    int mark;

} shr_pb_t;

/*!
 * \brief Create print buffer object.
 *
 * After use, the object should be destroyed by calling \ref
 * shr_pb_destroy.
 *
 * \return Pointer to print buffer object, or NULL on error.
 */
extern shr_pb_t *
shr_pb_create(void);

/*!
 * \brief Destroy print buffer object.
 *
 * Destroy a print buffer object created using \ref shr_pb_create.
 *
 * \param [in] pb Print buffer object
 */
extern void
shr_pb_destroy(shr_pb_t *pb);

/*!
 * \brief Append formatted string to a print buffer.
 *
 * This function appends a formatted string to the existing print
 * buffer using the standard printf syntax.
 *
 * \param [in] pb Print buffer object
 * \param [in] fmt printf format string
 *
 * \return Number of charaters added, or -1 on error.
 */
extern int
shr_pb_printf(shr_pb_t *pb, const char *fmt, ...)
    SHR_PB_ATTR ((format (printf, 2, 3)));

/*!
 * \brief Set reset marker position in a print buffer.
 *
 * The marker allows the caller to reset the print buffer to a
 * specific length. The feature can be used to recover an earlier
 * state or to remove one or more characters from the tail of the
 * print buffer.
 *
 * If a negative value is passed in, the marker will be set at the end
 * of the current string in the print buffer.
 *
 * \param [in] pb Print buffer object.
 * \param [in] offs New position of reset marker.
 *
 * \return Offset of the current reset marker.
 */
extern int
shr_pb_mark(shr_pb_t *pb, int offs);

/*!
 * \brief Reset print buffer object.
 *
 * Sets the current string length to zero. This function should be
 * called when a new string is created using an existing print buffer.
 *
 * \param [in] pb Print buffer object
 *
 * \return Nothing.
 */
extern void
shr_pb_reset(shr_pb_t *pb);

/*!
 * \brief Replace last character in print buffer.
 *
 * If the print buffer is empty, no changes are made.
 *
 * If the replacement character is zero, then the print buffer will be
 * truncated by one character.
 *
 * \param [in] pb Print buffer object
 * \param [in] ch Replacement character.
 *
 * \return Print buffer length after replacement.
 */
extern int
shr_pb_replace_last(shr_pb_t *pb, char ch);

/*!
 * \brief Get current string in the print buffer.
 *
 * Note that this function will always return a valid string, so no
 * error checking is necessary. In case of an error a special error
 * string will be returned.
 *
 * \param [in] pb Print buffer object
 *
 * \return Pointer to the current string, or "<PBERR>" if error.
 */
extern const char *
shr_pb_str(shr_pb_t *pb);

/*!
 * \brief Set the print buffer chunk size.
 *
 * If the print buffer is too small to hold a string, then it will be
 * expanded in chunks of this size.
 *
 * To get the current chunk size, pass in a value of zero.
 *
 * To reset the chunk size to the default value, pass in -1.
 *
 * \param [in] csize New chunk size.
 *
 * \return Current chunk size.
 */
extern int
shr_pb_chunk_size(int csize);

/*!
 * \brief Format the value in uint32_t array to print buffer.
 *
 * Format the scalar data stored in uint32_t array \c val with
 * array size \c count to a print buffer \c pb.
 * With this format, each 32-bit word in a long scalar data
 * will be separated by an underscore for better readability.
 *
 * If \c bitwidth is non-zero, the format will be left-justified with
 * the leading zeros to match the specified bit width \c bitwidth.
 *
 * \param [out] pb Print buffer object which is already created.
 * \param [in] prefix Prefix string to be added if not NULL.
 * \param [in] val Pointer to the uint32_t array.
 * \param [in] count Array size.
 * \param [in] bitwidth Width in bit to specify the format string length.
 *
 * \return Pointer to the current string, or "<PBERR>" if error.
 */
const char *
shr_pb_format_uint32(shr_pb_t *pb, const char *prefix,
                     const uint32 *val, int count, uint32 bitwidth);

/*!
 * \brief Format the value in uint64_t array to print buffer.
 *
 * Format the scalar data stored in uint64_t array \c val with
 * array size \c count to a print buffer \c pb.
 * With this format, each 32-bit word in a long scalar data
 * will be separated by an underscore for better readability.
 *
 * If \c bitwidth is non-zero, the format will be left-justified with
 * the leading zeros to match the specified bit width \c bitwidth.
 *
 * \param [out] pb Print buffer object which is already created.
 * \param [in] prefix Prefix string to be added if not NULL.
 * \param [in] val Pointer to the uint64_t array.
 * \param [in] count Array size.
 * \param [in] bitwidth Width in bit to specify the format string length.
 *
 * \return Pointer to the current string, or "<PBERR>" if error.
 */
const char *
shr_pb_format_uint64(shr_pb_t *pb, const char *prefix,
                     const uint64 *val, int count, uint32 bitwidth);

/*!
 * \brief Format the bit array to print buffer.
 *
 * Format the bit list stored in SHR_BITDCL array \c bits with
 * array size \c count as a set of ranges into a print buffer \c pb.
 * With this format, each continuous range will be separated by comma.
 *
 * \param [out] pb Print buffer object which is already created.
 * \param [in] bits Pointer to the bit array.
 * \param [in] count Array size.
 *
 * \return Pointer to the current string.
 */
const char *
shr_pb_format_bit_list(shr_pb_t *pb, const SHR_BITDCL *bits, int count);

#endif  /* !_SHR_PB_H_ */
