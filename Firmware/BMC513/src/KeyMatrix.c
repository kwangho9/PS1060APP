//===================================================================
// File Name : KeyMatrix.c
// Function  : 4 x 4 Key Matrix function.
// Program   : maeam  --  Boo-Ree Multimedia Inc.
// Date      : Aug, 20, 2007
// Version   : 1.0
// Mail      : support@boo-ree.com
// Web       : www.boo-ree.com
// History
//===================================================================

#define __KEYMATRIX_c__
#include "includes.h"

#define NO_KEY              (255)
#define SET_KEY             (13)
#define ENTER_KEY           (14)
#define DEL_KEY             (15)

const uint8_t keypad_Char[] = {'1', '2', '3', 'a', '4', '5', '6', 'b', '7', '8', '9', 'c', 'f', '0', 'e', 'd' };
//const uint8_t keypad_Value[] = {1, 2, 3, 10, 4, 5, 6, 11, 7, 8, 9, 12, 15, 0, 14, 13};
const uint8_t keypad_Value[] = {10, 11, 12, 13, 3, 6, 9, 14, 2, 5, 8, 0, 1, 4, 7, 15};
// return value : up/down(1/0) ... key value(0 ~ 15)
__near_func uint8_t KeyMatrix_Scan(void)
{
    uint8_t i, tmp, key = 0;

    tmp = rP5 & 0x0F;
//    if( tmp == 0x0F ) return (key | 0x80);
    if( tmp == 0x0F ) return HIGH;

    for(i=0; i<4; i++) {
        if( (tmp & (1<<i)) == 0 ) {
            key = i;
            break;
        }
    }

    for(i=4; i < 8; i++) {
        rP6 = (1<<i);
        {volatile uint8_t k; for(k=0; k<10; k++) ; }
        if( (rP5 & 0x0F) == 0x0F ) {
            key += (7 - i) * 4;
//            printf((char *)"Row Key = %d\n", key);
            break;
        }
    }
    rP6 = 0x00;

    // 위에서 rP5를 scan하는 중에 button이 release되면, 잘못된 값을 가져온다.
    // 따라서, 마지막으로 button release를 check한다.
    if( (rP5 & 0x0F) == 0x0F ) return HIGH;

//    printf((char *)"key = %d\n", key);

    keyVal = keypad_Value[key];
//    return key;
    return LOW;
}

__near_func void KeyMatrix_Init(void)
{
    uint8_t flag = 0;

    // Row Key.
	rP5MOD0 = 0x00;				// GP5[0:3] : Input(with Pull-up enable)

    // Column Key.
	rP6 = rP6 & 0x00;
	rP6MOD1 = 0x55;				// GP6[4:7] : Output Low(with Pull-up disable)

	rP5PU = rP5PU | 0x0F;         // GP5[0:3] Pull-up Enable.
	rP6PU = rP6PU & ~0xF0;        // GP6[4:7] Pull-up Disable.

    do {
        if( KeyMatrix_Scan() == LOW ) {
            printf((char *)"%d ", keyVal);
            flag = 0;
        } else {
            if( flag == 0 ) {
                printf((char *)"\nkey : ");
                flag = 1;
            }
        }
    } while( getkey() != ESC );
//        printf((char *)"rP5 = 0x%x\n", rP5 & 0x0F);
//    } while( getchar() != ESC );
}

