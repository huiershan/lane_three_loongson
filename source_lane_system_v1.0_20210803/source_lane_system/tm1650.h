#ifndef _TM1650_H
#define _TM1650_H

#include <semaphore.h>

typedef enum{
	local_enable=0x47,
	//lane_switch=0x4f,	
	//state_switch=0x57,
	lane_switch=0x57,	
	state_switch=0x4f,
	pass_ctrl=0x5f,
	forbid_ctrl=0x67,
	reverse_ctrl=0x6f,
	turn_ctrl=0x77
}button_type_t;

typedef enum{
	short_press=0,
	long_press=1
}button_press_type_t;

typedef struct{
	unsigned char local_enable_cnt;		//本地使能计数
	unsigned char lane_switch_cnt;		//车道选择计数
	unsigned char state_switch_cnt;		//状态选择计数
	unsigned char pass_ctrl_cnt;		//通行控制
	unsigned char forbid_ctrl_cnt;		//禁行
	unsigned char reverse_ctrl_cnt;		//逆行
	unsigned char turn_ctrl_short_cnt;	//正转
	unsigned char turn_ctrl_long_cnt;	//反转
}button_press_cnt_t;	//按钮控制结构体


typedef struct{
	button_type_t button_type;
	button_press_type_t press_type;
	button_press_cnt_t press_cnt;
}button_t;	//按钮总结构体


int led_system_run_display(int cmd); //HL1
int led_network_connect_display(int cmd);//HL2
int led_system_error_display(int cmd);//HL3
int led_uart_tx_display(int cmd);//HL4
int led_uart_rx_display(int cmd);//HL5
int led_control_state_select_display(int cmd);//HL6
int led_slow_lane_select_display(int cmd);//HL7
int led_main_lane_select_display(int cmd);//HL8
int led_over_lane_select_display(int cmd);//HL9

int led_pass_and_front_go_display(int cmd);//HL11
int led_forbid_and_font_stop_display(int cmd);//HL12
int led_turn_front_and_font_turn_display(int cmd);//HL13
int led_reverse_and_back_go_display(int cmd);//HL14
int led_back_stop_display(int cmd);//HL15
int led_turn_back_and_back_turn_display(int cmd);//HL16


int lane_pass_led_on(void);
int lane_forbid_led_on(void);
int lane_reverse_led_on(void);
int lane_turn_front_led_on(void);
int lane_turn_back_led_on(void);
int lane_all_ctrl_led_off(void);
int lane_pass_fornt_cha_led_on(void);

int led_and_button_init(void);
int button_press_check(void);

extern button_t pb1,pb2,pb3,pb4,pb5,pb6,pb7;
extern sem_t gButtonPress_sem;
extern unsigned char gButtonPressType;

#endif
/*-----------------------end file------------------------------*/