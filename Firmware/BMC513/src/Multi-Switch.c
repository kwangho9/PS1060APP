/**
 * @file Multi-Switch.c
 * @brief Multi Switch Test 프로젝트.
 * @author khjung ( khjung@leolsi.com )
 * @version 0.1
 * @date 2019-08-02
 * @see 참고할 내용들을 여기에 기술한다.
 * @todo 추가적으로 해야 할 사항.
 * @deprecated 향후 삭제 예정인 내용들을 표시함.
 */

#define __NewProject_c__
#include "includes.h"

#if 0
typedef struct _bit_count {
	uint8_t column;
	uint8_t col_sw;
	uint8_t row;
	uint8_t row_sw;
} tBitCount;
#endif

///< [분기][Type]
const tBitCount BitCount[] = {
    /*
#if (MAX_SWITCH == 120)
	{30, 4, 4, 30},		///< 4-분기.
	{20, 6, 6, 20},		///< 6-분기.
	{15, 8, 8, 15},		///< 8-분기.
	{12, 10, 10, 12},		///< 10-분기.
	{10, 12, 12, 10},		///< 12-분기.
	{8, 16, 16, 8},		///< 16-분기.
#elif (MAX_SWITCH == 60)
	{15, 4, 4, 15},		///< 4-분기.
	{10, 6, 6, 10},		///< 6-분기.
	{8, 8, 8, 8},			///< 8-분기.
	{6, 10, 10, 6},		///< 10-분기.
	{5, 12, 12, 5},		///< 12-분기.
	{4, 16, 16, 4},		///< 16-분기.
#elif (MAX_SWITCH == 30)
	{8, 4, 4, 8},		///< 4-분기.
	{5, 6, 6, 5},		///< 6-분기.
	{4, 8, 8, 4},			///< 8-분기.
	{3, 10, 10, 3},		///< 10-분기.
	{3, 12, 12, 3},		///< 12-분기.
	{2, 16, 16, 2},		///< 16-분기.
#endif
*/
	{8, 4, 4, 8},		///< 4-분기.
	{5, 6, 6, 5},		///< 6-분기.
	{4, 8, 8, 4},			///< 8-분기.
	{3, 10, 10, 3},		///< 10-분기.
	{3, 12, 12, 3},		///< 12-분기.
	{2, 16, 16, 2},		///< 16-분기.
};

const uint8_t c_GroupType[] = {BRANCH4, BRANCH6, BRANCH8, BRANCH10, BRANCH12, BRANCH16};

const uint8_t msb_bit_mask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
const uint8_t lsb_bit_mask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

/* Input Stream에서 Group/Channel의 선택 bit를 확인하고, Switch의 선택 bit를 check한다.
 */
/**
 * @brief 연속된 bit stream에서 n번째 bit의 값을 return한다.(MSB First)
 *
 * @param n bit number.
 * @param stream 연속된 bit stream.
 *
 * @return 0 or 1
 */
bool check_bit(uint8_t n, uint8_t *stream)
{
	return (stream[n>>3] & msb_bit_mask[n & 0x7]);
}

uint32_t copy_bit_msb_to_lsb(uint8_t s, uint8_t *stream, uint8_t n)
{
	uint8_t i;
	uint32_t ret = 0;

	for(i=0; i<n; i++, s++) {
//		printf((char *)"%d. stream = 0x%X, mask = 0x%X", i, stream[s>>3], msb_bit_mask[s & 0x7]);
		if( stream[s>>3] & msb_bit_mask[s & 0x7] ) ret |= ((uint32_t)1<<i);
//		printf((char *)", ret = 0x%lX\n", ret);
	}

	return ret;
}

uint32_t copy_bit_msb_to_msb(uint8_t s, uint8_t *stream, uint8_t n)
{
	uint32_t ret = 0;
	uint8_t i;

	for(i=0; i<n; i++, s++) {
		if( stream[s>>3] & msb_bit_mask[s & 0x7] ) ret |= ((uint32_t)1<<(31-i));
	}

	return ret;
}


/**
 * @brief 연속된 bit stream에서 n번째 bit를 set한다.(MSB first)
 *
 * @param n bit number.
 * @param stream 연속된 bit stream.
 */
void set_bit(uint8_t n, uint8_t *stream)
{
	stream[n>>3] |= msb_bit_mask[n & 0x7];
}

/**
 * @brief 연속된 bit stream에서 n번째 bit를 clear한다.(MSB first)
 *
 * @param n bit number.
 * @param stream 연속된 bit stream.
 */
void clear_bit(uint8_t n, uint8_t *stream)
{
	stream[n>>3] &= ~(1<<(7 - (n % 8)));
}


uint8_t *mSW_ALL_SWITCH_READ(uint8_t cmd, uint8_t chip)
{
	uint8_t n;
	uint8_t idx;

	for(n=0; n<gMaxFrame; n++) gFrame[n] = 0x0;

	idx = 0;
	gFrame[idx++] = 0xAA;
	gFrame[idx++] = cmd;
    gFrame[idx++] = (chip<<4);	///< chip & column/row number.
    idx += gMaxBytes;

    // operating clock.
    gFrame[idx++] = 0x0;
    // shift clock.
    gFrame[idx++] = 0x0;

	send_command(gFrame, idx);

    if( MAX_CHIP == 8 ) {
        return &RxBuffer[4];
    } else {
        for(uint8_t k=0; k<gMaxBytes; k++) mSW_Status[k] = 0;

        for(uint8_t k=0; k<gTotalSwitch; k++) {
            if( check_bit(k+MAX_CHIP, &RxBuffer[3]) )
                set_bit(k, &mSW_Status[0]);
        }

        return &mSW_Status[0];
    }
}

