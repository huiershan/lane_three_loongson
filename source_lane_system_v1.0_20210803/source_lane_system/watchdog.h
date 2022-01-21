
#ifndef _WATCHDOG_H
#define _WATCHDOG_H

int open_wdg(void);
void close_wdg(void);
//feed watchdog
void feed_wdg(void);
//set watchdog timeout
int set_wdg_timeout(int sec);
//watchdog 
int watchdog_running(int time);

#endif
/*-----------------------------------------------------*/
//end file

