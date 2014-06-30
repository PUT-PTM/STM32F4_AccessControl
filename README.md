STM32F4_AccessControl - Dokumentacja w jêzyku polskim(PL)

1. Informacje o projekcie

Projekt STM32F4_AccessControl zosta³ utworzony jako projekt z Podstaw Technik Mikroprocesorowych. Projekt by³ specjalnie tworzony na p³ytce STM32F407 Discovery. Napisany program ma umo¿liwiaæ kontrolê czyli otwieranie i zamykanie drzwi. Aby projekt dzia³a³ nale¿y pod³¹czyæ pod odpowiednie piny pamiêæ EEPROM, wyœwietlacz LCD 2x16, klawiaturê 4x4 oraz beeper.

2.Œrodowisko programistyczne i konfiguracja kompilatora

Projekt zosta³ zaimplementowany w œrodowisku CooCox IDE. Po zainstalowaniu œrodowiska nale¿y wskazaæ œcie¿kê do kompilatora GCC w programie CooCox IDE i nacisn¹æ przycisk Build w celu skompilowania projektu.
W œrodowisku Coocox istnieje mo¿liwoœæ debugowania kodu wystarczy nacisn¹æ Start Debug.

3.Sterowniki i oprogramowanie dla STM32F4xx

Oprogramowanie jest wgrywane na p³ytkê STM32F4xx Discovery przy u¿yciu przewodu USB. Aby wgraæ program na p³ytkê nale¿y nacisn¹æ przycisk Program Download. Sterownik potrzebny do poprawnego dzia³ania mikrokontlorera jest dostêpny na stronie producenta.

4. Za³o¿enia projektu

Po wczytaniu odpowiedniego numeru identyfikacyjnego oraz has³a zostaj¹ otwarte drzwi. W innym przypadku na wyœwietlaczu LCD pojawia siê informacja o b³êdzie.

5. Program wykonuje nastêpuj¹ce operacje
    a. Inicjalizuje klawiaturê, wyœwietlacz LCD, EEPROM, ustawia taktowanie.
    b. Nastêpnie program prosi u¿ytkownika o podanie numeru identyfikacyjnego.
    	> Jeœli w pamiêci EEPROM nie ma podanego numeru ID zostaje wyœwietlona informacja o b³êdzie.
    	> Jeœli numer ID istnieje program jest kontynuowany.
    c. Program prosi u¿ytkownika o podanie has³a
    	> Jeœli podane has³o istnieje dla podanego wczeœniej numeru identyfikacyjnego nie zostanie znalezione mikrokontroler zg³asza b³¹d.
    	> Jeœli has³o istnieje w pamiêci EEPROM zostan¹ otwarte drzwi(dok³adnie zostanie wys³any odpowiedni sygna³).