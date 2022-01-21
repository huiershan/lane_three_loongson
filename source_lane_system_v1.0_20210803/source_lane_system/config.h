/*
readConfig.h
*/
#ifndef _CONFIG_H__
#define _CONFIG_H__

#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define MaxLine 2048

#define CFGNAME "/home/app/config.txt"


#ifdef __cplusplus
extern "C"{
#endif

int GetCfgItem(char *pFileName, char *pKey, char *pValue, int *pValueLen);

//函数声明只是声明传入参数类型，参数名称可以和实现时不一样
int WriteCfgItem(char *pFileName, char *pItemName, char *pItemValue, int itemValueLen);

void str_to_hex(unsigned char *pbDest, unsigned char *pbSrc, int nLen);

#ifdef __cplusplus
}

#endif

#endif
