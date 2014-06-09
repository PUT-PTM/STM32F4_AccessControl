#ifndef eeprom_h
#define eeprom_h

#include "lcd_hd44780.h"
#include "stm32f4xx_rcc.h"
#include "kbd_4x4.h"
#include <stm32f4xx.h>
#include <misc.h>			 // I recommend you have a look at these in the ST firmware folder
#include "cmsis_lib/include/stm32f4xx_i2c.h"//c
#include "delay/delay.h"
#include<stdint.h>
#include<stdlib.h>
#include"stm32f4xx_gpio.h"

/*
//#include <stm32f4xx.h>
//#include <misc.h>			 // I recommend you have a look at these in the ST firmware folder
//#include <stm32f4xx_i2c.c> // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src
#include "cmsis_lib/include/stm32f4xx_i2c.h"//c

//dodalem o 23
#include "delay/delay.h"
#include<stdint.h>
#include<stdlib.h>
//koniec dodalem o23
*/
#define I2C_TIMEOUT_MAX 10000
#define MEM_DEVICE_WRITE_ADDR 0xA0
#define MEM_DEVICE_READ_ADDR 0xA1


enum  eepromtype  {M2401,M2402,M2404,M2408,M2416,M2432,M2464,M24128,M24256,M24512};

//void Delay(__IO uint32_t); // __IO => Volatile

void init_I2C1(void);

uint8_t Write_24Cxx(uint16_t, uint8_t, uint8_t);

uint8_t Read_24Cxx(uint16_t, uint8_t);

void wyswietl_gwiazdki(int ile);
int porownaj_id(int *a, int *b);
int adres(int numer_rekordu);
int znajdz_id(int num);
void wczytaj_haslo(int indeks);
int wczytaj_id();


#endif
