//===================================================================
// File Name : Multi-Switch.h
// Function  : Multi-Switch
// Program   : B.H. Im & Kits   --  Boo-Ree Multimedia Inc.
// Date      : Oct, 25, 2005
// Version   : 0.0
// Mail      : support@boo-ree.com
// Web       : www.boo-ree.com
// History
//===================================================================

#ifndef __NewProject_h__
#define __NewProject_h__

#ifdef __NewProject_c__
#define mSwitch_EXT
#else
#define mSwitch_EXT extern
#endif


/*!
 * @def TX_FRAME_DUMP
 * transmit command frame을 dump한다.
 */
#define TX_FRAME_DUMP		1

/*!
 * @def EMULATOR
 * Multi-Switch Emulator를 제어한다.
 */
#define EMULATOR			1

/*!
 * @def RX_FRAME_DUMP
 * receive frame을 dump한다.
 */
#define RX_FRAME_DUMP		1

/*!
 * @def CHECK_FRAME
 * transmit frame과 receive frame을 비교한다.
 */
#define CHECK_FRAME			1



#define BIT_ACCESS		1
#define BYTE_ACCESS		8

#define SWITCH_ACCESS_SIZE		BIT_ACCESS
//#define SWITCH_ACCESS_SIZE		BYTE_ACCESS

/*!
 * @def MAX_CHIP
 * 최대 연결 가능한 Chip의 개수.
 */
#define MAX_CHIP		1

#define SHIFT_BYTES		1

/*!
 * @def MAX_SWITCH
 * Chip당 최대 Switch의 개수.
 */
// #define MAX_SWITCH		120		// 120, 60, 30
// #define MAX_SWITCH		60		// 120, 60, 30
 #define MAX_SWITCH		30		// 120, 60, 30
#define MAX_BYTES		((MAX_SWITCH / 8) + ((MAX_SWITCH%8) ? 1 : 0))

#define MAX_FRAME		(MAX_BYTES + 3)

// #define DEFAULT_BRANCH	0			///< 4-row(4분기)
// #define DEFAULT_BRANCH	1			///< 6-row(6분기)
// #define DEFAULT_BRANCH	2			///< 8-row(8분기)
 #define DEFAULT_BRANCH	3			///< 10-row(10분기)
// #define DEFAULT_BRANCH	4			///< 12-row(12분기)
// #define DEFAULT_BRANCH	5			///< 16-row(16분기)


/*!
 * @def CHIP_MASK
 * Chip 개수에 따라서 Chip Address Assignment에 사용될 값.
 */
#define CHIP_MASK		(0xFF << (8 - MAX_CHIP))

/*! @defgroup BranchType Branch Type
 * 모든 switch의 분기 구성을 정의한다.
 * @{
 */
#define BRANCH4		4	///< 4분기 구성 : 4-switch per group -> 32-group, 4-channel
#define BRANCH6		6	///< 6분기 구성 : 6-switch per group -> 22-group, 6-channel
#define BRANCH8		8	///< 8분기 구성 : 8-switch per group -> 16-group, 8-channel
#define BRANCH10		10	///< 10분기 구성 : 10-switch per group -> 13-group, 10-channel
#define BRANCH12		12	///< 12분기 구성 : 12-switch per group -> 11-group, 12-channel
#define BRANCH16		16	///< 16분기 구성 : 16-switch per group -> 8-group, 16-channel
/*! @}*/


/*!
 * @defgroup CommandType Command Type
 * Command Type Field의 각 bit's의 의미를 정의한다.
 * @{
 */
/*!
 * Command Type에서 Chip Select Data의 존재 여부를 정의한다.
 */
#define CHIP_SELECT_MASK			(0x8)
/*!
 * Command Type에서 Group Select Data의 존재 여부를 정의한다.
 */
#define GROUP_SELECT_MASK		(0x4)
/*!
 * Command Type에서 Channel Select Data의 존재 여부를 정의한다.
 */
