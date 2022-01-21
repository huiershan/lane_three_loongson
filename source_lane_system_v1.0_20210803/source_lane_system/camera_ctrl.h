

#ifndef _CAMERA_CTRL_H
#define _CMERA_CTRL_H


//camera client
int get_led_state_by_camera(char* pBuf,char* pAddr, int port);
//camera selfcheck
int camera_selfcheck(void);

#endif
/*----------------------end file-------------------------------*/

