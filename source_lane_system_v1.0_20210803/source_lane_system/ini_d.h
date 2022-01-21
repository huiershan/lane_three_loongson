
#ifndef _INI_D_H
#define _INI_D_H

 
int ini_parse(const char* filename,
				int (*handler)(void *,const char*,const char*,const char*),
				void* user);

/* Strip whitespace chars off end of given string, in place. Return s. */
char* rstrip(char* s);
/* Return pointer to first non-whitespace char in given string. */
char* lskip(const char* s);
#endif
/*-------------------end file-----------------*/

