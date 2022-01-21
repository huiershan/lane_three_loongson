
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "module_ctrl.h"
#include "fixture_ctrl.h"
#include "io_map.h"
#include "led_warnning_ctrl.h"
#include "debug.h"
#include "global.h"
#include <sys/time.h>
#include <time.h>
#include "sysutil_d.h"


//product type
volatile ProductType_t gFixtureType = p_ERROR;
struct timeval gstart_tim;
volatile int gstart_tim_flag=0;

//等待延时sec
#define MAX_COUNT_DELAY		(5000/100)
/*--------------------------------------------*/
//wait place sensor
static int WaitPositionSensor(int addr, int port,int stat)
{
	int ret,rel;
	int count;

	printf("wait io module ack.\n");
	count = MAX_COUNT_DELAY;
	while(1)
	{
		rel = get_io(addr,port);
		printf("wait sensor get io %d %d %d\n",addr,port,rel);
		if(rel == stat)
		{
			ret = 0;
			break;
		}
		usleep(120*1000);
		count++;
		if(count > MAX_COUNT_DELAY*2)
		{
			printf("io module wait fail\n");
			print_can_msg_cnt();
			SetLedStat(LED_WARNING);
			return -2;	
		}
	}

	return ret;
}


//advance
int uut_advance(void)
{
	int ret;

	SetLedStat(LED_RUNNING);
	usleep(10*1000);
	ret = set_io(BACK_ADDR,BACK_PORT,IO_OFF);
	if(ret == -1)
	{
		//set io fail
		print_error(" get set UUT back port off result fail\n");
	//	return -1;
	}
	usleep(10*1000);
	ret = set_io(ADVANCE_ADDR,ADVANCE_PORT,IO_ON);
	if(ret == -1)
	{
		//set io fail
		print_error(" get set uut advance port on result fail\n");
	//	return -1;
	}

	usleep(20*1000);
	ret = WaitPositionSensor(ADVANCE_SENSOR_ADDR,ADVANCE_SENSOR_PORT,0);

	usleep(50*1000);
#if 1
	if(ret < 0)
	{
		return -1;
	}
#endif
	return 0;
}

//back
int uut_back(void)
{
	int ret;

	ret = set_io(ADVANCE_ADDR,ADVANCE_PORT,IO_OFF);
	if(ret == -1)
	{
		//set io fail
		print_error(" get set uut advance port off result fail\n");
	//	return -1;
	}
	usleep(10*1000);
	ret = set_io(BACK_ADDR,BACK_PORT,IO_ON);
	if(ret == -1)
	{
		//set io fail
		print_error(" get set uut back port on result fail\n");
	//	return -1;
	}

	usleep(20*1000);
	ret = WaitPositionSensor(BACK_SENSOR_ADDR,BACK_SENSOR_PORT,0);

	usleep(50*1000);
#if 1
	if(ret < 0)
	{
		return -1;
	}
#endif
	return 0;
}

//up
int probe_block_up(void)
{
	int ret;

	ret = set_io(DOWN_ADDR,DOWN_PORT,IO_OFF);
	if(ret == -1)
	{
		//set io fail
		print_error(" get set probe block down port off result fail\n");
	//	return -1;
	}
	usleep(10*1000);
	ret = set_io(UP_ADDR,UP_PORT,IO_ON);
	if(ret == -1)
	{
		//set io fail
		print_error(" get set probe block  up port on result fail\n");
	//	return -1;
	}

	usleep(20*1000);
	ret = WaitPositionSensor(UP_SENSOR_ADDR,UP_SENSOR_PORT,0);
	usleep(50*1000);

#if 1
	if(ret < 0)
	{
		return -1;
	}
#endif
	return 0;
}

