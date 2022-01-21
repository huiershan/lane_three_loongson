
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include "can_bus.h"
#include "can_msg.h"
#include "debug.h"


//message write wait result out time
#define CAN_WAIT_ACK_TIMEDOUT		2000		//ms
//message valid time
#define CAN_RCV_MSG_TIMEDOUT		4000		//ms

//find message ack in can recive message list
static int can_find_ack_msg(CAN_Use_t *can, CAN_Msg_t *msg);



//printf can msg data
int CAN_MsgPrint(CAN_Msg_t msg)
{
	int i;
	char buf[255]={'\0'};
	int ncnt=0;

	ncnt = sprintf(buf,"CAN MSG: dst(%02x) src(%02x) type(%02x) cmd(%02x)",
				msg.dst,msg.src,msg.type,msg.cmd);
	for(i=0; i<4; ++i)
	{
		ncnt += sprintf(buf+ncnt," data[%d]=%02x",i,msg.data[i]);
	}

	printf("%s\n",buf);
	return 0;
}
//打包数据包
int can_pack_msg(CAN_Msg_t *p,...)
{
	va_list ap;
	int ret = -1;
	int tmp;
	int port;
	int status;
	int i;
	unsigned int state;

	va_start(ap,p);
	memset(p->data,0,4);

	if(CMD_TYPE_CHECK == p->type)
	{
		switch(p->cmd)
		{
		case CMD_CHECK_ID:
			{
				ret = 0;
				break;
			}
		case CMD_SELF_CHECK_ID:
			{
				ret = 0;
				break;
			}
		default:
			{
				print_error("CAN type check No this cmd id %d\n",p->cmd);
				ret = -1;
				break;
			}
		}
	}
	else if(CMD_TYPE_CTRL == p->type)
	{
		switch(p->cmd)
		{
		case CMD_DI_ID:
			{
				port = va_arg(ap,int);
				p->data[0] = port&0xff;
				ret = 0;
				break;
			}
		case CMD_DO_ID:
			{
				port = va_arg(ap,int);
				status = va_arg(ap,int);
				p->data[0] = port&0xff;
				p->data[1] = status?0xff:0;
				ret = 0;
				break;
			}
		case CMD_AD_ID:
			{
				port = va_arg(ap,int);
				p->data[0] = port&0xff;
				ret = 0;
				break;
			}
		case CMD_DO_ALL_ID:
			{
				state = va_arg(ap,unsigned int);
				for(i=3; i>=0; i--)
				{
					p->data[i] = state&0xff;
					state = state>>8;
				}
				ret = 0;
				break;
			}
		case CMD_DI_ALL_ID:
			{
				ret = 0;
				break;
			}
		default:
			{
				print_error("CAN type ctrl No this cmd id %d\n",p->cmd);
				ret = -1;
				break;
			}
		
		}
	
	}
	else if(CMD_TYPE_CMP == p->type)
	{
		switch(p->cmd)
		{
		case CMD_STEPS_SET_ID:
			{
				tmp = va_arg(ap,int);
				p->data[0] = tmp&0xff;
				ret = 0;
				break;
			}
		case CMD_STEP_DO_SET_ID:
			{
				for(i=0; i<4; i++)
				{
					tmp = va_arg(ap,int);
					p->data[i] = tmp&0xff;
				}
				ret = 0;
				break;
			}
		case CMD_STEP_TIME_SET_ID:
			{
				tmp = va_arg(ap,int);
				p->data[0] = tmp&0xff;
				tmp = va_arg(ap,int);
				p->data[1] = tmp&0xff;
				tmp = tmp>>4;
				p->data[2] = tmp&0xf0;
				tmp = va_arg(ap,int);
				p->data[3] = tmp&0xff;
				tmp = tmp>>8;
				p->data[2] |= (tmp&0x0f);
				ret = 0;
				break;
			}
		case CMD_LIST_DO_ID:
			{
				for(i=0; i<3; i++)
				{
					tmp = va_arg(ap,int);
					p->data[i] = tmp&0xff;
				}
				if(p->data[i]>0)
				{
					p->data[i]=0xff;
				}
				ret = 0;
				break;
			}
		case CMD_LIST_STATE_ID:
			{
				ret = 0;
				break;
			}
		default:
			{
				print_error("CAN type cmp No this cmd id %d\n",p->cmd);
				ret = -1;
				break;
			}
		
		}
	
	}
	else
	{
		print_error("CAN type No this type id %d\n",p->type);
		ret = -1;
	}

	va_end(ap);

	return ret;
}


