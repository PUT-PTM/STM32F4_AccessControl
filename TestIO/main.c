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


	  x:
	  LCD_WriteText((unsigned char *)"Podaj ID:[A/C] \0");
	  //A-accept
	  //C-cancel
	  //goto nowa linia
	  //****
	  //if C -> goto x;
	  //if A -> check id
	  //true -> Podaj pin
	  //false -> "bledne id"
	  //true -> "Otworz drzwi"
	  //false -> "Bledny pin"

	  goto x;
	  while (1) {
	    Tekst[0]=KBD_ReadKey();
	    if (Tekst[0] !=0 ){
	    	LCD_WriteTextXY(Tekst,9,0);
	    }
	  };
	  return 0;
}
