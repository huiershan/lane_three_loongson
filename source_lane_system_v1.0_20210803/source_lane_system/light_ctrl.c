
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "debug.h"
#include "global.h"
#include "sysutil_d.h"
#include "light_ctrl.h"
#include <sys/time.h>
#include <sys/select.h>

int light_fd0,light_fd1,light_fd2;
#define SYSFS_GPIO_LIGHT1_OUT "/sys/class/gpio/gpio56/value"     //led7  bule
#define SYSFS_GPIO_LIGHT2_OUT "/sys/class/gpio/gpio59/value"     //led8 yellow
#define SYSFS_GPIO_LIGHT3_OUT "/sys/class/gpio/gpio54/value"     //led9 gree


int cf37_fd,cf36_fd,cf2_fd,cf40_fd,cf35_fd,cf3_fd,cf39_fd,cf38_fd;

#define SYSFS_GPIO_CF37_OUT "/sys/class/gpio/gpio37/value"
#define SYSFS_GPIO_CF36_OUT "/sys/class/gpio/gpio36/value"
#define SYSFS_GPIO_CF2_OUT "/sys/class/gpio/gpio2/value"
#define SYSFS_GPIO_CF40_OUT "/sys/class/gpio/gpio40/value"
#define SYSFS_GPIO_CF35_OUT "/sys/class/gpio/gpio35/value"
#define SYSFS_GPIO_CF3_OUT "/sys/class/gpio/gpio3/value"
#define SYSFS_GPIO_CF39_OUT "/sys/class/gpio/gpio39/value"
#define SYSFS_GPIO_CF38_OUT "/sys/class/gpio/gpio38/value"


volatile int gStartDownload=0;
volatile int gEndDownload=0;
volatile int gFailDownload=0;
int download_sucess=0;

int Light_State(void)
{
	while(1)
	{
		msleep_m(200);

		if((gStartDownload==1 || gEndDownload==1) && (gFailDownload==0))
		{
			Light_DO_Set(1,1); //close download finish light
			Light_DO_Set(2,1); //close download fail light
			
			msleep_m(250);
			Light_DO_Set(3,0);
			msleep_m(250);
			Light_DO_Set(3,1);
		}
		
		if(download_sucess == 1)
		{
			Light_DO_Set(3,1); //close download start light
			
			Light_DO_Set(1,0);
                   download_sucess=0;
			
		}

		if(gFailDownload == 1)
		{
			Light_DO_Set(2,0);
			Light_DO_Set(3,1);
			Light_DO_Set(1,1);
			gFailDownload=0;

		}
	}

	return 0;
}

int Download_Firmware(void)
{
	while(1)
	{
		msleep_m(200);
		//if(gStartDownload==1)
		//{
			
		//	system("sh /home/app/download_to_tftpboot.sh");
		//	printf("start download ,clear tftpboot!\n"); 
			
		//}

		if(gEndDownload==1)
		{
			CF_TO_1B();
			printf("cf to 1b!\n"); 
			msleep_m(1000);
			system("sh /home/app/rename.sh");
			msleep_m(1000);
			system("sh /home/app/copy_to_cf.sh");
			printf("copy to cf !\n"); 
			CF_TO_MASH();
			printf("connect to mash !\n"); 
			msleep_m(1000);
			gEndDownload=0;
			download_sucess=1;
			gStartDownload=0;
			gEndDownload=0;
		}
	}

	return 0;
}



int Light_Init(void)
{
	light_fd0 = open(SYSFS_GPIO_LIGHT1_OUT, O_WRONLY);        
	if(-1 == light_fd0)        
	{                
		printf("open light gpio error!\n");                               
 		return -1;        
 	}

	light_fd1 = open(SYSFS_GPIO_LIGHT2_OUT, O_WRONLY);        
	if(-1 == light_fd1)        
	{                
		printf("open light gpio error!\n");                               
 		return -1;        
 	}

	light_fd2 = open(SYSFS_GPIO_LIGHT3_OUT, O_WRONLY);        
	if(-1 == light_fd2)        
	{                
		printf("open light gpio error!\n");                               
 		return -1;        
 	}

	
	printf("light_gpio_fd=%d\n",light_fd0);
	return 1;
}

