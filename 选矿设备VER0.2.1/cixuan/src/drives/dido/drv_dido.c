/******************************************************************************
Copyright 2005 �������ķ�������޹�˾
All rights riserved.

�ļ���	��drv_dido.c
ģ�����ƣ�DIDO��������
���ܸ�Ҫ��DIDO��������

ȡ���汾��0.0.1
�޸���	������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include "config.h"


extern volatile DEV_STAT DevStat;



/******************************************************************************
 �������ƣ�DidoInit
 ���������� DIDO������ʼ��
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void DidoInit(void)
{
	ToggleWD();

	//Ҫ��PINSEL2����P2��ΪIO��
	//DI0-7
//  IO2DIR &= (~BIT16);		//P2.16����Ϊ����
//  IO2DIR &= (~BIT17);		//P2.17����Ϊ����
//  IO2DIR &= (~BIT18);		//P2.18����Ϊ����
//  IO2DIR &= (~BIT19);		//P2.19����Ϊ����
//  IO2DIR &= (~BIT20);		//P2.20����Ϊ����
//  IO2DIR &= (~BIT21);		//P2.21����Ϊ����
//  IO2DIR &= (~BIT22);		//P2.22����Ϊ����
//  IO2DIR &= (~BIT23);		//P2.23����Ϊ����

	//DO0-7
	IO2DIR |= BIT30;		//P2.30����Ϊ���
	IO2DIR |= BIT31;		//P2.31����Ϊ���
//  IO2DIR |= BIT26;        //P2.26����Ϊ���
//  IO2DIR |= BIT27;        //P2.27����Ϊ���
//  IO2DIR |= BIT28;		//P2.28����Ϊ���
//  IO2DIR |= BIT29;		//P2.29����Ϊ���
//  IO2DIR |= BIT30;		//P2.30����Ϊ���
//  IO2DIR |= BIT31;		//P2.31����Ϊ���
//
//  IO2DIR |= BIT0;			//P2.0����Ϊ���
//  IO2DIR |= BIT1;			//P2.1����Ϊ���


//	PINSEL2 = PINSEL2 & (~BIT3);        //P1.16~25Ϊͨ��IO
	IO1DIR |= BIT17;					//P1.17����Ϊ���
	
//	PINSEL2 = PINSEL2 & (~BIT4);        //P3.28 ���ó�GPIO
//	PINSEL2 = PINSEL2 & (~BIT5);		//P3.29 ���ó�GPIO	
	IO3DIR |= BIT28;					//P3.28����Ϊ���
	IO3DIR |= BIT29;					//P3.29����Ϊ���   			

//  PINSEL1 = PINSEL1 & (~BIT10);
//  PINSEL1 = PINSEL1 & (~BIT11);		//P0.21 ���ó�GPIO		DI_CTRL0
//  PINSEL1 = PINSEL1 & (~BIT12);
//  PINSEL1 = PINSEL1 & (~BIT13);		//P0.22 ���ó�GPIO		DI_CTRL1
//
	PINSEL1 = PINSEL1 & (~BIT24);
	PINSEL1 = PINSEL1 & (~BIT25);		//P0.28 ���ó�GPIO		DO_CLK
	PINSEL1 = PINSEL1 & (~BIT26);
	PINSEL1 = PINSEL1 & (~BIT27);		//P0.29 ���ó�GPIO		DO_CTRL0
	PINSEL1 = PINSEL1 & (~BIT28);
	PINSEL1 = PINSEL1 & (~BIT29);		//P0.30 ���ó�GPIO		DO_CTRL1

//  IO0DIR |= BIT21;		//P0.21����Ϊ���
//  IO0DIR |= BIT22;		//P0.22����Ϊ���
	IO0DIR |= BIT28;		//P0.28����Ϊ���
	IO0DIR |= BIT29;		//P0.29����Ϊ���
	IO0DIR |= BIT30;		//P0.30����Ϊ���

//  DI_CTRL0_SET_HIGH();
//  DI_CTRL1_SET_HIGH();
//  DO_CLK_SET_HIGH();
//  DO_CTRL0_SET_HIGH();
//  DO_CTRL1_SET_HIGH();

	SetDO(0x00, 0);

}

/******************************************************************************
 �������ƣ�GetDIStatus
 ����������ȡ��DI״̬
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ��DI״̬�����8λδʹ�ã��Ӹߵ������ζ�ӦDI23-DI0
				   
 �� 	 �� ������
 �� 	 �ڣ�2004-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
INT32U GetDIStatus(void)
{
	INT32U di1 = 0;
	INT32U di2 = 0;
	INT32U di3 = 0;
	INT32U temp_int32u = 0;

	ToggleWD();

	OS_ENTER_CRITICAL();

//	DI_CTRL0_SET_LOW();
//	DI_CTRL1_SET_LOW();
//  SleepMs(1);
	Sleep10us(10);
	temp_int32u = IO2PIN;
	di1 = (temp_int32u >> 16) & 0x000000FF;

//	DI_CTRL0_SET_HIGH();
//	DI_CTRL1_SET_LOW();
//  SleepMs(1);
	Sleep10us(10);
	temp_int32u = IO2PIN;
	di2 = (temp_int32u >> 16) & 0x000000FF;

//	DI_CTRL0_SET_LOW();
//	DI_CTRL1_SET_HIGH();
//  SleepMs(1);
	Sleep10us(10);
	temp_int32u = IO2PIN;
	di3 = (temp_int32u >> 16) & 0x000000FF;

//	DI_CTRL0_SET_HIGH();
//	DI_CTRL1_SET_HIGH();

	OS_EXIT_CRITICAL();

	temp_int32u = (di3 << 16) | (di2 << 8) | di1;

	return temp_int32u;
}

/******************************************************************************
 �������ƣ�SetDO
 ����������������ֵ����DO
 ����������
 �������ƣ� ����/����� ����		����
 do_set	����		INT16U			Ҫ���õ�DO�������ɸ�λ����λ���ζ�ӦDO15-DO0
				
 ��  ��  ֵ����
				   
 �� 	 �� ������
 �� 	 �ڣ�2004-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void SetDO(INT16U do_set, INT8U offset )
{
//	INT8U i = 0;
	INT8U temp_Sec;
	INT16U temp_Mil;

	ToggleWD();

	if(do_set == 0x00)
	{
		IO2SET = BIT30;
		IO2SET = BIT31;
		IO1SET = BIT17;
		IO0SET = BIT28;
		IO3SET = BIT29;
		IO3SET = BIT28;
		IO0SET = BIT29;
		IO0SET = BIT30;
		return;
	}
	else if(do_set == 0xffff)
	{
		switch ( offset )
		{
		   case 0:
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[0]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[0]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
				IO2CLR = (BIT30);
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   //SleepMs(2000);
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[0]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[0]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
				IO2SET = (BIT30);
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   break;
		   case 1:
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[1]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[1]%1000;
			   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
			   IO2CLR = (BIT31);
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[1]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[1]%1000;
			   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
			   IO2SET = (BIT31);
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   break;
		   case 2:
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[2]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[2]%1000;
			   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
			   IO1CLR = BIT17;
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil);
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[2]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[2]%1000;
			   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
			   IO1SET = BIT17;
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   break;
		   case 3:
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[3]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[3]%1000;
			   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
			   IO0CLR = BIT28;
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[3]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[3]%1000;
			   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
			   IO0SET = BIT28;
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   break;
//  	   case 4:
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[4]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[4]%1000;
//  		   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO3CLR = BIT29;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[4]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[4]%1000;
//  		   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO3SET = BIT29;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   break;
//  	   case 5:
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[5]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[5]%1000;
//  		   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO3CLR = BIT28;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[5]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[5]%1000;
//  		   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO3SET = BIT28;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   break;
//  	   case 6:
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[6]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[6]%1000;
//  		   	if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO0CLR = BIT29;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[6]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[6]%1000;
//  		   	if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO0SET = BIT29;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   break;
//  	   case 7:
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[7]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[7]%1000;
//  		   if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO0CLR = BIT30;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[7]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[7]%1000;
//  		   	if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO0SET = BIT30;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   break;
		   default:
			   break;
		}	
		return;
	}

}

/******************************************************************************
 �������ƣ�SetRelayModbus
 ������������DevStat.mod_coil���ü̵���
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ������
 �� 	 �ڣ�2004-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
 void SetRelayModbus(INT8U off_set)
{
//	INT8U i = 0;
	INT16U value = 0xffff;

	ToggleWD();

//  OS_ENTER_CRITICAL();
//  for(i=0; i<16; i++)
//  {
//  	if (DevStat.MOD_REG.REG_DETAIL.mod_coil[i] == 1)
//  		value |= (1 << i);
//  }
//  OS_EXIT_CRITICAL();

	SetDO(value, off_set);
}