uint8_t mSW_SFR_Ctrl(uint8_t cmd, uint8_t chip, uint8_t num, uint8_t data)
{
	uint8_t n;
	uint8_t idx;

	for(n=0; n<MAX_FRAME; n++) gFrame[n] = 0x0;

	idx = 0;
	gFrame[idx++] = 0xAA;
	gFrame[idx++] = cmd;
	switch( cmd ) {
		case 0xFF:						///< Pattern Reset.
			for(idx=0; idx<(gMaxBytes + 3); idx++) gFrame[idx] = 0xFF;
			gFrame[idx++] = 0x00;
			gFrame[idx++] = 0x00;
			gFrame[idx++] = 0x00;
			break;
		case 0x37:						///< Chip Address Assignment.
			gFrame[idx++] = chip;
			break;

        case 0x74:
			gFrame[idx++] = chip;
        case 0x34:
            gFrame[idx++] = 0x00;
            break;

        case 0x75:
			gFrame[idx++] = chip;
        case 0x35:
            gFrame[idx++] = 0x00;
            break;

		case 0x71:						///< Group Type select with chip select.
			gFrame[idx++] = chip;
		case 0x31:						///< Group Type select.
			gFrame[idx++] = num << 5;
			break;

		case 0x81:
			gFrame[idx++] = num;			///< SFR address.
			gFrame[idx++] = data;			///< SFR data.
			break;
		case 0xC0: case 0xC1:
			gFrame[idx++] = (chip << 4) | (num >> 4);			///< chip number.
			gFrame[idx++] = ((num & 0x0F) << 4);			///< SFR address.
			gFrame[idx++] = data;			///< SFR data.
			break;
        case 0xF6:                      ///< single switch read.
			gFrame[idx++] = (chip << 4) | (num >> 4);			///< chip number.
			gFrame[idx++] = ((num & 0x0F) << 4);			///< SFR address.
            idx++;
            break;
		case 0xC2: case 0xC4:
			gFrame[idx++] = (chip<<4);	///< chip & column/row number.
			idx += gMaxBytes;
			break;
        /*
		case 0xD2: case 0xE2: case 0xD4: case 0xE4:
			gFrame[idx++] = (chip<<4) | num;	///< chip & row/column number.
			idx++;						///< dummy cycle.
			if( cmd & (1<<5) ) {			///< column number.
				n = get_switch_count_in_column(chip);
			} else {						///< row number.
				n = get_switch_count_in_row(chip);
			}
			idx += (n >> 3) + ((n & 7)?1:0);
			break;
        */
		default: return 1;
	}

    // operating clock.
    gFrame[idx++] = 0x0;
    // shift clock.
    gFrame[idx++] = 0x0;

	return send_command(gFrame, idx);
}

uint8_t mSW_Ctrl(uint8_t cmd, uint8_t chip, uint8_t *bundle, uint8_t cnt)
{
	uint8_t idx = 0;
//	uint8_t gFrame[MAX_BYTES];

	gFrame[idx++] = 0xAA;

	if( chip == 0x0 ) {
		gFrame[idx++] = cmd;
	} else {
		gFrame[idx++] = cmd | (1<<6);
		gFrame[idx++] = chip;
	}

	if( bundle != NULL ) {
		for(uint8_t i=0; i<cnt; i++) {
			gFrame[idx++] = bundle[i];
		}
	}

    // operating clock.
    gFrame[idx++] = 0x0;
    // shift clock.
    gFrame[idx++] = 0x0;

	return send_command(gFrame, idx);
}

uint8_t mSW_Single_Ctrl(uint8_t cmd, uint8_t chip, uint8_t num)
{
	uint8_t idx = 0;

	gFrame[idx++] = 0xAA;

	if( chip == 0x0 ) {
		gFrame[idx++] = cmd;
	} else {
		gFrame[idx++] = cmd | (1<<6);
		gFrame[idx++] = chip;
	}

	gFrame[idx++] = num;

    // operating clock.
    gFrame[idx++] = 0x0;
    // shift clock.
    gFrame[idx++] = 0x0;

	return send_command(gFrame, idx);
}


bool get_switch_onoff(uint8_t id, uint8_t nSwitch)
{
//	return cStatus[chip][nSwitch];
	return 0;
}










const tROM_STRING deviceName[] = {"PS1030", "PS1060", "PS1120", "NoDeive"};

const tROM_STRING cmd_name[] = {
	"OFF",
	"ON",
	"HOLD OFF",
	"HOLD ON",
	"RELEASE OFF",
	"RELEASE ON",
	"FORCE OFF",
	"FORCE ON"
};

/* MSB first */
void display_bits(char __code *str, uint8_t cnt, uint8_t *buf)
{
	uint8_t i;

	printf((char *)"%s[", str);
	for(i=0; i<cnt; i++) {
		if( *buf & (1 << (7 - (i & 7))) ) putchar('1');
		else putchar('0');

		if( (i & 7) == 7 ) buf++;
	}
	printf((char *)"]");
}

void parse_command(uint8_t *buf, uint8_t cnt)
{
	uint8_t i;
	uint8_t idx = 0;
	tCommand cmd;

	if( buf[idx++] != 0xAA ) ERROR("SYNC ERROR\n");

	cmd.CMD = buf[idx++];

	printf((char *)"\t[%c] ", (cmd.mode)?'T':'U');

	printf((char *)"%d-type, %s > ", cmd.ctrl, cmd_name[cmd.opcode].str);

#if 0
	if( cmd.opcode == 0x7 ) {
		printf((char *)"Chip Address Assignment[0x%02X]\n", buf[idx]);
//		display_bits("Chip Address", 8, &buf[idx++]);
		return;
	} else if( cmd.opcode == 0x6 ) {
		printf((char *)"Group Type[%d]\n", buf[idx++]);
		return;
	}
#endif

	if( cmd.cs ) {
		printf((char *)"Chip[0x%02X] ", buf[idx++]);
//		display_bits("CS", 8, buf[idx++]);
	}

	if( cmd.type == 0 ) {			///< single command.
		printf((char *)"\n");
	} else if( cmd.type == 1 ) {	///< 1-switch command.
		printf((char *)", Switch Number[%d]\n", buf[idx++]);
	} else if( (cmd.type == 2) || (cmd.type == 3) ) {	///< row select command.
		uint8_t nRow = get_row_count(0);

		printf((char *)", Row[");
		for(i=0; i<nRow; i++) {
			if( check_bit(i, &buf[idx]) ) putchar('1');
			else putchar('0');
		}

		if( cmd.type == 3 ) {	///< row & switch select command.
			uint8_t nSw = get_switch_count_in_row(0);

			printf((char *)"], Switch[");
			for(; i<(nRow+nSw); i++) {
				if( check_bit(i, &buf[idx]) ) putchar('1');
				else putchar('0');
			}
		}
		printf((char *)"]\n");
	} else if( (cmd.type == 4) || (cmd.type == 5) ) {	///< column select command.
		uint8_t nColumn = get_column_count(0);

		printf((char *)", Column[");
		for(i=0; i<nColumn; i++) {
			if( check_bit(i, &buf[idx]) ) putchar('1');
			else putchar('0');
		}

		if( cmd.type == 5 ) {	///< column & switch select command.
			uint8_t nSw = get_switch_count_in_column(0);

			printf((char *)"], Switch[");
			for(; i<(nColumn+nSw); i++) {
				if( check_bit(i, &buf[idx]) ) putchar('1');
				else putchar('0');
			}
		}
		printf((char *)"]\n");
	} else if( cmd.type == 6 ) {	///< special command.
	} else if( cmd.type == 7 ) {	///< all switch command.
		printf((char *)", Switch[");
		for(i=0; i<MAX_BYTES; i++) {
			printf((char *)"0x%02X ", buf[idx++]);
		}
		printf((char *)"\b]\n");
	} else {
		ERROR("Command Type Error!!!\n");
	}
}

