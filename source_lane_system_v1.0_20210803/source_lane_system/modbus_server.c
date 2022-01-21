
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include "modbus_server.h"
#include "config.h"
#include "global.h"

unsigned short BITS_ADDRESS = 0;
unsigned short BITS_NB = 0;
unsigned char BITS_TAB[] = { 0xCD, 0x6B, 0xB2, 0x0E, 0x1B };

unsigned short INPUT_BITS_ADDRESS = 0;
unsigned short INPUT_BITS_NB = 0;
unsigned char INPUT_BITS_TAB[] = { 0xAC, 0xDB, 0x35 };

unsigned short REGISTERS_ADDRESS = 0x00;
unsigned short REGISTERS_NB = 200;
unsigned short REGISTERS_NB_MAX = 200;
unsigned short REGISTERS_TAB[200] = { 0x00 };

unsigned short INPUT_REGISTERS_ADDRESS = 0x00;
unsigned short INPUT_REGISTERS_NB = 301;
unsigned short INPUT_REGISTERS_TAB[301] = {0x00};


modbus_mapping_t *mb_mapping;

sem_t gModbus_ctrl_lane_sem;

#if 0
int modbus_tcp_server(void)
{
 	int s = -1;
	int i;
	int rc;
	modbus_t *ctx;    
	
	uint8_t *query;
	int header_length;

	unsigned short tmp=0;

	if(sem_init(&gModbus_ctrl_lane_sem,0,0)<0)

	{
		return -1;
	}
	
	ctx = modbus_new_tcp("0.0.0.0", 502);        
	query = malloc(MODBUS_TCP_MAX_ADU_LENGTH);

	header_length = modbus_get_header_length(ctx);    
	 //modbus_set_debug(ctx, TRUE);

	 
   	 mb_mapping = modbus_mapping_new_start_address(
        BITS_ADDRESS, BITS_NB,
        INPUT_BITS_ADDRESS, INPUT_BITS_NB,
        REGISTERS_ADDRESS, REGISTERS_NB_MAX,
        INPUT_REGISTERS_ADDRESS, INPUT_REGISTERS_NB);

	if (mb_mapping == NULL) 
	{
        	fprintf(stderr, "Failed to allocate the mapping: %s\n",modbus_strerror(errno));
        	modbus_free(ctx);
        	//return -1;
        }
	else
	{
		for (i=0; i < REGISTERS_NB; i++) 
		{
        		mb_mapping->tab_registers[i] = 0;
		}

		for (i=0; i < INPUT_REGISTERS_NB; i++) 
		{
        		mb_mapping->tab_input_registers[i] = 0;
		}

	}
	s = modbus_tcp_listen(ctx, 1);		

	while(1)
	{
		ditc_infor.network_connect_state=1;
		
		 modbus_tcp_accept(ctx, &s);

		 ditc_infor.network_connect_state=0;

		 for(;;)
		 {
			do {rc = modbus_receive(ctx, query);            /* Filtered queries return 0 */        
			} while (rc == 0);
			
			
			if(rc>1)
			{
				if (query[header_length] == 0x06) 
				{
					if(ditc_infor.local_enable==0)
					{
						if(query[header_length+2]==0x01) 
						{
							tmp=(query[header_length+3]<<8) | (query[header_length+4]);
							if(tmp>4 || tmp<0)
							{	
								ditc_infor.slow_ctrl_value_error=1;
							}
							else
							{
								ditc_infor.slow_ctrl_value_error=0;
								mb_mapping->tab_registers[0]=tmp;
								lane_infor.lane_select_switch=1;
								//printf("------------------------------mb_mapping->tab_registers[0]=0x%x\n",tmp);
								sem_post(&gModbus_ctrl_lane_sem);
							}

						}
						else if(query[header_length+2]==0x02) 
						{
							tmp=query[header_length+3]<<8|query[header_length+4];
							if(tmp>4 || tmp<0)
							{	
								ditc_infor.main_ctrl_value_error=1;
							}
							else
							{
								ditc_infor.main_ctrl_value_error=0;
								mb_mapping->tab_registers[1]=tmp;
								lane_infor.lane_select_switch=2;
								//printf("------------------------------mb_mapping->tab_registers[1]=0x%x\n",tmp);
								sem_post(&gModbus_ctrl_lane_sem);
							}

						}
						else if(query[header_length+2]==0x03) 
						{
							tmp=query[header_length+3]<<8|query[header_length+4];
							if(tmp>4 || tmp<0)
							{	
								ditc_infor.over_ctrl_value_error=1;
							}
							else
							{
								ditc_infor.over_ctrl_value_error=0;
								mb_mapping->tab_registers[2]=tmp;
								lane_infor.lane_select_switch=3;
								//printf("------------------------------mb_mapping->tab_registers[2]=0x%x\n",tmp);
								sem_post(&gModbus_ctrl_lane_sem);
							}
						}
						
					}
					
				}
				
				modbus_reply(ctx, query, rc, mb_mapping);
			}	
			else
			{
				modbus_close(ctx);
				break;
			}		
	    	}	
	}
}
#endif