//down
int probe_block_down(void)
{
	int ret;

	usleep(10*1000);
	ret = set_io(UP_ADDR,UP_PORT,IO_OFF);
	if(ret == -1)
	{
		//set io fail
		print_error(" get set probe block up  port off result fail\n");
	//	return -1;
	}
	usleep(10*1000);
	ret = set_io(DOWN_ADDR,DOWN_PORT,IO_ON);
	if(ret == -1)
	{
		//set io fail
		print_error(" get set probe block down port on result fail\n");
	//	return -1;
	}

	usleep(20*1000);
	ret = WaitPositionSensor(DOWN_SENSOR_ADDR,DOWN_SENSOR_PORT,0);
	usleep(50*1000);

#if 1
	if(ret < 0)
	{
		return -1;
	}
#endif
	return 0;
}
/*-------------------------------------*/

//power
//IPD and UUT power
#if 0
//on : ipd 3v3 -> ipd 5v ->uut 3v3 -> uut 5v
//off uut 3v3 -> uut 5v -> ipd 5v -> ipd 3v3
int ipd_uut_power(int cmd)
{

	if(cmd >= 1)
	{
		cmd = IO_ON;
	}
	else
	{
		cmd = IO_OFF;
	}

	if(cmd == IO_ON)
	{
		if(set_io(P4_IPD_3V3_ADDR,P4_IPD_3V3_PORT,cmd) == -1)
		{
			print_error("ipd 3V3 ON fail\n");
			return -3;
		}
//		usleep(1000*200);

		if(set_io(P3_IPD_5V_ADDR,P3_IPD_5V_PORT,cmd) == -1)
		{
			print_error("ipd 5V ON fail\n");
			return -1;
		}
//		usleep(1000*200);
		if(set_io(P1_UUT_3V3_ADDR,P1_UUT_3V3_PORT,cmd) == -1)
		{
			print_error("uut 3V3 ON fail\n");
			return -4;
		}

//		usleep(1000*200);
		if(set_io(P2_UUT_5V_ADDR,P2_UUT_5V_PORT,cmd) == -1)
		{
			print_error("uut 5V ON fail\n");
			return -2;
		}
		printf("UUT and IPD power is open !\n");
	}
	else
	{
		if(set_io(P1_UUT_3V3_ADDR,P1_UUT_3V3_PORT,cmd) == -1)
		{
			print_error("uut 3V3 off fail\n");
			return -4;
		}
//		usleep(1000*200);
		if(set_io(P2_UUT_5V_ADDR,P2_UUT_5V_PORT,cmd) == -1)
		{
			print_error("uut 5V off fail\n");
			return -2;
		}
//		usleep(1000*200);
		if(set_io(P3_IPD_5V_ADDR,P3_IPD_5V_PORT,cmd) == -1)
		{
			print_error("ipd 5V off fail\n");
			return -1;
		}
//		usleep(1000*200);
		if(set_io(P4_IPD_3V3_ADDR,P4_IPD_3V3_PORT,cmd) == -1)
		{
			print_error("ipd 3V3 off fail\n");
			return -3;
		}
		
		printf("UUT and IPD power is close !\n");
	}
	return 0;
}
#else
//on : uut 3v3 -> ipd 33v ->10ms->uut 5v -> ipd 5v
//off ipd 5v -> uut 5v -> uut 3v3 -> uut 3v3
int ipd_uut_power(int cmd)
{

	if(cmd >= 1)
	{
		cmd = IO_ON;
	}
	else
	{
		cmd = IO_OFF;
	}
	if(cmd == IO_ON)
	{
	
		if(set_io(UUT_3V3_5V_ADDR,UUT_3V3_5V_PORT,cmd) == -1)
		{
			print_error("uut 3V3 5V ON fail\n");
			return -4;
		}

		usleep(10*1000);

		if(set_io(P4_IPD_3V3_ADDR,P4_IPD_3V3_PORT,cmd) == -1)
		{
			print_error("ipd 3V3 ON fail\n");
			return -3;
		}
		usleep(10*1000);


		if(set_io(P3_IPD_5V_ADDR,P3_IPD_5V_PORT,cmd) == -1)
		{
			print_error("ipd 5V ON fail\n");
			return -1;
		}
		
		printf("UUT and IPD power is open !\n");
		sleep(1);
//		usleep(500*1000);
	}
	else
	{
		if(set_io(P3_IPD_5V_ADDR,P3_IPD_5V_PORT,cmd) == -1)
		{
			print_error("ipd 5V off fail\n");
			return -1;
		}

		if(set_io(UUT_3V3_5V_ADDR,UUT_3V3_5V_PORT,cmd) == -1)
		{
			print_error("uut 3V3 5V off fail\n");
			return -2;
		}

		usleep(10*1000);

		if(set_io(P4_IPD_3V3_ADDR,P4_IPD_3V3_PORT,cmd) == -1)
		{
			print_error("ipd 3V3 off fail\n");
			return -3;
		}
		
		printf("UUT and IPD power is close !\n");
		usleep(10*1000);
	}
#if 0
//tiger 4
	if(cmd == IO_ON)
	{
	
		if(set_io(P1_UUT_3V3_ADDR,P1_UUT_3V3_PORT,cmd) == -1)
		{
			print_error("uut 3V3 ON fail\n");
			return -4;
		}

		usleep(10*1000);

		if(set_io(P4_IPD_3V3_ADDR,P4_IPD_3V3_PORT,cmd) == -1)
		{
			print_error("ipd 3V3 ON fail\n");
			return -3;
		}
		usleep(10*1000);

		if(set_io(P2_UUT_5V_ADDR,P2_UUT_5V_PORT,cmd) == -1)
		{
			print_error("uut 5V ON fail\n");
			return -2;
		}

		if(set_io(P3_IPD_5V_ADDR,P3_IPD_5V_PORT,cmd) == -1)
		{
			print_error("ipd 5V ON fail\n");
			return -1;
		}
		
		printf("UUT and IPD power is open !\n");
		sleep(1);
//		usleep(500*1000);
	}
	else
	{
		if(set_io(P3_IPD_5V_ADDR,P3_IPD_5V_PORT,cmd) == -1)
		{
			print_error("ipd 5V off fail\n");
			return -1;
		}

		if(set_io(P2_UUT_5V_ADDR,P2_UUT_5V_PORT,cmd) == -1)
		{
			print_error("uut 5V off fail\n");
			return -2;
		}

		usleep(10*1000);

		if(set_io(P4_IPD_3V3_ADDR,P4_IPD_3V3_PORT,cmd) == -1)
		{
			print_error("ipd 3V3 off fail\n");
			return -3;
		}

		usleep(10*1000);

		if(set_io(P1_UUT_3V3_ADDR,P1_UUT_3V3_PORT,cmd) == -1)
		{
			print_error("uut 3V3 off fail\n");
			return -4;
		}
		
		printf("UUT and IPD power is close !\n");
		usleep(10*1000);
	}
#endif
	//usb_hub_ctrl(cmd);

	return 0;
}

