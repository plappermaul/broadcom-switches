/*
 * $Id:$
 *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */


#ifdef _ERR_MSG_MODULE_NAME
#error "_ERR_MSG_MODULE_NAME redefined"
#endif
#define _ERR_MSG_MODULE_NAME BSL_BCM_INTR



#include <shared/bsl.h>
#include <soc/dnxc/legacy/error.h>
#include <soc/drv.h>

#include <soc/error.h>

/* 
 *  include  
 */ 

#include <sal/core/libc.h>
#include <sal/core/alloc.h>
#include <shared/bitop.h>

#include <bcm/error.h>

#include <soc/dnxc/legacy/dnxc_ser_correction.h>


#define WIDE_ECC_MAX_INTERVALS 10
#define WIDE_ECC_MAX_DATA_WIDTH 60
#define WIDE_ECC_MAX_ECC_WIDTH  10

static void copy_bits(
                      uint32 *src,
                      uint32  src_start,
                      uint32 *dst,
                      uint32  dst_start,
                      uint32  len
                      )
{
    int i,j;
    for (i=src_start,j=dst_start;i<len +src_start ;i++,j++) {
        int bit;
/* Result is 0 only if all bits in the range are 0 */
       SHR_BITTEST_RANGE(src, i, 1, bit);
       if (bit) {
            SHR_BITSET_RANGE(dst, j, 1) ;        

       }
       else {
           SHR_BITCLR_RANGE(dst, j, 1) ;        
       }

    }
}

static int is_eq_bits(
                      uint32 *src,
                      uint32  src_start,
                      uint32 *dst,
                      uint32  dst_start,
                      uint32  len
                      )
{
    int i,j;
    for (i=src_start,j=dst_start;i<len +src_start ;i++,j++) {
        int bit1,bit2;
/* Result is 0 only if all bits in the range are 0 */
       SHR_BITTEST_RANGE(src, i, 1, bit1);
       SHR_BITTEST_RANGE(dst, j, 1, bit2);
       if (bit1 != bit2) {
            return 0 ;          

       }

    }

    return 1;
}

static soc_error_t print_data(
                       int unit,
                       const uint32 len_bits,
                       const uint8 *data,
                       const char *data_name,
                       int debug)
{
  uint32 i;
  char *buf;

  DNXC_INIT_FUNC_DEFS;
 
  if ((debug)) {
      buf = sal_alloc(len_bits + 1, ""); 
      DNXC_NULL_CHECK(buf);
      *buf ='\0';
      for (i=0;i<len_bits;i++) {
          sal_strncat(buf, data[i] ? "1" : "0", len_bits+1 - sal_strlen(buf) - 1);
      }


      cli_out("%s(l2m):%s\n",data_name,buf);
      sal_free(buf);
  }

exit:
    DNXC_FUNC_RETURN;

}
static int get_ecc_bit_len(uint32 data_len_bits)
{
    uint32 ecc_len_bits;
    int pow;
    for (ecc_len_bits=0,pow=1;pow<(ecc_len_bits+data_len_bits+1);ecc_len_bits++,pow<<=1) {
    }
    return ecc_len_bits;
}


static void data2bits(const uint32 data_len_bits,const uint32 *data,uint8 *bits)
{

    int j,i,start,end;
    int data_len_words = data_len_bits/(8*sizeof(uint32)) + (data_len_bits%(8*sizeof(uint32)) ? 1 : 0);

    for (j=0,i=0;i<data_len_words;i++) {

        end = (i==(data_len_words-1) && (data_len_bits%(8*sizeof(uint32))) ? (data_len_bits%(8*sizeof(uint32))) : 8*sizeof(uint32));
        for (start=0;start<end;start++,j++) {
            bits[j] = (1<<start) & data[i] ? 1 : 0;
        }
    }
}


static void bits2data(const uint32 data_len_bits, uint32 *data,const uint8 *bits)
{

    int j,i,start,end;
    int data_len_words = data_len_bits/(8*sizeof(uint32)) + (data_len_bits%(8*sizeof(uint32)) ? 1 : 0);

    for (j=0,i=0;i<data_len_words;i++) {
        data[i]=0;

        end = (i==(data_len_words-1) && (data_len_bits%(8*sizeof(uint32))) ? (data_len_bits%(8*sizeof(uint32))) : 8*sizeof(uint32));
        for (start=0;start<end;start++,j++) {
            data[i] |= (bits[j] ? (1<<start) : 0);
        }
    }

}

