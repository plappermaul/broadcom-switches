/*
 * This software is governed by the Broadcom Switch APIs license.
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenMDK/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * Parser for main symbol ID.
 */

#include <cdk/cdk_stdlib.h>
#include <cdk/cdk_string.h>
#include <cdk/cdk_symbols.h>
#include <cdk/cdk_chip.h>
#include <cdk/cdk_shell.h>

static int 
_parse_basic_id(const char *str, cdk_shell_basic_id_t *bid,
                int ignore_index, int intname)
{
    cdk_shell_tokens_t cst1; 
    cdk_shell_tokens_t cst2; 

    CDK_STRCPY(bid->id, str); 

    bid->start = bid->end = -1; 

    bid->name32 = 0; 
    bid->ext32 = 0; 

    /* 
     * The input can be of the following forms :
     *
     *	NAME
     *  NAME{x}
     *  NAME{x,y}
     *  NAME[x]
     *  NAME[x,y]
     *  x
     *  x,y
 */

    /* First, split up the tokens based on [] */
    if (cdk_shell_tokenize(str, &cst1, "[]") < 0) {
	return -1; 
    }
    
    /* At most we can have 2 tokens */
    if (cst1.argc > 2) return -1; 
       
    if (cst1.argc == 1) {
	/* 
	 * No indices were specified. 
	 * Input must be one of:
	 *	NAME
	 *	NAME{x}
	 *	NAME{x,y}
	 *	x
	 *	x,y
	 */
	
	/* Split it up by comma */
	if (cdk_shell_tokenize(cst1.argv[0], &cst2, ",") < 0) {
	    return -1;
	}
	
	/* At most we can have 2 tokens (only 1 comma) */
	if (cst2.argc > 2) return -1; 

	if (cst2.argc == 1) {
	    /* 
	     * No commas were specified. 
	     * Input must be one of:
	     *	NAME
	     *  NAME{x}
	     *	x
	     */
	    
	    /* Is the input an integer only? */
	    if (cdk_shell_parse_is_int(cst2.argv[0])) {
		/* 
		 * Input was:
		 *	x
		 */
                char *ptr; 
                int len;

		bid->valid = 1; 
		CDK_STRCPY(bid->name, cst2.argv[0]); 
                ptr = bid->name;
                /* Support for 64-bit raw addresses */
                if (ptr[0] == '0' && (ptr[1] == 'x' || ptr[1] == 'X') &&
                    (len = CDK_STRLEN(ptr)) > 10) {
                    ptr[len-8] = 0;
                    if (cdk_shell_parse_uint32(ptr, &bid->ext32) < 0) {
                        return -1; 
                    }       
                    /* Restore name */
                    CDK_STRCPY(ptr, cst2.argv[0]); 
                }
		if (cdk_shell_parse_uint32(ptr, &bid->name32) < 0) {
                    return -1; 
                }       
		bid->start = bid->end = -1; 
		/* Done */
		return 0; 
	    }
	    else {
		/*
		 * Input was one of:
		 *	NAME
		 *	NAME{X}
		 */
		char *nptr = bid->name; 
		char *ptr; 
		
		bid->valid = 1; 

		for (ptr = cst2.argv[0]; *ptr; ptr++, nptr++) {
		    if (!ignore_index && cdk_shell_parse_is_int(ptr)) {
			/* Number starts at ptr */
			*nptr = 0; 
			if(cdk_shell_parse_int(ptr, &bid->start) < 0) {
                            return -1;
                        }       
			bid->end = bid->start; 
			/* Done */
			return 0; 
		    }
		    /* Deposit this character into the name array */
		    *nptr = *ptr; 
		}

		/* No numbers found */
		*nptr = 0; 
		bid->start = bid->end = -1; 

		return 0; 
	    }		
	}
	if (cst2.argc == 2) {
	    /* 
	     * Input was one of:
	     * NAME{x,y}
	     * x,y
	     */
	    bid->valid = 1; 
	    bid->name[0] = 0; 
	    
	    if (cdk_shell_parse_is_int(cst2.argv[0])) {
		/* Input is x,y */
		if (cdk_shell_parse_int(cst2.argv[0], &bid->start) < 0) {
		    return -1; 
		}
		if (cdk_shell_parse_int(cst2.argv[1], &bid->end) < 0) {
		    return -1; 
		}
	    }
	    else {
		/* Input is NAME{x,y} */
		char *nptr = bid->name; 
		char *ptr; 
		bid->valid = 1; 
		
		for (ptr = cst2.argv[0]; *ptr; ptr++, nptr++) {
		    if (cdk_shell_parse_is_int(ptr)) {
			/* Number starts at ptr */
			*nptr = 0; 
			if(cdk_shell_parse_int(ptr, &bid->start) < 0) {
                            return -1; 
                        }       
			break;
		    }
		    *nptr = *ptr; 
		}
		if (cdk_shell_parse_int(cst2.argv[1], &bid->end) < 0) {
		    return -1; 
		}
	    }
	    /* Done */
	    return 0; 
	}
    }
    if (cst1.argc == 2) {
	/* 
	 * Indices were specified. 
	 * Input must be one of:
	 *	NAME[x]
	 *  	NAME[x,y]
	 */
	
	/* Carve up based on comma */
	if (cdk_shell_tokenize(cst1.argv[1], &cst2, ",") < 0) {
	    return -1; 
	}

	/* At most we can have 2 tokens (only 1 comma) */
	if (cst2.argc > 2) return -1; 
	
	if (cst2.argc == 1) {
	    /* 
	     * No comma specified. 
	     * Input was:
	     *	NAME[x]
	     */
	    bid->valid = 1; 
	    CDK_STRCPY(bid->name, cst1.argv[0]); 
	    if ((cdk_shell_parse_uint32(bid->name, &bid->name32) < 0) && 
                intname) {
		return -1; 
	    }	
	    if ((cdk_shell_parse_int(cst2.argv[0], &bid->start) < 0)) {
		return -1; 
	    }
	    bid->end = bid->start; 
	    
	    /* Done */
	    return 0; 
	}
	if (cst2.argc == 2) {
	    /*
	     * Comma specified. 
	     * Input was:
	     *	NAME[x,y]
	     */
	    bid->valid = 1; 
	    CDK_STRCPY(bid->name, cst1.argv[0]); 
	    if ((cdk_shell_parse_uint32(bid->name, &bid->name32) < 0) && 
                intname) {
		return -1; 
	    }
	    if ((cdk_shell_parse_int(cst2.argv[0], &bid->start) < 0)) {
		return -1; 
	    }	
	    if ((cdk_shell_parse_int(cst2.argv[1], &bid->end) < 0)) {
		return -1; 
	    }	
	    
	    /* Done */
	    return 0; 
	}
    }

    return -1; 
}
	    
