#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>   
#include <semaphore.h>
#include "gpio_input.h"
#include "global.h"
#include "remote_ctrl.h"

int input_gpio_value_fd[32];
char input_gpio_state[32];

sem_t gInputGPIO_sem;

lane_led_state_t lane;

gpio_input_state_t gpio_input;

int input_gpio_export(void)			
{
	int fd=open(SYSFS_GPIO_PATH,O_WRONLY);
	if(fd==-1)
	{
		printf("open input gpio export error\n");   
		return -1;
	}
	
	write(fd,INPUT1,sizeof(INPUT1)); 
	write(fd,INPUT2,sizeof(INPUT2)); 
	write(fd,INPUT3,sizeof(INPUT3)); 
	write(fd,INPUT4,sizeof(INPUT4)); 
	write(fd,INPUT5,sizeof(INPUT5)); 
	write(fd,INPUT6,sizeof(INPUT6)); 
	write(fd,INPUT7,sizeof(INPUT7)); 
	
	
	write(fd,INPUT8,sizeof(INPUT8));
	write(fd,INPUT9,sizeof(INPUT9)); 
	write(fd,INPUT10,sizeof(INPUT10)); 
	write(fd,INPUT11,sizeof(INPUT11)); 
	write(fd,INPUT12,sizeof(INPUT12)); 
	write(fd,INPUT13,sizeof(INPUT13)); 
	write(fd,INPUT14,sizeof(INPUT14)); 
	
	write(fd,INPUT15,sizeof(INPUT15)); 
	write(fd,INPUT16,sizeof(INPUT16)); 	
	write(fd,INPUT17,sizeof(INPUT17)); 
	write(fd,INPUT18,sizeof(INPUT18)); 
	write(fd,INPUT19,sizeof(INPUT19)); 
	write(fd,INPUT20,sizeof(INPUT20)); 
	write(fd,INPUT21,sizeof(INPUT21)); 
	
	
	write(fd,INPUT22,sizeof(INPUT22)); 
	write(fd,INPUT23,sizeof(INPUT23)); 
	write(fd,INPUT24,sizeof(INPUT24));
	write(fd,INPUT25,sizeof(INPUT25)); 
	write(fd,INPUT26,sizeof(INPUT26)); 
	write(fd,INPUT27,sizeof(INPUT27)); 
	write(fd,INPUT28,sizeof(INPUT28));  
	
	
	close(fd);	
	return fd;
}


int input_gpio_direction_set(void)
{
	int fd,i;
	char file[64]={0};

	for(i=0; i<28; i++)
	{
		memset(file,0,sizeof(file));
		if(i==0) strcpy(file,INPUT1_GPIO_DIR);
		else if(i==1) strcpy(file,INPUT2_GPIO_DIR);
		else if(i==2) strcpy(file,INPUT3_GPIO_DIR);
		else if(i==3) strcpy(file,INPUT4_GPIO_DIR);
		else if(i==4) strcpy(file,INPUT5_GPIO_DIR);
		else if(i==5) strcpy(file,INPUT6_GPIO_DIR);
		else if(i==6) strcpy(file,INPUT7_GPIO_DIR);
		else if(i==7) strcpy(file,INPUT8_GPIO_DIR);
		else if(i==8) strcpy(file,INPUT9_GPIO_DIR);
		else if(i==9) strcpy(file,INPUT10_GPIO_DIR);
		else if(i==10) strcpy(file,INPUT11_GPIO_DIR);
		else if(i==11) strcpy(file,INPUT12_GPIO_DIR);
		else if(i==12) strcpy(file,INPUT13_GPIO_DIR);
		else if(i==13) strcpy(file,INPUT14_GPIO_DIR);
		else if(i==14) strcpy(file,INPUT15_GPIO_DIR);
		else if(i==15) strcpy(file,INPUT16_GPIO_DIR);
		else if(i==16) strcpy(file,INPUT17_GPIO_DIR);
		else if(i==17) strcpy(file,INPUT18_GPIO_DIR);
		else if(i==18) strcpy(file,INPUT19_GPIO_DIR);
		else if(i==19) strcpy(file,INPUT20_GPIO_DIR);
		else if(i==20) strcpy(file,INPUT21_GPIO_DIR);
		else if(i==21) strcpy(file,INPUT22_GPIO_DIR);
		else if(i==22) strcpy(file,INPUT23_GPIO_DIR);
		else if(i==23) strcpy(file,INPUT24_GPIO_DIR);
		else if(i==24) strcpy(file,INPUT25_GPIO_DIR);
		else if(i==25) strcpy(file,INPUT26_GPIO_DIR);
		else if(i==26) strcpy(file,INPUT27_GPIO_DIR);
		else if(i==27) strcpy(file,INPUT28_GPIO_DIR);

		fd=open(file,O_WRONLY);
		if(fd==-1)
		{
			printf("open input[%d] gpio dir set fail\n",i+1);   
			//return -1;
		}
		else
		{
			write(fd,IN,sizeof(IN)); 
		}
		
	}

	close(fd);

	return 1;
}


