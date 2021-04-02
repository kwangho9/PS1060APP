/**
 * @file eSW_emulator.h
 * @brief Multi Switch Emulator header's
 * @author khjung ( khjung@leolsi.com )
 * @version 0.1
 * @date 2019-08-02
 * @see 참고할 내용들을 여기에 기술한다.
 * @todo 추가적으로 해야 할 사항.
 * @deprecated 향후 삭제 예정인 내용들을 표시함.
 */

#ifndef __eSW_emulator_h__
#define __eSW_emulator_h__

#ifdef __eSW_emulator_c__
#define Emulator_EXT
#else	/* not defined __eSW_emulator_c__ */
#define Emulator_EXT	extern
#endif	/* __eSW_emulator_c__ */


typedef union _switch_info {
	uint8_t Info;
	struct {
		uint8_t onoff : 1;		///< bit[0] : switch on/off status.
		uint8_t hold : 1;		///< bit[1] : switch hold status.
		uint8_t reserved : 6;
	};
} tSwitchInfo;

/*!
 * @struct _MultiSwitchIC
 * 128-channel Multi-Switch Emulator Variable.
 */
struct _MultiSwitchIC {
	/*! @var uint8_t ChipSelect
	 * 각 Multi-Switch별로 Chip Address bit-field를 저장한다.(0x01 ~ 0x80)
	 */
	uint8_t ChipSelect;

	/*! @var uint8_t GroupType
	 * 각 Multi-Switch별로 분기 구성을 저장한다.
	 * @todo 분기 구성은 모든 Chip에 동일하게 적용되므로, 삭제할 예정임.
	 */
	uint8_t GroupType;

#if (SWITCH_ACCESS_SIZE == BYTE_ACCESS)	///< switch 상태를 Byte 단위로 처리하기 위한 변수.
	tSwitchInfo bSwitch[MAX_SWITCH];
#elif (SWITCH_ACCESS_SIZE == BIT_ACCESS)	///< switch 상태를 bit 단위로 처리하기 위한 변수.
	union _Switch {
		uint8_t bSwitch[MAX_BYTES];
	};
	union _Hold {
		uint8_t bHold[MAX_BYTES];
	};
#endif
	/*! @var uint8_t FTP[8]
	 * 각 Multi-Switch별로 고유한 값은 저장한다.(Trim 값)
	 * @todo Flash 용량은 변경될 필요가 있다.
	 */
//	uint8_t FTP[4];

	/*! @var uint8_t SFR[8]
	 * 각 Multi-Switch별로 Internal register variable.
	 * @todo Multi-Switch의 Internal register format을 정의해야 한다.
	 */
	uint8_t SFR[20];
};
typedef struct _MultiSwitchIC tMultiSwitch;
/*!
 * @fn typedef _MultiSwitchIC tMultiSwitch
 * 128-channel Multi-Switch IC Emulator.
 */


/*! @var tMultiSwitch eSW[MAX_CHIP]
 * Multi-Switch Chip Emulator 변수.
 * Multi-Switch를 실행하기 이전에 초기화되어야 한다.
 */
Emulator_EXT tMultiSwitch eSW[MAX_CHIP];


//extern uint8_t result[];

Emulator_EXT const uint8_t SFR_ADDR[];

Emulator_EXT uint8_t SFR_INDEX(uint8_t addr);

Emulator_EXT uint8_t get_row_count(uint8_t id);
Emulator_EXT uint8_t get_column_count(uint8_t id);
Emulator_EXT uint8_t get_switch_count_in_row(uint8_t id);
Emulator_EXT uint8_t get_switch_count_in_column(uint8_t id);

Emulator_EXT uint8_t get_switch_number_in_column(uint8_t id, uint8_t nColumn, uint8_t nSwitch);
Emulator_EXT uint8_t get_switch_number_in_row(uint8_t id, uint8_t nRow, uint8_t nSwitch);
Emulator_EXT uint8_t get_column_number(uint8_t id, uint8_t nSwitch);
Emulator_EXT uint8_t get_row_number(uint8_t id, uint8_t nSwitch);

Emulator_EXT uint8_t emul_set_switch_ctrl(uint8_t id, uint8_t opcode, uint8_t sw);
Emulator_EXT uint8_t emul_set_switch_ctrl_in_column(uint8_t id, uint8_t op, uint8_t column, uint8_t sw);
Emulator_EXT uint8_t emul_set_switch_ctrl_in_row(uint8_t id, uint8_t op, uint8_t row, uint8_t sw);
Emulator_EXT uint8_t emul_set_column_ctrl(uint8_t id, uint8_t opcode, uint8_t column);
Emulator_EXT uint8_t emul_set_row_ctrl(uint8_t id, uint8_t opcode, uint8_t row);

Emulator_EXT uint8_t emul_get_switch(uint8_t id, uint8_t nSwitch);
Emulator_EXT bool emul_get_switch_hold(uint8_t id, uint8_t nSwitch);
Emulator_EXT bool emul_get_switch_onoff(uint8_t id, uint8_t nSwitch);
Emulator_EXT bool emul_get_switch_hold_in_row(uint8_t id, uint8_t nRow, uint8_t nSwitch);
Emulator_EXT bool emul_get_switch_onoff_in_row(uint8_t id, uint8_t nRow, uint8_t nSwitch);
Emulator_EXT bool emul_get_switch_hold_in_column(uint8_t id, uint8_t nColumn, uint8_t nSwitch);
Emulator_EXT bool emul_get_switch_onoff_in_column(uint8_t id, uint8_t nColumn, uint8_t nSwitch);

Emulator_EXT void emulator_switch_dump(uint8_t cs);

Emulator_EXT uint8_t eSwitch_Emulator(uint8_t id, uint8_t *frame);

Emulator_EXT void emulator_init(void);


#endif	/* __eSW_emulator_h__ */