uint8_t create_command(uint8_t *frame)
{
	uint8_t idx = 0;
	uint8_t i;
	tCommand cmd;

	frame[idx++] = 0xAA;

	cmd.CMD = rand() & 0x7F;		///< user mode command.
	frame[idx++] = cmd.CMD;

	if( cmd.cs ) {			///< using chip select.
		frame[idx++] = rand() & 0xFF;
	}

	if( cmd.mode ) {		///< test mode command.
	} else {					///< user mode command.
		if( cmd.type == 0 ) {			///< single command.
		} else if( cmd.type == 1 ) {	///< 1-switch command.
			frame[idx++] = rand() % MAX_SWITCH;
		} else if( (cmd.type == 2) || (cmd.type == 3) ) {	///< row select command.
			uint8_t cnt = get_row_count(0);

			if( cmd.type == 3 ) {	///< row & switch select command.
				cnt += get_switch_count_in_row(0);
			}
			cnt = (cnt / 8) + ((cnt & 0x7) ? 1 : 0);
			for(i=0; i<cnt; i++) frame[idx++] = rand() & 0xFF;
		} else if( (cmd.type == 4) || (cmd.type == 5) ) {	///< column select command.
			uint8_t cnt = get_column_count(0);

			if( cmd.type == 5 ) {	///< column & switch select command.
				cnt += get_switch_count_in_column(0);
			}
			cnt = (cnt / 8) + ((cnt & 0x7) ? 1 : 0);
			for(i=0; i<cnt; i++) frame[idx++] = rand() & 0xFF;
		} else if( cmd.type == 6 ) {	///< special command.
		} else if( cmd.type == 7 ) {	///< all switch command.
			for(i=0; i<MAX_BYTES; i++) {
				frame[idx++] = rand() & 0xFF;
			}
		}
	}

#if 1
	printf((char *)"%s() : ", __func__);
	for(i=0; i<idx; i++) {
		printf((char *)"0x%02X ", frame[i]);
	}
	printf((char *)"\n");

	parse_command(frame, idx);
#endif

	return idx;
}

void mSwitch_Init(void)
{
	// SPI port & block initialize.

	INFO("Chip Number = %d(0x%X), Switch Number = %d\n", MAX_CHIP, CHIP_MASK, MAX_SWITCH);
	INFO("max data length = %d-bytes, max frame length = %d-bytes\n", MAX_BYTES, MAX_FRAME);
	INFO("tMultiSwitch size = %d-bytes, Total eSW size = %d-bytes\n", sizeof(tMultiSwitch), sizeof(eSW));

	///< Emulator multi-switch initialize.
	emulator_init();
}

#define WaitForReady()		while( rSPICK & SPI_BUSY )
#define CSN_LOW()			rP5_b7 = 0
#define CSN_HIGH()			rP5_b7 = 1
uint8_t SendByte(uint8_t tx)
{
    CSN_LOW();

    rSPIDATA0 = tx;
    while( (rSPIMOD & SPI_INTPEND) == 0 ) ;
    rSPIMOD |= SPI_INTPEND;
    WaitForReady();

    CSN_HIGH();

	return rSPIDATA0;
}



#if 1	// simulator function.
uint8_t sim_set_switch_ctrl(uint8_t id, uint8_t opcode, uint8_t sw)
{
	if( sw > (MAX_SWITCH-1) ) {
//		printf((char *)"%s() : %d-SWITCH NUMBER ERROR!!!\n", __func__, sw);
		return -1;
	}

//	printf((char *)"%s() : %d-chip, opcode = 0x%X, switch = %d\n", __func__, id, opcode, sw);
	if( opcode == 0 ) {				///< Switch OFF command.
		if( check_bit(sw, &sSW[id].hold[0]) ) return 1;
		clear_bit(sw, &sSW[id].onoff[0]);
	} else if( opcode == 1 ) {		///< Switch ON command.
		if( check_bit(sw, &sSW[id].hold[0]) ) return 1;
		set_bit(sw, &sSW[id].onoff[0]);
	} else if( opcode == 2 ) {		///< Switch HOLD OFF command.
		if( check_bit(sw, &sSW[id].hold[0]) ) return 1;
		clear_bit(sw, &sSW[id].onoff[0]);
		set_bit(sw, &sSW[id].hold[0]);
	} else if( opcode == 3 ) {		///< Switch HOLD ON command.
		if( check_bit(sw, &sSW[id].hold[0]) ) return 1;
		set_bit(sw, &sSW[id].onoff[0]);
		set_bit(sw, &sSW[id].hold[0]);
//		printf((char *)"%d-switch : onoff = %d, hold = %d\n", sw, check_bit(sw, &sSW[id].onoff[0]), check_bit(sw, &sSW[id].hold[0]));
	} else if( opcode == 4 ) {		///< Switch RELEASE OFF command.
		if( check_bit(sw, &sSW[id].hold[0]) ) {
			clear_bit(sw, &sSW[id].onoff[0]);
			clear_bit(sw, &sSW[id].hold[0]);
		}
	} else if( opcode == 5 ) {		///< Switch RELEASE ON command.
		if( check_bit(sw, &sSW[id].hold[0]) ) {
			set_bit(sw, &sSW[id].onoff[0]);
			clear_bit(sw, &sSW[id].hold[0]);
		}
	} else if( opcode == 6 ) {		///< Switch FORCE OFF command.
		clear_bit(sw, &sSW[id].onoff[0]);
		clear_bit(sw, &sSW[id].hold[0]);
	} else if( opcode == 7 ) {		///< Switch FORCE ON command.
		set_bit(sw, &sSW[id].onoff[0]);
		clear_bit(sw, &sSW[id].hold[0]);
	}

	return 0;
}

uint8_t sim_set_row_ctrl(uint8_t id, uint8_t opcode, uint8_t row)
{
	uint8_t i, nSw;
	uint8_t nRow = get_switch_count_in_row(id);

	for(i=0; i<nRow; i++) {
		nSw = get_switch_number_in_row(id, row, i);
		sim_set_switch_ctrl(id, opcode, nSw);
	}

	return 0;
}

uint8_t sim_set_column_ctrl(uint8_t id, uint8_t opcode, uint8_t column)
{
	uint8_t nSw = get_switch_number_in_column(id, column, 0);

	for(uint8_t i=0; i<get_switch_count_in_column(id); i++) {
//		printf((char *)"%s() : switch = %d\n", __func__, nSw+i);
		sim_set_switch_ctrl(id, opcode, nSw+i);
	}

	return 0;
}

uint8_t sim_set_switch_ctrl_in_row(uint8_t id, uint8_t opcode, uint8_t row, uint8_t sw)
{
	return sim_set_switch_ctrl(id, opcode, get_switch_number_in_row(id, row, sw));
}

uint8_t sim_set_switch_ctrl_in_column(uint8_t id, uint8_t opcode, uint8_t column, uint8_t sw)
{
	return sim_set_switch_ctrl(id, opcode, get_switch_number_in_column(id, column, sw));
}

bool sim_get_switch_onoff(uint8_t id, uint8_t sw)
{
//	printf((char *)"%s() : %d-chip, %d-switch, onoff(0x%X), mask(0x%X)\n", __func__, id, sw, sSW[id].onoff[sw>>3], msb_bit_mask[sw&7]);
	if( sw < 128 ) return (sSW[id].onoff[sw>>3] & msb_bit_mask[sw&7]);

	return 0;
}