int input_gpio_value_init(void)
{
	int fd,i;
	char file[64]={0};

	for(i=0; i<28; i++)
	{
		memset(file,0,sizeof(file));
		if(i==0) strcpy(file,INPUT1_GPIO_VALUE);
		else if(i==1) strcpy(file,INPUT2_GPIO_VALUE);
		else if(i==2) strcpy(file,INPUT3_GPIO_VALUE);
		else if(i==3) strcpy(file,INPUT4_GPIO_VALUE);
		else if(i==4) strcpy(file,INPUT5_GPIO_VALUE);
		else if(i==5) strcpy(file,INPUT6_GPIO_VALUE);
		else if(i==6) strcpy(file,INPUT7_GPIO_VALUE);
		else if(i==7) strcpy(file,INPUT8_GPIO_VALUE);
		else if(i==8) strcpy(file,INPUT9_GPIO_VALUE);
		else if(i==9) strcpy(file,INPUT10_GPIO_VALUE);
		else if(i==10) strcpy(file,INPUT11_GPIO_VALUE);
		else if(i==11) strcpy(file,INPUT12_GPIO_VALUE);
		else if(i==12) strcpy(file,INPUT13_GPIO_VALUE);
		else if(i==13) strcpy(file,INPUT14_GPIO_VALUE);
		else if(i==14) strcpy(file,INPUT15_GPIO_VALUE);
		else if(i==15) strcpy(file,INPUT16_GPIO_VALUE);
		else if(i==16) strcpy(file,INPUT17_GPIO_VALUE);
		else if(i==17) strcpy(file,INPUT18_GPIO_VALUE);
		else if(i==18) strcpy(file,INPUT19_GPIO_VALUE);
		else if(i==19) strcpy(file,INPUT20_GPIO_VALUE);
		else if(i==20) strcpy(file,INPUT21_GPIO_VALUE);
		else if(i==21) strcpy(file,INPUT22_GPIO_VALUE);
		else if(i==22) strcpy(file,INPUT23_GPIO_VALUE);
		else if(i==23) strcpy(file,INPUT24_GPIO_VALUE);
		else if(i==24) strcpy(file,INPUT25_GPIO_VALUE);
		else if(i==25) strcpy(file,INPUT26_GPIO_VALUE);
		else if(i==26) strcpy(file,INPUT27_GPIO_VALUE);
		else if(i==27) strcpy(file,INPUT28_GPIO_VALUE);

		fd=open(file,O_RDONLY);	
		input_gpio_value_fd[i]=fd;
		//printf("input_gpio_value_fd[%d]=%d\n",i,input_gpio_value_fd[i]);
		if(fd==-1)
		{
			printf("open relay[%d] gpio value set fail\n",i+1);   
			//return -1;
		}
		
	}

	close(fd);

	return 1;

}

