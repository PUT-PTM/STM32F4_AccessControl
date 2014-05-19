#include "stm32f4xx_rcc.h"
#include "lcd_hd44780.h"
#include "kbd_4x4.h"
#include "string.h"
int main(void)
{
	unsigned char Tekst[2] = {"\0"};
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  //LCD KBD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //KBD
	KBD_Init();
	LCD_Initialize();                           //inicjalizacja wysietlacza
	LCD_WriteCommand(HD44780_CLEAR);            //wyczysc wyswietlacz


	x:
	LCD_WriteText((unsigned char *)"Podaj haslo: \0");
	LCD_GoTo(0,1);

	int i=0;
	char tab[5]={0,0,0,0,0};

	//1
	Tekst[0]=0;
	Tekst[1]=0;
	while (1)
	{
		Tekst[0]=KBD_ReadKey();
		if (Tekst[0] !=0 )
		{
			tab[0]=Tekst[0];
			break;
		}
	};
	LCD_GoTo(0,1);
	LCD_WriteText((unsigned char *)"*            ");

	//2
	Tekst[0]=0;
	Tekst[1]=0;
	while (1)
	{
		Tekst[0]=KBD_ReadKey();
		if (Tekst[0] !=0 )
		{
			tab[1]=Tekst[0];
			break;
		}
	};
	LCD_GoTo(0,1);
	LCD_WriteText((unsigned char *)"**           ");

	//3
	Tekst[0]=0;
	Tekst[1]=0;
	while (1)
	{
		Tekst[0]=KBD_ReadKey();
		if (Tekst[0] !=0 )
		{
			tab[2]=Tekst[0];
			break;
		}
	};
	LCD_GoTo(0,1);
	LCD_WriteText((unsigned char *)"***          ");

	//4
	Tekst[0]=0;
	Tekst[1]=0;
	while (1)
	{
		Tekst[0]=KBD_ReadKey();
		if (Tekst[0] !=0 )
		{
			tab[3]=Tekst[0];
			break;
		}
	};
	LCD_GoTo(0,1);
	LCD_WriteText((unsigned char *)"****         ");
	tab[4]=0;

	unsigned char* haslo="2013\0";

	if ( strcmp( tab, haslo )==0)
	{
		LCD_WriteCommand(HD44780_CLEAR);
		LCD_GoTo(0,0);
		LCD_WriteText((unsigned char *)"Haslo poprawne");
	}
	else
	{
		LCD_WriteCommand(HD44780_CLEAR);
		LCD_GoTo(0,0);
		LCD_WriteText((unsigned char *)"Blad");
	}

	while(1);

	return 0;
}
