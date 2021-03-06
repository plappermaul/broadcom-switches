/**
 * $Id: pkg_c3_lib_h.stg,v 1.18 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * Author: vn-1109210841
 *
 * C3Asm3__PkgInt.h
 *
 * Support library for the C3 asm3 package
 */

#ifndef __C3Asm3__header__defined__
#define __C3Asm3__header__defined__

#include "Bsc.h"

/******************************************************************************
 * C3Asm3__Md5
 *****************************************************************************/
typedef struct C3Asm3__Md5__ {
  unsigned long m_count[2], m_state[4]; /* number of bytes processed and intermediate digest state  */
  unsigned char m_buf[64], m_pad[64]; /* data block being processed and buffer with the default padding */
}
C3Asm3__Md5;

/* md5 context setup */
int C3Asm3__Md5__init (C3Asm3__Md5 *_p);

/* md5 process buffer */
int C3Asm3__Md5__update (C3Asm3__Md5 *_p, unsigned char *_input, int _ilen);

/* md5 final digest */
int C3Asm3__Md5__finish (C3Asm3__Md5 *_p, unsigned char _output[16]);

/******************************************************************************
 * C3Asm3__PkgNcae Named Constants Access Entry
 *****************************************************************************/
typedef struct C3Asm3__PkgNcae__ {
  char *m_name; /*name associated with the value*/
  unsigned int
    m_fdin,    /*flag indicating dynamic allocation, we do not save it in the buffer / file or read it from there*/
    m_fc,      /*flag which indicate if value is in the byte code*/
    m_va,      /*value of the constant*/
    /*corresponded byte code parameters, valid only if m_fc is not 0*/
    m_sn,  /*stream number*/
    m_in,  /*instruction number*/
    m_bo,  /*offset of the sub-field in the machine word (bits)*/
    m_bl;  /*length of the sub-field in the machine word (bits)*/
} C3Asm3__PkgNcae;

/* initialize interface structure to be empty */
int C3Asm3__PkgNcae__init(C3Asm3__PkgNcae *a_p);

/* compare 2 structure and return rezult in a_e */
int C3Asm3__PkgNcae__equ(C3Asm3__PkgNcae *a_a, C3Asm3__PkgNcae *a_b, int *a_e);

/* destroy interface structure to be empty */
int C3Asm3__PkgNcae__destroy(C3Asm3__PkgNcae *a_p);

/* serialize all interface data into the buffer */
int C3Asm3__PkgNcae__writeBsc(C3Asm3__PkgNcae *a_p, Bsc *a_pb);

/* deserialize all interface data from the buffer */
int C3Asm3__PkgNcae__readBsc(C3Asm3__PkgNcae *a_p, Bsc *a_pb);

/* copy NCA structure a_s to NCA structure a_d with dynamic buffers */
int C3Asm3__PkgNcae__copyD(C3Asm3__PkgNcae *a_d, C3Asm3__PkgNcae *a_s);

/******************************************************************************
 * C3Asm3__PkgInt
 *****************************************************************************/

typedef int C3Asm3__HwUcode (void *a_pv, unsigned int a_sn, unsigned int a_in, unsigned char *a_iw);

typedef struct C3Asm3__PkgInt__ {
  unsigned int 
    m_magic,     /*4 octets long magic number to identify binary form of package*/

    m_tver_maj,  /*tool major version - change of the package format or major functionality*/
    m_tver_min,  /*tool minor version - other changes*/
    m_tver_patch,/*tool patch number*/

    m_uver_maj,  /*ucode major version - change of the package format or major functionality*/
    m_uver_min,  /*ucode minor version*/
    m_uver_patch,/*ucode patch number*/

    m_fdin,      /*flag indicating dinamic allocation*/
    m_snum,      /*number of streams*/
    m_inum,      /*number of instructions*/
    m_onum,      /*number of octets in the machine word*/
    m_ncnum,     /*number of entries in the NCAT table*/
    m_hwid       /*hardware identifier, currently used 1 for caladan 1 and 2 for caladan 2*/
    ;

  char  *m_tver_name  /*string portion of tool version*/
       ,*m_uver_name; /*string portion of ucode version*/
    
  unsigned char m_md5a[16]; /* md5 sum for the  m_uver_name, m_uver_maj,  m_uver_min,  m_uver_patch, m_tmap, m_code */

  C3Asm3__PkgNcae
    *m_ncat;/*NCAT table*/

  unsigned char
    *m_tmap, /*task map*/
    *m_code; /*code*/

  C3Asm3__HwUcode *m_fPut, *m_fGet; /* call back functions to put machine word into hardware and get it from there */
  void *m_pv; /* pointer passed by consumer to call-back hardware access functions via functions of putNc family*/

} C3Asm3__PkgInt;

