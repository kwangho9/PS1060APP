/**
 * @file eSW_emulator.c
 * @brief Multi Switch Emulator function's.
 * @author khjung ( khjung@leolsi.com )
 * @version 0.1
 * @date 2019-08-02
 * @see ������ ������� ���⿡ ����Ѵ�.
 * @todo �߰������� �ؾ� �� ����.
 * @deprecated ���� ���� ������ ������� ǥ����.
 */

#define __eSW_emulator_c__
#include "includes.h"

const uint8_t SFR_ADDR[] = {0x50, 0x52, 0x54, 0x56, 0x58, 0x5A, 0x60, 0x62, 0x64, 0x66, 0x68, 0x70, 0x72, 0x80, 0x82, 0x84};

uint8_t SFR_INDEX(uint8_t addr)
{
	switch(addr) {
		case 0x50: return 0;
		case 0x52: return 1;
		case 0x54: return 2;
		case 0x56: return 3;
		case 0x58: return 4;
		case 0x5A: return 5;
		case 0x60: return 6;
		case 0x62: return 7;
		case 0x64: return 8;
		case 0x66: return 9;
		case 0x68: return 10;
		case 0x70: return 11;
		case 0x72: return 12;
		case 0x80: return 13;
		case 0x82: return 14;
		case 0x84: return 15;
		case 0x90: return 16;
		case 0x92: return 17;
		case 0x94: return 18;
		case 0x96: return 19;
	}

	return -1;
}

/*! @fn uint8_t get_row_count(uint8_t id)
 *
 * @param id Multi-Switch Address.
 *
 * @return channel�� ����
 */
uint8_t get_row_count(uint8_t id)
{
	return BitCount[eSW[id].GroupType].row;
//	return eSW[id].GroupType;
}

/*! @fn uint8_t get_group_count(uint8_t id)
 *
 * @param id Multi-Switch Address.
 *
 * @return group�� ����
 */
uint8_t get_column_count(uint8_t id)
{
	return BitCount[eSW[id].GroupType].column;
//	uint8_t gt = eSW[id].GroupType;
//
//	return ((MAX_SWITCH / gt) + ((MAX_SWITCH % gt) ? 1 : 0));
}

/*! @fn uint8_t get_switch_count_in_row(uint8_t id)
 * @brief 1-Row�� ����ִ� Switch�� ������ �˷��ش�.(�б⿡ ���� �޶�����.)
 *
 * @param id Multi-Switch Address.
 *
 * @return Row�� switch�� ����.
 */
uint8_t get_switch_count_in_row(uint8_t id)
{
	return BitCount[eSW[id].GroupType].column;
//	uint8_t gt = eSW[id].GroupType;
//
//	return ((MAX_SWITCH / gt) + ((MAX_SWITCH % gt) ? 1 : 0));
}

/*! @fn uint8_t get_switch_count_in_column(uint8_t id)
 * @brief Group�� Swtich�� ����(��, �б���� ã�´�.)
 *
 * @param id Multi-Switch Address.
 *
 * @return Group�� switch�� ����.
 */
uint8_t get_switch_count_in_column(uint8_t id)
{
	return BitCount[eSW[id].GroupType].row;
//	return eSW[id].GroupType;
}


/**
 * @brief Ư�� group�� ��ġ�� switch number(0~MAX_SWITCH)�� ã�´�.
 *
 * @param id Multi-Switch Chip Address.
 * @param nColumn switch�� ���Ե� Column�� number(0~63).
 * @param nSwitch Column���� switch�� number(0~63).
 *
 * @return switch number(0~MAX_SWITCH)
 */
uint8_t get_switch_number_in_column(uint8_t id, uint8_t nColumn, uint8_t nSwitch)
{
//	uint8_t ret;

//	ret = (nColumn * eSW[id].GroupType) + nSwitch;
//	printf((char *)"\n%s() : column(%d), switch(%d) = %d\n", __func__, nColumn, nSwitch, ret);
//	return ret;

//	return (nColumn * eSW[id].GroupType) + nSwitch;
	return (nColumn * BitCount[eSW[id].GroupType].row) + nSwitch;
}

