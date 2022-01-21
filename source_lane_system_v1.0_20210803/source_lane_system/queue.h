/*******************************************************

*******************************************************/

#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdbool.h>

typedef struct
{
	void** pItems;
	unsigned int iFront;
	unsigned int iRear;
	unsigned int nSize;
	unsigned int nCapacity;
}Queue;


Queue* QueueCreate(unsigned int nCapacity);
void QueueFinish(Queue* pQueue);

bool QueueJudgeFull(Queue* pQueue);
bool QueueJudgeEmpty(Queue* pQueue);

void QueuePush(Queue* pQueue, void* pItem);
void* QueuePop(Queue* pQueue);

#endif
/*--------------------end file-------------------------*/


