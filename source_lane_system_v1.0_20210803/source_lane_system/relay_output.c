#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>   
#include "relay_output.h"


int reley_gpio_value_fd[32];


int relay_gpio_export_init(void)		
{
	int fd;
	fd=open(SYSFS_GPIO_PATH,O_WRONLY);	
	if(fd==-1)
	{
		printf("open relay gpio export error\n");   
		return -1;
	}
	
	/* /sys/class/gpio/gpio208 */			
	write(fd,RELAY1,sizeof(RELAY1)); 
	write(fd,RELAY2,sizeof(RELAY2)); 
	write(fd,RELAY3,sizeof(RELAY3)); 
	write(fd,RELAY4,sizeof(RELAY4)); 
	write(fd,RELAY5,sizeof(RELAY5)); 
	write(fd,RELAY6,sizeof(RELAY6)); 
	write(fd,RELAY7,sizeof(RELAY7)); 	
	
	
	write(fd,RELAY8,sizeof(RELAY8)); 
	write(fd,RELAY9,sizeof(RELAY9)); 
	write(fd,RELAY10,sizeof(RELAY10)); 
	write(fd,RELAY11,sizeof(RELAY11)); 
	write(fd,RELAY12,sizeof(RELAY12)); 
	write(fd,RELAY13,sizeof(RELAY13)); 
	write(fd,RELAY14,sizeof(RELAY14));
	
	
	write(fd,RELAY15,sizeof(RELAY15)); 
	write(fd,RELAY16,sizeof(RELAY16)); 
	write(fd,RELAY17,sizeof(RELAY17)); 
	write(fd,RELAY18,sizeof(RELAY18)); 
	write(fd,RELAY19,sizeof(RELAY19)); 
	write(fd,RELAY20,sizeof(RELAY20)); 
	write(fd,RELAY21,sizeof(RELAY21));
	
	
	write(fd,RELAY22,sizeof(RELAY22)); 
	write(fd,RELAY23,sizeof(RELAY23)); 
	write(fd,RELAY24,sizeof(RELAY24)); 
	write(fd,RELAY25,sizeof(RELAY25)); 
	write(fd,RELAY26,sizeof(RELAY26)); 
	write(fd,RELAY27,sizeof(RELAY27)); 
	write(fd,RELAY28,sizeof(RELAY28)); 
	
	
	close(fd);	
	return 1;
}


int relay_gpio_direction_set_init(void)
{
	int fd,i;
	char file[64]={0};

	for(i=0; i<28; i++)
	{
		memset(file,0,sizeof(file));
		if(i==0) strcpy(file,RELAY1_GPIO_DIR);
		else if(i==1) strcpy(file,RELAY2_GPIO_DIR);
		else if(i==2) strcpy(file,RELAY3_GPIO_DIR);
		else if(i==3) strcpy(file,RELAY4_GPIO_DIR);
		else if(i==4) strcpy(file,RELAY5_GPIO_DIR);
		else if(i==5) strcpy(file,RELAY6_GPIO_DIR);
		else if(i==6) strcpy(file,RELAY7_GPIO_DIR);
		else if(i==7) strcpy(file,RELAY8_GPIO_DIR);
		else if(i==8) strcpy(file,RELAY9_GPIO_DIR);
		else if(i==9) strcpy(file,RELAY10_GPIO_DIR);
		else if(i==10) strcpy(file,RELAY11_GPIO_DIR);
		else if(i==11) strcpy(file,RELAY12_GPIO_DIR);
		else if(i==12) strcpy(file,RELAY13_GPIO_DIR);
		else if(i==13) strcpy(file,RELAY14_GPIO_DIR);
		else if(i==14) strcpy(file,RELAY15_GPIO_DIR);
		else if(i==15) strcpy(file,RELAY16_GPIO_DIR);
		else if(i==16) strcpy(file,RELAY17_GPIO_DIR);
		else if(i==17) strcpy(file,RELAY18_GPIO_DIR);
		else if(i==18) strcpy(file,RELAY19_GPIO_DIR);
		else if(i==19) strcpy(file,RELAY20_GPIO_DIR);
		else if(i==20) strcpy(file,RELAY21_GPIO_DIR);
		else if(i==21) strcpy(file,RELAY22_GPIO_DIR);
		else if(i==22) strcpy(file,RELAY23_GPIO_DIR);
		else if(i==23) strcpy(file,RELAY24_GPIO_DIR);
		else if(i==24) strcpy(file,RELAY25_GPIO_DIR);
		else if(i==25) strcpy(file,RELAY26_GPIO_DIR);
		else if(i==26) strcpy(file,RELAY27_GPIO_DIR);
		else if(i==27) strcpy(file,RELAY28_GPIO_DIR);


		fd=open(file,O_WRONLY);

		if(fd==-1)
		{
			printf("open relay[%d] gpio dir set fail\n",i+1);   
			//return -1;
		}
		else
		{
			write(fd,OUT,sizeof(OUT)); 
		}
		
	}

	close(fd);

	return 1;
}


