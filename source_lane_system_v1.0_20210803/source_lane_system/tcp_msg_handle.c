
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <sys/time.h>
#include <time.h>

#include "tcp_msg_handle.h"
#include "debug.h"
#include "global.h"
#include "ini_d.h"
#include "sysutil_d.h"

#include "button.h"
#include "light_ctrl.h"

#include "meter_ctrl.h"
#include "fixture_ctrl.h"


#include "io_map.h"
#include "adc_map.h"
#include "led_ctrl.h"
#include "camera_ctrl.h"


#include "application.h"
//#include "fixture_burn_in.h"
/*----------------------------------------------*/
#define MAC_NEAL		1
#define WINDOWNS_NEAL	2
#define LINUX_NEAL		1
#define CONNECT_ENV		WINDOWNS_NEAL
//#define CONNECT_ENV		WINDOWNS_NEAL
//#define CONNECT_ENV		MAC_NEAL


#if (1 == DEF_X530_USE_FLAG)
volatile int gEmcTest=0;
#endif


/*----------------------------------------------*/
//packet msg
static int make_packet(PACKET_t *packet,char *h,char *num,char *payload);
//packet and send msg
static int packet_and_send_msg(int fd,char *h,char *num,char *payload);

/*----------------handle-----------------------*/
//soft version
static int version_handle(int fd, char* buf);
//config.ini version
static int config_version_handle(int fd, char* buf);
//help
static int help_handle(int fd, char* buf);
//debug
static int debug_mode_handle(int fd, char* buf);

static int led_handle(int fd, char* buf);
static int current_meter_handle(int fd, char* buf);
static int volt_meter_handle(int fd, char* buf);
static int button_handle(int fd, char* buf);
static int light_handle(int fd, char* buf);

static int start_handle(int fd, char* buf);
static int end_handle(int fd, char* buf);
static int fail_handle(int fd, char* buf);


#if 0
//fixture locked start
static int fixture_locked_handle(int fd, char* buf);
#endif

/*---------------------------------------------------------------*/

Msg_hander_t client[]={
	{MSG_ConVersion, config_version_handle},
	{MSG_Version, version_handle},
	{MSG_Help, help_handle},
	{MSG_Debug, debug_mode_handle},
	{MSG_Led, led_handle},
	{MSG_VoltMeter, volt_meter_handle},
	{MSG_CurrentMeter, current_meter_handle},
	{MSG_Light, light_handle},
	{MSG_Button, button_handle},
	{MSG_Start, start_handle},
	{MSG_End, end_handle},
	{MSG_Fail, fail_handle}
};

//start download
static int start_handle(int fd, char* buf)
{
	int ret;

	gStartDownload = 1;
	
	ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_DONE);
	debug_printf("TCP Msg : start cmd done !\n");
	return ret;
}

static int end_handle(int fd, char* buf)
{
	int ret=0;
	gEndDownload = 1;
	
	ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_DONE);
	debug_printf("TCP Msg : end cmd done !\n");
	return ret;
}

static int fail_handle(int fd, char* buf)
{
	int ret=0;
	gFailDownload = 1;
	
	ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_DONE);
	debug_printf("TCP Msg : fail cmd done !\n");
	return ret;
}



static int light_handle(int fd, char* buf)
{
	int ret=0,res;
	int i;
	int arg[2]={0};
	char *p;

	unsigned int tmp;
	
	p = buf;
	for(i=0; i<2; ++i)
	{
		if(p == NULL)
		{
			ret = -1;
			break;
		}
		//去掉字符串前面段空格
		p = lskip(p);
		arg[i] = atoi(p);
		//找到下一个参数前空格位置
		p = strchr(p,' ');
		ret = 0;
	}

	if(ret < 0)
	{	//命令格式错误
		ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_ERROR);
	}
	else
	{
		printf("light number %d %d\n",arg[0],arg[1]);

		res = Light_DO_Set(arg[0], arg[1]);

		if(res >0)	
			ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_DONE);
		else
			ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_ERROR);
	}

	printf("TCP Msg : light %d %d done !\n",arg[0],arg[1]);
	return ret;

}

static int button_handle(int fd, char* buf)
{
	int ret;
	unsigned char gpio_value=0;
	unsigned char buffer[8];

	if(Button_Value_Get(&gpio_value) >0)
	{
		sprintf(buffer,"0x%x",gpio_value);
		ret = packet_and_send_msg(fd,REPLY_ACK,NULL,buffer);
	}
	else
	{
		ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_ERROR);
	}
	
	debug_printf("TCP Msg : button done !\n");
	return ret;

}