static   soc_error_t interleave_data_ecc(
        int unit,
        const uint32 data_len_bits,
        const uint8 *data, 
        const uint8 *ecc,
        int debug)
{
    uint32 ecc_len_bits = get_ecc_bit_len(data_len_bits);
    uint32 total_len_bits = data_len_bits + ecc_len_bits;
    char *buf = NULL, *buf4 = NULL;
    char buf2[12],buf3[12];
    int count;
    uint32 i,j,k,pos[1];

    DNXC_INIT_FUNC_DEFS;

    if (debug) {
        buf = sal_alloc(ecc_len_bits * 5 + data_len_bits * 3 + 1, "interleave_data_ecc.buf"); 
        buf4 = sal_alloc(ecc_len_bits*5 + data_len_bits*3+1,"interleave_data_ecc.buf4");
        DNXC_NULL_CHECK(buf);
        DNXC_NULL_CHECK(buf4);

        *buf = '\0';
        *buf4 = '\0';

        for (i=0,j=0,k=0;i<total_len_bits;i++) {
            *pos = i+1;
            SHR_BITCOUNT_RANGE(pos,count,0,sizeof(uint32)*8);
            /* on ecc bit position(power of 2)*/
            if (count==1) {
                sal_sprintf(buf2,"-(%d)-",ecc[k]);
                sal_sprintf(buf3,"(%2d) ",*pos);
                k++;
            }
            else {
                sal_sprintf(buf2,"-%d-",data[j]);
                sal_sprintf(buf3,"%2d ",*pos);
                j++;
            }
            sal_strncat(buf, buf2, ecc_len_bits*5 + data_len_bits*3+1 - sal_strlen(buf) - 1);
            sal_strncat(buf4, buf3, ecc_len_bits*5 + data_len_bits*3+1 - sal_strlen(buf4) - 1);

        }
        cli_out("interleave daya,ecc%s\n",buf);
        cli_out("                   %s\n",buf4);
    }
exit:
    DNXC_FREE(buf); 
    DNXC_FREE(buf4); 
    DNXC_FUNC_RETURN;
}

uint8 calc_parity(
        const uint32 data_len_bits,
        const uint8 *data)
{
    uint8 parity=0;
    uint32 i;
    for (i=0;i<data_len_bits;i++) {
        parity ^= data[i];
    }

    return parity;
}

soc_error_t calc_ecc(
        int unit,
        const uint32 data_len_bits,
        const uint32 *data,
        uint32 *ecc_data)
{
    uint32 ecc_len_bits = get_ecc_bit_len(data_len_bits);
    uint8 *data_bits = sal_alloc(data_len_bits,"");
    uint8 *calc_ecc_bits = sal_alloc(ecc_len_bits+1,"");
    uint32 i,j,k,l, pos[1];
    int count, parity_check = 0;
    uint32 total_len_bits = data_len_bits + ecc_len_bits;

    DNXC_INIT_FUNC_DEFS;

    DNXC_NULL_CHECK(data_bits);
    DNXC_NULL_CHECK(calc_ecc_bits);

    sal_memset(calc_ecc_bits,0,ecc_len_bits+1);
    data2bits(data_len_bits, data, data_bits);

    /* calculating the expected ecc*/
    for (i=0,j=0;i<total_len_bits;i++) {
        *pos = i+1;
        SHR_BITCOUNT_RANGE(pos,count,0,sizeof(uint32)*8);
        if (count==1) {
            continue;
        }

        if (data_bits[j]) {
            for (l=0,k=1;k<*pos;k<<=1,l++) {
                if (!(k & *pos)) {
                    continue;
                }
                calc_ecc_bits[l] ^= 1;
            }
        }

        j++;
    }

    for (i = 0; i < data_len_bits; i++) {
        if (data_bits[i] == 1) {
            parity_check++;
        }
    }

    for (i = 0; i < ecc_len_bits; i++) {
        if (calc_ecc_bits[i] == 1) {
            parity_check++;
        }
    }

    /*Calculate parity bit*/
    if (parity_check % 2 != 0) {
        calc_ecc_bits[ecc_len_bits] = 1;
    }
    bits2data(ecc_len_bits+1,ecc_data,calc_ecc_bits);

exit:
    DNXC_FREE(calc_ecc_bits);
    DNXC_FREE(data_bits);

    DNXC_FUNC_RETURN;
}