int relay_gpio_value_set_init(void)
{
	int fd,i;
	char file[64]={0};

	for(i=0; i<28; i++)
	{
		memset(file,0,sizeof(file));
		if(i==0) strcpy(file,RELAY1_GPIO_VALUE);
		else if(i==1) strcpy(file,RELAY2_GPIO_VALUE);
		else if(i==2) strcpy(file,RELAY3_GPIO_VALUE);
		else if(i==3) strcpy(file,RELAY4_GPIO_VALUE);
		else if(i==4) strcpy(file,RELAY5_GPIO_VALUE);
		else if(i==5) strcpy(file,RELAY6_GPIO_VALUE);
		else if(i==6) strcpy(file,RELAY7_GPIO_VALUE);
		else if(i==7) strcpy(file,RELAY8_GPIO_VALUE);
		else if(i==8) strcpy(file,RELAY9_GPIO_VALUE);
		else if(i==9) strcpy(file,RELAY10_GPIO_VALUE);
		else if(i==10) strcpy(file,RELAY11_GPIO_VALUE);
		else if(i==11) strcpy(file,RELAY12_GPIO_VALUE);
		else if(i==12) strcpy(file,RELAY13_GPIO_VALUE);
		else if(i==13) strcpy(file,RELAY14_GPIO_VALUE);
		else if(i==14) strcpy(file,RELAY15_GPIO_VALUE);
		else if(i==15) strcpy(file,RELAY16_GPIO_VALUE);
		else if(i==16) strcpy(file,RELAY17_GPIO_VALUE);
		else if(i==17) strcpy(file,RELAY18_GPIO_VALUE);
		else if(i==18) strcpy(file,RELAY19_GPIO_VALUE);
		else if(i==19) strcpy(file,RELAY20_GPIO_VALUE);
		else if(i==20) strcpy(file,RELAY21_GPIO_VALUE);
		else if(i==21) strcpy(file,RELAY22_GPIO_VALUE);
		else if(i==22) strcpy(file,RELAY23_GPIO_VALUE);
		else if(i==23) strcpy(file,RELAY24_GPIO_VALUE);
		else if(i==24) strcpy(file,RELAY25_GPIO_VALUE);
		else if(i==25) strcpy(file,RELAY26_GPIO_VALUE);
		else if(i==26) strcpy(file,RELAY27_GPIO_VALUE);
		else if(i==27) strcpy(file,RELAY28_GPIO_VALUE);


		fd=open(file,O_WRONLY);	

		reley_gpio_value_fd[i]=fd;
		if(fd==-1)
		{
			printf("open relay[%d] gpio value set fail\n",i+1);   
			//return -1;
		}
		else
		{
			write(fd,OFF,sizeof(OFF)); 
		}
		
	}

	close(fd);

	return 1;

}

