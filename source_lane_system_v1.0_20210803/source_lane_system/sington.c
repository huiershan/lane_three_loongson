
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "sington.h"
#include "lockfile.h"
#include "debug.h"


#define LOCKFILE "/var/run/tiger5.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)


//设置程序单例运行模式
//在运行返回1，不再运行返回0
int IsSoftwareRunning(void)
{
	int fd;
	int ret;
	char buf[16];

	fd = open(LOCKFILE,O_RDWR|O_CREAT,LOCKMODE);
	
	if (fd < 0) {
		print_error("can't open %s\n",LOCKFILE);
		exit(1);
	}

	if (lockfile(fd) < 0) {
		if (errno == EACCES || errno == EAGAIN) {
			close(fd);
			return 1;
		}
		print_error("can't lock %s\n",LOCKFILE);
		exit(1);
	}
	
	//改变文件大小为0
	ret =ftruncate(fd, 0);
	if ( ret  < 0)
	{
	    print_error("can not truncate\n");
		exit(2);
	}

	sprintf(buf, "%ld", (long)getpid());
	ret = write(fd, buf, strlen(buf)+1);
	if ( ret < 0)
	{
	    print_error("can not write to file\n");
		exit(3);
	}

	return(0);
}


/*--------------------end file---------------------------*/

