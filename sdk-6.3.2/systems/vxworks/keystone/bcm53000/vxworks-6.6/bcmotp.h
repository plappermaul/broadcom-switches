/*
 * OTP support.
 *
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
 * $Id: bcmotp.h 1.3 Broadcom SDK $
 */

/* OTP regions */
#define OTP_HW_RGN	1
#define OTP_SW_RGN	2
#define OTP_CI_RGN	4
#define OTP_FUSE_RGN	8

#if !defined(BCMDONGLEHOST)
extern void *otp_init(si_t *sih);
#endif /* !defined(BCMDONGLEHOST) */

extern int otp_status(void *oh);
extern int otp_size(void *oh);

extern int otp_read_region(void *oh, int region, uint16 *data, uint *wlen);
extern uint16 otpr(void *oh, chipcregs_t *cc, uint wn);
extern int otp_nvread(void *oh, char *data, uint *len);

#ifdef BCMNVRAMW
#if !defined(BCMDONGLEHOST)
extern int otp_write_region(void *oh, int region, uint16 *data, uint wlen);
extern int otp_fix_word16(void *oh, uint wn, uint16 mask, uint16 val);
extern int otp_write_rde(void *oh, int rde, uint bit, uint val);
extern int otpw(void *oh, int wn, uint16 data);
extern int otp_nvwrite(void *oh, uint16 *data, uint wlen);
#endif /* !defined(BCMDONGLEHOST) */
#endif /* BCMNVRAMW */

#if defined(WLTEST) || defined (BCMINTERNAL)
extern int otp_dump(void *oh, int arg, char *buf, uint size);
#endif