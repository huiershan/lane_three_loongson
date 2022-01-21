
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "can_config.h"
#include "can_bus.h"
#include "debug.h"

#if defined CAN_BUS_DEBUG_EN_FRAME
#include <sys/time.h>
#endif

/*------------end include----------------------*/
#if defined CAN_BUS_DEBUG_EN
static void print_can_frame(char * head_name,long count,struct  can_frame frame);
#endif
static can_frame_t *malloc_can_frame(void);
//open can socket
static int open_can_socket(char *can_name);
//insert can recive massage frame to massage recive list
static int insert_can_rcv_msg_list(CAN_t *can, struct can_frame frame);
//can recive msg thread
void *thread_can_frame_rcv(void *arg);
/*---------------------------------------------*/
void print_can_frame_t(char * head_name,long count, can_frame_t frame)
{
	char buf[218]={'\0'};
	int ncnt=0;
	int i;
	
	ncnt = sprintf(buf,"%s(%ld): <%03x> [%d]",head_name,count,frame.id,frame.dlc);
	for(i=0; i<frame.dlc; i++)
		ncnt +=sprintf(buf+ncnt," [%02x]",frame.data[i]);
	printf("%s\n",buf);
}

#if defined CAN_BUS_DEBUG_EN
static void print_can_frame(char * head_name,long count,struct  can_frame frame)
{
	int i;
	int ret=0;
	char buf[255]={'\0'};

		if(frame.can_id & CAN_EFF_FLAG)
			ret = snprintf(buf,sizeof(buf),"<0x%08x> ",frame.can_id & CAN_EFF_MASK);
		else
			ret = snprintf(buf,sizeof(buf),"<0x%03x> ",frame.can_id & CAN_SFF_MASK);

		ret +=snprintf(buf+ret,sizeof(buf)-ret, "[%d] ",frame.can_dlc);

		for(i=0; i< frame.can_dlc; i++)
		{
			ret += snprintf(buf+ret,sizeof(buf)-ret,"%02x ",frame.data[i]);
			if(i==7)
			{
				break;
			}
		}
		printf("%s(%ld): %s\n",head_name,count,buf);
}
#endif

/*--------------------------------------------*/
/*
 @ description: malloc a can frame
 @ return:		frame memory pointer 
 				NULL: malloc fail
*/
/*--------------------------------------------*/
static can_frame_t *malloc_can_frame(void)
{
	can_frame_t *p;
	p = malloc(sizeof(can_frame_t));
	if(p == NULL)
	{
		return p;
	}

	p->id = 0;
	p->dlc = 0;
	memset(p->data,0,sizeof(p->data));
	p->next = NULL;
	p->pre = NULL;
	
	return p;
}


//free can
static void free_can(CAN_t **can)
{
	can_frame_t *tmp=NULL;
	can_frame_t *list;

	CAN_t *can_tmp=NULL;
	can_tmp = *can;
	list = can_tmp->head;

	while(list->next != NULL)
	{
		tmp = list->next;
		list->next = tmp->next;
		free(tmp);
	}
	free(can_tmp);
	*can = NULL;
}


/*--------------------------------------------*/
/*
 @ description: write can frame
 @ input para:  
 				can: can type
				msg: can frame massage
 @ return:		write to can TR data count(byte)
 				<0: fail
*/
/*--------------------------------------------*/
int write_can_frame(CAN_t *can, can_frame_t msg)
{
	struct can_frame frame;
	int i;
	int ret=1;

	if(can->rcv_flag == 0)
	{
		print_error("%s recive thread isn't running\n",can->name);
		return -1;
	}
	
	if((msg.type&0x01) == 0x01)
	{
		frame.can_id = (msg.id&CAN_EFF_MASK)|CAN_EFF_FLAG;	//extend frame
	}
	else
		frame.can_id = msg.id&CAN_SFF_MASK; //standar frame
		
	if((msg.type&0x02) == 0x02)
	{
		frame.can_id |=  CAN_RTR_FLAG;
	}

	frame.can_dlc	= msg.dlc;
	for(i=0; i<frame.can_dlc; i++)
	{
		frame.data[i] = msg.data[i];
		if(i==7)
			break;
	}

	frame.can_id = msg.id;


#if defined CAN_BUS_DEBUG_EN_SEND
	print_can_frame("send_frame",can->snd_frame_cnt,frame);

#endif
	ret = write(can->can_fd, &frame,sizeof(frame));
	if(ret == -1)
	{
		print_error("can write fail\n");
		//wait buffer free
		usleep(200*1000);
		ret = -2;
	}
	else
	{
		can->snd_frame_cnt++;
	}
	
	return ret;
}


