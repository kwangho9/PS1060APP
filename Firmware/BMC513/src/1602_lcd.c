/******************************* Program Notes ******************************
 
            This program uses a 4-bit data interface and it uses the busy
              flag to determine when the LCD controller is ready.  The LCD
              RW line (pin 5) must therefore be connected to the uP.
 
            The use of the busy flag does not mean that all of the software
              time delays have been eliminated.  There are still several
              required in the LCD initialization routine where the busy flag
              cannot yet be used.  These delays are have been implemented at
              least twice as long as called for in the data sheet to
              accommodate some of the out of spec displays that may show up.
              There are also some other software time delays required to
              implement timing requirements such as setup and hold times for
              the various control signals.
 
  ***************************************************************************
 
            The four data lines as well as the three control lines may be
              implemented on any available I/O pin of any port.  These are
              the connections used for this program:
 
                 -----------                   ----------
                |   BMC513  |                 |   LCD    |
                |           |                 |          |
                |       GP43|---------------->|D7        |
                |       GP42|---------------->|D6        |
                |       GP41|---------------->|D5        |
                |       GP40|---------------->|D4        |
                |           |                 |D3        |
                |           |                 |D2        |
                |           |                 |D1        |
                |           |                 |D0        |
                |           |                 |          |
                |       GP46|---------------->|E         |
                |       GP45|---------------->|RW        |
                |       GP44|---------------->|RS        |
                 -----------                   ----------
 
  **************************************************************************/

#define __LCD_c__
#include <includes.h>

// LCD interface (should agree with the diagram above)
#define lcd_RS_port			(rP4_b4)
#define lcd_RW_port			(rP4_b5)
#define lcd_E_port			(rP4_b6)

#define lcd_D4_port			(rP4_b0)	// lcd D4 connection
#define lcd_D5_port			(rP4_b1)	// lcd D5 connection
#define lcd_D6_port			(rP4_b2)	// lcd D6 connection
#define lcd_D7_port			(rP4_b3)	// lcd D7 connection

#define lcd_D7_pin			(rP4_b3)  // busy flag

#define lcd_CTRL_output()   (rP4MOD1 = 0x55)        // P4[4:7] output mode.
#define lcd_DATA_output()   (rP4MOD0 = 0x55)        // P4[0:3] output mode.
#define lcd_DATA_input()    (rP4MOD0 = 0x00)        // P4[3] input mode.

#if 0
// LCD module information
#define lcd_LineOne     0x00                    // start of line 1
#define lcd_LineTwo     0x40                    // start of line 2
//#define   lcd_LineThree   0x14                  // start of line 3 (20x4)
//#define   lcd_lineFour    0x54                  // start of line 4 (20x4)
//#define   lcd_LineThree   0x10                  // start of line 3 (16x4)
//#define   lcd_lineFour    0x50                  // start of line 4 (16x4)

// LCD instructions
#define lcd_Clear           0x01	// 0b00000001          // replace all characters with ASCII 'space'
#define lcd_Home            0x02	// 0b00000010          // return cursor to first position on first line
#define lcd_EntryMode       0x06	// 0b00000110          // shift cursor from left to right on read/write
#define lcd_DisplayOff      0x08	// 0b00001000          // turn display off
#define lcd_DisplayOn       0x0F	// 0b00001111          // display on, cursor off, don't blink character
#define lcd_FunctionReset   0x30	// 0b00110000          // reset the LCD
#define lcd_FunctionSet4bit 0x28	// 0b00101000          // 4-bit data, 2-line display, 5 x 7 font
#define lcd_SetCursor       0x80	// 0b10000000          // set cursor position
#endif

#define lcd_send_ctrl(c)		lcd_send_byte(0, c)
#define lcd_send_char(c)		lcd_send_byte(1, c)
#define lcd_get_ctrl(c)		lcd_get_byte(0)
#define lcd_get_char(c)		lcd_get_byte(1)

// Function Prototypes

// Function Prototypes
static void lcd_gotoxy(uint8_t x, uint8_t y);
static void lcd_write_nibble(uint8_t);
static void lcd_check_BF_4(void);
static void lcd_send_byte(bool rs, uint8_t c);

void lcd_init(void);
void lcd_putc(char);
void lcd_puts(char *);

