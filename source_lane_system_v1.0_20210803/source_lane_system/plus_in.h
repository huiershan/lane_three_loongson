/*-----------------------------------------------------*/
/*-----------------------------------------------------*/

#ifndef _PLUS_IN_H
#define _PLUS_IN_H
#include "queue.h"

typedef struct
{
	unsigned char head;
	unsigned char type;
	unsigned char len;
	unsigned char data[8];
}Plus_t;

void Plus_Receive(void);

extern sem_t gPlusRxState_sem;
extern Queue* pPlusRxQueue;


#endif
/*-----------------------------------------------------*/
//end file