#define CHANNEL_SELECT_MASK		(0x2)
/*!
 * Command Type에서 Switch Select Data의 존재 여부를 정의한다.
 */
#define SWITCH_SELECT_MASK		(0x1)
/*! @}*/

#if 0
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
		uint8_t bHolder[MAX_BYTES];
	};
#endif
	/*! @var uint8_t FTP[8]
	 * 각 Multi-Switch별로 고유한 값은 저장한다.(Trim 값)
	 * @todo Flash 용량은 변경될 필요가 있다.
	 */
	uint8_t FTP[8];

	/*! @var uint8_t SFR[8]
	 * 각 Multi-Switch별로 Internal register variable.
	 * @todo Multi-Switch의 Internal register format을 정의해야 한다.
	 */
	uint8_t SFR[8];
};
typedef struct _MultiSwitchIC tMultiSwitch;
/*!
 * @fn typedef _MultiSwitchIC tMultiSwitch
 * 128-channel Multi-Switch IC Emulator.
 */
#endif

typedef struct _bit_count {
	uint8_t column;
	uint8_t col_sw;
	uint8_t row;
	uint8_t row_sw;
} tBitCount;
extern const tBitCount BitCount[];


#define SW_OFF				(0)
#define SW_ON				(1<<0)
#define HOLD				(1<<1)
#define RELEASE			(2<<1)
#define FORCE				(3<<1)

#define HOLD_OFF			(HOLD | SW_OFF)
#define HOLD_ON			(HOLD | SW_ON)
#define RELEASE_OFF			(RELEASE | SW_OFF)
#define RELEASE_ON			(RELEASE | SW_ON)
#define FORCE_OFF			(FORCE | SW_OFF)
#define FORCE_ON			(FORCE | SW_ON)

#define TYPE0				(0x0<<3)
#define TYPE1				(0x1<<3)
#define TYPE2				(0x2<<3)
#define TYPE3				(0x3<<3)
#define TYPE4				(0x4<<3)
#define TYPE5				(0x5<<3)
#define TYPE6				(0x6<<3)
#define TYPE7				(0x7<<3)
#define TYPE8				(0x8<<3)
#define TYPE9				(0x9<<3)
#define TYPE10				(0xA<<3)
#define TYPE11				(0xB<<3)
#define TYPE12				(0xC<<3)
#define TYPE13				(0xD<<3)
#define TYPE14				(0xE<<3)
#define TYPE15				(0xF<<3)

#define HOLD_ON			(HOLD | SW_ON)
#define HOLD_OFF			(HOLD | SW_OFF)
#define RELEASE_ON			(RELEASE | SW_ON)
#define RELEASE_OFF			(RELEASE | SW_OFF)
#define FORCE_ON			(FORCE | SW_ON)
#define FORCE_OFF			(FORCE | SW_OFF)

#define GROUP_TYPE			(0x1)
#define ADDR_ASSIGN			(0x7)

#define FTP_CTRL			(0x50)			///< SFR index = 0
#define FTP_Manual_CTRL0		(0x52)			///< SFR index = 1
#define FTP_Manual_CTRL1		(0x54)			///< SFR index = 2
#define FTP_DR_PD_CTRL		(0x56)			///< SFR index = 3
#define FTP_KEY			(0x58)			///< SFR index = 4
#define FTP_STATUS			(0x5A)			///< SFR index = 5

#define ALL_PAD_MASKING		(0x60)			///< SFR index = 6
#define ALL_PAD_CTRL		(0x62)			///< SFR index = 7
#define SINGLE_PAD_MASKING	(0x64)			///< SFR index = 8
#define SINGLE_PAD_CTRL		(0x66)			///< SFR index = 9
#define PAD_SEL_S			(0x68)			///< SFR index = 10

#define SW_NUM				(0x70)			///< SFR index = 11
#define SW_STATUS			(0x72)			///< SFR index = 12
#define SW_OP_TIME			(0x80)			///< SFR index = 13
	#define SW_OP_TIME_SCALE_NS		(0x00)
	#define SW_OP_TIME_SCALE_US		(0x40)
	#define SW_OP_TIME_SCALE_MS		(0x80)
	#define SW_OP_TIME_VALUE(n)		(n<<4)
