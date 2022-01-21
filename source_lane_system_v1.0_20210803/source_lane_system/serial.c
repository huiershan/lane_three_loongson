#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include "serial.h"
#include "modbus_server.h"
#include "sysutil_d.h"


RS485_device_t rs485_1,rs485_2,rs485_3;
pctrl_t pctrl1,pctrl2,pctrl3;
pcoll_t pcoll1,pcoll2,pcoll3;

int set_serial_opt(int fd, int baudrate, int databits,int stopbits, int parity)
{
	struct termios old_flags,new_flags;
	//get old opt
	if(tcgetattr(fd,&old_flags) != 0)
	{
		printf("Error serial : get attr fail !\n");
		return -1;
	}

	new_flags = old_flags;
	new_flags.c_cflag |= (CLOCAL | CREAD);
	new_flags.c_cflag &= ~CSIZE;
	
	switch (databits)
    	{
    	case 7:
     		 new_flags.c_cflag |= CS7;
     		 break;
    	case 8:
      		new_flags.c_cflag |= CS8;
      		break;
	default:
      		printf ("Unsupported uart data bits\n");
		return -1;
   	 }

	switch (stopbits)
    	{
   	 case 1:
      		new_flags.c_cflag &= ~CSTOPB;
      		break;
    	case 2:
      		new_flags.c_cflag |= CSTOPB;
      		break;
    	default:
      		printf ("Unsupported uart stopbits\n");
		return -1;
   	 }

	switch (parity)
   	 {
    	case 'n':
    	case 'N':
     	 	new_flags.c_cflag &= ~PARENB;	/* Clear parity enable */
      		new_flags.c_iflag &= ~INPCK;	/* Enable parity checking */
      		break;
    	case 'o':
    	case 'O':
      		new_flags.c_cflag |= (PARODD | PARENB);
      		new_flags.c_iflag |= INPCK;	/* Disnable parity checking */
      		break;
    	case 'e':
    	case 'E':
      		new_flags.c_cflag |= PARENB;	/* Enable parity */
      		new_flags.c_cflag &= ~PARODD;
      		new_flags.c_iflag |= INPCK;	/* Disnable parity checking */
      		break;
    	default:
    		printf ("Unsupported uart prity\n");
		return -1;
    	}
	
	switch(baudrate)
	{
	case 9600:
		cfsetispeed(&new_flags,B9600);
		cfsetospeed(&new_flags,B9600);
		break;
	case 19200:
		cfsetispeed(&new_flags,B19200);
		cfsetospeed(&new_flags,B19200);
		break;
	case 38400:
		cfsetispeed(&new_flags,B38400);
		cfsetospeed(&new_flags,B38400);
		break;
	case 57600:
		cfsetispeed(&new_flags,B57600);
		cfsetospeed(&new_flags,B57600);
		break;
	case 115200:
		cfsetispeed(&new_flags,B115200);
		cfsetospeed(&new_flags,B115200);
		break;
	default:
		printf ("Unsupported uart baudrate\n");
		return -1;

	}

	//选择原始模式	在原始模式下，串口输入数据是不经过处理的，
	//在串口接口接收的数据被完整保留。要使串口设备工作在原始模式，
	//需要关闭ICANON、ECHO、ECHOE和ISIG选项
	new_flags.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	//使用原始输出，就是禁用输出处理，使数据能不经过处理、
	//过滤地完整地输出到串口接口。当OPOST被禁止，c_oflag其它选项也被忽略
	new_flags.c_oflag &= ~OPOST;
	new_flags.c_oflag &= ~(ONLCR | OCRNL);

	 //禁用流控
	new_flags.c_cflag &= ~CRTSCTS;
	new_flags.c_iflag &= ~(ONLCR | OCRNL);
	new_flags.c_iflag &= ~(IXON | IXOFF | IXANY);


	//设置等待时间和最小接收字符
	new_flags.c_cc[VTIME] = 0;
	new_flags.c_cc[VMIN] = 0;

	//处理未接收的字符
	tcflush(fd,TCIFLUSH);
	tcflush(fd,TCOFLUSH);

	//激活新配置
	if(tcsetattr(fd,TCSANOW,&new_flags) != 0)
	{
		printf("Error serial : set attr fail !\n");
		return -1;
	}
	
	return 0;
}

