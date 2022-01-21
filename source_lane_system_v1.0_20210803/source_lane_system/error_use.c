
#include  "error_use.h"

/*--------------------------------------------*/

static volatile error_use_t _ferrno=ERROR_NONE;
/*---------------------------------------------*/

//get error number
error_use_t get_error_number(void)
{
	return _ferrno;
}


//set error number
void set_error_number(error_use_t erron)
{
	_ferrno = erron;
}

/*---------------------end file-------------------------------*/