#define SYS_CTRL			(0x82)			///< SFR index = 14
	#define SYS_CTRL_CLK_SEL(n)		(n<<7)
	#define SYS_CTRL_CLK_EN(n)		(n<<6)
#define FTPD0_ADDR			(0x90)
#define FTPD1_ADDR			(0x91)
#define FTPD2_ADDR			(0x92)
#define FTPD3_ADDR			(0x93)





#define CHIP_RESET          (0x84)
#define CHIP_STATUS         (0x86)
#define STACK_CTRL          (0x90)
#define IP_CTRL             (0x92)
#define CHIP_CTRL           (0x96)


#define mSW_FTP_READ(c, a)				mSW_SFR_Ctrl(0xC0, c, a, 0)
#define mSW_FTP_WRITE(c, a, d)			mSW_SFR_Ctrl(0xC1, c, a, d)
#define mSW_ALL_FTP_WRITE(a, d)			mSW_SFR_Ctrl(0x81, 0, a, d)
#define mSW_SFR_READ(c, a)				mSW_SFR_Ctrl(0xC0, c, a, 0)
#define mSW_SFR_WRITE(c, a, d)			mSW_SFR_Ctrl(0xC1, c, a, d)
#define mSW_ALL_SWITCH_ONOFF_READ(c)		mSW_ALL_SWITCH_READ(0xC2, c)
//#define mSW_ROW_SWITCH_ONOFF_READ(c, n)		mSW_SFR_Ctrl(0xD2, c, n, 0)
//#define mSW_COLUMN_SWITCH_ONOFF_READ(c, n)	mSW_SFR_Ctrl(0xE2, c, n, 0)
#define mSW_ALL_SWITCH_HOLD_READ(c)		mSW_ALL_SWITCH_READ(0xC4, c)
//#define mSW_ROW_SWITCH_HOLD_READ(c, n)		mSW_SFR_Ctrl(0xD4, c, n, 0)
//#define mSW_COLUMN_SWITCH_HOLD_READ(c, n)	mSW_SFR_Ctrl(0xE4, c, n, 0)
#define mSW_SINGLE_SWITCH_READ(c, a)		mSW_SFR_Ctrl(0xF6, c, a, 0)
#define mSW_SOFTWARE_RESET()				mSW_SFR_Ctrl(0xC1, 0, CHIP_RESET, 0xFF)
#define mSW_PATTERN_RESET()				mSW_SFR_Ctrl(0xFF, 0, 0, 0)
#define mSW_SET_GROUP_TYPE(n)				mSW_SFR_Ctrl(0x31, 0, n, 0)
#define mSW_SET_GROUP_TYPE_CHIP(c, n)		mSW_SFR_Ctrl(0x71, c, n, 0)
#define mSW_CHIP_ADDRESS_ASSIGN(c)			mSW_SFR_Ctrl(0x37, c, 0, 0)
#define mSW_ALL_AD_DISABLE()			mSW_SFR_Ctrl(0x34, 0, 0, 0)
#define mSW_ALL_AD_ENABLE()			mSW_SFR_Ctrl(0x35, 0, 0, 0)
#define mSW_CHIP_AD_DISABLE(c)			mSW_SFR_Ctrl(0x74, c, 0, 0)
#define mSW_CHIP_AD_ENABLE(c)			mSW_SFR_Ctrl(0x75, c, 0, 0)





