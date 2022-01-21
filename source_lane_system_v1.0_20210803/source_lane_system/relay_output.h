#ifndef _RELAY_OUTPUT_H
#define _RELAY_OUTPUT_H

#define SYSFS_GPIO_PATH           	"/sys/class/gpio/export"  				//gpio路径

#define RELAY1  						"208"									/*the first output port*/
#define RELAY2  						"209"
#define RELAY3  						"210"
#define RELAY4 						"211"
#define RELAY5  						"212"
#define RELAY6  						"213"
#define RELAY7  						"214"

#define RELAY8  						"215"									/*the second output port*/
#define RELAY9  						"216"
#define RELAY10  					"217"
#define RELAY11  					"218"
#define RELAY12 						"219"
#define RELAY13  					"220"
#define RELAY14  					"221"

#define RELAY15  					"222"									/*the thrid output port*/
#define RELAY16  					"223"
#define RELAY17 						"224"
#define RELAY18  					"225"
#define RELAY19  					"226"
#define RELAY20  					"227"
#define RELAY21 						"228"

#define RELAY22						"229"									/*the fourth output port*/							
#define RELAY23  					"230"
#define RELAY24 						"231"
#define RELAY25 						"232"
#define RELAY26 						"233"
#define RELAY27						"234"
#define RELAY28						"235"									//设置gpio引脚映射地址

#define RELAY1_GPIO_DIR         "/sys/class/gpio/gpio208/direction"		//gpio输出方向设置
#define RELAY2_GPIO_DIR         "/sys/class/gpio/gpio209/direction"		/*The frist part*/
#define RELAY3_GPIO_DIR         "/sys/class/gpio/gpio210/direction"
#define RELAY4_GPIO_DIR         "/sys/class/gpio/gpio211/direction"
#define RELAY5_GPIO_DIR         "/sys/class/gpio/gpio212/direction"
#define RELAY6_GPIO_DIR         "/sys/class/gpio/gpio213/direction"
#define RELAY7_GPIO_DIR         "/sys/class/gpio/gpio214/direction"

#define RELAY8_GPIO_DIR         "/sys/class/gpio/gpio215/direction"		/*The second part*/
#define RELAY9_GPIO_DIR         "/sys/class/gpio/gpio216/direction"
#define RELAY10_GPIO_DIR         "/sys/class/gpio/gpio217/direction"
#define RELAY11_GPIO_DIR         "/sys/class/gpio/gpio218/direction"
#define RELAY12_GPIO_DIR         "/sys/class/gpio/gpio219/direction"
#define RELAY13_GPIO_DIR         "/sys/class/gpio/gpio220/direction"
#define RELAY14_GPIO_DIR         "/sys/class/gpio/gpio221/direction"

#define RELAY15_GPIO_DIR         "/sys/class/gpio/gpio222/direction"	/*The thrid part*/
#define RELAY16_GPIO_DIR         "/sys/class/gpio/gpio223/direction"
#define RELAY17_GPIO_DIR         "/sys/class/gpio/gpio224/direction"
#define RELAY18_GPIO_DIR         "/sys/class/gpio/gpio225/direction"
#define RELAY19_GPIO_DIR         "/sys/class/gpio/gpio226/direction"
#define RELAY20_GPIO_DIR         "/sys/class/gpio/gpio227/direction"
#define RELAY21_GPIO_DIR         "/sys/class/gpio/gpio228/direction"

#define RELAY22_GPIO_DIR         "/sys/class/gpio/gpio229/direction"	/*The fourth part*/
#define RELAY23_GPIO_DIR         "/sys/class/gpio/gpio230/direction"
#define RELAY24_GPIO_DIR         "/sys/class/gpio/gpio231/direction"
#define RELAY25_GPIO_DIR         "/sys/class/gpio/gpio232/direction"
#define RELAY26_GPIO_DIR         "/sys/class/gpio/gpio233/direction"
#define RELAY27_GPIO_DIR         "/sys/class/gpio/gpio234/direction"
#define RELAY28_GPIO_DIR         "/sys/class/gpio/gpio235/direction"		



#define RELAY1_GPIO_VALUE			    "/sys/class/gpio/gpio208/value"
#define RELAY2_GPIO_VALUE			    "/sys/class/gpio/gpio209/value"			//gpio目前值
#define RELAY3_GPIO_VALUE			    "/sys/class/gpio/gpio210/value"			//gpio目前值
#define RELAY4_GPIO_VALUE			    "/sys/class/gpio/gpio211/value"			/*The frist part*/
#define RELAY5_GPIO_VALUE			    "/sys/class/gpio/gpio212/value"			
#define RELAY6_GPIO_VALUE			    "/sys/class/gpio/gpio213/value"			
#define RELAY7_GPIO_VALUE			    "/sys/class/gpio/gpio214/value"			

#define RELAY8_GPIO_VALUE			    "/sys/class/gpio/gpio215/value"			//gpio目前值
#define RELAY9_GPIO_VALUE			    "/sys/class/gpio/gpio216/value"			/*The second part*/
#define RELAY10_GPIO_VALUE			    "/sys/class/gpio/gpio217/value"			
#define RELAY11_GPIO_VALUE			    "/sys/class/gpio/gpio218/value"			
#define RELAY12_GPIO_VALUE			    "/sys/class/gpio/gpio219/value"			
#define RELAY13_GPIO_VALUE			    "/sys/class/gpio/gpio220/value"
#define RELAY14_GPIO_VALUE			    "/sys/class/gpio/gpio221/value"

#define RELAY15_GPIO_VALUE			    "/sys/class/gpio/gpio222/value"			/*The thrid part*/
#define RELAY16_GPIO_VALUE			    "/sys/class/gpio/gpio223/value"
#define RELAY17_GPIO_VALUE			    "/sys/class/gpio/gpio224/value"
#define RELAY18_GPIO_VALUE			    "/sys/class/gpio/gpio225/value"
#define RELAY19_GPIO_VALUE			    "/sys/class/gpio/gpio226/value"
#define RELAY20_GPIO_VALUE			    "/sys/class/gpio/gpio227/value"
#define RELAY21_GPIO_VALUE			    "/sys/class/gpio/gpio228/value"

#define RELAY22_GPIO_VALUE			    "/sys/class/gpio/gpio229/value"			/*The fourth part*/
#define RELAY23_GPIO_VALUE			    "/sys/class/gpio/gpio230/value"
#define RELAY24_GPIO_VALUE			    "/sys/class/gpio/gpio231/value"
#define RELAY25_GPIO_VALUE			    "/sys/class/gpio/gpio232/value"
#define RELAY26_GPIO_VALUE			    "/sys/class/gpio/gpio233/value"
#define RELAY27_GPIO_VALUE			    "/sys/class/gpio/gpio234/value"
#define RELAY28_GPIO_VALUE			    "/sys/class/gpio/gpio235/value"


#define OUT							"out"								//输出模式 继电器默认模式低电平有效
#define IN							"in"									//输入模式
#define OFF							"0"									//断开
#define ON							"1"									//吸合


int slow_lane_pass(void);
int slow_lane_forbid(void);
int slow_lane_reverse(void);
int slow_lane_turn_front(void);
int slow_lane_turn_back(void);
int slow_lane_pass_fornt_cha(void);	//临时添加

int main_lane_pass(void);
int main_lane_forbid(void);
int main_lane_reverse(void);
int main_lane_turn_front(void);
int main_lane_turn_back(void);

int over_lane_pass(void);
int over_lane_forbid(void);
int over_lane_reverse(void);
int over_lane_turn_front(void);
int over_lane_turn_back(void);


int relay_output_init(void);



#endif