/**
 * @brief Ư�� channel�� ��ġ�� switch number(0~MAX_SWITCH)�� ã�´�.
 *
 * @param id Multi-Switch Chip Address.
 * @param nChannel Ư�� channel�� number(0~12)
 * @param nSwitch Ư�� channel������ switch�� number(0~63)
 *
 * @return switch number(0~MAX_SWITCH)
 */
uint8_t get_switch_number_in_row(uint8_t id, uint8_t nRow, uint8_t nSwitch)
{
//	uint8_t ret;

//	ret = nChannel + (nSwitch * GroupType);
//	printf((char *)"%d-channel, %d-switch = %d-switch\n", nChannel, nSwitch, ret);
//	return ret;

//	return (nRow + (nSwitch * eSW[id].GroupType));
	return (nRow + (nSwitch * BitCount[eSW[id].GroupType].row));
}

uint8_t get_column_number(uint8_t id, uint8_t sw)
{
//	return sw / eSW[id].GroupType;
	return sw / BitCount[eSW[id].GroupType].row;
}

uint8_t get_row_number(uint8_t id, uint8_t sw)
{
//	return sw % eSW[id].GroupType;
	return sw % BitCount[eSW[id].GroupType].row;
}

/**
 * @brief Multi-switch���� Ư�� switch�� ���ؼ� ����(opcode)�� �����Ѵ�.
 * @details 'more'�� ������ �������� ����.\n
 * HOLD ����� Hold���� ���� switch�� ���ؼ� Hold ���¸� �����ϰ�, Switch�� ON/OFF ��Ų��.\n
 *         Hold�Ǿ� �ִ� switch�� ���´� �������� �ʴ´�.
 * RELEASE ����� Hold�� switch�� ���ؼ��� Hold ���¸� �����ϰ�, Switch�� ON/OFF ��Ų��.\n
 * FORCE ����� Hold�� ������� ��� switch�� Hold�� �����ϰ�, Switch�� ON/OFF ��Ų��.\n
 *
 * @param id Multi-Switch Address.
 * @param opcode switch�� ������ ����.
 * @param sw switch number(0~MAX_SWITCH)
 *
 * @bug �� �Լ����� ���� Bug�� �߰����� ���ߴ�.
 * @warning ���ϰ��� ���� ������� �ʾҴ�.
 * @todo 1���� switch�� ���ؼ��� �����ϴµ�, ���Ŀ��� multi switch�� �����ؾ� �Ѵ�.
 *
 * @return -1 ERROR
 *          0 OK
 */