__near_func uint8_t keypad_key(void)
{
    uint8_t event = 0;

    buttonVal = KeyMatrix_Scan();
//    buttonVal = (keyVal & 0x80) ? HIGH : LOW;

    if( (buttonVal == LOW) && (buttonLast == HIGH) && ((millis() - upTime) > debounce) ) {
        downTime = millis();
        ignoreUp = false;
        waitForUp = false;
        singleOK = true;
        holdEventPast = false;
        longHoldEventPast = false;
        if( ((millis() - upTime) < DCgap) && (DConUp == false) && (DCwaiting == true) ) {
            DConUp = true;
        } else {
            DConUp = false;
        }
        DCwaiting = false;
    } else if( (buttonVal == HIGH) && (buttonLast == LOW) && ((millis() - downTime) > debounce) ) {
        if( !ignoreUp ) {
            upTime = millis();
            if( DConUp == false ) {
                DCwaiting = true;
            } else {
                event = 2;
                DConUp = false;
                DCwaiting = false;
                singleOK = false;
            }
        }
    }

    if( (buttonVal == HIGH) && ((millis() - upTime) >= DCgap) && (DCwaiting == true) && (DConUp == false) && (singleOK == true) && (event != 2) ) {
        event = 1;
        DCwaiting = false;
    }

    if( (buttonVal == LOW) && ((millis() - downTime) >= holdTime) ) {
        if( !holdEventPast ) {
            event = 3;
            waitForUp = true;
            ignoreUp = true;
            DConUp = false;
            DCwaiting = false;
            holdEventPast = true;
        }

        if( (millis() - downTime) >= longHoldTime ) {
            if( !longHoldEventPast ) {
                event = 4;
                longHoldEventPast = true;
            }
        }
    }

    buttonLast = buttonVal;

    return event;
}

__near_func void keypad_init(void)
{
    // Row Key.
	rP5MOD0 = 0x00;				// GP5[0:3] : Input(with Pull-up enable)

    // Column Key.
	rP6 = rP6 & 0x00;
	rP6MOD1 = 0x55;				// GP6[4:7] : Output Low(with Pull-up disable)

	rP5PU = rP5PU | 0x0F;         // GP5[0:3] Pull-up Enable.
	rP6PU = rP6PU & ~0xF0;        // GP6[4:7] Pull-up Disable.

    // variable initialize
    millisec.l = 0;

    keyVal = 0;
    buttonVal = HIGH;
    buttonLast = HIGH;
    singleOK = false;
    DConUp = false;
    ignoreUp = false;
    waitForUp = false;
    holdEventPast = false;
    longHoldEventPast = false;
}

typedef uint8_t (*func_ptr_t)(void);

static uint8_t function0(void);
static uint8_t function1(void);
static uint8_t function2(void);
static uint8_t function3(void);
static uint8_t function4(void);
static uint8_t function5(void);
static uint8_t function6(void);
static uint8_t function7(void);
static uint8_t function8(void);
static uint8_t function9(void);
static uint8_t function10(void);
static uint8_t function11(void);
static uint8_t function12(void);
static uint8_t function13(void);
static uint8_t function14(void);
static uint8_t function15(void);

const func_ptr_t clickFunction[16] = {function0, function1, function2, function3, function4, function5, function6, function7, function8, function9, function10, function11, function12, function13, function14, function15};

static uint8_t function0(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}
static uint8_t function1(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}
static uint8_t function2(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}
static uint8_t function3(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}
static uint8_t function4(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}
static uint8_t function5(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}
static uint8_t function6(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}
static uint8_t function7(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}
static uint8_t function8(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}
static uint8_t function9(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}
static uint8_t function10(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}
static uint8_t function11(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}
static uint8_t function12(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}
static uint8_t function13(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}
static uint8_t function14(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}
static uint8_t function15(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);

    return 0;
}

__near_func void clickEvent(void)
{
	printf((char *)"click key = %d\n", keyVal);
//	lcd_printf((char *)"click key = %d\n", keyVal);
//    lcd_puts((char *)"click key\n");
    clickFunction[keyVal]();
}

__near_func void doubleClickEvent(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);
	lcd_printf((char *)"double key = %d\n", keyVal);
    switch( keyVal ) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        case 11:
            break;
        case 12:
            break;
        case 13:
            break;
        case 14:
            break;
        case 15:
            break;
    }
}

