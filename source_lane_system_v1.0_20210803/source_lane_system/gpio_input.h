#ifndef _GPIO_INPUT_H
#define _GPIO_INPUT_H

#include <semaphore.h>

#define SYSFS_GPIO_PATH           	"/sys/class/gpio/export"  				//gpio¬∑æ∂

#define INPUT1							"180"								/*The first input port*/
#define INPUT2							"181"
#define INPUT3							"182"
#define INPUT4							"183"
#define INPUT5							"184"
#define INPUT6							"185"
#define INPUT7							"186"

#define INPUT8							"187"								/*The second input port*/
#define INPUT9							"188"
#define INPUT10							"189"
#define INPUT11							"190"
#define INPUT12							"191"
#define INPUT13							"192"
#define INPUT14							"193"

#define INPUT15							"194"								/*The third input port*/
#define INPUT16							"195"
#define INPUT17							"196"
#define INPUT18							"197"
#define INPUT19							"198"
#define INPUT20							"199"
#define INPUT21							"200"

#define INPUT22 							"201"								/*The fourth input port*/
#define INPUT23							"202"
#define INPUT24							"203"
#define INPUT25							"204"
#define INPUT26							"205"
#define INPUT27							"206"
#define INPUT28							"207"								//gpio”≥…‰µƒŒÔ¿Ìµÿ÷∑

#define INPUT1_GPIO_DIR          	"/sys/class/gpio/gpio180/direction"		/*The frist input port*/
#define INPUT2_GPIO_DIR          	"/sys/class/gpio/gpio181/direction"
#define INPUT3_GPIO_DIR          	"/sys/class/gpio/gpio182/direction"
#define INPUT4_GPIO_DIR          	"/sys/class/gpio/gpio183/direction"
#define INPUT5_GPIO_DIR          	"/sys/class/gpio/gpio184/direction"
#define INPUT6_GPIO_DIR          	"/sys/class/gpio/gpio185/direction"
#define INPUT7_GPIO_DIR          	"/sys/class/gpio/gpio186/direction"

#define INPUT8_GPIO_DIR          	"/sys/class/gpio/gpio187/direction"		/*The second input port*/
#define INPUT9_GPIO_DIR          	"/sys/class/gpio/gpio188/direction"
#define INPUT10_GPIO_DIR          	"/sys/class/gpio/gpio189/direction"
#define INPUT11_GPIO_DIR          	"/sys/class/gpio/gpio190/direction"
#define INPUT12_GPIO_DIR          	"/sys/class/gpio/gpio191/direction"
#define INPUT13_GPIO_DIR          	"/sys/class/gpio/gpio192/direction"
#define INPUT14_GPIO_DIR          	"/sys/class/gpio/gpio193/direction"

#define INPUT15_GPIO_DIR          	"/sys/class/gpio/gpio194/direction"		/*The third input port*/
#define INPUT16_GPIO_DIR          	"/sys/class/gpio/gpio195/direction"
#define INPUT17_GPIO_DIR          	"/sys/class/gpio/gpio196/direction"
#define INPUT18_GPIO_DIR          	"/sys/class/gpio/gpio197/direction"
#define INPUT19_GPIO_DIR          	"/sys/class/gpio/gpio198/direction"
#define INPUT20_GPIO_DIR          	"/sys/class/gpio/gpio199/direction"
#define INPUT21_GPIO_DIR          	"/sys/class/gpio/gpio200/direction"

#define INPUT22_GPIO_DIR          	"/sys/class/gpio/gpio201/direction"		//gpio ‰»Î∑ΩœÚ…Ë÷√   					
#define INPUT23_GPIO_DIR          	"/sys/class/gpio/gpio202/direction"		/*The fourth input port*/
#define INPUT24_GPIO_DIR          	"/sys/class/gpio/gpio203/direction"
#define INPUT25_GPIO_DIR          	"/sys/class/gpio/gpio204/direction"
#define INPUT26_GPIO_DIR          	"/sys/class/gpio/gpio205/direction"
#define INPUT27_GPIO_DIR          	"/sys/class/gpio/gpio206/direction"
#define INPUT28_GPIO_DIR          	"/sys/class/gpio/gpio207/direction"

#define IN							"in"									// ‰»Îƒ£ Ω

