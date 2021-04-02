/**
 * @file Test.c
 * @brief Multi-Switch Emulator function Test.
 * @author khjung ( khjung@leolsi.com )
 * @version 0.1
 * @date 2019-08-02
 * @see 참고할 내용들을 여기에 기술한다.
 * @todo 추가적으로 해야 할 사항.
 * @deprecated 향후 삭제 예정인 내용들을 표시함.
 */

#define __TEST_c__
#include "includes.h"

///< command length + command's...
const uint8_t command_list[] = {
	0x03, 0x11, 0x22, 0x33,
	0x04, 0x11, 0x22, 0x33, 0x44,
	0x01, 0x11,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x05, 0x11, 0x22, 0x33, 0x44, 0x55,
	0x0C, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC,
	0x08, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
	0x00
};

#define SWITCH_INPUT_IN_GROUP		(0x0)
#define SWITCH_INPUT_IN_CHANNEL	(0x1)
#define CHANNEL_INPUT			(0x2)
#define CHANNEL_SWITCH_INPUT		(0x3)
#define GROUP_INPUT				(0x4)
#define GROUP_SWITCH_INPUT		(0x5)

///< ON -> HOLD OFF -> RELEASE ON -> FORCE OFF -> HOLD ON -> RELEASE OFF -> FORCE ON -> OFF
const uint8_t c_op[] = {0x1, 0x2, 0x5, 0x6, 0x3, 0x4, 0x7, 0x0};
uint8_t cmd_buf[MAX_FRAME];

uint8_t manual_input_select(uint8_t type, uint8_t *frame)
{
	uint8_t i;
	uint8_t idx = 0;
	uint8_t gt = BitCount[GroupType].row;

	if( type == 0 ) {
		printf((char *)"Input %d-bit Switch Select(MSB) ? ", gt);
		if( gt == BRANCH4 ) {
//			printf((char *)"Input 4-bit Switch Select(MSB) ? ");
			frame[idx++] = (getnum() & 0x0F) << 4;
		} else if( gt == BRANCH6 ) {
//			printf((char *)"Input 6-bit Switch Select(MSB) ? ");
			frame[idx++] = (getnum() & 0x3F) << 2;
		} else if( gt == BRANCH8 ) {
//			printf((char *)"Input 8-bit Switch Select(MSB) ? ");
			frame[idx++] = getnum() & 0xFF;
		} else if( gt == BRANCH10 ) {
//			printf((char *)"Input 10-bit Switch Select(MSB) ? ");
			frame[idx++] = getnum() & 0xFF;
			printf((char *)"Input Next Switch Select(MSB) ? ");
			frame[idx++] = (getnum() & 0x03) << 6;
		} else if( gt == BRANCH12 ) {
//			printf((char *)"Input 12-bit Switch Select(MSB) ? ");
			frame[idx++] = getnum() & 0xFF;
			printf((char *)"Input Next Switch Select(MSB) ? ");
			frame[idx++] = (getnum() & 0x0F) << 4;
		} else if( gt == BRANCH16 ) {
		}
	} else if( type == 1 ) {			// Switch Select in Channel.
		printf((char *)"Input First 8-bit Switch Select ? ");
		if( gt == BRANCH4 ) {
//			printf((char *)"Input First 8-bit Switch Select ? ");
			frame[idx++] = (getnum() & 0xFF);
			for(i=0; i<3; i++) {
				printf((char *)"Input Next 8-bit Switch Select ? ");
				frame[idx++] = getnum() & 0xFF;
			}
		} else if( gt == BRANCH6 ) {
//			printf((char *)"Input First 8-bit Switch Select ? ");
			frame[idx++] = (getnum() & 0xFF);
			printf((char *)"Input Next 8-bit Switch Select ? ");
			frame[idx++] = getnum() & 0xFF;
			printf((char *)"Input Next 6-bit Switch Select ? ");
			frame[idx++] = (getnum() & 0x3F) << 2;
		} else if( gt == BRANCH8 ) {
//			printf((char *)"Input First 8-bit Switch Select ? ");
			frame[idx++] = (getnum() & 0xFF);
			printf((char *)"Input Next 8-bit Switch Select ? ");
			frame[idx++] = getnum() & 0xFF;
		} else if( gt == BRANCH10 ) {
//			printf((char *)"Input First 8-bit Switch Select ? ");
			frame[idx++] = (getnum() & 0xFF);
			printf((char *)"Input Next 5-bit Switch Select ? ");
			frame[idx++] = (getnum() & 0x1F) << 3;
		} else if( gt == BRANCH12 ) {
//			printf((char *)"Input First 8-bit Switch Select ? ");
			frame[idx++] = (getnum() & 0xFF);
			printf((char *)"Input Next 3-bit Switch Select ? ");
			frame[idx++] = (getnum() & 0x07) << 5;
		} else if( gt == BRANCH16 ) {
		}
	} else if( type & (1<<1) ) {						// Channel select.
		printf((char *)"Input 2-bit Channel Select(MSB) ? ");
		if( gt == BRANCH4 ) {
//			printf((char *)"Input 4-bit Channel Select(MSB) ? ");
			frame[idx++] = (getnum() & 0x0F) << 4;
			if( type & (1<<0) ) {
				idx--;
				printf((char *)"Input First 4-bit Switch Select ? ");
				frame[idx++] |= getnum() & 0x0F;
				for(i=0; i<3; i++) {
					printf((char *)"Input Next 8-bit Switch Select ? ");
					frame[idx++] = getnum() & 0xFF;
				}
				printf((char *)"Input Last 4-bit Switch Select ? ");
				frame[idx++] = (getnum() & 0x0F) << 4;
			}
		} else if( gt == BRANCH6 ) {
//			printf((char *)"Input 6-bit Channel Select(MSB) ? ");
			frame[idx++] = (getnum() & 0x3F) << 2;
			if( type & (1<<0) ) {
				idx--;
				printf((char *)"Input First 2-bit Switch Select ? ");
				frame[idx++] |= getnum() & 0x03;
				printf((char *)"Input Next 8-bit Switch Select ? ");
				frame[idx++] = getnum() & 0xFF;
				printf((char *)"Input Next 8-bit Switch Select ? ");
				frame[idx++] = getnum() & 0xFF;
				printf((char *)"Input Last 4-bit Switch Select ? ");
				frame[idx++] = (getnum() & 0x0F) << 4;
			}
		} else if( gt == BRANCH8 ) {
//			printf((char *)"Input 8-bit Channel Select(MSB) ? ");
			frame[idx++] = getnum() & 0xFF;
			if( type & (1<<0) ) {
				printf((char *)"Input First 8-bit Switch Select ? ");
				frame[idx++] = getnum() & 0xFF;
				printf((char *)"Input Last 8-bit Switch Select ? ");
				frame[idx++] = getnum() & 0xFF;
			}
		} else if( gt == BRANCH10 ) {
//			printf((char *)"Input 10-bit Channel Select(MSB) ? ");
			frame[idx++] = getnum() & 0xFF;
			printf((char *)"Input Next Channel Select(MSB) ? ");
			frame[idx++] = (getnum() & 0x03) << 6;
			if( type & (1<<0) ) {
				idx--;
				printf((char *)"Input First 6-bit Switch Select ? ");
				frame[idx++] |= getnum() & 0x3F;
				printf((char *)"Input Last 7-bit Switch Select ? ");
				frame[idx++] = (getnum() & 0x7F) << 1;
			}
		} else if( gt == BRANCH12 ) {
//			printf((char *)"Input 12-bit Channel Select(MSB) ? ");
			frame[idx++] = getnum() & 0xFF;
			printf((char *)"Input Next Channel Select(MSB) ? ");
			frame[idx++] = (getnum() & 0x0F) << 4;
			if( type & (1<<0) ) {
				idx--;
				printf((char *)"Input First 4-bit Switch Select ? ");
				frame[idx++] |= getnum() & 0xF;
				printf((char *)"Input Last 7-bit Switch Select ? ");
				frame[idx++] = (getnum() & 0x7F) << 1;
			}
		} else if( gt == BRANCH16 ) {
		}
	} else if( type & (1 << 2) ) {				// group select.
		if( gt == BRANCH4 ) {
			printf((char *)"Input 32-bit Group Select(MSB) ? ");
			frame[idx++] = getnum() & 0xFF;
			for(i=0; i<3; i++) {
				printf((char *)"Input Next Group Select(MSB) ? ");
				frame[idx++] = getnum() & 0xFF;
			}
			if( type & (1<<0) ) {
				printf((char *)"Input 4-bit Switch Select ? ");
				frame[idx++] = (getnum() & 0x0F) << 4;
			}
		} else if( gt == BRANCH6 ) {
			printf((char *)"Input 22-bit Group Select(MSB) ? ");
			frame[idx++] = getnum() & 0xFF;
			printf((char *)"Input Next Group Select(MSB) ? ");
			frame[idx++] = getnum() & 0xFF;
			printf((char *)"Input Next Group Select(MSB) ? ");
			frame[idx++] = (getnum() & 0x3F) << 2;
			if( type & (1<<0) ) {
				uint8_t tmp;
				idx--;
				printf((char *)"Input 6-bit Switch Select ? ");
				tmp = getnum() & 0x3F;
				frame[idx++] |= (tmp & 0x30) >> 4;
				frame[idx++] = (tmp & 0x0F) << 4;
			}
		} else if( gt == BRANCH8 ) {
			printf((char *)"Input 16-bit Group Select(MSB) ? ");
			frame[idx++] = getnum() & 0xFF;
			printf((char *)"Input Next Group Select(MSB) ? ");
			frame[idx++] = getnum() & 0xFF;
			if( type & (1<<0) ) {
				printf((char *)"Input 8-bit Switch Select ? ");
				frame[idx++] = getnum() & 0xFF;
			}
		} else if( gt == BRANCH10 ) {
			printf((char *)"Input 13-bit Group Select(MSB) ? ");
			frame[idx++] = getnum() & 0xFF;
			printf((char *)"Input Next Group Select(MSB) ? ");
			frame[idx++] = (getnum() & 0x1F) << 3;
			if( type & (1<<0) ) {
				idx--;
				printf((char *)"Input First 3-bit Switch Select ? ");
				frame[idx++] |= (getnum() & 0x07);
				printf((char *)"Input Last 7-bit Switch Select ? ");
				frame[idx++] = (getnum() & 0x7F) << 1;
			}
		} else if( gt == BRANCH12 ) {
			printf((char *)"Input 11-bit Group Select(MSB) ? ");
			frame[idx++] = getnum() & 0xFF;
			printf((char *)"Input Next Group Select(MSB) ? ");
			frame[idx++] = (getnum() & 0x07) << 5;
			if( type & (1<<0) ) {
				idx--;
				printf((char *)"Input First 5-bit Switch Select ? ");
				frame[idx++] |= (getnum() & 0x1F);
				printf((char *)"Input Last 7-bit Switch Select ? ");
				frame[idx++] = (getnum() & 0x7F) << 1;
			}
		} else if( gt == BRANCH16 ) {
		}
	}

	return idx;
}

void Manual_Command_Test(void)
{
	uint8_t i, type, opcode, chip, idx;

	do {
		///< frame buffer clear.
		for(i=0; i<MAX_FRAME; i++) gFrame[i] = 0;

		idx = 0;
		gFrame[idx++] = 0xAA;

		printf((char *)"Input Command Type Hex Value(0 ~ 15) ? ");
		type = getnum() & 0x1F;

		printf((char *)"Input Command Opcode Hex Value(0 ~ 7) ? ");
		opcode = getnum() & 0x7;

		gFrame[idx++] = (type << 3) | opcode;

		if( type & (1<<3) ) {
			printf((char *)"Input Chip Select Hex Value ? ");
			chip = getnum();
			gFrame[idx++] = chip;
		}

		type = type & 0x7;
		if( type == 0 ) {				///< single command.
		} else if( type == 1 ) {			///< 1-switch command.
			printf((char *)"Input Switch Number ? ");
			gFrame[idx++] = (uint8_t)getnum();
		} else if( (type == 2) || (type == 3) ) {	///< row select command.
			uint8_t nRow = get_row_count(0);

			printf((char *)"Input %d-bits Row Select(MSB)\n", nRow);
			for(i=0; i<nRow; i++) {
				printf((char *)"\t%d-row select(Yes/No) ? ", i);
				if( tolower(getchar()) == 'y' ) {
					set_bit(i, &gFrame[idx]);
				}
				putchar('\n');
			}

			if( type == 3 ) {			///< row & switch select command.
				uint8_t nSw = get_switch_count_in_row(0);

				nSw += nRow;
				printf((char *)"Input %d-bits Switch Select(MSB)\n", nSw);
				for( ; i<nSw; i++) {
					printf((char *)"\t%d-switch select(Yes/No) ? ", i);
					if( tolower(getchar()) == 'y' ) {
						set_bit(i, &gFrame[idx]);
					}
					putchar('\n');
				}
			}
			idx += (i / 8) + ((i & 0x7) ? 1 : 0);
		} else if( (type == 4) || (type == 5) ) {			///< column select command.
			uint8_t nColumn = get_column_count(0);

			printf((char *)"Input %d-bits Row Select(MSB)\n", nColumn);
			for(i=0; i<nColumn; i++) {
				printf((char *)"\t%d-column select(Yes/No) ? ", i);
				if( tolower(getchar()) == 'y' ) {
					set_bit(i, &gFrame[idx]);
				}
				putchar('\n');
			}

			if( type == 5 ) {			///< column & switch select command.
				uint8_t nSw = get_switch_count_in_row(0);

				nSw += nColumn;
				printf((char *)"Input %d-bits Switch Select(MSB)\n", nSw);
				for( ; i<nSw; i++) {
					printf((char *)"\t%d-switch select(Yes/No) ? ", i);
					if( tolower(getchar()) == 'y' ) {
						set_bit(i, &gFrame[idx]);
					}
					putchar('\n');
				}
			}
			idx += (i / 8) + ((i & 0x7) ? 1 : 0);
		} else if( type == 6 ) {			///< special command.
		} else if( type == 7 ) {			///< all switch command.
			for(uint8_t i=0; i<MAX_BYTES; i++) {
				printf((char *)"Input (%d - %d)Switch Select Hex Value ? ", i*8, (i*8)+7);
				gFrame[idx++] = (uint8_t)getnum();
			}
		}

		printf((char *)"Command Frame : ");
		for(uint8_t i=0; i<idx; i++) {
			printf((char *)"0x%02X ", gFrame[i]);
		}
		printf((char *)"\n");

		send_command(gFrame, idx);

		printf((char *)"press ESC key for EXIT.\n");
	} while( getchar() != ESC );
}


void Script_Command_Test(void)
{
	uint8_t i, length;
	uint16_t idx;

	do {
		printf((char *)"Total command size = %d, 0x%x\n", sizeof(command_list), &command_list[0]);
		idx = 0;
		i = 0;
		do {
			length = command_list[idx++];
			if( length == 0 ) break;

			printf((char *)"%d. command : size = %d, address = 0x%X\n", i, length, &command_list[idx]);
			send_command((uint8_t *)&command_list[idx], length);

			idx += length;
			i++;
		} while( 1 );
	} while( getchar() != ESC );
}


void Chip_Address_Assignment_Test(void)
{
	uint8_t chip;

	// Chip Address Assignment.
	chip = 0xFF;
	mSW_CHIP_ADDRESS_ASSIGN(chip);

	INFO("chip address assigned -> ");
	for(uint8_t i=0; i<gMaxChip; i++) {
		if( eSW[i].ChipSelect & (1 << (7 - i)) ) {
			printf((char *)"%d(0x%X) ", i, eSW[i].ChipSelect);
		}
	}
	putchar('\n');
}


