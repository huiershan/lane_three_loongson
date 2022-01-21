
#ifndef _ERROR_USE_H
#define _ERROR_USE_H


//error number
typedef enum{
	ERROR_NONE = 0,
	ERROR_INITAL,
	CAN_ERROR,
	CAN_IO_ERROR1,
	CAN_IO_ERROR2,
	CAN_IO_ERROR3,
	CAN_ADC_ERROR1,
	CAN_ADC_ERROR2,
	CAN_ADC_ERROR3,
	CAMERA_ERROR,
	SCANNER_ERROR

}error_use_t;


//get error number
error_use_t get_error_number(void);
//set error number
void set_error_number(error_use_t erron);

#endif
/*---------------------end file-------------------------------*/