//gpio输入初始化
int gpio_input_init(void)
{
	if(input_gpio_export()<0) return -1;
	if(input_gpio_direction_set()<0) return -1;
	if(input_gpio_value_init()<0) return -1;

	if(sem_init(&gInputGPIO_sem,0,1)<0) return -1;	//gpio输入信号量的初始化


	return 1;
}
//字符串转十六进制函数
unsigned int StrToHex(const char *pstr)
{
    unsigned int ull = 0;
    if (pstr != NULL) {
        while (*pstr != 0) {
            //char ch = *pstr++;
		char ch = *pstr;
            if (ch != ' ') {
                    while (*pstr != 0) {
                        unsigned char uc = 0;
                        if (*pstr >= '0' && *pstr <= '9') {
                            uc = (unsigned char)(*pstr - '0');
                        } else if (*pstr >= 'a' && *pstr <= 'f') {
                            uc = (unsigned char)(*pstr - 'a' + 10);
                        } else if (*pstr >= 'A' && *pstr <= 'F') {
                            uc = (unsigned char)(*pstr - 'A' + 10);
                        } else {
                            break;
                        }
                        ull <<= 4;
                        ull |= uc;
                        pstr++;
                    }
                }
                break;
        }
    }
    return ull;
}
//获取gpio输入状态
int input_gpio_vaule_read(int fd, char *value)
{
	unsigned char value1[8];
	unsigned char var;
	
       memset(value1,0,sizeof(value1));
   
	lseek(fd, 0, SEEK_SET);
	
	//printf("fd=%d \n",fd);

	if(read(fd,value1,2) == -1)
	{
		//printf("read input gpio value error\n");
		return -1;
	}
	
	//printf("fd=%d --value1 = %s\n",fd,value1);
	//var=StrToHex(value1);
	//atoi是将字符串转换为整数的函数
	var=atoi(value1);
	
	*value = var;

	return 1;

}

