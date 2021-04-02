//===================================================================
// File Name : SysInit.c
// Function  : System Init
// Program   : maeam  --  Boo-Ree Multimedia Inc.
// Date      : Aug, 20, 2007
// Version   : 1.0
// Mail      : support@boo-ree.com
// Web       : www.boo-ree.com
// History
//===================================================================

#define __SYSINIT_c__
#include "includes.h"

__near_func void init_clk(void)
{
	WDT_DISABLE;

	rSYSCFG = 0;

	/* System clock initialize.
	 */
 	while(!(rSYSCFG & MAIN_OSC_STABLE)); 	//Wait for PXI Stable

	/* PLLCLK(Fpll) = EMCLK * (MDIV + 8) / ((PDIV + 2) * (2^SDIV))
	 *              = 12Mhz * (0x28 + 8) / ((0xA + 2) * (2^0))
	 *              = 12Mhz * (48) / 12 = 48Mhz
	 */
 	rPLLCON0 = PLL_EN_EN | PDIV;
 	rPLLCON1 = (MDIV << 2) | SDIV;
 	while(!(rPLLCON0 & PLL_STABLE));		// Wait for PLL Stable

	rPCON = PCON_EF_WAIT_MODE;			// eFlash wait enable.
	rSMCLKCON = SMCLK_EN_VDMA | SMCLK_EN_I80LCD | SMCLK_EN_JPEG | SMCLK_EN_BRAC
		   | FLASH_WAIT_CNT_2 | ONE_CACHE_EN;
	// Using PLL clock @ 24MHz, Fsource = Fsys = 24Mhz
	rCLKCON = CLKO_EMCLK | CLKDIV_1 | CLKSRC_PLLCLK;

	// Enable all system clock
	CLK_EN0_ALL_EN;
	CLK_EN1_ALL_EN;

	/* Interrrupt initialize.
	 */
	rIE0 = 0x00;
	rIE1 = 0x00;
	rIE2 = 0x00;

	rP1MOD1 = GP14_CLKOUT;

//    rP1MOD0 = 0x55;
//    rP1_b0 = 1;
    {volatile uint16_t k=0; for(k=0; k<65000; k++);}    // 100ms waiting.
//    rP1_b0 = 0;
    if( rP0_b0 == 0 ) {     // enter idle mode
    	rP0MOD0 = 0x00;				// GP00, GP01 : Input
    	rP0MOD1 = 0x00;		// GP05_KEY_UP, GP04_KEY_DOWN
    	rP1MOD0 = 0x00;				// GP1[0:3] : Input
    	rP1MOD1 = 0x00;				// GP1[4:6] : EINT5
    	rP3MOD0 = 0x00;				// GP3[0:3] : Input
    	rP3MOD1 = 0x00;
    	rP4MOD0 = 0x00;				// GP4[0:3] : Input
    	rP4MOD1 = 0x00;				// GP4[4:7] : Input
    	rP5MOD0 = 0x00;				// GP5[0:3] : Input
    	rP5MOD1 = 0x00;				// GP5[4:7] : Input
    	rP6MOD0 = 0x00;				// GP6[0:3] : Input
    	rP6MOD1 = 0x00;				// GP6[4:7] : Input
    	rP7MOD0 = 0x00;				// GP7[0:3] : Input
    	rP7MOD1 = 0x00;				// GP7[4:7] : Input
    	rP8MOD0 = 0x00;				// GP8[0:3] : Input
    	rP0PU = 0x00;					// GP0[0:7] Pull-up Disable
    	rP1PUD0 = 0x00;				// GP1[0:3] Pull-up/down Disable
    	rP1PUD1 = 0x00;				// GP1[4:6] Pull-up/down Disable
    	rP3PUD0 = 0x00;				// GP3[0:3] Pull-up/down Disable
    	rP3PUD1 = 0x00;				// GP3[4:7] Pull-up/down Disable
    	rP4PU = 0x00;					// GP4[0:7] Pull-up Disable
    	rP5PU = 0x00;					// GP5[0:7] Pull-up Disable
    	rP6PU = 0x00;					// GP6[0:6] Pull-up Disable
    	rP7PU = 0x00;					// GP7[0:7] Pull-up Disable
    	rP8PUD0 = 0x00;				// GP8[0:3] Pull-up/down Disable
        rPCON |= 0x1;
        while(1);
    }
}

