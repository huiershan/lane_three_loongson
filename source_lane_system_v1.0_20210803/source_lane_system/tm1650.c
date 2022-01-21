#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <semaphore.h>
#include "tm1650.h"


#define I2C_RETRIES 0x0701	/* number of times a device address should
				   be polled when not acknowledging */
#define I2C_TIMEOUT 0x0702	/* set timeout in units of 10 ms */
/* NOTE: Slave address is 7 or 10 bits, but 10-bit addresses
 * are NOT supported! (due to code brokenness)
 */
#define I2C_SLAVE   0x0703	/* Use this slave address */
#define I2C_SLAVE_FORCE 0x0706	/* Use this slave address, even if it
				   is already in use by a driver! */
#define I2C_TENBIT  0x0704	/* 0 for 7 bit addrs, != 0 for 10 bit */
#define I2C_FUNCS   0x0705	/* Get the adapter functionality mask */
#define I2C_RDWR    0x0707	/* Combined R/W transfer (one STOP only) */
#define I2C_PEC     0x0708	/* != 0 to use PEC with SMBus */
#define I2C_SMBUS   0x0720	/* SMBus transfer */
#define I2C_M_TEN 0x0010
#define I2C_M_RD 0x0001

#define I2C_ADAPTER_PATH  "/dev/i2c-0"


unsigned char tm1650_addr[] = {0x34, 0x35, 0x36, 0x37};
unsigned char tm1650_tab[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x00};

unsigned char group1_led_state=0,group2_led_state=0,group3_led_state=0;	//三段led标志位

button_t pb1,pb2,pb3,pb4,pb5,pb6,pb7;	//7个按键类型

unsigned char gButtonPressType;			

sem_t gButtonPress_sem;

int tm1650_fd;
struct i2c_rdwr_ioctl_data *tm1650_data;

struct i2c_msg {
	unsigned short addr;
	unsigned short flags;
	unsigned short len;
	unsigned char *buf;
};

struct i2c_rdwr_ioctl_data {
	struct i2c_msg *msgs;
	int nmsgs;
};

static int tm1650_i2c_open()
{
	int fd;
	fd = open(I2C_ADAPTER_PATH, O_RDWR);
	if (fd < 0) {
		perror("open error");
		return -1;
	}

	return fd;
}

static int tm1650_i2c_config(int fd)
{
	if(ioctl(fd, I2C_TIMEOUT, 1) < 0) {
		perror("ioctl:set timeout failed");
		return -1;
	}
	
	if(ioctl(fd, I2C_RETRIES, 2) < 0) {
		perror("ioctl:set retry failed");
		return -1;
	}
	
	if(ioctl(fd, I2C_TENBIT, 0) < 0) {
		perror("ioctl:set addr:7bit failed");
		return -1;
	}
/*
	if (ioctl(fd,I2C_SLAVE_FORCE, 0x24) < 0) {
		perror("ioctl:set slave address failed");
		return -1;
	}
*/	
	return 1;
}
//tm1650显示初始化
static int tm1650_display_init(int fd, struct i2c_rdwr_ioctl_data *tm1650_data)
{
	int ret;
	unsigned char buf[4]={0};

	tm1650_data->nmsgs = 1;
	(tm1650_data->msgs[0]).len = 1;
	(tm1650_data->msgs[0]).addr = 0x24;
	(tm1650_data->msgs[0]).flags = 0;
	//(tm1650_data->msgs[0]).buf = (unsigned char *)malloc(1);
	(tm1650_data->msgs[0]).buf=buf;
	(tm1650_data->msgs[0]).buf[0] = 0x09;
	
	ret = ioctl(fd, I2C_RDWR, (unsigned long)tm1650_data);
	if (ret < 0) {
		perror("ioctl write display error");
		return -1;
	}
     
	//free(tm1650_data->msgs[0].buf);

	return 1;
}
//显示实现函数
static int tm1650_display_set(int fd, struct i2c_rdwr_ioctl_data *tm1650_data, unsigned char addr, unsigned char data,int cmd)

