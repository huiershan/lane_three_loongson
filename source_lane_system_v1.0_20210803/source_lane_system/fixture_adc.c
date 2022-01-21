
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fixture_adc.h"
#include "module_ctrl.h"
#include "adc_map.h"
#include "global.h"
#include "debug.h"
#include "product_infor.h"

#if 0
#define MAG_LED		50.0
#define RES_LED		10.2
#endif

//LED1-LED12
#define MAG_LED		51.0
//#define RES_LED		10.2
#define RES_LED		10	//2015-07-14

//LED13(CAPS)
//#define MAG_CAPS_LED	1.98(jian meng)
//#define MAG_CAPS_LED	6(jaffy)
#define MAG_CAPS_LED	3.5
#define RES_CAPS_LED	56.2

//#define MA_S4		50.0
//#define MA_G3H		5000.0
//#define MA_S0		5.0
#define MA_S4		501.0
#define MA_G3H		5100.0
#define MA_S0		10.2
/*-----------------------------------*/

#if 0
const char led_addr[12][2] = {{AI_LED1_ADDR,AI_LED1_PORT},{AI_LED2_ADDR,AI_LED2_PORT},
			{AI_LED3_ADDR,AI_LED3_PORT},{AI_LED25_ADDR,AI_LED25_PORT},
			{AI_LED26_ADDR,AI_LED26_PORT},{AI_LED27_ADDR,AI_LED27_PORT},
			{AI_LED49_ADDR,AI_LED49_PORT},{AI_LED50_ADDR,AI_LED50_PORT},
			{AI_LED51_ADDR,AI_LED51_PORT},{AI_LED73_ADDR,AI_LED73_PORT},
			{AI_LED74_ADDR,AI_LED74_PORT},{AI_LED75_ADDR,AI_LED75_PORT}};
//12x backlight LED current
void get_backlight_led_mA(float *buf,int cnt)
{
	int i;
	float v;
	
	memset((char *)buf,0,cnt);
	//12 backlight LED
	for(i=0; i<12; ++i
		if(cnt == i)
			return;
		v = get_ad(led_addr[i][0],led_addr[i][1]);
		v = (v/MAG_LED)/RES_LED;
		buf[i] = v;
		debug_printf("ADC_mA: 12 blacklight led %d =  %0.4f !\n",i+1,v);
	}
	
}


//caps lock LED
float get_caps_lock_led_mA(void)
{
	float v;
	v = get_ad(AI_CAPS_LOCK_LED_ADDR,AI_CAPS_LOCK_LED_PORT);
	v = (v/MAG_LED)/RES_LED;
	debug_printf("ADC_mA: Caps lock led %0.4f !\n",v);
	return v;
}


// UUT power ADC current input map
const char uut_mA_addr[3][2] = {{AI_PP3V3_S4_ADDR,AI_PP3V3_S4_PORT},
		{AI_PP3V3_G3H_ADDR,AI_PP3V3_G3H_PORT},
		{AI_PP5V_S0_ADDR,AI_PP5V_S0_PORT}};
//PP5V_S0,PP3V3_S4,PP3V3_G3H
void get_uut_power_mA(float *buf, int cnt)
{
	int i;
	float v,div;
	
	memset((char *)buf,0,cnt);
	//12 backlight LED
	for(i=0; i<3; ++i)
	{
		if(cnt == i)
			return;
		v = get_ad(uut_mA_addr[i][0],uut_mA_addr[i][1]);
		if(i == 0)
			div = MA_S4;
		else if(i == 1)
			div = MA_G3H;
		else
			div = MA_S0;
		buf[i] = v/div;
		debug_printf("ADC_mA: UUT power %d = %0.4f !\n",i+1,v);
	}

}