int modbus_tcp_server(void)
{
 	int server_socket= -1;
	int i;
	int rc;
	modbus_t *ctx;    
	
	uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
	int header_length;

	int master_socket;
	fd_set refset;    
	fd_set rdset;
	int fdmax;

	unsigned short tmp=0;
	unsigned char adc_k[4]={0};
	float float_k;

	if(sem_init(&gModbus_ctrl_lane_sem,0,0)<0)

	{
		return -1;
	}
	
	ctx = modbus_new_tcp("0.0.0.0", 502);        
	
	header_length = modbus_get_header_length(ctx);    
	 //modbus_set_debug(ctx, TRUE);

	 
   	 mb_mapping = modbus_mapping_new_start_address(
        BITS_ADDRESS, BITS_NB,
        INPUT_BITS_ADDRESS, INPUT_BITS_NB,
        REGISTERS_ADDRESS, REGISTERS_NB_MAX,
        INPUT_REGISTERS_ADDRESS, INPUT_REGISTERS_NB);

	if (mb_mapping == NULL) 
	{
        	fprintf(stderr, "Failed to allocate the mapping: %s\n",modbus_strerror(errno));
        	modbus_free(ctx);
        	//return -1;
        }
	else
	{
		for (i=0; i < REGISTERS_NB; i++) 
		{
        		mb_mapping->tab_registers[i] = 0;
		}

		for (i=0; i < INPUT_REGISTERS_NB; i++) 
		{
        		mb_mapping->tab_input_registers[i] = 0;
		}

	}
	server_socket = modbus_tcp_listen(ctx, 10);	
	if(server_socket==-1)
	{
		printf("Unable to listen TCP connection\n");

	}

	/* Clear the reference set of socket */    
	FD_ZERO(&refset);    
	/* Add the server socket */    
	FD_SET(server_socket, &refset);    
	/* Keep track of the max file descriptor */    
	fdmax = server_socket;

	ditc_infor.network_connect_state=1;

	while(1)
	{		
		rdset = refset;
		if (select(fdmax+1, &rdset, NULL, NULL, NULL) == -1) 
		{            
			printf("Server select() failure.\n");                   
		}
		ditc_infor.network_connect_state=0;

		for (master_socket = 0; master_socket <= fdmax; master_socket++)
		{
			if (!FD_ISSET(master_socket, &rdset)) 
			{                
				continue;            
			}
			
			if (master_socket == server_socket) 
			{                
				/* A client is asking a new connection */               
				socklen_t addrlen;                
				struct sockaddr_in clientaddr;                
				int newfd;                
				/* Handle new connections */                
				addrlen = sizeof(clientaddr);                
				memset(&clientaddr, 0, sizeof(clientaddr));                
				newfd = accept(server_socket, (struct sockaddr *)&clientaddr, &addrlen);                
				if (newfd == -1) 
				{                   
					printf("Server accept() error\n");                
				} 
				else 
				{                    
					FD_SET(newfd, &refset);                   
					if (newfd > fdmax) 
					{                        
						/* Keep track of the maximum */                        
						fdmax = newfd;                   
					}                    
					printf("New connection from %s:%d on socket %d\n",  inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port, newfd);                
				}            
			} 
			else 
			{
				modbus_set_socket(ctx, master_socket);                
				rc = modbus_receive(ctx, query);

				if(rc>0)
				{
					if (query[header_length] == 0x06) 
					{
						if(ditc_infor.local_enable==0)
						{
							if(query[header_length+2]==REG_SLOW_CTRL_RW)  //	//Tx:000030-0A 23 00 00 00 06 01 06 00 01 00 03
							{
								tmp=(query[header_length+3]<<8) | (query[header_length+8]);
								if(tmp>8 || tmp<0)
								{	
									ditc_infor.slow_ctrl_value_error=1;
								}
								else
								{
									ditc_infor.slow_ctrl_value_error=0;
									mb_mapping->tab_registers[1]=tmp;
									lane_infor.lane_select_switch=1;
									//printf("------------------------------mb_mapping->tab_registers[1]=0x%x\n",tmp);
									sem_post(&gModbus_ctrl_lane_sem);	//使其线程不阻塞 增强信号量
								}
							}
							else if(query[header_length+2]==REG_MAIN_CTRL_RW)   //Tx:000052-0A E3 00 00 00 06 01 06 00 02 00 02
							{
								tmp=query[header_length+3]<<8|query[header_length+4];
								if(tmp>8 || tmp<0)
								{	
									ditc_infor.main_ctrl_value_error=1;
								}
								else
								{
									ditc_infor.main_ctrl_value_error=0;
									mb_mapping->tab_registers[2]=tmp;
									lane_infor.lane_select_switch=2;
									//printf("------------------------------mb_mapping->tab_registers[2]=0x%x\n",tmp);
									sem_post(&gModbus_ctrl_lane_sem);
								}
							}
							else if(query[header_length+2]==REG_OVER_CTRL_RW)  //Tx:000082-0B A2 00 00 00 06 01 06 00 03 00 04
							{
								tmp=query[header_length+3]<<8|query[header_length+4];
								if(tmp>8 || tmp<0)
								{	
									ditc_infor.over_ctrl_value_error=1;
								}
								else
								{
									ditc_infor.over_ctrl_value_error=0;
									mb_mapping->tab_registers[3]=tmp;
									lane_infor.lane_select_switch=3;
									//printf("------------------------------mb_mapping->tab_registers[3]=0x%x\n",tmp);
									sem_post(&gModbus_ctrl_lane_sem);
								}
							}	
						}
					}
					else if(query[header_length] == 0x10)   //Tx:000016-13 66 00 00 00 0B 01 10 00 14 00 02 04 E6 66 42 F6    float=123.45
					{
						if(query[header_length+2]==REG_ADC_1_K_L_RW)  
						{
							adc_k[0]=query[header_length+7];
							adc_k[1]=query[header_length+6];
							adc_k[2]=query[header_length+9];
							adc_k[3]=query[header_length+8];

							float_k=*(float *)adc_k;
							adc_device.sensor_1_k=float_k;
							sprintf(gSoft_infor.sensor_k1,"%f",float_k);
							WriteCfgItem(CFGNAME, "sensor_k1",gSoft_infor.sensor_k1,sizeof(gSoft_infor.sensor_k1));
						}
						else if(query[header_length+2]==REG_ADC_2_K_L_RW)  
						{
							adc_k[0]=query[header_length+7];
							adc_k[1]=query[header_length+6];
							adc_k[2]=query[header_length+9];
							adc_k[3]=query[header_length+8];

							float_k=*(float *)adc_k;
							adc_device.sensor_2_k=float_k;
							sprintf(gSoft_infor.sensor_k2,"%f",float_k);
							WriteCfgItem(CFGNAME, "sensor_k2",gSoft_infor.sensor_k2,sizeof(gSoft_infor.sensor_k2));
						}
						else if(query[header_length+2]==REG_ADC_3_K_L_RW)  
						{
							adc_k[0]=query[header_length+7];
							adc_k[1]=query[header_length+6];
							adc_k[2]=query[header_length+9];
							adc_k[3]=query[header_length+8];

							float_k=*(float *)adc_k;
							adc_device.sensor_3_k=float_k;
							sprintf(gSoft_infor.sensor_k3,"%f",float_k);
							WriteCfgItem(CFGNAME, "sensor_k3",gSoft_infor.sensor_k3,sizeof(gSoft_infor.sensor_k3));
						}
						else if(query[header_length+2]==REG_ADC_4_K_L_RW)  
						{
							adc_k[0]=query[header_length+7];
							adc_k[1]=query[header_length+6];
							adc_k[2]=query[header_length+9];
							adc_k[3]=query[header_length+8];

							float_k=*(float *)adc_k;
							adc_device.sensor_4_k=float_k;
							sprintf(gSoft_infor.sensor_k4,"%f",float_k);
							WriteCfgItem(CFGNAME, "sensor_k4",gSoft_infor.sensor_k4,sizeof(gSoft_infor.sensor_k4));
						}		

					}
					
					modbus_reply(ctx, query, rc, mb_mapping);
				}	
				else if(rc==-1)
				{
					/* This example server in ended on connection closing or any errors. */                    
					printf("Connection closed on socket %d\n", master_socket);                    
					close(master_socket);                    
					/* Remove from reference set */                    
					FD_CLR(master_socket, &refset);                    
					if (master_socket == fdmax) 
					{                        
						fdmax--;                    
					}
					ditc_infor.network_connect_state=1;
				}		
			}
		}
	}	
}


