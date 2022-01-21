
#ifndef _MODBUS_SERVER_H
#define _MODBUS_SERVER_H
#include "modbus.h"
#include <semaphore.h>

//lane ctrl reg address
#define REG_SLOW_CTRL_RW 1
#define REG_MAIN_CTRL_RW 2
#define REG_OVER_CTRL_RW 3

#define REG_SLOW_FRONT_STATE_R 1
#define REG_SLOW_BACK_STATE_R 2
#define REG_SLOW_STATE_R 3

#define REG_MAIN_FRONT_STATE_R 4
#define REG_MAIN_BACK_STATE_R 5
#define REG_MAIN_STATE_R 6

#define REG_OVER_FRONT_STATE_R 7
#define REG_OVER_BACK_STATE_R 8
#define REG_OVER_STATE_R 9

#define REG_LANE_ALARM_INFOR_R 10

#define REG_RUN_STATE_R 100
#define REG_LOCAL_ENABLE_R 101
#define REG_ALARM_INFOR_R 102

//adc reg address
#define REG_ADC_1_K_L_RW 20
#define REG_ADC_1_K_H_RW 21
#define REG_ADC_2_K_L_RW 22
#define REG_ADC_2_K_H_RW 23
#define REG_ADC_3_K_L_RW 30
#define REG_ADC_3_K_H_RW 31
#define REG_ADC_4_K_L_RW 32
#define REG_ADC_4_K_H_RW 33

#define REG_ADC_1_CURRENT_L_R 20
#define REG_ADC_1_CURRENT_H_R 21
#define REG_ADC_1_DEV_VAULE_L_R 22
#define REG_ADC_1_DEV_VAULE_H_R 23

#define REG_ADC_2_CURRENT_L_R 24
#define REG_ADC_2_CURRENT_H_R 25
#define REG_ADC_2_DEV_VAULE_L_R 26
#define REG_ADC_2_DEV_VAULE_H_R 27

#define REG_ADC_3_CURRENT_L_R 30
#define REG_ADC_3_CURRENT_H_R 31
#define REG_ADC_3_DEV_VAULE_L_R 32
#define REG_ADC_3_DEV_VAULE_H_R 33

#define REG_ADC_4_CURRENT_L_R 34
#define REG_ADC_4_CURRENT_H_R 35
#define REG_ADC_4_DEV_VAULE_L_R 36
#define REG_ADC_4_DEV_VAULE_H_R 37

#define REG_ADC_1_ERROR_R 28
#define REG_ADC_2_ERROR_R 29
#define REG_ADC_3_ERROR_R 38
#define REG_ADC_4_ERROR_R 39



int modbus_tcp_server(void);
extern modbus_mapping_t *mb_mapping;
extern sem_t gModbus_ctrl_lane_sem;

#endif


