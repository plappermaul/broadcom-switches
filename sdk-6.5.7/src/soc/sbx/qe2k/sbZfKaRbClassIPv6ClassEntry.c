/*
 * $Id: sbZfKaRbClassIPv6ClassEntry.c,v 1.2 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */


#include "sbTypes.h"
#include "sbZfKaRbClassIPv6ClassEntry.hx"
#include "sbWrappers.h"
#include <sal/types.h>



/* Pack from struct into array of bytes */
uint32
sbZfKaRbClassIPv6ClassEntry_Pack(sbZfKaRbClassIPv6ClassEntry_t *pFrom,
                                 uint8 *pToData,
                                 uint32 nMaxToDataIndex) {
#ifdef SAND_BIG_ENDIAN_HOST
  int i;
  int size = SB_ZF_ZFKARBCLASSIPV6CLASSENTRY_SIZE_IN_BYTES;

  if (size % 4) {
    size += (4 - size %4);
  }

  for ( i=0; i<size; i++ ) {
    (pToData)[i] = 0;
  }
  i = 0;

  /* Pack operation based on bigword endian */

  /* Pack Member: m_nReserve1 */
  (pToData)[2] |= ((pFrom)->m_nReserve1 & 0x03) <<6;

  /* Pack Member: m_nClass1Dp */
  (pToData)[2] |= ((pFrom)->m_nClass1Dp & 0x03) <<4;

  /* Pack Member: m_nClass1Lsb */
  (pToData)[2] |= ((pFrom)->m_nClass1Lsb & 0x0f);

  /* Pack Member: m_nReserve0 */
  (pToData)[3] |= ((pFrom)->m_nReserve0 & 0x03) <<6;

  /* Pack Member: m_nClass0Dp */
  (pToData)[3] |= ((pFrom)->m_nClass0Dp & 0x03) <<4;

  /* Pack Member: m_nClass0Lsb */
  (pToData)[3] |= ((pFrom)->m_nClass0Lsb & 0x0f);
#else
  int i;
  int size = SB_ZF_ZFKARBCLASSIPV6CLASSENTRY_SIZE_IN_BYTES;

  if (size % 4) {
    size += (4 - size %4);
  }

  for ( i=0; i<size; i++ ) {
    (pToData)[i] = 0;
  }
  i = 0;

  /* Pack operation based on little endian */

  /* Pack Member: m_nReserve1 */
  (pToData)[1] |= ((pFrom)->m_nReserve1 & 0x03) <<6;

  /* Pack Member: m_nClass1Dp */
  (pToData)[1] |= ((pFrom)->m_nClass1Dp & 0x03) <<4;

  /* Pack Member: m_nClass1Lsb */
  (pToData)[1] |= ((pFrom)->m_nClass1Lsb & 0x0f);

  /* Pack Member: m_nReserve0 */
  (pToData)[0] |= ((pFrom)->m_nReserve0 & 0x03) <<6;

  /* Pack Member: m_nClass0Dp */
  (pToData)[0] |= ((pFrom)->m_nClass0Dp & 0x03) <<4;

  /* Pack Member: m_nClass0Lsb */
  (pToData)[0] |= ((pFrom)->m_nClass0Lsb & 0x0f);
#endif

  return SB_ZF_ZFKARBCLASSIPV6CLASSENTRY_SIZE_IN_BYTES;
}




/* Unpack from array of bytes into struct */
void
sbZfKaRbClassIPv6ClassEntry_Unpack(sbZfKaRbClassIPv6ClassEntry_t *pToStruct,
                                   uint8 *pFromData,
                                   uint32 nMaxToDataIndex) {
  COMPILER_UINT64 tmp;

  (void) tmp;

#ifdef SAND_BIG_ENDIAN_HOST

  /* Unpack operation based on bigword endian */

  /* Unpack Member: m_nReserve1 */
  (pToStruct)->m_nReserve1 =  (uint32)  ((pFromData)[2] >> 6) & 0x03;

  /* Unpack Member: m_nClass1Dp */
  (pToStruct)->m_nClass1Dp =  (uint32)  ((pFromData)[2] >> 4) & 0x03;

  /* Unpack Member: m_nClass1Lsb */
  (pToStruct)->m_nClass1Lsb =  (uint32)  ((pFromData)[2] ) & 0x0f;

  /* Unpack Member: m_nReserve0 */
  (pToStruct)->m_nReserve0 =  (uint32)  ((pFromData)[3] >> 6) & 0x03;

  /* Unpack Member: m_nClass0Dp */
  (pToStruct)->m_nClass0Dp =  (uint32)  ((pFromData)[3] >> 4) & 0x03;

  /* Unpack Member: m_nClass0Lsb */
  (pToStruct)->m_nClass0Lsb =  (uint32)  ((pFromData)[3] ) & 0x0f;
#else

  /* Unpack operation based on little endian */

  /* Unpack Member: m_nReserve1 */
  (pToStruct)->m_nReserve1 =  (uint32)  ((pFromData)[1] >> 6) & 0x03;

  /* Unpack Member: m_nClass1Dp */
  (pToStruct)->m_nClass1Dp =  (uint32)  ((pFromData)[1] >> 4) & 0x03;

  /* Unpack Member: m_nClass1Lsb */
  (pToStruct)->m_nClass1Lsb =  (uint32)  ((pFromData)[1] ) & 0x0f;

  /* Unpack Member: m_nReserve0 */
  (pToStruct)->m_nReserve0 =  (uint32)  ((pFromData)[0] >> 6) & 0x03;

  /* Unpack Member: m_nClass0Dp */
  (pToStruct)->m_nClass0Dp =  (uint32)  ((pFromData)[0] >> 4) & 0x03;

  /* Unpack Member: m_nClass0Lsb */
  (pToStruct)->m_nClass0Lsb =  (uint32)  ((pFromData)[0] ) & 0x0f;
#endif

}



/* initialize an instance of this zframe */
void
sbZfKaRbClassIPv6ClassEntry_InitInstance(sbZfKaRbClassIPv6ClassEntry_t *pFrame) {

  pFrame->m_nReserve1 =  (unsigned int)  0;
  pFrame->m_nClass1Dp =  (unsigned int)  0;
  pFrame->m_nClass1Lsb =  (unsigned int)  0;
  pFrame->m_nReserve0 =  (unsigned int)  0;
  pFrame->m_nClass0Dp =  (unsigned int)  0;
  pFrame->m_nClass0Lsb =  (unsigned int)  0;

}
