

#ifndef _GLOBAL_H
#define _GLOBAL_H
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "remote_ctrl.h"
#include "adc7606.h"
#include "serial.h"

//software infor
typedef struct{
	char company[16];
	char version[8];
	char date[14];
	char time[9];
	char sensor_k1[16];
	char sensor_k2[16];
	char sensor_k3[16];
	char sensor_k4[16];
	
	char uart1_name[12];
	char uart1_baudrate[8];
	char uart1_databits[2];
	char uart1_stopbits[2];
	char uart1_parity[2];

	char uart2_name[12];
	char uart2_baudrate[8];
	char uart2_databits[2];
	char uart2_stopbits[2];
	char uart2_parity[2];

	char uart3_name[12];
	char uart3_baudrate[8];
	char uart3_databits[2];
	char uart3_stopbits[2];
	char uart3_parity[2];

	unsigned char pctrl1_enable[2];
	unsigned char pctrl1_uart_id[2];
	unsigned char pctrl1_head[68];
	unsigned char pctrl1_head_len[4];
	unsigned char pctrl1_start_addr[4];
	unsigned char pctrl1_data_len[4];
	unsigned char pctrl1_retry[2];
	unsigned char pctrl1_sendflag_addr[4];
	unsigned char pctrl1_loop_time[8];

	unsigned char pctrl2_enable[2];
	unsigned char pctrl2_uart_id[2];
	unsigned char pctrl2_head[68];
	unsigned char pctrl2_head_len[4];
	unsigned char pctrl2_start_addr[4];
	unsigned char pctrl2_data_len[4];
	unsigned char pctrl2_retry[2];
	unsigned char pctrl2_sendflag_addr[4];
	
	unsigned char pctrl3_enable[2];
	unsigned char pctrl3_uart_id[2];
	unsigned char pctrl3_head[68];
	unsigned char pctrl3_head_len[4];
	unsigned char pctrl3_start_addr[4];
	unsigned char pctrl3_data_len[4];
	unsigned char pctrl3_retry[2];
	unsigned char pctrl3_sendflag_addr[4];

	unsigned char pcoll1_enable[2];
	unsigned char pcoll1_uart_id[2];
	unsigned char pcoll1_auto_send[2];
	unsigned char pcoll1_message[68];
	unsigned char pcoll1_message_len[4];
	unsigned char pcoll1_recv_start_addr[4];
	unsigned char pcoll1_recv_len[4];
	unsigned char pcoll1_recv_data_addr[4];
	unsigned char pcoll1_retry[2];
	unsigned char pcoll1_loop_time[8];
	unsigned char pcoll1_rx_data_head[16];
	unsigned char pcoll1_rx_data_head_len[4];
	unsigned char pcoll1_rx_data_len[4];

	unsigned char pcoll2_enable[2];
	unsigned char pcoll2_uart_id[2];
	unsigned char pcoll2_auto_send[2];
	unsigned char pcoll2_message[68];
	unsigned char pcoll2_message_len[4];
	unsigned char pcoll2_recv_start_addr[4];
	unsigned char pcoll2_recv_len[4];
	unsigned char pcoll2_recv_data_addr[4];
	unsigned char pcoll2_retry[2];
	unsigned char pcoll2_loop_time[8];
	unsigned char pcoll2_rx_data_head[16];
	unsigned char pcoll2_rx_data_head_len[4];
	unsigned char pcoll2_rx_data_len[4];

	unsigned char pcoll3_enable[2];
	unsigned char pcoll3_uart_id[2];
	unsigned char pcoll3_auto_send[2];
	unsigned char pcoll3_message[68];
	unsigned char pcoll3_message_len[4];
	unsigned char pcoll3_recv_start_addr[4];
	unsigned char pcoll3_recv_len[4];
	unsigned char pcoll3_recv_data_addr[4];
	unsigned char pcoll3_retry[2];
	unsigned char pcoll3_loop_time[8];
	unsigned char pcoll3_rx_data_head[16];
	unsigned char pcoll3_rx_data_head_len[4];
	unsigned char pcoll3_rx_data_len[4];
		
} SoftInfor_t;


/*--------------------------------------*/
extern SoftInfor_t gSoft_infor;
extern volatile int gDebugMode;

extern ditc_infor_t ditc_infor;
extern lane_infor_t lane_infor;
extern adc_device_t adc_device;

extern RS485_device_t rs485_1,rs485_2,rs485_3;
extern pctrl_t pctrl1,pctrl2,pctrl3;
extern pcoll_t pcoll1,pcoll2,pcoll3;


#endif
/*-------------------end file----------------------------*/

