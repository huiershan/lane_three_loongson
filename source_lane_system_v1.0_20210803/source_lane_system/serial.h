

#ifndef _SERIAL_H
#define _SERIAL_H

typedef struct{
	char name[12];
	int baudrate;
	int databits;
	int stopbits;
	int parity;
	int fd;
	int tx_flag;
	int rx_flag;
}RS485_device_t;


typedef struct{
	int enable;
	int uart_id;
	unsigned char head[34];
	int head_len;
	int sart_addr;
	int data_len;
	int retry;
	int sendflag_addr;
	unsigned char cmdbuf[130];
	int loop_time;
}pctrl_t;


typedef struct{
	int enable;
	int uart_id;
	int auto_send;
	unsigned char message[34];
	int message_len;
	int recv_sart_addr;
	int recv_len;
	int recv_data_addr;
	int retry;
	int loop_time;
	unsigned char rx_data_head[8];
	char rx_data_head_len;
	int rx_data_len;
	unsigned char recvbuf[130];
}pcoll_t;

int serial_open(char *path,int baudrate, int databits,int stopbits, int parity);
int RS485_init(void);
int rs485_ctrl_device(void);

int rs485_1_send_collect_cmd(void);
int rs485_2_send_collect_cmd(void);
int rs485_3_send_collect_cmd(void);

int rs485_1_process_collect_data(void);
int rs485_2_process_collect_data(void);
int rs485_3_process_collect_data(void);

int rs485_tx_led(void);
int rs485_rx_led(void);
	

#endif