void menu_display(uint8_t type, char __code *str)
{
	printf((char *)"------------ %s Command Test. -------------", str);
	puts((char *)" 0 : Switch OFF");
	puts((char *)" 1 : Switch ON");
	puts((char *)" 2 : Switch Hold OFF");
	puts((char *)" 3 : Switch Hold ON");
	puts((char *)" 4 : Switch Release OFF");
	puts((char *)" 5 : Switch Release ON");
	puts((char *)" 6 : Switch Force OFF");
	puts((char *)" 7 : Switch Force ON");
	if( type == 0 ) {
	} else if( type == 2 ) {
		puts((char *)" 8 : new Chip Select");
		puts((char *)" 9 : random long-run test.");
	} else if( type == 3 ) {
		puts((char *)" 8 : new Switch");
		puts((char *)" 9 : new Chip Select");
	} else if( type == 4 ) {
		puts((char *)" 8 : new Switch Select");
		puts((char *)" 9 : new Chip Select");
	} else if( type == 5 ) {
		puts((char *)" 8 : new Row Select");
		puts((char *)" 9 : new Chip Select");
		puts((char *)" A : change Group Type");
	} else if( type == 6 ) {
		puts((char *)" 8 : new Row Select");
		puts((char *)" 9 : new Switch Select");
		puts((char *)" A : new Chip Select");
		puts((char *)" B : change Group Type");
	} else if( type == 7 ) {
		puts((char *)" 8 : new Row Select");
		puts((char *)" 9 : new Chip Select");
		puts((char *)" A : change Group Type");
	} else if( type == 8 ) {
		puts((char *)" 8 : new Row Select");
		puts((char *)" 9 : new Switch Select");
		puts((char *)" A : new Chip Select");
		puts((char *)" B : change Group Type");
	}
	puts((char *)"----------------------------------------------------");
	printf((char *)" menu select ? ");
}


void All_Switch_Command_Test(void)
{
	uint8_t sw[16];
	uint8_t i, idx, opcode, chip, ch;

	chip = 0;
	for(i=0; i<16; i++) sw[i] = 0xFF;

	do {
		idx = 0;
		menu_display(4, "All Switch");

		ch = getchar();
		putchar(ch);

		if( isxdigit(ch) ) {
			gFrame[idx++] = 0xAA;
			if( (ch >= '0') && (ch <= '7') ) {
				opcode = ch - '0';

				if( chip != 0 ) {
					gFrame[idx++] = (8<<3) | opcode;
					gFrame[idx++] = chip;			// switch number.
				} else {
					gFrame[idx++] = opcode;
				}
				for(i=0; i<16; i++) gFrame[idx++] = sw[i];

				send_command(gFrame, idx);

				emulator_switch_dump(0xFF);
			} else if( ch == '8' ) {
				printf((char *)"new switch select is random generated.\n");
				for(i=0; i<16; i++) {
					sw[i] = rand() & 0xFF;
				}
			} else if( ch == '9' ) {
				printf((char *)"new chip select(0x0 ~ 0xFF) ? ");
				chip = getnum();
			}
		}
	} while( ch != ESC );
}

void run_command(uint8_t cmd, uint8_t chip, uint8_t *bundle, uint8_t *sw)
{
	uint8_t i, idx;
	uint8_t gt = BitCount[GroupType].row;

	idx = 0;
	gFrame[idx++] = 0xAA;
	gFrame[idx++] = cmd;
	if( cmd != (1<<6) ) gFrame[idx++] = chip;			// switch number.

	if( (cmd & (1<<5)) && (bundle != NULL) ) {		// Group Select.
		switch( gt ) {
			case BRANCH4:
				for(i=0; i<4; i++) gFrame[idx++] = bundle[i];
				break;
			case BRANCH6:
				gFrame[idx++] = bundle[0];
				gFrame[idx++] = bundle[1];
				gFrame[idx++] = bundle[2] & 0xFC;
				break;
			case BRANCH8:
				gFrame[idx++] = bundle[0];
				gFrame[idx++] = bundle[1];
				break;
			case BRANCH10:
				gFrame[idx++] = bundle[0];
				gFrame[idx++] = bundle[1] & 0xF8;
				break;
			case BRANCH12:
				gFrame[idx++] = bundle[0];
				gFrame[idx++] = bundle[1] & 0xE0;
				break;
			case BRANCH16:
				break;
		}

		if( (cmd & (1<<3)) && (sw != NULL) ) {	// Switch Select.
			if( gt == BRANCH4 ) {		// 4-bit switch
				gFrame[idx++] = sw[0] & 0xF0;
			} else if( gt == BRANCH6 ) {		// 6-bit switch
				idx--;
				gFrame[idx++] |= (sw[0] & 0xC0) >> 6;
				gFrame[idx++] = (sw[0] & 0x3C) << 2;
			} else if( gt == BRANCH8 ) {		// 8-bit switch
				gFrame[idx++] = sw[0] & 0xFF;
			} else if( gt == BRANCH10 ) {		// 10-bit switch
				idx--;
				gFrame[idx++] |= (sw[0] & 0xE0) >> 5;
				gFrame[idx++] = ((sw[0] & 0x1F) << 3) | ((sw[1] & 0xC0) >> 6);
			} else if( gt == BRANCH12 ) {		// 12-bit switch
				idx--;
				gFrame[idx++] |= (sw[0] & 0xF8) >> 3;
				gFrame[idx++] = ((sw[0] & 0x07) << 5) | ((sw[1] & 0xF0) >> 3);
			} else if( gt == BRANCH16 ) {		// 12-bit switch
			}
		}
	} else if( (cmd & (1<<4)) && (bundle != NULL) ) {	// Channel Select.
		if( gt == BRANCH4 ) {		// 4-bit channel
			gFrame[idx++] = bundle[0] & 0xF0;
		} else if( gt == BRANCH6 ) {		// 6-bit channel
			gFrame[idx++] = bundle[0] & 0xFC;
		} else if( gt == BRANCH8 ) {		// 8-bit channel
			gFrame[idx++] = bundle[0] & 0xFF;
		} else if( gt == BRANCH10 ) {		// 10-bit channel
			gFrame[idx++] = bundle[0] & 0xFF;
			gFrame[idx++] = bundle[1] & 0xC0;
		} else if( gt == BRANCH12 ) {		// 12-bit channel
			gFrame[idx++] = bundle[0] & 0xFF;
			gFrame[idx++] = bundle[1] & 0xF0;
		} else if( gt == BRANCH16 ) {		// 12-bit channel
		}

		if( (cmd & (1<<3)) && (sw != NULL) ) {	// Channel Select.
			if( gt == BRANCH4 ) {		// 32-bit switch
				idx--;
				gFrame[idx++] |= (sw[0] & 0xF0) >> 4;
				gFrame[idx++] = ((sw[0] & 0x0F) << 4) | ((sw[1] & 0xF0) >> 4);
				gFrame[idx++] = ((sw[1] & 0x0F) << 4) | ((sw[2] & 0xF0) >> 4);
				gFrame[idx++] = ((sw[2] & 0x0F) << 4) | ((sw[3] & 0xF0) >> 4);
				gFrame[idx++] = ((sw[3] & 0x0F) << 4);
			} else if( gt == BRANCH6 ) {		// 22-bit switch
				idx--;
				gFrame[idx++] |= (sw[0] & 0xC0) >> 6;
				gFrame[idx++] = ((sw[0] & 0x3F) << 2) | ((sw[1] & 0xC0) >> 6);
				gFrame[idx++] = ((sw[1] & 0x3F) << 2) | ((sw[2] & 0xC0) >> 6);
				gFrame[idx++] = ((sw[2] & 0x0F) << 4);
			} else if( gt == BRANCH8 ) {		// 16-bit switch
				gFrame[idx++] = sw[0] & 0xFF;
				gFrame[idx++] = sw[1] & 0xFF;
			} else if( gt == BRANCH10 ) {		// 13-bit switch
				idx--;
				gFrame[idx++] |= (sw[0] & 0xFC) >> 2;
				gFrame[idx++] = ((sw[0] & 0x03) << 6) | ((sw[1] & 0xF8) >> 2);
			} else if( gt == BRANCH12 ) {		// 11-bit switch
				idx--;
				gFrame[idx++] |= (sw[0] & 0xF0) >> 4;
				gFrame[idx++] = ((sw[0] & 0x0F) << 4) | ((sw[1] & 0xE0) >> 4);
			} else if( gt == BRANCH16 ) {		// 8-bit switch
				gFrame[idx++] = sw[0] & 0xFF;
			}
		}
	}
	send_command(gFrame, idx);

	emulator_switch_dump(0xFF);
}

void Channel_Command_Test(void)
{
	uint8_t channel[2];
	uint8_t opcode, cmd, chip, ch;

	INFO("This function is still in writing.\n");
	chip = 0;
	channel[0] = channel[1] = 0xFF;

	do {
		menu_display(5, "Row");

		ch = getchar();
		putchar(ch);

		if( isxdigit(ch) ) {
			if( (ch >= '0') && (ch <= '7') ) {
				opcode = ch - '0';
				cmd = ((chip == 0) ? (0x2<<3) : (0xA<<3)) | opcode;

				run_command(cmd, chip, channel, NULL);
			} else if( ch == '8' ) {
				uint8_t nRow = get_row_count(0);
				printf((char *)"GroupType = %d\n", GroupType);
				manual_input_select(CHANNEL_INPUT, &channel[0]);
				printf((char *)"channel = 0x%x\n", channel[0]);
			} else if( ch == '9' ) {
				printf((char *)"new chip select(0x0 ~ 0xFF) ? ");
				chip = getnum() & 0xFF;
			} else if( toupper(ch) == 'A' ) {
				printf((char *)"new group type(0 ~ 5) ? ");
				GroupType = ((getnum() % 6) * 2) + 2;
			}
		}
	} while( ch != ESC );
}

void Channel_Switch_Command_Test(void)
{
	uint8_t channel[2];
	uint8_t sw[8];
	uint8_t opcode, cmd, chip, ch;

	do {
		chip = 0;
		channel[0] = channel[1] = 0xFF;

		menu_display(6, "Channel Switch");

		ch = getchar();
		putchar(ch);

		if( isxdigit(ch) ) {
			if( (ch >= '0') && (ch <= '7') ) {
				opcode = ch - '0';
				cmd = ((chip == 0) ? (0x3<<3) : (0xB<<3)) | opcode;

				run_command(cmd, chip, channel, NULL);
			} else if( ch == '8' ) {
				printf((char *)"GroupType = %d\n", GroupType);
				manual_input_select(CHANNEL_INPUT, &channel[0]);
				printf((char *)"channel = 0x%x\n", channel[0]);
			} else if( ch == '9' ) {
				printf((char *)"GroupType = %d\n", GroupType);
				manual_input_select(SWITCH_INPUT_IN_CHANNEL, &sw[0]);
			} else if( toupper(ch) == 'A' ) {
				printf((char *)"new chip select(0x0 ~ 0xFF) ? ");
				chip = getnum() % 0xFF;
			} else if( toupper(ch) == 'B' ) {
				printf((char *)"new group type(0 ~ 5) ? ");
				GroupType = ((getnum() % 6) * 2) + 2;
			}
		}
	} while( ch != ESC );
}

void Group_Command_Test(void)
{
	uint8_t group[8];
	uint8_t i, cmd, opcode, chip, ch;

	do {
		chip = 0;
		for(i=0; i<8; i++) group[i] = 0xFF;

		menu_display(7, "Group");

		ch = getchar();
		putchar(ch);

		if( isxdigit(ch) ) {
			if( (ch >= '0') && (ch <= '7') ) {
				opcode = ch - '0';
				cmd = ((chip == 0) ? (0x4<<3) : (0xC<<3)) | opcode;

				run_command(cmd, chip, group, NULL);
			} else if( ch == '8' ) {
				printf((char *)"GroupType = %d\n", GroupType);
				manual_input_select(GROUP_INPUT, &group[0]);
				printf((char *)"group = 0x%x\n", group[0]);
			} else if( ch == '9' ) {
				printf((char *)"new chip select(0x0 ~ 0xFF) ? ");
				chip = getnum() % 0xFF;
			} else if( toupper(ch) == 'A' ) {
				printf((char *)"new group type(0 ~ 5) ? ");
				GroupType = ((getnum() % 6) * 2) + 2;
			}
		}
	} while( ch != ESC );
}

void Group_Switch_Command_Test(void)
{
	uint8_t group[8];
	uint8_t sw[2];
	uint8_t i, opcode, cmd, chip, ch;

	do {
		chip = 0;
		for(i=0; i<8; i++) group[i] = sw[i] = 0xFF;

		menu_display(8, "Channel Switch");

		ch = getchar();
		putchar(ch);

		if( isxdigit(ch) ) {
			if( (ch >= '0') && (ch <= '7') ) {
				opcode = ch - '0';
				cmd = ((chip == 0) ? (0x5<<3) : (0xD<<3)) | opcode;

				run_command(cmd, chip, group, sw);
			} else if( ch == '8' ) {
				printf((char *)"GroupType = %d\n", GroupType);
				manual_input_select(GROUP_INPUT, &group[0]);
				printf((char *)"group = 0x%x\n", group[0]);
			} else if( ch == '9' ) {
				printf((char *)"GroupType = %d\n", GroupType);
				manual_input_select(SWITCH_INPUT_IN_GROUP, &sw[0]);
				printf((char *)"group = 0x%x\n", sw[0]);
			} else if( toupper(ch) == 'A' ) {
				printf((char *)"new chip select(0x0 ~ 0xFF) ? ");
				chip = getnum() % 0xFF;
			} else if( toupper(ch) == 'B' ) {
				printf((char *)"new group type(0 ~ 5) ? ");
				GroupType = ((getnum() % 6) * 2) + 2;
			}
		}
	} while( ch != ESC );
}



void Simple_Test(void)
{
	uint8_t idx = 0;

	///< all switch control
	gFrame[idx++] = 0xAA;
	gFrame[idx++] = (0x78 | SW_ON);			///< switch ON
	gFrame[idx++] = 0xFF;					///< chip select
	for(uint8_t i=0; i<16; i++) {
		gFrame[idx++] = rand() & 0xFF;
	}
	send_command(gFrame, idx);

	switch_dump(0xFF);
	emulator_switch_dump(0xFF);

	idx = 0;
	// all switch control
	gFrame[idx++] = 0xAA;
	gFrame[idx++] = (0xF<<3) | HOLD_OFF;		///< hold OFF
	gFrame[idx++] = 0xFF;					///< chip select.
	for(uint8_t i=0; i<15; i++) {				///< 120-switch.
		gFrame[idx++] = rand() & 0xFF;
	}
	send_command(gFrame, idx);

	switch_dump(0xFF);
	emulator_switch_dump(0xFF);

	Group_Switch_Command_Test();

	Channel_Switch_Command_Test();

	Group_Command_Test();

	Channel_Command_Test();

	Script_Command_Test();

	Manual_Command_Test();

	All_Switch_Command_Test();

	getchar();
}

