
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include "adc7606.h"

#include "modbus_server.h"

int adc_fd1,adc_fd2,adc_fd3,adc_fd4,adc_scale_fd;

adc_device_t adc_device;


/*
*function：ftoc(float a)
*decription:  浮点数转化成四个字节
*input:
*output:
*/
#if 1
void float_to_byte(float a,unsigned char *buff)//例如12.5--0x41 48 00 00;转换完之后，e[0]-00，e[1]-00，e[2]-48，e[3]-41
{
	unsigned char *px;
	unsigned char i;
	unsigned char x[4],e[4];                        /*定义字符数组，准备存储浮点数的四个字节*/
	void          *pf;
	px=buff;                                      /*px指针指向数组x*/
	pf=&a;                                     /*void 型指针指向浮点数首地址*/
	for(i=0;i<4;i++) 	
	{
		*(px+i)=*((char *)pf+i); /*强制void 型指针转成char型,因为void型指针不能运算*/
		//printf("x[%d]=0x%x\n",i,buff[i]);
	}
}
#endif

#if 1
/*
*function：float ByteToFloat(unsigned char* byteArray)
*decription:  将字节型转化成32bits浮点型
*input:长度为4的字节数组
*output:
*/
float ByteToFloat(unsigned char* byteArray)
{
	return *((float*)byteArray);
}

#endif

int adc_scale_init(void)			
{

	unsigned char scale[8];

	memset(scale,0,sizeof(scale));
	
	adc_scale_fd=open(ADC_SCALE_PATH,O_RDONLY);
	if(adc_scale_fd==-1)
	{
		printf("open adc_scale_path error\n");   
		return -1;
	}
	else
	{
		if(read(adc_scale_fd,scale,8) == -1)
		{
			printf("read adc_scale value error\n");
			return -1;
		}
		//printf("--scale = %s\n",scale);

		adc_device.scale=atof(scale);
		printf("scale_value=%f\n",adc_device.scale);

	}
	close(adc_scale_fd);
	
	return 0;
}

int adc_channel_init(void)
{
	int ret=0;
	
	adc_fd1=open(ADC1_PATH,O_RDONLY);	
	if(adc_fd1==-1)
	{
		printf("open adc1 error\n");
		ret=-1;
	}

	adc_fd2=open(ADC2_PATH,O_RDONLY);	
	if(adc_fd2==-1)
	{
		printf("open adc2 error\n");
		ret=-1;
	}

	adc_fd3=open(ADC3_PATH,O_RDONLY);	
	if(adc_fd3==-1)
	{
		printf("open adc3 error\n");
		ret=-1;
	}

	adc_fd4=open(ADC4_PATH,O_RDONLY);	
	if(adc_fd4==-1)
	{
		printf("open adc4 error\n");
		ret=-1;
	}

	close(adc_fd1);
	close(adc_fd2);
	close(adc_fd3);
	close(adc_fd4);


	return ret;

}

int adc_value_read(int channel, int *value)
{
	char adc_value[8];
	int fd;
	char file[64];
	
	memset(file,0,sizeof(file));
	if(channel==1) strcpy(file,ADC1_PATH);
	else if(channel==2) strcpy(file,ADC2_PATH);
	else if(channel==3) strcpy(file,ADC3_PATH);
	else if(channel==4) strcpy(file,ADC4_PATH);
	//printf("file=%s\n",file);
	
	memset(adc_value,0,sizeof(adc_value));

	fd=open(file,O_RDONLY);

	if(fd==-1)
	{
		printf("open adc[%d] error\n",channel);
		*value=0;
		close(fd);
		return -1;
	}

	if(read(fd,adc_value,8) == -1)
	{
		printf("read adc[%d] value error\n",channel);
		*value=0;
		close(fd);
		return -1;
	}
	else
	{
		//printf("---adc[%d]_value=%s\n",channel,adc_value);
		*value=atoi(adc_value);
		//printf("value[%d]=%d\n",channel,*value);
		
		close(fd);
		return 0;
	}

}