bool sim_get_switch_hold(uint8_t id, uint8_t sw)
{
//	printf((char *)"%s() : %d-chip, %d-switch, hold(0x%X), mask(0x%X)\n", __func__, id, sw, sSW[id].hold[sw>>3], msb_bit_mask[sw&7]);
	if( sw < 128 ) return (sSW[id].hold[sw>>3] & msb_bit_mask[sw&7]);

	return 0;
}

bool sim_get_switch_onoff_in_column(uint8_t id, uint8_t nColumn, uint8_t nSwitch)
{
#if 0
	uint8_t sw = get_switch_number_in_column(id, nColumn, nSwitch);
	uint8_t ret;

	ret = sim_get_switch_onoff(id, sw);
	printf((char *)"%s() : %d-switch is %s.\n", __func__, sw, ret ? "ON":"OFF");
	return ret;
#else
	return sim_get_switch_onoff(id, get_switch_number_in_column(id, nColumn, nSwitch));
#endif
}

bool sim_get_switch_hold_in_column(uint8_t id, uint8_t nColumn, uint8_t nSwitch)
{
	return sim_get_switch_hold(id, get_switch_number_in_column(id, nColumn, nSwitch));
}

bool sim_get_switch_onoff_in_row(uint8_t id, uint8_t nRow, uint8_t nSwitch)
{
#if 0
	uint8_t sw = get_switch_number_in_row(id, nRow, nSwitch);
	uint8_t ret;

	ret = sim_get_switch_onoff(id, sw);
	printf((char *)"%s() : %d-switch is %s.\n", __func__, sw, ret ? "ON":"OFF");
	return ret;
#else
	return sim_get_switch_onoff(id, get_switch_number_in_row(id, nRow, nSwitch));
#endif
}

bool sim_get_switch_hold_in_row(uint8_t id, uint8_t nRow, uint8_t nSwitch)
{
#if 0
	uint8_t sw = get_switch_number_in_row(id, nRow, nSwitch);
	uint8_t ret;

	ret = sim_get_switch_hold(id, sw);
	printf((char *)"%s() : %d-switch is %s.\n", __func__, sw, ret ? "HOLD":"RELEASE");
	return ret;
#else
	return sim_get_switch_hold(id, get_switch_number_in_row(id, nRow, nSwitch));
#endif
}







