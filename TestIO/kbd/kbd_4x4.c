//******************************************************************************
//    THE SOFTWARE INCLUDED IN THIS FILE IS FOR GUIDANCE ONLY.
//    AUTHOR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
//    OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
//    FROM USE OF THIS SOFTWARE.
//
//    PROGRAM ZAWARTY W TYM PLIKU PRZEZNACZONY JEST WYLACZNIE
//    DO CELOW SZKOLENIOWYCH. AUTOR NIE PONOSI ODPOWIEDZIALNOSCI
//    ZA ZADNE EWENTUALNE, BEZPOSREDNIE I POSREDNIE SZKODY
//    WYNIKLE Z JEGO WYKORZYSTANIA.
//******************************************************************************

#include "kbd_4x4.h"
#include "delay.h"
#include "stm32f4xx_gpio.h"

// z³acze C1,C2,C3,C4,R1,R2,R3,R4

#define KBD_Cols GPIOB     //Keyboard GPIO port
#define KBD_C1 GPIO_Pin_4  //4  //GIPO pins connected to keyboard column lines
#define KBD_C2 GPIO_Pin_5  //5
#define KBD_C3 GPIO_Pin_7  //7
#define KBD_C4 GPIO_Pin_8  //8

#define KBD_Rows GPIOE
#define KBD_R1 GPIO_Pin_3  //3 //GIPO pins connected to keyboard row lines
#define KBD_R2 GPIO_Pin_4  //4
#define KBD_R3 GPIO_Pin_5  //5
#define KBD_R4 GPIO_Pin_6  //6
                           //Keys mapping - keys ASCII codes
#define KBD_R1C1 0x31 //1
#define KBD_R1C2 0x32 //2
#define KBD_R1C3 0x33 //3
#define KBD_R1C4 0x49 //A

#define KBD_R2C1 0x34 //4
#define KBD_R2C2 0x35 //5
#define KBD_R2C3 0x36 //6
#define KBD_R2C4 0x51 //B

#define KBD_R3C1 0x37 //7
#define KBD_R3C2 0x38 //8
#define KBD_R3C3 0x39 //9
#define KBD_R3C4 0x55 //C

#define KBD_R4C1 0x2C //*
#define KBD_R4C2 0x40 //0
#define KBD_R4C4 0x53 //D
#define KBD_R4C3 0x54 //#

const char keyMap[4][4]={{KBD_R1C1, KBD_R1C2, KBD_R1C3, KBD_R1C4},
                         {KBD_R2C1, KBD_R2C2, KBD_R2C3, KBD_R2C4},
                         {KBD_R3C1, KBD_R3C2, KBD_R3C3, KBD_R3C4},
                         {KBD_R4C1, KBD_R4C2, KBD_R4C3, KBD_R4C4}};

const uint16_t GPIORows[4]={KBD_R1, KBD_R2, KBD_R3, KBD_R4};

const uint16_t GPIOCols[4]={KBD_C1, KBD_C2, KBD_C3, KBD_C4};

void KBD_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

  GPIO_InitStructure.GPIO_Pin = KBD_R1 | KBD_R2 | KBD_R3 | KBD_R4;
 	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(KBD_Rows, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = KBD_C1 | KBD_C2 | KBD_C3 | KBD_C4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(KBD_Cols, &GPIO_InitStructure);

  GPIO_SetBits(KBD_Cols, KBD_C1 | KBD_C2 | KBD_C3 | KBD_C4);
}

unsigned char KBD_ReadKey(void)
{ //returns ASCII code of a pressed key
  short r, c;
  unsigned short pressedKey=0x00;

  r=0;
  while (r<4) {
    //GPIO_ResetBits(KBD_GPIO, GPIORows[r]); //GPIO_Pin_11); // GPIORows[r]);            //activate row
	KBD_Rows->BSRRH = GPIORows[r];
	delay_ms(1);
	c=0;
    while (c<4) {
      if (GPIO_ReadInputDataBit(KBD_Cols, GPIOCols[c])==0){  //check columns
        pressedKey=keyMap[r][c];                          //if column active - find key value 
        delay_ms(1);
        //GPIO_WriteBit(KBD_GPIO, GPIORows[r], SET);          //deactivate row
        KBD_Rows->BSRRL = GPIORows[r];
        return pressedKey;
      }
      c++;
    }
    KBD_Rows->BSRRL = GPIORows[r];
    delay_ms(1);
    //GPIO_SetBits(KBD_GPIO, GPIO_Pin_11);  //GPIORows[r]);          //deactivate row
    r++;
  }
  //pressedKey = 0x2a;
  return pressedKey;                                      //return key ASCII value
}
