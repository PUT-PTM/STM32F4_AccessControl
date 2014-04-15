
#define DISPLAY_INCLUDED
#include "global.h"

#include <string.h>
#include "display.h"
#include "timer0.h"
#include "psd8xx.h"

#include "mem.h"
#include "kbd.h"
#include "rcp.h"


DISPLAY xdata LCD _at_ 0x0B00;

typedef struct
{
  BYTE charCode;
  BYTE charIndex;
} CHAR_DEF;

static CHAR_DEF CharDefTab[8];

// zmienne dla beepera czestotliwosciowego
BYTE BeepPuls;
BYTE BeeperNo;

void LCDInit (void)
{
//BYTE code *bptr;
//BYTE tbyte;

	//bptr = &tbyte; // mspace invalid
		/* if reset cond. met - LCD is initialized:
		8 bit interface, 1-line disp., 5 x 7 dot char.
		Disp. off, Cursor off, Blink off
		Mode increment, No shift*/

	// Initializing by instr. (if reset cond. not met):

	//Delay(T_5MS, 6);     // Leave time to initialize - req. 15 ms after Vcc rises to  4.5 V
	Delay(T_50MS, 1); // potrzebny dluzszy czas dla 'niebieskich' wyswietlaczy

	// "Fun. Set" (LCD Instr.)
	PSD8xx_reg.DATAOUT_B = 0xF3; 	// Set 8-bit interface
	LCD.cmdWr = 1;
	Delay(T_5MS, 1);  				// Leave time to end instr. - req. 4,1 ms

	// "Fun. Set"
	PSD8xx_reg.DATAOUT_B = 0xF3; 	// Set 8-bit interface
	LCD.cmdWr = 1;
	Delay(T_40US, 3);  	// Leave time to end instr. - req. 100 us

	// "Fun. Set"
	PSD8xx_reg.DATAOUT_B = 0xF3; 	// Set 8-bit interface
	LCD.cmdWr = 1;
	Delay(T_40US, 1);    // Leave time to end instr. - req. 40 us
	//BusyCheck();

	// Set LCD preferences:

	// "Fun. Set"
	PSD8xx_reg.DATAOUT_B = 0xF2; 	// Set 8-bit interface
	LCD.cmdWr = 1;
	Delay(T_40US, 1);    // Leave time to end instr. - req. 40 us
	//BusyCheck();

	PSD8xx_reg.DATAOUT_B = 0xF2; 	// Set 4-bit interf., 2-line display and 5 x 7 dot charecter
	LCD.cmdWr = 1;
	PSD8xx_reg.DATAOUT_B = 0xF8;
	LCD.cmdWr = 1;
	Delay(T_40US, 1);    // Leave time to end instr. - req. 40 us
	//BusyCheck();

	// "Disp. control" :
	PSD8xx_reg.DATAOUT_B = 0xF0;  // Disp. on, Cursor off, Blink off
	LCD.cmdWr = 1;
	PSD8xx_reg.DATAOUT_B = 0xFC;
	LCD.cmdWr = 1;
	Delay(T_40US, 1);    // Leave time to end instr. - req. 40 us
	//BusyCheck();

	// "Clear display" :
	PSD8xx_reg.DATAOUT_B = 0xF0;  // Clear disp.
	LCD.cmdWr = 1;
	PSD8xx_reg.DATAOUT_B = 0xF1;
	LCD.cmdWr = 1;
	Delay(T_40US, 50);  	// Leave time to end instr. - req. 1,64 ms
	//BusyCheck();

	// "Entry mode set" :
	PSD8xx_reg.DATAOUT_B = 0xF0;	// Mode inc, no shift
	LCD.cmdWr = 1;
	PSD8xx_reg.DATAOUT_B = 0xF6;
	LCD.cmdWr = 1;
	Delay(T_40US, 1);  	// Leave time to end instr. - req. 40 us
	//BusyCheck();

	// "Return home"
	PSD8xx_reg.DATAOUT_B = 0xF0;	//Cursor and shifted disp. home
	LCD.cmdWr = 1;
	PSD8xx_reg.DATAOUT_B = 0xF2;
	LCD.cmdWr = 1;
	Delay(T_40US, 50);  	// Leave time to end instr. - req. 1,64 ms
	//BusyCheck();

	if (SYS_DATA.lcdLed==LCDLED_OFF) LcdLedOff();
	else LcdLedOn();
}

