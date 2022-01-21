
#ifndef _FIXTURE_CTRL_H
#define _FIXTURE_CTRL_H

//fixture test product type
typedef enum{
	p_ERROR = -1,
	p_ANSI = 0,
	p_ISO = 1,
	p_JIS	= 2,
}ProductType_t;


//fixture sensor state
typedef enum{
	FIX_ERROR = -1,
	FIX_NONE = 0,
	FIX_UNLOCK=1,
	FIX_LOCK=2,
	FIX_PROBE_DOWN=3,
	FIX_PROBE_UP=4,
	FIX_UUT_IN=5,
	FIX_UUT_OUT=6,
}FixtureState_t;


//product type
extern volatile ProductType_t gFixtureType;
extern struct timeval gstart_tim;
extern volatile int gstart_tim_flag;


//UUT and fixture control
//advance
int uut_advance(void);
//back
int uut_back(void);
//up
int probe_block_up(void);
//down
int probe_block_down(void);
//fixture start
int fixture_start(void);
//fixture reset
int fixture_reset(void);

//get probe block state
FixtureState_t get_probe_state(void);
//get UUT state
FixtureState_t get_uut_state(void);
//get uut and probe state
FixtureState_t get_fixture_state(void);
/*---------------------*/

//power control
//IPD and UUT power
//on : ipd 5v -> uut 5v ->ipd 3v3 -> uut 3v3
//off uut 3v3 -> ipd 3v3 -> uut 5v -> ipd 5v
int ipd_uut_power(int cmd);
//beep control
int beep_ctrl(int cmd);

/*---------------------*/

//vacuum control
int vacuum_ctrl(int cmd);
/*---------------------*/

//button
//abort button
int is_abort_down(void);
//start button
int is_start_down(void);
/*---------------------*/

//is grating dark
int is_grating_dark(void);
int get_grating_sensor_state(void);
/*---------------------*/

//获取夹具类型
ProductType_t ReadFixtureType(void);

#endif
/*----------------------end file-----------------------*/