static
soc_error_t ecc_correction_std(
        int unit,
        const uint32 data_len_bits,
        int xor_flag,
        uint32 *data, 
        uint32 *ecc)
{

    uint32 ecc_len_bits = get_ecc_bit_len(data_len_bits);
    uint8 *data_bits = sal_alloc(data_len_bits,"");
    uint8 *ecc_bits = sal_alloc(ecc_len_bits+1,"");
    uint8 *calc_ecc_bits = sal_alloc(ecc_len_bits+1,"");
    uint32 total_len_bits = data_len_bits + ecc_len_bits;
    uint32 i,j,k,l;
    int count;
    uint32 bit1;
    uint32 index=0;
    uint32 pos[1];
    uint32 bits_for_count[1];

    DNXC_INIT_FUNC_DEFS;
    DNXC_NULL_CHECK(data_bits);
    DNXC_NULL_CHECK(ecc_bits);
    DNXC_NULL_CHECK(calc_ecc_bits);

    sal_memset(calc_ecc_bits,0,ecc_len_bits);

    data2bits(data_len_bits, data, data_bits); 
    data2bits(ecc_len_bits+1,ecc,ecc_bits);

    DNXC_IF_ERR_EXIT(print_data(unit,data_len_bits, data_bits, "input    data", 0));
    DNXC_IF_ERR_EXIT(print_data(unit, ecc_len_bits+1, ecc_bits,  "input    ecc", 0));
    /* calculating the expected ecc*/
    for (i=0,j=0;i<total_len_bits;i++) {
        /* we start from 1*/
        *pos = i+1;
        SHR_BITCOUNT_RANGE(pos,count,0,sizeof(uint32)*8);
        /* on ecc bit position(power of 2)*/
        if (count==1) {
            continue;
        }
        /*extract current bit from data*/
        bit1 = data_bits[j];
        if (bit1) {
            /*  loop over all the ecc bits belong to current bit(the positions of binary representation of pos(i+1) )*/
            for (l=0,k=1;k<*pos;k<<=1,l++) {
                if (!(k & *pos)) {
                    continue;
                }
                /* xor the current ecc bit at pos l with the current data bit at pos j*/
                calc_ecc_bits[l] ^= 1;
            }
        }

        /* advance to next data bit*/
        j++;


    }
    DNXC_IF_ERR_EXIT(interleave_data_ecc(unit, data_len_bits,data_bits,ecc_bits, 0));
    DNXC_IF_ERR_EXIT(interleave_data_ecc(unit, data_len_bits,data_bits,calc_ecc_bits, 0));

    /* calc number of diffs between input ecc to calculated ecc*/
    for (i=0,count=0;i<ecc_len_bits;i++) {
        if (ecc_bits[i]!=calc_ecc_bits[i]) {
            *pos = i;
            count++;
        }
    }
    if (!count) {
        if (xor_flag == 0) {
            cli_out("error found in parity bit of ecc\n"); 
            DNXC_IF_ERR_EXIT(print_data(unit, data_len_bits, data_bits, "corrcted data", 0));
            DNXC_IF_ERR_EXIT(print_data(unit, ecc_len_bits+1, calc_ecc_bits, "calc     ecc", 0));
        }

        SOC_EXIT;
    }

    if (count==1) {
        cli_out("the error bit is in ecc field at pos %d\n",*pos); 
        bits2data(ecc_len_bits,ecc,calc_ecc_bits);
        DNXC_IF_ERR_EXIT(print_data(unit, ecc_len_bits, calc_ecc_bits, "correct  ecc", 0));
        cli_out("corrected value %x\n",ecc[0]); 
        SOC_EXIT;
    }
 /* calculate thoe position of the faulse bit in the interleaved bit array*/
    for (i=0;i<ecc_len_bits;i++) {
        if (ecc_bits[i] ==  calc_ecc_bits[i]) {
            continue;
        }
        index += (1<<i);
    }

    /* fix to original index(subtract all location of power 2) from index*/
    for (i=4,j=3;i<index;i<<=1,j++) {
    }

    j=-1;
    for (i=0;i<=index;i++) {
        *bits_for_count = i;
        SHR_BITCOUNT_RANGE(bits_for_count,count,0,sizeof(uint32)*8);
        /* on ecc bit position(power of 2)*/
        if (count<=1) {
            continue;
        }
        j++;
    }
    index =j;
    /* correct the errornous bit*/
    if (index < data_len_bits) {
        data_bits[index] = !data_bits[index];
    }

    DNXC_IF_ERR_EXIT(print_data(unit, data_len_bits, data_bits, "corrcted data", 0));

    DNXC_IF_ERR_EXIT(print_data(unit, ecc_len_bits+1, calc_ecc_bits, "calc     ecc", 0));

    bits2data(data_len_bits,data,data_bits);
    cli_out("corrected data value %x\n",data[0]); 

exit:
    sal_free(calc_ecc_bits);
    sal_free(data_bits);
    DNXC_FREE(ecc_bits);
    DNXC_FUNC_RETURN;
}