int serial_open(char *path,int baudrate, int databits,int stopbits, int parity)
{
	int fd;

	fd = open(path,O_RDWR | O_NOCTTY | O_NDELAY);
	if(fd < 0)
	{
		printf("Error serial : open %s fail !\n",path);
		return -1;
	}
	printf("serial : %s open success !\n",path);

	//make serial O_NONBLOCK  
	if(fcntl(fd,F_SETFL,O_NONBLOCK) < 0)
	{
		printf("Error serial : block set fail !\n");
		return -1;
	}

	if(isatty(fd) == 0)
	{
		printf("This is not a terminal device \n");
		return -1;
	}

	if(set_serial_opt(fd,baudrate,databits,stopbits,parity) != 0)
	{
		printf("Error serial : opt set fail !\n");
		return -1;
	}
	
	return fd;
}

int RS485_init(void)
{
	int ret=0;
	int fd;

	fd=serial_open(rs485_1.name,rs485_1.baudrate,rs485_1.databits,rs485_1.stopbits,rs485_1.parity);
	if(fd<0)
	{
		printf("rs485_1 open fail\n");
		ret=-1;
	}
	else
	{
		rs485_1.fd=fd;
	}

	fd=serial_open(rs485_2.name,rs485_2.baudrate,rs485_2.databits,rs485_2.stopbits,rs485_2.parity);
	if(fd<0)
	{
		printf("rs485_2 open fail\n");
		ret=-1;
	}
	else
	{
		rs485_2.fd=fd;
	}

	fd=serial_open(rs485_3.name,rs485_3.baudrate,rs485_3.databits,rs485_3.stopbits,rs485_3.parity);
	if(fd<0)
	{
		printf("rs485_3 open fail\n");
		ret=-1;
	}
	else
	{
		rs485_3.fd=fd;
	}

	return ret;
	
}


void pctrl1_send_buff_handle()
{
	int i=0,len=0;
	unsigned crc16=0;
	unsigned buf[64];

	memset(pctrl1.cmdbuf,0,sizeof(pctrl1.cmdbuf));
	memset(buf,0,sizeof(buf));

	len=pctrl1.head_len+pctrl1.data_len*2;
	
	memcpy(pctrl1.cmdbuf,pctrl1.head,pctrl1.head_len);

	for(i=0; i<pctrl1.data_len; i++)
	{
		pctrl1.cmdbuf[pctrl1.head_len+i*2]=mb_mapping->tab_registers[pctrl1.sart_addr+i]>>8 & 0xff;
		pctrl1.cmdbuf[pctrl1.head_len+i*2+1]=mb_mapping->tab_registers[pctrl1.sart_addr+i] & 0xff;
	}
	memcpy(buf,pctrl1.cmdbuf,len);
	crc16=crc16table(buf,len);
	pctrl1.cmdbuf[len]=crc16 & 0xff;
	pctrl1.cmdbuf[len+1]=(crc16>>8) & 0xff;
		
}

void pctrl2_send_buff_handle()
{
	int i=0,len=0;
	unsigned crc16=0;
	unsigned buf[64];

	memset(pctrl2.cmdbuf,0,sizeof(pctrl2.cmdbuf));
	memset(buf,0,sizeof(buf));

	len=pctrl2.head_len+pctrl2.data_len*2;
	
	memcpy(pctrl2.cmdbuf,pctrl2.head,pctrl2.head_len);

	for(i=0; i<pctrl2.data_len; i++)
	{
		pctrl2.cmdbuf[pctrl2.head_len+i*2]=mb_mapping->tab_registers[pctrl2.sart_addr+i]>>8 & 0xff;
		pctrl2.cmdbuf[pctrl2.head_len+i*2+1]=mb_mapping->tab_registers[pctrl2.sart_addr+i] & 0xff;
	}
	memcpy(buf,pctrl2.cmdbuf,len);
	crc16=crc16table(buf,len);
	pctrl2.cmdbuf[len]=crc16 & 0xff;
	pctrl2.cmdbuf[len+1]=(crc16>>8) & 0xff;
}