__near_func void init_wdt(void)
{
	WDT_DISABLE;	// WDT disable, Default settig of WDT is enable.
}

__near_func void init_gpio(void)
{
	// GPIO Setting
	// bit 00 : Output mode , bit 11 : Input mode
	// Note : To prevent IO Leakage current, Set unused port to Output
	// GPIO Setting
	// Bit 11 : Output mode , Bit 00 : Input mode
	// Note : To prevent I/O Leakage current, Set unused port to Output
	rP0 = 0xff;
	rP1 = 0xff;
	rP3 = 0xff;
	rP4 = 0xff;
	rP5 = 0xff;
	rP6 = 0xff;
	rP7 = 0xff;
	rP8 = 0xff;

	rP0MOD0 = 0x00;				// GP00, GP01 : Input
	rP0MOD1 = GP04_DI | GP05_DI;		// GP05_KEY_UP, GP04_KEY_DOWN
	//check	rP0MOD3 = 0x00;				// GP06, GP07 : Input

	rP1MOD0 = 0x80;				// GP1[0:3] : Input
	rP1MOD1 = 0x2A;				// GP1[4:6] : EINT5

	rP3MOD0 = 0xAA;				// GP3[0:3] : Input
	rP3MOD1 = 0xAA;

	rP4MOD0 = 0x00;				// GP4[0:3] : Input
	rP4MOD1 = 0x00;				// GP4[4:7] : Input

	rP5MOD0 = 0xAA;				// GP5[0:3] : Input
	rP5MOD1 = 0xAA;				// GP5[4:7] : Input

	rP6MOD0 = 0xA6;				// GP6[0:3] : Input
	rP6MOD1 = 0xAA;				// GP6[4:7] : Input

	rP7MOD0 = 0xAA;				// GP7[0:3] : Input
	rP7MOD1 = 0xAF;				// GP7[4:7] : Input

	rP8MOD0 = 0xAA;				// GP8[0:3] : Input
	
//	rP0PUR = 0x00;					// GP0[0:7] Pull-up Disable
	rP1PUD0 = 0x00;				// GP1[0:3] Pull-up/down Disable
	rP1PUD1 = 0x00;				// GP1[4:6] Pull-up/down Disable
	rP3PUD0 = 0x00;				// GP3[0:3] Pull-up/down Disable
	rP3PUD1 = 0x00;				// GP3[4:7] Pull-up/down Disable
//	rP4PUR = 0x00;					// GP4[0:7] Pull-up Disable
//	rP5PUR = 0x00;					// GP5[0:7] Pull-up Disable
//	rP6PUR = 0x00;					// GP6[0:6] Pull-up Disable
//	rP7PUR = 0x00;					// GP7[0:7] Pull-up Disable
	rP8PUD0 = 0x00;				// GP8[0:3] Pull-up/down Disable

	rP7MOD0 = (rP7MOD0 & ~0xC0) | GP73_DO;
	rP7_b3 = 1;				// LED Off

    rP0MOD1 = 0x04;
    rP0_b5 = 1;
}

/*
 * Initialize SPI port
 */