//解数据包
int can_msg_parse(CAN_Msg_t *msg,...)
{
	va_list ap;
	int i;
	int ret = -1;

	int *type_p;
	int *tmp_p;
	int *port_p;
	int *status_p;
	unsigned int *state_p;
	float *f_v_p;
	char f_buf[12];
	uint32_t rel;

	va_start(ap, msg);

	if(CMD_TYPE_CHECK == msg->type)
	{
		switch(msg->cmd)
		{
		case CMD_CHECK_ACK_ID:
			{
				type_p = va_arg(ap,int*);
				*type_p = msg->data[0];
				port_p = va_arg(ap,int*);
				*port_p = msg->data[1];
				status_p = va_arg(ap,int*);
				*status_p = msg->data[2];
				ret = 0;
				break;
			}
		case CMD_SELF_CHECK_ACK_ID:
			{
				type_p = va_arg(ap, int*);
				*type_p = msg->data[0];
				ret = 0;
				break;
			}
		default:
			{		
				print_error("No this type check cmd %d\n",msg->cmd);
				#ifdef CAN_CTRL_DEBUG
				CAN_MsgPrint(*msg);	
				#endif
				ret = -1;
				break;
			}
		
		}
	}
	else if(CMD_TYPE_CTRL == msg->type)
	{
		switch(msg->cmd)
		{
		case CMD_DI_ACK_ID:
			{
				port_p = va_arg(ap,int*);
				status_p = va_arg(ap,int*);
				
				*port_p = msg->data[0];
				*status_p = msg->data[1]?1:0;
				ret = 0;
				break;
			}
		case CMD_DO_ACK_ID:
			{
				ret = 0;
				break;
			}
		case CMD_AD_ACK_ID:
			{
				port_p = va_arg(ap,int*);
				f_v_p = va_arg(ap,float*);
    
				*port_p = msg->data[0];
				if (msg->data[1] > 127)
				{
					rel = 0xffffff00 | msg->data[1];
				}
				else 
				{
					rel = msg->data[1];
				}
				sprintf(f_buf,"%d.%04d",rel,(msg->data[2]<<8) | msg->data[3]);
				*f_v_p = atof(f_buf);
				ret = 0;
				break;
			}
		case CMD_DO_ALL_ACK_ID:
			{
				ret = 0;
				break;
			}
		case CMD_DI_ALL_ACK_ID:
			{
				state_p = va_arg(ap,unsigned int*);
				rel = 0;
				for(i=0; i<4; i++)
				{
					rel = rel<<8;
					rel |= msg->data[i];
				}
				*state_p = rel;
				ret = 0;
				break;
			}
		default:
			{		
				print_error("No this type ctrl cmd %d\n",msg->cmd);
				#ifdef CAN_CTRL_DEBUG
				CAN_MsgPrint(*msg);	
				#endif
				ret = -1;
				break;
			}
		}
	}
	else if(CMD_TYPE_CMP == msg->type)
	{
		switch(msg->cmd)
		{
		case CMD_STEPS_SET_ACK_ID:
		case CMD_STEP_DO_SET_ACK_ID:
		case CMD_STEP_TIME_SET_ACK_ID:
		case CMD_LIST_DO_ACK_ID:
			{
				ret = 0;
				break;
			}
		case CMD_LIST_STATE_ACK_ID:
			{
				for(i=0; i<2; i++)
				{
					tmp_p = va_arg(ap,int *);
					*tmp_p = msg->data[i];
				}
				ret = 0;
				break;
			}
		default:
			{		
				print_error("No this type cmp cmd %d\n",msg->cmd);
				#ifdef CAN_CMP_DEBUG
				CAN_MsgPrint(*msg);	
				#endif
				ret = -1;
				break;
			}
		}
	}
	else
	{
		print_error("CAN type No this type id %d\n",msg->type);
		#ifdef CAN_CTRL_DEBUG
		CAN_MsgPrint(*msg);	
		#endif
		ret = -1;
	}

	va_end(ap);
	return ret;

}

