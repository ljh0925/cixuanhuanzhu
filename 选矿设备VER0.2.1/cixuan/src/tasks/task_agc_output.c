/******************************************************************************
Copyright 2005 �������ķ�������޹�˾
All rights riserved.

�ļ���	��task_agc_output.c
ģ�����ƣ�AGC����������
���ܸ�Ҫ��AGC����������

ȡ���汾��0.0.1
�޸���	������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"config.h"


extern volatile DEV_STAT DevStat;


/******************************************************************************
 �������ƣ�Task_Agc_Output
 ����������AGC�������
 ����������
 �������ƣ� ����/����� ����        ����

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2005-04-11
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void Task_Agc_Output(void * data)
{
	INT16U rega = 0;
	INT16U regb = 0;

	ToggleWD();

	DevStat.MOD_REG.reg[0x0015] = 4000;
	DevStat.MOD_REG.reg[0x0016] = 4000;

	data = data;

	for(;;)
	{
		ToggleWD();

		OSTimeDlyHMSM(0, 0, 0, 30);            //30msһ��

		OS_ENTER_CRITICAL();
		rega = DevStat.MOD_REG.reg[0x0015];      //��ʹ��AGC�Ĵ���
		regb = DevStat.MOD_REG.reg[0x0016];      //��ʹ��AGC�Ĵ���
		OS_EXIT_CRITICAL();

		Ltc1448Output(rega,regb);					//AGC�����ֹ�����0V

	}
}