uint8_t emul_set_switch_ctrl(uint8_t id, uint8_t opcode, uint8_t sw)
{
	if( sw > (MAX_SWITCH-1) ) return -1;			///< switch number error.

#if (SWITCH_ACCESS_SIZE == BYTE_ACCESS)	///< switch ���¸� Byte ������ ó���ϱ� ���� ����.
	if( opcode == 0 ) {				///< switch OFF command.
		if( eSW[id].bSwitch[sw].hold ) return 1;	///< check switch HOLD.
		eSW[id].bSwitch[sw].Info = 0x00;			///< switch OFF
	} else if( opcode == 1 ) {		///< switch ON command.
		if( eSW[id].bSwitch[sw].hold ) return 1;	///< check switch HOLD.
		eSW[id].bSwitch[sw].Info = 0x01;			///< switch ON
	} else if( opcode == 2 ) {		///< switch HOLD OFF command.
		if( eSW[id].bSwitch[sw].hold ) return 1;	///< check switch HOLD.
		eSW[id].bSwitch[sw].Info = 0x02;			///< switch HOLD OFF.
	} else if( opcode == 3 ) {		///< switch HOLD ON command.
		if( eSW[id].bSwitch[sw].hold ) return 1;	///< check switch HOLD.
		eSW[id].bSwitch[sw].Info = 0x03;			///< switch HOLD ON.
	} else if( opcode == 4 ) {		///< switch RELEASE OFF command.
		if( eSW[id].bSwitch[sw].hold == 0 ) return 1;	///< check switch not HOLD.
		eSW[id].bSwitch[sw].Info = 0x00;			///< switch RELEASE OFF.
	} else if( opcode == 5 ) {		///< switch RELEASE ON command.
		if( eSW[id].bSwitch[sw].hold == 0 ) return 1;	///< check switch not HOLD.
		eSW[id].bSwitch[sw].Info = 0x01;			///< switch RELEASE ON.
	} else if( opcode == 6 ) {		///< switch FORCE OFF command.
		eSW[id].bSwitch[sw].Info = 0x00;			///< switch FORCE OFF.
	} else if( opcode == 7 ) {		///< switch FORCE ON command.
		eSW[id].bSwitch[sw].Info = 0x01;			///< switch FORCE ON.
	}
#elif (SWITCH_ACCESS_SIZE == BIT_ACCESS)	///< switch ���¸� bit ������ ó���ϱ� ���� ����.
	if( opcode == 0 ) {				///< switch OFF command.
		if( check_bit(sw, &eSW[id].bHold[0]) ) return 1;	///< check switch HOLD.
		clear_bit(sw, &eSW[id].bSwitch[0]);				///< switch OFF
	} else if( opcode == 1 ) {		///< switch ON command.
		if( check_bit(sw, &eSW[id].bHold[0]) ) return 1;	///< check switch HOLD.
		set_bit(sw, &eSW[id].bSwitch[0]);					///< switch ON
	} else if( opcode == 2 ) {		///< switch HOLD OFF command.
		if( check_bit(sw, &eSW[id].bHold[0]) ) return 1;	///< check switch HOLD.
		clear_bit(sw, &eSW[id].bSwitch[0]);				///< switch OFF
		set_bit(sw, &eSW[id].bHold[0]);					///< HOLD flag set.
	} else if( opcode == 3 ) {		///< switch HOLD ON command.
		if( check_bit(sw, &eSW[id].bHold[0]) ) return 1;	///< check switch HOLD.
		set_bit(sw, &eSW[id].bSwitch[0]);					///< switch ON
		set_bit(sw, &eSW[id].bHold[0]);					///< HOLD flag set.
	} else if( opcode == 4 ) {		///< switch RELEASE OFF command.
		if( check_bit(sw, &eSW[id].bHold[0]) == 0) return 1;	///< check switch not HOLD.
		clear_bit(sw, &eSW[id].bSwitch[0]);				///< switch OFF
		clear_bit(sw, &eSW[id].bHold[0]);				///< HOLD flag clear.
	} else if( opcode == 5 ) {		///< switch RELEASE ON command.
		if( check_bit(sw, &eSW[id].bHold[0]) == 0) return 1;	///< check switch not HOLD.
		set_bit(sw, &eSW[id].bSwitch[0]);					///< switch ON
		clear_bit(sw, &eSW[id].bHold[0]);				///< HOLD flag clear.
	} else if( opcode == 6 ) {		///< switch FORCE OFF command.
		clear_bit(sw, &eSW[id].bSwitch[0]);				///< switch OFF
		clear_bit(sw, &eSW[id].bHold[0]);				///< HOLD flag clear.
	} else if( opcode == 7 ) {		///< switch FORCE ON command.
		set_bit(sw, &eSW[id].bSwitch[0]);					///< switch ON
		clear_bit(sw, &eSW[id].bHold[0]);				///< HOLD flag clear.
	}
#endif

	return 0;			///< operate Done.
}

/**
 * @brief group ������ Ư�� switch�� ON/OFF/HOLD ��Ų��.
 *
 * @param id Multi-Switch Address.
 * @param opcode switch operation command.
 * @param g group number.
 * @param s switch number.
 *
 * @return -1 ERROR
 *          0 OK
 */
uint8_t emul_set_switch_ctrl_in_column(uint8_t id, uint8_t opcode, uint8_t column, uint8_t sw)
{
	return emul_set_switch_ctrl(id, opcode, get_switch_number_in_column(id, column, sw));
}

/**
 * @brief channel ������ Ư�� switch�� ON/OFF/HOLD ��Ų��.
 *
 * @param id Multi-Switch Address.
 * @param opcode switch operation command.
 * @param c channel number.
 * @param s switch number.
 *
 * @return -1 ERROR
 *          0 OK
 */
uint8_t emul_set_switch_ctrl_in_row(uint8_t id, uint8_t opcode, uint8_t row, uint8_t sw)
{
	return emul_set_switch_ctrl(id, opcode, get_switch_number_in_row(id, row, sw));
}

