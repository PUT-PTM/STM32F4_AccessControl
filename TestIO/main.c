#include "stm32f4xx_rcc.h"
#include "lcd_hd44780.h"
#include "kbd_4x4.h"


int main(void)
{
	unsigned char Tekst[2] = {"\0"};


	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  //LCD KBD
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //KBD

	  KBD_Init();
	  LCD_Initialize();                           //inicjalizacja wysietlacza
	  LCD_WriteCommand(HD44780_CLEAR);            //wyczysc wyswietlacz
	  LCD_WriteText((unsigned char *)"Klawisz: \0");

	  while (1) {
	    Tekst[0]=KBD_ReadKey();
	    if (Tekst[0] !=0 ){
	    	LCD_WriteTextXY(Tekst,9,0);
	    }
	  };
	  return 0;
}
