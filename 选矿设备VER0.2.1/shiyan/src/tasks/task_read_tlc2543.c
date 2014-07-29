/******************************************************************************
Copyright 2005 �������ķ�������޹�˾
All rights riserved.

�ļ���	��task_read_tlc2543.c
ģ�����ƣ���ȡADֵ�������
���ܸ�Ҫ����ȡADֵ�������

ȡ���汾��0.0.1
�޸���	������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"config.h"


extern volatile DEV_STAT DevStat;


/******************************************************************************
 �������ƣ�Task_Read_TLC2543
 ������������ȡADֵ����
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void Task_Read_TLC2543(void * data)
{
	INT8U i = 0;
	INT16U ad = 0;
	FP32 temp = 0.0;		//�����¶�

	ToggleWD();

	data = data;

	for( ;; )
	{
		ToggleWD();

		for ( i=0; i<1; i++ )
		{
			ad = Get2543Channel(i);
			OS_ENTER_CRITICAL();
			DevStat.ad_avg[i][DevStat.ad_avg_ptr] = ad; 
			OS_EXIT_CRITICAL();
		}
		OS_ENTER_CRITICAL();
		DevStat.ad_avg_ptr++;
		if ( DevStat.ad_avg_ptr >= AD_QUEUE_LEN )
			DevStat.ad_avg_ptr = 0;
		OS_EXIT_CRITICAL();

		OS_ENTER_CRITICAL();
		for ( i=0; i<1; i++ )
		{
			DevStat.MOD_REG.reg[0x0010 + i] 
			= AVERAGE((INT16U *)&DevStat.ad_avg[i][0]);
		}
		OS_EXIT_CRITICAL();

		//������ۺ�����Ӧ�ĵ�ѹֵ
		OS_ENTER_CRITICAL();

		for(i=0;i<1;i++)
		{
			temp = (((FP64)DevStat.MOD_REG.reg[0x0010 + i] * 5.0) / 4095.0);
			DevStat.MOD_REG.reg[0x0010 + i] = (INT16S)(temp * 1000.0);
		}

		OS_EXIT_CRITICAL();

		OSTimeDlyHMSM(0, 0, 0, 100);			//100msһ��
	}
}

/******************************************************************************
 �������ƣ�average
 ������������AD���е�ƽ��ֵ
 ����������
 �������ƣ� ����/����� ����        ����
 adc_data   ����    	INT16U*        ADֵ����

 ��  ��  ֵ��AD��ƽ����

 ��      �� ������
 ��      �ڣ�2005-11-02
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
INT16U AVERAGE(INT16U* adc_data)
{
	INT32U sum = 0;
	INT8U i = 0;

	ToggleWD();

	for ( i=0; i<AD_QUEUE_LEN; i++ )
	{
		sum += (INT32U)adc_data[i];
	}

	return(INT16U)(sum / AD_QUEUE_LEN);
}

