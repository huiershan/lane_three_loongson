
#ifndef _LED_CTRL_H
#define _LED_CTRL_H

/*----------*/

int Led_Init(void);
int Led_Number_Set(unsigned char addr, unsigned int num);
int Led_Point_Set(unsigned char addr, unsigned short point);

#endif
/*-----------------------end file------------------------------*/

