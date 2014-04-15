
#define BF_BIT  	 0x80       // Busy flag
#define LINE1      0x80       // LCD display position (1st line)
#define LINE2      0xC0       // LCD display position (2nd line)
#define LL			 16
#define ERASE_CMD  0x01

#define LCD_KURSOR 	 0x5F	// '_' - character code zgodny z HD44780
#define LCD_ITEM_MARK 0x7E	// '->' - character code zgodny z HD44780

#define GLOSNO		2
#define CICHO		4

/*
#ifdef CLK24MHZ
 #define BEEPFREQ_H	3
 #define BEEPFREQ_L	6 //*/

#define BEEPER1	1
#define BEEPER2	2
 
#ifdef SIMPLE_BEEPER
  #define BEEPFREQ_H 1
  #define BEEPFREQ_L 1
#else

#ifdef CLK22MHZ
 #ifdef X2_ON
  #define BEEPFREQ_H	1	// 1 - dla beepera piezo (4kHz);	2 - dla beepera indukcyjnego (2kHz)
  #define BEEPFREQ_L	3	// 2 - dla beepera piezo (2kHz);	4 - dla beepera indukcyjnego (1kHz)
 #else
  #define BEEPFREQ_H	3
  #define BEEPFREQ_L	6 
 #endif
#else
 #define BEEPFREQ_H	2
 #define BEEPFREQ_L	4
#endif

#endif

#ifdef DISPLAY_INCLUDED
typedef struct LCD_display_st {
	BYTE  cmdWr;  		//   LCD_BASE +0x00
	BYTE  cmdRd;  		//            +0x01
	BYTE  RAMWr;		//            +0x02
	BYTE  RAMRd;  		//            +0x03
}  DISPLAY;

typedef struct {
	BYTE drawingBytes[8];
}  CHAR_HD;

CHAR_HD code CHAR_HD86 = { 0x00, 0x0E, 0x01, 0x0F, 0x11, 0x0F, 0x02, 0x01 };	// 'a' z kreska
CHAR_HD code CHAR_HD8D = { 0x02, 0x04, 0x0E, 0x10, 0x10, 0x11, 0x0E, 0x00 };  // 'c' z kreska
CHAR_HD code CHAR_HD8F = { 0x0E, 0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x02 };	// 'A' z kreska
CHAR_HD code CHAR_HD90 = { 0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F, 0x01 };	// 'E' z kreska
CHAR_HD code CHAR_HD91 = { 0x00, 0x0E, 0x11, 0x1F, 0x10, 0x0E, 0x02, 0x01 };	// 'e' z kreska
CHAR_HD code CHAR_HD92 = { 0x0C, 0x04, 0x06, 0x04, 0x0C, 0x04, 0x0E, 0x00 };  // 'l' z kreska
CHAR_HD code CHAR_HD95 = { 0x02, 0x0E, 0x15, 0x10, 0x10, 0x11, 0x0E, 0x00 };	// 'C' z kreska
CHAR_HD code CHAR_HD98 = { 0x02, 0x0F, 0x14, 0x10, 0x0E, 0x01, 0x1E, 0x00 };	// 'S' z kreska
CHAR_HD code CHAR_HD9C = { 0x08, 0x08, 0x0a, 0x0c, 0x08, 0x18, 0x0F, 0x00 };	// 'L' z kreska
CHAR_HD code CHAR_HD9E = { 0x02, 0x04, 0x0E, 0x10, 0x0E, 0x01, 0x1E, 0x00 };  // 's' z kreska
CHAR_HD code CHAR_HDA0 = { 0x02, 0x1F, 0x05, 0x02, 0x04, 0x08, 0x1F, 0x00 };	// 'Z' z kreska
CHAR_HD code CHAR_HDA1 = { 0x04, 0x1F, 0x01, 0x02, 0x04, 0x08, 0x1F, 0x00 };	// 'Z' z kropka
CHAR_HD code CHAR_HDA2 = { 0x02, 0x04, 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x00 };	// 'o' z kreska
CHAR_HD code CHAR_HDA3 = { 0x02, 0x0E, 0x15, 0x11, 0x11, 0x11, 0x0E, 0x00 };	// 'O' z kreska
CHAR_HD code CHAR_HDA4 = { 0x02, 0x04, 0x16, 0x19, 0x11, 0x11, 0x11, 0x00 };	// 'n' z kreska
CHAR_HD code CHAR_HDA5 = { 0x02, 0x15, 0x11, 0x19, 0x15, 0x13, 0x11, 0x00 };	// 'N' z kreska
CHAR_HD code CHAR_HDA6 = { 0x02, 0x04, 0x1F, 0x02, 0x04, 0x08, 0x1F, 0x00 };	// 'z' z kreska
CHAR_HD code CHAR_HDA7 = { 0x04, 0x00, 0x1F, 0x02, 0x04, 0x08, 0x1F, 0x00 };  // 'z' z kropka


#define CHAR_TAB_OFFSET  0x86
CHAR_HD code *code CHAR_TAB[34] = { &CHAR_HD86, NULL, NULL, NULL, NULL, NULL, NULL, &CHAR_HD8D,
												NULL, &CHAR_HD8F, &CHAR_HD90, &CHAR_HD91, &CHAR_HD92, NULL,
												NULL, &CHAR_HD95, NULL, NULL, &CHAR_HD98, NULL, NULL, NULL,
												&CHAR_HD9C, NULL, &CHAR_HD9E, NULL, &CHAR_HDA0, &CHAR_HDA1,
												&CHAR_HDA2, &CHAR_HDA3, &CHAR_HDA4, &CHAR_HDA5, &CHAR_HDA6,
												&CHAR_HDA7 };
#endif

#ifndef DISPLAY_INCLUDED
 void LCDInit (void);
 void LCDReset (void);
 void PrintStr (BYTE *Str, BYTE Ln, BYTE Line);
 void PrintChar (BYTE charCode, BYTE Line);
 void LCDEraseLine (BYTE line);
 void LCDErase (void);
 void PutLCDCmd (BYTE LCDCmd);
 void CharDef (void);
 void MakeBeep (BYTE beepFreq, BYTE beepTime);
 void MakeBeep2 (BYTE beepFreq, BYTE beepTime);
 void MakeBeep2_simple (BYTE beepFreq, BYTE beepTime);
 void ViewLcdCharCode (void);
#endif