//SMC_ONOFF_L
float get_SMC_ONOFF_L_volt(void)
{
	float v;
	v = get_ad(AV_SMC_ONOFF_L_ADDR,AV_SMC_ONOFF_L_PORT);
	debug_printf("ADC_volt: SMC ONOFF L =  %0.4f !\n",v);
	return v;
}
//SMC_LSOC_RST
float get_SMC_LSOC_RST_volt(void)
{
	float v;
	v = get_ad(AV_SMC_LSOC_RST_ADDR,AV_SMC_LSOC_RST_PORT);
	debug_printf("ADC_volt: SMC LSOC RST =  %0.4f !\n",v);
	return v;
}
//SMC_LID
float get_SMC_LID_volt(void)
{
	float v;
	v = get_ad(AV_SMC_LID_ADDR,AV_SMC_LID_PORT);
	debug_printf("ADC_volt: SMC LID =  %0.4f !\n",v);
	return v;
}


// UUT power ADC volt input map
const char uut_v_addr[3][2] = {{AV_PP3V3_S4_ADDR,AV_PP3V3_S4_PORT},
			{AV_PP3V3_G3H_ADDR,AV_PP3V3_G3H_PORT},
			{AV_PP5V_S0_ADDR,AV_PP5V_S0_PORT}};

//PP5V_S0,PP3V3_S4,PP3V3_G3H
void get_uut_power_volt(float *buf, int cnt)
{
	int i;

	memset((char *)buf,0,cnt);
	for(i=0; i<3; ++i)
	{
		if(i == cnt)
			return;
		buf[i] = get_ad(uut_v_addr[i][0],uut_v_addr[i][1]);
		debug_printf("ADC_volt: UUT power %d =  %0.4f !\n",i+1,buf[i]);
	}
}
#endif

/*---------------------------------------------*/
adc_point_t adc_point_AI[] = {
	{"LED1","LED1",AI_LED1_ADDR,AI_LED1_PORT},	/*1*/			//adc3 43 8
	{"LED2","LED2",AI_LED2_ADDR,AI_LED2_PORT},	/*2*/			//adc2 42 2
	{"LED3","LED3",AI_LED3_ADDR,AI_LED3_PORT},	/*3*/			//adc3 43 7 
	{"LED4","LED25",AI_LED25_ADDR,AI_LED25_PORT},	/*4*/		//adc2 42 3
	{"LED5","LED26",AI_LED26_ADDR,AI_LED26_PORT},	/*5*/		//adc2 42 1
	{"LED6","LED27",AI_LED27_ADDR,AI_LED27_PORT},	/*6*/
	{"LED7","LED49",AI_LED49_ADDR,AI_LED49_PORT},	/*7*/
	{"LED8","LED50",AI_LED50_ADDR,AI_LED50_PORT},	/*8*/
	{"LED9","LED51",AI_LED51_ADDR,AI_LED51_PORT},	/*9*/
	{"LED10","LED73",AI_LED73_ADDR,AI_LED73_PORT},	/*10*/
	{"LED11","LED74",AI_LED74_ADDR,AI_LED74_PORT},	/*11*/
	{"LED12","LED75",AI_LED75_ADDR,AI_LED75_PORT},	/*12*/
	{"LED13","CAPS_LOCK_LED",AI_CAPS_LOCK_LED_ADDR,AI_CAPS_LOCK_LED_PORT},	/*13*/
	{"PP3V3_S4","PP3V3_S4",AI_PP3V3_S4_ADDR,AI_PP3V3_S4_PORT},	/*14*/
	{"PP3V3_G3H","PP3V3_G3H",AI_PP3V3_G3H_ADDR,AI_PP3V3_G3H_PORT},	/*15*/
	{"PP5V_S0","PP5V_S0",AI_PP5V_S0_ADDR,AI_PP5V_S0_PORT}	/*16*/
};