/**
 * @brief group ������ ��� switch�� ON/OFF/HOLD ��Ų��.
 *
 * @param id Multi-Switch Address.
 * @param opcode switch operation command.
 * @param g column number.
 *
 * @return -1 ERROR
 *          0 OK
 */
uint8_t emul_set_column_ctrl(uint8_t id, uint8_t opcode, uint8_t column)
{
	uint8_t sn = get_switch_number_in_column(id, column, 0);

	for(uint8_t i=0; i<get_switch_count_in_column(id); i++) {
		emul_set_switch_ctrl(id, opcode, sn+i);
	}

	return 0;
}

/**
 * @brief channel ������ ��� switch�� ON/OFF/HOLD ��Ų��.
 *
 * @param id Multi-Switch Address.
 * @param opcode switch operation command.
 * @param c channel number.
 *
 * @return -1 ERROR
 *          0 OK
 */
uint8_t emul_set_row_ctrl(uint8_t id, uint8_t opcode, uint8_t row)
{
	uint8_t nSw;
	uint8_t nRow = get_switch_count_in_row(id);

//	printf((char *)"%s() : %d-row -> ", __func__, row);
	for(uint8_t i=0; i<nRow; i++) {
		nSw = get_switch_number_in_row(id, row, i);
//		printf((char *)"%d ", nSw);
		emul_set_switch_ctrl(id, opcode, nSw);
	}
//	printf((char *)"\n");

	return 0;
}



/**
 * @brief Ư�� switch�� HOLD & ON/OFF status�� read�Ѵ�.
 *
 * @param id Multi-Switch Address.
 * @param sw switch number.
 *
 * @return switch ON/OFF status.
 */
uint8_t emul_get_switch(uint8_t id, uint8_t sw)
{
#if (SWITCH_ACCESS_SIZE == BYTE_ACCESS)	///< switch ���¸� Byte ������ ó���ϱ� ���� ����.
	if( sw < MAX_SWITCH ) return (eSW[id].bSwitch[sw].Info & 0x3);
#elif (SWITCH_ACCESS_SIZE == BIT_ACCESS)	///< switch ���¸� bit ������ ó���ϱ� ���� ����.
	if( sw < MAX_SWITCH ) {
		uint8_t ret = 0;
		if( eSW[id].bSwitch[sw>>3] & msb_bit_mask[sw&0x7] ) ret |= 1;
		if( eSW[id].bHold[sw>>3] & msb_bit_mask[sw&0x7] ) ret |= 2;
		return ret;
	}
#endif

	return -1;
}

/**
 * @brief Ư�� switch�� ON/OFF status�� read�Ѵ�.
 *
 * @param id Multi-Switch Address.
 * @param sw switch number.
 *
 * @return switch ON/OFF status.
 */
bool emul_get_switch_onoff(uint8_t id, uint8_t sw)
{
#if (SWITCH_ACCESS_SIZE == BYTE_ACCESS)	///< switch ���¸� Byte ������ ó���ϱ� ���� ����.
	if( sw < MAX_SWITCH ) return (eSW[id].bSwitch[sw].Info & 0x1);
#elif (SWITCH_ACCESS_SIZE == BIT_ACCESS)	///< switch ���¸� bit ������ ó���ϱ� ���� ����.
	if( sw < MAX_SWITCH ) return (eSW[id].bSwitch[sw>>3] & msb_bit_mask[sw&7]);
#endif

	return -1;
}

/**
 * @brief Ư�� switch�� HOLD status�� read�Ѵ�.
 *
 * @param id Multi-Switch Address.
 * @param s switch number.
 *
 * @return switch HOLD status.
 */
bool emul_get_switch_hold(uint8_t id, uint8_t sw)
{
#if (SWITCH_ACCESS_SIZE == BYTE_ACCESS)	///< switch ���¸� Byte ������ ó���ϱ� ���� ����.
	if( sw < MAX_SWITCH ) return (eSW[id].bSwitch[sw].Info & 0x2);
#elif (SWITCH_ACCESS_SIZE == BIT_ACCESS)	///< switch ���¸� bit ������ ó���ϱ� ���� ����.
	if( sw < MAX_SWITCH ) return (eSW[id].bHold[sw>>3] & msb_bit_mask[sw&7]);
#endif

	return -1;
}


