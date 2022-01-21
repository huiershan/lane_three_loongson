
#ifndef _FIXTURE_ADC_H
#define _FIXTURE_ADC_H

#include "product_infor.h"

#define MAX_SIGNAL_NAME		128
typedef struct{
	char signal[MAX_SIGNAL_NAME];
	char point[MAX_SIGNAL_NAME];
	int addr;
	int port;
} adc_point_t;

#if 0
//12x backlight LED current
void get_backlight_led_mA(float *buf,int cnt);
//caps lock LED
float get_caps_lock_led_mA(void);
//PP5V_S0,PP3V3_S4,PP3V3_G3H
void get_uut_power_mA(float *buf, int cnt);

//SMC_ONOFF_L
float get_SMC_ONOFF_L_volt(void);
//SMC_LSOC_RST
float get_SMC_LSOC_RST_volt(void);
//SMC_LID
float get_SMC_LID_volt(void);
//PP5V_S0,PP3V3_S4,PP3V3_G3H
void get_uut_power_volt(float *buf, int cnt);
#endif

//read volt by signal
float get_volt(int debug, char *signal, volatile product_t * proc);
//read current by signal
float get_current(int debug, char *signal, volatile product_t * proc);

#endif
/*-------------------------end file-----------------------------------*/