void pctrl3_send_buff_handle()
{
	int i=0,len=0;
	unsigned crc16=0;
	unsigned buf[64];

	memset(pctrl3.cmdbuf,0,sizeof(pctrl3.cmdbuf));
	memset(buf,0,sizeof(buf));

	len=pctrl3.head_len+pctrl3.data_len*2;
	
	memcpy(pctrl3.cmdbuf,pctrl3.head,pctrl3.head_len);

	for(i=0; i<pctrl3.data_len; i++)
	{ 
		pctrl3.cmdbuf[pctrl3.head_len+i*2]=mb_mapping->tab_registers[pctrl3.sart_addr+i]>>8 & 0xff;
		pctrl3.cmdbuf[pctrl3.head_len+i*2+1]=mb_mapping->tab_registers[pctrl3.sart_addr+i] & 0xff;
	}
	memcpy(buf,pctrl3.cmdbuf,len);
	crc16=crc16table(buf,len);
	pctrl3.cmdbuf[len]=crc16 & 0xff;
	pctrl3.cmdbuf[len+1]=(crc16>>8) & 0xff;
		
}

int rs485_send_data(int uart_id,char *data,int data_len,int retry)
{
	int fd;
	int write_len=0,count=0;
	int write_timers=0;
	char *write_buf=data;

	if(uart_id==1) fd=rs485_1.fd;
	else if(uart_id==2) fd=rs485_2.fd;
	else if(uart_id==3) fd=rs485_3.fd;

	//printf("data_len=%d retry=%d\n",data_len,retry);

	while(count<data_len)
	{
		write_len=write(fd,write_buf,(data_len-count));
		
		if(write_len>0)
		{
			count+=write_len;
			write_buf+=write_len;
			//printf("write_len=%d count=%d\n",write_len,count);
			write_len=0;
			if(uart_id==1) rs485_1.tx_flag=1;
		}
		retry--;	
		if(retry==0)
		{
			return -1;
		}
	}
	return 0;
}

int rs485_recv_data(int uart_id,char *data,int len)
{
	int fd;
	int read_len=0,count=0;
	char *read_buf=data;

	if(uart_id==1) fd=rs485_1.fd;
	else if(uart_id==2) fd=rs485_2.fd;
	else if(uart_id==3) fd=rs485_3.fd;

	while(count<len)
	{
		read_len=read(fd,read_buf,(len-count));
		if(read_len>0)
		{
			count+=read_len;
			read_buf+=read_len;
			//printf("read_len=%d count=%d\n",read_len,count);
			
			read_len=0;
			
			if(uart_id==1) rs485_1.rx_flag=1;	
		}
		else
		{
			return -1;
		}
	}	
	return 0;
}