/*--------------------------------------------*/
/*
 @ description: read can frame from recive msg list
 @ input para:  
 				can: can type
				msg: can frame out
 @ return:		data count
 				<0: fail
*/
/*--------------------------------------------*/
int read_can_frame(CAN_t *can,can_frame_t *msg)
{
	can_frame_t *frame;
	int i;
#if 1
	pthread_mutex_lock(&can->rcv_list_mutex);
#else
	sem_wait(&can->rcv_sem);
#endif
	if(can->head->next == NULL)
	{
		//can no massage
		print_infor("can frame list is not have message\n");
#if 1
	pthread_mutex_unlock(&can->rcv_list_mutex);
#else
		sem_post(&can->rcv_sem);
#endif
		return -1;
	}

	frame = can->head->next;
	can->head->next = frame->next;
	can->frame_cnt -= 1;

	msg->id = frame->id;
	msg->dlc = frame->dlc;
	for(i=0; i<(msg->dlc); i++)
	{
		msg->data[i] = frame->data[i];
		if(i==7)
		{
			break;
		}
	}
	msg->next = NULL;


#if defined CAN_BUS_DEBUG_EN_MSG
	print_can_frame_t("recive_out_frame",can->rcv_frame_cnt,*msg);
#endif
	free(frame);
#if 1
	pthread_mutex_unlock(&can->rcv_list_mutex);
#else
	sem_post(&can->rcv_sem);
#endif
	return msg->dlc;
}

//insert can recive massage frame to massage recive list
static int insert_can_rcv_msg_list(CAN_t *can, struct can_frame frame)
{
	can_frame_t *msg, *p;
	int i;

	//malloc a node
	msg = malloc_can_frame();
	if(msg == NULL)
	{
		print_error("can msg malloc fail\n");
		return -1;
	}

	//standar or extend frame
	msg->type = 0;
	if(frame.can_id & CAN_EFF_FLAG)
	{
		msg->id = frame.can_id&CAN_EFF_MASK;
		msg->type |=0x01;
	}
	else
	{
		msg->id = frame.can_id&CAN_SFF_MASK;
		msg->type &=(~0x01);
	}

	if(frame.can_id & CAN_RTR_FLAG)
	{
		msg->type |=0x20;
	}

	msg->dlc = frame.can_dlc;
	for(i=0;i<frame.can_dlc; i++)
	{
		msg->data[i] = frame.data[i];
		if(i==7)
		{
			break;
		}
	}
	msg->next = NULL;
	//find recive massage list tail
#if 1
	pthread_mutex_lock(&can->rcv_list_mutex);
#else
	sem_wait(&can->rcv_sem);
#endif
	p = can->head;
	while(p->next != NULL)
	{
		p = p->next;
	}
	p->next = msg;

	can->frame_cnt += 1;
#if 1
	pthread_mutex_unlock(&can->rcv_list_mutex);
#else
	sem_post(&can->rcv_sem);
#endif
#if defined CAN_BUS_DEBUG_EN_MSG
	print_can_frame_t("recive_in_frame",can->rcv_frame_cnt,*msg);
	printf("sem post frame sem\n");
#endif
	sem_post(&can->frame_sem);
	return frame.can_dlc;
}

