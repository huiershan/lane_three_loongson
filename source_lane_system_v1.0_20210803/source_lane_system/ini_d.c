#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#undef INI_DEBUG
#ifdef INI_DEBUG
#define debug_printf(fmt,arg...) printf(fmt,##arg)
#else
#define debug_printf(fmt,arg...) do{}while(0)
#endif

#define INI_MAX_LINE 256
#define MAX_SECTION 256
#define MAX_NAME 256

/* Strip whitespace chars off end of given string, in place. Return s. */
char* rstrip(char* s)
{
    char* p = s + strlen(s);
    while (p > s && isspace((unsigned char)(*--p)))
        *p = '\0';
    return s;
}

/* Return pointer to first non-whitespace char in given string. */
char* lskip(const char* s)
{
    while (*s && isspace((unsigned char)(*s)))
        s++;
    return (char*)s;
}

/* Return pointer to first char c or ';' comment in given string, or pointer to
   null at end of string if neither found. ';' must be prefixed by a whitespace
   character to register as a comment. */
static char* find_char_or_comment(const char* s, char c)
{
    int was_whitespace = 0;
    while (*s && *s != c && !(was_whitespace && *s == ';')) {
        was_whitespace = isspace((unsigned char)(*s));
        s++;
    }
    return (char*)s;
}

/* Version of strncpy that ensures dest (size bytes) is null-terminated. */
static char* strncpy0(char* dest, const char* src, size_t size)
{
    strncpy(dest, src, size);
    dest[size - 1] = '\0';
    return dest;
}

int ini_parse_file(FILE* file,
					int (*handler)(void*,const char*,const char*,const char*),
					void* user)
{
	char line[INI_MAX_LINE];
	char section[MAX_SECTION]= "";
	char prev_name[MAX_NAME]= "";
	char* start;
	char* end;
	char* name;
	char* value;
	int lineno;

	lineno = 0;

	//读取一行字符串
	while(fgets(line,INI_MAX_LINE,file) != NULL)
	{
		lineno++;
		debug_printf("line %d : %s.",lineno,line);

		start = line;
		//去掉前后空格
		start = lskip(rstrip(start));
		debug_printf("line %d : %s.\n",lineno,start);
		
		if(*start == ';' || *start == '#' || *start == '\0')
		{
			//判断该行是否以“；” “#”为注释行或者为空行
			continue;
		}
		else if(*start == '[')
		{
			//判断该行是段名行
			//找到段名行结尾
			end = find_char_or_comment(start+1,']');
			if(*end == ']')
			{
				*end = '\0';
				strncpy0(section,start+1,sizeof(section));
				*prev_name = '\0';
				debug_printf("section : %s.\n",section);
			}
			else
			{
				//出错
				printf("ERROR: %s : %s line(%d)\nNo ] to end.\n",__FILE__,__func__,__LINE__);
				return lineno;
			}
		}
		else
		{
			end = find_char_or_comment(start,'=');
			if(*end != '=')
			{
				end = find_char_or_comment(start,':');
			}

			if(*end == '=' || *end == ':')
			{
				*end = '\0';
				name = rstrip(start);
				value = lskip(end+1);
				end = find_char_or_comment(value,'\0');
				if(*end == ';')
					*end = '\0';
				value = rstrip(value);
				strncpy0(prev_name,name,sizeof(prev_name));
				debug_printf("name: %s = %s.\n",name,value);
				handler(user,section,name,value);
			}
		}
	}

	return 0;

}

int ini_parse(const char* filename,
				int (*handler)(void *,const char*,const char*,const char*),
				void* user)
{
	FILE* file;
	int error;
	
	file = fopen(filename,"r");
	if(file == NULL)
		return -1;

	debug_printf("INI : open %s file success !\n",filename);
	error = ini_parse_file(file,handler,user);
	fclose(file);
	return error;
}
/*----------------end file---------------------*/