BYTE CharDefLoad (BYTE charCode)
{				// RAM wyswietlacza LCD
 BYTE i, dfflag;
 CHAR_DEF chardf;

	for (i=0; i<8; i++)
	  if ( CharDefTab[i].charCode==charCode ) break;

	if (i==8)
	 { dfflag = FALSE; CharDefTab[7].charCode = charCode; i = 7; }
	else dfflag = TRUE;

	chardf = CharDefTab[i];
	for (i; i>0; i--) CharDefTab[i] = CharDefTab[i-1];
	CharDefTab[0] = chardf;

	if (dfflag) return CharDefTab->charIndex;

	i = 8 * CharDefTab->charIndex;
	// Set CG RAM address
	PSD8xx_reg.DATAOUT_B = 0xF4 | (i>>4);
	LCD.cmdWr = 1;
	PSD8xx_reg.DATAOUT_B = 0xF0 | (i&0x0F);
	LCD.cmdWr = 1;
	Delay(T_40US, 1);         // Leave time to end instr. - 40 us
	//BusyCheck();

	for (i=0; i<8; i++)
	{
	  PSD8xx_reg.DATAOUT_B = 0xF0 | ((CHAR_TAB[CharDefTab->charCode-CHAR_TAB_OFFSET]->drawingBytes[i])>>4);
	  LCD.RAMWr = 1;
	  PSD8xx_reg.DATAOUT_B = 0xF0 | CHAR_TAB[CharDefTab->charCode-CHAR_TAB_OFFSET]->drawingBytes[i];
	  LCD.RAMWr = 1;
	  Delay(T_40US, 1);
		 //BusyCheck();
	}

	return CharDefTab->charIndex;
}

void PrintStr (BYTE *Str, BYTE Ln, BYTE Line)
{
 BYTE tempbuf[LL], i;

	memcpy(tempbuf, Str, Ln);
	for (i=0; i<Ln; i++)
	  if ( tempbuf[i]>0x80 ) tempbuf[i] = CharDefLoad(tempbuf[i]);

	// "Set DD RAM adres"
	PSD8xx_reg.DATAOUT_B = 0xF0 | (Line>>4);
	LCD.cmdWr = 1;
	PSD8xx_reg.DATAOUT_B = 0xF0 | Line;
	LCD.cmdWr = 1;
	Delay(T_40US, 1);
	//BusyCheck();

	// "Write Data to DD RAM"
	for (i=0; i<Ln; i++)
	{ // Write one char
	  PSD8xx_reg.DATAOUT_B = 0xF0 | (tempbuf[i]>>4);
	  LCD.RAMWr = 1;
	  PSD8xx_reg.DATAOUT_B = 0xF0 | tempbuf[i];
	  LCD.RAMWr = 1;
	  Delay(T_40US, 1);
	  //BusyCheck();
	}
}

void PrintChar (BYTE charCode, BYTE Line)
{
	if (charCode>0x80) charCode = CharDefLoad(charCode);

	// "Set DD RAM adres"
	PSD8xx_reg.DATAOUT_B = 0xF0 | (Line>>4);
	LCD.cmdWr = 1;
	PSD8xx_reg.DATAOUT_B = 0xF0 | Line;
	LCD.cmdWr = 1;
	Delay(T_40US, 1);
	//BusyCheck();

	// "Write one char to DD RAM"
	PSD8xx_reg.DATAOUT_B = 0xF0 | (charCode>>4);
	LCD.RAMWr = 1;
	PSD8xx_reg.DATAOUT_B = 0xF0 | (charCode&0x0F);
	LCD.RAMWr = 1;
	Delay(T_40US, 1);
	  //BusyCheck();
}

void LCDEraseLine (BYTE line)
{
	PrintStr("                ", LL, line);
}

void LCDErase (void)
{
	PSD8xx_reg.DATAOUT_B = 0xF0 | (ERASE_CMD>>4);
	LCD.cmdWr = 1;
	PSD8xx_reg.DATAOUT_B = 0xF0 | ERASE_CMD;
	LCD.cmdWr = 1;
	Delay(T_40US, 50);       // Leave time to end instr. - req. 1.64 ms
	//BusyCheck();
}

/*
void PutLCDCmd (BYTE LCDCmd)
{
	if (LCDCmd<32)				// allowed instr.
	{
	  PSD8xx_reg.DATAOUT_B = 0xF0 | (LCDCmd>>4);
	  LCD.cmdWr = 1;
	  PSD8xx_reg.DATAOUT_B = 0xF0 | LCDCmd;
	  LCD.cmdWr = 1;
	  if (LCDCmd<4) Delay(T_40US, 50);
	  else  Delay(T_40US, 1);
	  //BusyCheck();
	}
} //*/

