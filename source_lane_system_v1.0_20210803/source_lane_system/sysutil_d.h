
#ifndef _SYSUTIL_D_H
#define _SYSUTIL_D_H

/*----------------- io operation function --------------*/
/*-----read adn write io with timeout-------*/

//read io with timeout or character
int opio_readtv_ch(int fd, char* buf, int n, char ch, int sec, int ms);

//read io with non-blocking and timeout
int opio_readtv(int fd, char* buf, int n, int sec, int ms);
//write io with non-blocking and timeout
int opio_writetv(int fd, char* buf, int n, int sec, int ms);

//从命令行输入取第一个命令关键字进行比较，相等返回1，不等返回0
int strncmp_cmd(const char* cmd,const char* line,char ch);
long tim_subtrack(struct timeval *diff,struct timeval x,struct timeval y);

//ms delay
void msleep_m(int msec);

//crc16
unsigned short crc16table(unsigned char *ptr, unsigned char len);
#endif

/*------------------end file-------------------------*/