{
	int ret;
	unsigned char write_data=0;
	unsigned char buf[4]={0};

	if(cmd==1)
	{
		if(addr==0x34)//通过或运算进行处理只要进入这个程序找到对应灯的地址就能点亮一盏灯
		{
			if(data==0x01) group1_led_state=group1_led_state|0x01;
			else if(data==0x02) group1_led_state=group1_led_state|0x02;
			else if(data==0x04) group1_led_state=group1_led_state|0x04;
			else if(data==0x08) group1_led_state=group1_led_state|0x08;
			else if(data==0x10) group1_led_state=group1_led_state|0x10;
			else if(data==0x20) group1_led_state=group1_led_state|0x20;
			else if(data==0x40) group1_led_state=group1_led_state|0x40;
			//else if(data==0xff) group1_led_state=group1_led_state|0xff;
	  	 }

		else if(addr==0x35)
		{
			if(data==0x01) group2_led_state=group2_led_state|0x01;
			else if(data==0x02) group2_led_state=group2_led_state|0x02;
			else if(data==0x04) group2_led_state=group2_led_state|0x04;
			else if(data==0x08) group2_led_state=group2_led_state|0x08;
			else if(data==0x10) group2_led_state=group2_led_state|0x10;
			else if(data==0x20) group2_led_state=group2_led_state|0x20;
			else if(data==0x40) group2_led_state=group2_led_state|0x40;
			//else if(data==0xff) group2_led_state=group1_led_state|0xff;
	   	}

		else if(addr==0x36)
		{
			if(data==0x01) group3_led_state=group3_led_state|0x01;
			else if(data==0x02) group3_led_state=group3_led_state|0x02;
			//else if(data==0x04) group3_led_state=group3_led_state|0x04;
			//else if(data==0x08) group3_led_state=group3_led_state|0x08;
			//else if(data==0x10) group3_led_state=group3_led_state|0x10;
			//else if(data==0x20) group3_led_state=group3_led_state|0x20;
			//else if(data==0x40) group3_led_state=group3_led_state|0x40;
			//else if(data==0xff) group3_led_state=group1_led_state|0xff;
		}

	}

	if(cmd==0)
	{
		if(addr==0x34)//通过与运算进行处理只要进入这个程序找到对应灯的地址不让灯亮
		{
			if(data==0x01) group1_led_state=group1_led_state & 0xfe;
			else if(data==0x02) group1_led_state=group1_led_state & 0xfd;
			else if(data==0x04) group1_led_state=group1_led_state & 0xfb;
			else if(data==0x08) group1_led_state=group1_led_state & 0xf7;
			else if(data==0x10) group1_led_state=group1_led_state & 0xef;
			else if(data==0x20) group1_led_state=group1_led_state & 0xdf;
			else if(data==0x40) group1_led_state=group1_led_state & 0xbf;
			//else if(data==0xff) group1_led_state=group1_led_state & 0x00;
	  	 }

		else if(addr==0x35)
		{
			if(data==0x01) group2_led_state=group2_led_state & 0xfe;
			else if(data==0x02) group2_led_state=group2_led_state & 0xfd;
			else if(data==0x04) group2_led_state=group2_led_state & 0xfb;
			else if(data==0x08) group2_led_state=group2_led_state & 0xf7;
			else if(data==0x10) group2_led_state=group2_led_state & 0xef;
			else if(data==0x20) group2_led_state=group2_led_state & 0xdf;
			else if(data==0x40) group2_led_state=group2_led_state & 0xbf;
			//else if(data==0xff) group2_led_state=group2_led_state & 0x00;
	  	 }

		else if(addr==0x36)
		{
			if(data==0x01) group3_led_state=group3_led_state & 0xfe;
			else if(data==0x02) group3_led_state=group3_led_state & 0xfd;
			//else if(data==0x04) group3_led_state=group3_led_state & 0xfb;
			//else if(data==0x08) group3_led_state=group3_led_state & 0xf7;
			//else if(data==0x10) group3_led_state=group3_led_state & 0xef;
			//else if(data==0x20) group3_led_state=group3_led_state & 0xdf;
			//else if(data==0x40) group3_led_state=group3_led_state & 0xbf;
			//else if(data==0xff) group3_led_state=group3_led_state & 0x00;
	  	 }

	}
	

	if(addr==0x34) write_data=group1_led_state;
	else if(addr==0x35) write_data=group2_led_state;
	else if(addr==0x36) write_data=group3_led_state;

	if(data==0) write_data=0;

	tm1650_data->nmsgs = 1;
	(tm1650_data->msgs[0]).len = 1;
       (tm1650_data->msgs[0]).addr = addr;
       (tm1650_data->msgs[0]).flags = 0;
      // (tm1650_data->msgs[0]).buf = (unsigned char *)malloc(1);
	 (tm1650_data->msgs[0]).buf = buf;
       (tm1650_data->msgs[0]).buf[0] = write_data;

       ret = ioctl(fd, I2C_RDWR, (unsigned long)tm1650_data);
       if (ret < 0) {
               perror("ioctl write display error");
               return -1;
       }

	//free(tm1650_data->msgs[0].buf);
	return 1;
}
//按键扫描函数
static int tm1650_scan_key(int fd, struct i2c_rdwr_ioctl_data* tm1650_data, unsigned char *data)
{
        int ret;
	unsigned char buf[4]={0};

        tm1650_data->nmsgs = 1;
        (tm1650_data->msgs[0]).len = 1;
        (tm1650_data->msgs[0]).addr = 0x24;
        (tm1650_data->msgs[0]).flags = I2C_M_RD;
       // (tm1650_data->msgs[0]).buf = (unsigned char *)malloc(1);
	(tm1650_data->msgs[0]).buf = buf;
        (tm1650_data->msgs[0]).buf[0] = 0;
        ret = ioctl(fd, I2C_RDWR, (unsigned long)tm1650_data);
        if (ret < 0) {
                perror("ioctl read data error");
		return -1;
        }

	//printf("buff[0]=0x%x\n",(tm1650_data->msgs[0]).buf[0]);
	*data = tm1650_data->msgs[0].buf[0];
	
	//printf("data=0x%x\n", *data);
	//free(tm1650_data->msgs[0].buf);

	return 1;
}