//can recive msg thread
void *thread_can_frame_rcv(void *arg)
{
	int fd;
	CAN_t *can;
	struct can_frame frame;
	int nbytes;
	int ret;
#if defined CAN_BUS_DEBUG_EN_FRAME
	struct timeval start,end;
#endif
	printf("can massge recive thread is running\n");

	can = (CAN_t *)arg;
	fd = can->can_fd;

	can->rcv_flag = 1;

	while(can->rcv_flag)
	{
		if((nbytes = read(fd,&frame,sizeof(struct can_frame))) < 0)
		{
			print_error("can read fail\n");
			continue;
		}
		can->rcv_frame_cnt++;	//test use
#if defined CAN_BUS_DEBUG_EN_FRAME
		print_can_frame("recive_frame",can->rcv_frame_cnt,frame);
		gettimeofday(&start,NULL);
#endif
		ret = insert_can_rcv_msg_list(can,frame);
		if(ret<0)
			printf("insert to frame list fail\n");
#if defined CAN_BUS_DEBUG_EN_FRAME
		else
			printf("insert to frame list success\n");
		gettimeofday(&end,NULL);
		printf("insert a can message time use %ld s:%ld us\n",(long)(end.tv_sec-start.tv_sec),end.tv_usec-start.tv_usec);
#endif
	}

	can->rcv_flag = 0;
	printf("can massge recive thread is away\n");
	return NULL;
}


//open can
CAN_t *open_can(char *can_name)
{
	int ret=0;
	CAN_t *pcan=NULL;
	pthread_t rcv_id;

	//CAN molloc
	pcan = (CAN_t *)malloc(sizeof(CAN_t));
	if(pcan == NULL)
	{
		print_error("CAN molloc fail\n");
		return NULL;
	}

	//CAN para init
	strncpy(pcan->name,can_name,sizeof(pcan->name));
	pcan->head = malloc_can_frame();
	if(pcan->head == NULL)
	{
		printf("can bus message head malloc fail\n");
		free(pcan->head);
		free(pcan);
		return NULL;
	}
	pcan->rcv_flag = 0;
	pcan->head->next = NULL;
	pcan->frame_cnt = 0;

	pcan->rcv_frame_cnt = 0;
	pcan->snd_frame_cnt = 0;
	//sem init
#if 0
	ret = sem_init(&pcan->bus_sem,0,1);
	if(ret != 0)
	{
		print_error("CAN bus sem init fail\n");
		goto open_can_out;
	}
#endif
#if 1
	pthread_mutex_init(&pcan->rcv_list_mutex,NULL);
#else
	ret = sem_init(&pcan->rcv_sem,0,1);
	if(ret != 0)
	{
		print_error("CAN msg sem init fail\n");
		goto open_can_out;
	}
#endif
	ret = sem_init(&pcan->frame_sem,0,0);
	if(ret != 0)
	{
		print_error("CAN msg sem init fail\n");
		goto open_can_out;
	}

	pcan->can_fd = open_can_socket(can_name);
	if(pcan->can_fd < 0)
	{
		print_error("CAN socket open fail\n");
		goto open_can_out;
	}

	//open can massage frame recive thread
	if(pthread_create(&rcv_id,NULL,thread_can_frame_rcv,pcan) < 0)
	{
		print_error("Create %s recive thread fail\n",pcan->name);
		goto open_can_out;
	}

	usleep(200*1000);
	printf("%s: open success\n",can_name);
	return pcan;
//error out
open_can_out:
	pthread_mutex_destroy(&pcan->rcv_list_mutex);
	free(pcan->head);
	free(pcan);
	return NULL;
}

//close can
void close_can(CAN_t *can)
{
	//can recive frame thread free
	can->rcv_flag = 0;
	//close can socket
	close(can->can_fd);
	sleep(1);
	pthread_mutex_destroy(&can->rcv_list_mutex);
	//can free
	free_can(&can);
}

/*--------------------------------------------*/
/*
 @ description: open can socket
 @ input para:  can bus name
 @ return:		socket file decription
 				<0: fail
*/
/*--------------------------------------------*/
static int open_can_socket(char *can_name)
{
	struct ifreq ifr;
	struct sockaddr_can addr;
	int fd=0;

	if((fd = socket(PF_CAN,SOCK_RAW,CAN_RAW)) < 0)
	{
		print_error("CAN socket fail\n");
		return -1;
	}

	addr.can_family = PF_CAN;
	strncpy(ifr.ifr_name,can_name,sizeof(ifr.ifr_name));
	if(ioctl(fd, SIOCGIFINDEX,&ifr))
	{
		print_error("can ioctrl fail\n");
		return -1;
	}

	addr.can_ifindex = ifr.ifr_ifindex;

	if(bind(fd,(struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		print_error("can bind fail\n");
		return -1;
	}

//	signal(SIGTERM, sigterm);
//	signal(SIGHUP, sigterm);

	return fd;
}


/*------------------------------end file-----------------------------------*/

