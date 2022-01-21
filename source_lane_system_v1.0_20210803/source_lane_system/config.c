

/*
readConfig.c
*/
#include "config.h"


//获取配置项
int GetCfgItem(char *pFileName, char *pKey, char *pValue, int *pValueLen)
{
    int ret = 0;
    FILE *fp = NULL;
    char *pTmp = NULL, *pEnd = NULL, *pBegin = NULL;

    char lineBuf[MaxLine];

    fp = fopen(pFileName, "r");
    if (fp == NULL)
    {
        //-1 means the file is not exist
        ret = -1;
        return ret;
    }

    while (!feof(fp))
    {
        memset(lineBuf, 0 ,sizeof(lineBuf));
        fgets(lineBuf, MaxLine, fp);
        
        pTmp = strchr(lineBuf, '=');    //没有 = 号行跳过
        if (pTmp == NULL)
        {
            continue;
        }
        
        pTmp = strstr(lineBuf, pKey);    //判断该行是否有pKey
        if (pTmp == NULL)
        {
            continue;
        }
        pTmp = pTmp + strlen(pKey);    // wang = qing  ==> "= qing"
        
        pTmp = strchr(pTmp, '=');    //判断pKey后面是否有=
        if (pTmp == NULL)
        {
            continue;
        }
        
        pTmp = pTmp + 1;    //有等号把指针移到 = 号后一位

        //获取value的起点 去除空格，遇到第一个非空格符号位置赋予pBegin
        while(1)
        {
            if (*pTmp == ' ')
            {
                pTmp++;
            }
            else
            {
                pBegin = pTmp;
                if (*pBegin == '\n')
                {
                    //-2 means the value is empty
                    fclose(fp);
                    ret = -2;
                    return ret;
                }
                break;
            }
        }
        
        //获取value终点
        //遇到换行符或空格符终止(value值中不能有换行符空格符)
        while(1)
        {
            if ((*pTmp == ' ') || (*pTmp == '\n'))
            {
                break;
            }
            else
            {
                pTmp++;
            }
        }
        pEnd = pTmp;
        
        *pValueLen = pEnd - pBegin;
        memcpy(pValue, pBegin, pEnd-pBegin);
		//printf("--------------pvalue=%s\n",pValue);
    }
    if (fp != NULL)
    {
        fclose(fp);
    }
    return 0;
}

/*
写配置文件
循环写每一行，检查key配置项是否存在，若存在，则修改对应value值
若不存在，则在文件末尾添加 “key = value”
*/
int WriteCfgItem(char *pFileName, char *pKey, char *pValue, int ValueLen)
{
    int rv = 0, iTag = 0, length = 0;
    FILE *fp = NULL;
    char lineBuf[MaxLine];
    char *pTmp = NULL, *pBegin = NULL, *pEnd = NULL;
    char filebuf[1024*8] = {0};

    if (pFileName == NULL || pKey == NULL || pValue == NULL)
    {
        rv = -1;
        printf("setCfgItem() error. param error \n");
        return rv;
    }

    fp = fopen(pFileName, "r+");
    if (fp == NULL)
    {
        printf("The file %s is not exist, now create it.\n", pFileName);

        fp = fopen(pFileName, "w+t");
        if (fp == NULL)
        {
            rv = -2;
            printf("create new file %s error. \n",pFileName);
            return rv;
        }
    }
    
    fseek(fp, 0L, SEEK_END);    //把文件指针从文件开头移动到文件结尾
    length = ftell(fp);        //获取文件长度
    if (length > 1024 *8)
    {
        rv = -3;
        printf("the size of file is too big, and no support\n");
        fclose(fp);
        return rv;
    }

    fseek(fp, 0L, SEEK_SET);

    while (!feof(fp))
    {
        memset(lineBuf, 0, sizeof(lineBuf));
        pTmp = fgets(lineBuf, MaxLine, fp);
        if (pTmp == NULL)
        {
            break;
        }

        /*检查pKey是否在本行
        不在copy到filebuf中
        在先修改，copy到filebuf中（替换）
        读完整个文件
        */
        pTmp = strstr(lineBuf, pKey);
        if (pTmp == NULL)
        {
            strcat(filebuf, lineBuf);
            continue;
        }
        else
        {
            sprintf(lineBuf,"%s=%s\n", pKey, pValue);
            //将linebuf内容追加复制到filebuf中            
            strcat(filebuf, lineBuf);
            iTag = 1;
        }
    }

    //关键字不存在，追加
    if (iTag == 0)
    {
        fprintf(fp, "%s=%s\n", pKey, pValue);
    }
    else    //若关键字存在，则重新创建文件 先关闭源文件，再重新从头写入该文件中
    {
        if (fp != NULL)
        {
            fclose(fp);
            fp = NULL;
        }

        fp = fopen(pFileName, "w+t");
        if (fp == NULL)
        {
            rv = -4;
            printf("fopen() err. \n");
            if (fp != NULL)
            {
                fclose(fp);
                return rv;
            }
        }
        fputs(filebuf, fp);
    }

    if (fp != NULL)
    {
        fclose(fp);
    }
    return rv;
}



