/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved. 
 * File:       phy8806x_firmware.c
 * Purpose:    Firmware for 8806X PHYs
 */

#if defined(INCLUDE_PHY_8806X)
#include "phy8806x.h"

/* test array used by DV */

uint8 bcm_8806x_firmware[] = {
0x4D ,0x41 ,0x47 ,0x49 ,0x43 ,0x00 ,0x13 ,0x80,
0x00 ,0x0F ,0x00 ,0x0F ,0x47 ,0x4D ,0x73 ,0x6A,
0xEA ,0x00 ,0x00 ,0x23 ,0xEA ,0x00 ,0x00 ,0x06,
0xEA ,0x00 ,0x00 ,0x2B ,0xEA ,0x00 ,0x00 ,0x27,
0xEA ,0x00 ,0x00 ,0x33 ,0xEA ,0x00 ,0x00 ,0x34,
0xEA ,0x00 ,0x04 ,0xA0 ,0xEA ,0x00 ,0x04 ,0xB1,
0xE3 ,0xA0 ,0x10 ,0x00 ,0xE3 ,0xA0 ,0x00 ,0x00,
0xE3 ,0xA0 ,0x30 ,0x00 ,0xE3 ,0xA0 ,0x20 ,0x00,
0xE3 ,0xA0 ,0x50 ,0x00 ,0xE3 ,0xA0 ,0x40 ,0x00,
0xE3 ,0xA0 ,0x70 ,0x00 ,0xE3 ,0xA0 ,0x60 ,0x00,
0xE3 ,0xA0 ,0x90 ,0x00 ,0xE3 ,0xA0 ,0x80 ,0x00,
0xE3 ,0xA0 ,0xB0 ,0x01 ,0xE5 ,0x9F ,0xA0 ,0xEC,
0xE3 ,0xA0 ,0xA0 ,0x00 ,0xE5 ,0x8A ,0xB0 ,0x00,
0xE3 ,0xA0 ,0xC0 ,0x00 ,0xE3 ,0xA0 ,0xB0 ,0x00,
0xE5 ,0x9F ,0x00 ,0xC8 ,0xE3 ,0xA0 ,0xC0 ,0x00,
0xE3 ,0x21 ,0xF0 ,0xD1 ,0xE1 ,0xA0 ,0xD0 ,0x00,
0xE3 ,0x21 ,0xF0 ,0xD2 ,0xE2 ,0x40 ,0xDA ,0x09,
0xE3 ,0x21 ,0xF0 ,0xD7 ,0xE2 ,0x40 ,0xDC ,0x91,
0xE3 ,0x21 ,0xF0 ,0xDB ,0xE2 ,0x40 ,0xDC ,0x92,
0xE3 ,0x21 ,0xF0 ,0x13 ,0xE2 ,0x40 ,0xDC ,0x93,
0xE1 ,0xA0 ,0xF0 ,0x0C ,0xE5 ,0x9F ,0xC0 ,0xA8,
0xE5 ,0x9F ,0xA0 ,0x98 ,0xE5 ,0x2D ,0xE0 ,0x04,
0xE5 ,0x8A ,0xB0 ,0x00 ,0xE3 ,0xA0 ,0xB0 ,0x01,
0xE5 ,0x2D ,0xE0 ,0x04 ,0xE4 ,0x9D ,0xF0 ,0x04,
0xE3 ,0xA0 ,0xB0 ,0x02 ,0xE5 ,0x9F ,0xA0 ,0x84,
0xE4 ,0x9D ,0xF0 ,0x04 ,0xE5 ,0x8A ,0xB0 ,0x00,
0xEE ,0x15 ,0xBF ,0x30 ,0xE5 ,0x9F ,0xA0 ,0x7C,
0xE5 ,0x9F ,0xA0 ,0x74 ,0xE5 ,0x8A ,0xB0 ,0x00,
0xE5 ,0x8A ,0xB0 ,0x00 ,0xEE ,0x16 ,0xBF ,0x50,
0xE5 ,0x9F ,0xB0 ,0x6C ,0xE5 ,0x9F ,0xA0 ,0x6C,
0xE2 ,0x5E ,0xF0 ,0x08 ,0xE5 ,0x8A ,0xB0 ,0x00,
0xE5 ,0x9F ,0x90 ,0x60 ,0xE5 ,0x9F ,0xA0 ,0x5C,
0xE5 ,0x9F ,0xA0 ,0x48 ,0xE5 ,0x8A ,0x90 ,0x00,
0xE5 ,0x8A ,0xB0 ,0x00 ,0xEE ,0x15 ,0xBF ,0x10,
0xEE ,0x16 ,0xBF ,0x10 ,0xE5 ,0x9F ,0xA0 ,0x50,
0xE5 ,0x9F ,0xA0 ,0x34 ,0xE5 ,0x8A ,0xB0 ,0x00,
0xE5 ,0x8A ,0xB0 ,0x00 ,0xE3 ,0xA0 ,0xB0 ,0x04,
0xE5 ,0x9A ,0xC0 ,0x00 ,0xE5 ,0x9F ,0xA0 ,0x2C,
0xE1 ,0x5B ,0x00 ,0x0C ,0xE3 ,0xA0 ,0xB0 ,0x00,
0xE2 ,0x5E ,0xF0 ,0x08 ,0x1A ,0xFF ,0xFF ,0xFA,
0x00 ,0x01 ,0xF0 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
0x00 ,0x01 ,0xF0 ,0x14 ,0x00 ,0x00 ,0x90 ,0x00,
0x00 ,0x01 ,0xF2 ,0x84 ,0x00 ,0x00 ,0x01 ,0x5C,
0x00 ,0x01 ,0xF2 ,0x80 ,0x00 ,0x01 ,0xF2 ,0x8C,
0x00 ,0x00 ,0xDE ,0xAD ,0x00 ,0x03 ,0xDE ,0xAD,
0xEB ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0xF2 ,0x88,
0xE2 ,0x8F ,0x00 ,0x2C ,0xEB ,0x00 ,0x00 ,0x1F,
0xE0 ,0x8A ,0xA0 ,0x00 ,0xE8 ,0x90 ,0x0C ,0x00,
0xE2 ,0x4A ,0x70 ,0x01 ,0xE0 ,0x8B ,0xB0 ,0x00,
0x1A ,0x00 ,0x00 ,0x00 ,0xE1 ,0x5A ,0x00 ,0x0B,
0xE8 ,0xBA ,0x00 ,0x0F ,0xEB ,0x00 ,0x00 ,0x17,
0xE3 ,0x13 ,0x00 ,0x01 ,0xE2 ,0x4F ,0xE0 ,0x18,
0xE1 ,0x2F ,0xFF ,0x13 ,0x10 ,0x47 ,0xF0 ,0x03,
0x00 ,0x00 ,0x11 ,0xB0 ,0x00 ,0x00 ,0x11 ,0xA0,
0x28 ,0xB0 ,0x00 ,0x78 ,0xE2 ,0x52 ,0x20 ,0x10,
0x8A ,0xFF ,0xFF ,0xFB ,0x28 ,0xA1 ,0x00 ,0x78,
0x28 ,0xB0 ,0x00 ,0x30 ,0xE1 ,0xB0 ,0x2E ,0x82,
0x45 ,0x90 ,0x40 ,0x00 ,0x28 ,0xA1 ,0x00 ,0x30,
0xE1 ,0x2F ,0xFF ,0x1E ,0x45 ,0x81 ,0x40 ,0x00,
0x0A ,0x00 ,0x00 ,0xDA ,0xE3 ,0x51 ,0x00 ,0x78,
0xE1 ,0x2F ,0xFF ,0x1E ,0xE3 ,0xB0 ,0x00 ,0x00,
0xBD ,0x1F ,0xE8 ,0x98 ,0xF0 ,0x01 ,0xB5 ,0x1F,
0xE5 ,0x9F ,0xD0 ,0x28 ,0xBD ,0x10 ,0xB5 ,0x10,
0xE5 ,0x9F ,0xC0 ,0x1C ,0xFA ,0xFF ,0xFF ,0xFA,
0xE3 ,0x1C ,0x00 ,0x01 ,0xE0 ,0x8C ,0xC0 ,0x0F,
0x01 ,0xA0 ,0xE0 ,0x0F ,0x12 ,0x8F ,0xE0 ,0x0D,
0xE2 ,0x8F ,0xC0 ,0x01 ,0xE1 ,0x2F ,0xFF ,0x1C,
0xFA ,0x92 ,0xF0 ,0x00 ,0xE1 ,0x2F ,0xFF ,0x1C,
0x00 ,0x00 ,0xD0 ,0x00 ,0x00 ,0x00 ,0x10 ,0x9C,
0xEB ,0x00 ,0x01 ,0x48 ,0xFA ,0xFF ,0xFF ,0xF0,
0xEE ,0x10 ,0x0F ,0xB0 ,0xE9 ,0x2D ,0x1F ,0xFC,
0xE2 ,0x00 ,0x00 ,0xFF ,0xE1 ,0xA0 ,0x04 ,0x20,
0xE1 ,0x2F ,0xFF ,0x1E ,0xE8 ,0xBD ,0x1F ,0xFC,
0xEE ,0x11 ,0x0F ,0x10 ,0xE9 ,0x2D ,0x00 ,0x03,
0xE0 ,0x00 ,0x00 ,0x01 ,0xE3 ,0xE0 ,0x1A ,0x01,
0xF5 ,0x7F ,0xF0 ,0x6F ,0xEE ,0x01 ,0x0F ,0x10,
0xE1 ,0x2F ,0xFF ,0x1E ,0xE8 ,0xBD ,0x00 ,0x03,
0xEE ,0x11 ,0x0F ,0x10 ,0xE9 ,0x2D ,0x00 ,0x03,
0xE0 ,0x00 ,0x00 ,0x01 ,0xE3 ,0xE0 ,0x10 ,0x04,
0xE8 ,0xBD ,0x00 ,0x03 ,0xEE ,0x01 ,0x0F ,0x10,
0xE9 ,0x2D ,0x00 ,0x03 ,0xE1 ,0x2F ,0xFF ,0x1E,
0xEE ,0x08 ,0x0F ,0x17 ,0xE3 ,0xA0 ,0x10 ,0x00,
0xEE ,0x11 ,0x0F ,0x10 ,0xF5 ,0x7F ,0xF0 ,0x4F,
0xE1 ,0x80 ,0x00 ,0x01 ,0xE3 ,0xA0 ,0x10 ,0x01,
0xF5 ,0x7F ,0xF0 ,0x6F ,0xEE ,0x01 ,0x0F ,0x10,
0xE1 ,0x2F ,0xFF ,0x1E ,0xE8 ,0xBD ,0x00 ,0x03,
0xE3 ,0xA0 ,0x00 ,0x00 ,0xE5 ,0x2D ,0x00 ,0x04,
0xF5 ,0x7F ,0xF0 ,0x6F ,0xEE ,0x07 ,0x1F ,0x15,
0xF5 ,0x7F ,0xF0 ,0x4F ,0xEE ,0x07 ,0x0F ,0xD5,
0xEE ,0x11 ,0x0F ,0x10 ,0xF5 ,0x7F ,0xF0 ,0x6F,
0xEE ,0x01 ,0x0F ,0x10 ,0xE3 ,0x80 ,0x0A ,0x03,
0xE1 ,0x2F ,0xFF ,0x1E ,0xE4 ,0x9D ,0x00 ,0x04,
0xE3 ,0xA0 ,0x00 ,0x00 ,0xE9 ,0x2D ,0x00 ,0x1F,
0xF5 ,0x7F ,0xF0 ,0x6F ,0xEE ,0x40 ,0x0F ,0x10,
0xE3 ,0x00 ,0x41 ,0xFF ,0xEE ,0x30 ,0x0F ,0x10,
0xE2 ,0x84 ,0x40 ,0x01 ,0xE0 ,0x04 ,0x41 ,0xA0,
0xE0 ,0x03 ,0x06 ,0xA0 ,0xE3 ,0x07 ,0x3F ,0xFF,
0xE3 ,0xA0 ,0x10 ,0x00 ,0xE2 ,0x80 ,0x00 ,0x01,
0xE1 ,0xA0 ,0x2F ,0x01 ,0xE3 ,0xA0 ,0x30 ,0x00,
0xEE ,0x07 ,0x2F ,0x56 ,0xE1 ,0x82 ,0x22 ,0x83,
0xE1 ,0x50 ,0x00 ,0x03 ,0xE2 ,0x83 ,0x30 ,0x01,
0xE2 ,0x81 ,0x10 ,0x01 ,0x1A ,0xFF ,0xFF ,0xF9,
0x1A ,0xFF ,0xFF ,0xF5 ,0xE1 ,0x51 ,0x00 ,0x04,
0xEE ,0x11 ,0x0F ,0x10 ,0xF5 ,0x7F ,0xF0 ,0x4F,
0xEE ,0x01 ,0x0F ,0x10 ,0xE3 ,0x80 ,0x00 ,0x04,
0xE8 ,0xBD ,0x00 ,0x1F ,0xF5 ,0x7F ,0xF0 ,0x6F,
0xE5 ,0x9D ,0x40 ,0x00 ,0xE1 ,0x2F ,0xFF ,0x1E,
0xE3 ,0xA0 ,0x70 ,0x0F ,0xE5 ,0x2D ,0xE0 ,0x04,
0xEE ,0x06 ,0x1F ,0x12 ,0xE0 ,0x01 ,0x10 ,0x07,
0xE0 ,0x00 ,0x00 ,0x07 ,0xE3 ,0xE0 ,0x70 ,0x1F,
0xE3 ,0xA0 ,0x70 ,0x01 ,0xEE ,0x06 ,0x0F ,0x11,
0xE3 ,0xA0 ,0x70 ,0x3F ,0xE0 ,0x02 ,0x20 ,0x07,
0xE3 ,0xA0 ,0x0C ,0x03 ,0xE0 ,0x04 ,0x40 ,0x07,
0xE1 ,0x80 ,0x00 ,0x04 ,0xE1 ,0x80 ,0x06 ,0x02,
0xE3 ,0xA0 ,0x00 ,0x00 ,0xEE ,0x06 ,0x0F ,0x91,
0xE3 ,0x80 ,0x00 ,0x01 ,0xE1 ,0x80 ,0x00 ,0x83,
0xE4 ,0x9D ,0xF0 ,0x04 ,0xEE ,0x06 ,0x0F ,0x51,
0xEE ,0x11 ,0x1F ,0x10 ,0xE5 ,0x2D ,0xE0 ,0x04,
0xE1 ,0x81 ,0x10 ,0x00 ,0xE3 ,0xA0 ,0x00 ,0x01,
0xF5 ,0x7F ,0xF0 ,0x6F ,0xEE ,0x01 ,0x1F ,0x10,
0xE5 ,0x2D ,0xE0 ,0x04 ,0xE4 ,0x9D ,0xF0 ,0x04,
0xE3 ,0xC1 ,0x10 ,0x01 ,0xEE ,0x11 ,0x1F ,0x10,
0xEE ,0x01 ,0x1F ,0x10 ,0xF5 ,0x7F ,0xF0 ,0x4F,
0xE4 ,0x9D ,0xF0 ,0x04 ,0xF5 ,0x7F ,0xF0 ,0x6F,
0xE5 ,0x2D ,0x80 ,0x04 ,0xE5 ,0x2D ,0xE0 ,0x04,
0xEE ,0x07 ,0x8F ,0xBA ,0xE3 ,0xA0 ,0x80 ,0x00,
0xE4 ,0x9D ,0xF0 ,0x04 ,0xE4 ,0x9D ,0x80 ,0x04,
0xEE ,0x1F ,0x1F ,0x12 ,0xE3 ,0xA0 ,0x2C ,0xF1,
0xE3 ,0x0F ,0x21 ,0x04 ,0xE5 ,0x82 ,0x10 ,0x00,
0xE5 ,0x82 ,0x10 ,0x00 ,0xEE ,0x19 ,0x1F ,0x11,
0xE3 ,0x21 ,0xF0 ,0x1F ,0xE5 ,0x2D ,0xE0 ,0x04,
0xE3 ,0xA0 ,0x14 ,0x01 ,0xEE ,0x11 ,0x0F ,0x10,
0xEE ,0x01 ,0x0F ,0x10 ,0xE1 ,0x80 ,0x00 ,0x01,
0xE4 ,0x9D ,0xF0 ,0x04 ,0xE3 ,0x21 ,0xF0 ,0x13,
0xE3 ,0xA0 ,0xA1 ,0x02 ,0xE5 ,0x2D ,0xE0 ,0x04,
0xEE ,0x09 ,0xAF ,0x5C ,0xEE ,0x09 ,0xAF ,0x5E,
0xEE ,0x19 ,0xAF ,0x1D ,0xEE ,0x0F ,0xAF ,0xB1,
0xE5 ,0x9F ,0xB0 ,0x10 ,0xE3 ,0x0F ,0x90 ,0xF0,
0xE3 ,0xA0 ,0x9A ,0x0F ,0xE5 ,0x89 ,0xB0 ,0x00,
0xE5 ,0x89 ,0xB0 ,0x00 ,0xE5 ,0x9F ,0xB0 ,0x08,
0x00 ,0x01 ,0xAC ,0xED ,0xE4 ,0x9D ,0xF0 ,0x04,
0xE5 ,0x2D ,0xE0 ,0x04 ,0xAC ,0xED ,0xAC ,0xED,
0xEE ,0x09 ,0xAF ,0x1D ,0xE5 ,0x9F ,0xA0 ,0x24,
0xEE ,0x09 ,0x9F ,0x3E ,0xE3 ,0xA0 ,0x91 ,0x02,
0xEE ,0x0F ,0x9F ,0x11 ,0xEE ,0x09 ,0x9F ,0x3C,
0xEE ,0x19 ,0x7F ,0x1C ,0xE3 ,0xA0 ,0x80 ,0x01,
0xEE ,0x09 ,0x7F ,0x1C ,0xE1 ,0x87 ,0x70 ,0x08,
0xFF ,0xFF ,0xF8 ,0x00 ,0xE4 ,0x9D ,0xF0 ,0x04,
0xE5 ,0x1F ,0xA0 ,0x10 ,0xE5 ,0x2D ,0xE0 ,0x04,
0xE3 ,0xA0 ,0x91 ,0x02 ,0xEE ,0x09 ,0xAF ,0x1D,
0xEE ,0x09 ,0x9F ,0x3C ,0xEE ,0x09 ,0x9F ,0x3E,
0xE3 ,0xA0 ,0x80 ,0x01 ,0xEE ,0x0F ,0x9F ,0x31,
0xE1 ,0x87 ,0x70 ,0x08 ,0xEE ,0x19 ,0x7F ,0x1C,
0xE4 ,0x9D ,0xF0 ,0x04 ,0xEE ,0x09 ,0x7F ,0x1C,
0xE5 ,0x1F ,0xA0 ,0x40 ,0xE5 ,0x2D ,0xE0 ,0x04,
0xE3 ,0xA0 ,0x91 ,0x02 ,0xEE ,0x09 ,0xAF ,0x1D,
0xEE ,0x09 ,0x9F ,0x3C ,0xEE ,0x09 ,0x9F ,0x3E,
0xEE ,0x19 ,0x7F ,0x1C ,0xE3 ,0xA0 ,0x80 ,0x01,
0xEE ,0x09 ,0x7F ,0x1C ,0xE1 ,0x87 ,0x70 ,0x08,
0xE5 ,0x2D ,0xE0 ,0x04 ,0xE4 ,0x9D ,0xF0 ,0x04,
0xE4 ,0x9D ,0xF0 ,0x04 ,0xEE ,0x19 ,0xAF ,0x1D,
0xE3 ,0xA0 ,0xA1 ,0x02 ,0xE5 ,0x2D ,0xE0 ,0x04,
0xEE ,0x09 ,0xAF ,0x5C ,0xEE ,0x09 ,0xAF ,0x5E,
0xE4 ,0x9D ,0xF0 ,0x04 ,0xEE ,0x0F ,0xAF ,0x91,
0xE3 ,0xA0 ,0xA1 ,0x02 ,0xE5 ,0x2D ,0xE0 ,0x04,
0xEE ,0x09 ,0xAF ,0x5C ,0xEE ,0x09 ,0xAF ,0x5E,
0xE4 ,0x9D ,0xF0 ,0x04 ,0xEE ,0x0F ,0xAF ,0xB1,
0xE3 ,0xA0 ,0xA1 ,0x02 ,0xE5 ,0x2D ,0xE0 ,0x04,
0xEE ,0x09 ,0xAF ,0x5C ,0xEE ,0x09 ,0xAF ,0x5E,
0xE2 ,0x8F ,0xC0 ,0x01 ,0xE4 ,0x9D ,0xF0 ,0x04,
0x00 ,0x04 ,0xB5 ,0x70 ,0xE1 ,0x2F ,0xFF ,0x1C,
0xF3 ,0xAF ,0x00 ,0x21 ,0x68 ,0x10 ,0x00 ,0x0D,
0xD5 ,0x02 ,0x05 ,0x09 ,0x68 ,0x21 ,0x80 ,0x00,
0x4A ,0x0E ,0xE0 ,0x02 ,0x44 ,0x7A ,0x4A ,0x0F,
0x00 ,0x23 ,0x21 ,0x00 ,0x32 ,0x0E ,0x44 ,0x7A,
0x0F ,0x36 ,0x07 ,0x06 ,0xE0 ,0x05 ,0x33 ,0x24,
0x09 ,0x00 ,0x1C ,0x49 ,0x54 ,0x5E ,0x5D ,0x96,
0x23 ,0x00 ,0x68 ,0x20 ,0xD1 ,0xF7 ,0x28 ,0x00,
0xD0 ,0x03 ,0x2D ,0x70 ,0xD5 ,0x05 ,0x07 ,0x00,
0x23 ,0x02 ,0x32 ,0x11 ,0xD0 ,0x01 ,0x29 ,0x00,
0xBD ,0x70 ,0xF8 ,0x47 ,0xF0 ,0x00 ,0x00 ,0x20,
0xE3 ,0x52 ,0x00 ,0x03 ,0x00 ,0x00 ,0x0D ,0xB2,
0xE2 ,0x10 ,0xC0 ,0x03 ,0x9A ,0x00 ,0x00 ,0x17,
0xE4 ,0xD1 ,0x30 ,0x01 ,0x0A ,0x00 ,0x00 ,0x08,
0xE0 ,0x82 ,0x20 ,0x0C ,0xE3 ,0x5C ,0x00 ,0x02,
0xE4 ,0xC0 ,0x30 ,0x01 ,0x94 ,0xD1 ,0xC0 ,0x01,
0x94 ,0xC0 ,0xC0 ,0x01 ,0x34 ,0xD1 ,0x30 ,0x01,
0x34 ,0xC0 ,0x30 ,0x01 ,0xE2 ,0x42 ,0x20 ,0x04,
0x0A ,0x00 ,0x00 ,0x3D ,0xE2 ,0x11 ,0x30 ,0x03,
0x3A ,0x00 ,0x00 ,0x04 ,0xE2 ,0x52 ,0x20 ,0x08,
0xE4 ,0x91 ,0xC0 ,0x04 ,0xE4 ,0x91 ,0x30 ,0x04,
0xE4 ,0x80 ,0xC0 ,0x04 ,0xE4 ,0x80 ,0x30 ,0x04,
0xE2 ,0x92 ,0x20 ,0x04 ,0xEA ,0xFF ,0xFF ,0xF8,
0x54 ,0x80 ,0x30 ,0x04 ,0x54 ,0x91 ,0x30 ,0x04,
0xE1 ,0xB0 ,0x2F ,0x82 ,0xE1 ,0xA0 ,0x00 ,0x00,
0x24 ,0xD1 ,0xC0 ,0x01 ,0x24 ,0xD1 ,0x30 ,0x01,
0x24 ,0xC0 ,0x30 ,0x01 ,0x44 ,0xD1 ,0x20 ,0x01,
0x44 ,0xC0 ,0x20 ,0x01 ,0x24 ,0xC0 ,0xC0 ,0x01,
0x00 ,0x04 ,0xB5 ,0xFF ,0xE1 ,0x2F ,0xFF ,0x1E,
0x90 ,0x00 ,0x00 ,0x0D ,0x30 ,0x24 ,0xB0 ,0x81,
0x22 ,0x10 ,0xD5 ,0x04 ,0x06 ,0x88 ,0x68 ,0x21,
0xE0 ,0x00 ,0x60 ,0x21 ,0x43 ,0x91 ,0x69 ,0xE0,
0xDD ,0x00 ,0x42 ,0xA8 ,0x27 ,0x00 ,0x20 ,0x01,
0x19 ,0x7A ,0x69 ,0xA1 ,0x98 ,0x04 ,0x1B ,0x47,
0x68 ,0x20 ,0x61 ,0xA0 ,0x1A ,0x08 ,0x18 ,0x10,
0xF3 ,0xAF ,0x00 ,0x20 ,0xD4 ,0x02 ,0x06 ,0xC0,
0x98 ,0x03 ,0xE0 ,0x08 ,0x26 ,0x00 ,0x80 ,0x00,
0x47 ,0x90 ,0x5D ,0x80 ,0x68 ,0xA1 ,0x68 ,0x62,
0x62 ,0x20 ,0x1C ,0x76 ,0x1C ,0x40 ,0x6A ,0x20,
0x68 ,0x20 ,0xDB ,0xF3 ,0x42 ,0x86 ,0x98 ,0x04,
0xF3 ,0xAF ,0x00 ,0x20 ,0xD5 ,0x0A ,0x06 ,0xC0,
0x68 ,0xA1 ,0x68 ,0x62 ,0xE0 ,0x06 ,0x80 ,0x00,
0x1C ,0x40 ,0x6A ,0x20 ,0x47 ,0x90 ,0x20 ,0x30,
0x28 ,0x00 ,0x1E ,0x7F ,0x00 ,0x38 ,0x62 ,0x20,
0x68 ,0x62 ,0x98 ,0x00 ,0xE0 ,0x07 ,0xDC ,0xF4,
0x6A ,0x20 ,0x47 ,0x90 ,0x5D ,0x40 ,0x68 ,0xA1,
0x1E ,0x6D ,0x00 ,0x28 ,0x62 ,0x20 ,0x1C ,0x40,
0xF3 ,0xAF ,0x00 ,0x20 ,0xDC ,0xF3 ,0x28 ,0x00,
0x06 ,0x09 ,0x20 ,0x02 ,0x68 ,0x21 ,0x80 ,0x00,
0xBD ,0xF0 ,0xB0 ,0x05 ,0x20 ,0x01 ,0xD4 ,0x00,
0xE2 ,0x52 ,0x20 ,0x20 ,0xE9 ,0x2D ,0x40 ,0x10,
0xE8 ,0xB1 ,0x50 ,0x18 ,0x3A ,0x00 ,0x00 ,0x05,
0xE8 ,0xA0 ,0x50 ,0x18 ,0xE2 ,0x52 ,0x20 ,0x20,
0xE8 ,0xA0 ,0x50 ,0x18 ,0xE8 ,0xB1 ,0x50 ,0x18,
0xE1 ,0xB0 ,0xCE ,0x02 ,0x2A ,0xFF ,0xFF ,0xF9,
0x28 ,0xA0 ,0x50 ,0x18 ,0x28 ,0xB1 ,0x50 ,0x18,
0x48 ,0xA0 ,0x00 ,0x18 ,0x48 ,0xB1 ,0x00 ,0x18,
0xE1 ,0xB0 ,0xCF ,0x02 ,0xE8 ,0xBD ,0x40 ,0x10,
0x24 ,0x80 ,0x30 ,0x04 ,0x24 ,0x91 ,0x30 ,0x04,
0xE1 ,0xB0 ,0x2F ,0x82 ,0x01 ,0x2F ,0xFF ,0x1E,
0x44 ,0xD1 ,0x20 ,0x01 ,0x20 ,0xD1 ,0x30 ,0xB2,
0x44 ,0xC0 ,0x20 ,0x01 ,0x20 ,0xC0 ,0x30 ,0xB2,
0xF3 ,0xAF ,0x00 ,0x04 ,0xE1 ,0x2F ,0xFF ,0x1E,
0xED ,0x6C ,0xF7 ,0xFF ,0x00 ,0x20 ,0x80 ,0x00,
0xE5 ,0x9F ,0x10 ,0x0C ,0xE1 ,0x2F ,0xFF ,0x1E,
0xEF ,0x12 ,0x34 ,0x56 ,0xE3 ,0xA0 ,0x00 ,0x18,
0xFF ,0xFF ,0xFF ,0xEC ,0xE1 ,0x2F ,0xFF ,0x1E,
0xE5 ,0x9F ,0x00 ,0x0C ,0x00 ,0x02 ,0x00 ,0x26,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE3 ,0x0F ,0x10 ,0x00,
0xEA ,0xFF ,0xFF ,0xFE ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE3 ,0xA0 ,0x00 ,0x00 ,0xAC ,0xED ,0xAC ,0xED,
0xE5 ,0x81 ,0x00 ,0x08 ,0xE3 ,0x0F ,0x10 ,0x00,
0xE1 ,0x2F ,0xFF ,0x1E ,0xE5 ,0x81 ,0x00 ,0x0C,
0xEB ,0xFF ,0xFE ,0xA3 ,0xE9 ,0x2D ,0x47 ,0xF0,
0xE3 ,0xA0 ,0x00 ,0x01 ,0xE1 ,0xA0 ,0x90 ,0x00,
0xE3 ,0xA0 ,0x00 ,0x01 ,0xEB ,0xFF ,0xFF ,0xF5,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x16 ,0x8C,
0xE3 ,0xA0 ,0x70 ,0x01 ,0xE3 ,0xA0 ,0x80 ,0x01,
0xE3 ,0xA0 ,0x50 ,0x00 ,0xE3 ,0xA0 ,0x40 ,0x00,
0xE2 ,0x81 ,0x10 ,0x04 ,0xE3 ,0xA0 ,0x60 ,0x00,
0xEA ,0x00 ,0x01 ,0x95 ,0xE5 ,0x81 ,0x00 ,0x00,
0x1A ,0x00 ,0x01 ,0x4F ,0xE3 ,0x58 ,0x00 ,0x01,
0xE5 ,0x9F ,0x16 ,0x5C ,0xE3 ,0xA0 ,0x00 ,0x00,
0xEA ,0x00 ,0x01 ,0x45 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x9F ,0x16 ,0x50 ,0xE3 ,0xA0 ,0x00 ,0x00,
0xEA ,0x00 ,0x00 ,0x1B ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x06 ,0x44,
0xE5 ,0x9F ,0x16 ,0x3C ,0xE2 ,0x80 ,0x08 ,0x01,
0xE2 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x9F ,0x16 ,0x30 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE0 ,0x80 ,0x00 ,0x09 ,0xE0 ,0x80 ,0x00 ,0x01,
0xE5 ,0x91 ,0x10 ,0x00 ,0xE5 ,0x9F ,0x16 ,0x14,
0xE5 ,0x9F ,0x16 ,0x1C ,0xE0 ,0x80 ,0x00 ,0x01,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x00 ,0x00,
0xE5 ,0x91 ,0x10 ,0x00 ,0xE2 ,0x81 ,0x10 ,0x14,
0xE5 ,0x9F ,0x05 ,0xE8 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE2 ,0x80 ,0x00 ,0x04 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x15 ,0xDC,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x05 ,0xD4,
0x3A ,0xFF ,0xFF ,0xDF ,0xE3 ,0x50 ,0x08 ,0x01,
0xE5 ,0x9F ,0x15 ,0xC0 ,0xE3 ,0xA0 ,0x00 ,0x00,
0xEA ,0x00 ,0x00 ,0x1F ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x05 ,0xB4,
0xE5 ,0x9F ,0x15 ,0xAC ,0xE2 ,0x80 ,0x08 ,0x01,
0xE2 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x9F ,0x15 ,0xA0 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE0 ,0x80 ,0x00 ,0x09 ,0xE0 ,0x80 ,0x00 ,0x01,
0xE5 ,0x91 ,0x10 ,0x00 ,0xE5 ,0x9F ,0x15 ,0x84,
0xE5 ,0x9F ,0x15 ,0x90 ,0xE0 ,0x80 ,0x00 ,0x01,
0xE2 ,0x81 ,0x00 ,0x10 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x10 ,0x04,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x00 ,0x00,
0xE5 ,0x91 ,0x10 ,0x00 ,0xE2 ,0x41 ,0x10 ,0x04,
0x0A ,0x00 ,0x00 ,0x00 ,0xE1 ,0x50 ,0x00 ,0x01,
0xE5 ,0x9F ,0x05 ,0x48 ,0xE2 ,0x84 ,0x40 ,0x01,
0xE2 ,0x80 ,0x00 ,0x04 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x15 ,0x3C,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x05 ,0x34,
0x3A ,0xFF ,0xFF ,0xDB ,0xE3 ,0x50 ,0x08 ,0x01,
0xDA ,0x00 ,0x00 ,0x0E ,0xE3 ,0x54 ,0x00 ,0x00,
0xE5 ,0x9F ,0x15 ,0x1C ,0xE5 ,0x9F ,0x05 ,0x30,
0xE3 ,0xE0 ,0x00 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE2 ,0x41 ,0x10 ,0x14,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE2 ,0x81 ,0x00 ,0x00 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE2 ,0x81 ,0x10 ,0x14 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x91 ,0x10 ,0x00,
0xE5 ,0x9F ,0x04 ,0xF4 ,0xEA ,0x00 ,0x00 ,0x0D,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x14 ,0xE0,
0xE2 ,0x41 ,0x10 ,0x14 ,0xE5 ,0x9F ,0x04 ,0xEC,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x00 ,0x00,
0xE5 ,0x91 ,0x10 ,0x00 ,0xE2 ,0x81 ,0x10 ,0x14,
0xE3 ,0xA0 ,0x00 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x14 ,0xA4,
0xE5 ,0x9F ,0x04 ,0x98 ,0xEA ,0x00 ,0x00 ,0x1B,
0xE2 ,0x80 ,0x07 ,0x01 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x14 ,0x90,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x00 ,0x00,
0xE0 ,0x80 ,0x00 ,0x01 ,0xE5 ,0x9F ,0x14 ,0x98,
0xE5 ,0x9F ,0x14 ,0x68 ,0xE0 ,0x80 ,0x00 ,0x09,
0xE0 ,0x80 ,0x00 ,0x01 ,0xE5 ,0x91 ,0x10 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x14 ,0x70,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE2 ,0x81 ,0x00 ,0x00 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE2 ,0x81 ,0x10 ,0x14 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x91 ,0x10 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x04 ,0x3C,
0xE5 ,0x9F ,0x14 ,0x30 ,0xE2 ,0x80 ,0x00 ,0x04,
0xE5 ,0x9F ,0x04 ,0x28 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE3 ,0x50 ,0x08 ,0x02 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE3 ,0xA0 ,0x00 ,0x00 ,0x3A ,0xFF ,0xFF ,0xDF,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x14 ,0x14,
0xE5 ,0x9F ,0x04 ,0x08 ,0xEA ,0x00 ,0x00 ,0x1F,
0xE2 ,0x80 ,0x07 ,0x01 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x14 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x00 ,0x00,
0xE0 ,0x80 ,0x00 ,0x01 ,0xE5 ,0x9F ,0x14 ,0x08,
0xE5 ,0x9F ,0x13 ,0xD8 ,0xE0 ,0x80 ,0x00 ,0x09,
0xE0 ,0x80 ,0x00 ,0x01 ,0xE5 ,0x91 ,0x10 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x13 ,0xE4,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x00 ,0x10,
0xE2 ,0x81 ,0x10 ,0x04 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE2 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE2 ,0x41 ,0x10 ,0x04 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE1 ,0x50 ,0x00 ,0x01 ,0xE5 ,0x91 ,0x10 ,0x00,
0xE2 ,0x85 ,0x50 ,0x01 ,0x0A ,0x00 ,0x00 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x03 ,0x9C,
0xE5 ,0x9F ,0x13 ,0x90 ,0xE2 ,0x80 ,0x00 ,0x04,
0xE5 ,0x9F ,0x03 ,0x88 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE3 ,0x50 ,0x08 ,0x02 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE3 ,0x55 ,0x00 ,0x00 ,0x3A ,0xFF ,0xFF ,0xDB,
0xE5 ,0x9F ,0x03 ,0x90 ,0xDA ,0x00 ,0x00 ,0x0E,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x13 ,0x70,
0xE2 ,0x41 ,0x10 ,0x14 ,0xE3 ,0xE0 ,0x00 ,0x00,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x00 ,0x00,
0xE5 ,0x91 ,0x10 ,0x00 ,0xE2 ,0x81 ,0x10 ,0x14,
0xEA ,0x00 ,0x00 ,0x0D ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x9F ,0x13 ,0x34 ,0xE5 ,0x9F ,0x03 ,0x54,
0xE5 ,0x9F ,0x03 ,0x4C ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE2 ,0x41 ,0x10 ,0x14,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE2 ,0x81 ,0x00 ,0x00 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE2 ,0x81 ,0x10 ,0x14 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x91 ,0x10 ,0x00,
0xE5 ,0x9F ,0x12 ,0xF8 ,0xE3 ,0xA0 ,0x00 ,0x00,
0xEA ,0x00 ,0x00 ,0x1B ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x02 ,0xEC,
0xE5 ,0x9F ,0x12 ,0xE4 ,0xE2 ,0x80 ,0x08 ,0x06,
0xE2 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x9F ,0x12 ,0xF8 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE0 ,0x80 ,0x00 ,0x09 ,0xE0 ,0x80 ,0x00 ,0x01,
0xE5 ,0x91 ,0x10 ,0x00 ,0xE5 ,0x9F ,0x12 ,0xBC,
0xE5 ,0x9F ,0x12 ,0xC4 ,0xE0 ,0x80 ,0x00 ,0x01,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x00 ,0x00,
0xE5 ,0x91 ,0x10 ,0x00 ,0xE2 ,0x81 ,0x10 ,0x14,
0xE5 ,0x9F ,0x02 ,0x90 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE2 ,0x80 ,0x00 ,0x04 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x12 ,0x84,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x02 ,0x7C,
0x3A ,0xFF ,0xFF ,0xDF ,0xE3 ,0x50 ,0x08 ,0x02,
0xE5 ,0x9F ,0x12 ,0x68 ,0xE3 ,0xA0 ,0x00 ,0x00,
0xEA ,0x00 ,0x00 ,0x1F ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x02 ,0x5C,
0xE5 ,0x9F ,0x12 ,0x54 ,0xE2 ,0x80 ,0x08 ,0x06,
0xE2 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x9F ,0x12 ,0x68 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE0 ,0x80 ,0x00 ,0x09 ,0xE0 ,0x80 ,0x00 ,0x01,
0xE5 ,0x91 ,0x10 ,0x00 ,0xE5 ,0x9F ,0x12 ,0x2C,
0xE5 ,0x9F ,0x12 ,0x38 ,0xE0 ,0x80 ,0x00 ,0x01,
0xE2 ,0x81 ,0x00 ,0x10 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x10 ,0x04,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x00 ,0x00,
0xE5 ,0x91 ,0x10 ,0x00 ,0xE2 ,0x41 ,0x10 ,0x04,
0x0A ,0x00 ,0x00 ,0x00 ,0xE1 ,0x50 ,0x00 ,0x01,
0xE5 ,0x9F ,0x01 ,0xF0 ,0xE2 ,0x86 ,0x60 ,0x01,
0xE2 ,0x80 ,0x00 ,0x04 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x11 ,0xE4,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x01 ,0xDC,
0x3A ,0xFF ,0xFF ,0xDB ,0xE3 ,0x50 ,0x08 ,0x02,
0xDA ,0x00 ,0x00 ,0x0E ,0xE3 ,0x56 ,0x00 ,0x00,
0xE5 ,0x9F ,0x11 ,0xC4 ,0xE5 ,0x9F ,0x01 ,0xF0,
0xE3 ,0xE0 ,0x00 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE2 ,0x41 ,0x10 ,0x14,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE2 ,0x81 ,0x00 ,0x00 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE2 ,0x81 ,0x10 ,0x14 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x91 ,0x10 ,0x00,
0xE5 ,0x9F ,0x01 ,0xB4 ,0xEA ,0x00 ,0x00 ,0x0D,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x11 ,0x88,
0xE2 ,0x41 ,0x10 ,0x14 ,0xE5 ,0x9F ,0x01 ,0xAC,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x00 ,0x00,
0xE5 ,0x91 ,0x10 ,0x00 ,0xE2 ,0x81 ,0x10 ,0x14,
0xE5 ,0x9F ,0x01 ,0x4C ,0xE5 ,0x81 ,0x00 ,0x00,
0xE2 ,0x80 ,0x00 ,0x01 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x11 ,0x40,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x01 ,0x38,
0xE5 ,0x91 ,0x10 ,0x00 ,0xE5 ,0x9F ,0x11 ,0x2C,
0x3A ,0xFF ,0xFE ,0xB3 ,0xE1 ,0x50 ,0x00 ,0x01,
0xCA ,0x00 ,0x00 ,0x03 ,0xE3 ,0x54 ,0x00 ,0x00,
0xCA ,0x00 ,0x00 ,0x01 ,0xE3 ,0x55 ,0x00 ,0x00,
0xDA ,0x00 ,0x00 ,0x11 ,0xE3 ,0x56 ,0x00 ,0x00,
0xE5 ,0x9F ,0x11 ,0x0C ,0xE5 ,0x9F ,0x01 ,0x40,
0xE2 ,0x81 ,0x00 ,0x08 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE2 ,0x80 ,0x04 ,0xDF ,0xE5 ,0x90 ,0x00 ,0x00,
0xE2 ,0x41 ,0x10 ,0x14 ,0xE2 ,0x40 ,0x08 ,0x53,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x00 ,0x00,
0xE5 ,0x91 ,0x10 ,0x00 ,0xE2 ,0x81 ,0x10 ,0x14,
0xEA ,0x00 ,0x00 ,0x10 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x9F ,0x10 ,0xC4 ,0xE5 ,0x9F ,0x00 ,0xF8,
0xE2 ,0x81 ,0x00 ,0x08 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE2 ,0x80 ,0x04 ,0xA6 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE2 ,0x41 ,0x10 ,0x14 ,0xE2 ,0x40 ,0x08 ,0x5B,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x00 ,0x00,
0xE5 ,0x91 ,0x10 ,0x00 ,0xE2 ,0x81 ,0x10 ,0x14,
0xE3 ,0xA0 ,0x06 ,0x01 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x10 ,0x80,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x00 ,0x00,
0xE2 ,0x41 ,0x10 ,0x0C ,0xE5 ,0x90 ,0x00 ,0x00,
0xE2 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE2 ,0x81 ,0x10 ,0x10 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE1 ,0x50 ,0x00 ,0x01 ,0xE5 ,0x91 ,0x10 ,0x00,
0xE5 ,0x9F ,0x00 ,0x80 ,0x0A ,0x00 ,0x00 ,0x0B,
0xE5 ,0x9F ,0x10 ,0x38 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE3 ,0xA0 ,0x80 ,0x01 ,0xE5 ,0x81 ,0x00 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE2 ,0x81 ,0x00 ,0x04,
0xE0 ,0x80 ,0x00 ,0x01 ,0xE5 ,0x91 ,0x10 ,0x00,
0xE5 ,0x81 ,0x00 ,0x00 ,0xE5 ,0x9F ,0x10 ,0x60,
0xE3 ,0xA0 ,0x80 ,0x00 ,0xEA ,0x00 ,0x00 ,0x00,
0xCA ,0xFF ,0xFE ,0x67 ,0xE3 ,0x57 ,0x00 ,0x00,
0xEB ,0xFF ,0xFE ,0x4A ,0xE3 ,0xA0 ,0x00 ,0x01,
0x00 ,0x00 ,0x90 ,0x18 ,0xE8 ,0xBD ,0x87 ,0xF0,
0x00 ,0x00 ,0x90 ,0x0C ,0x00 ,0x00 ,0x90 ,0x10,
0x11 ,0x11 ,0x11 ,0x00 ,0x00 ,0x00 ,0x90 ,0x14,
0x00 ,0x00 ,0x90 ,0x04 ,0x00 ,0x00 ,0x90 ,0x00,
0x11 ,0x11 ,0x22 ,0x22 ,0x00 ,0x10 ,0x00 ,0x08,
0x00 ,0x10 ,0x00 ,0x18 ,0x22 ,0x22 ,0x22 ,0x00,
0x33 ,0x33 ,0x33 ,0x00 ,0x33 ,0x33 ,0x44 ,0x44,
0x55 ,0x55 ,0x66 ,0x66 ,0x00 ,0x10 ,0x00 ,0x28,
0x00 ,0x00 ,0x90 ,0x08 ,0x00 ,0x10 ,0x00 ,0x78,
0xE9 ,0x2D ,0x40 ,0x70 ,0x00 ,0x00 ,0x90 ,0x1C,
0xE1 ,0xA0 ,0x50 ,0x01 ,0xE1 ,0xA0 ,0x40 ,0x00,
0xE1 ,0xA0 ,0x20 ,0x06 ,0xE1 ,0xA0 ,0x60 ,0x02,
0xE1 ,0xA0 ,0x00 ,0x05 ,0xE1 ,0xA0 ,0x10 ,0x04,
0xE8 ,0xBD ,0x80 ,0x70 ,0xEB ,0xFF ,0xFD ,0xBF,
0xE1 ,0xA0 ,0x20 ,0x00 ,0xE9 ,0x2D ,0x40 ,0x7C,
0x1A ,0x00 ,0x00 ,0x04 ,0xE3 ,0x51 ,0x00 ,0x01,
0xE3 ,0xA0 ,0x00 ,0x00 ,0xE5 ,0xD2 ,0x40 ,0x00,
0xE5 ,0x8D ,0x40 ,0x00 ,0xE5 ,0x8D ,0x00 ,0x04,
0xE3 ,0x51 ,0x00 ,0x02 ,0xEA ,0x00 ,0x00 ,0x10,
0xE1 ,0xD2 ,0x50 ,0xB0 ,0x1A ,0x00 ,0x00 ,0x04,
0xE5 ,0x8D ,0x00 ,0x04 ,0xE3 ,0xA0 ,0x00 ,0x00,
0xEA ,0x00 ,0x00 ,0x09 ,0xE5 ,0x8D ,0x50 ,0x00,
0x1A ,0x00 ,0x00 ,0x03 ,0xE3 ,0x51 ,0x00 ,0x08,
0xE5 ,0x92 ,0x60 ,0x04 ,0xE5 ,0x92 ,0x00 ,0x00,
0xEA ,0x00 ,0x00 ,0x03 ,0xE8 ,0x8D ,0x00 ,0x41,
0xE3 ,0xA0 ,0x00 ,0x00 ,0xE5 ,0x92 ,0x30 ,0x00,
0xE5 ,0x8D ,0x30 ,0x00 ,0xE5 ,0x8D ,0x00 ,0x04,
0xE8 ,0xBD ,0x80 ,0x7C ,0xE5 ,0x9D ,0x00 ,0x00,
0xE1 ,0xA0 ,0x40 ,0x00 ,0xE9 ,0x2D ,0x40 ,0x70,
0xE1 ,0xA0 ,0x60 ,0x02 ,0xE1 ,0xA0 ,0x50 ,0x01,
0xE1 ,0xA0 ,0x10 ,0x05 ,0xE1 ,0xA0 ,0x20 ,0x06,
0xEB ,0xFF ,0xFD ,0x9A ,0xE1 ,0xA0 ,0x00 ,0x04,
0xE9 ,0x2D ,0x40 ,0x7C ,0xE8 ,0xBD ,0x80 ,0x70,
0x1A ,0x00 ,0x00 ,0x01 ,0xE3 ,0x52 ,0x00 ,0x01,
0xEA ,0x00 ,0x00 ,0x0D ,0xE5 ,0xC0 ,0x10 ,0x00,
0x1A ,0x00 ,0x00 ,0x01 ,0xE3 ,0x52 ,0x00 ,0x02,
0xEA ,0x00 ,0x00 ,0x09 ,0xE1 ,0xC0 ,0x10 ,0xB0,
0x1A ,0x00 ,0x00 ,0x06 ,0xE3 ,0x52 ,0x00 ,0x08,
0xE3 ,0xA0 ,0x40 ,0x00 ,0xE1 ,0xA0 ,0x30 ,0x01,
0xE5 ,0x9D ,0x50 ,0x04 ,0xE8 ,0x8D ,0x00 ,0x12,
0xE5 ,0x80 ,0x50 ,0x04 ,0xE5 ,0x80 ,0x10 ,0x00,
0xE5 ,0x80 ,0x10 ,0x00 ,0xEA ,0x00 ,0x00 ,0x00,
0xE9 ,0x2D ,0x4F ,0xF0 ,0xE8 ,0xBD ,0x80 ,0x7C,
0xE1 ,0xA0 ,0x40 ,0x00 ,0xE2 ,0x4D ,0xD0 ,0x24,
0x1A ,0x00 ,0x00 ,0x01 ,0xE3 ,0x54 ,0x00 ,0x00,
0xEA ,0x00 ,0x00 ,0x00 ,0xE3 ,0xA0 ,0x00 ,0x00,
0xE2 ,0x80 ,0x0C ,0xF2 ,0xE3 ,0x0F ,0x03 ,0x00,
0xEA ,0x00 ,0x00 ,0xB3 ,0xE5 ,0x8D ,0x00 ,0x20,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE5 ,0x9D ,0x00 ,0x20,
0x0A ,0x00 ,0x00 ,0x90 ,0xE3 ,0x50 ,0x00 ,0x00,
0xE5 ,0x90 ,0x00 ,0x0C ,0xE5 ,0x9D ,0x00 ,0x20,
0x1A ,0x00 ,0x00 ,0x8C ,0xE3 ,0x50 ,0x00 ,0x00,
0xE5 ,0x90 ,0x00 ,0x18 ,0xE5 ,0x9D ,0x00 ,0x20,
0x30 ,0x8F ,0xF1 ,0x00 ,0xE3 ,0x50 ,0x00 ,0x09,
0xEA ,0x00 ,0x00 ,0x07 ,0xEA ,0x00 ,0x00 ,0x80,
0xEA ,0x00 ,0x00 ,0x1B ,0xEA ,0x00 ,0x00 ,0x11,
0xEA ,0x00 ,0x00 ,0x32 ,0xEA ,0x00 ,0x00 ,0x25,
0xEA ,0x00 ,0x00 ,0x4A ,0xEA ,0x00 ,0x00 ,0x3D,
0xEA ,0x00 ,0x00 ,0x67 ,0xEA ,0x00 ,0x00 ,0x59,
0xE5 ,0x9D ,0x00 ,0x20 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE5 ,0x9F ,0x12 ,0x74 ,0xE5 ,0x90 ,0x00 ,0x04,
0xE5 ,0x8D ,0x00 ,0x1C ,0xE7 ,0x91 ,0x01 ,0x00,
0xE1 ,0x2F ,0xFF ,0x30 ,0xE5 ,0x9D ,0x00 ,0x1C,
0xE5 ,0x81 ,0x00 ,0x10 ,0xE5 ,0x9D ,0x10 ,0x20,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xEA ,0x00 ,0x00 ,0x6C,
0xE5 ,0x90 ,0x00 ,0x04 ,0xE5 ,0x9D ,0x00 ,0x20,
0xE7 ,0x91 ,0xB1 ,0x00 ,0xE5 ,0x9F ,0x12 ,0x48,
0xE5 ,0x91 ,0x00 ,0x1C ,0xE5 ,0x9D ,0x10 ,0x20,
0xE5 ,0x9D ,0x10 ,0x20 ,0xE1 ,0x2F ,0xFF ,0x3B,
0xEA ,0x00 ,0x00 ,0x61 ,0xE5 ,0x81 ,0x00 ,0x10,
0xE5 ,0x9D ,0x00 ,0x20 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE5 ,0x9F ,0x12 ,0x1C ,0xE5 ,0x90 ,0x00 ,0x04,
0xE5 ,0x9D ,0x20 ,0x20 ,0xE7 ,0x91 ,0xA1 ,0x00,
0xE1 ,0x2F ,0xFF ,0x3A ,0xE1 ,0xC2 ,0x01 ,0xDC,
0xE5 ,0x81 ,0x00 ,0x10 ,0xE5 ,0x9D ,0x10 ,0x20,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xEA ,0x00 ,0x00 ,0x56,
0xE5 ,0x90 ,0x00 ,0x04 ,0xE5 ,0x9D ,0x00 ,0x20,
0xE7 ,0x91 ,0x31 ,0x00 ,0xE5 ,0x9F ,0x11 ,0xF0,
0xE5 ,0x9D ,0x30 ,0x20 ,0xE5 ,0x8D ,0x30 ,0x18,
0xE8 ,0x92 ,0x00 ,0x07 ,0xE2 ,0x83 ,0x20 ,0x1C,
0xE1 ,0x2F ,0xFF ,0x33 ,0xE5 ,0x9D ,0x30 ,0x18,
0xE5 ,0x81 ,0x00 ,0x10 ,0xE5 ,0x9D ,0x10 ,0x20,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xEA ,0x00 ,0x00 ,0x48,
0xE5 ,0x90 ,0x00 ,0x04 ,0xE5 ,0x9D ,0x00 ,0x20,
0xE7 ,0x91 ,0x61 ,0x00 ,0xE5 ,0x9F ,0x11 ,0xB8,
0xE1 ,0xCC ,0x01 ,0xDC ,0xE5 ,0x9D ,0xC0 ,0x20,
0xE1 ,0x2F ,0xFF ,0x36 ,0xE1 ,0xCC ,0x22 ,0xD4,
0xE5 ,0x81 ,0x00 ,0x10 ,0xE5 ,0x9D ,0x10 ,0x20,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xEA ,0x00 ,0x00 ,0x3C,
0xE5 ,0x90 ,0x00 ,0x04 ,0xE5 ,0x9D ,0x00 ,0x20,
0xE7 ,0x91 ,0x71 ,0x00 ,0xE5 ,0x9F ,0x11 ,0x88,
0xE5 ,0x90 ,0x30 ,0x2C ,0xE5 ,0x9D ,0x00 ,0x20,
0xE1 ,0xC0 ,0x22 ,0xD4 ,0xE5 ,0x8D ,0x30 ,0x00,
0xE1 ,0x2F ,0xFF ,0x37 ,0xE1 ,0xC0 ,0x01 ,0xDC,
0xE5 ,0x81 ,0x00 ,0x10 ,0xE5 ,0x9D ,0x10 ,0x20,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xEA ,0x00 ,0x00 ,0x2E,
0xE5 ,0x90 ,0x00 ,0x04 ,0xE5 ,0x9D ,0x00 ,0x20,
0xE7 ,0x91 ,0xC1 ,0x00 ,0xE5 ,0x9F ,0x11 ,0x50,
0xE5 ,0x9D ,0x00 ,0x20 ,0xE5 ,0x8D ,0xC0 ,0x14,
0xE1 ,0xCD ,0x20 ,0xF0 ,0xE1 ,0xC0 ,0x22 ,0xDC,
0xE1 ,0xC0 ,0x01 ,0xDC ,0xE1 ,0xC0 ,0x22 ,0xD4,
0xE1 ,0x2F ,0xFF ,0x3C ,0xE5 ,0x9D ,0xC0 ,0x14,
0xE5 ,0x81 ,0x00 ,0x10 ,0xE5 ,0x9D ,0x10 ,0x20,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xEA ,0x00 ,0x00 ,0x1E,
0xE5 ,0x90 ,0x00 ,0x04 ,0xE5 ,0x9D ,0x00 ,0x20,
0xE7 ,0x91 ,0x81 ,0x00 ,0xE5 ,0x9F ,0x11 ,0x10,
0xE2 ,0x80 ,0x30 ,0x2C ,0xE5 ,0x9D ,0x00 ,0x20,
0xE8 ,0x8D ,0x00 ,0x0E ,0xE8 ,0x93 ,0x00 ,0x0E,
0xE1 ,0xC0 ,0x01 ,0xDC ,0xE1 ,0xC0 ,0x22 ,0xD4,
0xE5 ,0x9D ,0x10 ,0x20 ,0xE1 ,0x2F ,0xFF ,0x38,
0xEA ,0x00 ,0x00 ,0x0F ,0xE5 ,0x81 ,0x00 ,0x10,
0xE5 ,0x9D ,0x00 ,0x20 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE5 ,0x9F ,0x10 ,0xD4 ,0xE5 ,0x90 ,0x00 ,0x04,
0xE5 ,0x9D ,0x00 ,0x20 ,0xE7 ,0x91 ,0x91 ,0x00,
0xE1 ,0xC0 ,0x02 ,0xDC ,0xE1 ,0xC0 ,0x23 ,0xD4,
0xE5 ,0x9D ,0xC0 ,0x20 ,0xE8 ,0x8D ,0x00 ,0x0F,
0xE1 ,0xCC ,0x22 ,0xD4 ,0xE1 ,0xCC ,0x01 ,0xDC,
0xE5 ,0x9D ,0x10 ,0x20 ,0xE1 ,0x2F ,0xFF ,0x39,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xE5 ,0x81 ,0x00 ,0x10,
0xE3 ,0xA0 ,0x00 ,0x01 ,0xE3 ,0x20 ,0xF0 ,0x00,
0xE5 ,0x81 ,0x00 ,0x08 ,0xE5 ,0x9D ,0x10 ,0x20,
0xE5 ,0x9D ,0x00 ,0x20 ,0xE3 ,0xA0 ,0x10 ,0x01,
0xE5 ,0x9D ,0x00 ,0x20 ,0xE5 ,0x80 ,0x10 ,0x0C,
0xE3 ,0x50 ,0x00 ,0x00 ,0xE5 ,0x90 ,0x00 ,0x00,
0xE5 ,0x9D ,0x00 ,0x20 ,0x0A ,0x00 ,0x00 ,0x0C,
0xE3 ,0x50 ,0x00 ,0x00 ,0xE5 ,0x90 ,0x00 ,0x0C,
0xE5 ,0x9D ,0x00 ,0x20 ,0x0A ,0x00 ,0x00 ,0x08,
0xE3 ,0x50 ,0x00 ,0x00 ,0xE5 ,0x90 ,0x00 ,0x08,
0xE3 ,0xA0 ,0x10 ,0x00 ,0x1A ,0x00 ,0x00 ,0x04,
0xE5 ,0x80 ,0x10 ,0x00 ,0xE5 ,0x9D ,0x00 ,0x20,
0xE5 ,0x80 ,0x10 ,0x0C ,0xE5 ,0x9D ,0x00 ,0x20,
0x0A ,0x00 ,0x00 ,0x0B ,0xE3 ,0x54 ,0x00 ,0x00,
0xE5 ,0x90 ,0x00 ,0x00 ,0xE5 ,0x9D ,0x00 ,0x20,
0x1A ,0x00 ,0x00 ,0x07 ,0xE3 ,0x50 ,0x00 ,0x00,
0xE5 ,0x90 ,0x00 ,0x0C ,0xE5 ,0x9D ,0x00 ,0x20,
0x1A ,0x00 ,0x00 ,0x03 ,0xE3 ,0x50 ,0x00 ,0x00,
0xE3 ,0x55 ,0x00 ,0x00 ,0xE5 ,0x94 ,0x50 ,0x00,
0xEA ,0x00 ,0x00 ,0x00 ,0x0A ,0x00 ,0x00 ,0x00,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xEA ,0xFF ,0xFF ,0x4A,
0xE8 ,0xBD ,0x8F ,0xF0 ,0xE2 ,0x8D ,0xD0 ,0x24,
0xE9 ,0x2D ,0x40 ,0x10 ,0x00 ,0x00 ,0x90 ,0x20,
0xE3 ,0x20 ,0xF0 ,0x00 ,0xEB ,0xFF ,0xFD ,0x3A,
0xE9 ,0x2D ,0x50 ,0x3F ,0xEA ,0xFF ,0xFF ,0xFE,
0xE3 ,0xA0 ,0x10 ,0x01 ,0xE3 ,0xA0 ,0x20 ,0x04,
0xEB ,0xFF ,0xFF ,0x1E ,0xE3 ,0x0F ,0x02 ,0x98,
0xEA ,0x00 ,0x00 ,0x03 ,0xE3 ,0xA0 ,0x40 ,0x00,
0xE3 ,0x0F ,0x02 ,0x98 ,0xE3 ,0xA0 ,0x10 ,0x04,
0xE1 ,0xA0 ,0x40 ,0x00 ,0xEB ,0xFF ,0xFE ,0xF4,
0x1A ,0xFF ,0xFF ,0xF9 ,0xE3 ,0x54 ,0x00 ,0x02,
0xE8 ,0xBD ,0x50 ,0x3F ,0xEB ,0xFF ,0xFC ,0x50,
0xE9 ,0x2D ,0x50 ,0x3F ,0xE2 ,0x5E ,0xF0 ,0x04,
0xE5 ,0x9F ,0x00 ,0x14 ,0xE3 ,0xA0 ,0x10 ,0x04,
0xE1 ,0xA0 ,0x40 ,0x00 ,0xEB ,0xFF ,0xFE ,0xEA,
0xE1 ,0xA0 ,0x50 ,0x00 ,0xE1 ,0x2F ,0xFF ,0x34,
0xE2 ,0x5E ,0xF0 ,0x04 ,0xE8 ,0xBD ,0x50 ,0x3F,
0xE1 ,0x2F ,0xFF ,0x1E ,0x00 ,0x08 ,0x0F ,0x00,
0x37 ,0x36 ,0x35 ,0x34 ,0x33 ,0x32 ,0x31 ,0x30,
0x46 ,0x45 ,0x44 ,0x43 ,0x42 ,0x41 ,0x39 ,0x38,
0x33 ,0x32 ,0x31 ,0x30 ,0x00 ,0x58 ,0x30 ,0x40,
0x62 ,0x61 ,0x39 ,0x38 ,0x37 ,0x36 ,0x35 ,0x34,
0x00 ,0x78 ,0x30 ,0x40 ,0x66 ,0x65 ,0x64 ,0x63,
0x00 ,0x00 ,0x90 ,0x00 ,0x00 ,0x00 ,0x13 ,0x48,
0x00 ,0x00 ,0x01 ,0xA0 ,0x00 ,0x00 ,0x00 ,0x34,
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
0x00 ,0x00 ,0x0E ,0xA0 ,0x00 ,0x00 ,0x00 ,0x00,
0x00 ,0x00 ,0x0E ,0x7C ,0x00 ,0x00 ,0x0F ,0x34,
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x0F ,0x10,
};

int bcm_8806x_firmware_size = sizeof(bcm_8806x_firmware);

#else
typedef int _phy8806x_firmware_c_not_empty; /* Make ISO compilers happy. */
#endif /* INCLUDE_PHY_8806X */

