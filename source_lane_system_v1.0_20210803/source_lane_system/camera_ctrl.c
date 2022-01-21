
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "camera_ctrl.h"
#include "sysutil_d.h"
#include "debug.h"
#include "module_ctrl.h"
#include "io_map.h"
#include "global.h"

//camera io control
int camera_io_ctrl(int num)
{
	if(0 == num)
	{
		if(set_io(B_PINK_ADDR,B_PINK_PORT,IO_OFF) != 0)
		{
			return -2;
		}

		msleep_m(100);
		//usleep(100*1000);

		if(set_io(C_GRAY_ADDR,C_GRAY_PORT,IO_OFF) != 0)
		{
			return -2;
		}
	}
	else if(1 == num)
	{
		if(set_io(C_GRAY_ADDR,C_GRAY_PORT,IO_OFF) != 0)
		{
			return -2;
		}
		msleep_m(100);
		//usleep(100*1000);

		if(set_io(B_PINK_ADDR,B_PINK_PORT,IO_ON) != 0)
		{
			return -2;
		}
	}
	else if(2 == num)
	{
		if(set_io(B_PINK_ADDR,B_PINK_PORT,IO_OFF) != 0)
		{
			return -2;
		}

		msleep_m(100);
		//usleep(100*1000);

		if(set_io(C_GRAY_ADDR,C_GRAY_PORT,IO_ON) != 0)
		{
			return -2;
		}
	}
	else if(3 == num)
	{
		if(set_io(B_PINK_ADDR,B_PINK_PORT,IO_OFF) != 0)
		{
			return -2;
		}

		msleep_m(100);
		//usleep(100*1000);

		if(set_io(C_GRAY_ADDR,C_GRAY_PORT,IO_OFF) != 0)
		{
			return -2;
		}

		msleep_m(100);
		//usleep(100*1000);
		if(set_io(A_WHITE_ADDR,A_WHITE_PORT,IO_OFF) != 0)
		{
			return -2;
		}

		return 0;
	}
	else
	{
		return -1;
	}

	msleep_m(100);
	//usleep(100*1000);
	if(set_io(A_WHITE_ADDR,A_WHITE_PORT,IO_ON) != 0)
	{
		return -2;
	}

	msleep_m(100);
	//usleep(100*1000);

	if(set_io(A_WHITE_ADDR,A_WHITE_PORT,IO_OFF) != 0)
	{
		return -2;
	}

	return 0;
}

//create tcp client socket
static int createTcpClientSocket(void)
{
	int listenfd;

	listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd < 0)
	{
		print_error("scoket fail !\n");
		return -1;
	}
	
	return listenfd;
}


static int deal_recv_data(char *pdest, char *psrc,int size)
{
	int k1,k2;
	int i,t;

	for(i=0; i<size; ++i)
	{
		if(psrc[i] == '(')
		{
			k1 = i+1;
		}

		if(psrc[i] == ')')
		{
			k2 = i;
		}
	}

	for(i=0,t=k1; t<k2; ++t,++i)
	{
		if(('P' == psrc[t])||('p' == psrc[t]) ||('1' == psrc[t]))
			pdest[i] = '1';
		else
			pdest[i] = '0';
	}

	return i;
}

//camera client
int get_led_state_by_camera(char* pBuf,char* pAddr, int port)
{
	int fd,size;
	struct sockaddr_in servaddr;
	int i,maxfd,nready;
	fd_set rset,errset,allset;
	struct timeval timeout ={3,0};
	int rel=0;

	char buf[128];
	char *p;
	p = pBuf;

	printf("client addr : %s ; port=%d\n",pAddr,port);
	//create socket
	fd = createTcpClientSocket();
	if(fd<0)
	{
		return -1;
	}
	
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(pAddr);
	servaddr.sin_port	=	htons(port);

	maxfd = fd;
    FD_ZERO(&allset);
	FD_SET(fd,&allset);

	//connect server
	if(connect(fd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
	{
		close(fd);
		print_error("client connect error\n");
		return -1;
	}

	print_infor("camera working....\n");
	rel = 0;
	for(i=0; i<3; ++i)
	{

		memset(buf,'\0',sizeof(buf));
		rset = allset;
		errset = allset;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		msleep_m(100);
		//usleep(100*1000);
		//camera io control
		if(camera_io_ctrl(i) < 0)
		{
			close(fd);
			print_error("camera io control fail\n");
			return -1;
		}

		nready = select(maxfd+1,&rset,NULL,&errset,&timeout);
		if(nready < 0)
		{
			print_error("select time out or error\n");
			camera_io_ctrl(3);
			close(fd);
			return -1;
		}
		
		//get data
		//判断所否有请求链接事件
		if(FD_ISSET(fd,&rset))
		{
			size = recv(fd,buf,sizeof(buf),0);
			printf("camera (%d) recive data(%d) %s\n",i+1,size,buf);
			//deal data
			rel = deal_recv_data(p,buf,strlen(buf));
			p = p+rel;
			continue;
		}
		//error fd check
		if(FD_ISSET(fd, &errset))
		{
			printf("client error\r\n");
			camera_io_ctrl(3);
			close(fd);
			return -1;
		}
	}

	camera_io_ctrl(3);
	close(fd);

	return rel;
}

//camera selfcheck
int camera_selfcheck(void)
{
	int ret=0;
	char status[100];
	char *p;

	memset(status,'\0',sizeof(status));
	ret  = get_led_state_by_camera(status, gSoft_infor.client_addr, gSoft_infor.client_port);
	if(ret < 0)
	{
		printf("camera read FAIL\n");
		return -1;
	}

	p=status;
	printf("camera data(%d) : %s\n",strlen(p),status);
	if(strlen(p) != 89)
	{
		printf("camera read data count(%d) != 89\n",strlen(p));
		return -2;
	}

	return 0;
}

/*--------------------------------------------------------------------*/
//end file


