
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <linux/watchdog.h>
#include "debug.h"

static int wdg_fd=-1;
const char* wdg_path="/dev/watchdog";

int open_wdg(void)
{
	if(wdg_fd <= 0)
	{
		wdg_fd = open(wdg_path,O_RDONLY);
		if(wdg_fd < 0)
		{
			perror(wdg_path);
			wdg_fd = -1;
			return -1;
		}

		return wdg_fd;
	}

	return wdg_fd;
}



void close_wdg(void)
{
	if(wdg_fd > 0)
		close(wdg_fd);
	wdg_fd = -1;
}

//feed watchdog
void feed_wdg(void)
{
	if(wdg_fd > 0)
		ioctl(wdg_fd,WDIOC_KEEPALIVE,NULL);
}


//set watchdog timeout
int set_wdg_timeout(int sec)
{
	int timeout;
	
	timeout = sec;
	if(ioctl(wdg_fd,WDIOC_SETTIMEOUT,&timeout)<0)
	{
		return -1;
	}
	
	timeout = 0;
	if(ioctl(wdg_fd,WDIOC_GETTIMEOUT,&timeout)<0)
	{
		return -1;
	}

	if(sec != timeout)
	{
		return -1;
	}
	return 0;
}


int watchdog_running(int timeout)
{
	int cnt=0;
	
	if(open_wdg()<0)
	{
		print_error("watchdog open fail\n");
		return -1;
	}

	if(timeout <= 1)
	{
		timeout = 2;
	}

	if(set_wdg_timeout(timeout)<0)
	{
		print_error("watchdog set timeout fail\n");
		return -2;
	}

	while(1)
	{
		feed_wdg();
		sleep(1);
		cnt++;
		if(cnt > 10)
		{
			print_infor("watchdog is running\n");
			cnt = 0;
		}
	}
}
/*-----------------------------------------------------*/
//end file

