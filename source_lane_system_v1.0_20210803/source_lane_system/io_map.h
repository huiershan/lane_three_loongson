
#ifndef _IO_MAP_H
#define _IO_MAP_H


#define  IO_ON		1
#define  IO_OFF		0

//I/O Module Output
//green led
#define GREEN_LED_ADDR	13
#define GREEN_LED_PORT	26
//red led
#define RED_LED_ADDR	13
#define RED_LED_PORT	27


//yellow led
//#define YELLOW_LED_ADDR	11
//#define YELLOW_LED_PORT	2
/*---------------------------------*/

//fixture control
//advance
#define ADVANCE_ADDR	13
#define ADVANCE_PORT	31
//back
#define BACK_ADDR		13
#define BACK_PORT		30
//up
#define UP_ADDR			13
#define UP_PORT			29
//down
#define DOWN_ADDR		13
#define DOWN_PORT		28

/*---------------------------*/
//POWER control

//IPD power
//5V
#define P3_IPD_5V_ADDR		13
#define P3_IPD_5V_PORT		24
//3V3
#define P4_IPD_3V3_ADDR		13
#define P4_IPD_3V3_PORT		25

//UUT power
//P1  UUT 3V3
//#define P1_UUT_3V3_ADDR		13
//#define P1_UUT_3V3_PORT		22
//P2 UUT 5V
//#define P2_UUT_5V_ADDR		13
//#define P2_UUT_5V_PORT		23
//UUT 3V3 && 5V
#define UUT_3V3_5V_ADDR		13
#define UUT_3V3_5V_PORT		23

/*--------------------------------*/

//camera control
#define A_WHITE_ADDR		13
#define A_WHITE_PORT		18

#define B_PINK_ADDR			13
#define B_PINK_PORT			19

#define C_GRAY_ADDR			13
#define C_GRAY_PORT			20

/*-------------------------------*/

//vacuum control
#define VACUUM_CONTROL_ADDR	13
#define VACUUM_CONTROL_PORT	32
//Beep control
#define BEEP_ADDR			13
#define BEEP_PORT			17
/*-------------------------------*/
//key test
#if 0
//EMC control
#define EMC_CONTROL_ADDR	13
#define EMC_CONTROL_PORT	21
//power key
#define POWER_KEY_ADDR		11
#define POWER_KEY_PORT		14
//left shift key
#define LEFT_SHIFT_ADDR 	12	
#define LEFT_SHIFT_PORT		24 
//left ctrl key
#define LEFT_CTRL_ADDR		13
#define LEFT_CTRL_PORT		13
//left option key
#define LEFT_OPTION_ADDR	13
#define LEFT_OPTION_PORT	12
#else
//EMC control
//#define EMC_CONTROL_ADDR	13
//#define EMC_CONTROL_PORT	21
//left EMC control
#define EMC_L_CONTROL_ADDR	13
#define EMC_L_CONTROL_PORT	21
//right EMC control
#define EMC_R_CONTROL_ADDR	13
#define EMC_R_CONTROL_PORT	22

//power key
#define POWER_KEY_ADDR		12
#define POWER_KEY_PORT		28
//left shift key
#define LEFT_SHIFT_ADDR 	11	
#define LEFT_SHIFT_PORT		22 
//left ctrl key
#define LEFT_CTRL_ADDR		11
#define LEFT_CTRL_PORT		25
//left option key
#define LEFT_OPTION_ADDR	11
#define LEFT_OPTION_PORT	26

#define USE_JIS_OPTION		0		//x563=1
#if (1 == USE_JIS_OPTION)
//left option key (JIS)
#define LEFT_JIS_OPTION_ADDR	11
#define LEFT_JIS_OPTION_PORT	21
#endif

#endif
/*----------------------------*/
//I/O Module Input
//button
//start
#define START_BTN_ADDR			13
#define START_BTN_PORT			1
                  
//abort           
#define ABORT_BTN_ADDR			13
#define ABORT_BTN_PORT			2
/*----------------------*/
//fixture test  type
#define P_ANSI_ADDR				13
#define P_ANSI_PORT				8	

#define P_JIS_ADDR				13
#define P_JIS_PORT				9	

#define P_ISO_ADDR				13
#define P_ISO_PORT				10	

/*----------------------*/
//fixture sensor
//advance sensor
#define ADVANCE_SENSOR_ADDR		13
#define ADVANCE_SENSOR_PORT		3
//back sensor
#define BACK_SENSOR_ADDR		13
#define BACK_SENSOR_PORT		4
//up sensor
#define UP_SENSOR_ADDR			13
#define UP_SENSOR_PORT			5
//down sensor
#define DOWN_SENSOR_ADDR		13
#define DOWN_SENSOR_PORT		6

/*----------------------*/

//grating safty
#define GRATING_ADDR			13
#define GRATING_PORT			7

#endif
/*---------------------end file-------------------------------*/

