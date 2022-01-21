
#ifndef _CAN_BUS_H
#define _CAN_BUS_H

//can bus 调试信息开关

//#define CAN_BUS_DEBUG_EN

#if defined CAN_BUS_DEBUG_EN
//list get message
//#define CAN_BUS_DEBUG_EN_MSG
//socket recive frame
//#define CAN_BUS_DEBUG_EN_FRAME
//send
//#define CAN_BUS_DEBUG_EN_SEND
#endif

#if defined CAN_BUS_DEBUG_EN
#define print_can(fmt,args...)	printf(fmt,##args)
#else

#define print_can(fmt,args...)	do{}while(0)
#endif


#include <semaphore.h>
#include <stdint.h>
//CAN BUS NAME
#define CAN0	"can0"
#define CAN1	"can1"


typedef struct _can_frame_t{
	uint8_t type;	//bit0=0 = SFF,bit0=1=EFF,bit1=0 data frame, bit1=1 remote frame
	uint8_t  dlc;
	uint8_t data[8];
	uint32_t id;
	struct _can_frame_t *next;
	struct _can_frame_t *pre;
}can_frame_t;

//can type
typedef struct{
	char name[8];			//CAN bus name
	int can_fd;				//CAN socket file descriptor
	int rcv_flag;			//CAN recive thread is ready
	long rcv_frame_cnt;		//CAN recive frame count(test use)
	long snd_frame_cnt;		//CAN send frame count(test use)

	can_frame_t *head;		//CAN recive msg list head
	long frame_cnt;			//CAN recive msg not deal frame count(test use)
//	sem_t rcv_sem;			//CAN recive msg list sem-init 1
	pthread_mutex_t rcv_list_mutex;	//CAN recive message list
	sem_t frame_sem;		//recive a frame need to deal sem init 0 not
}CAN_t;

//open can
CAN_t *open_can(char *can_name);
//close can
void close_can(CAN_t *can);

//can write frame
int write_can_frame(CAN_t *can, can_frame_t msg);

//read can frame from recive msg list
int read_can_frame(CAN_t *can,can_frame_t *msg);

void print_can_frame_t(char * head_name,long count, can_frame_t frame);
#endif
/*----------------------------end file-------------------------------------*/

