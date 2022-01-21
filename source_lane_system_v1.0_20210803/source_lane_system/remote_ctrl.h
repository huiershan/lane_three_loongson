#ifndef _REMOTE_CTRL_H
#define _REMOTE_CTRL_H

typedef struct{
	unsigned short running_state;
	unsigned short local_enable;
	unsigned short alm_infor;

	unsigned char core_unit_error;
	unsigned char ext_power_error;
	unsigned char lightning_protect_error;
	unsigned char lane_led_display_error;

	unsigned char slow_ctrl_value_error;	//车道控制参数出错标志
	unsigned char main_ctrl_value_error;
	unsigned char over_ctrl_value_error;

	unsigned char slow_feedback_error;		//车道反馈出错标志
	unsigned char main_feedback_error;
	unsigned char over_feedback_error;

	unsigned char slow_loop_error;			//车道循环出错标志
	unsigned char main_loop_error;
	unsigned char over_loop_error;
	

	unsigned char network_connect_state;	//网络连接状态
	
}ditc_infor_t;
	
//枚举类型 通行 禁行 逆行 正转 反转
typedef enum{
	pass_s=0,
	forbid_s,	
	reverse_s,
	turn_front_s,
	turn_back_s,
	pass_fornt_cha_s,		//示例标志
	fault_s=999
}lane_state_t;


//led指示结构 绿箭、红叉、箭叉、左转、箭转、叉转、箭叉转
//含义 给指示赋值0 往下走后者等于前者加1
typedef enum{
	heiping=0,
	lvjian,	
	hongcha,
	jiancha,
	zuozhuan,
	jianzhuan,
	chazhuan,
	jianchazhuan
}led_state_t;



typedef struct{

	unsigned char start_ctrl_slow_lane_output_flag; /*1 start output ctrl relay*/
	unsigned char start_ctrl_main_lane_output_flag; /*1 start output ctrl relay*/
	unsigned char start_ctrl_over_lane_output_flag; /*1 start output ctrl relay*/
	
	unsigned char lane_select_switch;  /*1-slow lane 2-main-lane 3-over-lane*/
	unsigned char lane_ctrl_state; /*0-input state 1-output state*/
	
	unsigned short slow_state;	/*三个车道状态*/
	unsigned short main_state;
	unsigned short over_state;
	/*三车道指示器正反面状态*/
	unsigned short slow_front_led_state;
	unsigned short slow_back_led_state;
	unsigned short main_front_led_state;
	unsigned short main_back_led_state;
	unsigned short over_front_led_state;
	unsigned short over_back_led_state;

	unsigned short alarm_infor;	//告警信息
	/*三车道控制状态*/
	unsigned char slow_ctrl_state;
	unsigned char main_ctrl_state;
	unsigned char over_ctrl_state;
	
}lane_infor_t;


int remote_modbus_reg_update(void);
int remote_modbus_ctrl_lane(void);



#endif

