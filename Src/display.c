/*
 * display.c
 *
 *  Created on: Nov 23, 2020
 *      Author: Stancoj
 */

#include "display.h"
#include <string.h>

void updateDisplay(void);
void setDigit(uint8_t pos);
void setSign(uint8_t cislo);
unsigned char decode_7seg(unsigned char chr);

const unsigned char seven_seg_digits_decode_gfedcba[75]= {
/*  0     1     2     3     4     5     6     7     8     9     :     ;     */
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x00,
/*  <     =     >     ?     @     A     B     C     D     E     F     G     */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D,
/*  H     I     J     K     L     M     N     O     P     Q     R     S     */
    0x76, 0x30, 0x1E, 0x75, 0x38, 0x55, 0x54, 0x5C, 0x73, 0x67, 0x50, 0x6D,
/*  T     U     V     W     X     Y     Z     [     \     ]     ^     _     */
    0x78, 0x3E, 0x1C, 0x1D, 0x64, 0x6E, 0x5B, 0x00, 0x00, 0x00, 0x00, 0x08,
/*  `     a     b     c     d     e     f     g     h     i     j     k     */
    0x00, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D, 0x76, 0x30, 0x1E, 0x75,
/*  l     m     n     o     p     q     r     s     t     u     v     w     */
    0x38, 0x55, 0x54, 0x5C, 0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x1C, 0x1D,
/*  x     y     z     */
    0x64, 0x6E, 0x5B
};


char curr_str[SIZE_DISPLAY];
int idx = 0;
int inc = 1;
uint64_t disp_time = 0;
uint64_t disp_time_saved = 0;

uint32_t pin[8] ={
		SEGMENTA_PIN,
		SEGMENTB_PIN,
		SEGMENTC_PIN,
		SEGMENTD_PIN,
		SEGMENTE_PIN,
		SEGMENTF_PIN,
		SEGMENTG_PIN,
		SEGMENTDP_PIN
};

GPIO_TypeDef *port[8] = {
		SEGMENTA_PORT,
		SEGMENTB_PORT,
		SEGMENTC_PORT,
		SEGMENTD_PORT,
		SEGMENTE_PORT,
		SEGMENTF_PORT,
		SEGMENTG_PORT,
		SEGMENTDP_PORT
};


/*Reset (turn-off) all the segments of display*/
void resetSegments(void)
{
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_11);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_5);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_1);
	//LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_3);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_15);
}

/*Set (turn-on) all the segments of display*/
void setSegments(void)
{
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_11);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_8);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_1);
	//LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_3);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_15);
}

/* Reset (turn-off) all digits*/
void resetDigits(void)
{
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_12);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_6);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7);
}

/* Reset (turn-on) all digits*/
void setDigits(void)
{
	DIGIT_1_ON;
	DIGIT_2_ON;
	DIGIT_3_ON;
	DIGIT_4_ON;
	DIGIT_TIME_ON;
}

void changeDisplayString(char *string, int len){

  for(int i=0; i<SIZE_DISPLAY; i++)
  {
	  curr_str[i] = string[idx+i];
  }

  if(idx < (len-SIZE_DISPLAY) && inc)
  {
	  idx++;
	  if(idx == (len-SIZE_DISPLAY)){
		  inc = 0;
		  return;
	  }
  }
  if(idx > 0 && !inc){
	  idx--;
	  if(idx == 0){
		  inc = 1;
	  }
  }
}

void setDigit(uint8_t pos)
{
	switch(pos)
	{
		case 0:
			DIGIT_1_ON;
			break;
		case 1:
			DIGIT_2_ON;
			break;
		case 2:
			DIGIT_3_ON;
			break;
		case 3:
			DIGIT_4_ON;
			break;
	}
}

unsigned char decode_7seg(unsigned char chr)
{ /* Implementation uses ASCII */
	if (chr == (unsigned char)'-')
	        return 0x40;
    if (chr == (unsigned char)'.')
        return 0x80;
    if (chr > (unsigned char)'z')
        return 0x00;
    return seven_seg_digits_decode_gfedcba[chr - '0'];
}

void setSign(unsigned char cislo){
	uint8_t act_segment;
	for(int i = 0; i < 8; i++){
		act_segment = cislo & (1 << i);

		if(act_segment){
			LL_GPIO_ResetOutputPin(port[i], pin[i]);
		}
	}
}

void startNewStr(){
	idx = 0;
	inc = 1;
}

void updateDisplay(void)
{
	for(uint8_t i = 0; i < SIZE_DISPLAY; i++)
	{
		setDigit(i);

		setSign(decode_7seg(curr_str[i]));

//		disp_time_saved = disp_time;
//		while((disp_time_saved + 1) > disp_time){};
		LL_mDelay(1);

		resetDigits();
		resetSegments();
	}
}

//Update displayed data and keep display ON
void TIM2_IRQHandler(void)
{
	if(LL_TIM_IsActiveFlag_UPDATE(TIM2))
	{
		updateDisplay();
	}

	LL_TIM_ClearFlag_UPDATE(TIM2);
}

