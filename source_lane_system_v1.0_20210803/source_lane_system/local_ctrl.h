#ifndef _LOCAL_CTRL_H
#define _LOCAL_CTRL_H

/*----------*/

int local_output_ctrl(void);
int local_input_ctrl(void);
int network_connect_led(void);
int sys_fault_led(void);
int sys_running_led(void);
int input_output_loop_check(void);

#endif
/*-----------------------end file------------------------------*/