int rs485_ctrl_device(void)
{
	unsigned int looptime=0;
	int len=0;

	if(pctrl1.loop_time<=0) looptime=2000000;
	else looptime=pctrl1.loop_time*1000;
	
	while(1)
	{
		usleep(looptime);

		if(pctrl1.enable==1)
		{
			if(mb_mapping->tab_registers[pctrl1.sendflag_addr]==1)
			{
				pctrl1_send_buff_handle();
				len=pctrl1.head_len+pctrl1.data_len*2+2;
				if(rs485_send_data(pctrl1.uart_id, pctrl1.cmdbuf,len,pctrl1.retry)<0)
 				{
 					mb_mapping->tab_registers[pctrl1.sendflag_addr]=0x0003; //send fail
				}
				else
				{
					mb_mapping->tab_registers[pctrl1.sendflag_addr]=0x0002;  //send sucess
				}
			}
		}

		if(pctrl2.enable==1)
		{
			if(mb_mapping->tab_registers[pctrl2.sendflag_addr]==1)
			{
				pctrl2_send_buff_handle();
				len=pctrl2.head_len+pctrl2.data_len*2+2;
				if(rs485_send_data(pctrl2.uart_id, pctrl2.cmdbuf,len,pctrl2.retry)<0)
 				{
 					mb_mapping->tab_registers[pctrl2.sendflag_addr]=0x0003; //send fail
				}
				else
				{
					mb_mapping->tab_registers[pctrl2.sendflag_addr]=0x0002;  //send sucess
				}
			}
		}

		if(pctrl3.enable==1)
		{
			if(mb_mapping->tab_registers[pctrl3.sendflag_addr]==1)
			{
				pctrl3_send_buff_handle();
				len=pctrl3.head_len+pctrl3.data_len*2+2;
				if(rs485_send_data(pctrl3.uart_id, pctrl3.cmdbuf,len,pctrl3.retry)<0)
 				{
 					mb_mapping->tab_registers[pctrl3.sendflag_addr]=0x0003; //send fail
				}
				else
				{
					mb_mapping->tab_registers[pctrl3.sendflag_addr]=0x0002;  //send sucess
				}
			}
		}

	}

}


int rs485_1_send_collect_cmd(void)
{
	unsigned int looptime=0;

	if(pcoll1.loop_time<=0) looptime=2000000;
	else looptime=pcoll1.loop_time*1000;
	
	while(1)
	{
		usleep(looptime);
		if(pcoll1.enable==1 && pcoll1.auto_send==1)
		{
			rs485_send_data(pcoll1.uart_id,pcoll1.message,pcoll1.message_len,pcoll1.retry);
		}		
	}
}

int rs485_2_send_collect_cmd(void)
{
	unsigned int looptime=0;

	if(pcoll2.loop_time<=0) looptime=2000000;
	else looptime=pcoll2.loop_time*1000;
	
	while(1)
	{
		usleep(looptime);
		if(pcoll2.enable==1 && pcoll2.auto_send==1)
		{
			rs485_send_data(pcoll2.uart_id,pcoll2.message,pcoll2.message_len,pcoll2.retry);
		}		
	}
}

int rs485_3_send_collect_cmd(void)
{
	unsigned int looptime=0;

	if(pcoll3.loop_time<=0) looptime=2000000;
	else looptime=pcoll3.loop_time*1000;
	
	while(1)
	{
		usleep(looptime);
		if(pcoll3.enable==1 && pcoll3.auto_send==1)
		{
			rs485_send_data(pcoll3.uart_id,pcoll3.message,pcoll3.message_len,pcoll3.retry);
		}		
	}
}

int rs485_1_process_collect_data(void)
{
	int ret=0,i;
	int count=0;
	int error_count=0;
	
	while(1)
	{
		usleep(200000);
		if(pcoll1.enable==1)
		{
			memset(pcoll1.recvbuf,0,sizeof(pcoll1.recvbuf));
			count=0;
			
			ret=rs485_recv_data(pcoll1.uart_id,pcoll1.recvbuf,pcoll1.rx_data_len);
			//printf("ret=%d\n",ret);
			
			if(ret<0)
			{
				error_count++;
				//printf("error_count=%d\n",error_count);
				if(error_count==600)
				{
					for(i=0;i<pcoll1.recv_len/2;i++)
					{
						mb_mapping->tab_input_registers[pcoll1.recv_data_addr+i]=0x0000;
						//printf("pcoll1.recv_data_addr+i=[%d]+[%d]\n",pcoll1.recv_data_addr,i);
					}
					//error_count=0;
				}
			}
			else
			{
				error_count=0;
				for(i=0;i<pcoll1.rx_data_head_len;i++)
				{
					if(pcoll1.rx_data_head[i]==pcoll1.recvbuf[i]) 
					{
						count++;
					}
				}
				if(count==pcoll1.rx_data_head_len)
				{
					for(i=0;i<pcoll1.recv_len/2;i++)
					{
						mb_mapping->tab_input_registers[pcoll1.recv_data_addr+i]=pcoll1.recvbuf[pcoll1.recv_sart_addr-1+i*2]<<8 |pcoll1.recvbuf[pcoll1.recv_sart_addr+i*2];
					}
					
				}
			}
			
		}		
	}
}

