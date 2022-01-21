
#include "tm1650.h"
#include "relay_output.h"
#include "gpio_input.h"
#include "modbus_server.h"
#include "global.h"

#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <linux/ethtool.h>


int local_output_ctrl(void)
{
	int init_flag=0;
	
	while(1)
	{
		if(init_flag==0)
		{
			if(slow_lane_pass()>0) 
			{
				lane_infor.slow_ctrl_state=pass_s;
			}

			if(main_lane_pass()>0) 
			{
				lane_infor.main_ctrl_state=pass_s;
			}

			if(over_lane_pass()>0) 
			{
				lane_infor.over_ctrl_state=pass_s;
			}

			init_flag=1;
		}

		sem_wait(&gButtonPress_sem);

		switch(gButtonPressType)
		{
			
			case local_enable:
				if( pb1.press_cnt.local_enable_cnt==1)
				{
					ditc_infor.local_enable=1;
				}
				else
				{
					ditc_infor.local_enable=0;
				}				
				break;

			case lane_switch:	//车道选择按钮
				if(pb2.press_cnt.lane_switch_cnt==0)
				{
					led_slow_lane_select_display(1);
					led_main_lane_select_display(0);
					led_over_lane_select_display(0);	
				}
				
				//main lane
				else if(pb2.press_cnt.lane_switch_cnt==1 )
				{
					led_slow_lane_select_display(0);
					led_main_lane_select_display(1);
					led_over_lane_select_display(0);
				}
					
				//over lane
				else if(pb2.press_cnt.lane_switch_cnt==2)
				{
					led_slow_lane_select_display(0);
					led_main_lane_select_display(0);
					led_over_lane_select_display(1);
				}

				
				if(pb2.press_cnt.lane_switch_cnt==0 && lane_infor.lane_ctrl_state==1)
				{
					if(lane_infor.slow_ctrl_state==pass_s )
					{
						lane_pass_led_on();
					}
					else if(lane_infor.slow_ctrl_state==forbid_s)
					{
						lane_forbid_led_on();		
					}
					else if(lane_infor.slow_ctrl_state==reverse_s)
					{
						lane_reverse_led_on();
					}
					else if(lane_infor.slow_ctrl_state==turn_front_s)
					{
						lane_turn_front_led_on();
					}
					else if(lane_infor.slow_ctrl_state==turn_back_s)
					{
						lane_turn_back_led_on();
					}
					else
					{
						lane_all_ctrl_led_off();
					}
				}

				else if(pb2.press_cnt.lane_switch_cnt==1 && lane_infor.lane_ctrl_state==1)
				{
					if(lane_infor.main_ctrl_state==pass_s)
					{
						lane_pass_led_on();
					}
					else if(lane_infor.main_ctrl_state==forbid_s)
					{
						lane_forbid_led_on();		
					}
					else if(lane_infor.main_ctrl_state==reverse_s)
					{
						lane_reverse_led_on();
					}
					else if(lane_infor.main_ctrl_state==turn_front_s)
					{
						lane_turn_front_led_on();
					}
					else if(lane_infor.main_ctrl_state==turn_back_s)
					{
						lane_turn_back_led_on();
					}
					else
					{
						lane_all_ctrl_led_off();
					}
				}
				
				else if(pb2.press_cnt.lane_switch_cnt==2 && lane_infor.lane_ctrl_state==1)
				{					
					if(lane_infor.over_ctrl_state==pass_s )
					{
						lane_pass_led_on();
					}
					else if(lane_infor.over_ctrl_state==forbid_s)
					{
						lane_forbid_led_on();	
					}
					else if(lane_infor.over_ctrl_state==reverse_s)
					{
						lane_reverse_led_on();
					}
					else if(lane_infor.over_ctrl_state==turn_front_s)
					{
						lane_turn_front_led_on();
					}
					else if(lane_infor.over_ctrl_state==turn_back_s)
					{
						lane_turn_back_led_on();
					}
					else
					{
						lane_all_ctrl_led_off();
					}
				}
				
				break;

			case state_switch:	//状态选择按钮
				if(pb3.press_cnt.state_switch_cnt==1)
				{
					led_control_state_select_display(1);
					lane_infor.lane_ctrl_state=1;	
				}
				else
				{
					led_control_state_select_display(0);
					lane_infor.lane_ctrl_state=0;	
				}

				if(pb2.press_cnt.lane_switch_cnt==0 && lane_infor.lane_ctrl_state==1)
				{
					if(lane_infor.slow_ctrl_state==pass_s )
					{
						lane_pass_led_on();
					}
					else if(lane_infor.slow_ctrl_state==forbid_s)
					{
						lane_forbid_led_on();		
					}
					else if(lane_infor.slow_ctrl_state==reverse_s)
					{
						lane_reverse_led_on();
					}
					else if(lane_infor.slow_ctrl_state==turn_front_s)
					{
						lane_turn_front_led_on();
					}
					else if(lane_infor.slow_ctrl_state==turn_back_s)
					{
						lane_turn_back_led_on();
					}
					else
					{
						lane_all_ctrl_led_off();
					}
				}

				else if(pb2.press_cnt.lane_switch_cnt==1 && lane_infor.lane_ctrl_state==1)
				{
					if(lane_infor.main_ctrl_state==pass_s )
					{
						lane_pass_led_on();
					}
					else if(lane_infor.main_ctrl_state==forbid_s)
					{
						lane_forbid_led_on();		
					}
					else if(lane_infor.main_ctrl_state==reverse_s)
					{
						lane_reverse_led_on();
					}
					else if(lane_infor.main_ctrl_state==turn_front_s)
					{
						lane_turn_front_led_on();
					}
					else if(lane_infor.main_ctrl_state==turn_back_s)
					{
						lane_turn_back_led_on();
					}
					else
					{
						lane_all_ctrl_led_off();
					}
				}
				
				else if(pb2.press_cnt.lane_switch_cnt==2 && lane_infor.lane_ctrl_state==1)
				{					
					if(lane_infor.over_ctrl_state==pass_s )
					{
						lane_pass_led_on();
					}
					else if(lane_infor.over_ctrl_state==forbid_s)
					{
						lane_forbid_led_on();	
					}
					else if(lane_infor.over_ctrl_state==reverse_s)
					{
						lane_reverse_led_on();
					}
					else if(lane_infor.over_ctrl_state==turn_front_s)
					{
						lane_turn_front_led_on();
					}
					else if(lane_infor.over_ctrl_state==turn_back_s)
					{
						lane_turn_back_led_on();
					}
					else
					{
						lane_all_ctrl_led_off();
					}
				}
				

				break;

			case pass_ctrl:		//通行控制
									
				if(pb4.press_cnt.pass_ctrl_cnt==1)
				{
					if(ditc_infor.local_enable==1)
					{
						if(pb2.press_cnt.lane_switch_cnt==0)
						{
								
							if(slow_lane_pass()>0) 
							{
								lane_infor.slow_ctrl_state=pass_s;
								lane_infor.start_ctrl_slow_lane_output_flag=1;
								if(lane_infor.lane_ctrl_state==1)
								{
									lane_pass_led_on();
								}
								mb_mapping->tab_registers[0]=pass_s;
								//lane_infor.slow_ctrl_state=pass_s;
							}
						}

						else if(pb2.press_cnt.lane_switch_cnt==1)
						{
							if(main_lane_pass()>0) 
							{
								lane_infor.main_ctrl_state=pass_s;
								lane_infor.start_ctrl_main_lane_output_flag=1;
								if(lane_infor.lane_ctrl_state==1)
								{
									lane_pass_led_on();
								}
								mb_mapping->tab_registers[1]=pass_s;
								//lane_infor.main_ctrl_state=pass_s;
							}
						}

						else if(pb2.press_cnt.lane_switch_cnt==2)
						{
							if(over_lane_pass()>0) 
							{
								lane_infor.over_ctrl_state=pass_s;
								lane_infor.start_ctrl_over_lane_output_flag=1;
								if(lane_infor.lane_ctrl_state==1)
								{
									lane_pass_led_on();
								}
								mb_mapping->tab_registers[2]=pass_s;
								//lane_infor.over_ctrl_state=pass_s;
							}
						}				
					}			
				}
				break;

			case forbid_ctrl:		//禁行控制

				if(pb5.press_cnt.forbid_ctrl_cnt==1)
				{
					if(ditc_infor.local_enable==1)
					{
						if(pb2.press_cnt.lane_switch_cnt==0)
						{
								
							if(slow_lane_forbid()>0) 
							{
								lane_infor.slow_ctrl_state=forbid_s;
								if(lane_infor.lane_ctrl_state==1)
								{
									lane_forbid_led_on();
								}
								mb_mapping->tab_registers[0]=forbid_s;
								//lane_infor.slow_ctrl_state=forbid_s;
							}
						}

						else if(pb2.press_cnt.lane_switch_cnt==1)
						{
							if(main_lane_forbid()>0) 
							{
								lane_infor.main_ctrl_state=forbid_s;
								if(lane_infor.lane_ctrl_state==1)
								{
									lane_forbid_led_on();
								}
								mb_mapping->tab_registers[1]=forbid_s;
								//lane_infor.main_ctrl_state=forbid_s;
							}
						}

						else if(pb2.press_cnt.lane_switch_cnt==2)
						{
							if(over_lane_forbid()>0) 
							{
								lane_infor.over_ctrl_state=forbid_s;
								if(lane_infor.lane_ctrl_state==1)
								{
									lane_forbid_led_on();
								}
								mb_mapping->tab_registers[2]=forbid_s;
								//lane_infor.over_ctrl_state=forbid_s;
							}
						}				
					}			
				}
				break;

			case reverse_ctrl:		//逆行控制

				if(pb6.press_cnt.reverse_ctrl_cnt==1)
				{
					if(ditc_infor.local_enable==1)
					{
						if(pb2.press_cnt.lane_switch_cnt==0)
						{
								
							if(slow_lane_reverse()>0) 
							{
								lane_infor.slow_ctrl_state=reverse_s;
								if(lane_infor.lane_ctrl_state==1)
								{
									lane_reverse_led_on();
								}
								mb_mapping->tab_registers[0]=reverse_s;
								//lane_infor.slow_ctrl_state=reverse_s;
							}
						}

						else if(pb2.press_cnt.lane_switch_cnt==1)
						{
							if(main_lane_reverse()>0) 
							{
								lane_infor.main_ctrl_state=reverse_s;
								if(lane_infor.lane_ctrl_state==1)
								{
									lane_reverse_led_on();
								}
								mb_mapping->tab_registers[1]=reverse_s;
								//lane_infor.main_ctrl_state=reverse_s;
							}
						}

						else if(pb2.press_cnt.lane_switch_cnt==2)
						{
							if(over_lane_reverse()>0) 
							{
								lane_infor.over_ctrl_state=reverse_s;
								if(lane_infor.lane_ctrl_state==1)
								{
									lane_reverse_led_on();
								}
								mb_mapping->tab_registers[2]=reverse_s;
								//lane_infor.over_ctrl_state=reverse_s;
							}
						}				
					}			
				}
				break;

			case turn_ctrl:		//转向控制

				if(pb7.press_cnt.turn_ctrl_short_cnt==1)
				{
					pb7.press_cnt.turn_ctrl_short_cnt=0;
					if(ditc_infor.local_enable==1)
					{
						if(pb2.press_cnt.lane_switch_cnt==0)
						{
								
							if(slow_lane_turn_front()>0) 
							{
								lane_infor.slow_ctrl_state=turn_front_s;
								if(lane_infor.lane_ctrl_state==1)
								{
									lane_turn_front_led_on();
								}
								mb_mapping->tab_registers[0]=turn_front_s;
								//lane_infor.slow_ctrl_state=turn_front_s;
							}
						}

						else if(pb2.press_cnt.lane_switch_cnt==1)
						{
							if(main_lane_turn_front()>0) 
							{
								lane_infor.main_ctrl_state=turn_front_s;
								if(lane_infor.lane_ctrl_state==1)
								{
									lane_turn_front_led_on();
								}
								mb_mapping->tab_registers[1]=turn_front_s;
								//lane_infor.main_ctrl_state=turn_front_s;
							}
						}

						else if(pb2.press_cnt.lane_switch_cnt==2)
						{
							if(over_lane_turn_front()>0) 
							{
								lane_infor.over_ctrl_state=turn_front_s;
								if(lane_infor.lane_ctrl_state==1)
								{
									lane_turn_front_led_on();
								}
								mb_mapping->tab_registers[2]=turn_front_s;
								//lane_infor.over_ctrl_state=turn_front_s;
							}
						}				
					}			
				}

				else if(pb7.press_cnt.turn_ctrl_long_cnt==1)
				{
					pb7.press_cnt.turn_ctrl_long_cnt=0;
					if(ditc_infor.local_enable==1)
					{
						if(pb2.press_cnt.lane_switch_cnt==0)
						{
								
							if(slow_lane_turn_back()>0) 
							{
								lane_infor.slow_ctrl_state=turn_back_s;
								if(lane_infor.lane_ctrl_state==1)
								{
									lane_turn_back_led_on();
								}
								mb_mapping->tab_registers[0]=turn_back_s;
								//lane_infor.slow_ctrl_state=turn_back_s;
							}
						}

						else if(pb2.press_cnt.lane_switch_cnt==1)
						{
							if(main_lane_turn_back()>0) 
							{
								lane_infor.main_ctrl_state=turn_back_s;
								if(lane_infor.lane_ctrl_state==1)
								{
									lane_turn_back_led_on();
								}
								mb_mapping->tab_registers[1]=turn_back_s;
								//lane_infor.main_ctrl_state=turn_back_s;
								
							}
						}

						else if(pb2.press_cnt.lane_switch_cnt==2)
						{
							if(over_lane_turn_back()>0) 
							{
								lane_infor.over_ctrl_state=turn_back_s;
								if(lane_infor.lane_ctrl_state==1)
								{
									lane_turn_back_led_on();
								}
								mb_mapping->tab_registers[2]=turn_back_s;
								//lane_infor.over_ctrl_state=turn_back_s;
							}
						}				
					}			
				}				
				break;					
		}
	}
}