//can message to can frame
static void can_msg_to_frame(can_frame_t *frame, CAN_Msg_t msg, unsigned int id,int dlc)
{
	int i;

	memset(frame,'0',sizeof(can_frame_t));
	frame->id = id;
	frame->dlc = dlc;
	for(i=0; i<dlc; i++)
	{
		frame->data[i] = msg.data_m[i];
	}
}

//can frame to can msg
static int can_frame_to_msg(CAN_Msg_t *msg, can_frame_t frame)
{
	int i;
	memset(msg,'0',sizeof(CAN_Msg_t));
	for(i=0; i<frame.dlc; i++)
	{
		msg->data_m[i] = frame.data[i];
	}

	return frame.dlc;
}



//can use print message
void can_print_msg(char *head_name, long count, CAN_Msg_t msg)
{
	int i;
	int ncnt=0;
	char buf[128]={'\0'};

	for(i=0;i<sizeof(CAN_Msg_t);i++)
	{
		ncnt += sprintf(buf+ncnt," [%02x]",msg.data_m[i]);	
	}

	printf("%s(%ld):%s\n",head_name,count,buf);
}

static CAN_Msg_node_t *malloc_can_msg_node(void)
{
	CAN_Msg_node_t *p;
	p = malloc(sizeof(CAN_Msg_node_t));
	if(p == NULL)
	{
		return p;
	}

	p->next= NULL;
	
	return p;
}


static long time_subtrack(struct timeval *diff,struct timeval x,struct timeval y)
{
	long tim;
	struct timeval rel;
	if(x.tv_sec > y.tv_sec)
	{
		return -1;
	}

	if((x.tv_sec == y.tv_sec)&&(x.tv_usec>y.tv_usec))
	{
		return -1;
	}

	
	if(y.tv_usec >= x.tv_usec)
	{
		rel.tv_usec = y.tv_usec - x.tv_usec;
		rel.tv_sec = y.tv_sec - x.tv_sec;
	}
	else
	{
		rel.tv_usec = 1*1000*1000 + x.tv_usec - y.tv_usec;
		rel.tv_sec = y.tv_sec - x.tv_sec-1;
	}

	tim = (rel.tv_usec/1000);
	if((rel.tv_usec%1000)>500)
	{
		tim +=1;
	}

	tim += rel.tv_sec*1000;
	if(diff != NULL)
	{
		diff->tv_sec = rel.tv_sec;
		diff->tv_usec = rel.tv_usec;
	}

	return tim;
}



