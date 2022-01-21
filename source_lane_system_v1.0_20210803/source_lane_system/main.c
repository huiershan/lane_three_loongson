
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "global.h"
#include "application.h"
#include "config.h"

//»Ìº˛–≈œ¢
SoftInfor_t gSoft_infor;

//show soft information
void show_soft_infor(void)
{
	int i;
	memcpy(gSoft_infor.date,__DATE__,strlen(__DATE__));
	memcpy(gSoft_infor.time,__TIME__,strlen(__TIME__));
	
	printf("\n--------------------------------------------\n");
	printf("complie time : %s %s\n",gSoft_infor.date,gSoft_infor.time);
	printf("company : %s\n",gSoft_infor.company);
	printf("software Version : %s\n",gSoft_infor.version);
	
	printf("sensor_k1 : %s\n",gSoft_infor.sensor_k1);
	printf("sensor_k2 : %s\n",gSoft_infor.sensor_k2);
	printf("sensor_k3 : %s\n",gSoft_infor.sensor_k3);
	printf("sensor_k4 : %s\n",gSoft_infor.sensor_k4);
	
	printf("uart1_name : %s\n",gSoft_infor.uart1_name);
	printf("uart1_baudrate : %s\n",gSoft_infor.uart1_baudrate);
	printf("uart1_databits : %s\n",gSoft_infor.uart1_databits);
	printf("uart1_stopbits : %s\n",gSoft_infor.uart1_stopbits);
	printf("uart1_parity : %s\n",gSoft_infor.uart1_parity);

	printf("uart2_name : %s\n",gSoft_infor.uart2_name);
	printf("uart2_baudrate : %s\n",gSoft_infor.uart2_baudrate);
	printf("uart2_databits : %s\n",gSoft_infor.uart2_databits);
	printf("uart2_stopbits : %s\n",gSoft_infor.uart2_stopbits);
	printf("uart2_parity : %s\n",gSoft_infor.uart2_parity);

	printf("uart3_name : %s\n",gSoft_infor.uart3_name);
	printf("uart3_baudrate : %s\n",gSoft_infor.uart3_baudrate);
	printf("uart3_databits : %s\n",gSoft_infor.uart3_databits);
	printf("uart3_stopbits : %s\n",gSoft_infor.uart3_stopbits);
	printf("uart3_parity : %s\n",gSoft_infor.uart3_parity);

	printf("pctrl1_enable : %s\n",gSoft_infor.pctrl1_enable);
	printf("pctrl1_uart_id : %s\n",gSoft_infor.pctrl1_uart_id);
	printf("pctrl1_head : %s\n",gSoft_infor.pctrl1_head);
	printf("pctrl1_len_head : %s\n",gSoft_infor.pctrl1_head_len);
	printf("pctrl1_start_addr : %s\n",gSoft_infor.pctrl1_start_addr);
	printf("pctrl1_data_len : %s\n",gSoft_infor.pctrl1_data_len);
	printf("pctrl1_retry : %s\n",gSoft_infor.pctrl1_retry);
	printf("pctrl1_sendflag_addr : %s\n",gSoft_infor.pctrl1_sendflag_addr);
	printf("pctrl1_loop_time : %s\n",gSoft_infor.pctrl1_loop_time);

	printf("pctrl2_enable : %s\n",gSoft_infor.pctrl2_enable);
	printf("pctrl2_uart_id : %s\n",gSoft_infor.pctrl2_uart_id);
	printf("pctrl2_head : %s\n",gSoft_infor.pctrl2_head);
	printf("pctrl2_len_head : %s\n",gSoft_infor.pctrl2_head_len);
	printf("pctrl2_start_addr : %s\n",gSoft_infor.pctrl2_start_addr);
	printf("pctrl2_data_len : %s\n",gSoft_infor.pctrl2_data_len);
	printf("pctrl2_retry : %s\n",gSoft_infor.pctrl2_retry);
	printf("pctrl2_sendflag_addr : %s\n",gSoft_infor.pctrl2_sendflag_addr);

	printf("pctrl3_enable : %s\n",gSoft_infor.pctrl3_enable);
	printf("pctrl3_uart_id : %s\n",gSoft_infor.pctrl3_uart_id);
	printf("pctrl3_head : %s\n",gSoft_infor.pctrl3_head);
	printf("pctrl3_len_head : %s\n",gSoft_infor.pctrl3_head_len);
	printf("pctrl3_start_addr : %s\n",gSoft_infor.pctrl3_start_addr);
	printf("pctrl3_data_len : %s\n",gSoft_infor.pctrl3_data_len);
	printf("pctrl3_retry : %s\n",gSoft_infor.pctrl3_retry);
	printf("pctrl3_sendflag_addr : %s\n",gSoft_infor.pctrl3_sendflag_addr);

	printf("pcoll1_enable : %s\n",gSoft_infor.pcoll1_enable);
	printf("pcoll1_uart_id : %s\n",gSoft_infor.pcoll1_uart_id);
	printf("pcoll1_auto_send : %s\n",gSoft_infor.pcoll1_auto_send);
	printf("pcoll1_message : %s\n",gSoft_infor.pcoll1_message);
	printf("pcoll1_len_message : %s\n",gSoft_infor.pcoll1_message_len);
	printf("pcoll1_recv_start_addr : %s\n",gSoft_infor.pcoll1_recv_start_addr);
	printf("pcoll1_recv_len : %s\n",gSoft_infor.pcoll1_recv_len);
	printf("pcoll1_recv_data_addr : %s\n",gSoft_infor.pcoll1_recv_data_addr);
	printf("pcoll1_retry : %s\n",gSoft_infor.pcoll1_retry);
	printf("pcoll1_loop_time : %s\n",gSoft_infor.pcoll1_loop_time);
	printf("pcoll1_rx_data_head : %s\n",gSoft_infor.pcoll1_rx_data_head);
	printf("pcoll1_rx_len_head : %s\n",gSoft_infor.pcoll1_rx_data_head_len);
	printf("pcoll1_rx_data_len : %s\n",gSoft_infor.pcoll1_rx_data_len);
	

	printf("pcoll2_enable : %s\n",gSoft_infor.pcoll2_enable);
	printf("pcoll2_uart_id : %s\n",gSoft_infor.pcoll2_uart_id);
	printf("pcoll2_auto_send : %s\n",gSoft_infor.pcoll2_auto_send);
	printf("pcoll2_message : %s\n",gSoft_infor.pcoll2_message);
	printf("pcoll2_len_message : %s\n",gSoft_infor.pcoll2_message_len);
	printf("pcoll2_recv_start_addr : %s\n",gSoft_infor.pcoll2_recv_start_addr);
	printf("pcoll2_recv_len : %s\n",gSoft_infor.pcoll2_recv_len);
	printf("pcoll2_recv_data_addr : %s\n",gSoft_infor.pcoll2_recv_data_addr);
	printf("pcoll2_retry : %s\n",gSoft_infor.pcoll2_retry);
	printf("pcoll2_loop_time : %s\n",gSoft_infor.pcoll2_loop_time);
	printf("pcoll2_rx_data_head : %s\n",gSoft_infor.pcoll2_rx_data_head);
	printf("pcoll2_rx_len_head : %s\n",gSoft_infor.pcoll2_rx_data_head_len);
	printf("pcoll2_rx_data_len : %s\n",gSoft_infor.pcoll2_rx_data_len);
	

	printf("pcoll3_enable : %s\n",gSoft_infor.pcoll3_enable);
	printf("pcoll3_uart_id : %s\n",gSoft_infor.pcoll3_uart_id);
	printf("pcoll3_auto_send : %s\n",gSoft_infor.pcoll3_auto_send);
	printf("pcoll3_message : %s\n",gSoft_infor.pcoll3_message);
	printf("pcoll3_len_message : %s\n",gSoft_infor.pcoll3_message_len);
	printf("pcoll3_recv_start_addr : %s\n",gSoft_infor.pcoll3_recv_start_addr);
	printf("pcoll3_recv_len : %s\n",gSoft_infor.pcoll3_recv_len);
	printf("pcoll3_recv_data_addr : %s\n",gSoft_infor.pcoll3_recv_data_addr);
	printf("pcoll3_retry : %s\n",gSoft_infor.pcoll3_retry);
	printf("pcoll3_loop_time : %s\n",gSoft_infor.pcoll3_loop_time);
	printf("pcoll3_rx_data_head : %s\n",gSoft_infor.pcoll3_rx_data_head);
	printf("pcoll3_rx_len_head : %s\n",gSoft_infor.pcoll3_rx_data_head_len);
	printf("pcoll3_rx_data_len : %s\n",gSoft_infor.pcoll3_rx_data_len);

	printf("--------------------------------------------\n");
		
}

