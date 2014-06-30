STM32F4_AccessControl - Dokumentacja w j�zyku polskim(PL)

1. Informacje o projekcie

Projekt STM32F4_AccessControl zosta� utworzony jako projekt z Podstaw Technik Mikroprocesorowych. Projekt by� specjalnie tworzony na p�ytce STM32F407 Discovery. Napisany program ma umo�liwia� kontrol� czyli otwieranie i zamykanie drzwi. Aby projekt dzia�a� nale�y pod��czy� pod odpowiednie piny pami�� EEPROM, wy�wietlacz LCD 2x16, klawiatur� 4x4 oraz beeper.

2.�rodowisko programistyczne i konfiguracja kompilatora

Projekt zosta� zaimplementowany w �rodowisku CooCox IDE. Po zainstalowaniu �rodowiska nale�y wskaza� �cie�k� do kompilatora GCC w programie CooCox IDE i nacisn�� przycisk Build w celu skompilowania projektu.
W �rodowisku Coocox istnieje mo�liwo�� debugowania kodu wystarczy nacisn�� Start Debug.

3.Sterowniki i oprogramowanie dla STM32F4xx

Oprogramowanie jest wgrywane na p�ytk� STM32F4xx Discovery przy u�yciu przewodu USB. Aby wgra� program na p�ytk� nale�y nacisn�� przycisk Program Download. Sterownik potrzebny do poprawnego dzia�ania mikrokontlorera jest dost�pny na stronie producenta.

4. Za�o�enia projektu

Po wczytaniu odpowiedniego numeru identyfikacyjnego oraz has�a zostaj� otwarte drzwi. W innym przypadku na wy�wietlaczu LCD pojawia si� informacja o b��dzie.

5. Program wykonuje nast�puj�ce operacje
    a. Inicjalizuje klawiatur�, wy�wietlacz LCD, EEPROM, ustawia taktowanie.
    b. Nast�pnie program prosi u�ytkownika o podanie numeru identyfikacyjnego.
    	> Je�li w pami�ci EEPROM nie ma podanego numeru ID zostaje wy�wietlona informacja o b��dzie.
    	> Je�li numer ID istnieje program jest kontynuowany.
    c. Program prosi u�ytkownika o podanie has�a
    	> Je�li podane has�o istnieje dla podanego wcze�niej numeru identyfikacyjnego nie zostanie znalezione mikrokontroler zg�asza b��d.
    	> Je�li has�o istnieje w pami�ci EEPROM zostan� otwarte drzwi(dok�adnie zostanie wys�any odpowiedni sygna�).