//can write and read message
int can_write_and_read_msg(CAN_Use_t *can, CAN_Msg_t *msg)
{
	can_frame_t	frame;
	//CAN_Msg_t msg_tmp;
	struct timespec ts;
	struct timeval tt;
	int ret=0;
	int sem_row=0;
	int sem_col=0;

#if defined CAN_USE_DEBUG_EN_MSG
	static long i=0;
#endif
	
	struct timeval start,stop;
	long diff_time=0;
	gettimeofday(&start,NULL);

	pthread_mutex_lock(&can->bus_mutex);
	//msg to frame
	can_msg_to_frame(&frame,*msg,0,8);
	//send
	ret = write_can_frame(can->can_bus,frame);//have blocking
	if(ret < 0)
	{
		print_error("can write msg fail\n");
		pthread_mutex_unlock(&can->bus_mutex);
		return ret;
	}
	
	//wait ack
	sem_row = msg->type;
	sem_col = msg->cmd>>1;
	pthread_mutex_lock(&can->ack[sem_row][sem_col].mutex);
	can->ack[sem_row][sem_col].cnt += 1;
	pthread_mutex_unlock(&can->ack[sem_row][sem_col].mutex);

#if defined CAN_USE_DEBUG_EN
	printf("wait can row(%d),col(%d)result.....\n",sem_row,sem_col);
#endif
	gettimeofday(&tt,NULL);
	ts.tv_sec = tt.tv_sec;
	ts.tv_nsec = tt.tv_usec*1000 + (CAN_WAIT_ACK_TIMEDOUT%1000)*1000*1000;
	ts.tv_sec += ts.tv_nsec/(1000*1000*1000)+CAN_WAIT_ACK_TIMEDOUT/1000;
	ts.tv_nsec %= 1000*1000*1000;

while(1)
{
#if 1
	while((ret=sem_timedwait(&can->ack[sem_row][sem_col].sem,&ts)==-1)
		&&errno == EINTR)
	{
		printf("sem timedwait can ack sem ENITR...\n");
		continue;
	}

	if(ret == -1)
	{
		if(errno == ETIMEDOUT)
		{
			print_warn("can message wait read ack time out\n");
		}
		else
		{
			printf("%s\n",strerror(errno));
			print_error("can message wait read ack error(%d)\n",errno);
		}
	
	}
#else
	sem_wait(&can->ack[sem_row][sem_col].sem);
	ret = 1;
#endif

	gettimeofday(&stop,NULL);
	diff_time = time_subtrack(NULL,start,stop);
	if((ret >= 0)&&(diff_time == 0))
	{
		pthread_mutex_lock(&can->ack[sem_row][sem_col].mutex);
		printf("can wait ack cnt(%d)\n",can->ack[sem_row][sem_col].cnt);
		can->ack[sem_row][sem_col].cnt -= 1;
		if(can->ack[sem_row][sem_col].cnt == 0)
		{
			ret =-3;
			pthread_mutex_unlock(&can->ack[sem_row][sem_col].mutex);
			break;
		}
		pthread_mutex_unlock(&can->ack[sem_row][sem_col].mutex);
		printf("wait continue....\n");
		continue;
	}
	else
	{
		break;
	}
}
#if defined CAN_USE_DEBUG_EN
	printf("diff ret time = %ld ms\n",time_subtrack(NULL,start,stop));
#endif
	//find massage ack in can recive massage list
	ret = can_find_ack_msg(can,msg);
	if(ret < 0)
	{
		print_warn("can write msg can't find ack %d\n",ret);
		pthread_mutex_unlock(&can->bus_mutex);
		return -1;
	}

#if defined CAN_USE_DEBUG_EN_MSG
	i++;
	can_print_msg("recive_out",i,*msg);
#endif
	pthread_mutex_unlock(&can->bus_mutex);
	return ret;
}

//find message ack in can recive message list
static int can_find_ack_msg(CAN_Use_t *can, CAN_Msg_t *msg)
{
	CAN_Msg_node_t *msg_rcv[255];
	CAN_Msg_node_t *node,*ptmp;
	int i;
	int msg_cnt;

	pthread_mutex_lock(&can->rcv_list_mutex);
	ptmp = can->head;
	msg_cnt = 0;
	while(ptmp->next != NULL)
	{
		node = ptmp->next;
		msg_rcv[msg_cnt] = node;
		//pan duan
		if(msg_rcv[msg_cnt]->msg.dst == msg->src)
		{
			if(msg_rcv[msg_cnt]->msg.src == msg->dst)
			{
				if(msg_rcv[msg_cnt]->msg.type == msg->type)
				{
					if(msg_rcv[msg_cnt]->msg.cmd == (msg->cmd+1))
					{
						ptmp->next = node->next;
						msg_cnt++;
						if(msg_cnt>=sizeof(msg_rcv)/sizeof(msg_rcv[0]))
						{
							printf("some message isn't deal(%d)\n",msg_cnt);
							break;
						}
					}
				}
			}
		}
		else
		{
			ptmp->next = node->next;
			can_print_msg("msg not",1,node->msg);
			free(node);
		}
	}

	memset(msg,0,sizeof(CAN_Msg_t));
	//deal get messsage
	if(msg_cnt > 0)
	{
		node = msg_rcv[msg_cnt-1];
		for(i=msg_cnt-2; i>=0; i--)
		{
			printf("more message is deal(%d)\n",msg_cnt);
			free(msg_rcv[i]);
		}
		can->msg_cnt -= 1;
		*msg = node->msg;
		free(node);
	}
	else
	{
		print_warn("no message ack\n");
	}

#if defined CAN_USE_DEBUG_EN_OUT
	can_print_msg("recive_out_msg_list",0,*msg);
#endif
	pthread_mutex_unlock(&can->rcv_list_mutex);

	return (msg_cnt>0)?msg_cnt:-1;

}

