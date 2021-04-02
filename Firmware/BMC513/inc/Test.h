//===================================================================
// File Name : Test.h
// Function  : mSwitch Test Case function.
// Program   : khjung
// Date      : July, 24, 2019
// Version   : 0.0
// Mail      : khjung@leolsi.com
// Web       : www.leolsi.com
// History
//===================================================================

#ifndef __TEST_h__
#define __TEST_h__

#ifdef  __TEST_c__
#define TEST_EXT
#else
#define TEST_EXT extern
#endif

TEST_EXT uint8_t gStatus[MAX_BYTES];


TEST_EXT bool all_switch_status_check(void);



TEST_EXT uint8_t manual_input_select(uint8_t type, uint8_t *frame);

TEST_EXT void Simple_Test(void);
TEST_EXT void function_test(void);

TEST_EXT void Chip_Address_Assignment_Test(void);
TEST_EXT void All_Switch_Command_Test(void);
TEST_EXT void Channel_Command_Test(void);
TEST_EXT void Group_Command_Test(void);
TEST_EXT void Group_Switch_Command_Test(void);
TEST_EXT void Channel_Switch_Command_Test(void);
TEST_EXT void Manual_Command_Test(void);
TEST_EXT void Script_Command_Test(void);

TEST_EXT uint8_t Switch_Command_Test(uint8_t arg);

#endif	// __TEST_h__
