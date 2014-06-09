#ifndef delay_c
#define delay_c

#include "delay.h"

static inline void delay_us_core(void);

void delay_ms(__IO uint32_t nCount)
{
  while(nCount--)
  {
	  delay_1ms();
  }
}

void delay_4us(__IO uint32_t nCount)  //3us
{
	//delay_us_core();    //4us
	//asm volatile("nop"::);
	uint16_t i=1;
	while(i--) asm volatile("nop"::);
}

void delay_1ms(void)
{
	uint32_t nCount;
	nCount=100;
	  while(nCount--)
	  {
		  delay_10us();
	  }
}

void delay_x10us(__IO uint32_t nCount)
{
  //uint32_t nCount;
  //nCount=9;
  while(nCount--)
  {
	  delay_10us();
  }
}

void delay_10us(void)
{
  uint32_t nCount;
  nCount=9;
  while(nCount--)
  {
  }
}

static inline void delay_us_core(void)
{
	//uint16_t i=1;
	//while(i--) asm volatile("nop"::);
	asm volatile("nop"::);
}
#endif
