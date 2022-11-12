/*
 * lsm6ds0.h
 *
 *  Created on: 17. 11. 2019
 *      Author: Stancoj
 */

#include "main.h"
#include "i2c.h"


#define 	HTS221_DEVICE_ADDRESS_0					0xBE
//#define 	HTS221_DEVICE_ADDRESS_1					0xBA

#define 	HTS221_WHO_AM_I_VALUE					0xBC
#define 	HTS221_WHO_AM_I_ADDRES					0x0F

#define 	HTS221_ADDRESS_CTRL1					0x20

#define 	HTS221_ADDRESS_HUMIDITY_OUT_L			0x28
#define 	HTS221_ADDRESS_HUMIDITY_OUT_H			0x29

#define 	HTS221_ADDRESS_H0_rH_x2					0x30
#define 	HTS221_ADDRESS_H1_rH_x2					0x31

#define 	HTS221_ADDRESS_H0_T0_OUT				0x36
//#define 	HTS221_ADDRESS_H0_T0_OUT				0x37
#define 	HTS221_ADDRESS_H1_T0_OUT				0x3A

//#define 	HTS221_ADDRESS_TEMP_OUT_L				0x2A
//#define 	HTS221_ADDRESS_TEMP_OUT_H				0x2B

uint8_t hts221_init(void);
uint8_t hts221_read_byte(uint8_t reg_addr);
void hts221_write_byte(uint8_t reg_addr, uint8_t value);
float hts221_get_humidity();
//int8_t lsm6ds0_get_temp();
