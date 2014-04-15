
#define KASUJ    	0x31	// 'C'
#define PRZERWIJ 	0x41	// 'D'
#define KOK     	0x11	// 'A'
#define KA			0x11  // 'A'
#define KB     		0x21	// 'B'
#define KC			0x31  // 'C'
#define KD			0x41  // 'D'
#define K0			0x43
#define K1			0x14
#define K2			0x13
#define K3			0x12
#define K4			0x24
#define K5			0x23
#define K6			0x22
#define K7			0x34
#define K8			0x33
#define K9			0x32
#define LEWO		0x44
#define PRAWO		0x42

#define LINE_IN_LEN  4
#define LINE_IN_MASK 0xF0
#define LINE_OUT_LEN 4

//-- Data section --------------------------------------------------------------

/*== Prototype section =======================================================*/

// Wait a key during Step*100ms, if Beep = TRUE make a beep
BYTE Kbd_GetKey(BYTE Step, BYTE Beep);

// Convert key codes to key values
BYTE KeyVal(BYTE Code);

BYTE GetDecimalStr (BYTE maxLength, BYTE *strData, BYTE *strName, BYTE nameLength);
BYTE EditDecimalStr (BYTE maxLength, BYTE *strData, BYTE *strName, BYTE nameLength);

BYTE ChooseMenuItem (BYTE code**menuItemName, BYTE itemNo);
/*
BYTE GetString (BYTE length, BYTE *strData);
BYTE Pressed_ONOFF15 (void);
BYTE EditProcentValue (BYTE maxLength, BYTE *strData, BYTE *strName, BYTE nameLength);
*/