void emulator_function_test(void)
{
	uint8_t ch, i;

	do {
		puts((char *)"\n\n 1. get_row_count(chip) test.");
		puts((char *)" 2. get_column_count(chip) test.");
		puts((char *)" 3. get_switch_count_in_row(chip) test.");
		puts((char *)" 4. get_switch_count_in_column(chip) test.");
		puts((char *)" 5. get_switch_number_in_column(chip, column, switch) test.");
		puts((char *)" 6. get_switch_number_in_row(chip, row, switch) test.");

		puts((char *)" 7. emul_set_switch_ctrl(chip, opcode, switch) test.");
		puts((char *)" 8. emul_set_switch_ctrl_in_column(chip, opcode, column, switch) test.");
		puts((char *)" 9. emul_set_switch_ctrl_in_row(chip, opcode, column, switch) test.");
		puts((char *)" A. emul_set_column_ctrl(chip, opcode, column) test.");
		puts((char *)" B. emul_set_row_ctrl(chip, opcode, row) test.");

		puts((char *)" C. emul_get_switch(chip, switch) test.");
		puts((char *)" D. emul_get_switch_onoff(chip, switch) test.");
		puts((char *)" E. emul_get_switch_hold(chip, switch) test.");
		puts((char *)" F. emul_get_switch_onoff_in_column(chip, column, switch) test.");
		puts((char *)" G. emul_get_switch_onoff_in_row(chip, row, switch) test.");
		puts((char *)" H. emul_get_switch_hold_in_column(chip, column, switch) test.");
		puts((char *)" I. emul_get_switch_hold_in_row(chip, row, switch) test.");
		printf((char *)" select menu ? ");
		ch = getchar();

		ch = toupper(ch);
		putchar('\n');

		if( ch == '1' ) {
			uint8_t tmp = eSW[0].GroupType;
			for(i=0; i<6; i++) {
				eSW[0].GroupType = i;

				printf((char *)"[ %d-GroupType ]\n", BitCount[eSW[0].GroupType].row);
				printf((char *)" - get_row_count = %d\n", get_row_count(0));
				printf((char *)" - get_column_count = %d\n", get_column_count(0));
				printf((char *)" - get_switch_count_in_row = %d\n", get_switch_count_in_row(0));
				printf((char *)" - get_switch_count_in_column = %d\n", get_switch_count_in_column(0));
			}
			eSW[0].GroupType = tmp;
		} else if( ch == '2' ) {
		} else if( ch == '3' ) {
		} else if( ch == '4' ) {
		} else if( ch == '5' ) {
			uint8_t column;
			uint8_t nColumn = get_column_count(0);
			uint8_t nSw = get_switch_count_in_column(0);

			for(column=0; column<nColumn; column++) {
				printf((char *)"%d-column : ", column);
				for(i=0; i<nSw; i++) {
					printf((char *)"%d ", get_switch_number_in_column(0, column, i));
				}
				putchar('\n');
			}
		} else if( ch == '6' ) {
			uint8_t row;
			uint8_t nRow = get_row_count(0);
			uint8_t nSw = get_switch_count_in_row(0);

			for(row=0; row<nRow; row++) {
				printf((char *)"%d-row : ", row);
				for(i=0; i<nSw; i++) {
					printf((char *)"%d ", get_switch_number_in_row(0, row, i));
				}
				putchar('\n');
			}
		} else if( ch == '7' ) {
			uint8_t sw, op;

			for(sw=0; sw<MAX_SWITCH; sw++) {
				op = rand() & 0x7;
				emul_set_switch_ctrl(0, op, sw);			///< all switch hold on.
			}

			chip_status(0);
		} else if( ch == '8' ) {
		puts((char *)" 8. emul_set_switch_ctrl_in_column(chip, opcode, column, switch) test.");
		} else if( ch == '9' ) {
		puts((char *)" 9. emul_set_switch_ctrl_in_row(chip, opcode, column, switch) test.");
		} else if( ch == 'A' ) {
		puts((char *)" A. emul_set_column_ctrl(chip, opcode, column) test.");
		} else if( ch == 'B' ) {
		puts((char *)" B. emul_set_row_ctrl(chip, opcode, row) test.");
		} else if( ch == 'C' ) {
			uint8_t sw, column;
			uint8_t nColumn = get_column_count(0);
			uint8_t nSw = get_switch_count_in_column(0);

			putchar('\n');
			for(column=0; column<nColumn; column++) {
				printf((char *)"%2d-column : ", column);
				for(sw=0; sw<nSw; sw++) {
					printf((char *)"%d ", emul_get_switch(0, (column*nSw)+sw));
				}
				putchar('\n');
			}
			putchar('\n');
		} else if( ch == 'D' ) {
			uint8_t sw, column;
			uint8_t nColumn = get_column_count(0);
			uint8_t nSw = get_switch_count_in_column(0);

			putchar('\n');
			for(column=0; column<nColumn; column++) {
				printf((char *)"%2d-column : ", column);
				for(sw=0; sw<nSw; sw++) {
					printf((char *)"%d ", emul_get_switch_onoff(0, (column*nSw)+sw) );
				}
				putchar('\n');
			}
			putchar('\n');
		} else if( ch == 'E' ) {
			uint8_t sw, column;
			uint8_t nColumn = get_column_count(0);
			uint8_t nSw = get_switch_count_in_column(0);

			putchar('\n');
			for(column=0; column<nColumn; column++) {
				printf((char *)"%2d-column : ", column);
				for(sw=0; sw<nSw; sw++) {
					printf((char *)"%d ", emul_get_switch_hold(0, (column*nSw)+sw) );
				}
				putchar('\n');
			}
			putchar('\n');
		} else if( ch == 'F' ) {
			uint8_t sw, column;
			uint8_t nColumn = get_column_count(0);
			uint8_t nSw = get_switch_count_in_column(0);

			putchar('\n');
			for(column=0; column<nColumn; column++) {
				printf((char *)"%2d-column : ", column);
				for(sw=0; sw<nSw; sw++) {
					printf((char *)"%d ", emul_get_switch_onoff_in_column(0, column, sw) );
				}
				putchar('\n');
			}
			putchar('\n');
		} else if( ch == 'G' ) {
			uint8_t sw, row;
			uint8_t nRow = get_row_count(0);
			uint8_t nSw = get_switch_count_in_column(0);

			putchar('\n');
			for(row=0; row<nRow; row++) {
				printf((char *)"%2d-row : ", row);
				for(sw=0; sw<nSw; sw++) {
					printf((char *)"%d ", emul_get_switch_onoff_in_row(0, row, sw) );
				}
				putchar('\n');
			}
			putchar('\n');
		} else if( ch == 'H' ) {
			uint8_t sw, column;
			uint8_t nColumn = get_column_count(0);
			uint8_t nSw = get_switch_count_in_column(0);

			putchar('\n');
			for(column=0; column<nColumn; column++) {
				printf((char *)"%2d-column : ", column);
				for(sw=0; sw<nSw; sw++) {
					printf((char *)"%d ", emul_get_switch_hold_in_column(0, column, sw) );
				}
				putchar('\n');
			}
			putchar('\n');
		} else if( ch == 'I' ) {
			uint8_t sw, row;
			uint8_t nRow = get_row_count(0);
			uint8_t nSw = get_switch_count_in_column(0);

			putchar('\n');
			for(row=0; row<nRow; row++) {
				printf((char *)"%2d-row : ", row);
				for(sw=0; sw<nSw; sw++) {
					printf((char *)"%d ", emul_get_switch_hold_in_row(0, row, sw) );
				}
				putchar('\n');
			}
			putchar('\n');
		} else if( ch == 'J' ) {
		} else if( ch == 'K' ) {
		} else if( ch == 'L' ) {
		} else if( ch == 'M' ) {
		} else if( ch == 'N' ) {
		} else if( ch == 'O' ) {
		} else if( ch == 'P' ) {
		} else if( ch == 'Q' ) {
		} else if( ch == 'R' ) {
		} else if( ch == 'S' ) {
		}
	} while( ch != ESC );
}


void function_test(void)
{
	uint8_t ch;
	uint8_t idx;

	do {
		puts((char *)"\n\n--------------- software function test. ------------------");
		puts((char *)" 1. check_bit(uint8_t n, uint8_t *stream) test.");
		puts((char *)" 2. set_bit(uint8_t n, uint8_t *stream) test.");
		puts((char *)" 3. clear_bit(uint8_t n, uint8_t *stream) test.");
		puts((char *)" 4. get_column_count()/get_row_count() test.");
		puts((char *)" 5. 분기 구조에 따른 2D array test.");
		puts((char *)" 6. emulator function test.");
		puts((char *)" 7. copy_bit(uint8_t s, uint8_t *stream, uint8_t n) function test.");
		puts((char *)" A. All Switch OFF.");
		puts((char *)" B. All Switch ON.");
		puts((char *)" C. All Switch HOLD OFF.");
		puts((char *)" D. All Switch HOLD ON.");
		puts((char *)" E. All Switch RELEASE OFF.");
		puts((char *)" F. All Switch RELEASE ON.");
		puts((char *)" G. All Switch FORCE OFF.");
		puts((char *)" H. All Switch FORCE ON.");
		puts((char *)" I. Each Switch HOLD ON.");
//		puts((char *)" I. All Switch ON/OFF Status read.");
		for(uint8_t k=0; k<60; k++) putchar('-'); putchar('\n');
		printf((char *)" select menu ? ");

		ch = getchar();
		ch = tolower(ch);

		idx = 0;
		if( ch == '1' ) {
			uint8_t i;
			uint8_t tmp[MAX_BYTES];

			printf((char *)"\n INPUT : ");
			for(i=0; i<MAX_BYTES; i++) {
				tmp[i] = rand() & 0xFF;
				printf((char *)"0x%02x ", tmp[i]);
			}

			printf((char *)"\n");
			for(i=0; i<MAX_SWITCH; i++) {
				if( (i % 8) == 0 ) printf((char *)" ");

				putchar((check_bit(i, tmp) ? '1' : '0'));
			}
			printf((char *)"\n");
		} else if( ch == '2' ) {
			uint8_t i;
			uint8_t tmp[MAX_BYTES];

			printf((char *)"\n INPUT : ");
			for(i=0; i<MAX_BYTES; i++) {
				tmp[i] = 0x0;
				printf((char *)"0x%02x ", tmp[i]);
			}

			printf((char *)"\n");
			for(i=0; i<MAX_SWITCH; i++) {
				if( (i % 8) == 0 ) printf((char *)" ");

				set_bit(i, tmp);
				putchar((check_bit(i, tmp) ? '1' : '0'));
			}
			printf((char *)"\n");
		} else if( ch == '3' ) {
			uint8_t i;
			uint8_t tmp[MAX_BYTES];

			printf((char *)"\n INPUT : ");
			for(i=0; i<MAX_BYTES; i++) {
				tmp[i] = 0xFF;
				printf((char *)"0x%02x ", tmp[i]);
			}

			printf((char *)"\n");
			for(i=0; i<MAX_SWITCH; i++) {
				if( (i % 8) == 0 ) printf((char *)" ");

				clear_bit(i, tmp);
				putchar((check_bit(i, tmp) ? '1' : '0'));
			}
			printf((char *)"\n");
		} else if( ch == '4' ) {
			uint8_t i, tmp;

			tmp = eSW[0].GroupType;
			for(i=0; i<6; i++) {
				eSW[0].GroupType = i;
				printf((char *)"%d-branch : %d-column(%d-switch), %d-row(%d-switch)\n",
						BitCount[eSW[0].GroupType].row,
						get_column_count(0),
						get_switch_count_in_column(0),
						get_row_count(0),
						get_switch_count_in_row(0));
			}
			eSW[0].GroupType = tmp;
		} else if( ch == '5' ) {
#if (SWITCH_ACCESS_SIZE == BYTE_ACCESS)	///< switch 상태를 Byte 단위로 처리하기 위한 변수.
//			uint8_t (*group2)[2];		// 2분기, 2차원 배열.
			uint8_t (*group4)[4];		// 4분기, 2차원 배열.
			uint8_t (*group6)[6];		// 6분기, 2차원 배열.
			uint8_t (*group8)[8];		// 8분기, 2차원 배열.
			uint8_t (*group10)[10];		// 10분기, 2차원 배열.
			uint8_t (*group12)[12];		// 12분기, 2차원 배열.
			uint8_t (*group16)[16];		// 16분기, 2차원 배열.
			uint8_t chip, column, row, sw;

			for(chip=0; chip<MAX_CHIP; chip++) {
				for(sw=0; sw<MAX_SWITCH; sw++) {
					eSW[chip].bSwitch[sw].Info = 1+sw;
				}
			}

#if 0
			///< 2분기 group/channel별 switch read.
			printf((char *)"2 분기 구조 : all switch status.\n");
			group2 = (uint8_t (*)[2])eSW[0].bSwitch;
			printf((char *)"     row     0    1\n");
			for(column=0; column<64; column++) {
				printf((char *)"%2d column : ", column);
				for(row=0; row<2; row++) {
					if( ((column * 2) + row) < 128 ) {
						printf((char *)"0x%02X", group2[column][row]);
					} else {
						printf((char *)"0x..");
					}
					putchar((column==63)?'\n':',');
				}
			}
#endif

			///< 4분기 group/channel별 switch read.
			printf((char *)"\n4 분기 구조 : all switch status.\n");
			group4 = (uint8_t (*)[4])eSW[0].bSwitch;
			printf((char *)"     row      0    1    2    3\n");
			for(column=0; column<30; column++) {
				printf((char *)"%2d-column : ", column);
				for(row=0; row<4; row++) {
					if( ((column * 4) + row) < 128 ) {
						printf((char *)"0x%02X", group4[column][row]);
					} else {
						printf((char *)"0x..");
					}
					putchar((row==3)?'\n':',');
				}
			}

			///< 6분기 group/channel별 switch read.
			printf((char *)"\n6 분기 구조 : all switch status.\n");
			group6 = (uint8_t (*)[6])eSW[0].bSwitch;
			printf((char *)"      row     0    1    2    3    4    5\n");
			for(column=0; column<20; column++) {
				printf((char *)"%2d-column : ", column);
				for(row=0; row<6; row++) {
					if( ((column * 6) + row) < 128 ) {
						printf((char *)"0x%02X", group6[column][row]);
					} else {
						printf((char *)"0x..");
					}
					putchar((row==5)?'\n':',');
				}
			}

			///< 8분기 group/channel별 switch read.
			printf((char *)"\n8 분기 구조 : all switch status.\n");
			group8 = (uint8_t (*)[8])eSW[0].bSwitch;
			printf((char *)"      row      0    1    2    3    4    5    6    7\n");
			for(column=0; column<15; column++) {
				printf((char *)"%2d-column : ", column);
				for(row=0; row<8; row++) {
					if( ((column * 8) + row) < 120 ) {
						printf((char *)"0x%02X", group8[column][row]);
					} else {
						printf((char *)"0x..");
					}
					putchar((row==7)?'\n':',');
				}
			}
			///< 10분기 group/channel별 switch read.
			printf((char *)"\n10 분기 구조 : all switch status.\n");
			group10 = (uint8_t (*)[10])eSW[0].bSwitch;
			printf((char *)"      row      0    1    2    3    4    5    6    7    8    9\n");
			for(column=0; column<12; column++) {
				printf((char *)"%2d-column : ", column);
				for(row=0; row<10; row++) {
					if( ((column * 10) + row) < 120 ) {
						printf((char *)"0x%02X", group10[column][row]);
					} else {
						printf((char *)"0x..");
					}
					putchar((row==9)?'\n':',');
				}
			}
			printf((char *)"      row   0123456789\n");
			for(column=0; column<12; column++) {
				printf((char *)"%2d-column : ", column);
				for(row=0; row<10; row++) {
					if( ((column * 8) + row) < 120 ) {
						if( emul_get_switch_onoff_in_column(0, column, row) ) {
							printf((char *)"O");
						} else {
							printf((char *)".");
						}
					} else {
//						printf((char *)"0xXX%row", (column==15)?'\n':',');
					}
				}
				printf((char *)"\n");
			}


			///< 12분기 group/channel별 switch read.
			printf((char *)"\n12-branch : switch status per channel.\n");
			group12 = (uint8_t (*)[12])eSW[0].bSwitch;
			printf((char *)"     group     0    1    2    3    4 ...\n");
			for(column=0; column<10; column++) {
				printf((char *)"%2d-column : ", column);
				for(row=0; row<12; row++) {
					if( ((column * 12) + row) < 120 ) {
						printf((char *)"0x%02X", group12[column][row]);
					} else {
						printf((char *)"0x..");
					}
					putchar((row==11)?'\n':',');
				}
			}

			///< 16분기 group/channel별 switch read.
			printf((char *)"\n16-branch : switch status per channel.\n");
			group16 = (uint8_t (*)[16])eSW[0].bSwitch;
			printf((char *)"     group     0    1    2    3    4 ...\n");
			for(column=0; column<8; column++) {
				printf((char *)"%2d-column : ", column);
				for(row=0; row<16; row++) {
					if( ((column * 16) + row) < 120 ) {
						printf((char *)"0x%02X", group16[column][row]);
					} else {
						printf((char *)"0x..");
					}
					putchar((row==15)?'\n':',');
				}
			}
#elif (SWITCH_ACCESS_SIZE == BIT_ACCESS)	///< switch 상태를 bit 단위로 처리하기 위한 변수.
#endif
		} else if( ch == '6' ) {
			emulator_function_test();
		} else if( ch == '7' ) {
			Byte4 ret;
			uint8_t i;

			idx = 0;
			gFrame[idx++] = 0xC6;
			gFrame[idx++] = 0x7E;
			gFrame[idx++] = 0x81;
			gFrame[idx++] = 0x6B;
			gFrame[idx++] = 0x4B;
			gFrame[idx++] = 0xFB;
			gFrame[idx++] = 0xE2;
			gFrame[idx++] = 0xFB;
			printf((char *)"\ninput : ");
			for(i=0; i<idx; i++) printf((char *)"0x%02X ", gFrame[i]);
			putchar('\n');

			for(i=0; i<idx; i++) {
				ret.l = copy_bit_msb_to_lsb(i, gFrame, 32);
				printf((char *)"result : 0x%02X, 0x%02X, 0x%02X, 0x%02X\n", ret.b[0], ret.b[1], ret.b[2], ret.b[3]);
			}


		} else if( ch == 'a' ) {
			gFrame[idx++] = 0xAA;
			gFrame[idx++] = 0x00;		///< all switch off.
			send_command(gFrame, idx);
		} else if( ch == 'b' ) {
			gFrame[idx++] = 0xAA;		///< sync pattern
			gFrame[idx++] = 0x01;		///< all switch on.
			send_command(gFrame, idx);
		} else if( ch == 'c' ) {
			gFrame[idx++] = 0xAA;		///< sync pattern
			gFrame[idx++] = 0x02;		///< all switch hold off.
			send_command(gFrame, idx);
		} else if( ch == 'd' ) {
			gFrame[idx++] = 0xAA;		///< sync pattern
			gFrame[idx++] = 0x03;		///< all switch hold on.
			send_command(gFrame, idx);
		} else if( ch == 'e' ) {
			gFrame[idx++] = 0xAA;		///< sync pattern
			gFrame[idx++] = 0x04;		///< all switch release off.
			send_command(gFrame, idx);
		} else if( ch == 'f' ) {
			gFrame[idx++] = 0xAA;		///< sync pattern
			gFrame[idx++] = 0x05;		///< all switch release on.
			send_command(gFrame, idx);
		} else if( ch == 'g' ) {
			gFrame[idx++] = 0xAA;		///< sync pattern
			gFrame[1] = 0x06;		///< all switch force off.
			send_command(gFrame, idx);
		} else if( ch == 'h' ) {
			gFrame[idx++] = 0xAA;		///< sync pattern
			gFrame[idx++] = 0x07;		///< all switch force on.
			send_command(gFrame, idx);
		} else if( ch == 'i' ) {
			gFrame[idx++] = 0xAA;
			gFrame[idx++] = 0x0B;		///< single switch hold on.
			gFrame[idx++] = rand() % MAX_SWITCH;
			printf((char *)"All chip %d-switch hold on!\n", gFrame[idx-1]);
			send_command(gFrame, idx);
		} else if( ch == 'j' ) {
			printf((char *)"%s() : not support!\n", __func__);
		}
		emulator_switch_dump(0xFF);
		switch_dump(0xFF);
		all_switch_status_check();
	} while( ch != ESC );
}