//将gpio输入状态输出到灯板
int gpio_input_state_get(void)
{
	int i;
	unsigned char gpio_state[28];
	unsigned char gpio_state_tmp[28];
	unsigned char gpio_state_chage_flag=0;


	memset(input_gpio_state,1,sizeof(input_gpio_state));
	
	memset(&gpio_input,0,sizeof(gpio_input_state_t));

	memset(&lane,0,sizeof(lane_led_state_t));
	
	memset(gpio_state,0,sizeof(gpio_state));
	memset(gpio_state_tmp,0,sizeof(gpio_state_tmp));

	while(1)
	{
		usleep(1000000);
		for(i=0;i<28;i++)
		{
			input_gpio_vaule_read(input_gpio_value_fd[i], &input_gpio_state[i]);
			gpio_state[i]=input_gpio_state[i];

		}

#if 1
		if(input_gpio_state[0]==0) lane.slow_front_go=1;
		else lane.slow_front_go=0;
		if(input_gpio_state[1]==0) lane.slow_front_stop=1;
		else lane.slow_front_stop=0;
		if(input_gpio_state[2]==0) lane.slow_front_turn=1;
		else  lane.slow_front_turn=0;
		if(input_gpio_state[3]==0) lane.slow_back_go=1;
		else lane.slow_back_go=0;
		if(input_gpio_state[4]==0) lane.slow_back_stop=1;
		else lane.slow_back_stop=0;
		if(input_gpio_state[5]==0) lane.slow_back_turn=1;
		else lane.slow_back_turn=0;

		if(input_gpio_state[7]==0) lane.main_front_go=1;
		else lane.main_front_go=0; 
		if(input_gpio_state[8]==0) lane.main_front_stop=1;
		else lane.main_front_stop=0;
		if(input_gpio_state[9]==0) lane.main_front_turn=1;
		else lane.main_front_turn=0;
		if(input_gpio_state[10]==0) lane.main_back_go=1;
		else lane.main_back_go=0;
		if(input_gpio_state[11]==0) lane.main_back_stop=1;
		else lane.main_back_stop=0;
		if(input_gpio_state[12]==0) lane.main_back_turn=1;
		else lane.main_back_turn=0;

		if(input_gpio_state[14]==0) lane.over_front_go=1;
		else lane.over_front_go=0;
		if(input_gpio_state[15]==0) lane.over_front_stop=1;
		else lane.over_front_stop=0;
		if(input_gpio_state[16]==0) lane.over_front_turn=1;
		else lane.over_front_turn=0;
		if(input_gpio_state[17]==0) lane.over_back_go=1;
		else lane.over_back_go=0;
		if(input_gpio_state[18]==0) lane.over_back_stop=1;
		else lane.over_back_stop=0;
		if(input_gpio_state[19]==0) lane.over_back_turn=1;
		else lane.over_back_turn=0;
		
		
		/*the first group gpio slow lane front led*/
		if(input_gpio_state[0]==1 && input_gpio_state[1]==1 && input_gpio_state[2]==1)  
		{			
			lane_infor.slow_front_led_state=heiping;
				
		}
		else if(input_gpio_state[0]==0 && input_gpio_state[1]==1 && input_gpio_state[2]==1)  
		{			
			lane_infor.slow_front_led_state=lvjian;
				
		}
		else if(input_gpio_state[0]==1 && input_gpio_state[1]==0 && input_gpio_state[2]==1)  
		{			
			lane_infor.slow_front_led_state=hongcha;
				
		}
		else if(input_gpio_state[0]==0 && input_gpio_state[1]==0 && input_gpio_state[2]==1)  
		{			
			lane_infor.slow_front_led_state=jiancha;
				
		}
		else if(input_gpio_state[0]==1 && input_gpio_state[1]==1 && input_gpio_state[2]==0)  
		{			
			lane_infor.slow_front_led_state=zuozhuan;
				
		}
		else if(input_gpio_state[0]==0 && input_gpio_state[1]==1 && input_gpio_state[2]==0)  
		{			
			lane_infor.slow_front_led_state=jianzhuan;
				
		}
		else if(input_gpio_state[0]==1 && input_gpio_state[1]==0 && input_gpio_state[2]==0)  
		{			
			lane_infor.slow_front_led_state=chazhuan;
				
		}
		else if(input_gpio_state[0]==0 && input_gpio_state[1]==0 && input_gpio_state[2]==0)  
		{			
			lane_infor.slow_front_led_state=jianchazhuan;
				
		}
		/*slow_lane_back_led*/
		if(input_gpio_state[3]==1 && input_gpio_state[4]==1 && input_gpio_state[5]==1)  
		{			
			lane_infor.slow_back_led_state=heiping;
				
		}
		else if(input_gpio_state[3]==0 && input_gpio_state[4]==1 && input_gpio_state[5]==1)  
		{			
			lane_infor.slow_back_led_state=lvjian;
				
		}
		else if(input_gpio_state[3]==1 && input_gpio_state[4]==0 && input_gpio_state[5]==1)  
		{			
			lane_infor.slow_back_led_state=hongcha;
				
		}
		else if(input_gpio_state[3]==0 && input_gpio_state[4]==0 && input_gpio_state[5]==1)  
		{			
			lane_infor.slow_back_led_state=jiancha;
				
		}
		else if(input_gpio_state[3]==1 && input_gpio_state[4]==1 && input_gpio_state[5]==0)  
		{			
			lane_infor.slow_back_led_state=zuozhuan;
				
		}
		else if(input_gpio_state[3]==0 && input_gpio_state[4]==1 && input_gpio_state[5]==0)  
		{			
			lane_infor.slow_back_led_state=jianzhuan;
				
		}
		else if(input_gpio_state[3]==1 && input_gpio_state[4]==0 && input_gpio_state[5]==0)  
		{			
			lane_infor.slow_back_led_state=chazhuan;
				
		}
		else if(input_gpio_state[3]==0 && input_gpio_state[4]==0 && input_gpio_state[5]==0)  
		{			
			lane_infor.slow_back_led_state=jianchazhuan;
				
		}


		
		/*the second group gpio-main lane front led*/
		if(input_gpio_state[0+7]==1 && input_gpio_state[1+7]==1 && input_gpio_state[2+7]==1)  
		{			
			lane_infor.main_front_led_state=heiping;
				
		}
		else if(input_gpio_state[0+7]==0 && input_gpio_state[1+7]==1 && input_gpio_state[2+7]==1)  
		{			
			lane_infor.main_front_led_state=lvjian;
				
		}
		else if(input_gpio_state[0+7]==1 && input_gpio_state[1+7]==0 && input_gpio_state[2+7]==1)  
		{			
			lane_infor.main_front_led_state=hongcha;
				
		}
		else if(input_gpio_state[0+7]==0 && input_gpio_state[1+7]==0 && input_gpio_state[2+7]==1)  
		{			
			lane_infor.main_front_led_state=jiancha;
				
		}
		else if(input_gpio_state[0+7]==1 && input_gpio_state[1+7]==1 && input_gpio_state[2+7]==0)  
		{			
			lane_infor.main_front_led_state=zuozhuan;
				
		}
		else if(input_gpio_state[0+7]==0 && input_gpio_state[1+7]==1 && input_gpio_state[2+7]==0)  
		{			
			lane_infor.main_front_led_state=jianzhuan;
				
		}
		else if(input_gpio_state[0+7]==1 && input_gpio_state[1+7]==0 && input_gpio_state[2+7]==0)  
		{			
			lane_infor.main_front_led_state=chazhuan;
				
		}
		else if(input_gpio_state[0+7]==0 && input_gpio_state[1+7]==0 && input_gpio_state[2+7]==0)  
		{			
			lane_infor.main_front_led_state=jianchazhuan;
				
		}
		/*main_lane_back_led*/
		if(input_gpio_state[3+7]==1 && input_gpio_state[4+7]==1 && input_gpio_state[5+7]==1)  
		{			
			lane_infor.main_back_led_state=heiping;
				
		}
		else if(input_gpio_state[3+7]==0 && input_gpio_state[4+7]==1 && input_gpio_state[5+7]==1)  
		{			
			lane_infor.main_back_led_state=lvjian;
				
		}
		else if(input_gpio_state[3+7]==1 && input_gpio_state[4+7]==0 && input_gpio_state[5+7]==1)  
		{			
			lane_infor.main_back_led_state=hongcha;
				
		}
		else if(input_gpio_state[3+7]==0 && input_gpio_state[4+7]==0 && input_gpio_state[5+7]==1)  
		{			
			lane_infor.main_back_led_state=jiancha;
				
		}
		else if(input_gpio_state[3+7]==1 && input_gpio_state[4+7]==1 && input_gpio_state[5+7]==0)  
		{			
			lane_infor.main_back_led_state=zuozhuan;
				
		}
		else if(input_gpio_state[3+7]==0 && input_gpio_state[4+7]==1 && input_gpio_state[5+7]==0)  
		{			
			lane_infor.main_back_led_state=jianzhuan;
				
		}
		else if(input_gpio_state[3+7]==1 && input_gpio_state[4+7]==0 && input_gpio_state[5+7]==0)  
		{			
			lane_infor.main_back_led_state=chazhuan;
				
		}
		else if(input_gpio_state[3+7]==0 && input_gpio_state[4+7]==0 && input_gpio_state[5+7]==0)  
		{			
			lane_infor.main_back_led_state=jianchazhuan;
				
		}


		

		/*the third group gpio over lane front led*/
		if(input_gpio_state[0+14]==1 && input_gpio_state[1+14]==1 && input_gpio_state[2+14]==1)  
		{			
			lane_infor.over_front_led_state=heiping;
				
		}
		else if(input_gpio_state[0+14]==0 && input_gpio_state[1+14]==1 && input_gpio_state[2+14]==1)  
		{			
			lane_infor.over_front_led_state=lvjian;
				
		}
		else if(input_gpio_state[0+14]==1 && input_gpio_state[1+14]==0 && input_gpio_state[2+14]==1)  
		{			
			lane_infor.over_front_led_state=hongcha;
				
		}
		else if(input_gpio_state[0+14]==0 && input_gpio_state[1+14]==0 && input_gpio_state[2+14]==1)  
		{			
			lane_infor.over_front_led_state=jiancha;
				
		}
		else if(input_gpio_state[0+14]==1 && input_gpio_state[1+14]==1 && input_gpio_state[2+14]==0)  
		{			
			lane_infor.over_front_led_state=zuozhuan;
				
		}
		else if(input_gpio_state[0+14]==0 && input_gpio_state[1+14]==1 && input_gpio_state[2+14]==0)  
		{			
			lane_infor.over_front_led_state=jianzhuan;
				
		}
		else if(input_gpio_state[0+14]==1 && input_gpio_state[1+14]==0 && input_gpio_state[2+14]==0)  
		{			
			lane_infor.over_front_led_state=chazhuan;
				
		}
		else if(input_gpio_state[0+14]==0 && input_gpio_state[1+14]==0 && input_gpio_state[2+14]==0)  
		{			
			lane_infor.over_front_led_state=jianchazhuan;
				
		}

		/*over_lane_back_led*/
		if(input_gpio_state[3+14]==1 && input_gpio_state[4+14]==1 && input_gpio_state[5+14]==1)  
		{			
			lane_infor.over_back_led_state=heiping;
				
		}
		else if(input_gpio_state[3+14]==0 && input_gpio_state[4+14]==1 && input_gpio_state[5+14]==1)  
		{			
			lane_infor.over_back_led_state=lvjian;
				
		}
		else if(input_gpio_state[3+14]==1 && input_gpio_state[4+14]==0 && input_gpio_state[5+14]==1)  
		{			
			lane_infor.over_back_led_state=hongcha;
				
		}
		else if(input_gpio_state[3+14]==0 && input_gpio_state[4+14]==0 && input_gpio_state[5+14]==1)  
		{			
			lane_infor.over_back_led_state=jiancha;
				
		}
		else if(input_gpio_state[3+14]==1 && input_gpio_state[4+14]==1 && input_gpio_state[5+14]==0)  
		{			
			lane_infor.over_back_led_state=zuozhuan;
				
		}
		else if(input_gpio_state[3+14]==0 && input_gpio_state[4+14]==1 && input_gpio_state[5+14]==0)  
		{			
			lane_infor.over_back_led_state=jianzhuan;
				
		}
		else if(input_gpio_state[3+14]==1 && input_gpio_state[4+14]==0 && input_gpio_state[5+14]==0)  
		{			
			lane_infor.over_back_led_state=chazhuan;
				
		}
		else if(input_gpio_state[3+14]==0 && input_gpio_state[4+14]==0 && input_gpio_state[5+14]==0)  
		{			
			lane_infor.over_back_led_state=jianchazhuan;
				
		}
		//电源监测
		if(input_gpio_state[6]==1)
		{
			ditc_infor.ext_power_error=1;
		}
		else
		{
			ditc_infor.ext_power_error=0;
			
		}
		//防雷监测
		if(input_gpio_state[13]==1)
		{
			ditc_infor.lightning_protect_error=1;
			
		}
		else
		{
			ditc_infor.lightning_protect_error=0;
		
		}
		
		/**
			判断输入状态是否有变
			若有变则改变标志位的状态执行信号量程序
		**/
		for(i=0;i<28;i++)
		{
			if(gpio_state_tmp[i]!=gpio_state[i])
			{
				gpio_state_tmp[i]=gpio_state[i];
				gpio_state_chage_flag=1;
				
			}		
		}
		
		if(gpio_state_chage_flag==1)
		{
			printf("---gpio state change---\n");
			sem_post(&gInputGPIO_sem);
			gpio_state_chage_flag=0;
		}
#endif					
	}

}