void CharDef (void)	// Zapisanie wlasnych znakow do pamieci
{				// RAM wyswietlacza LCD
 BYTE i;

	for (i=0; i<8; i++)
	{
	  CharDefTab[i].charCode = 0x80;
	  CharDefTab[i].charIndex = i;
	}

	for (i=0; i<8; i++) CharDefLoad(0xA0+i);
} //*/

void MakeBeep2_simple (BYTE beepFreq, BYTE beepTime)
{				// Ustawienie Buzzera
  WORD  beepcounter;
									//   T[s] = 2*40us*beepFreq =>
									// 1.	f[Hz] = 12500/beepFreq
									//   t[s] = beepTime*25*T[s] =>
									// 2.	t[ms] = 2*beepTime*beepFreq
									
	beepcounter = 2*beepTime*beepFreq/X2*3; // czas zal¹czenia beepera w ms *3

#ifdef PCBOARDv2
	T1 = 0; // P3^5 - InK1
#else
	PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C & 0xFE; // PC.0 = 0
#endif	
	while (beepcounter--!=0)
	{
		Delay(T_40US, 25); // 1ms
	}
#ifdef PCBOARDv2
	T1 = 1; // P3^5 - InK1
#else
	PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C | 0x01; // PC.0 = 1
#endif	
}
#ifdef SIMPLE_BEEPER
void MakeBeep (BYTE beepFreq, BYTE beepTime)
{				// Ustawienie Buzzera
 WORD  beepcounter;
									//   T[s] = 2*40us*beepFreq =>
									// 1.	f[Hz] = 12500/beepFreq
									//   t[s] = beepTime*25*T[s] =>
									// 2.	t[ms] = 2*beepTime*beepFreq
									
	beepcounter = 2*beepTime*beepFreq/X2*3; // czas zal¹czenia beepera w ms *3
	
   PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C & 0x7F; // PC 7 = 0
	while (beepcounter--!=0)
	{
		Delay(T_40US, 25); // 1ms
	}
	PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C | 0x80; // PC 7 = 1
}//*/

void MakeBeep2 (BYTE beepFreq, BYTE beepTime)
{				// Ustawienie Buzzera
  WORD  beepcounter;
									//   T[s] = 2*40us*beepFreq =>
									// 1.	f[Hz] = 12500/beepFreq
									//   t[s] = beepTime*25*T[s] =>
									// 2.	t[ms] = 2*beepTime*beepFreq
									
	beepcounter = 2*beepTime*beepFreq/X2*3; // czas zal¹czenia beepera w ms *3

#ifdef PCBOARDv2
	T1 = 0; // P3^5 - InK1
#else
	PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C & 0xFE; // PC.0 = 0
#endif	
	while (beepcounter--!=0)
	{
		Delay(T_40US, 25); // 1ms
	}
#ifdef PCBOARDv2
	T1 = 1; // P3^5 - InK1
#else
	PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C | 0x01; // PC.0 = 1
#endif	
}

#else // ndef SIMPLE_BEEPER

