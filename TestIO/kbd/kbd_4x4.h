#ifndef kbd_h
#define kbd_h
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


#include "delay.h"
#include "stm32f4xx_gpio.h"

void KBD_Init(void);
unsigned char KBD_ReadKey(void);
void Beep_KBD(void);
void Beep(void);
void Zamek1_On(void);
#endif
