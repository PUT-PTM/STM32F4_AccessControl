#include "stm32f4xx.h"
#include "delay.h"

static inline void delay_us_core(void);


void delay_ms(__IO uint32_t nCount)
{
  while(nCount--)
  {
	  delay_1ms();
  }
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
	uint16_t i=34;
	while(i--) asm volatile("nop"::);
}
