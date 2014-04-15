
#include "global.h"
#include <string.h>

#include "mem.h"
#include "timer0.h"
#include "psd8xx.h"
#include "kbd.h"
#include "display.h"

//-- Define section ------------------------------------------------------------


//-- data definition -----------------------------------------------------------

/*BYTE code ConvTab[4][4] = {
	0x01, 0x02, 0x03, 0xF1,
	0x04, 0x05, 0x06, 0xF2,
	0x07, 0x08, 0x09, 0xF3,
	0x0C, 0x00, 0x0E, 0xF4 };*/

BYTE code ConvTab[4][4] = {
	'A','B','C','D',
	'3','6','9','#',
	'2','5','8','0',
	'1','4','7','*' };

BYTE code ConvRow[LINE_OUT_LEN] = {0xEF, 0xDF, 0xBF, 0x7F};
code BYTE *pusty   = "                   ";



BYTE Kbd_GetKey (BYTE Step, BYTE Beep)
{
 BYTE  key, row, i;

  PSD8xx_reg.DATAOUT_A = PSD8xx_reg.DATAOUT_A | LINE_IN_MASK;
  //PSD8xx_reg.DIRECTION_B = 0x00; 	 // 1= ouput mode, 0 = input mode

  if (Step)
	 for (row=0; row<LINE_OUT_LEN; row++)
	 {
		PSD8xx_reg.DATAOUT_B = ConvRow[row];
		key = PSD8xx_reg.DATAIN_A;
		for (i=0; i<5; i++); // wait about 45 us  Delay(T_40US, 1);
		if ((key & LINE_IN_MASK) != LINE_IN_MASK)
		  { key = 0xCF; goto endKBD; }  // Key already pressed
	 }
											// 2008-09-25
  if (Step) Timer50MS(10u*Step); // Krok 0,5s obliczany tutaj a nie na zewnatrz - dzieki temu zwieksza siê zakres KBDTimeOut do 127,5s.
  do										// Warunek to Timer50MS(WORD) .
  {
	 for (row=0; row<LINE_OUT_LEN; row++)
	 {
		PSD8xx_reg.DATAOUT_B = ConvRow[row];
		key = ~ PSD8xx_reg.DATAIN_A;
		for (i=0; i<5; i++); // wait about 45 us    Delay(T_40US, 1);
		key = key&LINE_IN_MASK;
		if (key != 0x00)
		{
		  for (i=0; i<LINE_IN_LEN; i++)
			 { if (key&0x80) break; key = key<<1; }
		  key = (row+1)<<4;
		  key = key + i+1;
		  Step = 0;
		  break;
		}
	 }
  }
  while (TestTimer() && Step);

  if (key==0x00) { key = 0x04; goto endKBD; } // Wait time is gone

  if (Beep) MakeBeep(BEEPFREQ_L, X2*12/BEEPFREQ_L); // 2*X2*12 ms
  // MakeBeep(SYS_DATA.beepVolume, X2*12/SYS_DATA.beepVolume); // do testów
  
  Timer50MS(40);
  Step = 0;
  while (TestTimer() && Step!=LINE_IN_MASK)
  {
	 PSD8xx_reg.DATAOUT_B = ConvRow[row];
	 Step = PSD8xx_reg.DATAIN_A;
	 for (i=0; i<5; i++); // wait about 45 us    Delay(T_40US, 1);
	 Step = Step & LINE_IN_MASK;
  }
  if (!TestTimer()) { key = 0xCF; goto endKBD; } // Key already pressed
  Delay(T_50MS, 1);	// stan nieustalony przy zestyku

endKBD:
  //PSD8xx_reg.DATAOUT_B = 0xFF;   // zeby na wyjsciowy port B nie "poszlo" zero
  //PSD8xx_reg.DIRECTION_B = 0xFF; // 1= ouput mode, 0 = input mode

  return key;
}

BYTE KeyVal(BYTE Code)
{
BYTE   i;

  i = Code;
  Code = (Code & 0x0F) - 1;
  i = (i >> 4) - 1;
  return(ConvTab[Code][i]);
}

BYTE DecimalStr (BYTE maxLength, BYTE *strData, BYTE *strName, BYTE nameLength, BYTE opType)
{
 BYTE i,j,c;

	 LCDEraseLine(LINE2);
	 PrintStr(strName, nameLength, LINE2);

	 if (opType=='E')
	 {
		PrintStr(strData, maxLength, LINE2+nameLength);
		j = maxLength;
	 }
	 else j = 0; // opType=='G'

	 c = 0xCF;
	 while ( 1 )
	 {
		do
		{
		  if (c==0x04)
		  {
			 if ( (++i)==SYS_DATA.KBDTimeOut ) { c = PRZERWIJ; break; }
		  }
		  else i = 0;

		  //i++;
		  if (i%2) PrintChar(' ', LINE2+nameLength+j);
		  else PrintChar(LCD_KURSOR, LINE2+nameLength+j);
		  c = Kbd_GetKey(1, TRUE); //KeyCode: cf-klawisz wczesniej wcisniety 04-nic nie wcisnieto
		}
		while ( c==KB || c==PRAWO || c==LEWO || c==0x04 || c==0xCF );

		switch(c)
		{
		 case PRZERWIJ: LCDEraseLine(LINE2); return FALSE;

		 case KOK:  if (j==0) break;
						for (i=0; i<j; i++) *(strData+maxLength-1-i) = *(strData+j-1-i);
						for (i=0; i<maxLength-j; i++) *(strData+i) = '0';
						LCDEraseLine(LINE2); return TRUE;
						/*
		 case KOK:  if (j==maxLength) { LCDErase(); return TRUE; }
						break; //*/


		 case KASUJ:if (j) j--;
						PrintStr("  ", 2, LINE2+nameLength+j);
						break;

		 default:   if (j<maxLength)
						{
						  *(strData+j) = KeyVal(c);
						  PrintChar(*(strData+j), LINE2+nameLength+j);
						  j++;
						}
						break;
		} // switch

	 } // while

} //*/