int main(int argc, char *argv[])
{	
	int rel=0,i;
	int vlen = 0;

	memset(&gSoft_infor,0,sizeof(SoftInfor_t));

	GetCfgItem(CFGNAME, "software_version", gSoft_infor.version, &vlen);
 	GetCfgItem(CFGNAME, "company", gSoft_infor.company, &vlen);
	GetCfgItem(CFGNAME, "sensor_k1", gSoft_infor.sensor_k1, &vlen);
	GetCfgItem(CFGNAME, "sensor_k2", gSoft_infor.sensor_k2, &vlen);
	GetCfgItem(CFGNAME, "sensor_k3", gSoft_infor.sensor_k3, &vlen);
	GetCfgItem(CFGNAME, "sensor_k4", gSoft_infor.sensor_k4, &vlen);

	adc_device.sensor_1_k=atof(gSoft_infor.sensor_k1);
	adc_device.sensor_2_k=atof(gSoft_infor.sensor_k2);
	adc_device.sensor_3_k=atof(gSoft_infor.sensor_k3);
	adc_device.sensor_4_k=atof(gSoft_infor.sensor_k4);

	GetCfgItem(CFGNAME, "uart1_name", gSoft_infor.uart1_name, &vlen);
	GetCfgItem(CFGNAME, "uart1_baudrate", gSoft_infor.uart1_baudrate, &vlen);
	GetCfgItem(CFGNAME, "uart1_databits", gSoft_infor.uart1_databits, &vlen);
	GetCfgItem(CFGNAME, "uart1_stopbits", gSoft_infor.uart1_stopbits, &vlen);
	GetCfgItem(CFGNAME, "uart1_parity", gSoft_infor.uart1_parity, &vlen);

	GetCfgItem(CFGNAME, "uart2_name", gSoft_infor.uart2_name, &vlen);
	GetCfgItem(CFGNAME, "uart2_baudrate", gSoft_infor.uart2_baudrate, &vlen);
	GetCfgItem(CFGNAME, "uart2_databits", gSoft_infor.uart2_databits, &vlen);
	GetCfgItem(CFGNAME, "uart2_stopbits", gSoft_infor.uart2_stopbits, &vlen);
	GetCfgItem(CFGNAME, "uart2_parity", gSoft_infor.uart2_parity, &vlen);

	GetCfgItem(CFGNAME, "uart3_name", gSoft_infor.uart3_name, &vlen);
	GetCfgItem(CFGNAME, "uart3_baudrate", gSoft_infor.uart3_baudrate, &vlen);
	GetCfgItem(CFGNAME, "uart3_databits", gSoft_infor.uart3_databits, &vlen);
	GetCfgItem(CFGNAME, "uart3_stopbits", gSoft_infor.uart3_stopbits, &vlen);
	GetCfgItem(CFGNAME, "uart3_parity", gSoft_infor.uart3_parity, &vlen);
	
	strcpy(rs485_1.name,gSoft_infor.uart1_name);
	rs485_1.baudrate=atoi(gSoft_infor.uart1_baudrate);
	rs485_1.databits=atoi(gSoft_infor.uart1_databits);
	rs485_1.stopbits=atoi(gSoft_infor.uart1_stopbits);
	rs485_1.parity=gSoft_infor.uart1_parity[0];
	
	strcpy(rs485_2.name,gSoft_infor.uart2_name);
	rs485_2.baudrate=atoi(gSoft_infor.uart2_baudrate);
	rs485_2.databits=atoi(gSoft_infor.uart2_databits);
	rs485_2.stopbits=atoi(gSoft_infor.uart2_stopbits);
	rs485_2.parity=gSoft_infor.uart2_parity[0];

	strcpy(rs485_3.name,gSoft_infor.uart3_name);
	rs485_3.baudrate=atoi(gSoft_infor.uart3_baudrate);
	rs485_3.databits=atoi(gSoft_infor.uart3_databits);
	rs485_3.stopbits=atoi(gSoft_infor.uart3_stopbits);
	rs485_3.parity=gSoft_infor.uart3_parity[0];

	GetCfgItem(CFGNAME, "pctrl1_enable", gSoft_infor.pctrl1_enable, &vlen);
	GetCfgItem(CFGNAME, "pctrl1_uart_id", gSoft_infor.pctrl1_uart_id, &vlen);
	GetCfgItem(CFGNAME, "pctrl1_head", gSoft_infor.pctrl1_head, &vlen);
	GetCfgItem(CFGNAME, "pctrl1_len_head", gSoft_infor.pctrl1_head_len, &vlen);
	GetCfgItem(CFGNAME, "pctrl1_start_addr", gSoft_infor.pctrl1_start_addr, &vlen);
	GetCfgItem(CFGNAME, "pctrl1_data_len", gSoft_infor.pctrl1_data_len, &vlen);
	GetCfgItem(CFGNAME, "pctrl1_retry", gSoft_infor.pctrl1_retry, &vlen);
	GetCfgItem(CFGNAME, "pctrl1_sendflag_addr", gSoft_infor.pctrl1_sendflag_addr, &vlen);
	GetCfgItem(CFGNAME, "pctrl1_loop_time", gSoft_infor.pctrl1_loop_time, &vlen);

	GetCfgItem(CFGNAME, "pctrl2_enable", gSoft_infor.pctrl2_enable, &vlen);
	GetCfgItem(CFGNAME, "pctrl2_uart_id", gSoft_infor.pctrl2_uart_id, &vlen);
	GetCfgItem(CFGNAME, "pctrl2_head", gSoft_infor.pctrl2_head, &vlen);
	GetCfgItem(CFGNAME, "pctrl2_len_head", gSoft_infor.pctrl2_head_len, &vlen);
	GetCfgItem(CFGNAME, "pctrl2_start_addr", gSoft_infor.pctrl2_start_addr, &vlen);
	GetCfgItem(CFGNAME, "pctrl2_data_len", gSoft_infor.pctrl2_data_len, &vlen);
	GetCfgItem(CFGNAME, "pctrl2_retry", gSoft_infor.pctrl2_retry, &vlen);
	GetCfgItem(CFGNAME, "pctrl2_sendflag_addr", gSoft_infor.pctrl2_sendflag_addr, &vlen);

	GetCfgItem(CFGNAME, "pctrl3_enable", gSoft_infor.pctrl3_enable, &vlen);
	GetCfgItem(CFGNAME, "pctrl3_uart_id", gSoft_infor.pctrl3_uart_id, &vlen);
	GetCfgItem(CFGNAME, "pctrl3_head", gSoft_infor.pctrl3_head, &vlen);
	GetCfgItem(CFGNAME, "pctrl3_len_head", gSoft_infor.pctrl3_head_len, &vlen);
	GetCfgItem(CFGNAME, "pctrl3_start_addr", gSoft_infor.pctrl3_start_addr, &vlen);
	GetCfgItem(CFGNAME, "pctrl3_data_len", gSoft_infor.pctrl3_data_len, &vlen);
	GetCfgItem(CFGNAME, "pctrl3_retry", gSoft_infor.pctrl3_retry, &vlen);
	GetCfgItem(CFGNAME, "pctrl3_sendflag_addr", gSoft_infor.pctrl3_sendflag_addr, &vlen);

	pctrl1.enable=atoi(gSoft_infor.pctrl1_enable);
	pctrl1.uart_id=atoi(gSoft_infor.pctrl1_uart_id);
	pctrl1.head_len=atoi(gSoft_infor.pctrl1_head_len);
	str_to_hex(pctrl1.head,gSoft_infor.pctrl1_head,pctrl1.head_len);
	pctrl1.sart_addr=atoi(gSoft_infor.pctrl1_start_addr);
	pctrl1.data_len=atoi(gSoft_infor.pctrl1_data_len);
	pctrl1.retry=atoi(gSoft_infor.pctrl1_retry);
	pctrl1.sendflag_addr=atoi(gSoft_infor.pctrl1_sendflag_addr);
	pctrl1.loop_time=atoi(gSoft_infor.pctrl1_loop_time);

	pctrl2.enable=atoi(gSoft_infor.pctrl2_enable);
	pctrl2.uart_id=atoi(gSoft_infor.pctrl2_uart_id);
	pctrl2.head_len=atoi(gSoft_infor.pctrl2_head_len);
	str_to_hex(pctrl2.head,gSoft_infor.pctrl2_head,pctrl2.head_len);
	pctrl2.sart_addr=atoi(gSoft_infor.pctrl2_start_addr);
	pctrl2.data_len=atoi(gSoft_infor.pctrl2_data_len);
	pctrl2.retry=atoi(gSoft_infor.pctrl2_retry);
	pctrl2.sendflag_addr=atoi(gSoft_infor.pctrl2_sendflag_addr);
	
	pctrl3.enable=atoi(gSoft_infor.pctrl3_enable);
	pctrl3.uart_id=atoi(gSoft_infor.pctrl3_uart_id);
	pctrl3.head_len=atoi(gSoft_infor.pctrl3_head_len);
	str_to_hex(pctrl3.head,gSoft_infor.pctrl3_head,pctrl3.head_len);
	pctrl3.sart_addr=atoi(gSoft_infor.pctrl3_start_addr);
	pctrl3.data_len=atoi(gSoft_infor.pctrl3_data_len);
	pctrl3.retry=atoi(gSoft_infor.pctrl3_retry);
	pctrl3.sendflag_addr=atoi(gSoft_infor.pctrl3_sendflag_addr);

	GetCfgItem(CFGNAME, "pcoll1_enable", gSoft_infor.pcoll1_enable, &vlen);
	GetCfgItem(CFGNAME, "pcoll1_uart_id", gSoft_infor.pcoll1_uart_id, &vlen);
	GetCfgItem(CFGNAME, "pcoll1_auto_send", gSoft_infor.pcoll1_auto_send, &vlen);
	GetCfgItem(CFGNAME, "pcoll1_message", gSoft_infor.pcoll1_message, &vlen);
	GetCfgItem(CFGNAME, "pcoll1_len_message", gSoft_infor.pcoll1_message_len, &vlen);
	GetCfgItem(CFGNAME, "pcoll1_recv_start_addr", gSoft_infor.pcoll1_recv_start_addr, &vlen);
	GetCfgItem(CFGNAME, "pcoll1_recv_len", gSoft_infor.pcoll1_recv_len, &vlen);
	GetCfgItem(CFGNAME, "pcoll1_recv_data_addr", gSoft_infor.pcoll1_recv_data_addr, &vlen);
	GetCfgItem(CFGNAME, "pcoll1_retry", gSoft_infor.pcoll1_retry, &vlen);
	GetCfgItem(CFGNAME, "pcoll1_loop_time", gSoft_infor.pcoll1_loop_time, &vlen);
	GetCfgItem(CFGNAME, "pcoll1_rx_data_head", gSoft_infor.pcoll1_rx_data_head, &vlen);
	GetCfgItem(CFGNAME, "pcoll1_rx_len_head", gSoft_infor.pcoll1_rx_data_head_len, &vlen);
	GetCfgItem(CFGNAME, "pcoll1_rx_data_len", gSoft_infor.pcoll1_rx_data_len, &vlen);

	GetCfgItem(CFGNAME, "pcoll2_enable", gSoft_infor.pcoll2_enable, &vlen);
	GetCfgItem(CFGNAME, "pcoll2_uart_id", gSoft_infor.pcoll2_uart_id, &vlen);
	GetCfgItem(CFGNAME, "pcoll2_auto_send", gSoft_infor.pcoll2_auto_send, &vlen);
	GetCfgItem(CFGNAME, "pcoll2_message", gSoft_infor.pcoll2_message, &vlen);
	GetCfgItem(CFGNAME, "pcoll2_len_message", gSoft_infor.pcoll2_message_len, &vlen);
	GetCfgItem(CFGNAME, "pcoll2_recv_start_addr", gSoft_infor.pcoll2_recv_start_addr, &vlen);
	GetCfgItem(CFGNAME, "pcoll2_recv_len", gSoft_infor.pcoll2_recv_len, &vlen);
	GetCfgItem(CFGNAME, "pcoll2_recv_data_addr", gSoft_infor.pcoll2_recv_data_addr, &vlen);
	GetCfgItem(CFGNAME, "pcoll2_retry", gSoft_infor.pcoll2_retry, &vlen);
	GetCfgItem(CFGNAME, "pcoll2_loop_time", gSoft_infor.pcoll2_loop_time, &vlen);
	GetCfgItem(CFGNAME, "pcoll2_rx_data_head", gSoft_infor.pcoll2_rx_data_head, &vlen);
	GetCfgItem(CFGNAME, "pcoll2_rx_len_head", gSoft_infor.pcoll2_rx_data_head_len, &vlen);
	GetCfgItem(CFGNAME, "pcoll2_rx_data_len", gSoft_infor.pcoll2_rx_data_len, &vlen);

	GetCfgItem(CFGNAME, "pcoll3_enable", gSoft_infor.pcoll3_enable, &vlen);
	GetCfgItem(CFGNAME, "pcoll3_uart_id", gSoft_infor.pcoll3_uart_id, &vlen);
	GetCfgItem(CFGNAME, "pcoll3_auto_send", gSoft_infor.pcoll3_auto_send, &vlen);
	GetCfgItem(CFGNAME, "pcoll3_message", gSoft_infor.pcoll3_message, &vlen);
	GetCfgItem(CFGNAME, "pcoll3_len_message", gSoft_infor.pcoll3_message_len, &vlen);
	GetCfgItem(CFGNAME, "pcoll3_recv_start_addr", gSoft_infor.pcoll3_recv_start_addr, &vlen);
	GetCfgItem(CFGNAME, "pcoll3_recv_len", gSoft_infor.pcoll3_recv_len, &vlen);
	GetCfgItem(CFGNAME, "pcoll3_recv_data_addr", gSoft_infor.pcoll3_recv_data_addr, &vlen);
	GetCfgItem(CFGNAME, "pcoll3_retry", gSoft_infor.pcoll3_retry, &vlen);
	GetCfgItem(CFGNAME, "pcoll3_loop_time", gSoft_infor.pcoll3_loop_time, &vlen);
	GetCfgItem(CFGNAME, "pcoll3_rx_data_head", gSoft_infor.pcoll3_rx_data_head, &vlen);
	GetCfgItem(CFGNAME, "pcoll3_rx_len_head", gSoft_infor.pcoll3_rx_data_head_len, &vlen);
	GetCfgItem(CFGNAME, "pcoll3_rx_data_len", gSoft_infor.pcoll3_rx_data_len, &vlen);

	pcoll1.enable=atoi(gSoft_infor.pcoll1_enable);
	pcoll1.uart_id=atoi(gSoft_infor.pcoll1_uart_id);
	pcoll1.auto_send=atoi(gSoft_infor.pcoll1_auto_send);
	pcoll1.message_len=atoi(gSoft_infor.pcoll1_message_len);
	str_to_hex(pcoll1.message,gSoft_infor.pcoll1_message,pcoll1.message_len);
	pcoll1.recv_sart_addr=atoi(gSoft_infor.pcoll1_recv_start_addr);
	pcoll1.recv_len=atoi(gSoft_infor.pcoll1_recv_len);
	pcoll1.recv_data_addr=atoi(gSoft_infor.pcoll1_recv_data_addr);
	pcoll1.retry=atoi(gSoft_infor.pcoll1_retry);
	pcoll1.loop_time=atoi(gSoft_infor.pcoll1_loop_time);
	pcoll1.rx_data_head_len=atoi(gSoft_infor.pcoll1_rx_data_head_len);
	str_to_hex(pcoll1.rx_data_head,gSoft_infor.pcoll1_rx_data_head,pcoll1.rx_data_head_len);
	pcoll1.rx_data_len=atoi(gSoft_infor.pcoll1_rx_data_len);

	pcoll2.enable=atoi(gSoft_infor.pcoll2_enable);
	pcoll2.uart_id=atoi(gSoft_infor.pcoll2_uart_id);
	pcoll2.auto_send=atoi(gSoft_infor.pcoll2_auto_send);
	pcoll2.message_len=atoi(gSoft_infor.pcoll2_message_len);
	str_to_hex(pcoll2.message,gSoft_infor.pcoll2_message,pcoll2.message_len);
	pcoll2.recv_sart_addr=atoi(gSoft_infor.pcoll2_recv_start_addr);
	pcoll2.recv_len=atoi(gSoft_infor.pcoll2_recv_len);
	pcoll2.recv_data_addr=atoi(gSoft_infor.pcoll2_recv_data_addr);
	pcoll2.retry=atoi(gSoft_infor.pcoll2_retry);
	pcoll2.loop_time=atoi(gSoft_infor.pcoll2_loop_time);
	pcoll2.rx_data_head_len=atoi(gSoft_infor.pcoll2_rx_data_head_len);
	str_to_hex(pcoll2.rx_data_head,gSoft_infor.pcoll2_rx_data_head,pcoll2.rx_data_head_len);
	pcoll2.rx_data_len=atoi(gSoft_infor.pcoll2_rx_data_len);

	pcoll3.enable=atoi(gSoft_infor.pcoll3_enable);
	pcoll3.uart_id=atoi(gSoft_infor.pcoll3_uart_id);
	pcoll3.auto_send=atoi(gSoft_infor.pcoll3_auto_send);
	pcoll3.message_len=atoi(gSoft_infor.pcoll3_message_len);
	str_to_hex(pcoll3.message,gSoft_infor.pcoll3_message,pcoll3.message_len);
	pcoll3.recv_sart_addr=atoi(gSoft_infor.pcoll3_recv_start_addr);
	pcoll3.recv_len=atoi(gSoft_infor.pcoll3_recv_len);
	pcoll3.recv_data_addr=atoi(gSoft_infor.pcoll3_recv_data_addr);
	pcoll3.retry=atoi(gSoft_infor.pcoll3_retry);
	pcoll3.loop_time=atoi(gSoft_infor.pcoll3_loop_time);
	pcoll3.rx_data_head_len=atoi(gSoft_infor.pcoll3_rx_data_head_len);
	str_to_hex(pcoll3.rx_data_head,gSoft_infor.pcoll3_rx_data_head,pcoll3.rx_data_head_len);
	pcoll3.rx_data_len=atoi(gSoft_infor.pcoll3_rx_data_len);

	show_soft_infor();
	
	if(application_start(rel) != 0)
	{
		printf("application enter error\n");
		return -4;
	}

	return 0;
}


