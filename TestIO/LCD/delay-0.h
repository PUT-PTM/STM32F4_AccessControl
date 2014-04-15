#ifndef __DELAY_H__
#define __DELAY_H__
#include "stm32f4xx.h"

void delay_ms(__IO uint32_t nCount);

void delay_1ms(void);
void delay_10us(void);

#endif

