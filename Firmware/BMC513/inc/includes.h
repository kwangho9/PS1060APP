#ifndef __INCLUDES_H__
#define __INCLUDES_H__


#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <intrinsics.h>

#define NL  0x0A
#define CR  0x0D
#define BSP 0x08
#define ESC 0x1B

#ifdef __enable_interrupt
#undef __enable_interrupt
#endif
#define __enable_interrupt()		(rGIE = 0x80)

#ifdef __disable_interrupt
#undef __disable_interrupt
#endif
#define __disable_interrupt()		(rGIE = 0x00)

#ifdef __get_interrupt_state
#undef __get_interrupt_state
#endif
#define __get_interrupt_state()		(rGIE)

#ifdef __set_interrupt_state
#undef __set_interrupt_state
#endif
#define __set_interrupt_state(x)		(rGIE = x)

#define	__file__		(strrchr((const char *)__FILE__, '\\') + 1)
#define INFO(fmt, ...)	printf((char *)("%s-%s()[%d]: " fmt), __file__, __func__, __LINE__, ## __VA_ARGS__)
#define ERROR(fmt, ...)	printf((char *)("%s-%s()[%d]-ERROR: " fmt), __file__, __func__, __LINE__, ## __VA_ARGS__)




#include "BMC513_REGDEF.h"

typedef struct {
	uint8_t (*func)(uint8_t);
	char __code *str;
} tFuncList;

typedef struct {
	char __code *str;
} tMenuList;

typedef struct {
	char __code *cmd;
} tCommandList;

typedef struct _rom_const {
	char __code *str;
} tROM_STRING;

#include "main.h"
#include "System.h"
#include "SysInit.h"
#include "Multi-Switch.h"
#include "eSW_emulator.h"
#include "KeyMatrix.h"
#include "lcd.h"
#include "Test.h"

#endif /* __INCLUDES_H__ */