/**
 * @brief column number ������ Ư�� switch�� ON/OFF status�� read�Ѵ�.
 *
 * @param id Multi-Switch Address.
 * @param nColumn column number.
 * @param nSwitch switch number.
 *
 * @return switch ON/OFF status.
 */
bool emul_get_switch_onoff_in_column(uint8_t id, uint8_t nColumn, uint8_t nSwitch)
{
	return emul_get_switch_onoff(id, get_switch_number_in_column(id, nColumn, nSwitch));
}

/**
 * @brief row number ������ Ư�� switch�� ON/OFF status�� read�Ѵ�.
 *
 * @param id Multi-Switch Address.
 * @param nRow row number.
 * @param nSwitch switch number.
 *
 * @return switch ON/OFF status.
 */
bool emul_get_switch_onoff_in_row(uint8_t id, uint8_t nRow, uint8_t nSwitch)
{
	return emul_get_switch_onoff(id, get_switch_number_in_row(id, nRow, nSwitch));
}

/**
 * @brief group number ������ Ư�� switch�� HOLD status�� read�Ѵ�.
 *
 * @param id Multi-Switch Address.
 * @param nColumn column number.
 * @param nSwitch switch number.
 *
 * @return switch HOLD status.
 */
bool emul_get_switch_hold_in_column(uint8_t id, uint8_t nColumn, uint8_t nSwitch)
{
	return emul_get_switch_hold(id, get_switch_number_in_column(id, nColumn, nSwitch));
}

/**
 * @brief row number ������ Ư�� switch�� HOLD status�� read�Ѵ�.
 *
 * @param id Multi-Switch Address.
 * @param nRow row number.
 * @param nSwitch switch number.
 *
 * @return switch HOLD status.
 */
bool emul_get_switch_hold_in_row(uint8_t id, uint8_t nRow, uint8_t nSwitch)
{
	return emul_get_switch_hold(id, get_switch_number_in_row(id, nRow, nSwitch));
}


uint8_t result[MAX_BYTES];








/**
 * @brief Multi-Switch ����ü�� ����� ������.
 *
 * @param frame Command Frame.
 *
 * @return 
 */