#define mSW_ALL_SWITCH(op)				mSW_Ctrl(TYPE0|op, 0, NULL, 0)
#define mSW_SINGLE_SWITCH(op, c, b)		mSW_Ctrl(TYPE1|op, c, b, 1)
#define mSW_ROW_ALL_SWITCH(op, c, b, n)		mSW_Ctrl(TYPE2|op, c, b, n)
#define mSW_ROW_EACH_SWITCH(op, c, b, n)	mSW_Ctrl(TYPE3|op, c, b, n)
#define mSW_COLUMN_ALL_SWITCH(op, c, b, n)	mSW_Ctrl(TYPE4|op, c, b, n)
#define mSW_COLUMN_EACH_SWITCH(op, c, b, n)	mSW_Ctrl(TYPE5|op, c, b, n)
#define mSW_EACH_SWITCH(op, c, b, n)		mSW_Ctrl(TYPE7|op, c, b, n)
#define mSW_SPECIAL_COMMAND(op, c, b, n)	mSW_Ctrl(TYPE6|op, c, b, n)

#define mSW_ALL_ON()					mSW_Ctrl(TYPE0|SW_ON, 0, NULL, 0)
#define mSW_ALL_OFF()					mSW_Ctrl(TYPE0|SW_OFF, 0, NULL, 0)
#define mSW_ALL_HOLD_ON()				mSW_Ctrl(TYPE0|HOLD_ON, 0, NULL, 0)
#define mSW_ALL_HOLD_OFF()				mSW_Ctrl(TYPE0|HOLD_OFF, 0, NULL, 0)
#define mSW_ALL_RELEASE_ON()				mSW_Ctrl(TYPE0|RELEASE_ON, 0, NULL, 0)
#define mSW_ALL_RELEASE_OFF()				mSW_Ctrl(TYPE0|RELEASE_OFF, 0, NULL, 0)
#define mSW_ALL_FORCE_ON()				mSW_Ctrl(TYPE0|FORCE_ON, 0, NULL, 0)
#define mSW_ALL_FORCE_OFF()				mSW_Ctrl(TYPE0|FORCE_OFF, 0, NULL, 0)

#define mSW_SWITCH(cmd, chip, n)			mSW_Single_Ctrl(TYPE1|cmd, chip, n)

#define mSW_SINGLE_SWITCH_ON(c, b)			mSW_Ctrl(TYPE1|SW_ON, c, &b, 1)
#define mSW_SINGLE_SWITCH_OFF(c, b)		mSW_Ctrl(TYPE1|SW_OFF, c, &b, 1)
#define mSW_SINGLE_SWITCH_HOLD_ON(c, b)		mSW_Ctrl(TYPE1|HOLD_ON, c, &b, 1)
#define mSW_SINGLE_SWITCH_HOLD_OFF(c, b)	mSW_Ctrl(TYPE1|HOLD_OFF, c, &b, 1)
#define mSW_SINGLE_SWITCH_RELEASE_ON(c, b)	mSW_Ctrl(TYPE1|RELEASE_ON, c, &b, 1)
#define mSW_SINGLE_SWITCH_RELEASE_OFF(c, b)	mSW_Ctrl(TYPE1|RELEASE_OFF, c, &b, 1)
#define mSW_SINGLE_SWITCH_FORCE_ON(c, b)	mSW_Ctrl(TYPE1|FORCE_ON, c, &b, 1)
#define mSW_SINGLE_SWITCH_FORCE_OFF(c, b)	mSW_Ctrl(TYPE1|FORCE_OFF, c, &b, 1)

#define mSW_ROW_ALL_ON(c, b, n)			mSW_Ctrl(TYPE2|SW_ON, c, b, n)
#define mSW_ROW_ALL_OFF(c, b, n)			mSW_Ctrl(TYPE2|SW_OFF, c, b, n)
#define mSW_ROW_ALL_HOLD_ON(c, b, n)		mSW_Ctrl(TYPE2|HOLD_ON, c, b, n)
#define mSW_ROW_ALL_HOLD_OFF(c, b, n)		mSW_Ctrl(TYPE2|HOLD_OFF, c, b, n)
#define mSW_ROW_ALL_RELEASE_ON(c, b, n)		mSW_Ctrl(TYPE2|RELEASE_ON, c, b, n)
#define mSW_ROW_ALL_RELEASE_OFF(c, b, n)	mSW_Ctrl(TYPE2|RELEASE_OFF, c, b, n)
#define mSW_ROW_ALL_FORCE_ON(c, b, n)		mSW_Ctrl(TYPE2|FORCE_ON, c, b, n)
#define mSW_ROW_ALL_FORCE_OFF(c, b, n)		mSW_Ctrl(TYPE2|FORCE_OFF, c, b, n)