//按键控制实现函数
button_type_t tm1650_key_get(int fd, struct i2c_rdwr_ioctl_data* tm1650_data, unsigned int us)
{

	int ret;
	unsigned char read_value=0;

	ret=tm1650_scan_key(tm1650_fd, tm1650_data, &read_value);
	
	if(ret>0)
	{
		//printf("read_value=0x%x\n",read_value);
		switch(read_value)
		{
			case local_enable:
				usleep(us);
				read_value=0;
				if(tm1650_scan_key(tm1650_fd, tm1650_data, &read_value)>0)
				{
					if(read_value==local_enable)
						return local_enable;
				}
				
			case lane_switch:
				usleep(us);
				read_value=0;
				if(tm1650_scan_key(tm1650_fd, tm1650_data, &read_value)>0)
				{
					if(read_value==lane_switch)
						return lane_switch;
				}
				
				
			case state_switch:
				usleep(us);
				read_value=0;
				if(tm1650_scan_key(tm1650_fd, tm1650_data, &read_value)>0)
				{
					if(read_value==state_switch)
						return state_switch;
				}
				
			case pass_ctrl:
				usleep(us);
				read_value=0;
				if(tm1650_scan_key(tm1650_fd, tm1650_data, &read_value)>0)
				{
					if(read_value==pass_ctrl)
						return pass_ctrl;
				}

			case forbid_ctrl:
				usleep(us);
				read_value=0;
				if(tm1650_scan_key(tm1650_fd, tm1650_data, &read_value)>0)
				{
					if(read_value==forbid_ctrl)
						return forbid_ctrl;
				}

			case reverse_ctrl:
				usleep(us);
				read_value=0;
				if(tm1650_scan_key(tm1650_fd, tm1650_data, &read_value)>0)
				{
					if(read_value==reverse_ctrl)
						return reverse_ctrl;
				}

			case turn_ctrl:
				usleep(us);
				read_value=0;
				if(tm1650_scan_key(tm1650_fd, tm1650_data, &read_value)>0)
				{
					if(read_value==turn_ctrl)
						return turn_ctrl;
				}
			default:
				return 0;

		}

	}

}


//运行指示HL1
int led_system_run_display(int cmd)
{
	int ret=0;
	
	ret=tm1650_display_set(tm1650_fd,tm1650_data,tm1650_addr[0],tm1650_tab[0],cmd);
	if(ret<0) return -1;

	return 1;
}