uint8_t display;	// 0-bit : blank, 1-bit : cursor, 2-bit : display, 3-bit : backlight,
uint8_t pos;

const uint8_t init_cmd[] = {
	lcd_FunctionSet4bit,  // set mode, lines, and font
	lcd_DisplayOff,       // turn display OFF
	lcd_Clear,            // clear display RAM
	lcd_EntryMode,        // set desired shift characteristics
	lcd_DisplayOn        // turn the display ON
};

void _delay_us(uint8_t u)
{
	volatile uint8_t i;

	do {
		for(i=0; i<10; i++);
	} while( --u );
}

void _delay_ms(uint8_t m)
{
	do {
		_delay_us(100);
	} while( --m );
}

/*============================== 4-bit LCD Functions ======================*/
/*
  Name:     lcd_init
  Purpose:  initialize the LCD module for a 4-bit data interface
  Entry:    equates (LCD instructions) set up for the desired operation
  Exit:     no parameters
  Notes:    uses the busy flag instead of time delays when possible
*/
void lcd_init(void)
{
	uint8_t i;

	pos = 1;
	// configure the microprocessor pins for the data lines
	// using port : GP4[0:7]
	//    RS      -> P4.4
	//    E       -> P4.6
	//    R/W     -> P4.5
	//    DB[4:7] -> P4[0:3]
    rP4PU = 0x0;
	lcd_CTRL_output();
	lcd_DATA_output();

	// Power-up delay
	_delay_ms(100);                                 // initial 40 mSec delay

	/* IMPORTANT - At this point the LCD module is in the 8-bit mode and
	 * it is expecting to receive 8 bits of data, one bit on each of its 8 data lines,
	 * each time the 'E' line is pulsed.
	 *
	 * Since the LCD module is wired for the 4-bit mode, only the upper four data lines are 
	 * connected to the microprocessor and the lower four data lines are typically left open.
	 * Therefore, when the 'E' line is pulsed, the LCD controller will read whatever data has
	 * been set up on the upper four data lines and the lower four data lines will be high
	 * (due to internal pull-up circuitry).
	 *
	 * Fortunately the 'FunctionReset' instruction does not care about what is on the lower 
	 * four bits so this instruction can be sent on just the four available data lines and
	 * it will be interpreted properly by the LCD controller.
	 * The 'lcd_write_nibble' subroutine will accomplish this if the control lines have
	 * previously been configured properly.
	 */

	// Set up the RS, E, and RW lines for the 'lcd_write_nibble' function.
	lcd_RS_port = 0;		// select the Instruction Register (RS low)
	lcd_E_port = 0;		// make sure E is initially low
	lcd_RW_port = 0;		// write to LCD module (RW low)

	// Reset the LCD controller
//	for(i=0; i<3; i++) {
		lcd_write_nibble(lcd_FunctionReset);                 // first part of reset sequence
		_delay_ms(10);                                  // 4.1 mS delay (min)
//	}
    lcd_write_nibble(lcd_FunctionReset);                 // first part of reset sequence
    _delay_us(200);                                  // 100 us delay (min)

    lcd_write_nibble(lcd_FunctionReset);                 // first part of reset sequence
    _delay_us(80);                                  // 100 us delay (min)

	/* Preliminary Function Set instruction - used only to set the 4-bit mode.
	 * The number of lines or the font cannot be set at this time since the controller 
	 * is still in the 8-bit mode, but the data transfer mode can be changed since 
	 * this parameter is determined by one of the upper four bits of the instruction.
	 */
 
	lcd_write_nibble(lcd_FunctionSet4bit);               // set 4-bit mode
//	_delay_us(80);                                  //  40 uS delay (min)
// --> from this point on the busy flag is available <--   

	for(i=0; i < sizeof(init_cmd); i++) {
		lcd_check_BF_4();                               // make sure LCD controller is ready
		lcd_send_ctrl(init_cmd[i]);
        _delay_ms(10);
	}
	lcd_send_ctrl(lcd_DisplayOn);
    _delay_us(100);

	lcd_flag = 1;
	display = (1<<0) | (1<<1) | (1<<2);

    lcd_display(1);
    lcd_blank(1);
    rP4_b7 = 1;         // back-light ON.
}

