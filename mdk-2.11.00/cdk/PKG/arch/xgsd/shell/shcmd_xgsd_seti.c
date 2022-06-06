/*
 * This software is governed by the Broadcom Switch APIs license.
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenMDK/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * XGSD shell command SETI
 */

#include <cdk/arch/xgsd_shell.h>

#include <cdk/arch/shcmd_xgsd_seti.h>

#if CDK_CONFIG_SHELL_INCLUDE_SETI == 1

static int
_parse_multiword(const char *str, cdk_shell_tokens_t *cst, 
                 uint32_t *words, int max_words)
{
    int idx;

    /* Parse string into word tokens */
    if (cdk_shell_tokenize(str, cst, ":") < 0) {
        return -1;
    }

    /* Check array size */
    if (cst->argc > max_words) {
	return -1;
    }

    /* Convert all tokens to integers */
    for (idx = 0; idx < cst->argc; idx++) {
        /* This argument must be an integer */
        if (cdk_shell_parse_uint32(cst->argv[idx], &words[idx]) < 0) {
            return -1;
	}
    }

    return 0;
}


/*******************************************************************************
 *
 * seti cmic 
 *
 * Set a cmic register
 */

static int
_seti_cmic(int argc, char *argv[], void *context)
{
    int unit = *((int *)context);
    uint32_t addr; 
    uint32_t data; 

    if (cdk_shell_parse_uint32(argv[0], &addr) < 0) {
        return cdk_shell_parse_error("address", argv[0]); 
    }
    if (cdk_shell_parse_uint32(argv[1], &data) < 0) {
        return cdk_shell_parse_error("data", argv[1]); 
    }

    addr &= ~3;
    CDK_DEV_WRITE32(unit, addr, data); 

    return 0;     
}

/*******************************************************************************
 *
 * seti reg
 */

static int
_seti_reg(int argc, char *argv[], void *context)
{
    int unit = *((int *)context);
    uint32_t and_masks[2];
    uint32_t or_masks[2]; 
    cdk_shell_id_t sid;
    cdk_shell_tokens_t cst;
    int size; 

    /* Register will be cleared */
    and_masks[0] = and_masks[1] = 0;

    /* Crack the identifier */
    if (argc == 0 || cdk_shell_parse_id(*argv, &sid, 1) < 0) {
        return cdk_shell_parse_error("address", *argv); 
    }
    argv++;
    argc--;

    /* 32 or 64 bit multiword value */
    or_masks[0] = or_masks[1] = 0;
    if (argc == 0 ||
        _parse_multiword(*argv, &cst, or_masks, COUNTOF(or_masks)) < 0) {
        return cdk_shell_parse_error("data", *argv);
    }
    argv++; 
    argc--;

    /* Default size is the number of words specified */
    size = cst.argc; 

    /* Optional third argument is the size of the register */
    if (argc > 0 && cdk_shell_parse_int(*argv, &size) < 0) {
        return cdk_shell_parse_error("size", *argv); 
    }

    return cdk_xgsd_shell_regops(unit, NULL, &sid, size, 0, and_masks, or_masks); 
}

static int
_seti_mem(int argc, char *argv[], void *context)
{
    int unit = *((int *)context);
    int size; 
    cdk_shell_tokens_t cst; 
    cdk_shell_id_t sid; 
    uint32_t and_masks[8]; 
    uint32_t or_masks[8]; 

    /* Memory will be cleared */
    CDK_MEMSET(and_masks, 0, sizeof(and_masks));

    /* Crack the identifier */
    if (argc == 0 || cdk_shell_parse_id(*argv, &sid, 1) < 0) {
        return cdk_shell_parse_error("address", *argv); 
    }
    argv++;
    argc--;

    /* Second argument is the data */
    CDK_MEMSET(or_masks, 0, sizeof(or_masks));
    if (argc == 0 ||
        _parse_multiword(*argv, &cst, or_masks, COUNTOF(or_masks)) < 0) {
        return cdk_shell_parse_error("data", *argv); 
    }
    argv++; 
    argc--;

    /* Default size is the number of words specified */
    size = cst.argc; 

    /* 
     * Optional third argument is the the memory size in words
     * Any words NOT specified in the data will be written as 
     * zero (up to the size.)
     */
    if (argc > 0 && (cdk_shell_parse_int(*argv, &size) < 0)) {
        return cdk_shell_parse_error("size", *argv); 
    }

    return cdk_xgsd_shell_memops(unit, NULL, &sid, size, 0, 0, and_masks, or_masks); 
}


/*******************************************************************************
 *
 * seti miim 
 */

static int
_seti_miim(int argc, char *argv[], void *context)
{
    int unit = *((int *)context);
    uint32_t data; 
    cdk_shell_id_t sid; 
    int i; 

    /* Crack the phy_id and addresses */
    if (argc == 0 || 
        cdk_shell_parse_id(*argv, &sid, 1) < 0 || sid.addr.start >= 0) {
        return cdk_shell_parse_error("miim addr", *argv); 
    }
    argv++;
    argc--;
    
    if (sid.port.start < 0 && sid.port.end < 0) {
        sid.port.start = 0; 
        sid.port.end = 0x1f; 
    } else if (sid.port.end < 0) {
        sid.port.end = sid.port.start; 
    }

    /* Get the data */
    if (argc == 0 || cdk_shell_parse_uint32(*argv, &data) < 0) {
	return cdk_shell_parse_error("miim data", *argv); 
    }
    argv++; 
    argc--;

    for (i = sid.port.start; i <= sid.port.end; i++) {
        if (cdk_xgsd_miim_write(unit, sid.addr.name32, i, data) < 0) {
	    CDK_PRINTF("%s writing miim(0x%"PRIx32")[0x%x]\n", 
                       CDK_CONFIG_SHELL_ERROR_STR, sid.addr.name32, i); 
	}
    }

    return 0; 
}


static cdk_shell_vect_t _seti_vects[] = 
{
    { "cmic",   _seti_cmic,     }, 
    { "reg",    _seti_reg,      }, 
    { "mem",    _seti_mem,      },     
    { "miim",   _seti_miim,     },
    { 0, 0 }, 
}; 
      
int
cdk_shcmd_xgsd_seti(int argc, char* argv[])
{
    int unit;

    unit = cdk_shell_unit_arg_extract(&argc, argv, 1);
    if(!CDK_DEV_EXISTS(unit)) {
        return CDK_SHELL_CMD_BAD_ARG;
    }
    if (cdk_shell_parse_vect(argc, argv, &unit, _seti_vects, NULL) < 0) {
        cdk_shell_parse_error("type", *argv); 
    }
    return 0; 
}

#endif /*  CDK_CONFIG_SHELL_INCLUDE_SETI */