#define mSW_ROW_SWITCH_ON(c, b, n)			mSW_Ctrl(TYPE3|SW_ON, c, b, n)
#define mSW_ROW_SWITCH_OFF(c, b, n)		mSW_Ctrl(TYPE3|SW_OFF, c, b, n)
#define mSW_ROW_SWITCH_HOLD_ON(c, b, n)		mSW_Ctrl(TYPE3|HOLD_ON, c, b, n)
#define mSW_ROW_SWITCH_HOLD_OFF(c, b, n)	mSW_Ctrl(TYPE3|HOLD_OFF, c, b, n)
#define mSW_ROW_SWITCH_RELEASE_ON(c, b, n)	mSW_Ctrl(TYPE3|RELEASE_ON, c, b, n)
#define mSW_ROW_SWITCH_RELEASE_OFF(c, b, n)	mSW_Ctrl(TYPE3|RELEASE_OFF, c, b, n)
#define mSW_ROW_SWITCH_FORCE_ON(c, b, n)	mSW_Ctrl(TYPE3|FORCE_ON, c, b, n)
#define mSW_ROW_SWITCH_FORCE_OFF(c, b, n)	mSW_Ctrl(TYPE3|FORCE_OFF, c, b, n)

#define mSW_COLUMN_ALL_ON(c, b, n)			mSW_Ctrl(TYPE4|SW_ON, c, b, n)
#define mSW_COLUMN_ALL_OFF(c, b, n)		mSW_Ctrl(TYPE4|SW_OFF, c, b, n)
#define mSW_COLUMN_ALL_HOLD_ON(c, b, n)		mSW_Ctrl(TYPE4|HOLD_ON, c, b, n)
#define mSW_COLUMN_ALL_HOLD_OFF(c, b, n)	mSW_Ctrl(TYPE4|HOLD_OFF, c, b, n)
#define mSW_COLUMN_ALL_RELEASE_ON(c, b, n)	mSW_Ctrl(TYPE4|RELEASE_ON, c, b, n)
#define mSW_COLUMN_ALL_RELEASE_OFF(c, b, n)	mSW_Ctrl(TYPE4|RELEASE_OFF, c, b, n)
#define mSW_COLUMN_ALL_FORCE_ON(c, b, n)	mSW_Ctrl(TYPE4|FORCE_ON, c, b, n)
#define mSW_COLUMN_ALL_FORCE_OFF(c, b, n)	mSW_Ctrl(TYPE4|FORCE_OFF, c, b, n)

#define mSW_COLUMN_SWITCH_ON(c, b, n)			mSW_Ctrl(TYPE5|SW_ON, c, b, n)
#define mSW_COLUMN_SWITCH_OFF(c, b, n)			mSW_Ctrl(TYPE5|SW_OFF, c, b, n)
#define mSW_COLUMN_SWITCH_HOLD_ON(c, b, n)		mSW_Ctrl(TYPE5|HOLD_ON, c, b, n)
#define mSW_COLUMN_SWITCH_HOLD_OFF(c, b, n)		mSW_Ctrl(TYPE5|HOLD_OFF, c, b, n)
#define mSW_COLUMN_SWITCH_RELEASE_ON(c, b, n)	mSW_Ctrl(TYPE5|RELEASE_ON, c, b, n)
#define mSW_COLUMN_SWITCH_RELEASE_OFF(c, b, n)	mSW_Ctrl(TYPE5|RELEASE_OFF, c, b, n)
#define mSW_COLUMN_SWITCH_FORCE_ON(c, b, n)		mSW_Ctrl(TYPE5|FORCE_ON, c, b, n)
#define mSW_COLUMN_SWITCH_FORCE_OFF(c, b, n)		mSW_Ctrl(TYPE5|FORCE_OFF, c, b, n)


