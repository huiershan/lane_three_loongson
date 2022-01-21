
#ifndef _DEBUG_PRINTF_H
#define _DEBUG_PRINTF_H

#include <string.h>
//#include "memwatch.h"

#if defined DEBUG

//#define debug_printf(fmt,args...)	printf("%s:%s Line(%d): \n"fmt,__FILE__,__func__,__LINE__,##args)

#define debug_printf(fmt,args...)	printf(fmt,##args)
#else

#define debug_printf(fmt,args...)	do{}while(0)

#endif


#define print_error(fmt,args...)	fprintf(stderr,"ERROR: %s(%d):%s: "fmt,__FILE__,__LINE__,__func__,##args)
#define print_warn(fmt,args...)	fprintf(stdout,"Warn: %s(%d):%s: "fmt,__FILE__,__LINE__,__func__,##args)
#define print_infor(fmt,args...)	fprintf(stdout,"Infor: "fmt,##args)

#endif
/*-----------------------end file-----------------------*/

