//===================================================================
// File Name : System.h
// Function  : System
// Program   : B.H. Im & Kits   --  Boo-Ree Multimedia Inc.
// Date      : Oct, 25, 2005
// Version   : 0.0
// Mail      : support@boo-ree.com
// Web       : www.boo-ree.com
// History
//===================================================================

#ifndef __SYSTEM_h__
#define __SYSTEM_h__

#define delay_10us() _udelay(45)
#define delay_20us() _udelay(92)
#define delay_30us() _udelay(140)
#define delay_40us() _udelay(190)
#define delay_50us() _udelay(238)
#define delay_100us() udelay(740)
#define delay_500us() udelay(2640)

#ifdef __SYSTEM_c__
#define SYSTEM_EXT
#else
#define SYSTEM_EXT extern
#endif

SYSTEM_EXT long map(long x, long in_min, long in_max, long out_min, long out_max);
SYSTEM_EXT uint32_t millis(void);

SYSTEM_EXT __near_func void _udelay(uint8_t us);
SYSTEM_EXT __near_func void udelay(uint16_t us);
SYSTEM_EXT __near_func void mdelay(uint16_t ms);
SYSTEM_EXT __near_func void delay(uint8_t sec);

SYSTEM_EXT __near_func uint8_t get_available(void);
SYSTEM_EXT __near_func int getkey(void);
SYSTEM_EXT __near_func char *getstr(char *buf);
SYSTEM_EXT __near_func void putch(uint8_t uByte);
SYSTEM_EXT __near_func void putint(uint8_t val, uint8_t cr);
SYSTEM_EXT __near_func void puthex(uint8_t val, uint8_t cr);
SYSTEM_EXT __near_func void putstr(const char __code *ptr);
SYSTEM_EXT __near_func uint32_t atoh(char *str);
SYSTEM_EXT __near_func uint32_t getnum(void);

SYSTEM_EXT __near_func void dump(uint8_t *addr, uint16_t size);

#endif   //__SYSTEM_h__
