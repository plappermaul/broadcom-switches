/*
 * $Id: dcmn_utils_eeprom.h,v 1.3 Broadcom SDK $ 
 *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * DCMN eeprom utils
 */
 
#ifndef _SOC_DCMN_UTILS_EEPROM_H_
#define _SOC_DCMN_UTILS_EEPROM_H_



int eeprom_write(unsigned short card_type, int addr, int size, int val);
int eeprom_write_str(unsigned short card_type, int addr, int size, char *str);
int eeprom_read(unsigned short card_type, int addr, int size, int *val);
int eeprom_read_str(unsigned short card_type, int addr, int size, char *str);

#define NEGEV_CHASSIS_EEPROM_NUM_MAX_ADRESS                  512
#define NEGEV_CHASSIS_EEPROM_NUM_ADRESS_PER_BANK          256
#define NEGEV_CHASSIS_EEPROM_ADRESS_BANK_1                        0x52
#define NEGEV_CHASSIS_EEPROM_ADRESS_BANK_2                        0x53

#define NEGEV_CHASSIS_EEPROM_ADRESS_BOARD_TYPE                0
#define NEGEV_CHASSIS_EEPROM_ADRESS_BOARD_SN                 4
#define NEGEV_CHASSIS_EEPROM_ADRESS_PCB_VERSION         8
#define NEGEV_CHASSIS_EEPROM_ADRESS_ASSEMBLY_VERSION    12
#define NEGEV_CHASSIS_EEPROM_ADRESS_BOARD_DESC                 16

#define NEGEV_CHASSIS_EEPROM_BYTE_SIZE_BOARD_TYPE                4
#define NEGEV_CHASSIS_EEPROM_BYTE_SIZE_ADRESS_BOARD_DESC                 30

#endif /* _SOC_DCMN_UTILS_EEPROM_H_ */