static
soc_error_t ecc_correction_wide(
        int unit,
        const uint32 data_len_bits,
        int xor_flag,
        uint32 *data, 
        uint32 *ecc)
{

    uint32 data_portions_array[WIDE_ECC_MAX_INTERVALS]={0};

    uint32 tmp_data_portion[WIDE_ECC_MAX_DATA_WIDTH]={0};
    uint32 tmp_ecc_portion[WIDE_ECC_MAX_ECC_WIDTH]={0};
    uint32 tmp_data_portion_orig[WIDE_ECC_MAX_DATA_WIDTH]={0};
    uint32 tmp_ecc_portion_orig[WIDE_ECC_MAX_ECC_WIDTH]={0};

    uint32 nof_portions=0;
    uint32 total_width = data_len_bits;
    int min_portion = 247;
    int max_portion = 502;
    int i,j;
    int ecc_pos=0,data_pos=0;

    

    DNXC_INIT_FUNC_DEFS;


    while (total_width > 0) {
          if (total_width > (max_portion+min_portion)) {

             data_portions_array[nof_portions++] = max_portion;
             total_width -= max_portion;
          } else if (total_width > max_portion) {
              if (SOC_IS_JERICHO_AND_BELOW(unit)) {
                  data_portions_array[nof_portions++] = (total_width - min_portion);
                  total_width -= (total_width-min_portion); 
              }
              else
              {
                  data_portions_array[nof_portions++] = min_portion;
                  total_width -= min_portion; 
              }
          } else {
              data_portions_array[nof_portions++] = total_width;
              total_width = 0;
          }
       }


    for (j=nof_portions-1,i=0;i<nof_portions;i++,j--) {
        int data_portion_len = data_portions_array[i];
        uint32 ecc_portion_len = get_ecc_bit_len(data_portion_len) + 1;



        copy_bits(data,data_pos,tmp_data_portion,0,data_portion_len);
        copy_bits(data,data_pos,tmp_data_portion_orig,0,data_portion_len);

        copy_bits(ecc,ecc_pos,tmp_ecc_portion,0,ecc_portion_len);
        copy_bits(ecc,ecc_pos,tmp_ecc_portion_orig,0,ecc_portion_len);


        DNXC_IF_ERR_EXIT(ecc_correction_std(unit,data_portion_len,xor_flag, tmp_data_portion, tmp_ecc_portion));

         if (!is_eq_bits(tmp_data_portion,0,tmp_data_portion_orig,0,data_portion_len) ) {
            copy_bits(tmp_data_portion,0,data,data_pos,data_portion_len);
                 SOC_EXIT;
            }

        if (!is_eq_bits(tmp_ecc_portion,0,tmp_ecc_portion_orig,0,ecc_portion_len) ) {
                 SOC_EXIT;
            }

        data_pos+=data_portion_len;
        ecc_pos+=ecc_portion_len;

    }



exit:
    DNXC_FUNC_RETURN;
}

soc_error_t ecc_correction(
        int unit,
        soc_mem_t mem,
        const uint32 data_len_bits,
        int xor_flag,
        uint32 *data, 
        uint32 *ecc)
{


    DNXC_INIT_FUNC_DEFS;
    if (SOC_MEM_ECC_TYPE(unit,mem) ==SOC_MEM_ECC_TYPE_WIDE ) {
        DNXC_IF_ERR_EXIT(ecc_correction_wide(unit,data_len_bits,xor_flag,data, ecc));

    }
    else {
        DNXC_IF_ERR_EXIT(ecc_correction_wide(unit,data_len_bits,xor_flag,data, ecc));
    }
exit:
    DNXC_FUNC_RETURN;
}