#endif

#if 1
//beep control
int beep_ctrl(int cmd)
{
	if(cmd >= 1)
	{
		cmd = IO_ON;
	}
	else
	{
		cmd = IO_OFF;
	}

	set_io(BEEP_ADDR,BEEP_PORT,cmd);
	usleep(20*1000);
	if(set_io(BEEP_ADDR,BEEP_PORT,cmd) == -1)
	{
		print_error("BEEP control fail\n");
		return -4;
	}
	printf("beep control %d\n",cmd);
	return 0;
}
#endif
/*-------------------------------------*/


/*-------------------------------------*/

//fixture start
int fixture_start(void)
{
	int ret=0;

	printf("wait gFixtStat sem...\n");
	sem_wait(&gFixStat_sem);
	gFixtureStat = 1;
	sem_post(&gFixStat_sem);
	printf("post gFixtStat sem...\n");
	printf("vacuum ctrl 1\n");
	//vacuum control
	ret = vacuum_ctrl(1);
	if(ret == -1)
	{
		printf("vacuum ctrl fail\n");
		printf("wait gFixtStat sem...\n");
		sem_wait(&gFixStat_sem);
		gFixtureStat = 0;
		sem_post(&gFixStat_sem);
		printf("post gFixtStat sem...\n");
		return -1;
	}

	usleep(10*1000);
	ret = get_io(UP_SENSOR_ADDR,UP_SENSOR_PORT);
	if(ret != 0)
	{
		printf("up sensor not light = %d\n",ret);
		printf("wait gFixtStat sem...\n");
		sem_wait(&gFixStat_sem);
		gFixtureStat = 0;
		sem_post(&gFixStat_sem);
		printf("post gFixtStat sem...\n");
		return -1;
	}

	usleep(10*1000);
	ret = get_io(BACK_SENSOR_ADDR,BACK_SENSOR_PORT);
	if( ret != 0)
	{
		printf("back sensor not light = %d\n",ret);
		printf("wait gFixtStat sem...\n");
		sem_wait(&gFixStat_sem);
		gFixtureStat = 0;
		sem_post(&gFixStat_sem);
		printf("post gFixtStat sem...\n");
		return -1;
	}
	printf("uut advance...\n");
	usleep(10*1000);
	ret = uut_advance();
	if(ret < 0)
	{
		printf("start : uut advance fail\n");
		return -1;
	}

	printf("probe block down...\n");
//	usleep(200*1000);
	msleep_m(500);
	ret = probe_block_down();
	if(ret < 0)
	{
		printf("start : probe block down fail\n");
		return -1;
	}

	//usleep(50*1000);
	msleep_m(500);
	printf("vacuum ctrl 0\n");
	//vacuum control
	ret = vacuum_ctrl(0);
	usleep(50*1000);
	if(ret == -1)
	{
		return -1;
	}
	return 0;
}