__near_func uint8_t eSwitch_Emulator(uint8_t id, uint8_t *frame)
{
	uint8_t i, type, idx = 0;
//	uint8_t tmp[MAX_BYTES];
	tCommand cmd;

	///< check sync pattern.
	if( frame[idx++] != 0xAA ) return 0;

	// Chip Address Assignment check.

	// command byte parse.
	cmd.CMD = frame[idx++];

	if( cmd.mode ) {				///< Test Command.
		uint8_t chip, bits, ret;
		uint8_t column, row;

		if( cmd.cs ) {
			if( (cmd.CMD == 0xC0) || (cmd.CMD == 0xC1) ) {
				chip = frame[idx++];
			} else {
				chip = (frame[idx] & 0xF0) >> 4;
			}
			if( chip != id ) return 0;
		}

		if( cmd.CMD & (1<<5) ) {			///< read column number.
			column = frame[idx++] & 0x0F;
//			sw = get_switch_number_in_column(chip, column, 0);
			bits = get_switch_count_in_column(chip);
		} else if( cmd.CMD & (1<<4) ) {	///< read row number.
			row = frame[idx++] & 0x0F;
//			sw = get_switch_number_in_row(chip, row, 0);
			bits = get_switch_count_in_row(chip);
		} else {
			bits = MAX_SWITCH;
		}

		if( cmd.opcode == 0 ) {				///< FTP/SFR read command.
			uint8_t sw;
			uint8_t addr = frame[idx++];
//			if( addr < FTP_CTRL ) {
//				gStatus[0] = eSW[chip].FTP[addr];
//			} else {
				switch( addr ) {
					case SW_STATUS:
						gStatus[0] = 0;
						sw = sSW[chip].SFR[SFR_INDEX(SW_NUM)];
						if( emul_get_switch_onoff(chip, sw) ) {
							gStatus[0] |= (1<<0);
						}
						if( emul_get_switch_hold(chip, sw) ) {
							gStatus[0] |= (1<<1);
						}
						break;
					default:
						gStatus[0] = eSW[chip].SFR[SFR_INDEX(addr)];
						break;
				}
//			}
			ret = 1;
		} else if( cmd.opcode == 1 ) {		///< FTP/SFR write command.
			uint8_t addr, data;

			addr = frame[idx++];	///< read SFR address.
			data = frame[idx++];	///< read SFR data.

//			if( addr < FTP_CTRL ) {
//				eSW[chip].FTP[addr] = data;
//			} else {
				eSW[chip].SFR[SFR_INDEX(addr)] = data;
//			}

//			printf((char *)"%s() : chip = %d, addr = 0x%x, data = 0x%x\n", __func__, chip, addr, data);
		} else if( cmd.opcode == 2 ) {			///< switch on/off status read.
			for(i=0; i<MAX_BYTES; i++) gStatus[i] = 0;

			if( cmd.CMD & (1<<4) ) {
				for(i=0; i<bits; i++) {
					if( emul_get_switch_onoff_in_row(chip, row, i) ) {
						set_bit(i, gStatus);						///< conversion bit format : MSB
					}
				}
			} else if( cmd.CMD & (1<<5) ) {
				for(i=0; i<bits; i++) {
					if( emul_get_switch_onoff_in_column(chip, column, i) ) {
						set_bit(i, gStatus);						///< conversion bit format : MSB
					}
				}
			} else {
				for(i=0; i<bits; i++) {
					if( emul_get_switch_onoff(chip, i) ) {
						set_bit(i, gStatus);
					}
				}
			}
			ret = (bits >> 3) + ((bits & 0x7)?1:0);
		} else if( cmd.opcode == 4 ) {			///< switch hold status read.
			for(i=0; i<MAX_BYTES; i++) gStatus[i] = 0;

			if( cmd.CMD & (1<<4) ) {
				for(i=0; i<bits; i++) {
					if( emul_get_switch_hold_in_row(chip, row, i) ) {
						set_bit(i, gStatus);						///< conversion bit format : MSB
					}
				}
			} else if( cmd.CMD & (1<<5) ) {
				for(i=0; i<bits; i++) {
					if( emul_get_switch_hold_in_column(chip, column, i) ) {
						set_bit(i, gStatus);						///< conversion bit format : MSB
					}
				}
			} else {
				for(i=0; i<bits; i++) {
					if( emul_get_switch_hold(chip, i) ) {
						set_bit(i, gStatus);
					}
				}
			}
			ret = (bits >> 3) + ((bits & 0x7)?1:0);
		}

		return ret;
	} else {							///< User Command.
		if( cmd.cs ) {		///< CS(chip select) frame�� �����.
			///< command frame���� CS(chip select)�� Multi-Switch ����ü�� CS(chip select)����Ͽ� ���� ��쿡�� command frame�� ���۽�Ų��.
			if( (frame[idx++] & eSW[id].ChipSelect) == 0 ) return 0;
		}
//		printf((char *)"[ %d-chip ]\n", id);

		type = cmd.type;
		if( type == 0 ) {				///< single command.
			for(i=0; i<MAX_SWITCH; i++) {
				emul_set_switch_ctrl(id, cmd.opcode, i);
			}
		} else if( type == 1 ) {			///< 1-switch command.
			emul_set_switch_ctrl(id, cmd.opcode, frame[idx]);
		} else if( type == 2 ) {			///< row select only command.
			uint8_t eRow = get_row_count(id);
			for(i=0; i<eRow; i++) {
				if( check_bit(i, &frame[idx]) == 1 ) {
					emul_set_row_ctrl(id, cmd.opcode, i);
				}
			}
		} else if( type == 3 ) {			///< row select & switch select command.
			uint8_t row, sw;
			uint8_t eRow = get_row_count(id);
			uint8_t nSW = get_switch_count_in_row(id);

			for(row=0; row<eRow; row++) {
				if( check_bit(row, &frame[idx]) == 0 ) continue;

				for(i=0; i<nSW; i++) {
					if( check_bit(eRow+i, &frame[idx]) == 0 ) continue;

					sw = get_switch_number_in_row(id, row, i);
					emul_set_switch_ctrl(id, cmd.opcode, sw);
				}
			}
		} else if( type == 4 ) {	///< column select only command.
			uint8_t eColumn = get_column_count(id);
			for(uint8_t column=0; column<eColumn; column++) {
				if( check_bit(column, &frame[idx]) == 1 ) {
					emul_set_column_ctrl(id, cmd.opcode, column);
				}
			}
		} else if( type == 5 ) {	///< column select & switch select command.
			uint8_t sw;
			uint8_t eColumn = get_column_count(id);
			uint8_t nSW = get_switch_count_in_column(id);
			for(uint8_t column=0; column<eColumn; column++) {
				if( check_bit(column, &frame[idx]) == 0 ) continue;

//				printf((char *)"%d-column, switch : ", column);
				for(i=0; i<nSW; i++) {
					if( check_bit(eColumn+i, &frame[idx]) == 0 ) continue;

					sw = get_switch_number_in_column(id, column, i);
//					printf((char *)"%d ", sw);
					emul_set_switch_ctrl(id, cmd.opcode, sw);
				}
//				printf((char *)"\n");
			}
		} else if( type == 6 ) {	///< special command.
			if( cmd.opcode == 0 ) {
			} else if( cmd.opcode == 1 ) {
//				GroupType = c_GroupType[(frame[idx++] & 0xE0) >> 5];
				GroupType = (frame[idx++] & 0xE0) >> 5;
				eSW[id].GroupType = GroupType;
//				printf((char *)"%d. Group Type = %d-branch\n", id, BitCount[eSW[id].GroupType].row);
				return idx;
			} else if( cmd.opcode == 2 ) {
			} else if( cmd.opcode == 3 ) {
			} else if( cmd.opcode == 4 ) {
			} else if( cmd.opcode == 5 ) {
			} else if( cmd.opcode == 6 ) {
			} else if( (cmd.opcode == 7) && (eSW[id].ChipSelect == 0) ) {
				uint8_t tmp = frame[idx] & 0xFF;
				for(uint8_t i=0; i<8; i++) {
					if( tmp & (1 << (7 - i)) ) {
						frame[idx] ^= (1 << (7 - i));
						eSW[id].ChipSelect = (1 << (7 - i));
//						printf((char *)"%d-Chip Address Assignment!\n", i);
						break;
					}
				}
				idx++;
				return idx;
			}
		} else if( cmd.type == 0x7 ) {	///< ���������� all switch Control
			for(i=0; i<MAX_SWITCH; i++) {
				if( check_bit(i, &frame[idx]) == 1 ) {
					emul_set_switch_ctrl(id, cmd.opcode, i);
				}
			}
		}
	}

	return 0;
}


