
#include"eeprom.h"




int main(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  //LCD KBD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //KBD
	KBD_Init(); //inicjalizacja klawiatury
	LCD_Initialize();                           //inicjalizacja wysietlacza
	LCD_WriteCommand(HD44780_CLEAR);            //wyczysc wyswietlacz

	init_I2C1();

	LCD_WriteText((unsigned char *)"Podaj id: \0");
	LCD_GoTo(0,1);

	int indeks=wczytaj_id();
	int znaleziono=znajdz_id(indeks);//sprawdza w eepromie czy jest takie id i zwraca indeks do tego rekordu(0-15)

	LCD_WriteCommand(HD44780_CLEAR);
	LCD_GoTo(0,0);
	if ( znaleziono>=0 )
	{
		LCD_WriteText((unsigned char *)"Podaj haslo:");
		wczytaj_haslo(znaleziono);//dalsze sprawdzanie hasla
	}
	else if( znaleziono==-1 )//nie udany odczyt z eepromu
	{
		LCD_WriteText((unsigned char *)"Blad odczytu !");
	}
	else if( znaleziono==-2 )//nie znaleziono takiego id
	{
		LCD_WriteText((unsigned char *)"ID nie istnieje");
	}

	while(1);

	return 0;
}
