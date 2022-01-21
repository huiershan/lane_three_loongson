
#ifndef _CAN_MSG_H
#define _CAN_MSG_H

#include <stdint.h>
#include <semaphore.h>
#include <pthread.h>
#include "can_bus.h"

//#define CAN_CTRL_DEBUG
#ifdef CAN_CTRL_DEBUG
	#define CAN_debug_print(fmt,args...)	printf(fmt,##args)
#else
	#define CAN_debug_print(fmt,args...)	do{}while(0)
#endif

//#define CAN_USE_DEBUG_EN_MSG
//#define CAN_USE_DEBUG_EN_IN
//#define CAN_USE_DEBUG_EN_OUT
//#define CAN_USE_DEBUG_EN


#define CMD_TYPE_CHECK     0
#define CMD_CHECK_ID	   1
#define CMD_CHECK_ACK_ID   2
#define CMD_SELF_CHECK_ID	    3
#define CMD_SELF_CHECK_ACK_ID   4

/*-----------------*/

#define CMD_TYPE_CTRL      1

#define CMD_DI_ID		   1
#define CMD_DI_ACK_ID	   2
#define CMD_DO_ID		   3
#define CMD_DO_ACK_ID	   4
#define CMD_AD_ID		   5
#define CMD_AD_ACK_ID	   6

#define CMD_DO_ALL_ID      7
#define CMD_DO_ALL_ACK_ID  8

#define CMD_DI_ALL_ID	   9
#define CMD_DI_ALL_ACK_ID  10

/*-----------------*/
#define CMD_TYPE_CMP		2

#define CMD_STEPS_SET_ID			1
#define CMD_STEPS_SET_ACK_ID		2

#define CMD_STEP_DO_SET_ID			3
#define CMD_STEP_DO_SET_ACK_ID		4

#define CMD_STEP_TIME_SET_ID		5
#define CMD_STEP_TIME_SET_ACK_ID	6

#define CMD_LIST_DO_ID				7
#define CMD_LIST_DO_ACK_ID			8

#define CMD_LIST_STATE_ID			9
#define CMD_LIST_STATE_ACK_ID		10

//msg type max count
#define CAN_MSG_TYPE_MAX	(2+1)
//msg command max count
#define CAN_MSG_CMD_MAX		10

/*--------------------------------------*/
//CAN msg type
typedef union
{
	struct{
	uint8_t dst;
	uint8_t src;
	uint8_t type;
	uint8_t cmd;
	uint8_t data[4];
	};
	uint8_t data_m[8];
}CAN_Msg_t;

//CAN msg node type
typedef struct _can_msg_node{
	CAN_Msg_t msg;		//recive massge
	struct _can_msg_node *next;
	struct _can_msg_node *pre;
	struct timeval start_time; //recive timea, use for time over
}CAN_Msg_node_t;

typedef struct{
	sem_t sem;
	uint32_t cnt;
	pthread_mutex_t mutex;
}CAN_ACK_sem_t;

//CAN massage recive
typedef struct
{
	uint32_t msg_cnt;	//not deal recive massage count
	long msg_timedout_cnt;	//test use,timed out message count
	CAN_ACK_sem_t ack[CAN_MSG_TYPE_MAX][CAN_MSG_CMD_MAX];
	CAN_t *can_bus;			//can bus
	CAN_Msg_node_t *head;	//can recive massage head
//	sem_t list_sem;
//	sem_t can_sem;
	pthread_mutex_t rcv_list_mutex;	//recive message list 
	pthread_mutex_t bus_mutex;	//point to ponit communication
}CAN_Use_t;


//can messgae inital
CAN_Use_t *can_message_init(char *name);
//打包数据包
int can_pack_msg(CAN_Msg_t *p,...);
//解数据包
int can_msg_parse(CAN_Msg_t *msg,...);

//write an wait for result by point to ponit
int can_write_and_read_msg(CAN_Use_t *can, CAN_Msg_t *msg);

//can use print message
void can_print_msg(char *head_name, long count, CAN_Msg_t msg);

#endif
/*------------------------------end file-----------------------------------*/