/*...........................................................................
  Name:     lcd_check_BF_4
  Purpose:  check busy flag, wait until LCD is ready
  Entry:    no parameters
  Exit:     no parameters
  Notes:    main program will hang if LCD module is defective or missing
            data is read while 'E' is high
            both nibbles must be read even though desired information is only in the high nibble
*/
static void lcd_check_BF_4(void)
{
#if 0
	bool busy_flag_copy;                         // busy flag 'mirror'

#if 1
	lcd_DATA_input();

	lcd_RS_port = 0;	// select the Instruction Register (RS low)
	lcd_RW_port = 1;	// read from LCD module (RW high)

	do
	{
		busy_flag_copy = 0;	// initialize busy flag 'mirror'
		lcd_E_port = 1;	// Enable pin high
		_delay_us(1);		// implement 'Delay data time' (160 nS) and 'Enable pulse width' (230 nS)

		busy_flag_copy = lcd_D7_pin;	// get actual busy flag status

		lcd_E_port = 0;	// Enable pin low
		_delay_us(1);                               // implement 'Address hold time' (10 nS), 'Data hold time' (10 nS), and 'Enable cycle time' (500 nS )
        
// read and discard alternate nibbles (D3 information)
		lcd_E_port = 1;	// Enable pin high
		_delay_us(1);		// implement 'Delay data time' (160 nS) and 'Enable pulse width' (230 nS)
		lcd_E_port = 0;	// Enable pin low
		_delay_us(1);		// implement 'Address hold time (10 nS), 'Data hold time' (10 nS), and 'Enable cycle time' (500 nS )
        
        putchar('.');
	} while (busy_flag_copy);		// check again if busy flag was high
#endif

	// arrive here if busy flag is clear -  clean up and return 
	lcd_RW_port = 0;		// write to LCD module (RW low)

	lcd_DATA_output();
#else
    volatile uint16_t k; for(k=0; k<1000; k++);
#endif
}

/*...........................................................................
  Name:     lcd_write_nibble
  Purpose:  send a nibble (4-bits) of information to the LCD module
  Entry:    (theByte) contains a byte of data with the desired 4-bits in the upper nibble
            RS is configured for the desired LCD register
            E is low
            RW is low
  Exit:     no parameters
  Notes:    use either time delays or the busy flag
*/
static void lcd_write_nibble(uint8_t theByte)
{
	rP4 = (rP4 & 0xF0) | ((theByte & 0xF0) >> 4);       // write the data
                                                        // 'Address set-up time' (40 nS)
	lcd_E_port = 1;     // Enable pin high
	_delay_us(1);       // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)
	lcd_E_port = 0;     // Enable pin low
	_delay_us(1);       // implement 'Data hold time' (10 nS) and 'Enable cycle time' (500 nS)
}

static uint8_t lcd_read_nibble(void)
{
	uint8_t nibble;

	lcd_E_port = 1;	// Enable pin high
	_delay_us(1);		// implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)

	nibble = (rP4 & 0x0F);			// read the data

	lcd_E_port = 0;	// Enable pin low
	_delay_us(1);		// implement 'Data hold time' (10 nS) and 'Enable cycle time' (500 nS)

	return nibble;
}

static void lcd_send_byte(bool rs, uint8_t c)
{
	lcd_check_BF_4();			// make sure LCD controller is ready    

	lcd_RW_port = 0;			// write to LCD module (RW low)
	lcd_RS_port = rs;		// select the Data(RS high)/Command(RS low) Register
	lcd_E_port = 0;			// make sure E is initially low

	lcd_write_nibble(c);		// write the upper 4-bits of the data
	lcd_write_nibble(c << 4);	// write the lower 4-bits of the data
}

static uint8_t lcd_get_byte(bool rs)
{
	char ch;

	lcd_check_BF_4();		// make sure LCD controller is ready    

	lcd_DATA_input();

	lcd_RW_port = 1;		// write to LCD module (RW high)
	lcd_RS_port = rs;		// select the Data Register (RS high)
						// select the Command Register (RS low)
	lcd_E_port = 0;		// make sure E is initially low

	ch = lcd_read_nibble();	// write the upper 4-bits of the data
	ch = (ch << 4) | lcd_read_nibble();		// write the lower 4-bits of the data

	lcd_DATA_output();

	return ch;
}