#define INPUT1_GPIO_VALUE				"/sys/class/gpio/gpio180/value"				/*The first part*/
#define INPUT2_GPIO_VALUE				"/sys/class/gpio/gpio181/value"
#define INPUT3_GPIO_VALUE				"/sys/class/gpio/gpio182/value"
#define INPUT4_GPIO_VALUE				"/sys/class/gpio/gpio183/value"
#define INPUT5_GPIO_VALUE				"/sys/class/gpio/gpio184/value"
#define INPUT6_GPIO_VALUE				"/sys/class/gpio/gpio185/value"
#define INPUT7_GPIO_VALUE				"/sys/class/gpio/gpio186/value"

#define INPUT8_GPIO_VALUE				"/sys/class/gpio/gpio187/value"				/*The second part*/
#define INPUT9_GPIO_VALUE				"/sys/class/gpio/gpio188/value"
#define INPUT10_GPIO_VALUE				"/sys/class/gpio/gpio189/value"
#define INPUT11_GPIO_VALUE				"/sys/class/gpio/gpio190/value"
#define INPUT12_GPIO_VALUE				"/sys/class/gpio/gpio191/value"
#define INPUT13_GPIO_VALUE				"/sys/class/gpio/gpio192/value"
#define INPUT14_GPIO_VALUE				"/sys/class/gpio/gpio193/value"

#define INPUT15_GPIO_VALUE				"/sys/class/gpio/gpio194/value"				/*The thrid part*/
#define INPUT16_GPIO_VALUE				"/sys/class/gpio/gpio195/value"
#define INPUT17_GPIO_VALUE				"/sys/class/gpio/gpio196/value"
#define INPUT18_GPIO_VALUE				"/sys/class/gpio/gpio197/value"
#define INPUT19_GPIO_VALUE				"/sys/class/gpio/gpio198/value"
#define INPUT20_GPIO_VALUE				"/sys/class/gpio/gpio199/value"
#define INPUT21_GPIO_VALUE				"/sys/class/gpio/gpio200/value"

#define INPUT22_GPIO_VALUE				"/sys/class/gpio/gpio201/value"
#define INPUT23_GPIO_VALUE				"/sys/class/gpio/gpio202/value"
#define INPUT24_GPIO_VALUE				"/sys/class/gpio/gpio203/value"
#define INPUT25_GPIO_VALUE				"/sys/class/gpio/gpio204/value"
#define INPUT26_GPIO_VALUE				"/sys/class/gpio/gpio205/value"
#define INPUT27_GPIO_VALUE				"/sys/class/gpio/gpio206/value"
#define INPUT28_GPIO_VALUE				"/sys/class/gpio/gpio207/value"



typedef struct{

	unsigned char slow_front_go;
	unsigned char slow_front_stop;
	unsigned char slow_front_turn;
	unsigned char slow_back_go;
	unsigned char slow_back_stop;
	unsigned char slow_back_turn;

	unsigned char main_front_go;
	unsigned char main_front_stop;
	unsigned char main_front_turn;
	unsigned char main_back_go;
	unsigned char main_back_stop;
	unsigned char main_back_turn;

	unsigned char over_front_go;
	unsigned char over_front_stop;
	unsigned char over_front_turn;
	unsigned char over_back_go;
	unsigned char over_back_stop;
	unsigned char over_back_turn;
}lane_led_state_t;



typedef struct{
	unsigned char slow_lane_pass;
	unsigned char slow_lane_forbid;
	unsigned char slow_lane_reverse;
	unsigned char slow_lane_turn_front;
	unsigned char slow_lane_turn_back;

	unsigned char main_lane_pass;
	unsigned char main_lane_forbid;
	unsigned char main_lane_reverse;
	unsigned char main_lane_turn_front;
	unsigned char main_lane_turn_back;

	unsigned char over_lane_pass;
	unsigned char over_lane_forbid;
	unsigned char over_lane_reverse;
	unsigned char over_lane_turn_front;
	unsigned char over_lane_turn_back;
}gpio_input_state_t;


int gpio_input_init(void);
int gpio_input_state_get(void);

extern sem_t gInputGPIO_sem;	//‰ø°Âè∑Èáè

extern lane_led_state_t lane;
extern gpio_input_state_t gpio_input;
extern char input_gpio_state[32];

	
#endif

