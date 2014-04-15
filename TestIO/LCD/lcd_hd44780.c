//zapisaæ skad kod
#include "delay.h"
#include "lcd_hd44780.h"
#include "stm32f4xx_gpio.h"

#define LCD_DELAY_MS(a)	delay_ms(a)

#define LCD_GPIO GPIOE
#define LCD_RS GPIO_Pin_8  //4
#define LCD_RW GPIO_Pin_9   //pin 5  do GND
#define LCD_EN GPIO_Pin_10  //6

#define LCD_D4 GPIO_Pin_11  //11
#define LCD_D5 GPIO_Pin_12
#define LCD_D6 GPIO_Pin_13
#define LCD_D7 GPIO_Pin_14  //14

#define LCD_ENA_1		LCD_GPIO->BSRRL = LCD_EN
#define LCD_ENA_0		LCD_GPIO->BSRRH = LCD_EN
#define LCD_RS_1		LCD_GPIO->BSRRL = LCD_RS
#define LCD_RS_0		LCD_GPIO->BSRRH = LCD_RS
#define LCD_DB4_1		LCD_GPIO->BSRRL = LCD_D4;
#define LCD_DB4_0		LCD_GPIO->BSRRH = LCD_D4;
#define LCD_DB5_1		LCD_GPIO->BSRRL = LCD_D5;
#define LCD_DB5_0		LCD_GPIO->BSRRH = LCD_D5;
#define LCD_DB6_1		LCD_GPIO->BSRRL = LCD_D6;
#define LCD_DB6_0		LCD_GPIO->BSRRH = LCD_D6;
#define LCD_DB7_1		LCD_GPIO->BSRRL = LCD_D7;
#define LCD_DB7_0		LCD_GPIO->BSRRH = LCD_D7;

GPIO_InitTypeDef GPIO_InitStructure;

//-----------------------------------------------------------------------------
void LCD_WriteNibble(unsigned char nibbleToWrite)
{
  GPIO_WriteBit(LCD_GPIO, LCD_EN, Bit_SET);
  GPIO_WriteBit(LCD_GPIO, LCD_D4, (nibbleToWrite & 0x01));
  GPIO_WriteBit(LCD_GPIO, LCD_D5, (nibbleToWrite & 0x02));
  GPIO_WriteBit(LCD_GPIO, LCD_D6, (nibbleToWrite & 0x04));
  GPIO_WriteBit(LCD_GPIO, LCD_D7, (nibbleToWrite & 0x08));
  GPIO_WriteBit(LCD_GPIO, LCD_EN, Bit_RESET);
}

unsigned char LCD_ReadNibble(void)
{
  unsigned char tmp = 0;
  GPIO_WriteBit(LCD_GPIO, LCD_EN, Bit_SET);
  tmp |= (GPIO_ReadInputDataBit(LCD_GPIO, LCD_D4) << 0);
  tmp |= (GPIO_ReadInputDataBit(LCD_GPIO, LCD_D5) << 1);
  tmp |= (GPIO_ReadInputDataBit(LCD_GPIO, LCD_D6) << 2);
  tmp |= (GPIO_ReadInputDataBit(LCD_GPIO, LCD_D7) << 3);
  GPIO_WriteBit(LCD_GPIO, LCD_EN, Bit_RESET);
  return tmp;
}


//-----------------------------------------------------------------------------
unsigned char LCD_ReadStatus(void)
{
  unsigned char status = 0;

  GPIO_InitStructure.GPIO_Pin   =  LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(LCD_GPIO, &GPIO_InitStructure);

  GPIO_WriteBit(LCD_GPIO, LCD_RW, Bit_SET);
  GPIO_WriteBit(LCD_GPIO, LCD_RS, Bit_RESET);

  status |= (LCD_ReadNibble() << 4);
  status |= LCD_ReadNibble();

  GPIO_InitStructure.GPIO_Pin   =  LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_Init(LCD_GPIO, &GPIO_InitStructure);

  return status;
}


//-----------------------------------------------------------------------------
void LCD_WriteData(unsigned char dataToWrite)
{
  GPIO_WriteBit(LCD_GPIO, LCD_RW, Bit_RESET);
  GPIO_WriteBit(LCD_GPIO, LCD_RS, Bit_SET);

  LCD_WriteNibble(dataToWrite >> 4);
  LCD_WriteNibble(dataToWrite & 0x0F);
  delay_ms(50);
  //while(LCD_ReadStatus() & 0x80);
}