uint8_t switch_result(uint8_t cmd, uint8_t status)
{
	uint8_t ret = status;

	cmd = cmd & 0x7;
	if( cmd == 0 ) {
		if( (status & (1<<1)) == 0 ) {	///< previous status : HOLD.
			ret = (0<<1) | (0<<0);
		}
	} else if( cmd == 1 ) {
		if( (status & (1<<1)) == 0 ) {	///< previous status : HOLD.
			ret = (0<<1) | (1<<0);
		}
	} else if( cmd == 2 ) {
		if( (status & (1<<1)) == 0 ) {	///< previous status : HOLD.
			ret = (1<<1) | (0<<0);
		}
	} else if( cmd == 3 ) {
		if( (status & (1<<1)) == 0 ) {	///< previous status : HOLD.
			ret = (1<<1) | (1<<0);
		}
	} else if( cmd == 4 ) {			///< RELEASE OFF
		if( status & (1<<1) ) {		///< previous status : HOLD.
			ret = (0<<1) | (0<<0);
		}
	} else if( cmd == 5 ) {			///< RELEASE ON
		if( status & (1<<1) ) {		///< previous status : HOLD.
			ret = (0<<1) | (1<<0);
		}
	} else if( cmd == 6 ) {			///< FORCE OFF
		ret = (0<<1) | (0<<0);
	} else if( cmd == 7 ) {			///< FORCE ON
		ret = (0<<1) | (1<<0);
	}

	return ret;
}



/**
 * @brief Emulator와 mSwitch에 대해서 모든 switch의 onoff/hold 상태를 check한다.
 *
 * @return 
 */
bool all_switch_status_check(void)
{
	uint8_t chip, sw;
	uint8_t error = 0;
	bool flag = Debug.gTransmit;

	Debug.gTransmit = 0;
	for(chip=0; chip<MAX_CHIP; chip++) {
		///< mSwitch ALL ONOFF status read.
		mSW_ALL_SWITCH_ONOFF_READ(chip);
#if 0
		printf((char *)"Simulator ONOFF Status : ");
		for(sw=0; sw<MAX_BYTES; sw++) printf((char *)"0x%02X ", gStatus[sw]);
		putchar('\n');
#endif
		///< compare status.
		for(sw=0; sw<MAX_BYTES; sw++) {
			if( gFrame[3+sw] != gStatus[sw] ) {
				error = 1;
				ERROR("%d-chip, %d-switch, onoff(0x%X : 0x%X)\n", chip, sw, gFrame[3+sw], gStatus[sw]);
			}
		}

		///< mSwitch ALL HOLD status read.
		mSW_ALL_SWITCH_HOLD_READ(chip);
#if 0
		printf((char *)"Simulator HOLD Status  : ");
		for(sw=0; sw<MAX_BYTES; sw++) printf((char *)"0x%02X ", gStatus[sw]);
		putchar('\n');
#endif
		///< compare status.
		for(sw=0; sw<MAX_BYTES; sw++) {
			if( gFrame[3+sw] != gStatus[sw] ) {
				error = 1;
				ERROR("%d-chip, %d-switch, hold(0x%X : 0x%X)\n", chip, sw, gFrame[3+sw], gStatus[sw]);
				dump((uint8_t *)&eSW[0], 0);
			}
		}
	}
	Debug.gTransmit = flag;

	return error;
}




const tMenuList Switch_Operation[] = {
	"Switch OFF",
	"Switch ON",
	"Switch Hold OFF",
	"Switch Hold ON",
	"Switch Release OFF",
	"Switch Release ON",
	"Switch Force OFF",
	"Switch Force ON"
};

uint8_t Single_Command_Test(uint8_t arg)
{
	uint8_t ch, i, idx, chip;
	bool flag = Debug.gTransmit;

	Debug.gTransmit = 0;
	chip = 0;
	do {
		printf((char *)"\n-------------- %s -----------------\n", __func__);
		for(i=0; i<(sizeof(Switch_Operation)/2); i++) {
			printf((char *)" %d : %s\n", i, Switch_Operation[i].str);
		}
		printf((char *)" %d : Input Chip Select Value.\n", i++);
		printf((char *)" %d : Single Command Long-Run Test.\n", i++);
		printf((char *)"-------------------- chip(0x%X) -----------------------\n", chip);
		printf((char *)" sub-menu select ? ");

		ch = getchar();

//		i = (isdigit(ch)?(ch - '0'):(toupper(ch) - 'A' + 10));
		if( !isxdigit(ch) ) continue;
		putchar('\n');

		if( (ch >= '0') && (ch <= '7') ) {
			uint8_t opcode = ch - '0';
			idx = 0;

			gFrame[idx++] = 0xAA;
			if( chip != 0 ) {					///< check using chip select.
				gFrame[idx++] = 0x40 | opcode;
				gFrame[idx++] = chip;			// switch number.
			} else {
				gFrame[idx++] = opcode;
			}
			Debug.gTransmit = 1;
			send_command(gFrame, idx);
			Debug.gTransmit = 0;

//			dump((uint8_t *)&sSW[0], 0);
			switch_dump(0xFF);
			emulator_switch_dump(0xFF);
		} else if( ch == '8' ) {
			printf((char *)"\nnew chip select(0x0 ~ 0xFF) ? ");
			chip = getnum();
		} else if( ch == '9' ) {
			uint8_t cmd;
			///< all chip all switch hold/release on/off test.
//			gFrame[0] = 0x0;			///< all switch off.
//			send_command(gFrame, 1);
			mSW_ALL_FORCE_OFF();
			all_switch_status_check();

			Debug.gTransmit = 1;
			do {
				idx = 0;

				gFrame[idx++] = 0xAA;

				cmd = rand() & 0x47;		///< single command generate.
				gFrame[idx++] = cmd;
				if( cmd & 0x40 ) {			///< using Chip Select
					chip = rand() & 0xFF;
					gFrame[idx++] = chip;
				}
				printf((char *)"Send %s command ... ", Switch_Operation[cmd & 0x7].str);
				send_command(gFrame, idx);

				///< check switch status.
				if( all_switch_status_check() ) printf((char *)"Error!!!\n");
				else printf((char *)"OK!!!\n");
#if 0
				for(i=0; i<MAX_CHIP; i++) {
					uint8_t sw;
					if( cmd & 0x40 ) {
						if( (chip & (1<<(7-i))) == 0 ) continue;
					}

					cStatus[i][0] = switch_result(cmd & 0x7, cStatus[i][0]);
					for(sw=0; sw<MAX_SWITCH; sw++) {
						if( emul_get_switch(i, sw) != cStatus[i][sw] ) {
							printf((char *)"\nERROR : %d-chip, %d-switch -> 0x%x:0x%x", i, sw, emul_get_switch(i, sw), cStatus[i]);
						}
					}
				}
				printf((char *)"OK(0x%x)\n", cStatus[0]);
#endif
			} while( getkey() != ESC );
		}
	} while( ch != ESC );
	Debug.gTransmit = flag;

	return 0;
}

const tROM_STRING sw_status[] = {
	"Switch OFF",
	"Switch ON",
	"Switch HOLD OFF",
	"Switch HOLD ON"
};

uint8_t Single_Switch_Command_Test(uint8_t arg)
{
	uint8_t ch, i, idx, opcode, chip, sw;//, status;
	bool flag = Debug.gTransmit;

	chip = 0;
	sw = 0;
	Debug.gTransmit = 0;
	do {
		for(i=0; i<MAX_FRAME; i++) gFrame[i] = 0;

		printf((char *)"\n-------------- %s -----------------\n", __func__);
		for(i=0; i<(sizeof(Switch_Operation)/2); i++) {
			printf((char *)" %X : %s\n", i, Switch_Operation[i].str);
		}
		printf((char *)" %X : Input Chip Select Value.\n", i++);
		printf((char *)" %X : Input Switch Number.\n", i++);
		printf((char *)" %X : Single Switch Command Long-Run Test.\n", i++);
		puts((char *)"-------------------------------------------------------");
		printf((char *)" sub-menu select ? ");

		ch = getchar();

		if( !isxdigit(ch) ) continue;

		putchar('\n');
		if( (ch >= '0') && (ch <= '7') ) {
			idx = 0;

			gFrame[idx++] = 0xAA;

			opcode = (1<<3) | (ch - '0');
			if( chip != 0 ) {					///< check using chip select.
				gFrame[idx++] = (1<<6) | opcode;
				gFrame[idx++] = chip;			///< chip select.
			} else {
				gFrame[idx++] = opcode;
			}
			gFrame[idx++] = sw;
			send_command(gFrame, idx);

			for(i=0; i<MAX_CHIP; i++) {
				uint8_t k = get_row_number(i, sw);

				printf((char *)"\t%d-chip, %d-switch : SWITCH ", i, sw);
#if 0   // 2021.02.03
				mSW_COLUMN_SWITCH_HOLD_READ(i, get_column_number(i, sw));
				if( check_bit(k, &gFrame[5]) ) {
					printf((char *)"HOLD ");
				}
				mSW_COLUMN_SWITCH_ONOFF_READ(i, get_column_number(i, sw));
				if( check_bit(k, &gFrame[5]) ) {
					printf((char *)"ON");
				} else printf((char *)"OFF");
				putchar('\n');
#endif

//				status = emul_get_switch(i, sw);
//				printf((char *)"%d-chip, %d-switch : %s.\n", i, sw, sw_status[status].str);
			}
//			switch_dump(0xFF);
//			emulator_switch_dump(0xFF);
		} else if( ch == '8' ) {
			printf((char *)"\nnew chip select(0x0 ~ 0xFF) ? ");
			chip = getnum();
		} else if( ch == '9' ) {
			printf((char *)"\nnew switch number(0 ~ %d) ? ", MAX_SWITCH-1);
			sw = getnum();
		} else if( toupper(ch) == 'A' ) {
			uint8_t cmd;

			///< all chip all switch hold/release on/off test.
			mSW_ALL_FORCE_OFF();
			all_switch_status_check();

			do {
				idx = 0;

				gFrame[idx++] = 0xAA;

				cmd = rand() & 0x47;
				gFrame[idx++] = cmd;
				if( cmd & 0x40 ) {			///< using Chip Select
					chip = rand() & 0xFF;
					gFrame[idx++] = chip;
				}

				sw = rand() % MAX_SWITCH;
				gFrame[idx++] = sw;
				printf((char *)"Send %d-%s command ... ", sw, Switch_Operation[cmd & 0x7].str);
				send_command(gFrame, idx);

				///< check switch status.
				if( all_switch_status_check() ) printf((char *)"Error!!!\n");
				else printf((char *)"OK!!!\n");
#if 0
				for(i=0; i<MAX_CHIP; i++) {
					if( cmd & 0x40 ) {			///< using Chip Select
						if( (chip & (1<<(7-i))) == 0 ) continue;
					}

					cStatus[i][0] = switch_result(gFrame[0] & 0x7, cStatus[i][0]);
					if( emul_get_switch(i, sw) != cStatus[i][0] ) {
						printf((char *)"\nERROR : %d-chip, %d-switch -> 0x%x:0x%x", i, sw, emul_get_switch(i, sw), cStatus[i][0]);
					}
				}
				printf((char *)"OK(0x%x)\n", cStatus[0][0]);
#endif
			} while( getkey() != ESC );
		}
	} while( ch != ESC );

	Debug.gTransmit = flag;

	return 0;
}

const uint16_t short_mask[] = {0, 0x1, 0x3, 0x7, 0xF, 0x1F, 0x3F, 0x7F, 0xFF, 0x1FF, 0x3FF, 0x7FF, 0xFFF, 0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF};
const uint32_t long_mask[] = {
	0, 0x1, 0x3, 0x7, 0xF, 0x1F, 0x3F, 0x7F, 0xFF,
	0x1FF, 0x3FF, 0x7FF, 0xFFF, 0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF,
	0x1FFFF, 0x3FFFF, 0x7FFFF, 0xFFFFF, 0x1FFFFF, 0x3FFFFF, 0x7FFFFF, 0xFFFFFF,
	0x1FFFFFF, 0x3FFFFFF, 0x7FFFFFF, 0xFFFFFFF, 0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF,
};