//fixture reset
int fixture_reset(void)
{
	int ret = 0;
	struct timeval end;
	
	usleep(100*1000);
	ret = get_io(UP_SENSOR_ADDR,UP_SENSOR_PORT);
	if( ret == 1)
	{
		probe_block_up();
	}
	else if(ret < 0)
	{
		printf("up sensor rio fail\n");
		return -1;
	}
	else
	{
		printf("up sensor light\n");
	}

	//usleep(500*1000);
	msleep_m(500);

	ret = get_io(BACK_SENSOR_ADDR,BACK_SENSOR_PORT);
	if( ret != 0)
	{
		uut_back();
	}
	else if(ret < 0)
	{
		printf("back sensor rio fail\n");
		return -1;
	}
	else
	{
		printf("back sensor light\n");
	}

	msleep_m(100);
	printf("wait gFixtStat sem...\n");
	sem_wait(&gFixStat_sem);
	gFixtureStat = 0;
	sem_post(&gFixStat_sem);
	printf("post gFixtStat sem...\n");
	//vacuum control
	ret = vacuum_ctrl(0);

	gettimeofday(&end,NULL);
	if(gstart_tim_flag == 1)
	{
		printf("ALL test use time: %ld ms\n", tim_subtrack(NULL,gstart_tim,end));
		gstart_tim_flag = 0;
	}
	if(ret == -1)
	{
		return -1;
	}

	return ret;
}

/*-------------------------------------*/

//vacuum control
int vacuum_ctrl(int cmd)
{
	if(cmd >= 1)
	{
		cmd = IO_ON;
	}
	else
	{
		cmd = IO_OFF;
	}
		
	if(set_io(VACUUM_CONTROL_ADDR,VACUUM_CONTROL_PORT,cmd) == -1)
	{
		return -1;
	}

	return 0;
}
/*-------------------------------------*/