void emulator_switch_dump(uint8_t cs)
{
	uint8_t i, chip, column, row;
	uint8_t eColumn, eRow;

	putchar('\n');
	for(i=0; i<40; i++) putchar('-');
	printf((char *)" Emulator Switch Status ");
	for(i=0; i<40; i++) putchar('-');
	putchar('\n');

	for(chip=0; chip<MAX_CHIP; chip+=4) {
		eColumn = get_column_count(chip);
		eRow = get_row_count(chip);
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

				for(row=0; row<eRow; row++) {
					if( emul_get_switch_onoff_in_column(i, column, row) ) {
						printf((char *)"*");
					} else {
						printf((char *)".");
					}
				}
				printf((char *)"(");
				for(row=0; row<eRow; row++) {
					if( emul_get_switch_hold_in_column(i, column, row) ) {
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

void emulator_init(void)
{
	uint8_t i, j;

	INFO("emulator struct initialize.\n");
	// tMultiSwitch struct initialize.
	for(i=0; i<MAX_CHIP; i++) {
		eSW[i].ChipSelect = 0;
		eSW[i].GroupType = 0;
#if (SWITCH_ACCESS_SIZE == BYTE_ACCESS)	///< switch ���¸� Byte ������ ó���ϱ� ���� ���.
		for(j=0; j<MAX_SWITCH; j++) {
			eSW[i].bSwitch[j].Info = 0;
		}
#elif (SWITCH_ACCESS_SIZE == BIT_ACCESS)	///< switch ���¸� bit ������ ó���ϱ� ���� ���.
		for(j=0; j<MAX_BYTES; j++) {
			eSW[i].bSwitch[j] = 0;
			eSW[i].bHold[j] = 0;
		}
#endif
		for(j=0; j<20; j++) {
//			eSW[i].FTP[j] = 0;
			eSW[i].SFR[j] = 0;
		}
	}
}