uint8_t Row_All_Switch_Command_Test(uint8_t arg)
{
	uint8_t i, idx, opcode, chip, ch;
	uint16_t row;
	bool flag = Debug.gTransmit;

	chip = 0;
	row = 0;
	Debug.gTransmit = 0;
	do {
		for(i=0; i<MAX_FRAME; i++) gFrame[i] = 0;

		printf((char *)"\n------------ %s -------------\n", __func__);
		for(i=0; i<(sizeof(Switch_Operation)/2); i++) {
			printf((char *)" %X : %s\n", i, Switch_Operation[i].str);
		}
		printf((char *)" %X : Input Chip Select Value.\n", i++);
		printf((char *)" %X : Input New Row Select Value.\n", i++);
		printf((char *)" %X : Row All Switch Command Long-Run Test.\n", i++);
		printf((char *)"------------- chip(0x%X), row(0x%X) ---------------\n", chip, row);
		printf((char *)" sub-menu select ? ");

		ch = getchar();

		if( !isxdigit(ch) ) continue;

		if( (ch >= '0') && (ch <= '7') ) {
			uint8_t eRow = get_row_count(0);
//			uint8_t status;
			idx = 0;
			for(i=0; i<MAX_FRAME; i++) gFrame[i] = 0;

			gFrame[idx++] = 0xAA;

			opcode = (2<<3) | (ch - '0');
			if( chip != 0 ) {					///< check using chip select.
				gFrame[idx++] = 0x40 | opcode;
				gFrame[idx++] = chip;			// switch number.
			} else {
				gFrame[idx++] = opcode;
			}

			for(i=0; i<eRow; i++) {
				if( row & (1 << (eRow - (i + 1))) ) {
					set_bit(i, &gFrame[idx]);
				}
			}
			idx += (eRow >> 3) + ((eRow & 0x7)?1:0);
			send_command(gFrame, idx);

			switch_dump(0xFF);
//			emulator_switch_dump(0xFF);
		} else if( ch == '8' ) {
			printf((char *)"\nnew chip select(0x0 ~ 0x%X) ? ", CHIP_MASK);
			chip = getnum();
		} else if( ch == '9' ) {
			printf((char *)"\nnew row select(0x0 ~ 0x%X) ? ", short_mask[get_row_count(0)]);
			row = getnum();
		} else if( toupper(ch) == 'A' ) {
//			uint8_t *pRow;
			uint8_t cmd;
			uint8_t nRow = get_row_count(0);
			uint8_t nSw = get_switch_count_in_row(0);

			///< all chip all switch hold/release on/off test.
			mSW_ALL_FORCE_OFF();
			all_switch_status_check();

			putchar('\n');
			do {
				for(i=0; i<MAX_FRAME; i++) gFrame[i] = 0;

				idx = 0;
				gFrame[idx++] = 0xAA;

				///< create command gFrame.
				cmd = (rand() & 0x47) | (2<<3);
				gFrame[idx++] = cmd;
				if( cmd & 0x40 ) {			///< using Chip Select
					chip = rand() & 0xFF;
					gFrame[idx++] = chip;
				}

//				pRow = &gFrame[idx];
				for(i=0; i<nRow; i++) {
					if( rand() & 0x1 ) set_bit(i, &gFrame[idx]);
				}
				idx += (nRow / 8) + ((nRow & 0x7)?1:0);

				///< send command gFrame.
				if( cmd & 0x40 ) {
					printf((char *)"Send Chip Select[0x%X], Row Select[0x%X%X], %s command ... ", chip, gFrame[3], gFrame[4], Switch_Operation[cmd & 0x7].str);
				} else {
					printf((char *)"Send All Chip Row Select[0x%X%X], %s command ... ", gFrame[2], gFrame[3], Switch_Operation[cmd & 0x7].str);
				}
				send_command(gFrame, idx);

				///< check switch status.
				if( all_switch_status_check() ) printf((char *)"Error!!!\n");
				else printf((char *)"OK!!!\n");

//				switch_dump(0xFF);
//				emulator_switch_dump(0xFF);
			} while( getkey() != ESC );
		}
	} while( ch != ESC );

	Debug.gTransmit = flag;

	return 0;
}

uint8_t Row_Each_Switch_Command_Test(uint8_t arg)
{
	uint8_t ch, i, idx, opcode, chip;
	uint32_t row, sw;
	uint8_t nRow, nSw, bytes;
	bool flag = Debug.gTransmit;

	printf((char *)"%s() : \n", __func__);

	nRow = get_row_count(0);
	nSw = get_switch_count_in_row(0);
	i = nRow + nSw;
	bytes = (i >> 3) + ((i & 0x7)?1:0);

	chip = 0;
	row = 0x5C;
	sw = 0x30F;
	Debug.gTransmit = 0;
	do {
		printf((char *)"\n-------------- %s -----------------\n", __func__);
		for(i=0; i<(sizeof(Switch_Operation)/2); i++) {
			printf((char *)" %X : %s\n", i, Switch_Operation[i].str);
		}
		printf((char *)" %X : Input Chip Select Value.\n", i++);
		printf((char *)" %X : Input New Row Select Value.\n", i++);
		printf((char *)" %X : Input New Switch Select Value.\n", i++);
		printf((char *)" %X : Row Each Switch Command Long-Run Test.\n", i++);
		printf((char *)"------------ chip(0x%X), row(0x%lX), switch(0x%lX) -------------\n", chip, row, sw);
		printf((char *)" sub-menu select ? ");

		ch = getchar();

		if( !isxdigit(ch) ) continue;

		putchar('\n');
		if( (ch >= '0') && (ch <= '7') ) {

			idx = 0;
			for(i=0; i<MAX_FRAME; i++) gFrame[i] = 0;

			gFrame[idx++] = 0xAA;

			opcode = (3<<3) | (ch - '0');
			if( chip != 0 ) {
				gFrame[idx++] = 0x40 | opcode;
				gFrame[idx++] = chip;			// switch number.
				printf((char *)"command(0x%X), chip select(0x%X)", gFrame[1], chip);
			} else {
				gFrame[idx++] = opcode;
				printf((char *)"command(0x%X)", gFrame[1]);
			}

			printf((char *)", row select(");
			for(i=0; i<nRow; i++) {
				if( row & (1 << (nRow - (i + 1))) ) {
					set_bit(i, &gFrame[idx]);
					putchar('1');
				} else {
					putchar('0');
				}
			}

			printf((char *)"), switch select(");
			for(i=0; i<nSw; i++) {
				if( sw & (1 << (nSw - (i + 1))) ) {
					set_bit(i+nRow, &gFrame[idx]);
					putchar('1');
				} else {
					putchar('0');
				}
			}
			printf((char *)") ... ");
			idx += bytes;
			send_command(gFrame, idx);

			switch_dump(0xFF);
//			emulator_switch_dump(0xFF);
		} else if( ch == '8' ) {
			printf((char *)"new chip select(0x0 ~ 0x%X) ? ", CHIP_MASK);
			chip = getnum();
		} else if( ch == '9' ) {
			printf((char *)"new row select(0x0 ~ 0x%X) ? ", short_mask[get_row_count(0)]);
			row = getnum();
		} else if( toupper(ch) == 'A' ) {
			printf((char *)"new switch select(0x0 ~ 0x%lX) ? ", long_mask[get_switch_count_in_row(0)]);
			sw = getnum();
		} else if( toupper(ch) == 'B' ) {
			uint8_t cmd;

			///< all chip all switch hold/release on/off test.
			mSW_ALL_FORCE_OFF();
			for(i=0; i<MAX_BYTES; i++) gStatus[i] = 0x0;

			do {
				idx = 0;
				for(i=0; i<MAX_FRAME; i++) gFrame[i] = 0;

				gFrame[idx++] = 0xAA;

				///< create command gFrame.
				cmd = (rand() & 0x47) | (3<<3);
				gFrame[idx++] = cmd;
				printf((char *)"command(0x%X)", cmd);
				if( cmd & 0x40 ) {			///< using Chip Select
					chip = rand() & 0xFF;
					gFrame[idx++] = chip;
					printf((char *)", chip(0x%X)", chip);
				}

				printf((char *)", row(");
				for(i=0; i<nRow; i++) {
					if( rand() & 0x1 ) {
						set_bit(i, &gFrame[idx]);
						putchar('1');
					} else {
						putchar('0');
					}
				}
				printf((char *)"), switch(");
				for(i=0; i<nSw; i++) {
					if( rand() & 0x1 ) {
						set_bit(i+nRow, &gFrame[idx]);
						putchar('1');
					} else {
						putchar('0');
					}
				}
				idx += bytes;

				printf((char *)") %s command ... ", Switch_Operation[cmd & 0x7].str);
				///< send command frame.
				send_command(gFrame, idx);

//				switch_dump(0xFF);
//				emulator_switch_dump(0xFF);

				///< check switch status.
				if( all_switch_status_check() ) printf((char *)"Error!!!\n");
				else printf((char *)"OK!!!\n");

			} while( getkey() != ESC );
		}
	} while( ch != ESC );

	Debug.gTransmit = flag;

	return 0;
}

uint8_t Column_All_Switch_Command_Test(uint8_t arg)
{
	uint8_t i, idx, opcode, chip, ch, nColumn;
	uint32_t mask;
	uint32_t column;
	bool flag = Debug.gTransmit;

	chip = 0;
	mask = long_mask[get_switch_count_in_column(0)];
	column = 0x5A5A & mask;
	Debug.gTransmit = 0;
	do {
		printf((char *)"\n-------------- %s -----------------\n", __func__);
		for(i=0; i<(sizeof(Switch_Operation)/2); i++) {
			printf((char *)" %X : %s\n", i, Switch_Operation[i].str);
		}
		printf((char *)" %X : Input Chip Select Value.\n", i++);
		printf((char *)" %X : Input New Column Select Value.\n", i++);
		printf((char *)" %X : Column All Switch Command Long-Run Test.\n", i++);
		printf((char *)"--------------- chip(0x%X), column(0x%lX) ----------------\n", chip, column);
		printf((char *)" sub-menu select ? ");

		ch = getchar();

		if( !isxdigit(ch) ) continue;

		putchar('\n');
		if( (ch >= '0') && (ch <= '7') ) {
			idx = 0;
			for(i=0; i<MAX_FRAME; i++) gFrame[i] = 0;

			gFrame[idx++] = 0xAA;

			opcode = (4<<3) | (ch - '0');
			if( chip != 0 ) {
				gFrame[idx++] = 0x40 | opcode;
				gFrame[idx++] = chip;			// switch number.
				printf((char *)"command(0x%X), chip select(0x%X)", gFrame[1], chip);
			} else {
				gFrame[idx++] = opcode;
				printf((char *)"command(0x%X)", gFrame[1]);
			}
			printf((char *)", column select(");
			nColumn = get_column_count(0);
			for(i=0; i<nColumn; i++) {
				if( column & (1 << (nColumn - (i + 1))) ) {
					set_bit(i, &gFrame[idx]);
					putchar('1');
				} else {
					putchar('0');
				}
			}
			printf((char *)") send.\n");
			idx += (nColumn >> 3) + ((nColumn & 0x7)?1:0);
			send_command(gFrame, idx);

			switch_dump(0xFF);
			emulator_switch_dump(0xFF);
		} else if( ch == '8' ) {
			printf((char *)"new chip select(0x0 ~ 0x%X) ? ", CHIP_MASK);
			chip = getnum();
		} else if( ch == '9' ) {
			printf((char *)"\nnew column select(0x0 ~ 0x%lX) ? ", long_mask[get_switch_count_in_column(0)]);
			column = getnum();
		} else if( toupper(ch) == 'A' ) {
			uint8_t cmd;//, nSw;
			uint8_t *pColumn;

			///< all chip all switch hold/release on/off test.
			mSW_ALL_FORCE_OFF();
			for(i=0; i<MAX_BYTES; i++) gStatus[i] = 0x0;

			nColumn = get_column_count(0);
//			nSw = get_switch_count_in_column(0);
			do {
				idx = 0;
				for(i=0; i<MAX_BYTES; i++) gFrame[i] = 0;

				gFrame[idx++] = 0xAA;

				///< create command gFrame.
				cmd = (rand() & 0x47) | (4<<3);
				gFrame[idx++] = cmd;
				printf((char *)"command(0x%X)", cmd);
				if( cmd & 0x40 ) {			///< using Chip Select
					chip = rand() & 0xFF;
					gFrame[idx++] = chip;
					printf((char *)", chip(0x%X)", chip);
				}

				pColumn = &gFrame[idx];
				printf((char *)", column(");
				for(i=0; i<nColumn; i++) {
					if( rand() & 0x1 ) {
						set_bit(i, pColumn);
						putchar('1');
					} else {
						putchar('0');
					}
				}
				printf((char *)")");
				idx += (nColumn / 8) + ((nColumn & 0x7)?1:0);

				///< send command frame.
				printf((char *)" %s command ... ", Switch_Operation[cmd & 0x7].str);
				send_command(gFrame, idx);

//				emulator_switch_dump(0xFF);
				///< check switch status.
				if( all_switch_status_check() ) printf((char *)"Error!!!\n");
				else printf((char *)"OK!!!\n");
			} while( getkey() != ESC );
		}
	} while( ch != ESC );

	Debug.gTransmit = flag;

	return 0;
}

uint8_t Column_Each_Switch_Command_Test(uint8_t arg)
{
	uint8_t i;
	uint8_t idx, opcode, chip, ch;
	uint8_t nColumn, nSw, bytes;
	uint32_t column, sw;
	bool flag = Debug.gTransmit;

	nColumn = get_column_count(0);
	nSw = get_switch_count_in_column(0);
	i = nColumn + nSw;
	bytes = (i >> 3) + ((i & 0x7)?1:0);

	chip = 0;
	column = rand() & long_mask[nColumn];
	sw = rand() & long_mask[nSw];

	Debug.gTransmit = 0;
	printf((char *)"%s() : nColumn(%d), nRow(%d)\n", __func__, nColumn, nSw);
	do {
		printf((char *)"\n-------------- %s -----------------\n", __func__);
		for(i=0; i<(sizeof(Switch_Operation)/2); i++) {
			printf((char *)" %X : %s\n", i, Switch_Operation[i].str);
		}
		printf((char *)" %X : Input Chip Select Value.\n", i++);
		printf((char *)" %X : Input New Column Select Value.\n", i++);
		printf((char *)" %X : Input New Switch Select Value.\n", i++);
		printf((char *)" %X : Column Each Switch Command Long-Run Test.\n", i++);
		printf((char *)"---------- chip(0x%X), column(0x%lX), switch(0x%lX) -----------\n", chip, column, sw);
		printf((char *)" sub-menu select ? ");

		ch = getchar();

		if( !isxdigit(ch) ) continue;

		putchar('\n');
		if( (ch >= '0') && (ch <= '7') ) {
			idx = 0;
			for(i=0; i<MAX_FRAME; i++) gFrame[i] = 0x0;

			gFrame[idx++] = 0xAA;

			opcode = (5<<3) | (ch - '0');
			if( chip != 0 ) {
				gFrame[idx++] = 0x40 | opcode;
				gFrame[idx++] = chip;			// switch number.
			} else {
				gFrame[idx++] = opcode;
			}

			for(i=0; i<nColumn; i++) {
				if( column & (1 << (nColumn - (i + 1))) ) {
					set_bit(i, &gFrame[idx]);
				}
			}

			for(i=0; i<nSw; i++) {
				if( sw & (1 << (nSw - (i + 1))) ) {
					set_bit(i+nColumn, &gFrame[idx]);
				}
			}
			idx += bytes;
			send_command(gFrame, idx);

			switch_dump(0xFF);
//			emulator_switch_dump(0xFF);
		} else if( ch == '8' ) {
			printf((char *)"new chip select(0x0 ~ 0x%X) ? ", CHIP_MASK);
			chip = getnum();
		} else if( ch == '9' ) {
			printf((char *)"new column select(0x0 ~ 0x%lX) ? ", long_mask[nColumn]);
			column = getnum();
		} else if( toupper(ch) == 'A' ) {
			printf((char *)"new switch select(0x0 ~ 0x%lX) ? ", long_mask[nSw]);
			sw = getnum();
		} else if( toupper(ch) == 'B' ) {
			uint8_t cmd;
			uint8_t *pColumn;

			///< all chip all switch hold/release on/off test.
			mSW_ALL_FORCE_OFF();
			for(i=0; i<MAX_BYTES; i++) gStatus[i] = 0x0;

//			nColumn = get_column_count(0);
//			nSw = get_switch_count_in_column(0);

			do {
				idx = 0;
				for(i=0; i<MAX_FRAME; i++) gFrame[i] = 0;

				gFrame[idx++] = 0xAA;

				///< create command frame.
				cmd = (rand() & 0x47) | (5<<3);
				gFrame[idx++] = cmd;
				printf((char *)"command(0x%X)", cmd);
				if( cmd & 0x40 ) {			///< using Chip Select
					chip = rand() & 0xFF;
					gFrame[idx++] = chip;
					printf((char *)", chip(0x%X)", chip);
				}

				pColumn = &gFrame[idx];
				printf((char *)", column(");
				for(i=0; i<nColumn; i++) {
					if( rand() & 0x1 ) {
						set_bit(i, pColumn);
						putchar('1');
					} else {
						putchar('0');
					}
				}

				printf((char *)"), switch(");
				for(i=0; i<nColumn; i++) {
					if( rand() & 0x1 ) {
						set_bit(i+nColumn, pColumn);
						putchar('1');
					} else {
						putchar('0');
					}
				}
				putchar(')');
				idx += bytes;

				///< send command frame.
				printf((char *)" %s command ... ", Switch_Operation[cmd & 0x7].str);
				send_command(gFrame, idx);

//				emulator_switch_dump(0xFF);
				///< check switch status.
				if( all_switch_status_check() ) printf((char *)"Error!!!\n");
				else printf((char *)"OK!!!\n");
			} while( getkey() != ESC );
		}
	} while( ch != ESC );

	Debug.gTransmit = flag;

	return 0;
}