int local_input_ctrl(void)
{

	while(1)
	{
		usleep(1000000);

		if(lane_infor.lane_ctrl_state==0)
		{
			//行车道
			if(pb2.press_cnt.lane_switch_cnt==0)
			{
				if(lane.slow_front_go==1) led_pass_and_front_go_display(1);  //正面箭反馈				
			
			#if 1	
				else led_pass_and_front_go_display(0);
				
				if(lane.slow_front_stop==1) led_forbid_and_font_stop_display(1);
				else led_forbid_and_font_stop_display(0);
				
				if(lane.slow_front_turn==1) led_turn_front_and_font_turn_display(1);
				else led_turn_front_and_font_turn_display(0);
				
				if(lane.slow_back_go==1) led_reverse_and_back_go_display(1);
				else led_reverse_and_back_go_display(0);
				
				if(lane.slow_back_stop==1) led_back_stop_display(1);
				else led_back_stop_display(0);
				
				if(lane.slow_back_turn==1) led_turn_back_and_back_turn_display(1);
				else led_turn_back_and_back_turn_display(0);
			#endif	
			}
#if 1
			//主车道
			else if(pb2.press_cnt.lane_switch_cnt==1)
			{
				if(lane.main_front_go==1)
				{
					led_pass_and_front_go_display(1);  //正面箭反馈
				}
				else led_pass_and_front_go_display(0);
				
				if(lane.main_front_stop==1)
				{
					led_forbid_and_font_stop_display(1);
				}
				else led_forbid_and_font_stop_display(0);
				
				if(lane.main_front_turn==1)
				{
					led_turn_front_and_font_turn_display(1);
				}
				else led_turn_front_and_font_turn_display(0);
				
				if(lane.main_back_go==1)
				{
					led_reverse_and_back_go_display(1);
				}
				else led_reverse_and_back_go_display(0);
				
				if(lane.main_back_stop==1)
				{
					led_back_stop_display(1);
				}
				else led_back_stop_display(0);
				
				if(lane.main_back_turn==1)
				{
					led_turn_back_and_back_turn_display(1);
				}
				else led_turn_back_and_back_turn_display(0);
					
			}

			//超车道
			else if(pb2.press_cnt.lane_switch_cnt==2)
			{
				if(lane.over_front_go==1)
				{
					led_pass_and_front_go_display(1);  //正面箭反馈
				}
				else led_pass_and_front_go_display(0);
				
				if(lane.over_front_stop==1)
				{
					led_forbid_and_font_stop_display(1);
				}
				else led_forbid_and_font_stop_display(0);
				
				if(lane.over_front_turn==1)
				{
					led_turn_front_and_font_turn_display(1);
				}
				else led_turn_front_and_font_turn_display(0);
				
				if(lane.over_back_go==1)
				{
					led_reverse_and_back_go_display(1);
				}
				else led_reverse_and_back_go_display(0);
				
				if(lane.over_back_stop==1)
				{
					led_back_stop_display(1);
				}
				else led_back_stop_display(0);
				
				if(lane.over_back_turn==1)
				{
					led_turn_back_and_back_turn_display(1);
				}
				else led_turn_back_and_back_turn_display(0);
				
			}
#endif
			
		}
	
	}

}


