/***********************************************************
*  File: tuya_gpio_test.h
*  Author: lql
*  Date: 20180502
***********************************************************/

typedef BYTE_T BOARD_TYPE; 
#define RTL_BOARD_WR1 0
#define RTL_BOARD_WR2 1
#define RTL_BOARD_WR3 2
#define RTL_BOARD_WR4 3
#define RTL_BOARD_WR5 4    //WR5P
#define RTL_BOARD_WR6 5    //RLC2V
#define RTL_BOARD_WR7 6    //WF_R710_RWD1
#define RTL_BOARD_WR8 8    //TYAUX_F
#define RTL_BOARD_WR9 9    //WR2L
#define RTL_BOARD_WR10 10  //WR1E
#define RTL_BOARD_WR11 11  //WR2E
#define RTL_BOARD_WR12 12  //WR2E
#define RTL_BOARD_MAX 13  //WR2E

BOOL_T gpio_test_cb(BOARD_TYPE type);
BOOL_T gpio_test_all(VOID) ;