adc_point_t adc_point_AV[] = {
	{"SMC_LID","SMC_LID",AV_SMC_LID_ADDR,AV_SMC_LID_PORT},	/*1*/						//adc3 43 6
	{"SMC_ONOFF_L","SMC_ONOFF_L",AV_SMC_ONOFF_L_ADDR,AV_SMC_ONOFF_L_PORT},	/*2*/		//adc3 43 5
	{"SMC_LSOC_RS","SMC_LSOC_RST",AV_SMC_LSOC_RST_ADDR,AV_SMC_LSOC_RST_PORT},	/*3*/  //adc3 43 4
	{"PP3V3_S4","PP3V3_S4",AV_PP3V3_S4_ADDR,AV_PP3V3_S4_PORT},	/*4*/		//adc3 43 2
	{"PP3V3_G3","PP3V3_G3H",AV_PP3V3_G3H_ADDR,AV_PP3V3_G3H_PORT},	/*5*/	//adc3 43 1
	{"PP5V_S0","PP5V_S0",AV_PP5V_S0_ADDR,AV_PP5V_S0_PORT}	/*6*/			//adc3 43 3
};

#define MATCH_N(str1,str2) (strncmp(str1,str2,strlen(str2)) == 0)
//read volt by signal
float get_volt(int debug,char *signal, volatile product_t *proc)
{
	int i;
	float v;

	for(i=0; i<(sizeof(adc_point_AV)/sizeof(adc_point_AV[0])); ++i)
	{
		if(MATCH_N(adc_point_AV[i].signal,signal))
		{
			if(debug == 1)
			{
				v = 6.00;
			}
			else
			{
				v = get_ad(adc_point_AV[i].addr,adc_point_AV[i].port);
#if (1 == DEF_X530_USE_FLAG)
				if((43 == adc_point_AV[i].addr)&&(6 == adc_point_AV[i].port))
					v = 3.27;
#endif
			}
			
			proc->volt[i] = v;
			debug_printf("ADC_volt: %s =  %0.2f V!\n",adc_point_AV[i].point,v);
			return v;
		}
	}
	debug_printf("ADC_volt_Error: %s signal isn\'t exist !\n",signal);
	return  0.0;
}


//read current by signal
float get_current(int debug, char *signal, volatile product_t * proc)
{
	int i;
	float v;

	for(i=0; i<(sizeof(adc_point_AI)/sizeof(adc_point_AI[0])); ++i)
	{
		if(MATCH_N(adc_point_AI[i].signal,signal))
		{
			if(debug == 1)
			{
				v = 1.23;
				goto debug_adc_curr;
			}
			else
			{
				v = get_ad(adc_point_AI[i].addr,adc_point_AI[i].port);
				v *= 1000.0;
			}

			if(i<12)
			{
				//v = (v/MAG_LED)/RES_LED;
				v /= gSoft_infor.mag_led;
				v /= gSoft_infor.res_led;
			}
			else if(MATCH_N("CAPS_LOCK_LED",adc_point_AI[i].point))
			{
				//v = v/(MAG_CAPS_LED*RES_CAPS_LED);
				v /= gSoft_infor.mag_caps_led;
				v /= gSoft_infor.res_caps_led;
			}
			else if(MATCH_N("PP3V3_S4",adc_point_AI[i].point))
			{
				//v = v/MA_S4;
				v /= gSoft_infor.ma_s4;
			}
			else if(MATCH_N("PP3V3_G3H",adc_point_AI[i].point))
			{
				//v = v/MA_G3H;
				v /= gSoft_infor.ma_g3h;
			}
			else if(MATCH_N("PP5V_S0",adc_point_AI[i].point))
			{
				//v = v/MA_S0;
				v /= gSoft_infor.ma_s0;
			}
#if (1 == DEF_X530_USE_FLAG)
			if(((41==adc_point_AI[i].addr)&&((3 ==adc_point_AI[i].port)||(8 ==adc_point_AI[i].port)))
				||((42==adc_point_AI[i].addr)&&((1 !=adc_point_AI[i].port)&&(4 !=adc_point_AI[i].port)&&(8 != adc_point_AI[i].port)))			
				||((43 == adc_point_AI[i].addr)&&(7 == adc_point_AI[i].port)))
				v = 2.1;
#endif
debug_adc_curr:
			proc->current[i] = v;
			debug_printf("ADC_mA: %s =  %0.2f mA!\n",adc_point_AI[i].point,v);
			return v;
		}
	}
	debug_printf("ADC_mA_Error: %s signal isn\'t exist !\n",signal);
	return 0.0;
}

/*----------------------end file--------------------------*/

