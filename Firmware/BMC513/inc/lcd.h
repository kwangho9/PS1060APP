//===================================================================
// File Name : lcd.h
// Function  : lcd header file
// Program   : Boo-Ree Multimedia Inc.
// Date      : February, 04, 2013
// Version   : 0.01
// Mail      : support@boo-ree.com
// Web       : www.boo-ree.com
// History
//===================================================================


#ifndef __LCD_h__
#define __LCD_h__

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

/* variable type definitions */

#ifdef __LCD_c__
#define LCD_EXTERN
/** local definitions **/

/** internal functions **/

#else	/* __LCD_c__ */
#define LCD_EXTERN			extern
#endif	/* __LCD_c__ */

#define lcd_blank(a)		lcd_display_ctrl(a, 0)
#define lcd_cursor(a)		lcd_display_ctrl(a, 1)
#define lcd_display(a)		lcd_display_ctrl(a, 2)

#define lcd_home()			lcd_instruction(lcd_Home)
#define lcd_clear()			lcd_instruction(lcd_Clear)

/** global variable define **/
LCD_EXTERN bool lcd_flag;

/** external functions **/
LCD_EXTERN uint8_t lcd_display_ctrl(bool f, uint8_t pos);
LCD_EXTERN void lcd_instruction(uint8_t inst);
LCD_EXTERN void lcd_backlight(bool onoff);
LCD_EXTERN void lcd_drawing(uint8_t *font);

LCD_EXTERN void lcd_init(void);
LCD_EXTERN void lcd_setcursor(uint8_t x, uint8_t y);
LCD_EXTERN void lcd_putc(char);
LCD_EXTERN void lcd_puts(char *);
LCD_EXTERN uint8_t lcd_printf(char *, ...);

LCD_EXTERN __near_func void lcd_main(void);
LCD_EXTERN __near_func void Lcd_Test(void);

#endif	/* __LCD_h__ */

