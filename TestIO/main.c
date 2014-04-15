#include "stm32f4xx_rcc.h"
#include "lcd_hd44780.h"


int main(void)
{
	unsigned char Tekst[2] = {"\0"};


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  //LCD

	delay_ms(500);
	LCD_Initialize();                           //inicjalizacja wysietlacza
	LCD_WriteCommand(HD44780_CLEAR);            //wyczysc wyswietlacz
	//LCD_WriteText((unsigned char *)"Hel45lo Wordl \0");

	const unsigned int max=17;
	unsigned char text[max];//={"abcdefghijklmnoprst"};
	unsigned char a='a';
	unsigned int i=0;
	while(i<max)
	{
		text[i]=(unsigned char)((int)a+i);
		i++;
	}
	unsigned char wyswietl[max];
	unsigned int start=0;

	while(1)
	{
		unsigned int ile_brakuje=max-start;
		i=start;
		while( i < max )
		{
			wyswietl[i]=text[i];
			i++;
		}
		i=ile_brakuje;
		while( i < ile_brakuje)
		{
			i++;
		}

		LCD_WriteTextXY("TEXT\0",0,0);
		LCD_WriteTextXY(text,0,1);
		start++;
	}
	return 0;



}