#ifdef X2_ON
extern WORD  puls;
extern BYTE  puls1s, counter1s;
//sbit	IO1 = P1^0;
void MakeBeep3 (BYTE beepFreq, BYTE beepTime)
{				// Ustawienie Buzzera
 WORD  beepcounter;
									//   T[s] = 2*40us*beepFreq =>
									// 1.	f[Hz] = 12500/beepFreq
									//   t[s] = beepTime*25*T[s] =>
									// 2.	t[ms] = 2*beepTime*beepFreq
									
	beepcounter = 2*beepTime*beepFreq/X2*3; // czas zal¹czenia beepera w ms *3
	
   PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C & 0x7F; // PC 7 = 0
	while (beepcounter--!=0)
	{
		Delay(T_40US, 25); // 1ms
	}
	PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C | 0x80; // PC 7 = 1
}//*/
void MakeBeep1 (BYTE beepFreq, BYTE beepTime)
{				// Ustawienie Buzzera
 WORD  beepcounter;
 BYTE  lastlevel;				//		COUNT_80US			|		COUNT_122US
									//   T[s] = 2*80us*beepFreq =>		|	  T[s] = 2*122us*beepFreq =>
									// 1. f[Hz] = 6250/beepFreq			|	1. f[Hz] = 4100/beepFreq
									//   t[s] = beepTime*25*T[s]/2 =>		|	  t[s] = beepTime*17*T[s]/2 =>
									// 2. t[ms] = 2*beepTime*beepFreq	|	2. t[ms] = 2*beepTime*beepFreq
									
	//beepcounter = beepTime*25u; // dla COUNT_80US
	beepcounter = beepTime*17u; // dla COUNT_122US

	TR0 = FALSE;
	// wylaczenie Timer1S()
	puls1s = 0;
	// ustawienie Timer0:
	TH0 = COUNT_122US;   //od 50 do 250
	TL0 = COUNT_122US;	
	counter1s = 0;	
	BeepPuls = T_122US*beepFreq;
	puls = BeepPuls;	// liczba przepelnien timera dla 1/2 okresu sygnalu beepera
	BeeperNo = BEEPER1;
	PSD8xx_reg.DATAOUT_C &= 0x7F; // wlacz beeper - sterowanie PC.7 przez: '07' = 0
	lastlevel = PSD8xx_reg.DATAOUT_C; 						//	'MAX232' = 1 - MAX poda ujemne napiecie
	TR0 = TRUE;
	
	while (beepcounter--!=0)
	{ 		   
		while ( !(PSD8xx_reg.DATAOUT_C^lastlevel) ); // oczekiwanie na zmiane stanu linii sterujacej beeperem
		lastlevel = PSD8xx_reg.DATAOUT_C;				// zmiana jest dokonywana pod przerwaniem Timer0Irq()
	}
	
	TR0 = FALSE;
	puls = 0;
	BeepPuls = FALSE;
	PSD8xx_reg.DATAOUT_C |= 0x80; // wylacz beeper - sterowanie przez: '07' = 1
}						// 'MAX232' = 0 - MAX poda dodatnie napiecie; z testow wyszlo, ze gdy podamy '1' to beeper wzbudza sie przy transmisji RS
void MakeBeep (BYTE beepFreq, BYTE beepTime)
{	 
	MakeBeep1(beepFreq,beepTime);
/*
	BYTE i,x;
   for (i=1;i<5;i++)
	{
		if (i<2) 
		{	for (x=1;x<2;x++)
			{	MakeBeep1(i,beepTime*3);
				Delay(T_50MS, 1);
			}
		}
		else //MakeBeep1(i,beepTime);
		{	for (x=1;x<2;x++)
			{	MakeBeep1(i,beepTime*2);
				Delay(T_50MS, 1);
			}
		}
		Delay(T_50MS, 10);
	}
	*/
}

void MakeBeep2 (BYTE beepFreq, BYTE beepTime)
{				// Ustawienie Buzzera
 WORD  beepcounter;
 BYTE  lastlevel;				//		COUNT_80US			|		COUNT_122US
									//   T[s] = 2*80us*beepFreq =>		|	  T[s] = 2*122us*beepFreq =>
									// 1. f[Hz] = 6250/beepFreq			|	1. f[Hz] = 4100/beepFreq
									//   t[s] = beepTime*25*T[s]/2 =>		|	  t[s] = beepTime*17*T[s]/2 =>
									// 2. t[ms] = 2*beepTime*beepFreq	|	2. t[ms] = 2*beepTime*beepFreq
									
	//beepcounter = beepTime*25u; // dla COUNT_80US
	beepcounter = beepTime*17u; // dla COUNT_122US

	TR0 = FALSE;
	// wylaczenie Timer1S()
	puls1s = 0;
	// ustawienie Timer0:	
	TH0 = COUNT_122US;
	TL0 = COUNT_122US;
	counter1s = 0;	
	BeepPuls = T_122US*beepFreq;
	puls = BeepPuls;	// liczba przepelnien timera dla 1/2 okresu sygnalu beepera
	BeeperNo = BEEPER2;
	T1 = 0; // wlacz beeper - sterowanie przez: '07' = 0
	lastlevel = T1;				//	'MAX232' = 1 - MAX poda ujemne napiecie
	TR0 = TRUE;
	
	while (beepcounter--!=0)
	{ 		   
		while ( !(T1^lastlevel) ); // oczekiwanie na zmiane stanu linii sterujacej beeperem
		lastlevel = T1;				// zmiana jest dokonywana pod przerwaniem Timer0Irq()
	}
	
	TR0 = FALSE;
	puls = 0;
	BeepPuls = FALSE;
	T1 = 1; // wylacz beeper - sterowanie przez: '07' = 1
}						// 'MAX232' = 0 - MAX poda dodatnie napiecie; z testow wyszlo, ze gdy podamy '1' to beeper wzbudza sie przy transmisji RS

