/*-----------------------------------------------------*/

/*-----------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "meter_ctrl.h"
#include "serial.h"
#include "global.h"

int Volt_Number_Set(unsigned char addr, unsigned int num)
{
	unsigned char buf[16];
	unsigned char tmp[16];
	unsigned short crc16=0;
	int i,res=0;

	memset(buf, 0, sizeof(buf));	
	memset(tmp, 0, sizeof(tmp));

	buf[0]=addr;
	buf[1]=0x06;
	buf[2]=0x00;
	buf[3]=0x00;
	buf[4]=(num>>8)&0xff;
	buf[5]=num&0xff;

	memcpy(tmp, buf, 6);
	
	crc16=crc16table(tmp, 6);
	printf("crc16=%x\n", crc16);
	buf[6]=crc16&0xff;
	buf[7]=(crc16>>8)&0xff;

	printf("----set volt number\n");
	for(i=0;i<8;i++)
	{
       	printf("buf[%d]=%02x\n",i,buf[i]);
	}

	//write(gUart_fd, buf, 8);
	if(res = write(gUart_fd, buf, 8) != 8)
	{
		printf("Error write uart!\n");
		return -1;

	}

	return 1;

}


int Current_Number_Set(unsigned char addr, unsigned int num)
{
	unsigned char buf[16];
	unsigned char tmp[16];
	unsigned short crc16=0;
	int i,res=0;

	memset(buf, 0, sizeof(buf));	
	memset(tmp, 0, sizeof(tmp));

	buf[0]=addr;
	buf[1]=0x06;
	buf[2]=0x00;
	buf[3]=0x01;
	buf[4]=(num>>8)&0xff;
	buf[5]=num&0xff;

	memcpy(tmp, buf, 6);
	
	crc16=crc16table(tmp, 6);
	printf("crc16=%x\n", crc16);
	buf[6]=crc16&0xff;
	buf[7]=(crc16>>8)&0xff;

	printf("----set current number\n");
	for(i=0;i<8;i++)
	{
       	printf("buf[%d]=%02x\n",i,buf[i]);
	}

	//write(gUart_fd, buf, 8);
	if(res = write(gUart_fd, buf, 8) != 8)
	{
		printf("Error write uart!\n");
		return -1;

	}

	return 1;

}



/*--------------------end file------------------------*/