static int current_meter_handle(int fd, char* buf)
{
	int ret=0;
	int i;
	int arg[2]={0};
	char *p;

	unsigned int tmp;
	
	p = buf;
	for(i=0; i<2; ++i)
	{
		if(p == NULL)
		{
			ret = -1;
			break;
		}
		//去掉字符串前面段空格
		p = lskip(p);
		arg[i] = atoi(p);
		//找到下一个参数前空格位置
		p = strchr(p,' ');
		ret = 0;
	}

	if(ret < 0)
	{	//命令格式错误
		ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_ERROR);
	}
	else
	{
		printf("volt number %d %d\n",arg[0],arg[1]);

		if(arg[1] > 2000)
		{
			arg[1]=2000;
			//ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_ERROR);
			//return ret;

		}

		tmp = (4095 * arg[1])/2000;
		
		if(Current_Number_Set(arg[0],tmp) > 0)
			
			ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_DONE);
		else
			ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_ERROR);
	}

	printf("TCP Msg : current %d %d done !\n",arg[0],arg[1]);
	return ret;

}
static int volt_meter_handle(int fd, char* buf)
{
	int ret=0;
	int i;
	int arg[2]={0};
	char *p;

	unsigned int tmp;
	
	p = buf;
	for(i=0; i<2; ++i)
	{
		if(p == NULL)
		{
			ret = -1;
			break;
		}
		//去掉字符串前面段空格
		p = lskip(p);
		arg[i] = atoi(p);
		//找到下一个参数前空格位置
		p = strchr(p,' ');
		ret = 0;
	}

	if(ret < 0)
	{	//命令格式错误
		ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_ERROR);
	}
	else
	{
		printf("volt number %d %d\n",arg[0],arg[1]);

		if(arg[1] > 940)
		{
			arg[1]=940;
			//ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_ERROR);
			//return ret;

		}

		
		tmp = (4095 * arg[1]/47)/20;
		
		if(Volt_Number_Set(arg[0],tmp) > 0)
			ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_DONE);
		else
			ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_ERROR);
	}

	printf("TCP Msg : volt %d %d done !\n",arg[0],arg[1]);
	return ret;

}


static int led_handle(int fd, char* buf)
{
	int ret=0;
	int i;
	int arg[2]={0};
	char *p;
	
	p = buf;
	for(i=0; i<2; ++i)
	{
		if(p == NULL)
		{
			ret = -1;
			break;
		}
		//去掉字符串前面段空格
		p = lskip(p);
		arg[i] = atoi(p);
		//找到下一个参数前空格位置
		p = strchr(p,' ');
		ret = 0;
	}

	if(ret < 0)
	{	//命令格式错误
		ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_ERROR);
	}
	else
	{
		printf("get led %d %d\n",arg[0],arg[1]);
		if(arg[1]>9999)
			arg[1]=9999;
		Led_Number_Set(arg[0],arg[1]);
		ret = packet_and_send_msg(fd,REPLY_ACK,NULL,REPLY_DONE);			
	}

	printf("TCP Msg : led %d %d done !\n",arg[0],arg[1]);
	return ret;

}



//debug
static int debug_mode_handle(int fd, char* buf)
{
	int ret=0;
	char *p;

	p = buf;
	//去掉字符串前面段空格
	p = lskip(p);
	//去掉字符串后空格
	p = rstrip(p);
	if(MATCH_N(p,"ON"))
	{
		gDebugMode = 1;
		ret = packet_and_send_msg(fd,NULL,NULL,"Debug mode on");
	}
	else if(MATCH_N(p,"OFF"))
	{
		gDebugMode = 0;
		ret = packet_and_send_msg(fd,NULL,NULL,"Debug mode off");
	}
	else
	{
		ret = packet_and_send_msg(fd,NULL,NULL,"Debug command parameter error");
	}

	printf("TCP Msg : Debug done !\n");

	return ret;
}

//help
static int help_handle(int fd, char* buf)
{
	int ret,i;
	char mbuf[kPayloadSize];

	memset(mbuf,'\0',kPayloadSize);
	strcat(mbuf,client[0].cmd);
	for(i=1; i< (sizeof(client)/sizeof(client[0])); ++i)
	{
		strcat(mbuf,"; ");
		strcat(mbuf,client[i].cmd);
	}
	ret = packet_and_send_msg(fd,NULL,NULL,mbuf);
	printf("TCP Msg : help done !\n");
	return ret;
}

//soft version
static int version_handle(int fd, char* buf)
{
	int ret;
	ret = packet_and_send_msg(fd,REPLY_VER,NULL,gSoft_infor.version);
	debug_printf("TCP Msg : version? done !\n");
	return ret;
}

