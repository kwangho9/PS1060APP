//===================================================================
// File Name : SysInit.h
// Function  : System Init
// Program   : maeam  --  Boo-Ree Multimedia Inc.
// Date      : Aug, 20, 2007
// Version   : 0.0
// Mail      : support@boo-ree.com
// Web       : www.boo-ree.com
// History
//===================================================================

#ifndef __SYSINIT_h__
#define __SYSINIT_h__

#ifdef  __SYSINIT_c__
#define SYSINIT_EXT
#else
#define SYSINIT_EXT extern
#endif

#define FOSC 			24			// 24 Mhz

#define Hz	(1)
#define KHz	(1000)
#define MHz	(1000000)

#define EMCLK	(12*MHz)
#define ESCLK	(32768*Hz)
#define IMCLK	(16*MHz)
#define ISCLK	(32768*Hz)

/* PLL Clock : 48Mhz, when EMCLK is 12Mhz
 * Fosc  : 24MHz
 */
#define MDIV	(40)
#define PDIV	(10)
#define SDIV	(0)

#define Fsys			(24*1000000)

//
// PCON  : 0
// UxCON0: SCSEL = 0
// UxCON1: ECLKSEL = 0, XDRATE = 0

#if (Fsys == 24*MHz)
#define BAUD_115200		12
#define BAUD_57600		25
#define BAUD_38400		38
#define BAUD_19200		77
#define BAUD_9600		155
#elif (Fsys == 16*MHz)
#define BAUD_115200		7
#define BAUD_57600		16
#define BAUD_38400		25
#define BAUD_19200		51
#define BAUD_9600		103
#endif


SYSINIT_EXT __near_func void init_clk(void);
SYSINIT_EXT __near_func void init_wdt(void);
SYSINIT_EXT __near_func void init_gpio(void);
SYSINIT_EXT __near_func void init_uart(uint8_t buad);
SYSINIT_EXT __near_func void init_timer(uint8_t channel, uint8_t mode, uint8_t period, uint8_t duty);

SYSINIT_EXT __near_func void init_system(void);

#endif  //__SYSINIT_h__
