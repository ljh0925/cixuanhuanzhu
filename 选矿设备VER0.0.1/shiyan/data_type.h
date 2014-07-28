/******************************************************************************
Copyright 2005 大连华科仪器有限公司
All rights riserved.

文件名  ：data_type.h
模块名称：定义了所有的数据类型

当前版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create
******************************************************************************/
#ifndef __DATA_TYPE_H__
#define __DATA_TYPE_H__

#include "OS_CPU.H"

typedef unsigned char  	uint8;                   /* 无符号8位整型变量                        */
typedef signed   char  	int8;                    /* 有符号8位整型变量                        */
typedef unsigned short 	uint16;                  /* 无符号16位整型变量                       */
typedef signed   short 	int16;                   /* 有符号16位整型变量                       */
typedef unsigned int   	uint32;                  /* 无符号32位整型变量                       */
typedef signed   int   	int32;                   /* 有符号32位整型变量                       */
typedef float          	fp32;                    /* 单精度浮点数（32位长度）                 */
typedef double         	fp64;                    /* 双精度浮点数（64位长度）                 */



#ifndef QUEUE_DATA_TYPE
	#define QUEUE_DATA_TYPE     		uint8
#endif

typedef struct 
{
    QUEUE_DATA_TYPE     *Out;                   /* 指向数据输出位置         */
    QUEUE_DATA_TYPE     *In;                    /* 指向数据输入位置         */
    QUEUE_DATA_TYPE     *End;                   /* 指向Buf的结束位置        */
    uint16              NData;                  /* 队列中数据个数           */
    uint16              MaxData;                /* 队列中允许存储的数据个数 */
    uint8               (* ReadEmpty)();        /* 读空处理函数             */
    uint8               (* WriteFull)();        /* 写满处理函数             */
    QUEUE_DATA_TYPE     Buf[1];                 /* 存储数据的空间           */
} DataQueue;


//typedef FP64 AD_TYPE;
typedef FP32 AD_TYPE;


#endif