//config version
static int config_version_handle(int fd, char* buf)
{
	int ret;
	ret = packet_and_send_msg(fd,"Config.ini_",REPLY_VER,gSoft_infor.ini_version);
	debug_printf("TCP Msg : version? done !\n");
	return ret;
}
/*---------------------------------------------------------------*/
//packet analysis
int packet_analysis(int fd,char* buf)
{
	int i;
	int ret;
	char buf_cmd[kPayloadSize];

	memset(buf_cmd,'\0',kPayloadSize);
	memcpy(buf_cmd, buf,strlen(buf)+1);

	for(i=0; i< (sizeof(client)/sizeof(client[0])); ++i)
	{
		if(MATCH_N(client[i].cmd,buf_cmd))
		{
			return client[i].handle(fd,buf_cmd+strlen(client[i].cmd));
		}
	}
	
	ret = packet_and_send_msg(fd,"Warning : \"",buf,"\"  command isn\'t exsit !");
	printf("Warning : \"%s\"  command isn\'t exsit !\n",buf);
	return 0;
}


/*---------------------------------------------------------------*/
//packet msg
static int make_packet(PACKET_t *packet,char *h,char *num,char *payload)
{
	char *p;
	int len;
	
	len = 0;
	p = (char *) packet;
	if(h != NULL)
	{
		len = strlen(h);

		if(len > PacketSize)
			return -2;
		p = strncpy(p,h,len);
		p += len;
	}
	if(num != NULL)
	{
		len += strlen(num);
		if(len > PacketSize)
			return -2;
		strncpy(p,num,strlen(num));
		p += strlen(num);
	}

	if(payload != NULL)
	{
		len += strlen(payload);
		if(len > PacketSize)
			return -2;
		strncpy(p,payload,strlen(payload));
		p += strlen(payload);
	}
	
	#if CONNECT_ENV == WINDOWNS_NEAL
		len += 2;
		if(len > (PacketSize-1))
			return -2;
		*p++ = '\r';
		*p++ = '\n';
		*p = '\0';
	#elif CONNECT_ENV == LINUX_NEAL
		len += 1;
		if(len > (PacketSize-1))
			return -2;
		*p++ = '\n';
		*p = '\0';
	#elif CONNECT_ENV == MAC_NEAL
		len += 1;
		if(len > (PacketSize-1))
			return -2;
		*p++ = '\r';
		*p = '\0';
	#endif	
	return len;
	
}


//packet and send msg
static int packet_and_send_msg(int fd,char *h,char *num,char *payload)
{
	PACKET_t p;
	char *pr;
	int ret,len;

	pr = (char *)&p;
	ret = make_packet(&p,h,num,payload);
	if(ret < 0)
	{
		if(ret == -2)
			debug_printf("Error : msg is too big !\n");
		return -1;
	}

	len = send(fd,&p,ret,0);
	if(len != ret)
	{	
		debug_printf("Error : send data isn't send all\n");
		return -1;
	}

	debug_printf("Tcp server send data(%d) : %s",len,p.payload);
	return len;
}

/*---------------------------------------------------------------*/


//客户端处理函数入口
int do_server(int fd)
{

	char buf[1024];
	int len;
	int ret;

	len = recv(fd,buf,sizeof(buf),0);
	if(len <= 0)
	{

		return -1;
	}
	
	debug_printf("do servet !\n");
#if defined DEBUG
	for(ret=0; ret<len; ++ret)
	{
		if(buf[ret] == '\r')
		{
			printf("\\r");
		}
		else if(buf[ret] == '\n')
		{
			printf("\\n");
		}
		else
		{
			printf("%c",buf[ret]);
		}
	}
	printf("\n");
#endif

	buf[len]='\0';
	printf("recv(%d) : %s\n",len, buf);
	ret = packet_analysis(fd,buf);

	#if 0
	if(MATCH_N("debug",buf) || MATCH_N("debug on",buf)|| MATCH_N("debug ON",buf))
	{
		gDebugMode = 1;
		ret = packet_and_send_msg(fd,"Mode : ",NULL,"Entern the debug mode .\r\nAnd you can send \"debug off|OFF\" to close debug mode.");

	}
	else if(MATCH_N("debug OFF",buf) || MATCH_N("debug off",buf))
	{
		gDebugMode = 0;
		ret = packet_and_send_msg(fd,"Mode : ",NULL,"Exit the debug mode .\r\nAnd you can send \"debug or debug on|ON\" to open debug mode.");

	}
	else
	{
		ret = packet_analysis(fd,buf);
	}

	#endif
	return 0;
}
/*------------------------end file--------------------------*/

