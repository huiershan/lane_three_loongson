
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "debug.h"
#include "tcp_server.h"
#include "tcp_msg_handle.h"
#include "global.h"


#include "plus_in.h"

int gSocketUploadfd;


#define MAX_CLIENT_COUNT  FD_SETSIZE   //MAX TCP client count
////øÕªß∂Àµÿ÷∑
struct sockaddr_in client_addr[MAX_CLIENT_COUNT]={};
//Ω” ‹«Î«ÛøÕªß∂À–≈œ¢≥ÿ∑
Socket_Item_t g_Socket_Item[MAX_CLIENT_COUNT];

//–ƒÃ¯∞¸…Ë÷√
int set_tcp_keepAlive(int fd, int start, int interval, int count,int flag)
{
    int keepAlive = flag;
    if (fd < 0 || start < 0 || interval < 0 || count < 0) return -1;
    //∆Ù”√–ƒÃ¯ª˙÷∆£¨»Áπ˚ƒ˙œÎπÿ±’£¨Ω´keepAlive÷√¡„º¥ø…
    if(setsockopt(fd,SOL_SOCKET,SO_KEEPALIVE,(void*)&keepAlive,sizeof(keepAlive)) == -1)
    {
        perror("setsockopt");
        return -1;
    }

	if(flag == 0)
		return 0;

	    ////∆Ù”√–ƒÃ¯ª˙÷∆ø™ ºµΩ ◊¥Œ–ƒÃ¯’Ï≤‚∞¸∑¢ÀÕ÷Æº‰µƒø’œ– ±º‰
    if(setsockopt(fd,SOL_TCP,TCP_KEEPIDLE,(void *)&start,sizeof(start)) == -1)
    {
	    perror("setsockopt");
    	return -1;
    }
    //¡Ω¥Œ–ƒÃ¯’Ï≤‚∞¸÷Æº‰µƒº‰∏Ù ±º‰
    if(setsockopt(fd,SOL_TCP,TCP_KEEPINTVL,(void *)&interval,sizeof(interval)) == -1)
    {
        perror("setsockopt");
        return -1;
    }
    //ÃΩ≤‚¥Œ ˝£¨º¥Ω´º∏¥ŒÃΩ≤‚ ß∞‹≈–∂®Œ™TCP∂œø™
    if(setsockopt(fd,SOL_TCP,TCP_KEEPCNT,(void *)&count,sizeof(count)) == -1)
    {
    	perror("setsockopt");
        return -1;
	}

	return 0;
}

//¥”øÕªß∂À–≈œ¢≥ÿ÷–…æ≥˝øÕªß∂À–≈œ¢
int delete_socket_item(int socket_fd)
{
    int i=0;
    for(;i<MAX_CLIENT_COUNT;i++)
    {
		if(g_Socket_Item[i].Socket_fd == socket_fd)
		{
		    g_Socket_Item[i].ipAddress= 0;
		    g_Socket_Item[i].Socket_fd= -1;
		    g_Socket_Item[i].flag=UNUSEFUL;
		    break;
		}
    }

    if(i==MAX_CLIENT_COUNT)
		return RETURN_ERROR;

    return i;
}

//ÃÌº”øÕªß∂À–≈œ¢µΩøÕªß∂À–≈œ¢≥ÿ÷–
int inset_socket_item(int socket_fd,  struct sockaddr_in sock_addr)
{
    int i=0;
    for(;i<MAX_CLIENT_COUNT;i++)
    {
		if(g_Socket_Item[i].flag==UNUSEFUL)
		{
		    g_Socket_Item[i].ipAddress=sock_addr.sin_addr.s_addr ;
		    g_Socket_Item[i].Socket_fd=socket_fd;
		    g_Socket_Item[i].flag=USEFUL;
		    break;
		}
    }

    if(i==MAX_CLIENT_COUNT)
	return RETURN_ERROR;

    return i;
}


//¥¥Ω®TCP∑˛ŒÒ∆˜Ã◊Ω”◊÷
int creatTcpServerSocket(int port)
{
	int listenfd,rc;
	struct sockaddr_in servaddr;

	listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd < 0)
	{
		print_error("scoket fail !\n");
		return -1;
	}
	
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port	=	htons(port);

	//…Ë÷√∂Àø⁄ø…÷ÿ”√
	int opt = 1;
	rc = setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(char*)&opt,sizeof(opt));
	if(rc < 0)
	{
		print_error("setsockopt fail !\n");
		close(listenfd);
		return -1;
	}

	rc = bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	if(rc < 0)
	{
		print_error("bind fail !\n");
		close(listenfd);
		return -1;
	}

	rc = listen(listenfd,MAX_CLIENT_COUNT);
	if(rc < 0)
	{
		print_error("listen fail !\n");
		close(listenfd);
		return -1;
	}

	return listenfd;

}


