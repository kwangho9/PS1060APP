
#define __SYSTEM_c__
#include "includes.h"


uint32_t millis(void)
{
    uint32_t ms;

    __disable_interrupt();
    ms = millisec.l;
    __enable_interrupt();

    return ms;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


// @ Fsys : 24Mhz
// 1us -> 2(min)
// 10us -> 45
// 20us -> 92
// 30us -> 140
// 40us -> 190
// 50us -> 238
// 54us -> 0(max)
__near_func void _udelay(uint8_t us)
{
	asm("DJNZ		R1, $");
}

// @ Fsys : 24Mhz
// 256 -> 54.128 us
// 740 -> 99.817 us
// 1000 -> 157.064 us
// 1200 -> 198.899 us
// 1500 -> 261.284 us
// 2500 -> 471.560 us
// 2600 -> 491.000 us
// 2650 -> 502.752 us
// 5000 -> 998.165 us
// 5050 -> 1.009 ms
// 9800 -> 2.004 ms
// 9900 -> 2.026 ms
// 10000 -> 2.048 ms
// 45000 -> 9.394 ms
// 47500 -> 9.945 ms
// 47800 -> 9.982 ms
// 48000 -> 10.018 ms
// 50000 -> 10.459 ms
// 0 -> 13.761 ms
__near_func void udelay(uint16_t us)
{
	asm("DJNZ		R2, $");
	asm("DJNZ		R3, $-2");
}

__near_func void mdelay(uint16_t ms)
{
	while( ms-- ) udelay(5000);			// 1 ms
}

__near_func void delay(uint8_t sec)
{
	while( sec-- ) mdelay(980);		// 1 sec
}

__near_func uint8_t get_available(void)
{
	if( rUTRSTAT & (1<<1) ) return 1;

	return 0;
}

__near_func int getkey(void)
{
	int c = -1;

	if( rUERSTAT & ((1<<0)|(1<<1)|(1<<2)|(1<<3)) ) {		// Error check.
		rUINTCON = (rUINTCON & 0x1F) | (1<<7);
	}

	if( rUTRSTAT & (1<<1) ) {
		c = rURXBUF;
	}

	return c;
}

#if (__CODE_MODEL__ == 2)
__near_func
#endif
int getchar(void)
{
	int ch = 0;

	do {
		ch = getkey();
	} while( ch == -1 );

	return ch;
}

__near_func char *getstr(char *buf)
{
	char *str = buf;
	char c;

	do {
		c = getchar();
		if( c == '\b' ) {
			if( str > buf ) {
				str--;
				putchar('\b');
				putchar(' ');
				putchar('\b');
			}
		} else if( (c == '\n') || (c == '\r') ) {
			putchar('\n');
			break;
		} else {//if( isalnum(c) || (c == ' ') || (c == '\t') ) {
			putchar(c);
			*str++ = c;//tolower(c);
		}
	} while(1);

	if( (c == (char)EOF) && (str == buf) ) return 0;

	*str = '\0';

	return str;
}


/*******************************************************************************
* Function Name  : putch
* Description    : Transmits signle data through the uart peripheral.
* Input          : uByte - the data to transmit.
* Output         : None
* Return         : None
*******************************************************************************/
__near_func void putch(uint8_t uByte)
{
	if(uByte == '\n') {
		while(!(rUTRSTAT & (1<<4)));
		rUTXBUF = '\r';
	}
	while(!(rUTRSTAT & (1<<4)));
	rUTXBUF= uByte;
}




#if (__CODE_MODEL__ == 2)
__near_func
#endif
int putchar(int ch)
{
	putch(ch);

	return ch;
}


__near_func uint32_t atoh(char *str)
{
	unsigned long value = 0;
	unsigned char c, digit;

	if( strncmp(str, (char *)"0x", 2) == 0 ) {
		str++; str++;
		while( (c=*str++) ) {
			if( (c >= '0') && (c <= '9') ) digit = c - '0';
			else if( (c >= 'a') && (c <= 'f') ) digit = c - 'a' + 10;
			else if( (c >= 'A') && (c <= 'F') ) digit = c - 'A' + 10;
			else break;
			value = (value << 4) + digit;
		}
	} else {
		value = atoi(str);
	}

	return value;
}

__near_func uint32_t getnum(void)
{
	uint8_t c;
	uint8_t tmp[16];
	uint8_t *ptr = tmp;

	do {
		c = getchar();

		if( c == '\b' ) {
			if( ptr > tmp ) {
				ptr--;
				putchar('\b');
				putchar(' ');
				putchar('\b');
			}
		} else if( (c == '\n') || (c == '\r') ) {
			putchar('\n');
			break;
		} else if( isxdigit(c) || (c == ' ') || (c == '\t') || (c == 'x') ) {
			putchar(c);
			*ptr++ = tolower(c);
		} else if( c == ESC ) {
			return 0;
		}
	} while( 1 );

	if( (c == (uint8_t)EOF) && (ptr == tmp) ) return 0;

	*ptr = '\0';

	return atoh((char *)tmp);
}


// max : 255
void putint(uint8_t val, uint8_t cr)
{
	uint8_t t, z = 0;

	t = (val / 100) % 10;
	if( t ) {
		putchar(t+'0');
		z = 1;
	}
	t = (val / 10) % 10;
	if( t || z ) putchar(t+'0');
	t = (val / 1) % 10;
	putchar(t+'0');

	if( cr ) putchar(cr);
}

void puthex(uint8_t val, uint8_t cr)
{
	uint8_t t, d;

	for(d=0; d<2; d++) {
		if( d == 0 ) {
			t = (val >> 4) & 0xF;
		} else {
			t = val & 0xF;
		}

		if( t < 10 ) t += '0';
		else t += ('A' - 10);
		putchar(t);
	}
	if( cr ) putchar(cr);
}


__near_func void putstr(const char __code *ptr)
{
	while(*ptr) putchar(*ptr++);
}

/* utility function */
__near_func void dump(uint8_t *addr, uint16_t size)
{
	uint16_t i, length;
	uint8_t j;

	if( size == 0 ) length = 512;
	else length = size;

	do {
		for(i=0; i<length; i+=16) {
			printf((char *)"0x%04x : ", addr+i);
			for(j=0; j<16; j++) {
				printf((char *)"%02x ", addr[i+j]);
			}
			printf((char *)"      ");
			for(j=0; j<16; j++) {
				if( iscntrl(addr[i+j]) ) putchar('.');
//				if((ch<0x20) || (ch > 0x7E))
				else putchar(addr[i+j]);
			}
			putchar('\n');
		}
		if( size != 0 ) break;
		putchar('\n');
		addr += 512;
	} while( getchar() != ESC );
}


/*	EOF	*/