__near_func void init_spi(void)
{
#if (PMIC_INTERFACE == HW_SPI)	// used SPI
//	rP3MOD1 = GP34_BSCLK|GP35_BMISO|GP36_BMOSI|GP37_BSCSN;

	// SPI Controller Initialize.
//	rP5MOD1 = GP54_SPI_CLK|GP55_SPI_MISO|GP56_SPI_MOSI|GP57_SPI_CSN;
	rP5MOD1 = GP54_SPI_CLK|GP55_SPI_MISO|GP56_SPI_MOSI|GP57_DO;
	rP5_b7 = 1;	// nSCS High

	rCLK_EN0 |= SPI_CKEN;
	/* 
	 * Please enable SPI_EN bit first, before setting SPI_SCKPHA_FIRST, SPI_SCKPOL_HI bit.
	 * Or set in the same time.
	 */
//	rSPIMOD = SPI_SCKPHA_FIRST|SPI_SCKPOL_HI|SPI_MATEN|SPI_DIR_MSB|SPI_MODE_RXTX|SPI_EN;
	rSPIMOD = SPI_MATEN|SPI_DIR_MSB|SPI_MODE_RXTX|SPI_EN;
//	rSPICK  = SPI_1BYTE|SPI_CKSEL_1100;	// 12 Mhz / 4 = 3 Mbps
//	rSPICK  = SPI_1BYTE|SPI_CKSEL_1011;	// 12 Mhz / 5 = 2.4 Mbps
//	rSPICK  = SPI_1BYTE|SPI_CKSEL_1010;	// 12 Mhz / 10 = 1.2 Mbps -> OK
	rSPICK  = SPI_1BYTE|SPI_CKSEL_1001;	// 12 Mhz / 12 = 1 Mbps -> OK
//	rSPICK  = SPI_1BYTE|SPI_CKSEL_1000;	// 12 Mhz / 25 = 480 Kbps -> OK
//	rSPICK  = SPI_1BYTE|SPI_CKSEL_0111;	// 12 Mhz / 50 = 240 Kbps -> OK
//	rSPICK  = SPI_1BYTE|SPI_CKSEL_0110;	// 12 Mhz / 125 = 96 Kbps -> OK
//	rSPICK  = SPI_1BYTE|SPI_CKSEL_0000;	// 12 Mhz / 4095 = 2.93 Kbps -> OK

#elif (PMIC_INTERFACE == SW_GPIO)	// used GPIO
	rP5MOD1 = GP54_DO|GP55_DI|GP56_DO|GP57_DO;
	rP5_b7 = 1;	// nSCS High
#endif
}

#if 0
/*******************************************************************************
* Function Name  : bmc513_baudrate
* Description    : calculrate uart baudrate value.
* Input          : - clk : 
			CLKSEL_DIV0	0x0
			CLKSEL_EXTUCLK	0x1
			CLKSEL_DIV2	0x2
			CLKSEL_DIV4	0x3
			CLKSEL_DIV8	0x4
			CLKSEL_DIV16	0x5
*                  - baud: 9600, 19200, 38400, 57600, 115200
* Output         : None
* Return         : uart baudrate value
*******************************************************************************/ 
__near_func int bmc513_uart_baudrate(int clk, long baud)
{	
	int 	x;
	long	fsys;
	
	switch (clk) {
		case 0:	fsys = 24000000;
			break;
		case 2: fsys = 24000000/2;
			break;
		case 3: fsys = 24000000/4;
			break;
		case 4:	fsys = 24000000/8;
			break;
		case 5: fsys = 24000000/16;
			break;
		case 6: fsys = 16*MHz;
			break;
		case 1: break; // EXTUCLK
		default:break; // reserved
	}
	
	/*
	 * baudrate = (select CLK)/((CNT0+1)*16)
	 */
	x = fsys/(baud * 16);
	return (x-1);
}
#endif

/*******************************************************************************
* Function Name  : init_uart
* Description    : Initializes the uart peripheral.
* Input          : None.
* Output         : None
* Return         : None
*******************************************************************************/
__near_func void init_uart(uint8_t baud)
{
	rP7MOD1 = (rP7MOD1 & (~0x0F)) | (GP75_UTXD | GP74_URXD);

	rUCON0	= TMODE_EN | RMODE_EN | PMD_NO_PARITY;
	rUCON1	= WL_8 | UBAUDCLK_DIV0;
	rUBAUD	= baud;
//	rUBAUD	= bmc513_uart_baudrate(UBAUDCLK_DIV0, 57600);
	
	rUFCON	= 0;
	rUINTCON = UART_INT_ALL;		

    rIE1 = IEURX | IEURXERR;
#if 0
	printf((char *)"rUBAUD = 0x%x\n", rUBAUD);
	{
		uint32_t baud[] = {115200, 57600, 38400, 19200, 9600};
		uint8_t i;
		for(i=0; i<5; i++) {
			printf((char *)"%lu -> %d\n", baud[i], bmc513_uart_baudrate(0, baud[i]));
		}
		for(i=0; i<5; i++) {
			printf((char *)"%lu -> %d\n", baud[i], bmc513_uart_baudrate(6, baud[i]));
		}
	}
#endif
}