#if 0
int network_connect_led(void)
{
	unsigned char value1[64];
	unsigned char var;

	int fd;
	int network_ok=0;

	memset(value1,0,sizeof(value1));

	fd=open("/sys/class/net/eth0/carrier",O_RDONLY);	
	if(fd==-1)
	{
		printf("open /sys/class/net/eth0/carrier error\n");   
		//return -1;
	}
   
	while(1)
	{
		
		usleep(1000000);
		if(fd >0)
		{
			lseek(fd, 0, SEEK_SET);
			if(read(fd,value1,2) == -1)
			{
				printf("read /sys/class/net/eth0/carrier error\n");
				led_network_connect_display(0);
				network_ok=0;
			}
			if(strncmp(value1,"1",1)==0)
			{
				network_ok=1;
			}
			else
			{
				network_ok=0;
				led_network_connect_display(0);	
			}		
		}
		else
		{
			led_network_connect_display(0);	

		}

		if(ditc_infor.network_connect_state==1 && network_ok==1)
		{
			usleep(1000000);
			led_network_connect_display(1);
			usleep(1000000);
			led_network_connect_display(0);	
		}
		else if(ditc_infor.network_connect_state==0 && network_ok==1)
		{
			led_network_connect_display(1);
		}
	}
}
#endif

