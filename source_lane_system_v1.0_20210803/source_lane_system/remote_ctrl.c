#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>   
#include "remote_ctrl.h"
#include "modbus_server.h"
#include "relay_output.h"
#include "gpio_input.h"



lane_infor_t lane_infor;
ditc_infor_t ditc_infor;
/**
	函数功能：输入远程控制模块
	函数参数：	无
	返回值：	无
**/
int remote_modbus_reg_update(void)
{
	int rel1=0,rel2=0,rel3=0,tmp1=1,tmp2=1,tmp3=1;
	unsigned int error_count_1=0,error_count_2=0,error_count_3=0;

	while(1)
	{
		usleep(1000000);


		rel1=0;
		rel2=0;
		rel3=0;
		tmp1=1;
		tmp2=1;
		tmp3=1;
		//车道输入反馈上传上位机监测
		if(lane_infor.slow_ctrl_state==pass_s )
		{		
			if(input_gpio_state[0]==0 && input_gpio_state[1]==1 && input_gpio_state[2]==1 && input_gpio_state[3]==1 && input_gpio_state[4]==0 && input_gpio_state[5]==1)
			{
				lane_infor.slow_state=pass_s;				
			}
			else 
			{
				rel1 |= tmp1;
				tmp1 = tmp1 << 1;
			}
		}		
		else if(lane_infor.slow_ctrl_state==forbid_s)
		{
			if(input_gpio_state[0]==1 && input_gpio_state[1]==0 && input_gpio_state[2]==1 && input_gpio_state[3]==1 && input_gpio_state[4]==0 && input_gpio_state[5]==1)
			{
				lane_infor.slow_state=forbid_s;				
			}
			else 
			{
				rel1 |= tmp1;
				tmp1 = tmp1 << 1;
			}
		}
		else if(lane_infor.slow_ctrl_state==reverse_s)
		{
			if(input_gpio_state[0]==1 && input_gpio_state[1]==0 && input_gpio_state[2]==1 && input_gpio_state[3]==0 && input_gpio_state[4]==1 && input_gpio_state[5]==1)
			{
				lane_infor.slow_state=reverse_s;				
			}
			else 
			{
				rel1 |= tmp1;
				tmp1 = tmp1 << 1;
			}
		}
		else if(lane_infor.slow_ctrl_state==turn_front_s)
		{
			if(input_gpio_state[0]==1 && input_gpio_state[1]==1 && input_gpio_state[2]==0 && input_gpio_state[3]==1 && input_gpio_state[4]==0 && input_gpio_state[5]==1)
			{
				lane_infor.slow_state=turn_front_s;				
			}
			else 
			{
				rel1 |= tmp1;
				tmp1 = tmp1 << 1;
			}
		}
		else if(lane_infor.slow_ctrl_state==turn_back_s)
		{
			if(input_gpio_state[0]==1 && input_gpio_state[1]==0 && input_gpio_state[2]==1 && input_gpio_state[3]==1 && input_gpio_state[4]==1 && input_gpio_state[5]==0)
			{
				lane_infor.slow_state=turn_back_s;				
			}
			else 
			{
				rel1 |= tmp1;
				tmp1 = tmp1 << 1;
			}
		}
		if(rel1>0)
		{
			error_count_1++;		
		}
		else
		{
			ditc_infor.slow_loop_error=0;
			error_count_1=0;
		}

		if(error_count_1>=2)
		{
			ditc_infor.slow_loop_error=1;
			lane_infor.slow_state=fault_s;
		}
		
		//main lane
		if(lane_infor.main_ctrl_state==pass_s)
		{
			if(input_gpio_state[0+7]==0 && input_gpio_state[1+7]==1 && input_gpio_state[2+7]==1 && input_gpio_state[3+7]==1 && input_gpio_state[4+7]==0 && input_gpio_state[5+7]==1)
			{
				lane_infor.main_state=pass_s;				
			}
			else 
			{
				rel2 |= tmp2;
				tmp2 = tmp2 << 1;
			}			
		}		
		else if(lane_infor.main_ctrl_state==forbid_s)
		{
			if(input_gpio_state[0+7]==1 && input_gpio_state[1+7]==0 && input_gpio_state[2+7]==1 && input_gpio_state[3+7]==1 && input_gpio_state[4+7]==0 && input_gpio_state[5+7]==1)
			{
				lane_infor.main_state=forbid_s;				
			}
			else 
			{
				rel2 |= tmp2;
				tmp2 = tmp2 << 1;
			}	
		}
		else if(lane_infor.main_ctrl_state==reverse_s)
		{
			if(input_gpio_state[0+7]==1 && input_gpio_state[1+7]==0 && input_gpio_state[2+7]==1 && input_gpio_state[3+7]==0 && input_gpio_state[4+7]==1 && input_gpio_state[5+7]==1)
			{
				lane_infor.main_state=reverse_s;				
			}
			else 
			{
				rel2 |= tmp2;
				tmp2 = tmp2 << 1;
			}		
		}
		else if(lane_infor.main_ctrl_state==turn_front_s)
		{
			if(input_gpio_state[0+7]==1 && input_gpio_state[1+7]==1 && input_gpio_state[2+7]==0 && input_gpio_state[3+7]==1 && input_gpio_state[4+7]==0 && input_gpio_state[5+7]==1)
			{
				lane_infor.main_state=turn_front_s;				
			}
			else 
			{
				rel2 |= tmp2;
				tmp2 = tmp2 << 1;
			}		
		}
		else if(lane_infor.main_ctrl_state==turn_back_s)
		{
			if(input_gpio_state[0+7]==1 && input_gpio_state[1+7]==0 && input_gpio_state[2+7]==1 && input_gpio_state[3+7]==1 && input_gpio_state[4+7]==1 && input_gpio_state[5+7]==0)
			{
				lane_infor.main_state=turn_back_s;				
			}
			else 
			{
				rel2 |= tmp2;
				tmp2 = tmp2 << 1;
			}		
		}
		if(rel2>0)
		{
			error_count_2++;		
		}
		else
		{
			ditc_infor.main_loop_error=0;
			error_count_2=0;
		}

		if(error_count_2>=2)
		{
			ditc_infor.main_loop_error=1;
			lane_infor.main_state=fault_s;
		}

		//over lane
		if(lane_infor.over_ctrl_state==pass_s )
		{
			if(input_gpio_state[0+14]==0 && input_gpio_state[1+14]==1 && input_gpio_state[2+14]==1 && input_gpio_state[3+14]==1 && input_gpio_state[4+14]==0 && input_gpio_state[5+14]==1)
			{
				lane_infor.over_state=pass_s;				
			}
			else 
			{
				rel3 |= tmp3;
				tmp3 = tmp3 << 1;
			}			
		}		
		else if(lane_infor.over_ctrl_state==forbid_s)
		{
			if(input_gpio_state[0+14]==1 && input_gpio_state[1+14]==0 && input_gpio_state[2+14]==1 && input_gpio_state[3+14]==1 && input_gpio_state[4+14]==0 && input_gpio_state[5+14]==1)
			{
				lane_infor.over_state=forbid_s;				
			}
			else 
			{
				rel3 |= tmp3;
				tmp3 = tmp3 << 1;
			}	
		}
		else if(lane_infor.over_ctrl_state==reverse_s)
		{
			if(input_gpio_state[0+14]==1 && input_gpio_state[1+14]==0 && input_gpio_state[2+14]==1 && input_gpio_state[3+14]==0 && input_gpio_state[4+14]==1 && input_gpio_state[5+14]==1)
			{
				lane_infor.over_state=reverse_s;				
			}
			else 
			{
				rel3 |= tmp3;
				tmp3 = tmp3 << 1;
			}	
		}
		else if(lane_infor.over_ctrl_state==turn_front_s)
		{
			if(input_gpio_state[0+14]==1 && input_gpio_state[1+14]==1 && input_gpio_state[2+14]==0 && input_gpio_state[3+14]==1 && input_gpio_state[4+14]==0 && input_gpio_state[5+14]==1)
			{
				lane_infor.over_state=turn_front_s;				
			}
			else 
			{
				rel3 |= tmp3;
				tmp3 = tmp3 << 1;
			}		
		}
		else if(lane_infor.over_ctrl_state==turn_back_s)
		{
			if(input_gpio_state[0+14]==1 && input_gpio_state[1+14]==0 && input_gpio_state[2+14]==1 && input_gpio_state[3+14]==1 && input_gpio_state[4+14]==1 && input_gpio_state[5+14]==0)
			{
				lane_infor.over_state=turn_back_s;				
			}
			else 
			{
				rel3 |= tmp3;
				tmp3 = tmp3 << 1;
			}	
		}
		if(rel3>0)
		{
			error_count_3++;		
		}
		else
		{
			ditc_infor.over_loop_error=0;
			error_count_3=0;
		}

		if(error_count_3>=2)
		{
			ditc_infor.over_loop_error=1;
			lane_infor.over_state=fault_s;
		}
			
		mb_mapping->tab_input_registers[REG_SLOW_FRONT_STATE_R]=lane_infor.slow_front_led_state;	/*slow front three di part to input_reg*/
		mb_mapping->tab_input_registers[REG_SLOW_BACK_STATE_R]=lane_infor.slow_back_led_state;		/*slow back three di part to input_reg*/
		mb_mapping->tab_input_registers[REG_SLOW_STATE_R]=lane_infor.slow_state;					/*slow lane state for input_reg*/
		
		mb_mapping->tab_input_registers[REG_MAIN_FRONT_STATE_R]=lane_infor.main_front_led_state;
		mb_mapping->tab_input_registers[REG_MAIN_BACK_STATE_R]=lane_infor.main_back_led_state;
		mb_mapping->tab_input_registers[REG_MAIN_STATE_R]=lane_infor.main_state;					/*main lane state for input_reg*/

		mb_mapping->tab_input_registers[REG_OVER_FRONT_STATE_R]=lane_infor.over_front_led_state;
		mb_mapping->tab_input_registers[REG_OVER_BACK_STATE_R]=lane_infor.over_back_led_state;
		mb_mapping->tab_input_registers[REG_OVER_STATE_R]=lane_infor.over_state;					/*over lane state for input_reg*/
		
		/*三车道继电器监测程序--上传错误码*/
		//slow lane
		if(ditc_infor.slow_ctrl_value_error==1)
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor |0x0001;
		}
		else
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor & 0xfffe;
		}
		if(ditc_infor.slow_feedback_error==1)
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor |0x0002;
		}
		else
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor & 0xfffd;
		}
		if(ditc_infor.slow_loop_error==1)
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor |0x0004;
		}
		else
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor & 0xfffb;
		}
		//main lane 
		if(ditc_infor.main_ctrl_value_error==1)
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor |0x0008;
		}
		else
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor & 0xfff7;
		}
		if(ditc_infor.main_feedback_error==1)
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor |0x0010;
		}
		else
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor & 0xffef;
		}
		if(ditc_infor.main_loop_error==1)
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor |0x0020;
		}
		else
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor & 0xffdf;
		}
		//over lane
		if(ditc_infor.over_ctrl_value_error==1)
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor |0x0040;
		}
		else
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor & 0xffbf;
		}
		if(ditc_infor.over_feedback_error==1)
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor |0x0080;
		}
		else
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor & 0xffef;
		}
		if(ditc_infor.over_loop_error==1)
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor |0x0100;
		}
		else
		{
			lane_infor.alarm_infor=lane_infor.alarm_infor & 0xfeff;
		}
		
		mb_mapping->tab_input_registers[REG_LANE_ALARM_INFOR_R]=lane_infor.alarm_infor;

		

		if(ditc_infor.ext_power_error==1)
		{
			ditc_infor.alm_infor=ditc_infor.alm_infor | 0x0002;
		}
		else
		{
			ditc_infor.alm_infor=ditc_infor.alm_infor & 0xfffd;
		}

		if(ditc_infor.lightning_protect_error==1)
		{
			ditc_infor.alm_infor=ditc_infor.alm_infor | 0x0004;
		}
		else
		{
			ditc_infor.alm_infor=ditc_infor.alm_infor & 0xfffb;
		}

		mb_mapping->tab_input_registers[REG_RUN_STATE_R]=ditc_infor.running_state;
		mb_mapping->tab_input_registers[REG_LOCAL_ENABLE_R]=ditc_infor.local_enable;
		mb_mapping->tab_input_registers[REG_ALARM_INFOR_R]=ditc_infor.alm_infor;

				
	}

}
/**
	函数功能：输出远程控制模块
	函数参数：	无
	返回值：	无
**/
int remote_modbus_ctrl_lane(void)
{
	mb_mapping->tab_registers[REG_SLOW_CTRL_RW]=pass_s;		//慢车道读写地址为1
	mb_mapping->tab_registers[REG_MAIN_CTRL_RW]=pass_s;		//主车道读写地址为2
	mb_mapping->tab_registers[REG_OVER_CTRL_RW]=pass_s;		//快车道读写地址为3
	

	while(1)
	{
		sem_wait(&gModbus_ctrl_lane_sem);

		if(ditc_infor.local_enable==0)
		{
			//slow_lane
			if(lane_infor.lane_select_switch==1)
			{
				if(mb_mapping->tab_registers[REG_SLOW_CTRL_RW]==0)
				{
					if(slow_lane_pass()>0) 
					{
						lane_infor.slow_ctrl_state=pass_s;
						lane_infor.start_ctrl_slow_lane_output_flag=1;
						if(lane_infor.lane_ctrl_state==1)
						{
							lane_pass_led_on();
						}
					}
				}
				else if(mb_mapping->tab_registers[REG_SLOW_CTRL_RW]==1)
				{
					if(slow_lane_forbid()>0) 
					{
						lane_infor.slow_ctrl_state=forbid_s;
						if(lane_infor.lane_ctrl_state==1)
						{
							lane_forbid_led_on();
						}
					}
				}
				else if(mb_mapping->tab_registers[REG_SLOW_CTRL_RW]==2)
				{
					if(slow_lane_reverse()>0) 
					{
						lane_infor.slow_ctrl_state=reverse_s;
						if(lane_infor.lane_ctrl_state==1)
						{
							lane_reverse_led_on();
						}
					}
				}
				else if(mb_mapping->tab_registers[REG_SLOW_CTRL_RW]==3)
				{
					if(slow_lane_turn_front()>0) 
					{
						lane_infor.slow_ctrl_state=turn_front_s;
						if(lane_infor.lane_ctrl_state==1)
						{
							lane_turn_front_led_on();
						}
					}
				}
				else if(mb_mapping->tab_registers[REG_SLOW_CTRL_RW]==4)
				{
					if(slow_lane_turn_back()>0) 
					{
						lane_infor.slow_ctrl_state=turn_back_s;
						if(lane_infor.lane_ctrl_state==1)
						{
							lane_turn_back_led_on();
						}
					}
				}
				
				//示例代码（不做最后处理）
				else if(mb_mapping->tab_registers[REG_SLOW_CTRL_RW]==5)
				{
					if(slow_lane_pass_fornt_cha()>0)
					{
						lane_infor.slow_ctrl_state=pass_fornt_cha_s;
						if(lane_infor.lane_ctrl_state==1)	//1是output 0是input
						{
							lane_pass_fornt_cha_led_on();	//示例灯 指令程序只调用一次 若要显示三个灯就要循环
						}
					}
				}
				
				else
				{
					lane_all_ctrl_led_off();
				}
				
			}

			//main_lane

			else if(lane_infor.lane_select_switch==2)
			{
				if(mb_mapping->tab_registers[REG_MAIN_CTRL_RW]==0)
				{
					if(main_lane_pass()>0) //主车道控制通行判断
					{				
						lane_infor.main_ctrl_state=pass_s;
						lane_infor.start_ctrl_main_lane_output_flag=1;
						if(lane_infor.lane_ctrl_state==1)
						{
							lane_pass_led_on();			//通行指示
						}
					}
				}
				else if(mb_mapping->tab_registers[REG_MAIN_CTRL_RW]==1)
				{
					if(main_lane_forbid()>0) //主车道控制禁行判断
					{
						lane_infor.main_ctrl_state=forbid_s;
						if(lane_infor.lane_ctrl_state==1)
						{
							lane_forbid_led_on();		//禁行指示
						}
					}
				}
				else if(mb_mapping->tab_registers[REG_MAIN_CTRL_RW]==2)
				{
					if(main_lane_reverse()>0) //主车道控制逆行判断
					{
						lane_infor.main_ctrl_state=reverse_s;
						if(lane_infor.lane_ctrl_state==1)
						{
							lane_reverse_led_on();		//逆行指示
						}
					}
				}
				else if(mb_mapping->tab_registers[REG_MAIN_CTRL_RW]==3)
				{
					if(main_lane_turn_front()>0) //主车道控制左转判断
					{
						lane_infor.main_ctrl_state=turn_front_s;
						if(lane_infor.lane_ctrl_state==1)
						{
							lane_turn_front_led_on();	//左转指示
						}
					}
				}
				else if(mb_mapping->tab_registers[REG_MAIN_CTRL_RW]==4)
				{
					if(main_lane_turn_back()>0) //主车道控制反转判断
					{
						lane_infor.main_ctrl_state=turn_back_s;
						if(lane_infor.lane_ctrl_state==1)
						{
							lane_turn_back_led_on();	//反转指示
						}
					}
				}
				else
				{
					lane_all_ctrl_led_off();
				}
			}
						
			//over_lane
			else if(lane_infor.lane_select_switch==3)
			{
				if(mb_mapping->tab_registers[REG_OVER_CTRL_RW]==0)
				{
					if(over_lane_pass()>0) 
					{		
						lane_infor.over_ctrl_state=pass_s;
						lane_infor.start_ctrl_over_lane_output_flag=1;
						if(lane_infor.lane_ctrl_state==1)
						{
							lane_pass_led_on();
						}
					}
				}
				else if(mb_mapping->tab_registers[REG_OVER_CTRL_RW]==1)
				{
					if(over_lane_forbid()>0) 
					{
						lane_infor.over_ctrl_state=forbid_s;
						if(lane_infor.lane_ctrl_state==1)
						{
							lane_forbid_led_on();
						}
					}
				}
				else if(mb_mapping->tab_registers[REG_OVER_CTRL_RW]==2)
				{
					if(over_lane_reverse()>0) 
					{
						lane_infor.over_ctrl_state=reverse_s;
						if(lane_infor.lane_ctrl_state==1)
						{
							lane_reverse_led_on();
						}
					}
				}
				else if(mb_mapping->tab_registers[REG_OVER_CTRL_RW]==3)
				{
					if(over_lane_turn_front()>0) 
					{
						lane_infor.over_ctrl_state=turn_front_s;
						if(lane_infor.lane_ctrl_state==1)
						{
							lane_turn_front_led_on();
						}
					}
				}
				else if(mb_mapping->tab_registers[REG_OVER_CTRL_RW]==4)
				{
					if(over_lane_turn_back()>0) 
					{
						lane_infor.over_ctrl_state=turn_back_s;
						if(lane_infor.lane_ctrl_state==1)
						{
							lane_turn_back_led_on();
						}
					}
				}
				else
				{
					lane_all_ctrl_led_off();
				}				
			}			
		}		
	}
}



