//===================================================================
// File Name : main.h
// Function  : main program header
// Program   : Boo-Ree Multimedia Inc.
// Date      : February, 04, 2013
// Version   : 0.01
// Mail      : support@boo-ree.com
// Web       : www.boo-ree.com
// History
//===================================================================

#ifndef __MAIN_h__
#define __MAIN_h__

/* variable type definitions */
typedef struct {
    union {
        uint16_t ctrl;
        struct {
            uint8_t ridx : 8;
            uint8_t widx : 8;
        };
    };
    uint8_t buf[64];
} fifo64_t;



#ifdef __MAIN_c__
#define MAIN_EXTERN
/** local definitions **/

/** internal functions **/

#else	/* __MAIN_c__ */
#define MAIN_EXTERN			extern
#endif	/* __MAIN_c__ */

/** global variable definitions **/
MAIN_EXTERN volatile uint8_t tick_1ms;
MAIN_EXTERN volatile uint8_t tick_10ms;
MAIN_EXTERN __idata fifo64_t rx_fifo;

/** external functions **/
MAIN_EXTERN uint8_t get_fifo(void);
MAIN_EXTERN __near_func void main(void);

#endif	/* __MAIN_h__ */