BYTE GetDecimalStr (BYTE maxLength, BYTE *strData, BYTE *strName, BYTE nameLength)
{
  return DecimalStr(maxLength, strData, strName, nameLength, 'G');
} //*/

BYTE EditDecimalStr (BYTE maxLength, BYTE *strData, BYTE *strName, BYTE nameLength)
{
  return DecimalStr(maxLength, strData, strName, nameLength, 'E');
} //*/

BYTE ChooseMenuItem (BYTE code**menuItemName, BYTE itemNo)
{
 BYTE i, j, c;

  i = 0;
  j = 0;
  while (1)
  {
	 if (j<i) i = j;
	 if (j>i+1) i = j-1;

	 PrintStr(menuItemName[i], LL, LINE1);
	 // c=strlen(menuItemName[i]) PrintStr(pusty, LL-c, LINE1);
	 PrintStr(menuItemName[i+1], LL, LINE2);
	 // c=strlen(menuItemName[i+1]) PrintStr(pusty, LL-c, LINE2);
	 if (i==j) PrintChar(LCD_ITEM_MARK, LINE1+1);
	 else PrintChar(LCD_ITEM_MARK, LINE2+1);

	 do
	 {
		c = Kbd_GetKey(SYS_DATA.KBDTimeOut, TRUE);
	 }
	 while ( c==0xCF || c==LEWO || c==PRAWO || c==K0 );	//c==0x04

	 switch(c)
	 {
		case 0x04:
		case PRZERWIJ: LCDErase(); return FALSE;
		case KOK:      LCDErase(); return j+1;
		case KB:       if (j!=0) j--; else j = itemNo-1;
							break;
		case KASUJ:    j = (++j)%itemNo;
							break;
		default:       c = KeyVal(c)-'0'; if (c<=itemNo) j = c-1;
							break;
	 }
  }  // while(1)
} //*/

/*BYTE Pressed_ONOFF15 (void)
{
 BYTE  i, key;

  if (!TestTimer())
  {
	 if (IddleCounter==0) return TRUE;
	 IddleCounter--;
	 Timer50MS(200);
  }

	 PSD8xx_reg.DATAOUT_B = 0xFF;
	 PSD8xx_reg.DIRECTION_B = 0x00; 	 // 1= ouput mode, 0 = input mode

	 PSD8xx_reg.DATAOUT_B = 0xEF;

	 key = PSD8xx_reg.DATAIN_A;
	 for (i=0; i<5; i++); // wait about 45 us  Delay(T_40US, 1);
	 if ((key & 0xF0) == 0xF0)
	 {
		Timer50MS(30);
		while (1)
		{
		  key = PSD8xx_reg.DATAIN_A;
		  for (i=0; i<5; i++); // wait about 45 us  Delay(T_40US, 1);
		  if ((key & 0xF0) != 0xF0) break;
		  if (!TestTimer()) return TRUE;
		}
		TR2 = 0;
	 }
	 PSD8xx_reg.DATAOUT_B = 0xFF;   // zeby na wyjsciowy port B nie "poszlo" zero
	 PSD8xx_reg.DIRECTION_B = 0xFF; // 1= ouput mode, 0 = input mode

  return FALSE;
} //*/