int TGetCfg()
{
    int ret = 0;
    char name[1024] = {0};
    char value[1024] = {0};
    int vlen = 0;

    printf("\nplease input key: ");
    scanf("%s,",name);

    ret = GetCfgItem(CFGNAME, name, value, &vlen);
    if (0 != ret)
    {
        printf("func WriteCfgItem err:%d \n",ret);
        return ret;
    }

    printf("value:%s \n",value);
    return ret;
}

int TWriteCfg()
{
    int ret = 0;
    char name[1024] = {0};
    char value[1024] = {0};

    printf("\nplease input key: ");
    scanf("%s",name);

    printf("\nplease input value: ");
    scanf("%s", value);

    ret = WriteCfgItem(CFGNAME, name, value, strlen(value));
    if (0 != ret)
    {
        printf("func WriteCfgItem err : %d \n", ret);
        return ret;
    }

    printf("your input name is %s , value is %s\n", name, value);
    return ret;
}

/*
// C prototype : void StrToHex(unsigned char *pbDest, unsigned char *pbSrc, int nLen)
// parameter(s): [OUT] pbDest - ���������
// [IN] pbSrc - �ַ���
// [IN] nLen - 16���������ֽ���(�ַ����ĳ���/2)
// return value:
// remarks : ���ַ���ת��Ϊ16������
*/
void str_to_hex(unsigned char *pbDest, unsigned char *pbSrc, int nLen)
{
	char h1,h2;
	unsigned char s1,s2;
	int i;

	for (i=0; i<nLen; i++)
		{
			h1 = pbSrc[2*i];
			h2 = pbSrc[2*i+1];

			s1 = toupper(h1) - 0x30; //ʮ������ 0x30   ,    decʮ���� 48	,   ͼ�� 0
			if (s1 > 9)
				s1 -= 7;

			s2 = toupper(h2) - 0x30;
			if (s2 > 9)
				s2 -= 7;

			pbDest[i] = s1*16 + s2;
		}
}

/*
int main()
{
	unsigned char tmp[65] = "4c8c6827f05dbc64b1c43e7e343d9adc4c8c6827f05dbc64b1c43e7e343d9adc";
	unsigned char out[32] = {0};
	memset(out, 0 ,sizeof(out));
    printf("sizeof(out):%ld\n", sizeof(out));

	printf("tmp:%s\n", tmp);
	StrToHex(out, tmp, 32);
	
	int i;
	for(i=0; i<sizeof(out); i++)
	{
		printf("out[%2d] : %2x  ", i, out[i]);
        if ((i + 1) % 8 == 0) 
        {
            printf("\r\n");
        }
	}

	return 0;
}
*/
/*
sizeof(out):32
tmp:4c8c6827f05dbc64b1c43e7e343d9adc4c8c6827f05dbc64b1c43e7e343d9adc
out[ 0] : 4c  out[ 1] : 8c  out[ 2] : 68  out[ 3] : 27  out[ 4] : f0  out[ 5] : 5d  out[ 6] : bc  out[ 7] : 64  
out[ 8] : b1  out[ 9] : c4  out[10] : 3e  out[11] : 7e  out[12] : 34  out[13] : 3d  out[14] : 9a  out[15] : dc  
out[16] : 4c  out[17] : 8c  out[18] : 68  out[19] : 27  out[20] : f0  out[21] : 5d  out[22] : bc  out[23] : 64  
out[24] : b1  out[25] : c4  out[26] : 3e  out[27] : 7e  out[28] : 34  out[29] : 3d  out[30] : 9a  out[31] : dc 
*/