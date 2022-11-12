/*
 * lsm6ds0.c
 *
 *  Created on: 17. 11. 2019
 *      Author: Stancoj
 */

#include "hts221.h"

uint8_t addres3 = HTS221_DEVICE_ADDRESS_0;
int H0;
int H1;
int H2;
int H3;

uint8_t hts221_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, addres3, 0));
}


void hts221_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, addres3, 0);
}


void hts221_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, addres3, 0);
}

float hts221_get_humidity(){

	uint8_t data[2];
	hts221_readArray(data, HTS221_ADDRESS_HUMIDITY_OUT_L, 2);
	int hum = (data[1]*256) + data[0];

//	return (temp[1] * 256.0) + temp[0];
//	return (((uint8_t)(~((uint16_t)(temp[1] << 8 | temp[0]))+1))*0.004);
	return ((1.0 * H1) - (1.0 * H0)) * (1.0 * hum - 1.0 * H2) / (1.0 * H3 - 1.0 * H2) + (1.0 * H0);
}


uint8_t hts221_init(void)
{

	uint8_t status = 1;
	uint8_t data[2];

	LL_mDelay(100);

	uint8_t val = hts221_read_byte(HTS221_WHO_AM_I_ADDRES);

	if(val == HTS221_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		status = 0;
	}

	uint8_t ctrl1 = hts221_read_byte(HTS221_ADDRESS_CTRL1);
	ctrl1 &= ~0xFC;
	ctrl1 |= 0x70;
	hts221_write_byte(HTS221_ADDRESS_CTRL1, ctrl1);

	hts221_readArray(data, HTS221_ADDRESS_H0_rH_x2, 2);
//	data[1] = hts221_read_byte(HTS221_ADDRESS_H1_rH_x2);

	H0 = data[0]/2;
	H1 = data[1]/2;

	hts221_readArray(data, HTS221_ADDRESS_H0_T0_OUT, 2);
	H2 = (data[1] * 256) + data[0];

	hts221_readArray(data, HTS221_ADDRESS_H1_T0_OUT, 2);
	H3 = (data[1] * 256) + data[0];

	return status;
}