/* old version - wiodace mrugajace zera, jedno miejsce po przecinku: 00,0
BYTE EditProcentValue (BYTE maxLength, BYTE *strData, BYTE *strName, BYTE nameLength )
{
 BYTE i, j, c, k;

  LCDErase();

  for (i=0; i<nameLength; i++) PrintChar(strName[i], LINE1+i);

  PrintChar(0x30, LINE3+3);
  PrintChar(0x30, LINE3+4);
  PrintChar(0xa1, LINE3+5);
  PrintChar(0x30, LINE3+6);
  PrintChar(0xa2, LINE3+7);

  //		 PrintStr(strName, nameLength, LINE2);
  //		 PrintStr(strData, maxLength, LINE2+nameLength);		// { for Edit
  for (i=0; i<maxLength; i++) *(strData+i) = '0';

	 j = 0;

	 while (1)
	 {
	  //Timer50MS(10);
	  k = 0;
	  i = j;
	  if (i==2) i=3;
	  do {  //if (!TestTimer())
		  k++;
		  if (k%2) PrintChar(*(strData+j), LINE3+3+i);
		  else PrintChar(0xa3, LINE3+3+i);
		  c = Kbd_GetKey(10, TRUE);
	  }  //}
	  while ( !((c>>4)<4 && (c&0x0F)>2 && (c&0x0F)<6 && c!=0x04) && c!=K0 && c!=KOK && c!=PRZERWIJ);

	  TR2 = FALSE;

		switch(c)
		{
		 case PRZERWIJ: LCDErase();  return FALSE;
		 case KOK:	    LCDErase();  return TRUE;
		 default:   break;
		} // switch

		c = KeyVal(c) + 0x30;
		*(strData+j) = c;
		if (j==2) j=3;
		PrintChar(c, LINE3+3+j);

		j =  (++j)&0x03;
	 } // for
}*/
/*
BYTE EditProcentValue (BYTE maxLength, BYTE *strData, BYTE *strName, BYTE nameLength )
{
 BYTE i, j, c, k;

  //LCDErase();

  for (i=0; i<nameLength; i++) PrintChar(strName[i], LINE1+i);

  if (*strData==' ')
  {
	 PrintChar(0xa4, LINE3+1);
	 for (i=1; i<maxLength; i++) PrintChar(0xa3, LINE3+1+i);
	 j = 0;
  }
  else
  {
	 for (i=0; i<maxLength-1; i++) if (*(strData+i)!='0') break;

	 for (j=0; i<maxLength; i++,j++) *(strData+j) = *(strData+i);

	 for (i=0; i<j; i++) PrintChar(*(strData+i), LINE3+1+i);
	 PrintChar(0xa4, LINE3+1+j);
	 for (i=j+1; i<maxLength; i++) PrintChar(0xa3, LINE3+1+i);
  }

  PrintChar(0xa3, LINE3+1+maxLength);
  PrintChar(0xa2, LINE3+1+maxLength+1);

  while (1)
  {
	 //Timer50MS(10);
	 i = FALSE;
	 k = 0;
	 do {  //if (!TestTimer())
		  k++;
		  if (k%2) PrintChar(0xa4, LINE3+1+j);
		  else PrintChar(0xa3, LINE3+1+j);
		  c = Kbd_GetKey(10, TRUE);
	 }
	 while ( !((c>>4)<4 && (c&0x0F)>1 && (c&0x0F)<6 && c!=0x04) && c!=K0 && c!=KOK && c!=GORA);

	  //TR2 = FALSE;

	 switch(c)
	 {
	  case KOK:       if (j==0) { i=TRUE; break; }
							for (i=0; i<j; i++) *(strData+maxLength-1-i) = *(strData+j-1-i);
							for (i=0; i<maxLength-j; i++) *(strData+i) = '0';
							LCDErase();  return c;

	  case DOL:       if (j==0) { i=TRUE; break; }
							for (i=0; i<j; i++) *(strData+maxLength-1-i) = *(strData+j-1-i);
							for (i=0; i<maxLength-j; i++) *(strData+i) = '0';
							LCDErase();  return c;

	  case KASUJ:     if (j!=0)
							{
							  PrintChar(0xa3, LINE3+1+j);
							  j--;
							  *(strData+j) = ' ';
							}
							i=TRUE; break;

	  case PRZERWIJ:  LCDErase();  return c;
	  case GORA:		if (j!=0)
							{
							  for (i=0; i<j; i++) *(strData+maxLength-1-i) = *(strData+j-1-i);
							  for (i=0; i<maxLength-j; i++) *(strData+i) = '0';
							}
							LCDErase();  return c;
	  default:      break;
	 } // switch

	 if (i==TRUE || j==maxLength) continue;

	 c = KeyVal(c) + 0x30;
	 *(strData+j) = c;
	 PrintChar(c, LINE3+1+j);

	 j++;
  } // while
}

BYTE GetDecimalStr (BYTE maxLength, BYTE *strData, BYTE *strName, BYTE nameLength )
{
 BYTE j, c;

	 //LCDEraseLine(LINE2);
	 PrintStr(strName, nameLength, LINE4);

	 j = 0;
	 while ( 1 )
	 {
		do c = Kbd_GetKey(1, TRUE); //KeyCode: cf-klawisz wczesniej wcisniety 04-nic nie wcisnieto
		while ( !((c>>4)<4 && (c&0x0F)>1 && (c&0x0F)<6 && c!=0x04 && c!=DOL) && c!=K0 && c!=KOK );

		switch(c)
		{
		 case PRZERWIJ: LCDErase(); return FALSE;

		 case KOK:  if (j<maxLength) break;
						LCDErase(); return TRUE;

		 case KASUJ:if (j) j--;
						PrintStr (" ", 1, LINE4+nameLength+j);
						break;

		 default:   if (j<maxLength)
						{
						  c = KeyVal(c) + 0x30;
						  *(strData+j) = c;
						  PrintStr (&c, 1, LINE4+nameLength+j);
						  j++;
						}
						break;
		} // switch

	 } // while

} //*/