int
cdk_shell_parse_id(const char *str, cdk_shell_id_t *sid, int intaddr)
{
    int i; 
    cdk_shell_tokens_t cst1; 
    cdk_shell_basic_id_t bids[3]; 

    /* Initialize destination structure */
    CDK_MEMSET(sid, 0, sizeof(*sid)); 
    CDK_STRCPY(sid->id, str); 
    sid->addr.valid = 0; 
    sid->addr.start=sid->addr.end = -1; 
    sid->block.valid = 0; 
    sid->block.start=sid->block.end = -1; 
    sid->port.valid = 0; 
    sid->port.start=sid->port.end = -1; 

    /* Carve up the input into period-delimited tokens */
    if (cdk_shell_tokenize(str, &cst1, ".") < 0) {
	return -1; 
    }

    /* 
     * Each period-delimited token is a basic identifier, 
     * and there can be at most 3.
     */
    if (cst1.argc > 3) return -1;

    for (i = 0; i < cst1.argc; i++) {
        /*
         * Note that for the first token (register/memory name) we do
         * not decode any index at the end of the name. This is
         * required for correct handling of names that end in a number
         * (e.g. TIMER1).
         */
	if (_parse_basic_id(cst1.argv[i], &bids[i], (i == 0), 0) < 0) {
	    return -1; 
	}
    }

    /* 
     * The first basic identifier is always the address 
     */
    CDK_MEMCPY(&sid->addr, &bids[0], sizeof(bids[0])); 
    
    /* Does the ADDR have to be an integer? */
    if ((cdk_shell_parse_is_int(sid->addr.name) == 0) & intaddr) {
	return -1; 
    }

    if (cst1.argc == 3) {
	/* Second argument is the block specifier */
	CDK_MEMCPY(&sid->block, &bids[1], sizeof(bids[1])); 
	if (cdk_shell_parse_is_int(bids[1].name)) {
	    /* Just a number specified, need to patch up the indices */
	    sid->block.start = sid->block.end = sid->block.name32; 
	}
	/* Third argument is the port specifier */
	CDK_MEMCPY(&sid->port, &bids[2], sizeof(bids[2])); 
	if (cdk_shell_parse_is_int(bids[1].name)) {
	    sid->port.start = sid->port.end = sid->port.name32; 
	}
    }
    else if (cst1.argc == 2) {
	/*
	 * Second argument is either a port or block specifier. 
	 * If 'port' is specified, it is a port.
	 * If '' is specified, it is a port.
	 * If 'block' is specified, it is a block.
	 * If any string not above is specified, that is assumed 
         * to be a block name.
	 */
	if (CDK_STRCMP(bids[1].name, "port") == 0 || bids[1].name[0] == 0 || 
            cdk_shell_parse_is_int(bids[1].name)) {
	    CDK_MEMCPY(&sid->port, &bids[1], sizeof(bids[1])); 
	    
	    if (cdk_shell_parse_is_int(bids[1].name)) {
		sid->port.name[0] = 0; 
		sid->port.start = sid->port.end = sid->port.name32;
	    }
	}	
	else {
	    CDK_MEMCPY(&sid->block, &bids[1], sizeof(bids[1])); 
	    if (cdk_shell_parse_is_int(bids[1].name)) {
		sid->block.name[0] = 0; 
		sid->block.start = sid->block.end = sid->block.name32; 
	    }
	}
    }

    /* Done */
    return 0; 
}