int rs485_2_process_collect_data(void)
{
	int ret=0,i;
	int count=0;
	int error_count=0;
	
	while(1)
	{
		usleep(200000);
		if(pcoll2.enable==1)
		{
			memset(pcoll2.recvbuf,0,sizeof(pcoll2.recvbuf));
			count=0;
			
			ret=rs485_recv_data(pcoll2.uart_id,pcoll2.recvbuf,pcoll2.rx_data_len);
			if(ret<0)
			{
				error_count++;
				if(error_count==600)
				{
					for(i=0;i<pcoll2.recv_len/2;i++)
					{
						mb_mapping->tab_input_registers[pcoll2.recv_data_addr+i]=0x0000;
					}
					//error_count=0;
				}		
			}
			else
			{
				error_count=0;
				for(i=0;i<pcoll2.rx_data_head_len;i++)
				{
					if(pcoll2.rx_data_head[i]==pcoll2.recvbuf[i]) 
					{
						count++;
					}
				}
				if(count==pcoll2.rx_data_head_len)
				{
					for(i=0;i<pcoll2.recv_len/2;i++)
					{
						mb_mapping->tab_input_registers[pcoll2.recv_data_addr+i]=pcoll2.recvbuf[pcoll2.recv_sart_addr-1+i*2]<<8 |pcoll2.recvbuf[pcoll2.recv_sart_addr+i*2];
					}
					
				}
			}
			
		}		
	}
}

int rs485_3_process_collect_data(void)
{
	int ret=0,i;
	int count=0;

	int error_count=0;
	
	while(1)
	{
		usleep(200000);
		if(pcoll3.enable==1)
		{
			memset(pcoll3.recvbuf,0,sizeof(pcoll3.recvbuf));
			count=0;
			
			ret=rs485_recv_data(pcoll3.uart_id,pcoll3.recvbuf,pcoll3.rx_data_len);
			if(ret<0)
			{
				error_count++;
				if(error_count==600)
				{
					for(i=0;i<pcoll3.recv_len/2;i++)
					{
						mb_mapping->tab_input_registers[pcoll3.recv_data_addr+i]=0x0000;
					}
					//error_count=0;
				}
						
			}
			else
			{
				error_count=0;
				for(i=0;i<pcoll3.rx_data_head_len;i++)
				{
					if(pcoll3.rx_data_head[i]==pcoll3.recvbuf[i]) 
					{
						count++;
					}
				}
				if(count==pcoll3.rx_data_head_len)
				{
					for(i=0;i<pcoll3.recv_len/2;i++)
					{
						mb_mapping->tab_input_registers[pcoll3.recv_data_addr+i]=pcoll3.recvbuf[pcoll3.recv_sart_addr-1+i*2]<<8 |pcoll3.recvbuf[pcoll3.recv_sart_addr+i*2];
					}
					
				}
			}
			
		}		
	}
}

int rs485_tx_led(void)
{
	while(1)
	{
		usleep(1000000);
		
		if(rs485_1.tx_flag==1)
		{
			usleep(1000000);
			led_uart_tx_display(1);
			usleep(1000000);
			led_uart_tx_display(0);
			rs485_1.tx_flag=0;
		}
		else
		{
			led_uart_tx_display(0);		
		}
	}
}

int rs485_rx_led(void)
{
	while(1)
	{
		usleep(1000000);
		
		if(rs485_1.rx_flag==1)
		{
			usleep(1000000);
			led_uart_rx_display(1);
			usleep(1000000);
			led_uart_rx_display(0);
			rs485_1.rx_flag=0;
		}
		else
		{
			led_uart_rx_display(0);		
		}
	}
}