uint8_t sSwitch_Simulator(uint8_t id, uint8_t *frame, uint8_t cnt)
{
	uint8_t i;
	uint8_t idx = 0;
	tCommand cmd;

#if 0
	printf((char *)"%s() : %d-chip, command = ", __func__, id);
	for(uint8_t k=0; k<cnt; k++) {
		printf((char *)"0x%02X ", frame[k]);
	}
	putchar('\n');
#endif

	if( frame[idx++] != 0xAA ) {
		printf((char *)"SYNC(0x%X) ERROR!!!\n", frame[idx-1]);
		return 0;
	}

	cmd.CMD = frame[idx++];

	if( cmd.mode == 1 ) {			///< Test Command.
		uint8_t chip, bits, ret;
		uint8_t column, row;

		if( cmd.cs ) {
			if( (cmd.CMD == 0xC0) || (cmd.CMD == 0xC1) ) {		///< SFR read/write.
				chip = frame[idx++];
			} else {
				chip = (frame[idx] & 0xF0) >> 4;
			}
			if( chip != id ) return 0;
		}

		if( cmd.CMD & (1<<5) ) {			///< read column number.
			column = frame[idx++] & 0x0F;
			bits = get_switch_count_in_column(chip);
		} else if( cmd.CMD & (1<<4) ) {	///< read row number.
			row = frame[idx++] & 0x0F;
			bits = get_switch_count_in_row(chip);
		} else {
			bits = MAX_SWITCH;
		}

		if( cmd.opcode == 0 ) {				///< FTP/SFR read command.
			uint8_t sw;
			uint8_t addr = frame[idx++];
//			if( addr < FTP_CTRL ) {
//				frame[idx++] = sSW[chip].FTP[addr];
//			} else {
				switch( addr ) {
					case SW_STATUS:
						frame[idx] = 0;
						sw = sSW[chip].SFR[SFR_INDEX(SW_NUM)];
						if( sim_get_switch_onoff(chip, sw) ) {
							frame[idx] |= (1<<0);
						}
						if( sim_get_switch_hold(chip, sw) ) {
							frame[idx] |= (1<<1);
						}
						idx++;
						break;
					default:
						frame[idx++] = sSW[chip].SFR[SFR_INDEX(addr)];
						break;
				}
//			}
			ret = 1;
//			printf((char *)"%s() : chip = %d, addr = 0x%x, ret = 0x%X\n", __func__, chip, addr, frame[idx-1]);
		} else if( cmd.opcode == 1 ) {		///< FTP/SFR write command.
			uint8_t addr, data;

			addr = frame[idx++];	///< read SFR address.
			data = frame[idx++];	///< read SFR data.

//			if( addr < FTP_CTRL ) {
//				sSW[chip].FTP[addr] = data;
//			} else {
				sSW[chip].SFR[SFR_INDEX(addr)] = data;
//			}
//			printf((char *)"%s() : chip = %d, addr = 0x%x, data = 0x%x\n", __func__, chip, addr, data);
		} else if( cmd.opcode == 2 ) {			///< switch on/off status read.
			idx++;
			if( cmd.CMD & (1<<4) ) {
				for(i=0; i<bits; i++) {
					if( sim_get_switch_onoff_in_row(chip, row, i) ) {
						set_bit(i, &frame[idx]);						///< conversion bit format : MSB
					}
				}
			} else if( cmd.CMD & (1<<5) ) {
//				INFO("%d-chip, %d-column, bits = %d\n", chip, column, bits);
				for(i=0; i<bits; i++) {
					if( sim_get_switch_onoff_in_column(chip, column, i) ) {
						set_bit(i, &frame[idx]);						///< conversion bit format : MSB
					}
				}
			} else {
				for(i=0; i<bits; i++) {
					if( sim_get_switch_onoff(chip, i) ) {
						set_bit(i, &frame[idx]);
					}
				}
#if 0
				printf((char *)"\n[");
				for(i=0; i<15; i++) printf((char *)"0x%02X ", frame[4+i]);
				printf((char *)"]\n");
#endif
			}
			ret = (bits >> 3) + ((bits & 0x7)?1:0);
		} else if( cmd.opcode == 4 ) {			///< switch hold status read.
			idx++;
//			for(i=0; i<MAX_BYTES; i++) result[i] = 0;

			if( cmd.CMD & (1<<4) ) {
				for(i=0; i<bits; i++) {
					if( sim_get_switch_hold_in_row(chip, row, i) ) {
						set_bit(i, &frame[idx]);						///< conversion bit format : MSB
					}
				}
			} else if( cmd.CMD & (1<<5) ) {
				for(i=0; i<bits; i++) {
					if( sim_get_switch_hold_in_column(chip, column, i) ) {
						set_bit(i, &frame[idx]);						///< conversion bit format : MSB
					}
				}
			} else {
				for(i=0; i<bits; i++) {
					if( sim_get_switch_hold(chip, i) ) {
						set_bit(i, &frame[idx]);
					}
				}
			}
			ret = (bits >> 3) + ((bits & 0x7)?1:0);
		}

		return ret;
	} else {						///< User Command.
		if( cmd.cs == 1 ) {			///< using Chip Select.
			if( (frame[idx++] & sSW[id].ChipSelect) == 0 ) {
//				ERROR("CHIP ID ERROR -> 0x%X\n", frame[idx-1]);
				return 0;
			}
		}

		if( cmd.type == 0 ) {		///< single command.
			for(i=0; i<MAX_SWITCH; i++) {
				sim_set_switch_ctrl(id, cmd.opcode, i);
			}
#if 0
			printf((char *)"\nONOFF Result : ");
			for(sw=0; sw<MAX_BYTES; sw++) printf((char *)"0x%X ", sSW[id].onoff[sw]);
			putchar('\n');
			printf((char *)"HOLD Result  : ");
			for(sw=0; sw<MAX_BYTES; sw++) printf((char *)"0x%X ", sSW[id].hold[sw]);
			putchar('\n');
#endif
		} else if( cmd.type == 1 ) {	///< 1-switch command.
//			printf((char *)"\n%s() : %d-switch, opcode = 0x%X -> ", __func__, frame[idx], cmd.opcode);
			sim_set_switch_ctrl(id, cmd.opcode, frame[idx]);
//			printf((char *)"0x%X\n", sSW[id].onoff[frame[idx]>>3]);
		} else if( cmd.type == 2 ) {	///< all switch in row command.
			uint8_t row;
			uint8_t eRow = get_row_count(id);
			for(row=0; row<eRow; row++) {
				if( check_bit(row, &frame[idx]) == 1 ) {
					sim_set_row_ctrl(id, cmd.opcode, row);
				}
			}
		} else if( cmd.type == 3 ) {	///< each switch in row command.
			uint8_t row, sw;
			uint8_t eRow = get_row_count(id);
			uint8_t eSW = get_switch_count_in_row(id);

			for(row=0; row<eRow; row++) {
				if( check_bit(row, &frame[idx]) == 0 ) continue;

				for(i=0; i<eSW; i++) {
					if( check_bit(eRow+i, &frame[idx]) == 0 ) continue;

					sw = get_switch_number_in_row(id, row, i);
					sim_set_switch_ctrl(id, cmd.opcode, sw);
				}
			}
		} else if( cmd.type == 4 ) {	///< all switch in column command.
			uint8_t column;
			uint8_t eColumn = get_column_count(id);
			for(column=0; column<eColumn; column++) {
				if( check_bit(column, &frame[idx]) == 1 ) {
//					printf((char *)"%s() : column = %d\n", __func__, column);
					sim_set_column_ctrl(id, cmd.opcode, column);
				}
			}
		} else if( cmd.type == 5 ) {	///< switch in column command.
			uint8_t column, sw, i;
			uint8_t eColumn = get_column_count(id);
			uint8_t eSW = get_switch_count_in_column(id);
			for(column=0; column<eColumn; column++) {
				if( check_bit(column, &frame[idx]) == 0 ) continue;

				for(i=0; i<eSW; i++) {
					if( check_bit(eColumn+i, &frame[idx]) == 0 ) continue;

					sw = get_switch_number_in_column(id, column, i);
					sim_set_switch_ctrl(id, cmd.opcode, sw);
				}
			}
		} else if( cmd.type == 6 ) {	///< special command.
			if( cmd.opcode == 0 ) {
			} else if( cmd.opcode == 1 ) {
//				GroupType = c_GroupType[(frame[idx++] & 0xE0) >> 5];
				GroupType = (frame[idx++] & 0xE0) >> 5;
				sSW[id].GroupType = GroupType;
//				printf((char *)"%d. Group Type = %d-branch\n", id, sSW[id].GroupType);
				return idx;
			} else if( cmd.opcode == 2 ) {
			} else if( cmd.opcode == 3 ) {
			} else if( cmd.opcode == 4 ) {
			} else if( cmd.opcode == 5 ) {
			} else if( cmd.opcode == 6 ) {
			} else if( (cmd.opcode == 7) && (sSW[id].ChipSelect == 0) ) {
				uint8_t i;
				uint8_t tmp = frame[idx] & 0xFF;
				for(i=0; i<8; i++) {
					if( tmp & (1 << (7 - i)) ) {
						frame[idx] ^= (1 << (7 - i));
						sSW[id].ChipSelect = (1 << (7 - i));
//						printf((char *)"%d-Chip Address 0x%X Assignment!\n", i, sSW[id].ChipSelect);
						break;
					}
				}
				idx++;
				return idx;
			}
		} else if( cmd.type == 7 ) {	///< all switch command.
			for(i=0; i<MAX_SWITCH; i++) {
				if( check_bit(i, &frame[idx]) == 1 ) {
					sim_set_switch_ctrl(id, cmd.opcode, i);
				}
			}
		}
	}

	return 0;
}
#endif