#define mSW_ALL_SWITCH_ON(c, b, n)			mSW_Ctrl(TYPE7|SW_ON, c, b, n)
#define mSW_ALL_SWITCH_OFF(c, b, n)		mSW_Ctrl(TYPE7|SW_OFF, c, b, n)
#define mSW_ALL_SWITCH_HOLD_ON(c, b, n)		mSW_Ctrl(TYPE7|HOLD_ON, c, b, n)
#define mSW_ALL_SWITCH_HOLD_OFF(c, b, n)	mSW_Ctrl(TYPE7|HOLD_OFF, c, b, n)
#define mSW_ALL_SWITCH_RELEASE_ON(c, b, n)	mSW_Ctrl(TYPE7|RELEASE_ON, c, b, n)
#define mSW_ALL_SWITCH_RELEASE_OFF(c, b, n)	mSW_Ctrl(TYPE7|RELEASE_OFF, c, b, n)
#define mSW_ALL_SWITCH_FORCE_ON(c, b, n)	mSW_Ctrl(TYPE7|FORCE_ON, c, b, n)
#define mSW_ALL_SWITCH_FORCE_OFF(c, b, n)	mSW_Ctrl(TYPE7|FORCE_OFF, c, b, n)


///< FTP Register Description.
#define FTP_CTRL				(0x50)
#define FTP_MANUAL_CTRL0			(0x52)
#define FTP_MANUAL_CTRL1			(0x54)
#define FTP_DR_PD_CTRL			(0x56)
#define FTP_KEY				(0x58)
#define FTP_STATUS				(0x5A)

///< Test Mode Register Description.
#define ALL_PAD_MASKING			(0x60)
#define ALL_PAD_CTRL			(0x62)
#define SINGLE_PAD_MASKING		(0x64)
#define SINGLE_PAD_CTRL			(0x66)
#define PAD_SEL_S				(0x68)

///< Switch Status Register Description.
#define SW_NUMBER				(0x70)
#define SW_STATUS				(0x72)

///< System Control Register Description.
#define SW_OP_TIME				(0x80)
	#define SW_OP_TIME_SCALE_NS		(0x00)
	#define SW_OP_TIME_SCALE_US		(0x40)
	#define SW_OP_TIME_SCALE_MS		(0x80)
	#define SW_OP_TIME_VALUE(n)		(n<<4)
#define SYS_CTRL				(0x82)
	#define SYS_CTRL_CLK_SEL(n)		(n<<7)
	#define SYS_CTRL_CLK_EN(n)		(n<<6)





typedef union {
	uint8_t flag;
	struct {
		bool gCheck : 1;			// SPI Tx/Rx data check enable/disable.
		bool gTransmit : 1;			// SPI Tx data dump enable/disable.
		bool gReceive : 1;			// SPI Rx data dump enable/disable.
		bool gEmulator : 1;			// Emulator enable/disable.
		bool cs_gen : 1;			// SPI nCS generation enable/disable.
		bool reserved : 3;
	};
} _tDebug;

mSwitch_EXT __bdata _tDebug Debug;


/*! @union _command
 * Switch Command 구조체.
 */
typedef union _command {
	uint8_t CMD;				///< Byte Access Variable.
	struct {
		uint8_t opcode : 3;		///< bit[2:0]는 Switch의 동작을 정의한다.
		uint8_t type : 3;		///< bit[5:3]는 Command의 Type을 정의한다.
		uint8_t cs : 1;		///< bit[6]는 Command에서 CS(chip select) 사용을 정의한다.
		uint8_t mode : 1;		///< bit[7]는 Command의 Mode(User/Test)를 정의한다.
	};
	struct {
		uint8_t op : 3;		///< bit[2:0]는 Switch의 동작을 정의한다.
		uint8_t ctrl : 5;		///< bit[7:3]는 Command의 Type을 정의한다.
	};
} tCommand;




