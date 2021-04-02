/**
 * @file main.c
 * @brief main function.
 * @author khjung ( khjung@leolsi.com )
 * @version 0.1
 * @date 2019-08-02
 * @see 참고할 내용들은 여기에 기술한다.
 * @todo 추가적으로 해야 할 사항들.
 * @deprecated 향후 삭제 예정인 내용들을 표시함.
 */

//===================================================================
//	Instruction to RUN !!!!!!
//		Build -> Build
//		Debug->Start Debugging
//		RUN
//===================================================================
#define __MAIN_c__
#include "includes.h"


/*
	***************************************
	* Memory map of BRmDPF
	***************************************

	0xFFFF +------------------+
	       | Fast debug code  |
	0xFC00 +------------------+
	       | Reserved         |
	0xFA00 +------------------+
	       | Environment data |
	0xF800 +------------------+
	       |                  |
	       |                  |
	       |                  |
	       | Application code |
	       |                  |
	       |                  |
	       |                  |
	0x1000 +------------------+
	       | Boot loader      |
	0x0100 +------------------+
	       | Interrupt vector |
	0x0000 +------------------+

	***************************************
*/
__near_func uint8_t check_tick(uint8_t task_tick)
{
	uint8_t ret;

	GlobalIntDisable;
	if( tick_10ms < task_tick ) ret = ((255 - task_tick) + tick_10ms);
	else ret = (tick_10ms - task_tick);
	GlobalIntEnable;

	return ret;
}

/*************************************************************************
 */

__idata fifo64_t rx_fifo;

void init_fifo(void)
{
//  rx_fifo.widx = rx_fifo.ridx = 0;
    rx_fifo.ctrl = 0;
}

uint8_t available_fifo(void)
{
    return (rx_fifo.widx - rx_fifo.ridx);
}

uint8_t put_fifo(uint8_t ch)
{
    if( ((rx_fifo.widx + 1) & 0x3F) == (rx_fifo.ridx & 0x3F) ) {
        lcd_puts((char *)"F");
        return 0;
    }

    rx_fifo.buf[rx_fifo.widx & 0x3F] = ch;
    rx_fifo.widx++;

    return 1;
}

uint8_t get_fifo(void)
{
    uint8_t ch;
    if( (rx_fifo.ridx & 0x3F) == (rx_fifo.widx & 0x3F) ) {
//        lcd_puts((char *)"E");
        return -1;
    }

    ch = rx_fifo.buf[rx_fifo.ridx & 0x3F];
    rx_fifo.ridx++;
    return ch;
}


uint8_t gGroup;
uint8_t buffer[128];