//TCP server »Îø⁄
int tcp_server(int port)
{
	int listenfd,connfd;
	socklen_t cliaddr_len;
	struct sockaddr_in cliaddr;

	int nready, client[FD_SETSIZE],cnetcnt=0;
	int i, maxi, maxfd, sockfd;
	fd_set rset,errset,allset;

	listenfd = creatTcpServerSocket(port);
	if(listenfd < 0)
		return -1;

	maxfd = listenfd;
	maxi = -1;
	for(i = 0; i< FD_SETSIZE; i++)
		client[i] = -1;
	
    FD_ZERO(&allset);
	FD_SET(listenfd,&allset);

	debug_printf("TCP Server is running\n");
	while(1)
	{
		rset = allset;
		errset = allset;

		//IO∂‡¬∑∏¥”√£¨rset∂¡º‡Ã˝£¨errset¥ÌŒÛº‡Ã˝
		nready = select(maxfd+1,&rset,NULL,&errset,NULL);
		if(nready < 0)
		{
			if(nready == -1 && errno == EINTR)
				continue;
			print_error("select error\n");
			while(1)
			{
				print_error("select error\n");
				sleep(1);
			}
			
		}
		
		//≈–∂œÀ˘∑Ò”–«Î«Û¡¥Ω” ¬º˛
		if(FD_ISSET(listenfd,&rset))
		{
			cliaddr_len = sizeof(cliaddr);
			connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&cliaddr_len);
			if(connfd < 0)
			{
				if(errno == EINTR)
					continue;
			}
			
			printf("accept : %s at PORT %d\n",inet_ntoa((struct in_addr)cliaddr.sin_addr),ntohs(cliaddr.sin_port));
			cnetcnt++;
			debug_printf("connect server client count: %d .\n",cnetcnt);
			if(inset_socket_item(connfd,cliaddr) == RETURN_ERROR)
			{
				while(1)
				{
					print_error("Tcp sever item buffer is full\n");
					sleep(1);
				}
			}
		
			for (i = 0; i < FD_SETSIZE; i++)
			{
				if (client[i] < 0) 
				{
					//sem_wait(&gFixUse_sem);
					//gFixtureUse = 1;
					//sem_post(&gFixUse_sem);

					client[i] = connfd; /* save descriptor */
					client_addr[i] = cliaddr;
					//
					if(set_tcp_keepAlive(connfd, 30,2,3,1) != 0)
					{
						print_error("set tcp keepAAlive fail!");
					}
					debug_printf("set tcp keepAAlive success!\n");
					break;
				}
			}

			if (i == FD_SETSIZE) 
			{
				print_error("clients over\n");
				while(1)
				{
					print_error("clients over\n");
					sleep(1);
				}
			}

		
			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd; /* for select */
			if (i > maxi)
				maxi = i;	/* max index in client[] array */

			if (--nready == 0)
				continue;	/* no more readable descriptors */
		
		}

		////Ω” ‹¥¶¿Ì«Î«Û
		for (i = 0; i <= maxi; i++)    /* check all clients for data */
		{	
			if ( (sockfd = client[i]) < 0)
				continue;
			
			//error fd check
			if(FD_ISSET(sockfd, &errset))
			{
				cnetcnt--;
				delete_socket_item(sockfd);
				client[i] = -1;
				FD_CLR(sockfd, &allset);
				close(sockfd);
				printf("break_error : %s at PORT %d\n",
						inet_ntoa((struct in_addr)client_addr[i].sin_addr),
						ntohs(client_addr[i].sin_port));
				debug_printf("connect server client count: %d .\n",cnetcnt);

			}
			else if (FD_ISSET(sockfd, &rset)) 
			{
				if(do_server(sockfd) == -1)
				{
					cnetcnt--;
					if(cnetcnt == 0)
					{
						//sem_wait(&gFixUse_sem);
						//gFixtureUse = 0;
						//sem_post(&gFixUse_sem);
					}
					delete_socket_item(sockfd);
					client[i] = -1;
					FD_CLR(sockfd ,&allset);
					close(sockfd);
					printf("break_away: %s at PORT %d\n",
							inet_ntoa((struct in_addr)client_addr[i].sin_addr),
							ntohs(client_addr[i].sin_port));
					debug_printf("connect server client count: %d .\n",cnetcnt);
				}

				if (--nready == 0)
					break;	/* no more readable descriptors */
			}
		}
	}//end while(1)

	return 0;
}



int do_upload(int fd)
{
	char buf[1024];
	int len;
	int ret;

	len = recv(fd,buf,sizeof(buf),0);
	if(len <= 0)
	{

		return -1;
	}
	
	buf[len]='\0';
	printf("recv(%d) : %s\n",len, buf);

	if(strcmp(buf,"start") == 0)
	{
		gSocketUploadfd = fd;
	}
	else if(strcmp(buf,"stop") == 0)
	{
		gSocketUploadfd = 0;
	}
	
	printf("gSocketUploadfd=%d\n",gSocketUploadfd);

	return 0;
}