__near_func void holdEvent(void)
{
    uint8_t key = keyVal;
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);
	lcd_printf((char *)"hold key = %d\n", keyVal);
    if( key == 0 ) {
    } else if( key == 1 ) {
    } else if( key == 2 ) {
    } else if( key == 3 ) {
    } else if( key == 4 ) {
    } else if( key == 5 ) {
    } else if( key == 6 ) {
    } else if( key == 7 ) {
    } else if( key == 8 ) {
    } else if( key == 9 ) {
    } else if( key == 10 ) {
    } else if( key == 11 ) {
    } else if( key == 12 ) {
    } else if( key == 13 ) {
    } else if( key == 14 ) {
    } else if( key == 15 ) {
    } else {
    }
}

__near_func void longHoldEvent(void)
{
	printf((char *)"%s()[%d] : key = %d\n", __func__, __LINE__, keyVal);
	lcd_printf((char *)"longHold key = %d\n", keyVal);
}


/*
 *
 */
//struct _keypad_variable {
    uint8_t keyValid = 0;
    bool keyDone = false;
    uint8_t keyNumber = 0;
//} tKeyVariable;
__near_func uint8_t keypad_getNumber(void)
{
    uint8_t ret = NO_KEY;
    uint8_t key = keypad_key();

    if( keyDone == true ) {
        keyDone = false;
        keyNumber = 0;
    }

    if( key == 0x1 ) {             // single key
        ret = keyVal;
//        if( (ret >= 0) && (ret <= 9) ) {
        if( ret <= 9 ) {
            keyNumber = (keyNumber * 10) + ret;
            lcd_printf((char *)"%d", ret);
        } else if( ret == DEL_KEY ) { // input delete.
            lcd_printf((char *)"\b \b");
            keyNumber = keyNumber / 10;
        } else if( (ret == ENTER_KEY) || (ret == SET_KEY) ) { // input end.
            if( ret == ENTER_KEY ) lcd_putc(' ');
            keyDone = true;
        }
    }

    return ret;
}

/* 2-digit만 항상 입력해야 한다. -> 0F, 10, 00, 111(X)
 * DEL_KEY는 입력 문자가 2개 이상일때 가능한다.
 * 2개 문자가 입력되면, 더이상 새로운 문자를 받지 않는다.
 */
uint8_t hexDigit = 0;
__near_func uint8_t keypad_getHex(void)
{
    uint8_t ret = NO_KEY;
    uint8_t key = keypad_key();

    if( hexDigit == 0 ) keyNumber = 0;

    if( key == 0x1 ) {             // single key
        ret = keyVal;
        if( hexDigit == 2 ) {
            if( ret == DEL_KEY ) { // input delete.
                lcd_printf((char *)"\b \b");
                keyNumber = keyNumber / 16;
                hexDigit--;
            } else if( (ret == ENTER_KEY) || (ret == SET_KEY) ) { // input end.
                lcd_putc(' ');
                hexDigit = 0;
            } 
        } else {
            keyNumber = (keyNumber * 16) + ret;
            lcd_printf((char *)"%c", (ret < 10) ? ret + '0' : (ret - 10) + 'A');
            hexDigit++;
            printf((char *)"hexDigit = %d(%d)\n", hexDigit, ret);
            ret = NO_KEY;
        }
    }

    return ret;
}