__near_func void main(void)
{
    uint8_t idx = 0;
    uint8_t ch;
//    uint8_t mask;
//	uint8_t tx_tick = 0;

 	init_system();

	__enable_interrupt();		// global interrupt enable.

	puts((char *)"\n\n");
	printf((char *)"System Information : Fsys = %s\n", ((rCLKCON & 0x7)==0x5)?"PLL":"EMCLK");
//	putstr("\n\n\nSystem Information : Fsys = ");
//	putstr(((rCLKCON & 0x7)==0x5)?"PLL":"EMCLK");
//	putchar('\n');
	printf((char *)"Multi-Switch(PS1030/1060) Project.\n");


    init_fifo();
    gDevice = 3;
    gGroup = 10;




    // 0. Logic Initialize
    printf((char *)"PS1xxx Logic Initialize.\n");
    for(uint16_t k=0; k<2000; k++) {
        SendByte(0x0);
    }
    device_detect();            // Multi-Switch.c
    /*
    // 1. Pattern Reset
    printf((char *)"\tPattern Reset.\n");
//    mSW_PATTERN_RESET();    // PS1030�� PS1060�� ���� �ٸ���.
    for(uint8_t k=0; k<18; k++) {   // PS1120 Pattern Reset�� ������.
        SendByte(0xFF);
    }
    SendByte(0x00);         // command cycle
    SendByte(0x00);         // op cycle
    SendByte(0x00);         // op cycle
    SendByte(0x00);         // shift cycle : dummy�� 4�� �־��� ����������.

    // 2. Chip ID Assignment -> ������ Device�� ������ �˼� �ִ�.
    printf((char *)"\tCHIP Address Assignment.\n");
    gMaxChip = 0;
    mask = mSW_CHIP_ADDRESS_ASSIGN(0xFF);  // '0' id assignment.
    // RxBuffer[3]�� bit[6]����
    for(uint8_t k=1; k<9; k++) {
        uint8_t tmp = (*(uint16_t *)&RxBuffer[0] >> k) & 0xFF;
        printf((char *)"tmp = 0x%X\n", tmp);

        if( 0xAA == tmp ) {
            gMaxChip = k;
            break;
        }
    }
    mask = (RxBuffer[2] << gMaxChip) | (RxBuffer[3] >> (8 - gMaxChip));
    printf((char *)"\tReturn ID Mask(0x%x)\n", mask);
    gChipSelect = 0;
    for(uint8_t id=0; id<8; id++) {
        if( (mask & (1 << (7 - id))) == 0 ) {
            // ID���� Device�� ����(60/30)�� check�Ѵ�.
            // (Device�� ��� ������ �� ����.)
            ch = mSW_SFR_READ(id, CHIP_CTRL);
            printf((char *)"\t%d. CHIP_CTRL(0x%X) : ", id, ch);
            if( ch & 0x80 ) {
                gDevice = 2;        // PS1120
                gTotalSwitch = 120;
                gMaxBytes = 15;         // max switch inform
                // sync + cmd + cs + switch + op + shift = 1 + 1 + 1 + 15 + 1 + 1 = 20
                gMaxFrame = 20;
                if( ch & 0x40 ) printf((char *)"Upper 120");
                else printf((char *)"Lower 120");
            } else {
                if( ch & 0x40 ) {
                    gDevice = 1;        // PS1060
                    gTotalSwitch = 60;
                    gMaxBytes = 8;
                    // sync + cmd + cs + switch + op + shift = 1+1+1+8+1+1 = 13
                    gMaxFrame = 13;
                    printf((char *)"60");
                } else {
                    gDevice = 0;        // PS1030
                    gTotalSwitch = 30;
                    gMaxBytes = 4;
                    // sync + cmd + cs + switch + op + shift = 1+1+1+4+1+1 = 9
                    gMaxFrame = 9;
                    printf((char *)"30");
                }
            }
            printf((char *)"-channel, ");
            printf((char *)"%d-group\n", c_GroupType[ch & 0x7]);
            gGroupType = c_GroupType[ch & 0x7];
            gChipSelect |= (1 << (7-id));
        }
    }
    */

    // Initialize Global Variable.
    printf((char *)"\t- gMaxChip = %d\n", gMaxChip);
    printf((char *)"\t- gDevice = %d\n", gDevice);
    printf((char *)"\t- gTotalSwitch = %d\n", gTotalSwitch);
    printf((char *)"\t- gMaxBytes = %d\n", gMaxBytes);
    printf((char *)"\t- gMaxFrame = %d\n", gMaxFrame);
    printf((char *)"\t- gGroupType = %d\n", gGroupType);
    printf((char *)"\t- gChipSelect = 0x%02X\n", gChipSelect);

    chip_id_assignment_test();

    mSW_PATTERN_RESET();
    mSW_CHIP_ADDRESS_ASSIGN(0xFF);

    if( gDevice == 2 ) {
        // PS1120 Stack Inform read.
        ch = mSW_SFR_READ(0, STACK_CTRL);
        printf((char *)"STACK_CTRL(0x%X) : ", ch);
        printf((char *)"%s, ", (ch & 0x2) ? "Stack" : "No Stack");
        printf((char *)"%s chip\n", (ch & 0x1) ? "Upper" : "Lower");
    }

    active_discharge_read_test();


    group_type_read_test();

    mSW_SET_GROUP_TYPE(3);      // 10-group.

    switch_status_read_test();

//    mSW_ALL_ON();
#if 0
    KeyMatrix_Test();
#else
    keypad_init();
#endif

#if 0
    Lcd_Test();
#else
    lcd_init();
#endif

//	mSwitch_Test();

    lcd_puts((char *)"START!");
//    mSW_PATTERN_RESET();
//    Debug.gTransmit = 1;
//    Debug.gReceive = 1;
    idx = 0;
    do {
        KeyService();

//        ch = getkey();
        ch = get_fifo();
        if( ch != (char)-1 ) {
#if 0   // console off.
            putchar(ch);
#endif
            if( (ch == '\n') || (ch == '\r') ) {
                char *token;
                uint8_t n = 0;
                char *split = (char *)" :,.-";

                token = strtok((char *)buffer, split);
                if( strncmp(token, (char *)"getDevice", 9) == 0 ) {
                    printf((char *)"device:%s:group:%d:\n", deviceName[gDevice].str, gGroup);
                } else if( strncmp(token, (char *)"setGroup", 8) == 0 ) {
                    uint8_t g;
                    token = strtok(NULL, split);
                    g = strtol(token, 0, 10);
                    printf((char *)"group = %d\n", g);
//                    lcd_puts((char *)"Get Group");
                    gFrame[0] = 0xAA;
                    gFrame[1] = 0x31;
                    for(uint8_t i=0; i<6; i++) {
                        if( c_GroupType[i] == g ) {
                            gFrame[2] = i << 5;
                            break;
                        }
                    }
                    gFrame[3] = 0x00;
                    gFrame[4] = 0x00;
                    send_command(gFrame, 5);
                } else {
                    while( token != NULL ) {
                        gFrame[n++] = strtol(token, 0, 16);
                        token = strtok(NULL, split);
                    }
                    send_command(gFrame, n);
                }
                for(uint8_t k=0; k<128; k++) buffer[k] = 0;
                idx = 0;
            } else {
                buffer[idx++] = ch;
            }
        }
    } while( 1 );
    /*
	do {
		mdelay(500);
		printf((char *)"%s()[%d]:\n", __func__, __LINE__);
		rP7_b3 ^= 1;
#if 0
		if( check_tick(tx_tick) >= 50 ) {			// 500 ms
			tx_tick = tick_10ms;

//			printf((char *)"%s()[%d]:\n", __func__, __LINE__);
			rP7_b3 ^= 1;
		}
#endif
	} while(getkey() != ESC);
    */
}