static void lcd_gotoxy(uint8_t x, uint8_t y)
{
	uint8_t addr;

	if( y != 1 ) addr = lcd_LineTwo;
	else addr = lcd_LineOne;

	addr += x - 1;
	lcd_send_ctrl(lcd_SetCursor | addr);
}

static void lcd_line_scroll(void)
{
	uint8_t i, ch;

	for(i=0; i<16; i++) {
		lcd_gotoxy(i+1, 2);		// second line.
		ch = lcd_get_char();	// second line read.
		lcd_gotoxy(i+1, 2);		// second line.
		lcd_send_char(' ');     // second line clear.
//        putchar(ch);

		lcd_gotoxy(i+1, 1);		// first line.
		lcd_send_char(ch);
	}
}




void lcd_putc(char c)
{
	switch(c) {
		case '\f':					// '\f' -> 0x0C, FF(form feed)
			lcd_send_ctrl(lcd_Clear);	// first line.
									// x, y clear.
			_delay_ms(4);
			pos = 1;
			break;
		case '\n':					// '\n' -> 0x0A, LF(line feed)
		case '\r':					// '\r' -> 0x0D, CR(carraige return)
			if( pos > 16 ) {
				// line scroll.
				lcd_line_scroll();
			}
			pos = 17;
			lcd_gotoxy(1, 2);			// second line.
			break;
		case '\b':
			if( pos == 17 ) lcd_gotoxy(16, 1);			// first line.
			else lcd_send_ctrl(0x10);
			lcd_send_char(' ');
			if( pos == 17 ) lcd_gotoxy(16, 1);			// first line.
			else lcd_send_ctrl(0x10);

			if( pos ) pos--;
			break;
		case ESC:
			break;
		default:
//            printf((char *)"pos = %d\n", pos);
			lcd_send_char(c);
			if( pos == 16 ) {
				lcd_gotoxy(1, 2);			// second line.
			} else if( pos == 32 ) {			// line scroll.
				lcd_line_scroll();
				lcd_gotoxy(1, 2);			// second line.
				pos = 16;
			}
			if( pos < 33) pos++;
			break;
	}
//	printf((char *)"pos = %d\n", pos);
}

void lcd_puts(char *s)
{
	while( *s ) lcd_putc(*s++);
}

uint8_t lcd_printf(char *szFormat, ...)
{
    uint8_t n;
    char szBuf[64] = {0, };
    va_list args;

    va_start(args, szFormat);
    n = vsprintf(szBuf, szFormat, args);

    lcd_puts(szBuf);
//    puts(szBuf);

    va_end(args);

    return n;
}

/* 1602-A1 LCD(LiquidCrystal Display) Function's 
 * lcd_init(void);
 * lcd_backlight(bool on_off);		// LCD 백라이트를 ON/OFF한다.
 * lcd_display(bool on_off);			// LCD 표시된 내용을 Display ON/OFF한다.
 * lcd_cursor(bool on_off)			// cursor display를 ON/OFF한다.
 * lcd_blink(bool on_off);			// cursor를 깜빡이는 기능을 ON/OFF한다.
 *
 * lcd_home(void);					// cursor의 좌표를 0,0으로 이동한다.
 * lcd_clear(void);				// LCD 화면의 내용을 모두 지움.
 *
 * lcd_setcursor(uint8_t x, uint8_t y);	// cursor의 좌표를 x, y로 이동한다.
 * lcd_putc(uint8_t ch);			// LCD 화면에 문자를 출력함.
 * lcd_puts(char *str);				// LCD 화면에 문자열을 출력함.
 *
 * lcd_scrollRight(void);			// LCD 내용을 우측으로 1칸 스크롤한다.
 * lcd_scrollLeft(void);			// LCD 내용을 좌측으로 1칸 스크롤한다.
 * lcd_autoscroll(bool on_off);		// 출력내용을 자동으로 우에서 좌로 스크롤 기능을 ON/OFF한다.
 */