int relay_on_off_set(int grop_id,int num, int cmd)
{
	int relay_num=0;

	relay_num=(grop_id-1)*7+num;

	if(relay_num>28 || relay_num<0) return -1;
	else
	{
		if(cmd==0) 
		{
			write(reley_gpio_value_fd[relay_num-1],OFF,sizeof(OFF)); 
			return 1;
		}

		else if(cmd==1)
		{
			write(reley_gpio_value_fd[relay_num-1],ON,sizeof(ON));
			return 1;
		}

		else return -1;
	}

}

int slow_lane_pass(void)
{
	if(relay_on_off_set(1,1,1)<0) return -1;
	if(relay_on_off_set(1,2,0)<0) return -1;
	if(relay_on_off_set(1,3,0)<0) return -1;
	if(relay_on_off_set(1,4,0)<0) return -1;
	if(relay_on_off_set(1,5,1)<0) return -1;
	if(relay_on_off_set(1,6,0)<0) return -1;

	return 1;
}

int slow_lane_forbid(void)
{
	if(relay_on_off_set(1,1,0)<0) return -1;
	if(relay_on_off_set(1,2,1)<0) return -1;
	if(relay_on_off_set(1,3,0)<0) return -1;
	if(relay_on_off_set(1,4,0)<0) return -1;
	if(relay_on_off_set(1,5,1)<0) return -1;
	if(relay_on_off_set(1,6,0)<0) return -1;


	return 1;
}

int slow_lane_reverse(void)
{
	if(relay_on_off_set(1,1,0)<0) return -1;
	if(relay_on_off_set(1,2,1)<0) return -1;
	if(relay_on_off_set(1,3,0)<0) return -1;
	if(relay_on_off_set(1,4,1)<0) return -1;
	if(relay_on_off_set(1,5,0)<0) return -1;
	if(relay_on_off_set(1,6,0)<0) return -1;

	
	return 1;
}

int slow_lane_turn_front(void)
{
	if(relay_on_off_set(1,1,0)<0) return -1;
	if(relay_on_off_set(1,2,0)<0) return -1;
	if(relay_on_off_set(1,3,1)<0) return -1;
	if(relay_on_off_set(1,4,0)<0) return -1;
	if(relay_on_off_set(1,5,1)<0) return -1;
	if(relay_on_off_set(1,6,0)<0) return -1;


	return 1;
}

int slow_lane_turn_back(void)
{
	if(relay_on_off_set(1,1,0)<0) return -1;
	if(relay_on_off_set(1,2,1)<0) return -1;
	if(relay_on_off_set(1,3,0)<0) return -1;
	if(relay_on_off_set(1,4,0)<0) return -1;
	if(relay_on_off_set(1,5,0)<0) return -1;
	if(relay_on_off_set(1,6,1)<0) return -1;

	
	return 1;
}

/*示范历程 不做最后代码处理*/
int slow_lane_pass_fornt_cha(void)
{
	if(relay_on_off_set(1,1,1)<0) return -1;
	if(relay_on_off_set(1,2,0)<0) return -1;
	if(relay_on_off_set(1,3,1)<0) return -1;
	if(relay_on_off_set(1,4,0)<0) return -1;
	if(relay_on_off_set(1,5,1)<0) return -1;
	if(relay_on_off_set(1,6,0)<0) return -1;

	
	return 1;
}

int main_lane_pass(void)
{
	if(relay_on_off_set(2,1,1)<0) return -1;
	if(relay_on_off_set(2,2,0)<0) return -1;
	if(relay_on_off_set(2,3,0)<0) return -1;
	if(relay_on_off_set(2,4,0)<0) return -1;
	if(relay_on_off_set(2,5,1)<0) return -1;
	if(relay_on_off_set(2,6,0)<0) return -1;

	
	return 1;
}

int main_lane_forbid(void)
{
	if(relay_on_off_set(2,1,0)<0) return -1;
	if(relay_on_off_set(2,2,1)<0) return -1;
	if(relay_on_off_set(2,3,0)<0) return -1;
	if(relay_on_off_set(2,4,0)<0) return -1;
	if(relay_on_off_set(2,5,1)<0) return -1;
	if(relay_on_off_set(2,6,0)<0) return -1;

	
	return 1;
}