/*
void MakeBeep2 (BYTE beepFreq, BYTE beepTime)
{				// Ustawienie Buzzera
 WORD  wtemp, beepcounter;
									//   T[s] = 2*80us*beepFreq =>
									// 1. f[Hz] = 6250/beepFreq
									//   t[s] = beepTime*25*T[s]/2 =>
									// 2. t[ms] = 2*beepTime*beepFreq
	beepcounter = beepTime*25;

	TR0 = FALSE;
	TH0 = COUNT_80US;
	TL0 = COUNT_80US;
	counter1s = 0;
	wtemp = T_80US*beepFreq;
	puls = wtemp;		
	TR0 = TRUE;
	
	while (beepcounter--!=0)
	{ 
	#ifdef PCBOARDv2
		T1 = T1 ^ 1; // !T1 (P3^5 - InK1)
	#else
		PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C ^ 0x01; // !PC.0
	#endif				
		while (puls);
	   puls = wtemp;
	}
	
	TR0 = FALSE;
 #ifdef PCBOARDv2  
	T1 = 1; //  1 - wylacz beeper
 #else
   PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C | 0x01; // PC.0 = 1 - wylacz beeper
 #endif
} //*/
#else // ndef X2_ON
void MakeBeep (BYTE beepFreq, BYTE beepTime)
{				// Ustawienie Buzzera
 WORD  beepcounter;
									//   T[s] = 2*40us*beepFreq =>
									// 1.	f[Hz] = 12500/beepFreq
									//   t[s] = beepTime*25*T[s] =>
									// 2.	t[ms] = 2*beepTime*beepFreq
	beepcounter = beepTime*25;

	while (beepcounter--!=0)
	{
		PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C & 0x7F; // PC 7 = 0
		Delay(T_40US, beepFreq);
		PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C | 0x80; // PC 7 = 1
		Delay(T_40US, beepFreq);
	}
}//*/

void MakeBeep2 (BYTE beepFreq, BYTE beepTime)
{				// Ustawienie Buzzera
  WORD  beepcounter;
									//   T[s] = 2*40us*beepFreq =>
									// 1.	f[Hz] = 12500/beepFreq
									//   t[s] = beepTime*25*T[s] =>
									// 2.	t[ms] = 2*beepTime*beepFreq
	beepcounter = beepTime*25;

	while (beepcounter--!=0)
	{
	#ifdef PCBOARDv2
		T1 = 0; // P3^5 - InK1
		Delay(T_40US, beepFreq);
		T1 = 1; // P3^5 - InK1
		Delay(T_40US, beepFreq);
	#else
		PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C & 0xFE; // PC.0 = 0
		Delay(T_40US, beepFreq);
		PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C | 0x01; // PC.0 = 1
		Delay(T_40US, beepFreq);	
	#endif
	}
}
#endif // ndef X2_ON
#endif // ndef SIMPLE_BEEPER

void LCDReset (void)
{
	LcdLedOff();
	PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C | 0x40; // LCD off
   PSD8xx_reg.DATAOUT_B = 0xF0;	// ustawienie niskiego poziomu napiecia na liniach danych: LCD0-LCD3
	LCD.cmdWr = 1;	// ustawienie niskiego poziomu napiecia na liniach sterujacych: RS i R/W (dla 'cmdWr' latched address out = 0x00)
	Delay(T_50MS, 40);
	PSD8xx_reg.DATAOUT_C = PSD8xx_reg.DATAOUT_C & 0xBF; // LCD on	
	LCDInit();
	CharDef();
}

#ifdef TESTING
void ViewLcdCharCode (void)
{
 BYTE i, j, c;
 BYTE tempbuf[16];

  i=0;
  while (1)
  {
	 LCDErase();

	 memcpy(tempbuf, "CharCode 00 Hex:", LL);
	 c = i*16;
	 HextoAscii(&c, 1, tempbuf+9);
	 PrintStr(tempbuf, LL, LINE1);
	 for (j=0; j<16; j++) PrintChar(c+j, LINE2+j);

	 do c = Kbd_GetKey(SYS_DATA.KBDTimeOut, TRUE);
	 while ( c!=0x04 && c!=KOK && c!=KB && c!=KASUJ && c!=PRZERWIJ );

	 if (c==PRZERWIJ) return; // c==0x04 ||
	 else if (c==KB) { if (i) i--; else i=15; }
			else if (c==KASUJ) { if (i<15) i++; else i=0; }
  }
}
#endif