//insert can recive message frame to message recive list
static int can_insert_message_node(CAN_Use_t *can, CAN_Msg_t msg)
{
	CAN_Msg_node_t *node, *p;
	struct timeval val;

	//malloc a node
	node = malloc_can_msg_node();
	if(node == NULL)
	{
		print_error("malloc can messgae node malloc fail\n");
		return -1;
	}
	
	node->msg = msg;
	node->next = NULL;
	gettimeofday(&val,NULL);
	node->start_time = val;

	//find recive massage list tail
	pthread_mutex_lock(&can->rcv_list_mutex);
	p = can->head;
	while(p->next != NULL)
	{
		p = p->next;
	}
	p->next = node;

	can->msg_cnt += 1;
#if defined CAN_USE_DEBUG_EN_IN
	 can_print_msg("recive_in_msg_list",can->msg_cnt, node->msg);
#endif
	pthread_mutex_unlock(&can->rcv_list_mutex);
	return 1;
}


//can message get thread
void *can_message_thread(void * arg)
{
	CAN_Use_t *can;
	can_frame_t frame;
	CAN_Msg_t msg;
	int row=0;
	int col=0;
	int ret;
	can = (CAN_Use_t *)arg;
	
	print_infor("can message thread running\n");
	while(can->can_bus->rcv_flag)
	{
		//delete time out message
		sem_wait(&can->can_bus->frame_sem);
		ret = read_can_frame(can->can_bus, &frame);
		if(ret < 0 )
		{
			printf("no can frame in frame list\n");
			continue;
		}
		//can frame to can msg
		ret = can_frame_to_msg(&msg,frame);
		if(ret < 8)
		{
			print_warn("can recive message missing dlc(%d)\n",ret);
			continue;
		}

		ret = can_insert_message_node(can, msg);
		if(ret >= 0)
		{
			row = msg.type;
			col = (msg.cmd-1)>>1;
			pthread_mutex_lock(&can->ack[row][col].mutex);
			can->ack[row][col].cnt--;
			if(can->ack[row][col].cnt < 0)
			{
				print_warn("can write wait ack[%d][%d].cnt =%d\n",row,col,can->ack[row][col].cnt);
			}
			pthread_mutex_unlock(&can->ack[row][col].mutex);
			sem_post(&can->ack[row][col].sem);
		}
		else
		{
			print_warn("can insert node to massage list fail(%d)\n",ret);
		}

	}

	printf("can use recive message thread out\n");
	return NULL;
}

//can messgae inital
CAN_Use_t *can_message_init(char *name)
{
	int i,j;
	CAN_t *can_bus;
	CAN_Use_t *can;

	pthread_t rcv_deal_id;

	//CAN_Use_t init
	can = malloc(sizeof(CAN_Use_t));
	if(can == NULL)
	{
		print_error("can use malloc fail\n");
		return NULL;
	}

	can->head = malloc_can_msg_node();
	if(can->head == NULL)
	{
		print_error("can use message head malloc fail\n");
		free(can);
		return NULL;
	}
	can->head->next = NULL;
	can->msg_timedout_cnt = 0;
	can->msg_cnt = 0;
	for(i=0; i<CAN_MSG_TYPE_MAX; i++)
	{
		for(j=0; j< CAN_MSG_CMD_MAX; j++)
		{
			sem_init(&can->ack[i][j].sem,0,0);
			can->ack[i][j].cnt = 0;
			pthread_mutex_init(&can->ack[i][j].mutex,NULL);
		}
	}
	pthread_mutex_init(&can->rcv_list_mutex,NULL);
	pthread_mutex_init(&can->bus_mutex,NULL);
	//open can
	can_bus = open_can(name);
	if(can_bus == NULL)
	{
		print_error("can bus open fail\n");
		goto can_msg_init_out;
	}
	can->can_bus = can_bus;
	//open can message get thread
	if(pthread_create(&rcv_deal_id,NULL,can_message_thread,can) < 0)
	{
		print_error("Create %s can message get thread fail\n",name);
		goto can_msg_init_out;
	}

	usleep(500*1000);

	return can;
can_msg_init_out:
	for(i=0; i<CAN_MSG_TYPE_MAX; i++)
	{
		for(j=0; j< CAN_MSG_CMD_MAX; j++)
		{
			pthread_mutex_destroy(&can->ack[i][j].mutex);
		}
	}
	pthread_mutex_destroy(&can->rcv_list_mutex);
	pthread_mutex_destroy(&can->bus_mutex);
	free(can->head);
	free(can);
	return NULL;
}
/*------------------------------end file-----------------------------------*/

