/* $Id: utils_front_end_host_card.h,v 1.2 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/


#ifndef __UTILS_FRONT_END_HOST_CARD_H_INCLUDED__
/* { */
#define __UTILS_FRONT_END_HOST_CARD_H_INCLUDED__

#ifdef  __cplusplus
extern "C" {
#endif

void
  front_end_FPGAs_power_on(
    const unsigned int power_on,
    const unsigned int verbose
  );

int
  front_does_card_exist_app(
    void
  );

int
  front_end_get_host_board_type(
    unsigned long* host_board_type
  );

#ifdef  __cplusplus
}
#endif


/* } __UTILS_FRONT_END_HOST_CARD_H_INCLUDED__*/
#endif