uint8_t Switch_Status_Register_Test(void)
{
	uint8_t i, op, sw, chip;

	printf((char *)"enter Multi-Switch Chip Number(0 ~ %d) ? ", MAX_CHIP-1);
	chip = getnum();
	printf((char *)"enter switch number(0 ~ %d) ? ", MAX_SWITCH-1);
	sw = getnum();

	i = 0;
	do {
		op = c_op[i++ & 0x7];
		mSW_SINGLE_SWITCH(op, 1<<(7-chip), &sw);
		printf((char *)" %d-switch : %s\t", sw, Switch_Operation[op].str);

		mSW_FTP_WRITE(chip, SW_NUM, sw);
		mSW_FTP_READ(chip, SW_STATUS);
		printf((char *)"Switch Status Read = 0x%02X(0x%02X)\n\n", gFrame[4], gStatus[0]);
	} while( getchar() != ESC );

	return 0;
}

uint8_t FTP_Register_RW_Test(void)
{
	uint8_t chip, addr, data;

	do {
		printf((char *)"enter Multi-Switch Chip Number(0 ~ %d, %d) ? ", MAX_CHIP-1, MAX_CHIP);
		chip = getnum();
		printf((char *)"enter SFR register address ? ");
		addr = getnum();
		printf((char *)"enter SFR register write data ? ");
		data = getnum();

		if( chip == MAX_CHIP ) {
			mSW_ALL_FTP_WRITE(addr, data);

			for(chip=0; chip<MAX_CHIP; chip++) {
				data = register_read(chip, addr);
				printf((char *)"%d-chip : 0x%02X SFR value = 0x%02X(0x%02X)\n\n", chip, addr, data, gStatus[0]);
			}
		} else {
			mSW_FTP_WRITE(chip, addr, data);

			data = register_read(chip, addr);
			printf((char *)"0x%02X SFR value = 0x%02X(0x%02X)\n\n", addr, data, gStatus[0]);
		}
	} while( getchar() != ESC );

	return 0;
}


uint8_t Special_Command_Test(uint8_t arg)
{
	uint8_t i, ch;

	printf((char *)"%s() : \n", __func__);

	do {
		puts((char *)" 1. Chip Address Assignment.");
		puts((char *)" 2. Group Type Select.");
		puts((char *)" 3. Pattern Reset Test.");
		puts((char *)" 4. Software Reset Test.");
		puts((char *)"...");
		puts((char *)" 8. Switch Status Register Test.");
		puts((char *)" 9. FTP Register Read/Write Test.");

		ch = getchar();

		if( !isxdigit(ch) ) continue;
		putchar('\n');

		if( ch == '1' ) {
			mSW_PATTERN_RESET();

			mSW_CHIP_ADDRESS_ASSIGN(0xFF);
		} else if( ch == '2' ) {
			printf((char *)"enter Multi-Switch Group Type(0 ~ 7) ? ");
			i = getnum();

			mSW_SET_GROUP_TYPE(i);
			mSW_ALL_FTP_WRITE(FTPD0_ADDR, i);

			printf((char *)"Group Type : ");
			for(i=0; i<MAX_CHIP; i++) {
				printf((char *)"0x%02X ", register_read(i, FTPD0_ADDR));
			}
			putchar('\n');
		} else if( ch == '3' ) {
//			mSW_PATTERN_RESET();
			for(i=0; i<MAX_FRAME; i++) gFrame[i] = 0xFF;
			for(; i<MAX_FRAME+3; i++) gFrame[i] = 0x00;
			send_command(gFrame, MAX_FRAME+3);
		} else if( ch == '4' ) {
		} else if( ch == '8' ) {
			Switch_Status_Register_Test();
		} else if( ch == '9' ) {
			FTP_Register_RW_Test();
		}
	} while( ch != ESC );

	return 0;
}

uint8_t Each_Switch_Command_Test(uint8_t arg)
{
	uint8_t i, ch;
	uint8_t idx, opcode, chip, sw[MAX_BYTES];
	bool flag = Debug.gTransmit;

	printf((char *)"%s() : \n", __func__);

	chip = 0;
	Debug.gTransmit = 0;
	for(i=0; i<MAX_BYTES; i++) sw[i] = rand() & 0xFF;
	do {
		printf((char *)"\n-------------- %s -----------------\n", __func__);
		for(i=0; i<(sizeof(Switch_Operation)/2); i++) {
			printf((char *)" %X : %s\n", i, Switch_Operation[i].str);
		}
		printf((char *)" %X : Input Chip Select Value.\n", i++);
		printf((char *)" %X : Input New Switch Select Value.\n", i++);
		printf((char *)" %X : Each Switch Control Command Long-Run Test.\n", i++);
		puts((char *)"-------------------------------------------------------");
		printf((char *)" sub-menu select ? ");

		ch = getchar();

		if( !isxdigit(ch) ) continue;

		putchar('\n');
		if( (ch >= '0') && (ch <= '7') ) {
			idx = 0;

			Debug.gTransmit = 1;
			gFrame[idx++] = 0xAA;

			opcode = (7<<3) | (ch - '0');
			if( chip != 0 ) {
				gFrame[idx++] = (1<<6) | opcode;
				gFrame[idx++] = chip;			// switch number.
			} else {
				gFrame[idx++] = opcode;
			}
			for(i=0; i<MAX_BYTES; i++) gFrame[idx++] = sw[i];
			send_command(gFrame, idx);
			Debug.gTransmit = 0;

			switch_dump(0xFF);
//			emulator_switch_dump(0xFF);
		} else if( ch == '8' ) {
			printf((char *)"new chip select(0x0 ~ 0x%X) ? ", CHIP_MASK);
			chip = getnum();
		} else if( ch == '9' ) {
#if 1
			for(i=0; i<MAX_BYTES; i++) {
				printf((char *)"\t%d-%d switch select(0x0 ~ 0xFF) ? ", i*8, (i*8)+7);
				sw[i] = getnum();
				putchar('\n');
			}
#else
			sw[0] = 0xF0;
			for(i=1; i<MAX_BYTES; i++) sw[i] = 0x0;
#endif
		} else if( toupper(ch) == 'A' ) {
			uint8_t cmd;

			///< all chip all switch hold/release on/off test.
			mSW_ALL_FORCE_OFF();
			for(i=0; i<MAX_BYTES; i++) gStatus[i] = 0x0;

			do {
				// create command frame.
				idx = 0;

				gFrame[idx++] = 0xAA;

				cmd = (rand() & 0x47) | (7<<3);
				gFrame[idx++] = cmd;
				printf((char *)"command(0x%X)", cmd);
				if( cmd & (1<<6) ) {
					chip = rand() & 0xFF;
					gFrame[idx++] = chip;
					printf((char *)", chip(0x%X)", chip);
				}

				printf((char *)", switch(0x");
				for(i=0; i<MAX_BYTES; i++) {
					gFrame[idx] = rand() & 0xFF;
					printf((char *)"%02X", gFrame[idx]);
					idx++;
				}
				printf((char *)")");

				// command execute.
				printf((char *)" %s command ... ", Switch_Operation[cmd & 0x7].str);
				send_command(gFrame, idx);

//				emulator_switch_dump(0xFF);
				///< check switch status.
				if( all_switch_status_check() ) printf((char *)"Error!!!\n");
				else printf((char *)"OK!!!\n");
			} while( getkey() != ESC );
		}
	} while( ch != ESC );

	Debug.gTransmit = flag;

	return 0;
}


uint8_t Read_Command_Test(uint8_t arg)
{
	uint8_t ch, i;
	uint8_t idx = 0;

#if 1
	for(i=0; i<MAX_FRAME; i++) gFrame[i] = 0;
	gFrame[idx++] = 0xAA;
	gFrame[idx++] = 0x7B;	// HOLD ON
	gFrame[idx++] = rand() & 0xFF;	// chip select
	for(i=0; i<15; i++) gFrame[idx++] = rand() & 0xFF;
	send_command(gFrame, idx);
	emulator_switch_dump(0xFF);
#endif

	do {
		idx = 0;

		printf((char *)"\n-------------- %s -----------------\n", __func__);
		printf((char *)" 1 : FTPD Register Write/Read Command Test.\n");
		printf((char *)" 2 : SFR Register Write/Read Command Test.\n");
		printf((char *)" 3 : FTPD/SFR Register Dump Command Test.\n");
		printf((char *)" 4 : ALL Switch ON/OFF Status Read Command Test.\n");
		printf((char *)" 5 : Column Switch ON/OFF Status Read Command Test.\n");
		printf((char *)" 6 : Row Switch ON/OFF Status Read Command Test.\n");
		printf((char *)" 7 : ALL Switch HOLD Status Read Command Test.\n");
		printf((char *)" 8 : Column Switch HOLD Status Read Command Test.\n");
		printf((char *)" 9 : Row Switch HOLD Status Read Command Test.\n");
		puts((char *)"---------------------------------------------------------");
		printf((char *)" sub-menu select ? ");

		ch = getchar();

		if( !isxdigit(ch) ) continue;

		if( ch == '1' ) {
			uint8_t chip, addr, data;

			printf((char *)"\nEnter chip number ? ");
			chip = getnum();

			printf((char *)"\nEnter FTP register address ? ");
			addr = getnum();

			printf((char *)"\nEnter FTP write data ? ");
			data = getnum();

			mSW_FTP_WRITE(chip, addr, data);

			data = register_read(chip, addr);

			printf((char *)"result : 0x%X(0x%X)\n", data, gStatus[0]);
		} else if( ch == '2' ) {
			uint8_t chip, addr, data;

			printf((char *)"\nEnter chip number ? ");
			chip = getnum();

			printf((char *)"\nEnter SFR register address ? ");
			addr = getnum();

			printf((char *)"\nEnter SFR write data ? ");
			data = getnum();

			mSW_FTP_WRITE(chip, addr, data);

			data = register_read(chip, addr);

			printf((char *)"result : 0x%X(0x%X)\n", data, gStatus[0]);
		} else if( ch == '3' ) {
			uint8_t chip, addr, data;
			bool flag = Debug.gTransmit;

			Debug.gTransmit = 0;
			for(chip=0; chip<MAX_CHIP; chip++) {
				printf((char *)"\n--------------------- %d-chip --------------------\n", chip);
				printf((char *)"FTP : ");
				for(addr=0; addr<4; addr++) {
					data = register_read(chip, addr);
					printf((char *)"0x%02X(0x%02X) ", data, gStatus[0]);
				}
				for(i=0; i<16; i++) {
					if( (i & 0x7) == 0 ) {
						if( i == 0 ) printf((char *)"\nSFR : ");
						else printf((char *)"\n      ");
					}
					data = register_read(chip, SFR_ADDR[i]);
					printf((char *)"0x%02X(0x%02X) ", data, gStatus[0]);
				}
				printf((char *)"\n");
			}
			Debug.gTransmit = flag;
		} else if( (ch == '4') || (ch == '7') ) {	// ALL Switch ONOFF/HOLD status read.
			uint8_t chip, column;
			uint8_t nColumn, nRow;
			uint8_t *result;

			printf((char *)"\nEnter chip number ? ");
			chip = getnum();

			if( ch == '4' ) {
				mSW_ALL_SWITCH_ONOFF_READ(chip);
			} else if( ch == '7' ) {
				mSW_ALL_SWITCH_HOLD_READ(chip);
			}

			result = &gFrame[3];
			///< Simulator status dump.
			printf((char *)"Simulator %d-chip : ", chip);
			for(uint8_t k=0; k<MAX_BYTES; k++) {
				printf((char *)"0x%X ", result[k]);
			}
			printf((char *)"\n");
			nRow = get_row_count(chip);
			nColumn = get_column_count(chip);
			printf((char *)"nRow = %d, nColumn = %d\n", nRow, nColumn);
			printf((char *)"            Simulator <-> Emulator\n");
			for(column=0; column<nColumn; column++) {
				printf((char *)"%2d-column : ", column);
				///< Simulator status dump.
				for(i=0; i<nRow; i++) {
					uint8_t k = (column * nRow) + i;
					if( check_bit(k, result) ) putchar('*');
					else putchar('.');
				}

				printf((char *)"   ");
				///< Emulator status dump.
				for(i=0; i<nRow; i++) {
					uint8_t k = (column * nRow) + i;
					if( check_bit(k, gStatus) ) putchar('*');
					else putchar('.');
				}
				putchar('\n');
			}
		} else if( (ch == '5') || (ch == '8') ) {
			uint8_t column, chip;
			uint8_t nSw;//, nRow;
			uint8_t *result;

			printf((char *)"\nEnter chip number ? ");
			chip = getnum();

			printf((char *)"\nEnter column number ? ");
			column = getnum();

//			nRow = get_row_count(chip);
			nSw = get_switch_count_in_column(chip);

#if 0   // 2021.02.03
			if( ch == '5' ) {
				mSW_COLUMN_SWITCH_ONOFF_READ(chip, column);
			} else if( ch == '8' ) {
				mSW_COLUMN_SWITCH_HOLD_READ(chip, column);
			}
#endif

			printf((char *)"RX : ");
			for(i=0; i<nSw; i++) {
				printf((char *)"0x%X ", gFrame[i]);
			}
			printf((char *)"\n");

			result = &gFrame[5];
			printf((char *)"%2d-column : ", column);
			///< Simulator status dump.
			for(i=0; i<nSw; i++) {
				if( check_bit(i, result) ) putchar('*');
				else putchar('.');
			}

			printf((char *)"   ");
			///< Emulator status dump.
			for(i=0; i<nSw; i++) {
				if( check_bit(i, gStatus) ) putchar('*');
				else putchar('.');
			}
			putchar('\n');
		} else if( (ch == '6') || (ch == '9') ) {
			uint8_t row, chip;
			uint8_t nSw;//, nColumn;
			uint8_t *result;

			printf((char *)"\nEnter chip number ? ");
			chip = getnum();

			printf((char *)"\nEnter row number ? ");
			row = getnum();

//			nColumn = get_row_count(chip);
			nSw = get_switch_count_in_row(chip);

#if 0   // 2021.02.03
			if( ch == '6' ) {
				mSW_ROW_SWITCH_ONOFF_READ(chip, row);
			} else if( ch == '9' ) {
				mSW_ROW_SWITCH_HOLD_READ(chip, row);
			}
#endif

			printf((char *)"RX : ");
			for(i=0; i<nSw; i++) {
				printf((char *)"0x%X ", gFrame[i]);
			}
			printf((char *)"\n");

			result = &gFrame[5];
			printf((char *)"%2d-row : ", row);
			///< Simulator status dump.
			for(i=0; i<nSw; i++) {
				if( check_bit(i, result) ) putchar('*');
				else putchar('.');
			}

			printf((char *)"   ");
			///< Emulator status dump.
			for(i=0; i<nSw; i++) {
				if( check_bit(i, gStatus) ) putchar('*');
				else putchar('.');
			}
			putchar('\n');
		}
	} while( ch != ESC );

	return 0;
}

