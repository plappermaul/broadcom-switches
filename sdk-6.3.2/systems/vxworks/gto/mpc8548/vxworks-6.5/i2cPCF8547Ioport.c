/*
 * $Id: i2cPCF8547Ioport.c 1.5 Broadcom SDK $
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
 */


#include <vxWorks.h>            /* vxWorks generics */
#include "config.h"
#include "sysMotI2c.h"
#include "i2cPCF8547Ioport.h"
#include <stdio.h>

STATUS
pcf8574_boardid_get(UINT8 *board_id)
{
    
    return (i2cRead(PCF8574_SMBUS_CHAN,
                    PCF8574_CCR_ADDRESS,
                    I2C_DEVICE_TYPE_IOPORT_PCF8574,
                    0,
                    1,
                    board_id));
}

STATUS
pcf8574_readv(int slave_addr, UINT8 *value)
{
    
    return (i2cRead(PCF8574_SMBUS_CHAN,
                    slave_addr,
                    I2C_DEVICE_TYPE_IOPORT_PCF8574,
                    0,
                    1,
                    value));
}

STATUS
pcf8574_writev(int slave_addr, UINT8 *value)
{
    
    return (i2cWrite(PCF8574_SMBUS_CHAN,
                    slave_addr,
                    I2C_DEVICE_TYPE_IOPORT_PCF8574,
                    0,
                    1,
                    value));
}

STATUS
pcf8574_write_val(int slave_addr, int value)
{
    UINT8 c = value & 0xFF;

    pcf8574_writev(slave_addr, &c);
}

UINT8
pcf8574_read_val(int slave_addr)
{
    UINT8 c = 0xFF;

    pcf8574_readv(slave_addr, &c);
    return c;
}

#ifdef INCLUDE_I2C_DEBUG
void pcf8574_boardid_show()
{
    UINT8 board_id;
    
    board_id = 0;

    if (pcf8574_boardid_get(&board_id) == 0) {
        printf("Board ID : %x\n", board_id);
    } else {
        printf("pcf8574_boardid_get() failed\n");
    }
}
#endif /* INCLUDE_I2C_DEBUG */