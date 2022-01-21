
#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#define USEFUL 1
#define UNUSEFUL 0
#define SUCCESS 0
#define FAILED -1

#define RETURN_OK 0
#define RETURN_ERROR -1
#define RETURN_PARAERROR -2
#define RETURN_PARANOEXSIT -3


typedef struct CSocketItem
{
       unsigned long int ipAddress;
	int Socket_fd;    
	char flag;          //标识该socket句柄是否有效
}Socket_Item_t;


//TCP server 入口
int tcp_server(int port);


extern int gSocketUploadfd;


#endif
/*-----------------------end file--------------------------*/