int Light_DO_Set(unsigned char num, unsigned char value)
{
	int light_fd;
	if(num ==1) light_fd=light_fd0;
	else if(num ==2) light_fd=light_fd1;
	else if(num ==3) light_fd=light_fd2;
	
	lseek(light_fd, 0, SEEK_SET);
	
	if(value == 1)
	{
		if(write(light_fd,"1",1) <0)
		{
			printf("write light gpio error!\n");
			return -1;
		}
		
	}else if(value == 0)
		{
			if(write(light_fd,"0",1) <0)
			{
				printf("write light gpio error!\n");
				return -1;
		}
	}

	//printf("--value = %d\n",value);
	return 1;
}


int CF_Init(void)
{
	cf37_fd = open(SYSFS_GPIO_CF37_OUT, O_WRONLY);        
	if(-1 == cf37_fd)        
	{                
		printf("open light gpio error!\n");                               
 		return -1;        
 	}

	cf36_fd = open(SYSFS_GPIO_CF36_OUT, O_WRONLY);        
	if(-1 == cf36_fd)        
	{                
		printf("open light gpio error!\n");                               
 		return -1;        
 	}

	cf2_fd = open(SYSFS_GPIO_CF2_OUT, O_WRONLY);        
	if(-1 == cf2_fd)        
	{                
		printf("open light gpio error!\n");                               
 		return -1;        
 	}

	cf40_fd = open(SYSFS_GPIO_CF40_OUT, O_WRONLY);        
	if(-1 == cf40_fd)        
	{                
		printf("open light gpio error!\n");                               
 		return -1;        
 	}

	cf35_fd = open(SYSFS_GPIO_CF35_OUT, O_WRONLY);        
	if(-1 == cf35_fd)        
	{                
		printf("open light gpio error!\n");                               
 		return -1;        
 	}

	cf3_fd = open(SYSFS_GPIO_CF3_OUT, O_WRONLY);        
	if(-1 == cf3_fd)        
	{                
		printf("open light gpio error!\n");                               
 		return -1;        
 	}

	cf39_fd = open(SYSFS_GPIO_CF39_OUT, O_WRONLY);        
	if(-1 == cf39_fd)        
	{                
		printf("open light gpio error!\n");                               
 		return -1;        
 	}

	cf38_fd = open(SYSFS_GPIO_CF38_OUT, O_WRONLY);        
	if(-1 == cf38_fd)        
	{                
		printf("open light gpio error!\n");                               
 		return -1;        
 	}
	
	return 1;
}



int CF_DO_Set(unsigned char num, unsigned char value)
{
	int CF_fd;
	if(num ==37) CF_fd=cf37_fd;
	else if(num ==36) CF_fd=cf36_fd;
	else if(num ==2) CF_fd=cf2_fd;
	else if(num ==40) CF_fd=cf40_fd;
	else if(num ==35) CF_fd=cf35_fd;
	else if(num ==3) CF_fd=cf3_fd;
	else if(num ==39) CF_fd=cf39_fd;
	else if(num ==38) CF_fd=cf38_fd;
	
	
	lseek(CF_fd, 0, SEEK_SET);
	
	if(value == 1)
	{
		if(write(CF_fd,"1",1) <0)
		{
			printf("write CF gpio error!\n");
			return -1;
		}

	}else if(value == 0)
		{
			if(write(CF_fd,"0",1) <0)
			{
				printf("write CF gpio error!\n");
				return -1;
		}
	}

	//printf("--value = %d\n",value);
	return 1;
}

int CF_TO_MASH(void)
{
	CF_DO_Set(38,1);
	CF_DO_Set(39,0);

	CF_DO_Set(36,0);
	CF_DO_Set(2,0);
	CF_DO_Set(40,0);
	CF_DO_Set(35,0);

	CF_DO_Set(37,0);
	CF_DO_Set(3,0);

	return 1;
}

int CF_TO_1B(void)
{
	CF_DO_Set(37,1);
	CF_DO_Set(36,1);

	CF_DO_Set(2,1);
	CF_DO_Set(40,1);
	CF_DO_Set(35,1);
	CF_DO_Set(3,1);

	CF_DO_Set(38,0);
	CF_DO_Set(39,1);

	return 1;
}

/*---------------------------end file-----------------------------------------*/
