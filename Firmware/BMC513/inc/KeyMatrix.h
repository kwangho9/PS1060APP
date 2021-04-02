//===================================================================
// File Name : KeyMatrix.h
// Function  : 4 x 4 Key Matrix header.
// Program   : maeam  --  Boo-Ree Multimedia Inc.
// Date      : Aug, 20, 2007
// Version   : 0.0
// Mail      : support@boo-ree.com
// Web       : www.boo-ree.com
// History
//===================================================================

#ifndef __KEYMATRIX_h__
#define __KEYMATRIX_h__

#define HIGH            (1)
#define LOW             (0)

#define debounce        (1)
#define DCgap           (250)
#define holdTime        (1000)
#define longHoldTime    (3000)

//#define millis()        (millisec.l)

#ifdef  __KEYMATRIX_c__
#define KEYMATRIX_EXT
/** local definitions **/

/** internal functions **/

/** local variable define **/
__bit __no_init bool buttonVal; // value read from button
__bit __no_init bool buttonLast; // buffered value of the button's previous state
__bit __no_init bool DCwaiting; // whether we're waiting for a double click (down)
__bit __no_init bool DConUp; // whether to register a double click on next release, or whether to wait and click
__bit __no_init bool singleOK; // whether it's OK to do a single click
__bit __no_init bool ignoreUp; // whether to ignore the button release because the click+hold was triggered
__bit __no_init bool waitForUp; // when held, whether to wait for the up event
__bit __no_init bool holdEventPast; // whether or not the hold event happened already
__bit __no_init bool longHoldEventPast;// whether or not the long hold event happened already

uint32_t downTime = 1;
uint32_t upTime = 1;
#else
#define KEYMATRIX_EXT extern
#endif

KEYMATRIX_EXT volatile Byte4 millisec;
KEYMATRIX_EXT uint8_t keyVal;

KEYMATRIX_EXT __near_func void clickEvent(void);
KEYMATRIX_EXT __near_func void doubleClickEvent(void);
KEYMATRIX_EXT __near_func void holdEvent(void);
KEYMATRIX_EXT __near_func void longHoldEvent(void);

KEYMATRIX_EXT __near_func void KeyMatrix_Init(void);
KEYMATRIX_EXT __near_func uint8_t keypad_key(void);
KEYMATRIX_EXT __near_func uint8_t keypad_getNumber(void);
KEYMATRIX_EXT __near_func void keypad_init(void);

KEYMATRIX_EXT __near_func uint8_t KeyService(void);


KEYMATRIX_EXT __near_func void KeyMatrix_Test(void);

#endif  //__KEYMATRIX_h__
