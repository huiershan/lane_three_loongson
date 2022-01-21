
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "debug.h"
#include "global.h"
#include "button_ctrl.h"
#include "led_warnning_ctrl.h"
#include "product_infor.h"
#include "fixture_ctrl.h"
#include "barcode.h"
#include "sysutil_d.h"
#include "io_map.h"
#include "watchdog.h"


int watchdog(int timeout)
{
	if(open_wdg()<0)
	{
		print_error("watchdog open fail\n");
		return -1;
	}

	if(set_wdg_timeout(timeout)<0)
	{
		print_error("watchdog set timeout fail\n");
		return -1;
	}
	
	return 0;
}

//button check 
int button_running(int flag)
{
	char *p;
	struct timeval tmp,end;
	gstart_tim_flag=0;
	int stat;
	int time_cnt=0;

	int step = 0;

	gFixtureStat = 0;
	beep_ctrl(IO_OFF);
	
	watchdog(15);

	while(1)
	{
#if 0
		if(gDebugMode == 1)
		{
			sleep(1);
			continue;
		}
#endif
		time_cnt++;
		if(time_cnt>200)
		{
			printf("button thread running...\n");
			if(time_cnt > 300)
			{
				print_warn("time_cnt > 70,thread warnning\n");
				time_cnt = 0;
			}
		}
		
		feed_wdg();
#if 1
		sem_wait(&gFixStat_sem);
		stat = gFixtureStat;
		sem_post(&gFixStat_sem);

		if(flag < 0)
		{
			stat = -1;
			if(time_cnt>200)
				printf("fxiture selfcheck fail(%d)\n",flag);
		}

		if(stat == 0)
		{

			
			if(is_start_down() == 1)
			{
				if((0 == step)||(0 == gSoft_infor.bt_mode))
				{
					feed_wdg();
					beep_ctrl(IO_OFF);
					gettimeofday(&gstart_tim,NULL);
					gstart_tim_flag = 1;
					proc_infor_clean();
					printf("start button down\n");
			#if 1
					//scan get baecode
					p = GetBarcodeByScannerWithTimeout(21,gSoft_infor.scan_time);
					feed_wdg();
					if((p == NULL) || (p[0] =='\0'))
					{
						printf("can\'t get barcode,please check the scanner\n");
						gstart_tim_flag = 0;
						msleep_m(200);
						continue;
					}
					printf("scan barcode(%d) : %s\n",strlen(p),p);
					
					if(strlen(p) == 0)
					{
						printf("Product Error : Barcode buf is small !\n");
						gstart_tim_flag = 0;
						msleep_m(200);
						continue;
					}
    
					if(proc_barcode_load(p) == -1)
					{
						printf("Product Error : Barcode buf is small !\n");
						gstart_tim_flag = 0;
						msleep_m(200);
						continue;
					}
					//测试夹具与产品匹配测试
					//length 
					if(strlen(p) != gSoft_infor.barcode_size)
					{
						printf("product size isn't match fixture,read(%d)=size(%d)\n",strlen(p),gSoft_infor.barcode_size);
						//set led stat
						SetLedStat(LED_TYPE);
						//vacuum control
						vacuum_ctrl(0);
						gstart_tim_flag = 0;
						msleep_m(200);
						continue;
					}
					//产品辨识字符
					if(IsFixtureProductMatching(p[gSoft_infor.barcode_seat -1]) != 1)
					{
						printf("product type isn't match fixture\n");
						//set led stat
						SetLedStat(LED_TYPE);
						//vacuum control
						vacuum_ctrl(0);
						gstart_tim_flag = 0;
						msleep_m(200);
						continue;
					}
					else
					{
						if(0 != gSoft_infor.bt_mode)
						{
							step = 1;
							//set led stat
							SetLedStat(LED_READY);
						}
						else
							step = 0;
					}
    
			#endif
				}
				else if(3 == step)
				{
					step = 4;
				}
				else
				{
					msleep_m(200);
					continue;
				}
					

				if((0 != gSoft_infor.bt_mode)&&(4 != step))
					continue;
				
				step = 5;
				if(is_start_down() == 1)
				{
					msleep_m(10);
					feed_wdg();
					if(is_start_down() != 1)
					{
						step = 0;
						gstart_tim_flag = 0;
						msleep_m(200);
						continue;
					}

					feed_wdg();
				}
				else
				{
					step = 0;
					feed_wdg();
					gstart_tim_flag = 0;
					msleep_m(200);
					continue;
				}

				feed_wdg();
				gTestResult = 0;
				if(0 == is_grating_dark())
				{
					step = 0;
					continue;
				}
				printf("Fixture start....\n");
#if 1
				if(fixture_start() < 0)
				{
					step = 0;

					feed_wdg();
					printf("Fixture start fail\n");
					//set led stat
					SetLedStat(LED_WARNING);
					printf("wait gFixtStat sem...\n");
					sem_wait(&gFixStat_sem);
					gFixtureStat = 0;
					sem_post(&gFixStat_sem);
					printf("post gFixtStat sem...\n");
					//vacuum control
					vacuum_ctrl(0);
					gstart_tim_flag = 0;
					msleep_m(200);
					continue;
				}
#endif
				feed_wdg();
				printf("wait gFixtStat sem...\n");
				sem_wait(&gFixStat_sem);
				gFixtureStat = 1;
				sem_post(&gFixStat_sem);
				printf("post gFixtStat sem...\n");
				msleep_m(200);
				ipd_uut_power(1);
				gettimeofday(&end,NULL);
				printf("fixture ready use time: %ld ms\n", tim_subtrack(NULL,gstart_tim,end));

			}
			else
			{
				if(1 == step)
				{
					step = 2;
					sleep(1);
				}
				else if(2 == step)
				{
					//is grating dark
					if(is_grating_dark())
					{
						msleep_m(200);
						if(is_grating_dark())
							step = 3;
					}
				}
				usleep(100*1000);
			}
		}
		else if(stat > 0) 
		{
			if(time_cnt>200)
			{
				printf("fixture is testing...\n");
			}
		}

		if(5 == step)
		{

			if(FIX_UNLOCK == get_fixture_state())
			{
				step = 0;
			}
		}

		
#if 0
		if(gFixtureUse == 1)
		{
			usleep(1000*50);
			continue;	
		}
#endif
		msleep_m(20);
		feed_wdg();
		if(is_abort_down() == 1)
		{
			printf("Abort button down\n");
			SetLedStat(LED_FAIL);

			feed_wdg();
			printf("wait gFixtStat sem...\n");
			sem_wait(&gFixStat_sem);
			if(gFixtureStat == 1)
			{
				sem_post(&gFixStat_sem);
				printf("post gFixtStat sem...\n");
				beep_ctrl(IO_ON);
				printf("wait gTestIntrpt sem...\n");
				sem_wait(&gTestIntrpt_sem);
				gTestIntrpt++;
				sem_post(&gTestIntrpt_sem);
				printf("post gTestIntrpt sem...\n");
			}
			else
			{
				sem_post(&gFixStat_sem);
				printf("post gFixtStat sem...\n");
				beep_ctrl(IO_OFF);
			}

			gettimeofday(&tmp,NULL);

			printf("testdone reult %d\n",gTestResult);
			gTestResult = 0;
			feed_wdg();
			ipd_uut_power(0);
			feed_wdg();
			fixture_reset();
			feed_wdg();
			gettimeofday(&end,NULL);
			printf("fixture reset use time: %ld ms\n", tim_subtrack(NULL,tmp,end));
			printf("wait gFixtStat sem...\n");
			sem_wait(&gFixStat_sem);
			gFixtureStat = 0;
			sem_post(&gFixStat_sem);
			printf("post gFixtStat sem...\n");
			step = 0;
		}
		else
		{
			if(time_cnt>200)
			{
				printf("abort button isn't down...\n");
				time_cnt = 0;
			}
		
		}
		
		msleep_m(100);
#endif
	}
	return 0;
}

/*----------------------end file-------------------------------*/