//Button
//abort button
int is_abort_down(void)
{
	if(get_io(ABORT_BTN_ADDR,ABORT_BTN_PORT) == 0)
	{
		debug_printf("Abort button is down !\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

//start button
int is_start_down(void)
{
	if(get_io(START_BTN_ADDR,START_BTN_PORT) == 0)
	{
		debug_printf("Start button is down !\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

/*-------------------------------------------------*/
//grating safty
int is_grating_dark(void)
{
	int flag = 0;

	if(get_io(BACK_SENSOR_ADDR,BACK_SENSOR_PORT) == 0)
	{
		flag = 1;
	}
	else
	{
		return 0;
	}

	if(get_io(GRATING_ADDR,GRATING_PORT) == 0)
	{
		flag |= 2;
		return flag;
	}
	else
		return 0;
}

//
int get_grating_sensor_state(void)
{

	if(get_io(GRATING_ADDR,GRATING_PORT) == 0)
		return 1;
	else
		return 0;

}

//get uut and probe state
FixtureState_t get_fixture_state(void)
{
	FixtureState_t probe,uut;
	probe = get_probe_state();
	uut = get_uut_state();

	if((FIX_PROBE_DOWN==probe)&&(FIX_UUT_IN == uut))
	{
		return FIX_LOCK;
	}
	else if((FIX_PROBE_UP==probe)&&(FIX_UUT_OUT == uut))
	{
		return FIX_UNLOCK;
	}
	else if((FIX_ERROR == probe)||(FIX_ERROR == uut))
	{
		return FIX_ERROR;
	}
	else
		return FIX_NONE;

}

//get probe block state
FixtureState_t get_probe_state(void)
{
	int down=-1,up=-1;

	msleep_m(5);
	down = get_io(DOWN_SENSOR_ADDR,DOWN_SENSOR_PORT);
	msleep_m(10);
	up = get_io(UP_SENSOR_ADDR,UP_SENSOR_PORT);
	sem_wait(&gFixmd_sem);
	gFixmd.down = down;
	gFixmd.up = up;
	sem_post(&gFixmd_sem);
//	print_infor("Plate state down=%d,up=%d\n",down,up);

	if((0==down)&&(1==up))
	{
		return FIX_PROBE_DOWN;
	}
	else if((0==up)&&(1==down))
	{
		return FIX_PROBE_UP;
	}
	else
	{
		return FIX_ERROR;
	}
}


//get UUT state
FixtureState_t get_uut_state(void)
{
	int in=-1,out=-1;

	msleep_m(5);
	in = get_io(ADVANCE_SENSOR_ADDR,ADVANCE_SENSOR_PORT);
	msleep_m(10);
	out = get_io(BACK_SENSOR_ADDR,BACK_SENSOR_PORT);

	sem_wait(&gFixmd_sem);
	gFixmd.in = in;
	gFixmd.out = out;
	sem_post(&gFixmd_sem);
//	print_infor("UUT state in=%d,out=%d\n",in,out);
	if((0==in)&&(1==out))
	{
		return FIX_UUT_IN;
	}
	else if((0==out)&&(1==in))
	{
		return FIX_UUT_OUT;
	}
	else
	{
		return FIX_ERROR;
	}
}

//获取夹具类型
ProductType_t ReadFixtureType(void)
{
	int ret = 0;
	int tmp;
	char type[4]={'E','J','I','A'};

	tmp = get_io(P_ANSI_ADDR, P_ANSI_PORT);
	if(0 == tmp)
	{
		ret = 1;
	}
	else if(tmp < 0)
	{
		ret = 0;
	}
	
	tmp = get_io(P_JIS_ADDR, P_JIS_PORT);
	if(0 == tmp)
	{
		ret |= 2;
	}
	else if(tmp < 0)
	{
		ret = 0;
	}
#if 0
	if(get_io(P_ISO_ADDR, P_ISO_PORT) == 0)
	{
		ret |= 4;
	}
#endif

	printf("fixture type = %d, %c\n",ret,type[ret]);

	sem_wait(&gFixmd_sem);
	gFixmd.type = type[ret];
	sem_post(&gFixmd_sem);
	if(3 == ret)
	{
		return p_ANSI;
	}
	else if(1 == ret)
	{
		return p_JIS;
	}
	else if(2 == ret)
	{
		return p_ISO;
	}
	else
		return p_ERROR;
}


/*------------------------------------end file-------------------------------------------------*/