//-----------------------------------------------------------------------------
void LCD_WriteCommand(unsigned char commandToWrite)
{
  GPIO_WriteBit(LCD_GPIO, LCD_RW | LCD_RS, Bit_RESET);
  LCD_WriteNibble(commandToWrite >> 4);
  LCD_WriteNibble(commandToWrite & 0x0F);
  delay_ms(50);
  //while(LCD_ReadStatus() & 0x80);
}


//-----------------------------------------------------------------------------
void LCD_WriteText(unsigned char * text)
{
  while(*text)
    LCD_WriteData(*text++);
}


//-----------------------------------------------------------------------------
void LCD_GoTo(unsigned char x, unsigned char y)
{
  LCD_WriteCommand(HD44780_DDRAM_SET | (x + (0x40 * y)));
}


//-----------------------------------------------------------------------------
void LCD_WriteTextXY(unsigned char * text, unsigned char x, unsigned char y)
{
  LCD_GoTo(x,y);
  while(*text)
    LCD_WriteData(*text++);
}


//-----------------------------------------------------------------------------
void LCD_WriteBinary(unsigned int var, unsigned char bitCount)
{
  signed char i;

  for(i = (bitCount - 1); i >= 0; i--)
     {
     LCD_WriteData((var & (1 << i))?'1':'0');
     }
}


//-----------------------------------------------------------------------------
void LCD_ShiftLeft(void)
{
  LCD_WriteCommand(HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_LEFT | HD44780_SHIFT_DISPLAY);
}


//-----------------------------------------------------------------------------
void LCD_ShiftRight(void)
{
  LCD_WriteCommand(HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_RIGHT | HD44780_SHIFT_DISPLAY);
}


//-----------------------------------------------------------------------------
void LCD_PinInit(void)
{
	GPIO_InitStructure.GPIO_Pin   =  LCD_D4|LCD_D5|LCD_D6|LCD_D7|LCD_RS|LCD_RW|LCD_EN;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(LCD_GPIO, &GPIO_InitStructure);
	delay_ms(1);
}

void LCD_Initialize(void)
{
  volatile unsigned char i = 0;
  //volatile unsigned int delayCnt = 0;
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  LCD_PinInit();
  GPIO_ResetBits(LCD_GPIO, LCD_RS | LCD_EN | LCD_RW);
  delay_ms(10);
  //for(delayCnt = 0; delayCnt < 300000; delayCnt++);
  
  for(i = 0; i < 3; i++) {
    LCD_WriteNibble(0x03);
    delay_ms(5);
  }
  
  LCD_WriteNibble(0x02);             
  delay_ms(1);
     
  LCD_WriteCommand(HD44780_FUNCTION_SET | 
                   HD44780_FONT5x7 | 
                   HD44780_TWO_LINE | 
                   HD44780_4_BIT);
  
  LCD_WriteCommand(HD44780_DISPLAY_ONOFF | 
                   HD44780_DISPLAY_OFF); 
  
  LCD_WriteCommand(HD44780_CLEAR); 
  
  LCD_WriteCommand(HD44780_ENTRY_MODE | 
                   HD44780_EM_SHIFT_CURSOR | 
                   HD44780_EM_INCREMENT);
  
  LCD_WriteCommand(HD44780_DISPLAY_ONOFF | 
                   HD44780_DISPLAY_ON |
                   HD44780_CURSOR_OFF | 
                   HD44780_CURSOR_NOBLINK);
}


//-----------------------------------------------------------------------------
void LCD_SetUserChar (unsigned char chrNum, unsigned char n, const unsigned char *p)
{         //chrNum  - character number (code) to be registered (0..7)
          //n       - number of characters to register
          //*p      - pointer to the character pattern (8 * n bytes)
	LCD_WriteCommand(HD44780_CGRAM_SET | chrNum * 8);
	n *= 8;
	do
		LCD_WriteData(*p++);
	while (--n);
}

void lcd_test(void)
{
	LCD_Initialize();                   //inicjalizacja wysietlacza
	LCD_WriteCommand(HD44780_CLEAR);    //wyczysc wyswietlacz
	LCD_WriteText("Odliczanie:\0");

}
//-----------------------------------------------------------------------------
