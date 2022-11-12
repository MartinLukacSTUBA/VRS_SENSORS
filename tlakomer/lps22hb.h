#include "main.h"
#include "i2c.h"
#include <math.h>



#define 	LPS22HB_DEVICE_ADDRESS_0				0xB8
#define 	LPS22HB_DEVICE_ADDRESS_1				0xBA

#define 	LPS22HB_WHO_AM_I_VALUE					0xB1
#define 	LPS22HB_WHO_AM_I_ADDRES					0x0F

#define 	LPS22HB_ADDRESS_CTRL1					0x10

#define 	LPS22HB_ADDRESS_PRESS_OUT_XL			0x28
#define 	LPS22HB_ADDRESS_PRESS_OUT_L				0x29
#define 	LPS22HB_ADDRESS_PRESS_OUT_H				0x2A

#define 	LPS22HB_ADDRESS_TEMP_OUT_L				0x2B
//#define 	LPS22HB_ADDRESS_TEMP_OUT_H				0x2C

uint8_t lps22hb_init(void);
uint8_t lps22hb_read_byte(uint8_t reg_addr);
void lps22hb_write_byte(uint8_t reg_addr, uint8_t value);

float lps22hb_get_temp();
float lps22hb_get_press();
float lps22hb_get_altitude();
double generalPower (double base, double power);
//int8_t lsm6ds0_get_temp();