/* serialize all interface data into the buffer */
int C3Asm3__PkgInt__writeBsc(C3Asm3__PkgInt *a_p, Bsc *a_pb);

/* deserialize all interface data from the buffer */
int C3Asm3__PkgInt__readBsc(C3Asm3__PkgInt *a_p, Bsc *a_pb);

/* update one entry in the named constants table */
int C3Asm3__PkgInt__updateNcEntry(C3Asm3__PkgInt *a_p, C3Asm3__PkgNcae *a_e, unsigned int *a_nc);

/* get Named Constant value */
int C3Asm3__PkgInt__getNc(C3Asm3__PkgInt *a_p, char *a_n, unsigned int *a_nc);

/* put Named Constant value */
int C3Asm3__PkgInt__putNc(C3Asm3__PkgInt *a_p, char *a_n, unsigned int *a_nc);

/* backup Nc values */
int C3Asm3__PkgInt__backupNc(C3Asm3__PkgInt *a_p, C3Asm3__PkgInt *new);
int C3Asm3__PkgInt__backupNc__force(C3Asm3__PkgInt *a_p, C3Asm3__PkgInt *new);

/* restore Nc values */
int C3Asm3__PkgInt__restoreNc(int unit, C3Asm3__PkgInt *a_p);
int C3Asm3__PkgInt__restoreNc__force(int unit, C3Asm3__PkgInt *a_p);


/* initialize interface structure to be empty */
int C3Asm3__PkgInt__init(C3Asm3__PkgInt *a_p);

/* initialize interface structure to be empty with initialization of pointers to the hw access functions*/
int C3Asm3__PkgInt__initF(C3Asm3__PkgInt *a_p, C3Asm3__HwUcode *a_fPut, C3Asm3__HwUcode *a_fGet, void *a_pv);

/* destroy interface structure to be empty */
int C3Asm3__PkgInt__destroy(C3Asm3__PkgInt *a_p);

/* calculate length of the buffer which could be used to serialize all interface data */
int C3Asm3__PkgInt__lenBuf(C3Asm3__PkgInt *a_p, unsigned int *a_l);

/* compare 2 interface structure and return rezult in a_e */
int C3Asm3__PkgInt__equ(C3Asm3__PkgInt *a_a, C3Asm3__PkgInt *a_b, int *a_e);

/* copy interface structure a_s to interface structure a_d with dynamic buffers */
int C3Asm3__PkgInt__copyD(C3Asm3__PkgInt *a_d, C3Asm3__PkgInt *a_s);

/* serialize all interface data into the buffer */
int C3Asm3__PkgInt__writeBuf(C3Asm3__PkgInt *a_p, unsigned char *a_b, unsigned int a_l);

/* deserialize all interface data from the buffer */
int C3Asm3__PkgInt__readBuf(C3Asm3__PkgInt *a_p, unsigned char *a_b, unsigned int a_l);

/* get pointer to the first octet of the machine word */
int C3Asm3__PkgInt__getWordPtr(C3Asm3__PkgInt *a_p, unsigned int a_sn, unsigned int a_in, unsigned char **a_wp);

/* get pointer to the first octet of the machine word */
int C3Asm3__PkgInt__getTask(C3Asm3__PkgInt *a_p, unsigned int a_in, unsigned int *a_t);

/* calculate md5a ( for all significunt data fields and arrays ) */
int C3Asm3__PkgInt__md5a(C3Asm3__PkgInt *a_p, unsigned char a_d[16]);

/* initialize interface structure to the default values generated by asm3 */
int C3Asm3__PkgInt__initDefault(C3Asm3__PkgInt *_p);

/* utilities */
int C3Asm3__hc2i (int _c);
int C3Asm3__i2hc (int _i);
int C3Asm3__s2b(unsigned char *_d, int _ld, unsigned char *_s, int _ls);
int C3Asm3__b2s (unsigned char *_d, int _ld, unsigned char *_s, int _ls);

/* Get table pointer ref */
int C3Asm3__PkgInt__getTablePtr(int unit, C3Asm3__PkgInt *a_p, char *a_n, const void **a_nc);

#endif /*__C3Asm3__header__defined__*/

/*end of file*/