//TCP server »Îø⁄
int upload_server(int port)
{
	int listenfd,connfd;
	socklen_t cliaddr_len;
	struct sockaddr_in cliaddr;

	int nready, client[FD_SETSIZE],cnetcnt=0;
	int i, maxi, maxfd, sockfd;
	fd_set rset,errset,allset;

	listenfd = creatTcpServerSocket(port);
	if(listenfd < 0)
		return -1;

	maxfd = listenfd;
	maxi = -1;
	for(i = 0; i< FD_SETSIZE; i++)
		client[i] = -1;
	
    FD_ZERO(&allset);
	FD_SET(listenfd,&allset);

	debug_printf("Upload Server is running\n");
	while(1)
	{
		rset = allset;
		errset = allset;

		//IO∂‡¬∑∏¥”√£¨rset∂¡º‡Ã˝£¨errset¥ÌŒÛº‡Ã˝
		nready = select(maxfd+1,&rset,NULL,&errset,NULL);
		if(nready < 0)
		{
			if(nready == -1 && errno == EINTR)
				continue;
			print_error("select error\n");
			while(1)
			{
				print_error("select error\n");
				sleep(1);
			}
			
		}
		
		//≈–∂œÀ˘∑Ò”–«Î«Û¡¥Ω” ¬º˛
		if(FD_ISSET(listenfd,&rset))
		{
			cliaddr_len = sizeof(cliaddr);
			connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&cliaddr_len);
			if(connfd < 0)
			{
				if(errno == EINTR)
					continue;
			}
			
			printf("accept : %s at PORT %d\n",inet_ntoa((struct in_addr)cliaddr.sin_addr),ntohs(cliaddr.sin_port));
			cnetcnt++;
			debug_printf("connect server client count: %d .\n",cnetcnt);
			if(inset_socket_item(connfd,cliaddr) == RETURN_ERROR)
			{
				while(1)
				{
					print_error("Tcp sever item buffer is full\n");
					sleep(1);
				}
			}
		
			for (i = 0; i < FD_SETSIZE; i++)
			{
				if (client[i] < 0) 
				{
					//sem_wait(&gFixUse_sem);
					//gFixtureUse = 1;
					//sem_post(&gFixUse_sem);

					client[i] = connfd; /* save descriptor */
					client_addr[i] = cliaddr;
					//
					if(set_tcp_keepAlive(connfd, 30,2,3,1) != 0)
					{
						print_error("set tcp keepAAlive fail!");
					}
					debug_printf("set tcp keepAAlive success!\n");
					break;
				}
			}

			if (i == FD_SETSIZE) 
			{
				print_error("clients over\n");
				while(1)
				{
					print_error("clients over\n");
					sleep(1);
				}
			}

		
			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd; /* for select */
			if (i > maxi)
				maxi = i;	/* max index in client[] array */

			if (--nready == 0)
				continue;	/* no more readable descriptors */
		
		}

		////Ω” ‹¥¶¿Ì«Î«Û
		for (i = 0; i <= maxi; i++)    /* check all clients for data */
		{	
			if ( (sockfd = client[i]) < 0)
				continue;
			
			//error fd check
			if(FD_ISSET(sockfd, &errset))
			{
				cnetcnt--;
				delete_socket_item(sockfd);
				client[i] = -1;
				FD_CLR(sockfd, &allset);
				close(sockfd);
				printf("break_error : %s at PORT %d\n",
						inet_ntoa((struct in_addr)client_addr[i].sin_addr),
						ntohs(client_addr[i].sin_port));
				debug_printf("connect server client count: %d .\n",cnetcnt);

			}
			else if (FD_ISSET(sockfd, &rset)) 
			{
				if(do_upload(sockfd) == -1)
				{
					cnetcnt--;
					if(cnetcnt == 0)
					{
						//sem_wait(&gFixUse_sem);
						//gFixtureUse = 0;
						//sem_post(&gFixUse_sem);
					}
					delete_socket_item(sockfd);
					client[i] = -1;
					FD_CLR(sockfd ,&allset);
					close(sockfd);
					printf("break_away: %s at PORT %d\n",
							inet_ntoa((struct in_addr)client_addr[i].sin_addr),
							ntohs(client_addr[i].sin_port));
					debug_printf("connect server client count: %d .\n",cnetcnt);
				}

				if (--nready == 0)
					break;	/* no more readable descriptors */
			}
		}
	}//end while(1)

	return 0;
}


int modbus_tcp_server(void)
{
 	int s = -1;
	modbus_t *ctx;    
	modbus_mapping_t *mb_mapping;
	uint8_t *query;
	int header_length;
	
	ctx = modbus_new_tcp("127.0.0.1", 502);        
	query = malloc(MODBUS_TCP_MAX_ADU_LENGTH);

	header_length = modbus_get_header_length(ctx);    
	 //modbus_set_debug(ctx, TRUE);

	 
	mb_mapping = modbus_mapping_new(0, 0,10, 120);
	if (mb_mapping == NULL)
	{
		fprintf(stderr, "Failed to allocate the mapping: %s\n",modbus_strerror(errno));
		modbus_free(ctx);
		//return -1;
	}

	mb_mapping->tab_registers[0] = 1;
	mb_mapping->tab_registers[1] = 2;

	s = modbus_tcp_listen(ctx, 1);  
	modbus_tcp_accept(ctx, &s);
	
}	
	

/*---------------------end file--------------------------*/

