/*-----------------------------------------------------*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include "debug.h"
#include "global.h"
#include "application.h"
#include "error_use.h"

#include "tm1650.h"
#include "gpio_input.h"
#include "relay_output.h"
#include "local_ctrl.h"
#include "modbus_server.h"
#include "remote_ctrl.h"
#include "adc7606.h"


void* thread_gpio_input(void *arg)
{
	gpio_input_state_get();	
	return NULL;
}

void* thread_button_press(void *arg)
{
	button_press_check();	
	return NULL;
}

void* thread_local_input(void *arg)
{
	local_input_ctrl();	
	return NULL;
}

void* thread_local_output(void *arg)
{
	local_output_ctrl();	
	return NULL;
}

void* thread_modbus_server(void *arg)
{
	modbus_tcp_server();	
	return NULL;
}

void* thread_modbus_reg_update(void *arg)
{
	remote_modbus_reg_update();	
	return NULL;
}

void* thread_modbus_ctrl_lane(void *arg)
{
	remote_modbus_ctrl_lane();	
	return NULL;
}

void* thread_network_connect_led(void *arg)
{
	network_connect_led();	
	return NULL;
}

void* thread_sys_fault_led(void *arg)
{
	sys_fault_led();	
	return NULL;
}

void* thread_sys_running_led(void *arg)
{
	sys_running_led();	
	return NULL;
}

void* thread_input_output_loop_check(void *arg)
{
	input_output_loop_check();	
	return NULL;
}

void* thread_adc_value_read(void *arg)
{
	adc_value_get();	
	return NULL;
}

void* thread_rs485_ctrl(void *arg)
{
	rs485_ctrl_device();	
	return NULL;
}

void* thread_rs485_1_send(void *arg)
{
	rs485_1_send_collect_cmd();	
	return NULL;
}

void* thread_rs485_2_send(void *arg)
{
	rs485_2_send_collect_cmd();	
	return NULL;
}

void* thread_rs485_3_send(void *arg)
{
	rs485_3_send_collect_cmd();	
	return NULL;
}

void* thread_rs485_1_process_data(void *arg)
{
	rs485_1_process_collect_data();	
	return NULL;
}

void* thread_rs485_2_process_data(void *arg)
{
	rs485_2_process_collect_data();	
	return NULL;
}

void* thread_rs485_3_process_data(void *arg)
{
	rs485_3_process_collect_data();	
	return NULL;
}

void* thread_rs485_tx_led(void *arg)
{
	rs485_tx_led();	
	return NULL;
}

void* thread_rs485_rx_led(void *arg)
{
	rs485_rx_led();	
	return NULL;
}

//application start
int application_start(int arg)
{
	int ret;
	int rel=0,tmp=1;
	int rel_init=0;
	int *retval;
	pthread_t th_gpio_input_id=-1, th_button_press_id=-1,th_local_input_id=-1,th_local_output_id=-1,th_modbus_server_id=-1,
		th_modbus_reg_update_id=-1,th_modbus_ctrl_lane_id=-1,th_sys_fault_led_id=-1,th_sys_run_led_id=-1,th_sys_network_led_id=-1,
		th_input_output_loop_id=-1,th_adc_value_read_id=-1,th_rs485_ctrl_id,th_rs485_1_send_id,th_rs485_2_send_id,th_rs485_3_send_id,
		th_rs485_1_process_id,th_rs485_2_process_id,th_rs485_3_process_id,th_rs485_tx_led_id,th_rs485_rx_led_id;

#if 1	
	if(led_and_button_init()<0)
	{
		printf("led and button init fail\n");

	}
#endif

#if 1
	if(relay_output_init()<0)
	{
		printf("relay output init fail\n");

	}
#endif

#if 1
	if(gpio_input_init()<0)
	{
		printf("gpio input init fail\n");
	}

#endif

#if 1
	if(adc_scale_init()<0)
	{
		printf("adc scale init fail\n");
	}

#endif

#if 1
	if(adc_channel_init()<0)
	{
		printf("adc channel init fail\n");
	}

#endif

#if 1
	if(RS485_init()<0)
	{
		printf("rs485 init fail\n");
	}

#endif


	printf("Application control is running.\n");
#if 1
	msleep_m(100);
	if((ret = pthread_create(&th_gpio_input_id,NULL,thread_gpio_input,NULL)) <  0)
	{
		print_error("creat gpio input state get thread fail ! \n");
		rel |= tmp;
		tmp = tmp << 1;
	}
	else
	{
		print_infor("gpio input thread is create\n");
	}
#endif
#if 1
	msleep_m(10);
	if((ret = pthread_create(&th_button_press_id,NULL,thread_button_press,NULL)) <  0)
	{
		print_error("creat button press thread fail ! \n");
		rel |= tmp;
		tmp = tmp << 1;
	}
	else
	{
		print_infor("button press thread is create\n");
	}
#endif
	
#if 1

	msleep_m(10);
	if((ret = pthread_create(&th_local_input_id,NULL,thread_local_input,NULL)) <  0)
	{
		print_error("create local input  thread fail ! \n");
		rel |= tmp;
		tmp = tmp << 1;
	}
	else
	{
		print_infor("local input thread is create\n");
	}
#endif

#if 1
	msleep_m(10);
	if((ret = pthread_create(&th_local_output_id,NULL,thread_local_output,NULL)) <  0)
	{
		print_error("create local output  thread fail ! \n");
		rel |= tmp;
		tmp = tmp << 1;
	}
	else
	{
		print_infor("local output thread is create\n");
	}
#endif

#if 1
	msleep_m(10);
	if((ret = pthread_create(&th_modbus_server_id,NULL,thread_modbus_server,NULL)) <  0)
	{
		print_error("create modbus server  thread fail ! \n");
		rel |= tmp;
		tmp = tmp << 1;
	}
	else
	{
		print_infor("modbus server thread is create\n");
	}
#endif

#if 1
	msleep_m(10);
	if((ret = pthread_create(&th_modbus_reg_update_id,NULL,thread_modbus_reg_update,NULL)) <  0)
	{
		print_error("create modbus reg update thread fail ! \n");
		rel |= tmp;
		tmp = tmp << 1;
	}
	else
	{
		print_infor("modbus reg update thread is create\n");
	}
#endif

#if 1
	msleep_m(10);
	if((ret = pthread_create(&th_modbus_ctrl_lane_id,NULL,thread_modbus_ctrl_lane,NULL)) <  0)
	{
		print_error("create modbus ctrl lane thread fail ! \n");
		rel |= tmp;
		tmp = tmp << 1;
	}
	else
	{
		print_infor("modbus ctrl lane thread is create\n");
	}
#endif


#if 1
	msleep_m(10);
	if((ret = pthread_create(&th_sys_network_led_id,NULL,thread_network_connect_led,NULL)) <  0)
	{
		print_error("create network connect led thread fail ! \n");
		rel |= tmp;
		tmp = tmp << 1;
	}
	else
	{
		print_infor("network connect led thread is create\n");
	}
#endif


#if 1
	msleep_m(10);
	if((ret = pthread_create(&th_sys_fault_led_id,NULL,thread_sys_fault_led,NULL)) <  0)
	{
		print_error("create system fault led thread fail ! \n");
		rel |= tmp;
		tmp = tmp << 1;
	}
	else
	{
		print_infor("system fault led thread is create\n");
	}
#endif

#if 1
	msleep_m(10);
	if((ret = pthread_create(&th_sys_run_led_id,NULL,thread_sys_running_led,NULL)) <  0)
	{
		print_error("create system running led thread fail ! \n");
		rel |= tmp;
		tmp = tmp << 1;
	}
	else
	{
		print_infor("system running led thread is create\n");
	}
#endif

#if 1
	msleep_m(10);
	if((ret = pthread_create(&th_rs485_ctrl_id,NULL,thread_rs485_ctrl,NULL)) <  0)
	{
		print_error("create rs485 ctrl device thread fail ! \n");
		rel |= tmp;
		tmp = tmp << 1;
	}
	else
	{
		print_infor("rs485 ctrl device thread is create\n");
	}
#endif

#if 1
	if(pcoll1.enable==1)
	{
		msleep_m(10);
		if((ret = pthread_create(&th_rs485_1_send_id,NULL,thread_rs485_1_send,NULL)) <  0)
		{
			print_error("create rs485_1_send thread fail ! \n");
			rel |= tmp;
			tmp = tmp << 1;
		}
		else
		{
			print_infor("rs485_1_send thread is create\n");
		}

		msleep_m(10);
		if((ret = pthread_create(&th_rs485_1_process_id,NULL,thread_rs485_1_process_data,NULL)) <  0)
		{
			print_error("create rs485_1_process_data thread fail ! \n");
			rel |= tmp;
			tmp = tmp << 1;
		}
		else
		{
			print_infor("rs485_1_process_data thread is create\n");
		}
		

	}
#endif

	if(pcoll2.enable==1)
	{
		msleep_m(10);
		if((ret = pthread_create(&th_rs485_2_send_id,NULL,thread_rs485_2_send,NULL)) <  0)
		{
			print_error("create rs485_2_send thread fail ! \n");
			rel |= tmp;
			tmp = tmp << 1;
		}
		else
		{
			print_infor("rs485_2_send thread is create\n");
		}

		msleep_m(10);
		if((ret = pthread_create(&th_rs485_2_process_id,NULL,thread_rs485_2_process_data,NULL)) <  0)
		{
			print_error("create rs485_2_process_data thread fail ! \n");
			rel |= tmp;
			tmp = tmp << 1;
		}
		else
		{
			print_infor("rs485_2_process_data thread is create\n");
		}
	}


	if(pcoll3.enable==1)
	{
		msleep_m(10);
		if((ret = pthread_create(&th_rs485_3_send_id,NULL,thread_rs485_3_send,NULL)) <  0)
		{
			print_error("create rs485_3_send thread fail ! \n");
			rel |= tmp;
			tmp = tmp << 1;
		}
		else
		{
			print_infor("rs485_3_send thread is create\n");
		}

		msleep_m(10);
		if((ret = pthread_create(&th_rs485_3_process_id,NULL,thread_rs485_3_process_data,NULL)) <  0)
		{
			print_error("create rs485_3_process_data thread fail ! \n");
			rel |= tmp;
			tmp = tmp << 1;
		}
		else
		{
			print_infor("rs485_3_process_data thread is create\n");
		}
	}
	

#if 1
	msleep_m(10);
	if((ret = pthread_create(&th_rs485_tx_led_id,NULL,thread_rs485_tx_led,NULL)) <  0)
	{
		print_error("create rs485_tx_led thread fail ! \n");
		rel |= tmp;
		tmp = tmp << 1;
	}
	else
	{
		print_infor("rs485_tx_led thread is create\n");
	}
#endif

#if 1
	msleep_m(10);
	if((ret = pthread_create(&th_rs485_rx_led_id,NULL,thread_rs485_rx_led,NULL)) <  0)
	{
		print_error("create rs485_rx_led thread fail ! \n");
		rel |= tmp;
		tmp = tmp << 1;
	}
	else
	{
		print_infor("rs485_rx_led thread is create\n");
	}
#endif

#if 0
	msleep_m(10);
	if((ret = pthread_create(&th_input_output_loop_id,NULL,thread_input_output_loop_check,NULL)) <  0)
	{
		print_error("create input output loop check thread fail ! \n");
		rel |= tmp;
		tmp = tmp << 1;
	}
	else
	{
		print_infor("input output loop check thread is create\n");
	}
#endif

#if 1
	msleep_m(10);
	if((ret = pthread_create(&th_adc_value_read_id,NULL,thread_adc_value_read,NULL)) <  0)
	{
		print_error("create adc value read thread fail ! \n");
		rel |= tmp;
		tmp = tmp << 1;
	}
	else
	{
		print_infor("adc value read thread is create\n");
	}
#endif

	
	if(rel > 0)
	{
		while(1)
		{
			print_error("Thread create fail(%d)\n",rel);
			
		}
	}

#if 0
	if(th_gpio_input_id > 0)
	{
		pthread_join(th_gpio_input_id,(void **)&retval);
	}
#endif

#if 1
	if(th_button_press_id > 0)
	{
		pthread_join(th_button_press_id,(void **)&retval);
	}
#endif 

#if 0	
	if(th_local_input_id > 0)
	{
		pthread_join(th_local_input_id,(void **)&retval);
	}
	
	if(th_local_output_id > 0)
	{
		pthread_join(th_local_output_id,(void **)&retval);
	}
#endif

	printf("Go away application\n");

	return 0;

}


/*-----------------------end file------------------------*/

