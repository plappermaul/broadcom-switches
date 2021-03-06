/*
 * This software is governed by the Broadcom Switch APIs license.
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenMDK/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.1,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 */

#include <board/sdk5690.h>

static unsigned char ledprog_sdk5690[] = {
    0xE0, 0x28, 0x60, 0xC3, 0x67, 0x4E, 0x67, 0x8A, 0x06, 0xC3, 0x80, 0xD2, 0x0C, 0x74, 0x01, 0x28,
    0x60, 0xC3, 0x32, 0x00, 0xD7, 0x87, 0x32, 0x01, 0xD7, 0x87, 0x32, 0x07, 0xD7, 0x87, 0x32, 0x08,
    0xD7, 0x87, 0x32, 0x0F, 0x87, 0x32, 0x0F, 0x87, 0x32, 0x0F, 0x87, 0x32, 0x0F, 0x87, 0x12, 0xC2,
    0x85, 0x05, 0xD2, 0x0F, 0x71, 0x38, 0x52, 0x00, 0x12, 0xC1, 0x85, 0x05, 0xD2, 0x1F, 0x71, 0x42,
    0x52, 0x00, 0x12, 0xC0, 0x85, 0x05, 0xD2, 0x05, 0x71, 0x4C, 0x52, 0x00, 0x3A, 0x38, 0x32, 0x00,
    0x97, 0x75, 0x5A, 0x12, 0xA0, 0xFE, 0xC3, 0x02, 0x0A, 0x50, 0x32, 0x01, 0x97, 0x75, 0x66, 0x12,
    0xAD, 0xFE, 0xC3, 0x02, 0x0A, 0x50, 0x12, 0xAD, 0xFE, 0xC3, 0x95, 0x75, 0x78, 0x85, 0x12, 0xA0,
    0xFE, 0xC3, 0x95, 0x75, 0xC0, 0x85, 0x77, 0xB9, 0x12, 0xA0, 0xFE, 0xC3, 0x95, 0x75, 0x82, 0x85,
    0x77, 0xC7, 0x16, 0xC0, 0xDA, 0x02, 0x71, 0xC7, 0x77, 0xC0, 0x32, 0x05, 0x97, 0x71, 0x9A, 0x32,
    0x02, 0x97, 0x71, 0xB9, 0x06, 0xC1, 0xD2, 0x01, 0x71, 0xB9, 0x06, 0xC3, 0x67, 0xB2, 0x75, 0xB9,
    0x32, 0x03, 0x97, 0x71, 0xC0, 0x32, 0x04, 0x97, 0x75, 0xC7, 0x06, 0xC2, 0xD2, 0x07, 0x71, 0xC7,
    0x77, 0xC0, 0x12, 0x80, 0xF8, 0x15, 0x1A, 0x00, 0x57, 0x32, 0x0E, 0x87, 0x32, 0x0E, 0x87, 0x57,
    0x32, 0x0E, 0x87, 0x32, 0x0F, 0x87, 0x57, 0x32, 0x0F, 0x87, 0x32, 0x0E, 0x87, 0x57
};

void *
sdk5690_ledprog_info(int *size)
{
    if (size) {
        *size = sizeof(ledprog_sdk5690);
    }
    return ledprog_sdk5690;
}