//通讯指示HL2
int led_network_connect_display(int cmd)
{
	int ret=0;
	
	ret=tm1650_display_set(tm1650_fd,tm1650_data,tm1650_addr[0],tm1650_tab[1],cmd);
	if(ret<0) return -1;

	return 1;
}

//故障指示HL3
int led_system_error_display(int cmd)
{
	int ret=0;
		
	ret=tm1650_display_set(tm1650_fd,tm1650_data,tm1650_addr[0],tm1650_tab[2],cmd);
	if(ret<0) return -1;

	return 1;
}
//串口tx 指示HL4
int led_uart_tx_display(int cmd)
{
	int ret=0;
	
	ret=tm1650_display_set(tm1650_fd,tm1650_data,tm1650_addr[0],tm1650_tab[3],cmd);
	if(ret<0) return -1;

	return 1;
}
//串口Rx 指示HL5
int led_uart_rx_display(int cmd)
{
	int ret=0;
	
	ret=tm1650_display_set(tm1650_fd,tm1650_data,tm1650_addr[0],tm1650_tab[4],cmd);
	if(ret<0) return -1;

	return 1;
}

//控制状态指示HL6
int led_control_state_select_display(int cmd)
{
	int ret=0;
	
	ret=tm1650_display_set(tm1650_fd,tm1650_data,tm1650_addr[0],tm1650_tab[5],cmd);
	if(ret<0) return -1;

	return 1;
}


//行车道选择指示HL7
int led_slow_lane_select_display(int cmd)
{
	int ret=0;
	
	ret=tm1650_display_set(tm1650_fd,tm1650_data,tm1650_addr[0],tm1650_tab[6],cmd);
	if(ret<0) return -1;

	return 1;
}

//主车道选择指示HL8
int led_main_lane_select_display(int cmd)
{
	int ret=0;
	
	ret=tm1650_display_set(tm1650_fd,tm1650_data,tm1650_addr[1],tm1650_tab[0],cmd);
	if(ret<0) return -1;

	return 1;
}

//超车道选择指示HL9
int led_over_lane_select_display(int cmd)
{
	int ret=0;
	
	ret=tm1650_display_set(tm1650_fd,tm1650_data,tm1650_addr[1],tm1650_tab[1],cmd);
	if(ret<0) return -1;

	return 1;
}



//正面箭反馈指示/通行控制指示HL11
int led_pass_and_front_go_display(int cmd)
{
	int ret=0;
	
	ret=tm1650_display_set(tm1650_fd,tm1650_data,tm1650_addr[1],tm1650_tab[3],cmd);
	if(ret<0) return -1;

	return 1;
}

//正面叉反馈指示/禁行控制指示HL12
int led_forbid_and_font_stop_display(int cmd)
{
	int ret=0;
	
	ret=tm1650_display_set(tm1650_fd,tm1650_data,tm1650_addr[1],tm1650_tab[4],cmd);
	if(ret<0) return -1;

	return 1;
}

//正面转向反馈指示/正面转向控制指示HL13
int led_turn_front_and_font_turn_display(int cmd)
{
	int ret=0;
	
	ret=tm1650_display_set(tm1650_fd,tm1650_data,tm1650_addr[1],tm1650_tab[5],cmd);
	if(ret<0) return -1;

	return 1;
}

// 反面箭头反馈指示/逆行控制指示HL14
int led_reverse_and_back_go_display(int cmd)
{
	int ret=0;
	
	ret=tm1650_display_set(tm1650_fd,tm1650_data,tm1650_addr[1],tm1650_tab[6],cmd);
	if(ret<0) return -1;

	return 1;
}

//反面叉反馈指示HL15
int led_back_stop_display(int cmd)
{
	int ret=0;
	
	ret=tm1650_display_set(tm1650_fd,tm1650_data,tm1650_addr[2],tm1650_tab[0],cmd);
	if(ret<0) return -1;

	return 1;
}

//反面转向反馈指示/反面转向控制指示HL16
int led_turn_back_and_back_turn_display(int cmd)
{
	int ret=0;
	
	ret=tm1650_display_set(tm1650_fd,tm1650_data,tm1650_addr[2],tm1650_tab[1],cmd);
	if(ret<0) return -1;

	return 1;
}