int main_lane_reverse(void)
{
	if(relay_on_off_set(2,1,0)<0) return -1;
	if(relay_on_off_set(2,2,1)<0) return -1;
	if(relay_on_off_set(2,3,0)<0) return -1;
	if(relay_on_off_set(2,4,1)<0) return -1;
	if(relay_on_off_set(2,5,0)<0) return -1;
	if(relay_on_off_set(2,6,0)<0) return -1;

	
	return 1;
}

int main_lane_turn_front(void)
{
	if(relay_on_off_set(2,1,0)<0) return -1;
	if(relay_on_off_set(2,2,0)<0) return -1;
	if(relay_on_off_set(2,3,1)<0) return -1;
	if(relay_on_off_set(2,4,0)<0) return -1;
	if(relay_on_off_set(2,5,1)<0) return -1;
	if(relay_on_off_set(2,6,0)<0) return -1;

	
	return 1;
}

int main_lane_turn_back(void)
{
	if(relay_on_off_set(2,1,0)<0) return -1;
	if(relay_on_off_set(2,2,1)<0) return -1;
	if(relay_on_off_set(2,3,0)<0) return -1;
	if(relay_on_off_set(2,4,0)<0) return -1;
	if(relay_on_off_set(2,5,0)<0) return -1;
	if(relay_on_off_set(2,6,1)<0) return -1;

	
	return 1;
}

int over_lane_pass(void)
{
	if(relay_on_off_set(3,1,1)<0) return -1;
	if(relay_on_off_set(3,2,0)<0) return -1;
	if(relay_on_off_set(3,3,0)<0) return -1;
	if(relay_on_off_set(3,4,0)<0) return -1;
	if(relay_on_off_set(3,5,1)<0) return -1;
	if(relay_on_off_set(3,6,0)<0) return -1;


	return 1;
}

int over_lane_forbid(void)
{
	if(relay_on_off_set(3,1,0)<0) return -1;
	if(relay_on_off_set(3,2,1)<0) return -1;
	if(relay_on_off_set(3,3,0)<0) return -1;
	if(relay_on_off_set(3,4,0)<0) return -1;
	if(relay_on_off_set(3,5,1)<0) return -1;
	if(relay_on_off_set(3,6,0)<0) return -1;


	return 1;
}

int over_lane_reverse(void)
{
	if(relay_on_off_set(3,1,0)<0) return -1;
	if(relay_on_off_set(3,2,1)<0) return -1;
	if(relay_on_off_set(3,3,0)<0) return -1;
	if(relay_on_off_set(3,4,1)<0) return -1;
	if(relay_on_off_set(3,5,0)<0) return -1;
	if(relay_on_off_set(3,6,0)<0) return -1;


	return 1;
}

int over_lane_turn_front(void)
{
	if(relay_on_off_set(3,1,0)<0) return -1;
	if(relay_on_off_set(3,2,0)<0) return -1;
	if(relay_on_off_set(3,3,1)<0) return -1;
	if(relay_on_off_set(3,4,0)<0) return -1;
	if(relay_on_off_set(3,5,1)<0) return -1;
	if(relay_on_off_set(3,6,0)<0) return -1;

	
	return 1;
}

int over_lane_turn_back(void)
{
	if(relay_on_off_set(3,1,0)<0) return -1;
	if(relay_on_off_set(3,2,1)<0) return -1;
	if(relay_on_off_set(3,3,0)<0) return -1;
	if(relay_on_off_set(3,4,0)<0) return -1;
	if(relay_on_off_set(3,5,0)<0) return -1;
	if(relay_on_off_set(3,6,1)<0) return -1;

	
	return 1;
}


int relay_output_init(void)
{

	if(relay_gpio_export_init()<0) return -1;
	if(relay_gpio_direction_set_init()<0) return -1;
	if(relay_gpio_value_set_init()<0) return -1;

	return 1;
	
}

