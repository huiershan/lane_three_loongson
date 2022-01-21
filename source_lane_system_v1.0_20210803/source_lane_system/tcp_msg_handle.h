
#ifndef _TCP_MSG_HANDLE_H
#define _TCP_MSG_HANDLE_H

/*--------------------------------------*/
#define MSG_MAX_LEN		64
typedef struct{
	char cmd[MSG_MAX_LEN];
	int (*handle)(int fd,char* buf);
} Msg_hander_t;

/*-----------------------------*/

#define kPayloadSize		1024
#define PacketSize			(kPayloadSize)
typedef struct{

	char payload[kPayloadSize];
} PACKET_t;


//#define MATCH_N(str1,str2) (strncmp(str1,str2,strlen(str2)) == 0)
#define MATCH_N(str1,str2) (strncmp_cmd(str1,str2,' ') == 0)
							
/*--------------------------------------*/
//msg command
//recive
#define MSG_ConVersion	"cversion?"
#define MSG_Version		"version?"
#define MSG_Help		"help"
#define MSG_Debug		"debug"
#define MSG_Led		       "led"
#define MSG_VoltMeter	 "voltage"
#define MSG_CurrentMeter	"current"
#define MSG_Light	"light"
#define MSG_Button	"button_read"
#define MSG_Start    "start"
#define MSG_End    "end"
#define MSG_Fail    "fail"

//send
#define REPLY_VER	 	"Version="
#define REPLY_SN	 	"SN="
#define REPLY_Volt		"VOLT: "
#define REPLY_CURR		"CURR: "
//ack
#define REPLY_ACK		"ACK_"
#define REPLY_DONE		"DONE"
#define REPLY_PASS		"PASS"
#define REPLY_FAIL		"FAIL"
#define REPLY_ERROR		"ERROR"



/*--------------------------------------*/

//客户端处理函数入口
int do_server(int fd);

#endif
/*-----------------------------------------------------*/