uint8_t send_command(uint8_t *frame, uint8_t cnt)
{
	uint8_t i, ret = 0;

#if TX_FRAME_DUMP
	if( Debug.gTransmit ) {
		printf((char *)" [ TX(%d) : ", cnt);
		for(i=0; i<cnt; i++) {
			printf((char *)"0x%02X ", frame[i]);
		}
		printf((char *)"] ");
	}
#endif

#if EMULATOR
	if( Debug.gEmulator ) {
		uint8_t tmp;
		if( frame[1] == 0x37 ) tmp = frame[2];		// chip address assignment.
		for(uint8_t id=0; id<gMaxChip; id++) {
			ret += eSwitch_Emulator(id, frame);
		}
		if( frame[1] == 0x37 ) frame[2] = tmp;		// chip address assignment.
	}
//	printf((char *)"eSW[%d].ChipSelect = 0x%x\n", chip, eSW[chip].ChipSelect);
#endif

#if 0	// simulator result. -> FPGA 적용시점에 제거해야 함.
//	printf((char *)"[ TX : ");
//	for(i=0; i<cnt; i++) {
//		printf((char *)"0x%02X ", frame[i]);
//	}
//	printf((char *)"]\n");
	for(uint8_t chip=0; chip<MAX_CHIP; chip++) {
		sSwitch_Simulator(chip, frame, cnt);
	}
#else
    // khjung
	for(i=0; i<cnt; i++) {
		RxBuffer[i] = SendByte(frame[i]);
	}
#endif

#if RX_FRAME_DUMP
	if( Debug.gReceive ) {
		printf((char *)" [ RX(%d) : ", cnt);
		for(i=0; i<cnt; i++) {
			printf((char *)"0x%02X ", RxBuffer[i]);
		}
		printf((char *)"]\n");
	}
#endif

#if CHECK_FRAME
	if( Debug.gCheck ) {
	}
#endif

    if( frame[1] == 0xC0 ) {            // SFR Read Command.
        for(uint8_t k=0; k<8; k++) {
            if( check_bit(gMaxChip + 4 + k, &RxBuffer[3]) == 1 ) {
                set_bit(k, &ret);
            }
        }
//        if( MAX_CHIP < 4 ) ret = (RxBuffer[3] << (MAX_CHIP+4)) | (RxBuffer[4] >> (4-MAX_CHIP));
//        else if( MAX_CHIP == 4 ) ret = RxBuffer[4];
//        else ret = (RxBuffer[4] << (MAX_CHIP - 4)) | (RxBuffer[5] >> (12-MAX_CHIP));
    } else if( frame[1] == 0xF6 ) {     // single switch read command.
        for(uint8_t k=0; k<2; k++) {
            if( check_bit(gMaxChip + 3 + k, &RxBuffer[4]) == 1 ) {
                set_bit(k+6, &ret);
            }
        }
//        if( MAX_CHIP < 4 ) ret = (RxBuffer[4] >> (3 - MAX_CHIP));
//        else if( MAX_CHIP == 4 ) ret = (RxBuffer[4] << 1) | (RxBuffer[5] >> 7);
//        else ret = RxBuffer[5] >> (11 - MAX_CHIP);
    } else if( frame[1] == 0x37 ) {     // chip ID assignment command.
        if( gMaxChip == 8 ) {
            ret = RxBuffer[3];
        } else {
            ret = (RxBuffer[2] << gMaxChip) | (RxBuffer[3] >> (8-gMaxChip));
        }
    }

	return ret;
}




/**
 * @brief 모든 Chip에 대해서 Switch Hold On/Off 상태를 출력해 준다.
 */
void switch_dump(uint8_t cs)
{
	uint8_t i, chip, column, row, sw;
	uint8_t eColumn, eRow;

	putchar('\n');
	for(i=0; i<40; i++) putchar('-');
	printf((char *)" Simulator Switch Status ");
	for(i=0; i<40; i++) putchar('-');
	putchar('\n');

	for(chip=0; chip<MAX_CHIP; chip+=4) {
		eColumn = get_column_count(chip);
		eRow = get_row_count(chip);
//		INFO("nColumn = %d, nRow = %d\n", eColumn, eRow);
		printf((char *)"               [ %d-chip ]             [ %d-chip ]", chip, chip+1);
		printf((char *)"            [ %d-chip ]             [ %d-chip ]\n", chip+2, chip+3);
		printf((char *)"      row : ");
		for(row=0; row<eRow; row++) printf((char *)"%d", row);
		printf((char *)"\n");

		for(column=0; column<eColumn; column++) {
			printf((char *)"%2d-column : ", column);
			for(i=chip; i<(chip+4); i++) {
				if( (cs & (1<<(7-i))) == 0 ) {
					for(uint8_t j=0; j<((eRow*2)+3); j++) putchar(' ');
					continue;
				}

//				mSW_COLUMN_SWITCH_ONOFF_READ(i, column);
				for(sw=0; sw<eRow; sw++) {
					if( check_bit(sw, &gFrame[4]) ) {
						printf((char *)"*");
					} else {
						printf((char *)".");
					}
				}

//				mSW_COLUMN_SWITCH_HOLD_READ(i, column);
				printf((char *)"(");
				for(sw=0; sw<eRow; sw++) {
					if( check_bit(sw, &gFrame[4]) ) {
						printf((char *)"H");
					} else {
						printf((char *)".");
					}
				}
				printf((char *)") ");
			}
			printf((char *)"\n");
		}
	}
}

uint8_t register_read(uint8_t chip, uint8_t addr)
{
	mSW_FTP_READ(chip, addr);

	return gFrame[4];
}

void ftpd_dump(void)
{
	uint8_t chip, addr, data;

	printf((char *)"\n--------------- FTPD Register Dump ----------------\n");
	for(chip=0; chip<MAX_CHIP; chip++) {
		printf((char *)"%d-chip : ", chip);
		for(addr=FTPD0_ADDR; addr<(FTPD3_ADDR+1); addr++) {
			data = register_read(chip, addr);
			printf((char *)"0x%02X(0x%02X) ", data, gStatus[0]);
		}
		printf((char *)"\n");
	}
}

void chip_status(uint8_t chip)
{
	uint8_t i, column, sw;
	uint8_t nSw = get_switch_count_in_column(chip);
	uint8_t nRow = get_row_count(chip);
	uint8_t nColumn = get_column_count(chip);

	printf((char *)"%d-chip : %d-branch, %d columns, %d rows\n",
		chip, BitCount[eSW[chip].GroupType].row, get_column_count(chip), get_row_count(chip));

	printf((char *)"      row : ");
	for(i=0; i<nRow; i++) printf((char *)"%d ", i);
	printf((char *)"   ");
	for(i=0; i<nRow; i++) printf((char *)"%d ", i);
	putchar('\n');

	for(column=0; column<nColumn; column++) {
		printf((char *)"%2d-column : ", column);
		for(sw=0; sw<nSw; sw++) {
			if( emul_get_switch_onoff(chip, (column*nSw)+sw) ) printf((char *)"* ");
			else printf((char *)". ");
//			printf((char *)"%d ", get_switch(chip, (column*nSw)+sw));
		}
		printf((char *)"   ");
		for(sw=0; sw<nSw; sw++) {
			if( emul_get_switch_hold(chip, (column*nSw)+sw) ) printf((char *)"H ");
			else printf((char *)". ");
		}
		putchar('\n');
	}
	putchar('\n');
}


uint8_t get_status(void)
{
	uint8_t i, chip;

	///< read명령에 대해서 Emulator status는 result[]배열에 return되고, Simulator status는 command frame의 read 영역에 return된다.

	///< switch status(onoff/hold) read.
	for(chip=0; chip<MAX_CHIP; chip++) {
		mSW_ALL_SWITCH_ONOFF_READ(chip);
		mSW_ALL_SWITCH_HOLD_READ(chip);
	}

	///< FTPD read.
	for(chip=0; chip<MAX_CHIP; chip++) {
		for(i=0; i<8; i++) {
			mSW_FTP_READ(chip, i);
		}
	}

	return 0;
}