int adc_current_get(int channel)
{
	int raw_volt_mv=0;
	int ret;
	float current_raw,current;
	char tmp[8]={0};
	
	
	ret=adc_value_read(channel,&raw_volt_mv);

	if(ret==0)
	{
		//printf("raw_volt_mv[%d]=%d\n",channel,raw_volt_mv);
		current_raw=(raw_volt_mv*adc_device.scale)/200;
		sprintf(tmp,"%.2f",current_raw);
		current=atof(tmp);

		if(channel==1) adc_device.adc1_current= current;
		else if(channel==2) adc_device.adc2_current= current;
		else if(channel==3) adc_device.adc3_current= current;
		else if(channel==4) adc_device.adc4_current= current;

		//printf("adc[%d] current=%f\n",channel,current);
	}
	else
	{
		printf("get adc[%d] current fail\n",channel);
		return -1;
	}

	return 0;

}


int adc_value_get(void)
{

	int value=0;
	int tmp=0;
	unsigned char buff[4]={0};

	float_to_byte(adc_device.sensor_1_k, buff);
	mb_mapping->tab_registers[REG_ADC_1_K_L_RW]=buff[1]<<8 |buff[0];
	mb_mapping->tab_registers[REG_ADC_1_K_H_RW]=buff[3]<<8 |buff[2];

	float_to_byte(adc_device.sensor_2_k, buff);
	mb_mapping->tab_registers[REG_ADC_2_K_L_RW]=buff[1]<<8 |buff[0];
	mb_mapping->tab_registers[REG_ADC_2_K_H_RW]=buff[3]<<8 |buff[2];

	float_to_byte(adc_device.sensor_3_k, buff);
	mb_mapping->tab_registers[REG_ADC_3_K_L_RW]=buff[1]<<8 |buff[0];
	mb_mapping->tab_registers[REG_ADC_3_K_H_RW]=buff[3]<<8 |buff[2];

	float_to_byte(adc_device.sensor_4_k, buff);
	mb_mapping->tab_registers[REG_ADC_4_K_L_RW]=buff[1]<<8 |buff[0];
	mb_mapping->tab_registers[REG_ADC_4_K_H_RW]=buff[3]<<8 |buff[2];


	while(1)
	{
		usleep(2000000);
	#if 1

		//ADC1 channel
		if(adc_current_get(1) < 0)
		{
			adc_device.sensor_1_error=3;
			adc_device.adc1_current=0;
			adc_device.sensor_1_value=0;
		}
		else
		{	
			tmp=(int)(adc_device.adc1_current);
			if(tmp>=1 && tmp<4)
			{
				adc_device.sensor_1_error=2;	
				adc_device.sensor_1_value=0;
			}
			else if(tmp<1)
			{
				adc_device.sensor_1_error=1;	
			}
			else
			{
				adc_device.sensor_1_error=0;	
				adc_device.sensor_1_value=adc_device.sensor_1_k*(adc_device.adc1_current-4);
			}
		}

		float_to_byte(adc_device.adc1_current,buff);
		mb_mapping->tab_input_registers[REG_ADC_1_CURRENT_L_R]=buff[1]<<8 |buff[0];
		mb_mapping->tab_input_registers[REG_ADC_1_CURRENT_H_R]=buff[3]<<8 |buff[2];

		float_to_byte(adc_device.sensor_1_value,buff);
		mb_mapping->tab_input_registers[REG_ADC_1_DEV_VAULE_L_R]=buff[1]<<8 |buff[0];
		mb_mapping->tab_input_registers[REG_ADC_1_DEV_VAULE_H_R]=buff[3]<<8 |buff[2];

		mb_mapping->tab_input_registers[REG_ADC_1_ERROR_R]=adc_device.sensor_1_error;

		//ADC2 channel
		if(adc_current_get(2) < 0)
		{
			adc_device.sensor_2_error=3;
			adc_device.adc2_current=0;
			adc_device.sensor_2_value=0;
		}
		else
		{	
			tmp=(int)(adc_device.adc2_current);
			if(tmp>=1 && tmp<4)
			{
				adc_device.sensor_2_error=2;	
				adc_device.sensor_2_value=0;
			}
			else if(tmp<1)
			{
				adc_device.sensor_2_error=1;	
			}
			else
			{
				adc_device.sensor_2_error=0;	
				adc_device.sensor_2_value=adc_device.sensor_2_k*(adc_device.adc2_current-4);
			}
		}

		float_to_byte(adc_device.adc2_current,buff);
		mb_mapping->tab_input_registers[REG_ADC_2_CURRENT_L_R]=buff[1]<<8 |buff[0];
		mb_mapping->tab_input_registers[REG_ADC_2_CURRENT_H_R]=buff[3]<<8 |buff[2];

		float_to_byte(adc_device.sensor_2_value,buff);
		mb_mapping->tab_input_registers[REG_ADC_2_DEV_VAULE_L_R]=buff[1]<<8 |buff[0];
		mb_mapping->tab_input_registers[REG_ADC_2_DEV_VAULE_H_R]=buff[3]<<8 |buff[2];

		mb_mapping->tab_input_registers[REG_ADC_2_ERROR_R]=adc_device.sensor_2_error;

		//ADC3 channel
		if(adc_current_get(3) < 0)
		{
			adc_device.sensor_3_error=3;
			adc_device.adc3_current=0;
			adc_device.sensor_3_value=0;
		}
		else
		{	
			tmp=(int)(adc_device.adc3_current);
			if(tmp>=1 && tmp<4)
			{
				adc_device.sensor_3_error=2;	
				adc_device.sensor_3_value=0;
			}
			else if(tmp<1)
			{
				adc_device.sensor_3_error=1;	
			}
			else
			{
				adc_device.sensor_3_error=0;	
				adc_device.sensor_3_value=adc_device.sensor_3_k*(adc_device.adc3_current-4);
			}
		}

		float_to_byte(adc_device.adc3_current,buff);
		mb_mapping->tab_input_registers[REG_ADC_3_CURRENT_L_R]=buff[1]<<8 |buff[0];
		mb_mapping->tab_input_registers[REG_ADC_3_CURRENT_H_R]=buff[3]<<8 |buff[2];

		float_to_byte(adc_device.sensor_3_value,buff);
		mb_mapping->tab_input_registers[REG_ADC_3_DEV_VAULE_L_R]=buff[1]<<8 |buff[0];
		mb_mapping->tab_input_registers[REG_ADC_3_DEV_VAULE_H_R]=buff[3]<<8 |buff[2];

		mb_mapping->tab_input_registers[REG_ADC_3_ERROR_R]=adc_device.sensor_3_error;

		//ADC4 channel
		if(adc_current_get(4) < 0)
		{
			adc_device.sensor_4_error=3;
			adc_device.adc4_current=0;
			adc_device.sensor_4_value=0;
		}
		else
		{	
			tmp=(int)(adc_device.adc4_current);
			if(tmp>=1 && tmp<4)
			{
				adc_device.sensor_4_error=2;	
				adc_device.sensor_4_value=0;
			}
			else if(tmp<1)
			{
				adc_device.sensor_4_error=1;	
			}
			else
			{
				adc_device.sensor_4_error=0;	
				adc_device.sensor_4_value=adc_device.sensor_4_k*(adc_device.adc4_current-4);
			}
		}

		float_to_byte(adc_device.adc4_current,buff);
		mb_mapping->tab_input_registers[REG_ADC_4_CURRENT_L_R]=buff[1]<<8 |buff[0];
		mb_mapping->tab_input_registers[REG_ADC_4_CURRENT_H_R]=buff[3]<<8 |buff[2];
		//printf("mb_mapping->tab_input_registers[REG_ADC_4_CURRENT_L_R]=0x%x\n",mb_mapping->tab_input_registers[REG_ADC_4_CURRENT_L_R]);
		//printf("mb_mapping->tab_input_registers[REG_ADC_4_CURRENT_H_R]=0x%x\n",mb_mapping->tab_input_registers[REG_ADC_4_CURRENT_H_R]);

		float_to_byte(adc_device.sensor_4_value,buff);
		mb_mapping->tab_input_registers[REG_ADC_4_DEV_VAULE_L_R]=buff[1]<<8 |buff[0];
		mb_mapping->tab_input_registers[REG_ADC_4_DEV_VAULE_H_R]=buff[3]<<8 |buff[2];

		mb_mapping->tab_input_registers[REG_ADC_4_ERROR_R]=adc_device.sensor_4_error;
	#endif 
	}
}