// 특수 문자 : "코"
uint8_t SpecialChar1[8][8] = //{ 0x0, 0xFF, 0x1, 0xFF, 0x1, 0x5, 0x4, 0xFF };
{
	{0x1D, 0x05, 0x0B, 0x15, 0x01, 0x0E, 0x11, 0x0E},		// 0x40, "정"
	{0x00, 0x0A, 0x00, 0x04, 0x11, 0x0E, 0x00, 0x00},		// 0x48, 웃는 모습
	{0x0A, 0x15, 0x11, 0x11, 0x11, 0x0A, 0x04, 0x00},		// 0x50, 하트
	{0x04, 0x1F, 0x00, 0x0E, 0x11, 0x0E, 0x04, 0x1F},		// 0x58, "호"
	{0x00, 0x12, 0x12, 0x12, 0x1F, 0x02, 0x02, 0x00},		// 0x60, "나"
	{0x02, 0x1E, 0x12, 0x13, 0x12, 0x1E, 0x02, 0x00},		// 0x68, "다"
	{0x09, 0x17, 0x15, 0x0B, 0x01, 0x06, 0x09, 0x06},		// 0x70, "영"
	{0x0E, 0x1B, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F}		// 0x78, battery
};

#define _JUNG_			(0)
#define _HAPPY_		(1)
#define _HART_			(2)
#define _HO_			(3)
#define _NA_			(4)
#define _DA_			(5)
#define _YOUNG_		(6)
#define _BATTERY_		(7)
void lcd_drawing(uint8_t *font)
{
	uint8_t i, j, battery;

	lcd_send_ctrl(lcd_Clear);	// first line.

	// CGRAM special char write.
	lcd_send_ctrl(0x40);
	for(i=0; i<8; i++) {
		for(j=0; j<8; j++) {
			lcd_send_char(SpecialChar1[i][j]);
		}
	}

	// DDRAM address
	lcd_send_ctrl(0x80);	// first line : DDRAM address(0x0)
	lcd_send_char(_JUNG_);
	lcd_send_char(_NA_);
	lcd_send_char(_YOUNG_);
	lcd_send_char(' ');
	lcd_send_char(_JUNG_);
	lcd_send_char(_DA_);
	lcd_send_char(_YOUNG_);
	lcd_send_char(' ');
	lcd_send_char(_HART_);
	lcd_send_char(' ');
	lcd_send_char(_BATTERY_);

	lcd_send_ctrl(0xC0);	// first line : DDRAM address(0x0)
	lcd_send_char(0x0);
	lcd_send_char(' ');
	lcd_send_char(_BATTERY_);

	i = 0;
	battery = 0;
	do {
		lcd_send_ctrl(0x78);			// battery DGRAM address.
		lcd_send_char(SpecialChar1[7][0]);
		for(j=1; j<7; j++) {
			if( battery & (1<<(j-1)) ) {
				lcd_send_char(0x1F);
			} else {
				lcd_send_char(SpecialChar1[7][j]);
			}
		}
		lcd_send_char(SpecialChar1[7][7]);

		i++;
		if( i == 7 ) {
			i = 0;
			battery = 0;
		} else {
			battery |= 1<<(6-i);
		}

		{volatile uint16_t k; for(k=0; k<65000; k++);}
	} while( getkey() != ESC );

}

//#define lcd_blank(a)		lcd_display_ctrl(a, 0)
//#define lcd_cursor(a)		lcd_display_ctrl(a, 1)
//#define lcd_display(a)		lcd_display_ctrl(a, 2)
uint8_t lcd_display_ctrl(bool f, uint8_t pos)
{
	if( f ) display |= (f<<pos);
	else display &= ~(1<<pos);

	lcd_check_BF_4();                               // make sure LCD controller is ready
	lcd_send_ctrl((1<<3) | display);	// first line.

	return display;
}

void lcd_backlight(bool ctrl)
{
//	lcd_send_ctrl((1<<3) | (ctrl << 2));	// first line.
}

void lcd_instruction(uint8_t inst)
{
	lcd_check_BF_4();                               // make sure LCD controller is ready
	lcd_send_ctrl(inst);
	if( inst == lcd_Clear ) pos = 1;
//	pos = 1;
}

void lcd_setcursor(uint8_t x, uint8_t y)
{
	lcd_gotoxy(x, y);
}

#if 0
void lcd_display(bool ctrl)
{
	lcd_send_ctrl((1<<3) | (ctrl << 2));	// first line.
}
#endif

