/*******************************************************

*******************************************************/


#include <stdio.h>
#include "queue.h"

Queue* QueueCreate(unsigned int nCapacity)
{
	Queue* pQueue = NULL;
	
	pQueue = malloc(sizeof(Queue));
	if(pQueue == NULL)
	{
		return NULL;
	}
	pQueue->pItems = malloc(nCapacity*sizeof(void*));

	if(pQueue->pItems == NULL)
	{
		free(pQueue);
		return NULL;
	}

	pQueue->nCapacity = nCapacity;
	pQueue->iFront = 0;
	pQueue->iRear = 0;
	pQueue->nSize = 0;

	return pQueue;
}

void QueueFinish(Queue* pQueue)
{
	if(pQueue == NULL)
	{
		return;
	}
	if(pQueue->pItems == NULL)
	{
		return;
	}

	memset(pQueue->pItems, 0, ((pQueue->nCapacity)*sizeof(void*)));
	free(pQueue->pItems);
	free(pQueue);
	return;
}

bool QueueJudgeFull(Queue* pQueue)
{
	if(pQueue == NULL)
		return true;

	if(pQueue->nCapacity == pQueue->nSize)
		return true;

	return false;
}

bool QueueJudgeEmpty(Queue* pQueue)
{
	if(pQueue == NULL)
		return false;

	if(pQueue->nSize > 0)
		return false;

	return true;
}

void QueuePush(Queue* pQueue, void* pItem)
{
	if(pQueue == NULL || pItem == NULL)
		return;

	pQueue->pItems[pQueue->iRear] = pItem;
	if((pQueue->iRear + 1) == pQueue->nCapacity)
	{
		pQueue->iRear = 0;
	}else{
		pQueue->iRear++;
	}
	pQueue->nSize++;

	return;
}

void* QueuePop(Queue* pQueue)
{
	void* pItem = NULL;

	if(pQueue == NULL)
		return NULL;

	pItem = pQueue->pItems[pQueue->iFront];
	if((pQueue->iFront + 1) == pQueue->nCapacity)
	{
		pQueue->iFront = 0;
	}else{
		pQueue->iFront++;
	}
	pQueue->nSize--;

	return pItem;
}

/*------------------------end file-------------------------------*/