//============================================================================
//		Secondary Interrupt Vectors Begin
//============================================================================
// 	Notice : -------------------------------------------------
// 	Interrupt Vector can be installed by "#pragma vector=0x##"
// 	But there is one Limitation that Interrupt Vector
//	SHOULD be used with main() within Same File.
//------------------------------------------------------------

#pragma vector=0x03
#pragma location="ISR_SEG"
__interrupt void _WdtInt(void)
{
	rWDTCON |= WINTPEND;
}


#pragma vector=0x0b
#pragma location="ISR_SEG"
__interrupt void _BracDmaInt(void)
{
}


#pragma vector=0x13
#pragma location="ISR_SEG"
__interrupt void _AdcInt(void)
{
}


#pragma vector=0x1b
#pragma location="ISR_SEG"
__interrupt void _ExInt0(void)
{
}


#pragma vector=0x23
#pragma location="ISR_SEG"
__interrupt void _ExInt1(void)
{
}



#pragma vector=0x2b
#pragma location="ISR_SEG"
__interrupt void _SpiInt(void)
{
}


#pragma vector=0x33
#pragma location="ISR_SEG"
__interrupt void _Tmr0MatInt(void)
{
	rT0CON |= TINTPEND;
	tick_1ms++;
}


#pragma vector=0x3b
#pragma location="ISR_SEG"
__interrupt void _Tmr1MatInt(void)
{
	rT1CON |= TINTPEND;
//	tick_10ms++;
    millisec.b[0]++;
    if( millisec.b[0] == 0 ) {
        millisec.b[1]++;
        if( millisec.b[1] == 0 ) {
            millisec.b[2]++;
            if( millisec.b[2] == 0 ) {
                millisec.b[3]++;
            }
        }
    }
}


#pragma vector=0x43
#pragma location="ISR_SEG"
__interrupt void _Tmr2MatInt(void)
{
}


#pragma vector=0x4b
#pragma location="ISR_SEG"
__interrupt void _BrJpegInt(void)
{
}


#pragma vector=0x53
#pragma location="ISR_SEG"
__interrupt void _UartTxInt(void)
{
	rUINTCON = (rUINTCON & 0x1F) | (1<<5);
}


#pragma vector=0x5b
#pragma location="ISR_SEG"
__interrupt void _UartRxInt(void)
{
	rUINTCON = (rUINTCON & 0x1F) | (1<<6);
    put_fifo(rURXBUF);
}


#pragma vector=0x63
#pragma location="ISR_SEG"
__interrupt void _Uart0ErrInt(void)
{
	rUINTCON = (rUINTCON & 0x1F) | (1<<7);
}


#pragma vector=0x6b
#pragma location="ISR_SEG"
__interrupt void _VdmaInt(void)
{
}


#pragma vector=0x73
#pragma location="ISR_SEG"
__interrupt void _I80LcdInt(void)
{
}


#pragma vector=0x83
#pragma location="ISR_SEG"
__interrupt void _ExInt4_5(void)
{
}


#pragma vector=0x7b
#pragma location="ISR_SEG"
__interrupt void _ExInt2_3(void)
{
}


#pragma vector=0x8B
#pragma location="ISR_SEG"
__interrupt void _ExInt6(void)
{
}


#pragma vector=0x93
#pragma location="ISR_SEG"
__interrupt void _ExInt7(void)
{
}


#pragma vector=0x9b
#pragma location="ISR_SEG"
__interrupt void _ExInt8(void)
{
}


#pragma vector=0xa3
#pragma location="ISR_SEG"
__interrupt void _ExInt9(void)
{
}


#pragma vector=0xab
#pragma location="ISR_SEG"
__interrupt void _RtcPriInt(void)
{
}


#pragma vector=0xb3
#pragma location="ISR_SEG"
__interrupt void _RtcAlmInt(void)
{
}


#pragma vector=0xbb
#pragma location="ISR_SEG"
__interrupt void _USBRSTInt()
{
}


#pragma vector=0xc3
#pragma location="ISR_SEG"
__interrupt void _USBSOFInt()
{
}

#pragma vector=0xcb
#pragma location="ISR_SEG"
__interrupt void _USBEP0Int()
{
}

#pragma vector=0xd3
#pragma location="ISR_SEG"
__interrupt void _USBEP1Int()
{
}

#pragma vector=0xdb
#pragma location="ISR_SEG"
__interrupt void _USBEP2Int()
{
}

#if EPIN_INTERRUT_ENABLE
#pragma vector=0xe3
#pragma location="ISR_SEG"
__interrupt void _USBEP3Int()
{
}
#endif

#pragma vector=0xeb
#pragma location="ISR_SEG"
__interrupt void _USBEP4Int()
{
}

//============================================================================
//		Secondary Interrupt Vectors End
//============================================================================