uint8_t Continuous_Command_Test(uint8_t arg)
{
	uint8_t i, k, n;
	uint8_t cmd, opcode, idx;
	uint8_t sel, cnt, chip;
	uint8_t nSw, nRow, nColumn;
	bool flag = Debug.gTransmit;

	printf((char *)"select test start command type ? ");
	n = getnum();
	for(i=n; i<16; i++) {
		cmd = i;

		///< opcode의 순서를 다시 정해야 한다. 순차적으로 입력되면, switch 상태가 변하지 않는 경우가 많다.
//		for(opcode=0; opcode<8; opcode++) {
		for(k=0; k<MAX_FRAME; k++) cmd_buf[k] = 0;

		cmd_buf[0] = 0xAA;
//		cmd_buf[idx++] = (cmd << 3) | c_op[opcode];

		if( cmd == 0 ) {			///< single command.
			cnt = 2;
			for(opcode=0; opcode<8; opcode++) {
				cmd_buf[1] = (cmd << 3) | c_op[opcode];
				printf((char *)"command(0x%02X)\n", cmd_buf[1]);

				Debug.gTransmit = 1;
				send_command(cmd_buf, cnt);
				Debug.gTransmit = 0;

				if( all_switch_status_check() ) printf((char *)"Error!!!\n");
				else printf((char *)"OK!!!\n");
			}
		} else if( cmd == 1 ) {		///< 1-switch command.
			cnt = 3;
			for(sel=0; sel<MAX_SWITCH; sel++) {
				cmd_buf[2] = sel;
				for(opcode=0; opcode<8; opcode++) {
					cmd_buf[1] = (cmd << 3) | c_op[opcode];
					printf((char *)"command(0x%02X), %d-switch\n", cmd_buf[1], cmd_buf[2]);

					Debug.gTransmit = 1;
					send_command(cmd_buf, cnt);
					Debug.gTransmit = 0;

					if( all_switch_status_check() ) printf((char *)"Error!!!\n");
					else printf((char *)"OK!!!\n");
				}
			}
		} else if( cmd == 2 ) {		///< all switch in row command.
			nRow = get_row_count(0);
			cnt = 2 + ((nRow >> 3) + ((nRow & 0x7) ? 1 : 0));
			printf((char *)"nRow = %d\n", nRow);

			// 1-bit only select.
			for(sel=0; sel<nRow; sel++) {
				set_bit(sel, &cmd_buf[2]);

				for(opcode=0; opcode<8; opcode++) {
					cmd_buf[1] = (cmd << 3) | c_op[opcode];
					printf((char *)"command(0x%02X), sRow(0x%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3]);

					Debug.gTransmit = 1;
					send_command(cmd_buf, cnt);
					Debug.gTransmit = 0;

					if( all_switch_status_check() ) printf((char *)"Error!!!\n");
					else printf((char *)"OK!!!\n");
				}

				for(k=1; k<cnt; k++) cmd_buf[k] = 0x0;
			}

			// multi-bit select.
			for(sel=0; sel<nRow; sel++) {
				set_bit(sel, &cmd_buf[2]);
				for(opcode=0; opcode<8; opcode++) {
					cmd_buf[1] = (cmd << 3) | c_op[opcode];
					printf((char *)"command(0x%02X), sRow(0x%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3]);

					Debug.gTransmit = 1;
					send_command(cmd_buf, cnt);
					Debug.gTransmit = 0;

					if( all_switch_status_check() ) printf((char *)"Error!!!\n");
					else printf((char *)"OK!!!\n");
				}
			}
		} else if( cmd == 3 ) {		///< each switch in row command.
			nRow = get_row_count(0);
			nSw = get_switch_count_in_row(0);
			idx = nRow + nSw;
			cnt = 2 + ((idx >> 3) + ((idx & 0x7) ? 1 : 0));
			printf((char *)"nRow = %d, nSw = %d\n", nRow, nSw);

			// 1-bit only select.
			for(sel=0; sel<nRow; sel++) {
				set_bit(sel, &cmd_buf[2]);
				for(k=0; k<nSw; k++) {
					set_bit(k+nRow, &cmd_buf[2]);
					for(opcode=0; opcode<8; opcode++) {
						cmd_buf[1] = (cmd << 3) | c_op[opcode];
						printf((char *)"command(0x%02X), sRow+sSW(0x%02X%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3], cmd_buf[4]);

						Debug.gTransmit = 1;
						send_command(cmd_buf, cnt);
						Debug.gTransmit = 0;

						if( all_switch_status_check() ) printf((char *)"Error!!!\n");
						else printf((char *)"OK!!!\n");
					}
					clear_bit(k+nRow, &cmd_buf[2]);
				}
				for(k=1; k<cnt; k++) cmd_buf[k] = 0x0;
			}

			// multi-bit select.
			for(sel=0; sel<nRow; sel++) {
				set_bit(sel, &cmd_buf[2]);
				for(k=0; k<nSw; k++) {
					set_bit(k+nRow, &cmd_buf[2]);
					for(opcode=0; opcode<8; opcode++) {
						cmd_buf[1] = (cmd << 3) | c_op[opcode];
						printf((char *)"command(0x%02X), sRow+sSW(0x%02X%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3], cmd_buf[4]);

						Debug.gTransmit = 1;
						send_command(cmd_buf, cnt);
						Debug.gTransmit = 0;

						if( all_switch_status_check() ) printf((char *)"Error!!!\n");
						else printf((char *)"OK!!!\n");
					}
				}
				for(k=0; k<nSw; k++) {
					clear_bit(k+nRow, &cmd_buf[2]);
				}
			}
		} else if( cmd == 4 ) {		///< all switch in column command.
			nColumn = get_column_count(0);
			printf((char *)"nColumn = %d\n", nColumn);
			cnt = 2 + ((nColumn >> 3) + ((nColumn & 0x7) ? 1 : 0));

			// 1-bit only select.
			for(sel=0; sel<nColumn; sel++) {
				set_bit(sel, &cmd_buf[2]);
				for(opcode=0; opcode<8; opcode++) {
					cmd_buf[1] = (cmd << 3) | c_op[opcode];
					printf((char *)"command(0x%02X), sColumn(0x%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3]);

					Debug.gTransmit = 1;
					send_command(cmd_buf, cnt);
					Debug.gTransmit = 0;

					if( all_switch_status_check() ) printf((char *)"Error!!!\n");
					else printf((char *)"OK!!!\n");
				}

				for(k=1; k<cnt; k++) cmd_buf[k] = 0x0;
			}

			// multi-bit select.
			for(sel=0; sel<nColumn; sel++) {
				set_bit(sel, &cmd_buf[2]);
				for(opcode=0; opcode<8; opcode++) {
					cmd_buf[1] = (cmd << 3) | c_op[opcode];
					printf((char *)"command(0x%02X), sColumn(0x%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3]);

					Debug.gTransmit = 1;
					send_command(cmd_buf, cnt);
					Debug.gTransmit = 0;

					if( all_switch_status_check() ) printf((char *)"Error!!!\n");
					else printf((char *)"OK!!!\n");
				}
			}
		} else if( cmd == 5 ) {		///< each switch in column command.
			nColumn = get_column_count(0);
			nSw = get_switch_count_in_column(0);
			printf((char *)"nColumn = %d, nSw = %d\n", nColumn, nSw);
			idx = nColumn + nSw;
			cnt = 2 + ((idx >> 3) + ((idx & 0x7) ? 1 : 0));

			// 1-bit only select.
			for(sel=0; sel<nColumn; sel++) {
				set_bit(sel, &cmd_buf[2]);
				for(k=0; k<nSw; k++) {
					set_bit(k+nColumn, &cmd_buf[2]);
					for(opcode=0; opcode<8; opcode++) {
						cmd_buf[1] = (cmd << 3) | c_op[opcode];
						printf((char *)"command(0x%02X), sColumn+sSW(0x%02X%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3], cmd_buf[4]);

						Debug.gTransmit = 1;
						send_command(cmd_buf, cnt);
						Debug.gTransmit = 0;

						if( all_switch_status_check() ) printf((char *)"Error!!!\n");
						else printf((char *)"OK!!!\n");
					}
					clear_bit(k+nColumn, &cmd_buf[2]);
				}
				for(k=1; k<cnt; k++) cmd_buf[k] = 0x0;
			}

			// multi-bit select.
			for(sel=0; sel<nColumn; sel++) {
				set_bit(sel, &cmd_buf[2]);
				for(k=0; k<nSw; k++) {
					set_bit(k+nColumn, &cmd_buf[2]);
					for(opcode=0; opcode<8; opcode++) {
						cmd_buf[1] = (cmd << 3) | c_op[opcode];
						printf((char *)"command(0x%02X), sColumn+sSW(0x%02X%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3], cmd_buf[4]);

						Debug.gTransmit = 1;
						send_command(cmd_buf, cnt);
						Debug.gTransmit = 0;

						if( all_switch_status_check() ) printf((char *)"Error!!!\n");
						else printf((char *)"OK!!!\n");
					}
				}

				for(k=0; k<nSw; k++) {
					clear_bit(k+nColumn, &cmd_buf[2]);
				}
			}
		} else if( cmd == 6 ) {		///< special command.
		} else if( cmd == 7 ) {		///< each switch command.
			cnt = 2 + MAX_BYTES;

			// 1-bit only select.
			for(sel=0; sel<MAX_SWITCH; sel++) {
				set_bit(sel, &cmd_buf[2]);
				for(opcode=0; opcode<8; opcode++) {
					cmd_buf[1] = (cmd << 3) | c_op[opcode];
					printf((char *)"command(0x%02X), (0x", cmd_buf[1]);
					for(uint8_t m=0; m<MAX_BYTES; m++) {
						printf((char *)"%02X", cmd_buf[2+m]);
					}
					printf((char *)")\n");

					Debug.gTransmit = 1;
					send_command(cmd_buf, cnt);
					Debug.gTransmit = 0;

					if( all_switch_status_check() ) printf((char *)"Error!!!\n");
					else printf((char *)"OK!!!\n");
				}

				for(k=1; k<cnt; k++) cmd_buf[k] = 0x0;
			}

			// multi-bit select.
			for(sel=0; sel<MAX_SWITCH; sel++) {
				set_bit(sel, &cmd_buf[2]);
				for(opcode=0; opcode<8; opcode++) {
					cmd_buf[1] = (cmd << 3) | c_op[opcode];
					printf((char *)"command(0x%02X), (0x", cmd_buf[1]);
					for(uint8_t m=0; m<MAX_BYTES; m++) {
						printf((char *)"%02X", cmd_buf[2+m]);
					}
					printf((char *)")\n");

					Debug.gTransmit = 1;
					send_command(cmd_buf, cnt);
					Debug.gTransmit = 0;

					if( all_switch_status_check() ) printf((char *)"Error!!!\n");
					else printf((char *)"OK!!!\n");
				}
			}
		} else if( cmd == 8 ) {			///< single command.
			cnt = 3;

			// 1-bit chip select.
			for(chip=0; chip<MAX_CHIP; chip++) {
				set_bit(chip, &cmd_buf[2]);
				for(opcode=0; opcode<8; opcode++) {
					cmd_buf[1] = (cmd << 3) | c_op[opcode];
					printf((char *)"command(0x%02X), chip(0x%02X)\n", cmd_buf[1], cmd_buf[2]);

					Debug.gTransmit = 1;
					send_command(cmd_buf, cnt);
					Debug.gTransmit = 0;

					if( all_switch_status_check() ) printf((char *)"Error!!!\n");
					else printf((char *)"OK!!!\n");
				}
				cmd_buf[2] = 0x0;
			}

			// multi chip select.
			for(chip=0; chip<MAX_CHIP; chip++) {
				set_bit(chip, &cmd_buf[2]);
				for(opcode=0; opcode<8; opcode++) {
					cmd_buf[1] = (cmd << 3) | c_op[opcode];
					printf((char *)"command(0x%02X), chip(0x%02X)\n", cmd_buf[1], cmd_buf[2]);

					Debug.gTransmit = 1;
					send_command(cmd_buf, cnt);
					Debug.gTransmit = 0;

					if( all_switch_status_check() ) printf((char *)"Error!!!\n");
					else printf((char *)"OK!!!\n");
				}
			}
		} else if( cmd == 9 ) {		///< 1-switch command.
			cnt = 4;
			// 1-bit chip select.
			for(chip=0; chip<MAX_CHIP; chip++) {
				set_bit(chip, &cmd_buf[2]);
				for(sel=0; sel<MAX_SWITCH; sel++) {
					cmd_buf[3] = sel;
					for(opcode=0; opcode<8; opcode++) {
						cmd_buf[1] = (cmd << 3) | c_op[opcode];
						printf((char *)"command(0x%02X), chip(0x%02X), %d-switch\n", cmd_buf[1], cmd_buf[2], cmd_buf[3]);

						Debug.gTransmit = 1;
						send_command(cmd_buf, cnt);
						Debug.gTransmit = 0;

						if( all_switch_status_check() ) printf((char *)"Error!!!\n");
						else printf((char *)"OK!!!\n");
					}
				}
				cmd_buf[2] = 0x0;
			}

			// multi chip select.
			for(chip=0; chip<MAX_CHIP; chip++) {
				set_bit(chip, &cmd_buf[2]);
				for(sel=0; sel<MAX_SWITCH; sel++) {
					cmd_buf[3] = sel;
					for(opcode=0; opcode<8; opcode++) {
						cmd_buf[1] = (cmd << 3) | c_op[opcode];
						printf((char *)"command(0x%02X), chip(0x%02X), %d-switch\n", cmd_buf[1], cmd_buf[2], cmd_buf[3]);

						Debug.gTransmit = 1;
						send_command(cmd_buf, cnt);
						Debug.gTransmit = 0;

						if( all_switch_status_check() ) printf((char *)"Error!!!\n");
						else printf((char *)"OK!!!\n");
					}
				}
			}
		} else if( cmd == 10 ) {		///< all switch in row command.
			nRow = get_row_count(0);
			cnt = 3 + ((nRow >> 3) + ((nRow & 0x7) ? 1 : 0));
			printf((char *)"nRow = %d\n", nRow);

			// 1-bit only select.
			for(chip=0; chip<MAX_CHIP; chip++) {
				set_bit(chip, &cmd_buf[2]);
				for(sel=0; sel<nRow; sel++) {
					set_bit(sel, &cmd_buf[3]);

					for(opcode=0; opcode<8; opcode++) {
						cmd_buf[1] = (cmd << 3) | c_op[opcode];
						printf((char *)"command(0x%02X), chip(0x%02X), sRow(0x%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3], cmd_buf[4]);

						Debug.gTransmit = 1;
						send_command(cmd_buf, cnt);
						Debug.gTransmit = 0;

						if( all_switch_status_check() ) printf((char *)"Error!!!\n");
						else printf((char *)"OK!!!\n");
					}
					clear_bit(sel, &cmd_buf[3]);
				}
				cmd_buf[2] = 0x0;
			}

			// multi-bit select.
			for(chip=0; chip<MAX_CHIP; chip++) {
				set_bit(chip, &cmd_buf[2]);
				for(sel=0; sel<nRow; sel++) {
					set_bit(sel, &cmd_buf[2]);
					for(opcode=0; opcode<8; opcode++) {
						cmd_buf[1] = (cmd << 3) | c_op[opcode];
						printf((char *)"command(0x%02X), chip(0x%02X), sRow(0x%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3], cmd_buf[4]);

						Debug.gTransmit = 1;
						send_command(cmd_buf, cnt);
						Debug.gTransmit = 0;

						if( all_switch_status_check() ) printf((char *)"Error!!!\n");
						else printf((char *)"OK!!!\n");
					}
				}
				for(sel=0; sel<nRow; sel++) {
					clear_bit(sel, &cmd_buf[2]);
				}
			}
		} else if( cmd == 11 ) {		///< each switch in row command.
			nRow = get_row_count(0);
			nSw = get_switch_count_in_row(0);
			idx = nRow + nSw;
			cnt = 3 + ((idx >> 3) + ((idx & 0x7) ? 1 : 0));
			printf((char *)"nRow = %d, nSw = %d\n", nRow, nSw);

			// 1-bit only select.
			for(chip=0; chip<MAX_CHIP; chip++) {
				set_bit(chip, &cmd_buf[2]);
				for(sel=0; sel<nRow; sel++) {
					set_bit(sel, &cmd_buf[3]);
					for(k=0; k<nSw; k++) {
						set_bit(k+nRow, &cmd_buf[3]);
						for(opcode=0; opcode<8; opcode++) {
							cmd_buf[1] = (cmd << 3) | c_op[opcode];
							printf((char *)"command(0x%02X), chip(0x%02X), sRow+sSW(0x%02X%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3], cmd_buf[4], cmd_buf[5]);

							Debug.gTransmit = 1;
							send_command(cmd_buf, cnt);
							Debug.gTransmit = 0;

							if( all_switch_status_check() ) {
								printf((char *)"Error!!!\n");
							} else {
								printf((char *)"OK!!!\n");
							}
						}
						clear_bit(k+nRow, &cmd_buf[2]);
					}
					clear_bit(sel, &cmd_buf[3]);
				}
				cmd_buf[2] = 0x0;
			}

			// multi-bit select.
			for(chip=0; chip<MAX_CHIP; chip++) {
				set_bit(chip, &cmd_buf[2]);
				for(sel=0; sel<nRow; sel++) {
					set_bit(sel, &cmd_buf[2]);
					for(k=0; k<nSw; k++) {
						set_bit(k+nRow, &cmd_buf[2]);
						for(opcode=0; opcode<8; opcode++) {
							cmd_buf[1] = (cmd << 3) | c_op[opcode];
							printf((char *)"command(0x%02X), chip(0x%02X), sRow+sSW(0x%02X%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3], cmd_buf[4], cmd_buf[5]);

							Debug.gTransmit = 1;
							send_command(cmd_buf, cnt);
							Debug.gTransmit = 0;

							if( all_switch_status_check() ) {
								printf((char *)"Error!!!\n");
							} else {
								printf((char *)"OK!!!\n");
							}
						}
					}
					for(k=0; k<nSw; k++) {
						clear_bit(k+nRow, &cmd_buf[2]);
					}
				}
				for(sel=0; sel<nRow; sel++) {
					clear_bit(sel, &cmd_buf[2]);
				}
			}
		} else if( cmd == 12 ) {		///< all switch in column command.
			nColumn = get_column_count(0);
			printf((char *)"nColumn = %d\n", nColumn);
			cnt = 3 + ((nColumn >> 3) + ((nColumn & 0x7) ? 1 : 0));
			printf((char *)"All Switch Control Command in the column.\n");

			// 1-bit only select.
			for(chip=0; chip<MAX_CHIP; chip++) {
				set_bit(chip, &cmd_buf[2]);
				for(sel=0; sel<nColumn; sel++) {
					set_bit(sel, &cmd_buf[3]);
					for(opcode=0; opcode<8; opcode++) {
						cmd_buf[1] = (cmd << 3) | c_op[opcode];
						printf((char *)"command(0x%02X), chip(0x%02X), sColumn(0x%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3], cmd_buf[4]);

						Debug.gTransmit = 1;
						send_command(cmd_buf, cnt);
						Debug.gTransmit = 0;

						if( all_switch_status_check() ) printf((char *)"Error!!!\n");
						else printf((char *)"OK!!!\n");
					}
					clear_bit(sel, &cmd_buf[3]);
				}
				cmd_buf[2] = 0x0;
			}

			// multi-bit select.
			for(chip=0; chip<MAX_CHIP; chip++) {
				set_bit(chip, &cmd_buf[2]);
				for(sel=0; sel<nColumn; sel++) {
					set_bit(sel, &cmd_buf[3]);
					for(opcode=0; opcode<8; opcode++) {
						cmd_buf[1] = (cmd << 3) | c_op[opcode];
						printf((char *)"command(0x%02X), chip(0x%02X), sColumn(0x%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3], cmd_buf[4]);

						Debug.gTransmit = 1;
						send_command(cmd_buf, cnt);
						Debug.gTransmit = 0;

						if( all_switch_status_check() ) printf((char *)"Error!!!\n");
						else printf((char *)"OK!!!\n");
					}
				}
				for(sel=0; sel<nColumn; sel++) {
					clear_bit(sel, &cmd_buf[3]);
				}
			}
		} else if( cmd == 13 ) {		///< each switch in column command.
			nColumn = get_column_count(0);
			nSw = get_switch_count_in_column(0);
			printf((char *)"nColumn = %d, nSw = %d\n", nColumn, nSw);
			idx = nColumn + nSw;
			cnt = 3 + ((idx >> 3) + ((idx & 0x7) ? 1 : 0));
			printf((char *)"Individual Switch Control Command in the column.\n");
			
			// 1-bit only select.
			for(chip=0; chip<MAX_CHIP; chip++) {
				set_bit(chip, &cmd_buf[2]);
				for(sel=0; sel<nColumn; sel++) {
					set_bit(sel, &cmd_buf[3]);
					for(k=0; k<nSw; k++) {
						set_bit(k+nColumn, &cmd_buf[3]);
						for(opcode=0; opcode<8; opcode++) {
							cmd_buf[1] = (cmd << 3) | c_op[opcode];
							printf((char *)"command(0x%02X), chip(0x%02X), sColumn+sSW(0x%02X%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3], cmd_buf[4], cmd_buf[5]);

							Debug.gTransmit = 1;
							send_command(cmd_buf, cnt);
							Debug.gTransmit = 0;

							if( all_switch_status_check() ) {
								printf((char *)"Error!!!\n");
							} else {
								printf((char *)"OK!!!\n");
							}
						}
						clear_bit(k+nColumn, &cmd_buf[3]);
					}
					clear_bit(sel, &cmd_buf[3]);
				}
				cmd_buf[2] = 0x0;
			}

			// multi-bit select.
			for(chip=0; chip<MAX_CHIP; chip++) {
				set_bit(chip, &cmd_buf[2]);
				for(sel=0; sel<nColumn; sel++) {
					set_bit(sel, &cmd_buf[3]);
					for(k=0; k<nSw; k++) {
						set_bit(k+nColumn, &cmd_buf[3]);
						for(opcode=0; opcode<8; opcode++) {
							cmd_buf[1] = (cmd << 3) | c_op[opcode];
							printf((char *)"command(0x%02X), chip(0x%02X), sColumn+sSW(0x%02X%02X%02X)\n", cmd_buf[1], cmd_buf[2], cmd_buf[3], cmd_buf[4], cmd_buf[5]);

							Debug.gTransmit = 1;
							send_command(cmd_buf, cnt);
							Debug.gTransmit = 0;

							if( all_switch_status_check() ) {
								printf((char *)"Error!!!\n");
							} else {
								printf((char *)"OK!!!\n");
							}
						}
					}

					for(k=0; k<nSw; k++) {
						clear_bit(k+nColumn, &cmd_buf[3]);
					}
				}
				for(sel=0; sel<nColumn; sel++) {
					clear_bit(sel, &cmd_buf[3]);
				}
			}
		} else if( cmd == 14 ) {		///< special command.
		} else if( cmd == 15 ) {		///< each switch command.
			cnt = 3 + MAX_BYTES;
			printf((char *)"Individual Switch Control Command.\n");

			// 1-bit only select.
			for(chip=0; chip<MAX_CHIP; chip++) {
				set_bit(chip, &cmd_buf[2]);
				for(sel=0; sel<MAX_SWITCH; sel++) {
					set_bit(sel, &cmd_buf[3]);
					for(opcode=0; opcode<8; opcode++) {
						cmd_buf[1] = (cmd << 3) | c_op[opcode];
						printf((char *)"command(0x%02X), chip(0x%02X), (0x", cmd_buf[1], cmd_buf[2]);
						for(uint8_t m=0; m<MAX_BYTES; m++) {
							printf((char *)"%02X", cmd_buf[3+m]);
						}
						printf((char *)")\n");

						Debug.gTransmit = 1;
						send_command(cmd_buf, cnt);
						Debug.gTransmit = 0;

						if( all_switch_status_check() ) printf((char *)"Error!!!\n");
						else printf((char *)"OK!!!\n");
					}
					clear_bit(sel, &cmd_buf[3]);
				}
				cmd_buf[2] = 0x0;
			}

			// multi-bit select.
			for(chip=0; chip<MAX_CHIP; chip++) {
				set_bit(chip, &cmd_buf[2]);
				for(sel=0; sel<MAX_SWITCH; sel++) {
					set_bit(sel, &cmd_buf[3]);
					for(opcode=0; opcode<8; opcode++) {
						cmd_buf[1] = (cmd << 3) | c_op[opcode];
						printf((char *)"command(0x%02X), chip(0x%02X), (0x", cmd_buf[1], cmd_buf[2]);
						for(uint8_t m=0; m<MAX_BYTES; m++) {
							printf((char *)"%02X", cmd_buf[2+m]);
						}
						printf((char *)")\n");

						Debug.gTransmit = 1;
						send_command(cmd_buf, cnt);
						Debug.gTransmit = 0;

						if( all_switch_status_check() ) printf((char *)"Error!!!\n");
						else printf((char *)"OK!!!\n");
					}
				}
				for(sel=0; sel<MAX_SWITCH; sel++) {
					clear_bit(sel, &cmd_buf[2]);
				}
			}
		}
//		getchar();
	}
	Debug.gTransmit = flag;

	return 0;
}

uint8_t Random_Command_Test(uint8_t arg)
{
	uint8_t idx, sel, cnt;
	uint8_t nSw, nRow, nColumn;
	tCommand cmd;
	bool flag = Debug.gTransmit;

	Debug.gTransmit = 1;
	do {
		idx = 0;
		cmd_buf[idx++] = 0xAA;
		cmd.CMD = rand() & 0xFF;
		cmd_buf[idx++] = cmd.CMD;

		if( (cmd.mode == 1) || (cmd.type == 6) ) {		// test & special command.
			for(sel=0; sel<MAX_FRAME; sel++) cmd_buf[sel] = 0x0;
			continue;
		}

		printf((char *)"%s : command(0x%02X)", Switch_Operation[cmd.opcode].str, cmd_buf[1]);
		if( cmd.cs == 1 ) {				// chip select command.
			cmd_buf[idx++] = rand() & CHIP_MASK;
			printf((char *)", chip(0x%02X)", cmd_buf[2]);
		}

		if( cmd.ctrl == 0 ) {			// single command.
		} else if( cmd.ctrl == 1 ) {		// 1-switch command.
			cmd_buf[idx++] = rand() % MAX_SWITCH;
			printf((char *)", %d-switch", cmd_buf[idx-1]);
		} else if( cmd.ctrl == 2 ) {		// all switch in row command.
			nRow = get_row_count(0);
			printf((char *)", row(");
			for(sel=0; sel<nRow; sel++) {
				if( rand() & 0x1 ) {
					set_bit(sel, &cmd_buf[idx]);
					putchar('1');
				} else {
					putchar('0');
				}
			}
			putchar(')');
			idx += (nRow >> 3) + ((nRow & 0x7) ? 1 : 0);
		} else if( cmd.ctrl == 3 ) {		// each switch in row command.
			nRow = get_row_count(0);
			nSw = get_switch_count_in_row(0);
			printf((char *)", row+switch(");
			cnt = nRow + nSw;

			for(sel=0; sel<cnt; sel++) {
				if( rand() & 0x1 ) {
					set_bit(sel, &cmd_buf[idx]);
					putchar('1');
				} else {
					putchar('0');
				}
			}
			putchar(')');
			idx += (cnt >> 3) + ((cnt & 0x7) ? 1 : 0);
		} else if( cmd.ctrl == 4 ) {		// all column switch command.
			nColumn = get_column_count(0);
			printf((char *)", column(");
			for(sel=0; sel<nColumn; sel++) {
				if( rand() & 0x1 ) {
					set_bit(sel, &cmd_buf[idx]);
					putchar('1');
				} else {
					putchar('0');
				}
			}
			putchar(')');
			idx += (nColumn >> 3) + ((nColumn & 0x7) ? 1 : 0);
		} else if( cmd.ctrl == 5 ) {		// each column switch command.
			nColumn = get_column_count(0);
			nSw = get_switch_count_in_row(0);
			printf((char *)", column+switch(");
			cnt = nColumn + nSw;

			for(sel=0; sel<cnt; sel++) {
				if( rand() & 0x1 ) {
					set_bit(sel, &cmd_buf[idx]);
					putchar('1');
				} else {
					putchar('0');
				}
			}
			putchar(')');
			idx += (cnt >> 3) + ((cnt & 0x7) ? 1 : 0);
		} else if( cmd.ctrl == 6 ) {		// special command.
			for(sel=0; sel<MAX_FRAME; sel++) cmd_buf[sel] = 0x0;
			continue;
		} else if( cmd.ctrl == 7 ) {		// each switch command.
			printf((char *)", sSwitch(0x");
			for(sel=0; sel<MAX_BYTES; sel++) {
				cmd_buf[idx] = rand() & 0xFF;
				printf((char *)"%02X", cmd_buf[idx]);
				idx++;
			}
			putchar(')');
//		} else if( cmd.ctrl == 8 ) {		// select chip single command.
//		} else if( cmd.ctrl == 9 ) {		// select chip 1-switch command.
//		} else if( cmd.ctrl == 10 ) {		// select chip all row switch command.
//		} else if( cmd.ctrl == 11 ) {		// select chip each row switch command.
//		} else if( cmd.ctrl == 12 ) {		// select chip all column switch command.
//		} else if( cmd.ctrl == 13 ) {		// select chip each column switch command.
//		} else if( cmd.ctrl == 14 ) {		// select chip special command.
//		} else if( cmd.ctrl == 15 ) {		// select chip each switch command.
		}
		printf((char *)" ... ");

		Debug.gTransmit = 1;
		send_command(cmd_buf, idx);
		Debug.gTransmit = 0;

		if( all_switch_status_check() ) printf((char *)"Error!!!\n");
		else printf((char *)"OK!!!\n");

		for(sel=0; sel<MAX_FRAME; sel++) cmd_buf[sel] = 0x0;

	} while( getkey() != ESC );

	Debug.gTransmit = flag;

	return 0;
}



const tFuncList CommandType[] = {
	{ Single_Command_Test,			"All Switch Control Command."},
	{ Single_Switch_Command_Test,		"1-Switch Control Command."},
	{ Row_All_Switch_Command_Test,	"Switch Control in Row Unit Command."},
	{ Row_Each_Switch_Command_Test,	"Individual Switch Control in Row Unit Command."},
	{ Column_All_Switch_Command_Test,	"Switch Control in Column Unit Command."},
	{ Column_Each_Switch_Command_Test, "Individual Switch Control in Column Unit Command."},
	{ Each_Switch_Command_Test,		"Individual Switch Control Command."},
	{ Read_Command_Test,			"Read Command Test."},
	{ Special_Command_Test,			"Special Command Test."},
	{ Continuous_Command_Test,		"Continuous Long-Run Test."},
	{ Random_Command_Test,			"Random Long-Run Test."},
	{ 0, NULL }
};

uint8_t Switch_Command_Test(uint8_t arg)
{
	uint8_t ch, fn;
	uint8_t i = 0;

//	printf((char *)"sizeof CommandType[] = %d\n", sizeof(CommandType));
	INFO("Multi-Switch Command Test.\n");
	do {
		i = 0;
		puts((char *)"\n----------------- Switch Command Type Test. -----------------");
		while( CommandType[i].func ) {
			printf((char *)" %c : %s\n", ((i<10) ? ('0'+i) : ('A'+(i-10))), CommandType[i].str);
			i++;
		}
		puts((char *)"-------------------------------------------------------------");
		printf((char *)" menu select ? ");

		ch = getchar();

		if( !isxdigit(ch) ) continue;

		putchar(ch); putchar('\n');

		if( isdigit(ch) ) fn = ch - '0';
		else fn = (toupper(ch) - 'A') + 10;

		if( fn < ((sizeof(CommandType)/4) - 1) ) {
			mSW_ALL_FORCE_OFF();
			((uint8_t (*)(uint8_t))(CommandType[fn].func))(0);
		}
	} while( ch != ESC );

	return 0;
}