uint8_t device_detect(void)
{
    uint8_t mask;

    // 1. Pattern Reset
    printf((char *)"\tPattern Reset.\n");
//    mSW_PATTERN_RESET();    // PS1030과 PS1060이 서로 다르다.
    for(uint8_t k=0; k<18; k++) {   // PS1120 Pattern Reset을 보낸다.
        SendByte(0xFF);
    }
    SendByte(0x00);         // command cycle
    SendByte(0x00);         // op cycle
    SendByte(0x00);         // op cycle
    SendByte(0x00);         // shift cycle : dummy를 4개 넣어야 정상동작함.

    // 2. Chip ID Assignment -> 연결된 Device의 개수를 알수 있다.
    printf((char *)"\tCHIP Address Assignment.\n");
    gMaxChip = 0;
    mask = mSW_CHIP_ADDRESS_ASSIGN(0xFF);  // '0' id assignment.
    // RxBuffer[3]의 bit[6]부터 
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
            // ID별로 Device의 종류(60/30)를 check한다.
            // (Device를 섞어서 사용할 수 없다.)
            uint8_t ch = mSW_SFR_READ(id, CHIP_CTRL);
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

    printf((char *)"%s device %d-chain detected.\n", deviceName[gDevice].str, gMaxChip);

    return gDevice;
}

void chip_id_assignment_test(void)
{
    uint8_t idx, val;

    // 3. Device SFR Read Test.
    printf((char *)"Pattern Reset & Chip ID Assignment Test.\n");
    for(idx=0; idx<8; idx++) {
        // Chip ID가 제대로 할당되는지 check한다.(첫번째 Chip에 대해서만 검토)
        // 방법 : Pattern Reset -> ID Assignment -> CHIP_STATUS read.
//        for(uint8_t k=0; k<(gMaxFrame+1); k++) {
//            if( k < (gMaxBytes + 3) ) SendByte(0xFF);
//            else SendByte(0x0);
//        }
        mSW_PATTERN_RESET();

        mSW_CHIP_ADDRESS_ASSIGN(1<<(7-idx));

        val = mSW_SFR_READ(idx, CHIP_STATUS);
        printf((char *)"\tCHIP_STATUS(0x%X) : ", val);
        printf((char *)"ID = %d, ", (val >> 4) & 0x7);
        printf((char *)"Assign Flag = %d\n", val & 0x1);
    }
}

void active_discharge_read_test(void)
{
    uint8_t val;

    printf((char *)"Active Discharge Enable/Disable Test.\n");
    // Active Discharge Enable/Disable이 제대로 설정되는지 check한다.
    // 방법 : Active Dischage Enable/Disable -> IP_CTRL read.
    mSW_ALL_AD_ENABLE();
    val = mSW_SFR_READ(0, IP_CTRL);
    printf((char *)"\tIP_CTRL(0x%X) : ", val);
    printf((char *)"Active Discharge %s\n", (val & 0x40) ? "Enable" : "Disable");

    mSW_ALL_AD_DISABLE();
    val = mSW_SFR_READ(0, IP_CTRL);
    printf((char *)"\tIP_CTRL(0x%X) : ", val);
    printf((char *)"Active Discharge %s\n", (val & 0x40) ? "Enable" : "Disable");
}

void group_type_read_test(void)
{
    uint8_t val;

    printf((char *)"Group Type Test.\n");
    for(uint8_t group=0; group<6; group++) {
        mSW_SET_GROUP_TYPE(group);

        val = mSW_SFR_READ(0, CHIP_CTRL);
        printf((char *)"\tCHIP_CTRL(0x%X) : ", val);
        if( val & 0x80 ) {
            if( val & 0x40 ) printf((char *)"Upper 60");
            else printf((char *)"Lower 60");
        } else {
            if( val & 0x40 ) printf((char *)"60");
            else printf((char *)"30");
        }
        printf((char *)"-channel, ");
        printf((char *)"%d-group\n", c_GroupType[val & 0x7]);
    }
}

void switch_status_read_test(void)
{
    uint8_t *buf;
    uint8_t idx;
    uint8_t val;

    printf((char *)"Single & All Switch Status Read Test.\n");
    mSW_ALL_HOLD_ON();
    buf = mSW_ALL_SWITCH_ONOFF_READ(0);   // all switch onoff read.
    printf((char *)"\tAll Switch OnOff : ");
    for(idx=0; idx<gTotalSwitch; idx++) {
        if( check_bit(idx, buf) == 0 ) break;
    }
    printf((char *)"%s -> ", (idx == gTotalSwitch) ? "PASS" : "FAIL");
    for(uint8_t k=0; k<gMaxBytes; k++) {
        printf((char *)"0x%02X%c", *buf++, (k==(gMaxBytes-1))?'\n':' ');
    }
    buf = mSW_ALL_SWITCH_HOLD_READ(0);   // all switch onoff read.
    printf((char *)"\tAll Switch Hold : ");
    for(idx=0; idx<gTotalSwitch; idx++) {
        if( check_bit(idx, buf) == 0 ) break;
    }
    printf((char *)"%s -> ", (idx == gTotalSwitch) ? "PASS" : "FAIL");
    for(uint8_t k=0; k<gMaxBytes; k++) {
        printf((char *)"0x%02X%c", *buf++, (k==(gMaxBytes-1))?'\n':' ');
    }
    val = mSW_SINGLE_SWITCH_READ(0, 0);   // switch[0] read.
    printf((char *)"\tSwitch[0](0x%X) : Switch %s, Hold %s\n", val, 
            (val & 0x1) ? "ON" : "OFF", (val & 0x2) ? "Enable" : "Disable");

    mSW_ALL_RELEASE_ON();
    buf = mSW_ALL_SWITCH_ONOFF_READ(0);   // all switch onoff read.
    printf((char *)"\tAll Switch OnOff : ");
    for(idx=0; idx<gTotalSwitch; idx++) {
        if( check_bit(idx, buf) == 0 ) break;
    }
    printf((char *)"%s -> ", (idx == gTotalSwitch) ? "PASS" : "FAIL");
    for(uint8_t k=0; k<gMaxBytes; k++) {
        printf((char *)"0x%02X%c", *buf++, (k==(gMaxBytes-1))?'\n':' ');
    }
    buf = mSW_ALL_SWITCH_HOLD_READ(0);   // all switch onoff read.
    printf((char *)"\tAll Switch Hold : ");
    for(idx=0; idx<gTotalSwitch; idx++) {
        if( check_bit(idx, buf) == 1 ) break;
    }
    printf((char *)"%s -> ", (idx == gTotalSwitch) ? "PASS" : "FAIL");
    for(uint8_t k=0; k<gMaxBytes; k++) {
        printf((char *)"0x%02X%c", *buf++, (k==(gMaxBytes-1))?'\n':' ');
    }

    val = mSW_SINGLE_SWITCH_READ(0, 0);   // switch[0] read.
    printf((char *)"\tSwitch[0](0x%X) : Switch %s, Hold %s\n", val, 
            (val & 0x1) ? "ON" : "OFF", (val & 0x2) ? "Enable" : "Disable");

    mSW_ALL_OFF();
    buf = mSW_ALL_SWITCH_ONOFF_READ(0);   // all switch onoff read.
    printf((char *)"\tAll Switch OnOff : ");
    for(idx=0; idx<gTotalSwitch; idx++) {
        if( check_bit(idx, buf) == 1 ) break;
    }
    printf((char *)"%s -> ", (idx == gTotalSwitch) ? "PASS" : "FAIL");
    for(uint8_t k=0; k<gMaxBytes; k++) {
        printf((char *)"0x%02X%c", *buf++, (k==(gMaxBytes-1))?'\n':' ');
    }
    buf = mSW_ALL_SWITCH_HOLD_READ(0);   // all switch onoff read.
    printf((char *)"\tAll Switch Hold : ");
    for(idx=0; idx<gTotalSwitch; idx++) {
        if( check_bit(idx, buf) == 1 ) break;
    }
    printf((char *)"%s -> ", (idx == gTotalSwitch) ? "PASS" : "FAIL");
    for(uint8_t k=0; k<gMaxBytes; k++) {
        printf((char *)"0x%02X%c", *buf++, (k==(gMaxBytes-1))?'\n':' ');
    }

    val = mSW_SINGLE_SWITCH_READ(0, 0);   // switch[0] read.
    printf((char *)"\tSwitch[0](0x%X) : Switch %s, Hold %s\n", val, 
            (val & 0x1) ? "ON" : "OFF", (val & 0x2) ? "Enable" : "Disable");
}