int lane_pass_led_on(void)
{
	led_pass_and_front_go_display(1);
	led_forbid_and_font_stop_display(0);
	led_reverse_and_back_go_display(0);
	led_turn_front_and_font_turn_display(0);
	led_turn_back_and_back_turn_display(0);
	led_back_stop_display(0);
	
	return 1;
}
//车道禁行灯
int lane_forbid_led_on(void)
{
	led_pass_and_front_go_display(0);
	led_forbid_and_font_stop_display(1);
	led_reverse_and_back_go_display(0);
	led_turn_front_and_font_turn_display(0);
	led_turn_back_and_back_turn_display(0);
	led_back_stop_display(0);
	
	return 1;
}
//车道逆行灯
int lane_reverse_led_on(void)
{
	led_pass_and_front_go_display(0);
	led_forbid_and_font_stop_display(0);
	led_reverse_and_back_go_display(1);
	led_turn_front_and_font_turn_display(0);
	led_turn_back_and_back_turn_display(0);
	led_back_stop_display(0);
	
	return 1;
}
//车道正转灯
int lane_turn_front_led_on(void)
{
	led_pass_and_front_go_display(0);
	led_forbid_and_font_stop_display(0);
	led_reverse_and_back_go_display(0);
	led_turn_front_and_font_turn_display(1);
	led_turn_back_and_back_turn_display(0);
	led_back_stop_display(0);
	
	return 1;
}
//车道反转灯
int lane_turn_back_led_on(void)
{
	led_pass_and_front_go_display(0);
	led_forbid_and_font_stop_display(0);
	led_reverse_and_back_go_display(0);
	led_turn_front_and_font_turn_display(0);
	led_turn_back_and_back_turn_display(1);
	led_back_stop_display(0);
	
	return 1;
}
//示例状态灯
int lane_pass_fornt_cha_led_on(void)
{
	led_pass_and_front_go_display(1);
	led_forbid_and_font_stop_display(0);
	led_reverse_and_back_go_display(1);
	led_turn_front_and_font_turn_display(0);
	led_turn_back_and_back_turn_display(1);
	led_back_stop_display(0);
	
	return 1;
}

int lane_all_ctrl_led_off(void)
{
	led_pass_and_front_go_display(0);
	led_forbid_and_font_stop_display(0);
	led_reverse_and_back_go_display(0);
	led_turn_front_and_font_turn_display(0);
	led_turn_back_and_back_turn_display(0);
	led_back_stop_display(0);
	
	return 1;
}


//tm1650初始化过程
int tm1650_init(void)
{
	int ret,i;
	
	if((tm1650_fd=tm1650_i2c_open()) < 0)
	{
		printf("tm1650 init fail,open i2c device error\n");
		return -1;
	}

	if((ret= tm1650_i2c_config(tm1650_fd)) < 0)
	{
		printf("tm1650 init fail,config i2c device error\n");
		return -1;
	}

	tm1650_data = (struct i2c_rdwr_ioctl_data* )malloc(sizeof(struct i2c_rdwr_ioctl_data));
	tm1650_data->msgs = (struct i2c_msg *)malloc(tm1650_data->nmsgs * sizeof(struct i2c_msg));

	ret=tm1650_display_init(tm1650_fd, tm1650_data);
	if(ret<0)
	{
		printf("tm1650 init fail,tm1650 display init error--\n");
		return -1;
	}

	
	/*clear dig1-dig4 reg 0*/
	for(i=0; i<4; i++)
	{
		ret=tm1650_display_set(tm1650_fd, tm1650_data, tm1650_addr[i], 0,0);
		if(ret < 0)
		{
			printf("tm1650 init fail, tm1650 display set error\n");
			return -1;
		}
	}

	led_system_run_display(1);
	led_slow_lane_select_display(1);
	
	return 1;
	
}