#if 1
const tROM_STRING mainMenu[] = {
//  [1234567890123456]
    "- Menu 1",
    "- Menu 2",
    "- Menu 3",
    "- Menu 4",
    "- Menu 5"
};
#else
char *mainMenu[3] = {
//   1234567890123456
    (char *)"1:Cursor blank on/off",
    (char *)"2:Cursor on/off",
    (char *)"3:Display on/off"
};
#endif
void menuControl(void * menu, uint8_t n)
{
    uint8_t ch;
    tROM_STRING *pt = (tROM_STRING *)menu;

    printf((char *)"mainMenu : addr = 0x%x\n", pt);
    for(uint8_t i=0; i<n; i++) {
        lcd_puts((char *)pt->str);
        if( i != (n-1) ) lcd_putc('\n');
        pt++;
    }

    // up : k, down : j, left : h, right : l
    do {
        ch = getchar();
        if( ch == 'k' ) {
        } else if( ch == 'j' ) {
        } else if( ch == 'h' ) {
        } else if( ch == 'l' ) {
        }
    } while( ch != ESC );
}

void Lcd_Test(void)
{
    uint8_t ch;
//    uint8_t ctrl;		// bit-0 : blank
						// bit-1 : cursor
						// bit-2 : display
						// bit-3 : backlight
    bool backlight, display, cursor, blank;

    backlight = display = cursor = blank = 0;
    lcd_init();

    puts((char *)" 1 : LCD cursor blank on/off");
    puts((char *)" 2 : LCD cursor on/off");
    puts((char *)" 3 : LCD display on/off");
    puts((char *)" 4 : LCD back-light on/off");
    puts((char *)" 5 : LCD cursor home");
    puts((char *)" 6 : LCD display clear");
    puts((char *)" 7 : LCD special char display");
    do {
//        ch = getchar();
        ch = get_fifo();
        if( ch != (uint8_t)-1 ) {
            if( isdigit(ch) ) {
                if( ch == '1' ) {
                    blank ^= 1;
                    printf((char *)"status = 0x%X\n", lcd_blank(blank));
                } else if( ch == '2' ) {
                    cursor ^= 1;
                    printf((char *)"status = 0x%X\n", lcd_cursor(cursor));
                } else if( ch == '3' ) {
                    display ^= 1;
                    printf((char *)"status = 0x%X\n", lcd_display(display));
                } else if( ch == '4' ) {
                    backlight ^= 1;
                    lcd_backlight(backlight);
                } else if( ch == '5' ) {
                    lcd_home();
                } else if( ch == '6' ) {
                    lcd_clear();
                } else if( ch == '7' ) {
                    lcd_drawing(NULL);
                } else if( ch == '8' ) {
                    uint8_t n = sizeof(mainMenu)/sizeof(char *);
                    menuControl((void *)mainMenu, n);
                }
            } else {
                putchar(ch);
                lcd_putc(ch);
            }
        }
#if 0
        if( ch == '1' ) {
            blank ^= 1;
            printf((char *)"status = 0x%X\n", lcd_blank(blank));
        } else if( ch == '2' ) {
            cursor ^= 1;
            printf((char *)"status = 0x%X\n", lcd_cursor(cursor));
        } else if( ch == '3' ) {
            display ^= 1;
            printf((char *)"status = 0x%X\n", lcd_display(display));
        } else if( ch == '4' ) {
            backlight ^= 1;
            lcd_backlight(backlight);
        } else if( ch == '5' ) {
            lcd_home();
        } else if( ch == '6' ) {
            lcd_clear();
        } else if( ch == '7' ) {
            lcd_drawing(NULL);
        } else if( ch == '8' ) {
            uint8_t n = sizeof(mainMenu)/sizeof(char *);
            menuControl((void *)mainMenu, n);
//            printf((char *)"mainMenu : addr = 0x%x, size = %d\n", mainMenu[0].str, sizeof(mainMenu)/2);
//            for(uint8_t i=0; i<n; i++) {
//                lcd_puts((char *)mainMenu[i].str);
//                if( i != (n-1) ) lcd_putc('\n');
//            }
        } else {
//            lcd_putc(ch);
        }
#endif
    } while( ch != ESC ) ;
}