__near_func void mSwitch_Test(void)
{
	uint8_t i, cnt;
//	uint8_t status;
	uint8_t group;

	printf((char *)"\n\n%s() : Start!!!\n", __func__);

#if 1	///< 64-bit union test. -> union은 사용할 수 없다.(1-byte를 넘어가면, MSB와 연결되지 않고, LSB와 연결된다.
#endif

	Debug.flag = 0;
	Debug.gEmulator = 1;
	Debug.gTransmit = 1;
	Debug.gReceive = 1;
	Debug.gCheck = 0;
	Debug.cs_gen = 0;

#if 1   // khjung
    getchar();
    {
        uint8_t ch, i;
        Byte4 sw;
        uint8_t tx[16], rx[16];

        sw.l = 0;
        do {
            puts((char *)"\n 1. Chip Initialize.");
            puts((char *)" 2. ID Assignment.");
            puts((char *)" 3. All Switch Toggle.");
            puts((char *)" 4. Single Switch Toggle.");
            puts((char *)" 5. .");
            puts((char *)" 6. .");
            puts((char *)" 0. Pattern Reset.");

            ch = getchar();

            if( ch == '1' ) {
                for(uint8_t k=0; k<100; k++) {
                    for(uint8_t j=0; j<100; j++) {
            		    SendByte(0x0);
                    }
                }
                cnt = 0;
            } else if( ch == '2' ) {
                uint8_t result;
                /*
                tx[0] = 0xAA;
                tx[1] = 0x37;
                tx[2] = 0xFF;
                tx[3] = 0x00;
                tx[4] = 0x00;
                tx[5] = 0x00;
                cnt = 6;
                */
                cnt = 0;
	            result = mSW_CHIP_ADDRESS_ASSIGN(0xFF);
                printf((char *)"Result : 0x%02X\n", result);
//	            Chip_Address_Assignment_Test();		///< test.c
            } else if( ch == '3' ) {
                if( sw.b[3] & (1<<7) ) {
                    mSW_ALL_OFF();
                } else {
                    mSW_ALL_ON();
                }
                /*
                tx[0] = 0xAA;
                tx[1] = (sw.b[3] & (1<<7)) ? 0x00 : 0x01;
                tx[2] = 0x00;
                tx[3] = 0x00;
                cnt = 4;
                */
                cnt = 0;
                sw.b[3] ^= (1<<7);
            } else if( ch == '4' ) {
                uint8_t pos;
                printf((char *)"Switch Number ? ");
                pos = getnum();

                printf((char *)"%d-switch toggle(Exit:ESC)\n", pos);
                do {
                    if( sw.b[pos/8] & (1<<(pos%8)) ) {
                        mSW_SINGLE_SWITCH_OFF(0, pos);
                    } else {
                        mSW_SINGLE_SWITCH_ON(0, pos);
                    }
                    cnt = 0;
                    sw.b[pos/8] ^= (1<<(pos%8));

                    /*
                    tx[0] = 0xAA;
                    tx[1] = ( sw.b[pos/8] & (1<<(pos%8)) ) ? 0x08 : 0x09;
                    tx[2] = pos;
                    tx[3] = 0x00;
                    tx[4] = 0x00;
                    cnt = 5;
                    sw.b[pos/8] ^= (1<<(pos%8));

                    for(i=0; i<cnt; i++) {
                        rx[i] = SendByte(tx[i]);
                    }
                    */
                } while( getchar() != ESC );
            } else if( ch == '5' ) {
            } else if( ch == '6' ) {
            } else if( ch == '7' ) {
            } else if( ch == '0' ) {
                mSW_PATTERN_RESET();
                /*
                for(uint8_t k=0; k<7; k++) {
                    tx[k] = 0xFF;
                }
                tx[7] = 0x00;
                tx[8] = 0x00;
                tx[9] = 0x00;
                tx[10] = 0x00;
                cnt = 11;
                */
                cnt = 0;
            }

            printf((char *)"TX(%d)[", cnt);
            for(i=0; i<cnt; i++) {
                printf((char *)"0x%02X ", tx[i]);
            }

            for(i=0; i<cnt; i++) {
                rx[i] = SendByte(tx[i]);
            }

            printf((char *)"] -> RX[");
            for(i=0; i<cnt; i++) {
                printf((char *)"0x%02X ", rx[i]);
            }
            printf((char *)"]\n");
        } while( ch != ESC );
    }
#else
    mSW_PATTERN_RESET();
    mSW_PATTERN_RESET();
#endif
	printf((char *)"\n\n%s() : Initialize Done!!!\n", __func__);
	mSwitch_Init();

	Chip_Address_Assignment_Test();		///< test.c

	// Group Type initialize.
	printf((char *)"select branch type(0 ~ 5) ? ");
	group = getnum();
//	group = (DEFAULT_BRANCH << 5);					///< 10-branch
	mSW_SET_GROUP_TYPE(group << 5);
	INFO("Group Type(%d-branch)\n", BitCount[eSW[0].GroupType].row);

	INFO("%d-row, %d-column\n", get_row_count(0), get_column_count(0));


//	function_test();					///< test.c

//	Simple_Test();

	Switch_Command_Test(0);

	printf((char *)"---------- %s() : Multi-Switch Long-Run Test. ----------\n", __func__);
	do {
		/* data frame clear */
		for(i=0; i<MAX_FRAME; i++) gFrame[i] = 0;

		/* data frame generation */
		cnt = create_command(gFrame);			///< Multi-Switch.c

		/* send data to Multi-switch */
		send_command(gFrame, cnt);			///< Multi-Switch.c

		/* read Multi-switch status */
//		status = get_status();

		/* verify */
//		verify_switch(gFrame, status);

	} while( getchar() != ESC );
}

/*	EOF	*/
