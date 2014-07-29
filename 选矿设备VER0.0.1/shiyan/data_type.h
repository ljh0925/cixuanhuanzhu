/******************************************************************************
Copyright 2005 ���������������޹�˾
All rights riserved.

�ļ���  ��data_type.h
ģ�����ƣ����������е���������

��ǰ�汾��0.0.1
�޸���	��������
������ڣ�2005.04.06
����˵����create
******************************************************************************/
#ifndef __DATA_TYPE_H__
#define __DATA_TYPE_H__

#include "OS_CPU.H"

typedef unsigned char  	uint8;                   /* �޷���8λ���ͱ���                        */
typedef signed   char  	int8;                    /* �з���8λ���ͱ���                        */
typedef unsigned short 	uint16;                  /* �޷���16λ���ͱ���                       */
typedef signed   short 	int16;                   /* �з���16λ���ͱ���                       */
typedef unsigned int   	uint32;                  /* �޷���32λ���ͱ���                       */
typedef signed   int   	int32;                   /* �з���32λ���ͱ���                       */
typedef float          	fp32;                    /* �����ȸ�������32λ���ȣ�                 */
typedef double         	fp64;                    /* ˫���ȸ�������64λ���ȣ�                 */



#ifndef QUEUE_DATA_TYPE
	#define QUEUE_DATA_TYPE     		uint8
#endif

typedef struct 
{
    QUEUE_DATA_TYPE     *Out;                   /* ָ���������λ��         */
    QUEUE_DATA_TYPE     *In;                    /* ָ����������λ��         */
    QUEUE_DATA_TYPE     *End;                   /* ָ��Buf�Ľ���λ��        */
    uint16              NData;                  /* ���������ݸ���           */
    uint16              MaxData;                /* ����������洢�����ݸ��� */
    uint8               (* ReadEmpty)();        /* ���մ�����             */
    uint8               (* WriteFull)();        /* д��������             */
    QUEUE_DATA_TYPE     Buf[1];                 /* �洢���ݵĿռ�           */
} DataQueue;


//typedef FP64 AD_TYPE;
typedef FP32 AD_TYPE;


#endif