uint8_t gSW[16];        // PS1120 switch buffer.
uint8_t index;
uint8_t opcode;
uint8_t mode = 0;
static uint8_t nSW=0;
static int8_t delta=1;
static bool Toggle=true;
const char __code *opName[] = {"OFF", "ON", "H OFF", "H ON", "R OFF", "R ON", "F OFF", "F ON"};
bool toggle = 0;
__near_func uint8_t KeyService(void)
{
    uint8_t bt, key;


    Debug.gTransmit = 0;
    Debug.gReceive = 0;
    /*
    for(uint8_t k=0; k<16; k++) gSW[k] = 0;
    gSW[0] = 0x12;
    gSW[3] = 0x89;
    printf((char *)"value : 0x%X, 0x%X, 0x%X, 0x%X\n", gSW[3], gSW[2], gSW[1], gSW[0]);
    printf((char *)"gSW : 0x%02X%02X%02X%02X\n", gSW[3], gSW[2], gSW[1], gSW[0]);
    printf((char *)"gSW : 0x%lX\n", *(uint32_t *)&gSW[0]);
    do {
        *(uint32_t *)&gSW[0] = 0x12345678;
        printf((char *)"gSW : 0x%lX\n", *(uint32_t *)&gSW[0]);
        *(uint32_t *)&gSW[0] = 0x0;
        gSW[3] = 0x55;
        gSW[0] = 0xAA;
        printf((char *)"gSW : 0x%lX\n", *(uint32_t *)&gSW[0]);
        gSW[7] = 0x55;
        gSW[4] = 0xAA;
        printf((char *)"gSW : 0x%lX\n", *(uint32_t *)&gSW[4]);
    } while( getchar() != ESC );
    do {
//        if( keypad_key() == 1 ) {
//            lcd_printf((char *)"%c(%d) ", keypad_Char[keyVal], keypad_Value[keyVal]);
//        }
        bt = keypad_getNumber();
        if( bt != 0xFF ) printf((char *)"input number = %d\n", bt);
    } while( getkey() != ESC );
    */
    switch( mode ) {
        case 0:         // pattern reset
            mSW_ALL_FORCE_OFF();
            for(uint8_t k=0; k<16; k++) gSW[k] = 0;
//            mSW_PATTERN_RESET();
//            mSW_SOFTWARE_RESET();       // group type만 clear된다.
        case 1:
            Toggle = false;
            index = 0;
            lcd_clear();
//            mSW_ALL_FORCE_OFF();
//            lcd_puts((char *)"\n- click");
//            lcd_puts((char *)"\n- double click");
//            lcd_puts((char *)"\n- long press");
//            lcd_puts((char *)"\npress click/D-click/L-click");
            lcd_printf((char *)"[ %s Test ]\n", deviceName[gDevice].str);
            mode = 2;
            break;
        case 2:
            bt = keypad_key();

            if( bt == 1 ) {
                if( keyVal <= 7 ) {             // select switch input.
                    opcode = keyVal;
                    for(uint8_t k=0; k<(gMaxFrame+3); k++) gFrame[k] = 0;

                    lcd_clear();
                    lcd_printf((char *)"%s switch ?\n", opName[opcode]);
//                    lcd_puts((char *)"Enter switch ? ");
                    printf((char *)"opcode = %d, switch : ", opcode);
                    mode = 30;
                } else if( keyVal == 10 ) {     // hex command input
                    index = 0;
                    lcd_clear();
                    lcd_puts((char *)"Enter Hex ? ");
                    mode = 50;
                } else if( keyVal == 15 ) {     // hex command input

                    if( toggle == 1 ) toggle = 0;
                    else toggle = 1;

                    rP4_b7 = toggle;
                    printf((char *)"rP4_b7 = %d\n", toggle);
                }
                if( (keyVal == SET_KEY) ) mode = 0;
            } else if( bt == 2 ) {              // double key input
                lcd_setcursor(1, 2);            // 2번째 line의 1번째 문자 위치.
                lcd_printf((char *)"%d", keyVal);
                if( keyVal == 0 ) {
                    if( Toggle == true ) mSW_ALL_OFF();
                    else mSW_ALL_ON();
                    Toggle = !Toggle;
                } else if( keyVal == 1 ) {      // row select all switch command.
                    printf((char *)"Toggle = %d, index = %d\n", Toggle, index);
                    if( Toggle == true ) {
                        mSW_SINGLE_SWITCH_OFF(0, index);
                    } else {
                        mSW_SINGLE_SWITCH_ON(0, index);
                    }
                    index++;
                    if( index == (30 * (1<<gDevice)) ) {
                        index = 0;
                        Toggle = !Toggle;
                    }
                } else if( keyVal == 2 ) {      // row select all switch command.
                    printf((char *)"Toggle = %d, index = %d\n", Toggle, index);
                    set_bit(index, gSW);
                    if( Toggle == true ) {
                        mSW_ROW_ALL_OFF(0, gSW, 2);
                    } else {
                        mSW_ROW_ALL_ON(0, gSW, 2);
                    }
                    index++;
                    if( index == 10 ) {
                        index = 0;
                        for(uint8_t k=0; k<16; k++) gSW[k] = 0;
                        Toggle = !Toggle;
                    }
                } else if( keyVal == 3 ) {      // column select all switch command.
                    printf((char *)"Toggle = %d, index = %d\n", Toggle, index);
                    set_bit(index, gSW);
                    if( Toggle == true ) {
                        mSW_COLUMN_ALL_OFF(0, gSW, 2);
                    } else {
                        mSW_COLUMN_ALL_ON(0, gSW, 2);
                    }
                    index++;
                    if( index == (3 * (1 << gDevice)) ) {
                        index = 0;
                        for(uint8_t k=0; k<16; k++) gSW[k] = 0;
                        Toggle = !Toggle;
                    }
                } else if( keyVal == 4 ) {
//                    for(uint8_t k=0; k<gMaxBytes; k++) gSW[k] = 0xFF;
                    mSW_ALL_SWITCH_OFF(0, gSW, gMaxBytes);

                    printf((char *)"Toggle = %d, index = %d\n", Toggle, index);
                    for(uint8_t k=0; k<gMaxBytes; k++) gSW[k] = rand() & 0xFF;
                    mSW_ALL_SWITCH_ON(0, gSW, gMaxBytes);
                } else if( keyVal == 5 ) {
                    printf((char *)"Pattern Reset!\n");
                    mSW_PATTERN_RESET();
                }
            } else if( bt == 3 ) {              // long key input
                if( keyVal == 1 ) {
                    mode = 40;
                    nSW = 0;
                    delta = 1;
                } else if( keyVal == 2 ) {
                    mode = 41;
                } else if( keyVal == 3 ) {
                    mode = 42;
                    mSW_ALL_OFF();
                    for(uint8_t k=0; k<16; k++) gSW[k] = 0;
                } else if( keyVal == 4 ) {
                    mode = 43;
                    nSW = 0;
                } else if( keyVal == 5 ) {
                    mode = 44;
                } else if( keyVal == 9 ) {      // device detect.
                }
            }
            break;

        case 30:         // get switch numbers.
            key = keypad_getNumber();
            if( (key == ENTER_KEY) || (key == SET_KEY) ) {
                set_bit(keyNumber, &gFrame[2]);
                printf((char *)"%d ", keyNumber);
                if( key == SET_KEY ) {
                    printf((char *)"\n");
                    gFrame[0] = 0xAA;
                    gFrame[1] = 0x38 | opcode;

                    gFrame[gMaxBytes+2] = 0x0;

                    send_command(gFrame, gMaxFrame);

                    for(uint8_t k=0; k<gMaxFrame; k++) gFrame[k] = 0;
                    mode = 1;
                }
            }
            break;

        case 40:
            mSW_SWITCH(SW_ON, 0, nSW);
            mdelay(10);
            mSW_SWITCH(SW_OFF, 0, nSW);
            mdelay(10);
            if( nSW == (gTotalSwitch - 1) ) delta = -1;
            else if( nSW == 0 ) delta = 1;
            nSW = nSW + delta;

            if( (keypad_key() == 1) && (keyVal == SET_KEY) ) mode = 1;
            break;
        case 41:
            if( Toggle == true ) mSW_ALL_ON();
            else mSW_ALL_OFF();
            mdelay(100);
            Toggle = !Toggle;

            if( (keypad_key() == 1) && (keyVal == SET_KEY) ) mode = 1;
            break;
        case 42:
            nSW = rand() % gTotalSwitch;
            if( Toggle == true ) {
                mSW_SWITCH(SW_ON, 0, nSW);
                mdelay(50);
                set_bit(nSW, &gSW[0]);
                if( gDevice == 0 ) {
//                    printf((char *)"Switch ON : 0x%lX\n", *(uint32_t *)&gSW[0]);
                    if( *(uint32_t *)&gSW[0] == 0xFCFFFFFF ) {
                        Toggle = false;
                    }
                } else if( gDevice == 1 ) {
//                    printf((char *)"Switch ON : 0x%lX, 0x%lX\n", *(uint32_t *)&gSW[0], *(uint32_t *)&gSW[4]);
                    if( (*(uint32_t *)&gSW[0] == 0xFFFFFFFF) &&
                        (*(uint32_t *)&gSW[4] == 0xF0FFFFFF) ) {
                        Toggle = false;
                    }
                } else if( gDevice == 2 ) {
                    if( (*(uint32_t *)&gSW[0] == 0xFFFFFFFF) &&
                        (*(uint32_t *)&gSW[4] == 0xFFFFFFFF) &&
                        (*(uint32_t *)&gSW[8] == 0xFFFFFFFF) &&
                        (*(uint32_t *)&gSW[12] == 0x00FFFFFF) ) {
                        Toggle = false;
                    }
                }

                if( Toggle == false ) mdelay(500);
            } else {
                mSW_SWITCH(SW_OFF, 0, nSW);
                mdelay(50);
                clear_bit(nSW, &gSW[0]);
                if( gDevice == 0 ) {
//                    printf((char *)"Switch OFF : 0x%lX\n", *(uint32_t *)&gSW[0]);
                    if( *(uint32_t *)&gSW[0] == 0x0 ) {
                        Toggle = true;
                    }
                } else if( gDevice == 1 ) {
//                    printf((char *)"Switch ON : 0x%lX, 0x%lX\n", *(uint32_t *)&gSW[0], *(uint32_t *)&gSW[4]);
                    if( (*(uint32_t *)&gSW[0] == 0x0) &&
                        (*(uint32_t *)&gSW[4] == 0x0) ) {
                        Toggle = true;
                    }
                } else if( gDevice == 2 ) {
                    if( (*(uint32_t *)&gSW[0] == 0x0) &&
                        (*(uint32_t *)&gSW[4] == 0x0) &&
                        (*(uint32_t *)&gSW[8] == 0x0) &&
                        (*(uint32_t *)&gSW[12] == 0x0) ) {
                        Toggle = true;
                    }
                }

                if( Toggle == true ) mdelay(500);
            }

            if( (keypad_key() == 1) && (keyVal == SET_KEY) ) mode = 1;
            break;
        case 43:
            if( nSW < gTotalSwitch ) {
                mSW_SWITCH(SW_ON, 0, nSW);
            }

            if( nSW >= 10 ) {
                mSW_SWITCH(SW_OFF, 0, nSW-10);
                if( (nSW-10) == (gTotalSwitch-1) ) {
                    nSW = 0;
                }
            }
//            printf((char *)"nSW = %d\n", nSW);
            mdelay(20);
//            getchar();
            nSW++;

            if( (keypad_key() == 1) && (keyVal == SET_KEY) ) mode = 1;
            break;

        case 44:
//            mdelay(20);
//            if( (keypad_key() == 1) && (keyVal == SET_KEY) ) mode = 1;
            break;

        case 50:
            key = keypad_getHex();
            if( (key == ENTER_KEY) || (key == SET_KEY) ) {
                gFrame[index++] = keyNumber;
                printf((char *)"hex = 0x%X\n", keyNumber);
                if( key == SET_KEY ) {
                    send_command(gFrame, index);

                    mode = 1;
                }
            }
            break;
    }

    return 0;
}


__near_func void KeyMatrix_Test(void)
{
    uint8_t bt;

    printf((char *)"%s()[%d]:\n", __func__, __LINE__);

//    KeyMatrix_Init();

    keypad_init();

    do {
        bt = keypad_key();

        if( bt == 1 ) {
            clickEvent();
        } else if( bt == 2 ) {
            doubleClickEvent();
        } else if( bt == 3 ) {
            holdEvent();
        } else if( bt == 4 ) {
            longHoldEvent();
        }
    } while( getkey() != ESC );
}

/*		EOF		*/