int led_and_button_init(void)
{
	if(tm1650_init()<0)
	{	
		return -1;
	}

	if(sem_init(&gButtonPress_sem,0,0)<0)

	{
		return -1;
	}

	return 1;
}
//按键检测
int button_press_check(void)
{
	int i;
	unsigned int time_count=0;
	button_type_t button_type;

	memset(&pb1,0,sizeof(button_t));
	memset(&pb2,0,sizeof(button_t));
	memset(&pb3,0,sizeof(button_t));
	memset(&pb4,0,sizeof(button_t));
	memset(&pb5,0,sizeof(button_t));
	memset(&pb6,0,sizeof(button_t));
	memset(&pb7,0,sizeof(button_t));

	while(1)
	{
		usleep(400000);
		button_type=tm1650_key_get(tm1650_fd, tm1650_data, 10000);
		
		switch(button_type)
		{
			//printf("------------button_type=0x%x\n",button_type);

			case local_enable:
				pb1.press_cnt.local_enable_cnt++;	//当本地使能按2次后本地时候能清0
				if(pb1.press_cnt.local_enable_cnt>=2) pb1.press_cnt.local_enable_cnt=0;
				pb1.button_type=local_enable;
				gButtonPressType=local_enable;
				sem_post(&gButtonPress_sem);
				break;

			case lane_switch:
				pb2.press_cnt.lane_switch_cnt++;
				if(pb2.press_cnt.lane_switch_cnt>=3) pb2.press_cnt.lane_switch_cnt=0;
				pb2.button_type=lane_switch;
				gButtonPressType=lane_switch;
				sem_post(&gButtonPress_sem);
				break;

			case state_switch:
				pb3.press_cnt.state_switch_cnt++;
				if(pb3.press_cnt.state_switch_cnt>=2) pb3.press_cnt.state_switch_cnt=0;
				pb3.button_type=state_switch;
				gButtonPressType=state_switch;
				sem_post(&gButtonPress_sem);
				break;

			case pass_ctrl:
				//pb4.press_cnt.pass_ctrl_cnt++;
				//if(pb4.press_cnt.pass_ctrl_cnt>=2) pb4.press_cnt.pass_ctrl_cnt=0;
				pb4.press_cnt.pass_ctrl_cnt=1;
				pb4.button_type=pass_ctrl;
				gButtonPressType=pass_ctrl;
				sem_post(&gButtonPress_sem);
				break;

			case forbid_ctrl:
				//pb5.press_cnt.forbid_ctrl_cnt++;
				//if(pb5.press_cnt.forbid_ctrl_cnt>=2) pb5.press_cnt.forbid_ctrl_cnt=0;
				pb5.press_cnt.forbid_ctrl_cnt=1;
				pb5.button_type=forbid_ctrl;
				gButtonPressType=forbid_ctrl;
				sem_post(&gButtonPress_sem);
				break;

			case reverse_ctrl:
				//pb6.press_cnt.reverse_ctrl_cnt++;
				//if(pb6.press_cnt.reverse_ctrl_cnt>=2) pb6.press_cnt.reverse_ctrl_cnt=0;
				pb6.press_cnt.reverse_ctrl_cnt=1;
				pb6.button_type=reverse_ctrl;
				gButtonPressType=reverse_ctrl;
				sem_post(&gButtonPress_sem);
				break;

			case turn_ctrl:

				while(tm1650_key_get(tm1650_fd, tm1650_data, 10000)==turn_ctrl)
				{
					time_count++;
					//printf("--time_count=%d\n",time_count);
				}	

				if(time_count>80)
				{
					//pb7.press_cnt.turn_ctrl_long_cnt++;
					pb7.press_cnt.turn_ctrl_long_cnt=1;
					pb7.press_type=long_press;
					time_count=0;
					//if(pb7.press_cnt.turn_ctrl_long_cnt>=2) pb7.press_cnt.turn_ctrl_long_cnt=0;
					pb7.button_type=turn_ctrl;
					gButtonPressType=turn_ctrl;

					printf("!!!!!!!!!!!long press\n");
					time_count=0;
					sem_post(&gButtonPress_sem);
					break;
				}
			
				else
				{
					//pb7.press_cnt.turn_ctrl_short_cnt;
					pb7.press_cnt.turn_ctrl_short_cnt=1;
					pb7.press_type=short_press;
					time_count=0;
					//if(pb7.press_cnt.turn_ctrl_short_cnt>=2) pb7.press_cnt.turn_ctrl_short_cnt=0;
					pb7.button_type=turn_ctrl;
					gButtonPressType=turn_ctrl;
					printf("!!!!!!!!!!!short press\n");
					time_count=0;
					sem_post(&gButtonPress_sem);
					break;
				}
				break;
				
			default:
				gButtonPressType=0;
				break;

		}
	}

}