#if 0
int get_netlink_status(const char *if_name)
{
    int skfd;
    struct ifreq ifr;
    struct ethtool_value edata;

    edata.cmd = ETHTOOL_GLINK;
    edata.data = 0;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, if_name, sizeof(ifr.ifr_name) - 1);
    ifr.ifr_data = (char *) &edata;

    if (( skfd = socket( AF_INET, SOCK_DGRAM, 0 )) < 0)
        return -1;

    if(ioctl( skfd, SIOCETHTOOL, &ifr ) == -1)
    {
        close(skfd);
        return -1;
    }

    close(skfd);
    return edata.data;
}
#endif

int network_connect_led(void)
{
	int network_ok=0;

	int skfd;
    	struct ifreq ifr;
    	struct ethtool_value edata;

    	edata.cmd = ETHTOOL_GLINK;
    	edata.data = 0;

	memset(&ifr, 0, sizeof(ifr));
    	strncpy(ifr.ifr_name, "eth0", sizeof(ifr.ifr_name) - 1);
    	ifr.ifr_data = (char *) &edata;

    	if (( skfd = socket( AF_INET, SOCK_DGRAM, 0 )) < 0)
    	{
		printf("creat eth0 link state socket fail\n");   
	}
   
	while(1)
	{
		
		usleep(1000000);
		if(skfd >0)
		{
			 if(ioctl( skfd, SIOCETHTOOL, &ifr ) == -1)
    			{
       			 printf("ioctl eth0 link state socket fail\n"); 
   			 }

			if(edata.data=1) 
			{
				network_ok=1;
			}
			else if(edata.data=0) 
			{
				network_ok=0;
				led_network_connect_display(0);	
			}		
		}
		else
		{
			led_network_connect_display(0);	

		}

		if(ditc_infor.network_connect_state==1 && network_ok==1)
		{
			usleep(1000000);
			led_network_connect_display(1);
			usleep(1000000);
			led_network_connect_display(0);	
		}
		else if(ditc_infor.network_connect_state==0 && network_ok==1)
		{
			led_network_connect_display(1);
		}
	}
}