#if 1	// simulator struct.
typedef struct _switch {
	uint8_t onoff[15];
	uint8_t hold[15];
	uint8_t ChipSelect;
	uint8_t GroupType;
//	uint8_t FTP[4];
	uint8_t SFR[20];
} tSwitch;

mSwitch_EXT tSwitch sSW[MAX_CHIP];	///< simulator switch
#endif

mSwitch_EXT uint8_t mSW_Status[15+1];		///< Multi-Switch Read buffer.
mSwitch_EXT const tROM_STRING deviceName[];


/*! @var uint8_t GroupType
 * Switch Chip의 Group(분기) 구성을 저장한다.
 * Group Type Command로 초기화된다.
 */
mSwitch_EXT __idata uint8_t GroupType;
mSwitch_EXT __idata uint8_t gTotalSwitch;
mSwitch_EXT __idata uint8_t gDevice;
mSwitch_EXT __idata uint8_t gMaxChip;
mSwitch_EXT __idata uint8_t gMaxBytes;
mSwitch_EXT __idata uint8_t gMaxFrame;
mSwitch_EXT __idata uint8_t gGroupType;
mSwitch_EXT __idata uint8_t gChipSelect;

mSwitch_EXT uint8_t gFrame[15+3+2+1];
mSwitch_EXT uint8_t RxBuffer[15+3+2+1];



mSwitch_EXT const uint8_t c_GroupType[];
mSwitch_EXT const uint8_t msb_bit_mask[];
mSwitch_EXT const uint8_t lsb_bit_mask[];
mSwitch_EXT const tROM_STRING cmd_name[];
mSwitch_EXT const tROM_STRING deviceName[];

/*!
 * byte stream의 n 번째 bit의 값을 return한다.
 * @param[in] n bit position number(0 ~ 255)
 * @param[in] stream byte stream address.
 *
 * @return 1 or 0
 */
mSwitch_EXT bool check_bit(uint8_t n, uint8_t *stream);
mSwitch_EXT uint32_t copy_bit_msb_to_lsb(uint8_t s, uint8_t *stream, uint8_t n);

mSwitch_EXT void set_bit(uint8_t n, uint8_t *stream);
mSwitch_EXT void clear_bit(uint8_t n, uint8_t *stream);
mSwitch_EXT void check_and_set_bit(uint8_t n, uint8_t *stream);
mSwitch_EXT void check_and_clear_bit(uint8_t n, uint8_t *stream);

mSwitch_EXT uint8_t SendByte(uint8_t tx);

mSwitch_EXT uint8_t register_read(uint8_t chip, uint8_t addr);
mSwitch_EXT void ftpd_dump(void);

mSwitch_EXT uint8_t *mSW_ALL_SWITCH_READ(uint8_t cmd, uint8_t chip);
mSwitch_EXT uint8_t mSW_SFR_Ctrl(uint8_t cmd, uint8_t chip, uint8_t num, uint8_t data);
mSwitch_EXT uint8_t mSW_Ctrl(uint8_t cmd, uint8_t chip, uint8_t *bundle, uint8_t cnt);
mSwitch_EXT uint8_t mSW_Single_Ctrl(uint8_t cmd, uint8_t chip, uint8_t num);

mSwitch_EXT void switch_dump(uint8_t cs);
mSwitch_EXT void chip_status(uint8_t chip);


mSwitch_EXT uint8_t gen_frame(tCommand cmd, uint8_t *buf);
mSwitch_EXT void parse_command(uint8_t *buf, uint8_t cnt);
mSwitch_EXT uint8_t create_command(uint8_t *frame);
mSwitch_EXT uint8_t send_command(uint8_t *frame, uint8_t cnt);

mSwitch_EXT uint8_t device_detect(void);
mSwitch_EXT void chip_id_assignment_test(void);
mSwitch_EXT void active_discharge_read_test(void);
mSwitch_EXT void group_type_read_test(void);
mSwitch_EXT void switch_status_read_test(void);

mSwitch_EXT __near_func void mSwitch_Test(void);

#endif   //__NewProject_h__