__near_func void init_timer(uint8_t channel, uint8_t mode, uint8_t period, uint8_t duty)
{
	uint32_t Tclk, Tcnt;
	uint8_t div;

	for(div=0; div < 11; div++) {
		Tclk = Fsys >> (div+2);
		Tcnt = (Tclk * period) / 1000;
		if( Tcnt < 0x10000 ) break;
	}

	printf((char *)"div = %d, Tcnt = %ld\n", div, Tcnt);
	if( (mode & 0x0C) == SEL_PWM ) {
		Tclk = (Tcnt * duty) / 100;
		printf((char *)"pwm data = %ld\n", Tclk);
	}

	// Timer-2 Run : 100ms tick.
	/* TnCLK = Fsys / 2^(CLK_SEL+2), Fsys = 24MHz
	 *    when CLK_SEL = 0 : TnCLK = Fsys / 2^2 = Fsys / 4 = 24MHz / 4 = 6MHz
	 *    when CLK_SEL = 1 : TnCLK = Fsys / 2^3 = Fsys / 8 = 24MHz / 8 = 3MHz
	 * TnOUT = TnCNT / TnCLK, TnCNT = TnOUT * TnCLK
	 *    1ms Time-out : Tcnt = 1ms * 6MHz = 1 * 6 * 10^3 = 6000
	 *    10ms Time-out : Tcnt = 10ms * 6MHz = 10 * 6 * 10^3 = 60000
	 */
	if( channel == 0 ) {
		rCLK_EN0 |= T0_CKEN;				// Timer0 Clock Enable

		// Timer0 clock = Fsys/(2^2) = 6MHz
		if( (mode & 0x0C) == SEL_PWM ) rT0PDR = (uint16_t)Tclk;
		rT0DATA  = (uint16_t)Tcnt;
		rT0CON   = mode | (div << 4);
		if( mode & TINTPEND ) {
			rIE0     |= IET0;		// Timer0 Match Interrupt enable
		}

//		tick_1ms = 0;
	} else if( channel == 1 ) {
		rCLK_EN0 |= T1_CKEN;				// Timer0 Clock Enable

		// Timer0 clock = Fsys/(2^2) = 6MHz
		if( (mode & 0x0C) == SEL_PWM ) rT1PDR = (uint16_t)Tclk;
		rT1DATA  = (uint16_t)Tcnt;
		rT1CON   = mode | (div << 4);
		if( mode & TINTPEND ) {
			rIE0     |= IET1;		// Timer0 Match Interrupt enable
		}
	} else if( channel == 2 ) {
		rCLK_EN0 |= T2_CKEN;				// Timer2 Clock Enable

		if( (mode & 0x0C) == SEL_PWM ) rT2PDR = (uint16_t)Tclk;
		rT2DATA  = (uint16_t)Tcnt;
		rT2CON   = mode | (div << 4);
		if( mode & TINTPEND ) {
			rIE1     |= IET2;		// Timer0 Match Interrupt enable
		}
	}

}

__near_func void init_system(void)
{
	__disable_interrupt();		// global interrupt disable.
	init_clk();  
	init_wdt(); 			// WDT disable, Default settig of WDT is enable.
	init_gpio();
    init_spi();
	init_uart(BAUD_57600);
//    printf((char *)"Press Any key\n"); getchar();
	rP0MOD1 = (rP0MOD1 & 0xF3) | 0x0C;				// GP0[5] : T0OUT
	init_timer(0, TINTPEND|SEL_PWM|TRUN, 10, 50);		// 10 ms timer-out
	init_timer(1, TINTPEND|SEL_INTERVAL|TRUN, 1, 0);		// 1 ms timer-out
	tick_1ms = 0;
	millisec.l = 0;
	tick_10ms = 0;

//	__enable_interrupt();		// global interrupt enable.
}

/*		EOF		*/

