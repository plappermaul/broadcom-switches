/*
 *
 * This software is governed by the Broadcom Switch APIs license.
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenMDK/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bmd/bmd.h>

#ifdef CDK_CONFIG_ARCH_XGSD_INSTALLED

#include <bmdi/arch/xgsd_led.h>

#include <cdk/cdk_device.h>

int
xgsd_led_update(int unit, int offset, uint32_t flags)
{
    uint32_t led_data;

    CDK_DEV_READ32(unit, CMIC_LED_DATA_RAM(offset), &led_data);

    led_data &= ~0x81;
    if (flags & XGSD_LED_LINK) {
	led_data |= 0x01;
    }
    if (flags & XGSD_LED_TURBO) {
	led_data |= 0x80;
    }

    CDK_DEV_WRITE32(unit, CMIC_LED_DATA_RAM(offset), led_data);

    return CDK_E_NONE;
}

#endif /* CDK_CONFIG_ARCH_XGSD_INSTALLED */