int sys_fault_led(void)
{

	int flag1=0,flag2=0;
	
	while(1)
	{
		usleep(1000000);

		if(ditc_infor.ext_power_error==1 || ditc_infor.lightning_protect_error==1 )
		{
			flag1=1;
		}
		else
		{
			flag1=0;
		}
		
		if(flag1==1)
		{
			led_system_error_display(1);
		}
		else if(ditc_infor.slow_loop_error==1 && pb2.press_cnt.lane_switch_cnt==0)
		{
			usleep(1000000);
			led_system_error_display(1);
			usleep(1000000);
			led_system_error_display(0);	
		}
		else if(ditc_infor.main_loop_error==1 && pb2.press_cnt.lane_switch_cnt==1)
		{
			usleep(1000000);
			led_system_error_display(1);
			usleep(1000000);
			led_system_error_display(0);	
		}
		else if(ditc_infor.over_loop_error==1 && pb2.press_cnt.lane_switch_cnt==2)
		{
			usleep(1000000);
			led_system_error_display(1);
			usleep(1000000);
			led_system_error_display(0);	
		}
		else
		{
			led_system_error_display(0);		
		}
	}
}

int sys_running_led(void)
{	
	while(1)
	{
		usleep(1000000);
		if(ditc_infor.local_enable==1)
		{
			usleep(1000000);
			led_system_run_display(1);
			usleep(1000000);			
			led_system_run_display(0);		
		}
		else
		{
			led_system_run_display(1);
		}	
	}
}

int input_output_loop_check(void)
{

	int rel1=0,rel2=0,rel3=0,tmp1=1,tmp2=1,tmp3=1;
	
	while(1)
	{
		usleep(1000000);

		rel1=0;
		rel2=0;
		rel3=0;

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
			ditc_infor.slow_loop_error=1;
			lane_infor.slow_state=fault_s;
		}
		else
		{
			ditc_infor.slow_loop_error=0;
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
			ditc_infor.main_loop_error=1;
			lane_infor.main_state=fault_s;
		}
		else
		{
			ditc_infor.main_loop_error=0;
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
			ditc_infor.over_loop_error=1;
			lane_infor.over_state=fault_s;
		}
		else
		{
			ditc_infor.over_loop_error=0;
		}		
			
	}
}

