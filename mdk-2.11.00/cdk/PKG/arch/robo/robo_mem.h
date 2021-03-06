/*
 * This software is governed by the Broadcom Switch APIs license.
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenMDK/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * ROBO register access functions.
 */

#ifndef __ROBO_MEM_H__
#define __ROBO_MEM_H__

#include <cdk/arch/robo_chip.h>

/*
 * Generic 32-bit ROBO memory address layout:
 *
 * [31:28] - Access method
 * [27:0]  - Memory address
 *
 *
 * Access method 0 (Invalid)
 *
 * Reserved in order to produce an error message when accessing
 * a memory with no valid access method defined, i.e. using an
 * unmodified regsfile address.
 * 
 *
 * Access method 1 (BCM539x ARL)
 *
 * Access through generic MEM_CTRL debug interface. Since the data
 * registers may be different for different chips, the rest of the
 * memory "address" is defined as follows:
 *
 *    [27:26] - Reserved flags
 *    [25]    - Flag indicating that secondary data word is 16-bits only
 *    [24]    - Flag indicating that MAC/VID are bit-reversed
 *    [23:16] - Register offset of 64-bit data high
 *    [15:8]  - Register page
 *    [7:0]   - Register offset of 64-bit data low
 *
 *
 * Access method 2 (BCM539x VLAN)
 *
 * Use of specific VLAN_1Q interface. The access registers are
 * identical across all chips, but located at different offsets:
 *
 *    [27:16] - Unused
 *    [15:8]  - Register page
 *    [7:0]   - Base register offset
 *
 *
 * Access method 3 (BCM5395 TCAM/CFP)
 *
 * Use of specific TCAM interface.
 *
 *         
 * Access method 4 (BCM532x2 Generic ARL)
 *
 * Access through ARL interface, which allows access to all memories,
 * except for the ARL table itself. The memory "address" is defined
 * as follows:
 *
 *    [27:24] - Reserved flags
 *    [23:16] - Memory type (see robo_mem_regs.h)
 *    [15:8]  - Register page
 *    [7:0]   - Register address of 64-bit data low
 *
 *
 * Access method 5 (Search ARL)
 *
 * Access through ARL search interface. This method provides read
 * access only and is rather slow when used for searching ARL table
 * by index. Used only when the normal memory debig interface is
 * broken (e.g. BCM53242/BCM53262). The data decoder translates
 * the search result to the field layout used by the debug memory
 * interface.
 *
 *    [27:24] - Reserved flags
 *    [25]    - Flag indicating that secondary data word is 16-bits only
 *    [24]    - Reserved flag
 *    [23:16] - Data decoder (see robo_mem_regs.h)
 *    [15:8]  - Register page
 *    [7:0]   - Register address of 64-bit data low
 *
 * 
 * Access method 6 (Thunderbolt Access table)
 *
 * Access through MEM_CTRL search interface. This method provides read
 * access only and is rather slow when used for searching ARL table
 * by index. Used only when the normal memory debig interface is
 * broken (e.g. BCM53280 and etc). The data decoder translates
 * the search result to the field layout used by the debug memory
 * interface.
 *
 *    [27:24] - Reserved flags
 *    [23:16] - Memory type (see robo_mem_regs.h)
 *    [15:8]  - Register page
 *    [7:0]   - Register address of 64-bit data low
 *
 *
 * Access method 7 (Thunderbolt ARL table)
 *
 * Access through MEM_CTRL search interface. This method provides read
 * access only and is rather slow when used for searching ARL table
 * by index. Used only when the normal memory debig interface is
 * broken (e.g. BCM53280 and etc). The data decoder translates
 * the search result to the field layout used by the debug memory
 * interface.
 *
 *    [27:24] - bin number
 *    [23:16] - Memory type (see robo_mem_regs.h)
 *    [15:8]  - Register page
 *    [7:0]   - Register address of 64-bit data low
 */

/* Register layout */
#define ROBO_MEM_ACCESS_METHOD_SHIFT    28
#define ROBO_MEM_ACCESS_ADDR_MASK       (LSHIFT32(1, ROBO_MEM_ACCESS_METHOD_SHIFT) - 1)

/* Access methods */
#define ROBO_MEM_ACC_INVALID            0
#define ROBO_MEM_ACC_ARL                1
#define ROBO_MEM_ACC_VLAN               2
#define ROBO_MEM_ACC_TCAM               3
#define ROBO_MEM_ACC_GARL               4
#define ROBO_MEM_ACC_SARL               5
#define ROBO_MEM_ACC_GEN                6

#define ROBO_MEM_ACCESS_METHOD(_r)      ((_r) >> ROBO_MEM_ACCESS_METHOD_SHIFT)

/* MAC address and VLAN ID are byte-wise bit-reversed */
#define ROBO_MEM_ARL_F_KEY_REV          LSHIFT32(1, 24)
#define ROBO_MEM_ARL_KEY_REV_0(_v) \
    (cdk_util_bit_rev_by_byte_word32(_v))
#define ROBO_MEM_ARL_KEY_REV_1(_v) \
    (cdk_util_bit_rev_by_byte_word32((_v) & 0x0fffffff) | ((_v) & 0xf0000000))

/* Most significant data word is 16 bits (otherhwise assume 64 bits) */
#define ROBO_MEM_ARL_F_MSD_16           LSHIFT32(1, 25)

extern int
cdk_robo_mem_read(int unit, uint32_t addr, uint32_t idx, void *vptr, int size);

extern int
cdk_robo_mem_write(int unit, uint32_t addr, uint32_t idx, void *vptr, int size);

extern int
cdk_robo_mem_arl_read(int unit, uint32_t addr, uint32_t idx, void *vptr, int size);

extern int
cdk_robo_mem_arl_write(int unit, uint32_t addr, uint32_t idx, void *vptr, int size);

extern int
cdk_robo_mem_vlan_read(int unit, uint32_t addr, uint32_t idx, void *vptr, int size);

extern int
cdk_robo_mem_vlan_write(int unit, uint32_t addr, uint32_t idx, void *vptr, int size);

extern int
cdk_robo_mem_garl_read(int unit, uint32_t addr, uint32_t idx, void *vptr, int size);

extern int
cdk_robo_mem_garl_write(int unit, uint32_t addr, uint32_t idx, void *vptr, int size);

extern int
cdk_robo_mem_gmem_read(int unit, uint32_t addr, uint32_t idx, void *vptr, int size);

extern int
cdk_robo_mem_gmem_write(int unit, uint32_t addr, uint32_t idx, void *vptr, int size);

extern int
cdk_robo_mem_sarl_read(int unit, uint32_t addr, uint32_t idx, void *vptr, int size);

extern int
cdk_robo_mem_sarl_write(int unit, uint32_t addr, uint32_t idx, void *vptr, int size);

extern int
cdk_robo_mem_clear(int unit, uint32_t addr, uint32_t si, uint32_t ei, int size); 

extern int
cdk_robo_mem_gen_read(int unit, uint32_t addr, uint32_t idx, void *vptr, int size);

extern int
cdk_robo_mem_gen_write(int unit, uint32_t addr, uint32_t idx, void *vptr, int size);

#define CDK_ROBO_MEM_CLEAR(_u, _m) \
    cdk_robo_mem_clear(_u, _m, _m##_MIN, _m##_MAX, _m##_SIZE)

#endif /* __ROBO_MEM_H__ */
