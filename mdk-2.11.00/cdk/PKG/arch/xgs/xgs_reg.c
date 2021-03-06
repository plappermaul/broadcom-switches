/*
 * This software is governed by the Broadcom Switch APIs license.
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenMDK/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * XGS register access functions.
 */

#include <cdk/cdk_device.h>
#include <cdk/cdk_assert.h>
#include <cdk/cdk_debug.h>
#include <cdk/cdk_simhook.h>

#include <cdk/arch/xgs_chip.h>
#include <cdk/arch/xgs_reg.h>
#include <cdk/arch/xgs_schan.h>

/*******************************************************************************
 *
 * Common register routines
 */

int
cdk_xgs_reg_read(int unit, uint32_t addr, uint32_t *data, int size)
{
    int rv; 
    int srcblk, dstblk, datalen;
    schan_msg_t schan_msg;

    CDK_ASSERT(CDK_DEV_EXISTS(unit)); 
    
    /* Simulator hooks */
    if (cdk_simhook_read) {
        return cdk_simhook_read(unit, 1, addr, data,
                                CDK_WORDS2BYTES(size));
    }

    /* Configure S-Channel parameters */
    srcblk = CDK_XGS_CMIC_BLOCK(unit);
    dstblk = CDK_XGS_ADDR2BLOCK(addr);
    datalen = 4 * size;
    if (CDK_XGS_FLAGS(unit) & CDK_XGS_CHIP_FLAG_SCHAN_SB0) {
        srcblk = 0;
    }
    if (CDK_XGS_FLAGS(unit) & CDK_XGS_CHIP_FLAG_SCHAN_MBI) {
        datalen = 0;
        if (dstblk != CDK_XGS_CMIC_BLOCK(unit)) {
	    addr &= 0xc0fff;
        }
    }

    /* Write message to S-Channel */
    SCHAN_MSG_CLEAR(&schan_msg);
    SCMH_OPCODE_SET(schan_msg.readcmd.header, READ_REGISTER_CMD_MSG);
    SCMH_SRCBLK_SET(schan_msg.readcmd.header, srcblk);
    SCMH_DSTBLK_SET(schan_msg.readcmd.header, dstblk);
    SCMH_DATALEN_SET(schan_msg.readcmd.header, datalen);
    schan_msg.readcmd.address = addr;   

    /* Write header word + address DWORD, read header word + data DWORD */
    rv = cdk_xgs_schan_op(unit, &schan_msg, 2, 1+size);
    if (CDK_FAILURE(rv)) {
        CDK_ERR(("cdk_xgs_reg_read[%d]: S-channel error addr=0x%08"PRIx32"\n",
                 unit, addr));
        return rv;
    }

    /* Check result */
    if (SCMH_OPCODE_GET(schan_msg.readresp.header) != READ_REGISTER_ACK_MSG) {
        CDK_ERR(("cdk_xgs_reg_read[%d]: Invalid S-channel ACK: %"PRIu32""
                 " (expected %d) addr=0x%08"PRIx32"\n", unit,
                 SCMH_OPCODE_GET(schan_msg.readresp.header),
                 READ_REGISTER_ACK_MSG, addr));
	return CDK_E_FAIL; 
    }
    
    /* Debug output */
    CDK_DEBUG_REG(("cdk_xgs_reg_read[%d]: addr=0x%08"PRIx32" data: 0x",
                   unit, addr));
    data[0] = schan_msg.readresp.data[0];
    if (size > 1) {
        /* Return requested data */
        data[1] = schan_msg.readresp.data[1];
        CDK_DEBUG_REG(("%08"PRIx32"", data[1]));
    }
    CDK_DEBUG_REG(("%08"PRIx32"\n", data[0]));

    return CDK_E_NONE; 
}

int
cdk_xgs_reg_write(int unit, uint32_t addr, uint32_t *data, int size)
{
    int rv; 
    int srcblk, dstblk, datalen;
    schan_msg_t schan_msg;

    CDK_ASSERT(CDK_DEV_EXISTS(unit)); 
    
    /* Simulator hooks */
    if (cdk_simhook_write) {
        return cdk_simhook_write(unit, 1, addr, data,
                                 CDK_WORDS2BYTES(size));
    }

    /* Configure S-Channel parameters */
    srcblk = CDK_XGS_CMIC_BLOCK(unit);
    dstblk = CDK_XGS_ADDR2BLOCK(addr);
    datalen = 4 * size;
    if (CDK_XGS_FLAGS(unit) & CDK_XGS_CHIP_FLAG_SCHAN_SB0) {
        srcblk = 0;
        if (CDK_XGS_CHIP_TREX_GET(unit) != 0) {
            srcblk |= 0x2; /* Set bit 15 in S-channel header */
        }
    }
    if (CDK_XGS_FLAGS(unit) & CDK_XGS_CHIP_FLAG_SCHAN_MBI) {
        if (dstblk != CDK_XGS_CMIC_BLOCK(unit)) {
	    addr &= 0xc0fff;
        }
    }

    /*
     * Setup S-Channel command packet
     *
     * NOTE: the datalen field matters only for the Write Memory and
     * Write Register commands, where it is used only by the CMIC to
     * determine how much data to send, and is in units of bytes.
     */

    SCHAN_MSG_CLEAR(&schan_msg);    
    SCMH_OPCODE_SET(schan_msg.writecmd.header, WRITE_REGISTER_CMD_MSG);
    SCMH_SRCBLK_SET(schan_msg.writecmd.header, srcblk);
    SCMH_DSTBLK_SET(schan_msg.writecmd.header, dstblk);
    SCMH_DATALEN_SET(schan_msg.writecmd.header, datalen); 

    schan_msg.writecmd.address = addr;
    schan_msg.writecmd.data[0] = data[0]; 
    schan_msg.writecmd.data[1] = (size > 1) ? data[1] : 0; 

    rv = cdk_xgs_schan_op(unit, &schan_msg, 2+size, 0); 
    if (CDK_FAILURE(rv)) {
        CDK_ERR(("cdk_xgs_reg_write[%d]: S-channel error addr=0x%08"PRIx32"\n",
                 unit, addr));
    }

    /* Debug output */
    CDK_DEBUG_REG(("cdk_xgs_reg_write[%d]: addr=0x%08"PRIx32" data: 0x",
                   unit, addr));
    if (size > 1) {
        CDK_DEBUG_REG(("%08"PRIx32"", data[1]));
    }
    CDK_DEBUG_REG(("%08"PRIx32"\n", data[0]));

    return rv;
}
