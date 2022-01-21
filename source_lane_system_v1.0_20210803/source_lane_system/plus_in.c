
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>


#include "sysutil_d.h"

#include "plus_in.h"
#include "serial.h"
#include "tcp_server.h"

sem_t gPlusRxState_sem;
Queue* pPlusRxQueue = NULL;
Plus_t plus;

#define CONFIG_MAX_PLUS_RX_QUEUE_SIZE  1024

int Plus_Init()
{
	int ret;

	ret = sem_init(&gPlusRxState_sem,0,1);
	if(ret < 0)
	{
		printf("Create plus rx sem fail!");	
		return -1;
	}

	pPlusRxQueue = QueueCreate(CONFIG_MAX_PLUS_RX_QUEUE_SIZE);	
	if(pPlusRxQueue == NULL)	
	{	
		printf("Create plus rx queue fail!");		
		return -1;	
	}
	return 1;
}



void Plus_Receive(void)
{
	unsigned char buf[8];
	
	int i;
	
	while(1)
	{
	
		memset(&plus, 0, sizeof(Plus_t));
		memset(buf, 0, sizeof(buf));
		
		read(gPlus_rs485_fd, buf, 7);
		//printf("buf[0]=0x%x, buf[1]=0x%x\n",buf[0],buf[1]);

		if(buf[0] == 0x68 && buf[1] == 0x04 && buf[6]==0xaa)
		{
			plus.head=0x68;
			plus.type=0x01;
			plus.len=0x04;
			for(i=0; i<4;i++)
			{
				plus.data[i]=buf[i+2];
				
				printf("plus.data[%d]=0x%x\n",i,buf[i+2]);
			}
			QueuePush(pPlusRxQueue, &plus);
			sem_post(&gPlusRxState_sem);
		}		
		msleep_m(10);
	}
	
}



int Plus_Upload(void)
{
	int len;
	int ret;

	while(1)
	{

		printf("wait plus rx sem!\n");	
		sem_wait(&gPlusRxState_sem);
		if(QueueJudgeEmpty(pPlusRxQueue) != true)		
		{
			Plus_t* pPlusData = QueuePop(pPlusRxQueue);			
			if(pPlusData == NULL)			
			{				
				printf("Plus rx queue take a mistake!\n");				
				continue;			
			}
			
			if(gSocketUploadfd > 0)
			{
				printf("send plus upload data\n");
				ret = sizeof(Plus_t);
				len = send(gSocketUploadfd,pPlusData,ret,0);
				printf("len=%d\n",len);
			      if(len != ret)
			     {	
				      printf("Error : send plus data isn't send all\n");
				     return -1;
				}
			}
			
		}
	}
	return 0;

}

/*-----------------------------------------------------*/
//end file

