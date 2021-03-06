/*
 * This software is governed by the Broadcom Switch APIs license.
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenMDK/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.1,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 */

#include <board/sdk56150.h>

static unsigned char ledprog_sdk56150[] = {
    0xe0, 0x28, 0x60, 0x7f, 0x67, 0x2f, 0x67, 0x6b, 0x06, 0x7f, 0x80, 0xd2, 0x1a, 0x74, 0x01, 0x12,
    0x7e, 0x85, 0x05, 0xd2, 0x0f, 0x71, 0x19, 0x52, 0x00, 0x12, 0x7d, 0x85, 0x05, 0xd2, 0x1f, 0x71,
    0x23, 0x52, 0x00, 0x12, 0x7c, 0x85, 0x05, 0xd2, 0x05, 0x71, 0x2d, 0x52, 0x00, 0x3a, 0x68, 0x32,
    0x00, 0x97, 0x75, 0x3b, 0x12, 0xa0, 0xfe, 0x7f, 0x02, 0x0a, 0x50, 0x32, 0x01, 0x97, 0x75, 0x47,
    0x12, 0xba, 0xfe, 0x7f, 0x02, 0x0a, 0x50, 0x12, 0xba, 0xfe, 0x7f, 0x95, 0x75, 0x59, 0x85, 0x12,
    0xa0, 0xfe, 0x7f, 0x95, 0x75, 0xa8, 0x85, 0x77, 0x9a, 0x12, 0xa0, 0xfe, 0x7f, 0x95, 0x75, 0x63,
    0x85, 0x77, 0xa1, 0x16, 0x7c, 0xda, 0x02, 0x71, 0xa1, 0x77, 0xa8, 0x32, 0x05, 0x97, 0x71, 0x76,
    0x06, 0x7d, 0xd2, 0x01, 0x71, 0x9a, 0x06, 0x7f, 0x67, 0x93, 0x75, 0x9a, 0x32, 0x02, 0x97, 0x71,
    0x9a, 0x32, 0x03, 0x97, 0x71, 0xa8, 0x32, 0x04, 0x97, 0x75, 0xa1, 0x06, 0x7e, 0xd2, 0x07, 0x71,
    0xa1, 0x77, 0xa8, 0x12, 0x80, 0xf8, 0x15, 0x1a, 0x00, 0x57, 0x32, 0x0e, 0x87, 0x32, 0x0e, 0x87,
    0x57, 0x32, 0x0e, 0x87, 0x32, 0x0f, 0x87, 0x57, 0x32, 0x0f, 0x87, 0x32, 0x0e, 0x87, 0x57
};

void *
sdk56150_ledprog_info(int *size)
{
    if (size) {
        *size = sizeof(ledprog_sdk56150);
    }
    return ledprog_sdk56150;
}
